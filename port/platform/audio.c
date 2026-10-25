#include "platform/audio.h"
#include "platform/clock.h"
#include "platform/host.h"
#include <stdio.h>
static SDL_AudioDeviceID device;
static int rate, failed;
static SDL_AudioSpec chosen;
SDL_AudioStream *port_audio_stream(SDL_AudioFormat format, int channels, int hz)
{
    SDL_AudioSpec output, input = {format, channels, hz};
    SDL_AudioStream *s;
    int frames;
    if (port_host_headless() || port_clock_mode() == PORT_CLOCK_DETERMINISTIC || failed) return NULL;
    if (!device) {
        if (!SDL_InitSubSystem(SDL_INIT_AUDIO) ||
            !(device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL))) goto fail;
        if (!SDL_GetAudioDeviceFormat(device, &output, &frames)) goto fail;
        chosen = output;
        rate = output.freq;
        fprintf(stderr, "SDL audio device: %d Hz, %d channels, format 0x%x, %d frames (shared CD/HMI)\n",
                rate, output.channels, output.format, frames);
    }
    if (!hz) input.freq = rate;
    s = SDL_CreateAudioStream(&input, NULL);
    if (!s) goto fail;
    if (!SDL_BindAudioStream(device, s)) { SDL_DestroyAudioStream(s); goto fail; }
    SDL_ResumeAudioDevice(device);
    return s;
fail:
    fprintf(stderr, "SDL audio: %s\n", SDL_GetError());
    failed = 1;
    return NULL;
}
int port_audio_rate(void) { return rate; }
int port_audio_spec(SDL_AudioSpec *spec) { if (!device) return 0; *spec=chosen; return 1; }
void port_audio_shutdown(void)
{
    if (device) SDL_CloseAudioDevice(device);
    device = 0; rate = failed = 0;
}
