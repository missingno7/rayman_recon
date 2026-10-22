/* INVENTED names. DOS-derived HMI active-voice module, 1:71afa..71b83.
 * Compile separately from the game: Watcom -od -s -3r.
 * Empty begin/end routines recur around HMI modules; retain their real extents. */
#include "hmi_local.h"

void sosDIGIPlyng_Start(void) {}
unsigned sosDIGISamplesPlaying(unsigned driver)
{
    unsigned voice;
    unsigned count;
    count = 0;
    for (voice = 0; voice < 32; voice++) {
        if (_lpSOSSampleList[driver][voice]->flags.raw & 0x8000)
            count++;
    }
    return count;
}
void sosDIGIPlyng_End(void) {}
