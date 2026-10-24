from models import *
FIELDS=['x_step_1','row_step_1','dx_1','dy_1','x_step_2','row_step_2','dx_2','dy_2','dest_width','dest_height','dest_offset','source_width','source_height']
LAYOUT={'name':'deform_blit_t;src/action_local.h (field names INVENTED/source reconstruction)','size':52,'fields':[{'offset':i*4,'width':4,'signed':True,'meaning':n} for i,n in enumerate(FIELDS)]}
ST=BASE[1]+0x47f0

@model('draw_sprite_deform','Generate a deformed row-major sprite from the13-dword deform_blit_t record built by AFFICHE_SPRITE_DEFORME. Prefill4*(floor(dest_width/4)+1)*dest_height consecutive bytes with0xA1. Walk the two edge vectors with the exact integer error/step rules in deform.py; source sampling is nearest-left Q16, source pitch=source_width, destination pitch=dest_width encoded in signed row_step fields. Outer source increment=(source_height<<16)/(dx_2+dy_2); inner increment=(source_width<<16)/max(dx_1,dy_1), unsigned division. Repeated outer correction reduces inner increment by64 Q16 units for that row. First source pointer is source-1 and each sample reads its+1 byte (the actual x86 reads4 bytes). When dx_1>dy_1, copy only over0xA1; when dx_1<=dy_1, copy only over0x81 despite the0xA1 prefill. This asymmetric original sentinel behavior is retained. Preserve all GPRs; writes14 code-owned scratch dwords.','Descriptor generated from a rotated/scaled rectangular sprite by src/affiche.c: nonnegative dx values,dy values forced>=1,positive destination dimensions and source byte dimensions; mapped destination footprint/source including3-byte read tail. No float sampling.',layouts=[LAYOUT],globals_written=['code1:047f0..04827 deformation scratch'])
def deform(s):
    r=s.r;desc=r['ecx'];source=r['esi']-1;dest=r['edi'];p=[s.mem.get(desc+4*i,signed=True) for i in range(13)]
    A,B,C,D,E,F,G,H,width,height,offset,sw,sh=p
    s.pushad();s.mem.put(ST+32,source);s.push(desc);s.push(dest)
    s.mem.write(dest,bytes([161])*(4*((width>>2)+1)*height));s.pop();s.pop()
    for i,v in enumerate(p[:8]):s.mem.put(ST+4*i,v)
    rowstep=(sh<<16)//(G+H);s.mem.put(ST+52,G+H);s.mem.put(ST+48,sw);s.mem.put(ST+44,rowstep)
    descending=C>D;limit=C if descending else D;goal=H if descending else G
    if descending:s.push(C)
    step=(sw<<16)//limit;s.mem.put(ST+40,step);s.mem.put(ST+36,step)
    if descending:s.pop()
    outer_error=0;outer_count=0;sample_y=0;row_dest=dest+offset;row_source=source;innerstep=step
    iterations=0
    while True:
        iterations+=1
        if iterations>10000:raise ValueError('deformation domain did not terminate')
        for v in (outer_error,row_dest,outer_count,goal,sample_y):s.push(v)
        error=0;sample_x=0;d=row_dest
        for k in range(limit):
            s.push(sample_x)
            value=s.mem.get(row_source+(sample_x>>16)+1,1);s.pop()
            if s.mem.get(d,1)==(161 if descending else 129):s.mem.put(d,value,1)
            sample_x=u32(sample_x+innerstep);error=u32(error+(D if descending else C))
            if k+1<limit:
                d+=A if descending else B
                if (error>C if descending else s32(error)>0):d+=B if descending else A;error=u32(error-(C if descending else D))
        for _ in range(5):s.pop()
        outer_count+=1;outer_error=u32(outer_error+(G if descending else H));row_dest+=F if descending else E;sample_y=u32(sample_y+rowstep)
        s.push(sample_y);row_source=source+sw*(sample_y>>16);s.pop();s.mem.put(ST+40,step);innerstep=step
        if outer_count>=goal:break
        correction=s32(outer_error)>0 if descending else s32(outer_error)>G
        if correction:
            row_dest+=(E-F) if descending else (F-E);outer_count-=1;outer_error=u32(outer_error-(G+H));innerstep=u32(step-64);s.mem.put(ST+40,innerstep)
    s.popad();ret(s)

def prepare(s,name,rng,seed):
    if name!='draw_sprite_deform':return
    # Integer basis vectors from a rotation/reflection, then the exact C caller's
    # abs/sign construction. Bounded small sprites keep every footprint mapped.
    import math
    angle=rng.randrange(360)*math.pi/180;w=rng.randrange(8,40);h=rng.randrange(8,40)
    x1=round(w*math.cos(angle));y1=round(w*math.sin(angle));x2=round(-h*math.sin(angle));y2=round(h*math.cos(angle))
    xs=[0,x1,x2,x1+x2];ys=[0,y1,y2,y1+y2];width=max(xs)-min(xs);height=max(ys)-min(ys)
    if width==0 or height==0:raise ValueError('zero deformation rectangle')
    dx1=abs(x1);dy1=max(1,abs(y1));dx2=abs(x2);dy2=max(1,abs(y2));desc=AUX+0x2000
    p=[1 if x1>=0 else -1,width if y1>=0 else -width,dx1,dy1,1 if x2>=0 else -1,width if y2>=0 else -width,dx2,dy2,width,height,-min(xs)-min(ys)*width,rng.randrange(4,40),rng.randrange(4,40)]
    for i,v in enumerate(p):s.mem.put(desc+4*i,v)
    s.r['ecx']=desc
