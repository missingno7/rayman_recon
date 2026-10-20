"""Annotated disassembly of the oracle code object.

    python tools/disasm.py [--oracle NAME] ADDR [LEN]     ADDR: name, '1:off', 'ida:addr' or bare obj1 hex offset

Without LEN, the extent runs to the next known function start (Rayverse annotations; Rayverse addresses are
v1.21-only, so for other oracles pass LEN). Fixup fields are marked with their target object:offset and name.
"""
import json
import sys
from pathlib import Path

import capstone

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import BUILD  # noqa: E402
from image import Image, parse_addr  # noqa: E402


def names():
    p = BUILD / "xref" / "xref.json"
    if not p.exists():
        return {}
    return {int(r["dos"].split(":")[1], 16): r["name"] for r in json.loads(p.read_text())}


def resolve(img, s, nm):
    for off, n in nm.items():
        if n == s:
            return 1, off
    return parse_addr(s)


def next_start(off, nm, limit):
    later = [o for o in nm if o > off]
    return min(later) if later else limit


def disasm(img, start, end, nm=None):
    nm = nm or {}
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.skipdata = True
    fx = img.fixups_in(1, start, end)
    lines = []
    for ins in md.disasm(img.code[start:end], start):
        notes = []
        for s in range(ins.address, ins.address + ins.size):
            if s in fx:
                _, to, toff = fx[s]
                tgt = f"{to}:{toff & 0xFFFFFFFF:x}" if toff >= 0 else f"{to}:-{-toff:x}"
                if to == 1 and toff in nm:
                    tgt += f" <{nm[toff]}>"
                notes.append(f"fix@+{s - ins.address}->{tgt}")
        if ins.mnemonic in ("call", "jmp") and ins.op_str.startswith("0x"):
            t = int(ins.op_str, 16)
            if t in nm:
                notes.append(f"<{nm[t]}>")
        lines.append(f"{img.fmt(1, ins.address)}  {ins.bytes.hex():20s} {ins.mnemonic:6s} {ins.op_str}"
                     + (f"   ; {' '.join(notes)}" if notes else ""))
    return lines


def main(argv):
    argv = list(argv)
    name = None
    if "--oracle" in argv:
        i = argv.index("--oracle")
        name = argv[i + 1]
        del argv[i:i + 2]
    img = Image(name)
    nm = names() if img.name == "gog_v121_us" else {}
    obj, off = resolve(img, argv[1], nm)
    end = off + int(argv[2], 16) if len(argv) > 2 else next_start(off, nm, len(img.code))
    if off in nm:
        print(f"; {nm[off]}  {img.fmt(1, off)} .. {img.fmt(1, end)}  ({end - off} bytes)")
    print("\n".join(disasm(img, off, end, nm)))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
