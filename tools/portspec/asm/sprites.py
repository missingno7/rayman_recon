"""Normal sprite models. Clipping is a portable interval computation with the
original partial-register results and stack scratch specified explicitly.
"""
from models import *
WIDTH=binding('saved_sprite_width')
COLOR=binding('saved_sprite_color')

def clip(s,flip=False,diff=False):
    r=s.r;x=s32(r['eax']);y=s32(r['ebx']);w=r['ecx']&255;h=(r['ecx']>>8)&255
    xmin,xmax,ymin,ymax=[s.mem.get(a,signed=True) for a in CLIP]
    width_addr=BASE[1]+0x6d2f if diff else WIDTH
    s.mem.put(width_addr,w);r['edx']=w
    def reject():s.cf=1;ret(s);return False
    cropped_left=x<xmin
    if cropped_left:
        distance=xmin-x
        if diff:s.push(r['eax'])
        r['eax']=u32(distance)
        if w<=distance:
            if diff:r['eax']=s.pop()
            return reject()
        distance&=0xfffc if diff else 0xffff
        r['eax']=distance;r['edx']=w-distance;r['ecx']=setlo(r['ecx'],w-distance)
        if diff:
            r['edx']=distance;r['eax']=s.pop();r['eax']=u32(r['eax']+distance)
            s.mem.put(r['esp']+4,s.mem.get(r['esp']+4)+distance);r['esi']+=distance
        else:
            if not flip:r['esi']+=distance
            r['eax']=u32(xmin)
        x=s32(r['eax']);w=r['ecx']&255
    r['edx']=u32(r['edx']+r['eax']) if not diff else u32(w+r['eax'])
    if diff and cropped_left:r['ecx']=setlo(r['ecx'],(r['ecx']&255)//4)
    elif r['edx']>u32(xmax):
        if r['eax']>=u32(xmax):return reject()
        r['edx']=u32(xmax-r['eax']);r['ecx']=setlo(r['ecx'],r['edx'])
        if flip:r['esi']+=s.mem.get(WIDTH)-r['edx']
        if diff:
            d=r['ecx']&255;r['edx']=d&3;r['ecx']=setlo(r['ecx'],(d+3)//4)
    elif diff:r['ecx']=setlo(r['ecx'],(r['ecx']&255)//4)
    r['edx']=(r['ecx']>>8)&255
    if y<ymin:
        r['edx']=u32(r['edx']+r['ebx'])
        if s32(r['edx'])<=ymin:return reject()
        skip=(ymin-y)&65535;r['ebx']=skip;r['edx']=h-skip;r['ecx']=ah(r['ecx'],h-skip)
        s.push(r['eax']);r['eax']=s.mem.get(width_addr)*skip
        r['esi']+=r['eax']
        if diff:s.mem.put(r['esp']+8,s.mem.get(r['esp']+8)+r['eax'])
        r['eax']=s.pop();r['ebx']=u32(ymin)
        r['edx']=0 if diff else (r['ecx']>>8)&255
    r['edx']=u32(r['edx']+r['ebx'])
    if r['edx']>u32(ymax):
        if r['ebx']>=u32(ymax):return reject()
        r['edx']=u32(ymax-r['ebx']);r['ecx']=ah(r['ecx'],r['edx'])
    s.cf=0;ret(s);return True

@model('clip_sprite_on_screen','Intersect packed CL width/CH height sprite with half-open clip bounds. Save original width, crop source left/top, update x/y and dimensions; CF=1 rejects. Exact output registers and failed-clip partial mutations are modeled.','1..255 width/height, ordered screen clip; mapped row-major source.',globals_read=['XMIN','XMAX','YMIN','YMAX'],globals_written=['saved_sprite_width'])
def clip_normal(s):clip(s)
@model('clip_sprite_on_screen_flipped','Clip horizontally mirrored sprite. Left cropping trims width without advancing source; right cropping advances source by original_width-new_width. Top crop advances source by original_width*rows. CF=1 rejects.','Same as clip_sprite_on_screen.',globals_read=['XMIN','XMAX','YMIN','YMAX'],globals_written=['saved_sprite_width'])
def clip_flip(s):clip(s,True)

def normal(s,name):
    r=s.r;flip='Flip' in name;color='Color' in name;raw='256' in name;noclip='NoClip' in name
    if color:r['edx']=u32(r['edx']*8);s.mem.put(COLOR,r['edx'])
    if noclip:r['edx']=r['ecx']&255;s.mem.put(WIDTH,r['edx'])
    else:
        callsite={'DrawSpriteNormal':0x6a29,'DrawSpriteNormal256':0x6aa2,'DrawSpriteFlipNormal':0x6b0d,
                  'DrawSpriteFlipNormal256':0x6b8d,'DrawSpriteColorNormal':0x6bfc,'DrawSpriteColorFlipNormal':0x6c9b}[name]
        s.push(BASE[1]+callsite);ok=clip(s,flip);s.cf=None
        if not ok or not r['ecx']&255:ret(s);return
    rowtable=binding('row_offsets_80_01e20')
    r['edx']=s.mem.get(rowtable+r['ebx']*4)*4;r['edi']+=r['edx']
    r['edx']=(r['ecx']>>8)&255;r['ebx']=s.mem.get(rowtable+r['edx']*4)*4+r['edi'];r['edi']+=r['eax'];r['ecx']&=255
    s.push(r['ebp']);r['edx']=s.mem.get(WIDTH)
    if not raw:r['eax']=0
    if color:r['eax']=ah(r['eax'],s.mem.get(COLOR,1))
    while True:
        i=0;start=r['edi']
        if flip:s.push(start)
        while True:
            value=s.mem.get(r['esi']+i,1);r['eax']=setlo(r['eax'],value)
            if raw:
                if value:s.mem.put(start+(r['ecx']-1-i if flip else i),value,1)
                i+=1
            elif value<160:
                if color:r['eax']=setlo(r['eax'],value|((r['eax']>>8)&255))
                s.mem.put(start+(r['ecx']-1-i if flip else i),r['eax'],1);i+=1
            else:
                if color:r['eax']&=255
                r['eax']=u32(r['eax']-160);i+=r['eax']
                if color:r['eax']=ah(r['eax'],s.mem.get(COLOR,1))
            if i>=r['ecx']:break
        if flip:r['edi']=s.pop()
        r['edi']+=320;r['esi']+=r['edx']
        if r['edi']>=r['ebx']:break
    r['ebp']=s.pop();ret(s)

for name in ('DrawSpriteNormalNoClip','DrawSpriteNormal','DrawSpriteNormal256','DrawSpriteFlipNormalNoClip','DrawSpriteFlipNormal','DrawSpriteFlipNormal256','DrawSpriteColorNormal','DrawSpriteColorFlipNormal'):
    desc=('Draw packed sprite with source pitch=original CL and destination pitch320. '+
          ('Zero is transparent; every other byte is copied. ' if '256' in name else 'Values<0xA0 draw; a marker>=0xA0 advances the source/destination column by marker-0xA0 (0xA0 is invalid). ')+
          ('Mirror destination columns. ' if 'Flip' in name else '')+
          ('Visible pixels OR ((EDX*8)&255). ' if 'Color' in name else '')+
          ('No clipping. ' if 'NoClip' in name else 'Use '+('mirrored ' if 'Flip' in name else '')+'clip helper. ')+
          'Preserve EBP; other output registers are specified by the executable ABI adapter.')
    def fn(s,n=name):normal(s,n)
    model(name,desc,'Positive packed byte width/height; ordered nonempty clip; valid run-marker rows; mapped disjoint320-byte-pitch destination and source.',layouts=[{'name':'sprite rows (INVENTED)','fields':'width*height u8 pixels, row pitch original width; transparency as described'}],globals_read=['row_offsets_80_01e20','sprite clip box'],globals_written=['saved_sprite_width']+(['saved_sprite_color'] if 'Color' in name else []))(fn)

@model('DrawSpriteDiffNormal','Clip image/mask together. Width becomes dword count: floor(width/4) when no right crop, ceil(cropped_width/4) at right edge; left crop advances in multiples of4 and may leave x up to3 pixels left of XMIN. At each nonzero image dword write (destination & mask)|image; zero image dwords leave destination untouched. Original source pitch in bytes; destination pitch320. Preserve EBP; exact clobbers in model.','Positive byte width/height; image/mask mapped with at least3 right padding bytes per row; clip window ordered; source width normally multiple4.',globals_read=['sprite clip box','row_offsets_80_01e20'],globals_written=['code1:06d2f saved original width'])
def diff(s):
    r=s.r;r['esp']-=12;s.mem.put(r['esp'],r['edx']);s.push(BASE[1]+0x6e18);ok=clip(s,diff=True);s.cf=None
    if ok and r['ecx']&255:
        t=binding('row_offsets_80_01e20');r['edx']=s.mem.get(t+r['ebx']*4)*4;r['edi']+=r['edx']
        r['edx']=(r['ecx']>>8)&255;r['ebx']=s.mem.get(t+r['edx']*4)*4+r['edi'];s.mem.put(r['esp']+4,r['ebx'])
        r['edi']+=r['eax'];r['edx']=s.mem.get(r['esp']);r['ecx']&=255;s.push(r['ebp']);r['ebx']=0
        while True:
            for i in range(r['ecx']):
                r['eax']=s.mem.get(r['esi']+4*i)
                if r['eax']:
                    r['ebx']=s.mem.get(r['edx']+4*i);s.mem.put(r['edi']+4*i,(s.mem.get(r['edi']+4*i)&r['ebx'])|r['eax'])
            r['edi']+=320;r['esi']+=s.mem.get(BASE[1]+0x6d2f);r['edx']+=s.mem.get(BASE[1]+0x6d2f)
            if r['edi']>=s.mem.get(r['esp']+8):break
        r['ebp']=s.pop()
    r['esp']+=12;ret(s)

def planar(s,name):
    r=s.r;color='Color' in name;noclip='NoClip' in name
    flip=name in ('DrawSpriteFlipX','DrawSpriteNoClipX','DrawSpriteColorFlipX')
    if color:r['edx']=u32(r['edx']*8);s.mem.put(COLOR,r['edx'])
    if noclip:r['edx']=r['ecx']&255;s.mem.put(WIDTH,r['edx'])
    else:
        returns={'DrawSpriteX':0x63f7,'DrawSpriteFlipX':0x656a,'DrawSpriteColorX':0x66f1,'DrawSpriteColorFlipX':0x6888}
        s.push(BASE[1]+returns[name]);ok=clip(s,flip);s.cf=None
        if not ok:ret(s);return
    s.push(r['ebp']);src_state=BASE[1]+(0x66d7 if color else 0x63de);dst_state=src_state+4
    r['edx']=s.mem.get(WIDTH);s.mem.put(src_state,r['edx'])
    t=binding('row_offsets_80_01e20');r['edi']+=s.mem.get(t+r['ebx']*4);r['edx']=(r['ecx']>>8)&255
    r['ebx']=s.mem.get(t+r['edx']*4)+r['edi'];r['ecx']&=255
    s.push(r['ecx']);x=r['eax'];r['eax']>>=2;r['edi']+=r['eax'];r['eax']=ah(r['eax'],1<<(x&3));r['ecx']=s.pop()
    if flip:r['esi']+=r['ecx']-1
    for phase in range(4):
        if phase<3:s.push(r['ecx'])
        count=(r['ecx']+3-phase)//4;r['ecx']=count
        if count>0:
            if flip:r['ecx']=u32(-count)
            s.mem.put(dst_state,80-count)
            if phase==0:
                r['edx']=0x3c4;r['eax']=setlo(r['eax'],2);s.out(0x3c4,r['eax']&65535,2)
                r['eax']=setlo(r['eax'],r['eax']>>8);r['edx']+=1
            else:
                mask=((r['eax']&255)*2)&255
                if mask==16:mask=1;r['edi']+=1
                r['eax']=setlo(r['eax'],mask);s.out(r['edx']&65535,mask)
            if phase<3:
                s.push(r['esi']);s.push(r['edi'])
                if color:s.push(r['eax']);r['eax']=s.mem.get(COLOR)
            while True:
                if color and phase==3:r['eax']=s.mem.get(COLOR)
                for i in range(count):
                    value=s.mem.get(r['esi']+(-4*i if flip else 4*i),1);r['eax']=ah(r['eax'],value)
                    if value<160:
                        if color:r['eax']=ah(r['eax'],value|(r['eax']&255))
                        s.mem.put(r['edi'],r['eax']>>8,1)
                    r['edi']+=1
                r['edi']+=s.mem.get(dst_state);r['esi']+=s.mem.get(src_state)
                if r['edi']>=r['ebx']:break
            if phase<3:
                if color:r['eax']=s.pop()
                r['edi']=s.pop();r['esi']=s.pop()
        if phase<3:
            r['ecx']=s.pop();r['esi']+=-1 if flip else 1
    r['ebp']=s.pop();ret(s)

for name in ('DrawSpriteX','DrawSpriteFlipX','DrawSpriteFlipNoClipX','DrawSpriteNoClipX','DrawSpriteColorX','DrawSpriteColorFlipX'):
    flip=name in ('DrawSpriteFlipX','DrawSpriteNoClipX','DrawSpriteColorFlipX')
    def fn(s,n=name):planar(s,n)
    model(name,'Draw '+('mirrored ' if flip else '')+'packed sprite through four VGA planes. Pixel values<0xA0 draw; all bytes>=0xA0 are individual transparent pixels (no run skipping). '+('Visible pixel ORs low byte of EDX*8. ' if 'Color' in name else '')+('No clipping. ' if 'NoClip' in name else 'Clip with shared sprite helper. ')+
        'For phase p=0..3, copy ceil((width-p)/4) samples with source column step'+('-4' if flip else '+4')+', original source row pitch and80-byte aperture pitch. Select initial plane1<<(x&3) using word OUT0x3C4, then remaining planes via byte OUT0x3C5; wrap increments aperture byte. Preserve EBP. Original no-clip entry names are counterintuitive: FlipNoClipX is forward, NoClipX mirrors. Exact clobbers, scratch values and plane bytes are modeled.',
        'Positive byte width/height; mapped VGA footprint and packed sprite; ordered clip window.',globals_read=['row_offsets_80_01e20','sprite clip box'],globals_written=['saved_sprite_width','mode_x_sprite_state' if 'Color' in name else 'code1:063de/063e2']+(['saved_sprite_color'] if 'Color' in name else []))(fn)

def prepare(s,name,rng,seed):
    if name not in MODELS or (not name.startswith(('DrawSprite','clip_sprite'))):return
    w=rng.choice([4,8,12,16,24,32,48,64]);h=rng.choice([1,2,8,16,32,64]);xmin,xmax,ymin,ymax=16,240,8,150
    for a,v in zip(CLIP,(xmin,xmax,ymin,ymax)):s.mem.put(a,v)
    xs=[16,17,20,220,239,240,241,12,-50,0];ys=[8,9,60,149,150,151,4,-70,0]
    x=rng.choice(xs);y=rng.choice(ys)
    if 'NoClip' in name:x=32;y=32
    if name.endswith('X'):
        w=rng.choice([1,2,3,4,5,7,8,15,16,31,32,64]);x=rng.choice([16,17,18,19,20,237,238,239,240,241,12,-50]);s.r['edi']=0xa0500
        if 'NoClip' in name:x=rng.randrange(32,36);y=32
    s.r.update(eax=u32(x),ebx=u32(y),ecx=(rng.getrandbits(16)<<16)|(h<<8)|w,edx=AUX+0x2000 if name=='DrawSpriteDiffNormal' else rng.randrange(32))
    src=s.r['esi']
    for row in range(h):
        for col in range(w):
            if '256' in name:v=rng.choice([0,0,rng.randrange(256)])
            elif name=='DrawSpriteDiffNormal':v=rng.choice([0,0,0,rng.randrange(256)])
            else:v=rng.randrange(160) if rng.randrange(3) else 160+rng.randrange(1,min(95,w-col)+1)
            s.mem.put(src+row*w+col,v,1)
    if name=='DrawSpriteDiffNormal':
        for row in range(h):
            for col in range(w):s.mem.put(s.r['edx']+row*w+col,rng.choice([0,255]),1)
