/*
 * Player settings: a small JSON file next to the program (rayman_sdl3.json), created with the defaults on the
 * first run from the game folder. Port presentation only: nothing here changes what the game computes.
 * Command-line options override the file.
 */
#ifndef PORT_PLATFORM_CONFIG_H
#define PORT_PLATFORM_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define PORT_CONFIG_FILE "rayman_sdl3.json"
#define PORT_CONFIG_EXTRA_DIRS 8

typedef struct port_config {
    int widescreen;        /* wider view drawn by the native renderer (16:9 in a window, the display's shape fullscreen) */
    int square_pixels;     /* 1: each game pixel square (320x200 = 16:10); 0: the original 4:3 picture (pixels 1.2 tall) */
    int fullscreen;
    int window_scale;      /* integer window scale; 0 = automatic */
    int vsync;             /* frames are shown at the display's refreshes (no tearing) */
    char cd[1024];         /* "auto": RAYMAN.cue in the game folder, else a mounted Rayman CD; "none"; "E:"; a .cue path */
    char save_dir[1024];   /* "" = the user's application-data folder */
    char extra_data_dirs[PORT_CONFIG_EXTRA_DIRS][1024];   /* further read-only folders, e.g. one with INTRO.DAT */
    int extra_data_dir_count;
} port_config;

void port_config_defaults(port_config *config);
/* 0: no file (config untouched); 1: loaded; 2: loaded, but the file lacks settings this version knows (the caller
   may rewrite it); -1: not valid JSON or a setting has the wrong type (config holds the defaults). */
int port_config_load(const char *path, port_config *config);
int port_config_save(const char *path, const port_config *config);

#ifdef __cplusplus
}
#endif

#endif
