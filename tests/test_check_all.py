"""tools/check_all.py: the one-command safety net runs every step and fails
if any step fails. No toolchain: the steps are replaced by scripted fakes."""
import sys
import unittest
from pathlib import Path
from types import SimpleNamespace

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import check_all


def quiet(*_a, **_k):
    pass


class TestCheckAll(unittest.TestCase):
    def test_fast_plan_needs_no_toolchain(self):
        names = [n for n, _ in check_all.steps()]
        self.assertEqual(names, ["unit tests", "status manifests",
                                 "type census", "prototype mismatches"])

    def test_full_and_gate_add_their_steps(self):
        names = [n for n, _ in check_all.steps(full=True, gate="upstream/main")]
        self.assertIn("byte gate (verify_promoted)", names)
        self.assertIn("contribution gate vs upstream/main", names)

    def test_a_failure_does_not_stop_the_other_steps(self):
        plan = [("a", ["a"]), ("b", ["b"]), ("c", ["c"])]
        ran = []

        def runner(argv):
            ran.append(argv[0])
            return SimpleNamespace(returncode=1 if argv[0] == "b" else 0,
                                   stdout="detail\n", stderr="")
        results = check_all.run_steps(plan, runner=runner, log=quiet)
        self.assertEqual(ran, ["a", "b", "c"])
        self.assertEqual([ok for _, ok, _, _ in results], [True, False, True])

    def test_exit_status_reflects_any_failure(self):
        calls = {"n": 0}

        def fake_run_steps(plan, runner=None, log=print):
            calls["n"] += 1
            return [("x", True, 0.0, ""), ("y", False, 0.0, "boom")]
        real = check_all.run_steps
        check_all.run_steps = fake_run_steps
        try:
            self.assertEqual(check_all.main([]), 1)
        finally:
            check_all.run_steps = real
        self.assertEqual(calls["n"], 1)


if __name__ == "__main__":
    unittest.main()
