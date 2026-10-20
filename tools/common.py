"""Shared paths and small helpers for rayman_recon tools (stdlib only)."""
import hashlib
import json
import os
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ASSETS = ROOT / "assets"
BUILD = ROOT / "build"
CONFIG = ROOT / "config"
TOOLS_DIR = Path(os.environ.get("RAYMAN_TOOLS", "C:/tools"))


def sha256_bytes(data):
    return hashlib.sha256(data).hexdigest()


def sha1_bytes(data):
    return hashlib.sha1(data).hexdigest()


def sha256_file(path, chunk=1 << 20):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        while True:
            b = f.read(chunk)
            if not b:
                break
            h.update(b)
    return h.hexdigest()


def load_json(path):
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)


def dump_json(path, obj):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix(path.suffix + f".{os.getpid()}.tmp")   # per-process: concurrent writers must not collide
    with open(tmp, "w", encoding="utf-8", newline="\n") as f:
        json.dump(obj, f, indent=1, sort_keys=False)
        f.write("\n")
    for attempt in range(20):          # Windows: a concurrent reader can hold the target open briefly
        try:
            os.replace(tmp, path)
            return
        except PermissionError:
            if attempt == 19:
                raise
            time.sleep(0.25)


def oracle(name):
    """Return (path, bytes) of a pinned oracle from config/oracles.json, failing closed on hash mismatch."""
    cfg = load_json(CONFIG / "oracles.json")
    ent = cfg["oracles"][name]
    src = ent["source"]
    if src["kind"] == "file":
        data = (ROOT / src["path"]).read_bytes()
    elif src["kind"] == "iso":
        from iso9660 import Image
        data = Image(ROOT / src["image"], src.get("mode", "auto")).read(src["path"])
    else:
        raise SystemExit(f"oracle {name}: unknown source kind {src['kind']}")
    got = sha256_bytes(data)
    if got != ent["sha256"]:
        raise SystemExit(f"oracle {name}: sha256 mismatch {got} != {ent['sha256']} (refusing)")
    return data
