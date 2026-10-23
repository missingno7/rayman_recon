"""Safety boundaries, deterministic search, accounting/resume, and hit gate.

Default tests need pycparser + capstone but no historical tool execution.
PERMUTE_TEST_TMP may redirect temporary files into a worker's allowed directory.
The actual compiler/pipeline integration is measured by permute_bench.py.
"""
import copy
import json
import os
import random
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "tools"))
import permute
from permute_backend import assess, instruction_distance
from permute_source import (AGGRESSIVE, C, Facts, Model, Refusal, body_bounds, emit,
                            mutate, mutations, sanitize)

FIXTURE = (HERE / "fixtures/permute_small.c").read_text(encoding="utf-8")


def fake_result(status="NEAR_MATCH", strict=("neighbour", "tail"), diffs=5):
    score = {"total": diffs + 2, "byte_diffs": diffs, "instruction_diffs": 1}
    return {"target": {"status": status, "function": "pilot", "first_diff": 2},
            "strict": list(strict), "score": score, "shape": [], "results": [], "segments": []}


def fake_preprocess(root, source, output, profile):
    text = source.read_text(encoding="utf-8")
    output.write_text(text, encoding="utf-8")
    return text


class SourceTests(unittest.TestCase):
    def setUp(self):
        self.model = Model.parse(FIXTURE, FIXTURE, "pilot")

    def test_nested_if_keeps_its_else(self):
        # pilot_mine 2026-10-23: an unbraced nested if in the true arm of an if/else captured the outer else.
        text = "int f(int a, int b) { int r; r = 0; if (a) { if (b) r = 1; } else r = 2; return r; }\n"
        m = Model.parse(text, text, "f")
        fn = copy.deepcopy(m.target())
        outer = fn.body.block_items[2]
        outer.iftrue = outer.iftrue.block_items[0]  # what a branch rewrite can produce
        body = m.unit(fn)
        again = Model.parse(body, body, "f").target().body.block_items[2]
        self.assertIsNotNone(again.iffalse)
        self.assertIsNone(again.iftrue.block_items[0].iffalse)

    def test_scope_and_signature_preserved(self):
        a, b = body_bounds(FIXTURE, "pilot")
        fn, _ = mutate(self.model, self.model.target(), random.Random(4), "aggressive")
        result = self.model.unit(fn)
        c, d = body_bounds(result, "pilot")
        self.assertEqual(result[:c], FIXTURE[:a])
        self.assertEqual(result[d:], FIXTURE[b:])
        self.assertIn("#pragma aux helper", result)

    def test_lexical_braces_and_prototype(self):
        text = 'int f(int); const char *s="f(x) { }"; /* } */ int f(int x) {return x;} int g(void){return 0;}'
        a, b = body_bounds(text, "f")
        self.assertEqual(text[a:b], "{return x;}")

    def test_far_near_qualifiers_and_strings_survive(self):
        for word in ("__far", "_far", "far", "__near", "_near", "near"):
            source = f'int {word} *f(int {word} *p) {{ int {word} *q; q=p; return q; }}'
            model = Model.parse(source, source, "f")
            result = model.unit(model.target())
            self.assertIn("__near" if "near" in word else "__far", result)
        text = 'int f(void){ const char *p="far near // /* restrict _Atomic"; return p[0]; }'
        model = Model.parse(text, text, "f")
        self.assertIn('"far near // /* restrict _Atomic"', model.unit(model.target()))

    def test_same_seed_same_mutations(self):
        def chain():
            fn, rng, steps = self.model.target(), random.Random(998), []
            for _ in range(6):
                fn, step = mutate(self.model, fn, rng, "aggressive", ["colour", "Score"])
                steps.append(step)
            return self.model.unit(fn), steps
        self.assertEqual(chain(), chain())

    def test_no_commutation_of_calls_volatile_or_pointer_arithmetic(self):
        for body in ("return helper(x)+x;", "volatile int y=x; return y+x;", "int *p; p=0; return (p+x)==p;"):
            source = f"int helper(int); int f(int x) {{{body}}}"
            m = Model.parse(source, source, "f")
            opts = list(mutations(m.ast, m.target(), "commute"))
            self.assertEqual(opts, [])

    def test_float_predicate_not_reversed_by_relational_complement(self):
        text = "int f(double x){ if(x<0) return 1; else return 2; }"
        m = Model.parse(text, text, "f")
        options = list(mutations(m.ast, m.target(), "branch"))
        self.assertIn("!(x < 0)", emit(options[0][1]))
        self.assertEqual(list(mutations(m.ast, m.target(), "commute")), [])

    def test_no_unproved_arithmetic_width_or_global_mutations(self):
        self.assertNotIn("type", AGGRESSIVE)
        self.assertNotIn("prototype", AGGRESSIVE)
        src = "int g; int f(int x){g=x; return g;}"
        m = Model.parse(src, src, "f")
        for family in ("temporary", "address_copy", "compound", "promote_snapshot"):
            self.assertEqual(list(mutations(m.ast, m.target(), family)), [])

    def test_reject_statics_vla_target_pragmas(self):
        for body in ("static int x; return x;", "int a[n]; return a[0];", "\n#pragma aux x\nreturn n;"):
            source = f"int f(int n){{{body}}}"
            with self.assertRaises(Refusal):
                Model.parse(source, source, "f")

    def test_dependencies_and_declaration_initializers_are_not_swapped(self):
        for source, family in (("int f(int x){int a=1; int b=2; return a+b+x;}", "declarations"),
                               ("int f(int x){int a; int b; a=x; b=a+1; return b;}", "independent")):
            m = Model.parse(source, source, "f")
            self.assertEqual(list(mutations(m.ast, m.target(), family)), [])

    def test_threshold_limits_and_switch_fallthrough(self):
        src = "int f(unsigned x){if(x<0)return 1;if(x>2147483647)return 2;return 0;}"
        m = Model.parse(src, src, "f")
        self.assertEqual(list(mutations(m.ast, m.target(), "threshold")), [])
        src = "int f(int x){switch(x){case 0:x++;case 1:return x;default:return 0;}}"
        m = Model.parse(src, src, "f")
        self.assertEqual(list(mutations(m.ast, m.target(), "cases")), [])

    def test_macro_reexpansion_and_generated_name_collisions_refused(self):
        source = "#define x (x+1)\nint f(int x){return x;}"
        with self.assertRaisesRegex(Refusal, "re-expand"):
            Model.parse(source, "int f(int x){return x+1;}", "f")
        source = "#define perm_value_0 7\nint f(int x){x=x+1;return x;}"
        m = Model.parse(source, source, "f")
        fn = copy.deepcopy(m.target())
        old, new, _ = next(mutations(m.ast, fn, "temporary"))
        from permute_source import replace
        replace(fn.body, old, new)
        with self.assertRaisesRegex(Refusal, "re-expand"):
            m.unit(fn)

    def test_struct_member_does_not_supply_a_global_type(self):
        source = "volatile int g; int f(int x){struct S{int g;} s; g=x; return g+x;}"
        m = Model.parse(source, source, "f")
        self.assertFalse(Facts(m.ast, m.target()).local("g"))
        for family in ("temporary", "address_copy", "compound", "commute"):
            self.assertEqual(list(mutations(m.ast, m.target(), family)), [])

    def test_octal_threshold_and_initializer_barrier(self):
        source = "int f(int x){if(x>010)return 1;return 0;}"
        m = Model.parse(source, source, "f")
        opts = list(mutations(m.ast, m.target(), "threshold"))
        self.assertIn(">= 9", emit(opts[0][1]))
        source = "int b; int f(int x){short a; int c=sizeof(b); int b; return c+x;}"
        m = Model.parse(source, source, "f")
        self.assertEqual(list(mutations(m.ast, m.target(), "declarations")), [])

    def test_fixed_automatic_array_preserved(self):
        source = "int f(int x){char a[200]; a[0]=x; if(x) return a[0]; return 0;}"
        m = Model.parse(source, source, "f")
        self.assertIn("char a[200]", m.unit(m.target()))
        self.assertEqual(list(mutations(m.ast, m.target(), "declarations")), [])

    def test_downstream_source_lines_and_literal_contents_preserved(self):
        source = 'int f(int x){if(x) return 1; else return 2;} int tail(void){return __LINE__;}'
        pp = source.replace('__LINE__', '1')
        m = Model.parse(source, pp, "f")
        a, b = body_bounds(source, "f")
        fn, _ = mutate(m, m.target(), random.Random(2), "aggressive")
        result = m.unit(fn)
        c, d = body_bounds(result, "f")
        self.assertEqual(source[:b].count('\n'), result[:d].count('\n'))
        from permute_source import preserve_lines
        self.assertEqual(preserve_lines('{\nreturn "a\\nb";\n}', '{}'), '{ return "a\\nb"; }')

    def test_all_families_generate_parseable_full_units(self):
        extras = [FIXTURE, "int pilot(int x){x++;if(x)x++;return x;}",
                  "int pilot(int x){switch(x){case 0:return 2;case 1:return 3;default:return 4;}}"]
        covered = set()
        for source in extras:
            m = Model.parse(source, source, "pilot")
            for family in AGGRESSIVE:
                for old, new, _ in mutations(m.ast, m.target(), family):
                    covered.add(family)
                    from permute_source import replace
                    fn = copy.deepcopy(m.target())
                    old_index = list(__import__("permute_source").walk(m.target().body)).index(old)
                    target = list(__import__("permute_source").walk(fn.body))[old_index]
                    if target is fn.body:
                        fn.body = new
                    else:
                        replace(fn.body, target, new)
                    candidate = m.unit(fn)
                    Model.parse(candidate, candidate, "pilot")
        self.assertEqual(covered, set(AGGRESSIVE))


class GateTests(unittest.TestCase):
    def test_only_strict_and_zero_losses_is_hit(self):
        base = fake_result()
        self.assertFalse(assess(base, fake_result("CODE_BYTES_MATCH"), "pilot")["hit"])
        self.assertFalse(assess(base, fake_result("STRICT", ("tail", "pilot")), "pilot")["hit"])
        self.assertTrue(assess(base, fake_result("STRICT", ("neighbour", "tail", "pilot")), "pilot")["hit"])

    def test_data_ownership_and_alignment_gate(self):
        base, cand = fake_result(), fake_result("STRICT")
        base["shape"] = [{"segments": [{"class": "DATA", "size": 4, "data_sha256": "a"}]}]
        cand["shape"] = copy.deepcopy(base["shape"])
        cand["shape"][0]["segments"][0]["data_sha256"] = "b"
        self.assertFalse(assess(base, cand, "pilot")["eligible"])

    def test_instruction_distance_keeps_registers_and_undecodable_tail(self):
        self.assertEqual(instruction_distance(bytes.fromhex("85c0c3"), bytes.fromhex("85c0c3")), 0)
        self.assertGreater(instruction_distance(bytes.fromhex("85d2c3"), bytes.fromhex("85c0c3")), 0)
        self.assertGreater(instruction_distance(bytes.fromhex("c30f"), bytes.fromhex("c3")), 0)
        self.assertEqual(instruction_distance(bytes.fromhex("b801000000c3"), bytes.fromhex("b802000000c3"), range(1, 5), range(1, 5)), 0)


class SearchTests(unittest.TestCase):
    def setUp(self):
        self.root = permute.repo_root()
        parent = os.environ.get("PERMUTE_TEST_TMP", str(self.root / "build"))
        self.temp = tempfile.TemporaryDirectory(dir=parent, prefix="permtest_")
        self.base = Path(self.temp.name)
        self.src = self.base / "fixture.c"
        self.src.write_text(FIXTURE, encoding="utf-8")
        self.out = self.base / "run"
        self.calls = []
    def tearDown(self):
        self.temp.cleanup()
    def run_search(self, limit, resume=False, backend=None):
        args = [str(self.src), "pilot", "--out", str(self.out), "--conservative", "--max-compiles", str(limit), "--seconds", "100", "--seed", "21"]
        if resume:
            args.append("--resume")
        def fake(task):
            self.calls.append(task)
            return (backend or (lambda task: fake_result()))(task)
        # Tests exercise the scheduler without hashing/launching an install.
        with patch.object(permute, "manifest", return_value={"test": True, "inputs": {self.src.name: {"sha256": permute.digest(self.src.read_bytes())}}}):
            permute.main(args, evaluate_fn=fake, preprocess_fn=fake_preprocess)
        return json.loads((self.out / "state.json").read_text())

    def test_budget_includes_preprocessing_and_resume_does_not_repeat_baseline(self):
        first = self.run_search(5)
        self.assertEqual(first["compiles"], 5)
        self.assertEqual(len(self.calls), 4)
        second = self.run_search(8, resume=True)
        self.assertEqual(second["compiles"], 8)
        self.assertEqual(len(self.calls), 7)
        self.assertEqual(first["candidates"], second["candidates"][:len(first["candidates"])])

    def test_fresh_check_required_and_budgeted_on_resume(self):
        def backend(task):
            return fake_result() if Path(task["source"]).parent.name == "input" else fake_result("STRICT", ("neighbour", "tail", "pilot"), 0)
        first = self.run_search(3, backend=backend)
        self.assertIsNone(first["accepted"])
        second = self.run_search(4, resume=True, backend=backend)
        self.assertIsNotNone(second["accepted"])
        self.assertEqual(second["compiles"], 4)
        self.assertTrue((self.out / "accepted" / self.src.name).exists())

    def test_changed_snapshot_refuses_resume(self):
        self.run_search(3)
        (self.out / "input" / self.src.name).write_text("bad", encoding="utf-8")
        with self.assertRaisesRegex(Refusal, "frozen input"):
            self.run_search(4, resume=True)

    def test_interrupted_reservation_charged_and_recovered(self):
        self.run_search(3)
        path = self.out / "state.json"
        s = json.loads(path.read_text())
        s["compiles"] += 1
        s["pending"] = [{"invocation": 4, "kind": "candidate", "candidate": 99}]
        path.write_text(json.dumps(s))
        state = self.run_search(5, resume=True)
        self.assertEqual(state["compiles"], 5)
        self.assertEqual(state["pending"], [])

    def test_resumed_candidate_stream_matches_uninterrupted_run(self):
        self.run_search(5)
        resumed = self.run_search(9, resume=True)
        self.out = self.base / "full"
        full = self.run_search(9)
        self.assertEqual([c["hash"] for c in resumed["candidates"]], [c["hash"] for c in full["candidates"]])
        self.assertEqual([c["chain"] for c in resumed["candidates"]], [c["chain"] for c in full["candidates"]])


if __name__ == "__main__":
    unittest.main()
