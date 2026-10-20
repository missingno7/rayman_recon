"""Verifier self-tests: the accepted rand unit must be STRICT, and every tampered variant must not be.

    python -m unittest discover -s tests -v
"""
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import match  # noqa: E402
import omf  # noqa: E402
import wcc  # noqa: E402
from common import BUILD  # noqa: E402
from image import Image  # noqa: E402

GOOD = ROOT / "src" / "rand.c"
# Same logic, but the index kept in a local: differs from the original in one store/move ordering.
NEAR = """extern short RandArray[256];
extern int RandomIndex;
short myRand(short max)
{
    short res;
    int r = RandomIndex + 1;
    if (r >= 256)
        r = 0;
    res = RandArray[r] % (max + 1);
    RandomIndex = r;
    return res;
}
"""


def compile_src(path, tag):
    prof = wcc.profile("game")
    out = BUILD / "tests" / f"{tag}.obj"
    out.parent.mkdir(parents=True, exist_ok=True)
    rc, text = wcc.compile_c(path, out, prof["flags"], prof["install"])
    assert rc == 0, text
    return omf.load(out)[0]


class MatchTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.img = Image("gog_v121_us")
        cls.good = compile_src(GOOD, "good")

    def run_with(self, mod, override=None):
        orig = match.Bindings.lookup

        def lookup(b, raw):
            if override and raw in override:
                v = override[raw]
                return (v, "test") if v else (None, None)
            return orig(b, raw)
        match.Bindings.lookup = lookup
        try:
            return {r["function"]: r for r in match.verify_object(self.img, mod)}
        finally:
            match.Bindings.lookup = orig

    def test_accepted_unit_strict(self):
        r = self.run_with(self.good)
        self.assertEqual(r["myRand"]["status"], "STRICT")
        self.assertEqual(r["MakeMyRand"]["status"], "STRICT")

    def test_wrong_data_binding(self):
        r = self.run_with(self.good, {"_RandArray": (2, 0x7346)})
        self.assertEqual(r["myRand"]["status"], "NEAR_MATCH")

    def test_wrong_library_binding(self):
        r = self.run_with(self.good, {"srand_": (1, 0x71396)})   # rand_'s address
        self.assertEqual(r["MakeMyRand"]["status"], "NEAR_MATCH")

    def test_unbound_reference_not_strict(self):
        r = self.run_with(self.good, {"time_": None})
        self.assertEqual(r["MakeMyRand"]["status"], "CODE_BYTES_MATCH")

    def test_wrong_placement(self):
        r = self.run_with(self.good, {"myRand_": (1, 0x5bf64)})
        self.assertNotEqual(r["myRand"]["status"], "STRICT")

    def test_near_source_variant(self):
        p = BUILD / "tests" / "near.c"
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(NEAR)
        r = self.run_with(compile_src(p, "near"))
        self.assertEqual(r["myRand"]["status"], "NEAR_MATCH")
        self.assertGreater(r["myRand"]["byte_diffs"], 0)

    def test_byte_flip_detected(self):
        import copy
        mod = copy.deepcopy(self.good)
        seg = mod.segments[1]
        seg.data[0x0a] ^= 0x01          # inside 'cmp ebx,0x100' immediate, not a fixup field
        r = self.run_with(mod)
        self.assertEqual(r["myRand"]["status"], "NEAR_MATCH")


class DataSegmentTests(unittest.TestCase):
    """The bonus object's CONST (one string literal) is VERIFIED; a changed literal must not be."""

    def segs(self, text):
        img = Image("gog_v121_us")
        p = BUILD / "tests" / "bonus_variant.c"
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text)
        mod = compile_src(p, "bonus_variant")
        out = []
        match.verify_object(img, mod, None, out)
        return {g["segment"]: g for g in out}

    def test_const_verified_and_tamper_detected(self):
        src = (ROOT / "src" / "bonus.c").read_text()
        self.assertEqual(self.segs(src)["CONST"]["status"], "VERIFIED")
        bad = src.replace('"/%2d : %02d/"', '"/%2d : %02d!"')
        self.assertNotEqual(bad, src)
        self.assertEqual(self.segs(bad)["CONST"]["status"], "MISMATCH")


class BssLayoutTests(unittest.TestCase):
    """rand defines RandomIndex, alone in its _BSS object: layout VERIFIED; an extra global must break it."""

    def segs(self, text):
        img = Image("gog_v121_us")
        p = BUILD / "tests" / "rand_bss.c"
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text)
        out = []
        match.verify_object(img, compile_src(p, "rand_bss"), None, out)
        return {g["segment"]: g for g in out}

    def test_bss_layout(self):
        src = (ROOT / "src" / "rand.c").read_text()
        self.assertEqual(self.segs(src)["_BSS"]["status"], "VERIFIED")
        extra = src.replace("int RandomIndex;", "int RandomIndex;\nint rand_extra_global;")
        self.assertNotEqual(self.segs(extra)["_BSS"]["status"], "VERIFIED")


if __name__ == "__main__":
    unittest.main()
