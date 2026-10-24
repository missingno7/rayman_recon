"""Explain NEAR differences with captured mechanisms, never a matching verdict.

python tools/explain.py UNIT FUNC [--source COPY.c] [--out NEW_DIR]

Machine alignment and IR association are diagnostic hypotheses. Decisions and
phase changes are observations. Missing/ambiguous IR links are reported openly.
No original DOS IR, costs, home order or rover stream is assumed to be known.
"""
import argparse
import collections
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
import scheduler
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


def instruction_token(ins):
    return ins.mnemonic + ' ' + ins.op_str


def reorder_windows(candidate, original):
    """Return exact equal-multiset reorder windows, as machine instruction slices."""
    a, b = list(map(instruction_token, candidate)), list(map(instruction_token, original))
    out, opcodes = [], difflib.SequenceMatcher(a=a, b=b, autojunk=False).get_opcodes()
    used = -1
    for x, (tag, ia, ib, ja, jb) in enumerate(opcodes):
        if tag == 'equal' or x <= used:
            continue
        for y in range(x, len(opcodes)):
            _, _, end_a, _, end_b = opcodes[y]
            count_a, count_b = end_a - ia, end_b - ja
            if max(count_a, count_b) > 32:
                break
            if (count_a > 1 and count_a == count_b and a[ia:end_a] != b[ja:end_b]
                    and collections.Counter(a[ia:end_a]) == collections.Counter(b[ja:end_b])):
                out.append(dict(candidate_start=ia, dos_start=ja,
                                candidate=candidate[ia:end_a], dos=original[ja:end_b],
                                candidate_tokens=a[ia:end_a], dos_tokens=b[ja:end_b]))
                used = y
                break
    return out


_INLINE_CALL_INSNS = {'cli', 'sti', 'int', 'int3', 'in', 'out', 'insb', 'insw', 'insd',
                      'outsb', 'outsw', 'outsd', 'pushfd', 'popfd', 'iretd'}
_SETCC = {'sete': 48, 'setz': 48, 'setne': 49, 'setnz': 49, 'setg': 50,
          'setnle': 50, 'setle': 51, 'setng': 51, 'setl': 52, 'setnge': 52,
          'setge': 53, 'setnl': 53}


def _reg_family(reg):
    for group in (('eax', 'ax', 'al', 'ah'), ('ebx', 'bx', 'bl', 'bh'),
                  ('ecx', 'cx', 'cl', 'ch'), ('edx', 'dx', 'dl', 'dh'),
                  ('esi', 'si'), ('edi', 'di'), ('ebp', 'bp'), ('esp', 'sp')):
        if reg in group:
            return group[0]
    return reg


def _schedule_associations(ins, nodes, trace):
    if ins.mnemonic in _INLINE_CALL_INSNS:
        return [n for n in nodes if n['op'] in (41, 54)]
    if ins.mnemonic in _SETCC:
        return [n for n in nodes if n['op'] == _SETCC[ins.mnemonic]]
    if ins.mnemonic not in FAMILY:
        return []
    machine_names = set(machine_regs(ins))
    machine_families = {_reg_family(reg) for reg in machine_names}
    constants = {op.imm & 0xffffffff for op in ins.operands if op.type == capstone.CS_OP_IMM}
    choices = []
    homes = {n['id']: n['location'] for event in trace.get('events', [])
             if event.get('event') == 'homes_return' for n in event['names']
             if n.get('cls') == 2 and n.get('flags', 0) & 1}
    signed = lambda value: value if value < 0x80000000 else value - 0x100000000
    frame_size = trace.get('_frame_size', 0)
    actual_stack = {op.mem.disp for op in ins.operands if op.type == capstone.CS_OP_MEM
                    and ins.reg_name(op.mem.base) == 'esp'}
    for node in nodes:
        if node['op'] not in FAMILY[ins.mnemonic]:
            continue
        ir_names = ir_regs(node)
        ir_families = {_reg_family(reg) for reg in ir_names}
        node_constants = {part.get('value') for op in node['operands']
                          for part in walk(op) if part.get('cls') == 0}
        score = 4 * len(machine_families & ir_families) - 3 * len(machine_families - ir_families)
        score += 12 * len(constants & node_constants)
        score += 4 * len(machine_names & ir_names)
        machine_mem = [op for op in ins.operands if op.type == capstone.CS_OP_MEM]
        node_mem = [part for op in node['operands'] + [node.get('result')]
                    for part in walk(op) if part.get('cls') == 4]
        if machine_mem and node_mem:
            if any(op.mem.disp == part['displacement'] for op in machine_mem for part in node_mem):
                score += 6
            else:
                score -= 6
        if ins.operands and ins.operands[0].type == capstone.CS_OP_REG:
            dst = operand(node.get('result'))
            machine_dst = ins.reg_name(ins.operands[0].reg)
            if _reg_family(dst) == _reg_family(machine_dst):
                score += 3
                if dst == machine_dst:
                    score += 4
        widths = [op.size for op in ins.operands if op.type in (capstone.CS_OP_REG, capstone.CS_OP_MEM)]
        node_widths = [part.get('size') for op in node['operands'] + [node.get('result')]
                       for part in walk(op) if part.get('cls') in (1, 2, 3, 4) and part.get('size')]
        if widths and node_widths:
            score += 2 if widths[0] in node_widths else -2
        if constants and constants != node_constants:
            score -= 4
        if machine_families and not machine_families & ir_families:
            continue
        if actual_stack:
            offsets = {frame_size + signed(homes[part['id']]) + signed(part['offset'])
                       for op in node['operands'] + [node.get('result')]
                       for part in walk(op) if part.get('cls') == 2 and part.get('id') in homes}
            score += 12 if offsets & actual_stack else -12
        choices.append((score, node))
    if not choices:
        return []
    best = max(score for score, _ in choices)
    return [node for score, node in choices if score == best]


def _window_assignments(window, block, trace, limit=256):
    nodes = scheduler.block_nodes(block)
    options = [_schedule_associations(ins, nodes, trace) for ins in window['candidate']]
    for index, ins in enumerate(window['candidate']):
        if ins.mnemonic.startswith('set') and index and window['candidate'][index - 1].mnemonic == 'cmp':
            compare_ids = {node['addr'] for node in options[index - 1]}
            options[index] = [node for node in options[index] if node['addr'] in compare_ids]
    if any(not choices for choices in options):
        return []
    # Few repeated machine instructions are ambiguous; enumerate bounded
    # one-to-one assignments and retain every top-scoring association.
    order = sorted(range(len(options)), key=lambda i: len(options[i]))
    found, chosen, used = [], {}, set()

    def visit(depth):
        if len(found) >= limit:
            return
        if depth == len(order):
            found.append([chosen[i] for i in range(len(options))])
            return
        index = order[depth]
        for node in options[index]:
            addr = node['addr']
            duplicate_pair = (index > 0 and window['candidate'][index].mnemonic.startswith('set')
                              and chosen.get(index - 1, {}).get('addr') == addr)
            already_used = addr in used
            if already_used and not duplicate_pair:
                continue
            if not already_used:
                used.add(addr)
            chosen[index] = node
            visit(depth + 1)
            if not already_used:
                used.remove(addr)
            del chosen[index]

    visit(0)
    model = scheduler.Model(nodes).run()
    forward_positions = {addr: pos for pos, addr in enumerate(model['forward'])}
    def collapsed(rows):
        return [node['addr'] for index, node in enumerate(rows)
                if not index or node['addr'] != rows[index - 1]['addr']]
    return [rows for rows in found
            if all(forward_positions[a] < forward_positions[b]
                   for a, b in zip(collapsed(rows), collapsed(rows)[1:]))]


def _dos_assignments(window, candidate_ids, limit=256):
    by_token = collections.defaultdict(list)
    for index, token in enumerate(window['candidate_tokens']):
        by_token[token].append(index)
    result, used, row = [], set(), []

    def visit(index):
        if len(result) >= limit:
            return
        if index == len(window['dos_tokens']):
            result.append(list(row))
            return
        token = window['dos_tokens'][index]
        for source in by_token[token]:
            if source in used:
                continue
            used.add(source)
            row.append(candidate_ids[source])
            visit(index + 1)
            row.pop()
            used.remove(source)

    visit(0)
    return result


def explain_schedule(candidate, original, trace):
    """Explain each exact reorder window using its captured candidate DAG."""
    if not trace.get('scheduler'):
        return []
    findings = []
    blocks = [block for block in trace['scheduler'] if block.get('dag')]
    for window in reorder_windows(candidate, original):
        options = []
        for block_index, block in enumerate(blocks):
            nodes = scheduler.block_nodes(block)
            model = scheduler.Model(nodes).run()
            captured = scheduler.captured_edges(block)
            if captured != model['deps']:
                continue
            assignments = _window_assignments(window, block, trace)
            for assignment in assignments:
                candidate_ids = [node['addr'] for node in assignment]
                for dos_ids in _dos_assignments(window, candidate_ids):
                    candidate_ir = [addr for index, addr in enumerate(candidate_ids)
                                    if not index or addr != candidate_ids[index - 1]]
                    dos_ir = [addr for index, addr in enumerate(dos_ids)
                              if not index or addr != dos_ids[index - 1]]
                    result = scheduler.classify_order(block, candidate_ir, dos_ir)
                    options.append((block_index, candidate_ids, dos_ids, result, model))
        classifications = {row[3]['classification'] for row in options}
        common = dict(mechanism='schedule', candidate_offset=window['candidate'][0].address,
                      dos_offset=window['dos'][0].address,
                      count=len(window['candidate']), candidate=window['candidate_tokens'],
                      dos=window['dos_tokens'],
                      confidence='Candidate scheduler evidence is observed; DOS machine-to-IR attribution is diagnostic.')
        if not options:
            common.update(classification='unverified', detail='no unique one-to-one candidate machine/IR mapping in one captured block')
        elif len(classifications) != 1 and not {'legal', 'illegal'} <= classifications:
            common.update(classification='ambiguous', detail='equally plausible instruction mappings disagree')
        else:
            mapping_ambiguous = len(options) > 1
            # Exact duplicate machine instructions can denote distinct backend
            # nodes. Preserve every assignment and report when only some make
            # the DOS order a valid linear extension of the captured graph.
            # Legal when at least one assignment is a legal linear extension; several assignments that are all
            # illegal stay illegal (rc_cluster 2026-10-24: n14 was reported legal with {'illegal': 3}).
            classification = 'legal' if 'legal' in classifications else next(iter(classifications))
            common['classification'] = classification
            common['block_index'] = options[0][0]
            results = [row[3] for row in options]
            if mapping_ambiguous:
                common['mapping_ambiguity'] = {key: sum(r['classification'] == key for r in results)
                                               for key in sorted(classifications)}
            if classification == 'legal':
                decisions = []
                seen = set()
                for result in results:
                    if result['classification'] != 'legal':
                        continue
                    for decision in result.get('decisions', []):
                        key = (decision['rule'], decision['detail'])
                        if key not in seen:
                            seen.add(key)
                            decisions.append(decision)
                common['decisions'] = decisions[:4]
                common['detail'] = ('DOS order has a legal linear extension in the captured candidate DAG; the '
                                    'reported priority rule(s) explain the observed candidate order.')
                if mapping_ambiguous:
                    common['detail'] += ' Identical machine instructions leave alternative IR attributions.'
            elif classification == 'illegal':
                violations, seen = [], set()
                for result in results:
                    for violation in result.get('violations', []):
                        key = (violation['later'], violation['earlier'], violation['rule'])
                        if key not in seen:
                            seen.add(key)
                            violations.append(violation)
                common['violations'] = violations[:4]
                common['detail'] = ('DOS order violates a captured candidate dependence edge; under the inferred '
                                    'mapping, the relevant difference predates scheduling.')
            else:
                common['detail'] = results[0].get('reason', 'scheduler model could not verify the observation')
        findings.append(common)
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
        result = dict(function=func, verifier=row, differences=differences, rover_offsets=offsets,
                    extent=dict(candidate=row['size'], dos=end-start, dos_end_known=bool(ends)),
                    limitations='Alignment and IR associations are diagnostic; ambiguous links retained. '
                    'ESP-home association assumes a fixed local frame; outgoing stack arguments and EBP frames are not modeled. '
                    'RET-frontier decoder excludes trailing tables; verifier checks full extent/references. '
                    'Final public DOS end uses candidate length if no following bound public.')
        schedules = explain_schedule(ci, oi, trace)
        if schedules:
            result['schedule'] = schedules
        return result
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
    for schedule_row in report.get('schedule', []):
        lines.append(f'schedule window +{schedule_row["candidate_offset"]:x}: {schedule_row["classification"]}; {schedule_row["detail"]}')
        if schedule_row.get('mapping_ambiguity'):
            lines.append('  scheduler IR attribution counts: ' + str(schedule_row['mapping_ambiguity']))
        for decision in schedule_row.get('decisions', []):
            lines.append(f'  schedule priority: {decision["rule"]}; {decision["detail"]}')
        for violation in schedule_row.get('violations', []):
            lines.append(f'  blocking edge: {violation["before"]} -> {violation["after"]}; '
                         f'{violation["rule"]}: {violation["detail"]}')
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
        cgtrace.trace(source, out, unit['profile'], args.func, schedule=True)
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
