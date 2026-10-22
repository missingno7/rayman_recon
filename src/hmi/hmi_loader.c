/* INVENTED names; DOS-derived HMIDRV loader group 1:791a8..79769. */
#include "hmi_local.h"

extern hmi_u32 __cdecl hmi_dpmi_unlock(hmi_u32 first, hmi_u32 second);
extern void __cdecl hmi_dpmi_free_linear_selector(void __far *memory,
                              hmi_u32 resource);
extern void sub_7875F(hmi_u8 *name, hmi_u8 *path);
extern void sub_7877E(hmi_u8 *name, const char *filename);
extern hmi_u32 __cdecl sub_713E7(hmi_u8 *name, hmi_u32 size);
extern void sub_7560B(hmi_u32 handle, void *buffer, hmi_u32 size);
extern hmi_far_address __far * __cdecl hmi_dpmi_alloc_linear_selector(
    hmi_u32 current, hmi_u32 *context, hmi_u32 *value);
extern hmi_far_address __far * __cdecl hmi_dpmi_alias_code_selector(
    hmi_u32 offset, hmi_u16 selector);
extern hmi_u32 __cdecl hmi_dpmi_lock(hmi_u32 value, hmi_u32 current);
extern void sub_7165A(hmi_u32 handle, void __far *memory, hmi_u32 count,
                     hmi_u32 *value);
extern void sub_716AB(hmi_u32 handle, hmi_u32 current, hmi_u32 mode);
extern void close(hmi_u32 handle);

void hmi_loader_begin(void) {}

hmi_u32 sub_791BD(hmi_u32 requested, hmi_u32 slot,
                        hmi_far_address __far *record,
                        hmi_far_address __far *scratch,
                        hmi_u32 *context, hmi_u8 *slot_out,
                        hmi_u32 *value_out)
{
    hmi_u32 file_handle;
    hmi_u32 current;
    hmi_u32 size_a;
    hmi_u32 size_b;
    hmi_u32 found;
    hmi_u32 i;
    hmi_far_value second;
    hmi_far_value first;
    hmi_u32 value;
    hmi_u32 local_value;
    hmi_u32 result;

    size_a = 0x2c;
    size_b = 0x30;
    found = 0;
    i = 0;
    if (slot > 5) {
        result = 10;
        goto done;
    }
    if (hmi_driver_tables.loaded[slot] != 0) {
        result = 9;
        goto done;
    }
    if (requested < 0xe000 || requested > 0xe200) {
        result = 6;
        goto done;
    }

    sub_7875F(hmi_detect_path, hmi_system_path);
    sub_7877E(hmi_detect_path, "hmidrv.386");
    file_handle = sub_713E7(hmi_detect_path, 0x200);
    if (file_handle == (hmi_u32)-1) {
        result = 15;
        goto done;
    }
    sub_7560B(file_handle, hmi_loader_header_a.unknown_00, size_a);
    while (i <= hmi_loader_header_a.record_count && found == 0) {
        sub_7560B(file_handle, hmi_loader_header_b.unknown_00, size_b);
        current = hmi_loader_header_b.value_24;
        if (hmi_loader_header_b.value_28 == requested &&
            (hmi_loader_header_b.flags_2d & 0x80)) {
            found = 1;
            first.pointer = hmi_dpmi_alloc_linear_selector(current, context,
                                                            &value);
            second.pointer = hmi_dpmi_alias_code_selector(
                first.parts.offset, first.parts.selector);
            if (hmi_dpmi_lock(value, current)) {
                result = 5;
                goto done;
            }
            sub_7165A(file_handle, first.pointer, current, &local_value);
            record->selector = second.parts.selector;
            record->offset = second.parts.offset;
            scratch->selector = first.parts.selector;
            scratch->offset = first.parts.offset;
            *slot_out = (hmi_u8)current;
            *value_out = value;
        } else {
            sub_716AB(file_handle, current, 1);
        }
        i++;
    }
    close(file_handle);
    hmi_driver_ids[slot] = hmi_loader_header_b.value_28;
    hmi_driver_tables.loaded[slot] = 1;
    result = 0;
done:
    return result;
}

hmi_u32 sub_793E0(hmi_u32 slot)
{
    if (slot > 5) {
        return 10;
    }
    if (hmi_driver_tables.loaded[slot] == 0) {
        return 10;
    }
    hmi_driver_tables.loaded[slot] = 0;
    if (hmi_dpmi_unlock(hmi_driver_tables.slot_a[slot],
                        hmi_driver_tables.slot_c[slot])) {
        return 5;
    }
    hmi_dpmi_free_linear_selector(hmi_driver_tables.far_a[slot].pointer,
                                  hmi_driver_tables.slot_e[slot]);
    hmi_dpmi_free_linear_selector(hmi_driver_tables.far_b[slot].pointer,
                                  hmi_driver_tables.slot_e[slot]);
    return 0;
}

hmi_u32 sub_794B8(hmi_u32 requested,
                            hmi_far_address __far *record,
                            hmi_far_address __far *scratch,
                            hmi_u32 *context, hmi_u8 *slot_out,
                            hmi_u32 *value_out)
{
    hmi_u32 file_handle;
    hmi_u32 current;
    hmi_u32 size_a;
    hmi_u32 size_b;
    hmi_u32 found;
    hmi_u32 i;
    hmi_far_value second;
    hmi_far_value first;
    hmi_u32 value;
    hmi_u32 local_value;
    hmi_u32 result;

    size_a = 0x2c;
    size_b = 0x30;
    found = 0;
    i = 0;
    if (requested < 0x1000 || requested > 0x1023) {
        result = 6;
        goto done;
    }

    sub_7875F(hmi_detect_path, hmi_system_path);
    sub_7877E(hmi_detect_path, "hmidrv.386");
    file_handle = sub_713E7(hmi_detect_path, 0x200);
    if (file_handle == (hmi_u32)-1) {
        result = 15;
        goto done;
    }
    sub_7560B(file_handle, hmi_loader_header_a.unknown_00, size_a);
    while (i <= hmi_loader_header_a.record_count && found == 0) {
        sub_7560B(file_handle, hmi_loader_header_b.unknown_00, size_b);
        current = hmi_loader_header_b.value_24;
        if (hmi_loader_header_b.value_28 == requested &&
            (hmi_loader_header_b.flags_2d & 0x40)) {
            found = 1;
            current += 0x4000;
            first.pointer = hmi_dpmi_alloc_linear_selector(current, context,
                                                            &value);
            second.pointer = hmi_dpmi_alias_code_selector(
                first.parts.offset, first.parts.selector);
            if (hmi_dpmi_lock(value, current)) {
                result = 5;
                goto done;
            }
            sub_7165A(file_handle, first.pointer, current, &local_value);
            record->selector = second.parts.selector;
            record->offset = second.parts.offset;
            scratch->selector = first.parts.selector;
            scratch->offset = first.parts.offset;
            *slot_out = (hmi_u8)current;
            *value_out = value;
        } else {
            sub_716AB(file_handle, current, 1);
        }
        i++;
    }
    close(file_handle);
    result = 0;
done:
    return result;
}

hmi_u32 sub_79692(hmi_u32 slot)
{
    if (hmi_dpmi_unlock(hmi_driver_tables.slot_b[slot],
                        hmi_driver_tables.slot_d[slot])) {
        return 5;
    }
    hmi_dpmi_free_linear_selector(hmi_driver_tables.far_c[slot].pointer,
                                  hmi_driver_tables.slot_f[slot]);
    hmi_dpmi_free_linear_selector(hmi_driver_tables.far_d[slot].pointer,
                                  hmi_driver_tables.slot_f[slot]);
    return 0;
}

hmi_u32 hmi_loader_empty_result(hmi_u32 first,
                                hmi_far_address __far *record,
                                hmi_far_address __far *scratch)
{
    return 0;
}

void hmi_loader_end(void) {}
