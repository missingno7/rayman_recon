"""Compile, naturally link, compare and pack the reconstructed DOS program.

Run from the repository root: python -B tools/link.py [compile|link|report|pack|all]
All generated artifacts go under build/link. The comparison is diagnostic; an
UNDEFSOK image is never executed or represented as a matching executable.
"""
import argparse
import collections
import dataclasses
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import time

sys.dont_write_bytecode = True
SCRIPT_DIR = Path(__file__).resolve().parent
ROOT = next(p for p in SCRIPT_DIR.parents if (p/'config/toolchain.json').is_file())
H = ROOT/'build/link' if SCRIPT_DIR.name=='tools' else SCRIPT_DIR/'build/link'
MANIFEST = H/'manifest.json'
DIAGNOSTIC_OMISSIONS = ()
sys.path.insert(0, str(ROOT / 'tools'))
import wcc
import omf
import match
import pmw1
from manifest import validate_manifest
from image import Image

def guard(event, args):
    paths = []
    if event == 'open':
        p, mode, flags = args
        if flags & (os.O_WRONLY | os.O_RDWR | os.O_CREAT | os.O_TRUNC | os.O_APPEND):
            paths = [p]
    elif event in ('os.mkdir','os.remove','os.rmdir','os.chmod','os.utime'):
        paths = args[:1]
    elif event in ('os.rename','os.replace'):
        paths = args[:2]
    for p in paths:
        if isinstance(p, (str, bytes)):
            assert Path(p).resolve().is_relative_to(H), (event, p)

def read(p): return json.loads(Path(p).read_text(encoding='utf-8'))
def save(n, v): (H / n).write_text(json.dumps(v, indent=2) + '\n', encoding='utf-8')
def sha(b): return hashlib.sha256(b).hexdigest()
def record(v):
    with (H / 'commands.jsonl').open('a', encoding='utf-8') as f:
        f.write(json.dumps(v) + '\n')
def units(): return read(H / 'units_snapshot.json')['units'] if (H/'units_snapshot.json').exists() else read(ROOT / 'config/units.json')['units']
def manifest(): return validate_manifest(read(MANIFEST))
def libraries():
    cfg=read(ROOT/'config/oracles.json')['oracles']['gog_v121_us']
    runtime=[wcc.TOOLS_DIR/r for r in cfg['runtime_libs']]
    by_name={p.name.lower():p for p in runtime}
    # Preserve the measured WLINK order: CLIB3R, software-FP MATH3R, then MATH387R.
    ordered=[by_name[n] for n in ('clib3r.lib','math3r.lib','math387r.lib') if n in by_name]
    ordered.extend(p for p in runtime if p not in ordered)
    return ordered+[wcc.install('hmisos3')[1]/'sosdw1cr.lib']

def modules():
    return {n: omf.load(H / 'objects' / (n + '.obj'))[0] for n in units()}
def run(argv, timeout=60):
    _, base = wcc.install('wc106')
    tmp = H / 'tmp'; tmp.mkdir(exist_ok=True)
    env = dict(SYSTEMROOT=os.environ.get('SYSTEMROOT',r'C:\Windows'),WATCOM=str(base),
               LIB=str(base/'LIB386')+';'+str(base/'LIB386/DOS'),PATH='',TMP=str(tmp),TEMP=str(tmp))
    start = time.time()
    p = subprocess.run([str(a) for a in argv],cwd=H,env=env,capture_output=True,timeout=timeout)
    rec = dict(argv=[str(a) for a in argv],cwd=str(H),env=env,timeout=timeout,rc=p.returncode,
               stdout=p.stdout.decode('latin1'),stderr=p.stderr.decode('latin1'),seconds=time.time()-start)
    record(rec)
    return rec

def oracle():
    cfg=read(ROOT/'config/oracles.json')['oracles']['gog_v121_us']
    b=(ROOT/cfg['source']['path']).read_bytes()
    assert sha(b)==cfg['sha256']
    p=pmw1.PMW1(b)
    data=[d for _,d,_ in p.object_data()]
    for i,d in enumerate(data,1): assert sha(d)==cfg['objects'][str(i)]['sha256']
    fx,_=p.fixups()
    return dict(entry=[p.cs_obj,p.eip,p.ss_obj,p.esp],objects=p.objects,data=data,fixups=fx)

def assets():
    # Explicit user-authorized whitelist. No other oracle bytes enter input objects.
    # PalFire is defined by data_5fa0 at 2:05fb2..05fe2, so it is never generated here.
    # Types from accepted skops.c / screen_types.h and palette.h, not guessed widths.
    data=oracle()['data'][1]
    specs=asset_specs()
    inv=[]
    for name,start,size,width in specs:
        b=data[start:start+size]
        (H/'assets').mkdir(exist_ok=True)
        (H/'assets'/(name+'.bin')).write_bytes(b)
        text='/* Generated external asset; extracted from user-owned GOG RAYMAN.EXE. */\n'
        if width==1:
            text+='#include "palette.h"\nrgb_t '+name+'[16] = {\n'
            text+='\n'.join('{'+','.join(str(x) for x in b[i:i+3])+'},' for i in range(0,size,3))+'\n};\n'
        else:
            text+='#include "rtypes.h"\n#include "screen_types.h"\ndef_bande_t Def_Bande[6][10][300] = {\n'
            for world in range(6):
                text+='{\n'
                for level in range(10):
                    text+='{\n'
                    for row in range(300):
                        at=((world*10+level)*300+row)*8
                        vals=[int.from_bytes(b[at:at+2],'little'),*b[at+2:at+8]]
                        text+='{'+','.join(str(x) for x in vals)+'},\n'
                    text+='},\n'
                text+='},\n'
            text+='};\n'
        src=H/'assets'/(name+'.c'); src.write_text(text,encoding='ascii')
        prof=wcc.profile('game')
        rc,output=wcc.build(src,H/'objects'/('asset_'+name+'.obj'),prof['flags']+
                            ['-i='+str(H/'src')],prof['install'])
        record(dict(api='tools/wcc.py build',source=str(src),rc=rc,text=output))
        assert rc==0,output
        asset_mod=omf.load(H/'objects'/('asset_'+name+'.obj'))[0]
        asset_seg=next(s for s in asset_mod.segments if s and s.size)
        assert bytes(asset_seg.data)==b and not asset_mod.fixups
        inv.append(dict(name=name,object=2,start=start,size=size,width=width,sha256=sha(b),
                        type='rgb_t[16]' if width==1 else 'def_bande_t[6][10][300]',omf_bytes_equal=True))
    save('assets.json',inv)


def asset_specs():
    return [('PalPlasma',0x5f70,48,1),('Def_Bande',0x80fc,144000,2)]

def compile_all():
    (H/'objects').mkdir(exist_ok=True)
    # Preserve local include paths and source leaf names; no canonical source mutation.
    for p in (ROOT/'src').rglob('*.h'):
        dst=H/p.relative_to(ROOT);dst.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(p,dst)
    snapshot=read(ROOT/'config/units.json')
    excluded={n:u for n,u in snapshot['units'].items() if u['source'].startswith('src/hmi/')}
    snapshot['units']={n:u for n,u in snapshot['units'].items() if n not in excluded}
    save('excluded_hmi_reconstruction.json',excluded)
    save('units_snapshot.json',snapshot)
    save('symbols_snapshot.json',read(ROOT/'config/symbols.json'))
    result=[]
    for i,(name,u) in enumerate(units().items()):
        src=H/u['source'];src.parent.mkdir(parents=True,exist_ok=True)
        origin=ROOT/u['source']
        shutil.copyfile(origin,src)
        prof=wcc.profile(u['profile'])
        rc,output=wcc.build(src,H/'objects'/(name+'.obj'),prof['flags'],prof['install'])
        row=dict(unit=name,source=u['source'],sha256=sha(src.read_bytes()),profile=u['profile'],
                 flags=prof['flags'],rc=rc,text=output)
        result.append(row); record(dict(api='tools/wcc.py build',**row))
        assert rc==0, row
        if i%50==0:print('compiled',i+1,'/',len(units()),flush=True)
    save('compile.json',result)
    assets()
    print(f'compiled all accepted units and {len(asset_specs())} generated external assets',flush=True)

def audit_symbols():
    mods=modules();exports=collections.defaultdict(list)
    for n,m in mods.items():
        for raw,si,off,local in m.publics:
            if not local:exports[raw].append(dict(unit=n,segment=m.segments[si].name if si else None,off=off))
    for spec in read(H/'assets.json'):
        n='asset_'+spec['name'];m=omf.load(H/'objects'/(n+'.obj'))[0]
        for raw,si,off,local in m.publics:exports[raw].append(dict(unit=n,segment=m.segments[si].name,off=off))
    libs={}
    for p in libraries():
        rel=str(p)
        for m in omf.load(p):
            for raw,si,off,local in m.publics:
                if not local:libs.setdefault(raw,[]).append([rel,m.name])
    missing=collections.defaultdict(list)
    for n,m in mods.items():
        for raw in m.externs[1:]:
            if raw not in exports and raw not in libs:missing[raw].append(n)
    save('symbol_audit.json',dict(duplicates={n:v for n,v in exports.items() if len(v)>1},
         unresolved=dict(missing),library_hashes={str(p):sha(p.read_bytes()) for p in libraries()}))
    print('duplicate symbols',sum(len(v)>1 for v in exports.values()),'unresolved names',len(missing),flush=True)

def expected_segments(mods):
    result={}
    bind=match.Bindings(Image())
    man=manifest()
    code={};data={}
    def visit(e):
        if e.get('source') and 'current_TEXT_base' in e:code[e['source']]=e['current_TEXT_base']
        if e.get('source'):
            for x in e.get('extents',[]):
                if x['segment']!='_TEXT':data[e['source'],x['segment']]=x
        for key in ('current_slices','subunits'):
            for c in e.get(key,[]):
                if isinstance(c,dict):visit(c)
    for e in man['contributions']+man['reconstruction_slices']:visit(e)
    for n,m in mods.items():
        verified=[]
        match.verify_object(Image(),m,seg_report=verified)
        placed={x['segment']:x for x in verified if x.get('oracle') and x.get('status')=='VERIFIED'}
        for si,seg in enumerate(m.segments):
            if not seg or not seg.size:continue
            candidates=[]
            e=data.get((units()[n]['source'],seg.name))
            if e:candidates.append((e['object'],e['start']))
            if not candidates and seg.cls.upper()=='CODE':
                source=units()[n]['source']
                if source in code:candidates=[(1,code[source])]
            if not candidates:
                for raw,sidx,off,local in m.publics:
                    if sidx==si:
                        a,_=bind.lookup(raw)
                        if a:candidates.append((a[0],a[1]-off))
            if seg.cls.upper()=='CODE':
                votes={a[0:1]+(a[1]-off,) for raw,sidx,off,_ in m.publics if sidx==si for a in [bind.lookup(raw)[0]] if a}
                if len(votes)==1:candidates=list(votes)
            if not candidates and seg.name in placed:
                obj,off=placed[seg.name]['oracle'].split(':')
                candidates=[(int(obj),int(off,16))]
            if candidates:
                result[n,si]=dict(object=candidates[0][0],start=candidates[0][1],
                    base_votes=sorted(set(candidates)),ambiguous=len(set(candidates))!=1,
                    evidence='manifest verified segment/code anchor; code end remains an envelope')
    for spec in read(H/'assets.json'):
        n='asset_'+spec['name']
        m=omf.load(H/'objects'/(n+'.obj'))[0];mods[n]=m
        si=next(si for si,s in enumerate(m.segments) if s and s.size)
        result[n,si]=dict(object=2,start=spec['start'],ambiguous=False,evidence='authorized asset range')
    return result

def plan():
    mods=modules();expected=expected_segments(mods)
    # Enforce the manifest's game-code order and all located per-segment orders.
    # Archives remain libraries: their actual extraction order is measured afterward.
    edges={n:set() for n in mods}; reasons=[]; conflicts=[]
    def edge(a,b,why):
        if a==b:return
        def reaches(n,target,seen):
            if n==target:return True
            if n in seen:return False
            seen.add(n)
            return any(reaches(x,target,seen) for x in edges[n])
        if reaches(b,a,set()):
            conflicts.append([a,b,why]);return
        edges[a].add(b);reasons.append([a,b,why])
    man=manifest();source_names={u['source']:n for n,u in units().items()}
    code_order=[]
    for cid in man['class_orders']['_TEXT']:
        e=next(e for e in man['contributions'] if e['id']==cid)
        if cid=='game:lowlevel_assembly':srcs=[c['source'] for c in e['current_slices']]
        elif e.get('source'):
            srcs=[e['source']]+[c['source'] for c in e.get('subunits',[]) if isinstance(c,dict)]
        else:continue
        code_order.extend(source_names[s] for s in srcs if s in source_names)
    # Include code fragments not represented by a manifest envelope as explicit anchors.
    all_code=sorted((e['start'],n) for (n,si),e in expected.items() if mods[n].segments[si].cls.upper()=='CODE')
    missing=[n for _,n in all_code if n not in code_order]
    if missing:
        code_order=[n for _,n in all_code]
    for a,b in zip(code_order,code_order[1:]):edge(a,b,'manifest _TEXT envelope order')
    orders={}
    for segname in sorted({m.segments[si].name for n,m in mods.items() for nn,si in expected if nn==n}):
        rows=sorted((e['object'],e['start'],n,si) for (n,si),e in expected.items() if mods[n].segments[si].name==segname)
        orders[segname]=rows
        for a,b in zip(rows,rows[1:]):
            if a[0]==b[0]:edge(a[2],b[2],segname+' address order')
    incoming={n:0 for n in mods}
    for followers in edges.values():
        for n in followers:incoming[n]+=1
    # Tie-break only; no byte placement, absolute target or padding is prescribed.
    rank={n:min((e['start'] for (nn,si),e in expected.items() if nn==n),default=10**9) for n in mods}
    order=[];ties=[]
    while len(order)<len(mods):
        ready=sorted((n for n,v in incoming.items() if v==0 and n not in order),key=lambda n:(rank[n],n))
        if not ready:break
        if len(ready)>1:ties.append(ready)
        n=ready[0];order.append(n)
        for b in edges[n]:incoming[b]-=1
    save('input_plan.json',dict(order_conflicts=conflicts,order=order,cycle=[n for n in mods if n not in order],edges=reasons,
         nonunique_choices=ties,segment_orders=orders,manifest_sha256=sha(MANIFEST.read_bytes()),
         expected=[dict(unit=n,seg_index=si,segment=mods[n].segments[si].name,**e) for (n,si),e in expected.items()]))
    assert len(order)==len(mods),'source-level order conflict: see input_plan.json'
    return order

def aliases():
    bind=match.Bindings(Image());exports=collections.defaultdict(list)
    for n,m in modules().items():
        for raw,si,off,local in m.publics:
            a,_=bind.lookup(raw)
            if not local and a:exports[a].append(raw)
    result=[]
    # Also account for local reconstruction names of proven archive contributions.
    runtime_exports=collections.defaultdict(list)
    for e in manifest()['contributions']:
        if e['kind']!='runtime_archive_member':continue
        m=next(m for m in omf.load(e['library']) if m.name==e['module'])
        for raw,si,off,local in m.publics:
            if not si or local:continue
            ext=next((x for x in e['extents'] if x['segment']==m.segments[si].name),None)
            if ext:runtime_exports[(ext['object'],ext['start']+off)].append((raw,e['id']))
    for raw,consumers in read(H/'symbol_audit.json')['unresolved'].items():
        a,_=bind.lookup(raw);targets=exports.get(a,[])
        if len(targets)==1:result.append(dict(source=raw,target=targets[0],oracle=list(a),
             evidence='existing canonical bindings and actual OMF public at identical address',consumers=consumers))
        elif not targets and runtime_exports.get(a):
            choices=runtime_exports[a]
            rawtarget,member=min(choices,key=lambda v:(v[0].startswith('_'),len(v[0]),v[0]))
            result.append(dict(source=raw,target=rawtarget,oracle=list(a),consumers=consumers,
                         evidence='canonical binding + located pinned OMF public: '+member,
                         equivalent_publics=[v[0] for v in choices]))
    # PalFire is explicitly authorized; the current code names that same 48-byte asset pal.
    if '_pal' in read(H/'symbol_audit.json')['unresolved']:
        assert bind.lookup('_pal')[0]==(2,0x5fb2)
        result.append(dict(source='_pal',target='_PalFire',oracle=[2,0x5fb2],
                      evidence='canonical pal binding + docs/DATA.md PalFire asset interval'))
    # The startup call is rel32: independently resolve it in the oracle, not by spelling.
    ent=next(e for e in manifest()['contributions'] if e.get('module')=='cmain386')
    mod=next(m for m in omf.load(wcc.install('wc106')[1]/'LIB386/DOS/CLIB3R.LIB') if m.name=='cmain386')
    f=next(f for f in mod.fixups if mod.target_name(f)=='main_')
    base=next(e['start'] for e in ent['extents'] if e['segment']=='_TEXT')
    site=base+f.off
    a=(1,site+4+struct.unpack_from('<i',Image().code,site)[0])
    assert f.self_rel and a==(bind.lookup('main_')[0] or bind.lookup('main_Ray_')[0])
    if 'main_' not in {raw for m in modules().values() for raw,si,off,local in m.publics if not local}:
        result.append(dict(source='main_',target='main_Ray_',oracle=list(a),
                          evidence=f'pinned cmain386 rel32 at 1:{site:05x} resolves to canonical main_Ray'))
    # delay386 is a five-byte JMP veneer, too small for the generic identity scan.
    # Its opcode and independently located __delay_ target prove the current spelling.
    a=bind.lookup('wait_milliseconds_')[0]
    choices=next(key for key,vals in runtime_exports.items() if any(raw=='__delay_' for raw,_ in vals))
    site=a[1]+1;target=site+4+struct.unpack_from('<i',Image().code,site)[0]
    delay=next(m for m in omf.load(wcc.install('wc106')[1]/'LIB386/DOS/CLIB3R.LIB') if m.name=='delay386')
    seg=next(s for s in delay.segments if s and s.name=='_TEXT')
    assert seg.size==5 and bytes(seg.data[:1])==Image().code[a[1]:a[1]+1]==b'\xe9'
    assert choices==(1,target) and len(delay.fixups)==2
    result.append(dict(source='wait_milliseconds_',target='delay_',oracle=list(a),
                      evidence=f'delay386 5-byte JMP at 1:{a[1]:05x}; rel32 target 1:{target:05x} is located __delay_'))
    save('aliases.json',result)
    return result


def response_file(stub_link, module_order, library_paths, alias_rows, image_name,
                  diagnostic=False, omitted=()):
    """Render WLINK input deterministically; no address directives or fill bytes."""
    omitted=set(omitted) if diagnostic else set()
    lines=[stub_link.rstrip(), 'system pmodew', 'option stack=8192', 'option caseexact']
    lines.extend('file objects/'+name+'.obj' for name in module_order if name not in omitted)
    lines.append('option nodefaultlibs')
    lines.extend('library '+str(path) for path in library_paths)
    lines.extend('alias '+row['source']+'='+row['target'] for row in alias_rows)
    lines.extend(('name '+image_name+'.exe', 'option map='+image_name+'.map'))
    if diagnostic:
        lines.append('option undefsok')
    return '\n'.join(lines)+'\n'


def link_all():
    order=plan();als=aliases()
    tc=read(ROOT/'config/toolchain.json')['installs']['pmodew122']
    pmwbase=wcc.TOOLS_DIR/tc['dir']
    for name in ('PMODEW.EXE','PMWLITE.EXE'):
        b=(pmwbase/name).read_bytes();assert sha(b)==tc['key_files'][name]
        (H/name).write_bytes(b)
    # Declaration-only segment/group setup. No data bytes, code bytes, or padding.
    text=".386p\n_DATA segment para public use32 'DATA'\n_DATA ends\nCONST segment dword public use32 'DATA'\nCONST ends\nCONST2 segment dword public use32 'DATA'\nCONST2 ends\nDATA32 segment dword public use32 'DATA'\nDATA32 ends\nDGROUP group _DATA, CONST, CONST2, DATA32\nend\n"
    (H/'layout.asm').write_text(text,encoding='ascii')
    prof=wcc.profile('asm');rc,out=wcc.build(H/'layout.asm',H/'objects/layout.obj',prof['flags'],prof['install'])
    record(dict(api='tools/wcc.py build',source='layout.asm',rc=rc,text=out));assert rc==0,out
    stub_link=(pmwbase/'PMODEW.LNK').read_text(encoding='latin1')
    results={}
    for name in ('natural','diagnostic'):
        if name=='diagnostic':print('INCOMPLETE measurement image: UNDEFSOK; missing references are explicit gaps; never run this image.',flush=True)
        if name=='diagnostic':
            save('diagnostic_omissions.json',dict(units=[],reason='No units are omitted; UNDEFSOK only permits the explicitly reported unresolved references in the diagnostic image.'))
        rsp=response_file(stub_link,order,libraries(),als,name,name=='diagnostic',DIAGNOSTIC_OMISSIONS)
        (H/(name+'.lnk')).write_text(rsp,encoding='ascii')
        (H/(name+'.exe')).unlink(missing_ok=True)
        result=run([wcc.install('wc106')[1]/'BINNT/WLINK.EXE','@'+name+'.lnk'])
        results[name]=result
        print(name,'rc',result['rc'],'image exists',(H/(name+'.exe')).exists(),flush=True)
        (H/(name+'.log')).write_text(result['stdout']+result['stderr'],encoding='utf-8')
    save('link_results.json',results)

def le_model(b):
    """Decode LE without relocating, trimming, masking or rewriting image bytes."""
    def u32(at):return struct.unpack_from('<I',b,at)[0]
    base=u32(60);assert b[base:base+2]==b'LE'
    def h(at):return u32(base+at)
    objects=[];data=[];fixups={};records=[]
    for oi in range(h(0x44)):
        vs,linear,flags,first,count,_=struct.unpack_from('<6I',b,base+h(0x40)+24*oi)
        raw=bytearray();items=[]
        for local in range(count):
            entry=base+h(0x48)+4*(first-1+local)
            page=int.from_bytes(b[entry:entry+3],'big');assert b[entry+3]==0 and page>0
            length=h(0x2c) if page==h(0x14) else h(0x28)
            start=h(0x80)+(page-1)*h(0x28);raw+=b[start:start+length]
            a,z=struct.unpack_from('<II',b,base+h(0x68)+4*(first-1+local))
            pos,end=base+h(0x6c)+a,base+h(0x6c)+z
            while pos<end:
                begin=pos;typ,flg,src=struct.unpack_from('<BBh',b,pos);pos+=4
                assert typ==7 and flg & ~0x50==0,(typ,flg)
                size=2 if flg&0x40 else 1;tobj=int.from_bytes(b[pos:pos+size],'little');pos+=size
                size=4 if flg&0x10 else 2;toff=int.from_bytes(b[pos:pos+size],'little');pos+=size
                if toff&0x80000000:toff-=0x100000000
                f=dict(type=typ,src=local*4096+src,tobj=tobj,toff=toff)
                items.append(f);records.append(dict(object=oi+1,page=local,record_bytes=pos-begin,**f))
            assert pos==end
        physical=len(raw);raw=bytes(raw[:vs])
        objects.append(dict(virtual_size=vs,flags=flags,data_size=len(raw),linear_base=linear,
                            physical_size=physical,padding=physical-len(raw)))
        data.append(raw);fixups[oi+1]=items
    return dict(entry=list(struct.unpack_from('<4I',b,base+0x18)),objects=objects,data=data,fixups=fixups,records=records)

def map_model(text):
    segs={};module_rows=[];current=None
    for line in text.splitlines():
        parts=line.split()
        if len(parts) in (4,5) and re.fullmatch(r'[0-9a-fA-F]{4}:[0-9a-fA-F]{8}',parts[-2]) and re.fullmatch(r'[0-9a-fA-F]{8}',parts[-1]):
            obj,off=parts[-2].split(':');segs[parts[0]]=dict(object=int(obj,16),start=int(off,16),size=int(parts[-1],16),cls=parts[1])
        if line.startswith('Module: '):
            current=dict(label=line[8:],publics={});module_rows.append(current)
        elif current:
            m=re.match(r'^([0-9A-Fa-f]{4}):([0-9A-Fa-f]{8})[+* ]+([^\s]+)',line)
            if m:current['publics'][m[3]]=[int(m[1],16),int(m[2],16)]
    return segs,module_rows

def loaded_modules(maprows):
    mods=modules()
    for spec in read(H/'assets.json'):
        n='asset_'+spec['name'];mods[n]=omf.load(H/'objects'/(n+'.obj'))[0]
    mods['layout']=omf.load(H/'objects/layout.obj')[0]
    library_modules={}
    for lib in libraries():
        for m in omf.load(lib):library_modules[(lib.name.lower(),m.name.lower())]=m
    out=[]
    for row in maprows:
        label=row['label']
        if label.startswith('objects/') or label.startswith('objects\\'):
            name=re.split(r'[/\\]',label.split('(')[0])[-1][:-4]
            out.append((name,mods[name],row['publics']))
        else:
            mt=re.search(r'([^/\\]+\.lib)\(([^)]+)\)',label,re.I)
            assert mt,label
            key=(mt[1].lower(),mt[2].lower())
            out.append((('vendor:' if mt[1].lower()=='sosdw1cr.lib' else 'runtime:')+mt[1].upper()+':'+mt[2],library_modules[key],row['publics']))
    return out

def contribution_rows(candidate):
    segs,maprows=map_model((H/'diagnostic.map').read_text())
    loaded=loaded_modules(maprows)
    expected={(x['unit'],x['seg_index']):x for x in read(H/'input_plan.json')['expected']}
    runtime={x['id'].lower():x for x in manifest()['contributions'] if x['kind']=='runtime_archive_member'}
    cursors={s:e['start'] for s,e in segs.items()};rows=[];errors=[]
    aligns={1:1,2:2,3:16,4:256,5:4,6:4096}
    for n,m,publics in loaded:
        for si,s in enumerate(m.segments):
            if not s or s.name not in segs:continue
            global_seg=segs[s.name];obj=global_seg['object']
            alignment=aligns[s.align];pred=(cursors[s.name]+alignment-1)&-alignment
            # A zero-length contribution can still align the next contribution.
            if not s.size:
                cursors[s.name]=pred
                continue
            votes=set()
            for raw,ps,off,local in m.publics:
                if ps==si and raw in publics:
                    a=publics[raw];votes.add((a[0],a[1]-off))
            if len(votes)>1:errors.append(dict(unit=n,segment=s.name,problem='inconsistent map public bases',votes=sorted(votes)))
            actual=next(iter(votes))[1] if len(votes)==1 else pred
            if len(votes)==1 and (obj, pred)!=next(iter(votes)):
                errors.append(dict(unit=n,segment=s.name,predicted=[obj,pred],map=list(next(iter(votes)))))
            cursors[s.name]=actual+s.size
            exp=expected.get((n,si))
            if n.startswith('runtime:'):
                e=runtime.get(n.lower())
                if not e:
                    e=next((e for e in runtime.values() if any(a['id'].lower()==n.lower() for a in e.get('alternative_members',[]))),None)
                ext=next((x for x in e['extents'] if x['segment']==s.name),None) if e else None
                if ext:exp=dict(object=ext['object'],start=ext['start'],evidence='manifest located runtime member')
                if n.lower()=='runtime:clib3r.lib:delay386' and s.name=='_TEXT':
                    a=next(a for a in read(H/'aliases.json') if a['source']=='wait_milliseconds_')
                    exp=dict(object=1,start=a['oracle'][1],evidence=a['evidence'])
            if n.startswith('vendor:'):
                binds=match.Bindings(Image())
                anchors={(a[0],a[1]-off) for raw,ps,off,local in m.publics if ps==si
                         for a in [binds.lookup(raw)[0]] if a}
                receipts=[]
                match.verify_object(Image(),m,seg_report=receipts)
                if len(anchors)==1:
                    oo,aa=next(iter(anchors))
                    exp=dict(object=oo,start=aa,evidence='canonical SDK publics, pinned member offsets',base_votes=sorted(anchors))
                else:
                    receipt=next((r for r in receipts if r['segment']==s.name and r.get('status')=='VERIFIED'),None)
                    if receipt:
                        oo,aa=receipt['oracle'].split(':')
                        exp=dict(object=int(oo),start=int(aa,16),evidence='unchanged matcher whole-segment VERIFIED',receipt=receipt)
                    else:assert not s.size,('unplaced HMI member',n,s.name,anchors,receipts)
            row=dict(unit=n,segment=s.name,cls=s.cls,seg_index=si,size=s.size,alignment=alignment,
                     actual_object=obj,actual_start=actual,expected=exp,map_public_votes=len(votes),
                     fixups=[dict(**dataclasses.asdict(f),target_name=m.target_name(f)) for f in m.fixups if f.seg==si])
            if exp:
                row['placement_equal']=(obj,actual)==(exp['object'],exp['start'])
                row['placement_delta']=actual-exp['start'] if obj==exp['object'] else None
            if s.cls.upper() not in ('BSS','STACK') and exp:
                a,zero=virtual_slice(candidate,obj,actual,s.size)
                o,ozero=virtual_slice(oracle_cache,exp['object'],exp['start'],s.size)
                row['bytes']=byte_diff(o,a)
                row['actual_virtual_zero_bytes']=zero;row['oracle_virtual_zero_bytes']=ozero
                skip={i for f in m.fixups if f.seg==si for i in range(f.off,f.off+f.size())}
                row['nonfixup_diagnostic']=dict(equal=all(x==y for i,(x,y) in enumerate(zip(o,a)) if i not in skip) and len(o)==len(a),
                    first_diff=next((i for i,(x,y) in enumerate(zip(o,a)) if i not in skip and x!=y),None),
                    warning='diagnostic only; excludes every OMF fixup field; never STRICT')
            rows.append(row)
    save('map_validation.json',dict(errors=errors,segments=segs,segment_ends=cursors,
         loaded_modules=[n for n,_,_ in loaded],warning='map anchors checked against natural per-segment concatenation'))
    return rows

def virtual_slice(model,obj,start,size):
    """Load semantics only: explicitly identify virtual zero tail, never write it to inputs."""
    assert start>=0 and start+size<=model['objects'][obj-1]['virtual_size'],(obj,start,size)
    data=model['data'][obj-1];raw=data[start:start+size]
    missing=size-len(raw)
    return raw+b'\0'*missing,missing

def byte_diff(expected,actual):
    first=next((i for i,(a,b) in enumerate(zip(expected,actual)) if a!=b),None)
    if first is None and len(expected)!=len(actual):first=min(len(expected),len(actual))
    return dict(equal=expected==actual,expected_size=len(expected),actual_size=len(actual),first_diff=first,
                different_bytes=sum(a!=b for a,b in zip(expected,actual))+abs(len(expected)-len(actual)),
                expected_sha256=sha(expected),actual_sha256=sha(actual),
                expected_hex=expected[first:first+16].hex() if first is not None else '',
                actual_hex=actual[first:first+16].hex() if first is not None else '')

def list_diff(expected,actual):
    at=next((i for i,(a,b) in enumerate(zip(expected,actual)) if a!=b),None)
    if at is None and len(expected)!=len(actual):at=min(len(expected),len(actual))
    return dict(equal=expected==actual,expected_count=len(expected),actual_count=len(actual),first_diff=at,
                expected=expected[at] if at is not None and at<len(expected) else None,
                actual=actual[at] if at is not None and at<len(actual) else None)

oracle_cache=None
def report_all():
    global oracle_cache
    oracle_cache=oracle();o=oracle_cache
    c=le_model((H/'diagnostic.exe').read_bytes())
    rows=contribution_rows(c)
    save('contribution_diff.json',rows)
    metadata=dict(scope='All game units plus original HMI library; UNDEFSOK measurement retains any unresolved names; never executed or claimed matching',
       entry_expected=o['entry'],entry_actual=c['entry'],objects_expected=o['objects'],objects_actual=c['objects'],
       object_bytes=[byte_diff(d,c['data'][i] if i<len(c['data']) else b'') for i,d in enumerate(o['data'])])
    save('image_diff.json',metadata)
    relocation={str(i):list_diff(o['fixups'].get(i,[]),c['fixups'].get(i,[])) for i in range(1,max(len(o['objects']),len(c['objects']))+1)}
    matched=[]
    for row in rows:
        if not row.get('bytes',{}).get('equal'):continue
        e=row['expected'];start=row['actual_start'];end=start+row['size']
        # Coordinates remain intact; filtering retains actual output traversal order and duplicates.
        old=[f for f in o['fixups'][e['object']] if e['start']<=f['src']<e['start']+row['size']]
        new=[f for f in c['fixups'][row['actual_object']] if start<=f['src']<end]
        matched.append(dict(unit=row['unit'],segment=row['segment'],placement_equal=row['placement_equal'],
                            expected_start=e['start'],actual_start=start,ordered=list_diff(old,new)))
    save('relocation_diff.json',dict(objects=relocation,byte_equal_contributions=matched,
         warning='ordered tuples retain duplicates; moved-region coordinates are not normalized'))
    # Class-wide comparisons use original coordinates, then contribution records explain drift.
    class_rows={}
    const2_start=min(e['start'] for x in manifest()['contributions'] for e in x['extents'] if e['segment']=='CONST2')
    const_start=min(r['expected']['start'] for r in rows if r['segment']=='CONST' and r.get('expected') and r['expected']['object']==2)
    ranges={'_TEXT':(1,0,0x7d4c2),'_DATA':(2,0x10,min(r['expected']['start'] for r in rows if r['segment']=='CONST' and r.get('expected') and r['expected']['object']==2)),'CONST':(2,const_start,const2_start),
            'CONST2':(2,const2_start,0x31ec4),'DATA32':(2,0x31ec4,0x32190)}
    for name,(obj,a,z) in ranges.items():
        group=[r for r in rows if r['segment']==name and r.get('expected') and r['expected']['object']==obj]
        group.sort(key=lambda r:(r['expected']['object'],r['expected']['start']))
        bad=next((r for r in group if not r.get('placement_equal') or (r.get('bytes') and not r['bytes']['equal'])),None)
        class_rows[name]=dict(oracle_range=[obj,a,z],absolute_bytes=byte_diff(o['data'][obj-1][a:z],c['data'][obj-1][a:z]),
            first_differing_contribution={k:v for k,v in bad.items() if k not in ('fixups',)} if bad else None)
        if name=='CONST':class_rows[name]['scope']='Starts at first located CONST contribution; unowned leading literals are separately inventoried as gaps.'
    bss=[r for r in rows if r['cls'].upper()=='BSS' and r.get('expected')]
    class_rows['_BSS']=dict(layout_only=True,first_differing_contribution=next((r for r in sorted(bss,key=lambda r:r['expected']['start']) if not r['placement_equal']),None))
    save('class_diff.json',class_rows)
    lines=['# Per-contribution diagnostic diff','',metadata['scope'],'',
           '| Unit | Segment | Oracle start | Linked start | Size | First byte diff | Placement delta |',
           '|---|---|---|---|---:|---:|---:|']
    for r in rows:
        e=r.get('expected');ex=f"{e['object']}:{e['start']:05x}" if e else 'OPEN'
        lines.append(f"| {r['unit']} | {r['segment']} | {ex} | {r['actual_object']}:{r['actual_start']:05x} | {r['size']} | {r.get('bytes',{}).get('first_diff','layout/open')} | {r.get('placement_delta','')} |")
    (H/'contribution_diff.md').write_text('\n'.join(lines)+'\n',encoding='utf-8')
    gap_reports(rows,c)
    print('reports:',len(rows),'contributions;',sum(r.get('bytes',{}).get('equal',False) for r in rows),'byte equal; map validation errors',len(read(H/'map_validation.json')['errors']),flush=True)

def gap_reports(rows,c):
    o=oracle_cache;bind=match.Bindings(Image())
    canonical=read(ROOT/'config/symbols.json')['gog_v121_us']
    log=(H/'diagnostic.log').read_text()
    unresolved=sorted(set(re.findall(r'Warning\(1028\): (\S+) is an undefined reference',log)))
    raw_consumers=read(H/'symbol_audit.json')['unresolved']
    missing=[]
    for raw in unresolved:
        a,_=bind.lookup(raw);entry=canonical['data'].get(match.demangle(raw),canonical['functions'].get(match.demangle(raw),{}))
        covering=[]
        if a:
            covering=[r['unit']+':'+r['segment'] for r in rows if r.get('expected') and r['expected']['object']==a[0]
                      and r['expected']['start']<=a[1]<r['expected']['start']+r['size']]
        if covering:category='existing contribution, missing exported label/spelling'
        elif raw.startswith('_dt_str_'):category='missing initialized string contribution'
        elif a and a[0]==2 and a[1]>=0x32190:category='missing BSS contribution'
        elif raw.startswith('sub_7') and 'hifi' in raw_consumers.get(raw,[]):category='missing sound-driver code'
        else:category='missing contribution or unresolved identity'
        missing.append(dict(raw=raw,oracle=list(a) if a else None,size=entry.get('size'),
                            consumers=raw_consumers.get(raw,[]),covering=covering,category=category))
    undefined_sites=[]
    for r in rows:
        for f in r['fixups']:
            if f['target_name'] not in unresolved:continue
            site=r['actual_start']+f['off'];width=(4 if f.get('dialect')=='borland-pharlap32' and f['loc']==5 else omf.LOC_SIZE[f['loc']])
            undefined_sites.append(dict(unit=r['unit'],segment=r['segment'],raw=f['target_name'],
                 object=r['actual_object'],site=site,width=width,self_relative=f['self_rel'],
                 linked_field_hex=c['data'][r['actual_object']-1][site:site+width].hex(),
                 warning='UNRESOLVED linker field; placeholder/addend is not a valid resolved reference'))
    save('undefined_sites.json',undefined_sites)
    def gaps(spans,lo,hi):
        out=[];at=lo
        for a,z in sorted(spans):
            a=max(a,lo);z=min(z,hi)
            if z<=a:continue
            if a>at:out.append([at,a])
            at=max(at,z)
        if at<hi:out.append([at,hi])
        return out
    init_spans=[(r['expected']['start'],r['expected']['start']+r['size']) for r in rows if r.get('expected')
                and r['expected']['object']==2 and r['cls'].upper() not in ('BSS','STACK')]
    bss_spans=[(r['expected']['start'],r['expected']['start']+r['size']) for r in rows if r.get('expected')
               and r['expected']['object']==2 and r['cls'].upper()=='BSS' and not r['unit'].startswith(('runtime:','vendor:'))]
    init_gaps=[]
    for a,z in gaps(init_spans,0,len(o['data'][1])):
        init_gaps.append(dict(object=2,start=a,end=z,size=z-a,nonzero_oracle_bytes=sum(x!=0 for x in o['data'][1][a:z]),
                       classification='OPEN; may include natural alignment/CRT markers, never synthesized'))
    bss_gaps=gaps(bss_spans,0x32190,0x57478)
    expected_runtime={x['id'] for x in manifest()['contributions'] if x['kind']=='runtime_archive_member'}
    selected={r['unit'] for r in rows if r['unit'].startswith('runtime:')}
    selected_lower={x.lower() for x in selected};expected_lower={x.lower() for x in expected_runtime}
    order=read(H/'map_validation.json')['loaded_modules']
    actual_code=[n for n in order if any(r['unit']==n and r['cls'].upper()=='CODE' for r in rows) and n.startswith('runtime:')]
    expected_code=[x for x in manifest()['class_orders']['_TEXT'] if x.startswith('runtime:')]
    common=set(x.lower() for x in actual_code)&set(x.lower() for x in expected_code)
    runtime_order=list_diff([x.lower() for x in expected_code if x.lower() in common],
                           [x.lower() for x in actual_code if x.lower() in common])
    save('gaps.json',dict(unresolved=missing,unresolved_count=len(missing),category_counts=dict(collections.Counter(x['category'] for x in missing)),
         initialized_uncovered=init_gaps,game_BSS_uncovered=bss_gaps,game_BSS_uncovered_bytes=sum(z-a for a,z in bss_gaps),
         expected_runtime_not_selected=sorted(x for x in expected_runtime if x.lower() not in selected_lower),
         selected_runtime_not_located_by_manifest=sorted(x for x in selected if x.lower() not in expected_lower),
         unknown_expected_segments=[[r['unit'],r['segment'],r['size']] for r in rows if not r.get('expected')],
         runtime_order=runtime_order,code_unassigned_envelopes=manifest()['gaps']['code_unassigned_envelopes'],
         warning='Uncovered intervals are debt, including OPEN alignment/ownership; no oracle fallback or fabricated contributions.'))
    # Exact propagation of start drift, without treating a NEAR compiled end as an oracle end.
    game=[r for r in rows if r['cls'].upper()=='CODE' and r.get('expected') and not r['unit'].startswith(('runtime:','vendor:'))]
    game.sort(key=lambda r:r['expected']['start']);chain=[]
    for a,b in zip(game,game[1:]):
        shift=b['placement_delta']-a['placement_delta']
        cause='current TU size/alignment vs original address interval; NEAR size drift when interval ownership is correct'
        chain.append(dict(unit=a['unit'],next_unit=b['unit'],oracle_spacing=b['expected']['start']-a['expected']['start'],
                 natural_spacing=b['actual_start']-a['actual_start'],change_in_drift=shift,cause=cause))
    segs=read(H/'map_validation.json')['segments']
    accounting={name:dict(linked_size=e['size'],omf_size_sum=sum(r['size'] for r in rows if r['segment']==name),
                    natural_alignment_bytes=e['size']-sum(r['size'] for r in rows if r['segment']==name)) for name,e in segs.items()}
    save('drift_accounting.json',dict(code_chain=chain,classes=accounting,
          BSS=dict(original_game_span=0x57478-0x32190,supplied_game_bytes=sum(r['size'] for r in rows if r['cls'].upper()=='BSS' and not r['unit'].startswith(('runtime:','vendor:'))),
                   missing_game_bytes=sum(z-a for a,z in bss_gaps),original_total_span=0x5795c-0x32190,
                   candidate_span=segs['_BSS']['size'],candidate_alignment_bytes=accounting['_BSS']['natural_alignment_bytes']),
          warning='Natural alignment is linker-generated and measured; it is never inserted to hide gaps.'))

def pack_all():
    assert (H/'diagnostic.exe').exists()
    tc=read(ROOT/'config/toolchain.json')['installs']['pmodew122']
    for name in ('PMODEW.EXE','PMWLITE.EXE'):
        assert sha((H/name).read_bytes())==tc['key_files'][name]
        assert sha((wcc.TOOLS_DIR/tc['dir']/name).read_bytes())==tc['key_files'][name]
    (H/'packed.exe').unlink(missing_ok=True)
    r=run([wcc.TOOLS_DIR/'nmlgcdos/msdos.exe',H/'PMWLITE.EXE','/C4',
           '/S'+str(H/'PMODEW.EXE'),'/Dpacked.exe','diagnostic.exe'],timeout=60)
    save('pack_result.json',r)
    assert r['rc']==0 and (H/'packed.exe').exists(),r
    b=(H/'packed.exe').read_bytes();p=pmw1.PMW1(b);fx,_=p.fixups();c=le_model((H/'diagnostic.exe').read_bytes())
    data=[d for _,d,_ in p.object_data()]
    checks=dict(entry=[p.cs_obj,p.eip,p.ss_obj,p.esp]==c['entry'],
        data=data==c['data'],fixups=fx==c['fixups'],
        virtual_sizes=[x['virtual_size'] for x in p.objects]==[x['virtual_size'] for x in c['objects']],
        flags=[x['flags'] for x in p.objects]==[x['flags'] for x in c['objects']])
    save('pack_roundtrip.json',dict(scope='diagnostic UNDEFSOK image only; never executed',checks=checks,
          packed_size=len(b),packed_sha256=sha(b),oracle_equal=sha(b)==read(ROOT/'config/oracles.json')['oracles']['gog_v121_us']['sha256']))
    assert all(checks.values()),checks
    print('PMWLITE /C4 packed diagnostic image:',len(b),'bytes; decode round trip passed',flush=True)

def main():
    global H,MANIFEST
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('stage',nargs='?',default='all',choices=['compile','audit','link','report','pack','all'])
    ap.add_argument('--out',type=Path,default=H,help='generated output directory (defaults to build/link)')
    ap.add_argument('--manifest',type=Path,default=None,help='manifest path (defaults to <out>/manifest.json)')
    args=ap.parse_args()
    H=args.out.resolve();MANIFEST=(args.manifest or (H/'manifest.json')).resolve()
    H.mkdir(parents=True,exist_ok=True);wcc.BUILD=H
    # Write guard only for command-line runs: importing this module (tests) must not restrict the process.
    sys.addaudithook(guard)
    # A link is reproducible only against the exact config, verifier report,
    # source files, oracle and pinned libraries used to build its manifest.
    for path,want in manifest()['inputs'].items():
        p=Path(path)
        assert p.is_file(),('manifest input is missing',path)
        got=sha(p.read_bytes())
        assert got==want,('stale manifest input',path,want,got)
    stages=dict(compile=compile_all,audit=audit_symbols,link=link_all,report=report_all,pack=pack_all)
    if args.stage=='all':
        for name,fn in stages.items():
            if name!='assets':fn()
        if read(H/'link_results.json')['natural']['rc']:
            print('FULL NATURAL LINK FAILED; diagnostic measurements and packing completed. No match/runnability claim.',flush=True)
            return 1
    else:
        stages[args.stage]()
        if args.stage=='link' and read(H/'link_results.json')['natural']['rc']:return 1
    return 0

if __name__=='__main__':sys.exit(main())
