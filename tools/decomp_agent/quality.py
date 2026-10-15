"""Quality review: static readability checks and the grounding record.

Byte-exact is necessary for a contribution, not sufficient. After a function is
proved ``matching`` it still has to read like source a person wrote, and every
name, type and offset in it has to be backed by something. This module holds
the two deterministic halves of that review; the judgement itself belongs to a
reviewer who did not write the code (see ``workflow.py``).

* :func:`lint_function` scans one function's C (comments and strings removed)
  for constructs that usually exist only to coax the compiler into the retail
  bytes -- permuter temporaries, ``do {} while (0)``, empty blocks, raw offset
  arithmetic, inline assembly, ... A finding is a question, not a verdict: most
  rules (``severity = "justify"``) are cleared by a written justification in
  the review, because an unusual construct can be the honest reconstruction.
  ``severity = "human"`` rules (inline assembly) must be justified *and* always
  escalate to a maintainer: hand-written assembly can stand in for a
  decompilation, so no automated reviewer may accept it, but a genuine
  hardware barrier in the original source is a maintainer's call.
* :func:`validate_review` checks that a reviewer's grounding record is
  complete: what the function does and why, which names / types / offsets /
  constants are confirmed and which stay hypotheses, why this is a reasonable
  representation, which simpler alternatives were tried, and a justification
  for every lint finding. It checks completeness and consistency only; whether
  the evidence is *true* is the reviewer's responsibility and cannot be
  verified mechanically.
"""
from __future__ import annotations

import hashlib
import re

# (key, severity, pattern, message). Patterns run on comment- and
# string-stripped text, so a comment that mentions `new_var` is not a finding.
RULES = (
    ("inline_asm", "human",
     r"\b(?:__asm__|__asm|asm)\b\s*(?:volatile\s*|__volatile__\s*)?\(",
     "inline assembly (or a register pinned with asm)"),
    ("permuter_temp", "justify", r"\bnew_var\d*\b",
     "decomp-permuter temporary name"),
    ("permuter_macro", "justify", r"\bPERM_\w+",
     "decomp-permuter macro"),
    ("decompiler_name", "justify",
     r"\b(?:temp|var|phi|saved_reg|unksp)_\w+"
     r"|\bsp(?=[0-9A-Fa-f]*[0-9])[0-9A-Fa-f]{1,4}\b|\bM2C_\w+",
     "decompiler-generated name (temp_*, var_*, spXX, M2C_*)"),
    ("do_while_zero", "justify", r"\bwhile\s*\(\s*0\s*\)",
     "`do { } while (0)` / `while (0)` wrapper"),
    ("constant_condition", "justify", r"\bif\s*\(\s*[01]\s*\)",
     "`if` on a constant 0 or 1"),
    ("empty_block", "justify", r"(?:\)|\belse|\bdo)\s*\{\s*\}",
     "empty block"),
    ("empty_statement", "justify", r"(?m)^\s*;\s*$",
     "stand-alone empty statement"),
    ("goto", "justify", r"\bgoto\b", "goto"),
    ("self_assignment", "justify",
     r"(?<![\w.>])([A-Za-z_]\w*(?:(?:->|\.)\w+)*)\s*=\s*\1\s*;",
     "variable assigned to itself"),
    ("noop_arithmetic", "justify",
     r"(?<=[\w)\]])\s*(?:[-+]\s*0|\*\s*1)(?:\.0+)?f?(?![\w.\]])",
     "arithmetic with no effect (+ 0, - 0, * 1)"),
    ("raw_offset_access", "justify",
     r"\*\s*\(\s*[A-Za-z_][\w ]*\*+\s*\)\s*\(\s*\(\s*"
     r"(?:char|u8|s8|unsigned char|signed char)\s*\*\s*\)",
     "field reached through raw byte-offset arithmetic instead of a struct "
     "member"),
    ("volatile", "justify", r"\bvolatile\b", "volatile qualifier"),
    ("local_define", "justify", r"(?m)^\s*#\s*define\b",
     "macro defined inside the function"),
)

_COMPILED = tuple((k, s, re.compile(p), m) for k, s, p, m in RULES)

CHECKLIST = (
    "natural_control_flow",      # structures a person would write
    "names_supported",           # every name backed by evidence (or listed
                                 # as a hypothesis)
    "types_coherent",            # types, casts, arithmetic, conditions agree
                                 # with the observed behaviour
    "no_artificial_constructs",  # nothing there only to coax the compiler
    "follows_repo_style",        # matches the conventions of the repository
)
CONFIRMED_KINDS = ("names", "types", "offsets", "constants")
VERDICTS = ("approve", "reject", "needs_human_review")
MIN_TEXT = 20                    # a one-word "ok" is not an explanation


def source_hash(text: str) -> str:
    """Stable hash of a function's C text (line endings normalised)."""
    norm = text.replace("\r\n", "\n").strip("\n") + "\n"
    return hashlib.sha256(norm.encode("utf-8")).hexdigest()


def strip_comments(text: str) -> str:
    """Blank out comments and string/char literals, keeping every newline
    and column so finding positions still point at the original source."""
    out, i, n = [], 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            out.append(re.sub(r"[^\n]", " ", text[i:j]))
            i = j
        elif text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            out.append(" " * (j - i))
            i = j
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] != c and text[j] != "\n":
                j += 2 if text[j] == "\\" else 1
            j = min(j + 1, n)
            out.append(c + " " * (j - i - 2) + (text[j - 1] if j - i > 1 else ""))
            i = j
        else:
            out.append(c)
            i += 1
    return "".join(out)


def lint_function(text: str, first_line: int = 1) -> list:
    """Findings for one function's C text, in source order.

    Each finding is ``{"key", "rule", "severity", "line", "excerpt",
    "message"}``; ``key`` (``rule@+offset``, the offset counted in lines from
    the function's first line) is what a review's ``lint_justifications``
    refers to. Lines are absolute when `first_line` is the definition's line.
    """
    code = strip_comments(text)
    lines = text.splitlines()
    body_open = code.find("{")       # an empty *function* is a real stub
    findings = []
    for key, severity, pattern, message in _COMPILED:
        for m in pattern.finditer(code):
            if key == "empty_block" and code.find("{", m.start()) == body_open:
                continue
            rel = code.count("\n", 0, m.start())
            findings.append({
                "key": f"{key}@+{rel}",
                "rule": key,
                "severity": severity,
                "line": first_line + rel,
                "excerpt": lines[rel].strip() if rel < len(lines) else "",
                "message": message,
            })
    findings.sort(key=lambda f: (f["line"], f["rule"]))
    return findings


def _text(value) -> bool:
    return isinstance(value, str) and len(value.strip()) >= MIN_TEXT


def validate_review(review, *, findings, expected_hash) -> list:
    """Problems with a grounding record (empty list = complete).

    `findings` is the lint result for the reviewed text and `expected_hash`
    its :func:`source_hash`: a review of any other text is rejected.
    """
    if not isinstance(review, dict):
        return ["review must be a JSON object"]
    problems = []

    if review.get("source_sha256") != expected_hash:
        problems.append("source_sha256 does not match the current C text of "
                        "the function (the review is for other code)")

    reviewer = review.get("reviewer")
    if not isinstance(reviewer, dict) or not str(reviewer.get("id", "")).strip():
        problems.append("reviewer.id is required")

    if review.get("verdict") not in VERDICTS:
        problems.append(f"verdict must be one of {', '.join(VERDICTS)}")

    if not _text(review.get("summary")):
        problems.append("summary: say what the function does "
                        f"(at least {MIN_TEXT} characters)")

    evidence = review.get("evidence")
    if not isinstance(evidence, list) or not evidence:
        problems.append("evidence: list at least one {claim, source}")
    else:
        for i, item in enumerate(evidence):
            if not (isinstance(item, dict) and str(item.get("claim", "")).strip()
                    and str(item.get("source", "")).strip()):
                problems.append(f"evidence[{i}]: needs a non-empty claim and "
                                "source")

    confirmed = review.get("confirmed")
    if not isinstance(confirmed, dict):
        problems.append("confirmed: object with " + ", ".join(CONFIRMED_KINDS))
    else:
        for kind in CONFIRMED_KINDS:
            if not isinstance(confirmed.get(kind), list):
                problems.append(f"confirmed.{kind}: list (may be empty)")

    hypotheses = review.get("hypotheses")
    if not isinstance(hypotheses, list):
        problems.append("hypotheses: list (empty when nothing is hypothetical)")
    else:
        for i, h in enumerate(hypotheses):
            if not (isinstance(h, dict) and str(h.get("text", "")).strip()
                    and isinstance(h.get("blocking"), bool)):
                problems.append(f"hypotheses[{i}]: needs text and a boolean "
                                "`blocking`")

    if not _text(review.get("rationale")):
        problems.append("rationale: why this C is a reasonable representation "
                        f"(at least {MIN_TEXT} characters)")
    if not _text(review.get("simpler_alternatives")):
        problems.append("simpler_alternatives: which simpler or more readable "
                        "forms were tried and what they produced (at least "
                        f"{MIN_TEXT} characters)")

    checklist = review.get("checklist")
    if not isinstance(checklist, dict):
        problems.append("checklist: object with " + ", ".join(CHECKLIST))
    else:
        for item in CHECKLIST:
            if not isinstance(checklist.get(item), bool):
                problems.append(f"checklist.{item}: true or false")

    justified = review.get("lint_justifications") or {}
    if not isinstance(justified, dict):
        problems.append("lint_justifications: object {finding key: text}")
        justified = {}
    for f in findings:
        if not _text(justified.get(f["key"])):
            problems.append(f"lint_justifications[{f['key']}]: justify "
                            f"{f['message']} at line {f['line']}")
    return problems


def blocking_reasons(review, findings) -> list:
    """Why a complete review still cannot make the function contribution-
    ready on its own (empty list = nothing blocks)."""
    reasons = []
    for f in findings:
        if f["severity"] == "human":
            reasons.append(f"line {f['line']}: {f['message']} (only a "
                           "maintainer can accept this)")
    checklist = review.get("checklist") or {}
    failed = [k for k in CHECKLIST if checklist.get(k) is False]
    if failed:
        reasons.append("checklist items not satisfied: " + ", ".join(failed))
    for h in review.get("hypotheses") or []:
        if isinstance(h, dict) and h.get("blocking"):
            reasons.append(f"blocking hypothesis: {h.get('text')}")
    return reasons


def extract_function(source: str, name: str):
    """(text, first_line) of the C *definition* of `name` in a unit's source,
    or None when the unit does not define it.

    A definition is the name followed by a parameter list and an opening
    brace with no `;` in between, so prototypes and calls never match; the
    body runs to the matching closing brace, counted on comment- and
    string-stripped text.
    """
    code = strip_comments(source)
    head = re.compile(r"(?m)^[A-Za-z_][^;{}()#\n]*?\b" + re.escape(name)
                      + r"\s*\([^;{}]*\)\s*\{")
    for m in head.finditer(code):
        depth = 0
        for j in range(m.end() - 1, len(code)):
            if code[j] == "{":
                depth += 1
            elif code[j] == "}":
                depth -= 1
                if depth == 0:
                    return (source[m.start():j + 1],
                            code.count("\n", 0, m.start()) + 1)
    return None
