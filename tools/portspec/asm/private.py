from models import *
from sprites import clip
from hardware import advance
from tiles import LIMIT

for i,name in enumerate(('get_sprite_clipping_xmin','get_sprite_clipping_xmax','get_sprite_clipping_ymin','get_sprite_clipping_ymax')):
    def fn(s,i=i):s.r['eax']=s.mem.get(CLIP[i]);ret(s)
    model(name,'Return current '+('xmin','xmax','ymin','ymax')[i]+' signed32 clip bound in EAX; preserve other GPRs. Private unreferenced semantic entry.','No arguments.',globals_read=[('XMIN','XMAX','YMIN','YMAX')[i]])(fn)

@model('asm_06d33','Private differential-sprite clip helper. Save original CL width at code1:06d2f. Left trim rounds down to4 bytes and advances ESI and mask pointer at caller stack+4; left path skips subsequent right clipping. Convert width to floor(width/4), or ceil(right-cropped width/4). Top trim advances image/mask by original_width*rows. Top-trim path leaves EDX from16-bit MUL, so bottom check uses y rather than y+remaining_height (original asymmetry). CF=1 rejects. All register outputs and live stack pointer update are defined by sprites.clip(diff=True).','Packed positive byte dimensions; original-width source/mask mapped with dword padding; ordered clip box; caller stack+4 holds mask pointer.',globals_read=['sprite clip box'],globals_written=['code1:06d2f original width','caller stack+4 mask pointer'])
def diffclip(s):clip(s,diff=True)

@model('loc_052DA','Private normal-tile clipping helper. CL must be16,CH height16. Save original width at code1:052d6. Left trim rounds down to4, advances ESI/EDI; right kept width rounds up to4; output CL counts dwords. Top trim advances ESI by original width*rows and EDI by mutable destination pitch*rows; shift caller row flags at stack+4 left by2 per skipped row. Update x/y/dimensions; CF=1 rejects.','Called with ECX=0x1010, valid Bloc_lim ordered viewport; mapped source/destination; writable flags dword at caller stack+4.',globals_read=['Bloc_lim_W1/W2/H1/H2','code1:04c86 destination pitch'],globals_written=['code1:052d6 original width','caller stack+4 row flags'])
def tileclip(s):
    r=s.r;x=s32(r['eax']);y=s32(r['ebx']);w=r['ecx']&255;h=(r['ecx']>>8)&255;xmin,xmax,ymin,ymax=[s.mem.get(a,signed=True) for a in LIMIT]
    r['edx']=w;s.mem.put(BASE[1]+0x52d6,w)
    def reject():s.cf=1;ret(s)
    if x<xmin:
        r['eax']=xmin-x
        if w<=r['eax']:reject();return
        r['eax']&=0xfffc;r['edx']=w-r['eax'];r['ecx']=setlo(r['ecx'],r['edx']);r['esi']+=r['eax'];r['edi']+=r['eax'];r['eax']=u32(xmin);r['ecx']=setlo(r['ecx'],(r['ecx']&255)//4)
    else:
        r['edx']=u32(r['edx']+r['eax'])
        if r['edx']>u32(xmax):
            if r['eax']>=u32(xmax):reject();return
            r['edx']=u32(xmax-r['eax']);r['ecx']=setlo(r['ecx'],(r['edx']+3)//4)
        else:r['ecx']=setlo(r['ecx'],4)
    r['edx']=(r['ecx']>>8)&255
    if y<ymin:
        r['edx']=u32(r['edx']+r['ebx'])
        if s32(r['edx'])<=ymin:reject();return
        skip=(ymin-y)&65535;r['ebx']=skip;r['edx']=h-skip;r['ecx']=ah(r['ecx'],h-skip);s.push(r['eax'])
        r['esi']+=w*skip;r['edi']+=s.mem.get(BASE[1]+0x4c86)*skip
        s.mem.put(r['esp']+8,s.mem.get(r['esp']+8)<<(2*skip));r['eax']=s.pop();r['ebx']=u32(ymin);r['edx']=(r['ecx']>>8)&255
    r['edx']=u32(r['edx']+r['ebx'])
    if r['edx']>u32(ymax):
        if r['ebx']>=u32(ymax):reject();return
        r['edx']=u32(ymax-r['ebx']);r['ecx']=ah(r['ecx'],r['edx'])
    s.cf=0;ret(s)

@model('asm_0275c','Private unreferenced planar upload. Copy four16000-byte source planes separated by ECX to one aperture destination using masks1,2,4,8 (word OUT0x3C4 at1:02769);16000 bytes are copied as4000 dwords, with incoming DF. End ESI=original+3*plane_pitch,EAX AH=16; preserve other GPRs.','Mapped4-plane source and VGA destination; DF=0 or1, with both traversed intervals mapped.')
def upload(s):
    r=s.r;r['eax']=ah(r['eax'],1);r['esi']-=r['ecx']
    for plane in range(4):
        s.push(r['edi']);s.push(r['edx']);s.push(r['eax']);s.out(0x3c4,(1<<plane)<<8|2,2);r['eax']=s.pop();r['edx']=s.pop()
        r['esi']+=r['ecx'];s.push(r['esi']);s.push(r['ecx']);s.movs(4,4000,s.inputs['direction']);r['ecx']=s.pop();r['esi']=s.pop();r['edi']=s.pop();r['eax']=ah(r['eax'],2<<plane)
    ret(s)

@model('local_04a83','Convert packed320x200 source into four contiguous RAM planes, using destination TableImageX[x&3]+floor(x/4)+80*y. Source advances64000 bytes; EDI and EBP preserved. End ECX=320,EDX=16000,EAX=TableImageX[3]+15999,EBX=last source pixel.','Disjoint mapped64000-byte source/destination; TableImageX[4] contains plane offsets0,16000,32000,48000.',globals_read=['TableImageX[4]'])
def transpose(s):
    r=s.r;src,dst=r['esi'],r['edi'];table=binding('TableImageX');last=0
    for y in range(200):
        for x in range(320):
            at=s.mem.get(table+4*(x&3))+x//4+y*80;last=s.mem.get(src+y*320+x,1);s.mem.put(dst+at,last,1)
    r.update(eax=s.mem.get(table+12)+15999,ebx=last,ecx=320,edx=16000,esi=src+64000);ret(s)

@model('asm_0309d','Newly inventoried private unreferenced world-map-like stamp (purpose INVENTED). No clipping. Four successive plane columns contain [none,12,0],[12,14,10,0],[0,10,9,0],[none,0,0]; explicit zeros ARE written. Initial word OUT0x3C4 selects1<<(x&3), then advance through helper1:02b30. End EAX=last mask<<8|2,EBX=64*y; preserve ECX/EDX/ESI/EDI/EBP.','x0..319,y0..199 with mapped stamp footprint. Not included in prior109+8 inventory.')
def stamp(s):
    r=s.r;x,y=r['eax'],r['ebx'];s.push(r['edi']);s.push(r['ecx']);s.push(r['edx']);r['edi']+=x//4+80*y;r['eax']=((1<<(x&3))<<8)|2;r['ebx']=y*64;r['edx']=setlo(r['edx'],0x3c4,16);s.out(0x3c4,r['eax'],2)
    for i,column in enumerate([[None,12,0],[12,14,10,0],[0,10,9,0],[None,0,0]]):
        if i:s.push(BASE[1]+[0x30d6,0x30f0,0x310a][i-1]);advance(s)
        for y,v in enumerate(column):
            if v is not None:s.mem.put(r['edi']+80*y,v,1)
    r['edx']=s.pop();r['ecx']=s.pop();r['edi']=s.pop();ret(s)

def prepare(s,name,rng,seed):
    if name.startswith('get_sprite_clipping_'):
        for a in CLIP:s.mem.put(a,rng.randrange(400))
    elif name in ('asm_06d33','loc_052DA'):
        if name=='loc_052DA':
            for a,v in zip(LIMIT,(16,240,8,152)):s.mem.put(a,v)
            s.r.update(eax=u32(rng.choice([-20,0,4,8,12,16,232,236,239,240,244])),ebx=u32(rng.choice([-20,0,4,8,136,144,148,152,155])),ecx=0x1010)
            s.mem.put(SP+4,rng.getrandbits(32))
        else:
            for a,v in zip(CLIP,(16,120,8,140)):s.mem.put(a,v)
            s.r.update(eax=u32(rng.choice([-20,0,4,8,12,16,100,116,119,120,124])),ebx=u32(rng.choice([-20,0,4,8,130,136,140,145])),ecx=(rng.choice([1,8,32,128,255])<<8)|rng.choice([4,8,32,128,252]));s.mem.put(SP+4,AUX+0x4000)
    elif name=='asm_0275c':
        direction=1 if seed%2 else -1;s.inputs['direction']=direction;s.r.update(esi=SRC+0x14000,edi=0xa0500 if direction==1 else 0xa4500,ecx=0x4000)
    elif name=='asm_0309d':s.r.update(edi=0xa0500,eax=rng.randrange(320),ebx=rng.randrange(200))
