from models import *
from hardware import advance
# Rows run top to bottom. '.' is untouched, decimal digit is palette index.
NORMAL={
 'draw_flocon5_Normal':['5'], 'draw_flocon1_Normal':['7'],
 'draw_flocon2_Normal':['78','67'], 'draw_flocon3_Normal':['.6.','866','.6.'],
 'draw_flocon4_Normal':['3'],'draw_normal_5755':['.55.','5775','5775','.55.'],
 'draw_pluie5_Normal':['.4','5.'],'draw_flocon7_Normal':['.66.','6886','6886','.66.'],
 'draw_pluie6_Normal':['.42','532','36.'],
 'draw_flocon6_Normal':['..66..','.7887.','688886','688886','.7887.','..66..'],
 'draw_pluie7_Normal':['..32','.324','543.','46..']}
# Mode-X patterns are columns, in plane-advance order. They intentionally differ
# from normal-mode patterns. Second tuple gives CALL-return sites for ABI scratch.
PLANAR={
 'mode_x_effect_2b3d':(['5'],[]), 'mode_x_effect_2bb7':(['7'],[]),
 'mode_x_effect_2c00':(['87','76'],[0x2c4e]),
 'mode_x_effect_2c59':(['.6.','686','.6.'],[0x2ca4,0x2cb7]),
 'mode_x_effect_2cbf':(['3'],[]),
 'mode_x_effect_2d08':(['.55.','5775','5775','.55.'],[0x2d66,0x2d80,0x2d9a]),
 'mode_x_effect_2da9':(['.5','4.'],[0x2df4]),
 'mode_x_effect_2dfb':(['.66.','6886','6886','.66.'],[0x2e59,0x2e73,0x2e8d]),
 'mode_x_effect_2e9c':(['.36','253','42.'],[0x2eee,0x2f01]),
 'mode_x_effect_2f0c':(['..66..','.7887.','688886','688886','.7887.','..66..'],[0x2f71,0x2f8f,0x2fb7,0x2fdf,0x2ffd]),
 'mode_x_effect_300f':(['..46','.354','243.','32..'],[0x3068,0x307f,0x3092])}

def normal(s,name):
    r=s.r;first=name in ('draw_flocon5_Normal','draw_flocon1_Normal');a,b=('eax','edi') if first else ('edi','eax')
    s.push(r[a]);s.push(r[b]);x,y=s32(r['eax']),s32(r['ebx'])
    top,bottom,left,right=[s.mem.get(BASE[1]+0x5618+4*i,signed=True) for i in range(4)]
    if x>=left and r['eax']<=u32(right) and y>=top and r['ebx']<=u32(bottom):
        dst=r['edi']+r['eax']+s.mem.get(binding('row_offsets_320_01b00')+r['ebx']*4)
        for dy,line in enumerate(NORMAL[name]):
            for dx,c in enumerate(line):
                if c!='.':s.mem.put(dst+320*dy+dx,int(c),1)
    r[b]=s.pop();r[a]=s.pop();ret(s)

for name,pattern in NORMAL.items():
    def fn(s,n=name):normal(s,n)
    model(name,'Weather stamp rows '+repr(pattern)+' (dot leaves pixel untouched, digits are palette indices), at EDI+EAX+320*EBX. Test anchor against inclusive weather bounds left<=x<=right,top<=y<=bottom; the stamp extent is not separately clipped. Preserve GPRs.','C weather anchor coordinates and configured clip box; mapped pattern footprint including edge fringe.',globals_read=['clip_top/bottom/left/right at code1:05618..05627','row_offsets_320_01b00'])(fn)
    DETAILS[name]['pixel_rows']=pattern

def planar(s,name):
    r=s.r;x,y=s32(r['eax']),s32(r['ebx']);s.push(r['edi'])
    unbounded=name=='fplot_mode_x_2b86'
    if unbounded or 0<=x<=320 and 0<=y<=200:
        s.push(r['ecx']);s.push(r['edx']);r['edi']+=(x//4)+80*y;r['eax']=((1<<(x&3))<<8)|2;r['ebx']=u32(y*64)
        r['edx']=setlo(r['edx'],0x3c4,16);s.out(0x3c4,r['eax'],2)
        if unbounded:patterns,returns=([str(r['esi']&255)],[])
        else:patterns,returns=PLANAR[name]
        for col,pattern in enumerate(patterns):
            if col:
                s.push(BASE[1]+returns[col-1]);advance(s)
            if unbounded:s.mem.put(r['edi'],r['esi'],1)
            else:
                for row,c in enumerate(pattern):
                    if c!='.':s.mem.put(r['edi']+80*row,int(c),1)
        r['edx']=s.pop();r['ecx']=s.pop()
    r['edi']=s.pop();ret(s)

for name,(columns,returns) in PLANAR.items():
    def fn(s,n=name):planar(s,n)
    model(name,'Mode-X weather stamp columns '+repr(columns)+' (top to bottom per column; dot untouched, digits palette indices). Anchor admitted iff0<=x<=320 and0<=y<=200 inclusive. Aperture address=EDI+floor(x/4)+80*y, initial plane=1<<(x&3); advance planes per column, wrapping8->1 and advancing aperture byte. OUT0x3C4,word((mask<<8)|2) initially and through helper1:02b30. No extent clipping. End EBX=64*y,EAX=final mask<<8|2; preserve ECX,EDX,ESI,EDI,EBP.','Mapped VGA footprint; anchor can lie outside screen and is then rejected. Plane writes and exact I/O are compared.')(fn)
    DETAILS[name]['pixel_columns']=columns;DETAILS[name]['plane_advance_return_sites']=[f'1:{a:05x}' for a in returns]
model('fplot_mode_x_2b86','Store low byte of ESI at VGA plane x&3,address EDI+floor(x/4)+80*y. OUT0x3C4,word((1<<(x&3))<<8|2) at1:02b9a. No clipping. End EAX=mask<<8|2,EBX=64*y; preserve other GPRs.','Mapped pixel; x0..319,y0..199.')(lambda s:planar(s,'fplot_mode_x_2b86'))

def prepare(s,name,rng,seed):
    if name in NORMAL:
        for i,v in enumerate((8,160,16,280)):s.mem.put(BASE[1]+0x5618+4*i,v)
        s.r.update(eax=u32(rng.choice([0,15,16,17,100,279,280,281,-1])),ebx=u32(rng.choice([0,7,8,9,80,159,160,161,-1])))
    elif name in PLANAR or name=='fplot_mode_x_2b86':
        s.r.update(edi=0xa0500,eax=u32(rng.choice([-1,0,1,2,3,4,100,317,318,319,320,321])),ebx=u32(rng.choice([-1,0,1,100,199,200,201])))
        if name=='fplot_mode_x_2b86':s.r.update(eax=rng.randrange(320),ebx=rng.randrange(200),esi=rng.randrange(256))
