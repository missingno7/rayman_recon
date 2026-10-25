/*
 * Software interrupts and DPMI real-mode calls the historical units issue, answered as the port's machine:
 * no Windows, virtual MSCDEX, no mouse, a DOS 6.22 kernel, VGA mode switches. Anything else stops the program.
 * The replaced unit src/dosmem.c (DPMI wrappers) is implemented here with its original contracts.
 */
#include "compat/src/pc_hardware.h"
#include "platform/clock.h"
#include "platform/cdrom.h"
#include "video/vga.h"

#include "compat/include/i86.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rtypes.h"
#include "dos_regs.h"

static void unknown(const char *kind, int intno, unsigned ax)
{
    char what[80];
    snprintf(what, sizeof what, "%s INT %02Xh AX=%04Xh", kind, intno, ax & 0xffff);
    port_unimplemented(what);
}

/* BIOS time-of-day ticks (18.2065 Hz: PIT/65536) since the port clock started. */
static uint32_t bios_ticks(void)
{
    return (uint32_t)(port_pc_pit_ticks() / 65536);
}

/* Original ASM 1:02748..0275b: INT 1Ah AH=0, store CX:DX at [ESI].
   TestCdLoop and pause timing use this same BIOS tick domain. Reading never waits. */
void sub_2748(uint32_t *out)
{
    *out = bios_ticks();
}

int int386(int intno, union REGS *in, union REGS *out)
{
    union REGS r = *in;
    r.x.cflag = 0;
    switch (intno) {
    case 0x10:
        switch (r.h.ah) {
        case 0x00: port_vga_set_mode(r.h.al & 0x7f); break;
        case 0x0f: r.h.al = (unsigned char)port_vga_mode(); r.h.ah = 80; r.h.bh = 0; break;
        default: unknown("video BIOS", intno, r.w.ax);
        }
        break;
    case 0x21:
        switch (r.h.ah) {
        case 0x30: r.h.al = 6; r.h.ah = 22; r.h.bh = 0; break;   /* DOS 6.22 */
        case 0x4c: fflush(NULL); exit(r.h.al);
        default: unknown("DOS", intno, r.w.ax);
        }
        break;
    case 0x33:
        if (r.w.ax == 0) {
            r.w.ax = 0;                                          /* no mouse driver */
            break;
        }
        unknown("mouse", intno, r.w.ax);
        break;
    case 0x1a:
        if (r.h.ah == 0) {
            uint32_t t = bios_ticks();
            r.w.cx = (unsigned short)(t >> 16);
            r.w.dx = (unsigned short)t;
            r.h.al = 0;
            break;
        }
        unknown("BIOS clock", intno, r.w.ax);
        break;
    default:
        unknown("software", intno, r.w.ax);
    }
    *out = r;
    return (int)r.x.eax;
}

int int386x(int intno, union REGS *in, union REGS *out, struct SREGS *seg)
{
    (void)seg;
    if (intno == 0x31)
        unknown("DPMI (pointer-carrying; replace the caller)", intno, in->w.ax);
    return int386(intno, in, out);
}

void segread(struct SREGS *seg)
{
    memset(seg, 0, sizeof *seg);
}

/* ---- src/dosmem.c (DOS 1:56460..56670), replaced ---------------------------------------------------------- */

/* DPMI 0100h: bounded virtual conventional storage, used by CD request packets. */
u16 dos_allocate_conventional_block(u16 bytes, u32 *selector_and_segment)
{
    return port_cd_allocate(bytes, selector_and_segment);
}

u8 dos_free_conventional_block(u16 handle)
{
    return port_cd_free(handle);
}

/* Real-mode interrupt through DPMI 0300h. Installation checks of absent resident programs leave the registers
   unchanged, exactly as an unhooked real-mode vector chain does. */
s16 sub_564f8(u8 interrupt_number, dos_regs_t *registers)
{
    if (interrupt_number == 0x2f) {
        if (port_cd_interrupt(registers)) return registers->ss;
        switch (registers->ax) {
        case 0x1600:   /* Windows enhanced mode check: AL 0 = not running */
            registers->ax = 0x1600 & 0xff00;
            return registers->ss;
        case 0x4680:   /* Windows 3.x real/standard mode check: unchanged = not running */
            return registers->ss;
        default:
            break;
        }
    }
    unknown("real-mode", interrupt_number, registers->ax);
    return 0;
}
