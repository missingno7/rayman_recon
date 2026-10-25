/*
 * Game-port joystick entries (docs/port/ASM_SPEC.md: get_joy_input2_dos1/2 1:0595d/1:05a57,
 * get_joy_input2_windows1/2 1:05b55/1:05c13, dos_game_io_port_something(2) 1:05cdf/1:05d14,
 * get_joy_input1_dos1/2 1:05d4a/1:05f34) and DetectCpu 1:0611e.
 *
 * The port has no analog game port yet: port 0x201 reads FFh (no buttons pressed, axis one-shots never
 * discharge), which is what the original code saw with nothing plugged in. Every routine is therefore its
 * original no-joystick outcome: buttons released, both axes at the calibrated centres after the 2000-poll
 * timeout, and for the dos1/2 copy variants the full framebuffer copy (floor(width/4)*4 bytes per row, pitches
 * 320). A gamepad mapping onto these globals is a later platform step (port/docs/BOUNDARY.md).
 */
#include <string.h>

#include "compat/src/pc_hardware.h"
#include "platform/clock.h"
#include "rtypes.h"

extern s16 joyx0, joyy0, joy_buttonA1, joy_buttonA2, joy_buttonB1, joy_buttonB2, xpadcentre, ypadcentre;
extern s16 During_The_Menu;

void port_vga_display_changed(void);

/* With nothing connected the original sampling loops ran their 2000-read timeout (about 2 ms of ISA port
   reads), during which interrupts (keyboard) were delivered. The port charges that time and delivers them, so
   the game's input-polling loops behave as on the machine (and terminate in deterministic time). */
#define GAMEPORT_TIMEOUT_NS 2000000ULL

static void no_joystick(int sample_axes)
{
    port_clock_wait_until(port_clock_ns() + GAMEPORT_TIMEOUT_NS);
    port_pc_poll();
    joy_buttonA1 = joy_buttonA2 = joy_buttonB1 = joy_buttonB2 = 0;
    if (sample_axes) {
        joyx0 = xpadcentre;
        joyy0 = ypadcentre;
    }
}

void get_joy_input2_dos1(void) { no_joystick(During_The_Menu != 1); }
void get_joy_input2_dos2(void) { no_joystick(During_The_Menu != 1); }
void get_joy_input2_windows1(void) { no_joystick(1); }
void get_joy_input2_windows2(void) { no_joystick(1); }

/* Axis presence test: the selected axis bits never fall, so the test fails. */
s32 dos_game_io_port_something(void) { return 0; }
s32 dos_game_io_port_something2(void) { return 0; }

static void copy_rows(u8 *source, u8 *destination, s32 width, s32 height)
{
    s32 y, n = (width / 4) * 4;
    for (y = 0; y < height; y++)
        memcpy(destination + y * 320, source + y * 320, (size_t)n);
}

/* C callers pass (source, destination, width, height) like Swap_To_Screen (island_swap_t). */
s32 get_joy_input1_dos1(u8 *source, u8 *destination, s32 width, s32 height)
{
    copy_rows(source, destination, width, height);
    no_joystick(1);
    port_vga_display_changed();
    return height;
}

s32 get_joy_input1_dos2(u8 *source, u8 *destination, s32 width, s32 height)
{
    copy_rows(source, destination, width, height);
    no_joystick(1);
    port_vga_display_changed();
    return height;
}

/* CPUID family of the port's virtual processor: a Pentium (5). TheProcesseur is only printed and stored. */
u8 DetectCpu(void) { return 5; }
