/* INVENTED descriptive binding. DOS group 1:79769..797f5. */
#include "hmi_local.h"

void hmi_callback_set_begin(void) {}

s32 hmi_set_driver_callback(s32 driver, s32 callback)
{
    void __far *entrypoint;

    if (hmi_loaded_driver_handles[driver] == 0)
        return 1;
    hmi_driver_callback_values[driver] = callback;
    /* HYPOTHESIS: DOS copies the far entrypoint into this local, then returns. */
    entrypoint = hmi_driver_entrypoints[driver].pointer;
    return 0;
}

void hmi_callback_set_end(void) {}
