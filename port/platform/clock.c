#include "platform/clock.h"
#include "platform/host.h"

#include <SDL3/SDL.h>

static int mode;
static uint64_t origin;
static uint64_t virtual_now;

void port_clock_init(int m)
{
    mode = m;
    origin = SDL_GetTicksNS();
    virtual_now = 0;
}

int port_clock_mode(void) { return mode; }

uint64_t port_clock_ns(void)
{
    if (mode == PORT_CLOCK_DETERMINISTIC)
        return virtual_now;
    return SDL_GetTicksNS() - origin;
}

void port_clock_wait_until(uint64_t t)
{
    if (mode == PORT_CLOCK_DETERMINISTIC) {
        if (virtual_now < t)
            virtual_now = t;
        return;
    }
    for (;;) {
        uint64_t now = port_clock_ns();
        if (now >= t)
            break;
        port_host_pump();
        if (port_host_quit_requested())
            break;
        /* Sleep coarsely, then spin the last stretch: retrace edges are only ~64 us apart. */
        if (t - now > 2000000ULL)
            SDL_DelayNS(t - now - 1000000ULL);
        else if (t - now > 200000ULL)
            SDL_DelayNS(100000ULL);
    }
}
