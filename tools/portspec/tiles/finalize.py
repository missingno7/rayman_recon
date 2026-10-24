"""Finalize summary tables and cross-check the delivered reports/export format."""
import json
from extract import HERE, ROOT, levels, sha
from catalogue import parse
from native_tiles import export_level

def main():
    c=json.loads((HERE/'shapes.json').read_text())
    p=json.loads((HERE/'proof.json').read_text())
    a=json.loads((HERE/'audit.json').read_text())
    assert p['result']==a['result']=='PASS'
    assert set(c['shapes'])==set(p['shape_executions'])
    bodies={r['sha256'] for l in c['levels'] for r in l['routines']}
    assert bodies==set(p['body_sha256s'])
    assert sum(p['shape_executions'].values())==p['summary']['executions']
    assert sum(l['routine_count'] for l in c['levels'])==a['summary']['matching_callback_bitmaps']
    assert c['summary']['unique_bodies']==p['summary']['distinct_bodies_tested']==len(bodies)
    assert all(s['level_bodies']==sum(v['bodies'] for v in s['levels'].values()) for s in c['shapes'].values())
    rows=['| Level | Callbacks | Shapes | Null slots | Code bytes |',
          '|---|---:|---:|---:|---:|']
    for l in c['levels']:
        assert sum(l['shape_counts'].values())==l['routine_count']
        assert sum(len(r['tiles']) for r in l['routines'])+l['null_entries']==1200
        rows.append('| '+' | '.join(str(v) for v in [l['path'].removeprefix('assets/GOG/PCMAP/'),
                    l['routine_count'],l['shape_count'],l['null_entries'],l['code_size']])+' |')
    s=c['summary']
    rows.append(f'| **Total** | **{s["level_bodies"]}** | **{s["normalized_shapes"]} distinct across levels** | **{s["null_entries"]}** | **{s["code_bytes"]}** |')
    spec=(HERE/'SPEC.md').read_text()
    spec=spec.split('<!-- LEVEL_COUNTS -->')[0]+'<!-- LEVEL_COUNTS -->\n\n'+'\n'.join(rows)+'\n'
    s=p['summary']
    spec+=f'\nFinal proof: **{s["executions"]:,} executions**, **{s["checked_pixel_writes"]:,} byte writes** from **{s["checked_store_instructions"]:,} store instructions**; '
    spec+=f'{s["valid_phase_runs"]:,} valid-phase and {s["out_of_range_phase_runs"]:,} out-of-range-phase runs. Both negative controls rejected.\n'
    (HERE/'SPEC.md').write_text(spec)
    # Round-trip EVERY exported mask/pixel pair in memory. Do not store bulk tile data.
    n=0
    for l in levels():
        native=export_level(l)
        assert native['callback_tile_count']==len(l['bodies'])
        for tile in native['callback_tiles']:
            pixels=bytes.fromhex(tile['pixels_hex']);mask=bytes.fromhex(tile['write_mask_hex'])
            assert len(pixels)==256 and len(mask)==32
            image=parse(l['bodies'][l['offsets'][tile['tile_id']]])['image']
            for y in range(16):
                row=int.from_bytes(mask[y*2:y*2+2],'little')
                for x in range(16):
                    at=y*16+x
                    assert bool(row&(1<<x))==(image[at] is not None)
                    assert pixels[at]==(image[at] or 0)
            n+=1
    result=dict(result='PASS',exported_tile_roundtrips=n,catalogue_proof_audit_consistent=True)
    (HERE/'delivery_check.json').write_text(json.dumps(result,indent=2)+'\n')
    files={f.name:dict(bytes=f.stat().st_size,sha256=sha(f.read_bytes()))
           for f in HERE.iterdir() if f.is_file() and f.name not in ('manifest.json','ledger.jsonl')}
    oracle=json.loads((ROOT/'config/oracles.json').read_text())
    manifest=dict(files=files,total_file_bytes=sum(v['bytes'] for v in files.values()),
                  oracle=oracle['oracles'][oracle['primary']],
                  evidence_source_sha256={name:sha((ROOT/name).read_bytes()) for name in
                       ['src/load.c','src/frame.c','src/map.c','src/cheat.c','src/include/map.h','src/lowlevel_04430.asm']})
    (HERE/'manifest.json').write_text(json.dumps(manifest,indent=1)+'\n')
    with (HERE/'ledger.jsonl').open('a') as f:
        f.write(json.dumps(dict(source_hashes={name:r['sha256'] for name,r in files.items() if name.endswith('.py')},
                                edit='Final exhaustive grammar, independent bitmap audit, Unicorn proof and exporter checks',
                                status='PASS',byte_diffs=0,counts=p['summary']))+'\n')
    print(json.dumps(result,indent=2))
    print('worker bytes',sum(f.stat().st_size for f in HERE.rglob('*') if f.is_file()))

if __name__=='__main__':main()
