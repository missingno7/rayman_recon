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
#include "platform/display.h"
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
#include <stdint.h>
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
static port_config *settings;      /* the settings file, when the program has one (on_frame saves display changes) */
static const char *settings_path;
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
    static const uint8_t zero[3];
    int stride = (width + 3) & ~3;   /* BMP rows are padded to 4 bytes (e.g. 434 columns at 57 per side) */
    uint32_t size = 54 + 1024 + (uint32_t)stride * 200;
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
    for (y = 199; y >= 0; y--) {
        fwrite(pixels + y * width, 1, (size_t)width, f);
        fwrite(zero, 1, (size_t)(stride - width), f);
    }
    fclose(f);
}

/* The CD images the settings' "auto" may insert, best first (main). */
static char cd_candidates[16][1100];
static int cd_candidate_count;

static void add_cd_candidate(const char *format, const char *a, const char *b)
{
    int k;
    if (cd_candidate_count == 16)
        return;
    snprintf(cd_candidates[cd_candidate_count], sizeof cd_candidates[0], format, a, b);
    for (k = 0; k < cd_candidate_count; k++)
        if (!SDL_strcasecmp(cd_candidates[k], cd_candidates[cd_candidate_count]))
            return;   /* RAYMAN.cue also matches *.cue */
    cd_candidate_count++;
}

/* The game version of the data folder. The game code is v1.21's, and its tables locate the pieces of the data
   archives (src/data/blk_215c.c: RAY.LNG's languages, VIGNET.DAT's pictures); other versions (the v1.12 CD) pack
   different files that this code cannot read. Compared by the archives' total sizes; NULL when they match,
   otherwise a message for the player. */
typedef struct { int32_t offset, size; uint8_t xor_byte, checksum_byte; uint16_t reserved; } archive_entry;
extern archive_entry language_infos[5], HeaderFilevignet[75];

static const char *data_version_problem(void)
{
    static const struct { const char *name; const archive_entry *table; int count; } archives[] = {
        {"RAY.LNG", language_infos, 5}, {"VIGNET.DAT", HeaderFilevignet, 75}};
    static char message[600];
    char path[1024];
    int i;
    for (i = 0; i < 2; i++) {
        const archive_entry *last = &archives[i].table[archives[i].count - 1];
        long expected = (long)last->offset + last->size, got = -1;
        FILE *f;
        if (port_files_resolve(archives[i].name, 0, path, sizeof path) && (f = fopen(path, "rb")) != NULL) {
            if (fseek(f, 0, SEEK_END) == 0)
                got = ftell(f);
            fclose(f);
        }
        if (got != expected) {
            snprintf(message, sizeof message,
                     "This port plays the game files of Rayman v1.21 (the GOG release). The game folder's %s is %ld "
                     "bytes, v1.21's is %ld: it holds another version of the game (for example the v1.12 CD), which "
                     "this port does not support yet.", archives[i].name, got, expected);
            return message;
        }
    }
    return NULL;
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
    /* Display settings changed by the player (the options menu, Alt+Enter, a resized window in "fit"): the view width
       follows at the next frame boundary, the settings file keeps the choice. */
    {
        static int logged = -1;
        int want = port_display_pad();
        if (want != logged && logged >= 0 && port_display_adjustable())
            printf("Widescreen: %d columns per side (%s pixels)\n", want, port_display_square() ? "square" : "4:3");
        logged = want;
        port_record_request_pad(want);
    }
    if (port_display_take_changed() && settings_path) {
        settings->widescreen = port_display_mode();
        settings->square_pixels = port_display_square();
        if (!PORT_ENTRY_ALWAYS_FULLSCREEN)
            settings->fullscreen = port_display_fullscreen();
        port_config_save(settings_path, settings);
    }
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
    int have_config = 0, no_vsync = 0, cd_auto = 0;   /* cd_auto: the CD was found by the settings' "auto" */
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
        else if (!strcmp(a, "--no-cd-movies")) port_files_set_cd_movies(0);   /* the movies only from data roots */
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
        settings = &config;
        settings_path = config_path;
        if (square < 0) square = config.square_pixels;
        if (config.widescreen && !wide_pad && !want_wide && renderer != 0 && !port_record_verify())
            config_wide = 1;
        if (config.fullscreen) host.fullscreen = 1;
        host.vsync = config.vsync;
        if (!host.scale) host.scale = config.window_scale;
        if (!save && config.save_dir[0]) save = config.save_dir;
        if (!cd && strcmp(config.cd, "auto") && strcmp(config.cd, "none")) cd = config.cd;
        if (!cd && !strcmp(config.cd, "auto")) {
            /* The CD, in this order (the first usable one is inserted, port_cd_init below): the original disc first,
               user decision 2026-11-13, since GOG's music files cut tracks 8, 13 and 14 short and replace track 14's
               piece (port/docs/DIVERGENCES.md): RAYMAN.cue in the game folder, another *.cue there or in the parent
               folder, a Rayman disc in a host drive (a real CD or a mounted image), then GOG's *.ins (game.gog +
               Music/raymanNN.ogg; Rayman Forever keeps the game in Rayman/ and its CD one level up). None: the game
               runs without CD music (game/cd_policy.c). */
            static const char *const sheets[] = {"*.cue", "*.ins"};
            char dirs[2][1100];
            int d, k, count;
            snprintf(dirs[0], sizeof dirs[0], "%s", base_dir);
            snprintf(dirs[1], sizeof dirs[1], "%s../", base_dir);
            if (base_dir[0])
                add_cd_candidate("%sRAYMAN.cue", base_dir, "");
            for (k = 0; k < 2; k++) {
                if (k == 1 && (host_cd[0] = port_cd_host_find()) != 0) {
                    host_cd[1] = ':';
                    add_cd_candidate("%s%s", host_cd, "");
                }
                for (d = 0; d < 2 && base_dir[0]; d++) {
                    char **found = SDL_GlobDirectory(dirs[d], sheets[k], SDL_GLOB_CASEINSENSITIVE, &count);
                    int f;
                    for (f = 0; found && f < count; f++)
                        add_cd_candidate("%s%s", dirs[d], found[f]);
                    SDL_free(found);
                }
            }
            cd_auto = 1;
        }
    }
    if (no_vsync)
        host.vsync = 0;
    /* Pixel shape (user decision 2026-11-12: square by default): the picture is 320 x 200 game pixels shown as 16:10,
       or as the original 4:3 (pixels 1.2 times as tall). The wider view: off, 16:9, 21:9 or as wide as the window
       (platform/display.h). Both can change while the game runs (the options menu's Display screen); a new view
       width takes effect at the next frame boundary (on_frame). */
    if (square < 0) square = 1;
    if (wide_pad > 0 && renderer == 0) {
        /* A wider view is the native renderer's: the original draw path has 320 columns. */
        fprintf(stderr, "rayman: --widescreen needs the native renderer (not --renderer reference)\n");
        return 2;
    }
    if ((wide_pad > 0 || want_wide) && port_record_verify()) {
        fprintf(stderr, "rayman: --verify-render compares the original 320-column view; not with --widescreen\n");
        return 2;
    }
    port_display_init(want_wide ? PORT_WIDE_16_9 : config_wide ? config.widescreen : PORT_WIDE_OFF, square,
                      host.fullscreen || PORT_ENTRY_ALWAYS_FULLSCREEN, wide_pad > 0 ? wide_pad : -1,
                      renderer == 0 || port_record_verify());
    host.display_rows = port_display_rows();
    wide_pad = port_display_pad();
    if (port_display_mode() == PORT_WIDE_FIT && wide_pad && port_display_fullscreen()) {
        /* Fullscreen: as wide as the display (up to 160 columns per side), e.g. 57 square or 100 original on a
           3120x1440 phone; the window reports its real size once it exists. */
        int fit = port_host_fit_pad(PORT_DISPLAY_MAX_PAD, host.display_rows);
        if (fit >= 0) wide_pad = fit;
    }
    if (wide_pad > 0)
        printf("Widescreen: %d columns per side (%s pixels)\n", wide_pad, square ? "square" : "4:3");
    port_record_set_pad(wide_pad);
    /* default: the native renderer, falling back to the original image for frames it cannot draw completely */
    port_record_set_native(renderer == 1 ? 1 : renderer == 0 ? 0 : 2);
    host.width = 320 + 2 * port_record_pad();
    if (data_count) data = data_roots[0]; /* Explicit roots replace RAYMAN_DATA. */
    if (!data) {
        fprintf(stderr, "rayman: put the program in the original game directory, or give it with --data DIR "
                        "(or RAYMAN_DATA)\n");
        return 2;
    }
    if (!save && base_dir[0]) {
        /* GOG's Rayman Forever: its DOSBox overlays ..\cloud_saves on the game folder, so the game's RAYMAN.CFG and
           saves live there (GOG Galaxy syncs that folder); the port keeps them in the same place, shared with GOG's
           launcher (user decision 2026-11-13). */
        static char gog_saves[1100];
        SDL_PathInfo info;
        snprintf(gog_saves, sizeof gog_saves, "%s../cloud_saves", base_dir);
        if (SDL_GetPathInfo(gog_saves, &info) && info.type == SDL_PATHTYPE_DIRECTORY)
            save = gog_saves;
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
    {
        const char *problem = data_version_problem();
        if (problem) {
            fprintf(stderr, "rayman: %s\n", problem);
            if (!host.headless)
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Rayman: unsupported game version", problem, NULL);
            return 2;
        }
    }
    port_clock_init(deterministic ? PORT_CLOCK_DETERMINISTIC : PORT_CLOCK_REALTIME);
    if (!port_host_init(&host))
        return 1;
    atexit(at_exit);
    if (!cd_auto) {
        if (!port_cd_init(cd, cd_letter)) return 2;   /* a CD the player named must work */
    } else {
        int k, inserted = 0;
        for (k = 0; k < cd_candidate_count && !inserted; k++) {
            FILE *probe = cd_candidates[k][1] == ':' && !cd_candidates[k][2] ? NULL : fopen(cd_candidates[k], "rb");
            if (probe) fclose(probe);
            if (!probe && !(cd_candidates[k][1] == ':' && !cd_candidates[k][2]))
                continue;   /* no such file: not a candidate */
            if ((inserted = port_cd_init(cd_candidates[k], cd_letter)) != 0)
                cd = cd_candidates[k];
            else
                fprintf(stderr, "rayman: %s is not a usable Rayman CD\n", cd_candidates[k]);
        }
        if (!inserted) {
            if (cd_candidate_count) fprintf(stderr, "rayman: playing without a CD\n");
            if (!port_cd_init(NULL, cd_letter)) return 2;
        }
    }
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
