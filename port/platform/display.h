/*
 * Display settings the player can change while the game runs (port presentation only; nothing here changes what the
 * game computes): the wider view's mode, the pixel shape and fullscreen. Shared by the game's thread (the Display
 * screen of the options menu, the frame boundary that applies a new view width) and the presenter's thread (the
 * window: its shape for "fit", fullscreen, the picture's height). Values are atomics; no locks.
 */
#ifndef PORT_PLATFORM_DISPLAY_H
#define PORT_PLATFORM_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

enum port_wide_mode {
    PORT_WIDE_OFF,      /* the original 320 columns */
    PORT_WIDE_16_9,     /* 16:9 at the chosen pixel shape */
    PORT_WIDE_21_9,     /* 21:9 */
    PORT_WIDE_FIT,      /* as wide as the window (fullscreen: the display), up to PORT_DISPLAY_MAX_PAD per side */
    PORT_WIDE_COUNT
};

#define PORT_DISPLAY_MAX_PAD 160

/* fixed_pad >= 0: a view width given on the command line (--wide-pad), kept until the player picks a mode;
   locked: the view stays 320 columns (--verify-render, --renderer reference). */
void port_display_init(int mode, int square, int fullscreen, int fixed_pad, int locked);

int port_display_mode(void);
void port_display_set_mode(int mode);
int port_display_square(void);
void port_display_set_square(int square);
int port_display_fullscreen(void);
void port_display_set_fullscreen(int fullscreen);
/* Whether the view width can change (not locked). */
int port_display_adjustable(void);

/* Picture height in game-pixel widths: 200 (square pixels) or 240 (the original 4:3 picture). */
int port_display_rows(void);
/* Columns per side the view should have now. */
int port_display_pad(void);
/* The presenter reports the window's drawable size (pixels; 0 x 0 = unknown, e.g. headless). */
void port_display_set_output(int width, int height);

/* A setting changed since the last call (the caller saves the settings file). */
int port_display_take_changed(void);

/* Short names for the menu and the settings file: "off", "16:9", "21:9", "fit". */
const char *port_display_mode_name(int mode);
/* -1 when the name is not a mode. */
int port_display_mode_from_name(const char *name);

#ifdef __cplusplus
}
#endif

#endif
