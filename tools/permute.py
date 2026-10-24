"""Reproducible, bounded, guarded source search for the pinned Watcom pipeline.

  python tools/permute.py src/collision1.c inter_box --out build/permute/inter_box 
      --conservative --seed 42 --max-compiles 100 --seconds 300 --mechanism colour
  python tools/permute.py ... --aggressive --jobs 2
  python tools/permute.py ... --resume --max-compiles 200 --seconds 600

Budget counts ALL compiler invocations: preprocessing, baseline, candidates, and
fresh hit checks, including failed/interrupted attempts. --seconds is cumulative
admission time: already running builds finish (wcc bounds each at 120 seconds).
Only fresh full-unit match.verify_object STRICT + zero baseline STRICT losses +
unchanged non-code data/public/alignment invariants can produce accepted.c.

pycparser >= 2.21 and capstone are required. No other permuter dependencies.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import platform
import random
import re
import shutil
import sys
import time
from pathlib import Path

sys.dont_write_bytecode = True
from permute_backend import IsolatedProcess, WorkerPool, assess, digest, evaluate, load_pipeline
from permute_source import Model, Refusal, body_bounds, mutate

SCHEMA = 1


def repo_root(explicit=None):
    if explicit:
        candidates = [Path(explicit).resolve()]
    else:
        candidates = list(Path(__file__).resolve().parents)
    for p in candidates:
        if (p / "config/toolchain.json").is_file() and (p / "tools/wcc.py").is_file():
            return p
    raise Refusal("cannot find repository root; use --repo-root")


def contained(path, parent):
    return path.resolve().is_relative_to(parent.resolve())


def atomic_json(path, value):
    path = Path(path)
    tmp = path.with_suffix(path.suffix + ".tmp")
    with tmp.open("w", encoding="utf-8", newline="\n") as f:
        json.dump(value, f, indent=2)
        f.write("\n")
        f.flush()
        os.fsync(f.fileno())
    os.replace(tmp, path)


def append_event(out, event):
    with (out / "candidates.jsonl").open("a", encoding="utf-8", newline="\n") as f:
        f.write(json.dumps(event, sort_keys=True) + "\n")
        f.flush()
        os.fsync(f.fileno())


def input_files(root, source):
    """Resolve quoted headers exactly as local-file then pinned project INCLUDE.

    System headers are read by wcc's pinned preprocessor, not emulated by Python.
    Reject escaping include spellings rather than silently changing their meaning.
    """
    files, pending = {source.name: source}, [(source.name, source)]
    while pending:
        rel, src = pending.pop()
        for inc in re.findall(r'^\s*#\s*include\s*"([^"\n]+)"', src.read_text(encoding="utf-8"), re.M):
            dst = Path(rel).parent / inc
            if dst.is_absolute() or ".." in dst.parts:
                raise Refusal(f"escaping quoted include unsupported: {inc}")
            path = src.parent / inc
            if not path.is_file():
                path = root / "src/include" / inc
            path = path.resolve()
            if not path.is_file() or contained(path, root / "assets") or contained(path, root / "external"):
                raise Refusal(f"unresolved or forbidden quoted include {inc}")
            key = dst.as_posix()
            if key in files and files[key] != path:
                raise Refusal(f"ambiguous local include {key}")
            if key not in files:
                files[key] = path
                pending.append((key, path))
    return files


def manifest(root, source, args, files):
    import pycparser
    wcc, _, _, _ = load_pipeline(root)
    prof = wcc.profile(args.profile)
    _, base = wcc.install(prof["install"])
    paths = [*sorted((root / "tools").glob("*.py")),
             *sorted((root / "config").glob("*.json")),
             *sorted((root / "src/include").glob("*.h")),
             *sorted((base / "H").rglob("*.h")),
             Path(__file__), Path(__file__).with_name("permute_source.py"),
             Path(__file__).with_name("permute_backend.py")]
    if args.symbols:
        paths.append(Path(args.symbols).resolve())
    return {"schema": SCHEMA, "source": str(source), "function": args.function,
            "mode": args.mode, "seed": args.seed, "jobs": args.jobs,
            "mechanisms": args.mechanism, "profile": args.profile, "oracle": args.oracle,
            "python": platform.python_version(), "pycparser": pycparser.__version__,
            "inputs": {key: {"path": str(p), "sha256": digest(p.read_bytes())} for key, p in sorted(files.items())},
            "context_hash": digest(json.dumps({str(p.resolve()): digest(p.read_bytes()) for p in paths}, sort_keys=True).encode())}


def preprocess(root, source, output, profile):
    wcc, _, _, _ = load_pipeline(root)
    prof = wcc.profile(profile)
    rc, text = wcc.build(source, output, list(prof["flags"]) + ["-p"], prof["install"])
    if rc or not output.is_file():
        raise Refusal(f"pinned preprocessing failed: {text}")
    return output.read_text(encoding="utf-8")


def make_task(root, source, function, args, stem):
    return {"root": str(root), "source": str(source), "function": function,
            "object": str(stem.with_suffix(".obj")), "log": str(stem.with_suffix(".log")),
            "profile": args.profile, "oracle": args.oracle,
            "symbols": str(Path(args.symbols).resolve()) if args.symbols else None}


def candidate_model(model, pp, source):
    a, b = body_bounds(pp, model.function)
    c, d = body_bounds(source, model.function)
    return Model.parse(source, pp[:a] + source[c:d] + pp[b:], model.function, model.macros)


def macro_names(root, args, files):
    wcc, _, _, _ = load_pipeline(root)
    _, base = wcc.install(wcc.profile(args.profile)["install"])
    # A superset from pinned headers is intentional: inactive #defines cause a
    # refusal rather than risking re-expansion of a preprocessed function body.
    paths = list(files.values()) + list((base / "H").rglob("*.h"))
    return {name for path in paths for name in re.findall(r"(?m)^\s*#\s*define\s+(\w+)",
                                                        path.read_text(encoding="utf-8", errors="replace"))}


def main(argv=None, evaluate_fn=evaluate, preprocess_fn=preprocess):
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("source", type=Path)
    p.add_argument("function")
    p.add_argument("--out", type=Path, required=True)
    p.add_argument("--repo-root")
    mode = p.add_mutually_exclusive_group(required=True)
    mode.add_argument("--conservative", action="store_const", dest="mode", const="conservative")
    mode.add_argument("--aggressive", action="store_const", dest="mode", const="aggressive")
    p.add_argument("--seed", type=int, default=1)
    p.add_argument("--max-compiles", type=int, default=100)
    p.add_argument("--seconds", type=float, default=300)
    p.add_argument("--jobs", type=int, default=1)
    p.add_argument("--mechanism", action="append", choices=("colour", "home", "Score", "rover", "schedule", "selection"), default=[])
    p.add_argument("--explanation", type=Path, help="explain.py explanation.json; mechanisms discovered from it")
    p.add_argument("--profile", default="game")
    p.add_argument("--oracle", default="gog_v121_us")
    p.add_argument("--symbols", type=Path)
    p.add_argument("--resume", action="store_true")
    args = p.parse_args(argv)
    if args.max_compiles < 0 or args.seconds <= 0 or not 1 <= args.jobs <= 16:
        p.error("require max-compiles >= 0, seconds > 0, and 1 <= jobs <= 16")
    root, source, out = repo_root(args.repo_root), args.source.resolve(), args.out.resolve()
    if not any(contained(out, root / part) and out != (root / part).resolve() for part in ("build", "scratch")):
        raise Refusal("--out must be a subdirectory of repository build/ or scratch/")
    if contained(source, out) or contained(source, root / "assets") or contained(source, root / "external"):
        raise Refusal("input source must be outside the run directory and original/reference trees")
    if source.suffix.lower() != ".c":
        raise Refusal("only C units are supported")
    if args.explanation:
        text = args.explanation.read_text(encoding="utf-8")
        json.loads(text)  # fail on malformed reports, never execute report text
        for key, patterns in {"colour": ("colour", "color"), "home": ("home",),
                              "Score": ("score",), "rover": ("rover",),
                              "schedule": ("schedule",), "selection": ("selection",)}.items():
            if any(word in text.lower() for word in patterns):
                args.mechanism.append(key)
    args.mechanism = sorted(set(args.mechanism))
    files = input_files(root, source)
    identity = manifest(root, source, args, files)
    started = time.monotonic()
    if args.resume:
        state = json.loads((out / "state.json").read_text(encoding="utf-8"))
        if state["manifest"] != identity:
            raise Refusal("resume context differs (source/headers/tools/config/mode/seed/jobs); start a new run")
        # A reserved call might have executed before a crash. Charge it even if
        # no result exists; retry only as a NEW budgeted invocation below.
        for pending in state.get("pending", []):
            append_event(out, {"event": "interrupted", **pending, "budget_charged": True})
        state["pending"] = []
        # state.json is checkpointed at most every 2 s: charge every compile and skip every candidate id that the
        # append-only log recorded after the last checkpoint.
        for line in (out / "candidates.jsonl").read_text(encoding="utf-8").splitlines():
            e = json.loads(line)
            if e.get("event") == "started":
                state["compiles"] = max(state["compiles"], e["invocation"])
            if e.get("event") == "candidate":
                state["next_id"] = max(state["next_id"], e["id"] + 1)
        for rel, entry in identity["inputs"].items():
            if digest((out / "input" / rel).read_bytes()) != entry["sha256"]:
                raise Refusal(f"frozen input changed: {rel}")
        if state.get("preprocessed_sha256") and digest((out / "context.i").read_bytes()) != state["preprocessed_sha256"]:
            raise Refusal("frozen preprocessed context changed")
        for c in state["candidates"]:
            if digest(Path(c["source"]).read_bytes()) != c["hash"]:
                raise Refusal(f"candidate source changed: {c['id']}")
    else:
        if out.exists() and any(out.iterdir()):
            raise Refusal("nonempty --out; use --resume or choose a fresh directory")
        out.mkdir(parents=True, exist_ok=True)
        state = {"schema": SCHEMA, "manifest": identity, "compiles": 0, "seconds": 0,
                 "candidates": [], "pending": [], "next_id": 0, "baseline": None, "accepted": None}
        (out / "input").mkdir(exist_ok=True)
        for rel, path in files.items():
            dst = out / "input" / rel
            dst.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(path, dst)
    initial_seconds = state["seconds"]
    last_checkpoint = [0.0]
    def checkpoint(force=False):
        # Throttled: candidates.jsonl (append-only, fsync'd) is the record of every reserved compile and candidate;
        # resume reconciles the budget and ids from it, so a lagging state.json never under-counts.
        now = time.monotonic()
        if not force and now - last_checkpoint[0] < 2.0:
            return
        last_checkpoint[0] = now
        state["seconds"] = initial_seconds + now - started
        atomic_json(out / "state.json", state)
    def available():
        return state["compiles"] < args.max_compiles and initial_seconds + time.monotonic() - started < args.seconds
    def reserve(kind, candidate=None):
        state["compiles"] += 1
        item = {"event": "started", "invocation": state["compiles"], "kind": kind, "candidate": candidate}
        state["pending"].append(item)
        append_event(out, item)
        checkpoint()
        return item
    def finish(item, result):
        append_event(out, {**item, "event": "completed", "result": result})
        state["pending"].remove(item)
        checkpoint()
    checkpoint()
    input_source = out / "input" / source.name
    if not state.get("preprocessed") and available():
        call = reserve("preprocess")
        preprocess_fn(root, input_source, out / "context.i", args.profile)
        state["preprocessed"] = True
        state["preprocessed_sha256"] = digest((out / "context.i").read_bytes())
        finish(call, {"context_sha256": digest((out / "context.i").read_bytes())})
    if not state["baseline"] and state.get("preprocessed") and available():
        call = reserve("baseline")
        baseline = evaluate_fn(make_task(root, input_source, args.function, args, out / "baseline"))
        if "error" in baseline:
            finish(call, baseline)
            raise Refusal("baseline did not compile")
        canonical = json.loads((root / "config/units.json").read_text(encoding="utf-8"))["units"]
        unit = next((u for u in canonical.values() if (root / u["source"]).resolve() == source), None)
        required = {n for n, r in unit["functions"].items() if r["status"] == "STRICT"} if unit else set()
        if required - set(baseline["strict"]):
            finish(call, baseline)
            raise Refusal("baseline loses recorded STRICT functions; fix context first")
        # Only these fields are read later; the full per-function report made every checkpoint ~100+ KB.
        state["baseline"] = {k: baseline[k] for k in ("target", "score", "strict", "shape", "seconds") if k in baseline}
        finish(call, {"score": baseline["score"], "strict": baseline["strict"]})
    baseline = state["baseline"]
    if baseline is None:
        checkpoint()
        atomic_json(out / "summary.json", {"status": "INCOMPLETE_BASELINE", "compiles": state["compiles"], "seconds": state["seconds"]})
        return 0
    pp = (out / "context.i").read_text(encoding="utf-8")
    model = Model.parse(input_source.read_text(encoding="utf-8"), pp, args.function, macro_names(root, args, files))
    parents = {"baseline": model.target()}
    for c in state["candidates"]:
        if c.get("eligible"):
            parents[str(c["id"])] = candidate_model(model, pp, Path(c["source"]).read_text(encoding="utf-8")).target()
    seen = {digest(input_source.read_bytes()), *(c["hash"] for c in state["candidates"])}
    # Recovered interrupted candidates are re-generated only if their hash was
    # not committed. IDs are monotonic; consumed compiler budget is never reset.
    stalled = 0
    processes = []
    # Persistent evaluation workers for --jobs > 1 (the real evaluator only; tests inject their own evaluate_fn).
    pool = WorkerPool(args.jobs) if args.jobs > 1 and evaluate_fn is evaluate else None

    def prune_candidate(source_path, result):
        """Keep only the candidate source and a compact result: the object, compiler log, copied headers and the
        full match report (~190 KB per candidate) filled tens of GB across worker runs (2026-10-24). Everything
        dropped is reproducible from the source; headers come back from out/input on a fresh hit check."""
        d = source_path.parent
        atomic_json(d / "result.json", {k: result[k] for k in ("strict", "score", "target", "error") if k in result})
        for p in d.iterdir():
            if p.is_dir():
                shutil.rmtree(p)
            elif p != source_path and p.name != "result.json":
                p.unlink()

    def restore_inputs(d):
        for p in (out / "input").iterdir():
            if p.is_dir():
                shutil.copytree(p, d / p.name, dirs_exist_ok=True)
            elif not (d / p.name).exists():      # never overwrites the candidate source
                shutil.copy2(p, d / p.name)

    def check_hits(candidates):
        for c in candidates:
            if c.get("hit") and available() and state["accepted"] is None:
                restore_inputs(Path(c["source"]).parent)
                call = reserve("fresh_hit_check", c["id"])
                result = evaluate_fn(make_task(root, Path(c["source"]), args.function, args,
                                               Path(c["source"]).parent / "fresh"))
                decision = assess(baseline, result, args.function)
                atomic_json(Path(c["source"]).parent / "fresh.json", result)
                if decision["hit"]:
                    state["accepted"] = c["id"]
                    shutil.copytree(Path(c["source"]).parent, out / "accepted", dirs_exist_ok=True)
                c["fresh_checked"] = True
                finish(call, decision)
    try:
        check_hits(c for c in state["candidates"] if not c.get("fresh_checked"))
        while available() and state["accepted"] is None and stalled < 100:
            ranked = sorted((c for c in state["candidates"] if c.get("eligible")), key=lambda c: (c["score"]["total"], c["id"]))[:4]
            parent_keys = ["baseline"] + [str(c["id"]) for c in ranked]
            batch = []
            for _ in range(min(args.jobs, args.max_compiles - state["compiles"])):
                if not available():
                    break
                ident = state["next_id"]
                state["next_id"] += 1
                rng = random.Random(f"{args.seed}:{ident}")
                parent_key = rng.choice(parent_keys)
                fn = parents[parent_key]
                chain = [] if parent_key == "baseline" else next(c["chain"] for c in state["candidates"] if str(c["id"]) == parent_key).copy()
                try:
                    for _ in range(rng.randint(1, 2 if args.mode == "conservative" else 4)):
                        fn, step = mutate(model, fn, rng, args.mode, args.mechanism)
                        chain.append(step)
                    text = model.unit(fn)
                    sha = digest(text.encode("utf-8"))
                    if sha in seen:
                        append_event(out, {"event": "duplicate", "id": ident, "hash": sha, "chain": chain})
                        stalled += 1
                        continue
                    seen.add(sha)
                    d = out / "c" / str(ident)
                    shutil.copytree(out / "input", d)
                    dest = d / source.name
                    dest.write_text(text, encoding="utf-8", newline="\n")
                    c = {"id": ident, "hash": sha, "source": str(dest), "parent": parent_key, "chain": chain,
                         "quality": "SYNTHETIC" if any(s["quality"] == "SYNTHETIC" for s in chain) else "COMPILER_STEERED"}
                    append_event(out, {"event": "candidate", **c})
                    call = reserve("candidate", ident)
                    task = make_task(root, dest, args.function, args, d / "compile")
                    if pool:
                        result = pool.submit(task)
                    elif args.jobs > 1:
                        result = IsolatedProcess(task)
                        processes.append(result)
                    else:
                        result = evaluate_fn(task)
                    batch.append((c, call, fn, result))
                except Refusal as exc:
                    append_event(out, {"event": "mutation_rejected", "id": ident, "reason": str(exc), "chain": chain})
                    stalled += 1
            for c, call, fn, pending in batch:
                result = pending.result() if args.jobs > 1 else pending
                decision = assess(baseline, result, args.function)
                prune_candidate(Path(c["source"]), result)
                c.update(decision, score=result.get("score"), status=result.get("target", {}).get("status", "COMPILE_ERROR"),
                         compiles=call["invocation"], elapsed=initial_seconds + time.monotonic() - started,
                         first_diff=result.get("target", {}).get("first_diff"))
                state["candidates"].append(c)
                if c["eligible"]:
                    parents[str(c["id"])] = fn
                finish(call, c)
                stalled = 0
            # All parallel builds complete before a fresh acceptance build.
            check_hits(c for c, _, _, _ in batch)
            if batch:
                best = min([baseline["score"]["total"]] + [c["score"]["total"] for c in state["candidates"] if c.get("eligible")])
                print(f"{args.function} {args.mode}: {state['compiles']}/{args.max_compiles} compiles, best score {best}", flush=True)
    finally:
        for process in processes:
            process.close()
        if pool:
            pool.close()
        checkpoint(force=True)
    eligible = [c for c in state["candidates"] if c.get("eligible")]
    best = min(eligible, key=lambda c: (c["score"]["total"], c["id"]), default=None)
    if best and best["score"]["total"] >= baseline["score"]["total"]:
        best = None
    if state["accepted"] is not None:
        best = next(c for c in state["candidates"] if c["id"] == state["accepted"])
    summary = {"function": args.function, "unit": source.stem, "mode": args.mode,
               "status_before": baseline["target"]["status"],
               "status_after": "STRICT" if state["accepted"] is not None else baseline["target"]["status"],
               "accepted": state["accepted"] is not None,
               "diffs_before": baseline["score"]["byte_diffs"],
               "diffs_best": min([baseline["score"]["byte_diffs"]] + [c["score"]["byte_diffs"] for c in eligible]),
               "score_before": baseline["score"], "score_best": best["score"] if best else baseline["score"],
               "diff_best_source": min(eligible, key=lambda c: (c["score"]["byte_diffs"], c["id"]))["source"]
                                   if eligible and min(c["score"]["byte_diffs"] for c in eligible) < baseline["score"]["byte_diffs"] else str(input_source),
               "best_source": best["source"] if best else str(input_source),
               "quality": best["quality"] if best else "NATURAL", "transformation": best["chain"] if best else [],
               "unit_strict_before": baseline["strict"],
               "unit_strict_after": json.loads((Path(best["source"]).parent / "result.json").read_text())["strict"] if best else baseline["strict"],
               "compiles": state["compiles"], "seconds": state["seconds"],
               "compiles_per_second": state["compiles"] / state["seconds"],
               "candidates": len(state["candidates"]), "rejected": sum(not c["eligible"] for c in state["candidates"]),
               "stop": "accepted" if state["accepted"] is not None else ("mutation_space_stalled" if stalled >= 100 else "budget"),
               "notes": "Ranking is diagnostic; only accepted=true is a fresh full-unit STRICT hit. Budgets include preprocessing and checks."}
    atomic_json(out / "summary.json", summary)
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (Refusal, ValueError, OSError) as exc:
        print(f"permute: {exc}", file=sys.stderr)
        sys.exit(2)
