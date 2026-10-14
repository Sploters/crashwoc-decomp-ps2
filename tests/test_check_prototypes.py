"""Unit tests for tools/check_prototypes.py (pure Python, no game files)."""
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from check_prototypes import (find_mismatches, function_name, param_classes,
                              strip_comments, type_class)

DEFINING = """
int NuSoundStopStream(int stream) { return 0; }
void NuStrCpy(char *dst, char *src) { }
void NuMtxRotateY(struct numtx_s *m, int a) { }
float NuFsqrt(float x) { return x; }
"""

STATES = {
    "src/nusound/def.c": {"NuSoundStopStream": "matching", "NuStrCpy": "matching",
                          "NuMtxRotateY": "matching", "NuFsqrt": "asm"},
    "src/game/use.c": {"Caller": "asm", "Matched": "matching"},
}


def check(using, states=STATES, trust_all=False):
    return find_mismatches({"src/nusound/def.c": DEFINING, "src/game/use.c": using},
                           states, trust_all)


class TestClasses(unittest.TestCase):
    def test_type_class(self):
        self.assertEqual(type_class("void"), "void")
        self.assertEqual(type_class("f32"), "float")
        self.assertEqual(type_class("struct nuvec_s *"), "int")
        self.assertEqual(type_class("unsigned short"), "int")
        self.assertEqual(type_class("double"), "wide")
        self.assertEqual(type_class("f64"), "wide")
        self.assertEqual(type_class("unsigned long long"), "wide")
        self.assertEqual(type_class("double *"), "int")

    def test_param_classes_ignore_cross_class_order(self):
        self.assertEqual(param_classes("int a, float b"), param_classes("float b, int a"))
        self.assertNotEqual(param_classes("int a"), param_classes("float a"))
        self.assertEqual(param_classes("void"), ((), ()))
        self.assertIsNone(param_classes("char *fmt, ..."))

    def test_soft_doubles_travel_in_gprs(self):
        # Soft-double: a double is a 64-bit GPR value, not an FPR one.
        self.assertEqual(param_classes("double d"), param_classes("u64 d"))
        self.assertNotEqual(param_classes("double d"), param_classes("float d"))
        self.assertNotEqual(param_classes("int a"), param_classes("s64 a"))
        self.assertNotEqual(param_classes("int a, double d"),
                            param_classes("double d, int a"))

    def test_function_name_strips_only_the_duplicate_suffix(self):
        self.assertEqual(function_name("pal103:unit-0001:00100000:Foo"), "Foo")
        self.assertEqual(function_name("pal103:unit-0001:00100000:Foo__00100000"), "Foo")
        self.assertEqual(function_name("pal103:unit-0001:00100000:__sread"), "__sread")
        self.assertEqual(function_name("pal103:unit-0001:00100000:__ieee754_pow"),
                         "__ieee754_pow")

    def test_strip_comments_keeps_line_numbers(self):
        text = "a\n/* x\ny */\nb"
        self.assertEqual(strip_comments(text).count("\n"), text.count("\n"))


class TestFindMismatches(unittest.TestCase):
    def test_void_vs_int_return_with_asm_caller(self):
        found = check("extern void NuSoundStopStream(int s);\n"
                      "void Caller(void) { NuSoundStopStream(4); }\n")
        self.assertEqual(len(found), 1)
        self.assertEqual(found[0]["name"], "NuSoundStopStream")
        self.assertEqual(found[0]["kinds"], ["return"])
        self.assertEqual(found[0]["asm_callers"], ["Caller"])
        self.assertEqual(found[0]["line"], 1)

    def test_value_vs_void_return(self):
        found = check("char *NuStrCpy(char *d, char *s);\n"
                      "void Caller(void) { NuStrCpy(0, 0); }\n")
        self.assertEqual([m["kinds"] for m in found], [["return"]])

    def test_float_vs_int_param(self):
        found = check("void NuMtxRotateY(struct numtx_s *m, float a);\n"
                      "void Caller(void) { NuMtxRotateY(0, 1.0f); }\n")
        self.assertEqual([m["kinds"] for m in found], [["params"]])

    def test_agreeing_prototype_is_not_reported(self):
        self.assertEqual(check("int NuSoundStopStream(int s);\n"
                               "void Caller(void) { NuSoundStopStream(4); }\n"), [])

    def test_only_matching_callers_are_not_reported(self):
        self.assertEqual(check("extern void NuSoundStopStream(int s);\n"
                               "void Matched(void) { NuSoundStopStream(4); }\n"), [])

    def test_unverified_definition_needs_all(self):
        using = ("int NuFsqrt(float x);\n"
                 "void Caller(void) { NuFsqrt(1.0f); }\n")
        self.assertEqual(check(using), [])
        self.assertEqual(len(check(using, trust_all=True)), 1)

    def test_soft_float_helpers_are_skipped(self):
        found = find_mismatches(
            {"src/sdk/fp.c": "double fptodp(float f) { return f; }\n",
             "src/game/use.c": "int fptodp(float f);\n"
                               "void Caller(void) { fptodp(1.0f); }\n"},
            {"src/sdk/fp.c": {"fptodp": "matching"}, "src/game/use.c": {"Caller": "asm"}})
        self.assertEqual(found, [])

    def test_comment_mentions_are_ignored(self):
        self.assertEqual(check("/* extern void NuSoundStopStream(int s); */\n"
                               "int NuSoundStopStream(int s);\n"
                               "void Caller(void) { NuSoundStopStream(4); }\n"), [])


if __name__ == "__main__":
    unittest.main()
