"""Contribution workflow: states, batch promotion bookkeeping, quality review.

Pure Python on the synthetic project of test_decomp_agent: compile_diff, the
promotion and the verifier results are scripted fakes, so these tests pin the
*rules* -- what may move a function to which state -- not toolchain output.
"""
import json
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(Path(__file__).resolve().parent))

from decomp_agent import quality, workflow
from test_decomp_agent import make_project

DOWORK = "pal103:unit-0001:00100060:NuListDoWork"
HEAD = "pal103:unit-0001:00100040:NuListGetHead"

CLEAN_C = """\
int NuListGetHead(void){return 0;}

int NuListDoWork(int count)
{
    return count + 1;
}
"""

PERMUTER_C = """\
int NuListGetHead(void){return 0;}

int NuListDoWork(int count)
{
    int new_var;
    new_var = count;
    do {
        new_var++;
    } while (0);
    return new_var;
}
"""


class Fakes:
    """Scripted toolchain: which functions compile exactly, which the
    verifier accepts, and what the last verifier run reported."""

    def __init__(self, project):
        self.project = project
        self.exact = {}
        self.reject = {}
        self.green = True
        self.promote_calls = []

    def compile_diff(self, project, fid):
        exact = self.exact.get(fid, False)
        return {"ok": True, "exact": exact,
                "differing_bytes": 0 if exact else 12,
                "diff_signature": "sig", "candidate_asm_hash": "h"}

    def promote(self, project, fids, report_path):
        self.promote_calls.append(list(fids))
        manifest = project.status_dir / "nucore" / "nulist.toml"
        text = manifest.read_text()
        accepted = [f for f in fids if f not in self.reject]
        for fid in accepted:
            text = text.replace(f'id = "{fid}"\nstate = "asm"',
                                f'id = "{fid}"\nstate = "matching"')
        manifest.write_text(text)
        return {"accepted": accepted,
                "rejected": {f: self.reject[f] for f in fids
                             if f in self.reject},
                "verifier_runs": 1 if not self.reject else 3, "error": None}

    def verify(self, project):
        return self.green

    def results(self, project):
        states = workflow.manifest_states(project)
        return {"all_green": self.green, "profiles_fingerprint": "fp",
                "functions": [{"id": f, "state": "matching", "verified": True,
                               "size": 8}
                              for f, s in states.items() if s == "matching"]}

    def batch(self, **kw):
        return workflow.batch(self.project, compile_diff=self.compile_diff,
                              promote=self.promote, verify=self.verify,
                              results=self.results, **kw)


def good_review(record, reviewer="reviewer-agent", **override):
    review = workflow.review_template(record)
    review.update({
        "reviewer": {"id": reviewer, "kind": "agent"},
        "verdict": "approve",
        "summary": "Returns its argument incremented by one; no side effects.",
        "evidence": [{"claim": "addiu v0, a0, 1 then jr ra",
                      "source": "retail disassembly at 0x00100060"}],
        "confirmed": {"names": [], "types": ["int count (a0, 32-bit)"],
                      "offsets": [], "constants": ["1"]},
        "hypotheses": [],
        "rationale": "A single expression mirrors the single addiu exactly.",
        "simpler_alternatives": "This is already the simplest form; nothing "
                                "shorter expresses the same operation.",
        "checklist": {k: True for k in quality.CHECKLIST},
    })
    review.update(override)
    return review


class WorkflowCase(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.p = make_project(Path(self.tmp.name))
        self.src = self.p.root / "src" / "nucore" / "nulist.c"
        self.src.write_text(CLEAN_C)
        self.f = Fakes(self.p)

    def tearDown(self):
        self.tmp.cleanup()

    def state(self, fid=DOWORK):
        return workflow.load(self.p, fid)["state"]

    def to_quality_review(self, author="author-agent"):
        self.f.exact[DOWORK] = True
        workflow.submit(self.p, DOWORK, author=author,
                        compile_diff=self.f.compile_diff)
        out = self.f.batch()
        self.assertEqual(out["promoted"], [DOWORK])
        self.assertEqual(self.state(), "quality_review")
        return workflow.load(self.p, DOWORK)

    def review(self, review):
        return workflow.submit_review(self.p, DOWORK, review,
                                      results=self.f.results)


class TestStates(WorkflowCase):
    def test_illegal_transitions_are_refused(self):
        record = {"function_id": DOWORK, "state": "in_progress"}
        for to in ("ready_for_contribution", "quality_review",
                   "needs_human_review"):
            with self.assertRaises(workflow.WorkflowError):
                workflow.transition(dict(record), to, "skip")
        with self.assertRaises(workflow.WorkflowError):
            workflow.transition({"function_id": DOWORK, "state": None},
                                "exact_match", "no draft")
        with self.assertRaises(workflow.WorkflowError):
            workflow.transition(dict(record), "matching", "manifest word")

    def test_readable_but_not_exact_never_leaves_in_progress(self):
        self.f.exact[DOWORK] = False
        out = workflow.submit(self.p, DOWORK, author="author-agent",
                              compile_diff=self.f.compile_diff)
        self.assertEqual(out["state"], "in_progress")
        self.assertFalse(out["queued"])
        self.assertEqual(self.f.batch()["queued"], 0)
        self.assertEqual(self.f.promote_calls, [])
        self.assertEqual(self.state(), "in_progress")
        refused = self.review(good_review(workflow.load(self.p, DOWORK)))
        self.assertFalse(refused["ok"])
        self.assertEqual(refused["error"]["code"], "wrong_state")
        self.assertEqual(workflow.manifest_states(self.p)[DOWORK], "asm")

    def test_author_is_required(self):
        out = workflow.submit(self.p, DOWORK, author="",
                              compile_diff=self.f.compile_diff)
        self.assertEqual(out["error"]["code"], "author_required")

    def test_full_path_to_ready_for_contribution(self):
        record = self.to_quality_review()
        self.assertTrue(Path(workflow._dir(self.p) / "review" /
                             f"{workflow._slug(DOWORK)}.md").is_file())
        out = self.review(good_review(record))
        self.assertTrue(out["ok"], out)
        self.assertEqual(out["state"], "ready_for_contribution")
        history = [h["to"] for h in workflow.load(self.p, DOWORK)["history"]]
        self.assertEqual(history, ["in_progress", "exact_match",
                                   "quality_review", "ready_for_contribution"])
        gate = workflow.gate(self.p, base="base", base_states={DOWORK: "asm",
                                                              HEAD: "matching"},
                             results=self.f.results)
        self.assertTrue(gate["passed"], gate)

    def test_exact_match_alone_does_not_pass_the_gate(self):
        self.to_quality_review()
        gate = workflow.gate(self.p, base="base", base_states={DOWORK: "asm",
                                                              HEAD: "matching"},
                             results=self.f.results)
        self.assertFalse(gate["passed"])
        self.assertIn("quality_review", gate["failing"][DOWORK])

    def test_matching_without_any_workflow_record_fails_the_gate(self):
        gate = workflow.gate(self.p, base="base", base_states={},
                             results=self.f.results)
        self.assertFalse(gate["passed"])
        self.assertIn("never went through", gate["failing"][HEAD])

    def test_edit_after_verification_invalidates_the_proof(self):
        record = self.to_quality_review()
        self.src.write_text(CLEAN_C.replace("count + 1", "1 + count"))
        out = self.review(good_review(record))
        self.assertFalse(out["ok"])
        self.assertEqual(out["error"]["code"], "stale")
        self.assertEqual(self.state(), "in_progress")

    def test_edit_after_ready_sends_it_back(self):
        record = self.to_quality_review()
        self.review(good_review(record))
        self.src.write_text(CLEAN_C.replace("count + 1", "1 + count"))
        gate = workflow.gate(self.p, base="base", base_states={DOWORK: "asm",
                                                              HEAD: "matching"},
                             results=self.f.results)
        self.assertFalse(gate["passed"])
        self.assertEqual(self.state(), "in_progress")


class TestVerifierFreshness(WorkflowCase):
    def write_results(self, fids):
        path = self.p.root / "build" / "pal103" / "verify_results.json"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(json.dumps({"all_green": True, "functions": [
            {"id": f, "state": "matching", "verified": True, "size": 8}
            for f in fids]}))
        return path

    def test_results_older_than_the_sources_prove_nothing(self):
        import os
        path = self.write_results([HEAD])
        newer = path.stat().st_mtime_ns + 10**9
        os.utime(path, ns=(newer, newer))
        fresh = workflow.read_verify_results(self.p)
        self.assertTrue(workflow.verified_by_results(fresh, HEAD))
        # A pull/rebase/edit touches a header after the run.
        header = self.p.root / "include" / "nu_types.h"
        header.parent.mkdir(parents=True, exist_ok=True)
        header.write_text("typedef int s32;\n")
        later = newer + 10**9
        os.utime(header, ns=(later, later))
        stale = workflow.read_verify_results(self.p)
        self.assertTrue(stale.get("_stale"))
        self.assertFalse(workflow.verified_by_results(stale, HEAD))

    def test_stale_results_block_the_gate(self):
        import os
        record = self.to_quality_review()
        self.review(good_review(record))
        path = self.write_results([DOWORK, HEAD])
        old = path.stat().st_mtime_ns - 10**10
        os.utime(path, ns=(old, old))           # run predates the sources
        gate = workflow.gate(self.p, base="base",
                             base_states={DOWORK: "asm", HEAD: "matching"})
        self.assertFalse(gate["passed"])
        self.assertIn("predates", gate["failing"][DOWORK])


class TestBatch(WorkflowCase):
    def test_one_promotion_call_for_the_whole_queue(self):
        self.src.write_text(CLEAN_C)
        self.f.exact[DOWORK] = True
        workflow.submit(self.p, DOWORK, author="a",
                        compile_diff=self.f.compile_diff)
        # HEAD is already `matching`: it is re-proved, not re-flipped.
        self.f.exact[HEAD] = True
        workflow.submit(self.p, HEAD, author="a",
                        compile_diff=self.f.compile_diff)
        out = self.f.batch()
        self.assertEqual(self.f.promote_calls, [[DOWORK]])
        self.assertEqual(sorted(out["promoted"]), sorted([DOWORK, HEAD]))

    def test_rejected_candidate_stays_in_progress_valid_one_kept(self):
        self.f.exact[DOWORK] = self.f.exact[HEAD] = True
        manifest = self.p.status_dir / "nucore" / "nulist.toml"
        manifest.write_text(manifest.read_text().replace(
            f'id = "{HEAD}"\nstate = "matching"', f'id = "{HEAD}"\nstate = "asm"'))
        for fid in (DOWORK, HEAD):
            workflow.submit(self.p, fid, author="a",
                            compile_diff=self.f.compile_diff)
        self.f.reject[HEAD] = "bytes differ from retail over the full extent"
        out = self.f.batch()
        self.assertEqual(out["promoted"], [DOWORK])
        self.assertIn("bytes differ", out["not_promoted"][HEAD])
        self.assertEqual(self.state(HEAD), "in_progress")
        self.assertEqual(self.state(DOWORK), "quality_review")
        self.assertEqual(workflow.manifest_states(self.p)[HEAD], "asm")

    def test_no_longer_exact_at_batch_time_is_dropped(self):
        self.f.exact[DOWORK] = True
        workflow.submit(self.p, DOWORK, author="a",
                        compile_diff=self.f.compile_diff)
        self.f.exact[DOWORK] = False
        out = self.f.batch()
        self.assertEqual(out["promoted"], [])
        self.assertIn("no longer exact", out["not_promoted"][DOWORK])
        self.assertEqual(self.f.promote_calls, [])
        self.assertEqual(self.state(), "in_progress")

    def test_red_verifier_promotes_nothing(self):
        self.f.exact[DOWORK] = True
        workflow.submit(self.p, DOWORK, author="a",
                        compile_diff=self.f.compile_diff)
        self.f.green = False
        out = self.f.batch()
        self.assertEqual(out["promoted"], [])
        self.assertEqual(self.state(), "in_progress")


class TestQualityReview(WorkflowCase):
    def test_exact_but_artificial_code_cannot_be_approved_unjustified(self):
        self.src.write_text(PERMUTER_C)
        record = self.to_quality_review()
        rules = {f["rule"] for f in record["lint"]["findings"]}
        self.assertTrue({"permuter_temp", "do_while_zero"} <= rules)
        out = self.review(good_review(record))
        self.assertFalse(out["ok"])
        self.assertEqual(out["error"]["code"], "incomplete")
        self.assertTrue(any("lint_justifications" in p
                            for p in out["problems"]))
        self.assertEqual(self.state(), "quality_review")

    def test_reviewer_rejecting_artificial_code_returns_it_to_work(self):
        self.src.write_text(PERMUTER_C)
        record = self.to_quality_review()
        out = self.review(good_review(
            record, verdict="reject",
            rationale="new_var and the do/while(0) exist only to steer "
                      "register allocation; rewrite as plain C."))
        self.assertEqual(out["state"], "in_progress")
        self.assertEqual(self.state(), "in_progress")

    def test_failed_checklist_escalates_to_a_human(self):
        record = self.to_quality_review()
        checklist = {k: True for k in quality.CHECKLIST}
        checklist["names_supported"] = False
        out = self.review(good_review(record, checklist=checklist))
        self.assertEqual(out["state"], "needs_human_review")

    def test_blocking_hypothesis_escalates_to_a_human(self):
        record = self.to_quality_review()
        out = self.review(good_review(record, hypotheses=[
            {"text": "the parameter may be an enum, not a count",
             "blocking": True}]))
        self.assertEqual(out["state"], "needs_human_review")

    def test_non_blocking_hypothesis_is_kept_and_allowed(self):
        record = self.to_quality_review()
        out = self.review(good_review(record, hypotheses=[
            {"text": "`count` is a guess from the caller", "blocking": False}]))
        self.assertEqual(out["state"], "ready_for_contribution")
        kept = workflow.load(self.p, DOWORK)["approved_review"]["hypotheses"]
        self.assertEqual(kept[0]["blocking"], False)

    def test_author_cannot_review_their_own_function(self):
        record = self.to_quality_review(author="same-agent")
        out = self.review(good_review(record, reviewer="same-agent"))
        self.assertFalse(out["ok"])
        self.assertEqual(out["error"]["code"], "not_independent")
        self.assertEqual(self.state(), "quality_review")

    def test_review_of_other_source_is_refused(self):
        record = self.to_quality_review()
        out = self.review(good_review(record, source_sha256="0" * 64))
        self.assertEqual(out["error"]["code"], "incomplete")
        self.assertTrue(any("source_sha256" in p for p in out["problems"]))

    def test_incomplete_record_lists_every_gap(self):
        record = self.to_quality_review()
        out = self.review({"verdict": "approve",
                           "source_sha256": record["verified"]["source_sha256"],
                           "reviewer": {"id": "r"}})
        self.assertFalse(out["ok"])
        joined = " ".join(out["problems"])
        for field in ("summary", "evidence", "confirmed", "hypotheses",
                      "rationale", "simpler_alternatives", "checklist"):
            self.assertIn(field, joined)

    def test_inline_asm_always_needs_a_human(self):
        self.src.write_text(CLEAN_C.replace(
            "    return count + 1;",
            "    __asm__ volatile (\"sync\");\n    return count + 1;"))
        record = self.to_quality_review()
        key = next(f["key"] for f in record["lint"]["findings"]
                   if f["rule"] == "inline_asm")
        just = {k: "The original SDK issues a sync barrier here; see "
                   "the retail disassembly." for k in
                [f["key"] for f in record["lint"]["findings"]]}
        out = self.review(good_review(record, lint_justifications=just))
        self.assertEqual(out["state"], "needs_human_review")
        self.assertTrue(any(key.split("@")[0] in r or "assembly" in r
                            for r in out["reasons"]))
        # A maintainer may accept it, but only with a note on record.
        short = workflow.human_decision(self.p, DOWORK, decision="approve",
                                        by="maintainer", note="ok",
                                        results=self.f.results)
        self.assertFalse(short["ok"])
        done = workflow.human_decision(
            self.p, DOWORK, decision="approve", by="maintainer",
            note="Checked against the SDK source: sync is original.",
            results=self.f.results)
        self.assertEqual(done["state"], "ready_for_contribution")


class TestLint(unittest.TestCase):
    def rules(self, body):
        return [f["rule"] for f in quality.lint_function(body)]

    def test_patterns_flagged(self):
        self.assertIn("permuter_temp", self.rules("void f(){int new_var2;}"))
        self.assertIn("decompiler_name", self.rules("void f(){int sp1C;}"))
        self.assertIn("do_while_zero", self.rules("void f(){do{g();}while(0);}"))
        self.assertIn("empty_block", self.rules("void f(int a){if(a){}}"))
        self.assertIn("raw_offset_access",
                      self.rules("int f(void *p){return *(int *)((char *)p + 0x10);}"))
        self.assertIn("self_assignment", self.rules("void f(int a){a = a;}"))
        self.assertIn("noop_arithmetic", self.rules("float f(float a){return a + 0.0f;}"))
        self.assertIn("inline_asm", self.rules('void f(){__asm__ volatile("sync");}'))

    def test_natural_code_is_not_flagged(self):
        self.assertEqual(self.rules("void f(void){}"), [])          # stub
        self.assertEqual(self.rules("void f(){while(1){g();}}"), [])
        self.assertEqual(self.rules("void f(int *v,int i){v[i*3 + 0] = 1;}"), [])
        self.assertEqual(self.rules("void f(struct s *spec){spec->x = 0x10;}"), [])
        self.assertEqual(self.rules("int f(int a){return a - 0x10;}"), [])

    def test_comments_and_strings_are_ignored(self):
        self.assertEqual(self.rules('void f(){/* new_var do{}while(0) */ '
                                    'g("asm(x)"); // goto\n}'), [])

    def test_extract_skips_prototypes_and_calls(self):
        src = ("int Foo(int a);\n"
               "void Bar(void) { Foo(1); }\n"
               "int Foo(int a)\n{\n    return a; /* } */\n}\n")
        text, line = quality.extract_function(src, "Foo")
        self.assertEqual(line, 3)
        self.assertTrue(text.startswith("int Foo(int a)\n{"))
        self.assertTrue(text.endswith("}"))
        self.assertIsNone(quality.extract_function(src, "Missing"))

    def test_hash_ignores_line_endings_only(self):
        a = "int f(void)\n{\n    return 0;\n}"
        self.assertEqual(quality.source_hash(a),
                         quality.source_hash(a.replace("\n", "\r\n")))
        self.assertNotEqual(quality.source_hash(a),
                            quality.source_hash(a.replace("0", "1")))


if __name__ == "__main__":
    unittest.main()
