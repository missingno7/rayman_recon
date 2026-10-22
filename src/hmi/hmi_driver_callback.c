/* INVENTED descriptive binding. DOS group 1:790ed..791a8. */
#include "hmi_local.h"

extern void __cdecl sosDRVGetCapsPtr(void __far *driver_context,
    void __far *driver_entry, void __far *new_callback_buffer);

void sosDIGICaps_Start(void) {}

s32 sosDIGIGetDeviceCaps(s32 driver, void __far *new_callback_buffer)
{
    if (_lpSOSDriver[driver].pointer == 0)
        return 1;
    if (new_callback_buffer != 0) {
        sosDRVGetCapsPtr(_lpSOSDriverCS[driver].pointer,
            _lpSOSDriver[driver].pointer, new_callback_buffer);
    } else {
        return 2;
    }
    return 0;
}

void sosDIGICaps_End(void) {}
