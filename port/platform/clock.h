/*
 * The port's single time source. The DOS game read time from hardware (PIT channel 2 for clock_ticks, the VGA
 * retrace status for synchro, the BIOS tick for waits); video/ and compat/ derive those signals from this clock,
 * so the game's own scheduling decisions run unchanged on top of it.
 *
 *  REALTIME       nanoseconds since start; waits sleep.
 *  DETERMINISTIC  time advances only when the game waits for a future event (a retrace, a timer deadline):
 *                 the behaviour of an infinitely fast machine, reproducible bit for bit (tests, recordings).
 */
#ifndef PORT_PLATFORM_CLOCK_H
#define PORT_PLATFORM_CLOCK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum { PORT_CLOCK_REALTIME = 0, PORT_CLOCK_DETERMINISTIC = 1 };

void port_clock_init(int mode);
int port_clock_mode(void);
uint64_t port_clock_ns(void);
/* Block until the clock reaches t (realtime) or set it to t if earlier (deterministic). Pumps host events. */
void port_clock_wait_until(uint64_t t);

/* Hardware frequencies the original timing code assumed. */
#define PORT_PIT_HZ 1193182ULL
/* VGA mode 13h: 25.175 MHz dot clock, 800 dots x 449 lines per frame = 70.086 Hz refresh. */
#define PORT_VGA_FRAME_NS 14268239ULL
/* Vertical sync pulse: 2 of the 449 lines. */
#define PORT_VGA_VSYNC_NS 63555ULL

#ifdef __cplusplus
}
#endif

#endif
