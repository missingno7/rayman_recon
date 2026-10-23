#include "type_island_abi.h"
/* Layout-only BSS fragment; names/types and evidence are recorded in NOTES.md. */
#include "bt.h"
#include "bt_bs_54da3.h"

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

dpmi_scratch_t dpmi_workspace;
