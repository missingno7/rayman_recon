"""Structural census rules (tools/census.py): normalisations that keep Watcom shape out of the DIFF classes.

    python -m unittest tests.test_census -v
"""
import collections
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import census  # noqa: E402


class CensusRuleTests(unittest.TestCase):
    def test_comparison_rewrites_are_not_constants(self):
        # `x > 15` vs `x >= 16`, width masks and 0/1 booleans are shape, a genuinely different constant is not
        self.assertEqual(census.const_diff([15, 0xff, 1, 7], [16, 0, 7]), ([], []))
        self.assertEqual(census.const_diff([40], [48]), ([40], [48]))

    def test_short_load_is_normalised(self):
        # -5 short load of s16 at 2:1000 (`mov eax,[0xffe]; ...; sar eax,16`) is the same target as a direct load
        code = bytes.fromhex("a1fe0f0000" "89c2" "c1f810" "c3")
        f = census.features(code, {1: "2:ffe"}, {})
        self.assertEqual(f["globals"], collections.Counter({"2:1000": 1}))

    def test_same_structure_is_shape(self):
        a = {"calls": ["1:10"], "globals": collections.Counter({"2:10": 1}), "fields": collections.Counter({4: 1}),
             "consts": collections.Counter({5: 1}), "toks": ["mov R, R", "call A"], "frame": ["ebx"]}
        b = dict(a, toks=["mov R, R", "nop", "call A"])
        self.assertEqual(census.classify(a, b, 3, []), "SHAPE")
        self.assertEqual(census.classify(dict(a, calls=[]), b, 3, []), "DIFF_calls")


if __name__ == "__main__":
    unittest.main()
