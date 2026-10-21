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
