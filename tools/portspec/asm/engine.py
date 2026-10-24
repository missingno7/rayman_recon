"""Oracle runner and independent model state. No source/tool/oracle writes.

Addresses are test coordinates, not claimed PMODE/W runtime addresses.
Expected memory comes exclusively from Python models, never a second x86 run.
"""
import sys, struct, random, copy
from pathlib import Path
sys.dont_write_bytecode=True
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]
sys.path.insert(0,str(ROOT/'tools'))
from image import Image
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_MEM_WRITE, UC_HOOK_MEM_READ, UC_HOOK_CODE, UC_HOOK_BLOCK, UC_HOOK_INSN, UC_HOOK_INTR
from unicorn.x86_const import *
import capstone

BASE={1:0x1000000,2:0x2000000}
BUF=0x4000000; SRC=BUF+0x40000; AUX=BUF+0x80000
STACK=0x8000000; SP=STACK+0x10000; STOP=BASE[1]+0xF0000
REGS={n:globals()['UC_X86_REG_'+n.upper()] for n in ('eax','ebx','ecx','edx','esi','edi','ebp','esp')}
def u32(v): return v & 0xffffffff
def s32(v): return (v&0x7fffffff)-(v&0x80000000)
def lo(v,n=8): return v&((1<<n)-1)
def setlo(v,x,n=8): return (v&~((1<<n)-1))|lo(x,n)
def ah(v,x): return (v&~0xff00)|((x&255)<<8)

class Memory:
    def __init__(self,segments):
        self.segments={a:bytearray(v) for a,v in segments.items()}; self.writes=set(); self.vga_writes={}; self.plane_mask=15; self.seq_index=2
        self.vga_planes=None;self.vga_latches=[0]*4;self.vga_mode=0;self.gc_index=0
    def locate(self,a,n):
        for b,v in self.segments.items():
            if b<=a and a+n<=b+len(v): return v,a-b
        raise ValueError('unmapped model memory '+hex(a)+' size '+str(n))
    def read(self,a,n):
        if self.vga_planes is not None and 0xa0000<=a and a+n<=0xc0000:
            self.vga_latches=[p[a-0xa0000+n-1] for p in self.vga_planes]
            return bytes(self.vga_planes[0][a-0xa0000:a-0xa0000+n])
        v,i=self.locate(a,n); return bytes(v[i:i+n])
    def write(self,a,data):
        v,i=self.locate(a,len(data)); v[i:i+len(data)]=data; self.writes.update(range(a,a+len(data)))
        for j,x in enumerate(data):
            if 0xA0000<=a+j<0xC0000:
                for plane in range(4):
                    if self.plane_mask&(1<<plane):
                        value=self.vga_latches[plane] if self.vga_mode==1 and self.vga_planes is not None else x
                        self.vga_writes[(plane,a+j)]=value
                        if self.vga_planes is not None:self.vga_planes[plane][a+j-0xa0000]=value
    def get(self,a,n=4,signed=False): return int.from_bytes(self.read(a,n),'little',signed=signed)
    def put(self,a,v,n=4): self.write(a,(v&((1<<(8*n))-1)).to_bytes(n,'little'))

class State:
    def __init__(self,segments,regs,inputs=None):
        self.mem=Memory(segments); self.r=regs.copy(); self.io=[]; self.inputs=copy.deepcopy(inputs or {}); self.input_pos={}; self.cf=None;self.flags=None;self.flagmask=0
    def push(self,v,n=4): self.r['esp']-=n; self.mem.put(self.r['esp'],v,n)
    def pop(self,n=4):
        v=self.mem.get(self.r['esp'],n); self.r['esp']+=n; return v
    def pushad(self):
        saved=self.r.copy()
        for r in ('eax','ecx','edx','ebx','esp','ebp','esi','edi'): self.push(saved[r])
    def popad(self):
        for r in ('edi','esi','ebp',None,'ebx','edx','ecx','eax'):
            v=self.pop()
            if r:self.r[r]=v
    def inp(self,port,size=1):
        i=self.input_pos.get(port,0); seq=self.inputs[port]
        if i>=len(seq): raise ValueError('input exhausted '+hex(port))
        v=seq[i]; self.input_pos[port]=i+1; self.io.append(['in',port,size,v]); return v
    def out(self,port,value,size=1):
        self.io.append(['out',port,size,lo(value,size*8)])
        if port==0x3c4:
            self.mem.seq_index=value&255
            if size==2 and value&255==2:self.mem.plane_mask=(value>>8)&15
        elif port==0x3c5 and self.mem.seq_index==2:self.mem.plane_mask=value&15
        elif port==0x3ce:self.mem.gc_index=value&255
        elif port==0x3cf and self.mem.gc_index==5:self.mem.vga_mode=value&3
    def ret(self,extra=0): self.r['esp']+=4+extra
    def movs(self,n,count=1,direction=1):
        for _ in range(count):
            self.mem.write(self.r['edi'],self.mem.read(self.r['esi'],n))
            self.r['esi']=u32(self.r['esi']+n*direction); self.r['edi']=u32(self.r['edi']+n*direction)
    def stos(self,n,count=1,direction=1):
        for _ in range(count):
            self.mem.put(self.r['edi'],self.r['eax'],n); self.r['edi']=u32(self.r['edi']+n*direction)

class Oracle:
    def __init__(self):
        self.image=Image(); self.segments={}
        for obj,raw in self.image.obj.items():
            data=bytearray(raw)
            for (o,off),(kind,tobj,toff) in self.image.fixups.items():
                if o!=obj: continue
                assert kind==7, ('unexpected fixup',kind)
                struct.pack_into('<I',data,off,u32(BASE[tobj]+toff))
            # Keep decoded buffers in memory only.
            virtual=next(o['virtual_size'] for o in self.image.meta['objects'] if o['index']==obj)
            extent=max(len(data),virtual,0x100000 if obj==1 else 0)
            self.segments[BASE[obj]]=bytes(data)+bytes(((extent+4095)&~4095)-len(data))
        self.md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32); self.md.detail=True
    def segments_for(self,rng):
        seg=self.segments.copy()
        seg[BUF]=rng.randbytes(0xc0000)
        seg[STACK]=rng.randbytes(0x20000)
        seg[0xA0000]=rng.randbytes(0x20000)
        return seg
    def run(self,off,initial,flags=2,limit=1000000):
        uc=Uc(UC_ARCH_X86,UC_MODE_32)
        for a,data in initial.mem.segments.items():
            uc.mem_map(a,(len(data)+4095)&~4095); uc.mem_write(a,bytes(data))
        if initial.inputs.get('protected_flat'):
            gdt=AUX+0x30000
            uc.mem_write(gdt,struct.pack('<QQQ',0,0x00cf9a000000ffff,0x00cf92000000ffff))
            uc.reg_write(UC_X86_REG_GDTR,(0,gdt,23,0))
            uc.reg_write(UC_X86_REG_CR0,uc.reg_read(UC_X86_REG_CR0)|1)
            uc.reg_write(UC_X86_REG_CS,8)
            for segment in (UC_X86_REG_DS,UC_X86_REG_ES,UC_X86_REG_SS):uc.reg_write(segment,16)
        for n,k in REGS.items(): uc.reg_write(k,initial.r[n])
        uc.reg_write(UC_X86_REG_EFLAGS,flags)
        writes=set(); blocks=set(); instructions=set(); io=[]; pos={}; error=None;vga_writes={};vga={'mask':15,'index':2,'mode':0,'gc_index':0,'latches':[0]*4}
        banks=copy.deepcopy(initial.mem.vga_planes)
        def wr(u,access,a,n,value,user):
            writes.update(range(a,a+n))
            if 0xA0000<=a<0xC0000:
                for j in range(n):
                    for p in range(4):
                        if vga['mask']&(1<<p):
                            val=vga['latches'][p] if banks is not None and vga['mode']==1 else (value>>(8*j))&255
                            vga_writes[(p,a+j)]=val
                            if banks is not None:banks[p][a+j-0xa0000]=val
        def rd(u,access,a,n,value,user):
            if banks is not None and 0xa0000<=a and a+n<=0xc0000:
                vga['latches']=[p[a-0xa0000+n-1] for p in banks]
                u.mem_write(a,bytes(banks[0][a-0xa0000:a-0xa0000+n]))
        def block(u,a,n,user): blocks.add(a-BASE[1])
        def ins(u,a,n,user):
            off=a-BASE[1];instructions.add(off)
            if 'cpu_capabilities' in initial.inputs:
                ac,ident=initial.inputs['cpu_capabilities']
                if off==0x6135:
                    if not ac:u.reg_write(UC_X86_REG_EFLAGS,u.reg_read(UC_X86_REG_EFLAGS)&~0x40000)
                    io.append(['flag_probe','AC',ac])
                elif off==0x614e:
                    if not ident:u.reg_write(UC_X86_REG_EFLAGS,u.reg_read(UC_X86_REG_EFLAGS)&~0x200000)
                    io.append(['flag_probe','ID',ident])
        def inp(u,port,size,user):
            i=pos.get(port,0); seq=initial.inputs[port]
            if i>=len(seq): raise ValueError('oracle input exhausted '+hex(port))
            v=seq[i]; pos[port]=i+1; io.append(['in',port,size,v]); return v
        def out(u,port,size,value,user):
            io.append(['out',port,size,value])
            if port==0x3c4:
                vga['index']=value&255
                if size==2 and value&255==2:vga['mask']=(value>>8)&15
            elif port==0x3c5 and vga['index']==2:vga['mask']=value&15
            elif port==0x3ce:vga['gc_index']=value&255
            elif port==0x3cf and vga['gc_index']==5:vga['mode']=value&3
        def intr(u,num,user):
            # BIOS services are abstract external effects with caller-specified response.
            regs={n:u.reg_read(k) for n,k in REGS.items()}
            io.append(['int',num,regs['eax'],regs['ebx'],regs['ecx'],regs['edx'],u.reg_read(UC_X86_REG_ES)])
            for n,v in initial.inputs.get(('int',num),{}).items(): u.reg_write(REGS[n],v)
        def cpuid(u,user):
            response=initial.inputs['cpu_response'];io.append(['cpuid',u.reg_read(UC_X86_REG_EAX),u.reg_read(UC_X86_REG_ECX)])
            for n,v in response.items():u.reg_write(REGS[n],v)
            return True
        uc.hook_add(UC_HOOK_MEM_WRITE,wr)
        if banks is not None:uc.hook_add(UC_HOOK_MEM_READ,rd)
        uc.hook_add(UC_HOOK_BLOCK,block)
        uc.hook_add(UC_HOOK_CODE,ins)
        uc.hook_add(UC_HOOK_INSN,inp,None,1,0,UC_X86_INS_IN)
        uc.hook_add(UC_HOOK_INSN,out,None,1,0,UC_X86_INS_OUT)
        uc.hook_add(UC_HOOK_INTR,intr)
        if 'cpu_response' in initial.inputs:uc.hook_add(UC_HOOK_INSN,cpuid,None,1,0,UC_X86_INS_CPUID)
        try: uc.emu_start(BASE[1]+off,STOP,count=limit)
        except Exception as e: error=repr(e)
        if not error and uc.reg_read(UC_X86_REG_EIP)!=STOP: error='instruction limit'
        return dict(uc=uc,writes=writes,blocks=blocks,instructions=instructions,io=io,
            regs={n:uc.reg_read(k) for n,k in REGS.items()},flags=uc.reg_read(UC_X86_REG_EFLAGS),error=error,vga_writes=vga_writes,es=uc.reg_read(UC_X86_REG_ES))
    def cfg(self,off):
        """Reachable intraprocedural instruction/basic-block starts; no linear data decoding.
        Calls are fallthrough, helper coverage is reported separately. Indirect jumps stay OPEN.
        """
        pending=[off]; seen={}; leaders={off}; unresolved=[]
        while pending:
            a=pending.pop()
            if a in seen or not (0x10<=a<0x6f80): continue
            ii=list(self.md.disasm(self.image.code[a:a+15],a,count=1))
            if not ii: unresolved.append(a); continue
            i=ii[0]; seen[a]=i; nxt=a+i.size
            if i.group(capstone.CS_GRP_RET) or i.group(capstone.CS_GRP_IRET): continue
            if i.group(capstone.CS_GRP_JUMP):
                if i.operands[0].type==capstone.x86.X86_OP_IMM:
                    target=i.operands[0].imm; leaders.add(target); pending.append(target)
                else:
                    # PROVEN bounded computed dispatches in draw_effect_segment.
                    # Counts are constrained by width<=320 and lead/tail<=3.
                    targets={0x751:[0x79b-24*j for j in range(4)],
                             0x7b9:[0x25bb-96*j for j in range(81)],
                             0x25d7:[0x2621-24*j for j in range(4)]}.get(a)
                    if targets:
                        leaders.update(targets);pending.extend(targets)
                    else:unresolved.append(a)
                if i.mnemonic!='jmp': leaders.add(nxt);pending.append(nxt)
            else: pending.append(nxt)
        return {'instructions':set(seen),'blocks':leaders&set(seen),'unresolved':unresolved}

def compare(expected,actual,stack=True):
    errors=[]
    if actual['error']: return [{'kind':'oracle_execution','detail':actual['error']}]
    for n,v in expected.r.items():
        if u32(v)!=actual['regs'][n]: errors.append({'kind':'register','register':n,'expected':hex(u32(v)),'actual':hex(actual['regs'][n])})
    if expected.cf is not None and expected.cf!=(actual['flags']&1): errors.append({'kind':'carry','expected':expected.cf,'actual':actual['flags']&1})
    if expected.flags is not None and (expected.flags^actual['flags'])&expected.flagmask:errors.append({'kind':'flags','expected':hex(expected.flags),'actual':hex(actual['flags']),'mask':hex(expected.flagmask)})
    if expected.inputs.get('protected_flat') and actual['es']!=16:errors.append({'kind':'preserved_segment','register':'ES','expected':16,'actual':actual['es']})
    if expected.io!=actual['io']: errors.append({'kind':'io_sequence','expected':expected.io,'actual':actual['io']})
    if expected.mem.vga_writes!=actual['vga_writes']:
        keys=expected.mem.vga_writes.keys()|actual['vga_writes'].keys()
        errors.append({'kind':'VGA_plane_bytes','diffs':[{'plane':p,'address':hex(a),'expected':expected.mem.vga_writes.get((p,a)),'actual':actual['vga_writes'].get((p,a))} for p,a in sorted(keys) if expected.mem.vga_writes.get((p,a))!=actual['vga_writes'].get((p,a))]})
    ew=expected.mem.writes; aw=actual['writes']
    if not stack:
        ew={a for a in ew if not STACK<=a<STACK+0x20000};aw={a for a in aw if not STACK<=a<STACK+0x20000}
    missing=ew-aw; extra=aw-ew
    if missing or extra: errors.append({'kind':'written_byte_set','missing_count':len(missing),'extra_count':len(extra),'missing_first':hex(min(missing)) if missing else None,'extra_first':hex(min(extra)) if extra else None})
    diffs=[]
    addresses=sorted(ew|aw); ranges=[]
    for a in addresses:
        if ranges and a==ranges[-1][1]: ranges[-1][1]+=1
        else:ranges.append([a,a+1])
    for a,b in ranges:
        ev=expected.mem.read(a,b-a); av=bytes(actual['uc'].mem_read(a,b-a))
        if ev!=av:
            diffs.extend({'address':hex(a+i),'expected':x,'actual':y} for i,(x,y) in enumerate(zip(ev,av)) if x!=y)
    if diffs: errors.append({'kind':'memory','count':len(diffs),'bytes':diffs})
    return errors
