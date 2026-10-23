"""Small reproducible pilot benchmark; search output is never promoted.

  python tools/permute_bench.py --out build/permute_benchmark --max-compiles 20

Six equal-budget searches (three functions x two modes) plus a serial/process
throughput control. Each throughput repeat freshly compiles and verifies the
same full TU, checks object SHA-256 agreement, and includes pool startup cost.
"""
from __future__ import annotations

import argparse
import json
import shutil
import subprocess
import sys
import time
from pathlib import Path

sys.dont_write_bytecode = True
from permute import atomic_json, contained, input_files, make_task, repo_root
from permute_backend import IsolatedProcess, digest, evaluate

PILOTS = [("collision1", "inter_box", "allocator_stack", "colour"),
          ("effect1", "SaveScreen", "order_lowering", "schedule"),
          ("ray", "RAY_BALANCE_ANIM", "score_test", "Score")]


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--out", required=True, type=Path)
    p.add_argument("--max-compiles", default=20, type=int)
    p.add_argument("--seconds", default=90, type=float)
    p.add_argument("--seed", default=20261023, type=int)
    p.add_argument("--throughput-compiles", default=12, type=int)
    p.add_argument("--throughput-repeats", default=2, type=int)
    args = p.parse_args(argv)
    root, out = repo_root(), args.out.resolve()
    if not contained(out, root / "build") or out == root / "build":
        p.error("output must be under build/")
    out.mkdir(parents=True, exist_ok=False)
    records = []
    for unit, fn, category, mechanism in PILOTS:
        arms = []
        for mode in ("conservative", "aggressive"):
            run_dir = out / f"{unit}_{mode}"
            cmd = [sys.executable, "-B", str(Path(__file__).with_name("permute.py")),
                   str(root / "src" / (unit + ".c")), fn, "--out", str(run_dir),
                   "--" + mode, "--seed", str(args.seed), "--max-compiles", str(args.max_compiles),
                   "--seconds", str(args.seconds), "--mechanism", mechanism]
            began = time.monotonic()
            run = subprocess.run(cmd, capture_output=True, text=True, timeout=args.seconds + 180)
            (out / f"{unit}_{mode}.log").write_text(run.stdout + run.stderr, encoding="utf-8")
            if run.returncode:
                raise RuntimeError(f"benchmark failed: {run_dir}: {run.stderr}")
            summary = json.loads((run_dir / "summary.json").read_text(encoding="utf-8"))
            state = json.loads((run_dir / "state.json").read_text(encoding="utf-8"))
            best_bytes, best_score = summary["diffs_before"], summary["score_before"]["total"]
            trajectory = [{"compiles": 2, "seconds": 0, "byte_diffs": best_bytes, "score": best_score}]
            for c in state["candidates"]:
                if c["eligible"] and (c["score"]["byte_diffs"] < best_bytes or c["score"]["total"] < best_score):
                    best_bytes = min(best_bytes, c["score"]["byte_diffs"])
                    best_score = min(best_score, c["score"]["total"])
                    trajectory.append({"compiles": c["compiles"], "seconds": c["elapsed"], "byte_diffs": best_bytes, "score": best_score})
            trajectory.append({"compiles": summary["compiles"], "seconds": summary["seconds"], "byte_diffs": best_bytes, "score": best_score})
            summary.update(trajectory=trajectory, external_wall_seconds=time.monotonic() - began,
                           command=cmd, output=str(run_dir))
            arms.append(summary)
            print(f"{fn} {mode}: {summary['diffs_before']} -> {best_bytes}; {summary['compiles']} compiles; {summary['seconds']:.2f}s", flush=True)
        winner = min(arms, key=lambda s: (not s["accepted"], s["diffs_best"], s["score_best"]["total"]))
        records.append({key: winner[key] for key in ("function", "unit", "status_before", "status_after", "diffs_before", "diffs_best", "quality", "transformation", "unit_strict_before", "unit_strict_after", "notes")})
        records[-1].update(category=category, compiles=sum(a["compiles"] for a in arms),
                           seconds=sum(a["seconds"] for a in arms), arms=arms)
    atomic_json(out / "RESULTS.json", records)
    # Compare process isolation on identical inputs, independent of search choices.
    source = root / "src/collision1.c"
    inp = out / "throughput_input"
    inp.mkdir()
    for rel, path in input_files(root, source).items():
        dest = inp / rel
        dest.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(path, dest)
    throughput = []
    settings = argparse.Namespace(profile="game", oracle="gog_v121_us", symbols=None)
    for repeat in range(args.throughput_repeats):
        for jobs in (1, 2, 4):
            d = out / f"tp{repeat}_{jobs}"
            d.mkdir()
            tasks = [make_task(root, inp / source.name, "inter_box", settings, d / str(i))
                     for i in range(args.throughput_compiles)]
            began = time.monotonic()
            if jobs == 1:
                results = [evaluate(t) for t in tasks]
            else:
                results = []
                for i in range(0, len(tasks), jobs):
                    workers = []
                    try:
                        for task in tasks[i:i + jobs]:
                            workers.append(IsolatedProcess(task))
                        results.extend(w.result() for w in workers)
                    finally:
                        for worker in workers:
                            worker.close()
            seconds = time.monotonic() - began
            hashes = [digest(Path(t["object"]).read_bytes()) for t in tasks]
            if any("error" in r for r in results) or len(set(hashes)) != 1:
                raise RuntimeError("parallel isolation control failed")
            row = {"repeat": repeat, "jobs": jobs, "compiles": len(tasks), "seconds": seconds,
                   "compiles_per_second": len(tasks) / seconds, "identical_objects": True,
                   "object_sha256": hashes[0], "identical_scores": len({r["score"]["total"] for r in results}) == 1}
            throughput.append(row)
            print(f"throughput repeat {repeat} jobs={jobs}: {row['compiles_per_second']:.2f} full compile+score/s", flush=True)
    atomic_json(out / "THROUGHPUT.json", throughput)
    return 0


if __name__ == "__main__":
    sys.exit(main())
