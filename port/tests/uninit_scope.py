"""Measure the runtime scope of the uninitialized-locals policy (port/docs/DIVERGENCES.md).

The historical units start every local at zero (RAY_UNINIT_LOCALS=zero, the supported build). A second build with
RAY_UNINIT_LOCALS=pattern starts them at 0xFE bytes instead. Where a run of the two builds diverges, some original
path read a local it never wrote and its value reached the game state or the image: the policy is what made that
read deterministic. Identical runs mean the demos never let such a value reach the state or the image.

    python port/tests/uninit_scope.py ZERO_RAYMAN_EXE PATTERN_RAYMAN_EXE DATA_DIR CD_CUE [FRAMES]

Reports, per original demo, the per-swap gameplay-state logs and the presented-frame logs of both builds: identical
or the first differing swap/frame. A measurement, not a gate.
"""
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path


def run(exe, data, cd, demo, work, tag, frames):
    state, log = work / f'{tag}{demo}.state', work / f'{tag}{demo}.frames'
    subprocess.run([exe, '--data', data, '--cd', cd, '--save', str(work / f'save{tag}{demo}'), '--demo', str(demo),
                    '--headless', '--deterministic', '--frames', str(frames), '--state-log', str(state),
                    '--frame-log', str(log)], capture_output=True, timeout=1800)
    return state.read_text().splitlines(), [line.split()[2] for line in log.read_text().splitlines()]


def first_difference(a, b):
    for i, (x, y) in enumerate(zip(a, b)):
        if x != y:
            return i
    return None if len(a) == len(b) else min(len(a), len(b))


def main(argv):
    zero, pattern, data, cd = str(Path(argv[1]).resolve()), str(Path(argv[2]).resolve()), argv[3], argv[4]
    frames = int(argv[5]) if len(argv) > 5 else 6000
    work = Path(tempfile.mkdtemp(prefix='rayman-uninit-'))
    try:
        for demo in range(6):
            zs, zf = run(zero, data, cd, demo, work, 'zero', frames)
            ps, pf = run(pattern, data, cd, demo, work, 'pattern', frames)
            ds, df = first_difference(zs, ps), first_difference(zf, pf)
            print(f'demo{demo}: swaps={len(zs)}/{len(ps)} state ' +
                  ('identical' if ds is None else f'differs from swap {ds}') +
                  f'; frames={len(zf)}/{len(pf)} image ' + ('identical' if df is None else f'differs from frame {df}'))
    finally:
        shutil.rmtree(work, ignore_errors=True)
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
