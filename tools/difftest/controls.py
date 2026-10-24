"""Full-pipeline controls. Mutations are C edits, never binary edits."""
from difftest import *


def function_span(text, name):
    m = re.search(r'\b' + re.escape(name) + r'\s*\([^;{}]*\)\s*\{', text)
    if not m: raise ValueError(name)
    end = m.end(); depth = 1
    while depth:
        if text[end] == '{': depth += 1
        elif text[end] == '}': depth -= 1
        end += 1
    return m.start(), end


def mutation_source(unit, name, label, old, new):
    src = copy_unit(unit)
    text = src.read_text()
    start, end = function_span(text, name)
    body = text[start:end]
    if body.count(old) != 1: raise ValueError((unit, name, old, body.count(old)))
    out = OUT / 'mutations' / label / src.name
    out.parent.mkdir(parents=True, exist_ok=True)
    for p in cdecls.read_closure(src, {}):
        if p.parent == OUT.resolve() and p != src.resolve():
            shutil.copyfile(p, out.parent / p.name)
    out.write_text(text[:start] + body.replace(old, new, 1) + text[end:], encoding='utf-8')
    return out


# v1 mutations (behaviour edits) and v2 mutations exercising the new observables.
MUTATIONS = [
    ('ray', 'determineRayAirInertia', 'constant', 'if (wind != 0) { ray.nb_cmd=1; return; }', 'if (wind != 0) { ray.nb_cmd=2; return; }'),
    ('collision1', 'inter_box', 'comparison', 'comp_x <= x_2', 'comp_x > x_2'),
    ('bbmont', 'INIT_BBMONT', 'store_removed', 'WaitForFinAtan = 10;', '/* deliberately removed store */'),
    ('moteur', 'correct_link', 'wrong_field', 'obj->link_has_gendoor = 0;', 'obj->field_1C = 0;'),
    ('basic', 'save_objects_flags', 'off_by_one', 'i < nb_objects', 'i <= nb_objects'),
    # v2: output parameters / object identity / declared widths / function-pointer contracts / call-time memory
    ('basic', 'snapToSprite', 'swapped_output_parameters',
     'GET_SPRITE_POS(obj_2, obj_2_spr, &obj_2_x, &obj_2_y, &obj_2_w, &obj_2_h);',
     'GET_SPRITE_POS(obj_2, obj_2_spr, &obj_2_y, &obj_2_x, &obj_2_w, &obj_2_h);'),
    ('breakout', 'init_briques', 'aliased_output_parameter',
     'get_brik_info(obj, brick_index, &x, &y, &kind)', 'get_brik_info(obj, brick_index, &y, &y, &kind)'),
    ('basic', 'GET_SPRITE_POS', 'wrong_output_parameter_value',
     '*h = sprite->inner_height;', '*h = sprite->inner_height + 1;'),
    ('basic', 'snapToSprite', 'wrong_callee_argument_width',
     'GET_SPRITE_POS(obj_2, obj_2_spr, &obj_2_x, &obj_2_y, &obj_2_w, &obj_2_h);',
     'GET_SPRITE_POS(obj_2, (s8)obj_2_spr, &obj_2_x, &obj_2_y, &obj_2_w, &obj_2_h);'),
    ('affiche', 'display_sprite', 'function_pointer_arguments_swapped',
     'draw_func(proj_x, color, proj_y, packed_size, target, image);',
     'draw_func(proj_y, color, proj_x, packed_size, target, image);'),
    ('basic', 'snapToSprite', 'store_moved_after_call',
     'obj_2->anim_frame = obj_2_anim_frame;\n  obj_2->anim_index = obj_2_anim_index;\n  GET_SPRITE_POS(obj_2, obj_2_spr, &obj_2_x, &obj_2_y, &obj_2_w, &obj_2_h);',
     'GET_SPRITE_POS(obj_2, obj_2_spr, &obj_2_x, &obj_2_y, &obj_2_w, &obj_2_h);\n  obj_2->anim_frame = obj_2_anim_frame;\n  obj_2->anim_index = obj_2_anim_index;'),
]
# Mutations that must stay EQUIVALENT (the harness must not over-observe).
NEUTRAL = [
    # Swapping the declaration order of two escaped locals changes their frame slots, never their identity.
    ('basic', 'snapToSprite', 'escaped_local_reordered',
     's16 obj_2_x;   s16 obj_2_y;', 's16 obj_2_y;   s16 obj_2_x;'),
]
EXPECTED_KIND = {
    'swapped_output_parameters': None, 'aliased_output_parameter': 'call_argument_identity',
    'wrong_output_parameter_value': 'memory', 'wrong_callee_argument_width': 'call_arguments',
    'function_pointer_arguments_swapped': 'call_arguments', 'store_moved_after_call': 'memory_at_call',
}


def run_controls(p=None, seeds=96):
    p = p or Program()
    report = {'positive': [], 'negative': [], 'neutral': [], 'known_cases': []}
    positive = [('mathutil', 'reflexion'), ('mathutil', 'set2bits'), ('mathutil', 'getbit'),
                ('mathutil', 'sinus_cosinus'), ('bonus', 'calc_left_time'), ('basic', 'set_main_and_sub_etat')]
    for unit, name in positive:
        if p.units[unit]['functions'].get(name, {}).get('status') != 'STRICT':
            raise AssertionError('positive control must be recorded STRICT: ' + name)
        r = run_function(p, unit, name, seeds=seeds, seconds=20)
        r['zero_noise'] = r['verdict'] == 'EQUIVALENT_TESTED' and not r.get('unjudged') and not r.get('blocking_limits')
        report['positive'].append(compact(r))
        print('positive', name, r['verdict'], r['coverage'], 'zero_noise' if r['zero_noise'] else 'NOISE', r.get('reason', ''), flush=True)
    for unit, name, label, old, new in MUTATIONS:
        src = mutation_source(unit, name, label, old, new)
        r = run_function(p, unit, name, seeds=seeds, seconds=25, source=src, allow_losses=True)
        r['mutation'] = {'kind': label, 'old': old, 'new': new, 'source': str(src.relative_to(ROOT))}
        d = (r.get('counterexample') or {}).get('first_difference') or {}
        r['first_kind'] = d.get('kind')
        want = EXPECTED_KIND.get(label)
        r['kind_ok'] = want is None or d.get('kind') == want
        report['negative'].append(compact(r))
        print('negative', label, name, r['verdict'], d.get('kind'), r['coverage'], r.get('reason', ''), flush=True)
    for unit, name, label, old, new in NEUTRAL:
        src = mutation_source(unit, name, label, old, new)
        r = run_function(p, unit, name, seeds=seeds, seconds=25, source=src, allow_losses=True)
        r['mutation'] = {'kind': label, 'old': old, 'new': new, 'source': str(src.relative_to(ROOT))}
        report['neutral'].append(compact(r))
        print('neutral', label, name, r['verdict'], r['coverage'], r.get('reason', ''), flush=True)
    for unit, name, scenario in [('special', 'oldest_planted', 'b19'), ('moteur', 'MOVE_OBJECT', 'b20')]:
        r = run_function(p, unit, name, seeds=seeds, seconds=25, scenario=scenario)
        report['known_cases'].append(compact(r))
        print('known', name, r['verdict'], r['coverage'], r.get('reason', ''), flush=True)
    b20 = report['known_cases'][1]
    b20_ok = (b20['verdict'] == 'DIVERGENT' and bool((b20.get('counterexample') or {}).get('original', {}).get('uninitialized_stack')))
    report['checks'] = {
        'positive_equivalent_zero_noise': all(r['zero_noise'] for r in report['positive']),
        'negative_all_divergent': all(r['verdict'] == 'DIVERGENT' for r in report['negative']),
        'negative_expected_observable': all(r['kind_ok'] for r in report['negative']),
        'neutral_equivalent': all(r['verdict'] == 'EQUIVALENT_TESTED' for r in report['neutral']),
        'b19_equivalent': report['known_cases'][0]['verdict'] == 'EQUIVALENT_TESTED',
        'b20_divergent_with_uninitialised_read': b20_ok,
    }
    report['passed'] = all(report['checks'].values())
    write_json(OUT / 'CONTROLS.json', report)
    print('checks:', report['checks'], flush=True)
    print('controls passed:', report['passed'], flush=True)
    return 0 if report['passed'] else 1


def compact(r):
    c = r.get('counterexample')
    if c:
        r = dict(r)
        r['counterexample'] = {k: c.get(k) for k in ('seed', 'first_difference', 'minimization')}
        r['counterexample']['original'] = {k: c['original'].get(k) for k in ('termination', 'uninitialized_stack', 'return')}
        r['counterexample']['candidate'] = {k: c['candidate'].get(k) for k in ('termination', 'uninitialized_stack', 'return')}
    return r


if __name__ == '__main__':
    sys.exit(run_controls())
