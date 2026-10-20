"""Locate Watcom library members inside the oracle code object and compare toolchain releases.

For every CODE segment of every module in the given .LIB/.OBJ files, search obj1 for the segment bytes with
the module's fixup fields masked. A hit is then classified:
  exact+sites  every unmasked byte equal AND every module fixup field coincides with an oracle fixup site
               (offset fixups) or is a rel32 field (self-relative, no oracle fixup expected) and the oracle
               has no extra fixup inside the segment extent;
  masked       bytes equal modulo fixup fields, fixup site sets differ.
Hits are library-identity evidence only; targets are not verified here.

    python tools/libscan.py [--oracle NAME] LIB [LIB...]            print hits (sorted by address)
    python tools/libscan.py [--oracle NAME] --compare A.LIB B.LIB   members found with only one release
    python tools/libscan.py [--oracle NAME] --symbols LIB...        write build/oracles/NAME/libsyms.json:
        public -> address for members found exactly once with class exact+sites (library-identity bindings)
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import omf  # noqa: E402
from common import BUILD, dump_json  # noqa: E402
from image import Image  # noqa: E402


def masked_pattern(mod, si):
    seg = mod.segments[si]
    mask = bytearray(seg.covered)
    sites = []
    for f in mod.fixups:
        if f.seg == si:
            sites.append((f.off, f.size(), f.self_rel))
            for k in range(f.size()):
                if f.off + k < len(mask):
                    mask[f.off + k] = 0
    return bytes(seg.data), bytes(mask), sites


def find_masked(hay, pat, mask, min_anchor=8):
    n = len(pat)
    best, cur, start = (0, 0), 0, 0
    for i, m in enumerate(mask):
        if m:
            if cur == 0:
                start = i
            cur += 1
            if cur > best[1]:
                best = (start, cur)
        else:
            cur = 0
    a0, alen = best
    if alen < min_anchor:
        return []
    anchor = pat[a0:a0 + alen]
    hits, p = [], hay.find(anchor)
    while p != -1:
        base = p - a0
        if base >= 0 and base + n <= len(hay) and all(not mask[i] or hay[base + i] == pat[i] for i in range(n)):
            hits.append(base)
        p = hay.find(anchor, p + 1)
    return hits


def scan(img, libs):
    code = img.code
    sites = set(img.code_fixup_sites)
    out = []
    for lib in libs:
        for m in omf.load(lib):
            for si, seg in enumerate(m.segments):
                if not seg or seg.cls.upper() != "CODE" or seg.size == 0:
                    continue
                pat, mask, fx = masked_pattern(m, si)
                for base in find_masked(code, pat, mask):
                    want = {base + o for o, sz, rel in fx if not rel and sz == 4}
                    have = {s for s in sites if base <= s < base + seg.size}
                    rel_ok = all(rel for o, sz, rel in fx if sz != 4 or rel) if fx else True
                    cls = "exact+sites" if want == have and rel_ok else "masked"
                    out.append({"lib": lib, "module": m.name, "seg": seg.name, "size": seg.size, "at": base,
                                "class": cls, "publics": [n for n, s, o, _ in m.publics if s == si],
                                "public_offsets": {n: base + o for n, s, o, _ in m.publics if s == si}})
    return out


def main(argv):
    argv = list(argv)
    name = None
    if "--oracle" in argv:
        i = argv.index("--oracle")
        name = argv[i + 1]
        del argv[i:i + 2]
    img = Image(name)
    if argv[1] == "--compare":
        a, b = argv[2], argv[3]
        ra, rb = scan(img, [a]), scan(img, [b])
        ka = {(r["module"], r["at"], r["class"]) for r in ra}
        kb = {(r["module"], r["at"], r["class"]) for r in rb}
        print(f"{a}: {len(ka)} hits ({sum(r['size'] for r in ra)} bytes); {b}: {len(kb)} hits "
              f"({sum(r['size'] for r in rb)} bytes)")
        for k in sorted(ka - kb, key=lambda x: x[1]):
            print(f"  only A: {k[1]:05x} {k[0]} {k[2]}")
        for k in sorted(kb - ka, key=lambda x: x[1]):
            print(f"  only B: {k[1]:05x} {k[0]} {k[2]}")
        return 0
    if argv[1] == "--symbols":
        res = scan(img, argv[2:])
        count = {}
        for r in res:
            count[(r["lib"], r["module"])] = count.get((r["lib"], r["module"]), 0) + 1
        syms = {}
        for r in res:
            if r["class"] != "exact+sites" or count[(r["lib"], r["module"])] != 1:
                continue
            for n, a in r["public_offsets"].items():
                syms[n] = {"addr": f"1:{a:05x}", "evidence": f"LIBRARY_MEMBER {Path(r['lib']).as_posix()}:{r['module']}"}
        out = BUILD / "oracles" / img.name / "libsyms.json"
        dump_json(out, syms)
        print(f"{len(syms)} library symbols -> {out}")
        return 0
    res = scan(img, argv[1:])
    for r in sorted(res, key=lambda r: r["at"]):
        print(f"1:{r['at']:05x} +{r['size']:<5x} {r['class']:11s} {Path(r['lib']).name}:{r['module']} "
              f"{' '.join(r['publics'][:3])}")
    print(f"# {len(res)} hits, {sum(r['size'] for r in res)} bytes, "
          f"{sum(1 for r in res if r['class'] == 'exact+sites')} exact+sites")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
