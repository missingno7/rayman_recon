/*
 * The display device the game drew to: the VGA memory window it writes in normal (chained 320x200) mode, the
 * 256-entry 6-bit DAC palette, the vertical retrace signal and the BIOS video mode. Presentation to the window is
 * a side effect of the device, like a monitor scanning the VGA memory: it never changes what the game computes.
 */
#ifndef PORT_VIDEO_VGA_H
#define PORT_VIDEO_VGA_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* The A0000h window (64 KiB) plus the B0000h half the game never addresses in normal mode. */
#define PORT_VGA_APERTURE_SIZE 0x20000
extern uint8_t port_vga_aperture[PORT_VGA_APERTURE_SIZE];

/* 768 bytes, 6-bit components as programmed through port 0x3C9. */
extern uint8_t port_vga_dac[768];

/* BIOS video mode (0x13 graphics, 0x03 text). */
int port_vga_mode(void);
void port_vga_set_mode(int bios_mode);

/* The game finished writing a new image to the visible window (Swap_To_Screen and friends). */
void port_vga_display_changed(void);

/* Wait for the retrace signal: in_retrace = 1 waits until inside vertical retrace (synchro),
   0 until outside it (endsynchro). */
void port_vga_wait_retrace(int in_retrace);

/* VGA register ports 0x3C0..0x3DF. */
unsigned port_vga_inp(unsigned port);
void port_vga_outp(unsigned port, unsigned value);

/* Observers of every presented image (tests, recordings): width x 200 pixels (320, or the native renderer's wider
   view), pitch width, DAC 768 bytes. */
typedef void (*port_vga_frame_hook)(const uint8_t *pixels, int width, const uint8_t *dac, void *user);
void port_vga_set_frame_hook(port_vga_frame_hook hook, void *user);

#ifdef __cplusplus
}
#endif

#endif
