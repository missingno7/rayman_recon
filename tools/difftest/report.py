"""Builds REPORT.md (the task deliverable) from RESULTS_v1.json, results/*.json, CONTROLS.json and
PROTOTYPES_UNKNOWN.json; the method text is in report_text.py."""
import collections
import json
from pathlib import Path

from report_text import HEAD, TAIL

HERE = Path(__file__).resolve().parent
OUT = HERE.parents[1] / 'build' / 'difftest'
ORDER = ['EQUIVALENT_TESTED', 'DIVERGENT', 'OUT_OF_DOMAIN_DIFFERENCE', 'HARNESS_LIMIT']


def load_v2():
    out = {}
    for p in sorted((OUT / 'results').glob('*.json')):
        r = json.loads(p.read_text())
        if r.get('harness_version') == 2:
            out[(r['unit'], r['function'])] = r
    return out


def fmt_value(v):
    if isinstance(v, int) and v > 9:
        return f'{v} ({v:#x})'
    return json.dumps(v, default=str)


def first_obs(d):
    if not d:
        return '-'
    k = d.get('kind')
    if k == 'memory':
        return (f"memory {d.get('symbol') or hex(d['address'])}: DOS {d['original']:#04x} vs C {d['candidate']:#04x} "
                f"(writers DOS {pc(d.get('original_pc'))}, C {pc(d.get('candidate_pc'))})")
    if k == 'memory_at_call':
        return (f"memory at call #{d['index']} ({d.get('callee')}) {d.get('symbol') or hex(d['address'])}: "
                f"DOS {d['original']:#04x} vs C {d['candidate']:#04x} (writers DOS {pc(d.get('original_pc'))}, C {pc(d.get('candidate_pc'))})")
    if k in ('call_arguments', 'call_argument_identity'):
        o, c = d['original'], d['candidate']
        return f"call #{d['index']} {o.get('name')} argument {d.get('arg')}: DOS {o.get('args')} vs C {c.get('args')}"
    if k == 'call_target':
        return f"call #{d['index']}: DOS {d['original'].get('name') or hex(d['original']['target'])} vs C {d['candidate'].get('name') or hex(d['candidate']['target'])}"
    if k == 'call_pointee':
        return f"call #{d['index']} {d['call'].get('name')} pointee of argument {d['arg']}: DOS {d['original']} vs C {d['candidate']}"
    if k == 'call_count':
        return f"number of calls: DOS {d['original']} vs C {d['candidate']}"
    if k == 'return':
        return f"return value (width {d['width']}): DOS {fmt_value(d['original'])} vs C {fmt_value(d['candidate'])}"
    if k == 'termination':
        return f"termination: DOS {d['original']} {d.get('original_fault') or ''} vs C {d['candidate']} {d.get('candidate_fault') or ''}"
    if k == 'system':
        return f"system event #{d['index']}: DOS {d['original']} vs C {d['candidate']}"
    return json.dumps(d, default=str)[:300]


def pc(v):
    if isinstance(v, int):
        if 0x10000 <= v < 0x90000:
            return f'1:{v - 0x10000:05x}'
        if v >= 0x58000000:
            return f'C+{v - 0x58000000:#x}'
        return hex(v)
    return str(v)


def main():
    v1 = {(r['unit'], r['function']): r for r in json.loads((OUT / 'RESULTS_v1.json').read_text())}
    v2 = load_v2()
    lines = []
    c1 = collections.Counter(r['verdict'] for r in v1.values())
    c2 = collections.Counter(r['verdict'] for r in v2.values())
    lines.append('## Verdict counts, v1 -> v2\n')
    lines.append('| verdict | v1 (first sweep) | v2 (this sweep) |')
    lines.append('|---|---:|---:|')
    for k in ORDER:
        lines.append(f'| {k} | {c1.get(k, 0)} | {c2.get(k, 0)} |')
    lines.append(f'| total | {sum(c1.values())} | {sum(c2.values())} |\n')
    trans = collections.Counter((v1.get(k, {}).get('verdict', '(absent)'), r['verdict']) for k, r in v2.items())
    lines.append('Transitions (v1 verdict -> v2 verdict):\n')
    lines.append('| v1 \\ v2 | ' + ' | '.join(ORDER) + ' |')
    lines.append('|---|' + '---:|' * len(ORDER))
    for a in ORDER + ['(absent)']:
        if any(trans[(a, b)] for b in ORDER):
            lines.append(f'| {a} | ' + ' | '.join(str(trans[(a, b)]) for b in ORDER) + ' |')
    missing = sorted(set(v1) - set(v2))
    if missing:
        lines.append(f'\nNot in v2 (v1 only): {missing}')
    lines.append('')

    eq = [r for r in v2.values() if r['verdict'] in ('EQUIVALENT_TESTED', 'OUT_OF_DOMAIN_DIFFERENCE')
          and r.get('in_domain_verdict', r['verdict']) == 'EQUIVALENT_TESTED']
    bins = [('100%', lambda c: c >= 100), ('90-99.9%', lambda c: 90 <= c < 100), ('75-89.9%', lambda c: 75 <= c < 90),
            ('50-74.9%', lambda c: 50 <= c < 75), ('<50%', lambda c: c < 50)]
    lines.append('## Coverage of the equivalence evidence\n')
    lines.append('Coverage = DOS basic blocks visited by judged (equal) seeds / all statically reachable DOS blocks. '
                 'Seeds that hit a per-seed limit (budget, CPU exception on both sides, NULL/unknown callee, one-sided '
                 'code aliasing) are not counted as evidence and do not contribute coverage.\n')
    lines.append('| coverage | functions (EQUIVALENT_TESTED in domain) | v1 EQUIVALENT_TESTED |')
    lines.append('|---|---:|---:|')
    v1eq = [r for r in v1.values() if r['verdict'] == 'EQUIVALENT_TESTED']
    for name, pred in bins:
        lines.append(f"| {name} | {sum(pred(r['coverage']) for r in eq)} | {sum(pred(r['coverage']) for r in v1eq)} |")
    seeds = [r.get('equal_seeds', 0) for r in eq]
    if seeds:
        seeds.sort()
        lines.append(f"\nJudged equal seeds per function: min {seeds[0]}, median {seeds[len(seeds) // 2]}, max {seeds[-1]}. "
                     f"Functions with some unjudged seeds: {sum(1 for r in eq if r.get('unjudged_seeds'))}.\n")
    noise = collections.Counter()
    for r in eq:
        for why, n in (r.get('unjudged') or {}).items():
            noise[classify(why)] += n
    if noise:
        lines.append('Unjudged seeds inside EQUIVALENT_TESTED functions, by cause (seeds):\n')
        for why, n in noise.most_common():
            lines.append(f'- {why}: {n}')
        lines.append('')

    lines.append('## DIVERGENT (first differing observable only; not triaged)\n')
    lines.append('| unit/function | v1 | seed | first observable | stub return mode |')
    lines.append('|---|---|---:|---|---|')
    for (u, f), r in sorted(v2.items()):
        if r['verdict'] != 'DIVERGENT':
            continue
        c = r.get('counterexample') or {}
        mode = 'even seed: extended' if c.get('seed', 0) % 2 == 0 else 'odd seed: upper bits callee-defined'
        ctx = (' [' + r['divergence_context'] + ']') if r.get('divergence_context') else ''
        dom = ' (inside documented domain)' if r.get('domain') else ''
        lines.append(f"| {u}/{f}{dom} | {v1.get((u, f), {}).get('verdict', '-')} | {c.get('seed')} | {esc(first_obs(c.get('first_difference')))}{ctx} | {mode} |")
    lines.append('')

    lines.append('## OUT_OF_DOMAIN_DIFFERENCE (difference only outside the documented domain)\n')
    lines.append('| unit/function | in-domain verdict | in-domain seeds / coverage | violated constraint(s) | first observable (outside) |')
    lines.append('|---|---|---|---|---|')
    for (u, f), r in sorted(v2.items()):
        if r['verdict'] != 'OUT_OF_DOMAIN_DIFFERENCE':
            continue
        o = r['out_of_domain_difference']
        viol = '; '.join(short_violation(v) for v in o.get('violations', []))
        lines.append(f"| {u}/{f} | {r.get('in_domain_verdict')} | {r.get('equal_seeds')} / {r['coverage']}% | {esc(viol)} | {esc(first_obs(o.get('first_difference')))} |")
    lines.append('')
    dom_eq = [k for k, r in v2.items() if r.get('domain') and r['verdict'] == 'EQUIVALENT_TESTED']
    if dom_eq:
        lines.append('Functions with a domain file and no difference found inside or outside it: ' +
                     ', '.join(f'{u}/{f}' for u, f in sorted(dom_eq)) + '\n')

    lines.append('## Remaining HARNESS_LIMIT, with reasons\n')
    hl = {k: r for k, r in v2.items() if r['verdict'] == 'HARNESS_LIMIT'}
    cats = collections.Counter(classify(r.get('reason') or '') for r in hl.values())
    lines.append('| cause | functions |')
    lines.append('|---|---:|')
    for k, n in cats.most_common():
        lines.append(f'| {k} | {n} |')
    lines.append('')
    lines.append('| unit/function | v1 | seeds (equal/run) | coverage of judged seeds | precise reason |')
    lines.append('|---|---|---|---:|---|')
    for (u, f), r in sorted(hl.items()):
        lines.append(f"| {u}/{f} | {v1.get((u, f), {}).get('verdict', '-')} | {r.get('equal_seeds', 0)}/{r.get('seeds', 0)} | "
                     f"{r.get('coverage', 0)}% | {esc((r.get('reason') or '')[:400])} |")
    lines.append('')
    proto = OUT / 'PROTOTYPES_UNKNOWN.json'
    if proto.exists():
        rep = json.loads(proto.read_text())
        lines.append('## Callables whose contract cannot be determined from the project declarations\n')
        for k, v in sorted(rep.items()):
            if k == '_entry':
                for fn, why in v.items():
                    lines.append(f'- entry {fn}: {why}')
                continue
            callers = sorted(set(v.get('callers', [])))
            lines.append(f"- {k}: {v.get('why')} (referenced by {len(callers)}: {', '.join(callers[:6])}{' ...' if len(callers) > 6 else ''})")
        lines.append('')
    ctl = OUT / 'CONTROLS.json'
    if ctl.exists():
        c = json.loads(ctl.read_text())
        lines.append('## Controls (CONTROLS.json)\n')
        lines.append(f"passed: **{c.get('passed')}**; checks: {c.get('checks')}\n")
        lines.append('| kind | function | mutation | verdict | first observable |')
        lines.append('|---|---|---|---|---|')
        for kind in ('positive', 'negative', 'neutral', 'known_cases'):
            for r in c.get(kind, []):
                m = (r.get('mutation') or {}).get('kind', '-')
                d = (r.get('counterexample') or {}).get('first_difference')
                extra = f"cov {r['coverage']}%, eq {r.get('equal_seeds')}, unjudged {r.get('unjudged_seeds')}" if not d else esc(first_obs(d))
                lines.append(f"| {kind} | {r['unit']}/{r['function']} | {m} | {r['verdict']} | {extra} |")
        lines.append('')
    (OUT / 'REPORT.md').write_text(HEAD + '\n'.join(lines) + '\n' + TAIL, encoding='utf-8')
    print('\n'.join(lines[:12]))


def short_violation(v):
    if 'any_of_failed' in v:
        return 'none of the documented alternatives: ' + '; '.join(
            ', '.join(short_violation(x) for x in alt) for alt in v['any_of_failed'][:2])
    loc = v['loc']
    where = loc.get('global') or f"arg{loc.get('arg')}"
    if loc.get('offset'):
        where += f"+{loc['offset']}"
    return f"{where}={v['value']} (expected {v['expected']})"


def classify(why):
    w = why.lower()
    if 'unknown callee prototype' in w or 'no declared contract' in w:
        return 'callee contract unknown (unjudged seeds)'
    if 'null function pointer' in w:
        return 'call through NULL function pointer (seed-chosen)'
    if 'budget' in w and 'asymmetric' in w:
        return 'asymmetric termination at the top instruction budget'
    if 'budget' in w:
        return 'both sides exceed the top instruction/call budget'
    if 'cpu exception' in w:
        return 'CPU exception on both sides'
    if 'aliased the executing function code' in w:
        return 'generated pointer aliased executing code (one side)'
    if 'self-modifying' in w:
        return 'self-modifying code (both sides)'
    if 'asymmetric termination' in w:
        return 'asymmetric termination (one side LIMIT/FAULT)'
    if 'entry prototype' in w:
        return 'entry prototype unsupported'
    if 'unresolved indirect jump' in w or 'undecodable' in w:
        return 'static CFG incomplete'
    if 'control escaped' in w or 'outside the static' in w or 'unmapped' in w:
        return 'control left the function (both sides)'
    if 'page budget' in w:
        return 'materialised page budget'
    if 'only' in w and 'judged' in w:
        return 'too few judged seeds'
    return why[:80]


def esc(s):
    return str(s).replace('|', '\\|').replace('\n', ' ')


if __name__ == '__main__':
    main()
