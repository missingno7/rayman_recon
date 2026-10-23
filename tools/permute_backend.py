"""Pinned compiler and unchanged match.py adapter, usable in isolated processes.

No historical executable is launched here: all compiler execution goes through
wcc.build. Each process performs at most one build at a time (wcc's TMP is PID
scoped). Threads must never be used for compilation with this wrapper.
"""
from __future__ import annotations

import difflib
import hashlib
import importlib
import json
import os
import subprocess
import sys
import time
from pathlib import Path


def digest(data):
    return hashlib.sha256(data).hexdigest()


def load_pipeline(root):
    sys.dont_write_bytecode = True
    sys.path.insert(0, str(Path(root) / "tools"))
    return tuple(importlib.import_module(n) for n in ("wcc", "match", "omf", "image"))


def object_shape(mods):
    """No output bytes in source: hashes only; exact non-code content/relocations."""
    out = []
    for m in mods:
        segments = []
        for si, s in enumerate(m.segments):
            if not s or s.name.startswith("$$"):
                continue
            ent = {"name": s.name, "class": s.cls, "align": s.align, "combine": s.combine,
                   "use32": s.use32, "size": s.size}
            if s.cls.upper() == "CODE":
                prefix = min((off for _, seg, off, _ in m.publics if seg == si), default=s.size)
                ent.update(prefix_size=prefix, prefix_sha256=digest(bytes(s.data[:prefix])),
                           prefix_fixups=[[f.off, f.loc, f.self_rel, list(f.target), f.disp, m.target_name(f)]
                                          for f in m.fixups if f.seg == si and f.off < prefix])
            else:
                ent.update(data_sha256=digest(bytes(s.data)), covered_sha256=digest(bytes(s.covered)),
                           publics=[list(p) for p in m.publics if p[1] == si],
                           fixups=[[f.off, f.loc, f.self_rel, list(f.target), f.disp, m.target_name(f)]
                                   for f in m.fixups if f.seg == si])
            segments.append(ent)
        out.append({"segments": segments,
                    "publics": sorted((n, m.segments[si].name, m.segments[si].cls)
                                      for n, si, _, _ in m.publics),
                    "comdefs": m.comdefs})
    return out


def invariants(shape):
    # Code size/offsets may change; code alignment, publics, data and BSS may not.
    out = json.loads(json.dumps(shape))
    for m in out:
        for s in m["segments"]:
            if s["class"].upper() == "CODE":
                s.pop("size")
    return out


def instruction_distance(candidate, oracle, candidate_masks=(), oracle_masks=()):
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    def tokens(code, masks):
        normalized = bytearray(code)
        for off in masks:
            if 0 <= off < len(normalized):
                normalized[off] = 0
        insns = list(md.disasm(bytes(normalized), 0))
        result = [(i.mnemonic, i.op_str) for i in insns]
        end = insns[-1].address + insns[-1].size if insns else 0
        # Undecodable tails are scored; never silently dropped.
        result.extend(("byte", str(b)) for b in normalized[end:])
        return result
    a, b = tokens(candidate, candidate_masks), tokens(oracle, oracle_masks)
    return sum(max(i2 - i1, j2 - j1) for tag, i1, i2, j1, j2 in
               difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes() if tag != "equal")


def score_target(img, mods, target, result, match):
    """Ranking only. The STRICT bit comes exclusively from verify_object."""
    binding, _ = match.Bindings(img).lookup(result["symbol"])
    if binding is None:
        raise ValueError("target needs an oracle binding")
    ob, start = binding
    root = Path(match.__file__).resolve().parent.parent
    names = set()
    for path in [root / "config/symbols.json"] + ([Path(match.Bindings.extra)] if match.Bindings.extra else []):
        names.update(json.loads(path.read_text(encoding="utf-8")).get(img.name, {}).get("functions", {}))
    binds = match.Bindings(img)
    bounds = sorted({binds.game[n][1] for n in names if n in binds.game and
                     binds.game[n][0] == ob and binds.game[n][1] > start})
    extent = (bounds[0] - start) if bounds else result["size"]
    # The successor binding is an independent length ranking hint, not acceptance.
    oracle = img.obj[ob][start:start + extent]
    for m in mods:
        for name, si, off, _ in m.publics:
            if name != result["symbol"]:
                continue
            cand = bytes(m.segments[si].data[off:off + result["size"]])
            masks, rels, sites = set(), set(), set()
            for f in m.fixups:
                if f.seg == si and off <= f.off < off + len(cand):
                    loc = f.off - off
                    masks.update(range(loc, loc + f.size()))
                    if f.self_rel:
                        rels.update(range(loc, loc + f.size()))
                    elif f.size() == 4:
                        sites.add(loc)
            oracle_sites = {s - start for o, s in img.fixups if o == ob and start <= s < start + extent}
            oracle_masks = rels | {i for s in oracle_sites for i in range(s, s + 4)}
            insns = instruction_distance(cand, oracle, masks, oracle_masks)
            parts = {"byte_diffs": result.get("byte_diffs", len(cand)),
                     "length_delta": abs(len(cand) - extent),
                     "fixup_site_diffs": len(sites ^ oracle_sites),
                     "instruction_diffs": insns,
                     "reference_problems": len(result.get("problems", [])) + len(result.get("unverified", []))}
            # No secondary function's bytes enter this score.
            parts["total"] = (parts["byte_diffs"] + parts["length_delta"] +
                              4 * parts["fixup_site_diffs"] + 2 * insns + 4 * parts["reference_problems"])
            return parts
    raise ValueError("target public missing from object")


def evaluate(task):
    """One fresh full-unit compile + unchanged full-unit verifier invocation."""
    began = time.monotonic()
    wcc, match, omf, image = load_pipeline(task["root"])
    src, out = Path(task["source"]), Path(task["object"])
    out.parent.mkdir(parents=True, exist_ok=True)
    out.unlink(missing_ok=True)
    profile = wcc.profile(task.get("profile", "game"))
    rc, text = wcc.build(src, out, profile["flags"], profile["install"])
    Path(task["log"]).write_text(text, encoding="utf-8")
    if rc or not out.exists():
        return {"error": "compile failed", "returncode": rc, "seconds": time.monotonic() - began}
    img = image.Image(task.get("oracle"))
    match.Bindings.extra = task.get("symbols")
    mods = omf.load(out)
    results, segments = [], []
    for m in mods:
        results.extend(match.verify_object(img, m, None, segments))
    targets = [r for r in results if r["function"] == task["function"]]
    if len(targets) != 1:
        raise ValueError("target missing/ambiguous in compiled unit")
    r = targets[0]
    return {"target": r, "score": score_target(img, mods, task["function"], r, match),
            "strict": sorted(r["function"] for r in results if r["status"] == "STRICT"),
            "results": results, "segments": segments, "shape": object_shape(mods),
            "seconds": time.monotonic() - began}


def assess(baseline, candidate, function):
    if "error" in candidate:
        return {"eligible": False, "hit": False, "losses": [], "reason": candidate["error"]}
    losses = sorted(set(baseline["strict"]) - set(candidate["strict"]))
    same = invariants(baseline["shape"]) == invariants(candidate["shape"])
    eligible = not losses and same
    return {"eligible": eligible, "hit": eligible and candidate["target"]["status"] == "STRICT",
            "losses": losses, "data_and_publics_unchanged": same,
            "reason": "" if eligible else "STRICT loss or data/public/alignment change"}


class IsolatedProcess:
    """One bounded Python worker; ordinary pipes/files, no named semaphores.

    Windows restricted tokens can deny multiprocessing's named synchronization
    objects. Plain child processes still provide the PID isolation wcc requires.
    Always close these in a finally block; no daemon/background services.
    """
    def __init__(self, task):
        stem = Path(task["object"])
        self.task_path = stem.with_suffix(".task.json")
        self.result_path = stem.with_suffix(".worker.json")
        self.task_path.parent.mkdir(parents=True, exist_ok=True)
        self.task_path.write_text(json.dumps(task), encoding="utf-8")
        self.result_path.unlink(missing_ok=True)
        self.process = subprocess.Popen([sys.executable, "-B", str(Path(__file__).resolve()),
                                         str(self.task_path), str(self.result_path)],
                                        stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                        creationflags=subprocess.CREATE_NO_WINDOW if os.name == "nt" else 0)
        self.value = None

    def result(self):
        if self.value is not None:
            return self.value
        try:
            stdout, stderr = self.process.communicate(timeout=180)
        except subprocess.TimeoutExpired:
            self.process.kill()
            stdout, stderr = self.process.communicate()
        self.task_path.with_suffix(".worker.log").write_bytes(stdout + stderr)
        if self.process.returncode or not self.result_path.exists():
            self.value = {"error": "isolated worker failed", "returncode": self.process.returncode}
        else:
            self.value = json.loads(self.result_path.read_text(encoding="utf-8"))
        return self.value

    def close(self):
        if self.process.poll() is None:
            self.process.kill()
        self.process.communicate()


if __name__ == "__main__":
    task = json.loads(Path(sys.argv[1]).read_text(encoding="utf-8"))
    Path(sys.argv[2]).write_text(json.dumps(evaluate(task)), encoding="utf-8")
