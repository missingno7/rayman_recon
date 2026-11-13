#include "platform/display.h"

#include <SDL3/SDL_atomic.h>
#include <string.h>

static SDL_AtomicInt mode, square = {1}, fullscreen, fixed_pad = {-1}, locked, out_w, out_h, changed;
static const char *const names[PORT_WIDE_COUNT] = {"off", "16:9", "21:9", "fit"};

void port_display_init(int m, int sq, int fs, int fixed, int lock)
{
    SDL_SetAtomicInt(&mode, m >= 0 && m < PORT_WIDE_COUNT ? m : PORT_WIDE_OFF);
    SDL_SetAtomicInt(&square, sq != 0);
    SDL_SetAtomicInt(&fullscreen, fs != 0);
    SDL_SetAtomicInt(&fixed_pad, fixed);
    SDL_SetAtomicInt(&locked, lock != 0);
    SDL_SetAtomicInt(&changed, 0);
}

static void set(SDL_AtomicInt *a, int v)
{
    if (SDL_GetAtomicInt(a) != v) {
        SDL_SetAtomicInt(a, v);
        SDL_SetAtomicInt(&changed, 1);
    }
}

int port_display_mode(void) { return SDL_GetAtomicInt(&mode); }
void port_display_set_mode(int m)
{
    if (m < 0 || m >= PORT_WIDE_COUNT)
        return;
    SDL_SetAtomicInt(&fixed_pad, -1);   /* the player's choice replaces a command-line width */
    set(&mode, m);
}
int port_display_square(void) { return SDL_GetAtomicInt(&square); }
void port_display_set_square(int s) { set(&square, s != 0); }
int port_display_fullscreen(void) { return SDL_GetAtomicInt(&fullscreen); }
void port_display_set_fullscreen(int f) { set(&fullscreen, f != 0); }
int port_display_adjustable(void) { return !SDL_GetAtomicInt(&locked); }

int port_display_rows(void) { return SDL_GetAtomicInt(&square) ? 200 : 240; }

/* Columns per side that make a 320 + 2 * pad by rows picture num:den wide, rounded. */
static int pad_for_ratio(int rows, int num, int den)
{
    int width = (rows * num + den / 2) / den;
    return (width - 320 + 1) / 2;
}

int port_display_pad(void)
{
    int rows = port_display_rows(), pad, w, h;
    if (SDL_GetAtomicInt(&locked))
        return 0;
    pad = SDL_GetAtomicInt(&fixed_pad);
    if (pad < 0) {
        switch (SDL_GetAtomicInt(&mode)) {
        case PORT_WIDE_16_9: pad = pad_for_ratio(rows, 16, 9); break;
        case PORT_WIDE_21_9: pad = pad_for_ratio(rows, 21, 9); break;
        case PORT_WIDE_FIT:
            w = SDL_GetAtomicInt(&out_w);
            h = SDL_GetAtomicInt(&out_h);
            /* unknown window (headless runs): 16:9 */
            pad = w > 0 && h > 0 ? (int)(((double)rows * w / h - 320) / 2.0 + 0.5) : pad_for_ratio(rows, 16, 9);
            break;
        default: pad = 0; break;
        }
    }
    return pad < 0 ? 0 : pad > PORT_DISPLAY_MAX_PAD ? PORT_DISPLAY_MAX_PAD : pad;
}

void port_display_set_output(int width, int height)
{
    SDL_SetAtomicInt(&out_w, width);
    SDL_SetAtomicInt(&out_h, height);
}

int port_display_take_changed(void) { return SDL_SetAtomicInt(&changed, 0); }

const char *port_display_mode_name(int m) { return m >= 0 && m < PORT_WIDE_COUNT ? names[m] : "off"; }

int port_display_mode_from_name(const char *name)
{
    int m;
    for (m = 0; m < PORT_WIDE_COUNT; m++)
        if (!strcmp(name, names[m]))
            return m;
    return -1;
}
