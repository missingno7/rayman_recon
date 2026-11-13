"""Gate: the native renderer's wider view changes nothing but the presentation.

For each original demo, one run presenting the original view (reference, 320 columns) and one each with --wide-pad 54
(the native renderer drawing 54 more columns per side) and --wide-pad 160 (the widest view: a window resting at a
level edge reaches 320 columns past the original screen), all headless and deterministic:
  * the gameplay-state logs (one digest per frame swap: camera, clock, random table, Rayman and every object) are
    identical: a wider view consumes no random numbers and changes no game state;
  * every dumped wide frame is 320 + 2 * pad columns wide;
  * the margins hold drawn pixels in gameplay frames (the world, objects, weather extend past the original 320
    columns), except where they must stay black: demo 3, a firefly level, whose darkness covers them like the unlit
    rest of the screen, and demo 4, a vertical-scrolling level whose map is 320 pixels wide (the view centres on
    the map, black beyond its edges).
    The dark check covers the rows below the status bar (from row HUD_ROWS): the HUD is anchored to the view's
    corners, so its elements (DISPLAY_FIXE: rows 0..49) are in the margins in every level.
    At the widest pad the dark check covers demo 4 only: there a window resting at a level's left edge moves the
    original screen itself into the outer columns (dx up to 2 * pad), and demo 3's firefly circle is lit there.
Then the in-game menu (port/tests/keys/ingame_menu.txt: a new game, Escape in world 1 level 1) at pad 54: the same
gameplay-state log as the original view, and the menu's greyscale background covers the margins (the capture of the
wider view, user decision 2026-11-13): at least MENU_FRAMES dumped frames whose margins are drawn and all grey.
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
PADS = (54, 160)
DARK_MARGIN_DEMOS = {3, 4}
MENU_FRAMES = 5              # dumps (every MENU_DUMP_EVERY of 7500 frames) inside the in-game menu
MENU_DUMP_EVERY = 50
HUD_ROWS = 50                # status bar band (render/native.c hud_anchor); below it dark margins must stay black   # demo 3: firefly darkness; demo 4: a map 320 pixels wide (black beyond its edges)


def run(exe, data, cd, demo, work, pad, source=None, frames=FRAMES, every=DUMP_EVERY):
    tag = f'wide{pad}' if pad else 'base'
    dumps = work / f'{tag}{demo}'
    dumps.mkdir()
    cmd = [exe, '--data', data, '--cd', cd, '--save', str(work / f'save{tag}{demo}')]
    cmd += source if source else ['--demo', str(demo)]
    cmd += ['--headless', '--deterministic', '--frames', str(frames), '--state-log', str(work / f'{tag}{demo}.state')]
    if not pad:
        cmd += ['--renderer', 'reference']
    if pad:
        cmd += ['--wide-pad', str(pad), '--dump-frames', str(dumps), '--dump-every', str(every)]
    subprocess.run(cmd, capture_output=True, text=True, timeout=1800)
    return dumps


def bmp(path):
    data = path.read_bytes()
    width = int.from_bytes(data[18:22], 'little')
    height = int.from_bytes(data[22:26], 'little')
    off = int.from_bytes(data[10:14], 'little')
    return width, height, data[off:off + width * height]


def palette(path):
    data = path.read_bytes()
    return [(data[54 + 4 * i + 2], data[54 + 4 * i + 1], data[54 + 4 * i]) for i in range(256)]


def menu_check(exe, data, cd, work, problems):
    pad = PADS[0]
    keys = Path(__file__).resolve().parent / 'keys' / 'ingame_menu.txt'
    source = ['--menus', '--keys', str(keys)]
    run(exe, data, cd, 'menu', work, 0, source, 7500)
    dumps = run(exe, data, cd, 'menu', work, pad, source, 7500, MENU_DUMP_EVERY)
    base = (work / 'basemenu.state').read_text().splitlines()
    wide = (work / f'wide{pad}menu.state').read_text().splitlines()
    common = min(len(base), len(wide))
    if not common or base[:common] != wide[:common]:
        problems.append(f'ingame menu pad{pad}: the wider view changed the gameplay-state log')
    grey = 0
    for f in sorted(dumps.glob('*.bmp')):
        w, h, px = bmp(f)
        pal = palette(f)
        colours = [pal[px[y * w + x]] for y in range(h) for x in list(range(pad)) + list(range(pad + 320, w))]
        if any(c != (0, 0, 0) for c in colours) and all(max(c) - min(c) <= 8 for c in colours):
            grey += 1
    print(f'ingame menu pad{pad}: state swaps base={len(base)} wide={len(wide)} frames_with_grey_margins={grey}')
    if grey < MENU_FRAMES:
        problems.append(f'ingame menu pad{pad}: the menu background does not cover the margins')


def main(argv):
    exe, data, cd = str(Path(argv[1]).resolve()), argv[2], argv[3]
    work = Path(tempfile.mkdtemp(prefix='rayman-wide-'))
    problems = []
    try:
        for demo in range(6):
            run(exe, data, cd, demo, work, 0)
            base = (work / f'base{demo}.state').read_text().splitlines()
            for pad in PADS:
                dumps = run(exe, data, cd, demo, work, pad)
                wide = (work / f'wide{pad}{demo}.state').read_text().splitlines()
                common = min(len(base), len(wide))
                same_state = common > 0 and base[:common] == wide[:common]
                widths = set()
                margin_frames = margin_px = frames = 0
                rows0 = HUD_ROWS if demo in DARK_MARGIN_DEMOS else 0
                for f in sorted(dumps.glob('*.bmp')):
                    w, h, px = bmp(f)
                    widths.add(w)
                    frames += 1
                    # the BMP is stored bottom-up: image row y is data row h - 1 - y
                    n = sum(1 for y in range(rows0, h) for x in list(range(pad)) + list(range(pad + 320, w))
                            if px[(h - 1 - y) * w + x])
                    margin_px += n
                    margin_frames += n > 0
                print(f'demo{demo} pad{pad}: state swaps base={len(base)} wide={len(wide)} identical_prefix={same_state} '
                      f'dumps={frames} widths={sorted(widths)} frames_with_margins={margin_frames} margin_px={margin_px}')
                if not same_state:
                    problems.append(f'demo{demo} pad{pad}: the wider view changed the gameplay-state log')
                if widths != {320 + 2 * pad}:
                    problems.append(f'demo{demo} pad{pad}: wide frames are not {320 + 2 * pad} columns wide')
                if demo in DARK_MARGIN_DEMOS and (pad == PADS[0] or demo == 4):
                    if margin_frames:
                        problems.append(f'demo{demo} pad{pad}: the margins should stay black')
                elif demo not in DARK_MARGIN_DEMOS and margin_frames * 2 < frames:
                    problems.append(f'demo{demo} pad{pad}: the margins are empty in most frames')
        menu_check(exe, data, cd, work, problems)
    finally:
        shutil.rmtree(work, ignore_errors=True)
    for p in problems:
        print('FAIL:', p)
    return 1 if problems else 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
