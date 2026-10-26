/*
 * The small part of the PC the historical C still talks to through port I/O and interrupt vectors: PIT channel 2
 * (the game's frame clock), the PIC mask, the keyboard controller's data latch and the VGA status/DAC registers.
 * Every other port or software interrupt stops the program (port_unimplemented) so that an untreated hardware
 * dependency can never pass silently. port/docs/BOUNDARY.md lists the modelled behaviour and its sources.
 */
#ifndef PORT_PC_HARDWARE_H
#define PORT_PC_HARDWARE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Let the host deliver pending events: window events, then one keyboard interrupt per queued scan code through
   the currently installed INT 9 vector (the game's own handler once Init_Clavier ran). */
void port_pc_poll(void);

/* PIT ticks (1.193182 MHz) of the port clock. */
uint64_t port_pc_pit_ticks(void);
/* IRQ0 periods elapsed; programming channel 0 retains ticks and restarts its phase. */
uint32_t port_pc_bios_ticks(void);

/* Abort with a clear message: an untreated DOS/hardware service was reached. */
void port_unimplemented(const char *what);

#ifdef __cplusplus
}
#endif

#endif
