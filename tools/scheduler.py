"""Recovered Watcom 10.6 integer scheduler model and order diagnostics.

Inputs are read-only scheduler observations from cgtrace.  The DOS compiler IR
is unavailable, so machine-to-IR associations remain diagnostic hypotheses.
FP/segment-register and FORTRAN scheduler paths are outside this model.
"""
from collections import Counter


def s32(n):
    return (n + 0x80000000) % 0x100000000 - 0x80000000


def same(a, b):
    return (a or {}).get('addr') == (b or {}).get('addr')


def overlap(a, b):
    x, y = s32(a['offset']), s32(b['offset'])
    return x <= y + b['size'] - 1 and y <= x + a['size'] - 1


def fullreg(r):
    out = 0
    for mask in (0x1000003, 0x200000c, 0x4000030, 0x80000c0,
                 0x10000100, 0x20000200, 0x400, 0x800):
        if mask & r:
            out |= mask
    return out


class Model:
    """Independent integer scheduler model, ported from sched/model.py."""

    def __init__(self, nodes):
        self.nodes = nodes
        self.order = {ins['addr']: pos for pos, ins in enumerate(nodes)}

    def ztemp(self, r, o, idx=False):
        cls = r['cls']
        if cls == 2:
            return r['id'] == o['id'] and (idx or overlap(r, o))
        if cls == 1:
            return False
        if cls == 4:
            base = r.get('base')
            if base and base['cls'] == 2 and base['id'] == o['id']:
                return True
            return bool(o.get('usage', 0) & 8) and base is None
        return False

    def zmem(self, r, o, idx=False):
        cls = r['cls']
        if cls == 1:
            return r['symbol'] == o['symbol'] and (idx or overlap(r, o))
        if cls in (2, 3):
            return False
        if cls == 4:
            base = r.get('base')
            if base is None:
                return bool(o.get('usage', 0) & 8)  # RELAX_ALIAS under -oa
            if base['cls'] == 2:
                return False
            return self.zmem(base, o, True)
        return False

    def zindexed(self, r, o):
        cls, base, index = r['cls'], o.get('base'), o.get('index')
        if cls == 1:
            if base is None:
                return bool(o.get('index_flags', 0) & 8)
            if base['cls'] == 2:
                return False
            return self.zmem(r, base, True)
        if cls == 2:
            if base is None and r.get('usage', 0) & 8:
                return True
            if base and base['cls'] == 2 and r['id'] == base['id']:
                return True
            return bool(index and index['cls'] == 2 and r['id'] == index['id'])
        if cls == 3:
            return bool((base and self.zop(r, base)) or self.zop(r, index))
        if cls == 4:
            if (same(r.get('base'), base) and same(r.get('index'), index)
                    and r['index_flags'] == o['index_flags'] and r['scale'] == o['scale']):
                x, y = r['displacement'], o['displacement']
                if x >= y + o['size'] or y >= x + r['size']:
                    return 2
            rbase = r.get('base')
            if rbase is None or base is None:
                return True
            if rbase['cls'] != base['cls']:
                return False
            if rbase['cls'] == 2:
                return self.ztemp(rbase, base, True)
            return self.zmem(rbase, base, True)
        return False

    def zop(self, r, o):
        if r is None or o is None:
            return False
        cls = o['cls']
        if cls in (1, 2) and o.get('usage', 0) & 32:
            return True
        if cls == 4 and o.get('index_flags', 0) & 2:
            return True
        if cls == 1:
            return self.zmem(r, o)
        if cls == 2:
            return self.ztemp(r, o)
        if cls == 3:
            return r['cls'] == 3 and bool(r['reg'] & o['reg'])
        if cls == 4:
            return self.zindexed(r, o)
        return False

    def visible(self, ins, op, modifies):
        if op['cls'] == 1:
            return not (modifies and ins['flags'] & 2)
        if op['cls'] == 4:
            return not op.get('base') or self.redef(ins, op['base'])
        if op['cls'] == 2:
            return bool(op.get('usage', 0) & 8)
        return False

    def redef(self, ins, op):
        if op['cls'] == 3 and ins['zap'] & op['reg']:
            return True
        if (op['cls'] in (1, 2) and op.get('usage', 0) & 256
                and not (op['cls'] == 1 and op.get('usage', 0) & 32)):
            return False
        z = self.zop(ins['result'], op)
        if z:
            return z
        return ins['op'] in (41, 54) and self.visible(ins, op, True)

    def hidden(self, ins, op):
        if op['cls'] != 3:
            return False
        family, current = fullreg(op['reg']), ins.get('result')
        # This preserves the observed 10.6 loop: it advances only after a
        # non-overlapping register, and therefore checks result + early inputs.
        for index in range(len(ins['operands'])):
            if current is None or current['cls'] != 3:
                continue
            if family & current['reg']:
                return True
            current = ins['operands'][index]
        return False

    def check(self, ins, other, op, linked):
        if op['cls'] == 4 and op.get('index'):
            index = op['index']
            if (linked and ins['ins_flags'] & 128 and ins['result']
                    and ins['result']['cls'] == 3 and index['cls'] == 3
                    and ins['result']['reg'] & index['reg']):
                if (op.get('base') or op['displacement']
                        or not any(n['cls'] == 0 for n in other['operands'])):
                    return 0
            if self.redef(ins, index):
                return 2
        redefined = self.redef(ins, op)
        if redefined == 2 and linked:
            a, b = sorted((self.order[ins['addr']], self.order[other['addr']]))
            redefined = any(self.redef(node, op['index']) for node in self.nodes[a:b])
        if redefined or self.hidden(ins, op):
            return 1
        return 0

    def data(self, ins, other, linked):
        for index in range(len(other['operands']) - 1, -1, -1):
            op = other['operands'][index]
            if index == 0 and other['op'] == 40 and op['cls'] != 4:
                continue
            dep = self.check(ins, other, op, linked)
            if dep:
                return dep
        if other.get('result'):
            return self.check(ins, other, other['result'], linked)
        return 0

    def stack(self, ins):
        if ins['op'] in (41, 42, 43, 54):
            return True
        for op in ins['operands'] + [ins.get('result')]:
            if op and op['cls'] == 4:
                op = op.get('index')
            if op and op['cls'] == 3 and op['reg'] & 0x800:
                return True
        return False

    def dependent(self, ins, other):
        if other['op'] == 0 and other.get('result') is None and other['gen_code'] == 0:
            return True
        if 45 <= ins['op'] <= 53:
            return True
        if (ins['op'] in (41, 54) and not ins['flags'] & 1 and other.get('result')
                and self.visible(ins, other['result'], False)):
            return True
        if self.stack(ins) and self.stack(other):
            return True
        return bool(self.data(ins, other, True) or self.data(other, ins, True))

    def _call_visibility(self, ins, other):
        return (ins['op'] in (41, 54) and not ins['flags'] & 1 and other.get('result')
                and self.visible(ins, other['result'], False))

    def _hidden_pair(self, ins, other):
        for op in other['operands'] + [other.get('result')]:
            if op and self.hidden(ins, op):
                return True
        for op in ins['operands'] + [ins.get('result')]:
            if op and self.hidden(other, op):
                return True
        return False

    @staticmethod
    def _memory_name(op):
        if not op:
            return False
        if op['cls'] in (1, 4):
            return True
        if op['cls'] == 2:
            return False
        return False

    def _has_memory_operand(self, ins):
        return any(self._memory_name(op) for op in ins['operands'] + [ins.get('result')])

    def _edge_rule(self, ins, other, multi):
        """Return the first BuildDag rule that adds this edge, if any."""
        if ins['op'] == 0:
            return dict(rule='NOP wall', detail='a scheduled NOP separates this pair')
        if self.dependent(ins, other):
            if self._call_visibility(ins, other):
                return dict(rule='call visibility', detail='the call can observe the earlier result')
            if self._hidden_pair(ins, other):
                return dict(rule='HiddenDependancy', detail='shared full-register family blocks motion')
            if (ins['op'] in (41, 54) or other['op'] in (41, 54)) and (
                    self._has_memory_operand(ins) or self._has_memory_operand(other)):
                return dict(rule='call visibility', detail='call read/write visibility creates a data edge')
            if self.stack(ins) and self.stack(other):
                return dict(rule='stack order', detail='both instructions access the implicit stack')
            if self._has_memory_operand(ins) or self._has_memory_operand(other):
                return dict(rule='alias/name', detail='backend memory-name identity or overlap creates a data edge')
            if 45 <= ins['op'] <= 53:
                return dict(rule='CC', detail='condition-code/selection instruction preserves order')
            return dict(rule='InsOrderDependant', detail='register/temp data dependence')
        if multi and other['op'] in (1, 3, 5, 17) and other['ins_flags'] & 32:
            return dict(rule='multi', detail='multi-result operation barrier')
        if ins['ins_flags'] & 32 and other['gen_cc'] != 32768:
            return dict(rule='CC', detail='the later instruction consumes condition codes')
        if ins['gen_cc'] != 32768 and (other['op'] in (2, 4, 6)
                or 46 <= other['op'] <= 53 and other['gen_code'] == 0):
            return dict(rule='CC', detail='condition-code producer/consumer order')
        return None

    def edges_with_rules(self):
        deps = {ins['addr']: [] for ins in self.nodes}
        rules = {}
        for pos, ins in enumerate(self.nodes):
            multi = ins['op'] in (2, 4, 6)
            for other in reversed(self.nodes[:pos]):
                reason = self._edge_rule(ins, other, multi)
                if reason:
                    deps[ins['addr']].insert(0, other['addr'])
                    rules[(ins['addr'], other['addr'])] = reason
                    if reason['rule'] == 'multi':
                        multi = False
        return deps, rules

    def edges(self):
        return self.edges_with_rules()[0]

    def stall(self, ins, suffix):
        if ins['op'] == 0 or ins['ins_flags'] & 32:
            return -1
        if not suffix:
            return 0
        fu = ins['fu']
        avail, unit, operand = fu['good_fu'], fu['unit'], fu['operand']
        for other in suffix:
            if unit == 0 and operand == 0:
                return 0
            if operand > 0:
                operand -= 1
                dep = self.data(ins, other, False)
                if dep == 1:
                    return operand * 2
                if dep == 2 or (self.stack(ins) and self.stack(other)):
                    return operand * 2 + 1
            if unit > 0:
                unit -= 1
                overlap_fu = avail & other['fu']['good_fu']
                if overlap_fu:
                    overlap_fu &= overlap_fu - 1
                    avail &= overlap_fu
                    if avail == 0:
                        return unit * 2
        return 0

    def stallable(self, ins):
        value = sum({4: 3, 3: 2, 1: 1}.get(op['cls'], 0) for op in ins['operands'])
        return value + 3 * bool(ins.get('result') and ins['result']['cls'] == 4)

    def compare_rule(self, a, b, suffix, height, stallable):
        """Explain why a outranks b using the recovered ScheduleIns comparator."""
        ca, cb = self.stall(a, suffix), self.stall(b, suffix)
        if ca != cb:
            return dict(rule='stall cost', detail=f'{ca} beats {cb} (lower is preferred)')
        ha, hb = height[a['addr']], height[b['addr']]
        if ha != hb:
            return dict(rule='DAG height', detail=f'{ha} beats {hb} (higher is preferred)')
        if a['ins_flags'] & 128 and self.data(a, b, False):
            return dict(rule='INDEX_ADJUST/DataDependant',
                        detail='the INDEX_ADJUST/data-dependence tie preference selects this instruction')
        sa, sb = stallable[a['addr']], stallable[b['addr']]
        if sa != sb:
            return dict(rule='stallability', detail=f'{sa} beats {sb} (higher is preferred)')
        if a['sequence'] == 0 and b['sequence'] != 0:
            return dict(rule='FP sequence', detail='sequence zero is preferred')
        if a['id'] != b['id']:
            return dict(rule='backend instruction ID', detail=f'{a["id"]} beats {b["id"]} (higher is preferred)')
        return dict(rule='ready-list order', detail='all captured priority fields tie')

    def run(self):
        """Recompute DAG, heights, readiness, costs, selections and audit rows."""
        deps, edge_rules = self.edges_with_rules()
        height, anc = {}, Counter()
        ins = {node['addr']: node for node in self.nodes}
        stallable = {node['addr']: self.stallable(node) for node in self.nodes}
        for node in self.nodes:
            for predecessor in deps[node['addr']]:
                anc[predecessor] += 1
            height[node['addr']] = max([height[p] for p in deps[node['addr']]] + [0]) + node['fu']['operand']
        ready = [node['addr'] for node in self.nodes if anc[node['addr']] == 0]
        suffix, chosen, audit = [], [], []
        while ready:
            best, best_rule = None, 'first ready instruction'
            candidate_rows = []
            for addr in ready:
                node = ins[addr]
                candidate_rows.append(dict(addr=addr, cost=self.stall(node, suffix),
                                           height=height[addr], stallable=stallable[addr],
                                           id=node['id'], sequence=node['sequence']))
                if best is None:
                    best = (addr, candidate_rows[-1]['cost'])
                    continue
                best_addr, best_cost = best
                old = ins[best_addr]
                new_height, old_height = height[addr], height[best_addr]
                replace = (candidate_rows[-1]['cost'] < best_cost
                           or candidate_rows[-1]['cost'] == best_cost and (
                               new_height > old_height or new_height == old_height and (
                                   node['ins_flags'] & 128 and self.data(node, old, False)
                                   or stallable[addr] > stallable[best_addr]
                                   or stallable[addr] == stallable[best_addr] and (
                                       node['sequence'] == 0 and old['sequence'] != 0
                                       or node['id'] > old['id']))))
                if replace:
                    best_rule = self.compare_rule(node, old, suffix, height, stallable)['rule']
                    best = (addr, candidate_rows[-1]['cost'])
            addr, _ = best
            chosen.append(addr)
            audit.append(dict(step=len(audit), chosen=addr, priority_rule=best_rule,
                              candidates=candidate_rows))
            suffix.insert(0, ins[addr])
            ready.remove(addr)
            for predecessor in deps[addr]:
                anc[predecessor] -= 1
                if anc[predecessor] == 0:
                    ready.insert(0, predecessor)
        if len(chosen) != len(self.nodes):
            raise ValueError('cyclic scheduler dependence graph')
        return dict(chosen=chosen, forward=list(reversed(chosen)), deps=deps,
                    edge_rules=edge_rules, height=height, stallable=stallable, audit=audit)

    def schedule(self):
        """Compatibility with the original recovery model API."""
        result = self.run()
        return result['chosen'], result['deps']


def block_nodes(block):
    """Scheduler observation lists DAG entries in the reverse linked order."""
    return [row['ins'] for row in reversed(block.get('dag', []))]


def captured_edges(block):
    """Translate dependency-list pointers to stable IR instruction addresses."""
    dag_by_addr = {row['addr']: row for row in block.get('dag', [])}
    return {row['ins']['addr']: [dag_by_addr[dep]['ins']['addr'] for dep in row.get('deps', [])]
            for row in block.get('dag', [])}


def _ir_label(ins):
    return f'IR@{ins["addr"]:x} op={ins["op"]}'


def classify_order(block, candidate_order, dos_order):
    """Classify mapped instruction IDs against one captured block.

    The orders contain IR addresses.  A DOS order is legal iff it respects each
    captured edge among the mapped instructions.  Candidate schedule replay and
    captured DAG equality are checked before any conclusion is returned.
    """
    nodes = block_nodes(block)
    model = Model(nodes)
    result = model.run()
    by_addr = {node['addr']: node for node in nodes}
    captured = captured_edges(block)
    if captured != result['deps']:
        return dict(classification='unverified', reason='captured DAG differs from recovered model')
    observed = [row.get('ins') for row in block.get('rounds', [])]
    if observed and observed != result['chosen']:
        return dict(classification='unverified', reason='captured selections differ from recovered model')
    positions = {addr: pos for pos, addr in enumerate(dos_order)}
    violations = []
    for later in dos_order:
        for earlier in result['deps'].get(later, []):
            if earlier in positions and positions[earlier] > positions[later]:
                rule = result['edge_rules'][(later, earlier)]
                violations.append(dict(before=_ir_label(by_addr[later]), after=_ir_label(by_addr[earlier]),
                                       later=later, earlier=earlier, rule=rule['rule'], detail=rule['detail']))
    if violations:
        return dict(classification='illegal', violations=violations,
                    confidence='PROVEN candidate DAG edge; DOS-to-IR association is diagnostic')
    candidate_positions = {addr: pos for pos, addr in enumerate(candidate_order)}
    dos_positions = {addr: pos for pos, addr in enumerate(dos_order)}
    inversions = [(a, b) for a in candidate_order for b in candidate_order
                  if candidate_positions[a] < candidate_positions[b]
                  and dos_positions[b] < dos_positions[a]]
    decisions = []
    for candidate_first, candidate_second in inversions:
        # The scheduler builds bottom-up; candidate_second is picked before
        # candidate_first when its forward order is candidate_first, candidate_second.
        chosen = candidate_second
        step = next((row for row in result['audit'] if row['chosen'] == chosen), None)
        if step is None or candidate_first not in {row['addr'] for row in step['candidates']}:
            continue
        detail = model.compare_rule(by_addr[chosen], by_addr[candidate_first],
                                    [by_addr[row['chosen']] for row in reversed(result['audit'][:step['step']])],
                                    result['height'], result['stallable'])
        decisions.append(dict(ours_before=_ir_label(by_addr[candidate_first]),
                              ours_after=_ir_label(by_addr[chosen]), dos_reverses=True,
                              rule=detail['rule'], detail=detail['detail']))
    return dict(classification='legal', decisions=decisions,
                confidence='PROVEN candidate DAG/model; DOS-to-IR association is diagnostic')

