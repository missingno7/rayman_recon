from models import *

@model('Copy_Plan0Diff_To_Buf','For positive ECX width/EBX height, copy ceil(width/8)*8 bytes per row from ESI with source pitchEDX to EDI with destination pitch320. Skip nonpositive dimensions. Preserve GPRs.','Mapped disjoint source/destination including7 padding bytes after logical width.')
def plan(s):
    r=s.r;s.pushad();w=s32(r['ecx']);h=s32(r['ebx'])
    if w>0 and h>0:
        r['esp']-=8;s.mem.put(r['esp']+4,h);s.mem.put(r['esp'],w);n=(w+7)//8;s.mem.put(r['esp'],n)
        src,dst,pitch=r['esi'],r['edi'],r['edx']
        for y in range(h):
            s.push(dst+320*y);s.push(pitch*(y+1));copy_bytes(s,dst+320*y,src+pitch*y,n*8);s.pop();s.pop();s.mem.put(r['esp']+4,h-y-1)
        r['esp']+=8
    s.popad();ret(s)

@model('asm_04d55','Copy positive rectangle in8x8 groups, width rounded up to8 bytes, source pitchEDX,destination pitch320. Ignore nonpositive width/height; positive height MUST be a multiple8. Preserve GPRs.','Height positive multiple8, or nonpositive; source/destination mapped including rounded width.')
def blocks(s):
    r=s.r;s.pushad();w=s32(r['ecx']);h=s32(r['ebx'])
    if w>0 and h>0:
        src,dst,pitch=r['esi'],r['edi'],r['edx'];r['esp']-=20;s.mem.put(r['esp']+4,h);s.mem.put(r['esp'],w);s.mem.put(r['esp']+16,src);n=(w+7)//8;s.mem.put(r['esp'],n)
        for by in range(0,h,8):
            s.mem.put(r['esp']+8,dst+320*by);s.mem.put(r['esp']+12,pitch*(by+8))
            for bx in range(n):
                for y in range(8):copy_bytes(s,dst+320*(by+y)+8*bx,src+pitch*(by+y)+8*bx,8)
            s.mem.put(r['esp']+4,h-by-8)
        r['esp']+=20
    s.popad();ret(s)

def prepare(s,name,rng,seed):
    if name not in ('Copy_Plan0Diff_To_Buf','asm_04d55'):return
    w=rng.choice([-1,0,1,7,8,9,16,23,64]);h=rng.choice([-1,0,8,16,24]) if name=='asm_04d55' else rng.choice([-1,0,1,2,8,16])
    s.r.update(ecx=u32(w),ebx=u32(h),edx=rng.choice([80,128,256]))
