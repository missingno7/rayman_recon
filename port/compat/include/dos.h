/* Watcom <dos.h> for the native port: interrupt vectors. The platform layer calls a registered handler as an
   ordinary function (keyboard: compat/src/dos_services.c). */
#ifndef PORT_DOS_VECTORS_H
#define PORT_DOS_VECTORS_H
#include "i86.h"
typedef void (*port_isr_t)();
#ifdef __cplusplus
extern "C" {
#endif
port_isr_t _dos_getvect(int intno);
void _dos_setvect(int intno, port_isr_t handler);
void _chain_intr(port_isr_t handler);
#ifdef __cplusplus
}
#endif
#endif
