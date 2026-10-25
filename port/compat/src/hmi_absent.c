/*
 * The HMI SOS sound library entry points the game calls (src/hifi.c, src/menu.c LOAD_CONFIG, src/movie.c),
 * answered as an installation without a sound driver: the system/timer services initialise, the driver does
 * not, and LOAD_CONFIG then takes the game's own no-sound-card path (CarteSonAutorisee = 0). The port's audio
 * backend replaces this file behind the same entry points (port/docs/BOUNDARY.md). A call that only a running
 * driver could receive stops the program.
 */
#include "compat/src/pc_hardware.h"

#include "rtypes.h"

/* Any nonzero code takes the game's failure branch (sub_3E780 / sub_3E710). */
#define HMI_NO_DRIVER 1

s32 sosDIGIInitSystem(s32 a1, s32 a2, s32 a3) { (void)a1; (void)a2; (void)a3; return 0; }
s32 sosDIGIUnInitSystem(void) { return 0; }
s32 sosTIMERInitSystem(s32 a1, s32 a2) { (void)a1; (void)a2; return 0; }
s32 sosTIMERUnInitSystem(s32 a1) { (void)a1; return 0; }
s32 sosDIGIInitDriver(s32 device_id, s32 timeout, s32 *port, void *config, void *handle)
{
    (void)device_id; (void)timeout; (void)port; (void)config; (void)handle;
    return HMI_NO_DRIVER;
}
s32 sosDIGIDetectInit(s32 a1, s16 a2) { (void)a1; (void)a2; return HMI_NO_DRIVER; }
s32 sosDIGIDetectUnInit(void) { return 0; }

static s32 needs_driver(const char *name)
{
    port_unimplemented(name);
    return 1;
}
s32 sosDIGIDetectFindFirst(const char *name, void *buffer) { (void)name; (void)buffer; return needs_driver("sosDIGIDetectFindFirst without a driver"); }
s32 sosDIGIDetectGetSettings(s32 *value) { (void)value; return needs_driver("sosDIGIDetectGetSettings without a driver"); }
s32 sosDIGIUnInitDriver(s32 driver, s32 enabled, s32 flags) { (void)driver; (void)enabled; (void)flags; return needs_driver("sosDIGIUnInitDriver without a driver"); }
s32 sosDIGIStartSample(s32 driver, void *request) { (void)driver; (void)request; return needs_driver("sosDIGIStartSample without a driver"); }
s32 sosDIGIStopSample(s32 driver, s16 voice) { (void)driver; (void)voice; return needs_driver("sosDIGIStopSample without a driver"); }
s32 sosDIGISetSampleVolume(s32 driver, s16 voice, s32 volume) { (void)driver; (void)voice; (void)volume; return needs_driver("sosDIGISetSampleVolume without a driver"); }
s32 sosDIGISetPanLocation(s32 driver, s16 voice, s32 pan) { (void)driver; (void)voice; (void)pan; return needs_driver("sosDIGISetPanLocation without a driver"); }
s32 sosDIGISampleDone(s32 driver, s16 voice) { (void)driver; (void)voice; return needs_driver("sosDIGISampleDone without a driver"); }
s32 sosDIGISamplesPlaying(s32 driver) { (void)driver; return needs_driver("sosDIGISamplesPlaying without a driver"); }
s32 sosTIMERRegisterEvent(s32 rate, void *callback, void *handle) { (void)rate; (void)callback; (void)handle; return needs_driver("sosTIMERRegisterEvent without a driver"); }
s32 sosTIMERRemoveEvent(s32 handle) { (void)handle; return needs_driver("sosTIMERRemoveEvent without a driver"); }
