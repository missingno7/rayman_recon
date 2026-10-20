"""Android Rayman Classic symbol table (external/rayman-from-scratch: `objdump -TC libSparkApp.so`, x86).

    python tools/android.py                 summary; writes build/xref/android.json
    python tools/android.py NAME...         show symbols (exact names)
    python tools/android.py --order N1 N2.. Android address order of the given functions/objects
    python tools/android.py --dis NAME      x86 disassembly of an Android function (semantic reference only;
                                            needs the APK in assets/Android, read as data, never executed)

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


def disassemble(name):
    """Uses the APK's own dynamic symbols (the text dumps are a different Android build)."""
    import capstone
    import androiddata
    b = androiddata.extract_so()
    secs = androiddata.elf_sections(b)
    funcs = {f["name"]: f for f in androiddata.dynsyms(b, secs, stype=2)}
    if name not in funcs:
        raise SystemExit(f"{name}: no function of that name in the APK's x86 library")
    f = funcs[name]
    sec = f["sec"]
    code = b[sec["offset"] + f["addr"] - sec["addr"]:][:f["size"]]
    by_addr = {g["addr"]: n for n, g in funcs.items()}
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    print(f"; Android x86 {name} @{f['addr']:08x} size {f['size']:#x} (different compiler; semantics only)")
    for ins in md.disasm(code, f["addr"]):
        note = ""
        if ins.mnemonic in ("call", "jmp") and ins.op_str.startswith("0x"):
            note = "  ; " + by_addr.get(int(ins.op_str, 16), "")
        print(f"  {ins.address:08x}  {ins.mnemonic:6s} {ins.op_str}{note}")


def main(argv):
    if len(argv) > 2 and argv[1] == "--dis":
        disassemble(argv[2])
        return 0
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
