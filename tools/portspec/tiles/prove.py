"""Differential execute ALL distinct shipped callbacks against a pixel model.
No historical executable is run on the host. Unicorn emulates isolated x86-32.
"""
import argparse
import json
import random
import time
from collections import Counter
import unicorn as uc
from unicorn import x86_const as x
from catalogue import census
from extract import HERE, sha

CODE=0x100000
STOP=0x110000
DEST=0x200000
STACK=0x300000
SOURCE=0x400000
SIZE=0x4000
REGS={n:getattr(x,'UC_X86_REG_'+n.upper()) for n in ('eax','ebx','ecx','edx','esi','edi','ebp','esp')}
ARITH=0x8d5  # OF SF ZF AF PF CF

def arithmetic_flags(a,b,bits,subtract=False):
    mask=(1<<bits)-1;sign=1<<(bits-1)
    result=(a-b if subtract else a+b)&mask
    carry=a<b if subtract else a+b>mask
    overflow=((a^b)&(a^result) if subtract else ~(a^b)&(a^result))&sign
    return (int(carry)|((result&255).bit_count()%2==0)*4|
            ((a^b^result)&16)|(result==0)*64|
            (bool(result&sign))*128|bool(overflow)*2048)

def model_draw(initial, image, phase, origin):
    """Straightforward independent per-pixel masked image drawing, fixed pitch 80.
    Returns the complete destination and exact set of bytes that must be written.
    """
    result=bytearray(initial);writes={}
    if phase<4:
        for y in range(16):
            for col in range(4):
                value=image[y*16+phase+4*col]
                if value is not None:
                    at=origin+y*80+col
                    result[at]=value
                    writes[DEST+at]=value
    return bytes(result),writes

def check_body(record,rng,trials,stats):
    body=record['body'];image=record['model']['image']
    u=uc.Uc(uc.UC_ARCH_X86,uc.UC_MODE_32)
    u.mem_map(CODE,0x20000)
    u.mem_write(CODE,body)
    u.mem_protect(CODE,0x20000,uc.UC_PROT_READ|uc.UC_PROT_EXEC)
    u.mem_map(DEST,SIZE)
    u.mem_map(STACK,SIZE)
    u.mem_map(SOURCE,SIZE)
    source=rng.randbytes(SIZE)
    u.mem_write(SOURCE,source)
    u.mem_protect(SOURCE,SIZE,uc.UC_PROT_READ)
    stack=rng.randbytes(SIZE)
    stack=stack[:0x2000]+STOP.to_bytes(4,'little')+stack[0x2004:]
    u.mem_write(STACK,stack)
    actual={};reads=[];write_calls=[]
    def write_hook(u,access,address,size,value,data):
        write_calls.append((address,size))
        assert DEST<=address and address+size<=DEST+SIZE, ('outside destination',hex(address))
        for k in range(size):
            assert address+k not in actual, ('duplicate write',hex(address+k))
            actual[address+k]=(value>>(8*k))&255
    def read_hook(u,access,address,size,value,data):
        reads.append((address,size))
    u.hook_add(uc.UC_HOOK_MEM_WRITE,write_hook)
    u.hook_add(uc.UC_HOOK_MEM_READ,read_hook)
    for trial in range(trials):
        for phase in (0,1,2,3,4,255):
            initial=(bytes(SIZE) if trial%5==3 else bytes([255])*SIZE if trial%5==4 else rng.randbytes(SIZE))
            # all address residues and page-boundary crossings occur across the census.
            origin=0x1000+rng.randrange(-7,8)
            u.mem_write(DEST,initial)
            before={n:rng.getrandbits(32) for n in REGS}
            before.update(eax=(before['eax'] & ~0xff00)|(phase<<8),ecx=240,
                          edi=DEST+origin,esp=STACK+0x2000)
            # ESI points at random source data on half the trials; the other trials
            # use an arbitrary (usually unmapped) address, proving it is not read.
            if trial%2==0:before['esi']=SOURCE+rng.randrange(SIZE)
            for n,value in before.items():u.reg_write(REGS[n],value)
            flags=2 | (rng.getrandbits(1)<<10) | (rng.getrandbits(1)<<9) | (rng.getrandbits(12)&ARITH)
            u.reg_write(x.UC_X86_REG_EFLAGS,flags)
            expected,writes=model_draw(initial,image,phase,origin)
            actual.clear();reads.clear();write_calls.clear()
            u.emu_start(CODE,STOP,count=1000)
            assert u.reg_read(x.UC_X86_REG_EIP)==STOP, 'did not return'
            assert bytes(u.mem_read(DEST,SIZE))==expected, ('destination',sha(body),phase)
            assert actual==writes, ('write set',sha(body),phase)
            assert reads==[(STACK+0x2000,4)], ('unexpected data read',reads)
            for n in ('eax','ebx','ecx','esi','ebp'):
                assert u.reg_read(REGS[n])==before[n], ('preservation',n,sha(body),phase)
            assert u.reg_read(REGS['edx'])==80
            assert u.reg_read(REGS['edi'])==before['edi']+(1200 if phase<4 else 0)
            assert u.reg_read(REGS['esp'])==before['esp']+4
            assert (u.reg_read(x.UC_X86_REG_EFLAGS) ^ flags)&(~ARITH & 0xffffffff)==0
            want_flags=(arithmetic_flags(before['edi']+4*240,240,32) if phase<4
                        else arithmetic_flags(phase,3,8,subtract=True))
            assert u.reg_read(x.UC_X86_REG_EFLAGS)&ARITH==want_flags
            assert bytes(u.mem_read(STACK,SIZE))==stack
            assert bytes(u.mem_read(SOURCE,SIZE))==source
            stats['executions']+=1
            stats['checked_pixel_writes']+=len(writes)
            stats['checked_store_instructions']+=len(write_calls)
            stats['valid_phase_runs' if phase<4 else 'out_of_range_phase_runs']+=1
    stats['distinct_bodies_tested']+=1

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--trials',type=int,default=5)
    p.add_argument('--seed',type=int,default=0x54494c45)
    args=p.parse_args();assert args.trials>0
    started=time.time();ls,records,catalogue=census()
    rng=random.Random(args.seed);stats=Counter();shape_counts=Counter()
    ordered=sorted(records.items())
    for k,(bid,r) in enumerate(ordered,1):
        check_body(r,rng,args.trials,stats)
        shape_counts[r['shape']]+=6*args.trials
        if k%250==0:print(f'{k}/{len(ordered)} bodies; {stats["executions"]} runs',flush=True)
    negative_controls=[]
    sample=next(r for r in records.values() if 0<r['model']['written_pixels']<256)
    for label,find,change in [('wrong_color',lambda p:p is not None,lambda p:(p%255)+1),
                              ('wrong_mask',lambda p:p is None,lambda p:17)]:
        wrong=list(sample['model']['image'])
        at=next(i for i,pixel in enumerate(wrong) if find(pixel))
        wrong[at]=change(wrong[at])
        bad=dict(sample,model=dict(sample['model'],image=wrong))
        try:
            check_body(bad,random.Random(args.seed),1,Counter())
        except AssertionError as error:
            assert error.args[0][0]=='destination', error
            negative_controls.append(dict(control=label,result='EXPECTED_MISMATCH'))
        else:raise AssertionError(('negative control accepted',label))
    out=dict(result='PASS',evidence='EQUIVALENT_TESTED (not STRICT)',
             unicorn=uc.__version__,seed=args.seed,trials=args.trials,
             phases=[0,1,2,3,4,255],summary=dict(stats),
             levels_covered=len(ls),normalized_shapes_tested=len(shape_counts),
             level_bodies_covered=catalogue['summary']['level_bodies'],
             all_unique_bodies_covered=len(records),shape_executions=dict(shape_counts),
             destination_patterns=['random','all_zero','all_255'] if args.trials>=5 else ['random'],
             negative_controls=negative_controls,
             body_sha256s=sorted(records),elapsed_seconds=round(time.time()-started,3))
    (HERE/'proof.json').write_text(json.dumps(out,indent=1)+'\n')
    print(json.dumps({k:v for k,v in out.items() if k not in ('shape_executions','body_sha256s')},indent=2))

if __name__=='__main__':main()
