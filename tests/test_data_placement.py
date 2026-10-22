"""Regression tests for own initialized-data segment placement (no verifier shortcuts).

Worker command: python -B build/workers/t_match/run.py tests
Real fixtures are byte-identical copies of lk_strdata candidates, compiled serially.
"""
import copy
from pathlib import Path
import struct
import sys
from types import SimpleNamespace
import unittest

ROOT = next(p for p in Path(__file__).resolve().parents if (p / "tools/match.py").is_file())
H = ROOT / "build/workers/t_match"
sys.path.insert(0, str(ROOT / "tools"))
import match
import omf
import wcc
from common import BUILD
from image import Image


@unittest.skipUnless(all((H / (n + ".c")).is_file() for n in ("data_4d5c", "cdaudio")),
                     "lk_strdata fixture copies require t_match/run.py setup")
class DataPlacementFixtures(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.img = Image()
        cls.mods = {}
        profile = wcc.profile("game")
        for name in ("data_4d5c", "cdaudio"):
            obj = BUILD / "tests" / (name + "_placement.obj")
            obj.parent.mkdir(parents=True, exist_ok=True)
            rc, output = wcc.build(H / (name + ".c"), obj,
                profile["flags"] + ["-i=" + str(H / "include"), "-i=" + str(H)], profile["install"])
            assert rc == 0, output
            cls.mods[name] = omf.load(obj)[0]

    def check(self, mod):
        segments = []
        functions = match.verify_object(self.img, mod, seg_report=segments)
        return functions, {s["segment"]: s for s in segments}

    def test_candidates_verified(self):
        for name, count, sizes in (("data_4d5c", 139, (1615, 2764)), ("cdaudio", 16, (379, 64))):
            with self.subTest(unit=name):
                functions, segments = self.check(self.mods[name])
                self.assertEqual(segments["CONST"]["placement_votes"], count)
                self.assertEqual((segments["CONST"]["size"], segments["_DATA"]["size"]), sizes)
                self.assertTrue(all(s["status"] == "VERIFIED" for s in segments.values()))
                self.assertEqual(sum(f["status"] == "STRICT" for f in functions), 65 if name == "cdaudio" else 0)

    def test_damaged_literal(self):
        for name, original in self.mods.items():
            with self.subTest(unit=name):
                mod = copy.deepcopy(original)
                const = next(s for s in mod.segments if s and s.name == "CONST")
                const.data[0] ^= 1
                _, segments = self.check(mod)
                self.assertEqual(segments["CONST"]["status"], "MISMATCH")
                self.assertIn("first byte difference", segments["CONST"]["problem"])

    def test_removed_fixup(self):
        for name, original in self.mods.items():
            with self.subTest(unit=name):
                mod = copy.deepcopy(original)
                data = next(i for i, s in enumerate(mod.segments) if s and s.name == "_DATA")
                mod.fixups.remove(next(f for f in mod.fixups if f.seg == data))
                _, segments = self.check(mod)
                self.assertEqual(segments["_DATA"]["status"], "MISMATCH")
                self.assertIn("oracle relocations without candidate", segments["_DATA"]["problem"])

    def test_pointer_into_wrong_literal(self):
        for name, original in self.mods.items():
            with self.subTest(unit=name):
                mod = copy.deepcopy(original)
                const = next(i for i, s in enumerate(mod.segments) if s and s.name == "CONST")
                data = next(i for i, s in enumerate(mod.segments) if s and s.name == "_DATA")
                f = next(f for f in mod.fixups if f.seg == data and (f.target[0] & 3) == 0 and f.target[1] == const)
                # Select another actual literal start in the same pool, not an arbitrary corrupt byte.
                pool = mod.segments[const].data
                other = pool.index(0) + 1
                field = struct.unpack_from("<i", mod.segments[f.seg].data, f.off)[0]
                self.assertNotEqual(f.disp + field, other)
                struct.pack_into("<i", mod.segments[f.seg].data, f.off, other - f.disp)
                _, segments = self.check(mod)
                self.assertEqual(segments["CONST"]["status"], "MISMATCH")
                self.assertIn("inconsistent bases", segments["CONST"]["problem"])
                self.assertEqual(segments["_DATA"]["status"], "MISMATCH")


class DataPlacementGuards(unittest.TestCase):
    def fixture(self, count=3):
        def segment(name, data):
            return omf.Segment(name, "DATA", 1, 2, len(data), True,
                               data=bytearray(data), covered=bytearray([1] * len(data)))
        mod = omf.Module()
        mod.segments += [segment("CONST", b"A\0B\0"), segment("_DATA", b"\0" * (4 * count))]
        mod.publics = [("_table", 2, 0, False)]
        data = bytearray(0x80)
        data[0x40:0x44] = mod.segments[1].data
        fixups = {}
        for i in range(count):
            # Second pointer tests both signed in-field addends and OMF displacement.
            field, disp = (-1, 3) if i == 1 else (0, 0)
            struct.pack_into("<i", mod.segments[2].data, i * 4, field)
            struct.pack_into("<I", data, 0x10 + i * 4, 0x40 + field + disp)
            mod.fixups.append(omf.Fixup(2, i * 4, 9, False, (1, 1), (4, 1), disp))
            fixups[(2, 0x10 + i * 4)] = (7, 2, 0x40 + field + disp)
        img = SimpleNamespace(obj={2: bytes(data)}, fixups=fixups)
        bindings = {"_table": (2, 0x10)}
        binds = SimpleNamespace(game={}, lookup=lambda n: (bindings.get(n), "test"))
        return img, mod, binds, bindings

    def report(self, fixture, bases=None):
        img, mod, binds, _ = fixture
        return {s["segment"]: s for s in match.segment_report(img, mod, {} if bases is None else bases, binds)}

    def test_unanimous_votes_and_signed_addend(self):
        result = self.report(self.fixture())
        self.assertEqual(result["CONST"]["placement_votes"], 3)
        self.assertTrue(all(s["status"] == "VERIFIED" for s in result.values()))

    def test_conflicting_votes_no_majority(self):
        fixture = self.fixture()
        fixture[1].fixups[-1].disp = 1
        result = self.report(fixture)
        self.assertEqual(result["CONST"]["placement_votes"], 3)
        self.assertEqual(result["CONST"]["status"], "MISMATCH")
        self.assertNotIn("oracle", result["CONST"])

    def test_single_anchored_vote_still_checks_contents(self):
        fixture = self.fixture(1)
        self.assertEqual(self.report(fixture)["CONST"]["status"], "VERIFIED")
        fixture[1].segments[1].data[0] ^= 1
        self.assertEqual(self.report(fixture)["CONST"]["status"], "MISMATCH")

    def test_unanchored_guess_not_used(self):
        fixture = self.fixture(1)
        fixture[3].clear()
        self.assertEqual(self.report(fixture)["CONST"]["status"], "UNPLACED")
        # Even an externally supplied source base cannot replace a bound public anchor.
        self.assertEqual(self.report(fixture, {2: (2, 0x10)})["CONST"]["status"], "UNPLACED")

    def test_conflicting_public_anchors_not_used(self):
        fixture = self.fixture()
        fixture[1].publics.append(("_other", 2, 4, False))
        fixture[3]["_other"] = (2, 0x15)
        self.assertEqual(self.report(fixture)["CONST"]["status"], "UNPLACED")

    def test_existing_placement_not_overridden(self):
        result = self.report(self.fixture(), {1: (2, 0x41)})
        self.assertNotIn("placement_votes", result["CONST"])
        self.assertEqual(result["CONST"]["status"], "MISMATCH")
        self.assertEqual(result["_DATA"]["status"], "MISMATCH")

    def test_inferred_segment_never_becomes_anchor(self):
        fixture = self.fixture()
        img, mod, binds, _ = fixture
        mod.segments.append(copy.deepcopy(mod.segments[1]))
        mod.segments[3].name = "OTHER"
        mod.fixups.append(omf.Fixup(1, 0, 9, False, (1, 1), (4, 3), 0))
        img.fixups[(2, 0x40)] = (7, 2, 0x50)
        self.assertEqual(self.report(fixture)["OTHER"]["status"], "UNPLACED")

    def test_data_segment_can_be_target(self):
        fixture = self.fixture()
        fixture[1].segments[1].name = "_DATA"
        fixture[1].segments[2].name = "CONST"
        result = self.report(fixture)
        self.assertEqual(result["_DATA"]["placement_votes"], 3)
        self.assertEqual(result["_DATA"]["status"], "VERIFIED")

    def test_missing_oracle_fixup_not_a_vote(self):
        fixture = self.fixture(1)
        fixture[0].fixups.clear()
        result = self.report(fixture)
        self.assertEqual(result["CONST"]["status"], "UNPLACED")
        self.assertEqual(result["_DATA"]["status"], "MISMATCH")

    def test_uncovered_source_field_not_a_vote(self):
        fixture = self.fixture(1)
        fixture[1].segments[2].covered[0] = 0
        result = self.report(fixture)
        self.assertEqual(result["CONST"]["status"], "UNPLACED")
        self.assertEqual(result["_DATA"]["status"], "MISMATCH")

    def test_unsupported_relocation_not_a_vote(self):
        for change in ("self_relative", "off16", "ptr16:16"):
            fixture = self.fixture(1)
            f = fixture[1].fixups[0]
            if change == "self_relative":
                f.self_rel = True
            else:
                f.loc = 3 if change == "ptr16:16" else 1
            result = self.report(fixture)
            self.assertEqual(result["CONST"]["status"], "UNPLACED")
            self.assertEqual(result["_DATA"]["status"], "MISMATCH")

    def test_uncovered_target_and_extra_oracle_fixup_rejected(self):
        fixture = self.fixture()
        fixture[1].segments[1].covered[0] = 0
        self.assertEqual(self.report(fixture)["CONST"]["status"], "MISMATCH")
        fixture = self.fixture()
        fixture[0].fixups[(2, 0x40)] = (7, 2, 0x70)
        self.assertEqual(self.report(fixture)["CONST"]["status"], "MISMATCH")

    def test_collects_votes_from_all_publicly_anchored_sources(self):
        fixture = self.fixture(1)
        img, mod, _, bindings = fixture
        mod.segments.append(copy.deepcopy(mod.segments[2]))
        mod.segments[3].name = "TABLE2"
        mod.publics.append(("_table2", 3, 0, False))
        bindings["_table2"] = (2, 0x20)
        mod.fixups.append(omf.Fixup(3, 0, 9, False, (1, 1), (4, 1), 0))
        img.fixups[(2, 0x20)] = (7, 2, 0x40)
        result = self.report(fixture)
        self.assertEqual(result["CONST"]["placement_votes"], 2)
        self.assertEqual(result["CONST"]["status"], "VERIFIED")
        img.fixups[(2, 0x20)] = (7, 2, 0x41)
        result = self.report(fixture)
        self.assertEqual(result["CONST"]["placement_votes"], 2)
        self.assertEqual(result["CONST"]["status"], "MISMATCH")

    def test_nonzero_public_offset_and_segment_order(self):
        fixture = self.fixture()
        fixture[1].publics[0] = ("_table", 2, 4, False)
        fixture[3]["_table"] = (2, 0x14)
        self.assertEqual(self.report(fixture)["CONST"]["status"], "VERIFIED")
        img, mod, binds, _ = fixture
        # Verify that the same target is found even if the source is reported first.
        bases = {2: (2, 0x10)}
        evidence = match.infer_data_bases(img, mod, bases, binds, [2, 1])
        self.assertEqual(evidence[1]["placement_votes"], 3)
        self.assertEqual(bases[1], (2, 0x40))

    def test_source_placement_must_agree_with_public_anchor(self):
        fixture = self.fixture()
        self.assertEqual(self.report(fixture, {2: (2, 0x14)})["CONST"]["status"], "UNPLACED")

    def test_bss_and_debug_targets_never_inferred(self):
        for kind in ("BSS", "debug"):
            fixture = self.fixture()
            target = fixture[1].segments[1]
            if kind == "BSS":
                target.cls = "BSS"
                result = self.report(fixture)["CONST"]
                self.assertNotIn("placement_votes", result)
                self.assertEqual(result["status"], "BSS")
            else:
                target.name = "$$DEBUG"
                self.assertNotIn("$$DEBUG", self.report(fixture))


if __name__ == "__main__":
    unittest.main()
