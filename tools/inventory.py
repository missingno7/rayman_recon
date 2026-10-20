"""Inventory every original asset: plain files under assets/ and the files inside the CD images.

    python tools/inventory.py            write build/inventory/inventory.json and print a summary
    python tools/inventory.py --compare  also compare the installed DOS tree against the DOS CD

Raw CD images are hashed as whole files; their ISO9660 contents are hashed individually.
Nothing under assets/ is modified.
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import ASSETS, BUILD, ROOT, dump_json, sha1_bytes, sha256_bytes, sha256_file  # noqa: E402
from iso9660 import Image  # noqa: E402

DATA_TRACKS = {
    "DOS_CD": "assets/DOS_CD/RAYMAN.bin",
    "PS1": "assets/PS1/Rayman (Track 01).bin",
}


def plain_files():
    out = []
    for p in sorted(ASSETS.rglob("*")):
        if p.is_file():
            rel = p.relative_to(ROOT).as_posix()
            out.append({"path": rel, "size": p.stat().st_size, "sha256": sha256_file(p)})
    return out


def iso_files(name, image):
    img = Image(ROOT / image)
    files = []
    for path, e in img.walk():
        if path.endswith("/"):
            continue
        data = img.read(path)
        files.append({"path": path, "lba": e["lba"], "size": e["size"], "sha256": sha256_bytes(data),
                      "sha1": sha1_bytes(data)})
    return {"image": image, "mode": img.mode, "system_id": img.system_id, "volume_id": img.volume_id,
            "created": img.created, "files": files}


def main(argv):
    inv = {"plain": plain_files(), "iso": {k: iso_files(k, v) for k, v in DATA_TRACKS.items()}}
    dump_json(BUILD / "inventory" / "inventory.json", inv)
    print(f"plain files: {len(inv['plain'])}")
    for k, v in inv["iso"].items():
        print(f"{k}: {v['image']} mode={v['mode']} system={v['system_id']!r} volume={v['volume_id']!r} "
              f"created={v['created']} files={len(v['files'])}")
    if "--compare" in argv:
        cd = {f["path"].upper(): f for f in inv["iso"]["DOS_CD"]["files"]}
        print("\ninstalled DOS tree vs DOS CD (RAYMAN/ subtree):")
        same = diff = missing = 0
        for f in inv["plain"]:
            if not f["path"].startswith("assets/DOS/"):
                continue
            rel = f["path"][len("assets/DOS/"):]
            c = cd.get(("RAYMAN/" + rel).upper())
            if c is None:
                missing += 1
                print(f"  NOT ON CD  {rel} {f['size']} {f['sha256'][:16]}")
            elif c["sha256"] == f["sha256"]:
                same += 1
            else:
                diff += 1
                print(f"  DIFFERENT  {rel} inst={f['size']}/{f['sha256'][:16]} cd={c['size']}/{c['sha256'][:16]}")
        print(f"  identical={same} different={diff} not_on_cd={missing}")
    print(f"wrote {(BUILD / 'inventory' / 'inventory.json').relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
