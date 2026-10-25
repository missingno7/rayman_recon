"""Scoped semantic registries: enforced code/layout and separately reported evidence drift.

This full file works both in the worker proposal and after integration. All test
scratch paths are under the selected semantic work root; compiles remain serial.
"""
import contextlib
import copy
import importlib.util
import io
import json
from pathlib import Path
import sys
import tempfile
import unittest

sys.dont_write_bytecode = True
ROOT = next(p for p in Path(__file__).resolve().parents if (p/'config/oracles.json').is_file())
HERE = Path(__file__).resolve().parent.parent
TOOL = HERE/'tools/semantics.py' if (HERE/'registries').is_dir() else ROOT/'tools/semantics.py'
spec = importlib.util.spec_from_file_location('semantics_under_test',TOOL)
semantics = importlib.util.module_from_spec(spec); spec.loader.exec_module(semantics)
REGISTRY_DIR = semantics.DEFAULT_REGISTRIES
SOURCE_ROOT = semantics.DEFAULT_SOURCE
WORK = semantics.DEFAULT_WORK/'tests'

def registries():
    return {p.name:semantics.load(p) for p in sorted(REGISTRY_DIR.glob('*.json'))}

def all_entities(regs):
    return [e for r in regs.values() for e in r['entities']]

class SemanticRegistry(unittest.TestCase):
    def test_continued_unused_directive_has_no_phantom_c_body(self):
        text='#define UNUSED(p) \\\n    do { \\\n        phantom(p); \\\n    } while (0)\nextern int real;\n'
        cleaned=semantics.clean(text)
        self.assertNotIn('phantom',cleaned)
        self.assertEqual(cleaned.count('\n'),text.count('\n'))
        ast=semantics.c_parser.CParser().parse(cleaned)
        self.assertEqual(set(semantics.global_declarations(ast)),{'real'})

    def test_function_typedef_extern_is_not_a_data_global(self):
        ast=semantics.c_parser.CParser().parse('typedef void routine_t(int); typedef routine_t alias_t; extern routine_t direct; extern alias_t indirect; extern routine_t *dispatch; extern int value;')
        self.assertEqual(set(semantics.global_declarations(ast)),{'dispatch','value'})

    def test_field_rename_does_not_rewrite_unrelated_local(self):
        entity=dict(kind='field',current_name='field_0',proposed_name='unprojected_x')
        self.assertEqual(semantics.reverse_names('unprojected_x + entry->unprojected_x + value.unprojected_x',[entity]),
                         'unprojected_x + entry->field_0 + value.field_0')

    def test_render_compatible_type_view_is_exact(self):
        regs=registries()
        if 'affiche.json' not in regs:self.skipTest('Rendering registry not selected')
        regs['affiche.json']['scope']['shared_type_views']['pix_gerbe_t']['omitted_fields']=[]
        self.assertTrue(any('affiche.json: shared type field coverage drift: pix_gerbe_t' in s for s in semantics.check_collection(regs,SOURCE_ROOT)))

    def test_render_type_view_requires_evidence(self):
        reg=copy.deepcopy(registries()['types.json'])
        reg['scope']['shared_type_views']={'pix_gerbe_t':dict(omitted_fields=['words'],size_bytes=642)}
        self.assertTrue(any('invalid shared type view' in s for s in semantics.validate_schema(reg)))

    def test_render_type_view_size_is_compiled(self):
        regs=registries()
        if 'affiche.json' not in regs:self.skipTest('Rendering registry not selected')
        reg=regs['affiche.json'];reg['scope']['shared_type_views']['pix_gerbe_t']['size_bytes']+=1
        self.assertTrue(semantics.check_layout(reg,WORK/'negative_render_view_size',SOURCE_ROOT))

    def test_missing_scope_kind_reports_schema_error(self):
        reg=copy.deepcopy(registries()['types.json'])
        del reg['scope']['kind']
        self.assertIn('invalid scope kind',semantics.validate_schema(reg))

    def test_loader_typedef_macro_drift_is_rejected(self):
        regs=registries()
        if 'load.json' not in regs:self.skipTest('Loader registry not selected')
        scope=regs['load.json']['scope']
        text=semantics.source_path(SOURCE_ROOT,'load.c').read_text(encoding='latin-1')
        changed=text.replace('#define archive_header_t archive_header_local_t',
                             '#define archive_header_t pcx_header_local_t')
        self.assertNotEqual(text,changed)
        WORK.mkdir(parents=True,exist_ok=True)
        with tempfile.TemporaryDirectory(dir=WORK) as d:
            (Path(d)/'load.c').write_text(changed,encoding='latin-1')
            with self.assertRaisesRegex(ValueError,'scoped typedef macro drift'):
                semantics.parse_source(Path(d),scope)

    def test_loader_local_header_offset_is_enforced(self):
        reg=copy.deepcopy(registries()['types.json'])
        fields=[e for e in reg['entities'] if e['kind']=='field' and e['struct']=='pcx_header_local_t' and e['current_name']=='bytes_per_line']
        if not fields:self.skipTest('Loader types not selected')
        fields[0]['offset']+=1
        self.assertTrue(semantics.check_layout(reg,WORK/'negative_loader_header_offset',SOURCE_ROOT))

    def test_shared_global_checked_in_loader_despite_earlier_owner(self):
        regs=registries()
        if 'load.json' not in regs:self.skipTest('Loader registry not selected')
        e=next(e for r in regs.values() for e in r['entities'] if e['id']=='global:level')
        e['context_declarators']['load.c']='obj_t *$'
        errors=semantics.check_collection(regs,SOURCE_ROOT)
        self.assertTrue(any('load.json: shared global declaration drift: level' in s for s in errors))

    def test_context_declarator_cannot_name_an_unscoped_unit(self):
        regs=registries()
        e=next(e for r in regs.values() for e in r['entities'] if e['id']=='global:level')
        e.setdefault('context_declarators',{})['missing_test_unit.c']='level_t $'
        self.assertTrue(any('contextual declarator has no scoped unit' in s for s in semantics.check_collection(regs,SOURCE_ROOT)))

    def test_loader_xor_change_requires_new_evidence(self):
        regs=registries()
        if 'load.json' not in regs:self.skipTest('Loader registry not selected')
        text=semantics.source_path(SOURCE_ROOT,'load.c').read_text(encoding='latin-1')
        changed=text.replace('^= 0x8F;', '^= 0x8E;',1)
        self.assertNotEqual(text,changed)
        WORK.mkdir(parents=True,exist_ok=True)
        with tempfile.TemporaryDirectory(dir=WORK) as d:
            (Path(d)/'load.c').write_text(changed,encoding='latin-1')
            errors=semantics.check_source(regs['load.json'],Path(d),shared_entities=all_entities(regs))
        self.assertTrue(any('function:load_world: function body drift' in s for s in errors))

    def test_registries_match_code(self):
        regs = registries()
        self.assertTrue(regs)
        self.assertEqual(semantics.check_collection(regs,SOURCE_ROOT),[])
        for name,reg in regs.items():
            with self.subTest(registry=name):
                self.assertEqual(semantics.check_layout(reg,WORK/Path(name).stem,SOURCE_ROOT),[])

    def test_wrong_field_offset_is_rejected(self):
        # Preserve the pilot's negative control, now explicitly owned by types.json.
        reg = copy.deepcopy(registries()['types.json'])
        field = next(e for e in reg['entities'] if e['kind']=='field' and not e.get('bit_width') and e['offset']>0)
        field['offset'] += 1
        self.assertTrue(semantics.check_layout(reg,WORK/'negative_offset',SOURCE_ROOT))

    def test_wrong_bit_mask_is_rejected(self):
        reg = copy.deepcopy(registries()['types.json'])
        field = next(e for e in reg['entities'] if e['kind']=='field' and e.get('bit_width')==1)
        field['bit_mask'] ^= 0x80
        self.assertTrue(semantics.check_layout(reg,WORK/'negative_mask',SOURCE_ROOT))

    def test_unknown_meanings_keep_neutral_names(self):
        for e in all_entities(registries()):
            if e['evidence_level'] in ('UNKNOWN','HYPOTHESIS'):
                self.assertFalse(e.get('previous_name'),e['id'])
                self.assertFalse(e.get('proposed_name'),e['id'])

    def test_unknown_rename_is_rejected_by_schema(self):
        reg = copy.deepcopy(registries()['moteur.json'])
        e = next(e for e in reg['entities'] if e['kind']=='local')
        e['evidence_level']='UNKNOWN'; e['proposed_name']='unsupported_name'
        self.assertTrue(any('rename lacks' in s for s in semantics.validate_schema(reg)))

    def test_entities_have_unique_ownership(self):
        regs = registries(); entities = all_entities(regs)
        self.assertEqual(len(entities),len({e['id'] for e in entities}))
        fields=[(e['struct'],e['current_name']) for e in entities if e['kind']=='field']
        self.assertEqual(len(fields),len(set(fields)))
        globals_=[e['current_name'] for e in entities if e['kind']=='global']
        self.assertEqual(len(globals_),len(set(globals_)))
        for name,reg in regs.items():
            if name!='types.json':
                self.assertFalse(any(e['kind'] in ('type','field') for e in reg['entities']),name)

    def test_duplicate_shared_field_is_rejected(self):
        regs=registries();reg=regs['moteur.json']
        field=next(e for e in regs['types.json']['entities'] if e['kind']=='field')
        reg['entities'].append(copy.deepcopy(field))
        self.assertTrue(any('duplicate cross-registry entity' in s for s in semantics.check_collection(regs,SOURCE_ROOT)))

    def test_repeated_history_typedef_drift_is_rejected(self):
        regs=registries()
        if 'ray.json' not in regs: self.skipTest('Rayman registry not selected')
        text=semantics.source_path(SOURCE_ROOT,'ray.c').read_text(encoding='latin-1')
        changed=text.replace('s16 x_pos,y_pos,poing_x_pos,poing_y_pos;',
                             'u16 x_pos,y_pos,poing_x_pos,poing_y_pos;')
        self.assertNotEqual(text,changed)
        WORK.mkdir(parents=True,exist_ok=True)
        with tempfile.TemporaryDirectory(dir=WORK) as d:
            (Path(d)/'ray.c').write_text(changed,encoding='latin-1')
            errors=semantics.check_collection(regs,Path(d))
        self.assertTrue(any('ray.json: shared declaration drift: ray_stack_t.x_pos' in s for s in errors))

    def test_repeated_callback_abi_drift_is_rejected(self):
        regs=registries()
        if 'collision1.json' not in regs: self.skipTest('Collision registry not selected')
        text=semantics.source_path(SOURCE_ROOT,'collision1.c').read_text(encoding='latin-1')
        changed=text.replace('void (*poing_collision)(obj_t *obj, s16 sprite);',
                             'void (*poing_collision)(obj_t *obj, u16 sprite);')
        self.assertNotEqual(text,changed)
        WORK.mkdir(parents=True,exist_ok=True)
        with tempfile.TemporaryDirectory(dir=WORK) as d:
            (Path(d)/'collision1.c').write_text(changed,encoding='latin-1')
            errors=semantics.check_collection(regs,Path(d))
        self.assertTrue(any('collision1.json: shared declaration drift: obj_procs_t.poing_collision' in s for s in errors))

    def test_unknown_rayman_state_cannot_receive_a_name(self):
        regs=registries()
        if 'ray.json' not in regs: self.skipTest('Rayman registry not selected')
        reg=copy.deepcopy(regs['ray.json'])
        state=next(s for s in reg['ray_state_machine']['substates'] if s['evidence_level']=='UNKNOWN')
        state['name']='unsupported_action'
        self.assertTrue(any('state name lacks PROVEN/STRONG' in s for s in semantics.validate_schema(reg)))

    def test_cross_registry_global_reference_is_required(self):
        regs=registries()
        regs['moteur.json']['scope']['globals'].append('global:missing_test_global')
        errors=semantics.check_collection(regs,SOURCE_ROOT)
        self.assertTrue(any('unresolved global entity' in s for s in errors))

    def test_moteur_function_body_is_enforced(self):
        regs=registries();reg=regs['moteur.json']
        e=next(e for e in reg['entities'] if e['kind']=='function' and e['current_name']=='horloges')
        e['source_body_sha256']='0'*64
        errors=semantics.check_source(reg,SOURCE_ROOT,shared_entities=all_entities(regs))
        self.assertTrue(any('function body drift' in s for s in errors))

    def test_statement_macro_definition_affects_body(self):
        scope=registries()['moteur.json']['scope']
        text=semantics.source_path(SOURCE_ROOT,scope['source']).read_text(encoding='latin-1')
        changed=text.replace('#define MAX_2(x, y) if (x < y) x = y;', '#define MAX_2(x, y) if (x < y) x = y + 1;')
        self.assertNotEqual(changed,text)
        self.assertNotEqual(semantics.expand_statement_macros(text,scope['statement_macros']),semantics.expand_statement_macros(changed,scope['statement_macros']))

    def test_other_unit_evidence_drift_is_reported_separately(self):
        regs=registries();reg=regs['moteur.json']
        path=next(iter(reg['evidence_source_fingerprints']))
        reg['evidence_source_fingerprints'][path]='0'*64
        self.assertEqual(semantics.check_source(reg,SOURCE_ROOT,shared_entities=all_entities(regs)),[])
        drift=semantics.check_evidence(reg,SOURCE_ROOT)
        self.assertTrue(any(path in s for s in drift))

    def test_pruned_artifact_is_not_drift_but_a_missing_hashed_one_is(self):
        reg=copy.deepcopy(registries()['moteur.json'])
        reg['evidence_artifact_fingerprints']={'pruned.json':None,'vanished.json':'0'*64}
        drift=semantics.check_evidence(reg,SOURCE_ROOT,artifact_root=WORK/'no_such_artifacts')
        self.assertFalse(any('pruned.json' in s for s in drift))
        self.assertTrue(any('vanished.json' in s for s in drift))

    def test_cli_evidence_drift_does_not_fail_enforcement(self):
        regs=registries();WORK.mkdir(parents=True,exist_ok=True)
        reg=regs['moteur.json'];path=next(iter(reg['evidence_source_fingerprints']))
        reg['evidence_source_fingerprints'][path]='0'*64
        with tempfile.TemporaryDirectory(dir=WORK) as d:
            for name,r in regs.items():(Path(d)/name).write_text(json.dumps(r),encoding='utf-8')
            output=io.StringIO()
            with contextlib.redirect_stdout(output):
                rc=semantics.main(['check','--registry-dir',d,'--source-root',str(SOURCE_ROOT),'--out',str(Path(d)/'layout'),'--evidence'])
            self.assertEqual(rc,0)
            self.assertIn('DRIFT:',output.getvalue())
            self.assertIn('PASS:',output.getvalue())

    def test_shared_declaration_type_drift_is_enforced(self):
        regs=registries();reg=regs['types.json']
        e=next(e for e in reg['entities'] if e['kind']=='field' and e['struct']=='obj_t' and e['current_name']=='speed_x')
        e['declarator']='u16 $';e['c_type']='u16';e['signedness']='unsigned'
        errors=semantics.check_source(reg,SOURCE_ROOT,shared_entities=all_entities(regs))
        self.assertTrue(any('field declaration type drift' in s for s in errors))

    def test_shared_global_declarations_are_checked_in_each_context(self):
        regs=registries();reg=regs['globals.json']
        e=next(e for e in reg['entities'] if e['current_name']=='horloge')
        e['declarator']='u8 $[24]';e['c_type']='u8 [24]'
        errors=semantics.check_source(reg,SOURCE_ROOT,shared_entities=all_entities(regs))
        self.assertTrue(any('basic.c' in s and 'global type drift' in s for s in errors))
        self.assertTrue(any('moteur.c' in s and 'global type drift' in s for s in errors))

    def test_generate_covers_every_registry(self):
        regs=registries();WORK.mkdir(parents=True,exist_ok=True)
        with tempfile.TemporaryDirectory(dir=WORK) as d:
            semantics.generate(regs,d)
            md=(Path(d)/'SEMANTICS.md').read_text(encoding='utf-8')
            for name,reg in regs.items():
                self.assertIn(name+' / '+reg['entities'][0]['id'],md)
            header=(Path(d)/'semantics_dos.h').read_text(encoding='utf-8')
            self.assertEqual(header.count('#define DOS_OBJ_T_SIZE '),1)
            self.assertIn('DOS_SAVE_STATE_T_SIZE',header)

    def test_refresh_uses_each_registry_scope(self):
        regs=registries();WORK.mkdir(parents=True,exist_ok=True)
        with tempfile.TemporaryDirectory(dir=WORK) as d:
            for name in ('basic.json','moteur.json','types.json','globals.json'):
                reg=copy.deepcopy(regs[name])
                functions=[e for e in reg['entities'] if e['kind']=='function']
                if functions:functions[0]['source_body_sha256']='0'*64
                with contextlib.redirect_stdout(io.StringIO()):
                    changed=semantics.refresh(reg,Path(d)/name,SOURCE_ROOT,shared_entities=all_entities(regs))
                if functions:self.assertIn(functions[0]['id'],changed)
                self.assertEqual(semantics.check_source(reg,SOURCE_ROOT,shared_entities=all_entities(regs)),[])

if __name__=='__main__':unittest.main()
