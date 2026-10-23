"""Guard: no reconstructed source carries the permuter dangling-else signature (tools/dangling_else.py)."""
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import dangling_else  # noqa: E402

BAD = """void f(int a, int b) {
    if (a)
        if (b)
            g();
    else
        h();
}
"""
OK = """void f(int a, int b) {
    if (a) {
        if (b)
            g();
    } else
        h();
}
"""


class DanglingElse(unittest.TestCase):
    def test_detects_signature(self):
        with tempfile.TemporaryDirectory() as d:
            (Path(d) / "bad.c").write_text(BAD)
            (Path(d) / "ok.c").write_text(OK)
            self.assertEqual(len(dangling_else.scan(str(Path(d) / "bad.c"))), 1)
            self.assertEqual(dangling_else.scan(str(Path(d) / "ok.c")), [])

    def test_sources_clean(self):
        files = sorted(str(p) for p in (ROOT / "src").rglob("*.c"))
        hits = [(f, h) for f in files for h in dangling_else.scan(f)]
        self.assertEqual(hits, [])


if __name__ == "__main__":
    unittest.main()
