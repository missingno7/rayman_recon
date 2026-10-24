"""Independent cross-check against each LEV's Mode Normal row-major bitmaps.
Also account for every map record and every byte of the code section.
"""
import json
import struct
from collections import Counter
from catalogue import census
from extract import HERE, ROOT, sha

def main():
    ls,records,catalogue=census()
    summary=Counter();per_level=[];negatives=Counter()
    for l in ls:
        b=(ROOT/l['path']).read_bytes()
        o=l['normal']
        offsets=struct.unpack_from('<1200I',b,o)
        total,full,size=struct.unpack_from('<III',b,o+4800)
        enc=b[o+4812:o+4812+size]
        assert len(enc)==size and sum(enc)%256==b[o+4812+size]
        assert o+4812+size+1==l['event']
        raw=bytes(x^255 for x in enc)
        checked=0
        for tid,off in enumerate(l['offsets']):
            if off==-1:continue
            r=records[sha(l['bodies'][off])]
            image=bytes(0 if p is None else p for p in r['model']['image'])
            at=offsets[tid]
            assert image==raw[at:at+256], (l['path'],tid,'normal bitmap mismatch')
            summary['matching_callback_bitmaps']+=1
            summary['matching_callback_pixels_including_holes']+=256
            checked+=1
            source=l['sources'][tid]
            if source+256>len(l['pixels']):negatives['callback_gros_source_out_of_range']+=1
            else:
                negatives['callback_gros_source_in_range']+=1
                if image!=l['pixels'][source:source+256]:
                    negatives['callback_gros_source_in_range_but_different']+=1
        maps=Counter()
        for p in range(0,len(l['map']),6):
            tid=struct.unpack_from('<H',l['map'],p)[0]
            kind=l['map'][p+4]
            assert tid<1200 and kind in (0,1,2)
            assert (kind==2)==(l['offsets'][tid]!=-1),(l['path'],p,tid,kind)
            maps[kind]+=1
        for kind,n in maps.items():summary[f'map_records_type_{kind}']+=n
        # Distinct callback pointers have no aliases or gaps: every extent passes
        # the grammar through its final RET, with no trailing padding/data.
        assert len(l['bodies'])==sum(off!=-1 for off in l['offsets'])
        assert sum(len(b) for b in l['bodies'].values())==l['code_size']
        summary['code_bytes_accounted']+=l['code_size']
        per_level.append(dict(level=l['path'],callback_bitmaps_checked=checked,
                              map_types=dict(maps),normal_total_blocks=total,
                              normal_full_blocks=full,normal_data_size=size))
    unique_models=[r['model'] for r in records.values()]
    summary['unique_bodies_with_empty_phase']=sum(any(all(p is None for p in plane) for plane in m['planes']) for m in unique_models)
    summary['unique_bodies_with_full_phase']=sum(any(all(p is not None for p in plane) for plane in m['planes']) for m in unique_models)
    summary['levels']=len(ls)
    out=dict(result='PASS',evidence='PROVEN exhaustive independent asset-data comparison',
             summary=dict(summary),negative_results=dict(negatives),levels=per_level)
    (HERE/'audit.json').write_text(json.dumps(out,indent=1)+'\n')
    print(json.dumps({k:v for k,v in out.items() if k!='levels'},indent=2))

if __name__=='__main__':main()
