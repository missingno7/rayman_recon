"""Offline controls for the recovered 10.6 scheduler explanation."""
import json
from pathlib import Path
import sys
import unittest

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
sys.path.insert(0, str(ROOT / 'tools'))
import cgtrace
sys.path.insert(0, str(ROOT / 'tools'))
import explain
import scheduler

FIX = json.loads((HERE / 'fixtures/scheduler.json').read_text())


class SchedulerModelTests(unittest.TestCase):
    def test_legal_dos_reorder_reports_selected_priority(self):
        row = FIX['legal']
        result = scheduler.classify_order(row['block'], row['candidate_order'], row['dos_order'])
        self.assertEqual(result['classification'], 'legal')
        self.assertEqual(result['decisions'][0]['rule'], 'backend instruction ID')

    def test_hidden_dependancy_blocks_dos_order(self):
        row = FIX['hidden']
        result = scheduler.classify_order(row['block'], row['candidate_order'], row['dos_order'])
        self.assertEqual(result['classification'], 'illegal')
        self.assertEqual(result['violations'][0]['rule'], 'HiddenDependancy')

    def test_edge_rules_name_alias_call_and_cc(self):
        for case in FIX['edge_rules']:
            model = scheduler.Model(case['nodes'])
            deps, rules = model.edges_with_rules()
            pair = (case['nodes'][1]['addr'], case['nodes'][0]['addr'])
            self.assertIn(pair[1], deps[pair[0]], case['name'])
            self.assertEqual(rules[pair]['rule'], case['expected'], case['name'])

    def test_explain_maps_machine_reorder_without_compiler(self):
        row = FIX['legal']
        candidate = explain.census.insns(bytes.fromhex(row['candidate']))
        original = explain.census.insns(bytes.fromhex(row['dos']))
        trace = dict(events=[], phases={'after_LdStCompress': []}, scheduler=[row['block']])
        result = explain.explain_schedule(candidate, original, trace)
        self.assertEqual(len(result), 1)
        self.assertEqual(result[0]['classification'], 'legal')
        self.assertEqual(result[0]['decisions'][0]['rule'], 'backend instruction ID')

    def test_function_trace_keeps_scheduler_blocks_out_of_legacy_events(self):
        events = [dict(event='scheduler_block', block=7, dag=[], rounds=[])]
        trace = cgtrace.function_trace('f', events)
        self.assertEqual(trace['scheduler'], [dict(block=7, dag=[], rounds=[])])
        self.assertEqual(trace['events'], [])


if __name__ == '__main__':
    unittest.main()
