#ifndef PORT_CD_AUDIO_H
#define PORT_CD_AUDIO_H
void port_cd_audio_reset(void);
void port_cd_audio_pump(int playing, unsigned position, unsigned end);
void port_cd_audio_shutdown(void);
/* Diagnostic for host tests: -1 means no device stream exists. */
int port_cd_audio_queued(void);
#endif
