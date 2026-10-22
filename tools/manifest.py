"""Generate the evidence manifest consumed by the natural WLINK pipeline.

Run from the repository root after `python tools/verify.py`:
    python -B tools/manifest.py [--out build/link] [--verify-dir build/verify]

The manifest is an evidence inventory and partial order, not a claim that the
original linker inputs have been recovered. Ambiguous ownership stays explicit.
"""
import argparse
import collections
import hashlib
import json
import os
from pathlib import Path
import re
import struct
import sys

sys.dont_write_bytecode = True
SCRIPT_DIR = Path(__file__).resolve().parent
ROOT = next(p for p in SCRIPT_DIR.parents if (p / 'config/toolchain.json').is_file())
DEFAULT_OUT = ROOT / 'build/link' if SCRIPT_DIR.name == 'tools' else SCRIPT_DIR / 'build/link'
H = DEFAULT_OUT.resolve()
VERIFY_DIR = ROOT / 'build/verify'
sys.path.insert(0, str(ROOT / 'tools'))
from common import TOOLS_DIR
import omf
import libscan
import datamap
import tumap
from image import Image


def guard(event, args):
    paths = []
    if event == 'open':
        p, mode, flags = args
        if flags & (os.O_WRONLY | os.O_RDWR | os.O_CREAT | os.O_TRUNC | os.O_APPEND):
            paths = [p]
    elif event in ('os.mkdir', 'os.remove', 'os.rmdir', 'os.chmod', 'os.utime'):
        paths = args[:1]
    elif event in ('os.rename', 'os.replace'):
        paths = args[:2]
    for p in paths:
        if isinstance(p, (str, bytes)):
            assert Path(p).resolve().is_relative_to(H), (event, p)


_guard_installed = False


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def save(name, value):
    (H / name).write_text(json.dumps(value, indent=2) + '\n', encoding='utf-8')


def validate_manifest(document):
    required = {'schema', 'oracle', 'oracle_sha256', 'inputs', 'contributions',
                'proof_scope', 'extents_half_open', 'pinned_libraries', 'class_contract',
                'class_orders', 'order_edges', 'candidate_topological_order', 'order_ties',
                'order_cycle_members', 'reconstruction_slices', 'data_objects', 'const_runs',
                'bss_rank_blocks', 'relative_order_checks', 'string_order_inversions',
                'runtime_conflicting_votes', 'gaps', 'central_TUs',
                'excluded_hmi_reconstruction', 'open'}
    missing = required - document.keys()
    if missing:
        raise ValueError(f"manifest missing required keys: {sorted(missing)}")
    if document['schema'] != 1 or document['oracle'] != 'gog_v121_us':
        raise ValueError('unsupported natural-link manifest schema or oracle')
    if (not isinstance(document['inputs'], dict) or not isinstance(document['contributions'], list)
            or not isinstance(document['reconstruction_slices'], list)
            or not isinstance(document['pinned_libraries'], list)
            or not isinstance(document['class_orders'], dict)):
        raise ValueError('manifest inputs/contributions have invalid types')
    if not re.fullmatch(r'[0-9a-f]{64}', document['oracle_sha256']):
        raise ValueError('manifest oracle SHA-256 is malformed')
    for library in document['pinned_libraries']:
        if not all(k in library for k in ('kind', 'path', 'sha256')):
            raise ValueError('pinned library needs kind, path, and SHA-256')
        if not re.fullmatch(r'[0-9a-f]{64}', library['sha256']):
            raise ValueError(f"malformed library SHA-256: {library['path']}")
    for item in document['contributions'] + document['reconstruction_slices']:
        if not isinstance(item.get('id'), str) or not isinstance(item.get('extents', []), list):
            raise ValueError('manifest contribution needs an id and extent list')
        for extent_row in item.get('extents', []):
            if not all(k in extent_row for k in ('object', 'start', 'end', 'size', 'segment')):
                raise ValueError(f"incomplete extent in {item['id']}")
            if extent_row['end'] - extent_row['start'] != extent_row['size']:
                raise ValueError(f"inconsistent extent size in {item['id']}")
    return document

def addr(v):
    a,b=v.split(':');return int(a),int(b,16)
def extent(obj,start,size,segment,level,evidence,ambiguous=False,**kw):
    return dict(object=obj,start=start,end=start+size,size=size,segment=segment,
                level=level,evidence=evidence,ambiguous=ambiguous,**kw)
def bounds(v): return f"{v['object']}:{v['start']:05x}..{v['end']:05x}"
def gaps(intervals,lo,hi):
    out=[];p=lo
    for a,b in sorted(intervals):
        a=max(a,lo);b=min(b,hi)
        if b<=a:continue
        if a>p:out.append([p,a])
        p=max(p,b)
    if p<hi:out.append([p,hi])
    return out

def runtime(img,libs):
    modules=[]; exports=collections.defaultdict(list);nodes={}; votes=collections.defaultdict(list)
    for lib in libs:
        for idx,m in enumerate(omf.load(lib)):
            mi=len(modules);modules.append((lib,idx,m))
            for n,si,off,local in m.publics:
                if not local and si:exports[n].append((mi,si,off))
    def vote(mi,si,obj,base,ev):
        node=(mi,si)
        if base<0 or base+modules[mi][2].segments[si].size>(len(img.obj[obj]) if obj==1 else 0x59960):return
        v=(obj,base)
        if v not in [x[0] for x in votes[node]]:votes[node].append((v,ev))
        if len(votes[node])==1:nodes[node]=v
        else:nodes.pop(node,None)
    def bytes_at(mi,si,obj,base):
        seg=modules[mi][2].segments[si]
        if seg.cls.upper()=='BSS':return True
        if base<0 or base+seg.size>(len(img.code) if obj==1 else 0x59960):return False
        pat,mask,_=libscan.masked_pattern(modules[mi][2],si)
        image=img.obj[obj]
        return all(not mask[i] or (image[base+i] if base+i<len(image) else 0)==pat[i] for i in range(seg.size))
    rejected=[]
    # Same matcher as tools/libscan.py, all hits and limitations retained.
    hits=libscan.scan(img,[str(p) for p in libs])
    save('runtime_code_scan.json',hits)
    byname={(str(lib),m.name):mi for mi,(lib,idx,m) in enumerate(modules)}
    for h in hits:
        identical=[x for x in hits if (x['lib'],x['module'],x['seg'])==(h['lib'],h['module'],h['seg'])]
        if h['class']!='exact+sites' or len(identical)!=1:continue
        mi=byname[(h['lib'],h['module'])];m=modules[mi][2]
        sis=[si for si,seg in enumerate(m.segments) if seg and seg.name==h['seg'] and seg.size==h['size']]
        if len(sis)==1:vote(mi,sis[0],1,h['at'],'tools/libscan.py unique exact+sites (targets not certified)')
    # Anchors are filtered later by cross-segment consistency; masked anchors
    # alone never certify a runtime member or its fixup reference targets.
    anchors=datamap.runtime_anchors(img,[str(p) for p in libs])
    save('runtime_data_scan.json',anchors)
    for a in anchors:
        for mi,(lib,idx,m) in enumerate(modules):
            if m.name!=a['module']:continue
            for si,seg in enumerate(m.segments):
                if seg and seg.name==a['segment'] and seg.size==a['size']:
                    vote(mi,si,2,a['at'],'tools/datamap.py unique masked data anchor')
    # References from located contributions establish external-public/segment
    # bases, including data-only archive members and runtime BSS. Every vote kept.
    for iteration in range(12):
        before=dict(nodes)
        for (mi,si),(obj,base) in list(nodes.items()):
            m=modules[mi][2];seg=m.segments[si]
            if seg.cls.upper()=='BSS':continue
            for f in m.fixups:
                if f.seg!=si or f.size()!=4 or base+f.off+4>len(img.obj[obj]):continue
                src=base+f.off
                if f.self_rel:
                    if obj!=1 or (obj,src) in img.fixups:continue
                    tobj=1;toff=src+4+struct.unpack_from('<i',img.obj[obj],src)[0]
                else:
                    actual=img.fixups.get((obj,src))
                    if not actual or actual[0]!=7:continue
                    _,tobj,toff=actual
                field=struct.unpack_from('<i',seg.data,f.off)[0]
                method,index=f.target;method &=3
                targets=[]
                if method==0:targets=[(mi,index,0)]
                elif method==2:targets=exports.get(m.externs[index],[])
                plausible=[t for t in targets if bytes_at(t[0],t[1],tobj,toff-field-f.disp-t[2])]
                if len(plausible)!=1:
                    if targets and not plausible:rejected.append(dict(source=m.name,site=f'{obj}:{src:05x}',
                        target=m.target_name(f),reason='referenced member candidate fails nonfixup byte check'))
                    continue
                tm,ts,puboff=plausible[0]
                vote(tm,ts,tobj,toff-field-f.disp-puboff,
                     f'OMF symbolic reference {m.name}:{seg.name}+{f.off:x} at {obj}:{src:05x}')
        if nodes==before:break
    # Tiny XI/YI and CONST2 contributions have too few literal bytes for the
    # generic anchor scan. Resolve their OMF targets first, then search matching
    # oracle reference tuples and check the complete segment/site set.
    bytarget=collections.defaultdict(list)
    for (o,a),(typ,to,off) in img.fixups.items():bytarget[(to,off)].append((o,a))
    for mi,(lib,idx,m) in enumerate(modules):
        for si,seg in enumerate(m.segments):
            if not seg or not seg.size or (mi,si) in nodes or seg.cls.upper() in ('BSS','STACK'):continue
            fxs=[f for f in m.fixups if f.seg==si and not f.self_rel and f.size()==4]
            known=[]
            for f in fxs:
                method,index=f.target;method &=3; targets=[]
                if method==0:targets=[(mi,index,0)]
                elif method==2:targets=exports.get(m.externs[index],[])
                values=set()
                for tm,ts,poff in targets:
                    if (tm,ts) in nodes:
                        to,tbase=nodes[(tm,ts)]
                        field=struct.unpack_from('<i',seg.data,f.off)[0]
                        values.add((to,tbase+poff+field+f.disp))
                if len(values)==1:known.append((f,next(iter(values))))
            if not known:continue
            f,value=known[0];candidates=[]
            for obj,src in bytarget[value]:
                base=src-f.off
                if obj!=(1 if seg.cls.upper()=='CODE' else 2) or not bytes_at(mi,si,obj,base):continue
                want={base+fx.off for fx in fxs}
                have={a for o,a in img.fixups if o==obj and base<=a<base+seg.size}
                if want!=have:continue
                if any(img.fixups.get((obj,base+fx.off),(None,None,None))[1:]!=v for fx,v in known):continue
                candidates.append((obj,base))
            if len(candidates)==1:
                obj,base=candidates[0]
                vote(mi,si,obj,base,'OMF symbolic targets + whole-segment bytes/site set, unique reverse reference search')
    entries=[];conflicts=[]
    for mi,(lib,idx,m) in enumerate(modules):
        located=[(si,nodes[(mi,si)]) for si,seg in enumerate(m.segments) if seg and seg.size and (mi,si) in nodes]
        if not located:continue
        ext=[]
        for si,(obj,base) in located:
            seg=m.segments[si]
            evidence=[v[1] for v in votes[(mi,si)]]
            pat,mask,_=libscan.masked_pattern(m,si)
            byte_ok = bytes_at(mi,si,obj,base)
            bss=seg.cls.upper()=='BSS'
            ext.append(extent(obj,base,seg.size,seg.name,'STRONG' if bss else 'PROVEN' if byte_ok else 'OPEN',evidence,
                  ambiguous=not (byte_ok or bss),omf_class=seg.cls,omf_align=seg.align,
                  nonfixup_bytes_equal=byte_ok,fixup_targets_certified=False,
                  virtual_zero_tail=max(0,base+seg.size-len(img.obj[obj])) if not bss else 0,
                  coverage='BSS extent inferred by symbolic references' if bss else 'library identity evidence'))
        # Ignore masked-only constant coincidences from unlinked alternatives.
        selected=any(any('OMF symbolic' in e for e in x['evidence']) or (x['object']==1) for x in ext)
        if not selected:continue
        unplaced=[dict(segment=seg.name,size=seg.size,cls=seg.cls,alignment=seg.align)
                  for si,seg in enumerate(m.segments) if seg and seg.size and (mi,si) not in nodes]
        vendor=PathName(lib).lower()=='sosdw1cr.lib'
        entries.append(dict(id=('vendor:' if vendor else 'runtime:')+PathName(lib)+':'+m.name,
               kind='vendor_archive_member' if vendor else 'runtime_archive_member',
               library=str(lib),archive_index=idx,module=m.name,extents=ext,unplaced_segments=unplaced,
               ambiguous=bool(unplaced) or any(x['ambiguous'] for x in ext),
               evidence=['Pinned OMF member + unique code/data evidence and symbolic reference propagation'],
               order_basis='per-segment address constraints; zero-code members do not have a code rank'))
    for node,vs in votes.items():
        if len(vs)>1:
            mi,si=node;conflicts.append(dict(module=modules[mi][2].name,segment=modules[mi][2].segments[si].name,votes=vs))
    save('runtime_rejected_candidates.json',rejected)
    # Identical member implementations in different archives, or atoi/atol,
    # cannot be counted twice just because the identity scan finds both.
    merged=[];seen={}
    entries.sort(key=lambda e:-sum(x['object']==1 for x in e['extents']))
    for e in entries:
        code=tuple((x['object'],x['start'],x['end']) for x in e['extents'] if x['object']==1)
        primary=seen.get(code) if code else None
        if primary is None:
            for p in merged:
                pe={(x['object'],x['segment'],x['start'],x['end']) for x in p['extents']}
                ee={(x['object'],x['segment'],x['start'],x['end']) for x in e['extents']}
                if ee and ee<=pe:
                    if p['module']==e['module']:primary=p;break
                    if not code and any(x['object']==1 for x in p['extents']):
                        p.setdefault('data_anchor_collisions',[]).append(dict(id=e['id'],reason='Same data occurs in variant archive member; code identity not established'))
                        primary=False;break
        if primary is False:continue
        if primary is not None:
            prior=primary
            prior.setdefault('alternative_members',[]).append(dict(id=e['id'],library=e['library'],module=e['module'],archive_index=e['archive_index']))
            prior['ambiguous']=True
            prior['evidence'].append('Identical whole code extent admits alternative archive identities; not two contributions')
        else:
            merged.append(e)
            if code:seen[code]=e
    return merged,conflicts

def PathName(p):return p.name

def generate(out=None, verify_dir=None):
    global H, VERIFY_DIR, _guard_installed
    if out is not None:
        H = Path(out).resolve()
    if verify_dir is not None:
        VERIFY_DIR = Path(verify_dir).resolve()
    H.mkdir(parents=True, exist_ok=True)
    if not _guard_installed:
        sys.addaudithook(guard)
        _guard_installed = True
    cfg=json.loads((ROOT/'config/oracles.json').read_text(encoding='utf-8'))['oracles']['gog_v121_us']
    assert sha(ROOT/cfg['source']['path'])==cfg['sha256']
    img=Image(); all_units=json.loads((ROOT/'config/units.json').read_text(encoding='utf-8'))['units']
    excluded_hmi={n:u for n,u in all_units.items() if u['source'].startswith('src/hmi/')}
    units={n:u for n,u in all_units.items() if n not in excluded_hmi}
    rp=VERIFY_DIR/'report.json'
    if not rp.is_file():
        raise SystemExit(f"{rp} is missing; run `python tools/verify.py` first")
    report=json.loads(rp.read_text(encoding='utf-8'))
    entries=[];fragments=[];game=[]
    # Re-run tumap in memory: the CLI normally writes canonical build/oracles.
    xref=json.loads((ROOT/'build/xref/xref.json').read_text(encoding='utf-8'))
    funcs={int(r['dos'].split(':')[1],16):r for r in xref}
    libsyms=json.loads((ROOT/'build/oracles/gog_v121_us/libsyms.json').read_text(encoding='utf-8'))
    anchors=set(funcs)|{img.entry[1]}|{addr(v['addr'])[1] for v in libsyms.values()}
    starts=tumap.boundaries(img.code,set(img.code_fixup_sites),anchors)
    tu=[dict(start=x,end=starts[i+1] if i+1<len(starts) else len(img.code)) for i,x in enumerate(starts)]
    save('tumap.json',tu)
    (H/'tumap_list.txt').write_text('\n'.join(f"1:{t['start']:05x}..1:{t['end']:05x}" for t in tu)+'\n')
    for name,u in units.items():
        results=[r for r in report.get(name,[]) if r.get('oracle')]
        segs=report.get('_segments',{}).get(name,[])
        ext=[]
        for seg in segs:
            if not seg.get('oracle'):continue
            obj,a=addr(seg['oracle'])
            ext.append(extent(obj,a,seg['size'],seg['segment'],'PROVEN' if seg['status']=='VERIFIED' else 'OPEN',
                    [f'Fresh tools/verify.py report _segments.{name}: {seg["status"]}'],
                    ambiguous=True,coverage='verified reconstruction slice; original TU ownership unproven',status=seg['status']))
        if results:
            first=min(results,key=lambda x:x['seg_offset'])
            start=addr(first['oracle'])[1]-first['seg_offset']
            prefix=min([start]+[x['start'] for x in ext if x['object']==1])
            m=omf.load(VERIFY_DIR/name/(Path(u['source']).stem+'.obj'))[0]
            text=next(seg for seg in m.segments if seg and seg.name=='_TEXT')
            maximum=max(addr(r['oracle'])[1] for r in results)
            game.append(dict(id='game:'+name,kind='game_code_candidate',source=u['source'],start=prefix,
                current_TEXT_base=start,code_object_prefix_bytes=start-prefix,
                last_bound_public=maximum,current_text_size=text.size,
                extents=ext,ambiguous=True,evidence=[f'First bound public {first["function"]}: oracle minus OMF segment offset',
                  'config/units.json, fresh verifier and OMF; original end not inferred from NEAR compiled size'],
                strict_functions=sum(r['status']=='STRICT' for r in results),
                current_functions=len(results),boundary_level='STRONG' if start in starts else 'HYPOTHESIS'))
        else:
            fragments.append(dict(id='slice:'+name,kind='reconstruction_slice',source=u['source'],extents=ext,
               ambiguous=True,evidence=['Accepted data/BSS source; NOT a claim of an original link module']))
    game.sort(key=lambda x:x['start'])
    # Current assembly files are accepted slices. Their individual boundaries are
    # not historical object boundaries, and tumap's zero heuristic splits ASM.
    asm=[x for x in game if x['source'].endswith('.asm')]
    game=[x for x in game if not x['source'].endswith('.asm')]
    entries.append(dict(id='game:lowlevel_assembly',kind='assembly_module_cluster',ambiguous=True,
       evidence=['docs/STATUS.md whole original assembly-origin region; historical module count OPEN'],
       sources=[x['source'] for x in asm]+['src/lowlevel_data.asm'],
       extents=[extent(1,0x10,0x6f7e-0x10,'_TEXT','PROVEN',['106 STRICT entries in 8 accepted slices; original count OPEN'],True),
                extent(2,0x10,0x215c-0x10,'_DATA','STRONG',['lowlevel_data verified spans and assembly access evidence'],True)],
       start=0x10,current_slices=asm,order_basis='leading game code; class precedence reproduced by leading _DATA SEGDEF'))
    # Non-paragraph starts cannot start a C OMF paragraph-aligned contribution.
    # Keep them as explicit subunits of the preceding candidate, not fake objects.
    groups=[]
    for x in game:
        if x['start']%16 and groups:
            groups[-1].setdefault('subunits',[]).append(x)
            groups[-1]['evidence'].append(f"{x['id']} starts unaligned at {x['start']:x}: current source split, not natural C object boundary")
            groups[-1]['extents']+=x['extents']
            groups[-1]['last_bound_public']=max(groups[-1]['last_bound_public'],x['last_bound_public'])
        else:groups.append(x)
    for i,x in enumerate(groups):
        end=groups[i+1]['start'] if i+1<len(groups) else 0x6fbe6
        x['extents'].insert(0,extent(1,x['start'],end-x['start'],'_TEXT',x['boundary_level'],
              ['Provisional address envelope to next candidate start; may include alignment or undiscovered split'],True,
              coverage='envelope, not certified original segment size'))
        entries.append(x)
    tc=json.loads((ROOT/'config/toolchain.json').read_text(encoding='utf-8'))
    libs=[TOOLS_DIR/p for p in cfg['runtime_libs']]
    # Keep the software-FP library explicit if an older oracle config omits it.
    math3r=TOOLS_DIR/tc['installs']['wc106']['dir']/'LIB386/MATH3R.LIB'
    if math3r.resolve() not in {p.resolve() for p in libs}:
        libs.append(math3r)
    hmi_ent=tc['installs']['hmisos3']
    hmi_lib=TOOLS_DIR/hmi_ent['dir']/'sosdw1cr.lib'
    assert sha(hmi_lib)==hmi_ent['key_files']['sosdw1cr.lib'], 'pinned HMI library hash mismatch'
    # The HMI vendor library is pinned and measured by the same byte/fixup/site
    # and symbolic-reference procedure; it remains a library, never src/hmi.
    rt,conflicts=runtime(img,libs+[hmi_lib]);entries+=rt
    # Unknown original data-only inputs need explicit placeholders in the future
    # input list. Cuts around current code-owned data are hypotheses, not proof
    # of a particular original TU count. BSS is kept as an independent block
    # inventory until it can be associated with these inputs.
    for tag,a,b in [('prefix',0x215c,0x6318),('middle',0x632c,0x683c),('suffix',0x6c88,0x2d998)]:
        entries.append(dict(id='unknown:central_DATA_'+tag,kind='unresolved_data_only_inputs',ambiguous=True,
            module_count='OPEN (one or more; ownership could move cuts)',
            evidence=['docs/DATA.md central ownership evidence; cuts exclude verified savecodec/menu _DATA'],
            extents=[extent(2,a,b-a,'_DATA','HYPOTHESIS',['Address envelope; includes reconstructed globals and external assets'],True)],
            original_owner='OPEN'))
    # Exact string extents (NUL included), associated only when referenced by
    # one code candidate. Unreferenced/gapped CONST remains explicitly unknown.
    codeowners=[x for x in entries if x['kind'] in ('game_code_candidate','assembly_module_cluster')]
    def owner(src):
        for e in codeowners:
            for ex in e['extents']:
                if ex['object']==1 and ex['segment']=='_TEXT' and ex['start']<=src<ex['end']:return e['id']
        return None
    refs=collections.defaultdict(set)
    for (o,src),(_,to,off) in img.fixups.items():
        if o==1 and to==2 and 0x2ff00<=off<0x317f4:refs[off].add(owner(src))
    runs=[]
    for a,owners in sorted(refs.items()):
        match=re.match(rb'[\x20-\x7e\t\r\n]{3,}\x00',img.obj[2][a:a+2048])
        if not match or len(owners)!=1 or None in owners:continue
        who=next(iter(owners));end=a+len(match[0])
        if runs and runs[-1]['owner']==who:
            runs[-1]['end']=max(runs[-1]['end'],end);runs[-1]['strings'].append([a,end])
        else:runs.append(dict(owner=who,start=a,end=end,strings=[[a,end]],ambiguous=True,
              evidence='Unique referencing code envelope; first/last observed strings, not whole CONST contribution'))
    # Data symbol inventory is independent of slice/module ownership.
    symbols=json.loads((ROOT/'config/symbols.json').read_text(encoding='utf-8'))['gog_v121_us']['data']
    objects=[]
    for n,v in symbols.items():
        obj,a=addr(v['addr']);size=v.get('size')
        objects.append(dict(name=n,object=obj,start=a,end=a+size if size else None,size=size,
             kind='BSS' if obj==2 and a>=0x3218a else 'initialized',ambiguous=True,
             evidence=v.get('evidence'),name_origin=v.get('name'),original_owner='OPEN'))
    objects.sort(key=lambda x:(x['object'],x['start']))
    # Reproduce the existing BSS hypothesis tool, preserving its documented
    # Android-size false resets instead of promoting these into original TUs.
    import contextlib,io,runpy
    stream=io.StringIO()
    with contextlib.redirect_stdout(stream):runpy.run_path(str(ROOT/'tools/probes/bss_objects.py'))
    (H/'bss_objects.txt').write_text(stream.getvalue(),encoding='utf-8')
    bss_blocks=[]
    for a,b,n,first in re.findall(r'2:([0-9a-f]+)\.\.2:([0-9a-f]+)\s+(\d+) globals\s+first (\w+)',stream.getvalue()):
        a,b=int(a,16),int(b,16)
        consumers=collections.Counter(owner(src) for (obj,src),(_,to,off) in img.fixups.items() if obj==1 and to==2 and a<=off<b)
        bss_blocks.append(dict(start=a,end=b,named_globals=int(n),first_name=first,ambiguous=True,
            level='HYPOTHESIS',evidence='tools/probes/bss_objects.py size-rank reset using Rayverse addresses and Android sizes',
            caveat='Android-width differences create false boundaries; docs/DATA.md specifically merges 3f842..3fa9d',
            referencing_code_envelopes=dict(consumers),original_owner='OPEN'))
    # A next-symbol bound is not a true C size. Preserve that distinction for
    # canonical bindings that lack size (the majority).
    for i,o in enumerate(objects):
        nxt=next((p['start'] for p in objects[i+1:] if p['object']==o['object'] and p['start']>o['start']),None)
        o['end_upper_bound_from_next_binding']=nxt
    # Class-local ordering gives edges; data-only modules remain a partial order.
    orders={};edges=collections.defaultdict(set)
    for seg in ('_TEXT','_DATA','CONST','CONST2','DATA32','XI','YI','_BSS'):
        seq=[]
        for e in entries:
            ee=[x for x in e['extents'] if x['segment']==seg and x['size'] and x['object']==(1 if seg=='_TEXT' else 2)]
            if ee:seq.append((min(x['start'] for x in ee),e['id']))
        seq.sort();orders[seg]=[n for a,n in seq]
        for (_,a),(_,b) in zip(seq,seq[1:]):
            if a!=b:edges[a].add(b)
    # Topological order is only a candidate extension; all unresolved ties marked.
    ids={e['id'] for e in entries};indeg={n:0 for n in ids}
    for a,bs in edges.items():
        for b in bs:indeg[b]+=1
    ready=sorted(n for n in ids if not indeg[n]);linear=[];ties=[]
    while ready:
        if len(ready)>1:ties.append(list(ready))
        a=ready.pop(0);linear.append(a)
        for b in sorted(edges[a]):
            indeg[b]-=1
            if not indeg[b]:ready.append(b);ready.sort()
    cyclic=sorted(ids-set(linear))
    # Compare owned data/CONST/BSS against independently inferred code ranks.
    rank={n:i for i,n in enumerate(orders['_TEXT'])};checks=[]
    for seg in ('_DATA','CONST','CONST2','DATA32','_BSS'):
        seq=[n for n in orders[seg] if n in rank]
        inversions=[dict(left=a,right=b,left_code_rank=rank[a],right_code_rank=rank[b]) for a,b in zip(seq,seq[1:]) if rank[a]>rank[b]]
        checks.append(dict(segment=seg,code_ranked_contributions=len(seq),sequence=seq,adjacent_inversions=inversions))
    srank=[(r['start'],rank.get(r['owner']),r['owner']) for r in runs]
    sinv=[dict(left=a,right=b) for a,b in zip(srank,srank[1:]) if a[1] is not None and b[1] is not None and a[1]>b[1]]
    # Coverage is by accepted slices + runtime segments. Never pretend inferred
    # object envelopes verify initialized data or BSS ownership.
    cov=[]
    for e in fragments+entries:
        for x in e['extents']:
            if x['object']==2 and (x.get('status')=='VERIFIED' or x.get('nonfixup_bytes_equal') or x.get('omf_class')=='BSS'):
                cov.append((x['start'],x['end']))
    missing={name:gaps(cov,a,b) for name,a,b in [('initialized',0,0x3218a),('game_BSS',0x3218a,0x57478),('runtime_BSS',0x57478,0x5795c)]}
    code_cov=[(x['start'],x['end']) for e in entries for x in e['extents'] if x['object']==1]
    missing['code_unassigned_envelopes']=gaps(code_cov,0,len(img.code))
    missing['game_code_envelope_caveat']='Game envelopes cover undecided cuts/alignment; absence of a gap is not code/object closure.'
    manifest=dict(schema=1,oracle='gog_v121_us',oracle_sha256=cfg['sha256'],
      proof_scope='Evidence manifest / partial link order, NOT an original-map reconstruction or natural-link proof',
      extents_half_open=True,inputs={str(p):sha(p) for p in [ROOT/'config/units.json',ROOT/'config/symbols.json',
          ROOT/'config/oracles.json',ROOT/'config/toolchain.json',rp,ROOT/cfg['source']['path'],*libs,hmi_lib,
          *sorted({ROOT/u['source'] for u in units.values()})]},
      pinned_libraries=([dict(kind='runtime',path=str(p),sha256=sha(p)) for p in libs] +
          [dict(kind='vendor_hmi',install='hmisos3',path=str(hmi_lib),sha256=sha(hmi_lib))]),
      class_contract=[dict(segment='_DATA',start=0x10,game_end=0x2d998,original_start_alignment_open=True),
        dict(segment='CONST',game_end=0x317f4),dict(segment='CONST2',start=0x31c20,evidence='MATH3R exp own table precedes istable at 31c40'),dict(segment='DATA32',start=0x31ec4),
        dict(segment='_BSS',clear_start=0x32190,game_runtime_boundary=0x57478,end=0x5795c),dict(segment='STACK',start=0x57960,end=0x59960)],
      contributions=sorted(entries,key=lambda e:linear.index(e['id']) if e['id'] in linear else len(linear)),
      class_orders=orders,order_edges={k:sorted(v) for k,v in edges.items()},
      candidate_topological_order=linear,order_ties=ties,order_cycle_members=cyclic,
      reconstruction_slices=fragments,data_objects=objects,const_runs=runs,bss_rank_blocks=bss_blocks,
      relative_order_checks=checks,string_order_inversions=sinv,runtime_conflicting_votes=conflicts,
      excluded_hmi_reconstruction={n:u['source'] for n,u in excluded_hmi.items()},
      gaps=missing,central_TUs=dict(level='STRONG for central ownership; OPEN for count/exact boundaries/order',
        initialized_envelope=[0x215c,0x2d998],BSS_central_envelope=[0x36d88,0x3fa9d],
        placement='No code rank; central _DATA is interspersed with current code-source definitions, whose original ownership is unproven. Largest central BSS candidate precedes audiocd and subsequent small BSS blocks',
        evidence=['docs/DATA.md scattered consumers','Accepted data/BSS slices are not original objects','docs/DATA.md BSS size-rank resets and hash-group evidence'],
        cautions=['The envelope includes code-owned contributions; it is not a single segment extent.',
                  'Leading central CONST before pcmain CONST is unresolved, not automatically pcmain-owned.',
                  'Current menu/savecodec _DATA byte equality does not independently prove original TU ownership.'],
        ownership_scenarios=[dict(level='HYPOTHESIS',description='One leading central TU owns all non-assembly game _DATA, a leading CONST run, and central BSS; current menu/savecodec _DATA definitions would move to it.'),
          dict(level='HYPOTHESIS',description='Several central data-only TUs are interleaved with code modules; three _DATA placeholder envelopes are the split-ownership input scenario used in class_orders.')]),
      open=['Exact central data/BSS TU count and header/declaration ordering',
            'Historical assembly module count, segment alignments and assembler OMF traversal',
            'Unlocated runtime segments and code-free member selection order',
            'Missing code boundaries with no zero alignment gap; current source split/merge choices',
            'Original FIXUPP order inside non-STRICT units; LE width choices for negative targets'])
    validate_manifest(manifest)
    save('manifest.json',manifest)
    md=['# Original contribution manifest','',manifest['proof_scope']+'.',
        'Addresses are half-open. Every ambiguous flag is retained for future natural linking.','',
        f"{len(entries)} candidate contributions; {len(fragments)} reconstruction slices; {len(objects)} bound data objects; {len(runs)} observed CONST runs.",
        f"Order constraints: {len(cyclic)} cyclic/unresolved nodes; {len(ties)} nonunique topological choices.",'',
        '## Candidate contributions in _TEXT order, then data-only members','',
        '| Contribution | Kind | Extents | Ambiguity / evidence |','|---|---|---|---|']
    def sortkey(e):
        code=[x['start'] for x in e['extents'] if x['object']==1]
        return (0,min(code)) if code else (1,min((x['start'] for x in e['extents']),default=0))
    for e in sorted(entries,key=sortkey):
        ex='; '.join(x['segment']+' '+bounds(x) for x in e['extents'])
        md.append(f"| {e['id']} | {e['kind']} | {ex} | {'OPEN' if e['ambiguous'] else 'located'}; {e['evidence'][0]} |")
    md+=['','## Relative-order checks','']
    for c in checks:md.append(f"- {c['segment']}: {c['code_ranked_contributions']} code-ranked contributors; {len(c['adjacent_inversions'])} adjacent inversions. Details in JSON.")
    md.append(f"- Unique-string runs: {len(runs)}; {len(sinv)} adjacent inversions. These are observed strings, not full segment extents.")
    md+=['','## Central data and ownership','',manifest['central_TUs']['placement']+'.',
         'Verified slices are indexed below for reconstruction, not placed into the original object order.',
         'The central initialized-data envelope is 2:0215c..2:2d998; current savecodec/menu _DATA definitions do not prove original ownership. Largest central BSS candidate is 2:36d88..2:3fa9d. Count, exact cuts and relative ordering remain OPEN.',
         'Both one leading central TU (including current menu/savecodec data definitions) and multiple interleaved central TUs remain possible. The three _DATA placeholder envelopes express the split-ownership scenario, not three proven original objects.',
         'Runtime data-only members have address constraints but often no unique position amongst code-bearing members. See order_ties and unplaced_segments.','',
         '## Observed CONST runs','',
         '| Code owner candidate | First..last observed string extent | Strings |','|---|---|---|']
    for r in runs:md.append(f"| {r['owner']} | 2:{r['start']:05x}..2:{r['end']:05x} | {len(r['strings'])} |")
    md+=['','## BSS size-rank block hypotheses','',
         'These reproduce tools/probes/bss_objects.py. Android-width false boundaries are retained as hypotheses; docs/DATA.md merges 3f842..3fa9d. Referencing code is consumer evidence, not ownership.','',
         '| Candidate envelope | First named global | Count |','|---|---|---|']
    for b in bss_blocks:md.append(f"| 2:{b['start']:05x}..2:{b['end']:05x} | {b['first_name']} | {b['named_globals']} |")
    md+=['',
         '## Verified reconstruction slice inventory','',
         '| Source | Extents | Original ownership |','|---|---|---|']
    for e in fragments:md.append('| '+e['source']+' | '+'; '.join(x['segment']+' '+bounds(x) for x in e['extents'])+' | OPEN; slice only |')
    md+=['','## Gaps in located/verified data coverage','']
    for region,gg in missing.items():
        if isinstance(gg,str):md.append('- '+region+': '+gg);continue
        obj=1 if region=='code_unassigned_envelopes' else 2
        md+=['- '+region+': '+', '.join(f'{obj}:{a:05x}..{obj}:{b:05x}' for a,b in gg)]
    md+=['','## Bound data-object index','',
         '| Name | Address | Size | Owner / evidence |','|---|---|---|---|']
    for o in objects:md.append(f"| {o['name']} | {o['object']}:{o['start']:05x} | {o['size']} | OPEN; {str(o['evidence']).replace('|','/')} |")
    md+=['','## OPEN','']+['- '+x for x in manifest['open']]
    (H/'manifest.md').write_text('\n'.join(md)+'\n',encoding='utf-8')
    print('Manifest:',len(entries),'contributions,',len(fragments),'slices,',len(objects),'objects,',len(runs),'CONST runs.')
    print('Order inversions:',[(c['segment'],len(c['adjacent_inversions'])) for c in checks],'; cycles:',len(cyclic),'; runtime conflicts:',len(conflicts))

def main(argv=None):
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--out',type=Path,default=DEFAULT_OUT,help='output directory (default: build/link)')
    ap.add_argument('--verify-dir',type=Path,default=ROOT/'build/verify',
                    help='fresh tools/verify.py output directory')
    args=ap.parse_args(argv)
    generate(args.out,args.verify_dir)
    return 0


if __name__=='__main__':sys.exit(main())
