"""Candidate original object (translation unit) boundaries in the oracle code object.

Evidence used: wcc386 emits _TEXT paragraph-aligned (OMF align 3) and pads functions inside an object with
NOP forms (90, 8bc0, 8d4000); WLINK fills the gap between objects with 00. So a 16-aligned offset preceded by
1..15 zero bytes that follow a control transfer (ret / jmp) is a strong object start. Objects whose _TEXT
ends exactly on a 16-byte boundary leave no trace (missed boundaries), and hand-written assembly may differ.

    python tools/tumap.py [--oracle NAME]         write build/oracles/NAME/tumap.json and print a summary
    python tools/tumap.py [--oracle NAME] --list  one line per candidate object with its Rayverse functions
"""
import json
import sys
from pathlib import Path

import capstone

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import BUILD, dump_json  # noqa: E402
from image import Image  # noqa: E402

MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)


def ends_with_transfer(code, anchor, q):
    """Linear-sweep from anchor; True if an instruction ends exactly at q and it is ret/retn/iret/jmp."""
    last = None
    for ins in MD.disasm(code[anchor:q], anchor):
        last = ins
    return last is not None and last.address + last.size == q and last.mnemonic in ("ret", "retn", "iretd", "jmp")


def boundaries(code, sites, anchors=()):
    """anchors: known instruction starts (e.g. Rayverse function starts); the sweep for a candidate starts at the
    nearest anchor or accepted boundary before it, so zeros inside an instruction are rejected."""
    out = [0]
    anchors = sorted(anchors)
    for p in range(16, len(code), 16):
        q = p
        while q > p - 16 and code[q - 1] == 0:
            q -= 1
        npad = p - q
        if npad == 0 or npad == 16:
            continue
        if any(s in sites for s in range(q - 4, p)):
            continue                        # zero bytes are (part of) a relocated field
        start = max([out[-1]] + [a for a in anchors if a < q])
        if q - start > 0x4000:
            start = q - 0x4000              # bound the sweep; may lose sync in rare cases
        if ends_with_transfer(code, start, q):
            out.append(p)
    return out


def main(argv):
    argv = list(argv)
    name = None
    if "--oracle" in argv:
        i = argv.index("--oracle")
        name = argv[i + 1]
        del argv[i:i + 2]
    img = Image(name)
    sites = set(img.code_fixup_sites)
    xp = BUILD / "xref" / "xref.json"
    funcs = {}
    if img.name == "gog_v121_us" and xp.exists():
        funcs = {int(r["dos"].split(":")[1], 16): r for r in json.loads(xp.read_text())}
    anchors = set(funcs) | {img.entry[1]}
    lp = BUILD / "oracles" / img.name / "libsyms.json"
    if lp.exists():                          # exact library-member publics are instruction starts too
        anchors |= {int(v["addr"].split(":")[1], 16) for v in json.loads(lp.read_text()).values()}
    b = boundaries(img.code, sites, anchors)
    units = []
    for k, start in enumerate(b):
        end = b[k + 1] if k + 1 < len(b) else len(img.code)
        fs = sorted(o for o in funcs if start <= o < end)
        files = sorted({funcs[o]["rayverse"].split(":")[0] for o in fs})
        units.append({"start": f"1:{start:05x}", "end": f"1:{end:05x}", "size": end - start,
                      "functions": [funcs[o]["name"] for o in fs], "rayverse_files": files})
    dump_json(BUILD / "oracles" / img.name / "tumap.json", units)
    if "--list" in argv:
        for u in units:
            print(f"{u['start']}..{u['end']} {u['size']:6d}  {len(u['functions']):3d} fn  "
                  f"{','.join(u['rayverse_files'])}  {' '.join(u['functions'][:6])}"
                  + (" ..." if len(u["functions"]) > 6 else ""))
    nf = sum(len(u["functions"]) for u in units)
    multi = sum(1 for u in units if len(u["rayverse_files"]) > 1)
    print(f"# {len(units)} candidate objects; {nf} Rayverse functions placed; {multi} objects span >1 Rayverse file")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
