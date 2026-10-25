"""Check that removing store instrumentation preserves the proved C behavior."""
import sys
sys.dont_write_bytecode=True
import ctypes as C
import hashlib
import json
import subprocess
import unittest
import test_lowlevel_native as t


class ReleaseParityTest(unittest.TestCase):
    def test_release(self):
        target=t.WORK/'native/lowlevel_release.dll'
        command=['C:/msys64/mingw64/bin/gcc.exe','-std=c11','-O2','-shared','-fwrapv','-funsigned-char',
                 '-Wall','-Wextra','-Werror',str(t.PORT/'video/lowlevel_normal.c'),
                 str(t.WORK/'native/device_stub.c'),'-o',str(target)]
        result=subprocess.run(command,text=True,capture_output=True,cwd=t.WORK)
        self.assertEqual(result.returncode,0,result.stdout+result.stderr)
        traced=t.Library(t.WORK/'native/lowlevel_normal.dll')
        release=t.Library(target)
        oracle=t.engine.Oracle()
        cases=0
        for name in t.NAMES:
            for i in range(8):
                initial,flags=t.prove.prepare(oracle,name,0xA550000+i)
                images=[t.NativeImage(lib,initial) for lib in (traced,release)]
                outputs=[image.run(name) for image in images]
                self.assertEqual(outputs[0],outputs[1],(name,i,'return/clip result'))
                for rid in range(len(t.STATE)):
                    self.assertEqual(images[0].state_bytes(rid),images[1].state_bytes(rid),(name,i,'state',rid))
                for left,right in zip(images[0].allocations,images[1].allocations):
                    self.assertEqual(C.string_at(left[2],left[1]),C.string_at(right[2],right[1]),(name,i,'memory',left[0]))
                self.assertEqual(traced('test_notifications'),release('test_notifications'))
                cases+=1
        report={'entries':len(t.NAMES),'cases':cases,'mismatches':0,'compiler_command':command,
                'source_sha256':hashlib.sha256((t.PORT/'video/lowlevel_normal.c').read_bytes()).hexdigest(),
                'scope':'untraced release vs original-DOS-proved traced C; supplementary compiler instrumentation check'}
        (t.WORK/'release_results.json').write_text(json.dumps(report,indent=2)+'\n')
        print(f'Release parity: {len(t.NAMES)} entries, {cases} cases, 0 mismatches',flush=True)


if __name__=='__main__':
    unittest.main(verbosity=2)
