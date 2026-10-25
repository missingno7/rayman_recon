"""Compare every effect frame from original i686, derived i686 and derived x64.
Usage: python compare_effect_runs.py REFERENCE_I686 DERIVED_I686 DERIVED_X64 [--out FILE]
The C driver hashes pixels/textures plus RNG state and records progress per frame.
"""
import argparse
import collections
import hashlib
import json
from pathlib import Path

NOTES = {
    'fragment': 'Both signs; 8x8 grid, rotations, gravity updates, sorting and completion.',
    'rows': 'All 12 modes, both signs; includes selector 4 alias, mode 9 immediate passes and clipping.',
    'wave': 'Both signs; ramp, propagation, silence and completion; fixed border vertices.',
    'sphere': 'Both signs; interpolation, sphere modes 1/2/0, bounce/rotation, drop and completion.',
    'panels': 'All 15 modes, both signs; three-axis rotation and hinge cases.',
    'radial': 'Both signs; displacement setup, movement, boundary reflections and completion.',
    'columns': 'Both signs; three remap phases, clear phase and zero-source fallback.',
    'random': '32 seeded selector-9/sentinel-100 calls through the loading/transition dispatch.',
    'pause': 'Normal pause, blocked toggle/hold/release, simple and FondAutorise=2 paths; both speed signs, timer, restore.',
    'band_save_restore': '320/256 pitch conversion in both directions and normal SaveScreen/RestoreScreen.',
    'blood': 'Three RNG seeds; every rendered frame through completion.',
}
EFFECTS = {1:'fragment', 2:'rows', 3:'wave', 4:'rows', 5:'sphere', 6:'panels', 7:'radial', 8:'columns'}

def group(case):
    if case.startswith('transition_'):
        return EFFECTS[int(case.split('_')[1])]
    if case.startswith('random_'): return 'random'
    if case.startswith('pause'): return 'pause'
    if case.startswith('blood_'): return 'blood'
    return case

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('runs', nargs=3, type=Path)
    p.add_argument('--out', type=Path)
    a=p.parse_args()
    data=[f.read_bytes() for f in a.runs]
    lines=[d.decode('ascii').splitlines() for d in data]
    assert len(lines[0])==len(lines[1])==len(lines[2]), 'frame counts differ'
    cases=collections.Counter()
    groups=collections.Counter()
    next_frame=collections.Counter()
    for i, (ref, b, c) in enumerate(zip(*lines)):
        assert ref==b==c, f'frame {i}: {ref!r} != {b!r} != {c!r}'
        fields=ref.split();case=fields[0]
        assert len(fields)==8, f'malformed record: {ref!r}'
        assert int(fields[1])==next_frame[case], f'missing frame in {case}'
        next_frame[case]+=1
        cases[case]+=1;groups[group(case)]+=1
    result={'status':'EQUIVALENT_TESTED','frames':len(lines[0]),'cases':len(cases),
            'all_frame_records_equal':True,
            'inputs':[{'path':str(f),'sha256':hashlib.sha256(d).hexdigest()} for f,d in zip(a.runs,data)],
            'effects':[{'effect':g,'frames':n,'a_equals_b_equals_c':True,'coverage':NOTES[g]} for g,n in groups.items()],
            'case_frames':dict(cases),
            'proof_boundary':'Finite deterministic native execution. The reference effect objects are unchanged historical C compiled with i686 pointer width; this is behavioral evidence, not a DOS byte-match or universal equivalence proof.'}
    if a.out:a.out.write_text(json.dumps(result,indent=1)+'\n')
    for x in result['effects']:print(f"{x['effect']}: {x['frames']} frames (a)=(b)=(c); {x['coverage']}")
    print(f"PASS: {result['frames']} frames, {result['cases']} cases, every progress/pixel/texture/RNG record equal")

if __name__=='__main__':main()
