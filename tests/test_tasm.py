"""Dialect fixtures are synthetic OMF; real probe objects were independently linked by WLINK.

python -m unittest tests.test_tasm -v
After adoption, ordinary unittest discovery uses the adopted omf/wcc modules.
"""
import copy
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = next(p for p in Path(__file__).resolve().parents if (p / 'config/oracles.json').is_file())
sys.path.insert(0, str(ROOT / 'tools'))
import omf
import wcc
import match
from image import Image


def record(typ, body=b''):
    head = bytes([typ]) + struct.pack('<H', len(body) + 1) + body
    return head + bytes([-sum(head) & 255])


def module(translator=b'Turbo Assembler  Version 2.0', extension=True, loc=5,
           fixupp=0x9C, use32=True, after=False):
    """One synthetic field at 0, target segment 1, no target displacement."""
    comments = (record(0x88, b'\x00\x00' + translator) if translator else b'')
    if extension:
        comments += record(0x88, b'\x40\xa1')
    body = record(0x96, b'\x05_TEXT\x04CODE\x00')
    body += record(0x98, bytes([0x68 | int(use32)]) + b'\x04\x00\x01\x02\x03')
    body += record(0xA0, b'\x01\x00\x00\x00\x00\x01\x00')
    body += record(fixupp, bytes([0xC0 | (loc << 2), 0, 0x54, 1]))
    return (record(0x80, b'\x01u') + (b'' if after else comments) + body
            + (comments if after else b'') + record(0x8A, b'\x00'))


class DialectTests(unittest.TestCase):
    def test_pharlap_retains_raw_location_and_reads_four_bytes(self):
        m = omf.parse_object(module())[0]
        self.assertEqual(m.dialect, 'borland-pharlap32')
        self.assertEqual(m.fixups[0].loc, 5)
        self.assertEqual(m.fixups[0].size(), 4)
        self.assertEqual(m.fixups[0].kind(), 'off32-pharlap')
        self.assertEqual(omf.LOC_SIZE[5], 2)

    def test_intel_location5_is_two_bytes(self):
        for translator in (None, b'Microsoft Macro Assembler 5.10'):
            m = omf.parse_object(module(translator, extension=False))[0]
            self.assertEqual(m.dialect, 'intel')
            self.assertEqual(m.fixups[0].size(), 2)
            self.assertEqual(m.fixups[0].kind(), 'off16l')

    def test_tasm_default_is_not_pharlap(self):
        m = omf.parse_object(module(loc=1, fixupp=0x9D))[0]
        self.assertEqual(m.dialect, 'intel')
        self.assertEqual(m.fixups[0].size(), 2)

    def test_ambiguous_tasm_signature_refused(self):
        for opts in ({'extension': False}, {'fixupp': 0x9D}, {'use32': False},
                     {'translator': b'Turbo Assembler  Version 2.01'}):
            with self.subTest(opts=opts), self.assertRaisesRegex(ValueError, 'refusing width guess'):
                omf.parse_object(module(**opts))

    def test_comments_after_fixups_and_module_scope(self):
        a, b = omf.parse_object(module(after=True) + module(None, extension=False))
        self.assertEqual(a.fixups[0].size(), 4)
        self.assertEqual(b.fixups[0].size(), 2)

    def test_conflicting_translator_refused(self):
        extra = record(0x88, b'\x00\x00Microsoft Macro Assembler')
        data = module()
        with self.assertRaisesRegex(ValueError, 'refusing width guess'):
            omf.parse_object(data[:6] + extra + data[6:])

    def test_independent_wlink_probe(self):
        # asm_id/fixup_probe: independent WLINK resolves target > 64K to
        # 0x10010 for both objects. Here require the raw field's high word.
        directory = Path(__file__).resolve().parent / 'data' / 'tasm'
        t = omf.load(directory / 'tasm_pharlap.obj')[0]
        i = omf.load(directory / 'intel32.obj')[0]
        self.assertEqual(t.dialect, 'borland-pharlap32')
        self.assertEqual([f.loc for f in t.fixups], [5, 5])
        self.assertEqual([f.size() for f in t.fixups], [4, 4])
        self.assertEqual([f.size() for f in i.fixups], [4, 4])
        for m in (t, i):
            # TASM carries the addend in LEDATA; MASM carries it in FIXUPP.
            self.assertEqual(bytes(m.segments[1].data)[::5], b'\xb8\xa1\xc3')
            self.assertEqual([struct.unpack_from('<I', m.segments[1].data, f.off)[0] + f.disp
                              for f in m.fixups], [0x10000, 0x10000])


SMOKE = """.386p
_TEXT segment para public use32 'CODE'
_TEXT ends
_DATA segment para public use32 'DATA'
_DATA ends
FLAT group _TEXT, _DATA
assume cs:_TEXT, ds:FLAT
_TEXT segment para public use32 'CODE'
public smoke_
smoke_:
 mov eax, offset target_
 ret
_TEXT ends
_DATA segment para public use32 'DATA'
public target_
target_ dd 12345678h
_DATA ends
end
"""


class BackendTests(unittest.TestCase):
    def setUp(self):
        root = wcc.BUILD / 'tmp'
        root.mkdir(parents=True, exist_ok=True)
        self.tmp = tempfile.TemporaryDirectory(dir=root)
        self.addCleanup(self.tmp.cleanup)
        self.src = Path(self.tmp.name) / 'smoke.asm'
        self.out = Path(self.tmp.name) / 'smoke.obj'
        self.src.write_text(SMOKE)
        self.p = wcc.profile('hmi_tasm')

    def build(self, flags=None):
        return wcc.build(self.src, self.out, self.p['flags'] if flags is None else flags, self.p['install'])

    def test_smoke(self):
        rc, msg = self.build()
        self.assertEqual(rc, 0, msg)
        m = omf.load(self.out)[0]
        self.assertEqual(m.dialect, 'borland-pharlap32')
        self.assertEqual(m.fixups[0].size(), 4)
        self.assertEqual(bytes(m.segments[1].data), b'\xb8\x00\x00\x00\x00\xc3')

    def test_default_mode(self):
        rc, msg = self.build(['/ml'])
        self.assertEqual(rc, 0, msg)
        self.assertEqual(omf.load(self.out)[0].dialect, 'intel')

    def test_public_at_zero_preserved(self):
        self.src.write_text(SMOKE.replace('smoke_:', 'nop\nsmoke_:'))
        rc, msg = self.build()
        self.assertNotEqual(rc, 0)
        self.assertIn('no public at offset 0', msg)

    def test_real_assembler_error(self):
        self.src.write_text(SMOKE.replace(' ret', ' not_an_instruction'))
        rc, msg = self.build()
        self.assertNotEqual(rc, 0, msg)
        self.assertFalse(self.out.exists())

    def test_zero_exit_without_fresh_object_refused(self):
        self.out.write_bytes(b'stale object')
        with patch.object(wcc.subprocess, 'run', return_value=subprocess.CompletedProcess([], 0, b'', b'')):
            rc, msg = self.build()
        self.assertNotEqual(rc, 0)
        self.assertIn('new object', msg)
        self.assertEqual(self.out.read_bytes(), b'stale object')

    def test_zero_exit_with_fatal_or_counted_error_refused(self):
        def fake_run(cmd, **kw):
            (Path(kw['cwd']) / 'UNIT.OBJ').write_bytes(module())
            return subprocess.CompletedProcess(cmd, 0, self.diagnostic, b'')
        for diagnostic in (b'Fatal: invalid switch', b'**Error** bad instruction', b'Error messages: 1'):
            self.diagnostic = diagnostic
            with patch.object(wcc.subprocess, 'run', side_effect=fake_run):
                rc, msg = self.build()
            self.assertNotEqual(rc, 0, msg)
            self.assertFalse(self.out.exists())

    def test_timeout_is_failure(self):
        with patch.object(wcc.subprocess, 'run', side_effect=subprocess.TimeoutExpired([], 30)):
            rc, msg = self.build()
        self.assertNotEqual(rc, 0)
        self.assertIn('timed out', msg)

    def test_install_and_runner_hashes_required_each_time(self):
        real_hash = wcc.sha256_file
        for name in ('TASM.EXE', 'msdos.exe'):
            def bad_hash(path, name=name):
                return '0' * 64 if Path(path).name == name else real_hash(path)
            with patch.object(wcc, 'sha256_file', side_effect=bad_hash):
                with self.assertRaisesRegex(SystemExit, 'sha256'):
                    self.build()


class StrictControls(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.img = Image('gog_v121_us')

    def test_hmi_strict_and_adversarial_controls(self):
        # Rebuild the delivered sources. No patched comparator, masks, or bindings.
        prof = wcc.profile('hmi_tasm')
        for name, count in (('hmi_timer_asm', 10), ('hmi_dpmi_asm', 40)):
            with self.subTest(unit=name):
                src = ROOT / 'src/hmi' / (name + '.asm')
                out = wcc.BUILD / (name + '.obj')
                rc, msg = wcc.build(src, out, prof['flags'], prof['install'])
                self.assertEqual(rc, 0, msg)
                m = omf.load(out)[0]
                segments = []
                good = match.verify_object(self.img, m, seg_report=segments)
                self.assertEqual(len(good), count)
                self.assertTrue(all(r['status'] == 'STRICT' for r in good))
                self.assertTrue(all(s['status'] == 'VERIFIED' for s in segments))
                code = next(s for s in m.segments if s and s.cls == 'CODE')
                for control in ('byte', 'site', 'target', 'high_word'):
                    bad = copy.deepcopy(m)
                    if control == 'byte':
                        bad.segments[m.segments.index(code)].data[0] ^= 1
                    elif control == 'site':
                        bad.fixups.pop(0)
                    elif control == 'target':
                        bad.fixups[0].disp += 4
                    else:
                        f = bad.fixups[0]
                        bad.segments[f.seg].data[f.off + 2] ^= 1
                    got = match.verify_object(self.img, bad)
                    self.assertTrue(any(r['status'] != 'STRICT' for r in got), control)
                if name == 'hmi_dpmi_asm':
                    bad = copy.deepcopy(m)
                    next(s for s in bad.segments if s and s.cls == 'DATA').data[0] ^= 1
                    badsegs = []
                    got = match.verify_object(self.img, bad, seg_report=badsegs)
                    self.assertTrue(any(r['status'] != 'STRICT' for r in got))
                    self.assertEqual(badsegs[0]['status'], 'MISMATCH')


if __name__ == '__main__':
    unittest.main()
