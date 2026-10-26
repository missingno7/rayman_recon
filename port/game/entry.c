/*
 * Process entry of the native port: options, platform start-up, then one of the game entries (port_main.h).
 *
 *   rayman [--data DIR] [--save DIR] [--cd PATH.cue] [--cd-drive D] [--demo N | --menus | --dos-main] [--headless] [--deterministic]
 *          [--frames N] [--frame-log FILE] [--dump-frames DIR --dump-every N] [--epoch SECONDS]
 *          [--scale N] [--fullscreen] [--widescreen | --wide-pad N] [--verify-render] [--state-log FILE]
 *
 * --keys FILE queues scan codes at given presented frames (tests).
 * --epoch sets the wall-clock time a deterministic run starts at (the game seeds its random table from time()).
 *
 * --frames/--frame-log exist for regression runs: stop after N presented images, log one hash per image.
 *
 * --widescreen (16:9, a 54-pixel margin per side) / --wide-pad N present the unchanged original image in the centre
 * of a wider frame whose margins the semantic renderer draws (port/docs/RENDERER.md). --verify-render checks the
 * semantic renderer against the original draw buffer every frame; --state-log writes one gameplay-state digest per
 * presented image (presentation-independence tests). Frame logs always hash the original 320x200 image.
 */
#include "game/port_main.h"
#include "platform/clock.h"
#include "platform/files.h"
#include "platform/cdrom.h"
#include "platform/hmi.h"
#include "platform/audio.h"
#include "platform/host.h"
#include "video/vga.h"
#include "render/capture.h"
#include "render/compose.h"

time_t port_time(time_t *out);
void port_set_time_epoch(long long seconds);

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern unsigned char JeuCracker, MusicCdActive, CarteSonAutorisee;
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

/* FNV-1a over the 64000 visible pixels and the 768 DAC bytes. */
static void on_frame(const uint8_t *pixels, const uint8_t *dac, void *user)
{
    (void)user;
    if (frame_log) {
        uint64_t h = 1469598103934665603ULL;
        int i;
        for (i = 0; i < 64000; i++)
            h = (h ^ pixels[i]) * 1099511628211ULL;
        for (i = 0; i < 768; i++)
            h = (h ^ dac[i]) * 1099511628211ULL;
        fprintf(frame_log, "%ld %llu %016llx\n", frame_count, (unsigned long long)port_clock_ns(),
                (unsigned long long)h);
        fflush(frame_log); /* Preserve completed frames across another port boundary crash. */
    }
    if (state_log) {
        fprintf(state_log, "%ld %016llx\n", frame_count, (unsigned long long)port_scene_state_digest());
        fflush(state_log);
    }
    if (dump_dir && dump_every > 0 && frame_count % dump_every == 0) {
        int width;
        const uint8_t *frame = port_render_last(&width);   /* the presented frame, wider with a margin */
        dump_frame(frame ? frame : pixels, frame ? width : 320, dac);
    }
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
        long drawn, black;
        printf("port exit: JeuCracker=%u MusicCdActive=%u frames=%ld world=%d level=%d CarteSonAutorisee=%u\n",
               JeuCracker, MusicCdActive, frame_count, num_world, num_level, CarteSonAutorisee);
        port_scene_verify_report();
        port_render_counts(&drawn, &black);
        if (port_render_pad())
            printf("wide frames: pad=%d margins_drawn=%ld margins_black=%ld\n", port_render_pad(), drawn, black);
    }
    port_hmi_shutdown();
    port_cd_shutdown();
    port_audio_shutdown();
    port_host_shutdown();
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
    port_host_options host = {0, 0, 0, 320};
    int deterministic = 0, demo = -1, dos_main = 0, i;

    setvbuf(stdout, NULL, _IONBF, 0);
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
        else if (!strcmp(a, "--widescreen")) port_render_set_pad(54);   /* 428 x 240 display: 16:9 */
        else if (!strcmp(a, "--wide-pad") && v) port_render_set_pad(atoi(argv[++i]));
        else if (!strcmp(a, "--verify-render")) port_scene_set_verify(1);
        else if (!strcmp(a, "--state-log") && v) state_log = fopen(argv[++i], "w");
        else {
            fprintf(stderr, "unknown option %s\n", a);
            return 2;
        }
    }
    host.width = 320 + 2 * port_render_pad();
    if (data_count) data = data_roots[0]; /* Explicit roots replace RAYMAN_DATA. */
    if (!data) {
        fprintf(stderr, "rayman: give the original game directory with --data DIR (or RAYMAN_DATA)\n");
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
    port_clock_init(deterministic ? PORT_CLOCK_DETERMINISTIC : PORT_CLOCK_REALTIME);
    if (!port_host_init(&host))
        return 1;
    atexit(at_exit);
    if (!port_cd_init(cd, cd_letter)) return 2;
    port_vga_set_frame_hook(on_frame, NULL);
    if (key_count)
        port_host_set_pump_hook(key_script_pump);
    if (dos_main)
        return port_run_dos_main(argc, argv);
    if (demo >= 0)
        return port_run_demo(demo);
    return port_run_menus();
}
