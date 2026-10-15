"""Unit tests for tools/check_types.py (pure Python, no game files)."""
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import check_types  # noqa: E402
from check_types import (census, check, normalize_type, param_type,  # noqa: E402
                         typedef_aliases)

A = """
typedef unsigned int u32;
typedef struct nuvec_s NuVec;
struct nuvec_s { float x; float y; float z; };
struct holder_s {
    int a;
    char pad[0x20 - sizeof(struct nuvec_s)];
};
extern u32 D_00100000;
extern struct nuvec_s D_00100010[4];
int strlen(char *s);
unsigned long levbits;
long long fine;
void NuVecAdd(NuVec *out, NuVec *a, NuVec *b);
int Foo(int x) { return *(int *)((char *)&x + 0x10); }
/* struct nuvec_s { int commented_out; }; long */
#if 0
struct nuvec_s { int disabled; };
#endif
"""

B = """
typedef unsigned int u32;
struct nuvec_s { float x, y, z; };
extern int D_00100000;
extern struct nuvec_s D_00100010[];
void NuVecAdd(struct nuvec_s *o, struct nuvec_s *a, struct nuvec_s *b);
int Foo(float x);
static int Local(void) { return 0; }
"""

C = """
static int Local(int x) { return x; }
int Bar(void) { return *(short *)((u32)ptr + 2); }
#define va_start(ap, last) (__builtin_args_info(2))
"""


def run(sources):
    return census(sources)


class TestNormalize(unittest.TestCase):
    def test_spellings(self):
        self.assertEqual(normalize_type("u32"), "unsigned int")
        self.assertEqual(normalize_type("unsigned"), "unsigned int")
        self.assertEqual(normalize_type("unsigned int"), "unsigned int")
        self.assertEqual(normalize_type("const s32 *"), "int *")
        self.assertEqual(normalize_type("long unsigned int"), "unsigned long")
        self.assertEqual(normalize_type("u64"), "unsigned long long")

    def test_aliases(self):
        aliases = {"NuVec": "struct nuvec_s"}
        self.assertEqual(normalize_type("NuVec *", aliases), "struct nuvec_s *")
        # A tag spelled like a typedef is not rewritten.
        self.assertEqual(normalize_type("struct NuVec", aliases), "struct NuVec")

    def test_param_type(self):
        self.assertEqual(param_type("unsigned int channel"), "unsigned int")
        self.assertEqual(param_type("unsigned int"), "unsigned int")
        self.assertEqual(param_type("char *s"), "char *")
        self.assertEqual(param_type("float v[3]"), "float *")
        self.assertEqual(param_type("void (*cb)(int a, char *b)"),
                         "void (*)(int, char *)")

    def test_typedef_aliases_need_agreement(self):
        same = typedef_aliases({"a": "typedef struct nuvec_s NuVec;",
                                "b": "typedef struct nuvec_s NuVec;"})
        self.assertEqual(same["NuVec"], "struct nuvec_s")
        split = typedef_aliases({"a": "typedef struct nuvec_s NuVec;",
                                 "b": "typedef struct NuVec { int x; } NuVec;"})
        self.assertNotIn("NuVec", split)


class TestCensus(unittest.TestCase):
    def setUp(self):
        self.result = run({"src/a.c": A, "src/b.c": B, "src/c.c": C})

    def value(self, metric):
        return self.result[metric]["value"]

    def test_struct_redefinitions(self):
        # nuvec_s in a.c and b.c; the commented and #if 0 copies do not count.
        self.assertEqual(self.value("struct_redefinitions"), 1)

    def test_typedef_redefinitions(self):
        self.assertEqual(self.value("typedef_redefinitions"), 1)   # u32

    def test_long(self):
        sites = self.result["long_types"]["findings"]
        self.assertEqual(self.value("long_types"), 1)
        self.assertEqual(sites[0][:2], ("src/a.c", 12))

    def test_offset_casts(self):
        self.assertEqual(self.value("offset_casts"), 2)

    def test_sizeof_pads(self):
        self.assertEqual(self.value("sizeof_pads"), 1)

    def test_libc(self):
        self.assertEqual(self.value("libc_prototypes"), 1)

    def test_varargs(self):
        self.assertEqual(self.value("varargs_hacks"), 1)

    def test_extern_conflicts(self):
        # D_00100000 is u32 vs int; D_00100010 agrees ([4] and [] are both arrays).
        details = [d for _p, _l, d in self.result["extern_conflicts"]["findings"]]
        self.assertEqual(len(details), 1)
        self.assertTrue(details[0].startswith("D_00100000:"))

    def test_prototype_conflicts(self):
        # NuVec resolves to struct nuvec_s, so NuVecAdd agrees; Foo differs;
        # Local is static in two files and is not compared.
        details = [d for _p, _l, d in self.result["prototype_conflicts"]["findings"]]
        self.assertEqual(len(details), 1)
        self.assertTrue(details[0].startswith("Foo:"))

    def test_shared_headers_exempt(self):
        result = run({"include/nu_stdarg.h": C, "include/nu_libc.h": "int strlen(char *);"})
        self.assertEqual(result["varargs_hacks"]["value"], 0)
        self.assertEqual(result["libc_prototypes"]["value"], 0)


class TestRatchet(unittest.TestCase):
    def test_check(self):
        result = {name: {"value": 5, "findings": []} for name in check_types.METRICS}
        same = dict.fromkeys(check_types.METRICS, 5)
        self.assertEqual(check(result, same), ([], []))
        worse, better = check(result, {**same, "long_types": 4, "offset_casts": 6})
        self.assertEqual(worse, ["long_types: 4 -> 5"])
        self.assertEqual(better, ["offset_casts: 6 -> 5"])
        worse, _ = check(result, {})
        self.assertEqual(len(worse), len(check_types.METRICS))

    def test_baseline_round_trip(self):
        result = run({"src/a.c": A, "src/b.c": B})
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "census.toml"
            check_types.write_baseline(result, path)
            baseline = check_types.load_baseline(path)
        self.assertEqual(check(result, baseline), ([], []))

    def test_committed_baseline_matches_tree(self):
        """The committed counts equal a fresh census of src/ and include/."""
        baseline = check_types.load_baseline()
        self.assertIsNotNone(baseline, "config/type_census.toml missing")
        self.assertEqual(check(census(check_types.load_sources()), baseline), ([], []))


if __name__ == "__main__":
    unittest.main()
