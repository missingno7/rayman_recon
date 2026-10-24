"""Read-only GOG LEV tile-code reader. All outputs are confined to this directory."""
from pathlib import Path
import struct
import hashlib
import argparse
from collections import Counter
import capstone

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
MD.detail = True

def sha(b):
    return hashlib.sha256(b).hexdigest()

def read_level(path):
    b = path.read_bytes()
    event, normal, w, h = struct.unpack_from('<IIHH', b)
    map_start = 12 + 3 * 768 + 1
    xheader = map_start + w*h*6 + 6
    n, size = struct.unpack_from('<II', b, xheader)
    pixels_start = xheader + 8
    source_table = pixels_start + n*256 + 1
    code_start = source_table + 4800
    code_table = code_start + size + 1
    assert code_table + 4800 <= len(b)
    pixels_enc = b[pixels_start:source_table-1]
    code_enc = b[code_start:code_table-1]
    assert sum(pixels_enc) % 256 == b[source_table-1], path
    assert sum(code_enc) % 256 == b[code_table-1], path
    offsets = struct.unpack_from('<1200i', b, code_table)
    sources = struct.unpack_from('<1200I', b, source_table)
    code = bytes(x ^ 0xf3 for x in code_enc)
    starts = sorted(set(x for x in offsets if x != -1))
    assert all(0 <= x < size for x in starts)
    assert (not starts and size==0) or (starts and starts[0]==0)
    assert code_table+4800==normal, ('noncontiguous sections',path)
    ends = dict(zip(starts, starts[1:] + [size]))
    bodies = {s: code[s:ends[s]] for s in starts}
    return dict(path=path.relative_to(ROOT).as_posix(), file_sha256=sha(b),
                width=w, height=h, event=event, normal=normal,
                map_start=map_start, xheader=xheader, pixel_count=n,
                pixels_start=pixels_start, source_table=source_table,
                code_start=code_start, code_size=size, code_table=code_table,
                after_code_table=code_table+4800, offsets=offsets, sources=sources,
                pixels=bytes(x ^ 0x7d for x in pixels_enc), code=code, bodies=bodies,
                map=b[map_start:map_start+w*h*6])

def levels():
    return [read_level(p) for p in sorted((ROOT/'assets/GOG/PCMAP').rglob('*.LEV'))]

def instructions(body):
    ins = list(MD.disasm(body, 0))
    assert sum(i.size for i in ins) == len(body), ('undecoded', sha(body))
    return ins

def normalized(body):
    """Keep instruction/register/width/scale structure; normalize immediates/displacements.
    Branch targets retain instruction index, hence exact CFG structure is preserved.
    """
    ins = instructions(body)
    indices = {i.address:j for j,i in enumerate(ins)}
    out = []
    for i in ins:
        ops = []
        for o in i.operands:
            if o.type == capstone.CS_OP_REG:
                ops.append(('r',i.reg_name(o.reg),o.size))
            elif o.type == capstone.CS_OP_IMM:
                if i.group(capstone.CS_GRP_JUMP):
                    ops.append(('target',indices.get(o.imm, 'EXTERNAL')))
                else:
                    ops.append(('imm',o.size))
            elif o.type == capstone.CS_OP_MEM:
                m=o.mem
                ops.append(('mem',o.size,i.reg_name(m.segment),i.reg_name(m.base),i.reg_name(m.index),m.scale))
            else:
                raise ValueError(i.op_str)
        out.append((i.mnemonic,ops))
    return out

def listing(body):
    return '\n'.join(f'{i.address:04x}  {i.bytes.hex():20} {i.mnemonic:7} {i.op_str}' for i in instructions(body))

if __name__ == '__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--sample', help='level suffix, e.g. JUNGLE/RAY1.LEV')
    p.add_argument('--tile', type=int, default=0)
    args=p.parse_args()
    ls=levels()
    if args.sample:
        l=next(l for l in ls if l['path'].endswith(args.sample))
        off=l['offsets'][args.tile]
        print(l['path'], 'tile',args.tile,'code offset',off)
        print(listing(l['bodies'][off]) if off!=-1 else 'NULL')
    else:
        allb={b for l in ls for b in l['bodies'].values()}
        print('levels',len(ls),'unique bodies',len(allb),'code bytes',sum(l['code_size'] for l in ls))
        print('mnemonics',Counter(i.mnemonic for b in allb for i in instructions(b)))
        print('normalized',len({repr(normalized(b)) for b in allb}))
        for l in ls[:2]:
            print({k:v for k,v in l.items() if k not in ('code','pixels','bodies','offsets','sources','map')})
            for s,b in list(l['bodies'].items())[:2]:
                print('offset',s,'bytes',len(b));print(listing(b))
