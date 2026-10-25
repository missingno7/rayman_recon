"""tools/semir.py: STRICT functions are EXACT_LOCAL_EQUIVALENT; a one-operator mutant is a MISMATCH (never equivalent)."""
from pathlib import Path
import sys
import unittest

ROOT = next(p for p in Path(__file__).resolve().parents if (p / "tools/semir.py").is_file())
sys.path.insert(0, str(ROOT / "tools"))
import semir
from common import BUILD


class Semir(unittest.TestCase):
    def test_strict_functions_exact(self):
        results = semir.run_unit("luciole", None, None, tag=None)
        strict = [r for r in results if r["match_status"] == "STRICT"]
        self.assertGreaterEqual(len(strict), 5)
        for r in strict:
            self.assertEqual(r["verdict"], "EXACT_LOCAL_EQUIVALENT", r["function"])

    def test_mutant_is_mismatch(self):
        src = (ROOT / "src" / "luciole.c").read_text(encoding="utf-8")
        old = "Bloc_lim_H2_Aff = center_y + rayon_luciole;"
        self.assertEqual(src.count(old), 1)
        copy = BUILD / "tests" / "semir" / "luciole.c"
        copy.parent.mkdir(parents=True, exist_ok=True)
        copy.write_text(src.replace(old, "Bloc_lim_H2_Aff = center_y - rayon_luciole;"), encoding="utf-8")
        r = next(r for r in semir.run_unit("luciole", {"set_luciole"}, str(copy), tag="luciole_mutant")
                 if r["function"] == "set_luciole")
        self.assertEqual(r["verdict"], "MISMATCH")


if __name__ == "__main__":
    unittest.main()
