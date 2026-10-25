"""Native C vs ORIGINAL GOG v1.21 bytes. No expected pixels come from a Python model.

Run: python -B port/tests/asm_diff/test_lowlevel_native.py
Optional: --count 256 --entries NAME ... (still runs negative controls).
Only build/port-asmdiff is written (DLLs, results.json); fixture imports have bytecode disabled.
Origin: build/workers/port_video (2026-10-25), integrated into port/video.
"""
import sys
sys.dont_write_bytecode = True
import argparse
import ctypes as C
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time
import unittest

PORT = Path(__file__).resolve().parents[2]
ROOT = PORT.parent
WORK = ROOT / 'build' / 'port-asmdiff'
WORK.mkdir(parents=True, exist_ok=True)
sys.path.insert(0, str(ROOT / 'tools/portspec/asm'))
import engine
import models
import prove
import raster
import sprites
import tiles
for module in prove.MODULES:
    __import__(module)

NAMES = '''DisplayBufferModeNormal DrawWldPointPlan2Normal DrawBlackBoxNormal DrawFondBoxNormal
DrawBorderBoxNormal DisplayAnyPictureNormal ClearDrawAndDisplayBufferNormal N_CLRSCR Swap_To_Screen
clear_borders_Normal Copy_Plan0Diff_To_Buf asm_04d55 Display_Bloc Display_Bloc_Plein Display_Bloc_Clippe
Display_Bloc_Plein_Clippe fplot_Normal flocon_clipping draw_flocon1_Normal draw_flocon2_Normal
draw_flocon3_Normal draw_flocon4_Normal draw_flocon5_Normal draw_flocon6_Normal draw_flocon7_Normal
draw_normal_5755 draw_pluie5_Normal draw_pluie6_Normal draw_pluie7_Normal default_sprite_clipping
get_sprite_clipping get_sprite_clipping_xmin get_sprite_clipping_xmax get_sprite_clipping_ymin
get_sprite_clipping_ymax sprite_clipping clip_sprite_on_screen clip_sprite_on_screen_flipped
DrawSpriteNormalNoClip DrawSpriteNormal DrawSpriteNormal256 DrawSpriteFlipNormalNoClip DrawSpriteFlipNormal
DrawSpriteFlipNormal256 DrawSpriteColorNormal DrawSpriteColorFlipNormal DrawSpriteDiffNormal copy_dwords
draw_sprite_deform local_04a83 effect_reset_geometry draw_effect_segment'''.split()
P, I, U = C.c_void_p, C.c_int32, C.c_uint32

SHIM = r'''
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
int32_t Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2;
static unsigned notifications;
void port_vga_display_changed(void) { notifications++; }
unsigned test_notifications(void) { return notifications; }
typedef struct region { uintptr_t base; size_t size, first, last; unsigned char *bits; } region;
static region regions[96]; static unsigned used, recent;
void test_trace_clear(void) {
    for (unsigned i=0;i<used;i++) free(regions[i].bits);
    used=recent=notifications=0;
}
unsigned test_trace_region(void *p,size_t n) {
    unsigned i=used++; regions[i]=(region){(uintptr_t)p,n,n,0,calloc(n,1)}; return i;
}
void test_trace_reset(void) {
    for (unsigned i=0;i<used;i++) {
        memset(regions[i].bits,0,regions[i].size);
        regions[i].first=regions[i].size; regions[i].last=0;
    }
    recent=notifications=0;
}
void port_normal_trace_write(const void *p,size_t n) {
    uintptr_t a=(uintptr_t)p; unsigned i=recent;
    if (i>=used || a<regions[i].base || a+n>regions[i].base+regions[i].size) {
        for (i=0;i<used;i++) if (a>=regions[i].base && a+n<=regions[i].base+regions[i].size) break;
        if (i==used) abort();
        recent=i;
    }
    size_t off=a-regions[i].base;
    memset(regions[i].bits+off,1,n);
    if (off<regions[i].first) regions[i].first=off;
    if (off+n>regions[i].last) regions[i].last=off+n;
}
const unsigned char *test_trace_bits(unsigned i,size_t *first,size_t *last) {
    *first=regions[i].first; *last=regions[i].last; return regions[i].bits;
}
'''


def compile_library(mutated=False):
    out = WORK / 'native'
    out.mkdir(exist_ok=True)
    shim = out / 'device_stub.c'
    shim.write_text(SHIM)
    path = out / ('lowlevel_mutated.dll' if mutated else 'lowlevel_normal.dll')
    cmd = [os.environ.get('CC', 'C:/msys64/mingw64/bin/gcc.exe'), '-std=c11', '-O2', '-shared', '-fwrapv', '-funsigned-char',
           '-Wall', '-Wextra', '-Werror', '-DLOWLEVEL_NORMAL_TRACE']
    if mutated:
        cmd.append('-DLOWLEVEL_NORMAL_MUTATE')
    cmd += [str(PORT / 'video/lowlevel_normal.c'), str(shim), '-o', str(path)]
    result = subprocess.run(cmd, text=True, capture_output=True, cwd=WORK)
    if result.returncode:
        raise RuntimeError(' '.join(cmd) + '\n' + result.stdout + result.stderr)
    return Library(path), cmd


class Clip(C.Structure):
    _fields_ = [('x', I), ('y', I), ('size', U), ('source', P)]


class Library:
    def __init__(self, path):
        self.dll = C.CDLL(str(path))
        self.functions = {}
        self.declare('lowlevel_normal_state_region', [U, C.POINTER(C.c_size_t), C.POINTER(U)], P)
        self.declare('lowlevel_normal_state_region_count', [], U)
        self.declare('test_trace_clear', [])
        self.declare('test_trace_reset', [])
        self.declare('test_trace_region', [P, C.c_size_t], U)
        self.declare('test_trace_bits', [U, C.POINTER(C.c_size_t), C.POINTER(C.c_size_t)], P)
        self.declare('test_notifications', [], U)
        self.regions = []
        for i in range(self.functions['lowlevel_normal_state_region_count']()):
            size, kind = C.c_size_t(), U()
            ptr = self.functions['lowlevel_normal_state_region'](i, C.byref(size), C.byref(kind))
            self.regions.append((ptr, size.value, kind.value))
        self.declare('lowlevel_normal_set_strides', [U, U])
        self.declare('DisplayBufferModeNormal', [P, P])
        self.declare('ClearDrawAndDisplayBufferNormal', [P, P])
        self.declare('N_CLRSCR', [P])
        self.declare('DrawWldPointPlan2Normal', [P, I, I])
        self.declare('DrawBlackBoxNormal', [P, I, I, I, I])
        self.declare('DrawFondBoxNormal', [P, I, I, I, I, I])
        self.declare('DrawBorderBoxNormal', [P, I, I, I, I, C.c_uint16])
        self.declare('DisplayAnyPictureNormal', [P, P] + [I]*7)
        self.declare('Swap_To_Screen', [P, P, I, I])
        self.declare('clear_borders_Normal', [P, I, I])
        for n in ('Copy_Plan0Diff_To_Buf', 'asm_04d55'):
            self.declare(n, [P, P, I, I, I])
        for n in ('Display_Bloc', 'Display_Bloc_Plein', 'local_04a83'):
            self.declare(n, [P, P])
        for n in ('Display_Bloc_Clippe', 'Display_Bloc_Plein_Clippe'):
            self.declare(n, [P, P, I, I])
        self.declare('fplot_Normal', [P, I, I, I])
        self.declare('flocon_clipping', [I]*4)
        for n in NAMES:
            if n.startswith(('draw_flocon', 'draw_pluie', 'draw_normal_')):
                self.declare(n, [P, I, I])
            if n.startswith('DrawSprite') and n != 'DrawSpriteDiffNormal':
                self.declare(n, [I, I, I, U, P, P])
            if n.startswith('get_sprite_clipping_'):
                self.declare(n, [], I)
        self.declare('default_sprite_clipping', [])
        self.declare('get_sprite_clipping', [P]*4)
        self.declare('sprite_clipping', [I]*4)
        for n in ('clip_sprite_on_screen', 'clip_sprite_on_screen_flipped'):
            self.declare(n, [C.POINTER(Clip)], I)
        self.declare('DrawSpriteDiffNormal', [C.c_int16, C.c_int16, C.c_uint16, P, P, P])
        self.declare('copy_dwords', [P, P, I])
        self.declare('lowlevel_copy_dwords_direction', [P, P, U, I])
        self.declare('draw_sprite_deform', [P, P, P])
        self.declare('effect_reset_geometry', [])
        self.declare('draw_effect_segment', [P, P])

    def declare(self, name, args, result=None):
        fn = getattr(self.dll, name)
        fn.argtypes, fn.restype = args, result
        self.functions[name] = fn

    def __call__(self, name, *args):
        return self.functions[name](*args)


# Each native state region is mapped to the original persistent bytes. Lists
# represent scattered code immediates, never instruction/opcode comparisons.
B = engine.BASE[1]
STATE = [
    list(models.CLIP), [sprites.WIDTH], [sprites.COLOR], [B+0x6d2f],
    [B+0x4c86, B+0x4c8a], [B+0x5618+4*i for i in range(4)], [B+0x52d6],
    [B+0x47f0+4*i for i in range(8)], [B+0x4810], [B+0x4814], [B+0x4818],
    [B+0x481c], [B+0x4820], [B+0x4824], [raster.ROW],
    [B+0x815+96*i for i in range(80)],
]
for patch in raster.PATCH:
    STATE.extend([[B+a] for a in patch])
for key in ('destination', 'descriptor', 'texture', 'du', 'neg_du', 'dv', 'neg_dv',
            'first_y', 'rows_left', 'scanline', 'row_offset'):
    STATE.append([raster.R[key]])


def ranges(addresses):
    result = []
    for a in sorted(addresses):
        if result and result[-1][1] == a:
            result[-1][1] += 1
        else:
            result.append([a, a+1])
    return result


class NativeImage:
    def __init__(self, lib, initial, initialize_state=True):
        self.lib, self.initial = lib, initial
        self.allocations, self.trace, self.special = [], [], []
        lib('test_trace_clear')
        for base, data in initial.mem.segments.items():
            if base == engine.STACK:
                continue  # CPU stack/return addresses have no native C observable counterpart.
            block = (C.c_uint8 * len(data)).from_buffer_copy(data)
            ptr = C.addressof(block)
            assert ptr > 0xffffffff, 'proof must exercise real 64-bit pointers'
            self.allocations.append((base, len(data), ptr, block))
            trace_id = lib('test_trace_region', ptr, len(data))
            self.trace.append((trace_id, ptr, len(data), None))
        assert len(STATE) == len(lib.regions)
        for rid, ((ptr, size, kind), addresses) in enumerate(zip(lib.regions, STATE)):
            if not initialize_state:
                pass
            elif kind == 2:
                value = initial.mem.get(addresses[0])
                # DOS deform_source is biased by -1. Native state holds the real
                # first pixel, so it never constructs a pointer before the source.
                if rid == 8 and value:
                    value = engine.u32(value+1)
                C.c_void_p.from_address(ptr).value = self.pointer(value)
            elif kind == 1:
                assert size == 4*len(addresses)
                for i, a in enumerate(addresses):
                    C.c_uint32.from_address(ptr+4*i).value = initial.mem.get(a)
            else:
                C.memmove(ptr, initial.mem.read(addresses[0], size), size)
            trace_id = lib('test_trace_region', ptr, size)
            self.trace.append((trace_id, ptr, size, (rid, kind, addresses)))
        for name, address in zip(('Bloc_lim_W1','Bloc_lim_W2','Bloc_lim_H1','Bloc_lim_H2'), tiles.LIMIT):
            C.c_int32.in_dll(lib.dll, name).value = initial.mem.get(address, signed=True)

    def pointer(self, a):
        for base, size, ptr, block in self.allocations:
            if base <= a < base+size:
                return ptr+a-base
        return a  # Unmapped stale scratch pointers are retained as inert values.

    def output_pointer(self, a):
        # get_sprite_clipping explicitly permits aliasing its own clip words.
        for rid, ((ptr,size,kind),addresses) in enumerate(zip(self.lib.regions,STATE)):
            if kind==1:
                for i,dos in enumerate(addresses):
                    if dos<=a<dos+4:
                        return ptr+4*i+a-dos
        return self.pointer(a)

    def dos_pointer(self, p):
        p = p or 0
        for native, size, dos in self.special:
            if native <= p < native+size:
                return dos+p-native
        for base, size, ptr, block in self.allocations:
            if ptr <= p <= ptr+size:
                return base+p-ptr
        return p

    def state_bytes(self, rid):
        ptr, size, kind = self.lib.regions[rid]
        if kind == 2:
            v = self.dos_pointer(C.c_void_p.from_address(ptr).value)
            if rid == 8 and v:
                v = engine.u32(v-1)
            return engine.u32(v).to_bytes(4, 'little')
        if kind == 1:
            return b''.join(C.c_uint32.from_address(ptr+i).value.to_bytes(4, 'little') for i in range(0, size, 4))
        return C.string_at(ptr, size)

    def trace_output(self):
        written = set()
        state = {}
        for trace_id, ptr, size, info in self.trace:
            first, last = C.c_size_t(), C.c_size_t()
            bitmap = self.lib('test_trace_bits', trace_id, C.byref(first), C.byref(last))
            if last.value <= first.value:
                continue
            touched = [i+first.value for i, b in enumerate(C.string_at(bitmap+first.value, last.value-first.value)) if b]
            if not info:
                written.update(self.dos_pointer(ptr+i) for i in touched)
                continue
            rid, kind, addresses = info
            data = self.state_bytes(rid)
            if kind == 2:
                touched = list(range(4))
            for i in touched:
                a = addresses[i//4]+i%4 if kind == 1 else addresses[0]+i
                written.add(a)
            if kind == 1:
                for i in range(len(addresses)):
                    state.update((addresses[i]+j, data[4*i+j]) for j in range(4))
            else:
                state.update((addresses[0]+i, v) for i, v in enumerate(data))
        return written, state

    def read(self, address, n, overlay):
        data = bytearray(C.string_at(self.pointer(address), n))
        for i in range(n):
            if address+i in overlay:
                data[i] = overlay[address+i]
        return bytes(data)

    def run(self, name):
        s, lib = self.initial, self.lib
        r, p = s.r, self.pointer
        a,b,c,d = [engine.s32(r[k]) for k in ('eax','ebx','ecx','edx')]
        src, dst = p(r['esi']), p(r['edi'])
        result = None
        if name in ('DisplayBufferModeNormal','ClearDrawAndDisplayBufferNormal','Display_Bloc','Display_Bloc_Plein','local_04a83'):
            lib(name, *( (dst,src) if name=='ClearDrawAndDisplayBufferNormal' else (src,dst)))
        elif name == 'N_CLRSCR': lib(name, dst)
        elif name == 'DrawWldPointPlan2Normal': lib(name,dst,a,b)
        elif name == 'DrawBlackBoxNormal': lib(name,dst,a,b,c,d)
        elif name in ('DrawFondBoxNormal','DrawBorderBoxNormal'): lib(name,dst,engine.s32(r['esi']),b,c,d,a)
        elif name == 'DisplayAnyPictureNormal': lib(name,src,dst,a,b,c,d,*[s.mem.get(engine.SP+i) for i in (4,8,12)])
        elif name == 'Swap_To_Screen': lib(name,src,dst,c,a)
        elif name == 'clear_borders_Normal': lib(name,dst,c,d)
        elif name in ('Copy_Plan0Diff_To_Buf','asm_04d55'): lib(name,src,dst,c,b,d)
        elif name in ('Display_Bloc_Clippe','Display_Bloc_Plein_Clippe'): lib(name,src,dst,a,b)
        elif name == 'fplot_Normal': lib(name,dst,a,b,engine.s32(r['esi']))
        elif name == 'flocon_clipping': lib(name,a,b,c,d)
        elif name.startswith(('draw_flocon','draw_pluie','draw_normal_')): lib(name,dst,a,b)
        elif name in ('sprite_clipping',): lib(name,a,b,c,d)
        elif name == 'get_sprite_clipping': lib(name,*[self.output_pointer(r[k]) for k in ('eax','ebx','ecx','edx')])
        elif name.startswith('get_sprite_clipping_'): result=lib(name)
        elif name in ('default_sprite_clipping','effect_reset_geometry'): lib(name)
        elif name.startswith('clip_sprite_'):
            clip=Clip(a,b,r['ecx'],src)
            ok=lib(name,C.byref(clip))
            result={'ok':ok,'x':clip.x,'y':clip.y,'size':clip.size,'source':self.dos_pointer(clip.source)}
        elif name == 'DrawSpriteDiffNormal': lib(name,a,b,r['ecx'],p(r['edx']),dst,src)
        elif name.startswith('DrawSprite'): lib(name,a,d,b,r['ecx'],dst,src)
        elif name == 'copy_dwords':
            if s.inputs['direction']<0: lib('lowlevel_copy_dwords_direction',src,dst,r['ecx'],-1)
            else: lib(name,src,dst,c)
        elif name == 'draw_sprite_deform': lib(name,src,dst,p(r['ecx']))
        elif name == 'draw_effect_segment':
            desc=(C.c_uint8*(C.sizeof(P)+40))()
            ptr=C.addressof(desc)
            C.c_void_p.from_address(ptr).value=p(s.mem.get(r['esi']))
            C.memmove(ptr+C.sizeof(P),s.mem.read(r['esi']+4,40),40)
            self.special=[(ptr,C.sizeof(P),r['esi']), (ptr+C.sizeof(P),40,r['esi']+4)]
            self.native_descriptor=desc
            lib(name,p(r['eax']),ptr)
        else: raise AssertionError(name)
        return result


def compare_native(name, native, original, result):
    errors=[]
    if original['error']:
        return ['oracle execution: '+original['error']]
    actual, overlay = native.trace_output()
    expected = {a for a in original['writes'] if not engine.STACK<=a<engine.STACK+0x20000}
    if actual != expected:
        missing, extra = expected-actual, actual-expected
        errors.append(f'write set missing={len(missing)} first={hex(min(missing)) if missing else None}; '
                      f'extra={len(extra)} first={hex(min(extra)) if extra else None}')
    for a,b in ranges(expected|actual):
        want=bytes(original['uc'].mem_read(a,b-a)); got=native.read(a,b-a,overlay)
        if want!=got:
            i=next(i for i,(x,y) in enumerate(zip(want,got)) if x!=y)
            errors.append(f'bytes at {hex(a+i)}: DOS={want[i]:02x} native={got[i]:02x}')
            break
    # Compare ALL persistent bytes, including fields the invocation did not write.
    for rid, ((ptr,size,kind),addresses) in enumerate(zip(native.lib.regions,STATE)):
        got=native.state_bytes(rid)
        want=b''.join(bytes(original['uc'].mem_read(a,4)) for a in addresses) if kind==1 else bytes(original['uc'].mem_read(addresses[0],4 if kind==2 else size))
        if got!=want:
            errors.append(f'persistent region {rid}: DOS={want.hex()[:100]} native={got.hex()[:100]}')
    # Detect any untraced buffer writes as well as omitted stores of unchanged values.
    for base,size,ptr,block in native.allocations:
        if base != engine.BUF and base != 0xa0000:
            continue
        want=bytes(original['uc'].mem_read(base,size)); got=C.string_at(ptr,size)
        if want!=got:
            errors.append(f'full buffer differs at {hex(base+next(i for i,(x,y) in enumerate(zip(want,got)) if x!=y))}')
    if name.startswith('get_sprite_clipping_') and engine.u32(result)!=original['regs']['eax']:
        errors.append('returned clipping bound')
    if name.startswith('clip_sprite_'):
        want={'ok':1-(original['flags']&1), 'x':engine.s32(original['regs']['eax']),
              'y':engine.s32(original['regs']['ebx']), 'size':original['regs']['ecx'],'source':original['regs']['esi']}
        if result!=want: errors.append(f'clip output DOS={want} native={result}')
    if native.lib('test_notifications')!=(1 if name=='Swap_To_Screen' else 0):
        errors.append('display notification count')
    return errors


COUNT=256
SELECTED=NAMES


class NativeDifferentialTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.lib, cls.command=compile_library()
        cls.mutated, cls.mutated_command=compile_library(True)
        cls.oracle=engine.Oracle()
        cls.inventory={f['name']:f for f in json.loads((engine.HERE/'inventory.json').read_text())}
        cls.report={'proof':'finite native-C/original-DOS differential execution; not STRICT or universal proof',
                    'compiler_command':cls.command,'negative_compiler_command':cls.mutated_command,
                    'seed_start':0xA550000,'entries':{},'negative_controls':{},
                    'source_sha256':{str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest()
                                     for p in [PORT/'video/lowlevel_normal.c',PORT/'video/lowlevel_normal.h',Path(__file__)]}}

    @classmethod
    def tearDownClass(cls):
        (WORK/'results.json').write_text(json.dumps(cls.report,indent=2)+'\n')

    def execute(self,name,seed,lib=None):
        initial,flags=prove.prepare(self.oracle,name,seed)
        original=self.oracle.run(self.inventory[name]['offset'],initial,flags)
        native=NativeImage(lib or self.lib,initial)
        result=native.run(name)
        return compare_native(name,native,original,result),original

    def test_00_initial_state(self):
        state=engine.State(self.oracle.segments,{n:0 for n in engine.REGS})
        native=NativeImage(self.lib,state,initialize_state=False)
        for rid,((ptr,size,kind),addresses) in enumerate(zip(self.lib.regions,STATE)):
            want=b''.join(state.mem.read(a,4) for a in addresses) if kind==1 else state.mem.read(addresses[0],4 if kind==2 else size)
            self.assertEqual(native.state_bytes(rid),want,f'initial persistent region {rid}')
        self.report['initial_state']={'regions':len(STATE),'mismatches':0}
        print('Initial persistent state: 51 regions; 0 mismatches',flush=True)

    def test_01_all_entries(self):
        for name in SELECTED:
            began=time.monotonic(); mismatches=[]; visited=set(); cases=0
            for i in range(COUNT):
                try:
                    errors,original=self.execute(name,0xA550000+i)
                    visited.update(original['instructions'])
                    cases+=1
                    if errors:
                        mismatches.append({'seed':hex(0xA550000+i),'errors':errors})
                        break
                except Exception as exc:
                    mismatches.append({'seed':hex(0xA550000+i),'exception':repr(exc)})
                    break
            cfg=self.oracle.cfg(self.inventory[name]['offset'])
            notes='Existing prepare() valid domain; exact visible write set/values and all persistent bytes; CPU stack excluded.'
            if name=='copy_dwords': notes+=' Both DF directions; reverse uses explicit C direction adapter.'
            if name=='draw_sprite_deform': notes+=' Native source state is unbiased; DOS comparison rebiases by -1.'
            entry={'cases':cases,'mismatches':mismatches,'blocks_covered':len(cfg['blocks']&visited),
                   'blocks_total':len(cfg['blocks']),'uncovered':[hex(a) for a in sorted(cfg['blocks']-visited)],
                   'coverage_notes':notes,'seconds':round(time.monotonic()-began,3)}
            self.report['entries'][name]=entry
            print(f'{name}: cases={cases} mismatches={len(mismatches)} blocks={entry["blocks_covered"]}/{entry["blocks_total"]}',flush=True)
            with self.subTest(entry=name):
                self.assertFalse(mismatches,str(mismatches)[:4000])
                self.assertGreaterEqual(cases,256)
        (WORK/'results.json').write_text(json.dumps(self.report,indent=2)+'\n')

    def test_02_negative_controls(self):
        for name in ('DrawBlackBoxNormal','fplot_Normal','sprite_clipping'):
            errors,original=self.execute(name,0xA550000,self.mutated)
            self.report['negative_controls'][name]={'detected':bool(errors),'errors':errors}
            print(f'NEGATIVE {name}: {"DETECTED" if errors else "MISSED"}',flush=True)
            self.assertTrue(errors)

    def test_03_stride_and_state_sequences(self):
        # Setters have no DOS entry: compare their state to direct fixture state
        # setup, then execute the original consumers with the same legal pitches.
        lib=self.lib
        for name in ('Swap_To_Screen','Display_Bloc','Display_Bloc_Clippe','Display_Bloc_Plein_Clippe'):
            for i in range(256):
                initial,flags=prove.prepare(self.oracle,name,0xA550000+i)
                # These are mapped disjoint rectangles, within each contract.
                pitches=(256 if i%2 else 384, 384 if i%2 else 256)
                initial.mem.put(B+0x4c86,pitches[0]);initial.mem.put(B+0x4c8a,pitches[1]);initial.mem.writes.clear()
                original=self.oracle.run(self.inventory[name]['offset'],initial,flags)
                native=NativeImage(lib,initial)
                # Start with wrong native pitches to require the setter's effect.
                stride_ptr=lib.regions[4][0]
                C.c_uint32.from_address(stride_ptr).value=320
                C.c_uint32.from_address(stride_ptr+4).value=320
                lib('lowlevel_normal_set_strides',*pitches)
                self.assertEqual(native.state_bytes(4),b''.join(p.to_bytes(4,'little') for p in pitches))
                lib('test_trace_reset')
                result=native.run(name)
                self.assertFalse(compare_native(name,native,original,result),(name,i))
        self.report['stride_cases']={'entries':4,'cases_each':256,'mismatches':0,'pitches':[256,384]}
        print('Mutable strides: 4 entries x 256 cases; 0 mismatches',flush=True)

    def test_04_clipping_output_aliases(self):
        for i in range(256):
            initial,flags=prove.prepare(self.oracle,'get_sprite_clipping',0xA550000+i)
            # All-identical outputs, and cyclic overwrites of the source bounds.
            targets=[models.CLIP[(i+j+1)%4] for j in range(4)] if i%2 else [engine.AUX+16]*4
            initial.r.update(zip(('eax','ebx','ecx','edx'),targets))
            original=self.oracle.run(0x61bd,initial,flags)
            native=NativeImage(self.lib,initial)
            result=native.run('get_sprite_clipping')
            self.assertFalse(compare_native('get_sprite_clipping',native,original,result),i)
        self.report['alias_cases']={'entry':'get_sprite_clipping','cases':256,'mismatches':0}
        print('Clip output aliases: 256 cases; 0 mismatches',flush=True)

    def test_05_deformation_history(self):
        # Existing valid-domain witness: docs/port/PLATFORM_OPEN.md section 9,
        # build/workers/platform_open/deformation_proof.py. Preserve both native
        # buffers AND native file-scope state across calls; never seed from DOS output.
        segments=self.oracle.segments.copy()
        segments.update({engine.BUF:bytes(0xc0000),engine.STACK:bytes(0x20000)})
        regs={n:0 for n in engine.REGS}
        regs.update(esp=engine.SP,esi=engine.SRC+0x2000,edi=engine.BUF+0x2000,ecx=engine.AUX+0x2000)
        state=engine.State(segments,regs)
        state.mem.put(engine.SP,engine.STOP)
        native=NativeImage(self.lib,state)
        fixtures=[([1,40,40,1,1,40,0,4,40,4,0,40,4],0x81),
                  ([1,-32,0,4,1,32,32,1,32,4,128,4,32],0x17)]
        visited=set()
        for index,(descriptor,color) in enumerate(fixtures):
            desc=b''.join(engine.u32(v).to_bytes(4,'little') for v in descriptor)
            source=bytes([color])*(descriptor[11]*descriptor[12]+16)
            state.mem.write(regs['ecx'],desc);state.mem.write(regs['esi']-1,source)
            C.memmove(native.pointer(regs['ecx']),desc,len(desc))
            C.memmove(native.pointer(regs['esi']-1),source,len(source))
            state.r=regs.copy();state.mem.put(engine.SP,engine.STOP);state.mem.writes.clear()
            self.lib('test_trace_reset')
            original=self.oracle.run(0x4828,state,2)
            result=native.run('draw_sprite_deform')
            self.assertFalse(compare_native('draw_sprite_deform',native,original,result),index)
            visited.update(original['instructions'])
            if index==1:
                self.assertIn(0x49ed,original['instructions'])
                self.assertEqual(C.string_at(native.pointer(regs['edi']+144),16),bytes([0x17])*16)
            for base,data in state.mem.segments.items():
                data[:]=original['uc'].mem_read(base,len(data))
        self.report['deformation_history']={'cases':2,'mismatches':0,'ascending_store_049ed_covered':True}
        entry=self.report['entries'].get('draw_sprite_deform')
        if entry:
            missing={int(a,16) for a in entry['uncovered']}-visited
            entry['uncovered']=[hex(a) for a in sorted(missing)]
            entry['blocks_covered']=entry['blocks_total']-len(missing)
            entry['coverage_notes']+=' Two-call caller-proven scratch-history witness covers 1:049ed.'
        print('Deformation history: 2 cases; 0 mismatches; ascending 1:049ed covered',flush=True)


if __name__=='__main__':
    parser=argparse.ArgumentParser()
    parser.add_argument('--count',type=int,default=256)
    parser.add_argument('--entries',nargs='+',choices=NAMES)
    args=parser.parse_args()
    COUNT=args.count
    SELECTED=args.entries or NAMES
    unittest.main(argv=[sys.argv[0]],verbosity=2)
