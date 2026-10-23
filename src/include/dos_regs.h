#ifndef DOS_REGS_H
#define DOS_REGS_H
#include "rtypes.h"
/* Real-mode register block passed to sub_564f8 (DOS 1:564f8..56664 reads +0 ax .. +20 ss). INVENTED names. */
typedef struct dos_regs {
    u16 ax, bx, cx, dx, si, di, flags, cs, ds, es, ss;
} dos_regs_t;
#endif
