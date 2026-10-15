"""Unit tests for tools/check_codegen.py (pure Python, no game files).

The objects are tiny little-endian ELF32 relocatables assembled in-test.
"""
import struct
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from check_codegen import compare, fingerprint, ninja_objects, read_elf  # noqa: E402

SHT_PROGBITS, SHT_SYMTAB, SHT_STRTAB, SHT_NOBITS, SHT_REL = 1, 2, 3, 8, 9
ALLOC_EXEC, ALLOC_WRITE = 0x6, 0x3


def make_object(text, functions, relocs=(), mdebug=b"", bss_size=0x10):
    """ELF32 LE object with .text/.bss/.mdebug/.rel.text/.symtab.

    functions: [(name, offset, size)]; relocs: [(offset, type, symbol name)]
    against undefined symbols.
    """
    shstr = b"\0.text\0.bss\0.mdebug\0.rel.text\0.symtab\0.strtab\0.shstrtab\0"

    def sh_name(name):
        return shstr.index(name.encode() + b"\0")

    strtab = b"\0"
    symbols = [struct.pack("<IIIBBH", 0, 0, 0, 0, 0, 0)]
    names = {}
    for name, offset, size in functions:
        names[name] = len(symbols)
        symbols.append(struct.pack("<IIIBBH", len(strtab), offset, size,
                                   (1 << 4) | 2, 0, 1))      # GLOBAL FUNC, .text
        strtab += name.encode() + b"\0"
    for _off, _type, target in relocs:
        if target not in names:
            names[target] = len(symbols)
            symbols.append(struct.pack("<IIIBBH", len(strtab), 0, 0,
                                       (1 << 4) | 0, 0, 0))  # GLOBAL NOTYPE, UND
            strtab += target.encode() + b"\0"
    symtab = b"".join(symbols)
    rel = b"".join(struct.pack("<II", off, (names[t] << 8) | rtype)
                   for off, rtype, t in relocs)

    blobs = [text, b"", mdebug, rel, symtab, strtab, shstr]
    offsets, pos = [], 0x34
    for blob in blobs:
        offsets.append(pos)
        pos += len(blob)
    shoff = (pos + 3) & ~3
    # (name, type, flags, offset, size, link, info, entsize)
    headers = [
        (0, 0, 0, 0, 0, 0, 0, 0),
        (sh_name(".text"), SHT_PROGBITS, ALLOC_EXEC, offsets[0], len(text), 0, 0, 0),
        (sh_name(".bss"), SHT_NOBITS, ALLOC_WRITE, offsets[1], bss_size, 0, 0, 0),
        (sh_name(".mdebug"), SHT_PROGBITS, 0, offsets[2], len(mdebug), 0, 0, 0),
        (sh_name(".rel.text"), SHT_REL, 0, offsets[3], len(rel), 5, 1, 8),
        (sh_name(".symtab"), SHT_SYMTAB, 0, offsets[4], len(symtab), 6, 1, 16),
        (sh_name(".strtab"), SHT_STRTAB, 0, offsets[5], len(strtab), 0, 0, 0),
        (sh_name(".shstrtab"), SHT_STRTAB, 0, offsets[6], len(shstr), 0, 0, 0),
    ]
    ident = b"\x7fELF\x01\x01\x01" + b"\0" * 9
    header = ident + struct.pack("<HHIIIIIHHHHHH", 1, 8, 1, 0, 0, shoff, 0,
                                 0x34, 0, 0, 40, len(headers), 7)
    body = b"".join(blobs)
    out = header + body + b"\0" * (shoff - pos)
    for name, stype, flags, off, size, link, info, entsize in headers:
        out += struct.pack("<IIIIIIIIII", name, stype, flags, 0, off, size,
                           link, info, 4, entsize)
    return out


TEXT = bytes(range(32))
FUNCS = [("Alpha", 0, 16), ("Beta", 16, 16)]
RELOCS = [(4, 4, "NuVecAdd"), (20, 4, "NuVecSub")]


class TestReadElf(unittest.TestCase):
    def test_parse(self):
        sections, symbols, relocations = read_elf(make_object(TEXT, FUNCS, RELOCS))
        self.assertEqual([s["name"] for s in sections][1:4], [".text", ".bss", ".mdebug"])
        self.assertEqual(sections[2]["size"], 0x10)
        self.assertEqual(sections[2]["data"], b"")
        self.assertIn("Beta", [s["name"] for s in symbols])
        self.assertEqual(len(relocations[1]), 2)

    def test_rejects_non_elf(self):
        with self.assertRaises(ValueError):
            read_elf(b"not an object at all" * 4)


class TestFingerprint(unittest.TestCase):
    def setUp(self):
        self.base = fingerprint(make_object(TEXT, FUNCS, RELOCS, mdebug=b"line 10"))

    def test_non_alloc_sections_ignored(self):
        shifted = fingerprint(make_object(TEXT, FUNCS, RELOCS, mdebug=b"line 13 moved"))
        self.assertEqual(shifted, self.base)
        self.assertNotIn(".mdebug", self.base["sections"])

    def test_function_bytes(self):
        text = bytearray(TEXT)
        text[20] ^= 0xFF
        lines = compare({"x.o": self.base},
                        {"x.o": fingerprint(make_object(bytes(text), FUNCS, RELOCS))})
        self.assertEqual(len(lines), 1)
        self.assertIn("functions changed: Beta", lines[0])
        self.assertNotIn("Alpha", lines[0])

    def test_relocation_target(self):
        relocs = [(4, 4, "NuVecAdd"), (20, 4, "NuVecMul")]
        lines = compare({"x.o": self.base},
                        {"x.o": fingerprint(make_object(TEXT, FUNCS, relocs))})
        self.assertIn("functions changed: Beta", lines[0])
        self.assertIn("relocs differ", lines[0])

    def test_bss_size(self):
        lines = compare({"x.o": self.base},
                        {"x.o": fingerprint(make_object(TEXT, FUNCS, RELOCS, bss_size=0x20))})
        self.assertIn("section .bss size 0x10 -> 0x20", lines[0])

    def test_added_function(self):
        funcs = [("Alpha", 0, 16), ("Beta", 16, 8), ("Gamma", 24, 8)]
        lines = compare({"x.o": self.base},
                        {"x.o": fingerprint(make_object(TEXT, funcs, RELOCS))})
        self.assertIn("functions added: Gamma", lines[0])

    def test_missing_and_identical(self):
        self.assertEqual(compare({"x.o": self.base}, {"x.o": self.base}), [])
        lines = compare({"x.o": self.base}, {"x.o": {"missing": True}})
        self.assertEqual(lines, ["x.o: missing"])
        self.assertEqual(compare({}, {"y.o": self.base}), ["y.o: added"])


class TestNinjaObjects(unittest.TestCase):
    def test_phony_sets(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "build.ninja"
            path.write_text(
                "build build/pal103/current/a.o: cc src/a.c\n"
                "build current: phony build/pal103/current/a.o build/pal103/current/b.o\n"
                "build matching: phony build/pal103/matching/a.o\n"
                "build check: phony current verify-loaded\n")
            found = ninja_objects(path)
        self.assertEqual(found["current"], ["build/pal103/current/a.o",
                                            "build/pal103/current/b.o"])
        self.assertEqual(found["matching"], ["build/pal103/matching/a.o"])
        self.assertNotIn("check", found)


if __name__ == "__main__":
    unittest.main()
