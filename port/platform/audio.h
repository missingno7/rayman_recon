/* One playback device, shared by independently queued CD and HMI presentation streams. */
#ifndef PORT_AUDIO_H
#define PORT_AUDIO_H
#include <SDL3/SDL.h>
SDL_AudioStream *port_audio_stream(SDL_AudioFormat format, int channels, int rate);
int port_audio_rate(void);
int port_audio_spec(SDL_AudioSpec *spec);
void port_audio_shutdown(void);
#endif
