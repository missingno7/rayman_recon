/* INVENTED descriptive binding. DOS group 1:79769..797f5. */
#include "hmi_local.h"

void sosDIGIRate_Start(void) {}

s32 sosDIGISetOutputRate(s32 driver, s32 callback)
{
    void __far *entrypoint;

    if (_wSOSDriverLoaded[driver] == 0)
        return 1;
    _wSOSOutputRate[driver] = callback;
    /* HYPOTHESIS: DOS copies the far entrypoint into this local, then returns. */
    entrypoint = _lpSOSDriver[driver].pointer;
    return 0;
}

void sosDIGIRate_End(void) {}
