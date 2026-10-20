# Probe (docs/COMPILER.md, docs/DATA.md): Watcom _BSS order rule vs random TUs.
import random, sys
sys.path.insert(0,'tools')
import omf, wcc
from bssrule import layout
from pathlib import Path
TYPES=[('char',1),('short',2),('int',4),('char',3),('short',3),('int',2),('char',8),('char',16),('int',8),('char',5),('char',6),('short',5)]
def run(inst, seed):
    rnd=random.Random(seed); n=rnd.randint(20,150); objs=[]; lines=[]; used=set()
    for i in range(n):
        while True:
            nm='v'+''.join(rnd.choice('abcdefghijklmnopqrstuvwxyz_0123456789') for _ in range(rnd.randint(0,9)))
            if nm[0]!='_' and nm not in used: used.add(nm); break
        t,k=rnd.choice(TYPES)
        if k in (1,2,4) and t in ('char','short','int') and {'char':1,'short':2,'int':4}[t]==k:
            lines.append(f'{t} {nm};'); size=k
        else:
            cnt=k; lines.append(f'{t} {nm}[{cnt}];'); size={'char':1,'short':2,'int':4}[t]*cnt
        objs.append((nm,size,i))
    p=Path(f'build/tmp/bss_{seed}.c'); p.write_text('\n'.join(lines)+'\n')
    out=p.with_suffix('.obj'); rc,txt=wcc.compile_c(p,out,['-zq','-oaxt','-s','-5r'],inst); assert rc==0,txt
    m=omf.load(out)[0]
    bss=[i for i,s in enumerate(m.segments) if s and s.name=='_BSS'][0]
    got=sorted((o,n[1:]) for n,s,o,_ in m.publics if s==bss)
    pred=[(o,n) for o,n,s,i in layout(objs)]
    out.unlink(); p.unlink()
    return got==pred, n
for inst in ('wc106','wc105'):
    res=[run(inst,s) for s in range(30)]
    print(inst, 'layouts predicted exactly:', sum(r[0] for r in res),'/',len(res), 'objects', sum(r[1] for r in res))
