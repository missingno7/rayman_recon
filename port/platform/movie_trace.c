#include "platform/movie_trace.h"
#include "platform/clock.h"
#include "platform/cd_audio.h"
#include <stdio.h>
#include <stdlib.h>
static FILE *trace;
static void close_trace(void) { if (trace) fclose(trace); trace = NULL; }
static void stamp(const char *event)
{
    fprintf(trace, "%s %llu", event, (unsigned long long)port_clock_ns());
}
void port_movie_trace_begin(const char *filename, int track, int loop)
{
    const char *path = getenv("RAYMAN_MOVIE_TRACE");
    if (!trace && path && *path) {
        trace = fopen(path, "w");
        if (trace) atexit(close_trace);
    }
    if (trace) {
        stamp("begin"); fprintf(trace, " %s track=%d loop=%d\n", filename, track, loop); fflush(trace);
    }
}
void port_movie_trace_frame(int frame, int row, int state, int selected, int width, int height)
{
    if (trace) {
        stamp("frame");
        fprintf(trace, " %d row=%d state=%d selected=%d size=%dx%d cd_queued=%d\n",
                frame, row, state, selected, width, height, port_cd_audio_queued());
        fflush(trace);
    }
}
void port_movie_trace_fade(void)
{
    if (trace) { stamp("fade"); fputc('\n', trace); fflush(trace); }
}
void port_movie_trace_end(int status)
{
    if (trace) { stamp("end"); fprintf(trace, " status=%d\n", status); fflush(trace); }
}
