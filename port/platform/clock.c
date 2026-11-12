#include "platform/clock.h"
#include "platform/host.h"

#include <SDL3/SDL.h>

static int mode;
static uint64_t machine_now;   /* the DOS machine's time: advances only in port_clock_wait_until */

/* Pacing (PORT_CLOCK_REALTIME): host_due is the host time at which machine_now is due. One machine nanosecond
   lasts rate_num / rate_den host nanoseconds: 1, or under the display lock the ratio that makes the game's frame
   exactly a whole number of display refreshes. */
static uint64_t host_due;
static uint64_t rate_num = 1, rate_den = 1, rate_acc;
static uint64_t late_last, late_max, reanchors;

void port_clock_init(int m)
{
    mode = m;
    machine_now = 0;
    host_due = SDL_GetTicksNS();
    rate_num = rate_den = 1;
    rate_acc = 0;
    late_last = late_max = reanchors = 0;
}

int port_clock_mode(void) { return mode; }

uint64_t port_clock_ns(void) { return machine_now; }

/* Let the host reach the moment the machine's time is due. A host that arrives late by more than
   PORT_CLOCK_FORGIVE_NS (a level load, a stalled window) is not chased: the schedule restarts from now, so the game
   never runs fast to catch up. */
static void pace(void)
{
    uint64_t now = SDL_GetTicksNS();
    if (now >= host_due) {
        late_last = now - host_due;
        if (late_last > late_max)
            late_max = late_last;
        if (late_last > PORT_CLOCK_FORGIVE_NS) {
            host_due = now;
            reanchors++;
        }
        return;
    }
    late_last = 0;
    for (;;) {
        now = SDL_GetTicksNS();
        if (now >= host_due)
            break;
        /* Sleep coarsely (serving the window meanwhile), then spin the last stretch. */
        if (host_due - now > 2000000ULL) {
            port_host_pump();
            if (port_host_quit_requested())
                break;
            SDL_DelayNS(host_due - now - 1000000ULL);
        } else if (host_due - now > 200000ULL) {
            SDL_DelayNS(100000ULL);
        }
    }
}

void port_clock_wait_until(uint64_t t)
{
    uint64_t delta;
    if (machine_now >= t)
        return;
    delta = t - machine_now;
    machine_now = t;
    if (mode != PORT_CLOCK_REALTIME)
        return;
    if (rate_num == rate_den || delta > 100000000000ULL) {
        host_due += delta;
    } else {
        rate_acc += delta * rate_num;   /* exact: the remainder carries to the next wait */
        host_due += rate_acc / rate_den;
        rate_acc %= rate_den;
    }
    pace();
}

void port_clock_frame_shown(uint64_t frame_ns)
{
    uint64_t refresh = port_host_refresh_ns(), n, num = 1, den = 1;
    if (mode != PORT_CLOCK_REALTIME)
        return;
    /* The game's frame is a whole number of display refreshes, to within PORT_CLOCK_LOCK_PPM: stretch machine time
       so that it is exactly that. Frames then keep their position between two refreshes instead of drifting
       across one every few seconds (which shows as one longer or shorter frame each time). */
    if (refresh && frame_ns && frame_ns < 1000000000ULL) {
        n = (frame_ns + refresh / 2) / refresh;
        if (n >= 1 && (n * refresh > frame_ns ? n * refresh - frame_ns : frame_ns - n * refresh) * 1000000ULL <=
                          frame_ns * PORT_CLOCK_LOCK_PPM) {
            num = n * refresh;
            den = frame_ns;
        }
    }
    if (num != rate_num || den != rate_den) {
        rate_num = num;
        rate_den = den;
        rate_acc = 0;
    }
}

void port_clock_stats(uint64_t *late_ns, uint64_t *max_late_ns, uint64_t *restarts, int *display_locked)
{
    *late_ns = late_last;
    *max_late_ns = late_max;
    *restarts = reanchors;
    *display_locked = rate_num != rate_den;
}
