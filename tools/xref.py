"""Build the DOS <-> Rayverse <-> PS1 function cross-reference (generated, build/xref/).

Sources (read-only):
  external/rayverse/src/*.c     `//XXXXX` comment before a definition = PC v1.21 address (obj1 offset + 0x10000)
  external/rayman-ps1-decomp    funcs-u.txt (name = 0x80...), definitions in src/**/*.c, INCLUDE_ASM stubs
  config/xref_evidence.json     curated, committed evidence per DOS function (category/confidence/notes)

    python tools/xref.py                write build/xref/xref.json + xref.tsv, print summary
    python tools/xref.py NAME|ADDR      query (name substring, '1:off' or 'ida:addr')

Name joins (Rayverse name == PS1 name) are candidate links only; categories come from xref_evidence.json.
"""
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import BUILD, CONFIG, ROOT, dump_json, load_json  # noqa: E402

RV = ROOT / "external" / "rayverse" / "src"
PS1 = ROOT / "external" / "rayman-ps1-decomp"
ADDR_RE = re.compile(r"^//\s*([0-9A-Fa-f]{5,6})\b(.*)$")
DEF_RE = re.compile(r"^\s*(?:static\s+|inline\s+)*[A-Za-z_][\w\s\*]*?\b([A-Za-z_]\w*)\s*\(([^;]*)$")
PS1_ADDR_RE = re.compile(r"\b(80[0-9A-Fa-f]{6})\b")


def rayverse_functions():
    out = []
    for f in sorted(RV.glob("*.c")):
        lines = f.read_text(encoding="latin-1").splitlines()
        for i, ln in enumerate(lines):
            m = ADDR_RE.match(ln)
            if not m:
                continue
            for j in range(i + 1, min(i + 4, len(lines))):
                d = DEF_RE.match(lines[j])
                if d and d.group(1) not in ("if", "while", "for", "switch", "return"):
                    # body extent: until a line that is exactly '}'
                    k = j
                    while k < len(lines) and lines[k].rstrip() != "}":
                        k += 1
                    body = "\n".join(lines[j:k + 1])
                    out.append({"ida": int(m.group(1), 16), "name": d.group(1), "file": f.name, "line": j + 1,
                                "note": m.group(2).strip(), "stub": "stub" in body.lower() and "Not implemented" in body,
                                "ps1_addrs": sorted(set(PS1_ADDR_RE.findall(body)))})
                    break
    return out


def ps1_functions():
    funcs = {}
    for ln in (PS1 / "funcs-u.txt").read_text().splitlines():
        m = re.match(r"\s*(\w+)\s*=\s*0x([0-9a-fA-F]+);", ln)
        if m:
            funcs[m.group(1)] = {"addr": int(m.group(2), 16)}
    defs = {}
    asm = {}
    for f in sorted((PS1 / "src").rglob("*.c")):
        rel = f.relative_to(PS1).as_posix()
        for ln in f.read_text(encoding="latin-1").splitlines():
            m = re.match(r'INCLUDE_ASM\("[^"]*",\s*(\w+)\)', ln)
            if m:
                asm[m.group(1)] = rel
                continue
            d = DEF_RE.match(ln)
            if d and not ln.startswith((" ", "\t")) and ln.rstrip().endswith((")", "{")):
                defs.setdefault(d.group(1), rel)
    for name, v in funcs.items():
        v["file"] = defs.get(name) or asm.get(name)
        v["state"] = "C" if name in defs else ("ASM" if name in asm else "?")
    return funcs


def build():
    rv = rayverse_functions()
    ps1 = ps1_functions()
    ev = load_json(CONFIG / "xref_evidence.json")["functions"] if (CONFIG / "xref_evidence.json").exists() else {}
    rows = []
    for r in rv:
        off = r["ida"] - 0x10000
        p = ps1.get(r["name"])
        key = f"1:{off:05x}"
        e = ev.get(key, {})
        rows.append({
            "dos": key, "ida": f"{r['ida']:05X}", "name": r["name"],
            "rayverse": f"{r['file']}:{r['line']}", "rayverse_note": r["note"], "rayverse_stub": r["stub"],
            "ps1_addr": f"{p['addr']:08X}" if p else (r["ps1_addrs"][0] if r["ps1_addrs"] else ""),
            "ps1_file": (p or {}).get("file") or "", "ps1_state": (p or {}).get("state", ""),
            "category": e.get("category", "UNKNOWN"), "confidence": e.get("confidence", ""),
            "evidence": e.get("notes", ""),
        })
    rows.sort(key=lambda x: x["dos"])
    return rows


def main(argv):
    rows = build()
    out = BUILD / "xref"
    dump_json(out / "xref.json", rows)
    cols = ["dos", "ida", "name", "rayverse", "ps1_addr", "ps1_file", "ps1_state", "category", "confidence"]
    with open(out / "xref.tsv", "w", encoding="utf-8", newline="\n") as f:
        f.write("\t".join(cols) + "\n")
        for r in rows:
            f.write("\t".join(str(r[c]) for c in cols) + "\n")
    if len(argv) > 1:
        q = argv[1].lower()
        for r in rows:
            if q in r["name"].lower() or q == r["dos"] or q == "ida:" + r["ida"].lower():
                print("\t".join(str(r[c]) for c in cols))
        return 0
    named = sum(1 for r in rows if r["ps1_addr"])
    print(f"rayverse-annotated PC v1.21 functions: {len(rows)}; with PS1 name/address link: {named}; "
          f"rayverse stubs: {sum(r['rayverse_stub'] for r in rows)}")
    cats = {}
    for r in rows:
        cats[r["category"]] = cats.get(r["category"], 0) + 1
    print("categories:", cats)
    print(f"wrote {(out / 'xref.tsv').relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
