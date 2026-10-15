#!/usr/bin/env python3
"""Line up every local copy ("view") of a struct before it moves to a header.

Each unit carries its own copy of the structs it uses, and the copies drift:
other field names, other types at the same offset, padding where another
copy has a field, or a different size. Before a struct is unified, this tool
collects every view of it (and of the alias tags given on the command line),
measures each view with the matching compiler in the context of its own
file, and prints:

  * the sizes the views have;
  * the distinct layouts (offset, size, type per field), with the files that
    use each one;
  * per offset, every name used for the field there.

The measurement compiles a copy of each defining file with a probe array
appended (sizeof the struct, offsetof/sizeof every field) and reads the
values back from the generated assembly, so it sees exactly what the unit
sees. Nothing is written outside build/<version>/views/.

Needs the toolchain; on a host run it through the container:
  python tools/dispatch.py python tools/merge_views.py numtx_s NuMtx
  python tools/dispatch.py python tools/merge_views.py --json nuvec_s NuVec NuVec3
"""
import argparse
import json
import re
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import cc  # noqa: E402
from check_types import normalize_type  # noqa: E402
from probe_layout import (PROBE_SYMBOL, drop_nested, matching_brace,  # noqa: E402
                          members, read_words, strip_comments)

COMMENT = re.compile(r"/\*.*?\*/|//[^\n]*", re.S)
TYPE_WORDS = frozenset(("char", "short", "int", "long", "signed", "unsigned"))


def field_decls(chunk):
    """[(name, type, bits)] declared by one member declaration.

    The type keeps pointer stars and array dimensions (`float[4][4]`,
    `struct nuvec_s *`); bits is the bit-field width or None.
    """
    code = " ".join(strip_comments(drop_nested(chunk)).split())
    code = code.replace("{}", " ").strip().rstrip(";").strip()
    if not code:
        return []
    unnamed = re.fullmatch(r"([\w\s]+?)\s*:\s*(\d+)", code)
    if unnamed:
        words = unnamed.group(1).split()
        if len(words) == 1 or words[-1] in TYPE_WORDS:
            return [("", " ".join(words), int(unnamed.group(2)))]   # padding bits
    fnptr = re.match(r"(.*?)\(\s*\*\s*(\w+)\s*\)\s*\((.*)\)\s*$", code)
    if fnptr:
        return [(fnptr.group(2), f"{' '.join(fnptr.group(1).split())} (*)()", None)]
    parts = [p.strip() for p in code.split(",")]
    first = re.match(r"(.*?)([\s\*]*)([A-Za-z_]\w*)\s*((?:\[[^\]]*\]\s*)*)(?::\s*(\d+))?$",
                     parts[0])
    if not first or not first.group(1).strip():
        return []                                   # anonymous member
    base = " ".join(first.group(1).split())
    out = []
    for k, part in enumerate(parts):
        m = first if k == 0 else re.match(
            r"()([\s\*]*)([A-Za-z_]\w*)\s*((?:\[[^\]]*\]\s*)*)(?::\s*(\d+))?$", part)
        if not m:
            continue
        stars = m.group(2).count("*")
        dims = "".join(re.findall(r"\[[^\]]*\]", m.group(4) or ""))
        ctype = base + (" " + "*" * stars if stars else "") + dims.replace(" ", "")
        out.append((m.group(3), ctype, int(m.group(5)) if m.group(5) else None))
    return out


def find_views(text, names):
    """[(type expression, tag or typedef name, line, fields)] defined in text."""
    names = set(names)
    commented = [m.span() for m in COMMENT.finditer(text)]
    found = []
    for m in re.finditer(r"\b(struct|union)\s+(\w*)\s*\{", text):
        if any(lo <= m.start() < hi for lo, hi in commented):
            continue
        close = matching_brace(text, m.end() - 1)
        tag = m.group(2)
        alias = None
        pre = text[max(0, m.start() - 40):m.start()]
        if re.search(r"\btypedef\s*$", pre):
            after = re.match(r"\s*(\w+)\s*;", text[close:])
            alias = after.group(1) if after else None
        if tag in names:
            expr, name = f"{m.group(1)} {tag}", tag
        elif alias in names:
            expr, name = alias, alias
        else:
            continue
        body = text[m.end():close - 1]
        fields = []
        for _off, chunk in members(body):
            decls = field_decls(chunk)
            if not decls and strip_comments(chunk).strip(" \t\n;"):
                # anonymous member: positional initializers can't address
                # the fields after it, so bit-fields stay unmeasured
                fields.append((None, "anonymous", None))
            fields += decls
        found.append((expr, name, text.count("\n", 0, m.start()) + 1, fields))
    return found


def bit_probes(views):
    """Static instances that set one bit-field each to all ones.

    Returns (source lines, [(view index, field name, symbol)]). A view with
    an anonymous member is skipped (positional initializers would misalign).
    """
    lines, order = [], []
    for k, (expr, _name, _line, fields) in enumerate(views):
        if any(n is None for n, _t, _b in fields):
            continue
        for j, (name, _ctype, bits) in enumerate(fields):
            if bits is None or not name:
                continue
            inits = []
            for name2, ctype2, bits2 in fields[:j + 1]:
                if name2 == "":
                    continue                    # unnamed bits take no initializer
                if name2 == name:
                    inits.append("-1")
                elif bits2 is None and ("[" in ctype2 or re.match(r"(struct|union)\b", ctype2)
                                        and "*" not in ctype2):
                    inits.append("{0}")
                else:
                    inits.append("0")
            symbol = f"nu_bits_{k}_{j}"
            lines.append(f"{expr} {symbol} = {{ {', '.join(inits)} }};")
            order.append((k, name, symbol))
    return lines, order


def read_data(asm, symbol):
    """Bytes of an initialized object in ee-gcc assembly output."""
    data, inside = bytearray(), False
    sizes = {".word": 4, ".half": 2, ".short": 2, ".byte": 1}
    for raw in asm.splitlines():
        line = raw.split("#")[0].strip()
        if line == f"{symbol}:":
            inside = True
            continue
        if not inside:
            continue
        m = re.match(r"(\.word|\.half|\.short|\.byte)\s+(.+)$", line)
        if m:
            for value in m.group(2).split(","):
                data += (int(value.strip(), 0) & ((1 << (8 * sizes[m.group(1)])) - 1)
                         ).to_bytes(sizes[m.group(1)], "little")
            continue
        m = re.match(r"\.(?:space|zero)\s+(\d+)$", line)
        if m:
            data += bytes(int(m.group(1)))
            continue
        if line.endswith(":") or (line and not line.startswith((".", "#"))):
            break
    return bytes(data)


def set_bits(data):
    """(first bit, count) of the set bits in little-endian data, or None."""
    value = int.from_bytes(data, "little")
    if not value:
        return None
    first = (value & -value).bit_length() - 1
    return first, bin(value).count("1")


def probe_lines(views):
    """Probe initializer lines and what each word means."""
    lines, order = [], []
    for k, (expr, _name, _line, fields) in enumerate(views):
        lines.append(f"    sizeof({expr}),")
        order.append((k, None, "size"))
        for name, _ctype, bits in fields:
            if bits is not None or name is None:
                continue
            lines.append(f"    (int)&(({expr} *)0)->{name},")
            order.append((k, name, "offset"))
            lines.append(f"    sizeof((({expr} *)0)->{name}),")
            order.append((k, name, "size"))
    return lines, order


def measure(rel, text, views, version):
    """Per view: {"size": n, "fields": [(name, type, offset, size)]}, or an error string.

    A bit-field's offset is its byte offset and its size is "bits S+W" (first
    bit within the struct, width); unmeasurable ones have None.
    """
    lines, order = probe_lines(views)
    bit_lines, bit_order = bit_probes(views)
    source = (text.rstrip() + f"\n\nint {PROBE_SYMBOL}[] = {{\n" + "\n".join(lines)
              + "\n    0\n};\n" + "".join(f"{l}\n" for l in bit_lines))
    work = ROOT / "build" / version / "views"
    work.mkdir(parents=True, exist_ok=True)
    src = work / (re.sub(r"[^A-Za-z0-9_]", "_", rel) + ".c")
    src.write_text(source)
    out_s = src.with_suffix(".s")
    try:
        cc.compile_s(src, out_s, profile="default", version=version)
    except subprocess.CalledProcessError as exc:
        return (exc.stderr or "compile failed").strip().splitlines()[-1]
    words = read_words(out_s.read_text())
    if len(words) != len(order) + 1:
        return f"probe returned {len(words)} values, expected {len(order) + 1}"
    result = [{"size": None, "fields": []} for _ in views]
    pending = {}
    for (k, name, what), value in zip(order, words):
        if name is None:
            result[k]["size"] = value
        elif what == "offset":
            pending[(k, name)] = value
        else:
            ctype = next(t for n, t, _b in views[k][3] if n == name)
            result[k]["fields"].append((name, ctype, pending[(k, name)], value))
    asm = out_s.read_text()
    measured_bits = {}
    for k, name, symbol in bit_order:
        hit = set_bits(read_data(asm, symbol))
        if hit:
            measured_bits[(k, name)] = hit
    for k, view in enumerate(views):
        for name, ctype, bits in view[3]:
            if bits is None or not name:
                continue
            hit = measured_bits.get((k, name))
            if hit:
                first, width = hit
                result[k]["fields"].append(
                    (name, f"{ctype}:{bits}", first // 8, f"bits {first}+{width}"))
            else:
                result[k]["fields"].append((name, f"{ctype}:{bits}", None, None))
        result[k]["fields"].sort(key=lambda f: (f[2] is None, f[2] or 0,
                                                int(f[3].split()[1].split("+")[0])
                                                if isinstance(f[3], str) else 0))
    return result


def spelling(ctype):
    """One spelling per type (`f32` = `float`, `s32` = `int`), dimensions kept."""
    base, _, dims = ctype.partition("[")
    return normalize_type(base) + (f"[{dims}" if dims else "")


def report(views):
    """Human-readable summary lines for measured views.

    views: [{"file", "line", "name", "size", "fields"}].
    """
    out = []
    sizes = defaultdict(list)
    for v in views:
        sizes[v["size"]].append(v)
    out.append("sizes: " + ", ".join(
        f"{s:#x} ({len(vs)})" if s is not None else f"? ({len(vs)})"
        for s, vs in sorted(sizes.items(), key=lambda kv: -len(kv[1]))))

    layouts = defaultdict(list)
    for v in views:
        key = tuple((off, size, spelling(t)) for _n, t, off, size in v["fields"])
        layouts[(v["size"], key)].append(v)
    out.append(f"{len(layouts)} distinct layout(s):")
    for (size, key), vs in sorted(layouts.items(), key=lambda kv: -len(kv[1])):
        files = ", ".join(sorted({v["file"] for v in vs}))
        out.append(f"  [{len(vs)}] size {size:#x}: {files}" if size is not None
                   else f"  [{len(vs)}] {files}")
        for (off, fsize, ctype), (fname, *_rest) in zip(key, vs[0]["fields"]):
            ctype = f"{ctype} {fname}"
            if off is None:
                where = "  ?     "
            elif isinstance(fsize, str):
                where = f"{off:#06x} {fsize}"
            else:
                where = f"{off:#06x}+{fsize:#x}"
            out.append(f"      {where}  {ctype}")

    names = defaultdict(lambda: defaultdict(int))
    for v in views:
        for name, _t, off, fsize in v["fields"]:
            if off is not None:
                names[(off, fsize if isinstance(fsize, str) else "")][name] += 1
    clashes = {off: n for off, n in names.items() if len(n) > 1 and off is not None}
    if clashes:
        out.append("names that differ at the same offset:")
        for off, bits in sorted(clashes):
            out.append(f"  {off:#06x}{' ' + bits if bits else ''}: " + ", ".join(
                f"{n} ({c})" for n, c in sorted(clashes[(off, bits)].items(),
                                                key=lambda kv: -kv[1])))
    return out


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("names", nargs="+", help="struct tags / typedef names of one type")
    parser.add_argument("--version", default="pal103")
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()

    if not cc.compiler_available("default", args.version):
        print("matching compiler not found; run through the container: "
              "python tools/dispatch.py python tools/merge_views.py ...", file=sys.stderr)
        return 2

    paths = [p for p in sorted((ROOT / "src").rglob("*.c"))
             if not p.relative_to(ROOT).as_posix().startswith("src/sdk/")]
    paths += cc.header_files()
    measured, failures = [], []
    for path in paths:
        rel = path.relative_to(ROOT).as_posix()
        text = path.read_text(errors="replace")
        views = find_views(text, args.names)
        if not views:
            continue
        if rel.startswith("include/"):
            # Probe a header through a unit that includes it.
            text = f'#include "{path.relative_to(ROOT / "include").as_posix()}"\n'
        result = measure(rel, text, views, args.version)
        if isinstance(result, str):
            failures.append(f"{rel}: {result}")
            continue
        for (expr, name, line, _f), r in zip(views, result):
            measured.append({"file": rel, "line": line, "name": name,
                             "size": r["size"], "fields": r["fields"]})

    if args.json:
        print(json.dumps({"views": measured, "failures": failures}, indent=1))
        return 0
    print(f"{' / '.join(args.names)}: {len(measured)} view(s) in "
          f"{len({v['file'] for v in measured})} file(s)")
    for line in report(measured):
        print(line)
    for line in failures:
        print(f"not measured: {line}")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
