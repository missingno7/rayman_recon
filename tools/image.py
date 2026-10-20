"""In-memory model of the extracted DOS oracle image (objects + PMW1 fixups).

Coordinates (never mix them):
  obj:off   canonical  - PMW1/LE object number (1 = code, 2 = data/BSS) and object-relative offset.
  file      packed     - offset of the compressed stream in RAYMAN.EXE (only per object/block, see pmw1.json).
  ida       convention - obj1 at 0x10000, obj2 at 0x90000 (le_disasm / IDA-style flat numbering used by
                         rayman_disasm). A display convention, NOT the runtime address.
Runtime linear addresses under PMODE/W are not determined yet.
"""
import json
import struct
import sys
from functools import cached_property
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import BUILD, CONFIG, ROOT, load_json, sha256_bytes  # noqa: E402

IDA_BASE = {1: 0x10000, 2: 0x90000}


class Image:
    def __init__(self, name=None):
        allcfg = load_json(CONFIG / "oracles.json")
        name = name or allcfg["primary"]
        self.name = name
        d = BUILD / "oracles" / name
        if not (d / "pmw1.json").exists():
            raise SystemExit(f"{d} missing: run `python tools/extract.py {name}` first")
        self.meta = load_json(d / "pmw1.json")
        cfg = allcfg["oracles"][name]
        if self.meta["exe_sha256"] != cfg["sha256"]:
            raise SystemExit("build/pmw1 was not extracted from the pinned oracle; re-run tools/extract.py")
        self.obj = {}
        for o in self.meta["objects"]:
            data = (d / f"obj{o['index']}.bin").read_bytes()
            if sha256_bytes(data) != o["sha256"]:
                raise SystemExit(f"obj{o['index']}.bin hash mismatch")
            if o["sha256"] != cfg["objects"][str(o["index"])]["sha256"]:
                raise SystemExit(f"obj{o['index']}.bin differs from the pinned decoded hash in config/oracles.json")
            self.obj[o["index"]] = data
        fx = load_json(d / "fixups.json")
        # site (obj, off) -> (type, target obj, target off)
        self.fixups = {}
        for k, items in fx.items():
            for it in items:
                self.fixups[(int(k), it["src"])] = (it["type"], it["tobj"], it["toff"])
        self.code = self.obj[1]
        self.entry = (self.meta["cs_obj"], self.meta["eip"])

    @cached_property
    def code_fixup_sites(self):
        return sorted(off for (o, off) in self.fixups if o == 1)

    def fixups_in(self, obj, start, end):
        return {off: self.fixups[(obj, off)] for (o, off) in self.fixups if o == obj and start <= off < end}

    def target_value(self, obj, off):
        """The 32-bit value stored at a fixup site (PMW1 stores the unrelocated image; normally 0 or addend)."""
        return struct.unpack_from("<I", self.obj[obj], off)[0]

    @staticmethod
    def ida(obj, off):
        return IDA_BASE[obj] + off

    @staticmethod
    def from_ida(addr):
        if addr >= IDA_BASE[2]:
            return 2, addr - IDA_BASE[2]
        return 1, addr - IDA_BASE[1]

    @staticmethod
    def fmt(obj, off):
        return f"{obj}:{off:05x}"


def parse_addr(s):
    """Accept '1:1234', 'ida:11234' or a bare hex obj1 offset."""
    s = s.strip().lower()
    if s.startswith("ida:"):
        return Image.from_ida(int(s[4:], 16))
    if ":" in s:
        a, b = s.split(":", 1)
        return int(a), int(b, 16)
    return 1, int(s, 16)
