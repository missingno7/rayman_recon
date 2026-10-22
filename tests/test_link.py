"""Fast contract tests for the reviewed natural-link tools."""
import json
from pathlib import Path
import re
import sys
import unittest

HERE = Path(__file__).resolve().parent
if (HERE / 'link.py').is_file() and (HERE / 'manifest.py').is_file():
    sys.path.insert(0, str(HERE))
else:
    PROJECT_ROOT = next(p for p in HERE.parents if (p / 'config/units.json').is_file())
    sys.path.insert(0, str(PROJECT_ROOT / 'tools'))
import link
import manifest

ROOT = link.ROOT


class ManifestSchemaTests(unittest.TestCase):
    def test_manifest_schema_accepts_a_small_valid_inventory(self):
        document = {
            'schema': 1,
            'oracle': 'gog_v121_us',
            'oracle_sha256': '0' * 64,
            'proof_scope': 'fixture',
            'extents_half_open': True,
            'inputs': {},
            'pinned_libraries': [],
            'contributions': [{
                'id': 'game:fixture',
                'kind': 'game_code_candidate',
                'extents': [{'object': 1, 'start': 16, 'end': 32, 'size': 16,
                             'segment': '_TEXT'}],
            }],
            'class_contract': [],
            'class_orders': {},
            'order_edges': {},
            'candidate_topological_order': [],
            'order_ties': [],
            'order_cycle_members': [],
            'reconstruction_slices': [],
            'data_objects': [],
            'const_runs': [],
            'bss_rank_blocks': [],
            'relative_order_checks': [],
            'string_order_inversions': [],
            'runtime_conflicting_votes': [],
            'gaps': {},
            'central_TUs': {},
            'excluded_hmi_reconstruction': {},
            'open': [],
        }
        self.assertIs(manifest.validate_manifest(document), document)

    def test_generated_manifest_schema_when_present(self):
        path = ROOT / 'build/link/manifest.json'
        if path.is_file():
            document = json.loads(path.read_text(encoding='utf-8'))
            self.assertIs(manifest.validate_manifest(document), document)

    def test_manifest_schema_rejects_inconsistent_extent(self):
        document = {
            'schema': 1, 'oracle': 'gog_v121_us', 'oracle_sha256': '0' * 64,
            'proof_scope': 'fixture', 'extents_half_open': True,
            'inputs': {}, 'contributions': [{
                'id': 'broken', 'extents': [{'object': 2, 'start': 8, 'end': 12,
                                               'size': 8, 'segment': '_DATA'}],
            }], 'pinned_libraries': [], 'class_contract': [], 'class_orders': {},
            'order_edges': {}, 'candidate_topological_order': [], 'order_ties': [],
            'order_cycle_members': [], 'reconstruction_slices': [], 'data_objects': [],
            'const_runs': [], 'bss_rank_blocks': [], 'relative_order_checks': [],
            'string_order_inversions': [], 'runtime_conflicting_votes': [], 'gaps': {},
            'central_TUs': {},
            'excluded_hmi_reconstruction': {},
            'open': [],
        }
        with self.assertRaisesRegex(ValueError, 'inconsistent extent size'):
            manifest.validate_manifest(document)


class InputRecipeTests(unittest.TestCase):
    def test_asset_list_has_no_symbol_defined_by_a_unit(self):
        units = json.loads((ROOT / 'config/units.json').read_text(encoding='utf-8'))['units']
        names = {name for name, *_ in link.asset_specs()}
        self.assertNotIn('PalFire', names)
        for symbol in names:
            c_definition = re.compile(
                rf'(?m)^\s*(?!extern\b)(?:static\s+)?(?:const\s+)?'
                rf'(?:rgb_t|def_bande_t|u8|s8|u16|s16|u32|s32|char)\s+'
                rf'{re.escape(symbol)}\b\s*(?:\[[^\]]*\]\s*)*(?:=|;)'
            )
            asm_definition = re.compile(
                rf'(?im)^\s*(?:public|global)\s+.*\b{re.escape(symbol)}\b|'
                rf'^\s*{re.escape(symbol)}\s+(?:db|dw|dd|label|proc)\b'
            )
            for unit in units.values():
                source = ROOT / unit['source']
                if not source.is_file():
                    continue
                text = source.read_text(encoding='latin-1')
                self.assertIsNone(c_definition.search(text), f'{symbol} C definition in {source}')
                self.assertIsNone(asm_definition.search(text), f'{symbol} ASM definition in {source}')

    def test_response_file_uses_only_declared_fixture_inputs(self):
        response = link.response_file(
            'option quiet', ['alpha', 'beta'], ['C:/fixture/CLIB.LIB'],
            [{'source': '_old', 'target': '_new'}], 'fixture',
        )
        lines = response.splitlines()
        self.assertLess(lines.index('file objects/alpha.obj'), lines.index('file objects/beta.obj'))
        self.assertIn('library C:/fixture/CLIB.LIB', lines)
        self.assertIn('alias _old=_new', lines)
        self.assertIn('name fixture.exe', lines)
        self.assertFalse(any(re.search(r'(?i)\b(order|pad|padding|offset)\b', line) for line in lines))


if __name__ == '__main__':
    unittest.main()
