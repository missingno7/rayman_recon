"""Gate: a wider presentation viewport changes nothing but the margins.

For each original demo, one run at the original width and one with --widescreen (both headless, deterministic):
  * the frame-hash logs (original 320x200 image + DAC per presented frame) are identical,
  * the gameplay-state logs (camera, clock, random table, Rayman and every object) are identical,
  * every dumped wide frame holds the original frame's pixels unchanged in its central 320 columns,
  * margins were drawn by the semantic renderer for gameplay frames, except in demo 3: its firefly level narrows
    the game's own viewport to a small lit window, and the margins stay black like the unlit original screen.
Also runs --verify-render: the native renderer reproduces every gameplay frame (tests/e2e_native.py has the full gate).

The margin compositor is the reference-renderer experiment of renderer slice 1 (port/docs/RENDERER.md, section 7):
kept and tested until the native renderer draws wider viewports itself, then removed.

    python port/tests/e2e_wide.py RAYMAN_EXE DATA_DIR CD_CUE
"""
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

FRAMES = 6000
DUMP_EVERY = 97
PAD = 54
NARROW_VIEW_DEMOS = {3}   # world 5 level 4: firefly darkness narrows Bloc_lim to the lit window


def run(exe, data, cd, demo, work, wide):
    tag = 'wide' if wide else 'base'
    dumps = work / f'{tag}{demo}'
    dumps.mkdir()
    cmd = [exe, '--data', data, '--cd', cd, '--save', str(work / f'save{tag}{demo}'), '--demo', str(demo),
           '--headless', '--deterministic', '--frames', str(FRAMES), '--frame-log', str(work / f'{tag}{demo}.frames'),
           '--state-log', str(work / f'{tag}{demo}.state'), '--dump-frames', str(dumps), '--dump-every',
           str(DUMP_EVERY), '--verify-render']
    if wide:
        cmd.append('--widescreen')
    out = subprocess.run(cmd, capture_output=True, text=True, timeout=1800).stdout
    return out, dumps


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
            base_out, base_dumps = run(exe, data, cd, demo, work, False)
            wide_out, wide_dumps = run(exe, data, cd, demo, work, True)
            same_frames = (work / f'base{demo}.frames').read_text() == (work / f'wide{demo}.frames').read_text()
            same_state = (work / f'base{demo}.state').read_text() == (work / f'wide{demo}.state').read_text()
            nframes = len((work / f'base{demo}.frames').read_text().splitlines())
            centre_ok = centre_n = margin_px = 0
            for f in sorted(base_dumps.glob('*.bmp')):
                bw, bh, bp = bmp(f)
                ww, wh, wp = bmp(wide_dumps / f.name)
                centre_n += 1
                if bw == 320 and ww == 320 + 2 * PAD and bh == wh == 200 and \
                        all(wp[y * ww + PAD:y * ww + PAD + 320] == bp[y * 320:y * 320 + 320] for y in range(200)):
                    centre_ok += 1
                margin_px += sum(1 for y in range(200) for x in list(range(PAD)) + list(range(PAD + 320, ww))
                                 if wp[y * ww + x])
            verify = re.search(r'render frames: gameplay=(\d+) gameplay_complete=\d+ gameplay_draw_exact=\d+ '
                               r'gameplay_display_exact=\d+ gameplay_complete_exact=(\d+)', wide_out)
            margins = re.search(r'margins_drawn=(\d+) margins_black=(\d+)', wide_out)
            print(f'demo{demo}: frames={nframes} frame_log_identical={same_frames} state_log_identical={same_state} '
                  f'centre_exact={centre_ok}/{centre_n} margin_px={margin_px} '
                  f'{margins.group(0) if margins else "no margin stats"} '
                  f'verify={verify.group(0) if verify else "missing"}')
            if not (same_frames and same_state):
                problems.append(f'demo{demo}: wide presentation changed the frame or state log')
            if centre_ok != centre_n or centre_n == 0:
                problems.append(f'demo{demo}: centre of the wide frame differs from the original frame')
            if not verify or verify.group(1) != verify.group(2):
                problems.append(f'demo{demo}: native renderer differs from the original draw buffer')
            drawn = int(margins.group(1)) if margins else -1
            if demo in NARROW_VIEW_DEMOS:
                if drawn != 0:
                    problems.append(f'demo{demo}: margins drawn around a narrowed viewport')
            elif drawn <= 0 or margin_px == 0:
                problems.append(f'demo{demo}: no margins drawn')
    finally:
        shutil.rmtree(work, ignore_errors=True)
    for p in problems:
        print('FAIL:', p)
    return 1 if problems else 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
