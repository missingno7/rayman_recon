import re, sys, json
sys.path.insert(0,'tools')
from bssrule import key, rank, sortkey
import android
A=android.load()
SIZES={'u8':1,'s8':1,'char':1,'bool':1,'u16':2,'s16':2,'u32':4,'s32':4,'int':4}
dos={}
for ln in open('external/rayverse/src/data.c',encoding='latin-1'):
    m=re.match(r'\s*[\w ]+?\s+\*?(\w+)\s*(\[[^\]]*\])*\s*;\s*//\s*([0-9A-Fa-f]{5,6})\b',ln)
    if m:
        a=int(m.group(3),16)-0x90000
        if a>=0x3218a: dos[m.group(1)]=a
common=[n for n in dos if n in A and A[n]['section']=='.bss' and A[n]['kind']=='O']
print('BSS names with DOS addr and Android .bss entry:', len(common))
# Android order index
aord={n:i for i,n in enumerate(sorted(common,key=lambda n:A[n]['addr']))}
dord=sorted(common,key=lambda n:dos[n])
# size from Android
def rk(n): return rank(A[n]['size'])
# Pairwise agreement test of the rule using Android order as declaration order (idx), with unknown group offset:
# compare predicted relative order (by sortkey with idx = android index) vs DOS order, for pairs in same rank.
best=None
for first in (0,):
  for scale in (1,):
    agree=tot=0
    for i,a in enumerate(dord):
        for b in dord[i+1:i+40]:
            if rk(a)!=rk(b): continue
            pa=sortkey(a,A[a]['size'],aord[a]); pb=sortkey(b,A[b]['size'],aord[b])
            tot+=1; agree+= pa<pb
    print('pairs agree (android idx as decl order):',agree,'/',tot)
# also: is Android order monotone with DOS within size-rank? i.e. maybe android sorts by hash too?
for rev in (False,True):
    agree=tot=0
    for i,a in enumerate(dord):
        for b in dord[i+1:i+40]:
            if rk(a)!=rk(b): continue
            tot+=1; agree+= (A[a]['addr']<A[b]['addr'])!=rev
    print('android address order agrees with DOS order (rev=%s):'%rev, agree,'/',tot)
# size agreement
mis=[(n,A[n]['size']) for n in common][:0]
