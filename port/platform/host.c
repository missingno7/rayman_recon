#include "platform/host.h"
#include "platform/audio.h"

#include <SDL3/SDL.h>
#include <stdio.h>
#include <string.h>

#define LOGICAL_W 320
#define LOGICAL_H 200
/* The 320x200 mode was shown on a 4:3 monitor: each pixel is 1.2 times as tall as it is wide. */
#define ASPECT_H 240

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;
static int headless = 1;
static int quit_requested;

static void (*pump_hook)(void);
static uint8_t scan_queue[512];
static unsigned scan_head, scan_tail;

void port_host_inject_scancode(uint8_t code)
{
    if (((scan_tail + 1) & 511) != scan_head) {
        scan_queue[scan_tail] = code;
        scan_tail = (scan_tail + 1) & 511;
    }
}

int port_host_next_scancode(void)
{
    int code;
    if (scan_head == scan_tail)
        return -1;
    code = scan_queue[scan_head];
    scan_head = (scan_head + 1) & 511;
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
    headless = options->headless;
    if (headless)
        return 1;
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 0;
    }
    scale = options->scale > 0 ? options->scale : 4;
    if (!SDL_CreateWindowAndRenderer("Rayman", LOGICAL_W * scale, ASPECT_H * scale,
                                     SDL_WINDOW_RESIZABLE | (options->fullscreen ? SDL_WINDOW_FULLSCREEN : 0),
                                     &window, &renderer)) {
        fprintf(stderr, "SDL_CreateWindowAndRenderer: %s\n", SDL_GetError());
        return 0;
    }
    SDL_SetRenderVSync(renderer, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING, LOGICAL_W, LOGICAL_H);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);
    return texture != NULL;
}

void port_host_shutdown(void)
{
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
int port_host_quit_requested(void) { return quit_requested; }

void port_host_present(const uint8_t *pixels, int pitch, const uint8_t *dac_rgb6)
{
    uint32_t lut[256];
    void *dst;
    int dst_pitch, x, y, w, h;
    SDL_FRect rect;
    if (headless)
        return;
    for (x = 0; x < 256; x++) {
        /* 6-bit DAC component c becomes the 8-bit level (c << 2) | (c >> 4). */
        uint32_t r = dac_rgb6[3 * x] & 63, g = dac_rgb6[3 * x + 1] & 63, b = dac_rgb6[3 * x + 2] & 63;
        lut[x] = ((r << 2 | r >> 4) << 16) | ((g << 2 | g >> 4) << 8) | (b << 2 | b >> 4);
    }
    if (SDL_LockTexture(texture, NULL, &dst, &dst_pitch)) {
        for (y = 0; y < LOGICAL_H; y++) {
            uint32_t *row = (uint32_t *)((uint8_t *)dst + y * dst_pitch);
            const uint8_t *src = pixels + y * pitch;
            for (x = 0; x < LOGICAL_W; x++)
                row[x] = lut[src[x]];
        }
        SDL_UnlockTexture(texture);
    }
    /* Largest 4:3 rectangle; pixel-art scaling keeps pixel edges sharp at non-integer vertical factors. */
    SDL_GetCurrentRenderOutputSize(renderer, &w, &h);
    rect.w = (float)w;
    rect.h = (float)w * ASPECT_H / LOGICAL_W;
    if (rect.h > h) {
        rect.h = (float)h;
        rect.w = (float)h * LOGICAL_W / ASPECT_H;
    }
    rect.x = (w - rect.w) / 2;
    rect.y = (h - rect.h) / 2;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);
}

void port_host_set_pump_hook(void (*hook)(void)) { pump_hook = hook; }

void port_host_pump(void)
{
    SDL_Event e;
    if (pump_hook)
        pump_hook();
    if (headless)
        return;
    while (SDL_PollEvent(&e)) {
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
            if (code & 0x100)
                port_host_inject_scancode(0xe0);
            port_host_inject_scancode((uint8_t)((code & 0x7f) | (e.key.down ? 0 : 0x80)));
            break;
        }
        default:
            break;
        }
    }
}
