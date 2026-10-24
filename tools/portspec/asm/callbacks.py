"""Wrapper proofs reuse tilecode's existing parser/pixel specification verbatim.
No new tile grammar, asset census or per-tile proof is performed here.
"""
import importlib.util
from models import *
from builders import MAP
TILEDIR=ROOT/'build/workers/tilecode'
def readmodule(name,path):
    spec=importlib.util.spec_from_file_location(name,path);m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);return m
extract=readmodule('asmspec_tile_extract',TILEDIR/'extract.py')
# Existing parser imports extract by its original name; isolate that import.
old=sys.modules.get('extract');sys.modules['extract']=extract
tile_catalogue=readmodule('asmspec_tile_catalogue',TILEDIR/'catalogue.py')
if old is None:del sys.modules['extract']
else:sys.modules['extract']=old
_pool=None
def pool():
    global _pool
    if _pool is None:
        level=next(p for p in sorted((ROOT/'assets/GOG/PCMAP').rglob('*.LEV')) if extract.read_level(p)['bodies'])
        data=extract.read_level(level);bodies=list(data['bodies'].values())[:16]
        _pool=[(b,tile_catalogue.parse(b)['image']) for b in bodies]
    return _pool

def wrapper(s,name):
    r=s.r;variant=name=='asm_04493';table=r['ecx'];r['esp'] # original ECX is restored
    s.push(r['ebp']);s.push(r['ecx']);s.push(r['ecx']);r['ebp']=table;r['esi']=r['edx'];r['ecx']=240
    if variant:
        r['edi']+=s.mem.get(r['ebx']+8)+80;r['esi']+=s.mem.get(r['ebx']+4)
        if r['esi']>=s.mem.get(r['ebx']+20):
            r['ecx']=s.pop();r['ecx']=s.pop();r['ebp']=s.pop();ret(s);return
    rows=12 if variant else 14;s.mem.put(r['esp']+4,rows)
    while True:
        s.push(r['ebx']);s.push(r['esi']);r['ebx']=0;s.mem.put(r['esp']+8,20)
        for col in range(20):
            kind=s.mem.get(r['esi']+4,1);r['ebx']=setlo(r['ebx'],kind)
            if kind==2:
                r['ebx']=setlo(r['ebx'],s.mem.get(r['esi'],2),16);r['edx']=s.mem.get(table+r['ebx']*4)
                if r['edx']:
                    s.push(r['edi']);s.push(BASE[1]+(0x44f9 if variant else 0x4488))
                    image=s.inputs['callback_images'][r['edx']];phase=(r['eax']>>8)&255
                    if phase<4:
                        for y in range(16):
                            for x in range(4):
                                value=image[y*16+phase+4*x]
                                if value is not None:s.mem.put(r['edi']+80*y+x,value,1)
                        r['edi']+=1200
                    r['edx']=80;ret(s);r['edi']=s.pop()
            r['esi']+=6;r['edi']+=4;s.mem.put(r['esp']+8,19-col)
        r['esi']=s.pop();r['ebx']=s.pop();rows-=1;s.mem.put(r['esp']+4,rows)
        if rows==0:break
        r['esi']+=s.mem.get(r['ebx']+4)
        if r['esi']>=s.mem.get(r['ebx']+20):break
        r['edi']+=s.mem.get(r['ebx']+8)
    r['ecx']=s.pop();r['ecx']=s.pop();r['ebp']=s.pop();ret(s)

for name in ('asm_04430','asm_04493'):
    def fn(s,n=name):wrapper(s,n)
    model(name,'REUSED: ../tilecode/SPEC.md sections on ModeX callback callers. Traverse20 map records per row and '+('up to12 inner rows after skipping first map/destination row' if name=='asm_04493' else 'up to14 rows')+'. Only transparency byte+4==2 with nonnull callback table entry draws a sparse constant16x16 tile selected by AH phase0..3. EDI advances4 per tile and context.run_step between rows; ESI advances context.source_end (used here as map byte stride) between rows and stops before context.last_tile. Callback pixel semantics/clobbers are imported from tilecode/catalogue.py and its existing71400-run certificate, not re-specified. Preserve EAX/EBX/ECX/EBP; ESI/EDI/EDX outputs specified by wrapper model.','Mapped6-byte map records,1200-entry callback table,frame_context_t,phaseAH0..3,valid sparse tile bodies from shipped GOG levels. Wrapper execution uses16 representative bodies from one level; full body corpus proof is inherited.',layouts=[MAP,{'name':'frame_context_t;src/frame.c (purpose names INVENTED)','fields':[{'offset':4,'width':4,'signed':False,'meaning':'source_end reused as map byte row stride'},{'offset':8,'width':4,'signed':False,'meaning':'run_step reused as destination row adjustment (normally1200)'},{'offset':20,'width':4,'signed':False,'meaning':'last_tile pointer stopping threshold'}]}])(fn)

def prepare(s,name,rng,seed):
    if name not in ('asm_04430','asm_04493'):return
    table=AUX;ctx=AUX+0x2000;tiles=AUX+0x3000;code=AUX+0x8000;images={};pp=pool()
    for i,(body,image) in enumerate(pp):
        assert len(body)<4096
        at=code+i*4096;s.mem.write(at,body);s.mem.put(table+4*i,at if rng.randrange(4) else 0);images[at]=image
    s.inputs['callback_images']=images
    s.r.update(eax=(rng.getrandbits(32)&~0xff00)|(rng.randrange(4)<<8),ebx=ctx,ecx=table,edx=tiles,edi=0xa0500,esi=SRC)
    width=rng.choice([20,24,32]);rows=rng.choice([1,2,5,14,15,16])
    for off,v in [(4,width*6),(8,1200),(20,tiles+width*6*rows-6)]:s.mem.put(ctx+off,v)
    for y in range(16):
        for x in range(width):s.mem.put(tiles+6*(y*width+x),rng.randrange(len(pp)),2);s.mem.put(tiles+6*(y*width+x)+4,rng.choice([0,1,2]),1)
