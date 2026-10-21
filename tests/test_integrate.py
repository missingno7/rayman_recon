"""Integrator binding safety (tools/integrate.py, tools/promote.py): the IF@DSIN/IF@DCOS loss class must fail closed.

    python -m unittest tests.test_integrate -v
"""
import json
import shutil
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import integrate  # noqa: E402
import promote  # noqa: E402


def ov(path, name, addr, evidence="DOS disassembly"):
    path.write_text(json.dumps({"gog_v121_us": {"functions": {name: {"addr": addr, "evidence": evidence}},
                                                "data": {}}}))
    return path


class IntegrateBindingTests(unittest.TestCase):
    def setUp(self):
        (ROOT / "build" / "tmp").mkdir(parents=True, exist_ok=True)
        self.tmp = Path(tempfile.mkdtemp(dir=ROOT / "build" / "tmp"))
        self.cfg = self.tmp / "symbols.json"
        self.cfg.write_text(json.dumps({"gog_v121_us": {"functions": {
            "sin": {"addr": "1:70872", "name": "RAYVERSE", "evidence": "x"}}, "data": {}}}))
        self._config, self._andr = promote.CONFIG, promote.android_mismatches
        promote.CONFIG = self.tmp
        promote.android_mismatches = lambda canon: []

    def tearDown(self):
        promote.CONFIG, promote.android_mismatches = self._config, self._andr
        shutil.rmtree(self.tmp)

    def bound(self):
        return json.loads(self.cfg.read_text())["gog_v121_us"]["functions"]

    def test_collision_is_reported_not_renamed(self):
        # worker A claims a second name at an already-bound address: integrate must stop, not silently rename
        a = ov(self.tmp / "a.json", "IF@DSIN", "1:70872", "libscan MATH3R.LIB member")
        self.assertEqual(integrate.collisions(a, self.cfg), [("functions", "IF@DSIN", "sin")])
        self.assertIn("IF@DSIN", json.loads(a.read_text())["gog_v121_us"]["functions"])   # overlay untouched

    def test_rename_records_alias_and_blocks_later_wrong_claim(self):
        # handoff section 2: A's IF@DSIN at 1:70872 renamed onto sin; B's IF@DSIN at 1:70899 must then conflict
        a = ov(self.tmp / "a.json", "IF@DSIN", "1:70872", "libscan MATH3R.LIB member")
        src = self.tmp / "a.c"
        src.write_text("double f(double x) { return sin(x); }\n")
        integrate.apply_renames(a, [src], {"IF@DSIN": "sin"}, self.cfg)
        self.assertEqual(self.bound()["sin"]["aliases"], ["IF@DSIN"])
        promote.merge_symbols(a, {})
        b = ov(self.tmp / "b.json", "IF@DSIN", "1:70899", "libscan MATH3R.LIB member")
        with self.assertRaises(SystemExit):
            promote.merge_symbols(b, {})
        self.assertNotIn("IF@DSIN", self.bound())

    def test_runtime_name_needs_library_or_probe_evidence(self):
        with self.assertRaises(SystemExit):
            promote.merge_symbols(ov(self.tmp / "c.json", "IF@DCOS", "1:70899", "target of my own call"), {})
        promote.merge_symbols(ov(self.tmp / "d.json", "IF@DCOS", "1:70899", "libscan: MATH3R.LIB sincos+0x26c"), {})
        self.assertEqual(self.bound()["IF@DCOS"]["addr"], "1:70899")

    def test_alias_cannot_move(self):
        # review R1: an established alias must not be re-recorded onto another binding at another address
        promote.merge_symbols(ov(self.tmp / "c.json", "cos", "1:70899"), {})
        a = ov(self.tmp / "a.json", "IF@DSIN", "1:70872", "libscan MATH3R.LIB member")
        integrate.apply_renames(a, [], {"IF@DSIN": "sin"}, self.cfg)
        b = ov(self.tmp / "b.json", "IF@DSIN", "1:70899", "libscan MATH3R.LIB member")
        with self.assertRaises(SystemExit):
            integrate.apply_renames(b, [], {"IF@DSIN": "cos"}, self.cfg)
        self.assertNotIn("aliases", self.bound()["cos"])

    def test_promote_rename_records_alias(self):
        # review R1: the public promote --rename path must keep the old name claimed too
        promote.merge_symbols(ov(self.tmp / "a.json", "IF@DSIN", "1:70872", "libscan"), {"IF@DSIN": "sin"})
        self.assertEqual(self.bound()["sin"]["aliases"], ["IF@DSIN"])
        with self.assertRaises(SystemExit):
            promote.merge_symbols(ov(self.tmp / "b.json", "IF@DSIN", "1:70899", "libscan"), {})

    def test_promote_rename_to_unbound_target_refused(self):
        # re-review R1: a rename onto a name that is not bound yet would drop the old claim
        with self.assertRaises(SystemExit):
            promote.merge_symbols(ov(self.tmp / "a.json", "IF@DSIN", "1:70800", "libscan"), {"IF@DSIN": "new_name"})
        self.assertNotIn("new_name", self.bound())

    def test_json_writer_leaves_no_sidecar(self):
        # re-review R7: an interrupted dump must not leave config/<file>.<pid>.tmp behind
        import common

        class Boom:
            pass
        target = self.tmp / "x.json"
        with self.assertRaises(TypeError):
            common.dump_json(target, {"a": Boom()})
        self.assertEqual([p.name for p in self.tmp.iterdir() if p.name.startswith("x.json")], [])

    def test_two_new_names_at_one_address_refused(self):
        # review R8: duplicates inside one overlay
        p = self.tmp / "d.json"
        p.write_text(json.dumps({"gog_v121_us": {"functions": {"a": {"addr": "1:12345"}, "b": {"addr": "1:12345"}},
                                                 "data": {}}}))
        with self.assertRaises(SystemExit):
            promote.merge_symbols(p, {})
        self.assertNotIn("a", self.bound())

    def test_overlay_may_not_carry_aliases(self):
        p = self.tmp / "e.json"
        p.write_text(json.dumps({"gog_v121_us": {"functions": {"x": {"addr": "1:1", "aliases": ["sin"]}},
                                                 "data": {}}}))
        with self.assertRaises(SystemExit):
            promote.merge_symbols(p, {})

    def test_cli_rejects_unsafe_pairs(self):
        # review R5/R11: path-escaping unit names and a second value after --rename are refused before any write
        for argv in (["w", "../outside=u.c", "--new", "-m", "x"], ["w", "u=../u.c", "-m", "x"],
                     ["w", "u=u.c", "--rename", "A=B", "C=D.c", "-m", "x"]):
            with self.assertRaises(SystemExit):
                integrate.parse(argv)

    def test_readonly_destination_restored(self):
        # review R2: a read-only file must not defeat rollback
        import os
        import stat
        f = self.tmp / "ro.c"
        f.write_text("ORIGINAL")
        os.chmod(f, stat.S_IREAD)
        w = integrate.Writes()
        w.touch(f)
        f.write_text("CANDIDATE")
        os.chmod(f, stat.S_IREAD)
        self.assertEqual(w.restore(), [])
        self.assertEqual(f.read_text(), "ORIGINAL")
        os.chmod(f, stat.S_IWRITE | stat.S_IREAD)

    def test_failure_restores_only_written_files(self):
        keep = self.tmp / "other_agent.c"           # not written by this run: must survive a restore
        keep.write_text("work in progress")
        mine, existing = self.tmp / "new.c", self.tmp / "old.c"
        existing.write_text("before")
        w = integrate.Writes()
        w.touch(mine)
        w.touch(existing)
        mine.write_text("candidate")
        existing.write_text("after")
        w.restore()
        self.assertFalse(mine.exists())
        self.assertEqual(existing.read_text(), "before")
        self.assertEqual(keep.read_text(), "work in progress")


if __name__ == "__main__":
    unittest.main()
