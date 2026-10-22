"""NEAR_MATCH structural census (diagnostic only - never a verdict; STRICT is decided by tools/match.py).

    python tools/census.py                 all accepted units -> build/near_census.json
    python tools/census.py --brief UNIT..  per-unit worker brief -> build/near_census_UNIT.md (run the census first)
    python tools/census.py --file SRC.c [--symbols OVERLAY.json]   brief for any candidate -> SRC.census.md

For every accepted NEAR function the compiled candidate is compared with the DOS bytes on structure:
  calls   ordered callee identities          globals  data targets (address+addend; CONST literals folded)
  fields  struct displacements (non-stack)   consts   program constants (width masks, 0/1, +-1 comparison
  frame   prologue push set / sub esp                 rewrites and width shifts ignored)
-5 short loads (`mov r,[x-2] ... sar r,16`) and affine addresses within blocks are normalised.
RET stops trailing-data decoding after all forward branch arms. Known callbacks and INT operations retain identity.
Classes: REFS_LAYOUT (bytes equal, only intra-object references differ), SHAPE/REGALLOC/FRAME/TYPE_WIDTH/CALL_ORDER
(same calls/globals/fields/constants: Watcom shape only), DIFF_<what> (pointers to a logic/binding/prototype fix).
Validation (docs/CENSUS.md): SAME-structure verdicts were shape-only fixes in 28/30 historical NEAR->STRICT cases;
DIFF pointers were right in ~60%; census-guided finish passes solved 87% / 50% of NEAR functions vs ~46% unguided.
"""
import collections
import json
import difflib
import re
import struct
import sys
from pathlib import Path

ROOT = next(p for p in Path(__file__).resolve().parents if (p / "config/units.json").exists())
OUT = ROOT / "build" if Path(__file__).resolve().parent == ROOT / "tools" else Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / "tools"))
import capstone  # noqa: E402
import match  # noqa: E402
import omf  # noqa: E402
from image import Image  # noqa: E402
import seed  # noqa: E402
from common import load_json  # noqa: E402

MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
MD.detail = True
REG = re.compile(r"\b(e?[abcd]x|[abcd][lh]|e?[sd]i|e?bp|e?sp)\b")
LIT_LO, LIT_HI = 0x2fd00, 0x3218a          # runtime/CONST literal area of obj2 (docs/DATA.md)
PAD = {"nop", "int3"}
WIDTH = {0xff, 0xffff}                   # masks from width conversions, not program constants
PARENT = {part: reg for reg, parts in {
    "eax": ("al", "ah", "ax"), "ebx": ("bl", "bh", "bx"), "ecx": ("cl", "ch", "cx"),
    "edx": ("dl", "dh", "dx"), "esi": ("si",), "edi": ("di",), "ebp": ("bp",)}.items() for part in parts}
TRACKED = {"eax", "ebx", "ecx", "edx", "esi", "edi", "ebp"}


def is_pad(ins):
    return ins.mnemonic in PAD or (ins.mnemonic in ("lea", "mov") and ins.op_str in (
        "eax, [eax]", "eax, eax", "esi, [esi]", "edi, [edi]", "edx, [edx]", "ebx, [ebx]", "ecx, [ecx]"))


def insns(code):
    out = list(MD.disasm(code, 0))
    # Keep later branch arms after an early RET, but never decode trailing tables/strings as code.
    # Forward targets are enough here: Watcom places switch arms before their common epilogue.
    frontier = 0
    for k, i in enumerate(out):
        if (i.group(capstone.CS_GRP_JUMP) and i.operands[0].type == capstone.CS_OP_IMM
                and 0 <= i.operands[0].imm < len(code)):
            frontier = max(frontier, i.operands[0].imm)
        if i.group(capstone.CS_GRP_RET) and i.address >= frontier:
            out = out[:k + 1]
            break
    while out and is_pad(out[-1]):
        out.pop()
    return out


def signed(v, size):
    """Canonical signed bit pattern (word FFFEh and a full-width -2 are the same sentinel)."""
    # Byte IDs (e.g. sound 214) are commonly materialised in a full register elsewhere.
    bits = max(16, size * 8)
    v &= (1 << bits) - 1
    return v - (1 << bits) if v & (1 << (bits - 1)) else v


def memory_features(ins, refs, short_at, call_effects):
    """Affine addresses: undo scheduled increments and retain cached global bases.

    MOV/LEA, constant arithmetic and scaled indices are propagated. Joins keep only agreeing
    facts; loop headers forget them. This is a diagnostic, not an alias/equivalence proof.
    """
    fields, globals_ = collections.Counter(), collections.Counter()
    state, stack = {}, {}
    esp = 0
    targets = {i.operands[0].imm for i in ins if i.group(capstone.CS_GRP_JUMP)
               and i.operands[0].type == capstone.CS_OP_IMM}
    loops = {i.operands[0].imm for i in ins if i.group(capstone.CS_GRP_JUMP)
             and i.operands[0].type == capstone.CS_OP_IMM and i.operands[0].imm <= i.address}
    incoming, fallthrough = collections.defaultdict(list), True
    frame = any(i.mnemonic == "mov" and i.op_str == "ebp, esp" for i in ins[:12])

    def val(reg):
        if reg == capstone.x86.X86_REG_ESP:
            return "STACK", esp
        return state.get(reg, (None, 0))

    def address(op, ref):
        b, d = val(op.mem.base)
        ib, delta = val(op.mem.index)
        d += delta * op.mem.scale
        # An off32 fixup is the displacement, not the Capstone numeric placeholder.
        if ref:
            return ref, d
        if not b and ib and op.mem.scale == 1:
            b = ib
        return b, d + op.mem.disp

    for i in ins:
        if i.address in targets:
            paths = incoming[i.address] + ([(state, stack, esp)] if fallthrough else [])
            if paths:
                state = {r: v for r, v in paths[0][0].items() if all(p[0].get(r) == v for p in paths)}
                stack = {s: v for s, v in paths[0][1].items() if all(p[1].get(s) == v for p in paths)}
                if len({p[2] for p in paths}) != 1:
                    stack.clear()
                esp = paths[0][2]
            if i.address in loops:
                state.clear()
                stack.clear()
        fallthrough = True
        ops, m = i.operands, i.mnemonic
        ref = next((refs[s] for s in range(i.address, i.address + i.size) if s in refs), None)
        for op in ops:
            if op.type != capstone.CS_OP_MEM or m == "lea":
                continue
            if op.mem.base == capstone.x86.X86_REG_ESP or (frame and op.mem.base == capstone.x86.X86_REG_EBP):
                continue
            b, d = address(op, ref)
            d += short_at.get(i.address, 0)
            if b == "STACK":
                continue
            if b:
                if b.startswith("1:"):
                    b = "TEXT"  # reading code/initialiser bytes, not taking a callback address
                if re.fullmatch(r"\d:[0-9a-f]+", b):
                    o, a = b.split(":")
                    b = f"{o}:{int(a, 16) + d:x}"
                globals_[b] += 1
            elif d and abs(d) < 0x10000:
                fields[d] += 1
        # Preserve a cached address through a local stack spill, including PUSH/POP scheduling.
        if m == "push":
            esp -= 4
        elif m == "pop":
            esp += 4
        if len(ops) == 2 and ops[0].type == capstone.CS_OP_REG and ops[0].reg == capstone.x86.X86_REG_ESP:
            if m in ("add", "sub") and ops[1].type == capstone.CS_OP_IMM:
                esp += ops[1].imm * (1 if m == "add" else -1)
        if m == "mov" and ops[0].type == capstone.CS_OP_MEM and ops[0].mem.base == capstone.x86.X86_REG_ESP:
            key = esp + ops[0].mem.disp
            stack.pop(key, None)
            if ops[1].type == capstone.CS_OP_REG and ops[1].size == 4:
                stack[key] = val(ops[1].reg)
        new = None
        if ops and ops[0].type == capstone.CS_OP_REG and ops[0].size == 4:
            dst = ops[0].reg
            if m == "mov" and ops[1].type == capstone.CS_OP_REG:
                new = val(ops[1].reg)
            elif m == "mov" and ops[1].type == capstone.CS_OP_IMM and ref:
                new = (ref, 0)
            elif m == "mov" and ops[1].type == capstone.CS_OP_MEM and ops[1].mem.base == capstone.x86.X86_REG_ESP:
                new = stack.get(esp + ops[1].mem.disp)
            elif m == "lea":
                new = address(ops[1], ref)
            elif m in ("add", "sub"):
                b, d = val(dst)
                if ops[1].type == capstone.CS_OP_IMM:
                    new = (ref, 0) if ref else (b, d + ops[1].imm * (1 if m == "add" else -1))
                elif m == "add" and ops[1].type == capstone.CS_OP_REG and b:
                    ib, delta = val(ops[1].reg)
                    if not ib:
                        new = (b, d + delta)
                elif m == "add" and ops[1].type == capstone.CS_OP_REG:
                    ib, delta = val(ops[1].reg)
                    new = (ib, d + delta)
            elif m in ("shl", "sal", "imul") and ops[-1].type == capstone.CS_OP_IMM:
                b, d = val(ops[1].reg if len(ops) == 3 else dst)
                if not b:
                    new = (None, d * (1 << ops[-1].imm if m != "imul" else ops[-1].imm))
            elif m in ("inc", "dec"):
                b, d = val(dst)
                new = (b, d + (1 if m == "inc" else -1))
        for reg in i.regs_access()[1]:
            # Partial register writes also invalidate their 32-bit parent.
            name = i.reg_name(reg)
            parent = PARENT.get(name, name)
            state.pop(getattr(capstone.x86, "X86_REG_" + parent.upper(), reg), None)
        if new is not None:
            state[dst] = new
        if m == "call":
            pop, preserved = call_effects.get(i.address + 1, (0, set()))
            esp += pop
            for reg in TRACKED - preserved:
                state.pop(getattr(capstone.x86, "X86_REG_" + reg.upper()), None)
        if i.group(capstone.CS_GRP_JUMP) and ops[0].type == capstone.CS_OP_IMM:
            incoming[ops[0].imm].append((state.copy(), stack.copy(), esp))
        if m == "jmp" or i.group(capstone.CS_GRP_RET):
            state.clear()
            stack.clear()
            fallthrough = False
    return fields, globals_


def features(code, refs, rel_calls, call_effects=None):
    """refs: {site: target_id} for absolute data refs; rel_calls: {site: target_id} for rel32 calls/jmps."""
    f = {"calls": [], "globals": collections.Counter(), "fields": collections.Counter(),
         "consts": collections.Counter(), "jcc": 0, "frame": [], "toks": [], "rtoks": []}
    ins = insns(code)
    prologue = True
    # -5 short loads `mov r,[x-2] ... sar/shr r,16` read x: undo the -2 so width changes do not look like new
    # targets (the shift may be scheduled a few instructions later)
    short_at = {}
    for k, a in enumerate(ins):
        m0 = re.match(r"mov (e\w\w), dword ptr \[", a.mnemonic + " " + a.op_str)
        if not m0:
            continue
        r = m0.group(1)
        for b in ins[k + 1:k + 9]:
            if b.mnemonic in ("sar", "shr") and b.op_str in (r + ", 0x10", r + ", 0x18"):
                short_at[a.address] = 2 if b.op_str.endswith("0x10") else 3   # short: [x-2]; signed byte: [x-3]
                break
            if b.op_str.split(",")[0].strip() in (r, r[1:]) and b.mnemonic not in ("cmp", "test", "push"):
                break
    for i in ins:
        m, ops = i.mnemonic, i.op_str
        if prologue and m == "push":
            f["frame"].append(ops)
        elif prologue and m == "sub" and ops.startswith("esp,"):
            f["frame"].append(ops)
        else:
            prologue = False
        has_fix = False
        for s in range(i.address, i.address + i.size):
            if s in rel_calls:
                has_fix = True
                if m == "call":
                    f["calls"].append(rel_calls[s])
            if s in refs:
                has_fix = True
                g = refs[s]
                if i.address in short_at and re.match(r"\d:[0-9a-f]+$", g):
                    o, a = g.split(":")
                    g = f"{o}:{int(a, 16) + short_at[i.address]:x}"
                if not any(op.type == capstone.CS_OP_MEM for op in i.operands) or m == "lea":
                    f["globals"][g] += 1
        if m == "int":
            f["calls"].append("int:" + format(i.operands[0].imm, "x"))
        if m.startswith("j") and m != "jmp":
            f["jcc"] += 1
        t = m + " " + re.sub(r"0x[0-9a-f]{5,}", "A", ops)
        if m.startswith("j") or m in ("call", "loop") or has_fix:
            t = m + " " + ("T" if (m.startswith("j") or m == "loop") else re.sub(r"0x[0-9a-f]+", "A", ops))
            f["rtoks"].append(t)
            f["toks"].append(REG.sub("R", t))
        else:
            f["rtoks"].append(t)
            f["toks"].append(REG.sub("R", t))
        if m.startswith("j") or m in ("call", "loop", "ret", "retf"):
            continue
        # The immediate can coexist with an off32 displacement: MOV [global],7 still contains 7.
        imm_fix = i.address + i.imm_offset in refs or i.address + i.imm_offset in rel_calls
        if not imm_fix:
            for op in i.operands:
                if op.type != capstone.CS_OP_IMM:
                    continue
                v = signed(op.imm, op.size)
                if m in ("add", "sub"):
                    v = abs(v)  # LEA [r-k] and SUB r,k use the same arithmetic magnitude
                if m in ("shl", "sar", "shr", "sal") and v in (16, 24):
                    continue
                if not (m in ("sub", "add") and ops.startswith("esp")):
                    f["consts"][v] += 1
        if (m == "lea" and i.operands[1].mem.disp and not has_fix
                and i.operands[1].mem.base not in (capstone.x86.X86_REG_ESP, capstone.x86.X86_REG_EBP)):
            f["consts"][abs(i.operands[1].mem.disp)] += 1
    f["fields"], mem_globals = memory_features(ins, refs, short_at, call_effects or {})
    f["globals"].update(mem_globals)
    return f


def ext_only(a, b):
    """True if the two token multisets differ only by width-extension / masking instructions."""
    ext = re.compile(r"^(movsx|movzx|cwde|cbw|cwd|cdq|and R, (0xff|0xffff)|shl R, 0x10|sar R, 0x10|shr R, 0x10)")
    ca, cb = collections.Counter(a), collections.Counter(b)
    d = (ca - cb) + (cb - ca)
    return bool(d) and all(ext.match(t) for t in d)


def const_diff(a, b):
    """Program constants that differ, ignoring width masks, 0/1 (boolean materialisation) and +-1 pairs (the same
    comparison written as `< n` or `<= n-1`)."""
    a = set(a) - WIDTH - {-1, 0, 1}
    b = set(b) - WIDTH - {-1, 0, 1}
    ma, mb = a - b, b - a
    for x in sorted(ma):
        for y in (x - 1, x + 1):
            if y in mb:
                ma.discard(x)
                mb.discard(y)
                break
    return sorted(ma), sorted(mb)


def classify(fc, fo, byte_diffs, problems):
    """Root-cause class of a NEAR function from candidate vs oracle features."""
    if byte_diffs == 0:
        if any("unbound" in p for p in problems):
            return "REFS_UNRESOLVED"
        return "REFS_LAYOUT"
    sem = []
    if collections.Counter(fc["calls"]) != collections.Counter(fo["calls"]):
        sem.append("calls")
    if set(fc["globals"]) != set(fo["globals"]):
        sem.append("globals")
    if set(fc["fields"]) != set(fo["fields"]):
        sem.append("fields")
    if any(const_diff(fc["consts"], fo["consts"])):
        sem.append("consts")
    if sem:
        return "DIFF_" + "+".join(sem)
    if collections.Counter(fc["calls"]) == collections.Counter(fo["calls"]) and fc["calls"] != fo["calls"]:
        return "CALL_ORDER"
    if fc["toks"] == fo["toks"]:
        return "REGALLOC" if fc["frame"] == fo["frame"] else "FRAME"
    if ext_only(fc["toks"], fo["toks"]):
        return "TYPE_WIDTH"
    if fc["calls"] != fo["calls"]:
        return "ORDER"
    return "SHAPE"


class Oracle:
    def __init__(self):
        self.img = Image()
        self.binds = match.Bindings(self.img)
        starts = sorted({a for a in list(self.binds.game.values()) + list(self.binds.lib.values()) if a[0] == 1})
        self.starts = [a[1] for a in starts]
        self.name_at = {}
        self.pop_cache = {}
        for n, a in list(self.binds.game.items()) + list(self.binds.lib.items()):
            self.name_at.setdefault(a, n)

    def extent(self, addr, minimum=0):
        import bisect
        k = bisect.bisect_right(self.starts, addr)
        end = self.starts[k] if k < len(self.starts) else len(self.img.obj[1])
        # the next bound start can be far away when unbound functions follow: cap at 1.5x the candidate + 64
        return min(end, addr + minimum + minimum // 2 + 64) if minimum else end

    def target_id(self, obj, off, text=None):
        if obj == 2 and LIT_LO <= off < LIT_HI:
            return "LIT"
        if obj == 1 and text and text[0] <= off < text[1] and (obj, off) not in self.name_at:
            return "TEXT"
        return f"{obj}:{off:x}"

    def call_effects(self, calls):
        """DOS call effects: RET argument cleanup and registers saved/restored on every exit."""
        for target in set(calls.values()):
            if target not in self.pop_cache:
                pops = set()
                preserved = set()
                if target.startswith("1:"):
                    addr = int(target[2:], 16)
                    if 0 <= addr < len(self.img.obj[1]):
                        code = bytes(self.img.obj[1][addr:self.extent(addr)])
                        body = insns(code)
                        saved = []
                        for i in body:
                            if i.mnemonic != "push" or i.operands[0].type != capstone.CS_OP_REG:
                                break
                            saved.append(i.op_str)
                        exits = []
                        for k, i in enumerate(body):
                            if not i.group(capstone.CS_GRP_RET):
                                continue
                            pops.add(i.operands[0].imm if i.operands else 0)
                            restored = []
                            for prev in reversed(body[:k]):
                                if prev.mnemonic != "pop":
                                    break
                                restored.append(prev.op_str)
                            exits.append(set(saved) if restored == saved else set())
                        if exits:
                            preserved = set.intersection(*exits)
                            written = {PARENT.get(i.reg_name(r), i.reg_name(r)) for i in body for r in i.regs_access()[1]}
                            if any(i.mnemonic == "call" for i in body):
                                written |= TRACKED  # nested callees are not summarized recursively
                            preserved |= TRACKED - written
                self.pop_cache[target] = (pops.pop() if len(pops) == 1 else 0, preserved)
        return {site: self.pop_cache[target] for site, target in calls.items()}

    def oracle_features(self, addr, size_hint):
        end = self.extent(addr, size_hint)
        code = bytes(self.img.obj[1][addr:end])
        refs, calls = {}, {}
        for (o, s), (_, to, toff) in self.img.fixups.items():
            if o == 1 and addr <= s < end:
                refs[s - addr] = self.target_id(to, toff, (addr - 0x4000, end + 0x4000))
        for i in MD.disasm(code, 0):
            if i.mnemonic in ("call", "jmp") and i.size == 5 and code[i.address] in (0xE8, 0xE9):
                t = addr + i.address + 5 + struct.unpack_from("<i", code, i.address + 1)[0]
                calls[i.address + 1] = f"1:{t:x}"
        return features(code, refs, calls, self.call_effects(calls)), end - addr


def candidate_features(orc, mod, r):
    """Features of one verify_object result r from module mod."""
    seg = next(s for s in mod.segments if s and s.name == r["segment"])
    o0 = r["seg_offset"]
    code = bytes(seg.data[o0:o0 + r["size"]])
    si = mod.segments.index(seg)
    refs, calls = {}, {}
    def code_target(si, off, fallback="TEXT"):
        off &= 0xffffffff
        for n, s, o, _ in mod.publics:
            if (s, o) == (si, off):
                a, _ = orc.binds.lookup(n)
                return f"1:{a[1]:x}" if a else "name:" + match.demangle(n)
        return fallback
    for f in mod.fixups:
        if f.seg != si or not (o0 <= f.off < o0 + r["size"]):
            continue
        rel = f.off - o0
        tname = mod.target_name(f)
        a, _ = orc.binds.lookup(tname)
        if f.self_rel:
            calls[rel] = f"1:{a[1] + f.disp:x}" if a else "name:" + match.demangle(tname)
        else:
            field = struct.unpack_from("<i", seg.data, f.off)[0] if f.size() == 4 else 0
            if (f.target[0] & 3) == 0:          # segment-relative: own CONST/DATA literal
                target = mod.segments[f.target[1]]
                refs[rel] = ("LIT" if target.name.upper() == "CONST" else
                             code_target(f.target[1], field + f.disp) if target.cls.upper() == "CODE" else "SEG")
            else:
                refs[rel] = (orc.target_id(a[0], a[1] + f.disp + field) if a
                             else "name:" + match.demangle(tname))
    # Same-TU direct calls are often already resolved in LEDATA and have no OMF fixup.
    for i in insns(code):
        if i.mnemonic == "call" and i.size == 5 and code[i.address] == 0xE8:
            target = (o0 + i.operands[0].imm) & 0xffffffff
            calls.setdefault(i.address + 1, code_target(si, target, f"unresolved:{si}:{target:x}"))
    return features(code, refs, calls, orc.call_effects(calls))


def compare_unit(orc, src, symbols=None, only=None):
    """Compile src with the game profile and return [(result, cand_features, oracle_features, class, sim)]."""
    import wcc
    obj = OUT / "tmp" / ("structcmp_" + Path(src).stem + ".obj")
    obj.parent.mkdir(parents=True, exist_ok=True)
    prof = wcc.profile("game")
    if OUT != ROOT / "build":
        wcc.BUILD = OUT
    rc, text = wcc.compile_c(Path(src), obj, prof["flags"], prof["install"], extra_include=[ROOT / "src"])
    if rc:
        raise SystemExit(f"compile failed: {src}\n{text[-800:]}")
    match.Bindings.extra = str(symbols) if symbols else None
    try:
        out = []
        for mod in omf.load(obj):
            for r in match.verify_object(orc.img, mod, set(only) if only else None):
                if not r.get("oracle") or r["status"] == "STRICT":
                    if r.get("oracle") and r["status"] == "STRICT":
                        out.append((r, None, None, "STRICT", 1.0))
                    continue
                fc = candidate_features(orc, mod, r)
                o, a = r["oracle"].split(":")
                fo, osize = orc.oracle_features(int(a, 16), r["size"])
                r["oracle_size"] = osize
                cls = classify(fc, fo, r.get("byte_diffs", 0), r["problems"] + r["unverified"])
                sim = difflib.SequenceMatcher(None, fc["toks"], fo["toks"], autojunk=False).ratio()
                out.append((r, fc, fo, cls, round(sim, 3)))
        return out
    finally:
        match.Bindings.extra = None




def rows_for(un, res):
    rows = []
    for r, fc, fo, cls, sim in res:
        if cls == "STRICT":
            continue
        rv = seed.rayverse_body(r["function"])[0]
        rows.append({
            "unit": un, "function": r["function"], "class": cls, "sim": sim,
            "size": r["size"], "oracle_size": r.get("oracle_size"), "byte_diffs": r.get("byte_diffs"),
            "first_diff": r.get("first_diff"),
            "ps1": bool(seed.ps1_body(r["function"])[0]),
            "rayverse": bool(rv and "Not implemented" not in rv and "//stub" not in rv),
            "calls_c": len(fc["calls"]), "calls_o": len(fo["calls"]),
            "calls_missing": sorted((collections.Counter(fo["calls"]) - collections.Counter(fc["calls"])).elements()),
            "calls_extra": sorted((collections.Counter(fc["calls"]) - collections.Counter(fo["calls"])).elements()),
            "globals_missing": sorted(set(fo["globals"]) - set(fc["globals"])),
            "globals_extra": sorted(set(fc["globals"]) - set(fo["globals"])),
            "fields_missing": sorted(set(fo["fields"]) - set(fc["fields"])),
            "fields_extra": sorted(set(fc["fields"]) - set(fo["fields"])),
            "consts_missing": sorted(set(fo["consts"]) - set(fc["consts"])),
            "consts_extra": sorted(set(fc["consts"]) - set(fo["consts"])),
            "jcc": [fc["jcc"], fo["jcc"]], "frame": [fc["frame"], fo["frame"]],
            "problems": r["problems"][:3],
        })
    return rows


def census():
    units = load_json(ROOT / "config" / "units.json")["units"]
    orc = Oracle()
    rows = []
    for un, u in units.items():
        near = [f for f, e in u["functions"].items() if e["status"] != "STRICT"]
        if not near:
            continue
        try:
            res = compare_unit(orc, ROOT / u["source"], only=near)
        except SystemExit as e:
            print("FAIL", un, str(e)[:200])
            continue
        rows += rows_for(un, res)
        print(un, collections.Counter(x["class"] for x in rows if x["unit"] == un), flush=True)
    out = OUT / "near_census.json"
    out.write_text(json.dumps(rows, indent=1), encoding="utf-8")
    print("total", len(rows), collections.Counter(x["class"] for x in rows))


GUIDE = {
    "REFS_LAYOUT": "code bytes already equal; only references into this object (jump tables / calls) differ - fixed "
                   "by making earlier functions the right size. Do not edit this function.",
    "SHAPE": "observed calls, globals, struct fields and constants agree with DOS; investigate Watcom shape "
             "(operand/branch order, temporaries, types/widths, loop form). This is not a semantic-equivalence proof.",
}


def name_of(orc, ident):
    if ident.startswith("1:"):
        return orc.name_at.get((1, int(ident[2:], 16)), ident)
    if ident.startswith("2:"):
        a = int(ident[2:], 16)
        best = None
        for (o, x), n in orc.name_at.items():
            if o == 2 and x <= a and (best is None or x > best[0]):
                best = (x, n)
        return f"{best[1]}+0x{a - best[0]:x}" if best and a - best[0] < 0x400 else ident
    return ident


# ---- rule hints (docs/COMPILER.md "Matching rules") and callee-prototype lookup (W-PROTO)
DECL = re.compile(r"^[ \t]*(?:extern\s+)?((?:const\s+|unsigned\s+|signed\s+)?[A-Za-z_]\w*(?:\s*\*+)?)\s*\b"
                  r"([A-Za-z_]\w*)\s*\(([^;{)]*)\)\s*;", re.M)
ALIAS = {"short": "s16", "signed short": "s16", "unsigned short": "u16", "int": "s32", "long": "s32", "unsigned": "u32",
         "unsigned int": "u32", "unsigned long": "u32", "char": "s8", "signed char": "s8", "unsigned char": "u8",
         "bool": "u8", "BOOL": "u8"}
TYPEWORDS = {"s8", "u8", "s16", "u16", "s32", "u32", "void", "char", "short", "int", "long", "unsigned", "signed",
             "const", "bool"}


def _norm_type(t):
    words = re.sub(r"\s+", " ", t.replace("*", " * ")).strip().split(" ")
    if len(words) > 1 and re.match(r"^[A-Za-z_]\w*$", words[-1]) and words[-1] not in TYPEWORDS             and not words[-1].endswith("_t"):
        words = words[:-1]
    base = " ".join(w for w in words if w not in ("*", "const"))
    return ALIAS.get(base, base) + "*" * words.count("*")


def _sig(ret, params):
    ps = [_norm_type(x) for x in params.split(",")] if params.strip() not in ("", "void") else ["void"]
    return _norm_type(ret) + "(" + ", ".join(ps) + ")"


def decl_index(extra=None):
    """{callee: {unit: normalised signature}} over every accepted unit's source (+ an extra candidate file)."""
    units = load_json(ROOT / "config" / "units.json")["units"]
    srcs = [(un, ROOT / u["source"]) for un, u in units.items()]
    if extra:
        srcs.append(extra)
    idx = collections.defaultdict(dict)
    for un, path in srcs:
        for m in DECL.finditer(Path(path).read_text(encoding="utf-8", errors="replace")):
            ret, name, params = m.groups()
            if ret not in ("return", "else", "if"):
                idx[name][un] = _sig(ret, params)
    return idx


def proto_hints(unit, src_text, func, idx):
    """Callees of FUNC whose declaration in this unit differs from other units' declarations."""
    r = seed.find_function(src_text, func)
    if not r:
        return []
    out = []
    for name in sorted(set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", src_text[r[0]:r[1]])) - {func}):
        per = idx.get(name, {})
        mine = per.get(unit)
        others = collections.defaultdict(list)
        for un, sg in per.items():
            if un != unit and sg != mine:
                others[sg].append(un)
        if mine and others:
            alt = "; ".join(f"{sg} in {', '.join(sorted(us)[:4])}" for sg, us in others.items())
            out.append(f"{name}: here {mine}; elsewhere {alt}")
    return out


def rule_hints(r):
    """Rule IDs of docs/COMPILER.md to try first, from the structural features of a census row."""
    cls, ids = r["class"], []
    if cls.startswith("REFS"):
        return ["W-LAYOUT"]
    if "calls" in cls:
        ids += ["W-LAYOUT", "W-PORT", "W-PROTO"]
    if "globals" in cls or "fields" in cls:
        ids += ["W-BIND"]
    if "fields" in cls:
        ids += ["W-BITF"]
    if "consts" in cls:
        ids += ["W-THRESH"]
    if r["frame"][0] != r["frame"][1]:
        cp = [x for x in r["frame"][0] if not x.startswith("esp")]
        dp = [x for x in r["frame"][1] if not x.startswith("esp")]
        if cp != dp:
            ids += ["W-RETW", "W-UNUSED", "W-TEMP"]
        if [x for x in r["frame"][0] if x.startswith("esp")] != [x for x in r["frame"][1] if x.startswith("esp")]:
            ids += ["W-DECL", "W-WIDTH", "W-TEMP"]
    if cls == "TYPE_WIDTH":
        ids += ["W-WIDTH", "W-RETW"]
    if cls in ("SHAPE", "REGALLOC", "CALL_ORDER") or not ids:
        ids += ["W-OPORD", "W-BRANCH", "W-STORE", "W-TEMP", "W-BOOL"]
    return list(dict.fromkeys(ids))



def first_size_drift(orc, rows):
    """W-LAYOUT: the earliest NEAR function (by bound DOS address) whose candidate size differs from the DOS extent.
    Every later intra-object reference shifts with it, so it is the function to fix first."""
    placed = []
    for r in rows:
        a = orc.binds.game.get(r["function"])
        if a and r.get("oracle_size") and r["size"] != r["oracle_size"]:
            placed.append((a, r))
    if not placed:
        return None
    a, r = min(placed, key=lambda t: t[0])
    return f"{r['function']} at {a[0]}:{a[1]:05x} (candidate {r['size']} bytes, DOS ~{r['oracle_size']})"


def brief(unit, rows=None, path=None, orc=None, src=None):
    orc = orc or Oracle()
    if rows is None:
        rows = [r for r in json.load(open(OUT / "near_census.json")) if r["unit"] == unit]
    out = [f"# NEAR census for `{unit}` ({len(rows)} functions)", "",
           "Structural comparison of the accepted candidate vs DOS (tools/census.py; docs/CENSUS.md). Validated:",
           "SAME-STRUCTURE (SHAPE) -> the fix was shape-only in 28/30 past cases; DIFF pointers were real in ~16/30",
           "(sometimes the fix is outside the function: a missing/empty callee, a prototype). 'oracle-only' = DOS has",
           "it, candidate does not; 'cand-only' = the reverse. Addresses are resolved to bound names where possible.", ""]
    units = load_json(ROOT / "config" / "units.json")["units"]
    if src is None and unit in units:
        src = ROOT / units[unit]["source"]
    src_text = Path(src).read_text(encoding="utf-8", errors="replace") if src else ""
    idx = decl_index((unit, src) if src and unit not in units else None)
    out.append("Rule IDs refer to docs/COMPILER.md \"Matching rules\" (triage order: W-LAYOUT, W-PROTO, W-BIND, W-PORT).")
    drift = first_size_drift(orc, rows)
    if drift:
        out.append("Layout (W-LAYOUT): first function whose size differs from DOS: " + drift +
                   " - later functions' intra-object references shift with it; fix it first.")
    out.append("")
    for r in sorted(rows, key=lambda r: -r["sim"]):
        cls = r["class"]
        out.append(f"## {r['function']}  [{cls}]  sim {r['sim']}  size cand {r['size']} / DOS ~{r['oracle_size']}")
        out.append("- rules to try: " + ", ".join(rule_hints(r)))
        for h in proto_hints(unit, src_text, r["function"], idx):
            out.append("- callee prototype (W-PROTO): " + h)
        if cls in GUIDE:
            out.append("- " + GUIDE[cls])
        for k in ("calls", "globals", "fields", "consts"):
            miss, extra = r.get(f"{k}_missing", []), r.get(f"{k}_extra", [])
            if k == "consts":
                extra, miss = const_diff(r.get("consts_extra", []), r.get("consts_missing", []))
            if miss or extra:
                f = (lambda x: name_of(orc, x)) if k in ("calls", "globals") else str
                out.append(f"- {k}: oracle-only {[f(x) for x in miss][:8]}  cand-only {[f(x) for x in extra][:8]}")
        if r["frame"][0] != r["frame"][1]:
            out.append(f"- prologue: cand {r['frame'][0]}  DOS {r['frame'][1]}")
        out.append("")
    p = Path(path) if path else OUT / f"near_census_{unit}.md"
    p.write_text("\n".join(out), encoding="utf-8")
    print(p)



def brief_file(src, symbols=None):
    """Census of any candidate source (e.g. a worker's in-progress unit) -> <src>.census.md next to it."""
    orc = Oracle()
    res = compare_unit(orc, Path(src), symbols=symbols)
    brief(Path(src).stem, rows_for(Path(src).stem, res), Path(src).with_suffix(".census.md"), orc, src=src)


def main(argv):
    if len(argv) > 2 and argv[1] == "--file":
        sym = argv[argv.index("--symbols") + 1] if "--symbols" in argv else None
        brief_file(argv[2], sym)
    elif len(argv) > 1 and argv[1] == "--brief":
        for u in argv[2:]:
            brief(u)
    else:
        census()


if __name__ == "__main__":
    main(sys.argv)
