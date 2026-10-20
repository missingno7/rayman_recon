# Probe (docs/DATA.md): Rayverse BSS names vs the _BSS hash order of the oracle.
import re, sys
sys.path.insert(0,'tools')
from bssrule import key, rank
SIZES={'u8':1,'s8':1,'char':1,'bool':1,'u16':2,'s16':2,'u32':4,'s32':4,'int':4}
rows=[]
for ln in open('external/rayverse/src/data.c',encoding='latin-1'):
    m=re.match(r'\s*(\w+)\s+(\w+)\s*(\[[^\]]*\])?\s*;\s*//\s*([0-9A-Fa-f]{5,6})\b',ln)
    if not m: continue
    t,n,arr,a=m.groups(); a=int(a,16)-0x90000
    if a<0x3218a or t not in SIZES: continue      # BSS only, scalar/simple-array types
    size=SIZES[t]
    if arr:
        try: size*=int(arr.strip('[]'),0)
        except ValueError: continue
    rows.append((a,n,size))
rows.sort()
print('BSS globals with Rayverse name+address+size:', len(rows))
# BSS blocks are contiguous per (object, size rank); look at consecutive pairs that are adjacent in memory and same rank
asc=desc=0; runs=[]
for (a1,n1,s1),(a2,n2,s2) in zip(rows,rows[1:]):
    if a1+s1==a2 and rank(s1)==rank(s2):
        if key(n1)<=key(n2): asc+=1
        else: desc+=1
print('adjacent same-rank pairs: key ascending', asc, 'descending', desc)
