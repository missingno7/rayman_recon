/*
 * SDL3 host: window, presentation of the original 320x200 indexed image, keyboard.
 *
 * The DOS game owns its loop and never returns to a host frame callback, so the host is driven from inside the
 * platform services the game calls: presenting (video/vga.c), waiting for retrace/timer (platform/clock.c) and
 * polling input all pump SDL events. Nothing here decides game timing.
 */
#ifndef PORT_PLATFORM_HOST_H
#define PORT_PLATFORM_HOST_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct port_host_options {
    int headless;        /* no window, no presentation (tests) */
    int scale;           /* initial window scale (integer); 0 = automatic */
    int fullscreen;
    int width;           /* logical image width: 320, or the native renderer's wider view (render/record.h) */
} port_host_options;

int port_host_init(const port_host_options *options);
void port_host_shutdown(void);
int port_host_headless(void);

/* Present a width x 200 image of palette indices with the 6-bit-per-component DAC palette (768 bytes). The 200
   rows fill the height of a 4:3 picture of the original 320 columns (pixel aspect 1.2); wider images extend it
   sideways. */
void port_host_present(const uint8_t *pixels, int pitch, int width, const uint8_t *dac_rgb6);

/* Process pending window/input events. Keyboard events become PC/AT set-1 scan codes (make, break, 0xE0
   prefixes, typematic repeats as repeated makes): what the DOS keyboard controller delivered on port 0x60. */
void port_host_pump(void);
int port_host_quit_requested(void);

/* Scan-code stream (consumed by compat/src/dos_services.c, which runs the INT 9 handler); -1 when empty. */
int port_host_next_scancode(void);
/* Tests and recorded input: queue a scan code as if typed. */
void port_host_inject_scancode(uint8_t code);
/* Called at every pump, before host events (tests: scripted input). */
void port_host_set_pump_hook(void (*hook)(void));

#ifdef __cplusplus
}
#endif

#endif
