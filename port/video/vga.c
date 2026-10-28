/*
 * VGA device model and the original mode-switch entries (docs/port/ASM_SPEC.md): InitFirstModeVideo 1:045fc,
 * InitModeNormal 1:0465f, InitModeX 1:046b8, textmode 1:047e7, select_display_buffer 1:045f1.
 * The port presents chained 320x200 only; Mode X is not available (port/docs/DIVERGENCES.md).
 */
#include "video/vga.h"
#include "compat/src/pc_hardware.h"
#include "platform/clock.h"
#include "platform/host.h"
#include "render/record.h"

#include <stdio.h>
#include <string.h>

uint8_t port_vga_aperture[PORT_VGA_APERTURE_SIZE];
uint8_t port_vga_dac[768];

static int bios_mode = 0x03;
static unsigned dac_write_index, dac_write_sub, dac_read_index, dac_read_sub;
static uint8_t dac_mask = 0xff;
static uint8_t seq_index, seq_regs[8], gc_index, gc_regs[16], crtc_index, crtc_regs[32], misc_output;
static int attr_flipflop;
static uint8_t attr_index, attr_regs[32];
static int dirty;
static uint64_t last_present;
static port_vga_frame_hook frame_hook;
static void *frame_hook_user;

void port_vga_set_frame_hook(port_vga_frame_hook hook, void *user)
{
    frame_hook = hook;
    frame_hook_user = user;
}

int port_vga_mode(void) { return bios_mode; }

void port_vga_set_mode(int mode)
{
    bios_mode = mode;
    if (mode == 0x13) {
        /* Standard mode 13h timing registers (IBM VGA BIOS). */
        static const uint8_t crtc13[25] = {0x5f, 0x4f, 0x50, 0x82, 0x54, 0x80, 0xbf, 0x1f, 0x00, 0x41, 0x00, 0x00,
                                           0x00, 0x00, 0x00, 0x00, 0x9c, 0x0e, 0x8f, 0x28, 0x40, 0x96, 0xb9, 0xa3,
                                           0xff};
        memcpy(crtc_regs, crtc13, sizeof crtc13);
        seq_regs[1] = 0x01;
        seq_regs[4] = 0x0e;
        misc_output = 0x63;
        /* BIOS mode set clears video memory and loads the default palette; the game programs its own palette
           before showing anything, so only the cleared memory is modelled. */
        memset(port_vga_aperture, 0, 0x10000);
    }
    dirty = 1;
}

static uint8_t shown_pixels[(320 + 2 * 160) * 200];
static int shown_width;
static uint8_t shown_dac[768];
static int shown_valid;

/* Like a monitor scanning the window: an image is shown when the window memory or the DAC changed. */
static void present(void)
{
    static const uint8_t black[768];
    const uint8_t *dac = bios_mode == 0x13 ? port_vga_dac : black;
    /* Native renderer: its own display (render/record.c), as wide as its view; otherwise the original display
       memory, 320 x 200. */
    int width = 320;
    const uint8_t *image = port_record_presented(&width);
    size_t bytes;
    if (!image) {
        image = port_vga_aperture;
        width = 320;
    }
    bytes = (size_t)width * 200;
    dirty = 0;
    if (shown_valid && width == shown_width && !memcmp(shown_pixels, image, bytes) &&
        !memcmp(shown_dac, dac, sizeof shown_dac)) {
        port_pc_poll();
        return;
    }
    memcpy(shown_pixels, image, bytes);
    memcpy(shown_dac, dac, sizeof shown_dac);
    shown_width = width;
    shown_valid = 1;
    last_present = port_clock_ns();
    if (frame_hook)
        frame_hook(image, width, dac, frame_hook_user);
    port_host_present(image, width, width, dac);
    port_pc_poll();
}

/* Called by the frame swaps (Swap_To_Screen and the joystick-polling swaps): the display now holds a built frame. */
void port_vga_display_changed(void)
{
    dirty = 1;
    present();
}

/* Refresh timing from the programmed registers, as the monitor saw it: dot clock (misc output bits 2-3),
   horizontal total (CRTC 0 + 5 character clocks of 8 or 9 dots, sequencer 1 bit 0) and vertical total
   (CRTC 6 with overflow bits 8/9 in CRTC 7, + 2). Mode 13h defaults give 25.175 MHz / (800 x 449) = 70.086 Hz;
   the game reprograms them for its 50/60/70/80/100 Hz choices (frame.c). */
static uint64_t frame_ns(void)
{
    static const uint64_t clocks[4] = {25175000, 28322000, 25175000, 25175000};
    uint64_t dots = (uint64_t)(crtc_regs[0] + 5) * ((seq_regs[1] & 1) ? 8 : 9);
    uint64_t lines = (uint64_t)crtc_regs[6] + ((crtc_regs[7] & 1) << 8) + ((crtc_regs[7] & 0x20) << 4) + 2;
    uint64_t hz_clock = clocks[(misc_output >> 2) & 3];
    if (bios_mode != 0x13 || dots < 8 || lines < 2)
        return PORT_VGA_FRAME_NS;
    return dots * lines * 1000000000ULL / hz_clock;
}

static uint64_t line_ns(void)
{
    return frame_ns() / 449;
}

static uint64_t vsync_ns(void)
{
    return 2 * line_ns();
}

static uint64_t retrace_phase(uint64_t now) { return now % frame_ns(); }

void port_vga_wait_retrace(int in_retrace)
{
    uint64_t now = port_clock_ns();
    uint64_t phase = retrace_phase(now);
    int inside = phase < vsync_ns();
    if (in_retrace && !inside) {
        port_clock_wait_until(now - phase + frame_ns());
        /* A refresh begins: show the window if it changed since the last image. */
        present();
    } else if (!in_retrace && inside) {
        port_clock_wait_until(now - phase + vsync_ns());
    }
}

unsigned port_vga_inp(unsigned port)
{
    switch (port) {
    case 0x3da: {
        uint64_t now = port_clock_ns();
        uint64_t phase = retrace_phase(now);
        unsigned inside = phase < vsync_ns();
        attr_flipflop = 0;
        /* Deterministic time does not pass while a loop polls; each read moves to the next edge. */
        if (port_clock_mode() == PORT_CLOCK_DETERMINISTIC)
            port_clock_wait_until(now - phase + (inside ? vsync_ns() : frame_ns()));
        return inside ? 0x09 : 0x00;
    }
    case 0x3c9: {
        uint8_t v = port_vga_dac[dac_read_index * 3 + dac_read_sub];
        if (++dac_read_sub == 3) {
            dac_read_sub = 0;
            dac_read_index = (dac_read_index + 1) & 255;
        }
        return v;
    }
    case 0x3c6: return dac_mask;
    case 0x3c4: return seq_index;
    case 0x3c5: return seq_regs[seq_index & 7];
    case 0x3ce: return gc_index;
    case 0x3cf: return gc_regs[gc_index & 15];
    case 0x3d4: return crtc_index;
    case 0x3d5: return crtc_regs[crtc_index & 31];
    case 0x3cc: return misc_output;
    default: break;
    }
    {
        char what[48];
        snprintf(what, sizeof what, "VGA IN from port 0x%x", port);
        port_unimplemented(what);
    }
    return 0;
}

void port_vga_outp(unsigned port, unsigned value)
{
    value &= 0xff;
    switch (port) {
    case 0x3c8: dac_write_index = value; dac_write_sub = 0; return;
    case 0x3c7: dac_read_index = value; dac_read_sub = 0; return;
    case 0x3c9:
        port_vga_dac[dac_write_index * 3 + dac_write_sub] = (uint8_t)(value & 63);
        if (++dac_write_sub == 3) {
            dac_write_sub = 0;
            dac_write_index = (dac_write_index + 1) & 255;
        }
        dirty = 1;
        return;
    case 0x3c6: dac_mask = (uint8_t)value; return;
    case 0x3c4: seq_index = (uint8_t)value; return;
    case 0x3c5: seq_regs[seq_index & 7] = (uint8_t)value; return;
    case 0x3ce: gc_index = (uint8_t)value; return;
    case 0x3cf: gc_regs[gc_index & 15] = (uint8_t)value; return;
    case 0x3d4: crtc_index = (uint8_t)value; return;
    case 0x3d5: crtc_regs[crtc_index & 31] = (uint8_t)value; return;
    case 0x3c2: misc_output = (uint8_t)value; return;
    case 0x3c0:
        if (!attr_flipflop)
            attr_index = (uint8_t)value;
        else
            attr_regs[attr_index & 31] = (uint8_t)value;
        attr_flipflop = !attr_flipflop;
        return;
    default: break;
    }
    {
        char what[48];
        snprintf(what, sizeof what, "VGA OUT 0x%x to port 0x%x", value, port);
        port_unimplemented(what);
    }
}

/* ---- original mode entries -------------------------------------------------------------------------------- */
void InitFirstModeVideo(void) { port_vga_set_mode(0x13); }
/* Reprograms the chained-mode registers only: no BIOS call, memory and palette are kept. */
void InitModeNormal(void) { bios_mode = 0x13; dirty = 1; }
void InitModeX(void) { port_unimplemented("InitModeX: Mode X presentation (the port presents normal mode only)"); }
void textmode(void) { port_vga_set_mode(0x03); }
/* 1:045f1: CRTC register 0Ch = AH of the buffer's linear address (A0000h + offset). Normal mode passes the window
   start (0); Mode X page flipping (A0500h/A6500h) is not presented by the port. */
void select_display_buffer(uint8_t *buffer)
{
    uintptr_t offset = (uintptr_t)(buffer - port_vga_aperture);
    crtc_regs[0x0c] = (uint8_t)((0xa0000u + offset) >> 8);
    if (offset >= PORT_VGA_APERTURE_SIZE || crtc_regs[0x0c] || crtc_regs[0x0d])
        port_unimplemented("select_display_buffer: display start other than the window start (Mode X paging)");
}
