/*
 * Process entry of the native port: options, platform start-up, then one of the game entries (port_main.h).
 *
 *   rayman [--data DIR] [--save DIR] [--cd PATH.cue] [--cd-drive D] [--demo N | --menus | --dos-main] [--headless] [--deterministic]
 *          [--frames N] [--frame-log FILE] [--dump-frames DIR --dump-every N] [--epoch SECONDS]
 *          [--scale N] [--fullscreen] [--aspect square|original] [--widescreen | --wide-pad N] [--verify-render] [--state-log FILE]
 *          [--renderer native|reference]
 *
 * --keys FILE queues scan codes at given presented frames (tests).
 * --epoch sets the wall-clock time a deterministic run starts at (the game seeds its random table from time()).
 *
 * --frames/--frame-log exist for regression runs: stop after N presented images, log one hash per image.
 *
 * --aspect square|original: square game pixels (320x200 shown as 16:10; the default, also "square_pixels" in the
 * settings file) or the original 4:3 picture (each pixel 1.2 times as tall).
 * --widescreen (16:9: 18 more columns per side with square pixels, 54 with the original aspect) / --wide-pad N (0..160)
 * make the native renderer draw a wider view
 * of the same frames (implies --renderer native; port/docs/RENDERER.md 9). --verify-render checks the native
 * renderer against the original draw path every frame (original width only); --state-log writes one gameplay-state digest per
 * frame swap (presentation-independence tests). By default the native renderer's image is presented, and the
 * original draw path's image only while a frame the native renderer could not draw completely is on screen;
 * --renderer native presents only the native image (gaps visible), --renderer reference only the original draw
 * path's display (port/docs/RENDERER.md). Frame logs hash the presented image.
 */
#include "game/port_main.h"
#include "platform/clock.h"
#include "platform/files.h"
#include "platform/cdrom.h"
#include "platform/hmi.h"
#include "platform/audio.h"
#include "platform/host.h"
#include "platform/config.h"
#include "platform/cd_host.h"
#include "video/vga.h"
#include "render/capture.h"
#include "render/record.h"

#include <time.h>
#ifdef __ANDROID__
#define PORT_ENTRY_ALWAYS_FULLSCREEN 1
#else
#define PORT_ENTRY_ALWAYS_FULLSCREEN 0
#endif
time_t port_time(time_t *out);
void port_set_time_epoch(long long seconds);

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern unsigned char JeuCracker, MusicCdActive, CarteSonAutorisee;
/* Pacing state of the original main loop (RAYMAN_PACE_TRACE). */
extern unsigned char need_timer, use_sync, Frequence, VGA_FREQ;
extern short display_mode, xmap;
extern unsigned short word_CF7F0, word_CF7E6;
extern int map_time;
static FILE *pace_trace;
extern short num_world, num_level;
static long frame_limit = -1;
static long frame_count;
static FILE *frame_log;
static FILE *state_log;
static const char *dump_dir;
static long dump_every = 0;

/* Scripted keyboard input for tests: "MILLISECONDS SCANCODE" lines (port-clock time, hex set-1 codes, 0x80 bit =
   release), queued once the clock reaches the time - the same queue SDL key events feed. */
static struct { long frame; int code; } key_script[4096];
static int key_count, key_next;

static void load_key_script(const char *path)
{
    FILE *f = fopen(path, "r");
    long frame;
    unsigned code;
    if (!f) {
        fprintf(stderr, "rayman: cannot read key script %s\n", path);
        exit(2);
    }
    while (key_count < 4096 && fscanf(f, "%ld %x", &frame, &code) == 2) {
        key_script[key_count].frame = frame;
        key_script[key_count].code = (int)code;
        key_count++;
    }
    fclose(f);
}

/* 320x200 8-bit BMP with the DAC palette (6-bit components scaled like the host presentation). */
static void dump_frame(const uint8_t *pixels, int width, const uint8_t *dac)
{
    char path[1024];
    uint8_t header[54 + 1024];
    FILE *f;
    int i, y;
    uint32_t size = 54 + 1024 + (uint32_t)width * 200;
    snprintf(path, sizeof path, "%s/frame_%06ld.bmp", dump_dir, frame_count);
    if (!(f = fopen(path, "wb")))
        return;
    memset(header, 0, sizeof header);
    header[0] = 'B'; header[1] = 'M';
    memcpy(header + 2, &size, 4);
    header[10] = 54; header[11] = 4;                     /* pixel data offset 54 + 1024 */
    header[14] = 40;
    header[18] = (uint8_t)(width & 255); header[19] = (uint8_t)(width >> 8);
    header[22] = 200;
    header[26] = 1; header[28] = 8;
    for (i = 0; i < 256; i++) {
        uint8_t r = dac[3 * i] & 63, g = dac[3 * i + 1] & 63, b = dac[3 * i + 2] & 63;
        header[54 + 4 * i] = (uint8_t)(b << 2 | b >> 4);
        header[54 + 4 * i + 1] = (uint8_t)(g << 2 | g >> 4);
        header[54 + 4 * i + 2] = (uint8_t)(r << 2 | r >> 4);
    }
    fwrite(header, 1, sizeof header, f);
    for (y = 199; y >= 0; y--)
        fwrite(pixels + y * width, 1, (size_t)width, f);
    fclose(f);
}

/* FNV-1a over the presented pixels (width x 200) and the 768 DAC bytes. */
static void on_frame(const uint8_t *pixels, int width, const uint8_t *dac, void *user)
{
    (void)user;
    if (frame_log) {
        uint64_t h = 1469598103934665603ULL;
        int i;
        for (i = 0; i < width * 200; i++)
            h = (h ^ pixels[i]) * 1099511628211ULL;
        for (i = 0; i < 768; i++)
            h = (h ^ dac[i]) * 1099511628211ULL;
        fprintf(frame_log, "%ld %llu %016llx\n", frame_count, (unsigned long long)port_clock_ns(),
                (unsigned long long)h);
        fflush(frame_log); /* Preserve completed frames across another port boundary crash. */
    }
    if (dump_dir && dump_every > 0 && frame_count % dump_every == 0)
        dump_frame(pixels, width, dac);
    frame_count++;
    if (frame_limit >= 0 && frame_count >= frame_limit) {
        if (frame_log)
            fclose(frame_log);
        frame_log = NULL;
        fflush(stdout);
        exit(0);
    }
    if (port_host_quit_requested())
        exit(0);
}

/* One gameplay-state digest per frame swap: the frames the game builds, whichever renderer presents them and however
   many of them are presented. */
static void on_swap(void)
{
    static long swaps;
    static uint64_t native_seen;
    if (state_log) {
        fprintf(state_log, "%ld %016llx\n", swaps, (unsigned long long)port_scene_state_digest());
        fflush(state_log);
    }
    /* One line per built frame: when, the engine's step counter and camera, and the original loop's pacing mode. */
    if (pace_trace) {
        uint64_t late, max_late, restarts, shown_at, shown_count;
        int locked;
        port_clock_stats(&late, &max_late, &restarts, &locked);
        shown_at = port_host_shown(&shown_count);
        fprintf(pace_trace, "%ld %llu map_time=%d xmap=%d display_mode=%d need_timer=%u use_sync=%u Frequence=%u "
                            "VGA_FREQ=%u frame_ticks=%u deadline_ticks=%u late_us=%llu max_late_us=%llu restarts=%llu locked=%d "
                            "shown_ns=%llu shown=%llu present_us=%llu host_ns=%llu native_us=%llu\n", swaps,
                (unsigned long long)port_clock_ns(), map_time, xmap, display_mode, need_timer, use_sync, Frequence,
                VGA_FREQ, word_CF7F0, word_CF7E6, (unsigned long long)(late / 1000),
                (unsigned long long)(max_late / 1000), (unsigned long long)restarts, locked,
                (unsigned long long)shown_at, (unsigned long long)shown_count,
                (unsigned long long)(port_host_present_cost_ns() / 1000),
                (unsigned long long)SDL_GetTicksNS(),
                (unsigned long long)((port_record_native_ns() - native_seen) / 1000));
        native_seen = port_record_native_ns();
    }
    swaps++;
}

/* Called by the host pump: release scripted keys whose time has come. */
static void key_script_pump(void)
{
    long now_ms = (long)(port_clock_ns() / 1000000ULL);
    while (key_next < key_count && key_script[key_next].frame <= now_ms)
        port_host_inject_scancode((uint8_t)key_script[key_next++].code);
}

static void at_exit(void)
{
    /* Headless regression runs: a one-line summary of where the game ended. */
    if (port_host_headless()) {
        printf("port exit: JeuCracker=%u MusicCdActive=%u frames=%ld world=%d level=%d CarteSonAutorisee=%u\n",
               JeuCracker, MusicCdActive, frame_count, num_world, num_level, CarteSonAutorisee);
        port_record_report();
    }
    port_hmi_shutdown();
    port_cd_shutdown();
    port_audio_shutdown();
    port_host_shutdown();
}

/* The game itself; with a window it runs on its own thread (platform/host.h port_host_run). */
static struct game_args { int dos_main, demo, argc; char **argv; } start;

static int run_game(void *arg)
{
    struct game_args *g = (struct game_args *)arg;
    if (g->dos_main)
        return port_run_dos_main(g->argc, g->argv);
    if (g->demo >= 0)
        return port_run_demo(g->demo);
    return port_run_menus();
}

int main(int argc, char **argv)
{
    const char *data = getenv("RAYMAN_DATA");
    const char *save = NULL;
    const char *data_roots[32];
    int data_count = 0;
    const char *cd = getenv("RAYMAN_CD");
    const char *cd_drive = getenv("RAYMAN_CD_DRIVE");
    char cd_letter = cd_drive && *cd_drive ? cd_drive[0] : 'D';
    char default_save[1024];
    port_host_options host = {0, 0, 0, 320, 1};
    int deterministic = 0, demo = -1, dos_main = 0, i;
    int renderer = -1, wide_pad = 0;   /* renderer: -1 default (native with fallback), 0 reference, 1 native */
    const char *config_path = NULL;
    static port_config config;
    static char base_dir[1024], base_config[1100], base_cue[1100], host_cd[3];
    int have_config = 0, no_vsync = 0;
    int want_wide = 0, config_wide = 0, square = -1;   /* square: -1 = settings file or default (square), 0 original 4:3, 1 square */

    setvbuf(stdout, NULL, _IONBF, 0);
    if (getenv("RAYMAN_PACE_TRACE") && *getenv("RAYMAN_PACE_TRACE"))
        pace_trace = fopen(getenv("RAYMAN_PACE_TRACE"), "w");
    for (i = 1; i < argc; i++) {
        const char *a = argv[i];
        const char *v = i + 1 < argc ? argv[i + 1] : NULL;
        if (!strcmp(a, "--data") && v) {
            if (data_count == 32) { fprintf(stderr, "rayman: at most 32 --data directories\n"); return 2; }
            data_roots[data_count++] = argv[++i];
        }
        else if (!strcmp(a, "--save") && v) save = argv[++i];
        else if (!strcmp(a, "--cd") && v) cd = argv[++i];
        else if (!strcmp(a, "--cd-drive") && v && strlen(v)==1) cd_letter = argv[++i][0];
        else if (!strcmp(a, "--demo") && v) demo = atoi(argv[++i]);
        else if (!strcmp(a, "--menus")) demo = -1;
        else if (!strcmp(a, "--dos-main")) dos_main = 1;
        else if (!strcmp(a, "--headless")) host.headless = 1;
        else if (!strcmp(a, "--deterministic")) deterministic = 1;
        else if (!strcmp(a, "--frames") && v) frame_limit = atol(argv[++i]);
        else if (!strcmp(a, "--frame-log") && v) frame_log = fopen(argv[++i], "w");
        else if (!strcmp(a, "--dump-frames") && v) dump_dir = argv[++i];
        else if (!strcmp(a, "--dump-every") && v) dump_every = atol(argv[++i]);
        else if (!strcmp(a, "--keys") && v) load_key_script(argv[++i]);
        else if (!strcmp(a, "--epoch") && v) port_set_time_epoch(atoll(argv[++i]));
        else if (!strcmp(a, "--scale") && v) host.scale = atoi(argv[++i]);
        else if (!strcmp(a, "--fullscreen")) host.fullscreen = 1;
        else if (!strcmp(a, "--no-vsync")) no_vsync = 1;
        else if (!strcmp(a, "--widescreen")) want_wide = 1;   /* 16:9 at the chosen pixel shape (below) */
        else if (!strcmp(a, "--aspect") && v) {
            const char *r = argv[++i];
            if (!strcmp(r, "square")) square = 1;
            else if (!strcmp(r, "original")) square = 0;
            else { fprintf(stderr, "rayman: --aspect square|original\n"); return 2; }
        }
        else if (!strcmp(a, "--wide-pad") && v) wide_pad = atoi(argv[++i]);
        else if (!strcmp(a, "--verify-render")) port_record_set_verify(1);
        else if (!strcmp(a, "--renderer") && v) {
            const char *r = argv[++i];
            if (!strcmp(r, "native")) renderer = 1;
            else if (!strcmp(r, "reference")) renderer = 0;
            else { fprintf(stderr, "rayman: --renderer native|reference\n"); return 2; }
        }
        else if (!strcmp(a, "--state-log") && v) state_log = fopen(argv[++i], "w");
        else if (!strcmp(a, "--config") && v) config_path = argv[++i];
        else {
            fprintf(stderr, "unknown option %s\n", a);
            return 2;
        }
    }
    /* Started from the game folder (no data directory named): the program's own directory is the game, and the
       player's settings file lives next to the program - created with the defaults on the first run. Runs that
       name their data (tests, development) read a settings file only when --config names one. */
    if (!data_count && !data) {
#ifdef __ANDROID__
        /* Android (port/docs/ANDROID.md): the game folder is the app's external files folder,
           Android/data/<package>/files/, which the player fills over USB or adb. */
        const char *base = SDL_GetAndroidExternalStoragePath();
        static char log_path[1100];
        snprintf(base_dir, sizeof base_dir, "%s/", base ? base : "");
        snprintf(log_path, sizeof log_path, "%srayman_log.txt", base_dir);
        if (base) {   /* stdout/stderr go nowhere on Android */
            FILE *marker;
            freopen(log_path, "w", stdout);
            freopen(log_path, "a", stderr);
            setvbuf(stdout, NULL, _IONBF, 0);
            /* No environment on Android: a file pace_trace.on in the game folder turns RAYMAN_PACE_TRACE on
               (written to pace_trace.txt there). */
            snprintf(log_path, sizeof log_path, "%space_trace.on", base_dir);
            if (!pace_trace && (marker = fopen(log_path, "rb")) != NULL) {
                fclose(marker);
                snprintf(log_path, sizeof log_path, "%space_trace.txt", base_dir);
                pace_trace = fopen(log_path, "w");
            }
        }
#else
        const char *base = SDL_GetBasePath();
#endif
        FILE *probe;
#ifndef __ANDROID__
        snprintf(base_dir, sizeof base_dir, "%s", base ? base : "");
#endif
        snprintf(base_cue, sizeof base_cue, "%sRAY.LNG", base_dir);
        if (base && (probe = fopen(base_cue, "rb")) != NULL) {
            fclose(probe);
            data = base_dir;
            if (!config_path) {
                snprintf(base_config, sizeof base_config, "%s%s", base_dir, PORT_CONFIG_FILE);
                config_path = base_config;
            }
        }
#ifdef __ANDROID__
        else {
            char message[1400];
            snprintf(message, sizeof message,
                     "Copy the files of your Rayman (GOG v1.21) game folder - RAY.LNG, RAYMAN.EXE, PCMAP, ... - "
                     "and your CD image RAYMAN.cue + RAYMAN.bin into\n\n%s\n\nthen start the app again.", base_dir);
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Rayman: game data needed", message, NULL);
            return 2;
        }
#endif
    }
    if (config_path) {
        int state = port_config_load(config_path, &config);
        if (state < 0)
            fprintf(stderr, "rayman: %s is not a valid settings file; using the defaults (delete it to get a fresh "
                            "one)\n", config_path);
        else if (state != 1) {   /* missing, or written by an earlier version: (re)write with every setting */
            if (state == 0) port_config_defaults(&config);
            if (port_config_save(config_path, &config))
                printf("Settings file %s: %s\n", state ? "updated" : "created", config_path);
        }
        have_config = 1;
        if (square < 0) square = config.square_pixels;
        if (config.widescreen && !wide_pad && !want_wide && renderer != 0 && !port_record_verify())
            config_wide = 1;
        if (config.fullscreen) host.fullscreen = 1;
        host.vsync = config.vsync;
        if (!host.scale) host.scale = config.window_scale;
        if (!save && config.save_dir[0]) save = config.save_dir;
        if (!cd && strcmp(config.cd, "auto") && strcmp(config.cd, "none")) cd = config.cd;
        if (!cd && !strcmp(config.cd, "auto")) {
            /* The CD: an image next to the program, else a mounted Rayman disc. */
            FILE *probe;
            snprintf(base_cue, sizeof base_cue, "%sRAYMAN.cue", base_dir);
            if (base_dir[0] && (probe = fopen(base_cue, "rb")) != NULL) {
                fclose(probe);
                cd = base_cue;
            } else if ((host_cd[0] = port_cd_host_find()) != 0) {
                host_cd[1] = ':';
                cd = host_cd;
            }
        }
    }
    if (no_vsync)
        host.vsync = 0;
    /* Pixel shape (user decision 2026-11-12: square by default): the picture is 320 x 200 game pixels shown as 16:10,
       or as the original 4:3 (pixels 1.2 times as tall). */
    if (square < 0) square = 1;
    host.display_rows = square ? 200 : 240;
    if ((want_wide || config_wide) && !wide_pad) {
        /* 16:9 at this pixel shape: 18 columns per side for square pixels, 54 for the original 4:3 */
        wide_pad = (host.display_rows * 16 + 8) / 9;              /* the 16:9 width, rounded */
        wide_pad = (wide_pad - 320 + 1) / 2;
        if (config_wide && (host.fullscreen || PORT_ENTRY_ALWAYS_FULLSCREEN)) {
            /* Fullscreen: as wide as the display (up to 160 columns per side), e.g. 57 square or 100 original on a
               3120x1440 phone. */
            int fit = port_host_fit_pad(160, host.display_rows);
            if (fit >= 0) wide_pad = fit;
        }
        printf("Widescreen: %d columns per side (%s pixels)\n", wide_pad, square ? "square" : "4:3");
    }
    if (wide_pad > 0) {
        /* A wider view is the native renderer's: the original draw path has 320 columns. Its verification compares
           the original view (render/record.c), so it runs at the original width. */
        if (renderer == 0) {
            fprintf(stderr, "rayman: --widescreen needs the native renderer (not --renderer reference)\n");
            return 2;
        }
        port_record_set_pad(wide_pad);   /* the default renderer or --renderer native: both are native */
    }
    if (port_record_pad() && port_record_verify()) {
        fprintf(stderr, "rayman: --verify-render compares the original 320-column view; not with --widescreen\n");
        return 2;
    }
    /* default: the native renderer, falling back to the original image for frames it cannot draw completely */
    port_record_set_native(renderer == 1 ? 1 : renderer == 0 ? 0 : 2);
    host.width = 320 + 2 * port_record_pad();
    if (data_count) data = data_roots[0]; /* Explicit roots replace RAYMAN_DATA. */
    if (!data) {
        fprintf(stderr, "rayman: put the program in the original game directory, or give it with --data DIR "
                        "(or RAYMAN_DATA)\n");
        return 2;
    }
    if (!save) {
        char *pref = SDL_GetPrefPath("Ubisoft", "Rayman (native port)");
        snprintf(default_save, sizeof default_save, "%s", pref ? pref : "rayman-save");
        SDL_free(pref);
        save = default_save;
    }
    if (!port_files_init(data, save)) {
        fprintf(stderr, "rayman: data directory %s or save directory %s is not usable\n", data, save);
        return 2;
    }
    for (i = 1; i < data_count; ++i) {
        if (!port_files_add_data(data_roots[i])) {
            fprintf(stderr, "rayman: data directory %s is not usable\n", data_roots[i]);
            return 2;
        }
    }
    for (i = 0; have_config && i < config.extra_data_dir_count; ++i) {
        if (!port_files_add_data(config.extra_data_dirs[i])) {
            fprintf(stderr, "rayman: data directory %s (settings file) is not usable\n", config.extra_data_dirs[i]);
            return 2;
        }
    }
    port_clock_init(deterministic ? PORT_CLOCK_DETERMINISTIC : PORT_CLOCK_REALTIME);
    if (!port_host_init(&host))
        return 1;
    atexit(at_exit);
    if (!port_cd_init(cd, cd_letter)) return 2;
    port_vga_set_frame_hook(on_frame, NULL);
    port_record_set_swap_hook(on_swap);
    if (key_count)
        port_host_set_pump_hook(key_script_pump);
    start.dos_main = dos_main;
    start.demo = demo;
    start.argc = argc;
    start.argv = argv;
    return port_host_run(run_game, &start);
}
