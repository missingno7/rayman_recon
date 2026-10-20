"""Locate Android initialised data objects (named, sized) inside the DOS data object by content.

    python tools/androiddata.py [--oracle NAME] [--min 8]    write build/xref/android_data.json, print summary

Source: lib/x86/libSparkApp.so from the user-supplied APK in assets/Android (extracted to build/android/, read as
data only, never executed). For every dynamic OBJECT symbol in .data/.rodata whose size >= --min, its initial
bytes are searched in DOS obj2's initialised region. Objects whose Android image holds relocated pointers cannot
match; such misses are expected. A hit is accepted as a binding candidate only if it is unique in DOS and the
bytes are not trivially repetitive (all-zero / single byte value).
"""
import struct
import sys
import zipfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import ASSETS, BUILD, dump_json  # noqa: E402
from image import Image  # noqa: E402


def extract_so():
    out = BUILD / "android" / "x86_libSparkApp.so"
    if not out.exists():
        apk = next((ASSETS / "Android").glob("*.apk"))
        out.parent.mkdir(parents=True, exist_ok=True)
        out.write_bytes(zipfile.ZipFile(apk).read("lib/x86/libSparkApp.so"))
    return out.read_bytes()


def elf_sections(b):
    shoff, = struct.unpack_from("<I", b, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", b, 0x2E)
    secs = []
    for i in range(shnum):
        f = struct.unpack_from("<IIIIIIIIII", b, shoff + i * shentsize)
        secs.append({"name_off": f[0], "type": f[1], "addr": f[3], "offset": f[4], "size": f[5], "link": f[6],
                     "entsize": f[9]})
    strtab = secs[shstrndx]
    for s in secs:
        o = strtab["offset"] + s["name_off"]
        s["name"] = b[o:b.index(b"\0", o)].decode()
    return secs


def dynsyms(b, secs, stype=1):
    dyn = next(s for s in secs if s["name"] == ".dynsym")
    strs = secs[dyn["link"]]
    out = []
    for k in range(dyn["size"] // 16):
        name_off, value, size, info, other, shndx = struct.unpack_from("<IIIBBH", b, dyn["offset"] + 16 * k)
        if (info & 0xF) != stype or shndx >= len(secs) or size == 0:     # 1 = STT_OBJECT, 2 = STT_FUNC
            continue
        o = strs["offset"] + name_off
        name = b[o:b.index(b"\0", o)].decode("latin-1")
        out.append({"name": name, "addr": value, "size": size, "section": secs[shndx]["name"], "sec": secs[shndx]})
    return out


def main(argv):
    name = argv[argv.index("--oracle") + 1] if "--oracle" in argv else None
    minsize = int(argv[argv.index("--min") + 1]) if "--min" in argv else 8
    img = Image(name)
    init_end = next(o for o in img.meta["objects"] if o["index"] == 2)["data_size"]
    dos = img.obj[2][:init_end]
    b = extract_so()
    secs = elf_sections(b)
    rows, stats = [], {"candidates": 0, "unique": 0, "ambiguous": 0, "absent": 0, "trivial": 0}
    for s in dynsyms(b, secs):
        if s["section"] not in (".data", ".rodata") or s["size"] < minsize:
            continue
        stats["candidates"] += 1
        sec = s["sec"]
        blob = b[sec["offset"] + s["addr"] - sec["addr"]:][:s["size"]]
        if len(set(blob)) <= 1:
            stats["trivial"] += 1
            continue
        hits, p = [], dos.find(blob)
        while p != -1 and len(hits) < 3:
            hits.append(p)
            p = dos.find(blob, p + 1)
        if not hits:
            stats["absent"] += 1
            continue
        if len(hits) > 1:
            stats["ambiguous"] += 1
            continue
        stats["unique"] += 1
        rows.append({"name": s["name"], "addr": f"2:{hits[0]:05x}", "size": s["size"], "android_section": s["section"]})
    rows.sort(key=lambda r: r["addr"])
    dump_json(BUILD / "xref" / "android_data.json", rows)
    print(f"[{img.name}] Android .data/.rodata objects >= {minsize} bytes: {stats}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
