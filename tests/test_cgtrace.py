"""Fast portable parser/alignment controls; live debugging is explicit opt-in."""
import copy
import json
import os
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch
from types import SimpleNamespace
import subprocess

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
ROOT = next(p for p in HERE.parents if (p / 'config/units.json').exists())
PROPOSAL = HERE.parent / 'tools'
sys.path.insert(0, str(PROPOSAL if (PROPOSAL/'cgtrace.py').exists() else ROOT/'tools'))
import cgtrace
import explain

FIX = json.loads((HERE / 'fixtures/cgtrace.json').read_text())
TMP = Path(os.environ.get('CGTRACE_TEST_TMP', str(ROOT/'build/cgtrace_tests')))


def instructions(code):
    return explain.census.insns(bytes.fromhex(code))


def minimal_trace():
    before, after = copy.deepcopy(FIX['predicate_before']), copy.deepcopy(FIX['predicate_after'])
    return dict(phases={'after_LdStCompress': [after], 'post_start': [before], 'regalloc': [before]},
                score=[dict(event='change_ins', success=True, instruction=before, after=after)],
                rover=[], colours=[], conflict_sorts=[], home_sorts=[], events=[])


class ParsingTests(unittest.TestCase):
    def test_definitions_skip_literals_comments_and_macros(self):
        self.assertEqual(cgtrace.definitions(FIX['definition_source']), ['first', 'second'])

    def test_unattributable_functions_refused(self):
        with self.assertRaisesRegex(ValueError, 'cannot attribute'):
            cgtrace.group_events(['a', 'b'], [dict(event='homes_return')], ['a'])

    def test_incomplete_phases_refused(self):
        with self.assertRaisesRegex(ValueError, 'phase'):
            cgtrace.group_events(['a'], [dict(event='homes_return')], ['a'])

    def test_per_function_phases_and_request_indices(self):
        phases = ['regalloc','post_start','after_LdStAlloc','after_Score','after_LdStCompress']
        events = [dict(event='payload_loaded')]
        for _ in range(2):
            events += [dict(event='phase', stage='phase_'+s, graph=[]) for s in phases]
            events += [dict(event='rover',kind=k,chosen=1) for k in (0,2,1)]
            events += [dict(event='homes_return')]
        groups = cgtrace.group_events(['a','b'], events, ['a','b'])
        for name in ('a','b'):
            trace = cgtrace.function_trace(name, groups[name])
            self.assertEqual([r['request_index'] for r in trace['rover']], [0,0,1])
            self.assertEqual(list(trace['phases']), phases)

    def test_focused_events_need_every_home_pass_and_one_window(self):
        phases = ['regalloc','post_start','after_LdStAlloc','after_Score','after_LdStCompress']
        rows = [dict(event='payload_loaded')] + [dict(event='phase', stage='phase_'+s, graph=[]) for s in phases]
        rows += [dict(event='rover', kind=2, chosen=1), dict(event='homes_return')]
        obs = SimpleNamespace(homes_seen=3, events=rows)
        groups = cgtrace.focused_events(['a','b','c'], 'b', obs, ['a','b'])
        self.assertEqual(list(groups), ['b'])
        self.assertEqual(list(cgtrace.focused_events(['a','b','B'], 'b', obs, ['a'])), ['b'])
        self.assertEqual(groups['b'][-2]['request_index'], 0)
        with self.assertRaisesRegex(ValueError, 'cannot attribute'):
            cgtrace.focused_events(['a','b','c'], 'b', SimpleNamespace(homes_seen=2, events=rows), ['a'])
        with self.assertRaisesRegex(ValueError, 'outside the focused'):
            cgtrace.focused_events(['a','b','c'], 'b', SimpleNamespace(homes_seen=3, events=rows + rows[-1:]), ['a'])

    def test_trace_key_follows_included_headers(self):
        TMP.mkdir(parents=True, exist_ok=True)
        with tempfile.TemporaryDirectory(dir=TMP) as d:
            d = Path(d)
            (d / 'unit.c').write_text('#include "local.h"\nint f(void) { return X; }\n')
            (d / 'local.h').write_text('#include "inner.h"\n')
            (d / 'inner.h').write_text('#define X 1\n')
            self.assertEqual(explain.included_files(d / 'unit.c'), sorted([(d / 'local.h').resolve(),
                                                                         (d / 'inner.h').resolve()]))
            before = explain.trace_key(d / 'unit.c', 'game', 'f')
            (d / 'inner.h').write_text('#define X 2\n')
            self.assertNotEqual(before, explain.trace_key(d / 'unit.c', 'game', 'f'))

    def test_incomplete_decision_refused(self):
        events = [dict(event='phase',stage='phase_'+s,graph=[]) for s in
                  ['regalloc','post_start','after_LdStAlloc','after_Score','after_LdStCompress']]
        events += [dict(event='rover'), dict(event='homes_return')]
        with self.assertRaisesRegex(ValueError, 'incomplete rover'):
            cgtrace.group_events(['a'],events,['a'])


class AlignmentTests(unittest.TestCase):
    def test_cmp_test_have_same_alignment_key(self):
        case = FIX['alignment'][0]
        pairs = explain.align(instructions(case['candidate']), instructions(case['original']))
        self.assertEqual(len(pairs),3)
        self.assertEqual([(a.mnemonic,b.mnemonic) for a,b,_ in pairs][1], ('test','cmp'))

    def test_stack_offset_does_not_break_alignment(self):
        case = FIX['alignment'][1]
        pairs = explain.align(instructions(case['candidate']), instructions(case['original']))
        self.assertTrue(all(tag=='shape' for a,b,tag in pairs))

    def test_insertions_retained(self):
        case = FIX['alignment'][3]
        pairs = explain.align(instructions(case['candidate']), instructions(case['original']))
        self.assertEqual(sum(a is None for a,b,_ in pairs),1)
        self.assertEqual(pairs[-1][0].mnemonic,'ret')

    def test_selection_mechanisms(self):
        for index, expected in ((0,'TEST/CMP'),(2,'SHL/LEA')):
            case = FIX['alignment'][index]
            a,b = instructions(case['candidate']),instructions(case['original'])
            pair = (a[1],b[1]) if index==0 else (a[0],b[0])
            findings = explain.describe_difference(*pair,minimal_trace())
            self.assertTrue(any(f['mechanism']=='selection' and expected in f['detail'] for f in findings))

    def test_ambiguous_ir_candidates_retained(self):
        trace = minimal_trace()
        node = copy.deepcopy(trace['phases']['after_LdStCompress'][0])
        node['addr'] = 101
        ins = instructions('85c9')[0]
        self.assertEqual(len(explain.associate(ins,[node,trace['phases']['after_LdStCompress'][0]])),2)

    def test_score_explains_changed_register(self):
        f = explain.describe_difference(instructions('85c9')[0],instructions('85c0')[0],minimal_trace())
        self.assertEqual(f[0]['mechanism'],'Score substitution')

    def test_unrelated_score_change_not_cause_of_stack_offset(self):
        case = FIX['alignment'][1]
        f = explain.describe_difference(instructions(case['candidate'])[0],instructions(case['original'])[0],minimal_trace())
        self.assertFalse(any(r['mechanism']=='Score substitution' for r in f))

    def test_savings_tie_reports_affinity_and_peer(self):
        trace=minimal_trace()
        memory=dict(cls=4,size=4,base=dict(cls=3,reg=0x1000003,size=4),index=None,scale=0,displacement=32)
        temp=dict(cls=2,id=7,size=4,offset=0)
        after=dict(addr=42,op=38,operands=[memory],result=dict(cls=3,reg=0x200000c,size=4))
        trace['phases']['after_LdStCompress']=[after]
        trace['phases']['regalloc']=[dict(after,result=temp)]
        trace['colours']=[dict(name=dict(id=7,conflict_savings=122),chosen=0x200000c,given=0,
                               scores=[dict(reg=0x200000c,score=0),dict(reg=0x4000030,score=0)])]
        trace['conflict_sorts']=[dict(output=[dict(savings=122,name=dict(id=8))])]
        f=explain.describe_difference(instructions('8b5820')[0],instructions('8b4820')[0],trace)
        tie=next(r for r in f if r['mechanism']=='colour tie')
        self.assertIn('savings=122',tie['detail'])
        self.assertIn('affinity=',tie['detail'])
        self.assertIn('peers=[8]',tie['detail'])
        self.assertEqual(tie['ir'][0]['addr'],42)

    def test_home_assignment_links_stack_access(self):
        trace=minimal_trace()
        node=dict(addr=43,op=38,operands=[dict(cls=2,id=3,size=2,offset=0)],result=dict(cls=3,reg=3,size=2))
        trace['phases']['after_LdStCompress']=[node]
        trace['events']=[dict(event='homes_return',names=[dict(cls=2,id=3,size=2,flags=1,location=0xfffffffc)])]
        trace['_frame_size']=8
        f=explain.describe_difference(instructions('668b442404')[0],instructions('668b442408')[0],trace)
        home=next(r for r in f if r['mechanism']=='home-sort order')
        self.assertIn('(3, 2, -4)',home['detail'])
        self.assertEqual(home['ir'][0]['addr'],43)


class SafetyAndReplayTests(unittest.TestCase):
    def test_rover_advances_before_testing_and_skips_exclusions(self):
        for row in FIX['rover']:
            self.assertEqual(explain.replay_rover(row,row['initial_index']),row['chosen'])

    def test_frozen_onsets_remain_ambiguous_without_evidence(self):
        row = FIX['rover'][0]
        rows = [dict(row, initial_index=i) for i in range(4)]
        fits = explain.rover_constraints(rows,{2:8})
        self.assertGreater(len(fits),1)
        self.assertIn(dict(request_index=0,delta=1),fits)

    def test_stream_insert_reports_offset_from_first_request(self):
        a = dict(FIX['rover'][0],instruction=FIX['predicate_before'])
        b = dict(a,instruction=dict(FIX['predicate_after'],op=38))
        changes = explain.compare_rovers([a],[b,a])
        self.assertIn(dict(width=0,change='request-index offset',starts_at_request=0,delta=1,matched_requests=1),changes)

    def test_identity_rejection_including_empty_output(self):
        TMP.mkdir(parents=True,exist_ok=True)
        with tempfile.TemporaryDirectory(dir=TMP) as work:
            a,b = Path(work)/'a',Path(work)/'b'
            for x,y in ((b'',b''),(b'obj1',b'obj2')):
                a.write_bytes(x); b.write_bytes(y)
                with self.assertRaisesRegex(RuntimeError,'REFUSED'):
                    cgtrace.require_identity(a,b)
            a.write_bytes(b'object'); b.write_bytes(b'object')
            self.assertEqual(len(cgtrace.require_identity(a,b)),64)

    def test_breakpoint_write_guard(self):
        observer = cgtrace.Observer(b'prefix')
        with self.assertRaisesRegex(ValueError,'breakpoint byte'):
            observer.write(0x1000,b'\x90')
        observer.bps[0x1000] = (b'\x90','probe')
        with self.assertRaises(ValueError):
            observer.write(0x1000,b'\x90\x90')

@unittest.skipUnless(os.name=='nt' and cgtrace.C.sizeof(cgtrace.C.c_void_p)==8 and os.environ.get('CGTRACE_RUN_DEBUGGER')=='1',
                     'set CGTRACE_RUN_DEBUGGER=1 on 64-bit Windows with pinned wc106')
class DebuggerTests(unittest.TestCase):
    def test_live_identity_and_scoped_wrapper(self):
        try:
            cgtrace.wcc.install('wc106')
        except (OSError,SystemExit) as exc:
            self.skipTest(str(exc))
        TMP.mkdir(parents=True,exist_ok=True)
        original = cgtrace.wcc.BUILD,cgtrace.wcc.subprocess
        with tempfile.TemporaryDirectory(dir=TMP) as work:
            source = Path(work)/'probe.c'
            source.write_text('int plus(int x) { return x+1; }\n')
            result = cgtrace.trace(source,Path(work)/'trace')
            self.assertTrue(result['whole_omf_equal'])
            self.assertEqual(result['functions'],['plus'])
            self.assertEqual(original,(cgtrace.wcc.BUILD,cgtrace.wcc.subprocess))

    def test_unequal_object_refuses_publication_and_restores_wrapper(self):
        try:
            cgtrace.wcc.install('wc106')
        except (OSError,SystemExit) as exc:
            self.skipTest(str(exc))
        def fake(content):
            def run(*args, **kw):
                cmd = args[-1]  # bound Observer method receives self,cmd
                Path(cmd[-1].removeprefix('-fo=')).write_bytes(content)
                return subprocess.CompletedProcess(cmd,0,'','')
            return run
        TMP.mkdir(parents=True,exist_ok=True)
        original = cgtrace.wcc.BUILD,cgtrace.wcc.subprocess
        with tempfile.TemporaryDirectory(dir=TMP) as work:
            source,out = Path(work)/'probe.c',Path(work)/'trace'
            source.write_text('int plus(int x) { return x+1; }\n')
            with patch.object(cgtrace.wcc,'subprocess',SimpleNamespace(run=fake(b'normal'))):
                with patch.object(cgtrace.Observer,'run',fake(b'observed')):
                    with self.assertRaisesRegex(RuntimeError,'REFUSED'):
                        cgtrace.trace(source,out)
                self.assertFalse((out/'manifest.json').exists())
                self.assertFalse((out/'observed.obj').exists())
                self.assertEqual(list(out.glob('plus.json')),[])
            self.assertEqual(original,(cgtrace.wcc.BUILD,cgtrace.wcc.subprocess))


if __name__=='__main__':
    unittest.main()
