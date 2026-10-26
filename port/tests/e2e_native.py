"""Gate: the native renderer reproduces every gameplay frame of the original demos on its own.

For each original demo, one run presenting the reference (original draw path) and one presenting only the native
renderer (--renderer native), both headless and deterministic, both with --verify-render:
  * every gameplay frame is complete (each operation drawn natively, nothing lost) and the native draw buffer and
    native display equal the original draw buffer and display over the full 320x200 image;
  * every gameplay pass (background, map, objects, ...) matches the original buffer changes over that pass;
  * the gameplay-state logs (one digest per frame swap) of the two runs are identical: the presentation backend
    does not change the simulation.
The verification renders natively from the recorded frame alone: no pixel of the original buffers enters the
native surfaces (port/docs/RENDERER.md, "Verification").

    python port/tests/e2e_native.py RAYMAN_EXE DATA_DIR CD_CUE
"""
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

FRAMES = 6000
GAMEPLAY_PASSES = ('frame_start', 'background', 'rear_weather', 'map', 'stars', 'objects', 'particles', 'front',
                   'front_weather', 'firefly', 'fairy_text', 'hud', 'time')


def run(exe, data, cd, demo, work, native):
    tag = 'native' if native else 'reference'
    cmd = [exe, '--data', data, '--cd', cd, '--save', str(work / f'save{tag}{demo}'), '--demo', str(demo),
           '--headless', '--deterministic', '--frames', str(FRAMES), '--state-log', str(work / f'{tag}{demo}.state'),
           '--verify-render']
    if native:
        cmd += ['--renderer', 'native']
    return subprocess.run(cmd, capture_output=True, text=True, timeout=1800).stdout


def check(out, demo, tag, problems):
    frames = re.search(r'render frames: gameplay=(\d+) gameplay_complete=(\d+) gameplay_draw_exact=(\d+) '
                       r'gameplay_display_exact=(\d+) gameplay_complete_exact=(\d+)', out)
    if not frames:
        problems.append(f'demo{demo} {tag}: no render report')
        return
    gameplay, complete, draw, display, both = map(int, frames.groups())
    print(f'demo{demo} {tag}: gameplay={gameplay} complete={complete} draw_exact={draw} display_exact={display} '
          f'complete_exact={both}')
    if gameplay == 0 or not gameplay == complete == draw == display == both:
        problems.append(f'demo{demo} {tag}: native gameplay frames differ from the original or are incomplete')
    for name, bad in re.findall(r'render pass (\w+): segments=\d+ bad_segments=(\d+)', out):
        if name in GAMEPLAY_PASSES and bad != '0':
            problems.append(f'demo{demo} {tag}: pass {name} differs in {bad} frames')


def main(argv):
    exe, data, cd = str(Path(argv[1]).resolve()), argv[2], argv[3]
    work = Path(tempfile.mkdtemp(prefix='rayman-native-'))
    problems = []
    try:
        for demo in range(6):
            check(run(exe, data, cd, demo, work, False), demo, 'reference', problems)
            check(run(exe, data, cd, demo, work, True), demo, 'native', problems)
            ref = (work / f'reference{demo}.state').read_text().splitlines()
            nat = (work / f'native{demo}.state').read_text().splitlines()
            print(f'demo{demo}: state swaps reference={len(ref)} native={len(nat)} identical={ref == nat}')
            if ref != nat or not ref:
                problems.append(f'demo{demo}: the native presentation changed the gameplay-state log')
    finally:
        shutil.rmtree(work, ignore_errors=True)
    for p in problems:
        print('FAIL:', p)
    return 1 if problems else 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
