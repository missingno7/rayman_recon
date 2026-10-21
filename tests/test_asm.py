"""Hand-written assembly path (tools/wcc.py build -> pinned WASM -> OMF -> tools/match.py), no comparator changes.

    python -m unittest tests.test_asm -v
"""
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import common  # noqa: E402
import omf  # noqa: E402
import wcc  # noqa: E402

SMOKE = ROOT / "tests" / "data" / "asm_smoke.asm"
HAVE = (common.TOOLS_DIR / "watcom-10.6" / "BINNT" / "WASM.EXE").exists()


@unittest.skipUnless(HAVE, "pinned WASM 10.6 not installed")
class AsmPathTests(unittest.TestCase):
    def build(self, text):
        d = Path(tempfile.mkdtemp(dir=ROOT / "build" / "tmp"))
        src, out = d / "u.asm", d / "u.obj"
        src.write_text(text, encoding="utf-8")
        prof = wcc.profile("asm")
        rc, msg = wcc.build(src, out, prof["flags"], prof["install"])
        return rc, msg, out

    def test_smoke_unit_assembles_to_dos_bytes(self):
        # synchro/endsynchro (DOS 1:06e8a, 32 bytes): the encodings WASM picks must stay the DOS ones
        rc, msg, out = self.build(SMOKE.read_text(encoding="utf-8"))
        self.assertEqual(rc, 0, msg)
        seg = omf.load(out)[0].segments[1]
        oracle = (ROOT / "build" / "oracles" / "gog_v121_us" / "obj1.bin").read_bytes()[0x6e8a:0x6eaa]
        self.assertEqual(bytes(seg.data), oracle)

    def test_prefix_before_first_public_is_refused(self):
        rc, msg, _ = self.build(SMOKE.read_text(encoding="utf-8").replace("synchro_:", "nop\nsynchro_:", 1))
        self.assertNotEqual(rc, 0)
        self.assertIn("no public at offset 0", msg)


if __name__ == "__main__":
    unittest.main()
