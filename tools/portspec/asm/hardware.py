from models import *
SAVE=binding('saved_vga_registers_0196a')
REGS_VGA=[(0x3ce,5),(0x3ce,6),(0x3c4,4),(0x3c4,2),(0x3d4,20),(0x3d4,23)]

@model('InitFirstModeVideo','Request BIOS INT10h AX=0x13 (1:045fc..04603), then save6 VGA register bytes. In order, OUT index / IN data: (0x3CE,5),(0x3CE,6),(0x3C4,4),(0x3C4,2),(0x3D4,0x14),(0x3D4,0x17). Data port=index port+1. Preserve GPRs.','Abstract BIOS and six VGA reads; service side effects excluded.',layouts=[{'name':'saved_vga_registers_0196a (INVENTED)','fields':[{'offset':i,'width':1,'signed':False,'meaning':hex(port)+' index '+hex(index)} for i,(port,index) in enumerate(REGS_VGA)]}],globals_written=['saved_vga_registers_0196a[6]'])
def first(s):
    s.pushad();r=s.r;s.io.append(['int',0x10,setlo(r['eax'],0x13,16),r['ebx'],r['ecx'],r['edx'],0])
    for i,(port,index) in enumerate(REGS_VGA):s.out(port,index);s.mem.put(SAVE+i,s.inp(port+1),1)
    s.popad();ret(s)

def mode(s,x):
    s.pushad()
    for i,(port,index) in enumerate(REGS_VGA):
        v=s.mem.get(SAVE+i,1)
        if x:v=[v&0xef,v&0xfd,(v&0xf7)|4,v,v&0xbf,v|0x40][i]
        s.out(port,index);s.out(port+1,v)
    s.popad();ret(s)
@model('InitModeNormal','Restore six saved VGA registers in the index/data order listed for InitFirstModeVideo (1:0465f..046b7). Preserve GPRs.','Valid saved VGA configuration.',globals_read=['saved_vga_registers_0196a[6]'])
def normal(s):mode(s,False)
@model('InitModeX','Program six VGA registers from saved configuration in same order as InitFirstModeVideo. Transform values respectively by &0xEF, &0xFD, (&0xF7)|4, identity, &0xBF, |0x40 (1:046b8..0471c). This unchains the four VGA planes. Preserve GPRs.','Valid saved VGA configuration.',globals_read=['saved_vga_registers_0196a[6]'])
def modex(s):mode(s,True)

@model('asm_02b30','Advance current plane mask AH by doubling; if it becomes16, reset to1 and increment EDI aperture address. OUT DX,AX (1:02b3a). AL is sequencer index2. Preserve all other GPRs.','AH in1,2,4,8;DX=0x3C4;AL=2.')
def advance(s):
    mask=((s.r['eax']>>8)&255)*2
    if mask==16:mask=1;s.r['edi']+=1
    s.r['eax']=ah(s.r['eax'],mask);s.out(s.r['edx']&65535,s.r['eax']&65535,2);ret(s)

@model('sub_1268A','Upload four80x200 planes to aperture0xAB000; source row pitch EBX, source plane pitch EDX. OUT0x3C4,word((mask<<8)|2) for masks1,2,4,8 at1:026a0/026ca/026f4/0271d. Clear DF. This uses 16-bit PUSHA/POPA: only low16 of GPRs restored; high halves of ECX,ESI,EDI reflect final transfer.','Source row pitch>=80; four mapped200-row planes; source plane pitch>=rowpitch*200.')
def upload(s):
    r=s.r;old=r.copy()
    for n in ('eax','ecx','edx','ebx','esp','ebp','esi','edi'):s.push(old[n],2)
    r['ebx']=u32(r['ebx']-80);origin=r['esi']
    for plane in range(4):
        if plane<3:s.push(origin+plane*old['edx'])
        r['esi']=origin+plane*old['edx'];r['edi']=0xab000;r['eax']=ah(r['eax'],1<<plane)
        s.push(r['edx']);s.push(r['eax']);s.out(0x3c4,((1<<plane)<<8)|2,2);r['eax']=s.pop();r['edx']=s.pop()
        for y in range(200):
            s.push(200-y);s.movs(1,80);r['esi']+=r['ebx'];s.pop()
        if plane<3:r['esi']=s.pop()
    r['ecx']=0
    for n in ('edi','esi','ebp',None,'ebx','edx','ecx','eax'):
        v=s.pop(2)
        if n:r[n]=setlo(r[n],v,16)
    ret(s)

@model('draw_blackbox','Fill a black rectangle in ModeX: x=ESI,y=EDX,height=EBX,width=ECX. Ignore nonpositive dimensions. Select sequencer index2 by byte OUT0x3C4=2; write left partial plane mask, then whole aperture bytes with mask15, then right partial mask. Aperture pitch80. Masks are left[15,14,12,8] and right[15,1,3,7]; overlapping edge byte combines masks. Preserve GPRs.','DF=0; x/y>=0; positive dimensions fit VGA aperture; nonpositive dimensions also tested.')
def black(s):
    r=s.r;s.pushad();h=s32(r['ebx']);w=s32(r['ecx'])
    if h>0 and w>0:
        x=r['esi'];y=r['edx'];base=r['edi']+80*y;end=x+w;a=x//4;phase=x&3
        s.out(0x3c4,2)
        if phase:
            s.push(base);s.push(h);mask=[15,14,12,8][phase]
            if end//4==a:mask&=[15,1,3,7][end&3]
            s.pop();s.out(0x3c5,mask)
            for yy in range(h):s.mem.put(base+80*yy+a,0,1)
            s.pop();a+=1
        middle=end//4-a
        if middle>=0:
            if middle:
                s.push(base);s.push(end);s.push(h);s.out(0x3c5,15);s.push(0x3c5)
                for yy in range(h):
                    if middle&3:s.push(middle//4)
                    s.mem.write(base+80*yy+a,bytes(middle))
                    if middle&3:s.pop()
                s.pop();s.pop();s.pop();s.pop()
            if end&3:
                s.out(0x3c5,[15,1,3,7][end&3])
                for yy in range(h):s.mem.put(base+80*yy+end//4,0,1)
    s.popad();ret(s)

@model('draw_borderbox','Draw a white(255) ModeX outline from(x=EAX,y=EBX) to(x+EDX,y+ECX), both endpoints included: width/height arguments are coordinate deltas. Initial word OUT0x3C4 selects plane at x; advance planes with helper1:02b30 after each column. Preserve EDI/EBP/ESI; end EBX=80*height,ECX=0,EDX low16=0x3C4,EAX=last plane mask<<8|2.','width delta>=2,height delta>=0; mapped VGA footprint including final row/column.')
def border(s):
    r=s.r;x,y,w,h=r['eax'],r['ebx'],r['edx'],r['ecx'];s.push(r['ebp']);s.push(r['edi']);s.push(h);s.push(w)
    r['eax']=((1<<(x&3))<<8)|2;r['edx']=setlo(r['edx'],0x3c4,16);s.out(0x3c4,r['eax'],2)
    r['edi']+=(x//4)+80*y;r['ebp']=s.pop();r['ecx']=s.pop();s.push(r['edi'])
    for yy in range(h+1):s.mem.put(r['edi']+80*yy,255,1)
    r['edi']=s.pop();s.push(BASE[1]+0x322c);advance(s)
    s.push(h);r['ebx']=h*80
    for xx in range(w-1):
        s.mem.put(r['edi'],255,1);s.mem.put(r['edi']+r['ebx'],255,1)
        s.push(BASE[1]+0x3246);advance(s)
    r['ecx']=s.pop()+1
    for yy in range(h+1):s.mem.put(r['edi'],255,1);r['edi']+=80
    r['ecx']=0;r['edi']=s.pop();r['ebp']=s.pop();ret(s)

for name,bars in [('asm_04504',False),('asm_0456e',True)]:
    def fn(s,bars=bars):
        s.pushad();base=s.r['edi']
        if bars:s.push(base)
        s.out(0x3c4,0xf02,2)
        for y in range(204):s.mem.put(base-2+y*80,0)
        if bars:
            s.pop();s.mem.write(base,bytes(1280));s.mem.write(base+0x3c00,bytes(640))
        s.popad();ret(s)
    model(name,'Select all planes with OUT0x3C4,word0x0F02 at entry, then clear4 aperture bytes at EDI-2+80*y for y0..203 (side fringes of page). '+('Also clear1280 bytes at EDI and640 at EDI+0x3C00; explicitly clear DF. ' if bars else '')+'Preserve all GPRs.','Mapped VGA page and its2-byte left fringe; abstract planar effects compared.')(fn)

@model('display_anypicture','Copy planar rectangle: EAX height,EBX width in dwords,DL source row pitch,DH source-plane mask(1,2,4,8),ECX plane pitch,ESI source,EDI aperture destination. Four output planes start at1<<(EDI&3). Each plane copies width*4 bytes per row at source pitchDL,destination pitch80; source traverses plane blocks, wrapping after mask8 by subtracting4*plane_pitch and advancing1 source byte. Emit five word OUT0x3C4 map-mask writes (initial plus one after each plane). Mutable source-pitch/source-plane state at code1:02799..027a1. End EDI=original+1,ESI=original-plane_pitch+1,EDX=0x3C4,ECX=0x1000|(original EDI&3); EBP/EBX preserved.','DF=0; height>=1; width dwords>=0; 4*width<=DL;DH in1,2,4,8; four source planes mapped with phase-dependent negative/positive margins.',globals_written=['code1:02799 source pitch','code1:0279d plane pitch','code1:027a1 source phase'])
def picture(s):
    r=s.r;s.push(r['ebp']);height=r['eax'];plane_pitch=r['ecx'];pitch=r['edx']&255;phase=(r['edx']>>8)&255
    s.mem.put(BASE[1]+0x279d,plane_pitch);r['ebp']=height;s.mem.put(BASE[1]+0x2799,pitch);s.mem.put(BASE[1]+0x27a1,phase,1)
    r['eax']=ah(r['eax'],1<<(r['edi']&3));r['ecx']=(r['edi']&3)|256;r['edx']=0x3c4;r['esi']-=plane_pitch;r['eax']=setlo(r['eax'],2);s.out(0x3c4,r['eax']&65535,2)
    for p in range(4):
        s.push(r['ecx']);s.push(r['edi']);r['esi']+=plane_pitch;s.push(r['esi']);s.push(height)
        for y in range(height):
            s.push(r['edi']);s.push(r['esi']);s.movs(4,r['ebx']);r['esi']=s.pop();r['edi']=s.pop();r['edi']+=80;r['esi']+=pitch
        r['ebp']=s.pop();r['esi']=s.pop();r['edi']=s.pop();s.push(BASE[1]+0x2804);advance(s)
        phase=s.mem.get(BASE[1]+0x27a1,1)*2
        if phase==16:r['esi']-=4*plane_pitch;r['esi']+=1;phase=1
        s.mem.put(BASE[1]+0x27a1,phase,1);r['ecx']=s.pop();r['ecx']=ah(r['ecx'],((r['ecx']>>8)&255)*2)
    r['ebp']=s.pop();ret(s)

@model('bios_wait_timer','Save EAX/ES, set ES=0,AX low16=0x8300 and issue BIOS INT0x15 at1:02743, with EBX flag pointer and CX:DX delay. Restore EAX/ES; other register outputs depend on BIOS/extender response and are modeled as abstract inputs. The protected-mode ES=0,EBX pointer translation remains OPEN; no assertion about PMODE/W BIOS completion.','C caller uses ECX=7,EDX=0xA120 (500000us); EBX mapped flag pointer. BIOS stub only; not proof of actual BIOS/extender semantics.')
def bios_wait(s):
    r=s.r;s.push(r['eax']);s.push(16)
    s.io.append(['int',0x15,setlo(r['eax'],0x8300,16),r['ebx'],r['ecx'],r['edx'],0])
    for n,v in s.inputs.get(('int',0x15),{}).items():r[n]=v
    s.pop();r['eax']=s.pop();ret(s)

def prepare(s,name,rng,seed):
    if name=='InitFirstModeVideo':
        for p in (0x3cf,0x3c5,0x3d5):s.inputs[p]=[rng.randrange(256),rng.randrange(256)]
    elif name in ('InitModeNormal','InitModeX'):
        for i in range(6):s.mem.put(SAVE+i,rng.randrange(256),1)
    elif name=='asm_02b30':s.r.update(eax=(rng.getrandbits(16)<<16)|(rng.choice([1,2,4,8])<<8)|2,edx=(rng.getrandbits(16)<<16)|0x3c4)
    elif name=='sub_1268A':
        pitch=rng.choice([80,84,96,128]);s.r.update(ebx=pitch,edx=pitch*200,esi=SRC)
    elif name=='draw_blackbox':s.r.update(edi=0xa0500,esi=rng.randrange(16),edx=rng.randrange(40),ebx=u32(rng.choice([-1,0,1,2,8,16])),ecx=u32(rng.choice([-1,0,1,2,3,4,5,8,9,16,17,32,40])))
    elif name=='draw_borderbox':s.r.update(edi=0xa0500,eax=rng.randrange(16),ebx=rng.randrange(40),edx=rng.randrange(2,40),ecx=rng.randrange(20))
    elif name in ('asm_04504','asm_0456e'):s.r['edi']=0xa0500
    elif name=='display_anypicture':
        pitch=rng.choice([16,32,64,80]);height=rng.randrange(1,24);plane_pitch=pitch*32
        s.r.update(edi=0xa0500+rng.randrange(4),esi=SRC+0x4000,eax=height,ebx=rng.randrange(pitch//4+1),ecx=plane_pitch,edx=(rng.choice([1,2,4,8])<<8)|pitch)
    elif name=='bios_wait_timer':
        s.r.update(ebx=AUX,ecx=7,edx=0xa120);s.inputs[('int',0x15)]={n:rng.getrandbits(32) for n in ('ebx','ecx','edx','esi','edi','ebp')};s.inputs['protected_flat']=True
    if name=='display_emptypicture':s.r['edi']=0xa0500
