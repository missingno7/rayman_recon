#include "platform/ogg.h"

#include <stdlib.h>

#define STB_VORBIS_NO_PUSHDATA_API
#include STB_VORBIS_SOURCE   /* the pinned stb_vorbis.c (CMakeLists.txt RAY_STB_VORBIS) */

struct port_ogg {
    stb_vorbis *decoder;
    int channels;
    uint64_t next;   /* the frame the decoder delivers next */
};

port_ogg *port_ogg_open(const char *path, uint64_t *frames)
{
    int error = 0;
    stb_vorbis *v = stb_vorbis_open_filename(path, &error, NULL);
    stb_vorbis_info info;
    port_ogg *ogg;
    if (!v)
        return NULL;
    info = stb_vorbis_get_info(v);
    if (info.sample_rate != 44100 || info.channels < 1 || info.channels > 2 || !(ogg = malloc(sizeof *ogg))) {
        stb_vorbis_close(v);
        return NULL;
    }
    ogg->decoder = v;
    ogg->channels = info.channels;
    ogg->next = 0;
    *frames = stb_vorbis_stream_length_in_samples(v);
    return ogg;
}

size_t port_ogg_read(port_ogg *ogg, uint64_t at, int16_t *out, size_t frames)
{
    size_t done = 0;
    if (at != ogg->next) {
        if (at > 0xffffffffu || !stb_vorbis_seek(ogg->decoder, (unsigned int)at))
            return 0;
        ogg->next = at;
    }
    while (done < frames) {
        int got = stb_vorbis_get_samples_short_interleaved(ogg->decoder, 2, out + 2 * done, (int)(frames - done) * 2);
        if (got <= 0)
            break;
        done += (size_t)got;
    }
    ogg->next += done;
    return done;
}

void port_ogg_close(port_ogg *ogg)
{
    if (!ogg)
        return;
    stb_vorbis_close(ogg->decoder);
    free(ogg);
}
