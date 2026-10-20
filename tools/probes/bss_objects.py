# Probe (docs/DATA.md): BSS object boundaries from size-rank resets. Within one object's _BSS, Watcom orders globals
# by descending size rank (tools/bssrule.py); a rank increase between address-adjacent globals starts a new object.
import re, sys
sys.path.insert(0, 'tools')
import android
from bssrule import rank
A = android.load()
BSS0 = 0x3218a
objs = {}
for ln in open('external/rayverse/src/data.c', encoding='latin-1'):
    m = re.match(r'\s*[\w ]+?\s+\*?(\w+)\s*(\[[^\]]*\])*\s*;.*//\s*(0x)?([0-9A-Fa-f]{5,6})\b', ln)
    if m:
        a = int(m.group(4), 16) - 0x90000
        n = m.group(1)
        if a >= BSS0 and n in A and A[n]['kind'] == 'O':
            objs[a] = (n, A[n]['size'])
rows = sorted((a, n, s) for a, (n, s) in objs.items())
print('named BSS globals:', len(rows), 'range 2:%05x..2:%05x' % (rows[0][0], rows[-1][0]))
groups = [[rows[0]]]
for prev, cur in zip(rows, rows[1:]):
    if rank(cur[2]) > rank(prev[2]):
        groups.append([cur])
    else:
        groups[-1].append(cur)
print('candidate BSS objects (rank resets):', len(groups))
for g in groups:
    print('  2:%05x..2:%05x  %4d globals  first %s' % (g[0][0], g[-1][0] + g[-1][2], len(g), g[0][1]))
