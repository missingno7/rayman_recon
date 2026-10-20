"""Seeding positive control: PS1-decomp C adapted by config/seed_rules.json must reproduce a known STRICT function.

    python -m unittest tests.test_seed -v
"""
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import seed  # noqa: E402


class SeedTests(unittest.TestCase):
    def test_ps1_seed_reproduces_strict(self):
        r = seed.try_one("DO_CAGE")          # src/cage.c, STRICT; PS1 body differs only by the Obj -> obj_t rule
        self.assertEqual(r["result"], "AUTO_STRICT")
        self.assertEqual(r["best"]["origin"], "ps1")

    def test_flag_rule(self):
        out = seed.adapt_ps1("if (obj->flags & FLG(OBJ_FLIP_X)) obj->flags &= ~FLG(OBJ_ALIVE);")
        self.assertEqual(out, "if (obj->flags.flip_x) obj->flags.alive = 0;")


if __name__ == "__main__":
    unittest.main()
