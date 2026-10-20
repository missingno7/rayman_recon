"""Read-only ISO9660 reader for raw CD images (MODE1/2352, MODE2/2352 XA form 1, or 2048-byte ISO).

    python tools/iso9660.py ls IMAGE              list files (path, LBA, size)
    python tools/iso9660.py cat IMAGE PATH OUT    extract one file (OUT must be under build/)
"""
import struct
import sys
from pathlib import Path


class Image:
    def __init__(self, path, mode="auto"):
        self.path = Path(path)
        self.f = open(self.path, "rb")
        size = self.path.stat().st_size
        if mode == "auto":
            if size % 2352 == 0:
                self.f.seek(16 * 2352 + 15)
                m = self.f.read(1)[0]
                mode = "mode1" if m == 1 else "mode2"
            else:
                mode = "iso"
        self.mode = mode
        self.sector_size, self.data_off = {"mode1": (2352, 16), "mode2": (2352, 24), "iso": (2048, 0)}[mode]
        pvd = self.sector(16)
        if pvd[0:6] != b"\x01CD001":
            raise ValueError(f"{path}: no ISO9660 PVD")
        self.pvd = pvd
        self.volume_id = pvd[40:72].decode("ascii", "replace").strip()
        self.system_id = pvd[8:40].decode("ascii", "replace").strip()
        self.created = pvd[813:829].decode("ascii", "replace")
        self.root = self._dirent(pvd[156:156 + 34])

    def sector(self, lba):
        self.f.seek(lba * self.sector_size + self.data_off)
        return self.f.read(2048)

    def _dirent(self, b):
        ln = b[0]
        lba = struct.unpack_from("<I", b, 2)[0]
        size = struct.unpack_from("<I", b, 10)[0]
        flags = b[25]
        nlen = b[32]
        name = b[33:33 + nlen].decode("ascii", "replace")
        date = tuple(b[18:25])
        return {"lba": lba, "size": size, "dir": bool(flags & 2), "name": name, "len": ln, "date": date}

    def listdir(self, ent):
        out = []
        n = (ent["size"] + 2047) // 2048
        for i in range(n):
            s = self.sector(ent["lba"] + i)
            p = 0
            while p < 2048:
                ln = s[p]
                if ln == 0:
                    break
                e = self._dirent(s[p:p + ln])
                p += ln
                if e["name"] in ("\x00", "\x01"):
                    continue
                out.append(e)
        return out

    def walk(self, ent=None, prefix=""):
        ent = ent or self.root
        for e in self.listdir(ent):
            name = e["name"].split(";")[0]
            path = prefix + "/" + name if prefix else name
            if e["dir"]:
                yield path + "/", e
                yield from self.walk(e, path)
            else:
                yield path, e

    def find(self, path):
        want = path.replace("\\", "/").strip("/").upper()
        for p, e in self.walk():
            if p.upper() == want:
                return e
        raise FileNotFoundError(path)

    def read(self, path):
        e = self.find(path)
        out = bytearray()
        n = (e["size"] + 2047) // 2048
        for i in range(n):
            out += self.sector(e["lba"] + i)
        return bytes(out[: e["size"]])


def main(argv):
    if len(argv) < 3:
        print(__doc__)
        return 2
    img = Image(argv[2])
    if argv[1] == "ls":
        print(f"# system={img.system_id!r} volume={img.volume_id!r} created={img.created} mode={img.mode}")
        for p, e in img.walk():
            print(f"{e['lba']:8d} {e['size']:10d} {p}")
    elif argv[1] == "cat":
        out = Path(argv[4]).resolve()
        build = (Path(__file__).resolve().parent.parent / "build").resolve()
        if build not in out.parents:
            raise SystemExit("refusing to write outside build/")
        out.parent.mkdir(parents=True, exist_ok=True)
        out.write_bytes(img.read(argv[3]))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
