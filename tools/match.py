"""Strict function verifier: compiled Watcom OMF object vs. the pinned DOS oracle.

    python tools/match.py [--oracle NAME] [--profile P | --install I FLAGS...] SRC.c [FUNC...] [--json OUT]
    python tools/match.py [--oracle NAME] --obj OBJ [FUNC...]
    --symbols EXTRA.json   overlay of hypothesised bindings (never conflicts with config/symbols.json)

For every public function of the object's CODE segment (or only FUNC...):
  placement   config/symbols.json binds the function name to an oracle address (the claim under test);
              intra-segment references of a function resolve through that function's own placement. Whether all
              publics of the object imply one contiguous _TEXT base is reported separately ("object _TEXT layout")
              and is a unit-level property, not part of a function's status;
  extent      candidate: public offset .. next public / segment end (includes intra-TU alignment padding);
  bytes       every byte outside fixup fields must equal the oracle;
  fixup sites candidate absolute (off32) fixup sites == oracle PMW1 fixup sites inside the extent;
  references  rel32: oracle target decoded from original bytes == bound address of the candidate target;
              off32: oracle (object, offset) == bound address of the candidate target + displacement.
              Targets resolve through (a) publics of this object placed consistently in the same segment,
              (b) config/symbols.json game bindings, (c) build/oracles/<o>/libsyms.json library bindings,
              (d) the object's own initialised data segments, located from the first reference and accepted
              only if their entire contents equal the oracle bytes there and every reference agrees.
Status: STRICT (all of the above), CODE_BYTES_MATCH (bytes + sites equal, some reference unverified),
NEAR_MATCH (differences, reported), UNPLACED (no binding). Masked equality is never reported as STRICT.
"""
import json
import os
import struct
import sys
from pathlib import Path

import capstone

sys.path.insert(0, str(Path(__file__).resolve().parent))
import omf  # noqa: E402
import wcc  # noqa: E402
from common import BUILD, CONFIG, dump_json, load_json  # noqa: E402
from image import Image, parse_addr  # noqa: E402

MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)


def demangle(n):
    """Watcom __watcall: functions 'name_', data '_name'. Returns the plain C name."""
    if n.endswith("_") and not n.startswith("_"):
        return n[:-1]
    if n.startswith("_") and not n.endswith("_"):
        return n[1:]
    return n


class Bindings:
    extra = None        # optional overlay file (worker hypotheses); same layout as config/symbols.json

    def __init__(self, img):
        self.game = {}
        for p in [CONFIG / "symbols.json"] + ([Path(self.extra)] if self.extra else []):
            if not p.exists():
                continue
            for kind in ("functions", "data"):
                for name, ent in load_json(p).get(img.name, {}).get(kind, {}).items():
                    a = parse_addr(ent["addr"])
                    if name in self.game and self.game[name] != a:
                        raise SystemExit(f"binding conflict for {name}: {self.game[name]} vs {a} ({p})")
                    self.game[name] = a
        self.lib = {}
        lp = BUILD / "oracles" / img.name / "libsyms.json"
        if lp.exists():
            for name, ent in load_json(lp).items():
                self.lib[name] = parse_addr(ent["addr"])

    def lookup(self, raw):
        """raw = OMF symbol name. Returns ((obj, off), source) or (None, None)."""
        if raw in self.lib:
            return self.lib[raw], "lib"
        c = demangle(raw)
        if c in self.game:
            return self.game[c], "symbols.json"
        if raw in self.game:
            return self.game[raw], "symbols.json"
        return None, None


def dis1(code, off, base):
    for ins in MD.disasm(code[off:off + 16], base + off):
        return ins
    return None


def side_by_side(cand, orig, start, base, n=10):
    """Instruction listing from `start` (candidate-relative) for both streams."""
    out = []
    pc = po = start
    for _ in range(n):
        a = dis1(cand, pc, 0) if pc < len(cand) else None
        b = dis1(orig, po, 0) if po < len(orig) else None
        sa = f"{a.bytes.hex():14s} {a.mnemonic} {a.op_str}" if a else "-"
        sb = f"{b.bytes.hex():14s} {b.mnemonic} {b.op_str}" if b else "-"
        mark = "  " if a and b and a.bytes == b.bytes else "!="
        out.append(f"   +{pc:04x} {sa:44.44s} {mark} 1:{base + po:05x} {sb}")
        pc += a.size if a else 1
        po += b.size if b else 1
    return out


def segment_report(img, mod, seg_base, binds):
    """Status of the object's data segments: VERIFIED (whole contents equal the oracle at one consistent base),
    MISMATCH, UNPLACED (no reference or binding fixes a base), BSS (contents not checkable here)."""
    out = []
    for si, seg in enumerate(mod.segments):
        if not seg or seg.cls.upper() == "CODE" or seg.size == 0 or seg.name.startswith("$$"):
            continue
        ent = {"segment": seg.name, "class": seg.cls, "size": seg.size}
        bases = set()
        if si in seg_base:
            bases.add(seg_base[si])
        for n, s2, o, _ in mod.publics:          # defined data bound in config/symbols.json
            if s2 == si:
                a, _src = binds.lookup(n)
                if a:
                    bases.add((a[0], a[1] - o))
        if seg.cls.upper() == "BSS":
            ent["status"] = "BSS"
        elif not bases:
            ent["status"] = "UNPLACED"
        elif len(bases) > 1:
            ent["status"] = "MISMATCH"
            ent["problem"] = "inconsistent bases " + ", ".join(f"{o}:{b:x}" for o, b in sorted(bases))
        else:
            o, b = next(iter(bases))
            ent["oracle"] = f"{o}:{b:05x}"
            seg_fx = [f for f in mod.fixups if f.seg == si]
            covered = bytes(seg.covered) == b"" * seg.size
            equal = img.obj[o][b:b + seg.size] == bytes(seg.data)
            ent["status"] = "VERIFIED" if covered and equal and not seg_fx else "MISMATCH"
            if seg_fx:
                ent["problem"] = f"{len(seg_fx)} fixups inside the segment (data relocations not verified yet)"
            elif not equal:
                diff = next(i for i in range(seg.size) if img.obj[o][b + i] != seg.data[i])
                ent["problem"] = f"first byte difference at +{diff:x}"
        out.append(ent)
    return out


def verify_object(img, mod, only=None, seg_report=None):
    binds = Bindings(img)
    code_si = [i for i, s in enumerate(mod.segments) if s and s.cls.upper() == "CODE"]
    results = []
    seg_base = {}          # segment index -> oracle (obj, base) inferred or placed
    seg_votes = {}
    for si in code_si:
        seg = mod.segments[si]
        pubs = sorted((o, n) for n, s, o, _ in mod.publics if s == si)
        # segment base from bound publics (all must agree)
        for o, n in pubs:
            a, _src = binds.lookup(n)
            if a:
                seg_votes.setdefault(si, set()).add((a[0], a[1] - o))
        if len(seg_votes.get(si, ())) == 1:
            seg_base[si] = next(iter(seg_votes[si]))
        for k, (o, n) in enumerate(pubs):
            name = demangle(n)
            if only and name not in only and n not in only:
                continue
            end = pubs[k + 1][0] if k + 1 < len(pubs) else seg.size
            res = {"function": name, "symbol": n, "segment": seg.name, "seg_offset": o, "size": end - o,
                   "status": None, "problems": [], "unverified": [], "references": []}
            results.append(res)
            place, _ = binds.lookup(n)
            if not place:
                res["status"] = "UNPLACED"
                res["problems"].append("no binding in config/symbols.json")
                continue
            votes = seg_votes.get(si, set())
            res["unit_text"] = ("contiguous" if len(votes) == 1 else
                                f"CONFLICT: publics of {seg.name} imply bases " +
                                ", ".join(f"{a}:{b:05x}" for a, b in sorted(votes)))
            pobj, paddr = place
            # this function's own placement fixes its segment's base for its own intra-segment references
            seg_base[si] = (pobj, paddr - o)
            res["oracle"] = f"{pobj}:{paddr:05x}"
            cand = bytes(seg.data[o:end])
            orig = img.obj[pobj][paddr:paddr + len(cand)]
            if bytes(seg.covered[o:end]) != b"\x01" * len(cand):
                res["problems"].append("candidate segment has uncovered (undefined) bytes in extent")
            fx = [f for f in mod.fixups if f.seg == si and o <= f.off < end]
            masked = set()
            cand_abs_sites = set()
            for f in fx:
                rel = f.off - o
                size = f.size()
                for q in range(size):
                    masked.add(rel + q)
                field = struct.unpack_from("<i", seg.data, f.off)[0] if size == 4 else 0
                tname = mod.target_name(f)
                tm = f.target[0] & 3
                taddr, tsrc = None, None
                if tm == 2:
                    taddr, tsrc = binds.lookup(tname)
                    if not taddr:
                        # a public of this object?
                        for n2, s2, o2, _ in mod.publics:
                            if n2 == tname and s2 in seg_base:
                                taddr, tsrc = (seg_base[s2][0], seg_base[s2][1] + o2), "same-object"
                elif tm == 0:
                    tseg = f.target[1]
                    if tseg in seg_base:
                        taddr, tsrc = seg_base[tseg], "segment"
                ref = {"site": rel, "kind": f.kind(), "self_rel": f.self_rel, "target": tname,
                       "disp": f.disp, "field": field}
                if f.self_rel:
                    if size != 4:
                        res["problems"].append(f"+{rel:x}: unsupported self-relative size {size}")
                        continue
                    if (pobj, paddr + rel) in img.fixups:
                        res["problems"].append(f"+{rel:x}: oracle has an absolute fixup at a rel32 field")
                    otgt = paddr + rel + 4 + struct.unpack_from("<i", img.obj[pobj], paddr + rel)[0]
                    ref["oracle_target"] = f"1:{otgt:05x}"
                    # Watcom stores rel32 fields as 0 with no displacement; anything else is reported.
                    if field not in (0,):
                        res["problems"].append(f"+{rel:x}: unexpected rel32 field contents {field}")
                    if taddr is None:
                        res["unverified"].append(f"+{rel:x} call/jmp -> {tname} (unbound; oracle 1:{otgt:05x})")
                    elif taddr != (1, otgt - f.disp):
                        res["problems"].append(f"+{rel:x}: rel32 -> {tname} bound {taddr[0]}:{taddr[1]:05x}, "
                                               f"oracle 1:{otgt:05x}")
                    else:
                        ref["verified_by"] = tsrc
                else:
                    if size != 4:
                        res["problems"].append(f"+{rel:x}: unsupported fixup location {f.kind()}")
                        continue
                    cand_abs_sites.add(rel)
                    ofx = img.fixups.get((pobj, paddr + rel))
                    if not ofx:
                        res["problems"].append(f"+{rel:x}: candidate off32 fixup, oracle has none")
                        continue
                    _, tobj, toff = ofx
                    ref["oracle_target"] = f"{tobj}:{toff & 0xFFFFFFFF:x}"
                    addend = f.disp + field
                    if tm == 0 and taddr is None:
                        tseg = f.target[1]
                        cseg = mod.segments[tseg]
                        base = (tobj, toff - addend)
                        if tseg in seg_base and seg_base[tseg] != base:
                            res["problems"].append(f"+{rel:x}: segment {cseg.name} reference disagrees")
                            continue
                        seg_base[tseg] = base
                        ok = cseg.size > 0 and cseg.cls.upper() != "BSS" and \
                            bytes(cseg.covered) == b"\x01" * cseg.size and \
                            img.obj[tobj][base[1]:base[1] + cseg.size] == bytes(cseg.data)
                        if ok:
                            ref["verified_by"] = f"segment {cseg.name} contents ({cseg.size} bytes) equal oracle"
                        else:
                            res["unverified"].append(f"+{rel:x} -> segment {cseg.name}+{addend:x} "
                                                     f"(oracle {tobj}:{toff:x}; contents not verified)")
                        res["references"].append(ref)
                        continue
                    if taddr is None:
                        res["unverified"].append(f"+{rel:x} -> {tname}+{addend:x} (unbound; oracle {tobj}:{toff:x})")
                    elif (taddr[0], taddr[1] + addend) != (tobj, toff):
                        res["problems"].append(f"+{rel:x}: off32 -> {tname}+{addend:x} = {taddr[0]}:"
                                               f"{taddr[1] + addend:x}, oracle {tobj}:{toff:x}")
                    else:
                        ref["verified_by"] = tsrc
                res["references"].append(ref)
            orig_sites = {s - paddr for (ob, s) in img.fixups if ob == pobj and paddr <= s < paddr + len(cand)}
            extra = sorted(orig_sites - cand_abs_sites)
            if extra:
                res["problems"].append("oracle fixups without candidate fixup at +" + ",".join(f"{x:x}" for x in extra))
            diffs = [i for i in range(len(cand)) if i not in masked and (i >= len(orig) or cand[i] != orig[i])]
            res["byte_diffs"] = len(diffs)
            if diffs:
                res["first_diff"] = diffs[0]
                # align to an instruction start in the candidate stream
                ins_start = 0
                for ins in MD.disasm(cand, 0):
                    if ins.address > diffs[0]:
                        break
                    ins_start = ins.address
                res["diff_view"] = side_by_side(cand, img.obj[pobj][paddr:paddr + len(cand) + 32], ins_start, paddr)
            if diffs or res["problems"]:
                res["status"] = "NEAR_MATCH"
            elif res["unverified"]:
                res["status"] = "CODE_BYTES_MATCH"
            else:
                res["status"] = "STRICT"
            res["verified_bytes"] = len(cand) if res["status"] == "STRICT" else 0
    if seg_report is not None:
        seg_report.extend(segment_report(img, mod, seg_base, binds))
    return results


def report(results, stream=sys.stdout):
    units = sorted({r["unit_text"] for r in results if r.get("unit_text")})
    for u in units:
        print(f"# object _TEXT layout: {u}", file=stream)
    for r in results:
        loc = r.get("oracle", "?")
        print(f"{r['status']:16s} {r['function']:28s} {loc:9s} size {r['size']:4d}"
              + (f"  byte diffs {r['byte_diffs']}" if r.get("byte_diffs") else ""), file=stream)
        for p in r["problems"]:
            print(f"    problem: {p}", file=stream)
        for u in r["unverified"]:
            print(f"    unverified: {u}", file=stream)
        if r.get("diff_view"):
            print(f"    first difference at +{r['first_diff']:x} (candidate | oracle):", file=stream)
            for ln in r["diff_view"]:
                print(ln, file=stream)


def main(argv):
    argv = list(argv[1:])
    opts = {}
    for k in ("--oracle", "--profile", "--install", "--obj", "--json", "--symbols"):
        if k in argv:
            i = argv.index(k)
            opts[k] = argv[i + 1]
            del argv[i:i + 2]
    img = Image(opts.get("--oracle"))
    Bindings.extra = opts.get("--symbols")
    if "--obj" in opts:
        obj = opts["--obj"]
        only = argv
    else:
        src = Path(argv[0])
        only = argv[1:]
        flags = [a for a in only if a.startswith("-")]
        only = [a for a in only if not a.startswith("-")]
        if "--install" in opts:
            inst = opts["--install"]
        else:
            prof = wcc.profile(opts.get("--profile", "game"))
            inst, flags = prof["install"], list(prof["flags"]) + flags
        out = BUILD / "match" / f"{src.stem}.{os.getpid()}.obj"
        out.parent.mkdir(parents=True, exist_ok=True)
        rc, text = wcc.compile_c(src.resolve(), out, flags, inst)
        if rc:
            print(text)
            return 2
        print(f"# compiled {src} with {inst} {' '.join(flags)}")
        obj = out
    mods = omf.load(obj)
    if "--obj" not in opts:
        Path(obj).unlink()
    results = []
    segs = []
    for m in mods:
        results += verify_object(img, m, set(only) if only else None, segs)
    report(results)
    for g in segs:
        print(f"# data segment {g['segment']:7s} {g['size']:5d} bytes: {g['status']}"
              + (f" at {g['oracle']}" if g.get("oracle") else "") + (f" ({g['problem']})" if g.get("problem") else ""))
    strict = [r for r in results if r["status"] == "STRICT"]
    print(f"# STRICT {len(strict)}/{len(results)} functions, {sum(r['verified_bytes'] for r in strict)} bytes")
    if "--json" in opts:
        dump_json(opts["--json"], results)
    return 0 if len(strict) == len(results) else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv))
