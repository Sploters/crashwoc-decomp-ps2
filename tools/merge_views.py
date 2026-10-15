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


def field_decls(chunk):
    """[(name, type, bits)] declared by one member declaration.

    The type keeps pointer stars and array dimensions (`float[4][4]`,
    `struct nuvec_s *`); bits is the bit-field width or None.
    """
    code = " ".join(strip_comments(drop_nested(chunk)).split())
    code = code.replace("{}", " ").strip().rstrip(";").strip()
    if not code:
        return []
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
            fields += field_decls(chunk)
        found.append((expr, name, text.count("\n", 0, m.start()) + 1, fields))
    return found


def probe_lines(views):
    """Probe initializer lines and what each word means."""
    lines, order = [], []
    for k, (expr, _name, _line, fields) in enumerate(views):
        lines.append(f"    sizeof({expr}),")
        order.append((k, None, "size"))
        for name, _ctype, bits in fields:
            if bits is not None:
                continue
            lines.append(f"    (int)&(({expr} *)0)->{name},")
            order.append((k, name, "offset"))
            lines.append(f"    sizeof((({expr} *)0)->{name}),")
            order.append((k, name, "size"))
    return lines, order


def measure(rel, text, views, version):
    """Per view: {"size": n, "fields": [(name, type, offset, size)]}, or an error string."""
    lines, order = probe_lines(views)
    source = text.rstrip() + f"\n\nint {PROBE_SYMBOL}[] = {{\n" + "\n".join(lines) + "\n    0\n};\n"
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
    for k, view in enumerate(views):
        for name, ctype, bits in view[3]:
            if bits is not None:
                result[k]["fields"].append((name, f"{ctype}:{bits}", None, None))
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
        for off, fsize, ctype in key:
            where = f"{off:#06x}+{fsize:#x}" if off is not None else "  bits  "
            out.append(f"      {where}  {ctype}")

    names = defaultdict(lambda: defaultdict(int))
    for v in views:
        for name, _t, off, _s in v["fields"]:
            names[off][name] += 1
    clashes = {off: n for off, n in names.items() if len(n) > 1 and off is not None}
    if clashes:
        out.append("names that differ at the same offset:")
        for off in sorted(o for o in clashes if o is not None):
            out.append(f"  {off:#06x}: " + ", ".join(
                f"{n} ({c})" for n, c in sorted(clashes[off].items(), key=lambda kv: -kv[1])))
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
