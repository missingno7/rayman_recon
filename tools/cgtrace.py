"""Pinned Watcom 10.6 observation, diagnostic only; no codegen-state writes.

python tools/cgtrace.py SRC.c --profile game --func NAME --out build/cgtrace/run

Breakpoints/layouts: cg_re + inv_alloc (2026-10-22); Score: cg_score.
Compiler RVAs below apply ONLY to the hash-pinned wc106 CF payload.
Outputs are published only after a same-source-path normal compile has identical
whole OMF bytes. Never a substitute for match.py's STRICT verdict.
"""
import argparse
import ctypes as C
import ctypes.wintypes as W
import hashlib
import json
import os
from pathlib import Path
import re
import struct
import subprocess
import sys
import tempfile
import time
from types import SimpleNamespace

sys.dont_write_bytecode = True
ROOT = next(p for p in Path(__file__).resolve().parents if (p / 'config/units.json').is_file())
sys.path.insert(0, str(ROOT / 'tools'))
import wcc

K = C.WinDLL('kernel32', use_last_error=True) if os.name == 'nt' else None


class CTX(C.Structure):
    _fields_ = [('ContextFlags', W.DWORD)] + [(x, W.DWORD) for x in ['Dr0', 'Dr1', 'Dr2', 'Dr3', 'Dr6', 'Dr7']] + [('FloatSave', C.c_byte * 112)] + [(x, W.DWORD) for x in ['SegGs', 'SegFs', 'SegEs', 'SegDs', 'Edi', 'Esi', 'Ebx', 'Edx', 'Ecx', 'Eax', 'Ebp', 'Eip', 'SegCs', 'EFlags', 'Esp', 'SegSs']] + [('ExtendedRegisters', C.c_byte * 512)]
if K is not None:
    K.WaitForDebugEvent.argtypes = [C.c_void_p, W.DWORD]
    K.ContinueDebugEvent.argtypes = [W.DWORD, W.DWORD, W.DWORD]
    K.Wow64GetThreadContext.argtypes = [W.HANDLE, C.POINTER(CTX)]
    K.Wow64SetThreadContext.argtypes = [W.HANDLE, C.POINTER(CTX)]
    K.ReadProcessMemory.argtypes = [W.HANDLE, C.c_void_p, C.c_void_p, C.c_size_t, C.c_void_p]
    K.WriteProcessMemory.argtypes = [W.HANDLE, C.c_void_p, C.c_void_p, C.c_size_t, C.c_void_p]
    K.FlushInstructionCache.argtypes = [W.HANDLE, C.c_void_p, C.c_size_t]
    K.TerminateProcess.argtypes = [W.HANDLE, W.UINT]
    K.CloseHandle.argtypes = [W.HANDLE]


class _ObserverCore:
    """Win64 debug transport plus the independently recovered 10.6 layouts.

    N_TEMP: size +8, conflict +12, alias offset +16, symbol +20,
    location +32, ID +36, flags +42. Other name classes share only the
    prefix; the extra raw fields are meaningful only for their own union arm.
    Instruction: opcode +34, tables +36/+40, result +48, type +56,
    operand count +63, operands +68. All accesses are read-only except the
    registered one-byte breakpoint protocol.
    """

    def __init__(self, phases=False, scheduler=False, scheduler_functions=None):
        self.events = []
        self.bps = {}
        self.threads = {}
        self.base = None
        self.active = None
        self.steps = {}
        self.pid = None
        self.callno = 0
        self.best_stack = []
        self.phases = phases
        self.scheduler_enabled = scheduler
        self.scheduler_functions = scheduler_functions or []
        self.scheduler_function_index = -1
        self.scheduler_function = None
        self.scheduler_current = None
        self.scheduler_round = []
        self.rover_stack = []

    def read(self, a, n):
        b = C.create_string_buffer(n)
        got = C.c_size_t()
        if not K.ReadProcessMemory(self.hp, a, b, n, C.byref(got)):
            return b.raw[:got.value]
        return b.raw

    def u32(self, a):
        b = self.read(a, 4)
        return struct.unpack('<I', b)[0] if len(b) == 4 else 0

    def write(self, a, b):
        if a not in self.bps or b not in (self.bps[a][0], b'\xcc') or len(b) != 1:
            raise ValueError('observer may write only a registered breakpoint byte or its original')
        buf = C.create_string_buffer(b)
        done = C.c_size_t()
        if not K.WriteProcessMemory(self.hp, a, buf, len(b), C.byref(done)):
            raise C.WinError(C.get_last_error())
        K.FlushInstructionCache(self.hp, a, len(b))

    def bp(self, a, label):
        if a in self.bps:
            return
        orig = self.read(a, 1)
        if len(orig) != 1:
            raise RuntimeError('unreadable breakpoint ' + hex(a))
        self.bps[a] = (orig, label)
        self.write(a, b'\xcc')

    def context(self, tid):
        c = CTX()
        c.ContextFlags = 0x1003f
        if not K.Wow64GetThreadContext(self.threads[tid], C.byref(c)):
            raise C.WinError(C.get_last_error())
        return c

    def setcontext(self, tid, c):
        if not K.Wow64SetThreadContext(self.threads[tid], C.byref(c)):
            raise C.WinError(C.get_last_error())

    def name(self, a):
        if not a:
            return None
        b = self.read(a, 48)
        if len(b) != 48:
            return {'addr': a}
        u = lambda off: struct.unpack_from('<I', b, off)[0]
        h = lambda off: struct.unpack_from('<H', b, off)[0]
        return dict(addr=a, next=u(0), cls=b[4], type=b[5], size=u(8), conflict=u(12),
                    conflict_savings=self.u32(u(12) + 20) if b[4] == 2 and u(12) else None,
                    offset=u(16), symbol=u(20), usage=h(24), alias=u(28),
                    location=u(32), id=u(36), flags=h(42))

    def names(self, a):
        out = []
        seen = set()
        while a and a not in seen and (len(out) < 3000):
            seen.add(a)
            n = self.name(a)
            out.append(n)
            a = n.get('next', 0)
        return out

    def conflicts(self, a):
        out = []
        seen = set()
        while a and a not in seen and (len(out) < 3000):
            seen.add(a)
            b = self.read(a, 88)
            if len(b) != 88:
                break
            u = lambda off: struct.unpack_from('<I', b, off)[0]
            out.append(dict(addr=a, next=u(0), name=self.name(u(8)), tree=u(16), savings=u(20), available=u(28), with_regs=u(32), state=struct.unpack_from('<H', b, 84)[0]))
            a = u(0)
        return out

    def operand(self, a, depth=0):
        if not a:
            return None
        b = self.read(a, 32)
        if len(b) != 32:
            return dict(addr=a)
        u = lambda off: struct.unpack_from('<I', b, off)[0]
        n = dict(addr=a, cls=b[4], type=b[5], size=u(8))
        if b[4] in (1, 2):
            n.update(self.name(a))
        elif b[4] == 3:
            n['reg'] = u(12)
        elif b[4] == 0:
            n.update(value=u(16), value_hi=u(20), constant_type=b[28])
        elif b[4] == 4:
            n.update(displacement=C.c_int32(u(20)).value, scale=b[25], index_flags=b[24])
            if depth < 3:
                n.update(index=self.operand(u(12), depth + 1), base=self.operand(u(16), depth + 1))
        return n

    def graph(self):
        out = []
        blk = self.u32(self.base + 0x8592c)
        seen = set()
        while blk and blk not in seen:
            seen.add(blk)
            p = self.u32(blk + 4)
            ii = set()
            while p and p not in ii:
                ii.add(p)
                b = self.read(p, 80)
                if len(b) != 80:
                    raise RuntimeError('unreadable IR graph instruction')
                if b[34] == 75:
                    break
                n = b[63]
                if n > 4:
                    raise RuntimeError('unexpected IR operand count')
                node = self.instruction(p)
                node['block'] = blk
                out.append(node)
                p = struct.unpack_from('<I', b, 4)[0]
            blk = self.u32(blk + 40)
        return out

    def _hit_core(self, label, c):
        if label == 'loaded':
            self.base = self.u32(self.image + 0x30008)
            expected = self.payload_prefix
            if self.read(self.base, 16) != expected:
                raise RuntimeError('loaded CF prefix differs from pinned payload')
            self.events.append(dict(event='payload_loaded', base=self.base))
            for r, n in [(0x42681, 'temp_created'), (0x426da, 'user_temp_created'), (0x61e94, 'phase_regalloc'),
                (0x703ae, 'sort'), (0x703e3, 'sort_return'), (0x5eaaf, 'homes'), (0x5ec02, 'homes_return'), (0x61488, 'best'),
                (0x61686, 'best_return'), (0x615a5, 'score'), (0x6158c, 'greed'), (0x61648, 'worth'), (0x5a3a5, 'loop_cost'),
                (0x6c4be, 'rover'), (0x6c503, 'rover_return'), (0x6c543, 'rover_return'), (0x6c54a, 'rover_return')]:
                if n != 'unused':
                    self.bp(self.base + r, n)
            if self.phases:
                for r, n in [(0x4c3a7, 'phase_post_start'), (0x4c3d2, 'phase_after_LdStAlloc'), (0x4c3d7, 'phase_after_Score'),
                    (0x4c427, 'phase_after_LdStCompress')]:
                    self.bp(self.base + r, n)
            if self.scheduler_enabled:
                for r, n in [(0x6dc89, 'scheduler_block'), (0x6dcb1, 'scheduler_dag'),
                             (0x6dab1, 'scheduler_cost'), (0x6db00, 'scheduler_adjust'),
                             (0x6db6c, 'scheduler_selected')]:
                    self.bp(self.base + r, n)
            return
        if label == 'temp_created':
            self.events.append(dict(event='temp_created', name=self.name(c.Eax), caller=hex(self.u32(c.Esp + 4) - self.base), caller2=hex(self.u32(c.Esp + 12) - self.base) if self.u32(c.Esp + 4) - self.base == 0x4268e else None))
            if self.u32(c.Esp + 4) - self.base == 0x4268e and self.u32(c.Esp + 12) - self.base == 0x74fc9:
                self.events[-1]['parent_instruction'] = [i for i in self.graph() if i['addr'] == c.Ecx]
        if label == 'user_temp_created':
            self.events.append(dict(event='user_temp_created', name=self.name(c.Eax)))
        if label == 'sort':
            self.callno += 1
            comp = c.Ebx - self.base
            if comp in (0x5eb49, 0x5a056, 0x61a08, 0x62ba5):
                self.active = dict(event='sort', number=self.callno, comparator=hex(comp), offset=c.Edx,
                    input=self.conflicts(c.Eax) if comp == 0x61a08 else self.names(c.Eax))
                self.events.append(self.active)
            else:
                self.active = None
        if label == 'sort_return' and self.active is not None:
            self.active['output'] = self.conflicts(c.Eax) if self.active['comparator'] == '0x61a08' else self.names(c.Eax)
            self.active = None
        if label == 'homes':
            self.events.append(dict(event='homes', names=self.names(self.u32(self.base + 0x85910))))
        if label == 'homes_return':
            self.events.append(dict(event='homes_return', names=self.names(self.u32(self.base + 0x85910))))
        if label == 'loop_cost':
            self.events.append(dict(event='loop_cost', weights=[self.u32(self.base + 0x86600 + 4 * i) for i in range(6)]))
        if label == 'best':
            tree = c.Edx
            regs = self.u32(tree + 16) if tree else 0
            choices = []
            if regs:
                for i in range(80):
                    v = self.u32(regs + i * 4)
                    if not v:
                        break
                    choices.append(v)
            row = dict(event='best', conflict=c.Eax, name=self.name(self.u32(c.Eax + 8)), tree=tree, temp=self.name(self.u32(tree + 8)) if tree else None,
                choices=choices, except_regs=c.Ebx, needs=c.Ecx & 255, given=self.u32(self.base + 0x858e0), scores=[],
                greed=[])
            self.events.append(row)
            self.best_stack.append(row)
        if label == 'best_return':
            self.best_stack.pop()['chosen'] = c.Ebx
        if label == 'score':
            self.best_stack[-1]['scores'].append(dict(reg=c.Esi, score=C.c_int32(c.Eax).value))
        if label == 'greed':
            self.best_stack[-1]['greed'].append(dict(reg=c.Esi, greed=c.Eax & 255))
        if label == 'worth':
            self.best_stack[-1]['worth'] = c.Eax & 255
        if label == 'rover':
            b = self.read(c.Eax, 80)
            kind = b[56]
            start = rover = 0
            choices = []
            if kind < 6:
                table, slot = ([(0x7f74c, 0x8679c)] * 2 + [(0x7f75c, 0x86798)] * 2 + [(0x7f778, 0x867a0)] * 2)[kind]
                start = self.u32(self.base + table)
                rover = self.u32(self.base + slot)
                for i in range(80):
                    v = self.u32(start + 4 * i)
                    if not v:
                        break
                    choices.append(v)
            except_regs = self.u32(c.Eax + 8) | self.u32(self.u32(c.Eax + 44) + 12)
            result = self.name(self.u32(c.Eax + 48))
            if result and result.get('cls') == 3:
                except_regs |= self.u32(result['addr'] + 12)
            row = dict(event='rover', kind=kind, choices=choices, initial_index=(rover - start) // 4 if rover else 0,
                except_regs=except_regs, was_null=not rover, instruction=self.instruction(c.Eax))
            self.events.append(row)
            self.rover_stack.append(row)
        if label == 'rover_return':
            self.rover_stack.pop()['chosen'] = self.u32(c.Eax) if c.Eax else 0
        if label.startswith('phase_'):
            self.stage = label
            self.events.append(dict(event='phase', stage=label, graph=self.graph()))

    def run(self, cmd, env, cwd, **kw):
        p = subprocess.Popen(cmd, cwd=cwd, env=env, stdout=subprocess.PIPE, stderr=subprocess.PIPE, creationflags=2 | 0x8000000)
        self.pid = p.pid
        start = time.monotonic()
        exited = False
        pending = None
        try:
            while not exited:
                if time.monotonic() - start > 90:
                    raise TimeoutError('debug compile bounded at 90s')
                event = C.create_string_buffer(176)
                if not K.WaitForDebugEvent(event, 1000):
                    continue
                b = event.raw
                code, pid, tid = struct.unpack_from('<III', b)
                pending = (pid, tid)
                u = b[16:]
                status = 0x10002
                if code == 3:
                    hf, self.hp, ht, self.image = struct.unpack_from('<QQQQ', u)
                    self.threads[tid] = ht
                    if hf:
                        K.CloseHandle(hf)
                elif code == 2:
                    self.threads[tid] = struct.unpack_from('<Q', u)[0]
                elif code == 4:
                    handle = self.threads.pop(tid, None)
                    if handle:
                        K.CloseHandle(handle)
                elif code == 6:
                    hf = struct.unpack_from('<Q', u)[0]
                    if hf:
                        K.CloseHandle(hf)
                elif code == 1:
                    ex = struct.unpack_from('<I', u)[0]
                    addr = struct.unpack_from('<Q', u, 16)[0]
                    if ex in (0x80000003, 0x4000001f):
                        if addr in self.bps:
                            orig, label = self.bps[addr]
                            self.write(addr, orig)
                            c = self.context(tid)
                            c.Eip = addr
                            self.hit(label, c)
                            c.EFlags |= 256
                            self.steps[tid] = addr
                            self.setcontext(tid, c)
                        elif self.base is None:
                            self.bp(self.image + 0x10460, 'loaded')
                        else:
                            status = 0x80010001
                    elif ex in (0x80000004, 0x4000001e) and tid in self.steps:
                        addr = self.steps.pop(tid)
                        self.write(addr, b'\xcc')
                        c = self.context(tid)
                        c.EFlags &= ~256
                        self.setcontext(tid, c)
                    else:
                        self.events.append(dict(event='exception', code=hex(ex), address=hex(addr)))
                        status = 0x80010001
                elif code == 5:
                    exited = True
                K.ContinueDebugEvent(pid, tid, status)
                pending = None
            stdout, stderr = p.communicate(timeout=5)
            return subprocess.CompletedProcess(cmd, p.returncode, stdout.decode(errors='replace'), stderr.decode(errors='replace'))
        finally:
            if not exited:
                p.kill()
                if pending:
                    K.ContinueDebugEvent(*pending, 0x10002)
                for unused in range(20):
                    ev = C.create_string_buffer(176)
                    if not K.WaitForDebugEvent(ev, 100):
                        break
                    ec, ep, et = struct.unpack_from('<III', ev.raw)
                    K.ContinueDebugEvent(ep, et, 0x10002)
                    if ec == 5:
                        break
                p.wait(timeout=5)
            for handle in self.threads.values():
                K.CloseHandle(handle)
            if hasattr(self, 'hp'):
                K.CloseHandle(self.hp)

class Observer(_ObserverCore):
    """Score pass hooks (RVAs 73515..73cd4); no worker-file dependency."""

    def instruction(self, a):
        b = self.read(a, 80)
        if len(b) != 80:
            return {'addr': a}
        u = lambda o: struct.unpack_from('<I', b, o)[0]

        def n(p):
            x = self.operand(p)
            if x and x.get('cls') == 3:
                x.update(reg=self.u32(p + 12), reg_index=self.u32(p + 16))
            return x
        t = b[57] if b[34] == 36 else b[56]
        gen = self.read(u(40), 4)
        return dict(addr=a, prev=u(0), next=u(4), live=u(8), op=b[34], table=hex(u(36) - self.base), gen_table=hex(u(40) - self.base),
            gen_code=gen[3] if len(gen) == 4 else None, gen_cc=struct.unpack_from('<H', gen)[0] & 49152 if len(gen) == 4 else None,
            zap=self.u32(u(44) + 12), result=n(u(48)), type=b[56], base_type=b[57], type_size=self.u32(self.base + 0x85b28 + 4 * t) if t < 15 else None,
            flags=b[60], ins_flags=b[64], operands=[n(self.u32(a + 68 + 4 * j)) for j in range(b[63])])

    def ring(self, sc, op):
        idx = self.u32(op + 16)
        p = sc + 16 * idx
        start = p
        out = []
        for _ in range(128):
            b = self.read(p, 16)
            if len(b) != 16:
                break
            index = b[12]
            entry = self.u32(self.u32(self.base + 0x867b8) + 4 * index)
            out.append(dict(addr=p, index=index, generation=b[13], reg=self.u32(entry + 4), size=self.u32(entry + 24), name=self.u32(entry), list=self.u32(p)))
            p = self.u32(p + 4)
            if p == start:
                break
        return out

    def hit(self, label, c):
        if label == 'loaded':
            self._hit_core(label, c)
            for a, n in [(0x73cd4, 'score_block'), (0x7375a, 'try_reg'), (0x73515, 'change_ins'), (0x7366d, 'change_result'),
                (0x7355b, 'change_widthfail'), (0x739b0, 'score_move'), (0x73ae0, 'score_move_result')]:
                self.bp(self.base + a, n)
            return
        if label == 'score_block':
            self.blk = c.Eax
            self.events.append(dict(event='score_block', block=self.blk, inputs=struct.unpack('<H', self.read(c.Eax + 86, 2))[0], block_class=self.u32(c.Eax + 92)))
            return
        if label == 'score_move':
            self.move = None
            i = self.instruction(c.Edx)
            if i['result'] and i['result']['cls'] == 3 and (i['operands'][0]['cls'] == 3):
                self.move = dict(event='score_move', block=self.blk, instruction=i, scoreboard=c.Eax, before=self.ring(c.Eax, i['result']['addr']))
                self.events.append(self.move)
            return
        if label == 'score_move_result':
            if self.move:
                self.move['after'] = self.ring(self.move['scoreboard'], self.move['instruction']['result']['addr'])
                self.move = None
            return
        if label == 'try_reg':
            i = self.instruction(c.Edx)
            op = self.u32(c.Ebx)
            if self.read(op + 4, 1) == b'\x03':
                self.events.append(dict(event='try_reg', block=self.blk, instruction=i, scoreboard=c.Eax, operand_slot=c.Ebx, ring=self.ring(c.Eax, op), next_live=self.u32(i['next'] + 8)))
            return
        if label == 'change_ins':
            self.change = None
            i = self.instruction(c.Eax)
            op = self.u32(c.Ebx)
            if op:
                self.change = dict(event='change_ins', instruction=i, replacement=self.operand(c.Edx),
                                   operand_slot=c.Ebx, pass_stage=self.stage,
                                   flags=c.Ecx & 255, return_rva=hex(self.u32(c.Esp) - self.base))
                self.events.append(self.change)
            return
        if label == 'change_result':
            if self.change is not None:
                self.change.update(success=self.read(c.Ebp - 4, 1) == b'\x01', after=self.instruction(c.Esi))
                self.change = None
            return
        if label == 'change_widthfail':
            if self.change is not None:
                self.change.update(success=False, width_rejected=True)
                self.change = None
            return
        if self.scheduler_enabled and label in ('phase_post_start', 'scheduler_block', 'scheduler_dag',
                                                'scheduler_cost', 'scheduler_adjust', 'scheduler_selected'):
            self._scheduler_hit(label, c)
        self._hit_core(label, c)

    def __init__(self, payload_prefix, scheduler=False, functions=None):
        super().__init__(phases=True, scheduler=scheduler, scheduler_functions=functions)
        self.payload_prefix = payload_prefix
        self.blk = 0
        self.change = None
        self.move = None
        self.stage = None

    def scheduler_instruction(self, a):
        ins = self.instruction(a)
        ins['id'] = self.u32(a + 52)
        ins['sequence'] = struct.unpack('<H', self.read(a + 58, 2))[0]
        gen = self.u32(a + 40)
        index = self.read(gen + 5, 1)
        fu = self.read(self.base + 0x782e3 + index[0] * 4, 4) if index and index[0] else b''
        ins['fu'] = dict(good_fu=struct.unpack_from('<H', fu)[0] if len(fu) == 4 else 0,
                         unit=fu[2] if len(fu) == 4 else 0,
                         operand=fu[3] if len(fu) == 4 else 0)
        return ins

    def scheduler_dag(self):
        addr = self.u32(self.base + 0x867b0)
        out, seen = [], set()
        while addr and addr not in seen and len(out) < 4096:
            seen.add(addr)
            deps, dep, dep_seen = [], self.u32(addr + 8), set()
            while dep and dep not in dep_seen and len(deps) < 4096:
                dep_seen.add(dep)
                deps.append(self.u32(dep + 4))
                dep = self.u32(dep)
            out.append(dict(addr=addr, prev=self.u32(addr), ins=self.scheduler_instruction(self.u32(addr + 4)),
                            deps=deps, stallable=self.u32(addr + 16) & 255,
                            height=self.u32(addr + 20), anc_count=self.u32(addr + 24)))
            addr = self.u32(addr)
        return out

    def _scheduler_hit(self, label, c):
        if label == 'phase_post_start':
            self.scheduler_function_index += 1
            if self.scheduler_function_index >= len(self.scheduler_functions):
                raise RuntimeError('scheduler function attribution exceeded source definitions')
            self.scheduler_function = self.scheduler_functions[self.scheduler_function_index]
        elif label == 'scheduler_block':
            self.scheduler_current = dict(event='scheduler_block', function=self.scheduler_function,
                                          block=self.u32(self.base + 0x867a4), dag=[], rounds=[])
            self.events.append(self.scheduler_current)
        elif label == 'scheduler_dag':
            if self.scheduler_current is None:
                raise RuntimeError('scheduler DAG arrived without a basic block')
            self.scheduler_current['dag'] = self.scheduler_dag()
        elif label == 'scheduler_cost':
            ins_addr = self.u32(c.Edi + 4)
            self.scheduler_round.append(dict(dag=c.Edi, ins=ins_addr,
                cost=C.c_int32(c.Eax).value, height=self.u32(c.Edi + 20),
                stallable=self.u32(c.Edi + 16) & 255, id=self.u32(ins_addr + 52),
                sequence=struct.unpack('<H', self.read(ins_addr + 58, 2))[0],
                last_seq=self.u32(c.Ebp - 4) & 65535, top=c.Esi, adjust_tie=False))
        elif label == 'scheduler_adjust':
            if self.scheduler_round:
                self.scheduler_round[-1]['adjust_tie'] = bool(c.Eax & 255)
        elif label == 'scheduler_selected':
            if self.scheduler_current is None:
                raise RuntimeError('scheduler selection arrived without a basic block')
            self.scheduler_current['rounds'].append(dict(candidates=self.scheduler_round,
                chosen=c.Ecx, ins=self.u32(c.Ecx + 4)))
            self.scheduler_round = []

def definitions(text):
    """C89 definitions in source order; unsupported preprocessing fails closed.

    This deliberately is not a C parser. Grouping must also agree with the
    observed home-pass count and OMF publics; macro-generated functions require
    a future compiler-symbol hook rather than guessed attribution.
    """
    text = re.sub(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',
                  lambda m: ' ' * len(m.group()), text, flags=re.S)
    text = re.sub(r'^\s*#(?:[^\n\\]|\\\n)*', '', text, flags=re.M)
    depth, depths = 0, []
    for char in text:
        depths.append(depth)
        depth += (char == '{') - (char == '}')
    return [m.group(1) for m in re.finditer(r'\b(\w+)\s*\([^;{}]*\)\s*\{', text)
            if depths[m.start()] == 0 and m.group(1) not in ('if', 'while', 'for', 'switch')]


def group_events(names, events, publics):
    count = sum(e['event'] == 'homes_return' for e in events)
    if len(names) != count or len({n.casefold() for n in names}) != len(names) or not set(publics) <= set(names):
        raise ValueError(f'cannot attribute functions: definitions={len(names)}, '
                         f'home passes={count}, unmatched publics={set(publics) - set(names)}')
    result = {name: [] for name in names}
    index = 0
    for event in events:
        if event['event'] == 'payload_loaded':
            continue
        if index >= len(names):
            raise ValueError('unexpected compiler event after final function')
        result[names[index]].append(event)
        if event['event'] == 'homes_return':
            index += 1
    for name, rows in result.items():
        phases = [r['stage'] for r in rows if r['event'] == 'phase']
        if phases != ['phase_regalloc', 'phase_post_start', 'phase_after_LdStAlloc',
                      'phase_after_Score', 'phase_after_LdStCompress']:
            raise ValueError(f'{name}: incomplete/out-of-order phase capture: {phases}')
        for event in rows:
            if event['event'] in ('best', 'rover') and 'chosen' not in event:
                raise ValueError(f'{name}: incomplete {event["event"]} capture')
            if event['event'] == 'sort' and 'output' not in event:
                raise ValueError(f'{name}: incomplete sort capture')
            if event['event'] == 'change_ins' and 'success' not in event:
                raise ValueError(f'{name}: incomplete substitution capture')
        request = {0: 0, 1: 0, 2: 0}
        for event in rows:
            if event['event'] == 'rover':
                width = event['kind'] // 2
                event['request_index'] = request.setdefault(width, 0)
                request[width] += 1
    return result


def function_trace(name, events):
    result = dict(schema=1, function=name,
                phases={e['stage'].removeprefix('phase_'): e['graph'] for e in events if e['event'] == 'phase'},
                home_sorts=[e for e in events if e['event'] == 'sort' and e['comparator'] == '0x5eb49'],
                conflict_sorts=[e for e in events if e['event'] == 'sort' and e['comparator'] == '0x61a08'],
                colours=[e for e in events if e['event'] == 'best'],
                rover=[e for e in events if e['event'] == 'rover'],
                score=[e for e in events if e['event'] == 'change_ins' and e['pass_stage']=='phase_after_LdStAlloc'],
                # Auxiliary lifecycle/home/ring records; phase graphs and primary
                # decisions already appear above, so do not serialize them twice.
                events=[e for e in events if e['event'] not in ('phase','best','rover','change_ins','scheduler_block')])
    blocks = [e for e in events if e['event'] == 'scheduler_block']
    if blocks:
        result['scheduler'] = [dict(block=e['block'], dag=e['dag'], rounds=e['rounds']) for e in blocks]
    return result


def require_identity(observed, normal):
    a, b = Path(observed).read_bytes(), Path(normal).read_bytes()
    if not a or a != b:
        raise RuntimeError('REFUSED: observed OMF differs from normal build; no trace published')
    return hashlib.sha256(a).hexdigest()


def trace(source, out, profile='game', func=None, schedule=False):
    """Serial, bounded compile through unchanged wcc.compile_c, with normal control.

    Only wcc's subprocess adapter and generated TMP root are scoped here. The
    global subprocess module, wrapper code, flags, include paths and environment
    are unchanged. No output from a failed/unequal run is accepted as a trace.
    """
    if os.name != 'nt' or C.sizeof(C.c_void_p) != 8:
        raise RuntimeError('observation requires 64-bit Windows Python and WOW64 debugging')
    if C.sizeof(CTX) != 716:
        raise RuntimeError('unexpected WOW64 context layout')
    source, out = Path(source).resolve(), Path(out).resolve()
    p = wcc.profile(profile)
    if p['install'] != 'wc106' or source.suffix.lower() != '.c':
        raise ValueError('observer RVAs require the pinned wc106 C compiler')
    # install() caches successful pins; explicitly refresh for each invocation.
    wcc._checked.discard('wc106')
    ent, base = wcc.install('wc106')
    payload = (base / ent['wcc386']['payload']).read_bytes()
    header = struct.unpack_from('<H', payload, 8)[0] * 16
    sig, start, size, reloc, memory, entry = struct.unpack_from('<6I', payload, header)
    if sig != 0x4643 or start + size != len(payload):
        raise RuntimeError('unexpected pinned CF payload layout')
    names = definitions(source.read_text())
    if func and func not in names:
        raise ValueError(f'function definition not found: {func}')
    # A fresh destination prevents stale manifests from surviving a failed run.
    out.mkdir(parents=True, exist_ok=False)
    old_build, old_subprocess = wcc.BUILD, wcc.subprocess
    source_hash = hashlib.sha256(source.read_bytes()).hexdigest()
    try:
        wcc.BUILD = out
        with tempfile.TemporaryDirectory(prefix='compile_', dir=out) as work:
            work = Path(work)
            obj, normal = work / 'observed.obj', work / 'normal.obj'
            obs = Observer(payload[start:start + 16], scheduler=schedule, functions=names)
            wcc.subprocess = SimpleNamespace(run=obs.run)
            rc, log = wcc.compile_c(source, obj, p['flags'], p['install'])
            (out / 'observed.log').write_text(log)
            wcc.subprocess = old_subprocess
            if rc or not obj.is_file():
                raise RuntimeError(f'observed compile failed ({rc}): {log}')
            wcc._checked.discard('wc106')
            rc, log = wcc.compile_c(source, normal, p['flags'], p['install'])
            (out / 'normal.log').write_text(log)
            if rc:
                raise RuntimeError(f'normal compile failed ({rc}): {log}')
            digest = require_identity(obj, normal)
            if hashlib.sha256(source.read_bytes()).hexdigest() != source_hash:
                raise RuntimeError('source changed during observation')
            import omf
            import match
            publics = [match.demangle(n) for m in omf.load(obj) for n, si, off, typ in m.publics
                       if m.segments[si].cls.upper() == 'CODE']
            groups = group_events(names, obs.events, publics)
            if any(e['event'] == 'exception' for e in obs.events):
                raise RuntimeError('unexpected compiler exception; trace refused')
            (out / 'observed.obj').write_bytes(obj.read_bytes())
            for name, events in groups.items():
                if func is None or name == func:
                    (out / (name + '.json')).write_text(json.dumps(function_trace(name, events), indent=2))
            manifest = dict(schema=1, source=str(source), source_sha256=source_hash,
                            profile=profile, install='wc106', flags=p['flags'],
                            whole_omf_equal=True, omf_sha256=digest,
                            compiler_pins=ent['key_files'], functions=[func] if func else names,
                            attribution='source definition order, home-count/phase/public checked')
            if schedule:
                manifest['scheduler'] = True
            (out / 'manifest.json').write_text(json.dumps(manifest, indent=2))
            return manifest
    finally:
        wcc.BUILD, wcc.subprocess = old_build, old_subprocess


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('source', type=Path)
    parser.add_argument('--profile', default='game')
    parser.add_argument('--func')
    parser.add_argument('--schedule', action='store_true', help='capture read-only scheduler DAGs and selections')
    parser.add_argument('--out', required=True, type=Path, help='new output directory')
    args = parser.parse_args(argv)
    try:
        result = trace(args.source, args.out, args.profile, args.func, args.schedule)
    except (RuntimeError, ValueError, OSError) as exc:
        parser.exit(1, f'cgtrace: {exc}\n')
    print(f'OMF identity PASS; {len(result["functions"])} function trace(s) in {args.out}')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
