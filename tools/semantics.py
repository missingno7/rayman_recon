"""Semantic registries: scoped source/bindings/layout checks and port-facing views.

check enforces schema, declarations, referenced entity coverage, unit bodies, bindings
and pinned compiled layout. --evidence separately reports dependency drift; it does
not turn another unit's edit into an enforcement failure. refresh requires prior
human review of the change. No meaning is proved by a hash or a passing layout test.
"""
from __future__ import annotations
import argparse
import collections
import hashlib
import json
import re
import struct
import sys
from pathlib import Path

sys.dont_write_bytecode = True
ROOT = next(p for p in Path(__file__).resolve().parents if (p / 'config/oracles.json').exists())
sys.path.insert(0, str(ROOT / 'tools'))
import image, omf, wcc
from pycparser import c_ast as ca, c_generator, c_parser

GEN = c_generator.CGenerator()
LEVELS = {'PROVEN', 'STRONG', 'HYPOTHESIS', 'UNKNOWN'}
KINDS = {'function', 'global', 'field', 'type', 'local'}
HERE = Path(__file__).resolve().parent.parent
PROPOSAL = (HERE / 'registries').is_dir()
DEFAULT_REGISTRIES = HERE / 'registries' if PROPOSAL else ROOT / 'docs/semantics'
DEFAULT_SOURCE = HERE / 'units' if PROPOSAL else ROOT / 'src'
DEFAULT_WORK = HERE / 'checks' if PROPOSAL else ROOT / 'build/semantics'

def sha(b):
    return hashlib.sha256(b).hexdigest()

def load(path):
    return json.loads(Path(path).read_text(encoding='utf-8'))

def canonical_type(d):
    text = GEN.visit(d).rstrip(';')
    return re.sub(r'\b' + re.escape(d.name) + r'\b', '$', text, count=1).replace('extern ', '').strip()

def type_text(t):
    return re.sub(r'\s+', ' ', re.sub(r'\s*:\s*\d+\s*$', '', t.replace('$', '').strip()))

def type_signedness(t):
    if '*' in t: return 'not applicable (pointer)'
    if '{' in t: return 'not applicable (aggregate)'
    if re.search(r'\b(s8|s16|s32|signed|int)\b', t) and not re.search(r'\bunsigned\b', t): return 'signed'
    if re.search(r'\b(u8|u16|u32|unsigned|bool)\b', t): return 'unsigned'
    if re.search(r'\bchar\b', t): return 'unsigned (plain char; pinned game profile)'
    return 'not applicable (aggregate)'

def clean(s):
    s = re.sub(r'/\*.*?\*/|//[^\n]*', lambda m: '\n' * m[0].count('\n'), s, flags=re.S)
    return re.sub(r'^\s*#.*$', '', s, flags=re.M)

def source_path(base, rel):
    """A proposal is a sparse overlay of src; canonical files are read-only fallback."""
    rel = rel.removeprefix('src/')
    p = Path(base) / rel
    return p if p.exists() else ROOT / 'src' / rel

def expand_statement_macros(text, names):
    """Expand the scope's single-line statement macros from their actual source.

    Other expressions remain in the pilot's spelling (e.g. get_eta is a call in
    the AST). These macros need expansion because callers may omit a semicolon.
    Changing the definition therefore changes the enforced function-body hash.
    """
    definitions = {}
    for name in names:
        m = re.search(r'^\s*#define\s+'+re.escape(name)+r'\(([^)]*)\)\s*(.*)$',text,re.M)
        if not m: raise ValueError('missing scoped statement macro '+name)
        definitions[name] = ([p.strip() for p in m[1].split(',')],m[2])
    result = clean(text)
    for name,(params,body) in definitions.items():
        pattern = re.compile(r'\b'+re.escape(name)+r'\s*\(')
        pos = 0
        while (m := pattern.search(result,pos)):
            start = m.end(); depth = 1; args = []; last = start; i = start
            while depth:
                if result[i] == '(': depth += 1
                elif result[i] == ')': depth -= 1
                elif result[i] == ',' and depth == 1: args.append(result[last:i].strip()); last = i+1
                i += 1
            args.append(result[last:i-1].strip())
            if len(args) != len(params): raise ValueError('statement macro arity: '+name)
            values = dict(zip(params,args))
            expanded = re.sub(r'\b(?:'+'|'.join(map(re.escape,params))+r')\b',lambda x:values[x[0]],body)
            result = result[:m.start()]+expanded+result[i:]; pos = m.start()+len(expanded)
    return result

def parse_source(base, scope):
    parts = []
    for rel in scope['headers'] + scope.get('local_headers', []):
        parts.append(clean(source_path(base, rel).read_text(encoding='latin-1')))
    parts.append(scope.get('preamble', ''))
    if scope.get('source'):
        parts.append(expand_statement_macros(source_path(base, scope['source']).read_text(encoding='latin-1'),scope.get('statement_macros',[])))
    return c_parser.CParser().parse('\n'.join(parts), filename=scope.get('source') or 'shared headers')

def field_declarations(ast):
    out = {}
    def walk(t, owner, path=''):
        for d in t.decls or []:
            p = path + (d.name or '')
            inner = d.type
            while isinstance(inner, ca.TypeDecl): inner = inner.type
            if isinstance(inner, (ca.Struct, ca.Union)) and inner.decls:
                if d.name: out[(owner, p)] = d
                walk(inner, owner, p + '.' if d.name else path)
            else: out[(owner, p)] = d
    for td in ast.ext:
        if isinstance(td, ca.Typedef) and isinstance(td.type, ca.TypeDecl) and isinstance(td.type.type, (ca.Struct, ca.Union)):
            walk(td.type.type, td.name)
    return out

def reverse_names(s, entities, function=None):
    for e in entities:
        p = e.get('proposed_name')
        if not p: continue
        if e['kind'] == 'local' and function == e['function']:
            s = re.sub(r'\b' + re.escape(p) + r'\b', e['current_name'], s)
        elif e['kind'] == 'field':
            s = re.sub(r'\b' + re.escape(p.split('.')[-1]) + r'\b', e['current_name'].split('.')[-1], s)
    return s

def validate_schema(reg):
    errors = []
    scope = reg.get('scope')
    if not isinstance(scope, dict): return ['scope must declare kind, headers and source/contexts']
    if scope.get('kind') not in ('unit', 'types', 'globals'): errors.append('invalid scope kind')
    contexts = scope.get('contexts', []) if scope.get('kind') == 'globals' else [scope]
    if not contexts: errors.append('scope has no source contexts')
    for c in contexts:
        if not isinstance(c.get('headers'), list): errors.append('scope headers must be a list')
        if c.get('source') and (Path(c['source']).is_absolute() or '..' in Path(c['source']).parts): errors.append('source must be relative to src')
    if scope.get('kind') == 'unit' and not scope.get('source'): errors.append('unit source missing')
    seen = set()
    for e in reg.get('entities', []):
        label = e.get('id', '<missing id>')
        if label in seen: errors.append('duplicate entity ' + label)
        seen.add(label)
        for key in ('id','kind','current_name','proposed_name','c_type','meaning','unit','value_domain','evidence_level','evidence','port_notes'):
            if key not in e: errors.append(label + ': missing ' + key)
        if e.get('kind') not in KINDS: errors.append(label + ': invalid kind')
        if e.get('evidence_level') not in LEVELS: errors.append(label + ': invalid evidence level')
        if e.get('proposed_name') and e.get('evidence_level') not in ('PROVEN', 'STRONG'): errors.append(label + ': rename lacks PROVEN/STRONG evidence')
        if not e.get('evidence'): errors.append(label + ': no evidence')
        if e.get('kind') in ('field','local','global') and 'declarator' in e:
            if type_text(e['declarator']) != e['c_type']: errors.append(label + ': C type metadata differs from declarator')
        if e.get('kind') == 'field':
            if 'declarator' not in e: errors.append(label + ': missing declarator'); continue
            if type_signedness(e['declarator']) != e.get('signedness'): errors.append(label + ': signedness metadata differs from declared C type')
            for k in ('offset','size'):
                if not isinstance(e.get(k), int) or e[k] < 0: errors.append(label + ': invalid ' + k)
    return errors

def function_declarations(ast):
    return {n.decl.name:n for n in ast.ext if isinstance(n, ca.FuncDef)}

def global_declarations(ast):
    return {n.name:n for n in ast.ext if isinstance(n, ca.Decl) and 'extern' in n.storage and not isinstance(n.type, ca.FuncDecl)}

def used_globals(funcs, globs):
    names = set()
    class Used(ca.NodeVisitor):
        def visit_ID(self, n): names.add(n.name)
        def visit_StructRef(self, n): self.visit(n.name)
    for f in funcs.values(): Used().visit(f.body)
    return names & globs.keys()

def local_declarations(f):
    actual = []
    class V(ca.NodeVisitor):
        def visit_Decl(self, d):
            if d.name and not isinstance(d.type, ca.FuncDecl): actual.append(('local', d))
            self.generic_visit(d)
    V().visit(f.body)
    for d in f.decl.type.args.params if f.decl.type.args else []:
        if isinstance(d, ca.Decl) and d.name: actual.append(('parameter', d))
    return actual

def check_source(reg, source_root=None, check_fingerprints=False, artifact_root=None, shared_entities=()):
    base = Path(source_root or DEFAULT_SOURCE).resolve()
    errors = validate_schema(reg)
    if errors: return errors
    scope = reg['scope']; entities = reg['entities']; all_entities = list(entities) + list(shared_entities)
    contexts = scope['contexts'] if scope['kind'] == 'globals' else [scope]
    asts = [parse_source(base, c) for c in contexts]
    ast = asts[0]; fds = field_declarations(ast)
    funcs = function_declarations(ast) if scope['kind'] == 'unit' else {}
    globs_by_context = [global_declarations(a) for a in asts]
    globs = {n:d for g in globs_by_context for n,d in g.items()}
    b = load(ROOT / 'config/symbols.json')[reg['oracle']]
    if scope['kind'] == 'unit':
        expected = {e.get('proposed_name') or e['current_name'] for e in entities if e['kind'] == 'function'}
        if set(funcs) != expected: errors.append(scope['source'] + ': function coverage differs: ' + str(sorted(set(funcs) ^ expected)))
        actual = used_globals(funcs, globs)
        expected = {s.removeprefix('global:') for s in scope['globals']}
        if actual != expected: errors.append(scope['source'] + ': global coverage differs: ' + str(sorted(actual ^ expected)))
    type_names = {e['current_name'] for e in entities if e['kind'] == 'type'}
    actual_fields = {(t,n) for t,n in fds if t in type_names}
    expected_fields = {(e['struct'],e.get('proposed_name') or e['current_name']) for e in entities if e['kind'] == 'field'}
    if actual_fields != expected_fields: errors.append('field coverage differs from scoped types: ' + str(sorted(actual_fields ^ expected_fields)))
    for e in entities:
        n = e.get('proposed_name') or e['current_name']; kind = e['kind']; label = e['id']
        if kind == 'field':
            d = fds.get((e['struct'], n))
            if d is None: errors.append(label + ': field declaration missing'); continue
            if reverse_names(canonical_type(d), all_entities) != e['declarator']: errors.append(label + ': field declaration type drift')
            bits = int(d.bitsize.value) if d.bitsize else None
            if bits != e.get('bit_width'): errors.append(label + ': bit width drift')
        elif kind == 'type':
            if n not in {td.name for td in ast.ext if isinstance(td,ca.Typedef)}: errors.append(label + ': type declaration missing')
        elif kind == 'function':
            if n not in funcs: errors.append(label + ': source definition missing'); continue
            if b['functions'].get(e['current_name'],{}).get('addr') != e['oracle_address']: errors.append(label + ': symbol address drift')
            if reverse_names(GEN.visit(funcs[n].decl), all_entities, n) != e['c_type']: errors.append(label + ': function declaration drift')
            body = reverse_names(GEN.visit(funcs[n].body), all_entities, n)
            if sha(body.encode()) != e['source_body_sha256']: errors.append(label + ': function body drift; re-evidence required')
        elif kind == 'global':
            found = False
            for c,g in zip(contexts,globs_by_context):
                if n not in g: continue
                found = True
                if reverse_names(canonical_type(g[n]), all_entities) != e['declarator']: errors.append(label + ': global type drift in ' + str(c.get('source')))
            if not found: errors.append(label + ': declaration missing'); continue
            if e.get('binding_kind') == 'game_data':
                if b['data'].get(e['current_name'],{}).get('addr') != e['oracle_address']: errors.append(label + ': global binding drift')
            elif e.get('binding_kind') == 'runtime_accessor_expression':
                if b['functions'].get('__get_errno_ptr_',{}).get('addr') != '1:704e8': errors.append(label + ': errno accessor binding drift')
                img = image.Image(reg['oracle']); a = image.parse_addr(e['oracle_address'])
                if img.code[0x704e8:0x704e9] != b'\xb8' or img.code[0x704ed] != 0xc3 or img.fixups.get((1,0x704e9),(None,None,None))[1:] != a:
                    errors.append(label + ': errno accessor return address differs')
            else: errors.append(label + ': unsupported binding kind')
    for fn,f in funcs.items():
        actual = local_declarations(f)
        expected = [e for e in entities if e['kind'] == 'local' and e['function'] == fn]
        if len(actual) != len(expected): errors.append(fn + ': local declaration coverage drift'); continue
        for (role,d),e in zip(actual,expected):
            if role != e['role'] or reverse_names(d.name, all_entities, fn) != e['current_name'] or reverse_names(canonical_type(d), all_entities, fn) != e['declarator']:
                errors.append(e['id'] + ': local declaration drift')
    # Compatibility for programmatic callers requesting the old combined diagnostic.
    # CLI uses check_evidence separately so reported drift never becomes enforcement.
    if check_fingerprints: errors += check_evidence(reg, base, artifact_root)
    return errors

def check_collection(registries, source_root=None):
    errors = []; by_id = {}
    for name,reg in registries.items():
        for e in reg['entities']:
            if e['id'] in by_id: errors.append('duplicate cross-registry entity ' + e['id'])
            by_id[e['id']] = e
    fields = set(); globals_ = set()
    for e in by_id.values():
        if e['kind'] == 'field':
            key = (e['struct'],e.get('proposed_name') or e['current_name'])
            if key in fields: errors.append('duplicate field ownership ' + str(key))
            fields.add(key)
        elif e['kind'] == 'global':
            if e['current_name'] in globals_: errors.append('duplicate global ownership ' + e['current_name'])
            globals_.add(e['current_name'])
    for name,reg in registries.items():
        if reg['scope']['kind'] == 'unit':
            for key in reg['scope']['globals']:
                if key not in by_id or by_id[key]['kind'] != 'global': errors.append(name + ': unresolved global entity ' + key)
            for key in reg['scope'].get('types', []):
                if key not in by_id or by_id[key]['kind'] != 'type': errors.append(name + ': unresolved type entity ' + key)
        for problem in check_source(reg, source_root, shared_entities=by_id.values()): errors.append(name + ': ' + problem)
    return errors

def dependency_path(path, base, artifact_root):
    if path.startswith('src/'): return source_path(base, path)
    return ROOT / path

def check_evidence(reg, source_root=None, artifact_root=None):
    base = Path(source_root or DEFAULT_SOURCE).resolve()
    artifact_base = Path(artifact_root or ROOT / (reg.get('artifact_root') or 'build/semantics'))
    drift = []
    for key in ('source_fingerprints','evidence_source_fingerprints','asset_input_fingerprints','reference_input_fingerprints','evidence_artifact_fingerprints'):
        for path,want in reg.get(key,{}).items():
            p = artifact_base / path if key == 'evidence_artifact_fingerprints' else dependency_path(path,base,artifact_base)
            if path.startswith('src/') and base != ROOT/'src' and (base/path[4:]).exists():
                want = reg.get('proposal_fingerprints',{}).get(path[4:],want)
            if not p.is_file() or sha(p.read_bytes()) != want: drift.append(path + ': ' + key + ' drift (re-review evidence)')
    return drift

def assertion_source(reg, proposal=False, source_root=None):
    scope = reg['scope']
    if scope['kind'] == 'globals': return None
    lines = ['#include <stddef.h>'] + ['#include "' + h.removeprefix('include/') + '"' for h in scope['headers']]
    lines += ['#include "' + h + '"' for h in scope.get('local_headers', [])]
    if scope.get('layout_local_types'):
        ast = parse_source(Path(source_root or DEFAULT_SOURCE),scope)
        lines += [GEN.visit(td) + ';' for td in ast.ext if isinstance(td,ca.Typedef) and td.name in scope['layout_local_types']]
    lines += ['typedef char sem_pointer_width[(sizeof(void*) == 4) ? 1 : -1];',
              'typedef char sem_int_width[(sizeof(int) == 4) ? 1 : -1];',
              'typedef char sem_short_width[(sizeof(short) == 2) ? 1 : -1];',
              'typedef char sem_char_unsigned[(((char)-1) > 0) ? 1 : -1];']
    for i,e in enumerate(reg['entities']):
        if e['kind'] == 'type': lines.append(f'typedef char sem_type_{i}[(sizeof({e["current_name"]}) == {e["size"]}) ? 1 : -1];')
        if e['kind'] != 'field': continue
        t = e['struct']; n = e.get('proposed_name') if proposal and e.get('proposed_name') else e['current_name']
        if e.get('bit_width'):
            lines.append(f'void sem_bit_{i}({t}* p) {{ p->{n} = {(1<<e["bit_width"])-1}; }}')
        else:
            lines.append(f'typedef char sem_field_{i}[(offsetof({t},{n}) == {e["offset"]} && sizeof((({t}*)0)->{n}) == {e["size"]}) ? 1 : -1];')
    return '\n'.join(lines) + '\n'

def check_layout(reg, work, source_root=None):
    if reg['scope']['kind'] == 'globals': return []
    work = Path(work).resolve(); work.mkdir(parents=True,exist_ok=True)
    base = Path(source_root or DEFAULT_SOURCE).resolve()
    src = work/'semantic_assertions.c'; out = work/'semantic_assertions.obj'
    src.write_text(assertion_source(reg,base != ROOT/'src',base),encoding='utf-8')
    def record(errors,status):
        with (work/'layout_trials.jsonl').open('a',encoding='utf-8') as ledger:
            ledger.write(json.dumps(dict(source_hash=sha(src.read_bytes()),edit='scoped offsetof/sizeof and bitfield assertions',status=status,byte_diffs=None,errors=errors))+'\n')
    old_build = wcc.BUILD
    try:
        wcc.BUILD = work/'compiler'; prof = wcc.profile('game')
        rc,log = wcc.compile_c(src,out,prof['flags'],prof['install'],extra_include=[base/'include',base,ROOT/'src'])
    finally: wcc.BUILD = old_build
    if rc:
        out.unlink(missing_ok=True); errors = ['Pinned layout assertions failed: '+log.strip()]
        record(errors,'COMPILE_REJECTED'); return errors
    m = omf.load(out)[0]; out.unlink()
    import unicorn as uc
    from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ESP
    sizes = {e['current_name']:e['size'] for e in reg['entities'] if e['kind']=='type'}
    errors = []
    for i,e in enumerate(reg['entities']):
        if e['kind'] != 'field' or not e.get('bit_width'): continue
        _,si,off,_ = next(p for p in m.publics if p[0]==f'sem_bit_{i}_')
        em = uc.Uc(uc.UC_ARCH_X86,uc.UC_MODE_32)
        for addr in (0x100000,0x200000,0x300000): em.mem_map(addr,0x10000)
        em.mem_write(0x100000,bytes(m.segments[si].data)); em.mem_write(0x30fff0,struct.pack('<I',0x10fff0))
        em.reg_write(UC_X86_REG_EAX,0x200000); em.reg_write(UC_X86_REG_ESP,0x30fff0)
        em.emu_start(0x100000+off,0x10fff0,count=100)
        got = bytes(em.mem_read(0x200000,sizes[e['struct']]))
        want = bytearray(len(got)); want[e['offset']] = e['bit_mask']
        if got != bytes(want) or e['size'] != 1: errors.append(e['id']+': bitfield offset/size/mask mismatch')
    record(errors,'LAYOUT_PASS' if not errors else 'BITFIELD_REJECTED')
    return errors

def generate(registries, out):
    if 'entities' in registries: registries = {'registry':registries}
    out = Path(out); out.mkdir(parents=True,exist_ok=True)
    header = ['/* Generated DOS ABI facts; native runtime structs need not be packed. */','#ifndef RAYMAN_SEMANTICS_DOS_H','#define RAYMAN_SEMANTICS_DOS_H']
    md = ['# Semantic registry','','Confidence describes the stated meaning, separately from STRICT matching.','', '| Registry / Entity | Level | Meaning | Units | DOS layout/address |','|---|---|---|---|---|']
    contracts = ['# Port-facing contracts','','Generated from all selected semantic registries. UNKNOWN and HYPOTHESIS are observations, not implementation requirements.','']
    for name,reg in sorted(registries.items()):
        contracts += ['## '+name,'',reg['proof_boundary'],'']
        for e in reg['entities']:
            layout = e.get('oracle_address','')
            if e['kind'] == 'field':
                layout = f'+0x{e["offset"]:x}, {e["size"]} B'
                macro = re.sub(r'\W','_',e['struct']+'_'+(e.get('proposed_name') or e['current_name'])).upper()
                header += ['/* '+e['evidence_level']+': '+e['meaning'].replace('*/','* /')+' */',f'#define DOS_{macro}_OFFSET 0x{e["offset"]:x}',f'#define DOS_{macro}_SIZE {e["size"]}']
                if e.get('bit_width'): header.append(f'#define DOS_{macro}_MASK 0x{e["bit_mask"]:02x}')
            elif e['kind'] == 'type': header.append(f'#define DOS_{e["current_name"].upper()}_SIZE {e["size"]}')
            md.append('| '+' | '.join(str(x).replace('|','\\|').replace('\n',' ') for x in (name+' / '+e['id'],e['evidence_level'],e['meaning'],e['unit'],layout))+' |')
            if e['kind'] in ('function','global','field'):
                contracts += ['### '+e['id'],'',e['evidence_level']+': '+e['meaning'],'','Units: '+e['unit']+'. DOS layout/address: '+str(layout)+'.','']
                if e.get('proposed_name'): contracts += ['Proposed source name: `'+e['proposed_name']+'` (registry records provenance).','']
                contracts += [p+'\n' for p in e['port_notes']]
                if 'shipped_initial_values' in e.get('value_domain',{}):
                    d = e['value_domain']['shipped_initial_values']
                    contracts += [f'Shipped initial sample: {d["count"]} records, {d["distinct"]} values, {d["minimum"]}..{d["maximum"]}. This does not bound runtime values.','']
    header += ['#endif','']
    for filename,lines in (('semantics_dos.h',header),('SEMANTICS.md',md),('PORT_CONTRACTS.md',contracts)):
        (out/filename).write_text('\n'.join(lines)+'\n',encoding='utf-8')

def refresh(reg,path,source_root=None,artifact_root=None,shared_entities=()):
    """After review, refresh hashes using the same scope/name normalization as check."""
    base = Path(source_root or DEFAULT_SOURCE).resolve(); changed = []
    if reg['scope']['kind'] == 'unit':
        funcs = function_declarations(parse_source(base,reg['scope']))
        for e in reg['entities']:
            if e['kind'] != 'function': continue
            n = e.get('proposed_name') or e['current_name']
            if n not in funcs: continue
            h = sha(reverse_names(GEN.visit(funcs[n].body),list(reg['entities'])+list(shared_entities),n).encode())
            if h != e.get('source_body_sha256'): changed.append(e['id']); e['source_body_sha256'] = h
    artifact_base = Path(artifact_root or ROOT/(reg.get('artifact_root') or 'build/semantics'))
    for key in ('source_fingerprints','evidence_source_fingerprints','asset_input_fingerprints','reference_input_fingerprints','evidence_artifact_fingerprints'):
        for rel in list(reg.get(key,{})):
            p = artifact_base/rel if key=='evidence_artifact_fingerprints' else dependency_path(rel,base,artifact_base)
            h = sha(p.read_bytes()) if p.exists() else None
            if h != reg[key][rel]: changed.append(key+': '+rel); reg[key][rel] = h
    # Proposal hashes are meaningful only for an overlay; refresh them too.
    for rel in reg.get('proposal_fingerprints',{}):
        p = source_path(base,rel); h = sha(p.read_bytes())
        if reg['proposal_fingerprints'][rel] != h: changed.append('proposal: '+rel); reg['proposal_fingerprints'][rel] = h
    Path(path).write_text(json.dumps(reg,indent=1,ensure_ascii=False)+'\n',encoding='utf-8')
    print(str(path)+': refreshed; changed '+str(len(changed)))
    for item in changed: print('  '+item)
    return changed

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('action',choices=('check','generate','refresh'))
    ap.add_argument('--registry',type=Path,action='append',help='Repeat for a subset; default: all registries')
    ap.add_argument('--registry-dir',type=Path,default=DEFAULT_REGISTRIES)
    ap.add_argument('--source-root',type=Path,default=DEFAULT_SOURCE)
    ap.add_argument('--evidence',action='store_true'); ap.add_argument('--out',type=Path)
    args = ap.parse_args(argv)
    paths = args.registry or sorted(args.registry_dir.glob('*.json'))
    if not paths: ap.error('no registries found')
    registries = {str(p):load(p) for p in paths}
    all_entities = [e for r in registries.values() for e in r['entities']]
    if args.action == 'generate': generate({Path(n).name:r for n,r in registries.items()},args.out or args.registry_dir/'generated'); return 0
    if args.action == 'refresh':
        for name,reg in registries.items(): refresh(reg,name,args.source_root,shared_entities=all_entities)
        return 0
    # An explicitly selected unit can reference the shared registries in its directory.
    dependencies = dict(registries)
    for name in list(registries):
        for p in sorted(Path(name).parent.glob('*.json')): dependencies.setdefault(str(p),load(p))
    errors = check_collection(dependencies,args.source_root)
    if not errors:
        for name,reg in registries.items(): errors += check_layout(reg,(args.out or DEFAULT_WORK)/Path(name).stem,args.source_root)
    for problem in errors: print('FAIL: '+problem)
    if args.evidence:
        for name,reg in registries.items():
            for drift in check_evidence(reg,args.source_root): print('DRIFT: '+Path(name).name+': '+drift)
    if errors: return 1
    counts = collections.Counter(e['kind'] for r in registries.values() for e in r['entities'])
    print('PASS: '+str(dict(counts))+' source/bindings/layout validated across '+str(len(registries))+' registries')
    return 0

if __name__ == '__main__': raise SystemExit(main())
