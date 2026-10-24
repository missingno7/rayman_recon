"""Bounded differential execution of pinned DOS code and pinned-toolchain C (harness v2).

All generated files stay next to this script. No verdict is a matching verdict (never STRICT).
Usage: python -B build/workers/difftest/difftest.py UNIT FUNCTION [--seeds 128]
       python -B build/workers/difftest/difftest.py --all [--units a,b] [--resume]
       python -B build/workers/difftest/difftest.py --controls
       python -B build/workers/difftest/difftest.py --replay RESULT_JSON
       python -B build/workers/difftest/difftest.py --prototype-report

Isolated-function experiment: outgoing calls are deterministic stubs (declared contracts from the project's
own C declarations, see cdecls.py), not game execution. See REPORT.md for the tested domain and limitations.

v2 changes (REPORT.md): candidate code is mapped identically on both sides at CAND; callee contracts come from a
project-wide declaration table (widths, signedness, pointees, #pragma aux registers, function-pointer typedefs,
variables and struct members); pointer arguments are compared by object identity; non-const scalar pointees are
written by the stubs; globals written before a call must agree at that call; per-seed instruction budgets escalate;
CPU exceptions are terminations; optional per-function domains (domains/UNIT_FUNCTION.json).
"""
from __future__ import annotations

import argparse
import bisect
import collections
import copy
import dataclasses
import hashlib
import json
import re
import shutil
import struct
import sys
import time
import traceback
from pathlib import Path

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
ROOT = next(p for p in HERE.parents if (p / 'config/units.json').exists())
OUT = ROOT / 'build' / 'difftest'   # every generated file (results, objects, unit copies, mutations, reports)
sys.path.insert(0, str(ROOT / 'tools'))
sys.path.insert(0, str(HERE))
import capstone as cs
import unicorn as uc
from unicorn import x86_const as ux
import match
import omf
import wcc
from image import Image, IDA_BASE, parse_addr
import cdecls
from cdecls import CType, Prototype, split_args

PAGE = 4096
CAND = 0x58000000          # candidate code; mapped with identical bytes in BOTH executions
FNPTR = 0x5e000000         # synthetic identities of declared function pointers (never mapped, never executed)
HEAP = 0x2000000
FRESH = HEAP + 0x800000    # domain-file allocations (disjoint pages)
STACK_LO, STACK_TOP, STACK_HI = 0x70000000, 0x70040000, 0x70041000
STOP = 0x60000000
MASK = 0xffffffff
REGS = ('eax', 'edx', 'ebx', 'ecx', 'esi', 'edi', 'ebp', 'esp')
REGID = {n: getattr(ux, 'UC_X86_REG_' + n.upper()) for n in REGS}
PARENT = {s: p for p, ss in {
    'eax': ('ax', 'al', 'ah'), 'edx': ('dx', 'dl', 'dh'), 'ebx': ('bx', 'bl', 'bh'),
    'ecx': ('cx', 'cl', 'ch'), 'esi': ('si',), 'edi': ('di',), 'ebp': ('bp',), 'esp': ('sp',)
}.items() for s in ss}
MD = cs.Cs(cs.CS_ARCH_X86, cs.CS_MODE_32)
MD.detail = True
HARNESS_SHA256 = hashlib.sha256(b''.join((HERE / n).read_bytes() for n in ('difftest.py', 'cdecls.py', 'domains.py'))).hexdigest()
GENERATOR = 'splitmix64-guided-v3'
TIERS = (20000, 300000, 2000000, 8000000)     # per-seed instruction budgets, escalated only while a side is still running
MAX_EVENTS = (400, 3000, 12000, 30000)      # per-tier outgoing call/system-event budgets (stubs are Python, instructions are native)
MIN_EQUAL = 16                       # judged equal seeds needed for EQUIVALENT_TESTED
MIN_JUDGED_FRACTION = 0.25
# Pure Watcom library members executed from the oracle image on both sides (register interface, memory effects
# tracked by the normal hooks): -fpc floating-point helpers and math intrinsics, and the string/memory routines
# (so a C call to memset and an inlined rep stosd are the same behaviour).
TRANSPARENT = re.compile(r'^(_FD[A-Z0-9]*|_FS[A-Z0-9]*|_I4FD|_U4FD|_RD[IU]4|IF@\w+|_fdiv\w*|fdiv_main_routine|verify_pentium_fdiv_bug'
                         r'|memset|memmove|memcpy|memcmp|strcpy|strncpy|strcat|strlen|strcmp|strncmp|strcmpi|stricmp|strnicmp|strupr)$')
LIB_EFFECTS = ('memcpy', 'memmove', 'memset', 'strcpy', 'strncpy', 'strcat', 'fread', 'read', 'sprintf')
SYSTEM_MNEMONICS = ('int', 'int3', 'into', 'in', 'out', 'insb', 'insw', 'insd', 'outsb', 'outsw', 'outsd', 'cli', 'sti',
                    'hlt', 'lgdt', 'lidt', 'rdtsc', 'cpuid')


class Limit(Exception):
    pass


def write_json(path, value):
    path = Path(path).resolve()
    path.relative_to(OUT)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2, default=str) + '\n', encoding='utf-8')


def mix(seed, addr, domain=0):
    """Integer-only, platform-independent SplitMix64; no access-order dependence."""
    x = (seed * 0x9e3779b97f4a7c15 + addr + domain * 0xd1b54a32d192ed03) & ((1 << 64) - 1)
    x = ((x ^ (x >> 30)) * 0xbf58476d1ce4e5b9) & ((1 << 64) - 1)
    x = ((x ^ (x >> 27)) * 0x94d049bb133111eb) & ((1 << 64) - 1)
    return (x ^ (x >> 31)) & MASK


def flat(addr):
    return (IDA_BASE[addr[0]] + addr[1]) & MASK


def ctype(text, aliases=None):
    """Context-free type parser (tests/backwards compatibility); the harness uses cdecls.Table."""
    ctx = cdecls.Context()
    ctx.scalar.update({'s8': 'signed char', 'u8': 'unsigned char', 's16': 'signed short', 'u16': 'unsigned short',
                       's32': 'signed int', 'u32': 'unsigned int', 'bool': 'unsigned char'})
    ctx.scalar.update(aliases or {})
    return ctx.ctype(text)


def copy_unit(unit, source=None):
    src = Path(source) if source else ROOT / json.loads((ROOT / 'config/units.json').read_text())['units'][unit]['source']
    OUT.mkdir(parents=True, exist_ok=True)
    dst = OUT / src.name
    if source:
        return src
    shutil.copyfile(src, dst)
    for p in cdecls.read_closure(src, {}):
        if p != src.resolve() and p.parent == src.parent.resolve():
            shutil.copyfile(p, OUT / p.name)
    return dst


_TABLE = None


def table():
    global _TABLE
    if _TABLE is None:
        _TABLE = cdecls.Table()
    return _TABLE


class Program:
    def __init__(self):
        self.img = Image(None)
        self.binds = match.Bindings(self.img)
        self.units = json.loads((ROOT / 'config/units.json').read_text())['units']
        self.symbols = json.loads((ROOT / 'config/symbols.json').read_text())[self.img.name]
        self.names = {flat(parse_addr(e['addr'])): n for n, e in self.symbols['functions'].items()}
        self.names.update({flat(a): match.demangle(n) for n, a in self.binds.lib.items()})
        self.data_syms = sorted((flat(parse_addr(e['addr'])), n) for n, e in self.symbols['data'].items())
        self.data_addrs = [a for a, _ in self.data_syms]
        self.images = {}
        for ob in self.img.meta['objects']:
            b = bytearray(ob['virtual_size'])
            b[:len(self.img.obj[ob['index']])] = self.img.obj[ob['index']]
            self.images[ob['index']] = b
        for (o, off), (kind, to, at) in self.img.fixups.items():
            if kind != 7:
                raise Limit(f'unsupported image fixup type {kind}')
            struct.pack_into('<I', self.images[o], off, flat((to, at)))
        self.image_ptrs = sorted(flat((o, a)) for (o, a) in self.img.fixups)
        self.bound_functions = sorted(a for a in self.names if IDA_BASE[1] <= a < IDA_BASE[1] + len(self.img.code))
        self.lib_starts = {flat(a) for a in self.binds.lib.values()}   # Watcom library members (libscan)
        # Pure Watcom -fpc floating-point / math runtime helpers are EXECUTED from the oracle image on both sides
        # (identical code, register-only interface), not stubbed: their results flow into later observables.
        self.transparent = sorted((a, self.oracle_end(a)) for a, n in self.names.items()
                                  if IDA_BASE[1] <= a < IDA_BASE[1] + len(self.img.code) and TRANSPARENT.match(n))
        self.transparent_starts = {a for a, _ in self.transparent}
        self.cache = {}
        self.table = table()

    def is_library_code(self, a):
        """Inside a Watcom library member (build/oracles/.../libsyms.json), not game code."""
        if not IDA_BASE[1] <= a < IDA_BASE[1] + len(self.img.code):
            return False
        k = bisect.bisect_right(self.bound_functions, a) - 1
        return k >= 0 and self.bound_functions[k] in self.lib_starts

    def in_transparent(self, a):
        k = bisect.bisect_right(self.transparent, (a, MASK)) - 1
        return k >= 0 and self.transparent[k][0] <= a < self.transparent[k][1]

    def data_symbol(self, addr):
        k = bisect.bisect_right(self.data_addrs, addr) - 1
        if k < 0:
            return None, 0
        a, n = self.data_syms[k]
        return n, addr - a

    def is_image_ptr(self, a, n):
        k = bisect.bisect_left(self.image_ptrs, a - 3)
        return k < len(self.image_ptrs) and self.image_ptrs[k] < a + n

    def compile(self, unit, source=None):
        src = copy_unit(unit, source)
        key = (unit, str(src), hashlib.sha256(src.read_bytes()).hexdigest())
        if key in self.cache:
            return self.cache[key]
        outdir = OUT / 'objects'
        outdir.mkdir(exist_ok=True)
        obj = outdir / (unit + '_' + key[2][:12] + '.obj')
        prof = wcc.profile(self.units[unit].get('profile', 'game'))
        if prof.get('install') != 'wc106':
            raise Limit('non-game compiler profile')
        rc, output = wcc.build(src, obj, prof['flags'], prof['install'])
        if rc:
            raise Limit('compile: ' + output[-1800:])
        mods = omf.load(obj)
        if len(mods) != 1:
            raise Limit('multiple OMF modules')
        mod = mods[0]
        segments = []
        verification = match.verify_object(self.img, mod, seg_report=segments)
        baseline = self.units[unit]['functions']
        losses = [r['function'] for r in verification if baseline.get(r['function'], {}).get('status') == 'STRICT' and r['status'] != 'STRICT']
        receipt = {'unit': unit, 'source': str(src.relative_to(ROOT)), 'source_sha256': key[2],
                   'object_sha256': hashlib.sha256(obj.read_bytes()).hexdigest(),
                   'strict_before': sum(e['status'] == 'STRICT' for e in baseline.values()),
                   'strict_after': sum(r['status'] == 'STRICT' for r in verification),
                   'strict_losses': losses, 'segments': segments,
                   'functions': [{k: r.get(k) for k in ('function', 'status')} for r in verification]}
        write_json(OUT / 'receipts' / (unit + '_' + key[2][:12] + '.json'), receipt)
        unit_table = cdecls.Table(files=[src])
        result = (mod, unit_table, receipt)
        self.cache[key] = result
        return result

    def oracle_end(self, start):
        return next((a for a in self.bound_functions if a > start), IDA_BASE[1] + len(self.img.code))


class Function:
    def __init__(self, program, unit, name, source=None, allow_losses=False):
        self.p, self.unit, self.name = program, unit, name
        mod, unit_table, receipt = program.compile(unit, source)
        self.receipt = receipt
        if receipt['strict_losses'] and not allow_losses:
            raise Limit('whole-unit STRICT losses: ' + ', '.join(receipt['strict_losses']))
        self.mod, self.unit_table = mod, unit_table
        self.call_contracts = {}       # synthetic address -> (Prototype, label)
        self.unknown_prototypes = {}   # name/address -> why
        cands = [p for p in unit_table.functions.get(name, []) if p.defined] or unit_table.functions.get(name, [])
        self.proto = cands[0] if cands else None
        if not self.proto or not self.proto.known or self.proto.ret.width is None or any(x.width is None for x in self.proto.args):
            raise Limit('entry prototype unknown or unsupported: ' + name)
        if self.proto.variadic:
            raise Limit('variadic entry function')
        self.stack_args = max(0, len(self.proto.args) - len(self.proto.parm))
        self.start = flat(program.binds.lookup(name)[0])
        self.end = program.oracle_end(self.start)
        pubs = [(n, s, o) for n, s, o, _ in mod.publics if match.demangle(n) == name]
        if len(pubs) != 1:
            raise Limit('candidate public not unique')
        _, self.si, self.off = pubs[0]
        seg = mod.segments[self.si]
        self.cstart = CAND + self.off
        self.cend = CAND + min([o for n, s, o, _ in mod.publics if s == self.si and o > self.off] + [seg.size])
        self.cbytes = bytearray(seg.data)
        self.resolutions = []
        bases = {}
        for si, s in enumerate(mod.segments):
            if not s or s.cls.upper() == 'CODE':
                continue
            votes = {(a[0], a[1] - o) for n, s2, o, _ in mod.publics if s2 == si for a, _ in [program.binds.lookup(n)] if a}
            if len(votes) == 1:
                bases[si] = next(iter(votes))
        match.infer_data_bases(program.img, mod, bases, program.binds,
                               [i for i, s in enumerate(mod.segments) if s and s.cls.upper() != 'CODE'])
        for r in receipt['segments']:
            if r['status'] == 'VERIFIED' and r.get('oracle'):
                si = next(i for i, s in enumerate(mod.segments) if s and s.name == r['segment'])
                bases[si] = parse_addr(r['oracle'])
        self.unresolved = []
        for f in mod.fixups:
            if f.seg != self.si:
                continue
            if f.size() != 4:
                self.unresolved.append((f.off, 'non-off32 fixup')); continue
            tm = f.target[0] & 3
            field = struct.unpack_from('<i', seg.data, f.off)[0]
            add = f.disp + (0 if f.self_rel else field)
            target = None
            if tm == 2:
                a, _ = program.binds.lookup(mod.target_name(f))
                if a:
                    target = flat((a[0], a[1] + add))
            elif tm == 0:
                tsi = f.target[1]
                if tsi == self.si:
                    public = next((n for n, s, o, _ in mod.publics if s == tsi and o == add), None)
                    if public and (f.self_rel or not (self.off <= add < self.cend - CAND)):
                        a, _ = program.binds.lookup(public)
                        if a: target = flat(a)
                    else:
                        target = CAND + add
                elif tsi in bases:
                    o, b = bases[tsi]
                    ds = mod.segments[tsi]
                    if ds.name == 'CONST' and bytes(ds.data) != program.img.obj[o][b:b + ds.size]:
                        self.unresolved.append((f.off, 'private CONST differs from oracle; refusing to substitute original constant'))
                        continue
                    target = flat((o, b + add))
                else:
                    ds = mod.segments[tsi]
                    if ds.cls.upper() != 'BSS' and not any(g.seg == tsi for g in mod.fixups) and all(ds.covered):
                        raw = bytes(ds.data)
                        locs = []
                        for ob, data in program.img.obj.items():
                            pos = data.find(raw)
                            if pos >= 0 and data.find(raw, pos + 1) < 0: locs.append((ob, pos))
                        if len(locs) == 1:
                            bases[tsi] = locs[0]; target = flat((locs[0][0], locs[0][1] + add))
            if target is None:
                self.unresolved.append((f.off, mod.target_name(f))); continue
            val = (target - (CAND + f.off + 4)) & MASK if f.self_rel else target & MASK
            struct.pack_into('<I', self.cbytes, f.off, val)
            self.resolutions.append({'site': f.off, 'target': target, 'symbol': mod.target_name(f), 'kind': f.kind()})
        self.ocode = bytes(program.images[1][self.start - IDA_BASE[1]:self.end - IDA_BASE[1]])
        self.oin, self.blocks, self.cfg_limits = self.cfg(self.start, self.end, False)
        self.cin, self.cblocks, self.ccfg_limits = self.cfg(self.cstart, self.cend, True)
        relevant = [(off, why) for off, why in self.unresolved if any(a <= CAND + off < a + i.size for a, i in self.cin.items())]
        if relevant:
            raise Limit('unresolved executed candidate fixups: ' + str(relevant[:6]))
        self.oaddrs = sorted(self.oin); self.caddrs = sorted(self.cin)
        self.oblocks = sorted(self.blocks); self.cblocks_sorted = sorted(self.cblocks)
        self.special = {False: self.specials(self.oin), True: self.specials(self.cin)}
        self.constants = list(dict.fromkeys(o.imm & MASK for i in sorted(self.oin.values(), key=lambda i: i.address) for o in i.operands
                                            if o.type == cs.CS_OP_IMM and not i.group(cs.CS_GRP_JUMP) and not i.group(cs.CS_GRP_CALL)
                                            and (1, i.address - IDA_BASE[1] + i.imm_offset) not in program.img.fixups))
        self.fnptr_slots = self.function_pointer_slots()
        self.mem_specs = self.memory_specs()
        self.global_guidance, self.field_guidance, self.bound_globals, self.bound_fields = self.guidance()

    # ---------------------------------------------------------------- static model
    def data_at(self, addr, size, candidate):
        if candidate and CAND <= addr < CAND + len(self.cbytes):
            return bytes(self.cbytes[addr - CAND:addr - CAND + size])
        for o, data in self.p.images.items():
            if IDA_BASE[o] <= addr < IDA_BASE[o] + len(data):
                return bytes(data[addr - IDA_BASE[o]:addr - IDA_BASE[o] + size])
        return b''

    def cfg(self, start, end, candidate):
        ins = {}; blocks = {start}; todo = [start]; limitations = []
        while todo:
            at = todo.pop()
            if not start <= at < end or at in ins: continue
            while start <= at < end and at not in ins:
                i = next(MD.disasm(self.data_at(at, 15, candidate), at, count=1), None)
                if i is None: limitations.append(f'undecodable {at:x}'); break
                ins[at] = i
                nxt = at + i.size
                if i.group(cs.CS_GRP_RET): break
                if i.group(cs.CS_GRP_JUMP):
                    op = i.operands[0]
                    if op.type == cs.CS_OP_IMM:
                        if start <= op.imm < end: blocks.add(op.imm); todo.append(op.imm)
                    elif op.type == cs.CS_OP_MEM and op.mem.index and not op.mem.base:
                        table_ = op.mem.disp & MASK
                        found = 0
                        for k in range(2048):
                            raw = self.data_at(table_ + k * 4, 4, candidate)
                            if len(raw) != 4: break
                            target = struct.unpack('<I', raw)[0]
                            if not start <= target < end: break
                            found += 1; blocks.add(target); todo.append(target)
                        if not found: limitations.append(f'unresolved indirect jump {at:x}')
                    else: limitations.append(f'unresolved indirect jump {at:x}')
                    if i.mnemonic == 'jmp': break
                    blocks.add(nxt); todo.append(nxt); break
                if i.group(cs.CS_GRP_CALL):
                    blocks.add(nxt)
                at = nxt
        return ins, blocks, limitations

    def shared_tail(self, address, candidate):
        """Instruction count of a straight pop*/ret epilogue at address (outside the function), else 0."""
        key = (address, candidate)
        cache = self.__dict__.setdefault('_tails', {})
        if key not in cache:
            n = 0; at = address; ok = False
            for _ in range(8):
                i = next(MD.disasm(self.data_at(at, 15, candidate), at, count=1), None)
                if i is None: break
                n += 1
                if i.group(cs.CS_GRP_RET): ok = True; break
                if i.mnemonic != 'pop' and not (i.mnemonic == 'add' and i.op_str.startswith('esp,')):
                    break
                at += i.size
            cache[key] = n if ok else 0
        return cache[key]

    @staticmethod
    def specials(ins):
        out = {}
        for a, i in ins.items():
            name = i.mnemonic.split()[-1]
            if i.group(cs.CS_GRP_CALL):
                out[a] = 'call'
            elif name in SYSTEM_MNEMONICS or any(o.type == cs.CS_OP_REG and i.reg_name(o.reg).startswith(('dr', 'cr')) for o in i.operands):
                out[a] = 'system'
            elif i.mnemonic in ('retf', 'iret', 'iretd', 'lcall', 'ljmp'):
                out[a] = 'segmented'
        return out

    def callee(self, target):
        """(Prototype or None, label, why)."""
        if target in self.call_contracts:
            p, label = self.call_contracts[target]
            return p, label, 'declared function-pointer contract'
        name = self.p.names.get(target)
        if not name:
            if target == 0:
                return None, None, 'call through a NULL function pointer (seed-chosen NULL)'
            return None, None, f'indirect target {target:#x} has no declared contract'
        p, why = self.p.table.prototype(name)
        if p is None or not p.known or p.ret.width is None or any(a.width is None for a in p.args):
            return None, name, why if p is None else 'incomplete prototype (' + why + ')'
        return p, name, why

    def function_pointer_slots(self):
        """Absolute addresses of declared function-pointer globals this function may load or call through."""
        refs = set()
        for i in list(self.oin.values()):
            for op in i.operands:
                if op.type == cs.CS_OP_MEM and not op.mem.base and IDA_BASE[2] <= (op.mem.disp & MASK) < IDA_BASE[2] + len(self.p.images[2]):
                    refs.add(op.mem.disp & MASK)
        for r in self.resolutions:
            if IDA_BASE[2] <= r['target'] < IDA_BASE[2] + len(self.p.images[2]):
                refs.add(r['target'])
        slots = {}
        tab = self.p.table
        for a in refs:
            name, off = self.p.data_symbol(a)
            if not name:
                continue
            base = a - off
            v = tab.variables.get(name)
            if not v:
                continue
            t, src, is_array = v
            if t.fnproto is not None:
                if is_array:
                    dims = re.findall(r'\[\s*(\d+|0x[0-9a-fA-F]+)\s*\]', t.text)
                    n = int(dims[0], 0) if dims else 1
                    for k in range(min(n, 1024)):
                        slots[base + 4 * k] = (t.fnproto, f'{name}[{k}]')
                else:
                    slots[base] = (t.fnproto, name)
            elif t.struct and t.struct in tab.struct_members:
                for moff, mname, mt in tab.struct_members[t.struct]:
                    if mt.fnproto is not None:
                        slots[base + moff] = (mt.fnproto, f'{name}.{mname}')
        return slots

    def memory_specs(self):
        specs = {}
        seq = sorted(self.oin.values(), key=lambda i: i.address)
        for k, i in enumerate(seq):
            if i.mnemonic == 'lea': continue
            for op in i.operands:
                if op.type != cs.CS_OP_MEM or op.mem.base or op.mem.index: continue
                a = op.mem.disp & MASK
                if not IDA_BASE[2] <= a < IDA_BASE[2] + len(self.p.images[2]): continue
                size = op.size; ptr = False
                if size == 4 and len(i.operands) > 1 and i.operands[0].type == cs.CS_OP_REG:
                    r = i.operands[0].reg
                    for j in seq[k + 1:k + 5]:
                        if j.mnemonic == 'sar' and j.operands[0].type == cs.CS_OP_REG and j.operands[0].reg == r and j.operands[1].type == cs.CS_OP_IMM:
                            n = j.operands[1].imm
                            if n in (16, 24): a += n // 8; size = 4 - n // 8
                            break
                        if any(o.type == cs.CS_OP_MEM and o.mem.base == r for o in j.operands): ptr = True; break
                        if r in j.regs_access()[1]: break
                if any(s <= a < s + 4 or a <= s < a + size for s in self.fnptr_slots):
                    continue
                specs[(a, size)] = ptr
        return [(a, n, p) for (a, n), p in specs.items()]

    def guidance(self):
        """Heuristic only: compare literals and loop bounds guide data generation, never verdicts."""
        gl = []; fields = []; state = {}; bounds = set(); bfields = set()
        self.divisor_globals = set(); self.divisor_fields = set()
        dmin, dmax = IDA_BASE[2], IDA_BASE[2] + len(self.p.images[2])
        def memloc(op):
            if op.type != cs.CS_OP_MEM: return None
            if not op.mem.base and not op.mem.index: return ('g', op.mem.disp & MASK, op.size)
            return None
        def fieldloc(i, op):
            if (op.type == cs.CS_OP_MEM and op.mem.base and i.reg_name(op.mem.base) not in ('esp', 'ebp')
                    and not op.mem.index and 0 <= op.mem.disp < 0x84):
                return (op.mem.disp, op.size)
            return None
        for i in sorted(self.oin.values(), key=lambda i: i.address):
            ops = i.operands
            if i.mnemonic == 'cmp' and len(ops) == 2:
                x, y = ops
                if y.type == cs.CS_OP_IMM:
                    val = y.imm & MASK
                    m = memloc(x); fl = fieldloc(i, x)
                    if m: gl.append((m[1], m[2], val))
                    elif fl: fields.append((fl[0], fl[1], val))
                    elif x.type == cs.CS_OP_REG and x.reg in state:
                        a, n = state[x.reg]; gl.append((a, min(n, x.size), val))
                else:
                    # register/memory comparisons: the memory side (or a register loaded from a global) is a bound
                    for o in (x, y):
                        m = memloc(o); fl = fieldloc(i, o)
                        if m: bounds.add((m[1], m[2]))
                        elif fl: bfields.add(fl)
                        elif o.type == cs.CS_OP_REG and o.reg in state: bounds.add(state[o.reg])
            if i.mnemonic in ('div', 'idiv') and ops:
                o = ops[0]
                m = memloc(o); fl = fieldloc(i, o)
                if m: self.divisor_globals.add((m[1], m[2]))
                elif fl: self.divisor_fields.add(fl)
                elif o.type == cs.CS_OP_REG and o.reg in state: self.divisor_globals.add(state[o.reg])
            new = None
            if ops and ops[0].type == cs.CS_OP_REG:
                if i.mnemonic in ('mov', 'movzx', 'movsx') and len(ops) > 1:
                    o = ops[1]
                    if o.type == cs.CS_OP_MEM and not o.mem.base and not o.mem.index:
                        new = (o.mem.disp & MASK, o.size)
                    elif o.type == cs.CS_OP_REG: new = state.get(o.reg)
                elif i.mnemonic == 'sar' and ops[0].reg in state and ops[1].type == cs.CS_OP_IMM:
                    a, n = state[ops[0].reg]; shift = ops[1].imm
                    if shift in (16, 24): new = (a + shift // 8, n - shift // 8)
                if ops[0].reg in i.regs_access()[1]:
                    state.pop(ops[0].reg, None)
                    if new: state[ops[0].reg] = new
            if i.group(cs.CS_GRP_CALL): state.clear()
        valid = lambda x: dmin <= x[0] < dmax and x[1] in (1, 2, 4) and not any(s <= x[0] < s + 4 for s in self.fnptr_slots)
        return (list(dict.fromkeys(x for x in gl if valid(x))), list(dict.fromkeys(fields)),
                sorted(x for x in bounds if valid(x)), sorted(bfields))


class Inputs:
    def __init__(self, f, seed, scenario=None, overrides=None, domain=None):
        self.f, self.seed = f, seed
        self.scenario = scenario or 'guided'
        self.overrides = {} if overrides is None else dict(overrides)
        self.initialized = set()
        self.page_cache = {}
        self.fresh_next = FRESH
        self.domain_applied = []
        self.consts = list(dict.fromkeys(f.constants + [0, 1, 2, 3, 4, 7, 8, 15, 16, 31, 32, 63, 64, 127, 128, 255, 256, 32767, 32768, 65535, MASK]))
        self.regs = {n: self.value(k * 4, 32) for k, n in enumerate(REGS)}
        self.regs['esp'] = STACK_TOP
        self.stack_args = []
        for k, t in enumerate(f.proto.args):
            v = self.argument_value(k, t)
            if k < len(f.proto.parm):
                reg = f.proto.parm[k]
                if reg not in REGID: raise Limit('unsupported entry register ' + reg)
                self.regs[reg] = v
            else:
                self.stack_args.append(v)
        self.regs['ebp'] = 0x13579bdf ^ mix(seed, 0, 9)
        self.put(STACK_TOP, STOP, 4)
        self.initialized.update(range(STACK_TOP, STACK_TOP + 4))
        for k, v in enumerate(self.stack_args):
            self.put(STACK_TOP + 4 + k * 4, v, 4); self.initialized.update(range(STACK_TOP + 4 + k * 4, STACK_TOP + 8 + k * 4))
        # Image overrides only at selected referenced scalars, avoiding fixup pointers.
        for a, n, p in f.mem_specs:
            if f.p.is_image_ptr(a, n): continue
            self.put(a, HEAP + 0x10000 if p and seed % 4 else self.value(a, n * 8), n)
        # Loop bounds: small values on most seeds (a global compared against a register/memory operand).
        for a, n in f.bound_globals:
            if f.p.is_image_ptr(a, n) or seed % 4 == 3: continue
            self.put(a, self.small(a), n)
        # Divisors loaded from globals: nonzero on 7/8 of the seeds (a zero divisor faults on both sides).
        for a, n in f.divisor_globals:
            if f.p.is_image_ptr(a, n) or seed % 8 == 7 or not IDA_BASE[2] <= a < IDA_BASE[2] + len(f.p.images[2]): continue
            self.put(a, 1 + self.small(a + 1) % 24, n)
        if f.global_guidance:
            a, n, v = f.global_guidance[(seed // 3) % len(f.global_guidance)]
            if not f.p.is_image_ptr(a, n):
                self.put(a, v + ((0, -1, 1)[seed % 3]), n)
        self.function_pointers()
        self.object_graph()
        if self.scenario == 'b20':
            self.regs['edi'] = 0
        if domain:
            domain.apply(self)

    def argument_value(self, k, t):
        if t.fnproto is not None:
            addr = FNPTR + 0x100000 + k * 0x10
            self.f.call_contracts[addr] = (t.fnproto, f'argument {k} callback')
            return addr
        if t.pointer:
            return HEAP + 0x10000 + k * 0x10000 if k < 4 else HEAP + 0x90000 + (k - 4) * 0x10000
        return self.value(k if k < 4 else k + 8, t.width or 32)

    def small(self, addr):
        h = mix(self.seed, addr, 77)
        if h % 8 == 0 and self.f.constants:
            return self.f.constants[(h >> 3) % len(self.f.constants)] & 0xffff
        return (h >> 3) % 25

    def value(self, addr, bits=32, uniform=True):
        h = mix(self.seed, addr)
        if uniform and self.seed % 8 == 0: v = self.consts[(self.seed // 8) % len(self.consts)]
        elif h % 16 < 3: v = 0
        elif h % 16 < 5: v = 1
        elif h % 16 < 7: v = MASK
        elif h % 16 < 11: v = self.consts[(h >> 4) % len(self.consts)]
        elif h % 16 < 15: v = ((h >> 4) % 33) - 16
        else: v = h
        return v & ((1 << bits) - 1)

    def stream(self, key, n):
        """Deterministic file/stream contents: little-endian words with the scalar value distribution."""
        import random
        rng = random.Random(key)
        out = bytearray(rng.randbytes(n + 4))
        for k in range(0, n, 4):
            r = out[k] % 8
            if r < 3:      # small word
                out[k:k + 4] = (out[k + 1] % 25).to_bytes(4, 'little')
            elif r < 4:
                out[k:k + 4] = b'\0\0\0\0'
        return bytes(out[:n])

    def put(self, a, v, n):
        for k in range(n): self.overrides[(a + k) & MASK] = (v >> (8 * k)) & 255

    def global_addr(self, name):
        a = self.f.p.binds.game.get(name)
        return flat(a) if a else None

    def function_pointers(self):
        """Declared function-pointer globals hold either their image value (a named function) or a synthetic
        identity with the declared contract; NULL on some seeds."""
        for k, (slot, (proto, label)) in enumerate(sorted(self.f.fnptr_slots.items())):
            cur = struct.unpack('<I', self.f.data_at(slot, 4, False).ljust(4, b'\0'))[0]
            if cur in self.f.p.names:
                continue
            addr = FNPTR + k * 0x10
            self.f.call_contracts[addr] = (proto, 'fnptr:' + label)
            self.put(slot, 0 if mix(self.seed, slot, 5) % 8 == 0 else addr, 4)

    def object_graph(self):
        level = self.global_addr('level')
        if level is not None:
            self.put(level, HEAP + 0x10000, 4)
            self.put(level + 4, 2 if self.scenario == 'b19' else self.seed % 9, 2)
        ray = self.global_addr('ray')
        objs = [HEAP + 0x10000 + i * 0x84 for i in range(12)]
        objs += [v for k, v in self.regs.items() if k != 'esp' and HEAP <= v < HEAP + 0x100000]
        if ray: objs.append(ray)
        for ix, a in enumerate(sorted(set(objs))):
            for off in range(0x34, 0x80, 2): self.put(a + off, self.value(a + off, 16), 2)
            for off in (0x24, 0x28, 0x2c, 0x30): self.put(a + off, self.value(a + off), 4)
            for off in (0, 4, 8, 12, 16, 20): self.put(a + off, HEAP + 0x200000 + off * 0x1000, 4)
            self.put(a + 0x34, ((a - HEAP - 0x10000) // 0x84) % 12 if a < HEAP + 0x11000 else ix, 2)
            self.put(a + 0x60, self.f.constants[(self.seed + ix) % len(self.f.constants)] if self.f.constants else self.seed % 256, 2)
            if self.f.field_guidance:
                off, n, v = self.f.field_guidance[(self.seed // 3 + ix) % len(self.f.field_guidance)]
                self.put(a + off, v + (0, -1, 1)[self.seed % 3], n)
            if self.seed % 4 != 3:
                for off, n in self.f.bound_fields:
                    if off >= 0x34 or off in (0x24, 0x28, 0x2c, 0x30):
                        self.put(a + off, self.small(a + off), n)
            if self.seed % 8 != 7:
                for off, n in self.f.divisor_fields:
                    self.put(a + off, 1 + self.small(a + off + 1) % 24, n)
            if self.scenario == 'b19':
                self.put(a + 0x60, 39, 2); self.put(a + 0x34, ix % 2, 2)
            if self.scenario == 'b20' and a == self.regs.get('eax'):
                self.put(a + 0x60, 94, 2); self.put(a + 0x40, 1, 2); self.put(a + 0x42, 1, 2)
            self.put(a + 0x81, self.value(a + 0x81, 8), 1)
            self.put(a + 0x82, self.value(a + 0x82, 8), 1)
        for ix in range(256): self.put(HEAP + 0x200000 + 12 * 0x1000 + ix * 4, HEAP + 0x300000 + ix * 0x1000, 4)

    def page(self, page):
        """Initial content of one page: image objects, candidate code, the STOP page or generated memory,
        with the explicit overrides applied.  Both executions materialise pages lazily from this."""
        if page in self.page_cache:
            return self.page_cache[page]
        out = None
        for o, data in self.f.p.images.items():
            if IDA_BASE[o] <= page < IDA_BASE[o] + len(data):
                out = bytearray(data[page - IDA_BASE[o]:page - IDA_BASE[o] + PAGE]).ljust(PAGE, b'\0')
        if out is None and CAND <= page < CAND + len(self.f.cbytes):
            out = bytearray(self.f.cbytes[page - CAND:page - CAND + PAGE]).ljust(PAGE, b'\0')
        if out is None and page == STOP:
            out = bytearray(PAGE)
        if out is None:
            out = bytearray(PAGE)
            for k in range(0, PAGE, 4):
                a = page + k
                v = HEAP + 0x400000 + (mix(self.seed, a) % 128) * PAGE if mix(self.seed, a, 1) % 8 == 0 else self.value(a)
                struct.pack_into('<I', out, k, v)
        for a in self.page_overrides().get(page, ()):
            out[a - page] = self.overrides[a]
        out = bytes(out)
        if len(self.page_cache) < 4096:
            self.page_cache[page] = out
        return out

    def page_overrides(self):
        key = len(self.overrides)
        if getattr(self, '_po_key', None) != key:
            po = {}
            for a in self.overrides: po.setdefault(a & ~(PAGE - 1), []).append(a)
            self._po, self._po_key = po, key
            self.page_cache = {}
        return self._po

    def initial_byte(self, a):
        """The byte both executions see at address a before the function runs."""
        if a in self.overrides:
            return self.overrides[a]
        return self.page(a & ~(PAGE - 1))[a & (PAGE - 1)]

    def read_initial(self, a, n):
        return int.from_bytes(bytes(self.initial_byte((a + k) & MASK) for k in range(n)), 'little')

    def fresh(self, size):
        a = self.fresh_next
        self.fresh_next += (size + PAGE - 1) // PAGE * PAGE + PAGE
        return a

    def summary(self):
        packed = sorted(self.overrides.items())
        return {'seed': self.seed, 'scenario': self.scenario, 'registers': self.regs,
                'overrides': packed, 'constant_pool': self.consts,
                'initialized_stack_bytes': sorted(self.initialized), 'domain_applied': self.domain_applied,
                'eflags': 0x202, 'memory_generator': GENERATOR}

    @classmethod
    def restore(cls, f, summary):
        if summary.get('memory_generator') != GENERATOR: raise Limit('counterexample uses an older generator; remeasure')
        x = cls(f, summary['seed'], summary['scenario'])
        x.regs = dict(summary['registers']); x.overrides = {int(a): v for a, v in summary['overrides']}
        x.consts = summary['constant_pool']; x.initialized = set(summary['initialized_stack_bytes'])
        x.page_cache = {}; x._po_key = None
        return x


def regread(machine, name):
    parent = PARENT.get(name, name)
    if parent not in REGID: raise Limit('unsupported ABI register ' + name)
    v = machine.reg_read(REGID[parent])
    if name.endswith('h') and len(name) == 2: return (v >> 8) & 255
    if len(name) == 2: return v & (255 if name.endswith('l') else 65535)
    return v


def regwrite(machine, name, v):
    parent = PARENT.get(name, name)
    if parent not in REGID: raise Limit('unsupported ABI register ' + name)
    old = machine.reg_read(REGID[parent])
    if name.endswith('h') and len(name) == 2: v = (old & ~0xff00) | ((v & 255) << 8)
    elif len(name) == 2:
        mask = 255 if name.endswith('l') else 65535
        v = (old & ~mask) | (v & mask)
    machine.reg_write(REGID[parent], v & MASK)


def format_words(fmt):
    """Number of 4-byte stack words a printf-family format consumes, or None when unsupported."""
    words = 0
    for m in re.finditer(r'%([-+ #0]*)(\*|\d+)?(?:\.(\*|\d+))?([hlLNF]?)([a-zA-Z%])', fmt):
        conv = m[5]
        if conv == '%':
            continue
        if m[2] == '*' or m[3] == '*':
            return None
        if conv in 'diouxXcsSpn':
            words += 1
        elif conv in 'eEfgG':
            words += 2 if m[4] != 'L' else 3
        else:
            return None
    return words


class Execution:
    def __init__(self, f, inputs, candidate=False, steps=TIERS[0], timeout_us=20000000, track_reads=False):
        self.f, self.inputs, self.candidate = f, inputs, candidate
        self.machine = uc.Uc(uc.UC_ARCH_X86, uc.UC_MODE_32)
        self.pages = set(); self.dirty = set(); self.stack_written = set(inputs.initialized)
        self.pending = set(); self.boundaries = []
        self.uninit = []; self.log = []; self.visited = set(); self.count = 0
        self.discarded_stack_reads = []
        self.input_reads = set(); self.writers = {}
        self.system_state = {}
        self.frame_ids = {}
        self.status = 'RUNNING'; self.reason = None; self.limit_class = None; self.steps = steps
        self.max_events = MAX_EVENTS[TIERS.index(steps)] if steps in TIERS else 12000
        self.unknown = []
        self.smc = []
        self.in_helper = False
        self.ins = f.cin if candidate else f.oin
        self.iaddrs = f.caddrs if candidate else f.oaddrs
        self.bstarts = f.cblocks_sorted if candidate else f.oblocks
        self.fstart, self.fend = (f.cstart, f.cend) if candidate else (f.start, f.end)
        self.argarea = (STACK_TOP + 4, STACK_TOP + 4 + 4 * f.stack_args)
        # Every page (image, candidate code, stack, generated memory) is materialised lazily from
        # Inputs.page(), so both executions see identical bytes everywhere, including both code mappings.
        for a in (self.fstart, STACK_TOP - PAGE, STACK_TOP, STOP):
            self.ensure(a)
        for n, v in inputs.regs.items(): self.machine.reg_write(REGID[n], v)
        self.machine.reg_write(ux.UC_X86_REG_EFLAGS, 0x202)
        self.machine.reg_write(ux.UC_X86_REG_FPCW, 0x037f)
        self.machine.hook_add(uc.UC_HOOK_MEM_UNMAPPED, self.on_unmapped)
        self.machine.hook_add(uc.UC_HOOK_MEM_WRITE, self.on_write)
        self.machine.hook_add(uc.UC_HOOK_MEM_READ, self.on_stack_read, begin=STACK_LO, end=STACK_HI - 1)
        if track_reads:
            self.machine.hook_add(uc.UC_HOOK_MEM_READ, self.on_read)
        self.machine.hook_add(uc.UC_HOOK_BLOCK, self.on_block)
        for a in f.special[candidate]:
            self.machine.hook_add(uc.UC_HOOK_CODE, self.on_special, begin=a, end=a)
        self.machine.hook_add(uc.UC_HOOK_INTR, self.on_intr)
        try:
            self.machine.emu_start(self.fstart, STOP, timeout=timeout_us)
            if self.status == 'RUNNING':
                ip = self.machine.reg_read(ux.UC_X86_REG_EIP)
                if ip == STOP: self.status = 'RETURN'
                else: self.set_limit('wall-clock emulator timeout', 'timeout')
        except uc.UcError as e:
            if self.status == 'RUNNING':
                self.set_limit(str(e), 'emulator')
        except Limit as e:
            self.set_limit(str(e), 'model')
        self.return_value = regread(self.machine, f.proto.value[0] if f.proto.value else 'eax')
        if f.proto.ret.width is not None:
            self.return_value &= (1 << f.proto.ret.width) - 1 if f.proto.ret.width else 0

    # ------------------------------------------------------------ termination helpers
    def set_limit(self, why, cls):
        if self.status in ('RUNNING',):
            self.status = 'LIMIT'; self.reason = why; self.limit_class = cls

    def halt(self, status, why=None, cls=None):
        if self.status == 'RUNNING':
            self.status = status; self.reason = why; self.limit_class = cls
        self.machine.reg_write(ux.UC_X86_REG_EIP, STOP)
        self.machine.emu_stop()

    def ensure(self, a):
        p = a & ~(PAGE - 1)
        if p not in self.pages:
            if len(self.pages) > 4096: raise Limit('materialised page budget')
            self.pages.add(p); self.machine.mem_map(p, PAGE)
            self.machine.mem_write(p, self.inputs.page(p))

    def map_range(self, start, size, fill=False):
        for p in range(start & ~(PAGE - 1), (start + size + PAGE - 1) & ~(PAGE - 1), PAGE):
            if p in self.pages: continue
            self.pages.add(p); self.machine.mem_map(p, PAGE)
            if fill: self.machine.mem_write(p, self.inputs.page(p))

    # ------------------------------------------------------------ hooks
    def on_unmapped(self, m, access, address, size, value, data):
        try:
            for a in range(address, address + size): self.ensure(a & MASK)
            return True
        except Limit as e:
            self.set_limit(str(e), 'pages'); return False

    def on_write(self, m, access, address, size, value, data):
        end = address + size
        if STACK_LO <= address < STACK_TOP:
            self.stack_written.update(range(address, end))
            return
        if self.argarea[0] <= address < self.argarea[1]:
            # The function's own stack parameters are dead after return (callee-owned); not observable.
            self.stack_written.update(range(address, end))
            return
        pc = m.reg_read(ux.UC_X86_REG_EIP)
        if self.fstart <= address < self.fend or address < self.fend <= end:
            self.smc.append({'pc': pc, 'offset': address - self.fstart, 'size': size, 'value': value & ((1 << (8 * size)) - 1)})
            self.halt('LIMIT', f'self-modifying write into the executing function at +{address - self.fstart:#x}', 'smc')
            return
        rng = range(address, end)
        self.dirty.update(rng); self.pending.update(rng)
        for a in rng: self.writers[a] = pc

    def on_stack_read(self, m, access, address, size, value, data):
        if access != uc.UC_MEM_READ or address >= STACK_TOP + 4 + 4 * self.f.stack_args:
            return
        missing = [a for a in range(address, address + size) if a not in self.stack_written]
        if not missing:
            return
        pc = m.reg_read(ux.UC_X86_REG_EIP)
        ins = self.ins.get(pc)
        if ins and ins.mnemonic == 'mov' and size == 4 and ins.operands[0].type == cs.CS_OP_REG:
            nextins = self.ins.get(pc + ins.size)
            if (nextins and nextins.mnemonic == 'sar' and nextins.operands[0].type == cs.CS_OP_REG
                    and nextins.operands[0].reg == ins.operands[0].reg and nextins.operands[1].type == cs.CS_OP_IMM
                    and nextins.operands[1].imm in (16, 24)):
                n = nextins.operands[1].imm // 8
                dropped = [a for a in missing if a < address + n]
                if dropped and len(self.discarded_stack_reads) < 32:
                    self.discarded_stack_reads.append({'pc': pc, 'sp_offset': address - STACK_TOP, 'discarded_bytes': len(dropped)})
                missing = [a for a in missing if a >= address + n]
        if missing and len(self.uninit) < 64:
            self.uninit.append({'pc': pc, 'sp_offset': address - STACK_TOP, 'size': size,
                                'bytes': [a - STACK_TOP for a in missing], 'value': bytes(m.mem_read(address, size)).hex()})

    def on_read(self, m, access, address, size, value, data):
        self.input_reads.update(a for a in range(address, address + size) if a not in self.dirty)

    def on_intr(self, m, intno, data):
        pc = m.reg_read(ux.UC_X86_REG_EIP)
        self.fault = {'vector': intno, 'pc': pc}
        self.halt('FAULT', f'CPU exception vector {intno}', 'fault')

    def on_block(self, m, address, size, data):
        if address == STOP:
            return
        if not self.fstart <= address < self.fend:
            if self.f.p.in_transparent(address):
                self.in_helper = True
                self.count += max(1, size // 3)
                if self.count > self.steps: self.halt('BUDGET')
                return
            if self.in_helper and self.f.p.is_library_code(address):
                self.count += max(1, size // 3)
                if self.count > self.steps: self.halt('BUDGET')
                return
            if self.in_helper:
                # e.g. __matherr / __get_errno_ptr_: the runtime's math-error path (out-of-domain argument)
                self.halt('LIMIT', f'runtime helper left the library (math-error path at {address:x})', 'mathlib')
                return
            if address in self.f.p.names:
                try: self.call(address, tail=True)
                except Limit as e: self.halt('LIMIT', str(e), 'model'); return
                self.halt('RETURN')
            elif self.f.shared_tail(address, self.candidate):
                # A compiler-shared epilogue (pops + ret) next to the function: part of its control flow.
                self.count += self.f.shared_tail(address, self.candidate)
            else:
                self.halt('LIMIT', f'control escaped function at {address:x}', 'control')
            return
        self.in_helper = False
        if address not in self.ins:
            self.halt('LIMIT', f'execution reached {address:x} outside the static instruction set', 'control')
            return
        lo = bisect.bisect_left(self.iaddrs, address); hi = bisect.bisect_left(self.iaddrs, address + size)
        self.count += max(1, hi - lo)
        if self.count > self.steps:
            self.halt('BUDGET')
            return
        k = bisect.bisect_left(self.bstarts, address)
        while k < len(self.bstarts) and self.bstarts[k] < address + size:
            self.visited.add(self.bstarts[k]); k += 1

    def on_special(self, m, address, size, data):
        if self.status != 'RUNNING':
            return
        ins = self.ins.get(address)
        if ins is None:
            return
        kind = self.f.special[self.candidate].get(address)
        try:
            if kind == 'call':
                target = self.operand_value(ins.operands[0], ins)
                if CAND <= target < CAND + len(self.f.cbytes):
                    public = next((n for n, s, o, _ in self.f.mod.publics if s == self.f.si and CAND + o == target), None)
                    if not public: raise Limit('call to private candidate label without oracle identity')
                    a, _ = self.f.p.binds.lookup(public)
                    if not a: raise Limit('unbound intra-unit callee')
                    target = flat(a)
                if target in self.f.p.transparent_starts:
                    return   # executed natively from the oracle image (pure runtime helper)
                self.call(target)
                if self.status == 'RUNNING':
                    m.reg_write(ux.UC_X86_REG_EIP, address + size)
            elif kind == 'system':
                self.system(ins)
                if self.status == 'RUNNING':
                    m.reg_write(ux.UC_X86_REG_EIP, address + size)
            elif kind == 'segmented':
                self.halt('LIMIT', 'segmented control transfer ' + ins.mnemonic, 'control')
        except Limit as e:
            self.halt('LIMIT', str(e), 'pages' if 'page budget' in str(e) else 'model')

    # ------------------------------------------------------------ calls
    def operand_value(self, op, ins):
        if op.type == cs.CS_OP_IMM: return op.imm & MASK
        if op.type == cs.CS_OP_REG: return regread(self.machine, ins.reg_name(op.reg))
        if op.type == cs.CS_OP_MEM:
            a = op.mem.disp
            if op.mem.base: a += regread(self.machine, ins.reg_name(op.mem.base))
            if op.mem.index: a += regread(self.machine, ins.reg_name(op.mem.index)) * op.mem.scale
            a &= MASK
            for k in range(op.size): self.ensure((a + k) & MASK)
            return int.from_bytes(self.machine.mem_read(a, op.size), 'little')
        raise Limit('unsupported indirect call operand')

    def identity(self, v):
        """Layout-independent identity of a pointer value."""
        v &= MASK
        if STACK_LO <= v < STACK_TOP:
            if v not in self.frame_ids:
                self.frame_ids[v] = len(self.frame_ids)
            return {'frame_local': self.frame_ids[v]}
        if STACK_TOP <= v < STACK_HI:
            return {'entry_frame': v - STACK_TOP}
        if CAND <= v < CAND + len(self.f.cbytes):
            off = v - CAND
            public = next((n for n, s, o, _ in self.f.mod.publics if s == self.f.si and o == off), None)
            if public:
                a, _ = self.f.p.binds.lookup(public)
                if a: return flat(a)
            return {'candidate_code': off} if self.candidate else v
        return v

    def flush_boundary(self):
        vals = {}
        for start, end in ranges(self.pending):
            try:
                raw = bytes(self.machine.mem_read(start, end - start))
            except uc.UcError:
                continue
            vals.update(zip(range(start, end), raw))
        self.boundaries.append(vals)
        self.pending = set()

    def read_bytes(self, a, n):
        for k in range(n): self.ensure((a + k) & MASK)
        return bytes(self.machine.mem_read(a, n))

    def call(self, target, tail=False):
        p, label, why = self.f.callee(target)
        regs = {n: regread(self.machine, n) for n in REGS[:-1]}
        sp = regread(self.machine, 'esp')
        argsp = sp + 4 if tail else sp
        raw_stack = []
        for k in range(8):
            raw_stack.append(int.from_bytes(self.read_bytes(argsp + k * 4, 4), 'little'))
        self.flush_boundary()
        args = []; pointees = []; known = p is not None
        stack_count = 0
        writes = []
        if p is not None:
            words = []
            nreg = 0 if p.variadic else len(p.parm)
            for k, t in enumerate(p.args):
                if k < nreg:
                    rn = p.parm[k]
                    if len(rn.split()) != 1 or PARENT.get(rn, rn) not in REGID:
                        known = False; why = 'unsupported register contract ' + rn; break
                    v = regread(self.machine, rn)
                else:
                    idx = k - nreg; stack_count = max(stack_count, idx + 1)
                    if idx >= 8: known = False; why = 'more than 8 stack arguments'; break
                    v = raw_stack[idx]
                args.append(self.normalize(v, t))
                words.append((k, t, v))
            if known and p.variadic:
                fmt_arg = next(((k, v) for k, t, v in words if t.pointer and t.const_pointee and t.pointee and t.pointee.width == 8), None)
                n = None
                if fmt_arg:
                    raw = self.read_bytes(fmt_arg[1], 256)
                    n = format_words(raw.split(b'\0', 1)[0].decode('latin-1'))
                if n is None or len(p.args) + n > 8:
                    known = False; why = 'variadic call with unsupported or unreadable format'
                else:
                    for k in range(n):
                        idx = len(p.args) + k
                        args.append(self.normalize(raw_stack[idx], CType('vararg word', 32)))
                    stack_count = len(p.args) + n
            if known:
                for k, t, v in words:
                    if not t.pointer or not v or t.fnproto is not None or t.pointee is None:
                        continue
                    w = t.pointee.width
                    if w in (8, 16, 32):
                        n = w // 8
                        if t.const_pointee and w == 8:
                            n = 64   # a const char* is compared as a string (up to NUL)
                        raw = self.read_bytes(v, n)
                        defined = [not (STACK_LO <= (v + b) & MASK < STACK_TOP) or ((v + b) & MASK) in self.stack_written for b in range(n)]
                        if n == 64:
                            cut = raw.find(b'\0')
                            cut = 64 if cut < 0 else cut + 1
                            upto = next((b for b in range(cut) if not defined[b]), cut)
                            pointees.append({'arg': k, 'string': raw[:upto].hex(), 'complete': upto == cut})
                        else:
                            pointees.append({'arg': k, 'defined': all(defined), 'value': int.from_bytes(raw, 'little')})
                        if not t.const_pointee:
                            writes.append((k, v, w // 8, t.pointee))
        else:
            self.unknown.append(label or hex(target))
        self.log.append({'kind': 'call', 'target': target, 'name': label or self.f.p.names.get(target),
                         'args': args, 'known': known, 'why': None if known else why, 'pointees': pointees,
                         'registers': regs, 'stack_words': raw_stack, 'stack_arg_count': stack_count})
        ix = len(self.log) - 1
        seed = self.inputs.seed
        if ix >= self.max_events:
            self.halt('BUDGET')
        lib_result = None
        if known and label in LIB_EFFECTS:
            lib_result = self.library_effect(label, [v for _, _, v in words] if p.variadic else [w for _, _, w in words], raw_stack, ix, target)
            writes = []
        if known:
            # Output parameters: deterministic values of the pointee's declared type (same on both sides).
            for k, v, n, pt in writes:
                if pt.pointer:
                    val = HEAP + 0x500000 + (mix(seed, target, ix * 16 + k) % 64) * 0x100
                else:
                    val = self.inputs.value(mix(seed, target ^ (ix << 8), 300 + k), n * 8, uniform=False)
                self.stub_write(v, val.to_bytes(4, 'little')[:n])
        width = p.ret.width if p else 32
        result = self.inputs.value(mix(seed, target, ix + 1), 32, uniform=False)
        if p and p.ret.pointer:
            result = HEAP + 0x500000 + (mix(seed, target, ix) % 16) * 0x1000
        elif p and width and width < 32:
            low = result & ((1 << width) - 1)
            if seed % 2 == 0:
                # declared width, properly extended by signedness
                if p.ret.signed and low >> (width - 1): result = (low - (1 << width)) & MASK
                else: result = low
            else:
                # declared width; upper bits are callee-defined garbage (identical on both sides)
                result = (mix(seed, target, ix + 7) & ~((1 << width) - 1) | low) & MASK
        if known and label == 'myRand':
            # STRICT myRand (src/rand.c:13): RandArray[i] % (max + 1), RandArray from rand() in 0..32767.
            m = args[0] - 0x10000 if args and args[0] >= 0x8000 else (args[0] if args else -1)
            if m >= 0:
                lib_result = (mix(seed, target, ix + 1) & 0x7fff) % (m + 1)
        if lib_result is not None:
            result = lib_result & MASK
        modify = p.modify if p else ['eax']
        value = p.value if p else ['eax']
        for rn in dict.fromkeys(modify + value):
            regwrite(self.machine, rn, result if rn in value else mix(seed, target, ix + 1))
        if stack_count and p and not p.cdecl and not tail:
            self.machine.reg_write(REGID['esp'], sp + 4 * stack_count)

    def library_effect(self, name, args, raw_stack, ix, target):
        """C-library semantics for the few library callees whose effect is a memory write (bounded)."""
        seed = self.inputs.seed
        cap = 65536
        def cstr(a, limit=4096):
            out = bytearray()
            while len(out) < limit:
                b = self.read_bytes((a + len(out)) & MASK, 1)
                if b == b'\0': break
                out += b
            return bytes(out)
        if name in ('memcpy', 'memmove'):
            d, src, n = args[0], args[1], min(args[2], cap)
            if n: self.stub_write(d, self.read_bytes(src, n))
            return d
        if name == 'memset':
            d, c, n = args[0], args[1], min(args[2], cap)
            if n: self.stub_write(d, bytes([c & 255]) * n)
            return d
        if name == 'strcpy':
            self.stub_write(args[0], cstr(args[1]) + b'\0'); return args[0]
        if name == 'strncpy':
            n = min(args[2], cap); src = cstr(args[1], n)
            self.stub_write(args[0], (src + b'\0' * n)[:n]) if n else None; return args[0]
        if name == 'strcat':
            d = args[0]; self.stub_write(d + len(cstr(d)), cstr(args[1]) + b'\0'); return d
        if name in ('fread', 'read'):
            buf, n = (args[0], args[1] * args[2]) if name == 'fread' else (args[1], args[2])
            n = min(n & MASK, cap)
            data = self.inputs.stream(mix(seed, ix, target), n)
            if n: self.stub_write(buf, data)
            items = args[2] if name == 'fread' else n
            return items if mix(seed, ix, 61) % 4 else mix(seed, ix, 62) % (items + 1)
        if name == 'sprintf':
            text = ('S%04x' % (mix(seed, ix, target) & 0xffff)).encode()
            self.stub_write(args[0], text + b'\0')
            return len(text)
        return None

    def stub_write(self, a, data):
        for k in range(len(data)): self.ensure((a + k) & MASK)
        self.machine.mem_write(a, data)
        rng = range(a, a + len(data))
        if STACK_LO <= a < STACK_HI:
            self.stack_written.update(rng)
        else:
            self.dirty.update(rng)
            for x in rng: self.writers[x] = 'stub'

    def normalize(self, v, t):
        if t.pointer:
            return self.identity(v)
        return v & ((1 << t.width) - 1) if t.width else 0

    def system(self, ins):
        name = ins.mnemonic.split()[-1]
        values = []
        if name == 'into' and not self.machine.reg_read(ux.UC_X86_REG_EFLAGS) & 0x800:
            return
        self.flush_boundary()
        sysregs = [o for o in ins.operands if o.type == cs.CS_OP_REG and ins.reg_name(o.reg).startswith(('dr', 'cr'))]
        if sysregs:
            if name != 'mov' or len(sysregs) != 1: raise Limit('unsupported system-register operation ' + ins.op_str)
            rn = ins.reg_name(sysregs[0].reg)
            if ins.operands[0].reg == sysregs[0].reg:
                v = self.operand_value(ins.operands[1], ins); self.system_state[rn] = v
                self.log.append({'kind': 'system', 'operation': 'write_' + rn, 'args': [v]})
            else:
                v = self.system_state.get(rn, mix(self.inputs.seed, sum(rn.encode()), 93))
                regwrite(self.machine, ins.reg_name(ins.operands[0].reg), v)
                self.log.append({'kind': 'system', 'operation': 'read_' + rn, 'args': [], 'result': v})
            return
        if name in ('insb', 'insw', 'insd', 'outsb', 'outsw', 'outsd'):
            unit = {'b': 1, 'w': 2, 'd': 4}[name[-1]]
            rep = ins.mnemonic.startswith('rep')
            count = regread(self.machine, 'ecx') if rep else 1
            if count * unit > 65536: raise Limit('port-string byte budget')
            out = name.startswith('out'); rn = 'esi' if out else 'edi'
            addr = regread(self.machine, rn); direction = -1 if self.machine.reg_read(ux.UC_X86_REG_EFLAGS) & 0x400 else 1
            port = regread(self.machine, 'dx'); stream = bytearray()
            for k in range(count):
                at = (addr + k * unit * direction) & MASK
                if out:
                    stream.extend(self.read_bytes(at, unit))
                else:
                    v = mix(self.inputs.seed, len(self.log) + k, port)
                    self.stub_write(at, v.to_bytes(4, 'little')[:unit])
            self.log.append({'kind': 'system', 'operation': ins.mnemonic, 'args': [port, count, unit],
                             'stream_sha256': hashlib.sha256(stream).hexdigest() if out else None})
            regwrite(self.machine, rn, addr + count * unit * direction)
            if rep: regwrite(self.machine, 'ecx', 0)
            return
        if name == 'int': values = [ins.operands[0].imm] + [regread(self.machine, r) for r in REGS[:-1]]
        elif name == 'out': values = [self.operand_value(o, ins) for o in ins.operands]
        elif name == 'in': values = [self.operand_value(ins.operands[1], ins)]
        elif name in ('cli', 'sti', 'int3', 'hlt'): values = []
        elif name == 'into':
            if not self.machine.reg_read(ux.UC_X86_REG_EFLAGS) & 0x800: return
            values = [regread(self.machine, r) for r in REGS[:-1]]
        elif name == 'rdtsc': values = []
        elif name == 'cpuid': values = [regread(self.machine, 'eax'), regread(self.machine, 'ecx')]
        elif name in ('lgdt', 'lidt'):
            op = ins.operands[0]
            a = op.mem.disp + (regread(self.machine, ins.reg_name(op.mem.base)) if op.mem.base else 0)
            values = [self.read_bytes(a & MASK, 6).hex()]
        else: raise Limit('unsupported system contract ' + name + ' ' + ins.op_str)
        self.log.append({'kind': 'system', 'operation': name, 'args': values})
        v = mix(self.inputs.seed, len(self.log), sum(name.encode()))
        if name == 'in': regwrite(self.machine, ins.reg_name(ins.operands[0].reg), v)
        elif name == 'int': regwrite(self.machine, 'eax', v)
        elif name in ('rdtsc', 'cpuid'):
            for ix, rn in enumerate(['eax', 'edx'] if name == 'rdtsc' else ['eax', 'ebx', 'ecx', 'edx']):
                regwrite(self.machine, rn, mix(self.inputs.seed, len(self.log), ix + 88))
        elif name in ('cli', 'sti'):
            flags = self.machine.reg_read(ux.UC_X86_REG_EFLAGS)
            self.machine.reg_write(ux.UC_X86_REG_EFLAGS, (flags | 0x200) if name == 'sti' else flags & ~0x200)

    def snapshot(self):
        out = {}
        for start, end in ranges(self.dirty):
            try: raw = bytes(self.machine.mem_read(start, end - start))
            except uc.UcError: continue
            out.update(zip(range(start, end), raw))
        return out

    def summary(self, full=False):
        log = self.log if full else self.log[:64]
        return {'termination': self.status, 'reason': self.reason, 'steps': self.count, 'step_budget': self.steps,
                'return': self.return_value, 'uninitialized_stack': self.uninit, 'unknown_calls': self.unknown[:16],
                'discarded_stack_reads': self.discarded_stack_reads, 'fault': getattr(self, 'fault', None),
                'call_log': log, 'call_log_length': len(self.log),
                'writes': {hex(a): v for a, v in list(self.snapshot().items())[:256]}, 'writes_total': len(self.dirty)}


def ranges(addresses):
    ss = sorted(addresses)
    if not ss: return
    first = prev = ss[0]
    for a in ss[1:]:
        if a != prev + 1:
            yield first, prev + 1; first = a
        prev = a
    yield first, prev + 1


def limit(reason, blocking, **extra):
    d = {'kind': 'harness_limit', 'reason': reason, 'blocking': blocking}
    d.update(extra)
    return d


def compare(a, b, notes=None):
    """First differing observable, a harness_limit (blocking or per-seed), or None."""
    notes = notes if notes is not None else []
    sa, sb = a.status, b.status
    resource = lambda x: x.status == 'BUDGET' or (x.status == 'LIMIT' and x.limit_class in ('pages', 'timeout'))
    if resource(a) and resource(b) and 'LIMIT' in (sa, sb):
        return limit('both sides exhausted a resource budget (instructions/calls/materialised pages/wall clock)', False,
                     original=a.reason or sa, candidate=b.reason or sb)
    if sa == 'LIMIT' or sb == 'LIMIT':
        if (a.limit_class == 'smc') != (b.limit_class == 'smc'):
            # A generated data pointer aliased the executing function's code on ONE side (the other side wrote
            # the same address as plain data); real self-modifying code writes on both sides.
            return limit('generated data pointer aliased the executing function code on one side only', False,
                         original=a.reason, candidate=b.reason)
        if sa == sb:
            blocking = a.limit_class in ('smc', 'model') or b.limit_class in ('smc', 'model')
            if a.limit_class == b.limit_class == 'control' and a.reason.split(' at ')[0] == b.reason.split(' at ')[0]:
                blocking = False
            why = a.reason if a.reason == b.reason else f'{a.reason} / {b.reason}'
            return limit(why, blocking, original=a.reason, candidate=b.reason, classes=[a.limit_class, b.limit_class])
        return limit(f'asymmetric termination: original {sa} ({a.reason}), candidate {sb} ({b.reason})', True,
                     original=sa, candidate=sb)
    if 'BUDGET' in (sa, sb):
        if sa == sb:
            return limit(f'both exceeded the {a.steps}-instruction / {a.max_events}-call budget', False)
        return limit(f'asymmetric termination at the top budget: original {sa}, candidate {sb}', True,
                     original=sa, candidate=sb, original_steps=a.count, candidate_steps=b.count)
    if sa == 'FAULT' and sb == 'FAULT' and a.fault['vector'] == b.fault['vector']:
        return limit(f"both raise CPU exception vector {a.fault['vector']}", False)
    if sa != sb or (sa == 'FAULT' and a.fault['vector'] != b.fault['vector']):
        return {'kind': 'termination', 'original': sa, 'candidate': sb,
                'original_fault': getattr(a, 'fault', None), 'candidate_fault': getattr(b, 'fault', None)}
    acc_a, acc_b = {}, {}
    for k, (x, y) in enumerate(zip(a.log, b.log)):
        d = compare_boundary(a, b, k, acc_a, acc_b)
        if d: return d
        if x['kind'] != y['kind']:
            return {'kind': 'call_kind', 'index': k, 'original': x, 'candidate': y}
        if x['kind'] == 'system':
            if x != y: return {'kind': 'system', 'index': k, 'original': x, 'candidate': y}
            continue
        if x['target'] != y['target']:
            return {'kind': 'call_target', 'index': k, 'original': brief(x), 'candidate': brief(y)}
        if not (x['known'] and y['known']):
            regs = [x['registers'][n] == y['registers'][n] for n in ('eax', 'edx', 'ebx', 'ecx')]
            return limit('unknown callee prototype: ' + str(x['name'] or hex(x['target'])) + ' (' + str(x['why']) + ')', False,
                         index=k, callee=x['name'] or hex(x['target']), register_agreement=regs)
        if x['args'] != y['args']:
            j = next((j for j, (u, v) in enumerate(zip(x['args'], y['args'])) if u != v), min(len(x['args']), len(y['args'])))
            u = x['args'][j] if j < len(x['args']) else None; v = y['args'][j] if j < len(y['args']) else None
            kind = 'call_argument_identity' if isinstance(u, dict) or isinstance(v, dict) else 'call_arguments'
            return {'kind': kind, 'index': k, 'arg': j, 'original': brief(x), 'candidate': brief(y)}
        for p, q in zip(x['pointees'], y['pointees']):
            if 'string' in p:
                if p['complete'] and q['complete'] and p['string'] != q['string']:
                    return {'kind': 'call_pointee', 'index': k, 'arg': p['arg'], 'original': p, 'candidate': q, 'call': brief(x)}
                if p['complete'] != q['complete']:
                    notes.append({'note': 'string pointee defined on one side only', 'index': k, 'callee': x['name'], 'arg': p['arg']})
            elif p['defined'] and q['defined']:
                if p['value'] != q['value']:
                    return {'kind': 'call_pointee', 'index': k, 'arg': p['arg'], 'original': p, 'candidate': q, 'call': brief(x)}
            elif p['defined'] != q['defined']:
                notes.append({'note': 'pointee defined before the call on one side only (callee may only write it)',
                              'index': k, 'callee': x['name'], 'arg': p['arg']})
    if len(a.log) != len(b.log):
        return {'kind': 'call_count', 'original': len(a.log), 'candidate': len(b.log)}
    for start, end in ranges(a.dirty | b.dirty):
        try:
            for x in (a, b):
                for pg in range(start & ~(PAGE - 1), end, PAGE): x.ensure(pg)
            aa = bytes(a.machine.mem_read(start, end - start)); bb = bytes(b.machine.mem_read(start, end - start))
        except (uc.UcError, Limit) as exc:
            return limit(f'unreadable final-memory range {start:x}..{end:x}: {exc}', True)
        if aa != bb:
            k = next(k for k, (x, y) in enumerate(zip(aa, bb)) if x != y); addr = start + k
            return {'kind': 'memory', 'address': addr, 'symbol': symbol_of(a.f, addr), 'original': aa[k], 'candidate': bb[k],
                    'original_pc': a.writers.get(addr), 'candidate_pc': b.writers.get(addr)}
    if a.status == 'RETURN' and a.return_value != b.return_value:
        return {'kind': 'return', 'width': a.f.proto.ret.width, 'original': a.return_value, 'candidate': b.return_value}
    return None


def symbol_of(f, addr):
    name, off = f.p.data_symbol(addr)
    if name and IDA_BASE[2] <= addr < IDA_BASE[2] + len(f.p.images[2]):
        return f'{name}+{off:#x}'
    return None


def brief(x):
    return {k: x.get(k) for k in ('target', 'name', 'args', 'pointees', 'known')}


def compare_boundary(a, b, k, acc_a, acc_b):
    """Memory written before call k must agree at call k (a callee may read any global)."""
    da = a.boundaries[k] if k < len(a.boundaries) else {}
    db = b.boundaries[k] if k < len(b.boundaries) else {}
    acc_a.update(da); acc_b.update(db)
    for addr in sorted(set(da) | set(db)):
        va = acc_a.get(addr); vb = acc_b.get(addr)
        if va is None: va = a.inputs.initial_byte(addr)
        if vb is None: vb = b.inputs.initial_byte(addr)
        if va != vb:
            return {'kind': 'memory_at_call', 'index': k, 'address': addr, 'symbol': symbol_of(a.f, addr),
                    'original': va, 'candidate': vb, 'callee': a.log[k].get('name'),
                    'original_pc': a.writers.get(addr), 'candidate_pc': b.writers.get(addr)}
    return None


def run_pair(f, state, deadline, track_reads=False, tiers=TIERS, stats=None):
    """Run both sides, escalating the instruction budget while one of them is still running.
    Adaptive per function (stats): a tier >= 2 that has already failed twice without ever completing a seed is
    not tried again, so endless loops do not consume the function's whole time budget."""
    stats = stats if stats is not None else {'ok': collections.Counter(), 'fail': collections.Counter()}
    for t, steps in enumerate(tiers):
        a = Execution(f, state, False, steps, track_reads=track_reads)
        b = Execution(f, state, True, steps, track_reads=track_reads)
        both = a.status == b.status == 'BUDGET'
        if both: stats['fail'][t] += 1
        elif 'BUDGET' not in (a.status, b.status): stats['ok'][t] += 1
        nxt = t + 1
        hopeless = nxt < len(tiers) and nxt >= 2 and stats['fail'][nxt] >= 2 and not stats['ok'][nxt]
        if 'BUDGET' not in (a.status, b.status) or nxt == len(tiers) or time.monotonic() > deadline or (both and hopeless):
            return a, b
    return a, b


def minimize(f, state, original, candidate, difference, deadline_seconds=5, top=TIERS[-1]):
    """Bounded delta reduction; never claims global minimality."""
    start = time.monotonic(); trials = 0
    best = copy.copy(state); best.overrides = dict(state.overrides); best.regs = dict(state.regs); best.page_cache = {}; best._po_key = None
    before = len(best.overrides)
    steps = max(original.steps, candidate.steps)
    def same(d):
        if not d or d['kind'] != difference['kind']: return False
        for key in ('address', 'index', 'width'):
            if key in difference and d.get(key) != difference[key]: return False
        return True
    def probe(overrides, regs):
        nonlocal trials
        trials += 1
        x = copy.copy(best); x.overrides = dict(overrides); x.regs = dict(regs); x.page_cache = {}; x._po_key = None
        a = Execution(f, x, False, steps); b = Execution(f, x, True, steps)
        d = compare(a, b)
        return (x, a, b, d) if same(d) else None
    last = (best, original, candidate, difference)
    ra = Execution(f, best, False, steps, track_reads=True); rb = Execution(f, best, True, steps, track_reads=True)
    relevant = ra.input_reads | rb.input_reads | ra.dirty | rb.dirty | state.initialized
    reduced = {a: v for a, v in best.overrides.items() if a in relevant}
    trial = probe(reduced, best.regs)
    if trial: last = trial; best = trial[0]
    granularity = 8
    complete = False
    while best.overrides and time.monotonic() - start < deadline_seconds:
        keys = sorted(best.overrides)
        chunk = max(1, (len(keys) + granularity - 1) // granularity)
        changed = False
        for pos in range(0, len(keys), chunk):
            if time.monotonic() - start >= deadline_seconds: break
            remove = set(keys[pos:pos + chunk])
            ov = {a: v for a, v in best.overrides.items() if a not in remove}
            trial = probe(ov, best.regs)
            if trial:
                last = trial; best = trial[0]; changed = True; break
        if not changed:
            if chunk == 1: complete = True; break
            granularity *= 2
    protected = {r for r, t in zip(f.proto.parm, f.proto.args) if t.pointer}
    for rn in REGS:
        if time.monotonic() - start >= deadline_seconds: break
        if rn == 'esp' or rn in protected or best.regs[rn] == 0: continue
        rr = dict(best.regs); rr[rn] = 0
        trial = probe(best.overrides, rr)
        if trial: last = trial; best = trial[0]
    x, a, b, d = last
    return {'seed': x.seed, 'inputs': x.summary(), 'first_difference': d, 'original': a.summary(), 'candidate': b.summary(),
            'minimization': {'trials': trials, 'overrides_before': before, 'overrides_after': len(x.overrides),
                             'elapsed_seconds': round(time.monotonic() - start, 3),
                             'deletion_pass_complete': complete, 'claim': 'bounded reduction, not global minimality'}}


def run_function(p, unit, name, seeds=128, seconds=45, source=None, scenario=None, allow_losses=False,
                 explore_seeds=48, use_domain=True):
    import domains
    started = time.monotonic()
    deadline = started + seconds
    result = {'unit': unit, 'function': name, 'verdict': 'HARNESS_LIMIT', 'seeds': 0, 'coverage': 0,
              'counterexample': None, 'uninitialized_reads': [],
              'stack_read_statistics': {'seeds_with_reads': 0, 'events': 0, 'discarded_overread_events': 0},
              'harness_version': 2, 'harness_sha256': HARNESS_SHA256, 'unicorn_version': uc.__version__}
    try:
        f = Function(p, unit, name, source, allow_losses)
    except (Limit, ValueError, KeyError, IndexError, struct.error) as e:
        result['reason'] = str(e); result['blocking_limits'] = {str(e): 1}; return result
    domain = domains.load(unit, name, f) if use_domain else None
    result['domain'] = domain.describe() if domain else None
    result['whole_unit'] = {k: f.receipt[k] for k in ('strict_before', 'strict_after', 'strict_losses', 'source_sha256', 'object_sha256')}
    result['source'] = f.receipt['source']
    result['step_tiers'] = list(TIERS)
    result['recorded_matching_status'] = p.units[unit]['functions'].get(name, {}).get('status')
    result['prototype'] = {'ret': f.proto.ret.text, 'args': [a.text for a in f.proto.args], 'parm': f.proto.parm,
                           'value': f.proto.value, 'source': f.proto.source}
    result['cfg_limits'] = f.cfg_limits + f.ccfg_limits
    result['blocks_total'] = len(f.blocks)
    result['function_pointer_slots'] = len(f.fnptr_slots)
    covered = set(); equal = 0; unjudged = collections.Counter(); blocking = collections.Counter()
    diagnostics = []; notes = collections.Counter(); unknown_callees = collections.Counter(); tiers_used = collections.Counter()
    out_of_domain = None; explored = {'seeds': 0, 'equal': 0, 'in_domain_draws': 0}
    tier_stats = {'ok': collections.Counter(), 'fail': collections.Counter()}
    phases = [('main', range(seeds))]
    if domain:
        phases.append(('explore', range(10000, 10000 + explore_seeds)))
    for phase, seed_range in phases:
        for seed in seed_range:
            if time.monotonic() >= deadline: break
            if phase == 'main' and seed >= MIN_EQUAL * 2 and time.monotonic() >= started + seconds * (0.75 if domain else 1.0):
                break
            try:
                state = Inputs(f, seed, scenario, domain=domain if phase == 'main' else None)
                a, b = run_pair(f, state, deadline, stats=tier_stats)
            except (Limit, uc.UcError) as exc:
                (unjudged if phase == 'main' else collections.Counter())['input/emulator setup: ' + str(exc)] += 1
                if phase == 'main': result['seeds'] += 1
                continue
            seed_notes = []
            try: diff = compare(a, b, seed_notes)
            except (Limit, uc.UcError) as exc:
                diff = limit('observable materialisation: ' + str(exc), True)
            if phase == 'explore':
                explored['seeds'] += 1
                if diff is None:
                    explored['equal'] += 1; continue
                if diff['kind'] == 'harness_limit':
                    continue
                inside = domain.contains(state)
                if inside:
                    explored['in_domain_draws'] += 1
                    phase_result_divergent(result, f, state, a, b, diff, deadline, 'unconstrained exploration draw inside the domain')
                    break
                if out_of_domain is None:
                    out_of_domain = {'seed': seed, 'violations': domain.violations(state), 'first_difference': diff,
                                     'inputs': state.summary(), 'original': a.summary(), 'candidate': b.summary()}
                continue
            result['seeds'] += 1
            tiers_used[max(a.steps, b.steps)] += 1
            for n in seed_notes: notes[n['note'] + ' (' + str(n['callee']) + ')'] += 1
            if a.uninit or b.uninit:
                result['stack_read_statistics']['seeds_with_reads'] += 1
                result['stack_read_statistics']['events'] += len(a.uninit) + len(b.uninit)
                if len(result['uninitialized_reads']) < 4:
                    result['uninitialized_reads'].append({'seed': seed, 'original': a.uninit[:8], 'candidate': b.uninit[:8]})
            result['stack_read_statistics']['discarded_overread_events'] += len(a.discarded_stack_reads) + len(b.discarded_stack_reads)
            if diff and diff['kind'] == 'harness_limit':
                (blocking if diff['blocking'] else unjudged)[diff['reason']] += 1
                if diff.get('callee'): unknown_callees[diff['callee']] += 1
                if len(diagnostics) < 4: diagnostics.append({'seed': seed, 'difference': diff})
                continue
            if diff:
                phase_result_divergent(result, f, state, a, b, diff, deadline, None)
                break
            covered.update(a.visited & f.blocks)
            equal += 1
        if result['verdict'] == 'DIVERGENT':
            break
    result['equal_seeds'] = equal
    result['unjudged_seeds'] = sum(unjudged.values())
    result['blocks_covered'] = len(covered)
    result['coverage'] = round(100 * len(covered) / max(1, len(f.blocks)), 2)
    result['coverage_basis'] = 'DOS basic blocks visited by judged (equal) seeds'
    result['uncovered_blocks'] = [hex(a) for a in sorted(f.blocks - covered)][:64]
    result['unjudged'] = dict(unjudged); result['blocking_limits'] = dict(blocking)
    result['unknown_callees'] = dict(unknown_callees)
    result['notes'] = dict(notes); result['diagnostics'] = diagnostics
    result['budget_tiers_used'] = {str(k): v for k, v in sorted(tiers_used.items())}
    result['exploration'] = explored if domain else None
    result['out_of_domain_difference'] = out_of_domain
    result['elapsed_seconds'] = round(time.monotonic() - started, 3)
    if result['verdict'] != 'DIVERGENT':
        judged_ok = equal >= MIN_EQUAL and equal >= MIN_JUDGED_FRACTION * max(1, result['seeds'])
        if blocking or f.cfg_limits or f.ccfg_limits:
            result['in_domain_verdict'] = 'HARNESS_LIMIT'
            result['reason'] = '; '.join(list(blocking) + f.cfg_limits + f.ccfg_limits)
        elif judged_ok:
            result['in_domain_verdict'] = 'EQUIVALENT_TESTED'
        else:
            result['in_domain_verdict'] = 'HARNESS_LIMIT'
            top = unjudged.most_common(3)
            result['reason'] = (f'only {equal} of {result["seeds"]} seeds judged; ' + '; '.join(f'{r} x{n}' for r, n in top)
                                if top else 'no completed seeds')
        result['verdict'] = 'OUT_OF_DOMAIN_DIFFERENCE' if out_of_domain else result['in_domain_verdict']
    return result


def phase_result_divergent(result, f, state, a, b, diff, deadline, why):
    result['verdict'] = 'DIVERGENT'
    result['in_domain_verdict'] = 'DIVERGENT'
    if why: result['divergence_context'] = why
    result['counterexample'] = {'seed': state.seed, 'inputs': state.summary(), 'first_difference': diff,
                                'original': a.summary(), 'candidate': b.summary()}
    remaining = min(5, max(0, deadline + 5 - time.monotonic()))
    if remaining > 0:
        try:
            result['counterexample'] = minimize(f, state, a, b, diff, remaining)
        except (Limit, uc.UcError) as exc:
            result['counterexample']['minimization_error'] = str(exc)
    result['counterexample']['stub_return_mode'] = (
        'narrow returns extended by declared signedness' if state.seed % 2 == 0
        else 'narrow returns carry callee-defined upper bits (identical on both sides)')


def prototype_report(p, targets):
    """Every callable referenced by the target functions whose contract cannot be determined."""
    out = {}
    for unit, name in targets:
        try:
            f = Function(p, unit, name)
        except Exception as exc:  # noqa: BLE001 - report only
            out.setdefault('_entry', {})[f'{unit}/{name}'] = str(exc); continue
        for side, ins in ((False, f.oin),):
            for a, i in ins.items():
                if not i.group(cs.CS_GRP_CALL): continue
                op = i.operands[0]
                if op.type == cs.CS_OP_IMM and (op.imm & MASK) in p.transparent_starts:
                    out.setdefault('_executed_runtime_helpers', {}).setdefault(p.names[op.imm & MASK], []).append(f'{unit}/{name}')
                    continue
                if op.type == cs.CS_OP_IMM:
                    proto, label, why = f.callee(op.imm & MASK)
                    if proto is None:
                        out.setdefault(label or hex(op.imm), {'why': why, 'callers': []})['callers'].append(f'{unit}/{name}')
        for slot, (proto, label) in f.fnptr_slots.items():
            if not proto.complete():
                out.setdefault('fnptr:' + label, {'why': 'incomplete function-pointer type', 'callers': []})['callers'].append(f'{unit}/{name}')
    return out


def targets_from_census(p):
    census = json.loads((ROOT / 'build/near_census.json').read_text())
    return [(r['unit'], r['function']) for r in census if not p.units[r['unit']]['source'].startswith('src/hmi/')]


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('unit', nargs='?'); ap.add_argument('function', nargs='?')
    ap.add_argument('--source'); ap.add_argument('--seeds', type=int, default=128)
    ap.add_argument('--seconds', type=float, default=45)
    ap.add_argument('--scenario', choices=['guided', 'b19', 'b20'])
    ap.add_argument('--all', action='store_true'); ap.add_argument('--controls', action='store_true')
    ap.add_argument('--units', help='comma-separated unit filter for --all')
    ap.add_argument('--resume', action='store_true', help='skip functions whose v2 result already exists')
    ap.add_argument('--no-domain', action='store_true')
    ap.add_argument('--replay', help='result JSON containing a v3-generator counterexample')
    ap.add_argument('--prototype-report', action='store_true')
    args = ap.parse_args(argv)
    p = Program()
    if args.replay:
        r = json.loads(Path(args.replay).read_text()); c = r['counterexample'] or r.get('out_of_domain_difference')
        f = Function(p, r['unit'], r['function'], args.source)
        state = Inputs.restore(f, c['inputs'])
        a, b = run_pair(f, state, time.monotonic() + 60)
        d = compare(a, b)
        out = {'unit': r['unit'], 'function': r['function'], 'difference': d,
               'original': a.summary(), 'candidate': b.summary()}
        write_json(OUT / 'replays' / f"{r['unit']}_{r['function']}.json", out)
        print(json.dumps(d, indent=2, default=str)); return 1 if d and d['kind'] != 'harness_limit' else 0
    if args.controls:
        from controls import run_controls
        return run_controls(p)
    if args.prototype_report:
        rep = prototype_report(p, targets_from_census(p))
        write_json(OUT / 'PROTOTYPES_UNKNOWN.json', rep)
        print(json.dumps({k: v['why'] if isinstance(v, dict) and 'why' in v else v for k, v in rep.items()}, indent=1)[:6000])
        return 0
    if args.all:
        targets = targets_from_census(p)
        if args.units:
            keep = set(args.units.split(','))
            targets = [t for t in targets if t[0] in keep]
    elif args.unit and args.function: targets = [(args.unit, args.function)]
    else: ap.error('specify UNIT FUNCTION or --all or --controls')
    results = []
    for unit, name in targets:
        path = OUT / 'results' / f'{unit}_{name}.json'
        if args.resume and path.exists():
            old = json.loads(path.read_text())
            if old.get('harness_version') == 2 and old.get('harness_sha256') == HARNESS_SHA256:
                print(f'{unit}/{name}: cached {old["verdict"]}', flush=True); continue
        try:
            r = run_function(p, unit, name, args.seeds, args.seconds, args.source, args.scenario, use_domain=not args.no_domain)
        except Exception as exc:
            error = traceback.format_exc()
            write_json(OUT / 'errors' / f'{unit}_{name}.json', {'error': error})
            r = {'unit': unit, 'function': name, 'verdict': 'HARNESS_LIMIT', 'seeds': 0, 'coverage': 0,
                 'counterexample': None, 'reason': 'unexpected harness exception: ' + str(exc),
                 'blocking_limits': {'unexpected harness exception': 1},
                 'harness_version': 2, 'harness_sha256': HARNESS_SHA256}
        results.append(r)
        write_json(path, r)
        print(f"{unit}/{name}: {r['verdict']} {r['seeds']} seeds eq={r.get('equal_seeds')} cov={r['coverage']}% "
              f"{r.get('elapsed_seconds')}s {(r.get('reason') or '')[:160]}", flush=True)
    collect()
    print(dict(collections.Counter(r['verdict'] for r in results)))
    return 0


def collect():
    """Aggregate RESULTS.json from all v2 per-function results (compact: no traces)."""
    out = []
    for path in sorted((OUT / 'results').glob('*.json')):
        r = json.loads(path.read_text())
        if r.get('harness_version') != 2:
            continue
        c = r.get('counterexample') or {}
        o = r.get('out_of_domain_difference') or {}
        out.append({k: r.get(k) for k in ('unit', 'function', 'verdict', 'in_domain_verdict', 'seeds', 'equal_seeds',
                                          'unjudged_seeds', 'coverage', 'blocks_covered', 'blocks_total', 'reason',
                                          'unjudged', 'blocking_limits', 'unknown_callees', 'notes', 'domain',
                                          'elapsed_seconds', 'recorded_matching_status', 'harness_sha256')}
                   | {'first_difference': c.get('first_difference'), 'counterexample_seed': c.get('seed'),
                      'out_of_domain_first_difference': o.get('first_difference'),
                      'out_of_domain_violations': o.get('violations')})
    write_json(OUT / 'RESULTS.json', out)
    return out


if __name__ == '__main__':
    sys.exit(main())
