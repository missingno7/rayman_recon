"""Affine quadrilateral raster specification, expressed as scanline and modular
fixed-point arithmetic, rather than an x86 instruction interpreter.
"""
from models import *
def truncdiv(a,b):return (abs(a)//abs(b))*(-1 if (a<0)!=(b<0) else 1)
ROW=binding('raster_rows')
R={n:binding('raster_'+n) for n in ('destination','descriptor','texture','du','neg_du','dv','neg_dv','first_y','rows_left','scanline','row_offset')}
RROW={'name':'raster row (INVENTED field names)','size':32,'fields':[
 {'offset':0,'width':4,'signed':False,'meaning':'left x +32000;65535 is empty; edge/consume writes low16 only,upper16 initialized zero'},
 {'offset':4,'width':4,'signed':False,'meaning':'right x +32000;0 is empty; edge/consume writes low16 only,upper16 initialized zero'},
 {'offset':8,'width':4,'signed':False,'meaning':'left texture u; edge writes low8 only,upper24 initialized zero'},
 {'offset':12,'width':4,'signed':False,'meaning':'right texture u; edge writes low8 only,upper24 initialized zero'},
 {'offset':16,'width':4,'signed':False,'meaning':'left texture v; edge writes low8 only,upper24 initialized zero'},
 {'offset':20,'width':4,'signed':False,'meaning':'right texture v; edge writes low8 only,upper24 initialized zero'}]}
DESC={'name':'effect_item_t subrecord at+0x3c; src/effect2.c (source names INVENTED)','size':44,'fields':[
 {'offset':0,'width':4,'signed':False,'meaning':'screen: pointer to256-byte-pitch texture plane'},
 {'offset':4,'width':32,'signed':True,'meaning':'vertices[8]:four (x,y) signed32 projected pairs'},
 {'offset':36,'width':8,'signed':False,'meaning':'rectangle[8]:four (u,v) byte pairs; u0=u1,u2=u3,v0=v3,v1=v2'}]}

def span_setup(s,row,width):
    u0,u1,v0,v1=[s.mem.get(row+i) for i in (8,12,16,20)]
    qv=u32(truncdiv((s32(u32(v1-v0+1))<<32),s32(u32(width<<16))))
    qu=u32(truncdiv((s32(u32(u1-u0+1))<<32),s32(u32(width<<16))))
    return ((qv&65535)<<16)|(qu>>16), ((qu&65535)<<16)|((qv>>16)&255), ((v0<<8)+u0)&65535

@model('asm_02640','Read left/right u/v dwords at raster_rows+EBP offsets8,12,16,20 and positive ECX span width. qU=trunc((uRight-uLeft+1)*65536/width), qV likewise. Return EAX=(frac16(qV)<<16)|int16(qU), EDX=(frac16(qU)<<16)|(int8(qV)&255), EBX=(vLeft<<8)|uLeft, ESI=0, ECX=width. Arithmetic is signed64 division toward zero. EBP/EDI preserved.','Row offset multiple32 within200 rows; byte-valued UV dwords; positive span width<=640 and representable quotients.',layouts=[RROW],globals_read=['raster_rows'])
def setup(s):
    r=s.r;a,d,b=span_setup(s,ROW+r['ebp'],r['ecx']);r.update(eax=a,edx=d,ebx=b,esi=0);ret(s)

# per-edge patch locations: constant coordinate bytes, UV fraction words, X fraction words
PATCH=[(0x1f9,0x216,0x223,0x198,0x21a,0x1b0),
       (0x34c,0x369,0x37c,0x2f1,0x373,0x309),
       (0x4ab,0x4c8,0x4d5,0x44a,0x4cc,0x462),
       (0x5fd,0x61a,0x62d,0x5a2,0x624,0x5ba)]

@model('draw_effect_segment','Rasterize a textured projected quadrilateral. Edge order(0,3),(3,2),(2,1),(1,0); include both endpoint scanlines. Each edge starts at its lower-y endpoint and uses count=abs(dy)+1. X slope Q16=trunc((dx+sign_nonzero(dx))*65536/count), with +1 for dx>=0 and-1 otherwise. Varying texture slope Q16=trunc(deltaTex*65536/count); other coordinate is constant. Clip y to0..199; retain strict min/max biased16-bit x per row (ties retain previous UV). Consume and reset row bounds; clip x to0..319. Horizontal slopes use endpoint differences PLUS1, divided by original inclusive span width. Sample texture at u+256*v without transparency. The two coupled modular accumulators and the original fast-forward for left clipping are defined in raster.py; all carries, rounding, writes to raster scratch and self-modified immediates are part of this contract. Preserve all GPRs.','Convex projected four-corner effect geometry in x/y range roughly-500..800; rectangle UV corresponds to an allocated256-byte-pitch half-screen texture (u0..159,v0..199). effect_reset_geometry must have initialized row bounds and80 span-store displacements. Source callers:effect2.c and other effect units pass effect_item_t+0x3c. Nondegenerate positive scanline spans; no divide overflow.',layouts=[DESC,RROW],globals_read=['raster_rows','80 reset span-store immediates'],globals_written=['all raster_* globals','raster_rows bounds/UV','24 edge interpolation immediate sites'])
def draw(s):
    r=s.r;desc=r['esi'];dst=r['eax'];s.pushad()
    texture=s.mem.get(desc);verts=[(s.mem.get(desc+4+8*i,signed=True),s.mem.get(desc+8+8*i,signed=True)) for i in range(4)]
    uv=[(s.mem.get(desc+36+2*i,1),s.mem.get(desc+37+2*i,1)) for i in range(4)]
    du=(uv[3][0]-uv[0][0])&255;dv=(uv[1][1]-uv[0][1])&255
    for key,value in [('destination',dst),('descriptor',desc+4),('texture',texture),('du',du),('neg_du',-du),('dv',dv),('neg_dv',-dv)]:s.mem.put(R[key],value)
    miny=min(v[1] for v in verts);maxy=max(v[1] for v in verts)
    if miny>=200 or maxy<0:s.popad();ret(s);return
    first=max(0,miny);last=min(199,maxy);s.mem.put(R['first_y'],first);s.mem.put(R['rows_left'],last-first+1)
    for edge,(i,j,axis,delta) in enumerate([(0,3,0,-du),(3,2,1,-dv),(2,1,0,du),(1,0,1,dv)]):
        # Assembly starts from endpoint j, and reverses when endpoint i is above it.
        (xa,ya),(xb,yb)=verts[j],verts[i];ta,tb=uv[j][axis],uv[i][axis];constant=uv[j][1-axis]
        if ya<0 and yb<0 or ya>=200 and yb>=200:continue
        if yb<ya:xa,xb=xb,xa;ya,yb=yb,ya;ta,tb=tb,ta;delta=-delta
        count=yb-ya+1;dx=xb-xa
        qx=truncdiv((dx+(1 if dx>=0 else -1))*65536,count);qt=truncdiv(delta*65536,count)
        p=PATCH[edge]
        for a in p[:2]:s.mem.put(BASE[1]+a,constant,1)
        for a in p[2:4]:s.mem.put(BASE[1]+a,qt,2)
        for a in p[4:]:s.mem.put(BASE[1]+a,qx,2)
        skip=max(0,-ya);clipped_y=max(0,ya)
        if skip:
            packed=((qx>>16)&65535)<<16|((qt>>16)&65535)
            s.push(packed);s.push(((qt*skip)&65535)<<16);s.pop();s.pop()
        s.push(clipped_y+9900);s.pop()
        for y in range(clipped_y,min(199,yb)+1):
            n=y-ya;biased=(((xa+32000)*65536+qx*n)>>16)&65535;t=((ta*65536+qt*n)>>16)&255
            addr=ROW+32*y;uu,vv=(t,constant) if axis==0 else (constant,t)
            if s.mem.get(addr,2)>biased:
                s.mem.put(addr,biased,2);s.mem.put(addr+8,uu,1);s.mem.put(addr+16,vv,1)
            if s.mem.get(addr+4,2)<biased:
                s.mem.put(addr+4,biased,2);s.mem.put(addr+12,uu,1);s.mem.put(addr+20,vv,1)
    s.mem.put(R['scanline'],dst+first*320);s.mem.put(R['row_offset'],first*32)
    for y in range(first,last+1):
        addr=ROW+32*y;left=s.mem.get(addr);right=s.mem.get(addr+4);s.mem.put(addr,65535,2);s.mem.put(addr+4,0,2)
        if left<32320 and right>=32000:
            width=u32(right-left+1);a,d,b=span_setup(s,addr,width);fraction=0;x=left-32000
            if x<0:
                skip=-x;width-=skip
                product=a*skip;v=b+(product&0xffffffff);carry=int(v>0xffffffff);b=u32(v)
                b=ah(b,((b>>8)+((product>>32)&255)+carry)&255)
                product=d*skip;b=setlo(b,(b&255)+((product>>32)&255));b=ah(b,((b>>8)+(product&255))&255)
                fraction=product&0xffffffff;x=0
            width=min(width,320-x);lead=min((dst+320*y+x)&3,width)
            s.push(width-lead);s.pop();s.push(width-lead);s.pop()
            for col in range(width):
                s.mem.put(dst+320*y+x+col,s.mem.get(texture+(b&65535),1),1)
                total=fraction+d;fraction=u32(total);carry=int(total>0xffffffff)
                total=b+a+carry;b=u32(total);carry=int(total>0xffffffff)
                b=ah(b,((b>>8)+(d&255)+carry)&255)
        s.mem.put(R['scanline'],dst+(y+1)*320);s.mem.put(R['row_offset'],(y+1)*32);s.mem.put(R['rows_left'],last-y)
    s.popad();ret(s)

def prepare(s,name,rng,seed):
    if name=='asm_02640':
        i=rng.randrange(200);s.r.update(ebp=i*32,ecx=rng.randrange(1,641))
        for off in (8,12,16,20):s.mem.put(ROW+32*i+off,rng.randrange(200))
    elif name=='draw_effect_segment':
        d=AUX;s.r.update(esi=d,eax=BUF+0x2000+seed%4)
        s.mem.put(d,SRC)
        # Convex parallelograms, including rotation, edge reversals and both kinds of clipping.
        x=rng.randint(-200,350);y=rng.randint(-140,230);w=rng.randint(4,480);h=rng.randint(2,160);slant=rng.randint(-40,40)
        tilty=rng.randint(-60,60) if seed%3 else 0
        if w*h-slant*tilty<=0:tilty=0
        verts=[(x,y),(x+slant,y+h),(x+w+slant,y+h+tilty),(x+w,y+tilty)]
        if seed%3==0:verts=[(xx,yy) for xx,yy in [(x+w,y+h),(x+w+slant,y),(x+slant,y),(x,y+h)]]
        for i,(xx,yy) in enumerate(verts):s.mem.put(d+4+8*i,xx);s.mem.put(d+8+8*i,yy)
        u=rng.randrange(100);v=rng.randrange(100);uw=rng.randrange(1,60);vh=rng.randrange(1,60)
        for i,(uu,vv) in enumerate([(u,v),(u,v+vh),(u+uw,v+vh),(u+uw,v)]):s.mem.put(d+36+i*2,uu,1);s.mem.put(d+37+i*2,vv,1)
        for i in range(200):
            # Reset leaves UV spare bytes zero as initialized by the game's data allocation.
            s.mem.write(ROW+32*i,bytes(32));s.mem.put(ROW+32*i,65535)
        for i in range(80):s.mem.put(BASE[1]+0x815+96*i,-320+4*i)
