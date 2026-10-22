/* INVENTED descriptive bindings. DOS timer group 1:71b83..7233a. */
#include "hmi_local.h"
#include <i86.h>

extern void __cdecl hmi_timer_set_divisor(u32 divisor);
extern void __cdecl hmi_timer_install_irq0(u32 vector, void (__far *handler)(void));
extern void hmi_timer_mask_irq0(void);
extern void hmi_timer_unmask_irq0(void);
extern void hmi_timer_restore_irq0(void);
extern void hmi_timer_chain_irq0(void);

/* PROVEN contiguous timer state at 2:2e707..2:2e83c in object 2. */
unsigned long hmi_timer_divisor = 0xffffffffUL;
void __far *hmi_timer_callbacks[16] = { 0 };
unsigned long hmi_timer_periods[16] = { 0 };
unsigned long hmi_timer_counts[16] = { 0 };
unsigned long hmi_timer_fractions[16] = { 0 };
unsigned char hmi_timer_options[16] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
unsigned char hmi_timer_last_option = 0;

void __far hmi_timer_dispatch(void);
void __far hmi_timer_irq_tail(void);

void hmi_timer_begin(void) {}

s32 sub_71B98(u32 period, u32 flags)
{
    /* HYPOTHESIS: models the unused dword at EBP-4 in the DOS frame. */
    u32 stack_slot;

    if (flags & 2)
        hmi_timer_install_mode = 1;
    else
        hmi_timer_install_mode = 0;

    if (!(flags & 1)) {
        hmi_timer_enabled = 1;
        hmi_timer_mask_irq0();
        hmi_timer_install_irq0(0xffff, hmi_timer_dispatch);
        hmi_timer_unmask_irq0();
    } else {
        hmi_timer_enabled = 0;
    }

    if (period != 0 && !(flags & 1)) {
        if (period == 0xff00) {
            hmi_timer_program_divisor(0xffff);
            hmi_timer_periods[15] = 0xff00;
        } else {
            hmi_timer_program_divisor(0x1234dc / period);
            hmi_timer_periods[15] = period;
        }
        hmi_timer_callbacks[15] = (void __far *)hmi_timer_irq_tail;
        hmi_timer_counts[15] = 0x10000;
    } else {
        hmi_timer_divisor = 0xffff;
    }

    return 0;
}

s32 sub_71C74(u32 flags)
{
    if (hmi_timer_enabled) {
        hmi_timer_mask_irq0();
        hmi_timer_restore_irq0();
        hmi_timer_unmask_irq0();
    }

    return 0;
}

s32 sub_71CAE(u32 period, void __far *callback, s32 __far *slot_out)
{
    u32 i;
    u32 slot;
    u32 factor;
    u32 value;

    factor = 0;
    for (i = 0; i < 16; i++) {
        if (hmi_timer_callbacks[i] == 0)
            break;
    }
    if (i >= 16) {
        return 11;
    } else {
        if (hmi_timer_enabled)
            hmi_timer_mask_irq0();
        slot = i;
        hmi_timer_callbacks[i] = callback;
        hmi_timer_periods[i] = period;

        if (0x1234dc / period < hmi_timer_divisor) {
            hmi_timer_program_divisor(0x1234dc / period);
            factor = (hmi_timer_divisor << 16) / (0x1234dc / period);
        }

        for (i = 0; i < 16; i++) {
            if (hmi_timer_callbacks[i] != 0) {
                if (hmi_timer_periods[i] == 0xff00) {
                    if (hmi_timer_divisor == 0xffff)
                        hmi_timer_counts[i] = 0x10000;
                    else
                        hmi_timer_counts[i] = 0x123333 / (0x1234dc / hmi_timer_divisor);
                } else {
                    hmi_timer_counts[i] = (hmi_timer_periods[i] << 16) /
                        (0x1234dc / hmi_timer_divisor);
                }

                if (factor != 0) {
                    value = hmi_timer_fractions[i] * (factor & 0xffff) >> 16;
                    value += hmi_timer_fractions[i] * (factor >> 16) >> 16;
                    hmi_timer_fractions[i] = value;
                }
            }
        }

        if (hmi_timer_enabled)
            hmi_timer_unmask_irq0();
        *slot_out = slot;
        return 0;
    }
}

s32 hmi_timer_set_period(u32 slot, u32 period)
{
    u32 i;

    if (slot >= 16)
        return 10;

    if (hmi_timer_callbacks[slot] != 0) {
        if (hmi_timer_enabled)
            hmi_timer_mask_irq0();

        hmi_timer_periods[slot] = period;
        if (0x1234dc / period < hmi_timer_divisor)
            hmi_timer_program_divisor(0x1234dc / period);

        for (i = 0; i < 16; i++) {
            if (hmi_timer_callbacks[i] != 0) {
                if (hmi_timer_periods[i] == 0xff00) {
                    if (hmi_timer_divisor == 0xffff)
                        hmi_timer_counts[i] = 0x10000;
                    else
                        hmi_timer_counts[i] = 0x123333 / (0x1234dc / hmi_timer_divisor);
                } else {
                    hmi_timer_counts[i] = (hmi_timer_periods[i] << 16) /
                        (0x1234dc / hmi_timer_divisor);
                }

                hmi_timer_fractions[i] = 0;
            }
        }

        if (hmi_timer_enabled)
            hmi_timer_unmask_irq0();
    } else {
        return 10;
    }
    return 0;
}

s32 sub_72033(u32 slot)
{
    u32 i;
    u32 period;

    period = 0;
    hmi_timer_callbacks[slot] = 0;
    for (i = 0; i < 16; i++) {
        if (hmi_timer_callbacks[i] != 0) {
            if (hmi_timer_periods[i] > period && hmi_timer_periods[i] != 0xff00)
                period = hmi_timer_periods[i];
        }
    }

    if (period != 0)
        hmi_timer_program_divisor(0x1234dc / period);
    else
        hmi_timer_program_divisor(0xffff);

    if (hmi_timer_enabled)
        hmi_timer_mask_irq0();

    for (i = 0; i < 16; i++) {
        if (hmi_timer_callbacks[i] != 0) {
            if (hmi_timer_periods[i] == 0xff00) {
                if (hmi_timer_divisor == 0xffff)
                    hmi_timer_counts[i] = 0x10000;
                else
                    hmi_timer_counts[i] = 0x123333 / (0x1234dc / hmi_timer_divisor);
            } else {
                hmi_timer_counts[i] = (hmi_timer_periods[i] << 16) /
                    (0x1234dc / hmi_timer_divisor);
            }
            hmi_timer_fractions[i] = 0;
        }
    }

    if (hmi_timer_enabled)
        hmi_timer_unmask_irq0();
    return 0;
}

u32 hmi_timer_get_period(u32 slot)
{
    return hmi_timer_periods[slot];
}

s32 hmi_timer_program_divisor(u32 divisor)
{
    hmi_timer_divisor = divisor;
    hmi_timer_set_divisor(divisor);
    return 0;
}

void __far hmi_timer_dispatch(void)
{
    hmi_timer_reentry_count++;
    for (hmi_timer_dispatch_index = 0;
         hmi_timer_dispatch_index < 16; hmi_timer_dispatch_index++) {
        if (hmi_timer_callbacks[hmi_timer_dispatch_index] != 0) {
            hmi_timer_fractions[hmi_timer_dispatch_index] +=
                hmi_timer_counts[hmi_timer_dispatch_index];
            if (((u8 *)&hmi_timer_fractions[hmi_timer_dispatch_index])[2] & 1) {
                ((u16 *)&hmi_timer_fractions[hmi_timer_dispatch_index])[1] = 0;
                if (hmi_timer_options[hmi_timer_dispatch_index] != 0xff)
                    hmi_timer_last_option = hmi_timer_options[hmi_timer_dispatch_index];
                ((void (__far *)(void))hmi_timer_callbacks[hmi_timer_dispatch_index])();
            }
        }
    }
    hmi_timer_reentry_count--;
}

void __far hmi_timer_irq_tail(void)
{
    hmi_timer_chain_irq0();
}

void hmi_timer_end(void) {}
