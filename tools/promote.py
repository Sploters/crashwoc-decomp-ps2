#!/usr/bin/env python3
"""Promote a function to `matching` -- the only writer of that state.

    python tools/promote.py NuListGetHead
    python tools/promote.py pal103:unit-0007:00105a50:NuListGetHead
    python tools/promote.py A B C            # one verifier run for all three
    python tools/promote.py --targets-file ready.txt
    python tools/promote.py --init src/nucore/nulist.c

A hand-edited `state = "matching"` is never trusted (and the container CI
re-verifies every claim); this tool makes the honest path the easy path:

  1. Find each function's manifest entry (by full id, or by name when the
     name is unique across manifests) and flip its state in the file. All
     targets are resolved before anything is written, so a typo cannot
     leave a batch half-flipped.
  2. Run the full promotion verifier (tools/verify_promoted.py): manifest
     consistency, each function's bytes against retail over its whole
     registry extent, every previously promoted function again, and the
     matching-image SHA gate.
  3. If ANYTHING fails, every manifest edit is rolled back exactly and the
     failure is reported -- the repo never holds an unverified promotion.

Because step 2 re-derives *every* `matching` claim in the repo, promoting N
functions together is exactly as strong a proof as promoting them one by one
-- and costs one verifier run instead of N (that run is several minutes and
grows with the matching set). The only difference is granularity of failure:
by default a batch rolls back whole. With `--isolate`, a failed batch is
narrowed down instead: functions the verifier proved byte-divergent are
dropped directly, the rest are re-verified in halves, and every function that
held is kept -- the final manifests are always re-proved by one last full
verifier run, so `--isolate` never keeps a promotion the verifier did not
pass. `--report FILE` writes the per-function outcome as JSON.

`--init` creates the status manifest for a source file that doesn't have
one yet (every function `asm`), deriving the unit from the canonical
source-path mapping (unit `nucore/nulist` <-> src/nucore/nulist.c). Edit
nothing by hand afterwards except `equivalent` states, which are a
reviewed human judgement.

Needs the toolchain for the verify step: run via
`python tools/dispatch.py python tools/promote.py ...` from the host.
"""
import argparse
import json
import re
import subprocess
import sys
import tomllib
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from gen_objdiff import unit_table
from declib.tu import parse_toml_blocks
from status import load_profile_names


def default_profile_for(category):
    """Safe default compiler profile for a translation-unit category.

    Game and engine code only matches under the SN ProDG compiler (profile
    `default`); the SCE / newlib / libgcc / runtime half -- all classified
    `sdk` by gen_objdiff.classify -- needs the Sony compiler (profile `sce`).
    An explicit --profile override still wins over this default.
    """
    return "sce" if category == "sdk" else "default"


def resolve_profile(profile, category, known):
    """The profile to write into a new manifest: an explicit override when
    given, otherwise the category default. Either way it must be a profile
    that exists in profiles.toml, so a typo fails loudly at --init time
    instead of only later in status validation."""
    chosen = profile if profile is not None else default_profile_for(category)
    if chosen not in known:
        raise SystemExit(
            f"promote --init: unknown profile {chosen!r} "
            f"(profiles.toml has: {', '.join(sorted(known))})")
    return chosen


def find_entry(version, needle):
    """(manifest_path, full_id) for a function id or unique name."""
    status_dir = ROOT / "config" / version / "status"
    hits = []
    for manifest in sorted(status_dir.rglob("*.toml")) \
            if status_dir.is_dir() else []:
        for entry in tomllib.loads(manifest.read_text()).get("function", []):
            fid = entry["id"]
            if fid == needle or fid.rsplit(":", 1)[1] == needle:
                hits.append((manifest, fid, entry["state"]))
    if not hits:
        raise SystemExit(
            f"promote: no manifest entry found for {needle!r}. If the unit "
            f"has no manifest yet, create one with --init src/<unit>.c")
    if len(hits) > 1:
        ids = ", ".join(h[1] for h in hits)
        raise SystemExit(f"promote: {needle!r} is ambiguous ({ids}); "
                         f"use the full id")
    return hits[0]


def flip_state(text, fid, new_state):
    """Return manifest text with `fid`'s state replaced (exactly once)."""
    pattern = re.compile(
        r'(id = "' + re.escape(fid) + r'"\s*\r?\nstate = ")[a-z]+(")')
    out, n = pattern.subn(rf"\g<1>{new_state}\g<2>", text)
    if n != 1:
        raise SystemExit(f"promote: could not locate the state line for "
                         f"{fid} ({n} matches)")
    return out


def run_verifier(version):
    return subprocess.run(
        [sys.executable, str(ROOT / "tools" / "verify_promoted.py"),
         "--version", version]).returncode == 0


def results_path(version):
    return ROOT / "build" / version / "verify_results.json"


def byte_results(path, since):
    """{id: verified} for the functions a verifier run byte-compared.

    Only entries that carry a `size` were compared against retail; a unit
    that failed before the comparison (hybrid build, unresolved externals)
    reports `verified = false` for all its functions without saying which
    one broke it, so those are left out. A results file not rewritten after
    `since` (the verifier died first) yields nothing.
    """
    try:
        if path.stat().st_mtime_ns <= since:
            return {}
        data = json.loads(path.read_text())
    except (OSError, json.JSONDecodeError):
        return {}
    return {r["id"]: bool(r.get("verified")) for r in data.get("functions", [])
            if "size" in r}


def isolate_batch(fids, apply, verify, log=print):
    """Keep every promotion in `fids` that the verifier proves.

    `apply(ids)` rewrites the manifests so exactly `ids` are flipped;
    `verify()` runs the full verifier and returns (ok, {id: verified}) as
    `byte_results` reads it. Each verifier run sees every promotion accepted
    so far plus the subset under test, so an accepted set is always one the
    verifier passed as a whole. Returns a dict with `accepted` (ids, in input
    order), `rejected` ({id: reason}), `runs`, and `error` (set when the
    repository fails even without the candidates, or the final confirmation
    fails; nothing is kept then).
    """
    accepted, rejected = [], {}
    state = {"runs": 0, "last": None, "baseline": None}

    def attempt(subset):
        ids = accepted + subset
        apply(ids)
        state["runs"] += 1
        state["last"] = list(ids)
        log(f"[isolate] verifier run {state['runs']}: "
            f"{len(accepted)} accepted + {len(subset)} under test")
        return verify()

    def finish(error=None):
        if error is not None:
            accepted.clear()
        apply(accepted)
        order = {f: i for i, f in enumerate(fids)}
        return {"accepted": sorted(accepted, key=order.get),
                "rejected": rejected, "runs": state["runs"], "error": error}

    queue = [list(fids)]
    while queue:
        subset = queue.pop(0)
        if not subset:
            continue
        ok, per = attempt(subset)
        if ok:
            accepted.extend(subset)
            continue
        diverged = [f for f in subset if per.get(f) is False]
        for f in diverged:
            rejected[f] = "bytes differ from retail over the full extent"
        rest = [f for f in subset if f not in diverged]
        if diverged:
            queue.insert(0, rest)          # retry the rest as one group
            continue
        if len(rest) == 1:
            # A lone candidate fails: make sure the repository itself (with
            # the promotions accepted so far) is green before blaming it.
            if state["baseline"] is None:
                state["baseline"], _ = attempt([])
            if not state["baseline"]:
                return finish("the verifier fails even without the remaining "
                              "candidates; fix the repository first")
            rejected[rest[0]] = ("verification failed with this promotion "
                                 "(unit or image gate; see verifier output)")
            continue
        half = len(rest) // 2
        queue[:0] = [rest[:half], rest[half:]]

    # Nothing kept means the original manifests come back as they were; any
    # kept set the last run did not prove exactly is proved once more.
    if accepted and state["last"] != accepted:
        ok, _ = attempt([])
        if not ok:
            return finish("the final confirmation run failed")
    return finish()


def unit_functions(version, unit_index):
    """[(addr_int, name)] for one unit, address-sorted, from functions.toml."""
    return sorted(
        (int(r["a"], 16), r["n"]) for r in parse_toml_blocks(
            ROOT / "config" / version / "functions.toml",
            {"n": r"name = '([^']*)'", "a": r"address = (0x[0-9A-Fa-f]+)",
             "u": r"unit = (\d+)"}) if int(r["u"]) == unit_index)


def manifest_text(version, unit_index, name, src_rel, profile, funcs):
    """The exact all-asm status manifest body for a unit (used by `--init`)."""
    lines = [
        f"# Function status for {version} unit {unit_index} ({name}.c).",
        "#",
        "# Generated by `python tools/promote.py --init`. States: asm (no",
        "# accepted C), equivalent (reviewed, behaviorally equivalent C),",
        "# matching (mechanically byte-exact C -- promoted only by",
        "# tools/promote.py, verified by tools/verify_promoted.py).",
        "",
        "schema = 1",
        f'unit = "{version}:unit-{unit_index:04d}"',
        f'source = "{src_rel}"',
        f'profile = "{profile}"',
        "complete = false",
        "",
    ]
    for addr, fname in funcs:
        lines += ["[[function]]",
                  f'id = "{version}:unit-{unit_index:04d}:{addr:08x}:{fname}"',
                  'state = "asm"', ""]
    return "\n".join(lines)


def init_manifest(version, source, profile=None):
    """Create a fresh all-asm manifest for src/<unit>.c."""
    src_rel = Path(source).as_posix()
    if not (ROOT / src_rel).is_file():
        raise SystemExit(f"promote --init: {src_rel} does not exist")
    name = src_rel.removeprefix("src/").removesuffix(".c")
    row = next((r for r in unit_table(version) if r[2] == name), None)
    if row is None:
        raise SystemExit(
            f"promote --init: no .text unit is named '{name}' "
            f"(see objdiff.json for the canonical unit names)")
    unit_index = row[0]
    category = row[3]
    profile = resolve_profile(profile, category,
                              load_profile_names(ROOT / "config" / version))

    out = ROOT / "config" / version / "status" / f"{name}.toml"
    if out.is_file():
        raise SystemExit(f"promote --init: {out.relative_to(ROOT).as_posix()} "
                         f"already exists")
    funcs = unit_functions(version, unit_index)
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(manifest_text(version, unit_index, name, src_rel, profile,
                                 funcs))
    print(f"wrote {out.relative_to(ROOT).as_posix()} "
          f"({len(funcs)} functions, all asm, profile {profile!r})")
    print("Regenerate the build config so the unit gets its hybrid edges:\n"
          "    python tools/gen_ninja.py")
    return 0


def read_manifest(path):
    """Manifest text, byte-preserving (see the newline="" note in promote)."""
    with open(path, encoding="utf-8", newline="") as f:
        return f.read()


def write_manifest(path, text):
    with open(path, "w", encoding="utf-8", newline="") as f:
        f.write(text)


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("target", nargs="*",
                        help="function id(s) or unique function name(s); "
                             "several are flipped together and proved by a "
                             "single verifier run")
    parser.add_argument("--targets-file", metavar="FILE",
                        help="read additional targets from FILE, one per line "
                             "(blank lines and #-comments ignored)")
    parser.add_argument("--to", default="matching", choices=("matching",),
                        help="target state (only `matching`; `equivalent` is "
                             "a reviewed hand edit)")
    parser.add_argument("--init", metavar="SRC",
                        help="create an all-asm manifest for src/<unit>.c "
                             "instead of promoting")
    parser.add_argument("--profile", default=None,
                        help="compiler profile for the new --init manifest "
                             "(default: derived from the unit category -- "
                             "sce for SDK/SCE/newlib/libgcc, else default); "
                             "must exist in profiles.toml")
    parser.add_argument("--isolate", action="store_true",
                        help="if the batch fails, narrow it down and keep "
                             "every promotion the verifier proves (instead "
                             "of rolling the whole batch back)")
    parser.add_argument("--report", metavar="FILE",
                        help="write the per-function outcome as JSON")
    parser.add_argument("--version", default="pal103")
    args = parser.parse_args()

    if args.profile and not args.init:
        parser.error("--profile only applies to --init")
    if args.init:
        return init_manifest(args.version, args.init, args.profile)

    targets = list(args.target)
    if args.targets_file:
        for line in Path(args.targets_file).read_text().splitlines():
            line = line.split("#", 1)[0].strip()
            if line:
                targets.append(line)
    if not targets:
        parser.error("a function id/name is required (or --init SRC)")

    # Resolve every target first: a typo must not leave half the batch flipped.
    pending = []                       # [(manifest, fid, old_state)]
    already = []
    for t in targets:
        manifest, fid, state = find_entry(args.version, t)
        rel = manifest.relative_to(ROOT).as_posix()
        if state == "matching":
            print(f"{fid} is already `matching` in {rel}; nothing to do.")
            already.append(fid)
            continue
        pending.append((manifest, fid, state))
    if not pending:
        return report_outcome({"accepted": [], "rejected": {}, "runs": 0,
                               "error": None, "already": already}, {},
                              args.report)

    # newline="" everywhere: the edit must be byte-preserving outside the
    # flipped words, whatever line endings the file uses and whatever
    # platform this runs on. One original per manifest, so several functions
    # in the same file compose instead of clobbering each other.
    originals = {}
    for manifest, _, _ in pending:
        if manifest not in originals:
            originals[manifest] = read_manifest(manifest)
    manifest_of = {fid: manifest for manifest, fid, _ in pending}

    def apply(ids):
        """Rewrite the touched manifests with exactly `ids` flipped. A
        manifest that already holds that text is left alone, so its mtime
        keeps saying it predates the verifier run that proved it."""
        texts = dict(originals)
        for fid in ids:
            texts[manifest_of[fid]] = flip_state(texts[manifest_of[fid]], fid,
                                                 "matching")
        for manifest, text in texts.items():
            if read_manifest(manifest) != text:
                write_manifest(manifest, text)

    for manifest, fid, state in pending:
        rel = manifest.relative_to(ROOT).as_posix()
        print(f"{fid}: {state} -> matching in {rel}")
    print(f"\nverifying {len(pending)} promotion(s) in "
          f"{len(originals)} manifest(s)...\n", flush=True)

    fids = [fid for _, fid, _ in pending]
    try:
        if args.isolate and len(fids) > 1:
            outcome = promote_isolated(args.version, fids, apply)
        else:
            apply(fids)
            if run_verifier(args.version):
                outcome = {"accepted": fids, "rejected": {}, "runs": 1,
                           "error": None}
            else:
                apply([])
                outcome = {"accepted": [], "runs": 1,
                           "error": "the verifier rejected the batch",
                           "rejected": {f: "batch verification failed"
                                        for f in fids}}
                if len(fids) > 1:
                    outcome["hint"] = ("re-run with --isolate (or the "
                                       "targets individually) to find the "
                                       "one that does not hold")
    except BaseException:
        apply([])                      # interrupted: never leave a flip behind
        raise
    outcome["already"] = already
    return report_outcome(outcome, manifest_of, args.report)


def promote_isolated(version, fids, apply):
    """isolate_batch wired to the real verifier and its results file."""
    path = results_path(version)

    def verify():
        try:
            since = path.stat().st_mtime_ns
        except OSError:
            since = -1
        ok = run_verifier(version)
        return ok, byte_results(path, since)

    return isolate_batch(fids, apply, verify)


def report_outcome(outcome, manifest_of, report=None):
    """Print the outcome, write the JSON report, return the exit status."""
    accepted, rejected = outcome["accepted"], outcome["rejected"]
    for fid in accepted:
        print(f"Promoted {fid} to `matching`.")
    if accepted:
        touched = sorted({manifest_of[f].relative_to(ROOT).as_posix()
                          for f in accepted})
        print(f"\nCommit: {', '.join(touched)}")
    if outcome.get("error"):
        print(f"\nVerification FAILED: {outcome['error']}; rolled back all "
              f"{len(manifest_of)} promotion(s).", file=sys.stderr)
        if outcome.get("hint"):
            print(outcome["hint"].capitalize() + ".", file=sys.stderr)
    else:
        for fid, why in rejected.items():
            print(f"Not promoted {fid}: {why}", file=sys.stderr)
    if report:
        Path(report).write_text(json.dumps(
            {"accepted": accepted, "rejected": rejected,
             "already_matching": outcome.get("already", []),
             "verifier_runs": outcome.get("runs"),
             "error": outcome.get("error")}, indent=2) + "\n")
    return 1 if rejected or outcome.get("error") else 0


if __name__ == "__main__":
    sys.exit(main())
