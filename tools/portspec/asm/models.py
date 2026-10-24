"""Portable operation models plus explicitly separated DOS ABI bookkeeping.

Python integers in the pixel computations are mathematical integers. u32/lo are
used only where the original observable wrap or register result requires them.
All descriptive names introduced here are INVENTED, unless source names are cited.
"""
import json
from engine import *
SYMS=json.loads((ROOT/'config/symbols.json').read_text())['gog_v121_us']
def binding(name):
    for group in SYMS.values():
        if isinstance(group,dict) and name in group:
            obj,off=group[name]['addr'].split(':'); return BASE[int(obj)]+int(off,16)
    raise KeyError(name)
CLIP=[BASE[1]+0x6184+4*i for i in range(4)]
TIMER=[binding('lowlevel_timer_'+x) for x in ('02148','02150','02154','02158')]
MODELS={}; DETAILS={}
def model(name,description,domain,layouts=None,globals_read=None,globals_written=None):
    def wrap(fn):
        MODELS[name]=fn; DETAILS[name]=dict(description=description,domain=domain,layouts=layouts or [],
            globals_read=globals_read or [],globals_written=globals_written or [],model=Path(fn.__code__.co_filename).name+':'+str(fn.__code__.co_firstlineno)+':'+fn.__name__)
        return fn
    return wrap
def saved(fn,s,*names):
    for n in names:s.push(s.r[n])
    fn()
    for n in reversed(names):s.r[n]=s.pop()
def copy_bytes(s,dst,src,count):
    # Forward sequential copying is intentional: overlap is not memmove.
    for i in range(count): s.mem.put(dst+i,s.mem.get(src+i,1),1)
def ret(s,extra=0):s.ret(extra)

@model('synchro','Read VGA status port 0x3DA until bit 3 becomes one (already in retrace suffices). Preserve all GPRs. IN site 1:06e90.', 'Finite status stream ending in a byte with bit 3 set.')
def synchro(s):
    s.push(s.r['edx']);s.push(s.r['eax'])
    while not s.inp(0x3da)&8:pass
    s.r['eax']=s.pop();s.r['edx']=s.pop();ret(s)

@model('endsynchro','Read VGA status port 0x3DA until bit 3 becomes zero. Preserve all GPRs. IN site 1:06ea0.', 'Finite status stream ending in a byte with bit 3 clear.')
def endsynchro(s):
    s.push(s.r['edx']);s.push(s.r['eax'])
    while s.inp(0x3da)&8:pass
    s.r['eax']=s.pop();s.r['edx']=s.pop();ret(s)

@model('sprite_clipping','Clamp each signed input independently: xmin/xmax to [0,320], ymin/ymax to [0,200]. Store four signed 32-bit bounds. Does not sort or repair inverted bounds. Returns clamped values in the input registers.','All signed 32-bit inputs.',globals_written=['XMIN','XMAX','YMIN','YMAX'])
def sprite_clipping(s):
    for r,a,upper in zip(('eax','ebx','ecx','edx'),CLIP,(320,320,200,200)):
        s.r[r]=max(0,min(upper,s32(s.r[r])));s.mem.put(a,s.r[r])
    ret(s)

@model('default_sprite_clipping','Store half-open sprite clip box [8,312) x [0,200). Preserve GPRs.','No arguments.',globals_written=['XMIN','XMAX','YMIN','YMAX'])
def default_sprite_clipping(s):
    for a,v in zip(CLIP,(8,312,0,200)):s.mem.put(a,v)
    ret(s)

@model('get_sprite_clipping','Copy current xmin,xmax,ymin,ymax to the four s32 pointers EAX,EBX,ECX,EDX in that order. Preserve GPRs. Aliasing obeys the sequential source reads.','Four mapped writable four-byte pointers.',globals_read=['XMIN','XMAX','YMIN','YMAX'])
def get_sprite_clipping(s):
    s.push(s.r['ebp']);s.push(s.r['edi'])
    for r,a in zip(('eax','ebx','ecx','edx'),CLIP):s.mem.put(s.r[r],s.mem.get(a))
    s.r['edi']=s.pop();s.r['ebp']=s.pop();ret(s)

@model('set_speaker_on','Reset last,epoch,repetition counters to zero; read port 0x61, set bits 0 and 1 and write it; set period=65535; OUT 0x43=0xB4, then OUT 0x42=0xFF twice. GPRs preserved. Sites 1:06eaa..06ee9.','Arbitrary input byte from port 0x61.',globals_written=['lowlevel_timer_02148','lowlevel_timer_02150','lowlevel_timer_02154','lowlevel_timer_02158'])
def set_speaker_on(s):
    for a in TIMER[1:]:s.mem.put(a,0)
    s.push(s.r['eax']);s.out(0x61,s.inp(0x61)|3)
    s.mem.put(TIMER[0],65535);s.out(0x43,0xb4);s.out(0x42,255);s.out(0x42,255)
    s.r['eax']=s.pop();ret(s)

@model('set_speaker_off','Read port 0x61, clear bits 0 and 1 and write it; OUT 0x43=0xB4, OUT 0x42=0 twice (PIT divisor 65536). Software timer globals are unchanged. Preserve GPRs. Sites 1:06eea..06efc.','Arbitrary input byte from port 0x61.')
def set_speaker_off(s):
    s.push(s.r['eax']);s.out(0x61,s.inp(0x61)&252);s.out(0x43,0xb4);s.out(0x42,0);s.out(0x42,0);s.r['eax']=s.pop();ret(s)

@model('speaker_enable','Call set_speaker_on, including resetting its software clock state. Preserve GPRs.','Same as set_speaker_on.',globals_written=['lowlevel_timer_02148','lowlevel_timer_02150','lowlevel_timer_02154','lowlevel_timer_02158'])
def speaker_enable(s):
    s.push(BASE[1]+0x6f7d);set_speaker_on(s);ret(s)

@model('clock_ticks','Latch PIT2 by OUT 0x43=0x80 and read low then high counter bytes at 0x42 (1:06f04..06f0c). Let delta=s32(period-counter), repeat=0. If delta<=s32(last), add period to epoch; when equal increment previous repetition count and if it exceeds 20 first set period=0 and repetition=0. Store last=delta,epoch,repeat and return u32(delta+epoch) in EAX; preserve other GPRs.','16-bit PIT counter; period in {0,65535}; game-maintained signed last, epoch, repeat.',globals_read=['lowlevel_timer_02148','lowlevel_timer_02150','lowlevel_timer_02154','lowlevel_timer_02158'],globals_written=['lowlevel_timer_02148','lowlevel_timer_02150','lowlevel_timer_02154','lowlevel_timer_02158'])
def clock_ticks(s):
    s.push(s.r['ebx']);s.push(s.r['edx'])
    s.out(0x43,0x80);counter=s.inp(0x42)|(s.inp(0x42)<<8)
    period,last,epoch,repeats=[s.mem.get(a) for a in TIMER]
    delta=u32(period-counter);repeat=0
    if s32(delta)<=s32(last):
        if delta==last:
            repeat=u32(repeats+1)
            if s32(repeat)>20:s.mem.put(TIMER[0],0);period=0;repeat=0
        epoch=u32(epoch+period);s.mem.put(TIMER[2],epoch)
    s.mem.put(TIMER[1],delta);s.mem.put(TIMER[3],repeat);s.r['eax']=u32(delta+epoch)
    s.r['edx']=s.pop();s.r['ebx']=s.pop();ret(s)

@model('wait_until','If period is zero set ECX=1 without polling. Otherwise poll clock_ticks until signed tick value > signed EBX target. ECX is polls-1; EAX is last tick; other GPRs preserved.','Reachable finite timer sequence, no signed epoch overflow during wait.',globals_read=['lowlevel_timer_02148'],globals_written=['timer state via clock_ticks'])
def wait_until(s):
    s.r['ecx']=1
    if s.mem.get(TIMER[0]):
        s.r['ecx']=0xffffffff
        while True:
            s.push(BASE[1]+0x6f72);clock_ticks(s);s.r['ecx']=u32(s.r['ecx']+1)
            if s32(s.r['eax'])>s32(s.r['ebx']):break
    ret(s)

@model('sub_2748','BIOS INT 0x1A AH=0 returns tick halves CX:DX; write ((CX&65535)<<16)|(DX&65535) through ESI. Preserve all GPRs. INT site 1:0274b. BIOS behavior is an abstract input.','Mapped u32 output pointer; BIOS response supplies CX and DX.')
def sub_2748(s):
    s.pushad();r=s.r
    s.io.append(['int',0x1a,ah(r['eax'],0),r['ebx'],r['ecx'],r['edx'],0])
    result=s.inputs[('int',0x1a)]
    s.mem.put(r['esi'],((result['ecx']&65535)<<16)|(result['edx']&65535))
    s.popad();ret(s)

@model('effect_reset_geometry','Initialize all 200 raster rows: u32 +0=65535, +4=0 at stride32; leave +8..31 unchanged. Restore 80 unrolled span-dispatch immediates at code offsets 0x815+96*i to signed values -320+4*i. Preserve GPRs.','Raster rows allocated for 200 records; code writable.',layouts=[{'name':'raster_row (INVENTED)','stride':32,'fields':[{ 'offset':0,'width':4,'signed':False,'meaning':'empty left extent biased by 0x8000'},{'offset':4,'width':4,'signed':False,'meaning':'empty right extent biased by 0x8000'}]}],globals_written=['raster_rows','80 writable code immediates'])
def effect_reset_geometry(s):
    s.pushad()
    for i in range(80):s.mem.put(BASE[1]+0x815+96*i,-320+4*i)
    for y in range(200):
        s.mem.put(binding('raster_rows')+32*y,65535);s.mem.put(binding('raster_rows')+32*y+4,0)
    s.popad();ret(s)

@model('N_CLRSCR','Zero exactly 64000 bytes at EDI (16000 dword stores). Advance EDI by 64000; preserve other GPRs.','DF=0; mapped 64000-byte destination.')
def N_CLRSCR(s):
    s.push(s.r['ecx']);s.push(s.r['eax']);s.mem.write(s.r['edi'],bytes(64000));s.r['edi']+=64000
    s.r['eax']=s.pop();s.r['ecx']=s.pop();ret(s)

@model('DisplayBufferModeNormal','Forward copy 16000 dwords (64000 bytes), preserving ECX and advancing ESI/EDI by 64000.','DF=0; two disjoint mapped 64000-byte buffers.')
def DisplayBufferModeNormal(s):
    s.push(s.r['ecx']);s.movs(4,16000);s.r['ecx']=s.pop();ret(s)

@model('copy_dwords','Sequentially copy ECX four-byte elements; DF selects +/-4 pointer movement. Consume ECX to zero, advance ESI/EDI; all other GPRs preserved.','Count 0..256 in proof; pointers cover entire traversed intervals, DF=0 or1.')
def copy_dwords(s):
    s.movs(4,s.r['ecx'],s.inputs['direction']);s.r['ecx']=0;ret(s)

@model('ClearDrawAndDisplayBufferNormal','Zero 64000 bytes at original EDI, then at original ESI. End EDI=ESI+64000, preserving all other GPRs.','DF=0; two disjoint mapped 64000-byte buffers.')
def ClearDrawAndDisplayBufferNormal(s):
    s.push(s.r['ecx']);s.push(s.r['eax'])
    s.mem.write(s.r['edi'],bytes(64000));s.mem.write(s.r['esi'],bytes(64000));s.r['edi']=s.r['esi']+64000
    s.r['eax']=s.pop();s.r['ecx']=s.pop();ret(s)

@model('asm_0283c','Select write planes: OUT 0x3C4,word((AL<<8)|2) at 1:0284b. End EAX=((input AL)<<8)|2; preserve EDX and other GPRs.','Any byte plane mask.')
def asm_0283c(s):
    s.push(s.r['edx'],2);s.r['eax']=((s.r['eax']&255)<<8)|2;s.out(0x3c4,s.r['eax'],2)
    s.r['edx']=setlo(s.r['edx'],s.pop(2),16);ret(s)

@model('select_display_buffer','Set CRTC high start address register: OUT 0x3D4,word((input AH<<8)|0x0C) at 1:045f8; low start register is untouched. EAX low byte becomes 0x0C, other GPRs preserved.','Page address in EAX (normally 0xA0500/0xA6500).')
def select_display_buffer(s):
    s.push(s.r['edx']);s.r['eax']=setlo(s.r['eax'],12);s.out(0x3d4,s.r['eax']&65535,2);s.r['edx']=s.pop();ret(s)

@model('display_emptypicture','OUT 0x3C4,word(0x0F02) at 1:0278d selects all planes, then zero 16000 aperture bytes (64000 planar pixels). End EAX=ECX=0,EDX=0x3C4,EDI+=16000.','DF=0; mapped planar aperture; flat bytes are aperture addresses.')
def display_emptypicture(s):
    s.out(0x3c4,0xf02,2);s.mem.write(s.r['edi'],bytes(16000));s.r.update(eax=0,ecx=0,edx=0x3c4,edi=s.r['edi']+16000);ret(s)

@model('block_clear','Clear 16 rows of four bytes at destination pitch80. Preserve GPRs. One plane of a 16x16 planar tile.','Mapped destination span of1204 bytes; no VGA I/O.')
def block_clear(s):
    s.pushad()
    for y in range(16):s.mem.put(s.r['edi']+80*y,0)
    s.popad();ret(s)

@model('block_copy','Copy 64 bytes of one tile plane: four bytes per row to16 destination rows at pitch80. Preserve GPRs.','Disjoint mapped source64 bytes and destination1204 bytes.',layouts=[{'name':'one 16x16 tile plane (INVENTED)','stride':4,'fields':'16 rows of four u8 palette indices'}])
def block_copy(s):
    s.pushad()
    for y in range(16):s.mem.write(s.r['edi']+80*y,s.mem.read(s.r['esi']+4*y,4))
    s.popad();ret(s)

@model('DrawBlackBoxNormal','Fill width EDX by height ECX with zero at dst+EAX+320*EBX. Preserve GPRs. No clipping.','DF=0; positive width/height; rectangle wholly mapped.')
def DrawBlackBoxNormal(s):
    s.pushad();r=s.r;d=r['edi']+r['eax']+320*r['ebx'];w=r['edx'];h=r['ecx']
    for y in range(h):
        s.push(h-y);s.mem.write(d+320*y,bytes(w));s.pop()
    s.popad();ret(s)

@model('DrawWldPointPlan2Normal','At dst+x+384*y write u16(+1)=0x0C00,u32(+384)=0x0C0E0A00,u32(+768)=0x000A0900,u16(+1153)=0. End EBX=384*y and EDI=start; preserve EAX and other GPRs.','Mapped 384-byte-pitch surface; room for pattern4 rows.')
def DrawWldPointPlan2Normal(s):
    s.r['ebx']=u32(s.r['ebx']*384);s.r['edi']=u32(s.r['edi']+s.r['ebx']+s.r['eax']);d=s.r['edi']
    for off,n,v in ((1,2,0xc00),(384,4,0xc0e0a00),(768,4,0xa0900),(1153,2,0)):s.mem.put(d+off,v,n)
    ret(s)

@model('DisplayAnyPictureNormal','Copy width by height raw bytes from source+(source_y*stride)+source_x to destination+dest_y*320+dest_x. Source pitch=stride, destination pitch320. Callee pops12 argument bytes. End EAX=ECX=0,EBX=320-width,EDX=stride-width; pointers advance by height*pitch.','DF=0; positive dimensions, mapped disjoint buffers; stack+4/+8/+12 contain s32 stride,width,height.',layouts=[{'name':'callee-popped arguments (INVENTED)','fields':[{'offset':4,'width':4,'signed':True,'meaning':'source stride'},{'offset':8,'width':4,'signed':True,'meaning':'width'},{'offset':12,'width':4,'signed':True,'meaning':'height'}]}])
def DisplayAnyPictureNormal(s):
    r=s.r;stride,w,h=[s.mem.get(r['esp']+i) for i in (4,8,12)]
    r['esi']+=r['ebx']*stride+r['eax'];r['edi']+=r['edx']*320+r['ecx']
    for y in range(h):
        copy_bytes(s,r['edi'],r['esi'],w);r['edi']+=320;r['esi']+=stride
    r.update(eax=0,ecx=0,ebx=320-w,edx=stride-w);ret(s,12)

@model('Swap_To_Screen','Copy floor(ECX/4)*4 bytes on each of EAX rows, using source/destination pitches stored at code1:04c86/04c8a (initially320). Preserve all GPRs.','DF=0; positive height; width0..320; disjoint mapped source/destination.',globals_read=['code1:04c86 source pitch','code1:04c8a destination pitch'])
def Swap_To_Screen(s):
    h,w=s.r['eax'],s.r['ecx']//4*4;src,dst=s.r['esi'],s.r['edi'];s.pushad();s.push(w//4)
    a,b=s.mem.get(BASE[1]+0x4c86),s.mem.get(BASE[1]+0x4c8a)
    for y in range(h):copy_bytes(s,dst+y*b,src+y*a,w)
    s.pop();s.popad();ret(s)

@model('clear_borders_Normal','For floor(ECX/2)*2 rows, zero four bytes at destination+row*320 and destination+row*320+EDX. Preserve GPRs; odd last row is not processed.','ECX>=2; mapped border columns; no clipping.')
def clear_borders_Normal(s):
    s.pushad()
    for y in range(s.r['ecx']//2*2):
        for x in (0,s.r['edx']):s.mem.put(s.r['edi']+320*y+x,0)
    s.popad();ret(s)

@model('flocon_clipping','Store top=EAX,bottom=EBX,left=ECX,right=EDX as signed32 weather clipping bounds. Preserve all GPRs.','No sorting/clamping; valid drawing windows generated by callers.',globals_written=['code1:05618..05627'])
def flocon_clipping(s):
    for i,r in enumerate(('eax','ebx','ecx','edx')):s.mem.put(BASE[1]+0x5618+4*i,s.r[r])
    ret(s)

@model('fplot_Normal','Write low byte of ESI at dst+EAX+row_offsets_320[EBX]. Preserve all GPRs. No clipping.','Mapped pixel; x0..319,y0..199.',globals_read=['row_offsets_320_01b00'])
def fplot_Normal(s):
    s.pushad();s.mem.put(s.r['edi']+s.r['eax']+s.mem.get(binding('row_offsets_320_01b00')+4*s.r['ebx']),s.r['esi'],1);s.popad();ret(s)

@model('textmode','BIOS INT10h AX=3 requests text mode; all GPRs preserved by PUSHAD/POPAD. INT site1:047ed. BIOS implementation excluded.','No arguments; service modeled as abstract interrupt.')
def textmode(s):
    s.pushad();r=s.r;s.io.append(['int',0x10,setlo(r['eax'],3,16),r['ebx'],r['ecx'],r['edx'],0]);s.popad();ret(s)

@model('DrawBorderBoxNormal','Draw beveled box with two colors AL/AH at dst+ESI+320*EBX. AL fills top row width and left column height. AH fills bottom row columns1..width-1, an additional full row at y+height, and right columns width-1 (rows1..height-1) and width (rows0..height-1). This original shadow extends one row and column beyond requested dimensions. End EBX=height,ECX=0,EDX=width-1,EDI=start+height*320+width-1,EAX low16 repeats original AH.','DF=0; height>=2,width>=1; mapped rectangle including one-pixel shadow.')
def DrawBorderBoxNormal(s):
    r=s.r;w=r['edx'];h=r['ecx'];d=r['edi']+320*r['ebx']+r['esi'];c=r['eax']&255;b=(r['eax']>>8)&255
    s.push(d)
    for y in range(h):s.mem.put(d+320*y,c,1)
    s.mem.write(d+320*(h-1)+1,bytes([b])*(w-1));s.mem.write(d+320*h,bytes([b])*w)
    s.pop();s.mem.write(d,bytes([c])*w)
    for y in range(1,h):s.mem.put(d+320*y+w-1,b,1)
    for y in range(h):s.mem.put(d+320*y+w,b,1)
    r.update(ebx=h,ecx=0,edx=w-1,edi=d+320*h+w-1,eax=setlo(r['eax'],b|(b<<8),16));ret(s)

@model('DrawFondBoxNormal','For type EAX!=2 traverse forward: read pixel at destination+321 and subtract5 modulo256. If source and result differ in high3 bits, replace output by (source XOR result)&0xE0. For type2 traverse backward from rectangle bottom-right, read destination-321 and add5 modulo256; on high-bit crossing replace by ((source XOR result)&0xE0)|31. Thus this is not ordinary saturation within the original32-color bank. Destination pitch320.','Positive width/height; x=ESI,y=EBX; one-pixel diagonal source margin mapped. Type2 reads upper-left, others lower-right.')
def DrawFondBoxNormal(s):
    r=s.r;w=r['edx'];h=r['ecx'];back=r['eax']==2;d=r['edi']+320*r['ebx']+r['esi']
    if back:d+=(h-1)*320+w-1
    src=d+(-321 if back else 321);step=-1 if back else 1
    for y in range(h):
        s.push(h-y)
        for x in range(w):
            v=s.mem.get(src,1);out=(v+(5 if back else -5))&255;cross=(v^out)&224
            if cross:out=cross|31 if back else cross
            r['eax']=setlo(r['eax'],out|(cross<<8),16);s.mem.put(d,out,1);src+=step;d+=step
        d+=step*(320-w);src+=step*(320-w);s.pop()
    r.update(edi=u32(d),esi=u32(src),ebx=320-w,ecx=0);ret(s)
