"""Explain NEAR differences with captured mechanisms, never a matching verdict.

python tools/explain.py UNIT FUNC [--source COPY.c] [--out NEW_DIR]

Machine alignment and IR association are diagnostic hypotheses. Decisions and
phase changes are observations. Missing/ambiguous IR links are reported openly.
No original DOS IR, costs, home order or rover stream is assumed to be known.
"""
import argparse
import difflib
import json
from pathlib import Path
import struct
import sys

sys.dont_write_bytecode = True
sys.path.insert(0, str(Path(__file__).resolve().parent))
import cgtrace
sys.path.insert(0, str(cgtrace.ROOT / 'tools'))
import capstone
import census
import match
import omf
from image import Image

REG = {0x1000003: 'eax', 0x80000c0: 'edx', 0x200000c: 'ebx', 0x4000030: 'ecx',
       0x10000100: 'esi', 0x20000200: 'edi', 0x400: 'ebp', 3: 'ax', 0xc0: 'dx',
       0xc: 'bx', 0x30: 'cx', 0x100: 'si', 0x200: 'di', 2: 'al', 1: 'ah', 0x80: 'dl',
       0x40: 'dh', 8: 'bl', 4: 'bh', 0x20: 'cl', 0x10: 'ch'}
OP = ('NOP ADD EXT_ADD SUB EXT_SUB MUL EXT_MUL DIV MOD AND OR XOR RSHIFT LSHIFT POW '
      'ATAN2 FMOD NEGATE COMPLEMENT LOG COS SIN TAN SQRT FABS ACOS ASIN ATAN COSH EXP '
      'LOG10 SINH TANH PTR_TO_NATIVE PTR_TO_FOREIGN SLACK CONVERT ROUND MOV CAREFUL_LA LA '
      'CALL_INDIRECT PUSH POP PARM_DEF SELECT BIT_TRUE BIT_FALSE CMP_EQUAL CMP_NOT_EQUAL '
      'CMP_GREATER CMP_LESS_EQUAL CMP_LESS CMP_GREATER_EQUAL CALL SET_EQUAL SET_NOT_EQUAL '
      'SET_GREATER SET_LESS_EQUAL SET_LESS SET_GREATER_EQUAL DEBUG CHEAP_NOP').split()
FAMILY = {'mov': {38}, 'movsx': {36}, 'movzx': {36}, 'cwde': {36},
          'lea': {1, 3, 5, 13, 39, 40}, 'shl': {5, 13}, 'sal': {5, 13},
          'shr': {12}, 'sar': {12, 36}, 'add': {1}, 'inc': {1}, 'sub': {3},
          'dec': {3}, 'imul': {5}, 'and': {9}, 'or': {10}, 'xor': {11, 38},
          'neg': {17}, 'not': {18}, 'test': set(range(46, 54)),
          'cmp': set(range(46, 54)), 'call': {41, 54}, 'push': {42}, 'pop': {43}}


def operand(n):
    if not n:
        return '-'
    kind = n.get('cls')
    if kind == 0:
        return '#' + hex(n.get('value', 0))
    if kind == 2:
        return f't{n["id"]}:{n["size"]}@{n["offset"]}'
    if kind == 3:
        return REG.get(n['reg'], hex(n['reg']))
    if kind == 1:
        return f'm{n["symbol"]:x}{n["offset"]:+d}:{n["size"]}'
    if kind == 4:
        return f'[{operand(n.get("base"))}+{operand(n.get("index"))}*{1 << n["scale"]}{n["displacement"]:+d}]'
    return '?'


def ir(ins):
    if not ins:
        return 'IR unavailable'
    op = OP[ins['op']] if ins['op'] < len(OP) else hex(ins['op'])
    return f'IR@{ins["addr"]:x} {op} ' + ', '.join(map(operand, ins['operands'])) + ' -> ' + operand(ins['result'])


def walk(n):
    if not n:
        return
    yield n
    if n.get('cls') == 4:
        yield from walk(n.get('base'))
        yield from walk(n.get('index'))


def ir_regs(ins):
    regs = set()
    for op in ins['operands'] + [ins['result']]:
        for n in walk(op):
            if n.get('cls') == 3:
                value = n['reg']
                if value in REG:
                    regs.add(REG[value])
                else:  # indexed operands can carry a two-register hardware set
                    regs.update(REG[r] for r in REG if r > 0xffffff and value & r == r)
    return regs


def machine_regs(ins):
    regs = []
    for op in ins.operands:
        if op.type == capstone.CS_OP_REG:
            regs.append(ins.reg_name(op.reg))
        elif op.type == capstone.CS_OP_MEM:
            regs.extend(ins.reg_name(r) for r in (op.mem.base, op.mem.index) if r)
    return regs


def key(ins):
    """Alignment ignores registers/stack offsets/branch distances, retains shape.

    A register-zero predicate has the same key for TEST r,r and CMP r,0.
    This is an alignment aid, not instruction or semantic equality.
    """
    ops = ins.operands
    if (ins.mnemonic == 'test' and len(ops) == 2 and ops[0].type == ops[1].type == capstone.CS_OP_REG
            and ops[0].reg == ops[1].reg):
        return ('zero', ops[0].size)
    if (ins.mnemonic == 'cmp' and len(ops) == 2 and ops[0].type == capstone.CS_OP_REG
            and ops[1].type == capstone.CS_OP_IMM and ops[1].imm == 0):
        return ('zero', ops[0].size)
    if ins.group(capstone.CS_GRP_JUMP) or ins.group(capstone.CS_GRP_CALL):
        return (ins.mnemonic,)
    return (ins.mnemonic, tuple((o.type, o.size,
            (o.imm if o.type == capstone.CS_OP_IMM else
             ('stack' if o.mem.base == capstone.x86.X86_REG_ESP else 'mem') if o.type == capstone.CS_OP_MEM else 'reg'))
            for o in ops))


def align(candidate, original):
    """Return all machine instruction pairs, including additions/deletions."""
    result = []
    matcher = difflib.SequenceMatcher(None, list(map(key, candidate)), list(map(key, original)), autojunk=False)
    for tag, a, b, c, d in matcher.get_opcodes():
        if tag == 'equal':
            result.extend((x, y, 'shape') for x, y in zip(candidate[a:b], original[c:d]))
        else:
            for index in range(max(b - a, d - c)):
                result.append((candidate[a + index] if a + index < b else None,
                               original[c + index] if c + index < d else None, 'gap'))
    return result


def associate(ins, graph, trace=None):
    """Conservative operation/register association; expose all best ties.

    No emitter hook has been recovered. This cannot prove an IR-to-byte link.
    Never silently choose the first among indistinguishable instructions.
    """
    if ins is None or ins.mnemonic not in FAMILY:
        return []
    def family(reg):
        for group in (('eax','ax','al','ah'),('ebx','bx','bl','bh'),('ecx','cx','cl','ch'),
                      ('edx','dx','dl','dh'),('esi','si'),('edi','di'),('ebp','bp'),('esp','sp')):
            if reg in group:
                return group[0]
        return reg
    regs = {family(r) for r in machine_regs(ins)}
    constants = {o.imm & 0xffffffff for o in ins.operands if o.type == capstone.CS_OP_IMM}
    choices = []
    for node in graph:
        if node['op'] not in FAMILY[ins.mnemonic]:
            continue
        nr = {family(r) for r in ir_regs(node)}
        nc = {n.get('value') for op in node['operands'] for n in walk(op) if n.get('cls') == 0}
        score = 4 * len(regs & nr) - 3 * len(regs - nr) + 2 * len(constants & nc)
        # Matching the selected destination and exact non-stack displacement
        # distinguishes adjacent MOVs that merely mention the same register.
        machine_mem = [o for o in ins.operands if o.type == capstone.CS_OP_MEM]
        node_mem = [n for op in node['operands'] + [node['result']] for n in walk(op) if n.get('cls') == 4]
        if machine_mem and node_mem:
            if any(o.mem.disp == n['displacement'] for o in machine_mem for n in node_mem):
                score += 6
            else:
                score -= 6
        if ins.operands and ins.operands[0].type == capstone.CS_OP_REG:
            dst = operand(node['result'])
            if family(dst) == family(ins.reg_name(ins.operands[0].reg)):
                score += 3
        if constants and nc and constants != nc:
            score -= 4
        if trace and any(o.type == capstone.CS_OP_MEM and ins.reg_name(o.mem.base) == 'esp' for o in ins.operands):
            homes = {n['id']: n['location'] for e in trace['events'] if e['event']=='homes_return'
                     for n in e['names'] if n.get('cls')==2 and n['flags'] & 1}
            signed = lambda n: n if n < 0x80000000 else n - 0x100000000
            offsets = {trace.get('_frame_size', 0) + signed(homes[n['id']]) + signed(n['offset'])
                       for op in node['operands'] + [node['result']] for n in walk(op)
                       if n.get('cls') == 2 and n['id'] in homes}
            actual = {o.mem.disp for o in ins.operands if o.type == capstone.CS_OP_MEM and ins.reg_name(o.mem.base)=='esp'}
            score += 12 if offsets & actual else -12
        if regs and not regs & nr:
            continue
        choices.append((score, node))
    if not choices:
        return []
    best = max(s for s, n in choices)
    return [n for s, n in choices if s == best]


def replay_rover(row, index):
    choices = row['choices']
    for step in range(1, len(choices) + 1):
        reg = choices[(index + step) % len(choices)]
        if not reg & row['except_regs']:
            return reg
    return 0


def rover_constraints(rows, constraints):
    """One extra/missing cursor step before a request, frozen eligibility only.

    Constraints map captured request indices to desired hardware masks. Return
    every fitting onset/delta, so sparse evidence never becomes a unique cause.
    """
    if not constraints or not rows:
        return []
    fits = []
    for onset in range(max(constraints) + 1):
        for delta in (-1, 1):
            cursor = rows[0]['initial_index']
            predicted = []
            for k, row in enumerate(rows):
                if k == onset:
                    cursor = (cursor + delta) % len(row['choices'])
                picked = replay_rover(row, cursor)
                predicted.append(picked)
                if picked:
                    cursor = row['choices'].index(picked)
            if all(predicted[k] == target for k, target in constraints.items()):
                fits.append(dict(request_index=onset, delta=delta))
    return fits


def compare_rovers(before, after):
    """Compare captured request streams across versions, ignoring process VAs.

    Backend temp IDs are function-local and can change; global frontend symbol
    handles are retained only as an explicit same-declarations assumption.
    """
    def signature(e):
        def sig(n):
            if not n:
                return None
            kind = n.get('cls')
            base = (kind, n.get('size'), n.get('type'))
            if kind == 0:
                return base + (n.get('value'), n.get('value_hi'))
            if kind == 1:
                return base + (n.get('symbol'), n.get('offset'))
            if kind == 3:
                return base + (n.get('reg'),)
            if kind == 4:
                return base + (sig(n.get('base')), sig(n.get('index')), n['scale'], n['displacement'])
            return base
        i = e['instruction']
        return (e['kind'], i['op'], tuple(map(sig, i['operands'])), sig(i['result']))
    result = []
    for width in sorted({r['kind']//2 for r in before + after}):
        a, b = ([r for r in rows if r['kind']//2 == width] for rows in (before, after))
        matcher = difflib.SequenceMatcher(None, list(map(signature, a)), list(map(signature, b)), autojunk=False)
        for tag, x, y, z, w in matcher.get_opcodes():
            if tag != 'equal':
                result.append(dict(width=width, change=tag, before=[x,y], after=[z,w],
                                   ir_before=[ir(r['instruction']) for r in a[x:y]],
                                   ir_after=[ir(r['instruction']) for r in b[z:w]]))
            elif z != x:
                result.append(dict(width=width, change='request-index offset', starts_at_request=x,
                                   delta=z-x, matched_requests=y-x))
    return result


def describe_difference(cand, dos, trace):
    """Mechanism evidence for one aligned difference; confidence is explicit."""
    links = associate(cand, trace['phases']['after_LdStCompress'], trace)
    addresses = {n['addr'] for n in links}
    findings = []
    changed_regs = {(a, b) for a, b in zip(machine_regs(cand), machine_regs(dos)) if a != b} if cand and dos else set()
    for e in trace['score']:
        if e.get('success') and e['instruction']['addr'] in addresses:
            before, after = e['instruction'], e['after']
            changed = {(operand(b), operand(a)) for b, a in zip(before['operands'], after['operands'])
                       if operand(b) != operand(a)}
            if any((after_reg, before_reg) in changed_regs for before_reg, after_reg in changed):
                findings.append(dict(mechanism='Score substitution', confidence='PROVEN decision; HYPOTHESIS machine link',
                                     detail=ir(before) + ' => ' + ir(after), ir=before))
    if cand and dos:
        if {cand.mnemonic, dos.mnemonic} in ({'test', 'cmp'}, {'lea', 'shl'}, {'lea', 'sal'}):
            early = [n for n in trace['phases']['post_start'] if n['addr'] in addresses]
            selected = ', '.join(f'IR@{n["addr"]:x} table={n.get("gen_table")}' for n in early)
            findings.append(dict(mechanism='selection', confidence='PROVEN machine difference; HYPOTHESIS IR link',
                                 detail=f'{cand.mnemonic.upper()}/{dos.mnemonic.upper()}; before Score: {selected}', ir=links))
        cr, dr = machine_regs(cand), machine_regs(dos)
        for source, target in sorted(changed_regs):
            for e in trace['rover']:
                if e.get('instruction', {}).get('addr') in addresses and REG.get(e['chosen']) == source:
                    findings.append(dict(mechanism='byte/word rover', confidence='PROVEN request; HYPOTHESIS machine link',
                                         detail=f'request {e["request_index"]} kind {e["kind"]}: {source}->{target}; '
                                                f'cursor {e["initial_index"]}, exclusions {e["except_regs"]:#x}',
                                         request_index=e['request_index'], desired=next((k for k, v in REG.items() if v == target), None),
                                         kind=e['kind'], ir=e['instruction']))
            # Trace temp identity before allocation, at the same instruction address.
            earlier = [n for n in trace['phases']['regalloc'] if n['addr'] in addresses]
            tids = {n.get('id') for i in earlier for op in i['operands'] + [i['result']]
                    for n in walk(op) if n.get('cls') == 2}
            for e in trace['colours']:
                if (e.get('name') or {}).get('id') not in tids or REG.get(e.get('chosen')) != source:
                    continue
                savings = (e.get('name') or {}).get('conflict_savings')
                peers = [c for sort in trace['conflict_sorts'] for c in sort['output']
                         if c['savings'] == savings and (c.get('name') or {}).get('id') not in tids]
                scores = {REG.get(s['reg'], hex(s['reg'])): s['score'] for s in e['scores']}
                findings.append(dict(mechanism='colour tie' if peers else 'colour decision',
                                     confidence='PROVEN costs/choice; HYPOTHESIS machine link',
                                     detail=f't{e["name"]["id"]} savings={savings}; affinity={scores}; '
                                            f'given={e["given"]:#x}; picked={source}, DOS={target}; '
                                            f'equal-savings peers={sorted({c["name"]["id"] for c in peers})}', ir=earlier))
        def stack(i):
            return [o.mem.disp for o in i.operands if o.type == capstone.CS_OP_MEM
                    and i.reg_name(o.mem.base) == 'esp']
        if stack(cand) != stack(dos) and stack(cand) and stack(dos):
            homes = next((e['names'] for e in trace['events'] if e['event']=='homes_return'), [])
            order = [(n['id'], n['size'], struct.unpack('<i', struct.pack('<I', n['location']))[0])
                     for n in homes if n['cls']==2 and n['flags'] & 1 and n['location'] != 0x7fffffff]
            findings.append(dict(mechanism='home-sort order', confidence='PROVEN stack difference/order; original order OPEN',
                                 detail=f'ESP displacements {stack(cand)} -> {stack(dos)}; '
                                        f'assigned homes (id,size,frame-location)={order}', ir=links))
    if not findings:
        findings.append(dict(mechanism='size/structure', confidence='OPEN cause',
                             detail='instruction, operand, schedule or branch/layout difference', ir=links))
    return findings


def relocated_code(mod, row, binds):
    """Diagnostic linking from candidate bindings only, never DOS field copying."""
    seg = next(s for s in mod.segments if s and s.name == row['segment'])
    si, off = mod.segments.index(seg), row['seg_offset']
    obj, base = (int(x, 16) for x in row['oracle'].split(':'))
    code = bytearray(seg.data[off:off + row['size']])
    for f in mod.fixups:
        if f.seg != si or not off <= f.off < off + len(code) or f.size() != 4:
            continue
        target, _ = binds.lookup(mod.target_name(f))
        if target is None and f.target[0] & 3 == 0 and f.target[1] == si:
            target = (obj, base - off)
        if target is None:
            continue
        site = f.off - off
        field = struct.unpack_from('<i', code, site)[0]
        value = target[1] + f.disp + field
        if f.self_rel:
            value -= base + site + 4
        struct.pack_into('<I', code, site, value & 0xffffffff)
    return bytes(code)


def explain_object(obj, func, trace, oracle=None, symbols=None):
    image = Image(oracle)
    old_extra = match.Bindings.extra
    match.Bindings.extra = symbols
    try:
        modules = omf.load(obj)
        rows = [(m, r) for m in modules for r in match.verify_object(image, m)]
        mod, row = next((m, r) for m, r in rows if r['function'] == func)
        if 'oracle' not in row:
            raise ValueError(f'{func} has no DOS binding')
        binds = match.Bindings(image)
        number, start = (int(x, 16) for x in row['oracle'].split(':'))
        ends = [binds.lookup(n)[0][1] for n, si, off, typ in mod.publics
                if si == next(j for j, s in enumerate(mod.segments) if s and s.name == row['segment'])
                and binds.lookup(n)[0] and binds.lookup(n)[0][0] == number and binds.lookup(n)[0][1] > start]
        end = min(ends) if ends else start + row['size']
        ci = census.insns(relocated_code(mod, row, binds))
        oi = census.insns(image.obj[number][start:end])
        trace['_frame_size'] = next((i.operands[1].imm for i in ci[:16] if i.mnemonic=='sub'
                                     and i.op_str.startswith('esp,') and i.operands[1].type==capstone.CS_OP_IMM), 0)
        differences, all_pairs = [], align(ci, oi)
        for c, o, confidence in all_pairs:
            if c and o and c.mnemonic == o.mnemonic and c.op_str == o.op_str and c.size == o.size:
                continue
            differences.append(dict(candidate_offset=c.address if c else None,
                                    dos_address=start + o.address if o else None,
                                    candidate=f'{c.mnemonic} {c.op_str}' if c else '-',
                                    dos=f'{o.mnemonic} {o.op_str}' if o else '-', alignment=confidence,
                                    findings=describe_difference(c, o, trace)))
        constraints = {}
        for diff in differences:
            for f in diff['findings']:
                if f['mechanism'] == 'byte/word rover' and f['desired']:
                    width = f['kind'] // 2
                    constraints.setdefault(width, {}).setdefault(f['request_index'], set()).add(f['desired'])
        offsets = []
        for width, targets in constraints.items():
            unique = {k: next(iter(v)) for k, v in targets.items() if len(v) == 1}
            requests = [e for e in trace['rover'] if e['kind'] // 2 == width]
            offsets.append(dict(width=width, constraints=unique,
                                fits=rover_constraints(requests, unique),
                                confidence='HYPOTHESIS: frozen eligibility + inferred machine/IR links'))
        return dict(function=func, verifier=row, differences=differences, rover_offsets=offsets,
                    extent=dict(candidate=row['size'], dos=end-start, dos_end_known=bool(ends)),
                    limitations='Alignment and IR associations are diagnostic; ambiguous links retained. '
                    'ESP-home association assumes a fixed local frame; outgoing stack arguments and EBP frames are not modeled. '
                    'RET-frontier decoder excludes trailing tables; verifier checks full extent/references. '
                    'Final public DOS end uses candidate length if no following bound public.')
    finally:
        match.Bindings.extra = old_extra


def render(report):
    row = report['verifier']
    lines = [f'{report["function"]}: {row["status"]}; {row["size"]} candidate bytes; '
             f'{row.get("byte_diffs", "?")} byte differences (unchanged verifier)',
             'Machine/IR associations are hypotheses; IR addresses identify this run only.']
    for offset in report['rover_offsets']:
        lines.append(f'rover width-class {offset["width"]}: frozen onset fits {offset["fits"]}')
    for change in report.get('rover_comparison', []):
        lines.append('captured rover stream: ' + str(change))
    for d in report['differences']:
        pos = f'+{d["candidate_offset"]:x}' if d['candidate_offset'] is not None else '-'
        dos = f'1:{d["dos_address"]:05x}' if d['dos_address'] is not None else '-'
        lines.append(f'{pos:>6} | {dos} | {d["candidate"]} => {d["dos"]}')
        for f in d['findings']:
            nodes = f['ir'] if isinstance(f['ir'], list) else [f['ir']]
            ids = '; '.join(ir(i) for i in nodes[:3]) or 'IR unavailable'
            if len(nodes) > 3:
                ids += f'; +{len(nodes)-3} ambiguous candidates (JSON)'
            detail = f['detail']
            if len(detail) > 320:
                detail = detail[:317] + '... (JSON)'
            lines.append(f'  {f["mechanism"]}: {detail}; {ids}')
    for problem in row['problems'] + row['unverified']:
        lines.append('reference/extent: ' + problem)
    if report['extent']['candidate'] != report['extent']['dos']:
        lines.append(f'size/structure: extent {report["extent"]}')
    return '\n'.join(lines) + '\n'


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('unit')
    parser.add_argument('func')
    parser.add_argument('--source', type=Path)
    parser.add_argument('--out', type=Path)
    parser.add_argument('--symbols', type=Path)
    parser.add_argument('--compare-trace', type=Path, help='optional other-version function JSON (same declarations)')
    args = parser.parse_args(argv)
    units = json.loads((cgtrace.ROOT / 'config/units.json').read_text())['units']
    if args.unit not in units:
        parser.error('unknown unit: ' + args.unit)
    unit = units[args.unit]
    source = args.source or cgtrace.ROOT / unit['source']
    out = args.out or cgtrace.ROOT / 'build/cgtrace' / (args.unit + '_' + args.func)
    try:
        cgtrace.trace(source, out, unit['profile'], args.func)
        trace = json.loads((out / (args.func + '.json')).read_text())
        result = explain_object(out / 'observed.obj', args.func, trace, unit.get('oracle'), args.symbols)
        if args.compare_trace:
            other = json.loads(args.compare_trace.read_text())
            if other['function'] != args.func:
                raise ValueError('comparison trace function differs')
            result['rover_comparison'] = compare_rovers(trace['rover'], other['rover'])
        (out / 'explanation.json').write_text(json.dumps(result, indent=2))
        text = render(result)
        (out / 'explanation.txt').write_text(text)
        print(text, end='')
    except (OSError, RuntimeError, ValueError, StopIteration) as exc:
        parser.exit(1, f'explain: {exc}\n')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
