"""Original menu -> intro -> world map -> main menu, missing-file skip, and demo exclusion.

Uses the player's files in place. The same test and evidence run on both pointer widths. No golden updates.
Optional RAYMAN_E2E_EVIDENCE retains text observations; --dump-frames remains available for visual inspection.
"""
import collections
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent


def require(condition, message):
    if not condition:
        raise AssertionError(message)


def fields(line):
    return dict(v.split("=", 1) for v in line.split() if "=" in v)


def main(argv):
    exe, primary, movies, cd = argv[1:5]
    results = {}
    evidence = os.environ.get("RAYMAN_E2E_EVIDENCE")
    with tempfile.TemporaryDirectory(prefix="rayman-movies-") as work:
        root = Path(work)

        def run(name, roots, entry, frames, keys):
            folder = root / name
            folder.mkdir()
            env = os.environ.copy()
            env.update(RAYMAN_MOVIE_TRACE=str(folder / "movie.txt"), RAYMAN_CD_TRACE=str(folder / "cd.txt"),
                       RAYMAN_SOUND_TRACE=str(folder / "sound.txt"))
            cmd = [exe, "--save", str(folder / "save"), "--cd", cd, "--headless", "--deterministic",
                   "--frames", str(frames), "--frame-log", str(folder / "frames.txt"), *entry]
            for data in roots:
                cmd += ["--data", data]
            if keys:
                cmd += ["--keys", str(HERE / "keys" / keys)]
            proc = subprocess.run(cmd, env=env, capture_output=True, text=True, timeout=180)
            out = {kind: (folder / f"{kind}.txt").read_text() if (folder / f"{kind}.txt").exists() else ""
                   for kind in ("movie", "cd", "frames", "sound")}
            out.update(stdout=proc.stdout, stderr=proc.stderr)
            if evidence:
                dest = Path(evidence)
                dest.mkdir(parents=True, exist_ok=True)
                for kind, text in out.items():
                    (dest / f"movie-{name}.{kind}.txt").write_text(text)
            require(proc.returncode == 0, f"{name} exit {proc.returncode}: {proc.stderr}")
            require("CarteSonAutorisee=1" in proc.stdout, f"{name}: HMI disabled")
            return out

        intro = run("intro", [primary, movies], ["--menus"], 5000, "movie_menu.txt")
        trace = intro["movie"].splitlines()
        frames = [l for l in trace if l.startswith("frame ")]
        require(len(frames) == 1804, f"intro decoded {len(frames)} frames")
        require([int(l.split()[2]) for l in frames] == list(range(1, 1805)), "intro frame indices")
        require(all(fields(l)["selected"] == "1" and fields(l)["size"] == "240x150" for l in frames),
                "intro selection/geometry")
        require(trace[0].endswith("intro.dat track=20 loop=0") and trace[-1].endswith("status=0"), "intro result")
        start, end = int(trace[0].split()[1]), int(trace[-1].split()[1])
        fade = int(next(l for l in trace if l.startswith("fade ")).split()[1])
        times = [int(l.split()[1]) for l in frames]
        require(119_000_000_000 < times[-1] - times[0] < 122_000_000_000, "intro pacing domain")
        require(1_000_000_000 < end - fade < 2_000_000_000, "final fade pacing")
        rows = []
        previous = None
        for line in frames:
            state = (int(fields(line)["row"]), int(fields(line)["state"]))
            if state != previous:
                rows.append([int(line.split()[2]), *state])
                previous = state
        require(rows == [[1, 0, 0], [223, 1, 0], [468, 2, 0], [920, 3, 0], [1195, 4, 0]],
                f"deterministic CD rows changed: {rows}")
        cd_commands = [l for l in intro["cd"].splitlines() if "cmd=" in l and start <= int(l.split()[0]) < end]
        commands = collections.Counter(fields(l)["cmd"] for l in cd_commands)
        require(commands["84"] == 1 and commands["88"] == 1, f"CD play/resume: {commands}")
        shown = [l.split() for l in intro["frames"].splitlines()]
        require(len(shown) == 5000 and int(shown[-1][1]) > end, "did not continue after the movie")
        # movie_menu.txt exits the world map via its original confirmation and returns to the main menu.
        # A paired x64/i686 full-log comparison and BMP inspection are recorded in VERIFICATION.md.
        results["intro"] = {
            "decoded": len(frames), "selected": len(frames), "start_ns": start, "end_ns": end,
            "first_to_last_ns": times[-1] - times[0], "fade_ns": end - fade,
            "changed_images_during_player": sum(start <= int(l[1]) <= end for l in shown),
            "changed_images_during_decode": sum(times[0] <= int(l[1]) <= fade for l in shown),
            "frame_intervals_ns": dict(collections.Counter(b-a for a, b in zip(times, times[1:]))),
            "rows": rows, "cd_commands": dict(commands), "last_image": " ".join(shown[-1]),
        }

        skip = run("missing", [primary], ["--menus"], 4200, "new_game.txt")
        trace = skip["movie"].splitlines()
        require(len(trace) == 2 and trace[0].endswith("intro.dat track=20 loop=0") and
                trace[1].endswith("status=-5"), f"missing file path: {trace}")
        require(trace[0].split()[1] == trace[1].split()[1], "missing movie advanced the clock")
        # The DOS root can also supply RAYMAN1.SAV: the ordered general search path deliberately exposes it.
        # Save selection may therefore differ before playback; do not claim identical menu walks.
        shared_startup = 0
        for a, b in zip(intro["frames"].splitlines(), skip["frames"].splitlines()):
            if a != b:
                break
            shared_startup += 1
        results["missing"] = {"status": -5, "shared_startup_frames": shared_startup,
                              "skip_clock_ns": int(trace[0].split()[1])}

        demo = run("demo", [primary, movies], ["--demo", "0"], 6000, None)
        require(not demo["movie"], "demo called the movie player")
        from e2e_demo import summary
        got = summary(demo["frames"])
        got["exit"] = 0
        want = json.loads((HERE / "golden" / "demo0.json").read_text())
        require(got == want, f"demo with movie search root changed: {got} != {want}")
        results["demo_with_movies"] = got
    print(json.dumps(results, indent=1))
    if evidence:
        (Path(evidence) / "movies.json").write_text(json.dumps(results, indent=1) + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
