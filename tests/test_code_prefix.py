"""The CODE-segment prefix (private initialisers before the first public) is part of the STRICT gate.

gap_code 2026-10-24: select_level_prg was STRICT while its local `s32 num[6]` initialiser at 1:240f0 held
0x1100,0x1000 instead of the DOS 17,16. A reference into the prefix now needs the prefix bytes and relocations too.
"""
import copy
from pathlib import Path
import sys
import unittest

ROOT = next(p for p in Path(__file__).resolve().parents if (p / "tools/match.py").is_file())
sys.path.insert(0, str(ROOT / "tools"))
import match
import omf
import wcc
from common import BUILD
from image import Image


class CodePrefix(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.img = Image()
        cls.mods = {}
        profile = wcc.profile("game")
        for name in ("display", "menu"):
            obj = BUILD / "tests" / (name + "_prefix.obj")
            obj.parent.mkdir(parents=True, exist_ok=True)
            rc, output = wcc.build(ROOT / "src" / (name + ".c"), obj, list(profile["flags"]), profile["install"])
            assert rc == 0, output
            cls.mods[name] = omf.load(obj)[0]

    def status(self, mod, function):
        return next(r for r in match.verify_object(self.img, mod) if r["function"] == function)

    def text(self, mod):
        return next(s for s in mod.segments if s and s.cls.upper() == "CODE")

    def test_current_sources_strict(self):
        self.assertEqual(self.status(self.mods["display"], "select_level_prg")["status"], "STRICT")
        # menu's prefix holds a pointer array into its own CONST literals (relocations verified by contents)
        self.assertEqual(self.status(self.mods["menu"], "INIT_GENERAL_CHOICE")["status"], "STRICT")

    def test_damaged_prefix_byte(self):
        mod = copy.deepcopy(self.mods["display"])
        self.text(mod).data[0] ^= 0x10          # num[0] 17 -> 1
        r = self.status(mod, "select_level_prg")
        self.assertEqual(r["status"], "CODE_BYTES_MATCH")
        self.assertTrue(any("prefix" in u for u in r["unverified"]))

    def test_removed_prefix_relocation(self):
        mod = copy.deepcopy(self.mods["menu"])
        text = mod.segments.index(self.text(mod))
        first = min(o for _, s, o, _ in mod.publics if s == text)
        mod.fixups.remove(next(f for f in mod.fixups if f.seg == text and f.off < first))
        self.assertNotEqual(self.status(mod, "AFFICHE_BUTTON_JOYSTICK")["status"], "STRICT")


if __name__ == "__main__":
    unittest.main()
