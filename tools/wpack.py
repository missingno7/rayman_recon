#!/usr/bin/env python3
"""Pure-Python reader for Watcom wpack installer archives.

Archive bytes and their numbered volume parts are read-only. Members are
validated before they are atomically written beneath build/wpack/ of this repository.
Provenance: written by a Codex worker for rayman_recon (2026-10-20) as a port of the Open Watcom v2 wpack decoder
(bld/wpack, commit 979e202f); used once to recover Watcom 10.5 BINW/WCC386.EXE (docs/TOOLCHAIN.md).

    python tools/wpack.py verify      unpack the 10.5 CD's DISKIMGS set and compare with its plain tree
"""
from __future__ import annotations

import argparse
import csv
import datetime as dt
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import re
import struct
import sys
import zlib
from dataclasses import dataclass
from typing import Sequence

SCRIPT_DIR = Path(__file__).resolve().parent.parent / "build" / "wpack"
SIGNATURE = 0x2403
RING_SIZE = 4096
LOOKAHEAD = 60
THRESHOLD = 2
NUM_CHARS = 256 - THRESHOLD + LOOKAHEAD
MAX_CODE_BITS = 16
MASK16 = 0xFFFF
ARCHIVE_RE = re.compile(r"^(PCK\d+)(?:\.(\d+))?$", re.IGNORECASE)


class WpackError(Exception):
    """Invalid or unsupported archive data."""


@dataclass(frozen=True)
class Member:
    name: str
    size: int
    disk_addr: int
    stamp: int
    crc32: int
    no_shannon: bool


@dataclass(frozen=True)
class Archive:
    label: str
    paths: tuple[Path, ...]
    data: bytes
    members: tuple[Member, ...]
    info_offset: int
    info_len: int


@dataclass(frozen=True)
class SetupManifest:
    records: dict[str, list[dict]]
    dirs: dict[int, str]


def date_text(stamp: int) -> str:
    try:
        return dt.datetime.fromtimestamp(stamp, dt.timezone.utc).isoformat()
    except (OverflowError, OSError, ValueError):
        return f"epoch:{stamp}"


def archive_parts(paths: Sequence[Path], search_root: Path | None = None) -> tuple[str, tuple[Path, ...]]:
    if not paths:
        raise WpackError("no archive paths supplied")
    resolved = [p.expanduser().resolve() for p in paths]
    for path in resolved:
        if not path.is_file():
            raise WpackError(f"archive part not found: {path}")
    matches = [ARCHIVE_RE.match(path.name) for path in resolved]
    if any(match is None for match in matches):
        raise WpackError("archive names must be PCK followed by digits, optionally .1/.2")
    bases = {match.group(1).casefold() for match in matches if match}
    if len(bases) != 1:
        raise WpackError("archive part arguments refer to different PCK names")
    label = matches[0].group(1).upper()  # type: ignore[union-attr]
    supplied = {int(match.group(2) or 0): path for match, path in zip(matches, resolved) if match}
    if len(supplied) != len(resolved):
        raise WpackError(f"{label}: duplicate part number supplied")

    if any(supplied):
        root = search_root
        if root is None:
            root = next((parent for parent in resolved[0].parents
                         if parent.name.casefold() == "diskimgs"), resolved[0].parent)
        candidates: dict[int, Path] = {}
        for path in root.rglob("*"):
            if not path.is_file():
                continue
            match = ARCHIVE_RE.match(path.name)
            if not match or match.group(1).casefold() != label.casefold():
                continue
            part = int(match.group(2) or 0)
            if part in candidates and candidates[part].resolve() != path.resolve():
                if part not in supplied:
                    raise WpackError(f"ambiguous {label} part .{part} below {root}")
            else:
                candidates[part] = path.resolve()
        candidates.update(supplied)
        numbers = sorted(candidates)
        if 0 in numbers:
            raise WpackError(f"{label}: numbered parts cannot be mixed with an unsplit archive")
        if numbers != list(range(1, max(numbers) + 1)):
            raise WpackError(f"{label}: missing split part(s); found {numbers}")
        parts = tuple(candidates[number] for number in numbers)
    else:
        if len(resolved) != 1:
            raise WpackError(f"{label}: unsplit archive has multiple input paths")
        parts = (resolved[0],)
    return label, parts


def parse_archive(label: str, paths: Sequence[Path]) -> Archive:
    data = b"".join(path.read_bytes() for path in paths)
    if len(data) < 12:
        raise WpackError(f"{label}: truncated archive header ({len(data)} bytes)")
    # Retail v1.1 archives have a 12-byte header. Later wpack builds read an
    # extra internal field at offset 12, which is packed data in these archives.
    signature, major, minor, count, info_len, info_offset = struct.unpack_from("<HBBHHI", data, 0)
    if signature != SIGNATURE:
        raise WpackError(f"{label}: bad signature 0x{signature:04X}, expected 0x{SIGNATURE:04X}")
    if major != 1 or minor > 1:
        raise WpackError(f"{label}: unsupported archive version {major}.{minor}")
    if info_offset < 12 or info_offset + info_len > len(data):
        raise WpackError(f"{label}: file information range is outside the joined archive")
    pos = info_offset
    info_end = info_offset + info_len
    members: list[Member] = []
    for index in range(count):
        if pos + 17 > info_end:
            raise WpackError(f"{label}: truncated member {index + 1} metadata")
        size, disk_addr, stamp, crc, nameflag = struct.unpack_from("<IIIIB", data, pos)
        name_len = nameflag & 0x7F
        end = pos + 17 + name_len
        if end > info_end:
            raise WpackError(f"{label}: truncated member {index + 1} name")
        name = data[pos + 17:end].decode("cp437")
        if not name or "\x00" in name:
            raise WpackError(f"{label}: invalid empty or NUL-containing member name")
        members.append(Member(name, size, disk_addr, stamp, crc, bool(nameflag & 0x80)))
        pos = end
    if pos != info_end:
        raise WpackError(f"{label}: metadata length mismatch ({info_end - pos} unparsed bytes)")
    addresses = [member.disk_addr for member in members]
    if addresses != sorted(addresses):
        raise WpackError(f"{label}: member data offsets are not in archive order")
    for index, member in enumerate(members):
        end = addresses[index + 1] if index + 1 < len(members) else info_offset
        if member.disk_addr < 12 or end < member.disk_addr or end > info_offset:
            raise WpackError(f"{label}:{member.name}: invalid packed byte range")
    return Archive(label, tuple(paths), data, tuple(members), info_offset, info_len)


def read_archive(paths: Sequence[Path], search_root: Path | None = None) -> Archive:
    label, parts = archive_parts(paths, search_root)
    return parse_archive(label, parts)


def discover_archives(root: Path) -> list[tuple[str, tuple[Path, ...]]]:
    root = root.resolve()
    found: dict[str, dict[int, Path]] = {}
    labels: dict[str, str] = {}
    for path in root.rglob("*"):
        if not path.is_file():
            continue
        match = ARCHIVE_RE.match(path.name)
        if not match:
            continue
        key = match.group(1).casefold()
        part = int(match.group(2) or 0)
        if part in found.setdefault(key, {}):
            raise WpackError(f"duplicate PCK name/part {path.name}: {found[key][part]} and {path}")
        found[key][part] = path.resolve()
        labels[key] = match.group(1).upper()
    result = []
    for key in sorted(found, key=lambda value: int(re.search(r"\d+", value).group())):
        parts = found[key]
        numbers = sorted(parts)
        if 0 in parts:
            if len(parts) != 1:
                raise WpackError(f"{labels[key]}: unsplit archive collides with numbered part(s)")
            ordered = (parts[0],)
        else:
            if numbers != list(range(1, max(numbers) + 1)):
                raise WpackError(f"{labels[key]}: missing split part(s); found {numbers}")
            ordered = tuple(parts[number] for number in numbers)
        result.append((labels[key], ordered))
    return result


def safe_member_path(name: str) -> PurePosixPath:
    normalized = name.replace("\\", "/")
    candidate = PurePosixPath(normalized)
    if candidate.is_absolute() or any(part in ("", ".", "..") for part in candidate.parts):
        raise WpackError(f"unsafe archive member path: {name!r}")
    if candidate.parts and re.match(r"^[A-Za-z]:", candidate.parts[0]):
        raise WpackError(f"drive-qualified archive member path: {name!r}")
    return candidate


class BitReader:
    def __init__(self, data: bytes, start: int):
        self.data = data
        self.pos = start
        self.getbuf = 0
        self.getlen = 0
        self.secondbuf = 0

    def read_byte(self) -> int:
        if self.pos >= len(self.data):
            raise WpackError("compressed bitstream reached end of archive")
        value = self.data[self.pos]
        self.pos += 1
        return value

    def get_byte(self) -> int:
        value = self.getbuf >> 8
        if self.getlen >= 8:
            self.getbuf = (self.getbuf << 8) & MASK16
            self.getlen -= 8
        else:
            self.getbuf = self.secondbuf
            value |= self.getbuf >> self.getlen
            self.getbuf = (self.getbuf << (16 - self.getlen)) & MASK16
            self.secondbuf = self.read_byte()
        return value

    def decode_position(self, d_code: Sequence[int], d_len: Sequence[int]) -> int:
        value = self.get_byte()
        position = d_code[value] << 6
        bits = d_len[value] - 2
        if bits > self.getlen:
            self.getbuf = (self.getbuf | (self.secondbuf << (8 - self.getlen))) & MASK16
            self.getlen += 8
            self.secondbuf = self.read_byte()
        value = (value << bits) | (self.getbuf >> (16 - bits))
        self.getbuf = (self.getbuf << bits) & MASK16
        self.getlen -= bits
        return position | (value & 0x3F)


_P_LEN = (
    3, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
)
_P_CODE = (
    0x00, 0x20, 0x30, 0x40, 0x50, 0x58, 0x60, 0x68,
    0x70, 0x78, 0x80, 0x88, 0x90, 0x94, 0x98, 0x9C,
    0xA0, 0xA4, 0xA8, 0xAC, 0xB0, 0xB4, 0xB8, 0xBC,
    0xC0, 0xC2, 0xC4, 0xC6, 0xC8, 0xCA, 0xCC, 0xCE,
    0xD0, 0xD2, 0xD4, 0xD6, 0xD8, 0xDA, 0xDC, 0xDE,
    0xE0, 0xE2, 0xE4, 0xE6, 0xE8, 0xEA, 0xEC, 0xEE,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
    0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
)


def position_tables() -> tuple[tuple[int, ...], tuple[int, ...]]:
    d_code = [0] * 256
    d_len = [0] * 256
    for code_index, (code, bit_len) in enumerate(zip(_P_CODE, _P_LEN)):
        prefix = code >> (8 - bit_len)
        repetitions = 1 << (8 - bit_len)
        for tail in range(repetitions):
            slot = (prefix << (8 - bit_len)) | tail
            d_code[slot] = code_index
            d_len[slot] = bit_len
    return tuple(d_code), tuple(d_len)


_D_CODE, _D_LEN = position_tables()


def qsort_indices(items: list[int], lengths: Sequence[int]) -> None:
    """Port Open Watcom's wpack_qsort for 4-byte index values."""
    def cmp(a: int, b: int) -> int:
        return lengths[a] - lengths[b]

    def med3(a: int, b: int, c: int) -> int:
        if cmp(items[a], items[b]) > 0:
            if cmp(items[a], items[c]) > 0:
                return b if cmp(items[b], items[c]) > 0 else c
            return a
        if cmp(items[a], items[c]) >= 0:
            return a
        return c if cmp(items[b], items[c]) > 0 else b

    base, n = 0, len(items)
    stack: list[tuple[int, int]] = []
    while True:
        while n > 1:
            if n < 16:
                for gap in (3, 1):
                    for p1 in range(base + gap, base + n, gap):
                        p2 = p1
                        while p2 > base and cmp(items[p2 - gap], items[p2]) > 0:
                            items[p2], items[p2 - gap] = items[p2 - gap], items[p2]
                            p2 -= gap
                break
            mid = base + (n >> 1)
            if n > 29:
                p1, p2 = base, base + n - 1
                if n > 42:
                    step = n >> 3
                    p1 = med3(p1, p1 + step, p1 + (step << 1))
                    mid = med3(mid - step, mid, mid + step)
                    p2 = med3(p2 - (step << 1), p2 - step, p2)
                mid = med3(p1, mid, p2)
            pivot = items[mid]
            pa = pb = base
            pc = pd = base + n - 1
            while True:
                while pb <= pc:
                    comparison = cmp(items[pb], pivot)
                    if comparison > 0:
                        break
                    if comparison == 0:
                        items[pa], items[pb] = items[pb], items[pa]
                        pa += 1
                    pb += 1
                while pc >= pb:
                    comparison = cmp(items[pc], pivot)
                    if comparison < 0:
                        break
                    if comparison == 0:
                        items[pc], items[pd] = items[pd], items[pc]
                        pd -= 1
                    pc -= 1
                if pb > pc:
                    break
                items[pb], items[pc] = items[pc], items[pb]
                pb += 1
                pc -= 1
            pn = base + n
            swap = min(pa - base, pb - pa)
            for offset in range(swap):
                items[base + offset], items[pb - swap + offset] = items[pb - swap + offset], items[base + offset]
            swap = min(pd - pc, pn - pd - 1)
            for offset in range(swap):
                items[pb + offset], items[pn - swap + offset] = items[pn - swap + offset], items[pb + offset]
            left_n, right_n = pb - pa, pd - pc
            if right_n >= left_n:
                stack.append((pn - right_n, right_n))
                n = left_n
            else:
                if left_n <= 1:
                    break
                stack.append((base, left_n))
                base, n = pn - right_n, right_n
        if not stack:
            break
        base, n = stack.pop()


def shannon_codes(reader: BitReader) -> tuple[list[int], list[int], list[int], int]:
    current = 0
    symbols: list[int] = []
    lengths = [0] * NUM_CHARS
    for _ in range(reader.read_byte() + 1):
        entry = reader.read_byte()
        if entry & 0x80:
            current += (entry & 0x7F) + 1
        else:
            count = (entry >> 4) + 1
            bit_len = (entry & 0x0F) + 1
            for _ in range(count):
                if current >= NUM_CHARS or len(symbols) >= NUM_CHARS:
                    raise WpackError("Shannon-Fano table exceeds the symbol range")
                symbols.append(current)
                lengths[current] = bit_len
                current += 1
    if not symbols:
        raise WpackError("empty Shannon-Fano table")
    qsort_indices(symbols, lengths)
    min_val = [0xFFFF] * (MAX_CODE_BITS + 1)
    map_offset = [0] * (MAX_CODE_BITS + 1)
    char_map = [0] * NUM_CHARS
    codeval = codeinc = lastlen = curroffset = 0
    min_code_len = lengths[symbols[0]]
    for symbol in reversed(symbols):
        codeval = (codeval + codeinc) & MASK16
        bit_len = lengths[symbol]
        if bit_len != lastlen:
            if not 1 <= bit_len <= MAX_CODE_BITS:
                raise WpackError(f"invalid Shannon-Fano code length {bit_len}")
            lastlen = bit_len
            codeinc = 1 << (16 - lastlen)
            min_val[lastlen] = codeval
            map_offset[lastlen] = curroffset
        char_map[curroffset] = symbol
        curroffset += 1
    return min_val, map_offset, char_map, min_code_len


def decode_member(archive: Archive, index: int) -> bytes:
    member = archive.members[index]
    start = member.disk_addr
    end = archive.members[index + 1].disk_addr if index + 1 < len(archive.members) else archive.info_offset
    packed = archive.data[start:end]
    calculated_crc = zlib.crc32(packed) & 0xFFFFFFFF
    if calculated_crc != member.crc32:
        raise WpackError(f"{archive.label}:{member.name}: packed CRC mismatch "
                         f"(stored {member.crc32:08x}, calculated {calculated_crc:08x})")
    if member.size == 0:
        return b""

    reader = BitReader(archive.data, start)
    ring = bytearray(RING_SIZE)
    ring[:RING_SIZE - LOOKAHEAD] = b" " * (RING_SIZE - LOOKAHEAD)
    r = RING_SIZE - LOOKAHEAD
    output = bytearray()

    def emit(value: int) -> None:
        nonlocal r
        if len(output) >= member.size + LOOKAHEAD:
            raise WpackError(f"{archive.label}:{member.name}: decoded output exceeds declared size")
        value &= 0xFF
        output.append(value)
        ring[r] = value
        r = (r + 1) & (RING_SIZE - 1)

    if member.no_shannon:
        reader.getlen = reader.getbuf = 0
        reader.secondbuf = reader.read_byte()
        count = 0
        while count < member.size:
            if reader.getlen == 0:
                reader.getbuf = ((reader.secondbuf << 8) | reader.read_byte()) & MASK16
                reader.getlen = 16
                reader.secondbuf = reader.read_byte()
            elif reader.getlen <= 8:
                reader.getbuf = (reader.getbuf | (reader.secondbuf << (8 - reader.getlen))) & MASK16
                reader.getlen += 8
                reader.secondbuf = reader.read_byte()
            if reader.getbuf & 0x8000:
                run_len = (reader.getbuf >> 9) & 0x3F
                if run_len == 0:
                    raise WpackError(f"{archive.label}:{member.name}: zero-length LZSS back-reference")
                reader.getlen -= 7
                reader.getbuf = (reader.getbuf << 7) & MASK16
                position = (r - reader.decode_position(_D_CODE, _D_LEN) - 1) & (RING_SIZE - 1)
                for k in range(run_len):
                    emit(ring[(position + k) & (RING_SIZE - 1)])
                count += run_len
            else:
                value = reader.getbuf >> 7
                reader.getbuf = (reader.getbuf << 9) & MASK16
                reader.getlen -= 9
                emit(value)
                count += 1
    else:
        min_val, map_offset, char_map, min_code_len = shannon_codes(reader)
        reader.getlen = reader.getbuf = 0
        reader.secondbuf = reader.read_byte()
        count = 0
        mask = (0, 1, 3, 7, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF)
        while count < member.size:
            if reader.getlen < 8:
                reader.getbuf = (reader.getbuf | (reader.secondbuf << (8 - reader.getlen))) & MASK16
                reader.getlen += 8
                reader.secondbuf = reader.read_byte()
            spare = reader.getlen - 8
            reader.getlen = 16
            reader.getbuf = (reader.getbuf | (reader.secondbuf >> spare)) & MASK16
            code_len = min_code_len
            while code_len <= MAX_CODE_BITS and reader.getbuf < min_val[code_len]:
                code_len += 1
            if code_len > MAX_CODE_BITS or min_val[code_len] == 0xFFFF:
                raise WpackError(f"{archive.label}:{member.name}: invalid Shannon-Fano code")
            slot = map_offset[code_len] + ((reader.getbuf - min_val[code_len]) >> (16 - code_len))
            if slot >= len(char_map):
                raise WpackError(f"{archive.label}:{member.name}: Shannon-Fano symbol index out of range")
            symbol = char_map[slot]
            reader.getbuf = (reader.getbuf << code_len) & MASK16
            reader.getlen -= code_len
            if spare > code_len:
                reader.getlen -= 8 - spare
            else:
                reader.getbuf = (reader.getbuf | ((reader.secondbuf & mask[spare]) << (code_len - spare))) & MASK16
                reader.getlen += spare
                reader.secondbuf = reader.read_byte()
            if symbol < 256:
                emit(symbol)
                count += 1
            else:
                position = (r - reader.decode_position(_D_CODE, _D_LEN) - 1) & (RING_SIZE - 1)
                run_len = symbol - 255 + THRESHOLD
                for k in range(run_len):
                    emit(ring[(position + k) & (RING_SIZE - 1)])
                count += run_len
    if len(output) != member.size:
        raise WpackError(f"{archive.label}:{member.name}: size mismatch "
                         f"(stored {member.size}, decoded {len(output)})")
    return bytes(output)


def output_root(path: str | Path) -> Path:
    root = Path(path).expanduser().resolve()
    try:
        common = Path(os.path.commonpath((str(SCRIPT_DIR), str(root))))
    except ValueError as exc:
        raise WpackError(f"output directory must be under {SCRIPT_DIR}") from exc
    if os.path.normcase(str(common)) != os.path.normcase(str(SCRIPT_DIR)):
        raise WpackError(f"output directory must be under {SCRIPT_DIR}: {root}")
    return root


def write_atomic(path: Path, content: bytes, stamp: int) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    temp = path.with_name(path.name + f".wpack-tmp-{os.getpid()}")
    try:
        with open(temp, "wb") as handle:
            handle.write(content)
            handle.flush()
            os.fsync(handle.fileno())
        os.replace(temp, path)
        try:
            os.utime(path, (stamp, stamp))
        except (OverflowError, OSError, ValueError):
            pass
    finally:
        try:
            temp.unlink(missing_ok=True)
        except OSError:
            pass


def extract_archives(out_dir: Path, archives: Sequence[Archive]) -> list[dict]:
    records = []
    for archive in archives:
        archive_dir = out_dir / archive.label
        seen: set[str] = set()
        for index, member in enumerate(archive.members):
            relative = safe_member_path(member.name)
            key = relative.as_posix().casefold()
            if key in seen:
                raise WpackError(f"{archive.label}: duplicate output member path {member.name!r}")
            seen.add(key)
            content = decode_member(archive, index)
            destination = archive_dir.joinpath(*relative.parts)
            write_atomic(destination, content, member.stamp)
            records.append({
                "archive": archive.label, "member": member.name, "size": len(content),
                "packed_crc32": f"{member.crc32:08x}", "mtime_utc": date_text(member.stamp),
                "output_path": str(destination.resolve()),
                "sha256": hashlib.sha256(content).hexdigest(),
            })
    return records


def load_manifest(path: Path) -> SetupManifest:
    text = path.read_text(encoding="cp1252", errors="replace")
    in_files = False
    in_dirs = False
    records: dict[str, list[dict]] = {}
    dirs: dict[int, str] = {}
    pending: list[str] = []
    pending_start = 0

    def flush() -> None:
        nonlocal pending
        if not pending:
            return
        logical = "".join(line.rstrip().rstrip("\\").strip() for line in pending)
        fields = [item.strip() for item in logical.split(",")]
        if fields and re.fullmatch(r"(?i)pck\d+(?:\.\d+)?", fields[0]):
            archive = fields[0].upper()
            try:
                count = int(fields[1], 36)
            except (IndexError, ValueError):
                count = 0
            names = []
            for token in fields[2:2 + count]:
                if token and token != "\\":
                    name = token.split("!", 1)[0].strip().strip('"')
                    if name and name != "\\":
                        names.append(name)
            destination_id = None
            destination_dir = None
            if len(fields) > 2 + count:
                try:
                    destination_id = int(fields[2 + count], 36)
                    destination_dir = dirs.get(destination_id)
                except ValueError:
                    pass
            key = re.sub(r"\.\d+$", "", archive, flags=re.IGNORECASE).casefold()
            records.setdefault(key, []).append({
                "archive_record": archive, "manifest_line": pending_start,
                "members": names, "raw_record": logical,
                "destination_dir_id": destination_id, "destination_dir": destination_dir,
            })
        pending = []

    for line_no, raw in enumerate(text.splitlines(), 1):
        stripped = raw.strip()
        if stripped.startswith("["):
            flush()
            in_files = stripped.casefold() == "[files]"
            in_dirs = stripped.casefold() == "[dirs]"
            continue
        if not stripped or stripped.startswith(";"):
            continue
        if in_dirs:
            fields = [part.strip() for part in stripped.split(",")]
            if len(fields) >= 3:
                dirs[len(dirs) + 1] = fields[0]
            continue
        if not in_files:
            continue
        if pending:
            pending.append(raw)
        elif re.match(r"(?i)^pck\d+(?:\.\d+)?\s*,", stripped):
            pending_start = line_no
            pending = [raw]
        if pending and not raw.rstrip().endswith("\\"):
            flush()
    flush()
    return SetupManifest(records, dirs)


def plain_index(root: Path) -> tuple[dict[str, list[Path]], dict[str, int], dict[str, Path]]:
    by_name: dict[str, list[Path]] = {}
    sizes: dict[str, int] = {}
    by_relative_path: dict[str, Path] = {}
    diskimgs = root / "DISKIMGS"
    for path in root.rglob("*"):
        if not path.is_file():
            continue
        try:
            path.relative_to(diskimgs)
            continue
        except ValueError:
            pass
        resolved = path.resolve()
        by_name.setdefault(path.name.casefold(), []).append(resolved)
        by_relative_path[path.relative_to(root).as_posix().casefold()] = resolved
        try:
            sizes[str(resolved)] = path.stat().st_size
        except OSError:
            pass
    return by_name, sizes, by_relative_path


def setup_member_map(records: Sequence[dict], manifest: SetupManifest) -> dict[tuple[str, str], dict]:
    """Map archived names to SETUP.INF destination names and directory selectors."""
    grouped: dict[str, list[dict]] = {}
    for record in records:
        grouped.setdefault(record["archive"].casefold(), []).append(record)
    result: dict[tuple[str, str], dict] = {}
    for archive, actual_records in grouped.items():
        expected = [
            {"name": name, "entry": entry, "used": False}
            for entry in manifest.records.get(archive, [])
            for name in entry["members"]
        ]
        unmatched_actual = []
        for record in actual_records:
            actual_name = PurePosixPath(record["member"].replace("\\", "/")).as_posix().casefold()
            match = next((item for item in expected
                          if not item["used"] and PurePosixPath(item["name"].replace("\\", "/")).as_posix().casefold() == actual_name), None)
            if match is None:
                unmatched_actual.append(record)
            else:
                match["used"] = True
                entry = match["entry"]
                result[(archive, record["member"].casefold())] = {
                    "setup_member": match["name"], "destination_dir_id": entry["destination_dir_id"],
                    "destination_dir": entry["destination_dir"], "manifest_line": entry["manifest_line"],
                    "alias": False,
                }
        unmatched_expected = [item for item in expected if not item["used"]]
        # Installer names can differ from the short names stored in a PCK.
        # Assign only an unambiguous one-to-one remainder; otherwise use the
        # archive name fallback rather than guessing among several files.
        if len(unmatched_actual) == 1 and len(unmatched_expected) == 1:
            record = unmatched_actual[0]
            item = unmatched_expected[0]
            entry = item["entry"]
            result[(archive, record["member"].casefold())] = {
                "setup_member": item["name"], "destination_dir_id": entry["destination_dir_id"],
                "destination_dir": entry["destination_dir"], "manifest_line": entry["manifest_line"],
                "alias": True,
            }
    return result


def compare_records(records: Sequence[dict], by_name: dict[str, list[Path]],
                    sizes: dict[str, int], by_relative_path: dict[str, Path],
                    manifest_map: dict[tuple[str, str], dict]) -> tuple[list[dict], dict[str, int]]:
    rows = []
    counts = {"identical": 0, "different": 0, "no_counterpart": 0}
    hashes: dict[str, str] = {}
    for record in records:
        setup_info = manifest_map.get((record["archive"].casefold(), record["member"].casefold()), {})
        setup_member = setup_info.get("setup_member", record["member"])
        member_path = PurePosixPath(setup_member.replace("\\", "/"))
        basename = member_path.name.casefold()
        archived_basename = PurePosixPath(record["member"].replace("\\", "/")).name.casefold()
        destination_dir = setup_info.get("destination_dir")
        setup_destination = None
        if destination_dir is not None:
            dir_path = PurePosixPath(destination_dir.replace("\\", "/"))
            expected_relative = PurePosixPath(*(part for part in dir_path.parts if part != "."), *member_path.parts)
            setup_destination = expected_relative.as_posix()
        expected_path = by_relative_path.get(setup_destination.casefold()) if setup_destination else None
        if expected_path is not None:
            candidates = [expected_path]
        else:
            candidates = []
            for key in dict.fromkeys((archived_basename, basename)):
                for path in by_name.get(key, []):
                    if path not in candidates:
                        candidates.append(path)
        exact_size = [path for path in candidates if sizes.get(str(path)) == record["size"]]
        match_method = "SETUP.INF destination path" if expected_path is not None else "filename+size fallback"
        if not candidates:
            status, reason, same = "no_counterpart", "no plain-tree file with this basename", []
        elif not exact_size:
            status = "different"
            if expected_path is not None:
                reason = f"SETUP.INF destination exists at {sizes.get(str(expected_path))} bytes; extracted size is {record['size']}"
            else:
                reason = "basename exists, but no candidate has the extracted size"
            same = []
        else:
            same = []
            for path in exact_size:
                key = str(path)
                if key not in hashes:
                    digest = hashlib.sha256()
                    with open(path, "rb") as handle:
                        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
                            digest.update(chunk)
                    hashes[key] = digest.hexdigest()
                if hashes[key] == record["sha256"]:
                    same.append(key)
            if same:
                status, reason = "identical", "SHA-256 matches"
            else:
                status, reason = "different", "same basename and size, but SHA-256 differs"
        counts[status] += 1
        rows.append({
            "archive": record["archive"], "member": record["member"], "size": record["size"],
            "status": status, "reason": reason, "sha256": record["sha256"],
            "match_method": match_method, "setup_destination": setup_destination,
            "setup_member": setup_member if setup_info else None,
            "manifest_line": setup_info.get("manifest_line"),
            "manifest_alias": bool(setup_info.get("alias", False)),
            "same_size_candidates": [str(path) for path in exact_size],
            "identical_plain_paths": same,
            "basename_candidates": [{"path": str(path), "size": sizes.get(str(path))} for path in candidates],
        })
    return rows, counts


def write_listing(path: Path, records: Sequence[dict]) -> None:
    with path.open("w", encoding="utf-8", newline="") as handle:
        writer = csv.writer(handle, delimiter="\t", lineterminator="\n")
        writer.writerow(("archive", "member", "size", "packed_crc32", "mtime_utc", "sha256", "output_path"))
        for row in records:
            writer.writerow((row["archive"], row["member"], row["size"], row["packed_crc32"],
                             row["mtime_utc"], row["sha256"], row["output_path"]))


def write_comparison(path: Path, rows: Sequence[dict]) -> None:
    with path.open("w", encoding="utf-8", newline="") as handle:
        writer = csv.writer(handle, lineterminator="\n")
        writer.writerow(("archive", "member", "size", "status", "setup_member", "manifest_line", "match_method",
                         "setup_destination", "reason", "sha256",
                         "same_size_candidates", "identical_plain_paths", "basename_candidates"))
        for row in rows:
            writer.writerow((row["archive"], row["member"], row["size"], row["status"], row["setup_member"] or "",
                             row["manifest_line"] or "",
                             row["match_method"], row["setup_destination"] or "",
                             row["reason"], row["sha256"],
                             json.dumps(row["same_size_candidates"], ensure_ascii=False),
                             json.dumps(row["identical_plain_paths"], ensure_ascii=False),
                             json.dumps(row["basename_candidates"], ensure_ascii=False)))


def target_tools(records: Sequence[dict], manifest: SetupManifest) -> dict:
    names = ("wcc386.exe", "wpp386.exe", "wlink.exe", "wasm.exe", "wlib.exe")
    manifest_lines: dict[tuple[str, str], list[int]] = {}
    for archive_key, entries in manifest.records.items():
        for entry in entries:
            for member in entry["members"]:
                key = (archive_key, Path(member.replace("\\", "/")).name.casefold())
                manifest_lines.setdefault(key, []).append(entry["manifest_line"])
    result: dict[str, list[dict]] = {name: [] for name in names}
    for record in records:
        basename = Path(record["member"].replace("\\", "/")).name.casefold()
        if basename in result:
            result[basename].append({
                "archive": record["archive"], "member": record["member"], "size": record["size"],
                "sha256": record["sha256"], "output_path": record["output_path"],
                "manifest_lines": manifest_lines.get((record["archive"].casefold(), basename), []),
            })
    return result


def verify(args: argparse.Namespace) -> int:
    diskimgs, plain_root, manifest_path = Path(args.diskimgs), Path(args.plain_tree), Path(args.setup_inf)
    out_dir = output_root(args.outdir)
    result_path = output_root(args.result_json)
    table_path = output_root(args.comparison_table)
    listing_path = output_root(args.archive_list)
    if not diskimgs.is_dir():
        raise WpackError(f"DISKIMGS directory not found: {diskimgs}")
    if not plain_root.is_dir():
        raise WpackError(f"plain tree not found: {plain_root}")
    if not manifest_path.is_file():
        raise WpackError(f"SETUP.INF not found: {manifest_path}")
    groups = discover_archives(diskimgs)
    if not groups:
        raise WpackError(f"no PCK archives found under {diskimgs}")
    archives = [parse_archive(label, paths) for label, paths in groups]
    manifest = load_manifest(manifest_path)
    by_name, sizes, by_relative_path = plain_index(plain_root)

    out_dir.mkdir(parents=True, exist_ok=True)
    records = extract_archives(out_dir, archives)
    write_listing(listing_path, records)
    manifest_map = setup_member_map(records, manifest)
    comparison, counts = compare_records(records, by_name, sizes, by_relative_path, manifest_map)
    write_comparison(table_path, comparison)
    tools = target_tools(records, manifest)
    recovered = next((row for row in tools["wcc386.exe"] if row["archive"].casefold() == "pck00017"), None)
    if recovered is None:
        raise WpackError("recovered BINW payload PCK00017:wcc386.exe was not found")
    result = {
        "status": "done",
        "format": "Watcom wpack archive v1.1",
        "verification": {
            "diskimgs": str(diskimgs.resolve()), "plain_tree": str(plain_root.resolve()),
            "setup_inf": str(manifest_path.resolve()), "archive_count": len(archives),
            "physical_archive_part_count": sum(len(paths) for _, paths in groups),
            "split_archive_count": sum(len(paths) > 1 for _, paths in groups),
            "split_part_distribution": {
                str(part_count): sum(len(paths) == part_count for _, paths in groups)
                for part_count in sorted({len(paths) for _, paths in groups})
            },
            "extracted_file_count": len(records), "comparison_counts": counts,
            "setup_manifest_mapped_count": sum(bool(row.get("setup_member")) for row in comparison),
            "setup_manifest_alias_count": sum(bool(row.get("manifest_alias")) for row in comparison),
            "filename_fallback_count": sum(row["match_method"] == "filename+size fallback" for row in comparison),
            "comparison_table": str(table_path.resolve()), "archive_listing": str(listing_path.resolve()),
            "output_directory": str(out_dir.resolve()),
            "different_files": [row for row in comparison if row["status"] == "different"],
        },
        "critical_executables": tools,
        "recovered_wcc386": recovered,
        "manifest_tool_entries": {
            name: [
                {"archive_record": entry["archive_record"], "manifest_line": entry["manifest_line"],
                 "members": entry["members"], "raw_record": entry["raw_record"],
                 "destination_dir_id": entry["destination_dir_id"],
                 "destination_dir": entry["destination_dir"]}
                for entries in manifest.records.values() for entry in entries
                if any(Path(member.replace("\\", "/")).name.casefold() == name for member in entry["members"])
            ]
            for name in ("wcc386.exe", "wpp386.exe", "wlink.exe", "wasm.exe", "wlib.exe")
        },
    }
    result_path.parent.mkdir(parents=True, exist_ok=True)
    result_path.write_text(json.dumps(result, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")
    print(f"archives={len(archives)} extracted={len(records)}")
    print("comparison=" + json.dumps(counts, sort_keys=True))
    print(f"table={table_path.resolve()}")
    print(f"result={result_path.resolve()}")
    print(f"recovered_wcc386={recovered['output_path']} size={recovered['size']} sha256={recovered['sha256']}")
    return 0


def cli_archives(paths: Sequence[str]) -> list[Archive]:
    grouped: dict[str, list[Path]] = {}
    order: list[str] = []
    for raw in paths:
        path = Path(raw).expanduser()
        match = ARCHIVE_RE.match(path.name)
        if not match:
            raise WpackError(f"not a PCK archive path: {path}")
        key = match.group(1).casefold()
        if key not in grouped:
            grouped[key] = []
            order.append(key)
        grouped[key].append(path)
    return [read_archive(grouped[key]) for key in order]


def main(argv: Sequence[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    p_list = sub.add_parser("list", help="list member metadata")
    p_list.add_argument("archives", nargs="+")
    p_extract = sub.add_parser("extract", help="extract members beneath OUTDIR")
    p_extract.add_argument("outdir")
    p_extract.add_argument("archives", nargs="+")
    p_verify = sub.add_parser("verify", help="extract all DISKIMGS archives and compare with the plain tree")
    p_verify.add_argument("--diskimgs", default=r"C:\tools\watcom-10.5\DISKIMGS")
    p_verify.add_argument("--plain-tree", default=r"C:\tools\watcom-10.5")
    p_verify.add_argument("--setup-inf", default=r"C:\tools\watcom-10.5\DISKIMGS\DISK01\SETUP.INF")
    p_verify.add_argument("--outdir", default=str(SCRIPT_DIR / "out"))
    p_verify.add_argument("--result-json", default=str(SCRIPT_DIR / "RESULT.json"))
    p_verify.add_argument("--comparison-table", default=str(SCRIPT_DIR / "comparison.csv"))
    p_verify.add_argument("--archive-list", default=str(SCRIPT_DIR / "archive_list.tsv"))
    args = parser.parse_args(argv)
    try:
        if args.command == "list":
            archives = cli_archives(args.archives)
            print("archive\tmember\tsize\tpacked_crc32\tmtime_utc")
            for archive in archives:
                for member in archive.members:
                    print(f"{archive.label}\t{member.name}\t{member.size}\t{member.crc32:08x}\t{date_text(member.stamp)}")
            return 0
        if args.command == "extract":
            out_dir = output_root(args.outdir)
            out_dir.mkdir(parents=True, exist_ok=True)
            records = extract_archives(out_dir, cli_archives(args.archives))
            for row in records:
                print(f"{row['archive']}\t{row['member']}\t{row['size']}\t{row['output_path']}")
            return 0
        return verify(args)
    except (OSError, WpackError, struct.error) as exc:
        print(f"wpack: error: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
