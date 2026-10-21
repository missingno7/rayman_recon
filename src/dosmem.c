/* Candidate for obj1:56460..56670. These are DOS DPMI wrappers (INVENTED names). */
#include "rtypes.h"
#include <i86.h>

/* INVENTED layout, offsets proven at 1:564f8..1:56664. */
typedef struct real_mode_regs {
    u32 edi, esi, ebp, reserved, ebx, edx, ecx, eax;
    u16 flags, es, ds, fs, gs, ip, cs, sp, ss, padding;
} real_mode_regs_t;
typedef struct dpmi_scratch {
    union REGS regs;
    real_mode_regs_t real;
    struct SREGS segments;
} dpmi_scratch_t;
extern dpmi_scratch_t dpmi_workspace;
extern void clear_memory(void *destination, s32 value, s32 bytes);

u16 dos_allocate_conventional_block(u16 bytes, s32 *selector_and_segment)
{
    u32 paragraphs;
    u16 handle;

    dpmi_workspace.regs.x.eax = 0x100;
    paragraphs = ((s32)bytes + 15) / 16;
    dpmi_workspace.regs.x.ebx = paragraphs;
    int386(0x31, &dpmi_workspace.regs, &dpmi_workspace.regs);
    if ((dpmi_workspace.regs.x.cflag & 1) != 0) {
        *selector_and_segment = 0;
        return 0;
    }
    handle = dpmi_workspace.regs.w.dx;
    *selector_and_segment = (u32)dpmi_workspace.regs.w.ax << 16;
    return handle;
}

u8 dos_free_conventional_block(u16 handle)
{
    dpmi_workspace.regs.w.ax = 0x101;
    dpmi_workspace.regs.w.dx = handle;
    int386(0x31, &dpmi_workspace.regs, &dpmi_workspace.regs);
    return (dpmi_workspace.regs.x.cflag & 1) != 0;
}

typedef struct dos_regs {
    s16 ax, bx, cx, dx, si, di, flags, cs, ds, es, ss;
} dos_regs_t;

s16 sub_564f8(u8 interrupt_number, dos_regs_t *registers)
{
    clear_memory(&dpmi_workspace.regs, 0, 0x1c);
    clear_memory(&dpmi_workspace.segments, 0, 0x0c);
    clear_memory(&dpmi_workspace.real, 0, 0x34);

    dpmi_workspace.real.eax = (u16)registers->ax;
    dpmi_workspace.real.ebx = (u16)registers->bx;
    dpmi_workspace.real.ecx = (u16)registers->cx;
    dpmi_workspace.real.edx = (u16)registers->dx;
    dpmi_workspace.real.esi = (u16)registers->si;
    dpmi_workspace.real.edi = (u16)registers->di;
    dpmi_workspace.regs.w.ax = 0x0300;
    dpmi_workspace.real.flags = registers->flags;
    dpmi_workspace.regs.h.bl = interrupt_number;
    dpmi_workspace.real.cs = registers->cs;
    dpmi_workspace.regs.w.cx = 0;
    dpmi_workspace.real.ds = registers->ds;
    dpmi_workspace.real.es = registers->es;
    dpmi_workspace.real.ss = registers->ss;
    dpmi_workspace.regs.h.bh = 0;
    dpmi_workspace.segments.es = FP_SEG(&dpmi_workspace.real);
    dpmi_workspace.regs.x.edi = (u32)&dpmi_workspace.real;
    int386x(0x31, &dpmi_workspace.regs, &dpmi_workspace.regs,
            &dpmi_workspace.segments);

    registers->ax = (s16)dpmi_workspace.real.eax;
    registers->bx = (s16)dpmi_workspace.real.ebx;
    registers->cx = (s16)dpmi_workspace.real.ecx;
    registers->dx = (s16)dpmi_workspace.real.edx;
    registers->si = (s16)dpmi_workspace.real.esi;
    registers->di = (s16)dpmi_workspace.real.edi;
    registers->flags = (s16)dpmi_workspace.real.flags;
    registers->cs = (s16)dpmi_workspace.real.cs;
    registers->ds = (s16)dpmi_workspace.real.ds;
    registers->es = (s16)dpmi_workspace.real.es;
    registers->ss = (s16)dpmi_workspace.real.ss;
    return registers->ss;
}
