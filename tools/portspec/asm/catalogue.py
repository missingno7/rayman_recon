"""Read-only source/oracle inventory. All outputs stay beside this script."""
import sys, json, re, hashlib
from pathlib import Path
HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
sys.dont_write_bytecode = True
sys.path.insert(0, str(ROOT / 'tools'))
from image import Image

def inventory():
    units = json.loads((ROOT/'config/units.json').read_text())['units']
    syms = json.loads((ROOT/'config/symbols.json').read_text())['gog_v121_us']
    sources = {str(p.relative_to(ROOT)).replace('\\','/'):p.read_text(errors='replace')
               for p in (ROOT/'src').rglob('*') if p.suffix in ('.h','.c','.asm')}
    sys.path.insert(0,str(ROOT/'build/workers/platform'))
    import asm_contracts
    from contracts import C
    result=[]
    for unit,u in units.items():
        if u.get('profile') != 'asm' or not u['source'].startswith('src/lowlevel_'): continue
        text=sources[u['source']]
        for name,record in u['functions'].items():
            if name not in syms['functions']: continue
            symbol=syms['functions'][name]
            addr=int(symbol['addr'].split(':')[1],16)
            result.append(dict(name=name,address=symbol['addr'],offset=addr,name_provenance=symbol.get('name','OPEN'),
                source=u['source'],reconstruction_status=record['status'],contracts=[],callers=[],
                callsite_count=0,address_references=[],prior_inventory=C.get(name,{}),proof={'status':'OPEN'},
                layouts=[],description_status='OPEN'))
    for name,off in [('asm_0275c',0x275c),('asm_0309d',0x309d),('local_04a83',0x4a83),('loc_052DA',0x52da),
                     ('get_sprite_clipping_xmin',0x61e2),('get_sprite_clipping_xmax',0x61e8),
                     ('get_sprite_clipping_ymin',0x61ee),('get_sprite_clipping_ymax',0x61f4),('asm_06d33',0x6d33)]:
        result.append(dict(name=name,address=f'1:{off:05x}',offset=off,name_provenance='INVENTED',private=True,
            newly_identified=off==0x309d,source='src/lowlevel_02640.asm' if off<0x3125 else 'src/lowlevel_04430.asm',
            reconstruction_status='STRICT_CONTRIBUTION',contracts=[],callers=[],address_references=[],callsite_count=0,prior_inventory={},proof={'status':'OPEN'},layouts=[],description_status='OPEN'))
    names={f['name']:f for f in result};pattern=re.compile(r'\b('+ '|'.join(map(re.escape,names))+r')\b')
    aliases={n:set() for n in names}
    for path,content in sources.items():
        if not path.endswith(('.c','.h')):continue
        lines=content.splitlines()
        for no,line in enumerate(lines,1):
            pragma=re.search(r'#pragma\s+aux\s+(\w+)\b',line)
            if pragma and pragma[1] in names:
                joined=line.strip();i=no
                while joined.endswith('\\') and i<len(lines):joined+='\n'+lines[i].strip();i+=1
                names[pragma[1]]['contracts'].append(dict(source=path,line=no,pragma=joined))
            if '#pragma' in line or not path.endswith('.c'):continue
            for m in pattern.finditer(line):
                name=m[1];after=line[m.end():];before=line[:m.start()]
                item=dict(source=path,line=no,text=line.strip())
                if re.match(r'\s*\(',after):
                    if re.match(r'\s*(?:extern\s+)?(?:void|s32|u32|int|s16|u16|u8|char)\b',before):continue
                    names[name]['callers'].append(item)
                else:
                    names[name]['address_references'].append(item)
                    assignment=re.search(r'(\w+)\s*=\s*(?:\([^)]*\)\s*)?\(?\s*&?\s*$',before)
                    if assignment:aliases[name].add(assignment[1])
    for f in result:
        f['dispatch_aliases']=sorted(aliases[f['name']]);indirect=[]
        for alias in f['dispatch_aliases']:
            ap=re.compile(r'\b'+re.escape(alias)+r'\s*\(')
            for path,content in sources.items():
                if not path.endswith('.c'):continue
                for no,line in enumerate(content.splitlines(),1):
                    if ap.search(line):indirect.append(dict(source=path,line=no,text=line.strip(),alias=alias))
        f['indirect_callers']=indirect
        f['callsite_count']=len({(x['source'],x['line']) for x in f['callers']+indirect})
    return sorted(result,key=lambda x:x['offset']), sources

if __name__=='__main__':
    fs,sources=inventory()
    (HERE/'inventory.json').write_text(json.dumps(fs,indent=2)+'\n')
    for f in sorted(fs,key=lambda f:(-f['callsite_count'],f['offset'])):
        print(f"{f['address']:9} {f['name']:34} {f['callsite_count']:3} calls {len(f['contracts'])} pragmas")
    print('TOTAL',len(fs))
