from models import *
from records import RUN,pushregs,popregs
MAP={'name':'map_tile_t;src/include/map.h (RAYVERSE structure, unknown fields retained)','size':6,'fields':[
 {'offset':0,'width':2,'signed':False,'meaning':'texture_id'}, {'offset':2,'width':1,'signed':False,'meaning':'tile_type'},
 {'offset':3,'width':1,'signed':False,'meaning':'field_3'}, {'offset':4,'width':1,'signed':False,'meaning':'transparency:0=opaque map cache,nonzero=background visible'},
 {'offset':5,'width':1,'signed':False,'meaning':'field_5'}]}
def row_cross(s,cachepos,cachebase):
    r=s.r;pushregs(s,'edi','ecx');s.push(r['edx']);s.push(r['eax']);s.push(r['ebx']);s.push(cachebase);s.push(cachebase);s.pop();s.push(cachebase);s.pop();s.pop();s.r['ebx']=s.pop();s.r['eax']=s.pop();s.r['edx']=s.pop();popregs(s,'edi','ecx')
    return ((cachepos-cachebase+4*r['ecx'])&65535)//80!=((cachepos-cachebase)&65535)//80

def build(s,name):
    r=s.r;first=name=='asm_02850';inner=name=='asm_02a73';s.push(r['ebp'])
    if first:s.push(r['ebp']);s.mem.put(r['esp'],0);s.push(r['ecx'])
    s.push(r['ebx']);cachepos=r['ebx'];cachebase=r['ecx'];r['ebx']=cachebase+(0x4600 if first else 0);r['edx']-=128;r['ecx']=setlo(r['ecx'],14 if inner else 0)
    if inner:r['eax']+=120+r['edi']
    for y in range(14):
        pushregs(s,'ecx','ebx');r['edx']+=128;r['ebx']=setlo(r['ebx'],1 if first else 0,16);r['ebp']=u32(-12);s.mem.put(r['edx'],0);r['ecx']=0;s.push(r['edi'])
        if inner:r['eax']+=6;r['esi']+=4;cachepos+=4;s.mem.put(r['esp']+12,cachepos)
        for col in range(19 if inner else 20):
            r['ecx']=col;category=s.mem.get(r['eax']+4,1);r['edi']=setlo(r['edi'],category,16);previous=r['ebx']&65535;want=category==0 if first else category!=0;new=False
            if want:
                if first:
                    if previous:
                        s.mem.put(r['esp']+20,0);cross=row_cross(s,cachepos,cachebase)
                        if cross:s.mem.put(r['esp']+20,80)
                        new=True
                    else:
                        cross=row_cross(s,cachepos,cachebase);flag=80 if cross else 0
                        if s.mem.get(r['esp']+20)!=flag:s.mem.put(r['esp']+20,flag);new=True
                else:new=not previous
                if new:
                    pushregs(s,'edi','ecx');n=s.mem.get(r['edx']);s.mem.put(r['edx'],n+1);r['ebp']=u32(r['ebp']+12)
                    source=cachepos+(0 if inner else col*4)
                    if first:source-=s.mem.get(r['esp']+28)
                    if inner:cachepos+=4;s.mem.put(r['esp']+20,cachepos)
                    entry=r['edx']+r['ebp']+4
                    for off,value in [(0,source),(4,r['esi']),(8,0)]:s.mem.put(entry+off,value)
                    popregs(s,'edi','ecx')
                s.mem.put(r['edx']+r['ebp']+12,s.mem.get(r['edx']+r['ebp']+12)+1)
            r['ebx']=setlo(r['ebx'],category,16);r['eax']+=6;r['esi']+=4
            if inner and not new:cachepos+=4;s.mem.put(r['esp']+12,cachepos)
        r['edi']=s.pop();r['eax']+=r['edi'];r['esi']+=1200;popregs(s,'ecx','ebx')
        if first:
            cachepos+=1280
            if cachepos>=r['ebx']:cachepos-=0x4600
        elif inner:cachepos+=1200
        else:cachepos+=r['ebx']
        s.mem.put(r['esp'],cachepos);r['ecx']=setlo(r['ecx'],13-y if inner else y+1)
    r['ebx']=s.pop()
    if first:r['ecx']=s.pop();s.pop()
    r['ebp']=s.pop();ret(s)

for name in ('asm_02850','asm_029d3','asm_02a73'):
    def fn(s,n=name):build(s,n)
    desc={'asm_02850':'Build14 rows of20 map tiles, emitting runs of byte+4==0. Source cache starts in EBX, wraps at ECX+0x4600 by subtracting0x4600 after each1280-byte tile row. Split runs at80-byte cache-row seams; on wrapped horizontal segment subtract80 from source. Destination starts ESI and advances4 per tile,1280 per row.',
          'asm_029d3':'Build14 rows of20 map tiles, emitting runs of byte+4!=0. Source starts EBX and advances ECX per tile row; destination ESI advances4 per tile,1280 per row.',
          'asm_02a73':'Skip initial map row, then build14 rows of19 inner tiles starting at column1. Emit runs of byte+4!=0. Source saved EBX and destination ESI advance4 per tile including skipped first column,1280 per row. This variant ignores ECX as source stride when advancing the saved source slot.'}[name]
    model(name,desc+' Each128-byte output row starts with run count;12-byte entries are source address,destination address,tile count. EAX advances through6-byte map records; EDI is additional map row skip in bytes and is preserved. EDX ends on last output row; ESI ends14*1280 beyond start. '+('EBX is advanced through one full0x4600-byte circular plane and returns to its original value; ECX and EBP are preserved.' if name=='asm_02850' else 'Saved EBX is advanced and returned modified; EBP preserved.'),
          'Mapped map with at least14 rows (15 for02a73),width>=20; row skip=(map_width-20)*6; output14x128 bytes; category0/nonzero; at most10 runs per row; cache-row positions valid for8-bit quotient division.',layouts=[MAP,RUN])(fn)

def prepare(s,name,rng,seed):
    if name not in ('asm_02850','asm_029d3','asm_02a73'):return
    width=rng.choice([20,24,32,40]);tiles=AUX+0x2000;table=AUX;source=SRC+0x2000
    s.r.update(eax=tiles,edx=table,ebx=source+rng.randrange(224)*80+rng.randrange(20)*4,ecx=source if name=='asm_02850' else rng.choice([1280,1536,2048]),esi=0xa0500,edi=(width-20)*6)
    for y in range(16):
        for x in range(width):s.mem.put(tiles+6*(width*y+x)+4,rng.choice([0,0,1,2]),1)
