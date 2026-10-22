"""Census false-positive regressions from the 2026-10-22 semantic audits (tools/census.py).

Uses compiled fixtures kept in build/workers/t_census (objects, verification.json, historical menu.obj); the
fixture-based classes skip when that directory is absent.

    python -m unittest tests.test_census_audit -v
"""
import collections
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import importlib.util
import json
import struct
WORK = ROOT / "build" / "workers" / "t_census"
spec = importlib.util.spec_from_file_location("census", ROOT / "tools" / "census.py")
census = importlib.util.module_from_spec(spec)
spec.loader.exec_module(census)
import omf


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

    def test_rule_hints_follow_triage(self):
        # code-equal -> only the neighbour rule; a changed push set -> width/unused-param rules; data diffs -> binding
        self.assertEqual(census.rule_hints({"class": "REFS_LAYOUT", "frame": [[], []]}), ["W-LAYOUT"])
        h = census.rule_hints({"class": "SHAPE", "frame": [["ebx", "ecx"], ["ebx", "ecx", "edx"]]})
        self.assertIn("W-RETW", h)
        self.assertIn("W-UNUSED", h)
        self.assertEqual(census.rule_hints({"class": "DIFF_globals", "frame": [[], []]})[0], "W-BIND")
        self.assertEqual(census.rule_hints({"class": "DIFF_calls", "frame": [[], []]})[0], "W-LAYOUT")

    def test_prototype_signature_normalisation(self):
        # parameter names and C type spellings must not create false W-PROTO conflicts
        self.assertEqual(census._sig("short", "obj_t *obj, int x"), census._sig("s16", "obj_t*, s32"))
        self.assertNotEqual(census._sig("u8", "s16, s16"), census._sig("s16", "s16, s16"))


@unittest.skipUnless((WORK / "verification.json").exists(), "t_census fixtures not present")
class AuditRegressionTests(unittest.TestCase):
    """Fresh local objects + immutable DOS oracle; no compiling or generated writes in these tests."""
    @classmethod
    def setUpClass(cls):
        cls.orc = census.Oracle()
        cls.results = json.loads((WORK / "verification.json").read_text())

    def pair(self, unit, name):
        r = next(r for r in self.results[unit] if r["function"] == name)
        mod = omf.load(WORK / "objects" / (unit + ".obj"))[0]
        fc = census.candidate_features(self.orc, mod, r)
        fo = self.orc.oracle_features(int(r["oracle"].split(":")[1], 16), r["size"])[0]
        return fc, fo

    def test_ebp_is_a_data_base(self):
        # sem_c: effect2 1:28a74 / candidate +4e, both items[i].sort_key at +34h.
        c, o = self.pair("effect2", "sub_38A30")
        self.assertIn(0x34, c["fields"])
        self.assertIn(0x34, o["fields"])
        # A genuine EBP frame must still be excluded.
        f = census.features(bytes.fromhex("5589e58b45085dc3"), {}, {})
        self.assertFalse(f["fields"])

    def test_lea_arithmetic_is_not_a_field(self):
        # sem_c setToleranceDist +cb LEA [EDX-1] vs DOS NEG/DEC.
        c, o = self.pair("collision1", "setToleranceDist")
        self.assertNotIn(-1, c["fields"])
        # allocateMOSKITOMAMA +43 LEA [EBX+5a] vs DOS ADD EAX,5a at 1:2f132.
        c, o = self.pair("hybrid", "allocateMOSKITOMAMA")
        self.assertNotIn(90, c["fields"])
        self.assertIn(90, c["consts"])
        self.assertIn(90, o["consts"])

    def test_postincrement_address(self):
        # sem_c CaptureVideo +94/+99 ADD EBX,4; [EBX-4] vs 1:149e0 old pointer store.
        c, o = self.pair("box", "CaptureVideo")
        self.assertNotIn(-4, c["fields"])
        # sem_c sub_3A34C +29/+2c: WORD store [new EAX-2], not a -5 short load.
        c, o = self.pair("movieprefix", "sub_3A34C")
        self.assertNotIn(-2, c["fields"])

    def test_cached_global_base(self):
        # sem_c PlaySnd +b1..+c1 vs 1:62338/6233f/6234f: hard_sound_table +0/+2/+1.
        c, o = self.pair("sound", "PlaySnd")
        for target in ("2:7a44", "2:7a45", "2:7a46"):
            self.assertIn(target, c["globals"])
            self.assertIn(target, o["globals"])
        self.assertNotIn(1, c["fields"])
        self.assertNotIn(6, c["fields"])  # cached voice pointer survives KeyOff's RET 4 and stack reload
        self.assertEqual(set(c["fields"]), set(o["fields"]))

    def test_next_record_base(self):
        # sem_c effect_activate +f6..+124 vs 1:5047e..504ae: next rectangle base +2c.
        c, o = self.pair("effect50350", "effect_activate")
        for offset in (0x51, 0x52, 0x55, 0x56, 0x57):
            self.assertIn(offset, c["fields"])
            self.assertIn(offset, o["fields"])

    def test_immediate_in_fixup_instruction(self):
        # sem_c phase_dark2=7 at candidate +3e0 vs DOS 1:30898/3089f.
        c, o = self.pair("hybrid", "DO_DARK_PHASE2_COMMAND")
        self.assertIn(7, c["consts"])
        self.assertIn(7, o["consts"])

    def test_negative_full_width_constants(self):
        # sem_c collision_briques: sound 214 must be visible even when encoded beside a data fixup.
        c, o = self.pair("breakout", "collision_briques")
        self.assertIn(214, c["consts"])
        self.assertIn(214, o["consts"])
        # sem_a main_Ray AND EAX,ffff1ff0 was previously discarded for exceeding 0xffff.
        c, o = self.pair("pcmain", "main_Ray")
        self.assertIn(-57360, o["consts"])

    def test_word_sentinel(self):
        # sem_c PlaySnd sentinel: MOV [voice],FFFEh vs MOV EDX,FFFFFFFEh / MOV [voice],DX.
        c, o = self.pair("sound", "PlaySnd")
        self.assertIn(-2, c["consts"])
        self.assertIn(-2, o["consts"])
        self.assertNotIn(65534, c["consts"])
        self.assertEqual(census.const_diff([-2], [-3]), ([], []))  # inherited +/-1 tolerance
        self.assertEqual(census.const_diff([-2], [-8]), ([-2], [-8]))

    def test_real_field_global_and_constant_changes_remain_visible(self):
        # Negative controls for the same instruction families as the audit witnesses.
        a = census.features(bytes.fromhex("8b4534c3"), {}, {})
        b = census.features(bytes.fromhex("8b4538c3"), {}, {})
        self.assertIn("fields", census.classify(a, b, 1, []))
        a = census.features(bytes.fromhex("c7050000000007000000c3"), {2: "2:5000"}, {})
        b = census.features(bytes.fromhex("c7050000000013000000c3"), {2: "2:5000"}, {})
        self.assertIn("consts", census.classify(a, b, 1, []))
        b = census.features(bytes.fromhex("c7050000000007000000c3"), {2: "2:6000"}, {})
        self.assertIn("globals", census.classify(a, b, 1, []))

    def test_trailing_data_and_early_returns(self):
        # sem_c mereDenisExplodeBombs RET 1:41064, false CALLs in following switch table.
        c, o = self.pair("meredenis", "mereDenisExplodeBombs")
        self.assertFalse(c["calls"])
        self.assertFalse(o["calls"])
        # sem_c effect_helper_348a4 RET 1:34d87; strings start 1:34d90.
        c, o = self.pair("effect34580", "effect_helper_348a4")
        self.assertNotIn(25959, o["consts"])
        self.assertNotIn(118, o["fields"])
        # A forward branch past an early RET must retain its later arm.
        f = census.features(bytes.fromhex("7401c3b82a000000c3"), {}, {})
        self.assertIn(42, f["consts"])

    def test_const_segment_name(self):
        for unit, name in (("affiche", "DISPLAY_SAVE_SPRITES"), ("breakout", "DISPLAY_FIXE_CB"),
                           ("moteur", "build_active_table")):
            c, o = self.pair(unit, name)
            self.assertIn("LIT", c["globals"])
            self.assertIn("LIT", o["globals"])
            self.assertNotIn("SEG", c["globals"])

    def test_internal_calls_without_fixups(self):
        # sem_a: actual ordered callees agree; original census omitted same-TU calls.
        for unit, name in (("cdaudio", "sub_19790"), ("cdaudio", "sub_1a4ac")):
            c, o = self.pair(unit, name)
            self.assertEqual(c["calls"], o["calls"])
            self.assertTrue(c["calls"])

    def test_callback_identity(self):
        # sem_a JoystickPresent retains its named callback assignments.
        c, o = self.pair("input", "JoystickPresent")
        callbacks = {x for x in o["globals"] if x.startswith("1:")}
        self.assertTrue(callbacks)
        self.assertTrue(callbacks <= set(c["globals"]))
        # Changing a real callback must remain visible even inside the same object.
        a = census.features(bytes.fromhex("b800000000c3"), {1: "1:123"}, {})
        b = census.features(bytes.fromhex("b800000000c3"), {1: "1:456"}, {})
        self.assertIn("globals", census.classify(a, b, 1, []))

    def test_missing_dos_interrupts(self):
        c, o = self.pair("cheat", "LOAD_CONTINUE_SCREEN")
        self.assertEqual(o["calls"].count("int:21"), 2)
        self.assertNotIn("int:21", c["calls"])
        self.assertIn("calls", census.classify(c, o, 1, []))

    def test_historical_menu_wrong_word(self):
        # History is materialised by history_control.py from the parent of the sem_a fix.
        record = json.loads((WORK / "history_control.json").read_text())
        mod = omf.load(WORK / "historical/menu.obj")[0]
        r = record["result"]
        c = census.candidate_features(self.orc, mod, r)
        o = self.orc.oracle_features(int(r["oracle"].split(":")[1], 16), r["size"])[0]
        self.assertNotIn("2:3f79e", c["globals"])
        self.assertIn("2:3f79e", o["globals"])
        self.assertIn("globals", census.classify(c, o, r["byte_diffs"], []))
        c, o = self.pair("menu", "AFFICHE_ECRAN_OPTIONS")
        self.assertIn("2:3f79e", c["globals"])
        self.assertEqual(set(c["globals"]), set(o["globals"]))


if __name__ == "__main__":
    unittest.main()
