/* Optional observations only; no movie policy or clock advancement here. */
#ifndef PORT_MOVIE_TRACE_H
#define PORT_MOVIE_TRACE_H
void port_movie_trace_begin(const char *filename, int track, int loop);
void port_movie_trace_frame(int frame, int row, int state, int selected, int width, int height);
void port_movie_trace_fade(void);
void port_movie_trace_end(int status);
#endif
