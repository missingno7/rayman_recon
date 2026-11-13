/*
 * Ogg Vorbis audio tracks of a CD image (GOG's game.ins names Music\rayman02.ogg .. rayman20.ogg as tracks 2..20):
 * decoded to the CD's own format, 44.1 kHz 16-bit stereo, at any frame position. stb_vorbis (public domain, pinned
 * in CMakeLists.txt) does the decoding.
 */
#ifndef PORT_PLATFORM_OGG_H
#define PORT_PLATFORM_OGG_H

#include <stddef.h>
#include <stdint.h>

typedef struct port_ogg port_ogg;

/* NULL when the file is not an Ogg Vorbis stream at 44.1 kHz with one or two channels. *frames: its length in
   sample frames. */
port_ogg *port_ogg_open(const char *path, uint64_t *frames);
/* Up to `frames` stereo frames from frame `at` (interleaved, mono doubled); fewer at the end. Sequential reads
   continue the decoder; another position seeks. */
size_t port_ogg_read(port_ogg *ogg, uint64_t at, int16_t *out, size_t frames);
void port_ogg_close(port_ogg *ogg);

#endif
