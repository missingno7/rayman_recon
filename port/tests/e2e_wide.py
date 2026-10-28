"""Gate: the native renderer's wider view changes nothing but the presentation.

For each original demo, one run presenting the original view (reference, 320 columns) and one with --widescreen
(the native renderer drawing 54 more columns per side), both headless and deterministic:
  * the gameplay-state logs (one digest per frame swap: camera, clock, random table, Rayman and every object) are
    identical: a wider view consumes no random numbers and changes no game state;
  * every dumped wide frame is 428 columns wide;
  * the margins hold drawn pixels in gameplay frames (the world, objects, weather extend past the original 320
    columns). In demo 3, a firefly level, the darkness covers the margins like the unlit rest of the screen.
The native renderer's exactness in the original view is gated separately (tests/e2e_native.py).

    python port/tests/e2e_wide.py RAYMAN_EXE DATA_DIR CD_CUE
"""
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

FRAMES = 6000
DUMP_EVERY = 97
PAD = 54
DARK_MARGIN_DEMOS = {3}   # world 5 level 4: the firefly darkness covers everything outside the lit window


def run(exe, data, cd, demo, work, wide):
    tag = 'wide' if wide else 'base'
    dumps = work / f'{tag}{demo}'
    dumps.mkdir()
    cmd = [exe, '--data', data, '--cd', cd, '--save', str(work / f'save{tag}{demo}'), '--demo', str(demo),
           '--headless', '--deterministic', '--frames', str(FRAMES), '--state-log', str(work / f'{tag}{demo}.state')]
    if wide:
        cmd += ['--widescreen', '--dump-frames', str(dumps), '--dump-every', str(DUMP_EVERY)]
    subprocess.run(cmd, capture_output=True, text=True, timeout=1800)
    return dumps


def bmp(path):
    data = path.read_bytes()
    width = int.from_bytes(data[18:22], 'little')
    height = int.from_bytes(data[22:26], 'little')
    off = int.from_bytes(data[10:14], 'little')
    return width, height, data[off:off + width * height]


def main(argv):
    exe, data, cd = str(Path(argv[1]).resolve()), argv[2], argv[3]
    work = Path(tempfile.mkdtemp(prefix='rayman-wide-'))
    problems = []
    try:
        for demo in range(6):
            run(exe, data, cd, demo, work, False)
            dumps = run(exe, data, cd, demo, work, True)
            base = (work / f'base{demo}.state').read_text().splitlines()
            wide = (work / f'wide{demo}.state').read_text().splitlines()
            common = min(len(base), len(wide))
            same_state = common > 0 and base[:common] == wide[:common]
            widths = set()
            margin_frames = margin_px = frames = 0
            for f in sorted(dumps.glob('*.bmp')):
                w, h, px = bmp(f)
                widths.add(w)
                frames += 1
                n = sum(1 for y in range(h) for x in list(range(PAD)) + list(range(PAD + 320, w)) if px[y * w + x])
                margin_px += n
                margin_frames += n > 0
            print(f'demo{demo}: state swaps base={len(base)} wide={len(wide)} identical_prefix={same_state} '
                  f'dumps={frames} widths={sorted(widths)} frames_with_margins={margin_frames} margin_px={margin_px}')
            if not same_state:
                problems.append(f'demo{demo}: the wider view changed the gameplay-state log')
            if widths != {320 + 2 * PAD}:
                problems.append(f'demo{demo}: wide frames are not {320 + 2 * PAD} columns wide')
            if demo not in DARK_MARGIN_DEMOS and margin_frames * 2 < frames:
                problems.append(f'demo{demo}: the margins are empty in most frames')
    finally:
        shutil.rmtree(work, ignore_errors=True)
    for p in problems:
        print('FAIL:', p)
    return 1 if problems else 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
