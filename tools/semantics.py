"""Semantic registry (docs/semantics/*.json): check it against the real source, bindings and pinned C layout.

    python -B tools/semantics.py check [--registry docs/semantics/basic.json]   enforced facts (also tests/test_semantics.py):
        schema, declarations/types/signedness, function bodies of the unit, bindings, offsetof/sizeof + bitfield layout
    python -B tools/semantics.py check --evidence      also report drift of evidence sources (readers/writers in other
        units, assets, references) - run after integrating matching work; drift means "re-review", not "wrong"
    python -B tools/semantics.py refresh [--registry ...]   after reviewing a reported change: re-hash bodies and
        evidence sources (prints every entity whose evidence changed); never run it just to silence a failure
    python -B tools/semantics.py generate [--out docs/semantics/generated]   port-facing views (header, markdown)

Adopted 2026-10-24 from the basic.c names-and-meanings pilot (build/workers/sem_pilot). Passing checks proves the
recorded facts still match the code, not that an editorial meaning is true; meanings carry their evidence level.
"""
from __future__ import annotations
import argparse, collections, copy, hashlib, json, re, struct, sys
from pathlib import Path

sys.dont_write_bytecode=True
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'config/oracles.json').exists())
sys.path.insert(0,str(ROOT/'tools'))
import image,omf,wcc
from pycparser import c_ast as ca,c_generator,c_parser
GEN=c_generator.CGenerator()
LEVELS={'PROVEN','STRONG','HYPOTHESIS','UNKNOWN'}
KINDS={'function','global','field','type','local'}
HEADERS=['rtypes.h','palette.h','obj.h','game.h','map.h']

def sha(b):return hashlib.sha256(b).hexdigest()
def load(path):return json.loads(Path(path).read_text())
def canonical_type(d):
    text=GEN.visit(d).rstrip(';')
    return re.sub(r'\b'+re.escape(d.name)+r'\b','$',text,count=1).replace('extern ','').strip()
def type_text(t):return re.sub(r'\s+',' ',re.sub(r'\s*:\s*\d+\s*$','',t.replace('$','').strip()))
def type_signedness(t):
    if '*' in t:return 'not applicable (pointer)'
    if '{' in t:return 'not applicable (aggregate)'
    if re.search(r'\b(s8|s16|s32|signed|int)\b',t) and not re.search(r'\bunsigned\b',t):return 'signed'
    if re.search(r'\b(u8|u16|u32|unsigned|bool)\b',t):return 'unsigned'
    if re.search(r'\bchar\b',t):return 'unsigned (plain char; pinned game profile)'
    return 'not applicable (aggregate)'
def clean(s):
    s=re.sub(r'/\*.*?\*/|//[^\n]*',lambda m:'\n'*m[0].count('\n'),s,flags=re.S)
    return re.sub(r'^\s*#.*$','',s,flags=re.M)
def source_path(base,rel):
    p=base/rel
    return p if p.exists() else ROOT/'src'/rel
def parse_source(base):
    prefix='\n'.join(clean(source_path(base,'include/'+h).read_text()) for h in HEADERS)
    prefix+='\n'+clean(source_path(base,'basic_local.h').read_text())
    prefix+='\ntypedef unsigned int size_t; extern int errno;\n'
    return c_parser.CParser().parse(prefix+'\n'+clean(source_path(base,'basic.c').read_text()))

def field_declarations(ast):
    out={}
    def walk(t,owner,path=''):
        for d in t.decls or []:
            p=path+(d.name or '');inner=d.type
            while isinstance(inner,ca.TypeDecl):inner=inner.type
            if isinstance(inner,(ca.Struct,ca.Union)) and inner.decls:
                if d.name:out[(owner,p)]=d
                walk(inner,owner,p+'.' if d.name else path)
            else:out[(owner,p)]=d
    for td in ast.ext:
        if isinstance(td,ca.Typedef) and isinstance(td.type,ca.TypeDecl) and isinstance(td.type.type,(ca.Struct,ca.Union)):
            walk(td.type.type,td.name)
    return out

def reverse_names(s,entities,function=None):
    for e in entities:
        if not e.get('proposed_name'):continue
        if e['kind']=='local' and function and e['function']==function:
            s=re.sub(r'\b'+re.escape(e['proposed_name'])+r'\b',e['current_name'],s)
        elif e['kind']=='field':
            s=re.sub(r'\b'+re.escape(e['proposed_name'])+r'\b',e['current_name'].split('.')[-1],s)
    return s

def validate_schema(reg):
    errors=[];seen=set()
    for e in reg.get('entities',[]):
        if e.get('id') in seen:errors.append('duplicate entity '+e['id'])
        seen.add(e.get('id'))
        for key in ['id','kind','current_name','proposed_name','c_type','meaning','unit','value_domain','evidence_level','evidence','port_notes']:
            if key not in e:errors.append(f'{e.get("id")}: missing {key}')
        if e.get('kind') not in KINDS:errors.append(f'{e.get("id")}: invalid kind')
        if e.get('evidence_level') not in LEVELS:errors.append(f'{e.get("id")}: invalid evidence level')
        if e.get('proposed_name') and e.get('evidence_level') not in ['PROVEN','STRONG']:
            errors.append(f'{e.get("id")}: rename lacks PROVEN/STRONG evidence')
        if not e.get('evidence'):errors.append(f'{e.get("id")}: no evidence')
        if e.get('kind') in ['field','local','global'] and 'declarator' in e:
            if type_text(e['declarator'])!=e['c_type']:errors.append(e['id']+': C type metadata differs from declarator')
        if e.get('kind')=='field':
            if type_signedness(e['declarator'])!=e['signedness']:errors.append(e['id']+': signedness metadata differs from declared C type')
            for k in ['offset','size']:
                if not isinstance(e.get(k),int) or e[k]<0:errors.append(e['id']+': invalid '+k)
    return errors

def check_source(reg,source_root=None,check_fingerprints=True,artifact_root=None):
    base=Path(source_root or ROOT/'src').resolve();entities=reg['entities'];errors=validate_schema(reg)
    ast=parse_source(base);fds=field_declarations(ast)
    funcs={n.decl.name:n for n in ast.ext if isinstance(n,ca.FuncDef)}
    globs={n.name:n for n in ast.ext if isinstance(n,ca.Decl) and 'extern' in n.storage and not isinstance(n.type,ca.FuncDecl)}
    b=load(ROOT/'config/symbols.json')[reg['oracle']]
    expected_functions={e['current_name'] for e in entities if e['kind']=='function'}
    if set(funcs)!=expected_functions:errors.append('function coverage differs from actual basic.c')
    used_ids=set()
    class Used(ca.NodeVisitor):
        def visit_ID(self,n):used_ids.add(n.name)
        def visit_StructRef(self,n):self.visit(n.name) # member tokens are not global identifiers
    for f in funcs.values():Used().visit(f.body)
    actual_globals=set(globs)&used_ids
    expected_globals={e['current_name'] for e in entities if e['kind']=='global'}
    if actual_globals!=expected_globals:errors.append('global coverage differs from actual basic.c')
    type_names={e['current_name'] for e in entities if e['kind']=='type'}
    actual_fields={(t,n) for (t,n) in fds if t in type_names}
    expected_fields={(e['struct'],e.get('proposed_name') if base!=ROOT/'src' and e.get('proposed_name') else e['current_name']) for e in entities if e['kind']=='field'}
    if actual_fields!=expected_fields:errors.append('field coverage differs from actual shared types')
    for e in entities:
        n=e['current_name'];kind=e['kind'];label=e['id']
        if kind=='field':
            path=e.get('proposed_name') if base!=ROOT/'src' and e.get('proposed_name') else n
            d=fds.get((e['struct'],path))
            if d is None:errors.append(label+': field declaration missing');continue
            got=reverse_names(canonical_type(d),entities)
            if got!=e['declarator']:errors.append(label+': field declaration type drift')
            bits=int(d.bitsize.value) if d.bitsize else None
            if bits!=e.get('bit_width'):errors.append(label+': bit width drift')
        elif kind=='function':
            if n not in funcs:errors.append(label+': source definition missing');continue
            if b['functions'].get(n,{}).get('addr')!=e['oracle_address']:errors.append(label+': symbol address drift')
            typ=reverse_names(GEN.visit(funcs[n].decl),entities,n)
            if typ!=e['c_type']:errors.append(label+': function declaration drift')
            body=reverse_names(GEN.visit(funcs[n].body),entities,n)
            if sha(body.encode())!=e['source_body_sha256']:errors.append(label+': function body drift; re-evidence required')
        elif kind=='global':
            if n not in globs:errors.append(label+': declaration missing');continue
            if canonical_type(globs[n])!=e['declarator']:errors.append(label+': global type drift')
            if e.get('binding_kind')=='game_data':
                if b['data'].get(n,{}).get('addr')!=e['oracle_address']:errors.append(label+': global binding drift')
            elif e.get('binding_kind')=='runtime_accessor_expression':
                # errno is an lvalue macro. Independently verify the bound getter
                # and its relocation, rather than inventing a second game symbol.
                if b['functions'].get('__get_errno_ptr_',{}).get('addr')!='1:704e8':errors.append(label+': errno accessor binding drift')
                img=image.Image(reg['oracle'])
                a=image.parse_addr(e['oracle_address'])
                if img.code[0x704e8:0x704e9]!=b'\xb8' or img.code[0x704ed]!=0xc3 or img.fixups.get((1,0x704e9),(None,None,None))[1:]!=a:
                    errors.append(label+': errno accessor return address differs')
            else:errors.append(label+': unsupported binding kind')
    for fn,f in funcs.items():
        actual=[]
        class V(ca.NodeVisitor):
            def visit_Decl(self,d):
                if d.name and not isinstance(d.type,ca.FuncDecl):actual.append(('local',d))
                self.generic_visit(d)
        V().visit(f.body)
        for d in f.decl.type.args.params if f.decl.type.args else []:
            if isinstance(d,ca.Decl) and d.name:actual.append(('parameter',d))
        expected=[e for e in entities if e['kind']=='local' and e['function']==fn]
        if len(actual)!=len(expected):errors.append(fn+': local declaration coverage drift');continue
        for (role,d),e in zip(actual,expected):
            if role!=e['role'] or reverse_names(d.name,entities,fn)!=e['current_name'] or reverse_names(canonical_type(d),entities,fn)!=e['declarator']:
                errors.append(e['id']+': local declaration drift')
    if check_fingerprints:
        for path,want in reg['source_fingerprints'].items():
            rel=path.removeprefix('src/');p=source_path(base,rel)
            if base!=ROOT/'src' and (base/rel).exists():want=reg.get('proposal_fingerprints',{}).get(rel,want)
            if sha(p.read_bytes())!=want:errors.append(path+': source fingerprint drift; review registry evidence')
        # A new interpretation in any reader/writer must not pass silently just
        # because basic.c and the struct declaration stayed unchanged. Full-file
        # hashes are deliberately conservative and require explicit review even
        # for compiler-steering edits in one of these evidence dependencies.
        for path,want in reg.get('evidence_source_fingerprints',{}).items():
            p=ROOT/path;rel=path.removeprefix('src/')
            if base!=ROOT/'src' and path.startswith('src/') and (base/rel).exists():
                p=base/rel;want=reg.get('proposal_fingerprints',{}).get(rel,want)
            if not p.exists() or sha(p.read_bytes())!=want:errors.append(path+': reader/writer evidence dependency drift')
        artifact_base=Path(artifact_root or ROOT/reg.get('artifact_root','build/workers/sem_pilot'))
        for key,root in [('evidence_artifact_fingerprints',artifact_base),('asset_input_fingerprints',ROOT),('reference_input_fingerprints',ROOT)]:
            for path,want in reg.get(key,{}).items():
                p=root/path
                if not p.exists() or sha(p.read_bytes())!=want:errors.append(path+': '+key+' drift')
    return errors

def assertion_source(reg,proposal=False):
    lines=['#include <stddef.h>','#include "obj.h"','#include "game.h"','#include "map.h"']
    lines += ['typedef char sem_pointer_width[(sizeof(void*) == 4) ? 1 : -1];',
              'typedef char sem_int_width[(sizeof(int) == 4) ? 1 : -1];',
              'typedef char sem_short_width[(sizeof(short) == 2) ? 1 : -1];',
              'typedef char sem_char_unsigned[(((char)-1) > 0) ? 1 : -1];']
    bits=[]
    for i,e in enumerate(reg['entities']):
        if e['kind']=='type':lines.append(f'typedef char sem_type_{i}[(sizeof({e["current_name"]}) == {e["size"]}) ? 1 : -1];')
        if e['kind']!='field':continue
        t=e['struct'];n=e['proposed_name'] if proposal and e.get('proposed_name') else e['current_name']
        if e.get('bit_width'):
            bits.append(e)
            lines.append(f'void sem_bit_{i}({t}* p) {{ p->{n} = {(1<<e["bit_width"])-1}; }}')
        else:
            lines.append(f'typedef char sem_field_{i}[(offsetof({t},{n}) == {e["offset"]} && sizeof((({t}*)0)->{n}) == {e["size"]}) ? 1 : -1];')
    return '\n'.join(lines)+'\n'

def check_layout(reg,work,source_root=None):
    work=Path(work).resolve();work.mkdir(parents=True,exist_ok=True)
    # Scratch path and include override only; immutable wrapper still hashes the
    # compiler, uses the game profile, and owns a fresh isolated environment.
    wcc.BUILD=work/'compiler';base=Path(source_root or ROOT/'src').resolve()
    src=work/'semantic_assertions.c';out=work/'semantic_assertions.obj'
    src.write_text(assertion_source(reg,base!=ROOT/'src'))
    def record_trial(errors,status):
        with (work/'layout_trials.jsonl').open('a') as ledger:
            ledger.write(json.dumps(dict(source_hash=sha(src.read_bytes()),edit='generated registry offsetof/sizeof and bitfield assertions',
              status=status,byte_diffs=None,errors=errors,byte_diff_note='Layout probe, not a game function match.'))+'\n')
    prof=wcc.profile('game')
    rc,log=wcc.compile_c(src,out,prof['flags'],prof['install'],extra_include=[base/'include'])
    if rc:
        out.unlink(missing_ok=True)
        errors=['Pinned layout assertions failed: '+log.strip()]
        record_trial(errors,'COMPILE_REJECTED');return errors
    m=omf.load(out)[0];out.unlink()
    # ISO C disallows sizeof/offsetof(bitfield). Test the compiler-created setter
    # instead, against the recorded byte offset/mask, including every alias view.
    import unicorn as uc
    from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ESP
    sizes={e['current_name']:e['size'] for e in reg['entities'] if e['kind']=='type'}
    errors=[];checked=0
    for i,e in enumerate(reg['entities']):
        if e['kind']!='field' or not e.get('bit_width'):continue
        p=next(p for p in m.publics if p[0]==f'sem_bit_{i}_');_,si,off,_=p
        em=uc.Uc(uc.UC_ARCH_X86,uc.UC_MODE_32)
        em.mem_map(0x100000,0x10000);em.mem_map(0x200000,0x10000);em.mem_map(0x300000,0x10000)
        em.mem_write(0x100000,bytes(m.segments[si].data));em.mem_write(0x30fff0,struct.pack('<I',0x10fff0))
        em.reg_write(UC_X86_REG_EAX,0x200000);em.reg_write(UC_X86_REG_ESP,0x30fff0)
        em.emu_start(0x100000+off,0x10fff0,count=100)
        got=bytes(em.mem_read(0x200000,sizes[e['struct']]))
        want=bytearray(len(got));want[e['offset']]=e['bit_mask']
        if got!=bytes(want) or e['size']!=1:errors.append(e['id']+': bitfield offset/size/mask mismatch')
        checked+=1
    record_trial(errors,'LAYOUT_PASS' if not errors else 'BITFIELD_REJECTED')
    return errors

def generate(reg,out):
    out=Path(out);out.mkdir(parents=True,exist_ok=True)
    header=['/* Generated DOS layout facts. Native structs need not share DOS packing. */',
            '#ifndef RAYMAN_SEMANTICS_DOS_H','#define RAYMAN_SEMANTICS_DOS_H']
    md=['# basic.c semantic registry','',reg['proof_boundary'],'',
        '| Entity | Level | Meaning | Units | DOS layout/address |',
        '|---|---|---|---|---|']
    contracts=['# Port-facing contracts for basic.c','',
       'Generated from semantics.json. Confidence applies to the stated, scoped meaning; it is separate from STRICT status. ',
       'DOS offsets and packed widths describe the original ABI/assets. They do not require packed native runtime structs.','']
    for e in reg['entities']:
        label=e['id'];layout=e.get('oracle_address','')
        if e['kind']=='field':
            layout=f'+0x{e["offset"]:x}, {e["size"]} B'
            name=re.sub(r'\W','_',e['struct']+'_'+e['current_name']).upper()
            header+=['/* '+e['evidence_level']+': '+e['meaning'].replace('*/','* /')+' */',
                     f'#define DOS_{name}_OFFSET 0x{e["offset"]:x}',f'#define DOS_{name}_SIZE {e["size"]}']
            if e.get('bit_width'):header.append(f'#define DOS_{name}_MASK 0x{e["bit_mask"]:02x}')
        elif e['kind']=='type':
            header.append(f'#define DOS_{e["current_name"].upper()}_SIZE {e["size"]}')
        md.append('| '+' | '.join(str(x).replace('|','\\|').replace('\n',' ') for x in [label,e['evidence_level'],e['meaning'],e['unit'],layout])+' |')
        if e.get('proposed_name'):header.append('/* Proposed source name: '+e['proposed_name']+' */')
        if e['kind'] in ['function','global','field']:
            contracts += ['## '+e['id'],'',e['evidence_level']+': '+e['meaning'],'',
                          'Units: '+e['unit']+'. DOS layout/address: '+str(layout)+'.','']
            if e.get('proposed_name'):contracts += ['Proposed name: `'+e['proposed_name']+'` (see registry provenance).','']
            contracts += [p+'\n' for p in e['port_notes']]
            if 'shipped_initial_values' in e.get('value_domain',{}):
                d=e['value_domain']['shipped_initial_values']
                contracts += [f'Shipped initial-data sample: {d["count"]} records, {d["distinct"]} distinct values, range {d["minimum"]}..{d["maximum"]}. This is not the full runtime domain.','']
    header+=['#endif','']
    (out/'semantics_dos.h').write_text('\n'.join(header))
    (out/'SEMANTICS.md').write_text('\n'.join(md)+'\n')
    (out/'PORT_CONTRACTS.md').write_text('\n'.join(contracts)+'\n')

def refresh(reg,path):
    """Re-hash function bodies and evidence sources after a reviewed change; report what changed."""
    entities=reg['entities'];ast=parse_source(ROOT/'src')
    funcs={n.decl.name:n for n in ast.ext if isinstance(n,ca.FuncDef)}
    changed=[]
    for e in entities:
        if e['kind']=='function' and e['current_name'] in funcs:
            h=sha(reverse_names(GEN.visit(funcs[e['current_name']].body),entities,e['current_name']).encode())
            if h!=e.get('source_body_sha256'):changed.append(e['id']);e['source_body_sha256']=h
    for key,root in [('source_fingerprints',ROOT),('evidence_source_fingerprints',ROOT),('asset_input_fingerprints',ROOT),('reference_input_fingerprints',ROOT)]:
        for rel in list(reg.get(key,{})):
            p=root/rel
            h=sha(p.read_bytes()) if p.exists() else None
            if h!=reg[key][rel]:changed.append(rel);reg[key][rel]=h
    for key in ('proposal_fingerprints','evidence_artifact_fingerprints'):
        reg.pop(key,None)
    Path(path).write_text(json.dumps(reg,indent=1,ensure_ascii=False)+'\n',encoding='utf-8')
    print('refreshed; changed:',len(changed));[print('  ',c) for c in changed[:60]]
    return 0

def main(argv=None):
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('action',choices=['check','generate','refresh'])
    ap.add_argument('--registry',type=Path,default=ROOT/'docs/semantics/basic.json');ap.add_argument('--source-root',type=Path)
    ap.add_argument('--evidence',action='store_true');ap.add_argument('--out',type=Path);args=ap.parse_args(argv);reg=load(args.registry)
    if args.action=='generate':generate(reg,args.out or args.registry.parent/'generated');return 0
    if args.action=='refresh':return refresh(reg,args.registry)
    errors=check_source(reg,args.source_root,check_fingerprints=args.evidence,artifact_root=args.registry.parent)
    if not errors:errors+=check_layout(reg,args.out or ROOT/'build/semantics/layout',args.source_root)
    for e in errors:print('FAIL:',e)
    if errors:return 1
    c=collections.Counter(e['kind'] for e in reg['entities'])
    print('PASS:',dict(c),'source/bindings/layout validated')
    return 0

if __name__=='__main__':raise SystemExit(main())
