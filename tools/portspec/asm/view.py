import sys,re,json
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[2]
fs=json.loads((HERE/'inventory.json').read_text())
for name in sys.argv[1:]:
    f=next(f for f in fs if f['name']==name)
    text=(ROOT/f['source']).read_text();start=re.search(r'^'+re.escape(name)+r'_:\s*',text,re.M).start()
    following=[]
    for other in fs:
        if other['source']==f['source']:
            m=re.search(r'^'+re.escape(other['name'])+r'_:',text,re.M)
            if m and m.start()>start:following.append(m.start())
    body=text[start:min(following) if following else len(text)]
    print('\n###',name,f['address'])
    if '--short' in sys.argv:
        print('\n'.join(l for l in body.splitlines() if any(k in l for k in ('mov byte ptr [edi','mov word ptr [edi','mov dword ptr [edi','call ','add edi','sub edi'))))
    else:print(body)
