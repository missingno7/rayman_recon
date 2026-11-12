"""Gate: the native renderer reproduces whole original runs on its own, every presented image.

For each scenario of SCENARIOS (the six original demos, the menu path left idle until it starts its own demo, and the
key scripts of port/tests/keys: a new game to world 1 level 1, the options menu, the in-game menu, pause, the credits,
a new game holding Right from the first level of each world (RAYMAN_START_LEVEL: deaths, the continue screen with its
countdown, game over, the menus and a demo; levels of worlds 1-6); with the movie data also the intro movie), one run presenting the reference (original draw path) and one presenting
only the native renderer (--renderer native), both headless and deterministic, both with --verify-render:
  * the presented-frame logs (image + DAC hash of every presented frame) of the two runs are identical: the whole run
    (start-up logo, title animation, menus with their plasma boxes and deformed name letters, world map binoculars,
    world vignettes and their transition effects, gameplay) looks the same;
  * every frame is complete (each operation drawn natively, nothing lost) and the native draw buffer and native
    display equal the original draw buffer and display over the full 320x200 image, and every presented image
    equals the original display;
  * every gameplay pass (background, map, objects, ...) matches the original buffer changes over that pass;
  * the gameplay-state logs (one digest per frame swap) of the two runs are identical: the presentation backend
    does not change the simulation.
The verification renders natively from the recorded frame alone: no pixel of the original buffers enters the
native surfaces (port/docs/RENDERER.md, "Verification").

    python port/tests/e2e_native.py RAYMAN_EXE DATA_DIR CD_CUE [MOVIE_DATA_DIR]
"""
import concurrent.futures
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

FRAMES = 6000
GAMEPLAY_PASSES = ('frame_start', 'background', 'rear_weather', 'map', 'stars', 'objects', 'particles', 'front',
                   'front_weather', 'firefly', 'fairy_text', 'hud', 'time')


EXE = [None]
KEYS = Path(__file__).resolve().parent / 'keys'
SCENARIOS = [(f'demo{n}', ['--demo', str(n)], FRAMES) for n in range(6)] + [
    ('menus', ['--menus'], FRAMES),
    # START, a new game in the first slot (name entry: three letters), the world map, then world 1 level 1
    ('new_game', ['--menus', '--keys', str(KEYS / 'start_game.txt')], FRAMES),
    # Down, Enter: the options menu (music, sound gauge, key setup)
    ('options_menu', ['--menus', '--keys', str(KEYS / 'options_menu.txt')], 3000),
    # the new game, then in world 1 level 1: P twice, Escape (the in-game menu over a greyscale capture of the
    # frame), Escape
    ('ingame_menu', ['--menus', '--keys', str(KEYS / 'ingame_menu.txt')], 7500),
    # the new game, then the Pause key twice: the pause screen (a spinning snapshot of the frame)
    ('pause', ['--menus', '--keys', str(KEYS / 'pause.txt')], 7500),
    # Down, Down, Enter: the credits
    ('credits', ['--menus', '--keys', str(KEYS / 'credits.txt')], 6000),
    # the new game under other start-up clocks: the vignette transitions are chosen by the game's random table,
    # seeded from the clock (3000: the row wipe; 4000: a segment-rasterizer transition)
    ('start_epoch3000', ['--menus', '--epoch', '3000', '--keys', str(KEYS / 'start_game.txt')], 3700),
    ('start_epoch4000', ['--menus', '--epoch', '4000', '--keys', str(KEYS / 'start_game.txt')], 3700),
] + [
    # a new game, then Right held from world W's first level (RAYMAN_START_LEVEL picks it on the world map): the
    # level's layers and enemies, Rayman's deaths, the continue screen and its countdown, game over, the menus
    (f'walk_world{w}', ['--menus', '--keys', str(KEYS / 'walk_right.txt')], 9000 if w == 1 else 7000,
     {'RAYMAN_START_LEVEL': f'{w} 1'}) for w in range(1, 7)
]
# With the movie data (optional fourth argument): the intro movie, the world map and its fire-animated quit box.
MOVIE_SCENARIO = ('movie', ['--menus', '--keys', str(KEYS / 'movie_menu.txt')], 5000)


def run(exe, data, cd, demo, args, frames, work, native, env=None):
    tag = 'native' if native else 'reference'
    cmd = [exe, '--data', data, '--cd', cd, '--save', str(work / f'save{tag}{demo}')] + args + [
           '--headless', '--deterministic', '--frames', str(frames), '--state-log', str(work / f'{tag}{demo}.state'),
           '--frame-log', str(work / f'{tag}{demo}.frames'), '--verify-render']
    cmd += ['--renderer', 'native' if native else 'reference']
    full_env = dict(os.environ, **env) if env else None
    return subprocess.run(cmd, capture_output=True, text=True, timeout=1800, env=full_env).stdout


def check(out, demo, tag, problems):
    frames = re.search(r'render frames: gameplay=(\d+) gameplay_complete=(\d+) gameplay_draw_exact=(\d+) '
                       r'gameplay_display_exact=(\d+) gameplay_complete_exact=(\d+)', out)
    if not frames:
        problems.append(f'{demo} {tag}: no render report')
        return
    gameplay, complete, draw, display, both = map(int, frames.groups())
    other = re.search(r'other=(\d+) other_complete=(\d+) other_draw_exact=(\d+) other_display_exact=(\d+)', out)
    presents = re.search(r'presents=(\d+) present_exact=(\d+)', out)
    print(f'{demo} {tag}: gameplay={gameplay} complete={complete} draw_exact={draw} display_exact={display} '
          f'complete_exact={both}; {other.group(0) if other else "no other-frame report"}; '
          f'{presents.group(0) if presents else "no present report"}')
    if not gameplay == complete == draw == display == both:
        problems.append(f'{demo} {tag}: native gameplay frames differ from the original or are incomplete')
    if not other or len(set(other.groups())) != 1 or gameplay + int(other.group(1)) == 0:
        problems.append(f'{demo} {tag}: native frames outside gameplay differ from the original or are incomplete')
    if not presents or presents.group(1) != presents.group(2):
        problems.append(f'{demo} {tag}: a presented native image differs from the original display')
    for name, bad in re.findall(r'render pass (\w+): segments=\d+ bad_segments=(\d+)', out):
        if name in GAMEPLAY_PASSES and bad != '0':
            problems.append(f'{demo} {tag}: pass {name} differs in {bad} frames')


def main(argv):
    exe, data, cd = str(Path(argv[1]).resolve()), argv[2], argv[3]
    EXE[0] = exe
    scenarios = [s if len(s) == 4 else s + (None,) for s in SCENARIOS]
    if len(argv) > 4 and argv[4]:
        name, args, frames = MOVIE_SCENARIO
        scenarios.append((name, ['--data', argv[4]] + args, frames, None))
    work = Path(tempfile.mkdtemp(prefix='rayman-native-'))
    problems = []
    try:
        # the runs are independent processes (own save directories and logs): four at a time
        jobs = [(s, native) for s in scenarios for native in (False, True)]
        with concurrent.futures.ThreadPoolExecutor(max_workers=4) as pool:
            outs = list(pool.map(lambda j: run(EXE[0], data, cd, j[0][0], j[0][1], j[0][2], work, j[1], j[0][3]),
                                 jobs))
        for ((demo, args, frames, env), native), out in zip(jobs, outs):
            check(out, demo, 'native' if native else 'reference', problems)
        for demo, args, frames, env in scenarios:
            ref = (work / f'reference{demo}.state').read_text().splitlines()
            nat = (work / f'native{demo}.state').read_text().splitlines()
            print(f'{demo}: state swaps reference={len(ref)} native={len(nat)} identical={ref == nat}')
            if ref != nat or not ref:
                problems.append(f'{demo}: the native presentation changed the gameplay-state log')
            ref_frames = [line.split()[2] for line in (work / f'reference{demo}.frames').read_text().splitlines()]
            nat_frames = [line.split()[2] for line in (work / f'native{demo}.frames').read_text().splitlines()]
            print(f'{demo}: presented frames reference={len(ref_frames)} native={len(nat_frames)} '
                  f'identical={ref_frames == nat_frames}')
            if ref_frames != nat_frames or not ref_frames:
                problems.append(f'{demo}: the native presentation shows different images')
    finally:
        shutil.rmtree(work, ignore_errors=True)
    for p in problems:
        print('FAIL:', p)
    return 1 if problems else 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
