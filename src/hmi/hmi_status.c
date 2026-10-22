/* SDK public names; DOS-derived HMI voice status module 1:754ce..755ce. */
#include "hmi_local.h"
void sosDIGIDone_Start(void) {}
unsigned sosDIGISampleDone(unsigned driver, unsigned voice)
{
    if (voice < 32) {
        if (!(_lpSOSSampleList[driver][voice]->flags.raw & 0x8000))
            return 1;
        else
            return 0;
    } else
        return 10;
}
unsigned sosDIGISampleFilling(unsigned driver, unsigned voice)
{
    if (voice < 32) {
        if (_lpSOSSampleList[driver][voice]->flags.raw & 0x1000)
            return 0;
        if (_lpSOSSampleList[driver][voice]->flags.raw & 0x8000)
            return 1;
        else
            return 0;
    } else
        return 10;
}
void sosDIGIDone_End(void) {}
