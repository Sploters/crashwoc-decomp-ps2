"""Unit tests for tools/merge_views.py (pure Python, no compiler)."""
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from merge_views import field_decls, find_views, probe_lines, report, spelling  # noqa: E402

SOURCE = """
struct numtx_s {
    float _00, _01, _02, _03;
    float _10, _11, _12, _13;
    float _20, _21, _22, _23;
    struct nuvec_s pos; /* 0x30 */
    float _33;
};

/* struct numtx_s { int commented_out; }; */

typedef struct {
    float m[16];
} NuMtx;

struct other_s {
    struct numtx_s mtx;
    struct { int a; } inner;
};
"""


class TestFieldDecls(unittest.TestCase):
    def test_shapes(self):
        self.assertEqual(field_decls("float _00, _01;"),
                         [("_00", "float", None), ("_01", "float", None)])
        self.assertEqual(field_decls("float m[4][4];"), [("m", "float[4][4]", None)])
        self.assertEqual(field_decls("struct nuvec_s *a, b[2]; /* 0x0 */"),
                         [("a", "struct nuvec_s *", None), ("b", "struct nuvec_s[2]", None)])
        self.assertEqual(field_decls("unsigned int r : 8;"), [("r", "unsigned int", 8)])
        self.assertEqual(field_decls("void (*cb)(int a);"), [("cb", "void (*)()", None)])
        self.assertEqual(field_decls("union { float f; int i; };"), [])
        self.assertEqual(field_decls("union { float f; int i; } u;"), [("u", "union", None)])


class TestFindViews(unittest.TestCase):
    def test_tags_and_typedefs(self):
        views = find_views(SOURCE, ["numtx_s", "NuMtx"])
        self.assertEqual([(e, n) for e, n, _l, _f in views],
                         [("struct numtx_s", "numtx_s"), ("NuMtx", "NuMtx")])
        names = [f[0] for f in views[0][3]]
        self.assertEqual(names[-2:], ["pos", "_33"])
        self.assertEqual(views[1][3], [("m", "float[16]", None)])

    def test_probe_lines(self):
        views = find_views(SOURCE, ["NuMtx"])
        lines, order = probe_lines(views)
        self.assertEqual(lines, ["    sizeof(NuMtx),",
                                 "    (int)&((NuMtx *)0)->m,",
                                 "    sizeof(((NuMtx *)0)->m),"])
        self.assertEqual(order, [(0, None, "size"), (0, "m", "offset"), (0, "m", "size")])


class TestReport(unittest.TestCase):
    def test_layouts_and_name_clashes(self):
        a = {"file": "a.c", "line": 1, "name": "nuplane_s", "size": 16,
             "fields": [("a", "f32", 0, 4), ("d", "float", 12, 4)]}
        b = {"file": "b.c", "line": 1, "name": "nuplane_s", "size": 16,
             "fields": [("x", "float", 0, 4), ("d", "float", 12, 4)]}
        lines = report([a, b])
        self.assertEqual(lines[0], "sizes: 0x10 (2)")
        self.assertIn("1 distinct layout(s):", lines)     # f32 == float
        self.assertIn("  0x0000: a (1), x (1)", lines)

    def test_spelling(self):
        self.assertEqual(spelling("f32[4][4]"), "float[4][4]")
        self.assertEqual(spelling("s32"), "int")


if __name__ == "__main__":
    unittest.main()
