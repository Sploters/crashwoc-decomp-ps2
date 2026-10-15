"""Contribution workflow: explicit states from first draft to contribution.

The status manifests keep their three committed states (``asm``,
``equivalent``, ``matching``) and ``tools/promote.py`` stays their only
writer. This module adds a separate, local ledger
(``build/<version>/agent_workflow/``, gitignored) that tracks the *process*
around a function, because a byte-exact function is not automatically a good
contribution:

``in_progress``
    Being written. ``submit`` runs ``compile_diff``; ``exact = true`` there is
    the only way into the promotion queue.
``exact_match``
    The official verifier (``tools/promote.py`` -> ``tools/verify_promoted.py``)
    proved it byte-identical over its full extent, with the whole image.
``quality_review``
    The readability lint has run and a review packet waits for a reviewer who
    did not write the code.
``ready_for_contribution``
    Verified, the source unchanged since the verification, a complete
    grounding record approved by an independent reviewer, every lint finding
    justified and nothing blocking.
``needs_human_review``
    Doubt the automation cannot settle: a blocking hypothesis, a construct
    only a maintainer may accept (inline assembly), a failed checklist item,
    an author who cannot be shown to differ from the reviewer, or a reviewer
    who asked for a human.

Any edit to a function's C after its verification sends it back to
``in_progress`` (the byte proof was for other text), and so does a manifest
that no longer says ``matching``. ``gate`` is the check to run before opening
a PR: every function the branch newly marks ``matching`` must be
``ready_for_contribution``.

The toolchain-bound steps (``compile_diff``, the promotion) are injected as
callables so the state logic is testable without a compiler; the defaults
call the real tools.
"""
from __future__ import annotations

import json
import os
import subprocess
import sys
import tempfile
import time
import tomllib
from pathlib import Path

from . import schemas, quality
from .project import DecompProject
from .registry import Registry

STATES = ("in_progress", "exact_match", "quality_review",
          "ready_for_contribution", "needs_human_review")

# Every allowed edge; anything else is refused.
TRANSITIONS = {
    None: {"in_progress"},
    "in_progress": {"in_progress", "exact_match"},
    "exact_match": {"quality_review", "in_progress"},
    "quality_review": {"ready_for_contribution", "needs_human_review",
                       "in_progress"},
    "needs_human_review": {"ready_for_contribution", "in_progress"},
    "ready_for_contribution": {"in_progress"},
}

# States that rest on a byte proof of one exact source text.
VERIFIED_STATES = ("exact_match", "quality_review", "ready_for_contribution",
                   "needs_human_review")

SCHEMA = 1


class WorkflowError(Exception):
    pass


# --- storage ----------------------------------------------------------------


def _dir(project) -> Path:
    d = project.root / "build" / project.version / "agent_workflow"
    d.mkdir(parents=True, exist_ok=True)
    return d


def _slug(fid: str) -> str:
    return fid.replace(":", "_")


def _path(project, fid) -> Path:
    return _dir(project) / f"{_slug(fid)}.json"


def _atomic_write(path: Path, data: dict) -> None:
    fd, tmp = tempfile.mkstemp(dir=str(path.parent), suffix=".tmp")
    try:
        with os.fdopen(fd, "w", encoding="utf-8") as f:
            json.dump(data, f, indent=2)
            f.write("\n")
        os.replace(tmp, path)
    finally:
        if os.path.exists(tmp):
            os.remove(tmp)


def load(project, fid):
    path = _path(project, fid)
    if not path.is_file():
        return None
    return json.loads(path.read_text())


def save(project, record) -> None:
    _atomic_write(_path(project, record["function_id"]), record)


def all_records(project):
    for path in sorted(_dir(project).glob("*.json")):
        try:
            yield json.loads(path.read_text())
        except (OSError, json.JSONDecodeError):
            continue


def _now():
    return time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime())


def transition(record, to, reason, by=None):
    """Move `record` to state `to`, refusing any edge not in TRANSITIONS."""
    frm = record.get("state")
    if to not in STATES:
        raise WorkflowError(f"unknown state {to!r}")
    if to not in TRANSITIONS.get(frm, set()):
        raise WorkflowError(f"{record['function_id']}: {frm} -> {to} is not "
                            "an allowed transition")
    record["state"] = to
    record.setdefault("history", []).append(
        {"at": _now(), "from": frm, "to": to, "reason": reason, "by": by})
    return record


def _new_record(fid, author):
    return {"schema": SCHEMA, "function_id": fid, "state": None,
            "author": {"id": author}, "history": []}


# --- live facts (source, manifest, verifier results) -------------------------


def _resolve(project, target):
    reg = Registry(project)
    res = reg.resolve(target)
    if res.status != "resolved" or res.record is None:
        raise WorkflowError(res.detail or f"cannot resolve {target!r} to one "
                            "function (use the canonical id)")
    return reg, res.record


def function_source(project, reg, fn):
    """(text, first_line, source_path) of the function's C, or None."""
    unit = reg.units.get(fn.unit_index)
    if unit is None or not unit.source_path:
        return None
    path = project.root / unit.source_path
    if not path.is_file():
        return None
    found = quality.extract_function(path.read_text(errors="replace"), fn.name)
    if found is None:
        return None
    return found[0], found[1], unit.source_path


def current_hash(project, reg, fn):
    src = function_source(project, reg, fn)
    return quality.source_hash(src[0]) if src else None


def manifest_states(project):
    """{function id: committed state} from the status manifests on disk."""
    out = {}
    for path in sorted(project.status_dir.rglob("*.toml")):
        for entry in tomllib.loads(path.read_text()).get("function", []):
            out[entry["id"]] = entry["state"]
    return out


def _newest_input(project):
    """mtime (ns) of the newest file a verifier run depends on: C sources,
    headers, and the version's registries/manifests/profiles."""
    newest = 0
    for base, pattern in (("src", "*.c"), ("include", "*.h"),
                          (f"config/{project.version}", "*")):
        root = project.root / base
        if not root.is_dir():
            continue
        for path in root.rglob(pattern):
            if path.is_file():
                newest = max(newest, path.stat().st_mtime_ns)
    return newest


def read_verify_results(project):
    """The last verifier run's results, marked `_stale` when any source,
    header or config file changed after it (a pull, a rebase, an edit): the
    run then proves nothing about the code as it is now."""
    path = project.root / "build" / project.version / "verify_results.json"
    try:
        data = json.loads(path.read_text())
        written = path.stat().st_mtime_ns
    except (OSError, json.JSONDecodeError):
        return {}
    if _newest_input(project) > written:
        data["_stale"] = True
    return data


def verified_by_results(results, fid) -> bool:
    """True when the latest verifier run is current, all green, and
    byte-proved fid."""
    if results.get("_stale") or not results.get("all_green"):
        return False
    return any(r.get("id") == fid and r.get("verified") and "size" in r
               for r in results.get("functions", []))


def refresh(project, record, reg=None, fn=None, states=None):
    """Send a record back to in_progress when its proof no longer holds:
    the C changed since verification, or the manifest is not `matching`.
    Returns the reason, or None when nothing changed."""
    if record.get("state") not in VERIFIED_STATES:
        return None
    if reg is None or fn is None:
        reg, fn = _resolve(project, record["function_id"])
    states = manifest_states(project) if states is None else states
    reason = None
    if states.get(record["function_id"]) != "matching":
        reason = "the status manifest no longer marks it `matching`"
    elif current_hash(project, reg, fn) != \
            (record.get("verified") or {}).get("source_sha256"):
        reason = "its C changed after the byte verification"
    if reason:
        transition(record, "in_progress", reason, by="workflow")
        record.pop("candidate", None)
        save(project, record)
    return reason


# --- default toolchain hooks ------------------------------------------------


def _default_compile_diff(project, fid):
    from . import diff
    return diff.compile_diff(project, fid)


def _run_logged(project, cmd):
    """Run a gate script with its output kept in the ledger directory (not
    on stdout, which carries the JSON result)."""
    proc = subprocess.run(cmd, cwd=str(project.root), capture_output=True,
                          text=True)
    (_dir(project) / "last_verifier.log").write_text(
        proc.stdout + ("\n--- stderr ---\n" + proc.stderr if proc.stderr
                       else ""))
    return proc


def _default_promote(project, fids, report_path):
    """tools/promote.py --isolate over `fids`; returns the parsed report."""
    proc = _run_logged(project, [
        sys.executable, "tools/promote.py", "--isolate", "--report",
        str(report_path), "--version", project.version, *fids])
    try:
        report = json.loads(Path(report_path).read_text())
    except (OSError, json.JSONDecodeError):
        report = {"accepted": [], "rejected": {f: "promote.py produced no "
                                                  "report" for f in fids},
                  "error": f"promote.py exited {proc.returncode}"}
    return report


def _default_verify(project):
    """Run the full verifier once (used when nothing needs flipping)."""
    return _run_logged(project, [
        sys.executable, "tools/verify_promoted.py", "--version",
        project.version]).returncode == 0


# --- operations -------------------------------------------------------------


def submit(project: DecompProject, target, *, author, notes=None,
           compile_diff=None) -> dict:
    """Record a draft and, when compile_diff is exact, queue it for promotion.

    A function that is readable but not byte-exact stays `in_progress` with
    no place in the queue: nothing downstream can reach it.
    """
    if not str(author or "").strip():
        return schemas.err("workflow_submit", "an --author id is required (it "
                           "is what the independent review is checked against)",
                           code="author_required")
    try:
        reg, fn = _resolve(project, target)
    except WorkflowError as exc:
        return schemas.err("workflow_submit", str(exc), code="unresolved")
    fid = fn.id_str()
    record = load(project, fid) or _new_record(fid, author)
    refresh(project, record, reg, fn)
    if record["state"] not in (None, "in_progress"):
        return schemas.err(
            "workflow_submit", f"{fid} is `{record['state']}`; edit its C "
            "first (that returns it to in_progress) or finish its review",
            code="wrong_state", state=record["state"])
    if record["state"] is None:
        transition(record, "in_progress", "draft submitted", by=author)
    record["author"] = {"id": author}
    if notes:
        record["author_notes"] = notes

    src = function_source(project, reg, fn)
    if src is None:
        save(project, record)
        return schemas.err("workflow_submit", f"{fn.name} has no C definition "
                           "in its unit yet", code="missing_c")
    digest = quality.source_hash(src[0])
    result = (compile_diff or _default_compile_diff)(project, fid)
    exact = bool(result.get("ok") and result.get("exact"))
    record["last_compile_diff"] = {
        "at": _now(), "source_sha256": digest, "exact": exact,
        "differing_bytes": result.get("differing_bytes"),
        "diff_signature": result.get("diff_signature"),
        "error": (result.get("error") or {}).get("message")}
    if exact:
        record["candidate"] = {"at": _now(), "source_sha256": digest,
                               "candidate_asm_hash":
                                   result.get("candidate_asm_hash")}
    else:
        record.pop("candidate", None)
    save(project, record)
    return schemas.ok("workflow_submit", function_id=fid,
                      state=record["state"], exact=exact, queued=exact,
                      differing_bytes=result.get("differing_bytes"))


def queue(project):
    return [r for r in all_records(project)
            if r.get("state") == "in_progress" and r.get("candidate")]


def batch(project: DecompProject, *, limit=None, compile_diff=None,
          promote=None, verify=None, results=None) -> dict:
    """Promote the queued exact candidates with one isolated verifier batch.

    1. Re-check every queued candidate with compile_diff (the source may have
       changed since it was queued); only `exact = true` goes on.
    2. Hand the ones still `asm` to ``promote.py --isolate``: one verifier
       run when the batch holds, a bisection keeping every valid promotion
       when it does not. Candidates already `matching` (re-verification after
       an edit) are proved by a single verifier run instead.
    3. Each function the verifier proved (latest run all green, its bytes
       verified) becomes `exact_match` bound to the exact source it proved,
       and its quality review opens. The rest stay `in_progress` with the
       reason.
    """
    compile_diff = compile_diff or _default_compile_diff
    promote = promote or _default_promote
    verify = verify or _default_verify
    results = results or read_verify_results

    pending = queue(project)
    if limit:
        pending = pending[:limit]
    if not pending:
        return schemas.ok("workflow_batch", queued=0, promoted=[],
                          not_promoted={}, verifier_runs=0)

    states = manifest_states(project)
    reg = Registry(project)
    exact, dropped = {}, {}
    for record in pending:
        fid = record["function_id"]
        fn = reg.resolve(fid).record
        if fn is None:
            dropped[fid] = "no longer in the function registry"
            continue
        result = compile_diff(project, fid)
        digest = current_hash(project, reg, fn)
        if result.get("ok") and result.get("exact") and digest:
            exact[fid] = digest
        else:
            dropped[fid] = ("compile_diff is no longer exact"
                            if result.get("ok") else
                            "compile_diff failed: " +
                            str((result.get("error") or {}).get("message")))

    to_flip = [f for f in exact if states.get(f) != "matching"]
    already = [f for f in exact if states.get(f) == "matching"]
    runs, report = 0, {"accepted": [], "rejected": {}, "error": None}
    if to_flip:
        report_path = _dir(project) / "last_promote_report.json"
        report = promote(project, to_flip, report_path)
        runs = report.get("verifier_runs") or 0
    elif already:
        verify(project)
        runs = 1
    latest = results(project)

    promoted, not_promoted = [], dict(dropped)
    for fid, why in (report.get("rejected") or {}).items():
        not_promoted[fid] = why
    if report.get("error"):
        for fid in to_flip:
            not_promoted.setdefault(fid, report["error"])
    for fid in exact:
        if fid in not_promoted:
            continue
        if fid in to_flip and fid not in (report.get("accepted") or []):
            not_promoted[fid] = "not accepted by promote.py"
            continue
        if not verified_by_results(latest, fid):
            not_promoted[fid] = ("the latest verifier run did not prove it "
                                 "(not all green, or its bytes not verified)")
            continue
        promoted.append(fid)

    for record in pending:
        fid = record["function_id"]
        if fid in promoted:
            record["verified"] = {
                "at": _now(), "source_sha256": exact[fid],
                "by": "tools/promote.py --isolate" if fid in to_flip
                      else "tools/verify_promoted.py",
                "profiles_fingerprint": latest.get("profiles_fingerprint")}
            record.pop("candidate", None)
            transition(record, "exact_match", "byte-identical per the "
                       "official verifier", by="workflow")
            save(project, record)
            open_review(project, fid, record=record)
        else:
            record.pop("candidate", None)
            record["last_batch"] = {"at": _now(), "reason": not_promoted[fid]}
            save(project, record)

    return schemas.ok("workflow_batch", queued=len(pending),
                      promoted=promoted, not_promoted=not_promoted,
                      verifier_runs=runs, error=report.get("error"),
                      verifier_log=str(_dir(project) / "last_verifier.log")
                      if runs else None)


def open_review(project, target, *, record=None) -> dict:
    """exact_match -> quality_review: run the lint, write the review packet."""
    reg, fn = _resolve(project, target)
    fid = fn.id_str()
    record = record or load(project, fid)
    if record is None or record.get("state") != "exact_match":
        return schemas.err("workflow_open_review", f"{fid} is not exact_match",
                           code="wrong_state")
    src = function_source(project, reg, fn)
    findings = quality.lint_function(src[0], src[1])
    record["lint"] = {"source_sha256": quality.source_hash(src[0]),
                      "findings": findings}
    transition(record, "quality_review", "lint run; review packet written",
               by="workflow")
    save(project, record)
    packet = write_packet(project, reg, fn, record, src)
    return schemas.ok("workflow_open_review", function_id=fid,
                      findings=findings, packet=str(packet))


def review_template(record) -> dict:
    """The grounding record a reviewer fills in (keys are checked)."""
    lint = (record.get("lint") or {}).get("findings", [])
    return {
        "function_id": record["function_id"],
        "source_sha256": (record.get("verified") or {}).get("source_sha256"),
        "reviewer": {"id": "", "kind": "agent or human"},
        "verdict": "approve | reject | needs_human_review",
        "summary": "",
        "evidence": [{"claim": "", "source": ""}],
        "confirmed": {k: [] for k in quality.CONFIRMED_KINDS},
        "hypotheses": [],
        "rationale": "",
        "simpler_alternatives": "",
        "checklist": {k: None for k in quality.CHECKLIST},
        "lint_justifications": {f["key"]: "" for f in lint},
    }


def write_packet(project, reg, fn, record, src):
    """Everything an independent reviewer needs, in one file.

    Lives under build/ (gitignored): it embeds the retail disassembly, which
    is game-derived and must never be committed.
    """
    from . import context
    disasm = context.get_disassembly(project, fn.id_str())
    lint = record.get("lint", {}).get("findings", [])
    lines = [
        f"# Quality review: {fn.name}", "",
        f"- function id: `{fn.id_str()}`",
        f"- source: `{src[2]}` line {src[1]}",
        f"- size: {fn.size} bytes",
        f"- verified: {json.dumps(record.get('verified'))}",
        f"- author: `{record.get('author', {}).get('id')}` (you must not be "
        "the author)", "",
        "Byte-exactness is already proved; your job is to judge whether this "
        "is honest, readable C and whether its names, types, offsets and "
        "constants are backed by evidence. Do not take the author's notes on "
        "trust; check them against the disassembly and the headers.", "",
        "## C under review", "", "```c", src[0], "```", "",
        "## Readability lint", "",
    ]
    if lint:
        for f in lint:
            lines.append(f"- `{f['key']}` ({f['severity']}) line {f['line']}: "
                         f"{f['message']} -- `{f['excerpt']}`")
    else:
        lines.append("No findings.")
    lines += ["", "## Author notes (unverified claims)", "",
              record.get("author_notes") or "(none)", "",
              "## Retail disassembly (ground truth)", "", "```",
              disasm.get("disassembly") or f"(unavailable: "
                                           f"{disasm.get('reason')})", "```",
              "", "## Grounding record to return", "",
              "Fill every field; leave nothing invented. Anything not backed "
              "by evidence goes in `hypotheses` (set `blocking` when a "
              "maintainer must decide).", "", "```json",
              json.dumps(review_template(record), indent=2), "```", ""]
    out = _dir(project) / "review" / f"{_slug(fn.id_str())}.md"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text("\n".join(lines))
    return out


def submit_review(project: DecompProject, target, review, *,
                  results=None) -> dict:
    """Apply an independent reviewer's grounding record.

    Only a complete record, approved, by someone other than the author, for
    the exact source the verifier proved -- with every lint finding justified
    and nothing blocking -- makes the function ready_for_contribution.
    """
    results = results or read_verify_results
    try:
        reg, fn = _resolve(project, target)
    except WorkflowError as exc:
        return schemas.err("workflow_review", str(exc), code="unresolved")
    fid = fn.id_str()
    record = load(project, fid)
    if record is None:
        return schemas.err("workflow_review", f"{fid} has no workflow record",
                           code="wrong_state")
    stale = refresh(project, record, reg, fn)
    if stale:
        return schemas.err("workflow_review", f"{fid} went back to "
                           f"in_progress: {stale}", code="stale")
    if record.get("state") != "quality_review":
        return schemas.err("workflow_review", f"{fid} is `{record.get('state')}`"
                           ", not quality_review", code="wrong_state")

    findings = record["lint"]["findings"]
    expected = record["verified"]["source_sha256"]
    verdict = review.get("verdict") if isinstance(review, dict) else None
    reviewer = str(((review or {}).get("reviewer") or {}).get("id", "")).strip()
    author = str(record.get("author", {}).get("id", "")).strip()

    record.setdefault("reviews", []).append({"at": _now(), "review": review})
    if verdict in ("reject", "needs_human_review"):
        if not reviewer or not quality._text(review.get("rationale")):
            save(project, record)
            return schemas.err("workflow_review", "a reject / "
                               "needs_human_review verdict needs reviewer.id "
                               "and a rationale", code="incomplete")
        to = "in_progress" if verdict == "reject" else "needs_human_review"
        transition(record, to, f"reviewer {reviewer}: {review['rationale']}",
                   by=reviewer)
        if to == "in_progress":
            record.pop("candidate", None)
        save(project, record)
        return schemas.ok("workflow_review", function_id=fid, state=to)

    problems = quality.validate_review(review, findings=findings,
                                       expected_hash=expected)
    if problems:
        save(project, record)
        return schemas.err("workflow_review", "the grounding record is "
                           "incomplete", code="incomplete", problems=problems,
                           state=record["state"])

    reasons = quality.blocking_reasons(review, findings)
    if not author:
        reasons.append("the author is unknown, so independence of the review "
                       "cannot be checked")
    elif reviewer == author:
        save(project, record)
        return schemas.err("workflow_review", "the reviewer is the author; an "
                           "independent reviewer is required",
                           code="not_independent", state=record["state"])
    if not verified_by_results(results(project), fid):
        reasons.append("the latest verifier run does not prove it (re-run "
                       "the verifier)")
    if reasons:
        transition(record, "needs_human_review", "; ".join(reasons),
                   by=reviewer)
        save(project, record)
        return schemas.ok("workflow_review", function_id=fid,
                          state="needs_human_review", reasons=reasons)

    record["approved_review"] = review
    transition(record, "ready_for_contribution", f"approved by {reviewer}",
               by=reviewer)
    save(project, record)
    return schemas.ok("workflow_review", function_id=fid,
                      state="ready_for_contribution")


def human_decision(project: DecompProject, target, *, decision, by, note,
                   results=None) -> dict:
    """A maintainer settles a needs_human_review function.

    `approve` still requires the integrity checks (same source as verified,
    still `matching`, latest verifier run proves it, a complete review on
    file with every lint finding justified); it only settles the judgement
    calls the automation escalated. `rework` sends it back to in_progress.
    """
    results = results or read_verify_results
    if decision not in ("approve", "rework"):
        return schemas.err("workflow_human", "decision is approve or rework",
                           code="bad_decision")
    if not str(by or "").strip() or not quality._text(note):
        return schemas.err("workflow_human", "--by and a --note of at least "
                           f"{quality.MIN_TEXT} characters are required",
                           code="incomplete")
    try:
        reg, fn = _resolve(project, target)
    except WorkflowError as exc:
        return schemas.err("workflow_human", str(exc), code="unresolved")
    fid = fn.id_str()
    record = load(project, fid)
    if record is None:
        return schemas.err("workflow_human", f"{fid} has no workflow record",
                           code="wrong_state")
    stale = refresh(project, record, reg, fn)
    if stale or record.get("state") != "needs_human_review":
        return schemas.err("workflow_human", f"{fid} is "
                           f"`{record.get('state')}`" +
                           (f" ({stale})" if stale else "") +
                           ", not needs_human_review", code="wrong_state")
    if decision == "rework":
        transition(record, "in_progress", f"human {by}: {note}", by=by)
        save(project, record)
        return schemas.ok("workflow_human", function_id=fid,
                          state="in_progress")

    findings = record["lint"]["findings"]
    review = next((r["review"] for r in reversed(record.get("reviews", []))
                   if isinstance(r.get("review"), dict)), None)
    problems = []
    if review is None:
        problems.append("no review on file")
    else:
        problems += quality.validate_review(
            review, findings=findings,
            expected_hash=record["verified"]["source_sha256"])
    if not verified_by_results(results(project), fid):
        problems.append("the latest verifier run does not prove it")
    if problems:
        return schemas.err("workflow_human", "cannot approve", code="blocked",
                           problems=problems)
    record["human_approval"] = {"at": _now(), "by": by, "note": note}
    transition(record, "ready_for_contribution", f"human {by}: {note}", by=by)
    save(project, record)
    return schemas.ok("workflow_human", function_id=fid,
                      state="ready_for_contribution")


def status(project: DecompProject, *, state=None) -> dict:
    """Every tracked function with its state (stale proofs refreshed first)."""
    states = manifest_states(project)
    reg = Registry(project)
    rows = []
    for record in all_records(project):
        res = reg.resolve(record["function_id"])
        if res.status == "resolved":
            refresh(project, record, reg, res.record, states)
        if state and record.get("state") != state:
            continue
        rows.append({"function_id": record["function_id"],
                     "state": record.get("state"),
                     "queued": bool(record.get("candidate")),
                     "last": (record.get("history") or [{}])[-1].get("reason")})
    counts = {}
    for r in rows:
        counts[r["state"]] = counts.get(r["state"], 0) + 1
    return schemas.ok("workflow_status", counts=counts, functions=rows)


def _base_states(project, base):
    """{function id: state} in the manifests at git revision `base`."""
    rel = project.status_dir.relative_to(project.root).as_posix()
    listing = subprocess.run(
        ["git", "ls-tree", "-r", "--name-only", base, rel],
        cwd=str(project.root), capture_output=True, text=True)
    if listing.returncode != 0:
        raise WorkflowError(f"cannot read manifests at {base!r}: "
                            f"{listing.stderr.strip()}")
    out = {}
    for path in listing.stdout.split():
        if not path.endswith(".toml"):
            continue
        text = subprocess.run(["git", "show", f"{base}:{path}"],
                              cwd=str(project.root), capture_output=True,
                              text=True).stdout
        for entry in tomllib.loads(text).get("function", []):
            out[entry["id"]] = entry["state"]
    return out


def gate(project: DecompProject, *, base, base_states=None,
         results=None) -> dict:
    """Pre-PR check: every function this branch newly marks `matching` must
    be ready_for_contribution, for its current source, and still proved by
    the latest verifier run."""
    results = results or read_verify_results
    try:
        before = base_states if base_states is not None else \
            _base_states(project, base)
    except WorkflowError as exc:
        return schemas.err("workflow_gate", str(exc), code="base")
    now = manifest_states(project)
    new = sorted(f for f, s in now.items()
                 if s == "matching" and before.get(f) != "matching")
    latest = results(project)
    reg = Registry(project)
    failing = {}
    for fid in new:
        record = load(project, fid)
        if record is None:
            failing[fid] = "never went through the workflow (no review)"
            continue
        res = reg.resolve(fid)
        if res.status == "resolved":
            refresh(project, record, reg, res.record, now)
        if record.get("state") != "ready_for_contribution":
            failing[fid] = f"state is {record.get('state')}"
        elif not verified_by_results(latest, fid):
            failing[fid] = ("the latest verifier run does not prove it"
                            + (" (it predates the current sources; re-run "
                               "the verifier)" if latest.get("_stale")
                               else ""))
    return schemas.ok("workflow_gate", base=base, newly_matching=len(new),
                      ready=len(new) - len(failing), failing=failing,
                      passed=not failing)
