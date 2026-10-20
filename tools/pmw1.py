"""PMODE/W PMW1 container reader and decompressor.

Format: PMODE/W 1.33 source distribution, DOCS/PMW1FMT.TXT and PMWLITE/PMW1.H (MIT licence, see
docs/TOOLCHAIN.md for provenance). The decoder below is a line-by-line port of DECODE.ASM from that
distribution; every compressed block must decode to exactly its declared size and consume exactly its
declared compressed size, otherwise extraction fails.

    python tools/pmw1.py info  EXE           header, objects, relocation summary
    python tools/pmw1.py extract EXE OUTDIR  write objN.bin + fixups.json + pmw1.json under OUTDIR (build/ only)
"""
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import BUILD, dump_json, sha256_bytes  # noqa: E402

HDR = struct.Struct("<4sHHIIIIIIII")      # 0x28 bytes
OBJ = struct.Struct("<IIIIII")             # 0x18 bytes
RTBLOCK = struct.Struct("<HH")             # size (compressed), usize
RTITEM = struct.Struct("<BiBi")            # 10 bytes, packed (wcc386 default -zp1)


class DecodeError(Exception):
    pass


def decode(src, pos, out_size):
    """Port of PMODE/W DECODE.ASM. Returns (bytes, end_pos). Raises DecodeError on malformed input."""
    out = bytearray()
    n = len(src)
    bits = 0      # EBP
    left = 0      # DL (signed counter)

    def getbit():
        nonlocal bits, left, pos
        left -= 1
        if left < 0:
            if pos + 4 > n:
                raise DecodeError("bit refill past end")
            bits = struct.unpack_from("<I", src, pos)[0]
            pos += 4
            left = 31
        b = (bits >> 31) & 1
        bits = (bits << 1) & 0xFFFFFFFF
        return b

    def byte():
        nonlocal pos
        if pos >= n:
            raise DecodeError("byte read past end")
        v = src[pos]
        pos += 1
        return v

    out.append(byte())                 # movs (first byte is always a literal)
    while True:
        if len(out) > out_size:
            raise DecodeError("output overrun")
        if not getbit():
            out.append(byte())         # literal
            continue
        # code
        bl = byte()
        bh = 0
        if getbit():                   # extended
            if getbit():
                cnt = (1 + getbit()) << 1          # adc cl,1 ; shl cl,1  -> 2 or 4 high bits
                for _ in range(cnt):
                    bh = ((bh << 1) | getbit()) & 0xFF
            dist = ((bh << 8) | bl) + 1
            # decode_length
            length = None
            dh = 2
            for _ in range(4):
                dh += 1
                if getbit():
                    length = dh
                    break
            if length is None:
                if getbit():
                    length = byte() + 15
                else:
                    dh = 0
                    for _ in range(3):
                        dh = (dh << 1) | getbit()
                    length = dh + 7
        elif getbit():                 # code11: 3 high bits, length 2
            for _ in range(3):
                bh = ((bh << 1) | getbit()) & 0xFF
            dist = ((bh << 8) | bl) + 1
            length = 2
        else:                          # code00: 8-bit distance, length 2; bl == 0 terminates
            if bl == 0:
                break
            dist = bl
            length = 2
        if dist > len(out):
            raise DecodeError(f"distance {dist} before start at out={len(out)}")
        for _ in range(length):        # byte-wise copy (overlap allowed)
            out.append(out[-dist])
    if len(out) != out_size:
        raise DecodeError(f"decoded {len(out)} bytes, expected {out_size}")
    return bytes(out), pos


class PMW1:
    def __init__(self, exe):
        self.exe = exe
        self.base = struct.unpack_from("<I", exe, 0x3C)[0]
        f = HDR.unpack_from(exe, self.base)
        if f[0] != b"PMW1":
            raise ValueError("no PMW1 header at e_lfanew")
        (_, self.version, self.flags, self.cs_obj, self.eip, self.ss_obj, self.esp,
         self.objtbl_off, self.nobj, self.rt_off, self.data_off) = f
        self.compressed = bool(self.flags & 1)
        self.objects = []
        for i in range(self.nobj):
            v = OBJ.unpack_from(exe, self.base + self.objtbl_off + i * OBJ.size)
            self.objects.append(dict(zip(("virtual_size", "stored_size", "flags", "rt_offset", "rt_blocks",
                                          "data_size"), v)))

    def version_text(self):
        return f"{self.version & 0xFF}.{self.version >> 8:02d}"

    def object_data(self):
        """Yield (index, data, compressed_span) for every object, in file order."""
        pos = self.base + self.data_off
        for i, o in enumerate(self.objects):
            span = (pos, pos + o["stored_size"])
            if o["stored_size"] == 0:
                data = b""
            elif self.compressed:
                data, end = decode(self.exe, pos, o["data_size"])
                if end != span[1]:
                    raise DecodeError(f"object {i + 1}: consumed {end - pos} of {o['stored_size']} bytes")
            else:
                data = self.exe[pos:span[1]]
            yield i + 1, data, span
            pos = span[1]

    def fixups(self):
        """Yield (object_index, item dict) for all relocation items, plus per-object block spans."""
        result = {}
        spans = {}
        for i, o in enumerate(self.objects):
            pos = self.base + self.rt_off + o["rt_offset"]
            raw = bytearray()
            blocks = []
            for _ in range(o["rt_blocks"]):
                csize, usize = RTBLOCK.unpack_from(self.exe, pos)
                pos += RTBLOCK.size
                if self.compressed:
                    data, end = decode(self.exe, pos, usize)
                    if end != pos + csize:
                        raise DecodeError(f"object {i + 1} reloc block: consumed {end - pos} of {csize}")
                else:
                    data = self.exe[pos:pos + csize]
                blocks.append((pos, csize, usize))
                raw += data
                pos += csize
            if len(raw) % RTITEM.size:
                raise DecodeError(f"object {i + 1}: relocation data {len(raw)} not a multiple of 10")
            items = []
            for k in range(0, len(raw), RTITEM.size):
                t, src, tobj, toff = RTITEM.unpack_from(raw, k)
                items.append({"type": t, "src": src, "tobj": tobj, "toff": toff})
            result[i + 1] = items
            spans[i + 1] = blocks
        return result, spans


def main(argv):
    if len(argv) < 3:
        print(__doc__)
        return 2
    exe = Path(argv[2]).read_bytes()
    p = PMW1(exe)
    print(f"PMW1 at 0x{p.base:x}: version {p.version_text()} flags 0x{p.flags:x} (compressed={p.compressed})")
    print(f"  CS:EIP obj {p.cs_obj}:0x{p.eip:x}  SS:ESP obj {p.ss_obj}:0x{p.esp:x}  objects {p.nobj}")
    for i, o in enumerate(p.objects):
        print(f"  obj{i + 1}: vsize 0x{o['virtual_size']:x} stored 0x{o['stored_size']:x} data 0x{o['data_size']:x} "
              f"flags 0x{o['flags']:x} rt_off 0x{o['rt_offset']:x} rt_blocks {o['rt_blocks']}")
    fx, spans = p.fixups()
    for k, v in fx.items():
        types = {}
        for it in v:
            types[it["type"]] = types.get(it["type"], 0) + 1
        print(f"  obj{k}: {len(v)} fixups, types {dict(sorted(types.items()))}")
    if argv[1] == "extract":
        out = Path(argv[3]).resolve()
        if BUILD.resolve() not in out.parents and out != BUILD.resolve():
            raise SystemExit("refusing to write outside build/")
        out.mkdir(parents=True, exist_ok=True)
        meta = {"exe_sha256": sha256_bytes(exe), "pmw1_offset": p.base, "version": p.version_text(),
                "flags": p.flags, "cs_obj": p.cs_obj, "eip": p.eip, "ss_obj": p.ss_obj, "esp": p.esp,
                "objects": []}
        for idx, data, span in p.object_data():
            (out / f"obj{idx}.bin").write_bytes(data)
            o = dict(p.objects[idx - 1])
            o.update({"index": idx, "file_span": list(span), "sha256": sha256_bytes(data),
                      "reloc_blocks": [list(b) for b in spans[idx]]})
            meta["objects"].append(o)
            print(f"  obj{idx}: decoded {len(data)} bytes sha256 {sha256_bytes(data)[:16]}")
        dump_json(out / "pmw1.json", meta)
        dump_json(out / "fixups.json", {str(k): v for k, v in fx.items()})
        print(f"wrote {out}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
