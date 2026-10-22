/* INVENTED names; DOS-derived HMI voice status module 1:754ce..755ce. */
#include "hmi_local.h"
void hmi_status_begin(void) {}
unsigned sub_754E3(unsigned driver, unsigned voice)
{
    if (voice < 32) {
        if (!(hmi_voice_slots[driver][voice]->flags.raw & 0x8000))
            return 1;
        else
            return 0;
    } else
        return 10;
}
unsigned hmi_sample_playing(unsigned driver, unsigned voice)
{
    if (voice < 32) {
        if (hmi_voice_slots[driver][voice]->flags.raw & 0x1000)
            return 0;
        if (hmi_voice_slots[driver][voice]->flags.raw & 0x8000)
            return 1;
        else
            return 0;
    } else
        return 10;
}
void hmi_status_end(void) {}
