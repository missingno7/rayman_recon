"""Build-only API reference: unchanged function bodies from src/hmi, with private names.
No driver, DMA or timer code is executed; only the library's voice API operations.
"""
import re
import sys
from pathlib import Path
root, output = map(Path, sys.argv[1:])
groups = {
    "hmi_samples.c": ["sosDIGIStartSample", "sosDIGIStopSample"],
    "hmi_status.c": ["sosDIGISampleDone"],
    "hmi_active.c": ["sosDIGISamplesPlaying"],
    "hmi_voice_params.c": ["sosDIGISetSampleVolume", "sosDIGISetPanLocation"],
}
names = [n for group in groups.values() for n in group]
text = ["/* Generated from the current src/hmi function bodies; never tracked. */",
        "#define __far", '#include "hmi_local.h"', "#include <string.h>"]
text += [f"#define {n} ref_{n}" for n in names]
text += ["hmi_far_value _lpSOSDriver[1];", "WORD _wSOSDriverType[1];",
         "_SOS_SAMPLE *_lpSOSSampleList[1][32];", "static _SOS_SAMPLE state[32];"]
for file, funcs in groups.items():
    source = (root / "src/hmi" / file).read_text()
    for name in funcs:
        match = re.search(r"^(?:int|unsigned) " + name + r"\([^;]+?\)\s*\{", source, re.M)
        if not match:
            raise SystemExit(f"Cannot locate reference {name}")
        pos, depth = match.end(), 1
        while depth:
            depth += (source[pos] == "{") - (source[pos] == "}")
            pos += 1
        text += [source[match.start():pos]]
text += ["""
unsigned hmi_reference_flags(unsigned voice) { return (unsigned short)state[voice].flags.raw; }
void hmi_reference_reset(void)
{
    unsigned i;
    memset(state,0,sizeof state);
    for(i=0;i<32;i++) _lpSOSSampleList[0][i]=&state[i];
}
int hmi_reference_start(unsigned char *data, unsigned flags, unsigned volume, unsigned pan)
{
    _SOS_START_SAMPLE r;
    memset(&r,0,sizeof r);
    r.pointer_00.pointer=data; r.dword_08=20000;
    r.flags_28.raw=flags; r.word_14=volume; r.word_40=pan;
    r.dword_38=65536;
    return ref_sosDIGIStartSample(0,&r);
}
"""]
output.parent.mkdir(parents=True, exist_ok=True)
output.write_text("\n".join(text)+"\n")
