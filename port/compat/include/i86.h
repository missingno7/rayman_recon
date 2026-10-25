/*
 * Watcom <i86.h> interface for the native port: the register-image types the historical units fill in before a
 * software interrupt, and the interrupt/port primitives. Layouts follow Watcom 10.6 for __386__ (DWORD view,
 * WORD/BYTE views with fillers). The primitives are implemented in compat/src/dos_services.c, which answers
 * only the services the port has decided on (port/docs/BOUNDARY.md) and stops loudly on anything else.
 */
#ifndef PORT_I86_H
#define PORT_I86_H

#include <stdint.h>

struct DWORDREGS { unsigned int eax, ebx, ecx, edx, esi, edi, cflag; };
struct WORDREGS {
    unsigned short ax, _1, bx, _2, cx, _3, dx, _4, si, _5, di, _6;
    unsigned int cflag;
};
struct BYTEREGS {
    unsigned char al, ah; unsigned short _1;
    unsigned char bl, bh; unsigned short _2;
    unsigned char cl, ch; unsigned short _3;
    unsigned char dl, dh; unsigned short _4;
};
union REGS { struct DWORDREGS x; struct WORDREGS w; struct BYTEREGS h; };
struct SREGS { unsigned short es, cs, ss, ds, fs, gs; };
/* Register image an __interrupt handler receives (the port calls handlers as plain functions). */
struct INTPACKX { unsigned gs, fs, es, ds, edi, esi, ebp, esp, ebx, edx, ecx, eax, eip, cs, flags; };
union INTPACK { struct INTPACKX x; };

#ifdef __cplusplus
extern "C" {
#endif
int int386(int intno, union REGS *in, union REGS *out);
int int386x(int intno, union REGS *in, union REGS *out, struct SREGS *seg);
void segread(struct SREGS *seg);
void _disable(void);
void _enable(void);
unsigned inp(unsigned port);
unsigned outp(unsigned port, unsigned value);
unsigned inpw(unsigned port);
unsigned outpw(unsigned port, unsigned value);
#ifdef __cplusplus
}
#endif

/* Segment/offset helpers: the port has one flat address space, so a far pointer is its offset. */
#define FP_SEG(p) ((unsigned short)0)
#define FP_OFF(p) ((uintptr_t)(p))
#define MK_FP(seg, off) ((void *)(uintptr_t)(off))

#endif
