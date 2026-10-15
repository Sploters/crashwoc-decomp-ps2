"""Unit tests for tools/probe_layout.py (pure Python, no compiler)."""
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from probe_layout import (PROBE_SYMBOL, compare, parse_header,  # noqa: E402
                          probe_source, read_words)

HEADER = """\
struct nuvec_s {
    float x;
    float y;
    float z;
}; /* 0xC */

/* Comment that mentions struct fake_s { int a; }; must be ignored. */

struct creature_s {
    int id;                       /* 0x0 */
    struct nuvec_s pos;           /* 0x4 */
    struct numtx_s mtx[2][16];    /* 0x10 (model-major,
                                   * wraps over two lines) */
    struct nuvec_s mom[2][16];    /* 0x810 */
    union {
        float f;                  /* 0x0 inside the union */
        int i;
    } u;                          /* 0x990 */
    unsigned int flag : 1;        /* 0x994:0 */
    void (*cb)(int a, int b);     /* 0x998 */
    float a, b;                   /* 0x99C */
};

// Size: 0x8, DWARF: 0x1234
struct pair_s
{
    int first; // Offset: 0x0, DWARF: 0x1250
    int second; // Offset: 0x4, DWARF: 0x1270
};

union nx_u { // 0x4
	/* 0x0c */ float f32;
	/* 0x0c */ unsigned char u8[4];
};

typedef struct {
    short x; /* 0x0 */
    short y; /* 0x2 */
} point_t; /* 0x4 */
"""


def by_type(structs):
    return {s["type"]: s for s in structs}


class TestParseHeader(unittest.TestCase):
    def setUp(self):
        self.structs = by_type(parse_header(HEADER))

    def test_struct_list(self):
        self.assertEqual(sorted(self.structs), [
            "point_t", "struct creature_s", "struct nuvec_s",
            "struct pair_s", "union nx_u"])

    def test_trailing_size(self):
        self.assertEqual(self.structs["struct nuvec_s"]["size"], (0xC, 5))
        self.assertEqual(self.structs["struct nuvec_s"]["fields"], [])

    def test_trailing_offsets(self):
        fields = self.structs["struct creature_s"]["fields"]
        self.assertEqual([(n, o) for n, o, _l in fields], [
            ("id", 0x0), ("pos", 0x4), ("mtx", 0x10), ("mom", 0x810),
            ("u", 0x990), ("cb", 0x998), ("a", 0x99C)])
        # A comment that wraps onto a second line stays with its member.
        self.assertEqual(fields[3][2], 14)
        self.assertIsNone(self.structs["struct creature_s"]["size"])

    def test_dwarf_style(self):
        pair = self.structs["struct pair_s"]
        self.assertEqual(pair["size"], (0x8, 24))
        self.assertEqual([(n, o) for n, o, _l in pair["fields"]],
                         [("first", 0), ("second", 4)])

    def test_leading_style_and_open_brace_size(self):
        nx = self.structs["union nx_u"]
        self.assertEqual(nx["size"][0], 0x4)
        self.assertEqual([(n, o) for n, o, _l in nx["fields"]],
                         [("f32", 0xC), ("u8", 0xC)])

    def test_typedef_struct(self):
        point = self.structs["point_t"]
        self.assertEqual(point["size"][0], 0x4)
        self.assertEqual([(n, o) for n, o, _l in point["fields"]],
                         [("x", 0), ("y", 2)])


class TestProbe(unittest.TestCase):
    def test_source_and_compare(self):
        structs = [s for s in parse_header(HEADER) if s["type"] == "union nx_u"]
        source, order = probe_source("nugscn.h", structs)
        self.assertIn('#include "nugscn.h"', source)
        self.assertIn("sizeof(union nx_u)", source)
        self.assertIn("(int)&((union nx_u *)0)->f32", source)
        self.assertEqual(order, [(0, None), (0, "f32"), (0, "u8")])

        asm = (f"\t.globl\t{PROBE_SYMBOL}\n{PROBE_SYMBOL}:\n"
               "\t.word\t4\n\t.word\t0\n\t.word\t0\n\t.word\t0\n\t.text\n")
        words = read_words(asm)
        self.assertEqual(words, [4, 0, 0, 0])
        bad, checked = compare("include/nugscn.h", structs, order, words)
        self.assertEqual(checked, 3)
        self.assertEqual(len(bad), 2)
        self.assertIn("union nx_u.f32 comment 0xc, compiler 0x0", bad[0])

    def test_short_output_is_an_error(self):
        structs = [s for s in parse_header(HEADER) if s["type"] == "point_t"]
        _src, order = probe_source("x.h", structs)
        bad, checked = compare("include/x.h", structs, order, [4])
        self.assertEqual(checked, 0)
        self.assertIn("probe returned 1 values", bad[0])


if __name__ == "__main__":
    unittest.main()
