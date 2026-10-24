"""Optional per-function input domains: domains/UNIT_FUNCTION.json.

A domain pins globals, fields or arguments to the values the game actually produces, each with its evidence
(the STRICT setter or caller that establishes it).  The harness tests the function INSIDE the domain for its
verdict and additionally explores unconstrained inputs; a difference that only occurs outside the documented
domain is reported as OUT_OF_DOMAIN_DIFFERENCE with its counterexample, never hidden.

File format:
{
  "unit": "jumelle", "function": "RecaleRayPosInJumelle",
  "evidence": "free text: why this is the game's domain",
  "constraints": [ CONSTRAINT, ... ]
}
CONSTRAINT := {"loc": LOC, "values": [v, ...], "evidence": "..."}
            | {"loc": LOC, "range": [lo, hi], "evidence": "..."}          (inclusive)
            | {"loc": LOC, "fresh": {"size": n, "fields": {"0x10": {"fresh": {...}} | {"copy_global": NAME} |
                                                     {"values": [...], "size": s}}}, "evidence": "..."}
            | {"any_of": [[CONSTRAINT, ...], [CONSTRAINT, ...]], "evidence": "..."}  (one alternative per seed)
            | {"loc": LOC, "string": ["RAY1.WLD", ...], "evidence": "..."}   (pointer to a fresh NUL-terminated copy)
            | {"loc": LOC, "file_slice": {"file": "assets/...", "slices": [[off, len], ...]}, "evidence": "..."}
                  (pointer to a fresh copy of one slice of a shipped data file, read at run time)
            | {"loc": {"arg": k}, "from_arg": j, "add": n, "evidence": "..."}   (argument k = argument j + n)
FRESH fields may also hold {"string": [...]} and a fresh spec may carry
            "array": {"count": n, "stride": s, "offset": o, "fields": {"0x4": FIELD, ...}}   (or a list of arrays)
          (n elements of s bytes from offset o; FIELD := {"values"|"range": ..., "size": s} picked per element
           | {"index": {"div": d, "mod": m, "mul": a, "add": b}} = ((k // d) % m) * a + b for element k
           | {"fresh": {...}} a separate fresh allocation per element)
"environment": {...} configures the environment models of models.py (file layer, allocators, ...), not inputs.
LOC := {"global": NAME, "offset": n, "size": s}         memory at the bound oracle address of NAME (+offset)
     | {"arg": k}                                       the k-th entry argument (register or stack word)
     | {"arg": k, "offset": n, "size": s}               memory at (argument k) + n
     | {"global": NAME, "deref": true, "offset": n, "size": s}   memory at *(u32 *)NAME + n
"""
from __future__ import annotations

import json
import struct
from pathlib import Path

HERE = Path(__file__).resolve().parent
DOMAINS = HERE / 'domains'


def load(unit, name, f):
    path = DOMAINS / f'{unit}_{name}.json'
    if not path.exists():
        return None
    return Domain(json.loads(path.read_text(encoding='utf-8')), f, path)


def num(x):
    return int(x, 0) if isinstance(x, str) else int(x)


class Domain:
    def __init__(self, spec, f, path):
        self.spec, self.f, self.path = spec, f, path
        if spec.get('function') not in (None, f.name):
            raise ValueError(f'domain file {path.name} is for {spec.get("function")}')
        self.constraints = spec['constraints']

    def describe(self):
        return {'file': str(self.path.relative_to(HERE)), 'evidence': self.spec.get('evidence'),
                'constraints': len(self.constraints)}

    # ------------------------------------------------------------------ locations
    def global_addr(self, name):
        from difftest import flat
        a = self.f.p.binds.game.get(name)
        if a is None:
            raise ValueError(f'domain: unbound global {name}')
        return flat(a)

    def resolve(self, loc, state, read):
        """('mem', address, size) or ('arg', k)."""
        if 'arg' in loc and 'offset' not in loc:
            return ('arg', int(loc['arg']))
        size = num(loc.get('size', 4))
        off = num(loc.get('offset', 0))
        if 'arg' in loc:
            base = self.arg_value(state, int(loc['arg']))
        else:
            base = self.global_addr(loc['global'])
            if loc.get('deref'):
                base = read(base, 4)
        return ('mem', (base + off) & 0xffffffff, size)

    def arg_value(self, state, k):
        p = self.f.proto
        if k < len(p.parm):
            from difftest import PARENT
            return state.regs[PARENT.get(p.parm[k], p.parm[k])]
        return state.stack_args[k - len(p.parm)] if k - len(p.parm) < len(state.stack_args) else \
            state.read_initial(0x70040000 + 4 + 4 * (k - len(p.parm)), 4)

    def set_arg(self, state, k, v):
        p = self.f.proto
        from difftest import STACK_TOP
        if k < len(p.parm):
            state.regs[p.parm[k]] = v & 0xffffffff
        else:
            idx = k - len(p.parm)
            state.put(STACK_TOP + 4 + 4 * idx, v, 4)
            if idx < len(state.stack_args): state.stack_args[idx] = v

    # ------------------------------------------------------------------ apply
    def apply(self, state):
        for c in self.constraints:
            self.apply_one(c, state, salt=len(state.domain_applied))

    def pick(self, c, state, salt):
        from difftest import mix
        h = mix(state.seed, 0xd0d0 + salt, 41)
        if 'values' in c:
            vals = [num(v) for v in c['values']]
            return vals[h % len(vals)]
        lo, hi = num(c['range'][0]), num(c['range'][1])
        r = h % 8
        if r == 0: return lo
        if r == 1: return hi
        if r == 2: return min(hi, lo + 1)
        return lo + (h >> 3) % (hi - lo + 1)

    def apply_one(self, c, state, salt):
        if 'any_of' in c:
            from difftest import mix
            alts = c['any_of']
            alt = alts[mix(state.seed, 0xa17 + salt, 43) % len(alts)]
            for k, sub in enumerate(alt):
                self.apply_one(sub, state, salt * 31 + k + 1)
            return
        where = self.resolve(c['loc'], state, state.read_initial)
        if 'fresh' in c:
            v = self.fresh(c['fresh'], state)
        elif 'string' in c:
            v = self.fresh_string(c['string'], state, salt)
        elif 'file_slice' in c:
            v = self.fresh_file_slice(c['file_slice'], state, salt)
        elif 'from_arg' in c:
            v = (self.arg_value(state, int(c['from_arg'])) + num(c.get('add', 0))) & 0xffffffff
        else:
            v = self.pick(c, state, salt)
        if where[0] == 'arg':
            self.set_arg(state, where[1], v)
        else:
            state.put(where[1], v, where[2])
        state.domain_applied.append({'loc': c['loc'], 'value': v})

    def fresh_file_slice(self, spec, state, salt):
        """Fresh copy of one [offset, length] slice of a shipped data file (read-only, at run time)."""
        from difftest import mix, ROOT
        import models
        data = models.file_bytes(ROOT / spec['file'])
        slices = spec['slices']
        off, n = slices[mix(state.seed, 0xf11e + salt, 53) % len(slices)]
        off, n = num(off), num(n)
        if off < 0 or off + n > len(data):
            raise ValueError('domain: file slice outside ' + spec['file'])
        a = state.fresh(n)
        for k, byte in enumerate(data[off:off + n]):
            state.overrides[a + k] = byte
        state.domain_applied.append({'file_slice': spec['file'], 'offset': off, 'length': n})
        return a

    def fresh_string(self, choices, state, salt):
        from difftest import mix
        text = choices[mix(state.seed, 0x5751 + salt, 47) % len(choices)].encode('latin-1') + b'\0'
        a = state.fresh(len(text))
        for k, ch in enumerate(text):
            state.put(a + k, ch, 1)
        return a

    def fresh(self, spec, state):
        size = num(spec['size'])
        a = state.fresh(size)
        arrays = spec.get('array') or []
        for arr in ([arrays] if isinstance(arrays, dict) else arrays):
            n, stride, base = num(arr['count']), num(arr['stride']), num(arr.get('offset', 0))
            if base + n * stride > size:
                raise ValueError('domain: array exceeds the fresh allocation')
            for k in range(n):
                for off, sub in (arr.get('fields') or {}).items():
                    off = num(off)
                    at = a + base + k * stride + off
                    if 'fresh' in sub:
                        v = self.fresh(sub['fresh'], state)
                    elif 'index' in sub:
                        # value from the element index: ((k // div) % mod) * mul + add
                        ix = sub['index']
                        q = k // num(ix.get('div', 1))
                        if 'mod' in ix: q %= num(ix['mod'])
                        v = q * num(ix.get('mul', 1)) + num(ix.get('add', 0))
                    else:
                        v = self.pick(sub, state, at)
                    state.put(at, v & 0xffffffff, num(sub.get('size', 4)))
        for off, sub in (spec.get('fields') or {}).items():
            off = num(off)
            if 'fresh' in sub:
                state.put(a + off, self.fresh(sub['fresh'], state), 4)
            elif 'string' in sub:
                state.put(a + off, self.fresh_string(sub['string'], state, off), 4)
            elif 'copy_global' in sub:
                g = self.global_addr(sub['copy_global'])
                state.put(a + off, (state.read_initial(g, 4) + num(sub.get('add', 0))) & 0xffffffff, 4)
            elif 'values' in sub or 'range' in sub:
                state.put(a + off, self.pick(sub, state, off), num(sub.get('size', 4)))
        return a

    # ------------------------------------------------------------------ membership
    def contains(self, state):
        return not self.violations(state)

    def violations(self, state):
        out = []
        for c in self.constraints:
            v = self.check(c, state)
            if v: out.append(v)
        return out

    def check(self, c, state):
        if 'any_of' in c:
            subs = [[self.check(s, state) for s in alt] for alt in c['any_of']]
            if any(not any(r) for r in subs):
                return None
            return {'any_of_failed': [[r for r in alt if r] for alt in subs]}
        where = self.resolve(c['loc'], state, state.read_initial)
        if where[0] == 'arg':
            v = self.arg_value(state, where[1])
            size = 4
        else:
            v = state.read_initial(where[1], where[2]); size = where[2]
        if 'fresh' in c:
            ok = self.fresh_ok(v, c['fresh'], state)
            return None if ok else {'loc': c['loc'], 'value': v, 'expected': 'fresh disjoint allocation'}
        if 'from_arg' in c:
            want = (self.arg_value(state, int(c['from_arg'])) + num(c.get('add', 0))) & 0xffffffff
            return None if v == want else {'loc': c['loc'], 'value': v, 'expected': f'argument {c["from_arg"]} + {c.get("add", 0)}'}
        if 'file_slice' in c:
            from difftest import FRESH, HEAP
            return None if FRESH <= v < HEAP + 0x1000000 else {'loc': c['loc'], 'value': v, 'expected': 'fresh file slice'}
        if 'string' in c:
            from difftest import FRESH, HEAP
            text = None
            if FRESH <= v < HEAP + 0x1000000:
                text = bytes(state.initial_byte((v + k) & 0xffffffff) for k in range(64)).split(b'\0', 1)[0]
            ok = text is not None and text.decode('latin-1') in c['string']
            return None if ok else {'loc': c['loc'], 'value': v, 'expected': 'fresh string from the list'}
        if 'values' in c:
            allowed = {num(x) & ((1 << (8 * size)) - 1) for x in c['values']}
            return None if v in allowed else {'loc': c['loc'], 'value': v, 'expected': c['values']}
        lo, hi = num(c['range'][0]), num(c['range'][1])
        sv = v - (1 << (8 * size)) if lo < 0 and v >> (8 * size - 1) else v
        return None if lo <= sv <= hi else {'loc': c['loc'], 'value': sv, 'expected': c['range']}

    def fresh_ok(self, v, spec, state):
        from difftest import FRESH, HEAP
        if not FRESH <= v < HEAP + 0x1000000:
            return False
        for off, sub in (spec.get('fields') or {}).items():
            if 'fresh' in sub and not self.fresh_ok(state.read_initial(v + num(off), 4), sub['fresh'], state):
                return False
        return True
