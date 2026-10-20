"""Shared-header layout checks: offsets measured by the pinned Watcom compiler, compared with DOS code evidence.

Each expected value cites where the DOS machine code proves it (STRICT functions or code-wide statistics).
    python -m unittest tests.test_layout -v
"""
import struct
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import omf  # noqa: E402
import wcc  # noqa: E402
from common import BUILD  # noqa: E402

# (expression, expected value, evidence level, evidence)
# PROVEN = an instruction inside a STRICT function (or a code-wide statistic) fixes the value.
# RAYVERSE = layout taken from Rayverse types.h (derived from the PC binary); not yet proven by our STRICT code.
# The test pins the header to these values either way; promote RAYVERSE rows to PROVEN as matches cover them.
EXPECT = [
    ("sizeof(obj_t)", 0x84, "PROVEN", "84 `imul r,r,0x84` object-array strides in game code"),
    ("sizeof(eta_t)", 8, "PROVEN", "EOA 1:0d074 indexes eta[main_etat] with shl 2 and [sub_etat] with shl 3"),
    ("offsetof(obj_t, eta)", 0x0C, "RAYVERSE", "EOA 1:0d074 (not yet STRICT) loads [obj+0xc] before indexing eta"),
    ("offsetof(obj_t, iframes_timer)", 0x56, "PROVEN", "bat_get_eject_sens (STRICT) reads ray+0x56 (2:3e6a6)"),
    ("offsetof(obj_t, sub_etat)", 0x6C, "PROVEN", "BAT_ray_in_zone (STRICT) reads [obj+0x6c]"),
    ("offsetof(obj_t, flags)", 0x81, "PROVEN", "BAT_ray_in_zone (STRICT) tests [obj+0x81]"),
    ("sizeof(obj_flags_t)", 2, "RAYVERSE", "9 one-bit fields in unsigned char units; field_83 follows"),
    ("offsetof(obj_t, is_active)", 0x24, "RAYVERSE", ""),
    ("offsetof(obj_t, x)", 0x28, "RAYVERSE", ""),
    ("offsetof(obj_t, y)", 0x2C, "RAYVERSE", ""),
    ("offsetof(obj_t, screen_x)", 0x36, "RAYVERSE", ""),
    ("offsetof(obj_t, speed_x)", 0x40, "RAYVERSE", ""),
    ("offsetof(obj_t, follow_id)", 0x4A, "RAYVERSE", ""),
    ("offsetof(obj_t, ray_dist)", 0x54, "RAYVERSE", ""),
    ("offsetof(obj_t, type)", 0x60, "RAYVERSE", ""),
    ("offsetof(obj_t, offset_bx)", 0x68, "RAYVERSE", ""),
    ("offsetof(obj_t, anim_index)", 0x6A, "RAYVERSE", ""),
    ("offsetof(obj_t, main_etat)", 0x6D, "RAYVERSE", ""),
    ("offsetof(obj_t, cmd)", 0x70, "RAYVERSE", ""),
    ("offsetof(obj_t, hit_points)", 0x76, "RAYVERSE", ""),
]


def measure(header, exprs):
    src = BUILD / "tests" / "layout_probe.c"
    src.parent.mkdir(parents=True, exist_ok=True)
    src.write_text('#include <stddef.h>\n#include "%s"\nint layout_values[] = {\n%s\n};\n'
                   % (header, ",\n".join(f"    {e}" for e in exprs)))
    out = src.with_suffix(".obj")
    prof = wcc.profile("game")
    rc, text = wcc.compile_c(src, out, prof["flags"], prof["install"])
    assert rc == 0, text
    m = omf.load(out)[0]
    for n, si, o, _ in m.publics:
        if n == "_layout_values":
            data = m.segments[si].data
            return [struct.unpack_from("<i", data, o + 4 * k)[0] for k in range(len(exprs))]
    raise AssertionError("layout_values not found")


class LayoutTests(unittest.TestCase):
    def test_obj_h(self):
        got = measure("obj.h", [e for e, _, _, _ in EXPECT])
        bad = [f"{e} = {g:#x}, expected {v:#x} ({lvl} {why})" for (e, v, lvl, why), g in zip(EXPECT, got) if g != v]
        self.assertEqual(bad, [])


if __name__ == "__main__":
    unittest.main()
