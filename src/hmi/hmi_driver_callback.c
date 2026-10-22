/* INVENTED descriptive binding. DOS group 1:790ed..791a8. */
#include "hmi_local.h"

extern void __cdecl hmi_driver_op8_copy_selectors(void __far *driver_context,
    void __far *driver_entry, void __far *new_callback_buffer);

void hmi_driver_callback_begin(void) {}

s32 hmi_driver_callback_update(s32 driver, void __far *new_callback_buffer)
{
    if (hmi_driver_entrypoints[driver].pointer == 0)
        return 1;
    if (new_callback_buffer != 0) {
        hmi_driver_op8_copy_selectors(hmi_driver_contexts[driver].pointer,
            hmi_driver_entrypoints[driver].pointer, new_callback_buffer);
    } else {
        return 2;
    }
    return 0;
}

void hmi_driver_callback_end(void) {}
