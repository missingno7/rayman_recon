/* INVENTED descriptive names; DOS-derived HMI system group, 1:73023..743e2. */
#include "hmi_local.h"
#include <i86.h>

void hmi_system_begin(void) {}
void hmi_system_end(void);

extern void hmi_errors_begin(void);
extern void hmi_errors_end(void);
extern void hmi_driver_callback_begin(void);
extern void hmi_driver_callback_end(void);
extern void hmi_status_begin(void);
extern void hmi_status_end(void);
extern void hmi_loader_begin(void);
extern void hmi_loader_end(void);
extern void hmi_active_begin(void);
extern void hmi_active_end(void);
extern void hmi_callback_set_begin(void);
extern void hmi_callback_set_end(void);
extern void hmi_samples_begin(void);
extern void hmi_samples_end(void);
extern void hmi_timer_begin(void);
extern void hmi_timer_end(void);
extern void hmi_voice_params_begin(void);
extern void hmi_voice_params_end(void);
extern void hmi_detect_begin(void);
extern void hmi_detect_end(void);
extern void hmi_timer_asm_begin(void);
extern void hmi_timer_asm_end(void);
extern void hmi_dpmi_asm_begin(void);
extern void hmi_dpmi_asm_end(void);

extern unsigned char hmi_system_data_2dd10[];
extern unsigned char hmi_system_data_2ec28[];
extern char hmi_system_path[0xff];
extern unsigned hmi_system_initialized;
extern hmi_dma_record hmi_system_dma_record;
extern int __cdecl hmi_dpmi_free_dos_wide(unsigned);
extern void __cdecl hmi_driver_op5(void __far *, void __far *);
extern void __cdecl hmi_driver_op1(void __far *, void __far *);
extern void __cdecl hmi_windows_vds_unlock(unsigned);
extern int __cdecl hmi_dpmi_lock(unsigned, unsigned);
extern int __cdecl hmi_dpmi_unlock(unsigned, unsigned);
extern void sub_797F5(char __far *, const char __far *);
extern unsigned hmi_windows_query(void);
extern int __cdecl hmi_dpmi_alloc_dos_extra(unsigned, unsigned *, unsigned *);
extern int __cdecl hmi_dpmi_alloc_dos(unsigned, unsigned *, unsigned *);
extern unsigned __cdecl hmi_vds_lock_real_interrupt(unsigned);
extern void sub_79824(void __far *, void __far *, unsigned);
extern void __cdecl hmi_dpmi_free_dos(unsigned, unsigned);
extern unsigned __cdecl hmi_dpmi_map_physical_selector(unsigned);
extern unsigned __cdecl hmi_dpmi_free_selector(unsigned);
extern void __cdecl hmi_vds_unlock_real_interrupt(unsigned);
extern void sub_793E0(unsigned);
extern void sub_79692(unsigned);
extern void hmi_free_dma(unsigned, unsigned, unsigned);
extern unsigned sub_791BD(unsigned, unsigned, void __far *, void __far *,
                          unsigned *, unsigned *, unsigned *);
extern unsigned hmi_driver_callback_update(unsigned, unsigned, void __far *);
extern unsigned sub_794B8(unsigned, void __far *, void __far *,
                          unsigned *, unsigned *, unsigned *);
extern void __cdecl hmi_driver_op11_op12_transfer(void __far *, void __far *, void __far *, void __far *);
extern void __cdecl hmi_driver_op10_callbacks(void __far *, void __far *, void __far *,
                              unsigned, unsigned, unsigned, unsigned);
extern void __cdecl hmi_build_voice_slots(unsigned, void __far *);
extern void __cdecl hmi_windows_vds_lock(unsigned);
extern void __cdecl hmi_driver_op0_parameters(void __far *, void __far *,
                              unsigned, unsigned, unsigned, unsigned, unsigned, unsigned);
extern void __cdecl hmi_driver_op3_bx8(void __far *, void __far *);
extern void __cdecl hmi_driver_op2_parameter(void __far *, void __far *, unsigned);
extern void __cdecl hmi_driver_op4_parameters(void __far *, void __far *, unsigned, unsigned);
extern unsigned long __far *hmi_get_dma_area(unsigned, unsigned *, unsigned *);

#define HMI_SYSTEM_U32(offset) (*(unsigned *)(hmi_system_data_2dd10 + (offset)))
#define HMI_SYSTEM_U16(offset) (*(unsigned short *)(hmi_system_data_2dd10 + (offset)))
#define HMI_SYSTEM_U8(offset) (*(unsigned char *)(hmi_system_data_2dd10 + (offset)))
#define HMI_SYSTEM_U32_PTR(offset, index) ((unsigned *)(hmi_system_data_2dd10 + (offset)) + (index))
#define HMI_SYSTEM_FAR_SLOT(offset, index) ((hmi_far_slot *)(hmi_system_data_2dd10 + (offset)) + (index))

unsigned hmi_lock_system_regions(void)
{
    if (hmi_dpmi_lock((unsigned)hmi_errors_begin,
                   (unsigned)hmi_errors_end - (unsigned)hmi_errors_begin))
        return 5;
    if (hmi_dpmi_lock((unsigned)hmi_driver_callback_begin,
                   (unsigned)hmi_driver_callback_end - (unsigned)hmi_driver_callback_begin))
        return 5;
    if (hmi_dpmi_lock((unsigned)hmi_status_begin,
                   (unsigned)hmi_status_end - (unsigned)hmi_status_begin))
        return 5;
    if (hmi_dpmi_lock((unsigned)hmi_system_begin,
                   (unsigned)hmi_system_end - (unsigned)hmi_system_begin))
        return 5;
    if (hmi_dpmi_lock((unsigned)hmi_loader_begin,
                   (unsigned)hmi_loader_end - (unsigned)hmi_loader_begin))
        return 5;
    if (hmi_dpmi_lock((unsigned)hmi_active_begin,
                   (unsigned)hmi_active_end - (unsigned)hmi_active_begin))
        return 5;
    if (hmi_dpmi_lock((unsigned)hmi_callback_set_begin,
                   (unsigned)hmi_callback_set_end - (unsigned)hmi_callback_set_begin))
        return 5;
    if (hmi_dpmi_lock((unsigned)hmi_samples_begin,
                   (unsigned)hmi_samples_end - (unsigned)hmi_samples_begin))
        return 5;
    if (hmi_dpmi_lock((unsigned)hmi_timer_begin,
                   (unsigned)hmi_timer_end - (unsigned)hmi_timer_begin))
        return 5;
    if (hmi_dpmi_lock((unsigned)hmi_voice_params_begin,
                   (unsigned)hmi_voice_params_end - (unsigned)hmi_voice_params_begin))
        return 5;
    if (hmi_dpmi_lock((unsigned)hmi_detect_begin,
                   (unsigned)hmi_detect_end - (unsigned)hmi_detect_begin))
        return 5;
    if (hmi_dpmi_lock((unsigned)hmi_timer_asm_begin,
                   (unsigned)hmi_timer_asm_end - (unsigned)hmi_timer_asm_begin))
        return 5;
    if (hmi_dpmi_lock((unsigned)hmi_dpmi_asm_begin,
                   (unsigned)hmi_dpmi_asm_end - (unsigned)hmi_dpmi_asm_begin))
        return 5;
    if (hmi_dpmi_lock((unsigned)hmi_system_data_2dd10,
                   (unsigned)hmi_system_data_2ec28 - (unsigned)hmi_system_data_2dd10))
        return 5;
    if (hmi_dpmi_lock((unsigned)&hmi_timer_asm_data_begin,
                   (unsigned)&hmi_timer_asm_data_end - (unsigned)&hmi_timer_asm_data_begin))
        return 5;
    if (hmi_dpmi_lock((unsigned)&hmi_dpmi_asm_data_begin,
                   (unsigned)&hmi_dpmi_asm_data_end - (unsigned)&hmi_dpmi_asm_data_begin))
        return 5;
    return 0;
}

unsigned hmi_unlock_system_regions(void)
{
    if (hmi_dpmi_unlock((unsigned)hmi_errors_begin,
                   (unsigned)hmi_errors_end - (unsigned)hmi_errors_begin))
        return 5;
    if (hmi_dpmi_unlock((unsigned)hmi_driver_callback_begin,
                   (unsigned)hmi_driver_callback_end - (unsigned)hmi_driver_callback_begin))
        return 5;
    if (hmi_dpmi_unlock((unsigned)hmi_status_begin,
                   (unsigned)hmi_status_end - (unsigned)hmi_status_begin))
        return 5;
    if (hmi_dpmi_unlock((unsigned)hmi_system_begin,
                   (unsigned)hmi_system_end - (unsigned)hmi_system_begin))
        return 5;
    if (hmi_dpmi_unlock((unsigned)hmi_loader_begin,
                   (unsigned)hmi_loader_end - (unsigned)hmi_loader_begin))
        return 5;
    if (hmi_dpmi_unlock((unsigned)hmi_active_begin,
                   (unsigned)hmi_active_end - (unsigned)hmi_active_begin))
        return 5;
    if (hmi_dpmi_unlock((unsigned)hmi_callback_set_begin,
                   (unsigned)hmi_callback_set_end - (unsigned)hmi_callback_set_begin))
        return 5;
    if (hmi_dpmi_unlock((unsigned)hmi_samples_begin,
                   (unsigned)hmi_samples_end - (unsigned)hmi_samples_begin))
        return 5;
    if (hmi_dpmi_unlock((unsigned)hmi_timer_begin,
                   (unsigned)hmi_timer_end - (unsigned)hmi_timer_begin))
        return 5;
    if (hmi_dpmi_unlock((unsigned)hmi_voice_params_begin,
                   (unsigned)hmi_voice_params_end - (unsigned)hmi_voice_params_begin))
        return 5;
    if (hmi_dpmi_unlock((unsigned)hmi_detect_begin,
                   (unsigned)hmi_detect_end - (unsigned)hmi_detect_begin))
        return 5;
    if (hmi_dpmi_unlock((unsigned)hmi_timer_asm_begin,
                   (unsigned)hmi_timer_asm_end - (unsigned)hmi_timer_asm_begin))
        return 5;
    if (hmi_dpmi_unlock((unsigned)hmi_dpmi_asm_begin,
                   (unsigned)hmi_dpmi_asm_end - (unsigned)hmi_dpmi_asm_begin))
        return 5;
    if (hmi_dpmi_unlock((unsigned)hmi_system_data_2dd10,
                   (unsigned)hmi_system_data_2ec28 - (unsigned)hmi_system_data_2dd10))
        return 5;
    if (hmi_dpmi_unlock((unsigned)&hmi_timer_asm_data_begin,
                   (unsigned)&hmi_timer_asm_data_end - (unsigned)&hmi_timer_asm_data_begin))
        return 5;
    if (hmi_dpmi_unlock((unsigned)&hmi_dpmi_asm_data_begin,
                   (unsigned)&hmi_dpmi_asm_data_end - (unsigned)&hmi_dpmi_asm_data_begin))
        return 5;
    return 0;
}

unsigned sub_735C4(const char __far *path, unsigned unused)
{
    unsigned result;

    result = hmi_lock_system_regions();
    if (result)
        return result;
    hmi_system_initialized = 1;
    if (path)
        sub_797F5((char __far *)hmi_system_path, path);
    else
        hmi_system_path[0] = 0;
    return 0;
}

unsigned sub_7363D(void)
{
    unsigned result;

    result = hmi_unlock_system_regions();
    if (result)
        return 0;
    hmi_system_initialized = 0;
    return 0;
}

unsigned sub_7367D(unsigned driver_id,
                                       hmi_driver_record __far *driver_record,
                                       hmi_driver_init_state __far *initial_state,
                                       unsigned __far *slot_out)
{
    unsigned value_00;
    unsigned value_0c;
    unsigned value_10_a;
    unsigned value_10_b;
    unsigned value_14;
    unsigned value_18;
    unsigned index;
    unsigned slot;
    unsigned status;
    unsigned linear;
    unsigned cached_driver_value;
    unsigned record_00;
    unsigned record_04;
    unsigned record_0c;
    unsigned fill_index;
    unsigned short value_38;
    unsigned short value_40;
    hmi_far_address field_04_08;
    hmi_far_address field_24_28;
    hmi_far_address field_2c_30;
    hmi_far_address field_34_38;
    hmi_far_address field_3c_40;
    unsigned long __far *dma_area;

    value_00 = initial_state->value_00;
    value_10_a = initial_state->value_10;
    field_04_08.selector = initial_state->value_08;
    field_04_08.offset = initial_state->value_04;
    value_0c = initial_state->value_0c;
    value_10_b = initial_state->value_10;
    value_14 = initial_state->value_14;
    value_18 = initial_state->value_18;

    slot = 6;
    index = 0;
    while (index < 5) {
        if (HMI_SYSTEM_U32(0x5b2 + index * 4) == 0) {
            slot = index;
            break;
        }
        ++index;
    }
    if (slot == 6)
        return 11;

    HMI_SYSTEM_U32(0x5b2 + slot * 4) = driver_id;
    if (initial_state->value_24 != 0 || initial_state->value_28 != 0) {
        field_24_28.selector = initial_state->value_28;
        field_24_28.offset = initial_state->value_24;
        field_2c_30.selector = initial_state->value_30;
        field_2c_30.offset = initial_state->value_2c;
    } else {
        status = sub_791BD(driver_id, slot,
                           (void __far *)&field_2c_30,
                           (void __far *)&field_24_28,
                           HMI_SYSTEM_U32_PTR(0xcc, slot),
                           HMI_SYSTEM_U32_PTR(0x68, slot),
                           HMI_SYSTEM_U32_PTR(0x04, slot));
        if (status)
            return status;
        initial_state->value_28 = field_24_28.selector;
        initial_state->value_24 = field_24_28.offset;
        initial_state->value_30 = field_2c_30.selector;
        initial_state->value_2c = field_2c_30.offset;
    }

    *(unsigned short *)HMI_SYSTEM_FAR_SLOT(0x2c, slot)->selector_bytes = field_24_28.selector;
    *(unsigned *)HMI_SYSTEM_FAR_SLOT(0x2c, slot)->offset_bytes = field_24_28.offset;
    *(unsigned short *)HMI_SYSTEM_FAR_SLOT(0x4a, slot)->selector_bytes = field_2c_30.selector;
    *(unsigned *)HMI_SYSTEM_FAR_SLOT(0x4a, slot)->offset_bytes = field_2c_30.offset;

    status = hmi_driver_callback_update(driver_id, slot,
                       (void __far *)(hmi_system_data_2dd10 + 0x5ca + slot * 0x6c));
    if (status)
        return status;

    if (HMI_SYSTEM_U32(0x5b2 + slot * 4) < 0xe106) {
        HMI_SYSTEM_U32(0x344 + slot * 4) = value_00;
        if (value_0c != 0) {
            HMI_SYSTEM_U32(0xe70 + slot * 4) = 1;
            dma_area = hmi_get_dma_area(value_00, &linear,
                                        HMI_SYSTEM_U32_PTR(0x18a, slot));
            if (dma_area == 0) {
                sub_793E0(slot);
                HMI_SYSTEM_U32(0x5b2 + slot * 4) = 0;
                return 5;
            }
            field_04_08.offset = FP_OFF(dma_area);
            field_04_08.selector = FP_SEG(dma_area);
            *(unsigned short *)HMI_SYSTEM_FAR_SLOT(0x158, slot)->selector_bytes = field_04_08.selector;
            *(unsigned *)HMI_SYSTEM_FAR_SLOT(0x158, slot)->offset_bytes = field_04_08.offset;
            if (HMI_SYSTEM_U32(0x5ee + slot * 0x6c) == 8 &&
                (HMI_SYSTEM_U8(0x607 + slot * 0x6c) & 0x80) != 0) {
                for (fill_index = 0; fill_index < value_00; ++fill_index)
                    ((unsigned char __far *)dma_area)[fill_index] = 0x80;
            } else {
                for (fill_index = 0; fill_index < value_00; ++fill_index)
                    ((unsigned char __far *)dma_area)[fill_index] = 0;
            }
        } else {
            linear = initial_state->value_48;
            *(unsigned short *)HMI_SYSTEM_FAR_SLOT(0x158, slot)->selector_bytes = field_04_08.selector;
            *(unsigned *)HMI_SYSTEM_FAR_SLOT(0x158, slot)->offset_bytes = field_04_08.offset;
        }
        initial_state->value_48 = linear;

        if (initial_state->value_34 == 0 && initial_state->value_38 == 0) {
            if (initial_state->value_44 != 0) {
                status = sub_794B8(initial_state->value_44,
                                   (void __far *)&field_3c_40,
                                   (void __far *)&field_34_38,
                                   HMI_SYSTEM_U32_PTR(0xe0, slot),
                                   HMI_SYSTEM_U32_PTR(0x7c, slot),
                                   HMI_SYSTEM_U32_PTR(0x18, slot));
            } else {
                status = sub_794B8(HMI_SYSTEM_U32(0x632 + slot * 0x6c),
                                   (void __far *)&field_3c_40,
                                   (void __far *)&field_34_38,
                                   HMI_SYSTEM_U32_PTR(0xe0, slot),
                                   HMI_SYSTEM_U32_PTR(0x7c, slot),
                                   HMI_SYSTEM_U32_PTR(0x18, slot));
            }
            if (status)
                return status;
        } else {
            field_34_38.selector = initial_state->value_38;
            field_34_38.offset = initial_state->value_34;
            field_3c_40.selector = initial_state->value_40;
            field_3c_40.offset = initial_state->value_3c;
        }

        if ((HMI_SYSTEM_U8(0x607 + slot * 0x6c) & 2) != 0)
            hmi_driver_op11_op12_transfer(MK_FP(field_3c_40.selector, field_3c_40.offset),
                      MK_FP(field_34_38.selector, field_34_38.offset),
                      MK_FP(field_2c_30.selector, field_2c_30.offset),
                      MK_FP(field_04_08.selector, field_04_08.offset));

        initial_state->value_38 = field_34_38.selector;
        initial_state->value_34 = field_34_38.offset;
        initial_state->value_40 = field_3c_40.selector;
        initial_state->value_3c = field_3c_40.offset;
        *(unsigned short *)HMI_SYSTEM_FAR_SLOT(0x90, slot)->selector_bytes = field_34_38.selector;
        *(unsigned *)HMI_SYSTEM_FAR_SLOT(0x90, slot)->offset_bytes = field_34_38.offset;
        *(unsigned short *)HMI_SYSTEM_FAR_SLOT(0xae, slot)->selector_bytes = field_3c_40.selector;
        *(unsigned *)HMI_SYSTEM_FAR_SLOT(0xae, slot)->offset_bytes = field_3c_40.offset;
        HMI_SYSTEM_U32(0x108 + slot * 4) = 1;

        cached_driver_value = HMI_SYSTEM_U32(0x852 + driver_record->value_08 * 4);
        hmi_driver_op10_callbacks(MK_FP(field_3c_40.selector, field_3c_40.offset),
                  MK_FP(field_34_38.selector, field_34_38.offset),
                  MK_FP(field_04_08.selector, field_04_08.offset),
                  value_00, cached_driver_value, slot,
                  (unsigned)(hmi_system_data_2dd10 + 0xe98));

        *(unsigned short *)HMI_SYSTEM_FAR_SLOT(0x1b2, slot)->selector_bytes = HMI_SYSTEM_U16(0xe9c);
        *(unsigned *)HMI_SYSTEM_FAR_SLOT(0x1b2, slot)->offset_bytes = HMI_SYSTEM_U32(0xe98);
        *(unsigned short *)HMI_SYSTEM_FAR_SLOT(0x590, slot)->selector_bytes = HMI_SYSTEM_U16(0xea4);
        *(unsigned *)HMI_SYSTEM_FAR_SLOT(0x590, slot)->offset_bytes = HMI_SYSTEM_U32(0xea0);
        hmi_build_voice_slots((unsigned)&hmi_voice_slots[slot][0],
                  MK_FP(HMI_SYSTEM_U16(0xeac), HMI_SYSTEM_U32(0xea8)));
    } else {
        *(unsigned short *)HMI_SYSTEM_FAR_SLOT(0x1b2, slot)->selector_bytes = 0;
        *(unsigned *)HMI_SYSTEM_FAR_SLOT(0x1b2, slot)->offset_bytes = 0;
    }

    field_2c_30.offset = *(unsigned *)HMI_SYSTEM_FAR_SLOT(0x1b2, slot)->offset_bytes;
    field_2c_30.selector = *(unsigned short *)HMI_SYSTEM_FAR_SLOT(0x1b2, slot)->selector_bytes;
    initial_state->value_1c = field_2c_30.offset;
    initial_state->value_20 = field_2c_30.selector;
    hmi_windows_vds_lock(driver_record->value_08);
    HMI_SYSTEM_U32(0x176 + slot * 4) = initial_state->value_48;

    record_00 = driver_record->value_00;
    record_04 = driver_record->value_04;
    cached_driver_value = driver_record->value_08;
    record_0c = driver_record->value_0c;
    HMI_SYSTEM_U8(0x872 + slot) = (unsigned char)cached_driver_value;
    hmi_driver_op0_parameters(MK_FP(field_2c_30.selector, field_2c_30.offset),
              MK_FP(field_04_08.selector, field_04_08.offset),
              record_00, record_04, cached_driver_value, value_14,
              value_18, record_0c);
    hmi_driver_op3_bx8(MK_FP(field_2c_30.selector, field_2c_30.offset),
              MK_FP(field_04_08.selector, field_04_08.offset));
    hmi_driver_op2_parameter(MK_FP(field_2c_30.selector, field_2c_30.offset),
              MK_FP(field_04_08.selector, field_04_08.offset), value_10_b);
    linear = HMI_SYSTEM_U32(0x176 + slot * 4);
    hmi_driver_op4_parameters(MK_FP(field_2c_30.selector, field_2c_30.offset),
              MK_FP(field_04_08.selector, field_04_08.offset), linear, value_00);
    HMI_SYSTEM_U32(0xf4 + slot * 4) = 1;
    HMI_SYSTEM_U32(0x11c + slot * 4) = 1;

    HMI_SYSTEM_U32(0x877 + slot * 0x4c) = initial_state->value_00;
    HMI_SYSTEM_U16(0x87f + slot * 0x4c) = initial_state->value_08;
    HMI_SYSTEM_U32(0x87b + slot * 0x4c) = initial_state->value_04;
    HMI_SYSTEM_U32(0x883 + slot * 0x4c) = initial_state->value_0c;
    HMI_SYSTEM_U32(0x887 + slot * 0x4c) = value_10_a;
    HMI_SYSTEM_U32(0x88b + slot * 0x4c) = driver_id;
    HMI_SYSTEM_U32(0x88f + slot * 0x4c) = initial_state->value_18;
    HMI_SYSTEM_U16(0x897 + slot * 0x4c) = initial_state->value_20;
    HMI_SYSTEM_U32(0x893 + slot * 0x4c) = initial_state->value_1c;
    HMI_SYSTEM_U16(0x89f + slot * 0x4c) = initial_state->value_28;
    HMI_SYSTEM_U32(0x89b + slot * 0x4c) = initial_state->value_24;
    HMI_SYSTEM_U16(0x8af + slot * 0x4c) = initial_state->value_38;
    HMI_SYSTEM_U32(0x8ab + slot * 0x4c) = initial_state->value_34;
    HMI_SYSTEM_U16(0x8a7 + slot * 0x4c) = initial_state->value_30;
    HMI_SYSTEM_U32(0x8a3 + slot * 0x4c) = initial_state->value_2c;
    HMI_SYSTEM_U32(0x8b3 + slot * 0x4c) = initial_state->value_3c;
    HMI_SYSTEM_U16(0x8b7 + slot * 0x4c) = initial_state->value_40;
    HMI_SYSTEM_U32(0x8bb + slot * 0x4c) = initial_state->value_44;
    *slot_out = slot;
    return 0;
}

unsigned sub_73F3D(unsigned driver, unsigned enabled, unsigned extra_cleanup)
{
    hmi_far_address previous_dma;
    hmi_far_address first_address;
    hmi_far_address second_address;

    if (HMI_SYSTEM_U32(0xf4 + driver * 4) == 0)
        return 1;

    HMI_SYSTEM_U32(0x11c + driver * 4) = 0;
    if (HMI_SYSTEM_U32(0x19e + driver * 4) < 0xe106) {
        if (HMI_SYSTEM_U32(0xe70 + driver * 4) != 0 && enabled != 0) {
            previous_dma.selector = HMI_SYSTEM_U16(0x15c + driver * 6);
            previous_dma.offset = HMI_SYSTEM_U32(0x158 + driver * 6);
            if ((hmi_windows_query() & 0xff) == 3) {
                hmi_free_dma(previous_dma.selector,
                             HMI_SYSTEM_U32(0x18a + driver * 4),
                             HMI_SYSTEM_U32(0x176 + driver * 4));
            } else {
                hmi_dpmi_free_dos_wide(previous_dma.selector);
            }
            HMI_SYSTEM_U32(0xe70 + driver * 4) = 0;
        }
    }

    first_address.offset = HMI_SYSTEM_U32(0x2c + driver * 6);
    first_address.selector = HMI_SYSTEM_U16(0x30 + driver * 6);
    second_address.offset = HMI_SYSTEM_U32(0x4a + driver * 6);
    second_address.selector = HMI_SYSTEM_U16(0x4e + driver * 6);
    hmi_driver_op5(MK_FP(first_address.selector, first_address.offset),
              MK_FP(second_address.selector, second_address.offset));
    hmi_driver_op1(MK_FP(first_address.selector, first_address.offset),
              MK_FP(second_address.selector, second_address.offset));
    hmi_windows_vds_unlock(HMI_SYSTEM_U8(0x872 + driver));
    if (extra_cleanup != 0) {
        sub_793E0(driver);
        sub_79692(driver);
    }

    HMI_SYSTEM_U16(0x30 + driver * 6) = 0;
    HMI_SYSTEM_U32(0x2c + driver * 6) = 0;
    HMI_SYSTEM_U16(0x4e + driver * 6) = 0;
    HMI_SYSTEM_U32(0x4a + driver * 6) = 0;
    HMI_SYSTEM_U16(0x1b6 + driver * 6) = 0;
    HMI_SYSTEM_U32(0x1b2 + driver * 6) = 0;
    HMI_SYSTEM_U16(0x94 + driver * 6) = 0;
    HMI_SYSTEM_U32(0x90 + driver * 6) = 0;
    HMI_SYSTEM_U16(0xb2 + driver * 6) = 0;
    HMI_SYSTEM_U32(0xae + driver * 6) = 0;
    HMI_SYSTEM_U32(0x5b2 + driver * 4) = 0;
    HMI_SYSTEM_U32(0xf4 + driver * 4) = 0;
    return 0;
}

unsigned hmi_allocate_dma(hmi_far_address *address, unsigned *selector_out,
                          unsigned *linear_out);

unsigned long __far *hmi_get_dma_area(unsigned size, unsigned *linear,
                                      unsigned *selector_out)
{
    unsigned base;
    unsigned selector;
    unsigned output_selector;
    unsigned output_linear;
    hmi_far_address address;

    selector = 0;
    if ((hmi_windows_query() & 0xff) == 3) {
        if (hmi_allocate_dma(&address, &output_selector, &output_linear))
            return 0;
        *linear = output_linear;
        *selector_out = output_selector;
        return MK_FP(address.selector, address.offset);
    }
    hmi_dpmi_alloc_dos_extra((size >> 4) + 4, &base, &selector);
    *linear = base;
    while ((base & 0xffff) > (0xffff - size)) {
        hmi_dpmi_alloc_dos_extra((size >> 4) + 4, &base, &selector);
        *linear = base;
    }
    return MK_FP((unsigned short)selector, 0);
}

unsigned hmi_allocate_dma(hmi_far_address *address, unsigned *selector_out,
                         unsigned *linear_out)
{
    unsigned base;
    unsigned selector;
    unsigned status;
    unsigned temporary;
    unsigned result;
    unsigned __far *memory;

    status = 0;
    temporary = 0;
    address->selector = 0;
    address->offset = 0;
    *selector_out = 0;
    *linear_out = 0;
    if (hmi_dpmi_alloc_dos(0x20, &selector, &base))
        return 0;
    memory = (unsigned __far *)MK_FP((unsigned short)selector, 0);
    *memory = 0x1000;
    status = hmi_vds_lock_real_interrupt(base);
    sub_79824((void __far *)&hmi_system_dma_record, memory, 0x10);
    hmi_dpmi_free_dos(base, selector);
    if (status)
        return 5;
    hmi_system_dma_record.selector.low &= 0xf000;
    temporary = hmi_system_dma_record.selector.value;
    result = hmi_dpmi_map_physical_selector(temporary);
    address->offset = 0;
    address->selector = (unsigned short)result;
    *linear_out = hmi_system_dma_record.selector.value;
    *selector_out = result;
    return 0;
}

void hmi_free_dma(unsigned driver, unsigned unused, unsigned unused2)
{
    unsigned base;
    unsigned selector;
    unsigned __far *memory;

    hmi_dpmi_free_selector(driver);
    hmi_dpmi_alloc_dos(0x20, &base, &selector);
    memory = (unsigned __far *)MK_FP((unsigned short)base, 0);
    sub_79824(memory, (void __far *)&hmi_system_dma_record, 0x10);
    hmi_vds_unlock_real_interrupt(selector);
    hmi_dpmi_free_dos(base, selector);
}

void hmi_system_end(void) {}
