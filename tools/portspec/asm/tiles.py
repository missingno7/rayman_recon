from models import *
LIMIT=[binding(n) for n in ('Bloc_lim_W1','Bloc_lim_W2','Bloc_lim_H1','Bloc_lim_H2')]
LAYOUT={'name':'normal tile (INVENTED descriptive names; map caller uses MAP_BLOCKS)','size':516,'fields':[{'offset':0,'width':256,'signed':False,'meaning':'16x16 u8 pixels, pitch16'},{'offset':256,'width':256,'signed':False,'meaning':'per-pixel AND masks, pitch16'},{'offset':512,'width':4,'signed':False,'meaning':'2 bits per row, row0 in bits31..30; bit30 skips, otherwise bit31 copies opaque, otherwise AND/OR'}]}
def row(s,src,dst,n,kind):
    if kind&1:return
    if kind&2:s.mem.write(dst,s.mem.read(src,n));return
    for i in range(n):s.mem.put(dst+i,(s.mem.get(dst+i,1)&s.mem.get(src+256+i,1))|s.mem.get(src+i,1),1)

@model('Display_Bloc','Render masked16x16 tile using two-bit row flags. For each row, low flag bit skips; otherwise high flag bit copies16 opaque bytes; otherwise destination=(destination&mask)|pixel. Source pixels at+0,AND masks+256,flags+512. Destination pitch read from code1:04c86. Preserve GPRs.','Mapped516-byte source and16-row destination; disjoint.',layouts=[LAYOUT],globals_read=['code1:04c86 destination pitch'])
def masked(s):
    r=s.r;s.pushad();s.push(4);src,dst=r['esi'],r['edi'];flags=s.mem.get(src+512);pitch=s.mem.get(BASE[1]+0x4c86)
    for y in range(16):
        row(s,src+16*y,dst+pitch*y,16,(flags>>(30-2*y))&3)
        if y%4==3:s.mem.put(r['esp'],3-y//4)
    s.pop();s.popad();ret(s)

@model('Display_Bloc_Plein','Copy opaque16x16 tile from source pitch16 to destination pitch320. Preserve GPRs. Unlike masked variant this entry uses literal320 rather than mutable stride.','Mapped256-byte source and16-row destination; disjoint.',layouts=[LAYOUT])
def opaque(s):
    s.pushad()
    for y in range(16):s.mem.write(s.r['edi']+320*y,s.mem.read(s.r['esi']+16*y,16))
    s.popad();ret(s)

def clipped(s,masked):
    r=s.r;x=s32(r['eax']);y=s32(r['ebx']);src,dst=r['esi'],r['edi'];pitch=s.mem.get(BASE[1]+0x4c86)
    xmin,xmax,ymin,ymax=[s.mem.get(a,signed=True) for a in LIMIT]
    s.pushad();r['esp']-=5;flags=s.mem.get(src+512) if masked else 0;s.mem.put(r['esp'],flags)
    s.push(BASE[1]+(0x53dd if masked else 0x5573));s.mem.put(BASE[1]+0x52d6,16)
    ok=True;n=16;rows=16;skipx=0;skipy=0;adjusted_x=x
    if x<xmin:
        if xmin-x>=16:ok=False
        else:
            skipx=(xmin-x)&0xfffc;n=((16-skipx)//4)*4;adjusted_x=xmin
    elif x+16>xmax:
        if x>=xmax:ok=False
        else:n=((xmax-x+3)//4)*4
    if ok:
        if y<ymin:
            if y+16<=ymin:ok=False
            else:
                skipy=ymin-y;rows-=skipy
                s.push(u32(adjusted_x));flags=u32(flags<<(2*skipy));s.mem.put(r['esp']+8,flags);s.pop()
        elif y+16>ymax:
            if y>=ymax:ok=False
            else:rows=ymax-y
    ret(s) # helper returns
    if ok:
        s.mem.put(r['esp']+4,rows,1)
        for i in range(rows):
            row(s,src+skipy*16+skipx+16*i,dst+skipy*pitch+skipx+pitch*i,n,(flags>>(30-2*i))&3 if masked else 2)
            s.mem.put(r['esp']+4,rows-i-1,1)
    r['esp']+=5;s.popad();ret(s)

for name,is_masked in (('Display_Bloc_Clippe',True),('Display_Bloc_Plein_Clippe',False)):
    def fn(s,m=is_masked):clipped(s,m)
    model(name,'Render '+('masked' if is_masked else 'opaque')+'16x16 tile at provided destination pointer, clipped using EAX/EBX position and Bloc_lim_* bounds. Horizontal crop is dword aligned: left trim rounds down to4 bytes, right kept width rounds up to4. Top crop advances source by16*rows and destination by pitch*rows; row flags shift by2 per skipped row. Destination pitch from code1:04c86. Preserve all GPRs.','Ordered clip with dword-aligned game tile boundaries; mapped tile and destination including rounded fringe.',layouts=[LAYOUT],globals_read=['Bloc_lim_W1','Bloc_lim_W2','Bloc_lim_H1','Bloc_lim_H2','code1:04c86'],globals_written=['code1:052d6 original width'])(fn)

def prepare(s,name,rng,seed):
    if name not in ('Display_Bloc','Display_Bloc_Plein','Display_Bloc_Clippe','Display_Bloc_Plein_Clippe'):return
    src=s.r['esi'];flags=0
    for y in range(16):
        kind=rng.choice([0,1,2]);flags|=kind<<(30-2*y)
        for x in range(16):
            transparent=kind==1 or kind==0 and rng.randrange(2)
            s.mem.put(src+y*16+x,0 if transparent else rng.randrange(1,256),1);s.mem.put(src+256+y*16+x,255 if transparent else 0,1)
    s.mem.put(src+512,flags)
    for a,v in zip(LIMIT,(16,240,8,152)):s.mem.put(a,v)
    s.r.update(eax=u32(rng.choice([-32,0,4,8,12,16,20,224,228,232,236,240,244])),ebx=u32(rng.choice([-24,-8,0,4,8,16,136,140,144,148,152,156])))
