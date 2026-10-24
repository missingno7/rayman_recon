"""Semantic registry (docs/semantics/*.json) stays tied to the real code: declarations, bindings, function bodies of
the documented unit and the struct layout compiled by the pinned compiler. Evidence drift in other units is reported by
`tools/semantics.py check --evidence`, not enforced here (matching work elsewhere must not be blocked)."""
import copy
import json
from pathlib import Path
import sys
import unittest

ROOT = next(p for p in Path(__file__).resolve().parents if (p / "tools/semantics.py").is_file())
sys.path.insert(0, str(ROOT / "tools"))
import semantics
from common import BUILD

REGISTRIES = sorted((ROOT / "docs" / "semantics").glob("*.json"))


class SemanticRegistry(unittest.TestCase):
    def test_registries_match_code(self):
        self.assertTrue(REGISTRIES)
        for path in REGISTRIES:
            with self.subTest(registry=path.name):
                reg = json.loads(path.read_text(encoding="utf-8"))
                errors = semantics.check_source(reg, check_fingerprints=False)
                self.assertEqual(errors, [])
                errors = semantics.check_layout(reg, BUILD / "tests" / "semantics" / path.stem)
                self.assertEqual(errors, [])

    def test_wrong_field_offset_is_rejected(self):
        reg = json.loads(REGISTRIES[0].read_text(encoding="utf-8"))
        bad = copy.deepcopy(reg)
        field = next(e for e in bad["entities"] if e["kind"] == "field" and not e.get("bit_width") and e["offset"] > 0)
        field["offset"] += 1
        errors = semantics.check_layout(bad, BUILD / "tests" / "semantics" / "negative")
        self.assertTrue(errors)

    def test_unknown_meanings_keep_neutral_names(self):
        for path in REGISTRIES:
            reg = json.loads(path.read_text(encoding="utf-8"))
            for e in reg["entities"]:
                if e["evidence_level"] in ("UNKNOWN", "HYPOTHESIS"):
                    self.assertFalse(e.get("previous_name"), e["id"])


if __name__ == "__main__":
    unittest.main()
