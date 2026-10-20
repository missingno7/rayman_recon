"""Watcom wcc386 _BSS layout rule (PROVEN for 10.5/10.6 here: tools/probes/bss_rule.py; docs/COMPILER.md).

Provenance: functions pjw/key/rank/group/sortkey/layout copied from kegg_recon tools/bssorder.py (same author),
where the rule was first proven for Watcom 10.0.
"""

HASH_SIZE = 241
FIRST = 5
PER = 25


def pjw(name: str) -> int:
    """cfe identifier hash (before % 241)."""
    b = name.encode("latin1")
    if not b:
        return 0
    h = b[0]
    if len(b) < 2:
        return h
    h = (h << 4) + b[1]
    i = 2
    while True:
        h &= 0xFFF
        if i >= len(b):
            break
        h = (h << 4) + b[i]
        i += 1
        h = (h ^ (h >> 12)) & 0xFFF
        if i >= len(b):
            break
        h = (h << 4) + b[i]
        i += 1
        h = h ^ (h >> 12)
    return h


def key(name: str) -> int:
    return pjw(name) % HASH_SIZE


def rank(size: int) -> int:
    if size % 8 == 0:
        return 5
    if size % 4 == 0:
        return 4
    if size % 2 == 0:
        return 3 if size > 2 else 2
    return 1 if size > 1 else 0


def group(idx: int) -> int:
    return 0 if idx < FIRST else 1 + (idx - FIRST) // PER


def sortkey(name, size, idx):
    return (-rank(size), -group(idx), key(name), -idx)


def layout(objs):
    """objs: [(name, size, idx)] -> [(off, name, size, idx)] in address order."""
    out, off = [], 0
    for n, s, i in sorted(objs, key=lambda o: sortkey(*o)):
        out.append((off, n, s, i))
        off += s
    return out



