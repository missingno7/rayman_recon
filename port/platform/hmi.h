#ifndef PORT_HMI_H
#define PORT_HMI_H
#include <stddef.h>
#include <stdint.h>
#include "game/hmi_types.h"
s32 sosDIGIInitSystem(s32,s32,s32);
s32 sosDIGIUnInitSystem(void);
s32 sosDIGIInitDriver(s32,s32,s32 *,void *,void *);
s32 sosDIGIUnInitDriver(s32,s32,s32);
s32 sosDIGIDetectInit(s32,s16);
s32 sosDIGIDetectUnInit(void);
s32 sosDIGIDetectFindFirst(const char *,void *);
s32 sosDIGIDetectGetSettings(s32 *);
s32 sosDIGIStartSample(s32,dos_sound_request_t *);
s32 sosDIGIStopSample(s32,s16);
s32 sosDIGISampleDone(s32,s16);
s32 sosDIGISamplesPlaying(s32);
s32 sosDIGISetSampleVolume(s32,s16,s32);
s32 sosDIGISetPanLocation(s32,s16,s32);
s32 sosTIMERInitSystem(s32,s32);
s32 sosTIMERUnInitSystem(s32);
s32 sosTIMERRegisterEvent(s32,void *,void *);
s32 sosTIMERRemoveEvent(s32);
void port_hmi_pump(void);
void port_hmi_shutdown(void);
int port_hmi_audio_queued(void);
/* Pure clock-position and mixer inspection, also used by unit tests. */
uint64_t port_hmi_frame(unsigned voice, uint64_t ns);
void port_hmi_mix(float *stereo, size_t frames, unsigned rate, uint64_t ns);
uint32_t port_hmi_flags(unsigned voice);
#endif
