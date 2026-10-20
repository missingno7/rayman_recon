"""Android Rayman Classic symbol table (external/rayman-from-scratch: `objdump -TC libSparkApp.so`, x86).

    python tools/android.py                 summary; writes build/xref/android.json
    python tools/android.py NAME...         show symbols (exact names)
    python tools/android.py --order N1 N2.. Android address order of the given functions/objects

The .so is a different build (Android, x86 GCC/Clang, C++ wrapper code); only names, data sizes and sections
(.data = initialised, .bss = uninitialised, .rodata = const) are meaningful for DOS. Addresses/sizes of
functions are NOT comparable with DOS code.
"""
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import BUILD, ROOT, dump_json  # noqa: E402

SRC = ROOT / "external" / "rayman-from-scratch"
LINE = re.compile(r"^([0-9a-f]{8}) (.{7}) (\S+)\s+([0-9a-f]{8})\s+(.+)$")


def load(rev="1.txt"):
    out = {}
    for ln in (SRC / rev).read_text(encoding="latin-1").splitlines():
        m = LINE.match(ln)
        if not m:
            continue
        addr, flags, section, size, name = m.groups()
        kind = "F" if "F" in flags else ("O" if "O" in flags else "?")
        if section == "*UND*" or "(" in name or "::" in name:
            continue
        out[name.strip()] = {"addr": int(addr, 16), "size": int(size, 16), "section": section, "kind": kind}
    return out


def main(argv):
    syms = load()
    dump_json(BUILD / "xref" / "android.json", syms)
    if len(argv) > 1 and argv[1] == "--order":
        rows = sorted((syms[n]["addr"], n) for n in argv[2:] if n in syms)
        for a, n in rows:
            s = syms[n]
            print(f"{a:08x} {s['section']:8s} {s['size']:6x} {n}")
        missing = [n for n in argv[2:] if n not in syms]
        if missing:
            print("not in Android table:", " ".join(missing))
        return 0
    if len(argv) > 1:
        for n in argv[1:]:
            print(n, syms.get(n))
        return 0
    from collections import Counter
    c = Counter((s["kind"], s["section"]) for s in syms.values())
    print(f"{len(syms)} named symbols: " + ", ".join(f"{k[0]}/{k[1]} {v}" for k, v in c.most_common(8)))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
