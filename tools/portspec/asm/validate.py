"""Fail-closed artifact and provenance validation; no historical compilation."""
import json,hashlib,sys,datetime
from pathlib import Path
sys.dont_write_bytecode=True
from prove import MODULES,bundle_hashes
from models import MODELS,DETAILS
from engine import HERE,ROOT,Oracle
for module in MODULES:__import__(module)

def main():
    data=json.loads((HERE/'asm_spec.json').read_text());entries=data['entries'];hashes=bundle_hashes()
    assert len(entries)==118 and len({f['name'] for f in entries})==118
    assert sum(not f.get('private') for f in entries)==109
    assert sum(bool(f.get('private')) for f in entries)==9
    assert set(MODELS)=={f['name'] for f in entries}
    assert data['script_sha256s']==hashes
    cases=0;covered=0;total=0;hist=0
    for f in entries:
        p=json.loads((HERE/'results'/(f['name']+'.json')).read_text())
        assert f['proof']==p
        assert p['status']=='EQUIVALENT_TESTED' and not p['mismatches']
        assert p['cases']==p['requested_cases'] and p['cases']>=256
        assert p['seed_end']-p['seed_start']+1==p['cases']
        assert p['script_sha256s']==hashes,('stale proof scripts',f['name'])
        for k in ('all_GPRs','all_written_bytes_including_stack','written_byte_address_sets','exact_IO_sequence'):assert p['comparison'][k]
        b=p['basic_blocks'];assert 0<b['covered']<=b['total'];assert not b['unresolved_indirect_jumps']
        assert b['total']-b['covered']==len(b['uncovered'])
        assert f['description'] and f['domain'] and f['input_contract'] and f['output_contract']
        for layout in f['layouts']:
            fields=layout.get('fields',[])
            if isinstance(fields,list):
                for field in fields:assert all(k in field for k in ('offset','width','signed','meaning'))
        file=f['model'].split(':')[0];assert (HERE/file).exists()
        cases+=p['cases'];covered+=b['covered'];total+=b['total'];hist+=sum(len(x['mismatches']) for x in p.get('mismatch_history',[]))
    assert cases==data['counts']['cases'];assert covered==data['counts']['blocks_covered'];assert total==data['counts']['blocks_total']
    assert hist==data['counts']['retained_historical_mismatch_runs']
    for rel,digest in data['canonical_source_sha256s'].items():assert hashlib.sha256((ROOT/rel).read_bytes()).hexdigest()==digest,('source drift',rel)
    controls=json.loads((HERE/'controls.json').read_text());assert controls['status']=='PASS';assert controls['negative_control_count']==12
    assert controls['script_sha256s']==hashes
    assert all(x['status']=='EXPECTED_MISMATCH' for x in controls['negative_controls'])
    # Revalidate the decoded oracle hashes,without writing derived objects.
    oracle=Oracle();assert oracle.image.name==data['oracle']['name']
    tile=ROOT/'build/workers/tilecode/proof.json';assert hashlib.sha256(tile.read_bytes()).hexdigest()==data['tilecode_reuse']['proof_sha256']
    assert data['tilecode_reuse']['executions']==71400 and data['tilecode_reuse']['distinct_bodies']==2380
    markdown=(HERE/'ASM_SPEC.md').read_text(encoding='utf-8')
    for f in entries:assert '\n## '+f['name']+'\n' in markdown
    files=[p for p in HERE.rglob('*') if p.is_file()];size=sum(p.stat().st_size for p in files)
    assert size<300*1024*1024
    assert all(p.suffix in ('.py','.json','.md') for p in files),('unexpected bulk files',[str(p) for p in files if p.suffix not in ('.py','.json','.md')])
    out=dict(status='PASS',timestamp_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),counts=data['counts'],negative_controls=12,artifact_bytes=size,artifact_MiB=round(size/1024**2,3),source_hashes_checked=len(data['canonical_source_sha256s']),all_current_model_and_harness_hashes_match=True,outputs_only_worker_artifacts=True,universal_formal_equivalence_claimed=False)
    (HERE/'validation.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
if __name__=='__main__':main()
