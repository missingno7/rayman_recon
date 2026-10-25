/*
 * PC hardware model and the original timing entries (src/lowlevel_04430.asm, docs/port/ASM_SPEC.md):
 * synchro 1:06e8a, endsynchro 1:06e9a, set_speaker_on 1:06eaa, set_speaker_off 1:06eea, clock_ticks 1:06efd,
 * wait_until 1:06f5d, speaker_enable 1:06f78. Their algorithms are kept; only the hardware they read is modelled.
 */
#include "compat/src/pc_hardware.h"
#include "platform/clock.h"
#include "platform/host.h"
#include "video/vga.h"

#include "compat/include/dos.h"
#include "compat/include/i86.h"
#include <stdio.h>
#include <stdlib.h>

typedef int32_t s32;
typedef uint32_t u32;

void port_unimplemented(const char *what)
{
    fprintf(stderr, "PORT: unimplemented DOS/hardware service reached: %s (port/docs/BOUNDARY.md)\n", what);
    fflush(stderr);
    exit(3);
}

/* ---- PIT channel 2 --------------------------------------------------------------------------------------- */
/* A port access took about one PIT period on the original ISA bus, so two counter latches never returned the
   same value; clock_ticks relies on that (an equal read is taken as a wrap). The model guarantees it. */
static uint64_t pit_last_latch;
static uint64_t pit_load_tick;      /* PIT tick at which the reload value was written */
static uint32_t pit_reload = 65536; /* 0 written = 65536 */
static int pit_write_low = 1;
static uint8_t pit_new_low;
static uint16_t pit_latched;
static int pit_read_low = 1;
static uint8_t port61;

uint64_t port_pc_pit_ticks(void)
{
    return port_clock_ns() * PORT_PIT_HZ / 1000000000ULL;
}

static uint64_t pit_latch_tick(void)
{
    uint64_t t = port_pc_pit_ticks();
    if (t <= pit_last_latch) {
        if (port_clock_mode() == PORT_CLOCK_DETERMINISTIC)
            port_clock_wait_until(((pit_last_latch + 1) * 1000000000ULL + PORT_PIT_HZ - 1) / PORT_PIT_HZ);
        t = pit_last_latch + 1;
    }
    pit_last_latch = t;
    return t;
}

static uint16_t pit_counter_at(uint64_t tick)
{
    /* Mode 2 counts reload..1 while the gate (port 0x61 bit 0) is high. */
    uint64_t elapsed = (port61 & 1) ? tick - pit_load_tick : 0;
    return (uint16_t)(pit_reload - (uint32_t)(elapsed % pit_reload));
}

/* ---- BIOS data area ------------------------------------------------------------------------------------ */
/* Tick count at 0040:006C (linear 0x46C): PIT channel 0 at its BIOS divisor 65536. Code polls it in loops
   (load_world waits for two tick edges); in deterministic time each read moves the clock to the next edge. */
uint8_t port_bios_tick_byte(void)
{
    uint64_t pit = port_pc_pit_ticks();
    uint8_t value = (uint8_t)(pit / 65536);
    if (port_clock_mode() == PORT_CLOCK_DETERMINISTIC)
        port_clock_wait_until(((pit / 65536 + 1) * 65536 * 1000000000ULL + PORT_PIT_HZ - 1) / PORT_PIT_HZ);
    else
        port_pc_poll();
    return value;
}

/* ---- keyboard controller and PIC -------------------------------------------------------------------------- */
static uint8_t kbd_data;
static uint8_t pic_mask;
static port_isr_t vectors[256];
static int in_interrupt;

static void bios_int9(void)
{
    /* The BIOS handler is only active before the game installs its own (Init_Clavier) and after it restores it;
       its keyboard buffer is not used by the game in those phases, so the code is consumed without effect. */
}

void port_pc_poll(void)
{
    int code;
    port_host_pump();
    if (in_interrupt)
        return;
    while ((code = port_host_next_scancode()) >= 0) {
        kbd_data = (uint8_t)code;
        if (pic_mask & 2)
            continue;
        in_interrupt = 1;
        if (vectors[9])
            vectors[9]();
        else
            bios_int9();
        in_interrupt = 0;
    }
}

port_isr_t _dos_getvect(int intno) { return vectors[intno & 255]; }
void _dos_setvect(int intno, port_isr_t handler) { vectors[intno & 255] = handler; }
void _chain_intr(port_isr_t handler)
{
    if (handler)
        handler();
}

void _disable(void) {}
void _enable(void) {}

unsigned inp(unsigned port)
{
    switch (port) {
    case 0x60: return kbd_data;
    case 0x61: return port61;
    case 0x201: return 0xff;                 /* game port, nothing connected (compat/src/gameport.c) */
    case 0x21: return pic_mask;
    case 0x42: {
        uint8_t v = pit_read_low ? (uint8_t)pit_latched : (uint8_t)(pit_latched >> 8);
        pit_read_low = !pit_read_low;
        return v;
    }
    default:
        if (port >= 0x3c0 && port <= 0x3df)
            return port_vga_inp(port);
    }
    {
        char what[64];
        snprintf(what, sizeof what, "IN from port 0x%x", port);
        port_unimplemented(what);
    }
    return 0;
}

unsigned outp(unsigned port, unsigned value)
{
    value &= 0xff;
    switch (port) {
    case 0x20: return value;                 /* end of interrupt */
    case 0x201: return value;                /* game port one-shot trigger: nothing connected */
    case 0x21: pic_mask = (uint8_t)value; return value;
    case 0x61: port61 = (uint8_t)value; return value;
    case 0x43:
        if ((value & 0xc0) == 0x80 && (value & 0x30) == 0) {   /* latch channel 2 */
            pit_latched = pit_counter_at(pit_latch_tick());
            pit_read_low = 1;
        } else if ((value & 0xc0) == 0x80) {                   /* channel 2 mode/access */
            pit_write_low = 1;
        }
        return value;
    case 0x42:
        if (pit_write_low) {
            pit_new_low = (uint8_t)value;
        } else {
            pit_reload = pit_new_low | (value << 8);
            if (!pit_reload)
                pit_reload = 65536;
            pit_load_tick = port_pc_pit_ticks();
        }
        pit_write_low = !pit_write_low;
        return value;
    default:
        if (port >= 0x3c0 && port <= 0x3df) {
            port_vga_outp(port, value);
            return value;
        }
    }
    {
        char what[64];
        snprintf(what, sizeof what, "OUT 0x%x to port 0x%x", value, port);
        port_unimplemented(what);
    }
    return value;
}

unsigned inpw(unsigned port)
{
    return inp(port) | (inp(port + 1) << 8);
}

unsigned outpw(unsigned port, unsigned value)
{
    outp(port, value & 0xff);
    outp(port + 1, (value >> 8) & 0xff);
    return value;
}

/* ---- original timing entries ------------------------------------------------------------------------------ */
/* lowlevel_timer_02148..02158: period, last, epoch, repeat (ASM_SPEC clock_ticks). */
static u32 timer_period, timer_last, timer_epoch, timer_repeat;

void synchro(void)
{
    port_vga_wait_retrace(1);
}

void endsynchro(void)
{
    port_vga_wait_retrace(0);
}

void set_speaker_on(void)
{
    timer_last = timer_epoch = timer_repeat = 0;
    outp(0x61, inp(0x61) | 3);
    timer_period = 65535;
    outp(0x43, 0xb4);
    outp(0x42, 0xff);
    outp(0x42, 0xff);
}

void set_speaker_off(void)
{
    outp(0x61, inp(0x61) & 252);
    outp(0x43, 0xb4);
    outp(0x42, 0);
    outp(0x42, 0);
}

void speaker_enable(void)
{
    set_speaker_on();
}

s32 clock_ticks(void)
{
    u32 counter, delta, repeat = 0;
    outp(0x43, 0x80);
    counter = inp(0x42);
    counter |= inp(0x42) << 8;
    delta = timer_period - counter;
    if ((s32)delta <= (s32)timer_last) {
        if (delta == timer_last) {
            repeat = timer_repeat + 1;
            if ((s32)repeat > 20) {
                timer_period = 0;
                repeat = 0;
            }
        }
        timer_epoch += timer_period;
    }
    timer_last = delta;
    timer_repeat = repeat;
    return (s32)(delta + timer_epoch);
}

s32 wait_until(s32 target)
{
    u32 polls;
    if (!timer_period)
        return 1;
    polls = 0xffffffffu;
    for (;;) {
        s32 now = clock_ticks();
        polls++;
        if (now > target)
            break;
        /* The original busy-polls. Let the port clock reach the target, in steps shorter than one counter
           period so that clock_ticks still observes every wrap. */
        {
            uint64_t ahead = (uint64_t)((int64_t)target - now + 1);
            if (ahead > 30000)
                ahead = 30000;
            port_clock_wait_until(port_clock_ns() + ahead * 1000000000ULL / PORT_PIT_HZ);
            port_pc_poll();
        }
    }
    return (s32)polls;
}
