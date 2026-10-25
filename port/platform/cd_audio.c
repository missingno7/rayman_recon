/* Main-thread CD-DA feeder. The device clock determines position, never the SDL queue.
   No callback, thread, or device exists in headless/deterministic mode. */
#include "platform/cd_audio.h"
#include "platform/cdrom.h"
#include "platform/clock.h"
#include "platform/host.h"
#include <SDL3/SDL.h>
#include <stdio.h>

static SDL_AudioStream *stream;
static uint64_t next_sample;
static int failed;

int port_cd_audio_queued(void)
{
    return stream ? SDL_GetAudioStreamQueued(stream) : -1;
}

void port_cd_audio_shutdown(void)
{
    if (stream) SDL_DestroyAudioStream(stream);
    stream = NULL;
    failed = 0;
}

void port_cd_audio_reset(void)
{
    if (stream) SDL_ClearAudioStream(stream);
    next_sample = UINT64_MAX;
}

void port_cd_audio_pump(int playing, unsigned position, unsigned end)
{
    int16_t pcm[588 * 2 * 8];
    uint64_t current = (uint64_t)position * 588;
    uint64_t limit = (uint64_t)end * 588;
    size_t count;
    if (port_host_headless() || port_clock_mode() == PORT_CLOCK_DETERMINISTIC || failed)
        return;
    if (!playing) { port_cd_audio_reset(); return; }
    if (!stream) {
        SDL_AudioSpec spec = {SDL_AUDIO_S16LE, 2, 44100};
        if (!SDL_InitSubSystem(SDL_INIT_AUDIO) ||
            !(stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL))) {
            fprintf(stderr, "CD audio device: %s\n", SDL_GetError());
            failed = 1;
            return;
        }
        SDL_ResumeAudioStreamDevice(stream);
        next_sample = current;
    }
    /* Catch up after a long host stall; never play a backlog after the device clock. */
    if (next_sample == UINT64_MAX || next_sample < current || next_sample > current + 588 * 16) {
        SDL_ClearAudioStream(stream);
        next_sample = current;
    }
    if (limit > current + 588 * 8) limit = current + 588 * 8;
    while (next_sample < limit &&
           SDL_GetAudioStreamQueued(stream) < (int)sizeof pcm) {
        count = (size_t)(limit - next_sample);
        if (count > 588 * 8) count = 588 * 8;
        count = port_cd_pcm(next_sample, pcm, count);
        if (!count) break;
        if (!SDL_PutAudioStreamData(stream, pcm, (int)(count * 4))) break;
        next_sample += count;
    }
}
