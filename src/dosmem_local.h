/* Exact dosmem.c declarations; the pinned SDK supplies REGS/SREGS. */
#ifndef DOSMEM_LOCAL_H
#define DOSMEM_LOCAL_H
#include "rtypes.h"
#include <i86.h>
typedef struct real_mode_regs {
    u32 edi, esi, ebp, reserved, ebx, edx, ecx, eax;
    u16 flags, es, ds, fs, gs, ip, cs, sp, ss, padding;
} real_mode_regs_t;
typedef struct dpmi_scratch {
    union REGS regs;
    real_mode_regs_t real;
    struct SREGS segments;
} dpmi_scratch_t;
#endif
