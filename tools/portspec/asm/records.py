from models import *
RUN={'name':'row run list (INVENTED fields; src/frame.c declares u32[14][32])','stride':128,'fields':[
 {'offset':0,'width':4,'signed':False,'meaning':'number of12-byte run entries,0..10'},
 {'offset':4,'width':4,'signed':False,'meaning':'run[0].source: byte address in selected background/cache plane'},
 {'offset':8,'width':4,'signed':False,'meaning':'run[0].destination: VGA aperture byte address or offset added to EDI'},
 {'offset':12,'width':4,'signed':False,'meaning':'run[0].tile_count: horizontal16-pixel tiles (4 aperture bytes each)'},
 {'offset':16,'width':0,'signed':False,'meaning':'remaining entries repeat at12-byte stride'}]}
CTX={'name':'frame_context_t first3 fields; purpose names INVENTED,src/frame.c','fields':[
 {'offset':0,'width':4,'signed':False,'meaning':'plane_size (saved then discarded by these renderers)'},
 {'offset':4,'width':4,'signed':False,'meaning':'source_end threshold; also returned in ESI'},
 {'offset':8,'width':4,'signed':False,'meaning':'run_step: source wrap subtraction (caller03257 uses plan0bit_length-1)'}]}

def pushregs(s,*names):
    for n in names:s.push(s.r[n])
def popregs(s,*names):
    for n in reversed(names):s.r[n]=s.pop()
def strip(s,n):
    """One/two-byte vertical16-row strip, plus exact callee-local scratch."""
    r=s.r;pushregs(s,'ebp','ebx','ecx','esi','edi');src,dst,pitch=r['esi'],r['edi'],r['edx']
    for y in range(16):s.mem.write(dst+80*y,s.mem.read(src+pitch*y,n))
    if n==1:r['eax']=setlo(r['eax'],(s.mem.get(src+14*pitch,1)<<8)|s.mem.get(src+15*pitch,1),16)
    else:r['eax']=setlo(r['eax'],s.mem.get(src+14*pitch,2),16)
    popregs(s,'ebp','ebx','ecx','esi','edi')
def trailing(s,n,offset):
    r=s.r;pushregs(s,'esi','edi','ecx');r['ecx']=r['ecx']*4-offset;r['esi']+=r['ecx'];r['edi']+=r['ecx'];strip(s,n);popregs(s,'esi','edi','ecx')

def rectangle(s):
    r=s.r
    if r['edi']&1:
        strip(s,1);trailing(s,1,1);r['edi']+=1;r['esi']+=1
        if r['edi']&2:
            strip(s,2);r['esi']+=2;r['edi']+=2;r['ecx']-=1
        else:trailing(s,2,4);r['ecx']-=1
    elif r['edi']&2:
        strip(s,2);trailing(s,2,2);r['ecx']-=1;r['esi']+=2;r['edi']+=2
    s.push(r['esi'])
    if r['ecx']:
        n=r['ecx']*4;r['eax']=80-n
        for y in range(16):s.mem.write(r['edi']+80*y,s.mem.read(r['esi']+r['edx']*y,n))
    r['esi']=s.pop()

def renderer(s,name):
    r=s.r;context=r['ecx'];table=r['esi'];rows=12 if name=='asm_03ea1' else 14
    s.push(r['ebp'])
    for off in (8,4,0):s.push(s.mem.get(context+off))
    r['ebp']=table;r['eax']=ah(r['eax'],r['eax']);pushregs(s,'edx','eax');s.out(0x3c4,(r['eax']&0xff00)|2,2);popregs(s,'edx','eax')
    r['ecx']=0
    for y in range(rows):
        r['ecx']=y;r['ebp']=table+128*y;pushregs(s,'ecx','ebp');records=s.mem.get(r['ebp']);r['ebp']+=4
        for i in range(records):
            pushregs(s,'edi','ecx') # ECX is record countdown, not row number
            # Replace count scratch just pushed with the actual live countdown.
            s.mem.put(r['esp'],records-i)
            r['esi']=u32(s.mem.get(r['ebp'])+r['ebx'])
            if r['esi']>=s.mem.get(context+4):r['esi']=u32(r['esi']-s.mem.get(context+8))
            r['edi']+=s.mem.get(r['ebp']+4);r['ecx']=s.mem.get(r['ebp']+8)
            pushregs(s,'ebp','ebx','ecx','edx');rectangle(s);popregs(s,'ebp','ebx','ecx','edx');popregs(s,'edi','ecx')
            r['ecx']-=1;r['ebp']+=12
        popregs(s,'ecx','ebp')
    r['ecx']=rows;r['esi']=s.pop();r['esi']=s.pop();r['esp']+=4;r['ebp']=s.pop();ret(s)

for name in ('asm_03257','asm_03912','asm_03ea1'):
    def fn(s,n=name):renderer(s,n)
    model(name,'Render '+('12' if name=='asm_03ea1' else '14')+'128-byte row lists. Each run copies tile_count*4 bytes on16 scanlines at source pitchEDX and destination pitch80. Source=run.source+EBX; when source>=context.source_end subtract context.run_step once. Destination=run.destination+EDI. AL selects VGA plane through word OUT0x3C4 (index2). Alignment fragments copy first/last bytes or words, then middle dwords; visible result is the same rectangle. ESI returns context.source_end; ECX returns row count; EAX is last copy scratch (fully specified by model); preserve EBX/EDX/EDI/EBP.','DF=0; list count0..10; positive tile_count<=20; mapped source/destination rectangles, wrap bound valid; EDI offset0 or1 at C callers. No globals read.',layouts=[RUN,CTX])(fn)

@model('asm_037e4','VGA latch-copy path. Set graphics register5 write-mode bits to1 via OUT0x3CE=5,IN/OUT0x3CF at1:037ef..037f7; select all planes with OUT0x3C4,word0x0F02. Copy4 aperture bytes per row for184 rows from0xAB502 to ECX+0x502 at pitch80. Then process12 run-list rows: each nonzero tile count copies4*count+1 aperture bytes on16 rows,source/destination pitch80,source/dest absolute addresses from run entries. A read latches all4 source planes and each subsequent byte write copies all4 latches (CPU byte itself is ignored by VGA write-mode1). Finally read graphics register5 and clear low2 bits via1:03906..0390c. CLI around run copies,final STI. EBP preserved; other GPR scratch outputs defined in model.','DF=0; valid VGA source/destination rectangles, run count0..10,tile count0..19;4 independent mapped VGA planes. Hardware latch behavior is an abstract device stub with exact port-sequence and logical-plane-byte comparison.',layouts=[RUN])
def latchcopy(s):
    r=s.r;table=r['esi'];dest=r['ecx'];s.push(r['ebp']);r['ebp']=table;pushregs(s,'edx','eax');s.out(0x3ce,5);s.out(0x3cf,(s.inp(0x3cf)&252)|1);popregs(s,'edx','eax')
    r['edx']=setlo(r['edx'],0x3c4,16);r['eax']=setlo(r['eax'],0xf02,16);s.out(0x3c4,0xf02,2);r['edi']=dest+0x502;r['esi']=0xab502
    for y in range(184):s.movs(1,4);r['edi']+=76;r['esi']+=76
    for y in range(12):
        r['ecx']=y;r['ebp']=table+128*y;pushregs(s,'ecx','ebp');count=s.mem.get(r['ebp']);r['ebp']+=4
        for i in range(count):
            r['ecx']=count-i;s.push(r['ecx']);r['esi']=s.mem.get(r['ebp']);r['edi']=s.mem.get(r['ebp']+4);r['ecx']=s.mem.get(r['ebp']+8)
            if r['ecx']:
                n=r['ecx']*4+1;r['ebx']=n;r['eax']=80-n;r['edx']=80-n
                for row in range(16):
                    s.movs(1,n)
                    if row<15:r['esi']+=r['edx'];r['edi']+=r['eax']
            r['ecx']=s.pop()-1;r['ebp']+=12
        popregs(s,'ecx','ebp')
    r['ecx']=12;pushregs(s,'edx','eax');s.out(0x3ce,5);s.out(0x3cf,s.inp(0x3cf)&252);popregs(s,'edx','eax');r['ebp']=s.pop();ret(s)

def prepare(s,name,rng,seed):
    if name=='asm_037e4':
        table=AUX;s.r.update(esi=table,ecx=0xa0500);value=rng.randrange(256);s.inputs[0x3cf]=[value,(value&252)|1]
        s.mem.vga_planes=[bytearray(s.mem.segments[0xa0000])]+[bytearray(rng.randbytes(0x20000)) for _ in range(3)]
        for y in range(12):
            count=rng.choice([0,1,2,3]);s.mem.put(table+128*y,count)
            for i in range(count):
                source=0xab000+y*8*80+rng.randrange(8);dst=0xa0500+y*8*80+rng.randrange(8);tiles=rng.choice([0,1,2,4,8])
                for j,v in enumerate((source,dst,tiles)):s.mem.put(table+128*y+4+12*i+4*j,v)
        return
    if name not in ('asm_03257','asm_03912','asm_03ea1'):return
    table=AUX;ctx=AUX+0x1000;src=SRC+0x2000;dest=0xa0500;offset=rng.choice([0,256,512])
    s.r.update(eax=rng.choice([1,2,4,8]),edx=rng.choice([80,96,128]),ebx=offset,ecx=ctx,esi=table,edi=rng.randrange(2))
    for off,v in [(0,0x4600),(4,src+0x4600),(8,0x4600)]:s.mem.put(ctx+off,v)
    for y in range(14):
        count=rng.choice([0,0,1,2,3]);s.mem.put(table+128*y,count)
        for i in range(count):
            source=src+rng.randrange(80)
            if rng.randrange(2):source+=0x4600
            dst=dest+y*80*8+rng.randrange(8);tiles=rng.choice([1,2,3,4,8])
            for j,v in enumerate((source,dst,tiles)):s.mem.put(table+128*y+4+12*i+4*j,v)
