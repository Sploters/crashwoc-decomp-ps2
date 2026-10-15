"""Batch promotion with failure isolation (tools/promote.py --isolate).

No toolchain: the verifier is replaced by a scripted fake that knows which
promotions "hold". The invariants under test are the ones that protect the
repository -- a kept promotion is always part of a set the verifier passed as
a whole, a failure never costs the valid promotions, a repository that is red
without the candidates keeps nothing, and an interrupted run leaves no flip.
"""
import json
import os
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import promote
from promote import byte_results, isolate_batch


class FakeVerifier:
    """Passes a set of promotions only if none of them is bad.

    `diverge` are reported per function as byte mismatches (what the real
    verifier records for a function whose bytes differ); `breaks` fail the
    run without saying which function did it (a hybrid/unit or image gate
    failure). `red` makes every run fail (a repository broken without the
    candidates)."""

    def __init__(self, diverge=(), breaks=(), red=False, fail_after=None):
        self.diverge, self.breaks, self.red = set(diverge), set(breaks), red
        self.fail_after = fail_after
        self.current = []
        self.applied = []
        self.passed = []

    def apply(self, ids):
        self.current = list(ids)
        self.applied.append(list(ids))

    def verify(self):
        run = len(self.applied)
        cur = set(self.current)
        per = {f: f not in self.diverge for f in cur}
        ok = not self.red and not (cur & (self.diverge | self.breaks))
        if self.fail_after is not None and run > self.fail_after:
            ok = False
        if ok:
            self.passed.append(set(cur))
        return ok, per


IDS = [f"pal103:unit-0001:{0x100000 + 8 * i:08x}:F{i}" for i in range(8)]


def quiet(*_a, **_k):
    pass


class TestIsolateBatch(unittest.TestCase):
    def check_invariants(self, v, out):
        # What is left on disk is exactly the accepted set ...
        self.assertEqual(set(v.current), set(out["accepted"]))
        # ... and the verifier passed that exact set as a whole.
        if out["accepted"]:
            self.assertIn(set(out["accepted"]), v.passed)
        # Nothing is both kept and rejected.
        self.assertFalse(set(out["accepted"]) & set(out["rejected"]))

    def test_all_valid_is_one_verifier_run(self):
        v = FakeVerifier()
        out = isolate_batch(IDS, v.apply, v.verify, log=quiet)
        self.assertEqual(out["accepted"], IDS)
        self.assertEqual(out["rejected"], {})
        self.assertEqual(out["runs"], 1)
        self.check_invariants(v, out)

    def test_byte_divergence_is_dropped_without_bisection(self):
        v = FakeVerifier(diverge={IDS[3]})
        out = isolate_batch(IDS, v.apply, v.verify, log=quiet)
        self.assertEqual(list(out["rejected"]), [IDS[3]])
        self.assertIn("bytes differ", out["rejected"][IDS[3]])
        self.assertEqual(out["accepted"], [f for f in IDS if f != IDS[3]])
        self.assertEqual(out["runs"], 2)      # failing run + the rest
        self.check_invariants(v, out)

    def test_unattributed_failure_is_isolated_by_bisection(self):
        v = FakeVerifier(breaks={IDS[5]})
        out = isolate_batch(IDS, v.apply, v.verify, log=quiet)
        self.assertEqual(list(out["rejected"]), [IDS[5]])
        self.assertEqual(out["accepted"], [f for f in IDS if f != IDS[5]])
        self.check_invariants(v, out)
        # Far fewer runs than verifying all 8 one by one would need with a
        # baseline check each time.
        self.assertLess(out["runs"], 1 + 8)

    def test_two_bad_functions_both_isolated(self):
        v = FakeVerifier(breaks={IDS[0]}, diverge={IDS[6]})
        out = isolate_batch(IDS, v.apply, v.verify, log=quiet)
        self.assertEqual(set(out["rejected"]), {IDS[0], IDS[6]})
        self.assertEqual(set(out["accepted"]), set(IDS) - {IDS[0], IDS[6]})
        self.check_invariants(v, out)

    def test_red_repository_keeps_nothing(self):
        v = FakeVerifier(red=True)
        out = isolate_batch(IDS, v.apply, v.verify, log=quiet)
        self.assertEqual(out["accepted"], [])
        self.assertIn("even without", out["error"])
        self.assertEqual(v.current, [])        # manifests back to original

    def test_final_confirmation_runs_and_its_failure_keeps_nothing(self):
        # Two unattributed failures among four: the last bisection run tests
        # a set larger than what is kept, so one confirmation run (the 9th)
        # re-proves the kept set. Here that run fails.
        ids = IDS[:4]
        ok_v = FakeVerifier(breaks={ids[1], ids[3]})
        out = isolate_batch(ids, ok_v.apply, ok_v.verify, log=quiet)
        self.assertEqual(out["accepted"], [ids[0], ids[2]])
        self.assertEqual(out["runs"], 9)
        self.assertEqual(ok_v.applied[-1], [ids[0], ids[2]])
        self.check_invariants(ok_v, out)

        bad_v = FakeVerifier(breaks={ids[1], ids[3]}, fail_after=8)
        out = isolate_batch(ids, bad_v.apply, bad_v.verify, log=quiet)
        self.assertEqual(out["accepted"], [])
        self.assertIn("final confirmation", out["error"])
        self.assertEqual(bad_v.current, [])

    def test_all_bad_needs_no_confirmation_run(self):
        v = FakeVerifier(diverge=set(IDS[:2]))
        out = isolate_batch(IDS[:2], v.apply, v.verify, log=quiet)
        self.assertEqual(out["accepted"], [])
        self.assertEqual(out["runs"], 1)
        self.assertEqual(v.current, [])


class TestByteResults(unittest.TestCase):
    def test_only_byte_compared_entries_and_fresh_file(self):
        with tempfile.TemporaryDirectory() as d:
            path = Path(d) / "verify_results.json"
            path.write_text(json.dumps({"functions": [
                {"id": "a", "state": "matching", "verified": True, "size": 8},
                {"id": "b", "state": "matching", "verified": False, "size": 8},
                {"id": "c", "state": "matching", "verified": False,
                 "reason": "unit failed"}]}))
            before = path.stat().st_mtime_ns - 1
            self.assertEqual(byte_results(path, before),
                             {"a": True, "b": False})
            # Not rewritten since `since`: the verifier died -> no evidence.
            self.assertEqual(byte_results(path, path.stat().st_mtime_ns), {})
            self.assertEqual(byte_results(Path(d) / "missing.json", -1), {})


MANIFEST = """\
schema = 1
unit = "pal103:unit-0001"
source = "src/nucore/nulist.c"
profile = "default"
complete = false

[[function]]
id = "pal103:unit-0001:00100040:NuListGetHead"
state = "asm"

[[function]]
id = "pal103:unit-0001:00100060:NuListDoWork"
state = "asm"

[[function]]
id = "pal103:unit-0001:00100100:NuListEnd"
state = "asm"
"""


class TestPromoteMain(unittest.TestCase):
    """promote.main end to end on a temporary manifest tree."""

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        status = self.root / "config" / "pal103" / "status" / "nucore"
        status.mkdir(parents=True)
        self.manifest = status / "nulist.toml"
        self.manifest.write_bytes(MANIFEST.encode())
        self.results = self.root / "build" / "pal103" / "verify_results.json"
        self.results.parent.mkdir(parents=True)
        self.patch = mock.patch.object(promote, "ROOT", self.root)
        self.patch.start()

    def tearDown(self):
        self.patch.stop()
        self.tmp.cleanup()

    def fake_verifier(self, bad):
        """run_verifier stand-in: reads the manifest promote.py wrote and
        fails when a `bad` function is flipped (recording it as a byte
        mismatch, as the real verifier does)."""
        def run(version):
            text = self.manifest.read_text()
            flipped = [line.split('"')[1] for line, nxt in
                       zip(text.splitlines(), text.splitlines()[1:])
                       if line.startswith("id =") and "matching" in nxt]
            funcs = [{"id": f, "state": "matching", "verified": f not in bad,
                      "size": 8} for f in flipped]
            self.results.write_text(json.dumps({"functions": funcs}))
            st = self.results.stat()
            os.utime(self.results, ns=(st.st_atime_ns, st.st_mtime_ns + 1000))
            return not (set(flipped) & set(bad))
        return run

    def main(self, *argv):
        with mock.patch.object(sys, "argv", ["promote.py", *argv]):
            return promote.main()

    def test_isolate_keeps_valid_and_reports(self):
        bad = {"pal103:unit-0001:00100060:NuListDoWork"}
        report = self.root / "report.json"
        with mock.patch.object(promote, "run_verifier", self.fake_verifier(bad)):
            rc = self.main("NuListGetHead", "NuListDoWork", "NuListEnd",
                           "--isolate", "--report", str(report))
        self.assertEqual(rc, 1)                      # something was rejected
        out = json.loads(report.read_text())
        self.assertEqual(out["accepted"],
                         ["pal103:unit-0001:00100040:NuListGetHead",
                          "pal103:unit-0001:00100100:NuListEnd"])
        self.assertEqual(list(out["rejected"]), sorted(bad))
        expected = MANIFEST.replace(
            'NuListGetHead"\nstate = "asm"', 'NuListGetHead"\nstate = "matching"'
        ).replace('NuListEnd"\nstate = "asm"', 'NuListEnd"\nstate = "matching"')
        # Byte-exact manifest: only the two kept words changed.
        self.assertEqual(self.manifest.read_bytes(), expected.encode())

    def test_without_isolate_a_failure_rolls_back_everything(self):
        bad = {"pal103:unit-0001:00100060:NuListDoWork"}
        with mock.patch.object(promote, "run_verifier", self.fake_verifier(bad)):
            rc = self.main("NuListGetHead", "NuListDoWork")
        self.assertEqual(rc, 1)
        self.assertEqual(self.manifest.read_bytes(), MANIFEST.encode())

    def test_single_function_still_works(self):
        with mock.patch.object(promote, "run_verifier", self.fake_verifier(())):
            rc = self.main("NuListEnd")
        self.assertEqual(rc, 0)
        self.assertIn('NuListEnd"\nstate = "matching"',
                      self.manifest.read_text())
        self.assertEqual(self.manifest.read_text().count("matching"), 1)

    def test_kept_manifest_is_not_rewritten_after_the_proving_run(self):
        # The final apply() must not touch a manifest whose text is already
        # right: a newer mtime would make the verifier results look stale.
        bad = {"pal103:unit-0001:00100060:NuListDoWork"}
        stamps = []
        real = self.fake_verifier(bad)

        def run(version):
            ok = real(version)
            stamps.append(self.manifest.stat().st_mtime_ns)
            return ok
        with mock.patch.object(promote, "run_verifier", run):
            self.main("NuListGetHead", "NuListDoWork", "NuListEnd",
                      "--isolate")
        self.assertEqual(self.manifest.stat().st_mtime_ns, stamps[-1])

    def test_interrupt_restores_the_manifest(self):
        def boom(version):
            raise KeyboardInterrupt
        with mock.patch.object(promote, "run_verifier", boom):
            with self.assertRaises(KeyboardInterrupt):
                self.main("NuListGetHead", "NuListEnd", "--isolate")
        self.assertEqual(self.manifest.read_bytes(), MANIFEST.encode())


if __name__ == "__main__":
    unittest.main()
