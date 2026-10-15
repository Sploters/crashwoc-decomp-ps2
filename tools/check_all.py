#!/usr/bin/env python3
"""Run every check a change must pass, in one command, before committing.

    python tools/check_all.py                      # fast: no toolchain needed
    python tools/check_all.py --full               # + the full byte gate
    python tools/check_all.py --gate upstream/main # + the contribution gate

Fast steps (what validate.yml runs, plus the prototype report):
  unit tests, status manifests, type census, prototype mismatches.
`--full` adds tools/verify_promoted.py (every `matching` claim and the
image SHA; needs the toolchain). `--gate REF` adds the workflow gate:
every function newly `matching` against REF must be ready_for_contribution.

Each step runs as its own process; one failing does not stop the others,
so a single run reports everything that is wrong. Exit status is 0 only if
every step passed.
"""
import argparse
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def steps(full=False, gate=None):
    """[(name, argv)] in the order they run."""
    py = sys.executable
    out = [
        ("unit tests", [py, "-m", "unittest", "discover", "-s", "tests"]),
        ("status manifests", [py, "tools/status.py", "--check"]),
        ("type census", [py, "tools/check_types.py", "--check"]),
        ("prototype mismatches", [py, "tools/check_prototypes.py"]),
    ]
    if full:
        out.append(("byte gate (verify_promoted)",
                    [py, "tools/verify_promoted.py"]))
    if gate:
        out.append((f"contribution gate vs {gate}",
                    [py, "-m", "tools.decomp_agent.cli", "workflow", "gate",
                     "--base", gate]))
    return out


def run_steps(plan, runner=None, log=print):
    """Run every step; return [(name, ok, seconds, tail)]."""
    runner = runner or (lambda argv: subprocess.run(
        argv, cwd=str(ROOT), capture_output=True, text=True))
    results = []
    for name, argv in plan:
        start = time.monotonic()
        proc = runner(argv)
        secs = time.monotonic() - start
        text = (proc.stdout or "") + (proc.stderr or "")
        tail = "\n".join(text.strip().splitlines()[-6:])
        ok = proc.returncode == 0
        results.append((name, ok, secs, tail))
        log(f"{'PASS' if ok else 'FAIL'}  {name}  ({secs:.0f}s)")
        if not ok and tail:
            log("      " + tail.replace("\n", "\n      "))
    return results


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--full", action="store_true",
                        help="also run the full byte gate (needs the toolchain)")
    parser.add_argument("--gate", metavar="REF",
                        help="also require everything newly `matching` vs REF "
                             "to be ready_for_contribution")
    args = parser.parse_args(argv)
    results = run_steps(steps(full=args.full, gate=args.gate))
    failed = [name for name, ok, _, _ in results if not ok]
    print(f"\n{len(results) - len(failed)}/{len(results)} checks passed"
          + (f"; failed: {', '.join(failed)}" if failed else ""))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
