"""Run: python -B -m unittest discover -s build/workers/difftest/tests -v"""
import json
import sys
import unittest
from pathlib import Path

sys.dont_write_bytecode = True
sys.path.insert(0, str(next(p for p in Path(__file__).resolve().parents if (p / 'tools/difftest').is_dir()) / 'tools' / 'difftest'))
from difftest import *
import domains


class PureTests(unittest.TestCase):
    def test_type_widths(self):
        for s, w, p in [('s8 x', 8, False), ('unsigned short x', 16, False), ('obj_t *x', 32, True),
                        ('s32 x', 32, False), ('void', 0, False), ('char x[10]', 32, True)]:
            t = ctype(s)
            self.assertEqual((t.width, t.pointer), (w, p), s)
        self.assertIsNone(ctype('opaque_t value').width)
        self.assertTrue(ctype('double x').floating)
        self.assertTrue(ctype('s16 x').signed)
        self.assertFalse(ctype('u16 x').signed)
        self.assertFalse(ctype('char c').signed)          # Watcom 10.6 without -j: char is unsigned
        t = ctype('const char *fmt')
        self.assertTrue(t.const_pointee); self.assertEqual(t.pointee.width, 8)
        t = ctype('s16 *out')
        self.assertFalse(t.const_pointee); self.assertEqual(t.pointee.width, 16)
        t = ctype('void (*func)(s16, s16)')
        self.assertTrue(t.pointer); self.assertEqual(len(t.fnproto.args), 2)

    def test_printf_format_words(self):
        self.assertEqual(format_words('%d %s %c%%'), 3)
        self.assertEqual(format_words('%5.2f'), 2)
        self.assertEqual(format_words('%ld|%04x'), 2)
        self.assertIsNone(format_words('%*d'))

    def test_ranges(self):
        self.assertEqual(list(ranges({1, 2, 3, 7, 9, 10})), [(1, 4), (7, 8), (9, 11)])


class DeclarationTableTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls): cls.t = table()

    def proto(self, name):
        p, why = self.t.prototype(name)
        self.assertIsNotNone(p, why)
        return p

    def test_definitions_and_pragmas(self):
        self.assertTrue(self.proto('MOVE_OBJECT').args[0].pointer)
        p = self.proto('wait_until')
        self.assertEqual((p.parm, p.value), (['ebx'], ['ecx']))
        self.assertEqual(self.proto('get_sprite_clipping').modify, [])
        self.assertEqual(self.proto('EOA').ret.width, 8)
        self.assertEqual(self.proto('atol').ret.width, 32)
        self.assertEqual(len(self.proto('Bresenham').args), 7)
        self.assertEqual(len(self.proto('BOX_IN_COLL_ZONES').args), 6)
        self.assertEqual(self.proto('HAS_MIT_JUMP').ret.width, 8)
        p = self.proto('GET_SPRITE_POS')
        self.assertEqual([a.pointee.width if a.pointee else None for a in p.args[2:]], [16, 16, 16, 16])
        self.assertEqual(self.proto('set_main_and_sub_etat').args[1].width, 8)

    def test_library_and_variadic_contracts(self):
        p = self.proto('sprintf')
        self.assertTrue(p.variadic); self.assertEqual(p.parm, []); self.assertTrue(p.cdecl)
        p = self.proto('fread')
        self.assertEqual(len(p.args), 4); self.assertEqual(p.parm, ['eax', 'edx', 'ebx', 'ecx'])

    def test_function_typedefs(self):
        # `extern block_copy_t block_copy;` declares a function of a typedef'd type with its pragma contract.
        p = self.proto('block_copy')
        self.assertEqual(p.parm, ['esi', 'edi'])
        # a parameter of function type decays to a function pointer
        self.assertIsNotNone(self.proto('SYNCHRO_LOOP').args[0].fnproto)

    def test_function_pointer_variables_and_members(self):
        t, src, is_array = self.t.variables['DrawSpriteNormalEtX']
        self.assertEqual(t.fnproto.parm, ['eax', 'edx', 'ebx', 'ecx', 'edi', 'esi'])
        t, src, is_array = self.t.variables['calcbloc1']
        self.assertTrue(is_array); self.assertEqual(len(t.fnproto.args), 2)
        members = self.t.struct_members['options_jeu_t']
        self.assertEqual([(o, n) for o, n, mt in members if mt.fnproto][:2], [(0, 'test_fire1'), (4, 'test_fire0')])


class PipelineTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls): cls.p = Program()

    def test_original_fixups_are_applied_to_flat_layout(self):
        for (o, a), (_, to, at) in list(self.p.img.fixups.items())[::199]:
            self.assertEqual(struct.unpack_from('<I', self.p.images[o], a)[0], flat((to, at)))

    def test_generator_is_address_based_and_code_identical(self):
        f = Function(self.p, 'mathutil', 'set2bits')
        s1 = Inputs(f, 73); s2 = Inputs(f, 73); s3 = Inputs(f, 74)
        self.assertEqual(s1.page(HEAP), s2.page(HEAP))
        self.assertNotEqual(s1.page(HEAP + 0x700000), s3.page(HEAP + 0x700000))
        # Both code mappings are plain identical input bytes for every execution (no code/data aliasing).
        self.assertEqual(s1.page(CAND)[:len(f.cbytes)], bytes(f.cbytes)[:PAGE])
        self.assertEqual(s1.page(IDA_BASE[1]), bytes(self.p.images[1][:PAGE]))
        a = Execution(f, s1); b = Execution(f, s1, True)
        for x in (a, b):
            x.ensure(CAND)
            self.assertEqual(bytes(x.machine.mem_read(CAND, 16)), bytes(f.cbytes[:16]))

    def test_exact_controls_memory_return_and_stack_arguments(self):
        for name in ['set2bits', 'getbit', 'sinus_cosinus']:
            with self.subTest(name=name):
                r = run_function(self.p, 'mathutil', name, seeds=20, seconds=10)
                self.assertEqual(r['verdict'], 'EQUIVALENT_TESTED', r.get('reason'))
                self.assertFalse(r['unjudged'])
                self.assertEqual(r['whole_unit']['strict_losses'], [])

    def test_near_stack_arguments(self):
        r = run_function(self.p, 'collision1', 'inter_box', seeds=24, seconds=10)
        self.assertEqual(r['verdict'], 'EQUIVALENT_TESTED', r.get('reason'))
        self.assertEqual(len(r['prototype']['args']), 8)

    def test_escaped_locals_compare_by_identity_and_stub_writes_outputs(self):
        # GET_SPRITE_POS(obj, spr, &x, &y, &w, &h): frame offsets differ between layouts, identities do not.
        r = run_function(self.p, 'basic', 'snapToSprite', seeds=24, seconds=15)
        self.assertEqual(r['verdict'], 'EQUIVALENT_TESTED', r.get('reason'))
        f = Function(self.p, 'basic', 'snapToSprite'); st = Inputs(f, 3)
        a = Execution(f, st); b = Execution(f, st, True)
        calls = [x for x in a.log if x['name'] == 'GET_SPRITE_POS']
        if calls:
            self.assertEqual(calls[0]['args'][2:], [{'frame_local': 0}, {'frame_local': 1}, {'frame_local': 2}, {'frame_local': 3}])

    def test_identity_numbering(self):
        f = Function(self.p, 'mathutil', 'reflexion'); st = Inputs(f, 0)
        e = Execution(f, st)
        self.assertEqual(e.identity(STACK_TOP - 40), {'frame_local': 0})
        self.assertEqual(e.identity(STACK_TOP - 8), {'frame_local': 1})
        self.assertEqual(e.identity(STACK_TOP - 40), {'frame_local': 0})
        self.assertEqual(e.identity(STACK_TOP + 8), {'entry_frame': 8})
        self.assertEqual(e.identity(0x12345), 0x12345)

    def test_function_pointer_globals_get_declared_contracts(self):
        f = Function(self.p, 'affiche', 'display_sprite')
        labels = {label for _, label in f.fnptr_slots.values()}
        self.assertIn('DrawSpriteNormalEtX', labels)
        st = Inputs(f, 1)
        self.assertTrue(any(lbl.startswith('fnptr:') for _, lbl in f.call_contracts.values()))

    def test_transparent_runtime_helpers(self):
        f = Function(self.p, 'breakout', 'collision_rayman')
        self.assertTrue(self.p.transparent)
        r = run_function(self.p, 'breakout', 'collision_rayman', seeds=24, seconds=15)
        self.assertNotIn('_I4FD', json.dumps(r.get('unknown_callees')))

    def test_b19_dead_load_and_discarded_halfword(self):
        r = run_function(self.p, 'special', 'oldest_planted', seeds=16, seconds=10, scenario='b19')
        self.assertEqual(r['verdict'], 'EQUIVALENT_TESTED', r.get('reason'))
        self.assertEqual(r['coverage'], 100)
        self.assertFalse(r['uninitialized_reads'])

    def test_b20_live_stack_residue_changes_written_bit(self):
        r = run_function(self.p, 'moteur', 'MOVE_OBJECT', seeds=6, seconds=10, scenario='b20')
        self.assertEqual(r['verdict'], 'DIVERGENT', r.get('reason'))
        c = r['counterexample']
        # the residue-dependent bit is observed at the next call (memory_at_call) or in final memory
        self.assertIn(c['first_difference']['kind'], ('memory', 'memory_at_call'))
        self.assertEqual(c['first_difference']['address'], HEAP + 0x10000 + 0x82)
        self.assertTrue(c['original']['uninitialized_stack'])
        self.assertFalse(c['candidate']['uninitialized_stack'])

    def test_unknown_calls_cannot_certify_equality(self):
        f = Function(self.p, 'mathutil', 'reflexion')
        state = Inputs(f, 0)
        a = Execution(f, state); b = Execution(f, state, True)
        a.call(0x12345678); b.call(0x12345678)
        d = compare(a, b)
        self.assertEqual(d['kind'], 'harness_limit')
        self.assertFalse(d['blocking'])
        self.assertIn('unknown callee prototype', d['reason'])

    def test_memory_before_a_call_must_agree(self):
        f = Function(self.p, 'mathutil', 'reflexion'); state = Inputs(f, 0)
        a = Execution(f, state); b = Execution(f, state, True)
        g = IDA_BASE[2] + 0x100
        v = bytes([(state.initial_byte(g) + 1) & 255])
        a.ensure(g); b.ensure(g)
        a.machine.mem_write(g, v); a.dirty.add(g); a.pending.add(g)
        a.call(0x12345678); b.call(0x12345678)
        b.machine.mem_write(g, v); b.dirty.add(g)
        d = compare(a, b)
        self.assertEqual(d['kind'], 'memory_at_call')
        self.assertEqual(d['address'], g)

    def test_asymmetric_step_budget_is_a_blocking_limit(self):
        f = Function(self.p, 'mathutil', 'reflexion')
        state = Inputs(f, 0)
        a = Execution(f, state, steps=1); b = Execution(f, state, True, steps=100)
        d = compare(a, b)
        self.assertEqual(d['kind'], 'harness_limit'); self.assertTrue(d['blocking'])

    def test_equal_step_budgets_with_different_partial_effects_are_limits(self):
        f = Function(self.p, 'mathutil', 'reflexion'); state = Inputs(f, 0)
        a = Execution(f, state, steps=1); b = Execution(f, state, True, steps=1)
        a.ensure(HEAP); b.ensure(HEAP)
        a.machine.mem_write(HEAP, b'\x01'); b.machine.mem_write(HEAP, b'\x02')
        a.dirty.add(HEAP); b.dirty.add(HEAP)
        d = compare(a, b)
        self.assertEqual(d['kind'], 'harness_limit'); self.assertFalse(d['blocking'])

    def test_replay_preserves_lazy_memory_and_overrides(self):
        f = Function(self.p, 'mathutil', 'set2bits'); state = Inputs(f, 13)
        restored = Inputs.restore(f, json.loads(json.dumps(state.summary())))
        for page in [HEAP, HEAP + PAGE, STACK_TOP - PAGE, IDA_BASE[2]]:
            self.assertEqual(state.page(page), restored.page(page))
        a = Execution(f, restored); b = Execution(f, restored, True)
        self.assertIsNone(compare(a, b))

    def test_domain_apply_and_membership(self):
        f = Function(self.p, 'jumelle', 'RecaleRayPosInJumelle')
        d = domains.load('jumelle', 'RecaleRayPosInJumelle', f)
        self.assertIsNotNone(d)
        width = flat(self.p.binds.game['LargeurJumelle'])
        for seed in range(8):
            st = Inputs(f, seed, domain=d)
            self.assertTrue(87 <= st.read_initial(width, 4) <= 224)
            self.assertTrue(d.contains(st), d.violations(st))
        outside = [Inputs(f, seed) for seed in range(8)]
        self.assertTrue(any(not d.contains(st) for st in outside))

    def test_domain_any_of_with_argument(self):
        f = Function(self.p, 'ray', 'ray_inertia_speed')
        d = domains.load('ray', 'ray_inertia_speed', f)
        world = flat(self.p.binds.game['num_world'])
        for seed in range(16):
            st = Inputs(f, seed, domain=d)
            w = st.read_initial(world, 2)
            self.assertIn(w, (1, 2, 3, 4, 5, 6))
            if w == 5:
                self.assertEqual(st.regs[f.proto.parm[1]] & 0xffff, 0)


class ModelTests(unittest.TestCase):
    """models.py: file layer on the real game data, allocator, environment parsing (no emulation)."""

    def test_resolve_is_case_insensitive_and_stays_inside_the_game_directory(self):
        import models
        p = models.resolve('pcmap\\jungle\\ray1.lev')
        self.assertIsNotNone(p)
        self.assertEqual(p.name.upper(), 'RAY1.LEV')
        self.assertTrue(p.is_relative_to(models.GAME_ROOT))
        self.assertIsNone(models.resolve('..\\VIGNET.DAT'))      # path_has_changed variant: parent directory
        self.assertIsNone(models.resolve('C:\\RAYMAN\\VIGNET.DAT'))
        self.assertIsNone(models.resolve('PCMAP\\NOSUCH.DAT'))
        self.assertIsNone(models.resolve('PCMAP'))                # a directory is not a file

    def test_file_layer_follows_the_c_library(self):
        import models
        fl = models.FileLayer()
        h = fl.fopen('RAY.LNG', 'rb')
        data = models.file_bytes(models.resolve('RAY.LNG'))
        self.assertNotEqual(h, 0)
        self.assertEqual(fl.fread(4, 2, h), (data[:8], 2))
        self.assertEqual(fl.ftell(h), 8)
        self.assertEqual(fl.fseek(h, len(data) - 3, 0), 0)
        self.assertEqual(fl.fread(2, 2, h), (data[-3:], 1))      # partial item: 3 bytes read, 1 complete item
        self.assertEqual(fl.ftell(h), len(data))
        self.assertEqual(fl.fread(1, 10, h), (b'', 0))           # at EOF
        self.assertEqual(fl.fseek(h, -1 & 0xffffffff, 1), 0)     # SEEK_CUR with a negative long
        self.assertEqual(fl.ftell(h), len(data) - 1)
        self.assertEqual(fl.fseek(h, -len(data) - 5 & 0xffffffff, 2), -1)
        self.assertEqual(fl.fseek(h, 100, 2), 0)                 # beyond EOF is allowed; reads return nothing
        self.assertEqual(fl.fread(1, 1, h), (b'', 0))
        self.assertEqual(fl.fclose(h), 0)
        self.assertEqual(fl.fclose(h), -1)
        self.assertEqual(fl.fopen('NOSUCH.DAT', 'rb'), 0)
        with self.assertRaises(models.ModelError):
            fl.fopen('RAY.LNG', 'wb')

    def test_allocator_and_environment(self):
        import models
        a = models.Allocator()
        x, y, z = a.alloc(10), a.alloc(0), a.alloc(100)
        self.assertTrue(x < y < z and y - x >= 10 and z - y >= 1 and x % 16 == y % 16 == z % 16 == 0)
        with self.assertRaises(ValueError):
            models.Environment({'file_layer': {}})                # every model needs evidence
        with self.assertRaises(ValueError):
            models.Environment({'nonsense': {'evidence': 'x'}})
        e = models.Environment({'boundary': {'callee': 'synchro', 'occurrence': 3, 'evidence': 'x'}})
        self.assertEqual(e.boundary, ('callee', 'synchro', 3))
        self.assertFalse(models.Environment())


class EnvironmentPipelineTests(unittest.TestCase):
    """Environment models and hardware models inside the emulation (single seeds, both sides)."""

    @classmethod
    def setUpClass(cls): cls.p = Program()

    def pair(self, unit, name, seed=1):
        f = Function(self.p, unit, name)
        d = domains.load(unit, name, f)
        f.attach_environment(Environment(d.spec.get('environment') if d else None))
        st = Inputs(f, seed, domain=d)
        return f, st, Execution(f, st, False, TIERS[-1]), Execution(f, st, True, TIERS[-1])

    def test_loader_reads_the_real_file_identically(self):
        f, st, a, b = self.pair('load', 'load_big_ray', 3)
        self.assertEqual((a.status, b.status), ('RETURN', 'RETURN'))
        self.assertIsNone(compare(a, b))
        self.assertTrue(a.files.log[0][1].upper().endswith('BRAY.DAT'))
        reads = [x for x in a.log if x.get('name') == 'fread']
        self.assertEqual(reads[0]['args'][1:3], [4, 1])
        # the first dword of BRAY.DAT (atlas size) arrived in wldobj[nb_des].x and sized the first allocation
        import models
        size = int.from_bytes(models.file_bytes(models.resolve('PCMAP\\BRAY.DAT'))[:4], 'little')
        allocs = [x for x in a.log if x.get('name') == 'block_malloc']
        self.assertEqual(allocs[0]['args'][1], size)

    def test_noreturn_ends_both_sides_after_the_compared_call(self):
        f = Function(self.p, 'load', 'LoadPcxOptions')
        d = domains.load('load', 'LoadPcxOptions', f)
        f.attach_environment(Environment(d.spec['environment']))
        st = Inputs(f, 5, domain=d)
        st.put(flat(self.p.binds.game['path_has_changed']), 1, 2)    # "..\\VIGNET.DAT" is not in the installation
        st.put(IDA_BASE[2] + 0x30e40, int.from_bytes(b'..\\\0', 'little'), 4)   # the "..\\" literal (inline strcpy source)
        st.page_cache = {}; st._po_key = None
        a = Execution(f, st); b = Execution(f, st, True, TIERS[0])
        self.assertEqual((a.status, b.status), ('EXIT', 'EXIT'))
        self.assertEqual(a.log[-1]['name'], 'FatalError')
        self.assertIsNone(compare(a, b))

    def test_boundary_stops_both_sides_at_the_same_call(self):
        f, st, a, b = self.pair('blood', 'DoEffectBloodOut', 2)
        self.assertEqual((a.status, b.status), ('BOUNDARY', 'BOUNDARY'))
        self.assertEqual(a.log[-1]['name'], 'synchro')
        self.assertEqual(sum(x.get('name') == 'synchro' for x in a.log), 3)
        self.assertIsNone(compare(a, b))

    def test_strict_leaf_callee_is_executed_natively(self):
        f, st, a, b = self.pair('effect2', 'sub_38ED4', 2)
        self.assertIn(flat(self.p.binds.lookup('sinus_cosinus')[0]), f.native_callees)
        self.assertEqual(a.status, 'RETURN')
        self.assertTrue(any(x.get('name') == 'sinus_cosinus' for x in a.log))
        self.assertIsNone(compare(a, b))
        with self.assertRaises(Limit):     # a non-leaf callee is refused
            f.attach_environment(Environment({'execute': {'callees': ['MOVE_OBJECT'], 'evidence': 'x'}}))

    def test_bios_tick_busy_waits_terminate(self):
        f = Function(self.p, 'pcmain', 'PcMain')
        self.assertTrue(f.bios_tick)
        f, st, a, b = self.pair('pcmain', 'PcMain', 1)
        self.assertIn(a.status, ('RETURN', 'BOUNDARY'))
        self.assertTrue(any(x.get('operation') == 'write_dr7' for x in a.log))

    def test_far_pointer_arguments_and_flat_selectors(self):
        t = table().prototype('sub_194a8')[0]
        self.assertTrue(t.args[0].far)
        f, st, a, b = self.pair('cdaudio', 'sub_195ec', 4)
        self.assertTrue(f.segment_loads)
        self.assertEqual((a.status, b.status), ('RETURN', 'RETURN'))
        call = next(x for x in a.log if x.get('name') == 'sub_194a8')
        self.assertEqual(len(call['args']), 5)                    # offset, selector, argument, command, size
        self.assertEqual(call['stack_arg_count'], 1)              # size is the one stack word (callee pops it)

    def test_interrupt_entry_and_overlay_prototypes(self):
        f, st, a, b = self.pair('input', 'PC_keyboard_interrupt_handler', 2)
        self.assertTrue(f.interrupt)
        self.assertEqual((a.status, b.status), ('RETURN', 'RETURN'))
        self.assertEqual(a.log[0]['name'], '__GETDS')
        self.assertTrue(a.log[0]['known'])
        f, st, a, b = self.pair('cheat', 'FIRST_INIT', 2)
        speaker = next(x for x in a.log if x.get('name') == 'set_speaker_on')
        self.assertTrue(speaker['known'])
        self.assertIsNone(compare(a, b))

    def test_domain_strings_slices_arrays_and_derived_arguments(self):
        f = Function(self.p, 'movie', 'sub_3ABE4')
        d = domains.load('movie', 'sub_3ABE4', f)
        for seed in range(4):
            st = Inputs(f, seed, domain=d)
            frame = d.arg_value(st, 1)
            self.assertEqual(d.arg_value(st, 2), frame + 16)
            self.assertEqual(st.read_initial(frame + 4, 2), 0xf1fa)     # a real FLC frame header
            self.assertTrue(d.contains(st), d.violations(st))
        f = Function(self.p, 'load', 'load_level')
        d = domains.load('load', 'load_level', f)
        st = Inputs(f, 7, domain=d)
        name = bytes(st.initial_byte(d.arg_value(st, 2) + k) for k in range(12)).split(b'\0')[0]
        self.assertRegex(name.decode(), r'^RAY\d+\.LEV$')
        f = Function(self.p, 'effect6ec80', 'effect_helper_6ef5c')
        d = domains.load('effect6ec80', 'effect_helper_6ef5c', f)
        st = Inputs(f, 3, domain=d)
        state = st.read_initial(flat(self.p.binds.game['effect_state_6e']), 4)
        verts = st.read_initial(state + 0x1c, 4)
        self.assertEqual([st.read_initial(verts + 0x34 * k, 2) for k in (0, 1, 20, 21, 22)], [0, 16, 320, 0, 16])
        self.assertEqual([st.read_initial(verts + 0x34 * k + 2, 1) for k in (0, 21, 230)], [0, 20, 200])


if __name__ == '__main__': unittest.main()
