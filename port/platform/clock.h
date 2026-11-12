/*
 * The port's single time source: the time of the DOS machine the game runs on. The game read time from hardware
 * (PIT channel 2 for clock_ticks, the VGA retrace status for synchro, the BIOS tick for waits); video/ and compat/
 * derive those signals from this clock, so the game's own scheduling decisions run unchanged on top of it.
 *
 * Machine time is virtual in every mode: it advances only when the game waits for a future event (a retrace, a
 * timer deadline, the next counter value of a polled timer) - the behaviour of an ideal, fast enough machine,
 * identical on every host and in every run. Nothing the host does (presenting a frame, a late wake-up, a slow
 * disk) is ever visible to the game as time. The original's timing decisions - its timer calibration, its 31/32
 * frame deadline, its frame-skip mode - therefore come out the same in play as in the tests.
 *
 *  REALTIME       paced: each wait also lets the host's clock reach the moment the machine's time is due, so the
 *                 game runs at its own speed for the player. Sound streams, wall-clock time().
 *  DETERMINISTIC  not paced: runs as fast as the host computes (tests, recordings). No sound streams, fixed epoch.
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
/* Advance machine time to t (no effect if it is already there). REALTIME: also sleeps until that moment is due on
   the host, serving window events meanwhile. */
void port_clock_wait_until(uint64_t t);

/* The host has just shown a frame of the game, whose frame period is frame_ns (video/vga.c). When the display's
   refresh rate is known (port_host_refresh_ns) and the game's frame is a whole number of refreshes to within
   PORT_CLOCK_LOCK_PPM, machine time is paced so that the frame lasts exactly that many refreshes (display lock:
   the game's speed for the player then differs from the DOS machine's by that fraction; machine time itself, and
   so everything the game computes, is unchanged). Otherwise one machine second lasts one host second. */
void port_clock_frame_shown(uint64_t frame_ns);
#define PORT_CLOCK_LOCK_PPM 10000ULL       /* 1 % */
/* A host later than this is not chased; the schedule restarts (the game slows down, it never fast-forwards). */
#define PORT_CLOCK_FORGIVE_NS 8000000ULL

/* Diagnostics (RAYMAN_PACE_TRACE): how late the host was at the last wait and at most, schedule restarts, and
   whether pacing currently follows the display. */
void port_clock_stats(uint64_t *late_ns, uint64_t *max_late_ns, uint64_t *restarts, int *display_locked);

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
