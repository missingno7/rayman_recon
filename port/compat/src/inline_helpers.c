/*
 * Portable bodies of the ORIGINAL_INLINE helpers (src/include/inline_helpers.h, src/original_inline.h,
 * src/menu_inline.h, src/cheat_inline.h, src/frame_local.h and the unit-local ones). Each reproduces the visible
 * effect of the exact instruction sequence documented next to its declaration; hardware sequences go through the
 * port's device model (compat/src/pc_hardware.c, video/vga.c). Port decisions for the protection and debug
 * sequences are in port/docs/DIVERGENCES.md.
 */
#include "compat/src/pc_hardware.h"
#include "platform/clock.h"
#include "video/vga.h"

#include "compat/include/i86.h"
#include <stdlib.h>
#include <string.h>

#include "rtypes.h"

/* ---- src/include/inline_helpers.h ------------------------------------------------------------------------- */

/* test ax,ax / js / sar ax,cl / neg-sar-neg: rounds toward zero; -32768 negates to itself. Count masked to 5 bits. */
s16 ashr16(s16 value, u8 shift)
{
    unsigned c = shift & 31;
    if (value >= 0)
        return (s16)(value >> (c > 15 ? 15 : c));
    {
        s16 v = (s16)(u16)(0u - (u16)value);
        v = (s16)(v >> (c > 15 ? 15 : c));
        return (s16)(u16)(0u - (u16)v);
    }
}

/* cwde; shl eax,cl */
s32 ashl16(s16 value, u8 shift) { return (s32)((u32)(s32)value << (shift & 31)); }
/* shl eax,cl */
s32 ashl32(s32 value, u8 shift) { return (s32)((u32)value << (shift & 31)); }
/* zero path negates 0; both paths are an arithmetic shift */
s32 ashr32_inline(s32 value, u8 shift) { return value >> (shift & 31); }
/* test eax,eax / jge / neg eax: INT_MIN stays INT_MIN */
s32 abs32_inline(s32 value) { return value >= 0 ? value : (s32)(0u - (u32)value); }
s32 inrange(s32 value, s32 lower, s32 upper) { return value >= lower && value <= upper; }
s32 sgn(s32 value) { return value > 0 ? 1 : value < 0 ? -1 : 0; }
s32 min32(s32 a, s32 b) { return a < b ? a : b; }
s32 max32(s32 a, s32 b) { return a > b ? a : b; }

/* mov ecx,16000; rep stosd */
void fill(void *destination) { memset(destination, 0, 64000); }
void fill_cld(void *destination) { memset(destination, 0, 64000); }

/* cld; rep movsb (count & 3); rep movsd (count >> 2): forward, overlap-sensitive like the original. */
static void forward_copy(const u8 *s, u8 *d, u32 count)
{
    u32 i, n = count & 3;
    for (i = 0; i < n; i++)
        *d++ = *s++;
    for (i = 0; i < count >> 2; i++, s += 4, d += 4) {
        u8 t[4];
        memcpy(t, s, 4);
        memcpy(d, t, 4);
    }
}
void copy(const void *source, void *destination, u32 count) { forward_copy(source, destination, count); }
void copy_guarded(const void *source, void *destination, u32 count) { forward_copy(source, destination, count); }

/* out 3c8h,first; rep outsb 3*(last-first+1) bytes from palette+3*first to 3c9h */
void palette_output(s32 first, s32 last, const void *palette)
{
    u32 n = (u32)(last - first + 1) * 3;
    const u8 *p = (const u8 *)palette + 3 * first;
    outp(0x3c8, (u8)first);
    while (n--)
        outp(0x3c9, *p++);
}

/* DAC index 1, 0x2fd bytes from palette+3 */
void write_movie_palette(u8 *palette)
{
    u32 i;
    outp(0x3c8, 1);
    for (i = 0; i < 0x2fd; i++)
        outp(0x3c9, palette[3 + i]);
}

/* ---- src/frame_local.h, src/menu_inline.h: port string output to the DAC ----------------------------------- */
void frame_palette_index(u32 port, u32 color) { outp(port, color); }
void frame_palette_stream(u32 port, const u8 *source, u32 count) { while (count--) outp(port, *source++); }
void frame_disable_irq(void) {}
void frame_enable_irq(void) {}
void menu_palette_index(u16 port, u8 index) { outp(port, index); }
void menu_palette_stream(u16 port, const u8 *source, u32 count) { while (count--) outp(port, *source++); }
void menu_palette_disable(void) {}
void menu_palette_enable(void) {}

/* ---- interrupt flag, PIC (input.c, cheat.c, load.c) ----------------------------------------------------- */
void inhibit_interrupts(void) {}
void permit_interrupts(void) {}
void load_enable_interrupts(void) {}
u8 read_pic_mask(void) { return (u8)inp(0x21); }
void write_pic_mask(u8 mask) { outp(0x21, mask); }
void enable_tick_interrupts(void) {}

/* ---- CPU flags and debug registers (src/original_inline.h, load.c, menu_inline.h) -------------------------
 * The startup debug-register watch is omitted by port decision (docs/port/PLATFORM_OPEN.md): there are no
 * hardware breakpoints, the watch registers read as zero and writes have no effect. */
u32 read_processor_flags(void) { return 0x202; }   /* IF set, reserved bit 1 */
void write_processor_flags(u32 flags) { (void)flags; }
void write_debug_address0(u32 v) { (void)v; }
void write_debug_address1(u32 v) { (void)v; }
void write_debug_address2(u32 v) { (void)v; }
void write_debug_address3(u32 v) { (void)v; }
u32 read_debug_status(void) { return 0; }
void write_debug_status(u32 v) { (void)v; }
u32 read_debug_control(void) { return 0; }
void write_debug_control(u32 v) { (void)v; }
void load_watch_address(u32 address) { (void)address; }
u32 load_watch_control(void) { return 0; }
void load_set_watch_control(u32 control) { (void)control; }
/* INT3 breakpoints in the loader: without a debugger the DOS handler returned immediately. */
void load_break_world_open(void) {}
void load_break_world_backgrounds(void) {}
void load_break_world_eta(void) {}
void load_break_level_open(void) {}
void load_break_level_map(void) {}
void load_break_level_object(void) {}
void load_break_level_close(void) {}
void load_break_big_ray_animation(void) {}
void load_break_big_ray_eta(void) {}
void load_break_world_animation(void) {}
void load_break_fix_eta(void) {}
/* DR0..DR3 all zero: no breakpoint, execution continues (1:3a45a..3a482). */
void menu_check_debug_breakpoints(void) {}
/* Self-modifying run-once trampoline (1:39eb5..39edf): it only patches its own code and restores ESP. */
void menu_patch_once(void) {}

/* ---- protection sequences (src/cheat_inline.h) ---------------------------------------------------------- */
/* 1:26d73: a detected crack sets text mode and terminates. */
void exit_if_cracked(s32 cracked)
{
    if (cracked) {
        port_vga_set_mode(0x03);
        exit(0);
    }
}

/* 1:2662a: INT 21h AH=36h on the CD drive must report a read-only medium. The port serves the game data the
   player installed in place of the CD, so the drive check succeeds (port decision, DIVERGENCES.md). */
void exit_if_not_cd_drive(u32 service, u8 drive)
{
    (void)service;
    (void)drive;
}
