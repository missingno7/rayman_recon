"""Export callback tiles as portable masked row-major data (optional output).
An absent pixel is encoded as 0; exhaustive census proves no callback writes 0.
The original per-tile callback absence is retained separately. Opaque tiles are
not callback tiles and must be loaded by the native port's ordinary data path.
"""
import argparse
import json
from extract import HERE,levels,sha
from catalogue import parse

def export_level(l):
    tiles=[]
    for tid,off in enumerate(l['offsets']):
        if off==-1:continue
        image=parse(l['bodies'][off])['image']
        assert 0 not in image
        mask=bytes(sum((image[y*16+x] is not None)<<x for x in range(16))>>(8*k)&255
                   for y in range(16) for k in range(2))
        pixels=bytes(0 if p is None else p for p in image)
        tiles.append(dict(tile_id=tid,pixels_hex=pixels.hex(),write_mask_hex=mask.hex()))
    return dict(level=l['path'],source_sha256=l['file_sha256'],table_size=1200,tile_width=16,tile_height=16,
                format='row-major uint8 palette indices; 16 little-endian uint16 row masks (bit x)',
                callback_tile_count=len(tiles),callback_tiles=tiles)

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--level',help='suffix such as JUNGLE/RAY1.LEV; default all levels')
    p.add_argument('--out',required=True,help='JSON output path inside this worker directory')
    args=p.parse_args()
    out=PathOut(args.out)
    ls=levels()
    if args.level:
        ls=[l for l in ls if l['path'].endswith(args.level)]
        assert ls,'level not found'
    data=[export_level(l) for l in ls]
    out.write_text(json.dumps(data,separators=(',',':'))+'\n')
    print('exported',sum(l['callback_tile_count'] for l in data),'callback tiles to',out)

def PathOut(name):
    from pathlib import Path
    p=Path(name)
    if not p.is_absolute():p=HERE/p
    p=p.resolve()
    assert p.is_relative_to(HERE),'outputs must stay inside the worker directory'
    return p

if __name__=='__main__':main()
