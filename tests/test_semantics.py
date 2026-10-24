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
