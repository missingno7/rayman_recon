#include "platform/host.h"
#include "platform/audio.h"
#include "platform/pad.h"
#include "platform/touch.h"

#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOGICAL_W 320
#define LOGICAL_H 200
/* Height of the presented picture in game-pixel widths: 240 shows the 320x200 mode as on a 4:3 monitor (each pixel
   1.2 times as tall as wide), 200 shows square pixels (port_host_options.display_rows). */
static int ASPECT_H = 240;
#ifdef __ANDROID__
#define PORT_ALWAYS_FULLSCREEN 1   /* phones: the whole screen, no status bar */
#else
#define PORT_ALWAYS_FULLSCREEN 0
#endif

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;
static int texture_width;
static int headless = 1;
static int quit_requested;
static int vsync_on;
static uint64_t shown_ns, shown_count, present_cost_ns;

static void (*pump_hook)(void);
static uint8_t scan_queue[512];
static unsigned scan_head, scan_tail;

/* Presenter (port_host_run): the game runs on its own thread; the main thread owns the window, the events and
   the renderer. `shared` guards what the two exchange: the scan-code queue, the newest frame, the cached refresh
   period. NULL while the game runs on the main thread (headless, tests). */
static SDL_Mutex *shared;
static SDL_Condition *frame_ready;
static uint8_t newest_pixels[(LOGICAL_W + 2 * 160) * LOGICAL_H], newest_dac[768];
static int newest_width;
static uint64_t newest_seq, drawn_seq;
static uint64_t cached_refresh_ns;
static SDL_AtomicInt game_finished, presenter_stop, presenter_stopped;

static void push_codes(const uint8_t *codes, int n)
{
    int i;
    if (shared) SDL_LockMutex(shared);
    for (i = 0; i < n; i++)
        if (((scan_tail + 1) & 511) != scan_head) {
            scan_queue[scan_tail] = codes[i];
            scan_tail = (scan_tail + 1) & 511;
        }
    if (shared) SDL_UnlockMutex(shared);
}

void port_host_inject_scancode(uint8_t code) { push_codes(&code, 1); }

int port_host_next_scancode(void)
{
    int code = -1;
    if (shared) SDL_LockMutex(shared);
    if (scan_head != scan_tail) {
        code = scan_queue[scan_head];
        scan_head = (scan_head + 1) & 511;
    }
    if (shared) SDL_UnlockMutex(shared);
    return code;
}

/* SDL scancode -> PC/AT set-1 make code; bit 8 = 0xE0-prefixed (extended) key; 0 = no PC key. */
static int set1_code(SDL_Scancode s)
{
    static const uint8_t letters[26] = { /* A..Z */
        0x1e, 0x30, 0x2e, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24, 0x25, 0x26, 0x32,
        0x31, 0x18, 0x19, 0x10, 0x13, 0x1f, 0x14, 0x16, 0x2f, 0x11, 0x2d, 0x15, 0x2c};
    if (s >= SDL_SCANCODE_A && s <= SDL_SCANCODE_Z)
        return letters[s - SDL_SCANCODE_A];
    if (s >= SDL_SCANCODE_1 && s <= SDL_SCANCODE_0)
        return 0x02 + (s - SDL_SCANCODE_1);
    if (s >= SDL_SCANCODE_F1 && s <= SDL_SCANCODE_F10)
        return 0x3b + (s - SDL_SCANCODE_F1);
    switch (s) {
    case SDL_SCANCODE_ESCAPE: return 0x01;
    case SDL_SCANCODE_AC_BACK: return 0x01;   /* Android Back: Esc, as the Android release's back-button listener */
    case SDL_SCANCODE_MINUS: return 0x0c;
    case SDL_SCANCODE_EQUALS: return 0x0d;
    case SDL_SCANCODE_BACKSPACE: return 0x0e;
    case SDL_SCANCODE_TAB: return 0x0f;
    case SDL_SCANCODE_LEFTBRACKET: return 0x1a;
    case SDL_SCANCODE_RIGHTBRACKET: return 0x1b;
    case SDL_SCANCODE_RETURN: return 0x1c;
    case SDL_SCANCODE_LCTRL: return 0x1d;
    case SDL_SCANCODE_SEMICOLON: return 0x27;
    case SDL_SCANCODE_APOSTROPHE: return 0x28;
    case SDL_SCANCODE_GRAVE: return 0x29;
    case SDL_SCANCODE_LSHIFT: return 0x2a;
    case SDL_SCANCODE_BACKSLASH: return 0x2b;
    case SDL_SCANCODE_COMMA: return 0x33;
    case SDL_SCANCODE_PERIOD: return 0x34;
    case SDL_SCANCODE_SLASH: return 0x35;
    case SDL_SCANCODE_RSHIFT: return 0x36;
    case SDL_SCANCODE_KP_MULTIPLY: return 0x37;
    case SDL_SCANCODE_LALT: return 0x38;
    case SDL_SCANCODE_SPACE: return 0x39;
    case SDL_SCANCODE_CAPSLOCK: return 0x3a;
    case SDL_SCANCODE_NUMLOCKCLEAR: return 0x45;
    case SDL_SCANCODE_SCROLLLOCK: return 0x46;
    case SDL_SCANCODE_KP_7: return 0x47;
    case SDL_SCANCODE_KP_8: return 0x48;
    case SDL_SCANCODE_KP_9: return 0x49;
    case SDL_SCANCODE_KP_MINUS: return 0x4a;
    case SDL_SCANCODE_KP_4: return 0x4b;
    case SDL_SCANCODE_KP_5: return 0x4c;
    case SDL_SCANCODE_KP_6: return 0x4d;
    case SDL_SCANCODE_KP_PLUS: return 0x4e;
    case SDL_SCANCODE_KP_1: return 0x4f;
    case SDL_SCANCODE_KP_2: return 0x50;
    case SDL_SCANCODE_KP_3: return 0x51;
    case SDL_SCANCODE_KP_0: return 0x52;
    case SDL_SCANCODE_KP_PERIOD: return 0x53;
    case SDL_SCANCODE_F11: return 0x57;
    case SDL_SCANCODE_F12: return 0x58;
    case SDL_SCANCODE_RCTRL: return 0x100 | 0x1d;
    case SDL_SCANCODE_RALT: return 0x100 | 0x38;
    case SDL_SCANCODE_KP_ENTER: return 0x100 | 0x1c;
    case SDL_SCANCODE_KP_DIVIDE: return 0x100 | 0x35;
    case SDL_SCANCODE_INSERT: return 0x100 | 0x52;
    case SDL_SCANCODE_DELETE: return 0x100 | 0x53;
    case SDL_SCANCODE_HOME: return 0x100 | 0x47;
    case SDL_SCANCODE_END: return 0x100 | 0x4f;
    case SDL_SCANCODE_PAGEUP: return 0x100 | 0x49;
    case SDL_SCANCODE_PAGEDOWN: return 0x100 | 0x51;
    case SDL_SCANCODE_UP: return 0x100 | 0x48;
    case SDL_SCANCODE_LEFT: return 0x100 | 0x4b;
    case SDL_SCANCODE_RIGHT: return 0x100 | 0x4d;
    case SDL_SCANCODE_DOWN: return 0x100 | 0x50;
    default: return 0;
    }
}

int port_host_init(const port_host_options *options)
{
    int scale;
    const char *touch_env = getenv("RAYMAN_TOUCH");
    int touch;
    headless = options->headless;
    if (headless)
        return 1;
#ifdef __ANDROID__
    touch = !(touch_env && touch_env[0] == '0');
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");
#else
    touch = touch_env && touch_env[0] == '1';
#endif
    /* Touch controls see fingers only; on a desktop test run the mouse acts as a finger. */
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
    if (touch)
        SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS, "1");
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 0;
    }
    scale = options->scale > 0 ? options->scale : 4;
    texture_width = options->width > LOGICAL_W ? options->width : LOGICAL_W;
    if (options->display_rows >= LOGICAL_H)
        ASPECT_H = options->display_rows;
    if (!SDL_CreateWindowAndRenderer("Rayman", texture_width * scale, ASPECT_H * scale,
                                     SDL_WINDOW_RESIZABLE | (options->fullscreen || PORT_ALWAYS_FULLSCREEN ? SDL_WINDOW_FULLSCREEN : 0),
                                     &window, &renderer)) {
        fprintf(stderr, "SDL_CreateWindowAndRenderer: %s\n", SDL_GetError());
        return 0;
    }
    port_pad_init();
    port_touch_init(touch);
    vsync_on = options->vsync && SDL_SetRenderVSync(renderer, 1);
    if (!vsync_on)
        SDL_SetRenderVSync(renderer, 0);
    return 1;
}

int port_host_fit_pad(int max_pad, int display_rows)
{
    const SDL_DisplayMode *m;
    int w, h, pad;
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
        return -1;
    m = SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay());
    if (!m || m->w <= 0 || m->h <= 0) {
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        return -1;
    }
    w = m->w > m->h ? m->w : m->h;   /* the game runs in landscape */
    h = m->w > m->h ? m->h : m->w;
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    pad = (int)(((double)display_rows * w / h - LOGICAL_W) / 2.0 + 0.5);
    return pad < 0 ? 0 : pad > max_pad ? max_pad : pad;
}

void port_host_shutdown(void)
{
    if (shared) {
        /* The game's thread is ending the program (exit) while the main thread presents: stop the presenter and
           leave its window and renderer to the process exit - they belong to that thread. */
        int i;
        SDL_SetAtomicInt(&presenter_stop, 1);
        SDL_LockMutex(shared);
        SDL_SignalCondition(frame_ready);
        SDL_UnlockMutex(shared);
        for (i = 0; i < 500 && !SDL_GetAtomicInt(&presenter_stopped); i++)
            SDL_Delay(1);
        port_audio_shutdown();
        return;
    }
    if (texture)
        SDL_DestroyTexture(texture);
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    texture = NULL;
    renderer = NULL;
    window = NULL;
    port_audio_shutdown();
    if (!headless)
        SDL_Quit();
}

int port_host_headless(void) { return headless; }
uint64_t port_host_shown(uint64_t *count) { *count = shown_count; return shown_ns; }
uint64_t port_host_present_cost_ns(void) { return present_cost_ns; }

static uint64_t query_refresh_ns(void);

uint64_t port_host_refresh_ns(void)
{
    uint64_t ns;
    if (!shared)
        return query_refresh_ns();
    SDL_LockMutex(shared);   /* asked by the game's thread: the presenter keeps the value current */
    ns = cached_refresh_ns;
    SDL_UnlockMutex(shared);
    return ns;
}

static uint64_t query_refresh_ns(void)
{
    const SDL_DisplayMode *m;
    if (headless || !window)
        return 0;
    /* asked each time: the window can move to another display, the mode can change */
    m = SDL_GetCurrentDisplayMode(SDL_GetDisplayForWindow(window));
    if (!m || m->refresh_rate < 1.0f)
        return 0;
    if (m->refresh_rate_numerator > 0 && m->refresh_rate_denominator > 0)
        return (uint64_t)m->refresh_rate_denominator * 1000000000ULL / (uint64_t)m->refresh_rate_numerator;
    return (uint64_t)(1e9 / m->refresh_rate);
}
int port_host_quit_requested(void) { return quit_requested; }

static void draw_frame(const uint8_t *pixels, int pitch, int width, const uint8_t *dac_rgb6);

void port_host_present(const uint8_t *pixels, int pitch, int width, const uint8_t *dac_rgb6)
{
    int y;
    if (headless)
        return;
    if (!shared) {
        draw_frame(pixels, pitch, width, dac_rgb6);
        return;
    }
    /* Hand the frame to the presenter; the game never waits for the display. */
    if (width > LOGICAL_W + 2 * 160)
        return;
    SDL_LockMutex(shared);
    for (y = 0; y < LOGICAL_H; y++)
        memcpy(newest_pixels + y * width, pixels + y * pitch, (size_t)width);
    memcpy(newest_dac, dac_rgb6, 768);
    newest_width = width;
    newest_seq++;
    SDL_SignalCondition(frame_ready);
    SDL_UnlockMutex(shared);
}

static void draw_frame(const uint8_t *pixels, int pitch, int width, const uint8_t *dac_rgb6)
{
    uint32_t lut[256];
    void *dst;
    int dst_pitch, x, y, w, h;
    SDL_FRect rect;
    uint64_t entered = SDL_GetTicksNS();
    if (!texture || width != texture_width) {
        if (texture)
            SDL_DestroyTexture(texture);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING, width, LOGICAL_H);
        if (!texture)
            return;
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);
        texture_width = width;
    }
    for (x = 0; x < 256; x++) {
        /* 6-bit DAC component c becomes the 8-bit level (c << 2) | (c >> 4). */
        uint32_t r = dac_rgb6[3 * x] & 63, g = dac_rgb6[3 * x + 1] & 63, b = dac_rgb6[3 * x + 2] & 63;
        lut[x] = ((r << 2 | r >> 4) << 16) | ((g << 2 | g >> 4) << 8) | (b << 2 | b >> 4);
    }
    if (SDL_LockTexture(texture, NULL, &dst, &dst_pitch)) {
        for (y = 0; y < LOGICAL_H; y++) {
            uint32_t *row = (uint32_t *)((uint8_t *)dst + y * dst_pitch);
            const uint8_t *src = pixels + y * pitch;
            for (x = 0; x < width; x++)
                row[x] = lut[src[x]];
        }
        SDL_UnlockTexture(texture);
    }
    /* Largest width:ASPECT_H rectangle (4:3 or 16:10 for the original 320 columns); pixel-art scaling keeps pixel edges sharp
       at non-integer vertical factors. */
    SDL_GetCurrentRenderOutputSize(renderer, &w, &h);
    rect.w = (float)w;
    rect.h = (float)w * ASPECT_H / width;
    if (rect.h > h) {
        rect.h = (float)h;
        rect.w = (float)h * width / ASPECT_H;
    }
    rect.x = (w - rect.w) / 2;
    rect.y = (h - rect.h) / 2;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, &rect);
    port_touch_draw(renderer);
    SDL_RenderPresent(renderer);
    shown_ns = SDL_GetTicksNS();
    present_cost_ns = shown_ns - entered;
    {
        /* RAYMAN_PRESENT_TRACE=FILE: one line per shown frame - host ns when drawing began and when the present
           returned. */
        static FILE *trace;
        static int asked;
        if (!asked) {
            const char *path = getenv("RAYMAN_PRESENT_TRACE");
            asked = 1;
            if (path && *path)
                trace = fopen(path, "w");
        }
        if (trace)
            fprintf(trace, "%llu %llu %llu\n", (unsigned long long)shown_count, (unsigned long long)entered,
                    (unsigned long long)shown_ns);
    }
    shown_count++;
}

void port_host_set_pump_hook(void (*hook)(void)) { pump_hook = hook; }

static void poll_events(void);

void port_host_pump(void)
{
    if (pump_hook)
        pump_hook();
    if (headless || shared)   /* with a presenter, the main thread polls the events */
        return;
    poll_events();
}

static void poll_events(void)
{
    SDL_Event e;
    int out_w = 0, out_h = 0;
    while (SDL_PollEvent(&e)) {
        if (port_pad_event(&e) || port_touch_event(&e))
            continue;
        switch (e.type) {
        case SDL_EVENT_QUIT:
            quit_requested = 1;
            break;
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP: {
            int code = set1_code(e.key.scancode);
            if (!code)
                break;
            if (e.key.down && e.key.scancode == SDL_SCANCODE_RETURN && (e.key.mod & SDL_KMOD_ALT)) {
                SDL_SetWindowFullscreen(window, !(SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN));
                break;
            }
            {
                uint8_t codes[2] = {0xe0, (uint8_t)((code & 0x7f) | (e.key.down ? 0 : 0x80))};
                if (code & 0x100)
                    push_codes(codes, 2);
                else
                    push_codes(codes + 1, 1);
            }
            break;
        }
        default:
            break;
        }
    }
    if (renderer)
        SDL_GetCurrentRenderOutputSize(renderer, &out_w, &out_h);
    port_touch_update(out_w, out_h);
    port_pad_update();
}

/* ---- presenter --------------------------------------------------------------------------------------------- */
typedef struct game_start { int (*run)(void *); void *arg; } game_start;

static int game_thread(void *p)
{
    game_start *g = (game_start *)p;
    int result = g->run(g->arg);
    SDL_SetAtomicInt(&game_finished, 1);
    SDL_LockMutex(shared);
    SDL_SignalCondition(frame_ready);
    SDL_UnlockMutex(shared);
    return result;
}

int port_host_run(int (*run)(void *), void *arg)
{
    static uint8_t pixels[sizeof newest_pixels], dac[768];
    game_start start = {run, arg};
    SDL_PropertiesID props;
    SDL_Thread *thread;
    uint64_t refresh_asked = 0, last_fresh = 0;
    int result = 1, width = 0, fresh;
    if (headless || !renderer)
        return run(arg);
    shared = SDL_CreateMutex();
    frame_ready = SDL_CreateCondition();
    cached_refresh_ns = query_refresh_ns();
    props = SDL_CreateProperties();
    SDL_SetPointerProperty(props, SDL_PROP_THREAD_CREATE_ENTRY_FUNCTION_POINTER, (void *)game_thread);
    SDL_SetStringProperty(props, SDL_PROP_THREAD_CREATE_NAME_STRING, "game");
    SDL_SetPointerProperty(props, SDL_PROP_THREAD_CREATE_USERDATA_POINTER, &start);
    SDL_SetNumberProperty(props, SDL_PROP_THREAD_CREATE_STACKSIZE_NUMBER, 16 * 1024 * 1024);
    thread = SDL_CreateThreadWithProperties(props);
    SDL_DestroyProperties(props);
    if (!thread) {
        fprintf(stderr, "SDL_CreateThread: %s\n", SDL_GetError());
        SDL_DestroyCondition(frame_ready);
        SDL_DestroyMutex(shared);
        shared = NULL;
        return run(arg);
    }
    while (!SDL_GetAtomicInt(&game_finished) && !SDL_GetAtomicInt(&presenter_stop)) {
        uint64_t now = SDL_GetTicksNS(), refresh = 0;
        poll_events();
        if (now - refresh_asked > 250000000ULL) {   /* the window can move to another display */
            refresh = query_refresh_ns();
            refresh_asked = now;
        }
        SDL_LockMutex(shared);
        if (refresh)
            cached_refresh_ns = refresh;
        if (newest_seq == drawn_seq)
            SDL_WaitConditionTimeout(frame_ready, shared, 2);   /* events stay served every 2 ms */
        fresh = newest_seq != drawn_seq;
        if (fresh) {
            width = newest_width;
            memcpy(pixels, newest_pixels, (size_t)width * LOGICAL_H);
            memcpy(dac, newest_dac, sizeof dac);
            drawn_seq = newest_seq;
        }
        SDL_UnlockMutex(shared);
        if (fresh && !SDL_GetAtomicInt(&presenter_stop)) {
            draw_frame(pixels, width, width, dac);   /* with vsync this returns at the display's refresh */
            last_fresh = SDL_GetTicksNS();
        } else if (width && SDL_GetTicksNS() - last_fresh > 50000000ULL && port_touch_needs_redraw() &&
                   !SDL_GetAtomicInt(&presenter_stop))
            /* A still screen (no game frame for 50 ms): show the touch overlay's change. While frames arrive the
               overlay is drawn with them; an extra vsync'd present would delay the next game frame. */
            draw_frame(pixels, width, width, dac);
    }
    if (SDL_GetAtomicInt(&presenter_stop)) {
        /* exit() on the game's thread is ending the process: nothing more is drawn. */
        SDL_SetAtomicInt(&presenter_stopped, 1);
        for (;;)
            SDL_Delay(1000);
    }
    SDL_WaitThread(thread, &result);
    SDL_DestroyCondition(frame_ready);
    SDL_DestroyMutex(shared);
    shared = NULL;
    return result;
}
