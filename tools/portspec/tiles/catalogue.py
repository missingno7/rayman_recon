"""Fail-closed tile-code grammar and complete compact shape census."""
import json
from collections import Counter, defaultdict
from extract import HERE, levels, instructions, normalized, sha, listing
from capstone.x86 import X86_OP_REG, X86_OP_IMM, X86_OP_MEM

def reg(i, o, name):
    return o.type == X86_OP_REG and i.reg_name(o.reg) == name

def parse(body):
    """Validate every instruction; recover four sparse 16x4 constant planes.
    This is a structural decoder, not a general x86 interpreter.
    Each pixel is None (no write) or an explicit byte value, including zero.
    """
    ins = instructions(body)
    p = 0
    def take(mnemonic):
        nonlocal p
        i=ins[p]; p+=1
        assert i.mnemonic==mnemonic, (p, i.mnemonic, mnemonic)
        if mnemonic=='ret':assert len(i.operands)==0
        return i
    i=take('mov')
    assert len(i.operands)==2 and reg(i,i.operands[0],'edx')
    assert i.operands[1].type==X86_OP_IMM and i.operands[1].imm==80
    planes=[]; store_metrics=[]
    for phase in range(4):
        i=take('cmp');a,b=i.operands
        assert reg(i,a,'ah') and b.type==X86_OP_IMM and b.imm==phase
        j=take('jne'); target=j.operands[0].imm
        assert target>j.address
        rows_advanced=0
        plane=[None]*64
        stores=Counter()
        while ins[p].mnemonic!='ret':
            i=ins[p];p+=1
            assert i.address<target
            if i.mnemonic=='add':
                a,b=i.operands
                assert reg(i,a,'edi') and reg(i,b,'ecx')
                rows_advanced+=3
                assert rows_advanced<=15
            else:
                assert i.mnemonic=='mov'
                a,b=i.operands
                assert a.type==X86_OP_MEM and b.type==X86_OP_IMM
                m=a.mem
                assert not m.segment and i.reg_name(m.base)=='edi'
                assert (not m.index and m.scale==1) or (i.reg_name(m.index)=='edx' and m.scale in (1,2))
                row=rows_advanced+(m.scale if m.index else 0)
                col=m.disp
                assert 0<=row<16 and 0<=col<4 and col+a.size<=4
                assert a.size in (1,2,4)
                value=b.imm & ((1<<(8*a.size))-1)
                for k in range(a.size):
                    index=row*4+col+k
                    assert plane[index] is None, ('overlap',phase,index)
                    plane[index]=(value>>(8*k))&255
                stores[a.size]+=1
        assert rows_advanced==15
        take('ret')
        assert ins[p].address==target, ('wrong branch landing',phase)
        planes.append(plane);store_metrics.append(dict(stores))
    take('ret')
    assert p==len(ins)
    image=[None]*256
    for phase,plane in enumerate(planes):
        for y in range(16):
            for c in range(4):
                image[y*16+phase+4*c]=plane[y*4+c]
    return dict(image=image, planes=planes, stores=store_metrics,
                written_pixels=sum(x is not None for x in image))

def census():
    ls=levels()
    unique={b for l in ls for b in l['bodies'].values()}
    records={}
    shapes={}
    for b in sorted(unique, key=sha):
        model=parse(b)
        norm=normalized(b)
        sid='S'+sha(json.dumps(norm,separators=(',',':')).encode())[:16]
        bid=sha(b)
        records[bid]=dict(body=b, model=model, shape=sid)
        if sid not in shapes:
            schedules=[]
            for phase in range(4):
                rows=['']*6;group=0;active=False
                for i in instructions(b):
                    if i.mnemonic=='cmp':active=i.operands[1].imm==phase
                    elif active and i.mnemonic=='add':group+=1
                    elif active and i.mnemonic=='mov':
                        m=i.operands[0].mem
                        rows[group]+=({1:'B',2:'W',4:'D'}[i.operands[0].size]+str(m.scale if m.index else 0)+' ')
                    elif active and i.mnemonic=='ret':break
                schedules.append([r.strip() for r in rows])
            shapes[sid]=dict(family='phase_selected_sparse_constant_16x16',
                description='Write the selected x mod 4 plane of a constant 16x16 tile; leave masked pixels unchanged.',
                phase_store_schedules=schedules, unique_bodies=0, level_bodies=0, tile_entries=0,
                representative=None, code_bytes_range=[len(b),len(b)],
                written_pixels_range=[model['written_pixels'],model['written_pixels']],
                stores_by_phase=model['stores'], levels={})
        s=shapes[sid];s['unique_bodies']+=1
        for key,value in [('code_bytes_range',len(b)),('written_pixels_range',model['written_pixels'])]:
            s[key]=[min(s[key][0],value),max(s[key][1],value)]
    level_records=[]
    for l in ls:
        counts=Counter(); table_counts=Counter(); tiles_by_off=defaultdict(list)
        for tile,off in enumerate(l['offsets']):
            if off!=-1: tiles_by_off[off].append(tile)
        routines=[]
        for off,b in l['bodies'].items():
            bid=sha(b);r=records[bid];sid=r['shape'];s=shapes[sid]
            counts[sid]+=1;table_counts[sid]+=len(tiles_by_off[off])
            loc=dict(level=l['path'],tile=tiles_by_off[off][0],code_offset=off,file_offset=l['code_start']+off)
            if s['representative'] is None:s['representative']=loc
            s['level_bodies']+=1;s['tile_entries']+=len(tiles_by_off[off])
            routines.append(dict(code_offset=off,bytes=len(b),sha256=bid,shape=sid,tiles=tiles_by_off[off]))
        for sid in counts:
            shapes[sid]['levels'][l['path']]=dict(bodies=counts[sid],tile_entries=table_counts[sid])
        plain={k:v for k,v in l.items() if k not in ('pixels','code','bodies','offsets','sources','map')}
        plain.update(null_entries=l['offsets'].count(-1),routine_count=len(l['bodies']),
                     shape_count=len(counts),shape_counts=dict(counts),routines=routines,
                     code_sha256=sha(l['code']),pixels_sha256=sha(l['pixels']),
                     unreferenced_code_prefix=min(l['bodies'],default=l['code_size']))
        level_records.append(plain)
    summary=dict(levels=len(ls),table_entries=len(ls)*1200,
                 nonnull_entries=sum(1200-l['offsets'].count(-1) for l in ls),
                 null_entries=sum(l['offsets'].count(-1) for l in ls),
                 level_bodies=sum(len(l['bodies']) for l in ls),unique_bodies=len(unique),
                 normalized_shapes=len(shapes),semantic_families=1,
                 code_bytes=sum(l['code_size'] for l in ls),
                 unexplained_routines=[],
                 stored_zero_pixels=sum(r['model']['image'].count(0) for r in records.values()),
                 pixel_count_range=[min(r['model']['written_pixels'] for r in records.values()),
                                    max(r['model']['written_pixels'] for r in records.values())])
    return ls,records,dict(schema=1,evidence='PROVEN structural census; differential results in proof.json',
                           normalization='Immediates and memory displacements erased; registers, operand widths, addressing scales and branch target instruction indices retained. Shape ID hashes that normalized instruction list. phase_store_schedules is a compact equivalent: four phases, six groups separated by ADD EDI,ECX; B/W/D=1/2/4 byte immediate store, 0/1/2=EDX multiplier relative to EDI. Fixed prologue and four CMP/JNE/RET wrappers are specified in SPEC.md.',
                           summary=summary,shapes=shapes,levels=level_records)

def main():
    ls,records,out=census()
    (HERE/'shapes.json').write_text(json.dumps(out,indent=1)+'\n')
    # Two bounded samples: smallest and largest complete routine, not bulk dumps.
    sample=[]
    for label,r in [('smallest',min(records.values(),key=lambda r:len(r['body']))),
                    ('largest',max(records.values(),key=lambda r:len(r['body'])))]:
        sample.extend([label+' '+r['shape']+' '+sha(r['body']),listing(r['body']),''])
    (HERE/'samples.txt').write_text('\n'.join(sample))
    print(json.dumps(out['summary'],indent=2))
    print('shapes.json bytes', (HERE/'shapes.json').stat().st_size)

if __name__=='__main__':main()
