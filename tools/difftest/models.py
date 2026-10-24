"""Environment models of the differential harness (opt-in per function through its domain file).

Every model is applied IDENTICALLY to both executions (original DOS function and our compiled function); it replaces
a deterministic random stub by the behaviour of the real environment, so that loaders, allocators and long loops can
run on realistic data.  None of them inspects which side is running.

* FileLayer: the Watcom C stdio calls fopen / fread / fseek / ftell / fclose operate on the REAL installed game data
  (assets/GOG, read-only, opened at run time and never copied).  A DOS path built by the game ("PCMAP\\JUNGLE\\RAY1.LEV",
  "..\\VIGNET.DAT" is NOT accepted: see resolve()) is mapped case-insensitively below the game directory.
* Allocators: game allocation routines (block_malloc, temp_malloc, ...) return fresh, disjoint, aligned regions of the
  requested size from a bump allocator (ALLOC_BASE), instead of a pointer into a shared 64 KB stub area.
* NoReturn: callees that never return (FatalError ends in exit(-1), src/basic.c:1651) end both executions after the
  call is compared (termination status EXIT).
* Execute: STRICT leaf callees (no calls, verified at load) are executed natively from the oracle image on both
  sides after their call has been compared, instead of being stubbed (realistic results, e.g. sinus_cosinus).
* Boundary: the comparison stops at a well-defined event that is identical on both sides when their event sequences
  agree (the Nth call of a named callee, or the Nth logged event overall = outgoing calls and system instructions).
  Everything before it (all calls, arguments, pointees, memory written before each call and at the boundary) is
  compared; behaviour after it is not judged (termination status BOUNDARY on both sides).
* BIOS timer tick (hardware, automatic): reads of the BIOS data area tick counter 0:046C (dword) by a function that
  references 0x46c observe a counter that advances by one per read access, so DOS busy-waits on the tick terminate.

Allowed environment keys in a domain file (each needs "evidence"):
  "file_layer": {"evidence": ...}
  "allocators": {"callees": {"block_malloc": SIZE_ARG_INDEX, ...}, "evidence": ...}
  "noreturn": {"callees": [NAME, ...], "evidence": ...}
  "execute": {"callees": [NAME, ...], "evidence": ...}
  "boundary": {"callee": NAME, "occurrence": N, "evidence": ...} | {"calls": N, "evidence": ...}
  "budget": {"seconds": S, "steps": N, "events": N, "seeds": N, "evidence": ...}
                                     (per-function time, top-tier instruction/call budgets, main-phase seed count)
  "flat_selectors": {"evidence": ...}      (every selector loads as a flat 4 GB data segment, base 0)
"""
from __future__ import annotations

import os
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = next(p for p in HERE.parents if (p / 'config/units.json').exists())
GAME_ROOT = ROOT / 'assets' / 'GOG'
ALLOC_BASE = 0x30000000
ALLOC_LIMIT = 0x38000000
FILE_HANDLE_BASE = 0x5c000000     # synthetic FILE* identities (opaque; never dereferenced by the game code)
BIOS_TICK = 0x46c
FILE_CALLS = ('fopen', 'fread', 'fseek', 'ftell', 'fclose')
ENV_KEYS = ('file_layer', 'allocators', 'noreturn', 'execute', 'boundary', 'budget', 'flat_selectors')


class ModelError(Exception):
    pass


_FILE_CACHE = {}


def resolve(dos_path, root=GAME_ROOT):
    """Map a DOS path the game builds to a file below the game directory (case-insensitive), or None.

    Only relative paths inside the game directory are mapped: a drive letter, a leading backslash or a '..' component
    (the game's path_has_changed variant, which addresses the parent directory of the game) resolve to None, i.e. the
    file does not exist in the modelled installation and fopen fails (both sides identically)."""
    text = dos_path.replace('/', '\\')
    if not text or ':' in text or text.startswith('\\'):
        return None
    cur = root
    for part in text.split('\\'):
        if part in ('', '.'):
            continue
        if part == '..':
            return None
        if not cur.is_dir():
            return None
        match = [c for c in os.listdir(cur) if c.upper() == part.upper()]
        if len(match) != 1:
            return None
        cur = cur / match[0]
    return cur if cur.is_file() else None


def file_bytes(path):
    path = Path(path)
    if path not in _FILE_CACHE:
        _FILE_CACHE[path] = path.read_bytes()    # read-only; assets are never written or copied
    return _FILE_CACHE[path]


class FileLayer:
    """Per-execution open-file table.  Same call sequence -> same handles, positions and data on both sides."""

    def __init__(self):
        self.files = {}      # handle -> [Path, data, pos]
        self.opened = 0
        self.log = []        # (operation, path, details) for the result summary

    def fopen(self, name, mode):
        if any(c in mode for c in 'wa+'):
            raise ModelError('fopen with a writing mode %r is not modelled (assets are read-only)' % mode)
        path = resolve(name)
        if path is None:
            self.log.append(('fopen', name, 'not found'))
            return 0
        handle = FILE_HANDLE_BASE + self.opened * 0x40
        self.opened += 1
        self.files[handle] = [path, file_bytes(path), 0]
        self.log.append(('fopen', str(path.relative_to(ROOT)), handle))
        return handle

    def fread(self, size, count, handle):
        """(bytes read, items returned).  Watcom/ANSI: reads up to size*count bytes from the current position,
        advances by the bytes actually read and returns the number of complete items."""
        f = self.files.get(handle)
        size &= 0xffffffff; count &= 0xffffffff
        total = (size * count) & 0xffffffff
        if f is None or size == 0 or count == 0:
            return b'', 0
        data = f[1][f[2]:f[2] + total] if f[2] < len(f[1]) else b''
        f[2] += len(data)
        return data, len(data) // size

    def fseek(self, handle, offset, whence):
        f = self.files.get(handle)
        if f is None:
            return -1
        offset = offset - (1 << 32) if offset & 0x80000000 else offset
        base = {0: 0, 1: f[2], 2: len(f[1])}.get(whence)
        if base is None or base + offset < 0:
            return -1
        f[2] = base + offset
        return 0

    def ftell(self, handle):
        f = self.files.get(handle)
        return -1 if f is None else f[2]

    def fclose(self, handle):
        return 0 if self.files.pop(handle, None) is not None else -1


class Allocator:
    def __init__(self):
        self.next = ALLOC_BASE

    def alloc(self, size):
        size &= 0xffffffff
        a = self.next
        self.next = (a + max(size, 1) + 15) & ~15
        if self.next > ALLOC_LIMIT:
            raise ModelError('modelled allocations exceed %d MB' % ((ALLOC_LIMIT - ALLOC_BASE) >> 20))
        return a


class Environment:
    """Parsed environment block of a domain file (empty = the plain v2 harness)."""

    def __init__(self, spec=None, program=None):
        spec = spec or {}
        unknown = set(spec) - set(ENV_KEYS)
        if unknown:
            raise ValueError('unknown environment keys: ' + ', '.join(sorted(unknown)))
        for k, v in spec.items():
            if not isinstance(v, dict) or not v.get('evidence'):
                raise ValueError(f'environment.{k} needs an "evidence" entry')
        self.spec = spec
        self.file_layer = 'file_layer' in spec
        self.allocators = {k: int(v) for k, v in (spec.get('allocators') or {}).get('callees', {}).items()}
        self.noreturn = set((spec.get('noreturn') or {}).get('callees', []))
        self.execute = list((spec.get('execute') or {}).get('callees', []))
        b = spec.get('boundary')
        self.boundary = None
        if b:
            if 'calls' in b:
                self.boundary = ('calls', int(b['calls']))
            else:
                self.boundary = ('callee', b['callee'], int(b['occurrence']))
        budget = spec.get('budget') or {}
        self.seconds = budget.get('seconds')
        self.top_steps = budget.get('steps')
        self.top_events = budget.get('events')
        self.seeds = budget.get('seeds')
        self.flat_selectors = 'flat_selectors' in spec

    def describe(self):
        return {k: {kk: vv for kk, vv in v.items()} for k, v in self.spec.items()}

    def __bool__(self):
        return bool(self.spec)
