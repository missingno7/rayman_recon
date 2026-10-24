"""Assemble reviewable portable specifications from models and certificates."""
import sys,json,re,hashlib,datetime
from pathlib import Path
sys.dont_write_bytecode=True
from prove import MODULES,bundle_hashes
from engine import *
from models import MODELS,DETAILS,SYMS
for module in MODULES:__import__(module)
import android

GPRS=['eax','ebx','ecx','edx','esi','edi','ebp']
def prose(text):
    names=set(MODELS)|{n.lower() for n in MODELS};protected=[]
    def protect(m):
        if m[0] not in names:return m[0]
        protected.append(m[0]);return '\u00a7'+str(len(protected)-1)+'\u00a7'
    text=re.sub(r'\b[A-Za-z_]\w*\b',protect,text)
    text=re.sub(r'(?<![\w])([A-Za-z]{2,})(?=(?:0x|[0-9]))',r'\1 ',text)
    text=re.sub(r'([,;])(?=\S)',r'\1 ',text)
    for i,value in enumerate(protected):text=text.replace('\u00a7'+str(i)+'\u00a7',value)
    return text
PRIVATE_INPUTS={
 'asm_0275c':'ESI source, EDI aperture, ECX plane pitch; incoming DF',
 'asm_0309d':'EAX x, EBX y, EDI aperture', 'local_04a83':'ESI packed source, EDI RAM planes',
 'loc_052DA':'EAX x, EBX y, ECX=0x1010, ESI pixels, EDI destination; stack+4 row flags',
 'asm_06d33':'EAX x, EBX y, CL byte width, CH byte height, ESI image; stack+4 mask pointer',
 'asm_02640':'EBP raster row byte offset, ECX positive span width'}
BASE_INPUTS={
 'draw_effect_segment':'EAX destination, ESI effect_item_t+0x3c',
 'effect_reset_geometry':'No arguments', 'sprite_clipping':'EAX xmin,EBX xmax,ECX ymin,EDX ymax',
 'get_sprite_clipping':'EAX/EBX/ECX/EDX writable s32 pointers to xmin/xmax/ymin/ymax',
 'sub_1268A':'ESI source,EBX source row pitch,EDX source plane pitch',
 'copy_dwords':'ESI source,EDI destination,ECX dword count; incoming DF',
 'bios_wait_timer':'EBX flag pointer,CX:DX interval; ES saved/zeroed/restored',
 'sub_2748':'ESI writable u32 BIOS tick output', 'display_emptypicture':'EDI aperture (ECX in some C declarations is ignored)',
 'display_anypicture':'ESI source,EDI aperture,EAX height,DL source row pitch,DH source plane phase,ECX plane pitch,EBX dword width',
 'asm_0283c':'AL map mask', 'draw_blackbox':'EDI aperture,ESI x,EDX y,EBX height,ECX width',
 'draw_borderbox':'EDI aperture,EAX x,EBX y,ECX height delta,EDX width delta',
 'asm_037e4':'ESI run lists,ECX VGA destination page',
 'select_display_buffer':'EAX display page address (AH programs CRTC start high byte)',
 'block_clear':'EDI destination', 'block_copy':'ESI source tile plane,EDI destination',
 'draw_sprite_deform':'ESI atlas source,EDI output sprite,ECX deform_blit_t*',
 'DisplayBufferModeNormal':'ESI source,EDI destination', 'DrawWldPointPlan2Normal':'EAX x,EBX y,EDI384-byte-pitch destination',
 'DrawBlackBoxNormal':'EAX x,EBX y,ECX height,EDX width,EDI destination',
 'DrawFondBoxNormal':'EAX type,ESI x,EBX y,ECX height,EDX width,EDI destination',
 'DrawBorderBoxNormal':'AL/AH edge/shadow colors,ESI x,EBX y,ECX height,EDX width,EDI destination',
 'DisplayAnyPictureNormal':'ESI source,EDI destination,EAX source x,EBX source y,ECX dest x,EDX dest y; stack+4 stride,+8 width,+12 height (s32),RET12',
 'ClearDrawAndDisplayBufferNormal':'EDI first buffer,ESI second buffer', 'N_CLRSCR':'EDI buffer',
 'Swap_To_Screen':'EAX height,ECX byte width,ESI source,EDI destination',
 'clear_borders_Normal':'EDI first border,EDX byte separation,ECX row count',
 'Copy_Plan0Diff_To_Buf':'ESI source,EDI destination,ECX width,EBX height,EDX source pitch',
 'asm_04d55':'ESI source,EDI destination,ECX width,EBX height,EDX source pitch',
 'flocon_clipping':'EAX top,EBX bottom,ECX left,EDX right', 'wait_until':'EBX signed target tick',
 'asm_02b30':'AH plane mask,AL=2,DX=0x3C4,EDI aperture pointer'}
def inputs(f):
    n=f['name']
    if n in PRIVATE_INPUTS:return PRIVATE_INPUTS[n]
    if n in BASE_INPUTS:return BASE_INPUTS[n]
    if n.startswith('DrawSprite'):
        return 'EAX x,EBX y,CL byte width,CH byte height,EDI destination,ESI source; '+('EDX parallel mask pointer' if n=='DrawSpriteDiffNormal' else 'EDX color bank' if 'Color' in n else 'EDX compatibility argument ignored')
    if n.startswith('clip_sprite'):return 'EAX x,EBX y,CL byte width,CH byte height,ESI source (EDI unchanged)'
    if n in ('asm_03257','asm_03912','asm_03ea1'):return 'AL plane mask,EDX source pitch,EBX plane offset,ECX context pointer,ESI run lists,EDI destination offset'
    if n in ('asm_02850','asm_029d3','asm_02a73'):return 'EAX map_tile_t*,EDX output run lists,EBX source/cache pointer,ECX source stride or cache base,ESI destination,EDI map row skip bytes'
    if n in ('asm_04430','asm_04493'):return 'AH phase0..3,EDX map_tile_t*,EBX frame_context_t*,ECX callback table,EDI aperture,ESI background compatibility argument unused'
    if n in ('Display_Bloc','Display_Bloc_Plein'):return 'ESI normal tile,EDI destination'
    if n.startswith('Display_Bloc'):return 'ESI normal tile,EDI destination,EAX x,EBX y'
    if n.startswith('asm_045'):return 'EDI page aperture'
    if n.startswith(('mode_x_effect','draw_flocon','draw_pluie','draw_normal_')):return 'EAX x,EBX y,EDI destination'
    if n.startswith('fplot'):return 'EAX x,EBX y,ESI color,EDI destination'
    if n.startswith('get_joy_input1'):return 'EAX height,ECX byte width,ESI source,EDI destination'
    return 'No explicit arguments; globals/device inputs as below'

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def hardware_sites(o,f):
    cfg=o.cfg(f['offset']);out=[]
    for a in sorted(cfg['instructions']):
        i=next(o.md.disasm(o.image.code[a:a+15],a,count=1))
        if i.mnemonic in ('in','out','int','cli','sti','cpuid','pushf','pushfd','popf','popfd') or ('es' in i.op_str.split(',') and i.mnemonic in ('push','pop','mov')):
            out.append(dict(address=f'1:{a:05x}',instruction=i.mnemonic+' '+i.op_str))
    return out

def main():
    inv=json.loads((HERE/'inventory.json').read_text());o=Oracle();an=android.load();ref=json.loads((ROOT/'build/workers/platform/reference_abi.json').read_text())
    sources={p.relative_to(ROOT).as_posix():p.read_text(errors='replace') for p in (ROOT/'src').rglob('*') if p.suffix in ('.h','.c')}
    types={}
    for path,content in sources.items():
        for no,line in enumerate(content.splitlines(),1):
            m=re.search(r'#pragma\s+aux\s+(\w+)\b',line)
            if m:types.setdefault(m[1],[]).append(dict(source=path,line=no,pragma=line.strip()))
    for f in inv:
        n=f['name'];d=DETAILS[n].copy();d['description']=prose(d['description']);d['domain']=prose(d['domain']);p=HERE/'results'/f'{n}.json';result=json.loads(p.read_text())
        f.update(d);f['description_status']='PROVEN computation from DOS; EQUIVALENT_TESTED independent executable model'
        f['proof']=result;f['reference_abi']=ref.get(f['address']);f['input_contract']=prose(inputs(f));f['hardware_sites']=hardware_sites(o,f)
        f['android_symbol']=an.get(n);f['name_evidence']=SYMS['functions'].get(n,{}).get('evidence','Private address label; INVENTED')
        f['stack_contract']='Entry[ESP] return address; ordinary RET advances ESP4; all temporary stack written bytes checked.'
        if n=='DisplayAnyPictureNormal':f['stack_contract']='Entry[ESP+4/+8/+12] s32 source pitch,width,height; RET12 advances ESP16. No caller stack arguments remain.'
        elif n in ('loc_052DA','asm_06d33'):f['stack_contract']+=' Caller-owned stack+4 flags/mask is updated as described.'
        f['type_pragmas']=[]
        for path,content in sources.items():
            if not path.endswith('.c'):continue
            m=re.search(r'extern\s+(\w+)\s+'+re.escape(n)+r'\s*;',content)
            if m:f['type_pragmas'].extend(types.get(m[1],[]))
        # Contracts on actual dispatch aliases are evidence of the C-side ABI.
        for alias in f.get('dispatch_aliases',[]):
            for path,content in sources.items():
                if not path.endswith('.h'):continue
                m=re.search(r'(\w+)\s*\*\s*'+re.escape(alias)+r'\s*;',content)
                if m:f['type_pragmas'].extend(types.get(m[1],[]))
        unique={(x['source'],x['line']):x for x in f['type_pragmas']};f['type_pragmas']=list(unique.values())
        f['output_contract']={'registers':'Exact functions of inputs in '+d['model'],'preserved_on_test_domain':result.get('register_behavior',{}).get('preserved_in_all_cases',[]),'carry_output':n in ('clip_sprite_on_screen','clip_sprite_on_screen_flipped','loc_052DA','asm_06d33'),'memory':'Every final written byte value and written address set checked, including private stack and code scratch; hardware services abstracted.'}
    proofs=[f['proof'] for f in inv];counts=dict(public_entries=sum(not f.get('private') for f in inv),private_helpers=sum(bool(f.get('private')) for f in inv),entries=len(inv),modeled=len(MODELS),equivalent_tested=sum(p['status']=='EQUIVALENT_TESTED' for p in proofs),mismatched=sum(p['status']!='EQUIVALENT_TESTED' for p in proofs),cases=sum(p['cases'] for p in proofs),blocks_covered=sum(p['basic_blocks']['covered'] for p in proofs),blocks_total=sum(p['basic_blocks']['total'] for p in proofs),full_block_coverage=sum(p['basic_blocks']['percent']==100 for p in proofs),logical_VGA_plane_byte_checks=sum(p.get('logical_VGA_plane_byte_checks',0) for p in proofs))
    counts['retained_historical_mismatch_runs']=sum(len(x.get('mismatches',[])) for p in proofs for x in p.get('mismatch_history',[]))
    paths=[ROOT/'config/oracles.json',ROOT/'config/symbols.json',ROOT/'config/units.json',ROOT/'tools/image.py']+list((ROOT/'src').rglob('*.h'))+[ROOT/p for p in ('src/lowlevel_00010.asm','src/lowlevel_02640.asm','src/lowlevel_04430.asm','src/lowlevel_data.asm','src/affiche.c','src/frame.c','src/effect2.c')]
    source_hashes={p.relative_to(ROOT).as_posix():sha(p) for p in paths}
    inherited=json.loads((TILEDIR/'proof.json').read_text()) if (TILEDIR:=ROOT/'build/workers/tilecode') else None
    out=dict(schema=1,title='Portable specifications of original DOS assembly',generated_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),evidence_levels={'PROVEN':'Direct DOS instruction/data layout or algebraic fact','EQUIVALENT_TESTED':'Finite randomized execution certificate; not a universal proof or STRICT','STRONG':'Caller-derived domain/intent','HYPOTHESIS':'Unconfirmed historical intent, never required behavior','OPEN':'Unresolved external mechanism or coverage reachability'},counts=counts,oracle={'name':o.image.name,'exe_sha256':o.image.meta['exe_sha256'],'objects':o.image.meta['objects'],'load_bases':BASE},harness={'unicorn':'2.1.4','comparison':'All8 GPRs,CF where an output,IF/DF on joystick/latch paths,ES on BIOS wrapper,every final written byte and address set including stack/self-modifying code,exact IN/OUT/INT/CPUID/probe sequence,and logical VGA plane bytes','hardware_abstraction':'Scripted port/BIOS responses; VGA map-mask projection and write-mode1 latches; CPU AC/ID capability profiles. No real device,BIOS,interrupt host or CPUID is invoked.','basic_blocks':'Recursive intraprocedural CFG; calls fall through. Draw-effect computed targets explicitly bounded to3 scalar fragments/80 dword blocks. Private callees/callback grammar certified separately.','not_claimed':'Universal equivalence,write-event ordering,complete caller-domain reachability,or external BIOS/PMODE implementation.'},script_sha256s=bundle_hashes(),canonical_source_sha256s=source_hashes,tilecode_reuse={'spec':'../tilecode/SPEC.md','proof_sha256':sha(TILEDIR/'proof.json'),'executions':inherited['summary']['executions'],'distinct_bodies':inherited['all_unique_bodies_covered'],'wrapper_models':'callbacks.py reuses existing reader/parser and16 bodies,without repeating corpus analysis'},open_items=[{'id':'BIOS_PMODE','status':'OPEN','entry':'bios_wait_timer','question':'PMODE/W translation of ES=0,EBX flag pointer to BIOS INT15h AH83h; wrapper and abstract request are tested,actual host completion is excluded.'},{'id':'DEFORM_ASCENDING_WRITE','status':'OPEN','entry':'draw_sprite_deform','question':'Static block1:049ed writes only when destination is0x81; it is unvisited with caller-derived rotated rectangles prefilling0xA1. No invalid input was introduced to force coverage.'}],entries=inv)
    (HERE/'asm_spec.json').write_text(json.dumps(out,indent=2)+'\n');render(out)
    print(json.dumps(counts,indent=2))

def render(data):
    c=data['counts'];lines=['# Portable specifications: original DOS assembly','',f"**{c['public_entries']} public entries + {c['private_helpers']} private helpers; {c['equivalent_tested']} EQUIVALENT_TESTED, {c['mismatched']} current mismatches.** {c['cases']:,} executions; {c['blocks_covered']}/{c['blocks_total']} intraprocedural basic blocks covered ({c['full_block_coverage']} entries at100%).",'',
      'This is a portable behavior specification and a finite differential execution certificate, not a universal formal proof or the reconstruction verifier\'s STRICT status. Original assembly remains unchanged. All outputs are confined to this worker directory. The source inventory originally listed8 private helpers; a ninth unreferenced callable starts at1:0309d after RET1:0309c. No binding was added.','',
      'Reproduce from the repository root with installed Python,Capstone and Unicorn2.1.4:','',
      '```text','python -B build/workers/asmspec/catalogue.py','python -B build/workers/asmspec/prove.py --count 256','python -B build/workers/asmspec/controls.py','python -B build/workers/asmspec/report.py','python -B build/workers/asmspec/validate.py','```','',
      'The main seed interval is0x0A550000..0x0A5500FF; each result records its actual interval and exact script hashes. Longer focused reruns,when present,replace that entry\'s interval. Per-routine JSON keeps every detected historical mismatch,including harness failures; current mismatches must be empty. No per-seed trace is stored.','',
      '## Proof boundary and conventions','',
      'PROVEN labels direct DOS operations,record widths,and algebraic facts. STRONG labels caller-derived intent/domain. EQUIVALENT_TESTED labels finite independent-model execution. HYPOTHESIS is not used to prescribe behavior. OPEN questions are listed below. Function name provenance follows config/symbols.json; invented helper/field names are explicitly identified. Android symbols were checked read-only through android.load(),because its CLI writes outside this worker.','',
      'All integers in records are little endian. “width” is bytes/pixels unless otherwise stated. The portable image is8-bit palette indices; no DAC expansion is implied. Test pointers are synthetic flat addresses: code0x1000000,data0x2000000,RAM0x4000000,stack0x8000000,VGA0xA0000. BSS is zero-extended; PMW1 fixups are relocated in emulator memory only. tools/image.py hash-checks decoded objects against the pinned GOG oracle.','',
      'Models compute drawing,clipping,rounding and device effects independently from x86 execution. Separate push/pop bookkeeping models the original ABI,including dead stack contents. Comparisons cover all8 GPRs,the exact written byte-address set and every final byte value (stack and writable code included),CF for clip helpers,selected preserved flags/ES,and exact I/O sequences. They do not compare intermediate write-event order. ModeX writes are also projected onto four logical planes; latch-copy tests begin with independently randomized planes. BIOS and CPU service implementations are excluded and replaced by explicit input/output stubs.','',
      'Block coverage uses the DOS CFG,not a model CFG; direct calls fall through and private callees have their own certificates. Computed raster dispatches enumerate all bounded targets. Callback bodies reuse [the existing tile specification](../tilecode/SPEC.md) and its71,400-run corpus proof; wrapper tests execute16 existing representative bodies without redoing that analysis.','',
      '## Findings and remaining questions','',
      '- PROVEN: Normal beveled borders write an extra shadow row/column; ModeX borders use inclusive coordinate deltas.','- PROVEN: DrawFondBoxNormal uses XOR bank-crossing arithmetic,not ordinary palette-bank saturation.','- PROVEN: Differential sprite clipping rounds to dwords,may touch a rounded fringe,and has asymmetric simultaneous-edge clipping; the exact behavior is retained.','- PROVEN: draw_sprite_deform prefills0xA1 but its ascending path tests0x81. The unvisited store at1:049ed remains a coverage/reachability question.','- PROVEN: asm_02850 mutates saved EBX internally but14*1280 equals the0x4600 circular plane size,so valid calls return the original EBX. Its pragma is correct for that domain.','- PROVEN: asm_02850\'s seam-reversal block1:02920 cannot occur in a valid forward20-tile row: floor((column+4*i)/80) crosses the seam at most once. See controls.json arithmetic check.','- OPEN: Actual PMODE/W BIOS INT15h pointer translation/completion; the isolated wrapper and exact abstract request are tested.','- STRONG: Source caller structs explain raster,deformation and row-run fields. Names of reconstructed helper fields remain INVENTED where no original symbol exists.','',
      '## Entry index','', '| Entry | Address | C sites* | Cases | Blocks | Status |','|---|---|---:|---:|---:|---|']
    for f in data['entries']:
        p=f['proof'];b=p['basic_blocks'];lines.append(f"| [{f['name']}](#{f['name'].lower()}) | {f['address']} | {f['callsite_count']} | {p['cases']} | {b['covered']}/{b['total']} | {p['status']} |")
    lines+=['','*Syntactic direct calls plus resolved dispatch-variable calls; address references are retained separately in JSON. This is an auditable lower bound,not runtime frequency or a complete points-to proof.','']
    for f in data['entries']:
        n=f['name'];p=f['proof'];b=p['basic_blocks'];lines += [f'## {n}', '', f"`{f['address']}`; {'private' if f.get('private') else 'public'}; name **{f['name_provenance']}**. Source: [{f['source']}](../../../{f['source']}). Reconstructed bytes: {f['reconstruction_status']} (imported acceptance status,no fresh compile).",'',
          '**Contract.** '+f['input_contract']+'. '+f['stack_contract'], '',
          '**Register outputs.** Exact transformations: ['+f['model']+']('+f['model'].split(':')[0]+'). Preserved throughout the tested domain: '+', '.join(x.upper() for x in f['output_contract']['preserved_on_test_domain'])+'. '+('CF is a specified accept/reject output. ' if f['output_contract']['carry_output'] else '')+'ESP returns as specified above.','']
        pragmas=f['contracts']+f['type_pragmas']
        if pragmas:
            lines+=['C-side declarations (ABI-only pragmas):','', '```c']+[x['pragma']+' /* '+x['source']+':'+str(x['line'])+' */' for x in pragmas]+['```','']
        elif f.get('reference_abi'):
            lines+=['No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.','']
        lines+=['**Globals.** Reads: '+('; '.join(f['globals_read']) or 'none beyond explicit buffers/descriptor and abstract device inputs')+'. Writes: '+('; '.join(f['globals_written']) or 'no named global beyond output buffers/ABI stack')+'.','']
        lines+=['**Layouts.**','']
        if not f['layouts']:lines+=['Raw byte buffers or scalar pointers only; pitches,extents and any packing are specified below.','']
        for layout in f['layouts']:
            lines += [layout['name']+(' — size'+str(layout['size']) if 'size' in layout else '')+(' bytes; stride'+str(layout['stride']) if 'stride' in layout else '')+'.','']
            fields=layout.get('fields',[])
            if isinstance(fields,list):
                lines+=['| Offset | Width | Signed | Meaning |','|---:|---:|---|---|']
                for field in fields:lines.append(f"| {field['offset']:#x} | {field['width']} | {'yes' if field['signed'] else 'no'} | {field['meaning']} |")
                lines.append('')
            else:lines += [str(fields),'']
        lines += ['**Computation.** '+f['description'],'','**Valid test domain.** '+f['domain'],'']
        if f['hardware_sites']:
            lines+=['**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:','']
            lines += ['- `'+x['address']+' '+x['instruction']+'`' for x in f['hardware_sites']]+['']
        lines += [f"**Proof.** {p['status']}; {p['cases']} cases; seeds `{p['seed_start']:#x}..{p['seed_end']:#x}`; blocks {b['covered']}/{b['total']} ({b['percent']}%). Current mismatches: {len(p['mismatches'])}. [Per-routine certificate](results/{n}.json).",'']
        if b['uncovered']:lines+=['Uncovered: '+', '.join(b['uncovered'])+'. See scope/reachability notes; no comparison was weakened to increase coverage.','']
        if f['callers'] or f.get('indirect_callers'):
            lines+=['**Caller evidence.** '+', '.join(x['source']+':'+str(x['line']) for x in (f['callers']+f.get('indirect_callers',[]))[:8])+'. Full lists and address registrations are in JSON.','']
    formatted=[];fenced=False
    for line in lines:
        if line.startswith('```'):fenced=not fenced;formatted.append(line)
        elif fenced:formatted.append(line)
        else:formatted.append(prose(line))
    (HERE/'ASM_SPEC.md').write_text('\n'.join(formatted)+'\n',encoding='utf-8')

if __name__=='__main__':main()
