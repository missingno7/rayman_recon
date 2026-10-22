/* INVENTED names; DOS-derived HMIDET load/detection group 1:7233a..73023. */
#include "hmi_local.h"
#include <i86.h>

extern hmi_s32 close(hmi_s32 handle);
extern hmi_s32 __cdecl sub_713E7(const char *path, hmi_s32 mode);
extern void sub_7165A(hmi_s32 handle, void __far *memory,
    hmi_u32 count, hmi_s32 *value);
extern hmi_s32 sub_716AB(hmi_s32 handle, hmi_s32 position,
    hmi_s32 origin);
extern void sub_7560B(hmi_s32 handle, void *buffer, hmi_u32 size);
extern void sub_7875F(char *destination, const char *source);
extern void sub_7877E(char *destination, const char *source);
extern hmi_s32 __cdecl hmi_dpmi_lock(hmi_u32 address, hmi_u32 size);
extern hmi_s32 __cdecl hmi_dpmi_unlock(hmi_u32 address, hmi_u32 size);
extern void __far *__cdecl hmi_dpmi_alloc_linear_selector(hmi_u32 size, hmi_u32 *address,
    hmi_u32 *selector);
extern hmi_s32 __cdecl hmi_dpmi_free_linear_selector(void __far *memory, hmi_u32 size);
extern void __far *__cdecl hmi_dpmi_alias_code_selector(void __far *memory);
extern void __cdecl hmi_driver_op2_copy_selectors(void __far *memory_b, void __far *memory_a,
    void __far *third);
extern hmi_s32 __cdecl hmi_driver_op0(void __far *memory_b,
    void __far *memory_a);
extern void __cdecl hmi_driver_op1_detect_results(void __far *memory_b,
    void __far *memory_a);
extern hmi_s32 __cdecl hmi_driver_op3_parameters(void __far *memory_b, hmi_s32 value_00,
    hmi_s32 value_04, hmi_s32 value_08, void __far *memory_a);
extern void __cdecl hmi_windows_vds_lock(hmi_s32 value);
extern void __cdecl hmi_windows_vds_unlock(hmi_s32 value);
extern void __cdecl hmi_driver_op4_tables(void __far *memory_b,
    void __far *memory_a);
extern hmi_s32 __cdecl hmi_driver_copy_record(hmi_s32 index);
extern void __cdecl hmi_driver_set_table_callback(hmi_s32 index, hmi_s32 value);
extern hmi_s32 __cdecl sub_79038(void);

void hmi_detect_begin(void) {}

hmi_s32 sub_7234F(const char __far *prefix)
{
    hmi_u32 file_length;
    hmi_s32 size;

    if (hmi_detect.initialized != 0) {
        return 3;
    } else {
        size = 0x2c;
        if (prefix != 0) {
            sub_7875F(hmi_detect_path, (const char *)prefix);
            sub_7877E(hmi_detect_path, "hmidet.386");
        } else {
            sub_7875F(hmi_detect_path, "hmidet.386");
        }

        hmi_detect.file_handle = sub_713E7(hmi_detect_path, 0x200);
        if (hmi_detect.file_handle == -1) {
            return 0x0f;
        } else {
            sub_7560B(hmi_detect.file_handle, &hmi_detect_file_header, size);
            file_length = hmi_detect_file_header.length;
            hmi_detect.memory_a = (void __far *)hmi_dpmi_alloc_linear_selector(0x1000,
                &hmi_detect.unknown_1c, &hmi_detect.unknown_20);
            hmi_detect.memory_b = (void __far *)hmi_dpmi_alias_code_selector(hmi_detect.memory_a);
            if (hmi_dpmi_lock(hmi_detect.unknown_20, 0x1000) != 0) {
                return 5;
            } else {
                sub_716AB(hmi_detect.file_handle, 0, 0);
                hmi_detect.initialized = 1;
                return 0;
            }
        }
    }
}

hmi_s32 sub_72499(void)
{

    hmi_detect.initialized = 0;
    close(hmi_detect.file_handle);
    if (hmi_dpmi_unlock(hmi_detect.unknown_20, 0x1000) != 0) {
        return 5;
    } else {
        hmi_dpmi_free_linear_selector(hmi_detect.memory_a, hmi_detect.unknown_1c);
        hmi_dpmi_free_linear_selector(hmi_detect.memory_b, hmi_detect.unknown_1c);
        return 0;
    }
}

hmi_s32 sub_72524(hmi_u32 port, void __far *source,
    hmi_s32 __far *output)
{
    hmi_s32 record;
    hmi_s32 found;
    hmi_s32 status;
    hmi_s32 size;
    hmi_s32 position;
    hmi_s32 value;
    hmi_s32 index;
    hmi_s32 error;

    size = 0x30;
    index = 0;
    error = 0;
    if (source == 0) {
        return 2;
    }
    if (port < 0xe000 || port > 0xe200) {
        return 6;
    }

    found = 0;
    status = 0;
    hmi_detect.record_index = 0;
    sub_716AB(hmi_detect.file_handle, 0, 0);
    sub_7560B(hmi_detect.file_handle, &hmi_detect_file_header, 0x2c);
    while (hmi_detect.record_index <= hmi_detect_file_header.record_count
            && found == 0) {
        position = sub_716AB(hmi_detect.file_handle, 0, 1);
        sub_7560B(hmi_detect.file_handle, &hmi_detect_info, size);
        record = hmi_detect_info.value_24;
        if (hmi_detect_info.value_28 == port
                && (hmi_detect_info.flags_2d & 0x80) != 0) {
            found = 1;
            sub_7165A(hmi_detect.file_handle, hmi_detect.memory_a, record,
                &value);
        } else {
            sub_716AB(hmi_detect.file_handle, record, 1);
        }
        hmi_detect.record_index++;
    }

    if (found == 1) {
        hmi_driver_op2_copy_selectors(hmi_detect.memory_b, hmi_detect.memory_a,
            (void __far *)&hmi_detect.data_selector);
        if ((hmi_detect.flags_67 & 1) != 0) {
            hmi_driver_op4_tables(hmi_detect.memory_b, hmi_detect.memory_a);
            index = 0;
            while (hmi_driver_copy_record(index) != 0) {
                hmi_driver_copy_record(index);
                error = sub_79038();
                hmi_driver_set_table_callback(index, error);
                index++;
            }
        }
        status = hmi_driver_op0(hmi_detect.memory_b, hmi_detect.memory_a);
        if (status != 0) {
            hmi_driver_op2_copy_selectors(hmi_detect.memory_b, hmi_detect.memory_a, source);
            hmi_detect.table = source;
            *output = status;
            hmi_detect.unknown_14 = position;
            return 0;
        } else {
            return 8;
        }
    } else {
        return 7;
    }

}

hmi_s32 sub_727A7(hmi_u32 port, void __far *source)
{
    hmi_s32 value;
    hmi_s32 found;
    hmi_s32 status;
    hmi_s32 size;
    hmi_s32 position;
    hmi_s32 read_result;

    size = 0x30;
    if (source == 0) {
        return 2;
    }
    if (port < 0xe000 || port > 0xe200) {
        return 6;
    }

    found = 0;
    status = 0;
    hmi_detect.record_index = 0;
    sub_716AB(hmi_detect.file_handle, 0, 0);
    sub_7560B(hmi_detect.file_handle, &hmi_detect_file_header, 0x2c);
    while (hmi_detect.record_index <= hmi_detect_file_header.record_count
            && found == 0) {
        position = sub_716AB(hmi_detect.file_handle, 0, 1);
        sub_7560B(hmi_detect.file_handle, &hmi_detect_info, size);
        value = hmi_detect_info.value_24;
        if (hmi_detect_info.value_28 == port
                && (hmi_detect_info.flags_2d & 0x80) != 0) {
            found = 1;
            sub_7165A(hmi_detect.file_handle, hmi_detect.memory_a, value,
                &read_result);
        } else {
            sub_716AB(hmi_detect.file_handle, value, 1);
        }
        hmi_detect.record_index++;
    }

    if (found == 1) {
        hmi_driver_op2_copy_selectors(hmi_detect.memory_b, hmi_detect.memory_a, source);
        hmi_detect.table = source;
        hmi_detect.unknown_14 = position;
        return 0;
    } else {
        return 7;
    }

}

hmi_s32 sub_7293D(const char __far *name, void __far *output)
{
    hmi_s32 value_24;
    hmi_s32 value_28;
    hmi_s32 found;
    hmi_s32 status;
    hmi_s32 size;
    hmi_s32 file_position;
    hmi_s32 result_value;
    hmi_s32 index;
    hmi_s32 error;

    value_28 = -1;
    size = 0x30;
    index = 0;
    error = 0;
    if (name == 0)
        return 2;
    found = 0;
    status = 0;
    hmi_detect.record_index = 0;
    sub_716AB(hmi_detect.file_handle, 0, 0);
    sub_7560B(hmi_detect.file_handle, &hmi_detect_file_header, 0x2c);
    while (hmi_detect.record_index <= hmi_detect_file_header.record_count
            && found == 0) {
        file_position = sub_716AB(hmi_detect.file_handle, 0, 1);
        sub_7560B(hmi_detect.file_handle, &hmi_detect_info, size);
        value_24 = hmi_detect_info.value_24;
        value_28 = hmi_detect_info.value_28;
        if ((hmi_detect_info.flags_2d & 0x80) != 0) {
            sub_7165A(hmi_detect.file_handle, hmi_detect.memory_a, value_24,
                &result_value);
            hmi_driver_op2_copy_selectors(hmi_detect.memory_b, hmi_detect.memory_a,
                (void __far *)&hmi_detect.data_selector);
            if ((hmi_detect.flags_67 & 1) != 0) {
                hmi_driver_op4_tables(hmi_detect.memory_b, hmi_detect.memory_a);
                index = 0;
                while (hmi_driver_copy_record(index) != 0) {
                    hmi_driver_copy_record(index);
                    error = sub_79038();
                    hmi_driver_set_table_callback(index, error);
                    index++;
                }
            }
            status = hmi_driver_op0(hmi_detect.memory_b, hmi_detect.memory_a);
            if (status != 0) {
                hmi_detect.unknown_14 = file_position;
                hmi_driver_op2_copy_selectors(hmi_detect.memory_b, hmi_detect.memory_a,
                    (void __far *)name);
                hmi_detect.table = (void __far *)name;
                *(hmi_s32 __far *)output = status;
                return 0;
            }
        } else {
            sub_716AB(hmi_detect.file_handle, value_24, 1);
        }
        hmi_detect.record_index++;
    }
    return 7;
}

hmi_s32 sub_72B8F(const char __far *name, void __far *output)
{
    hmi_s32 value_24;
    hmi_s32 value_28;
    hmi_s32 found;
    hmi_s32 status;
    hmi_s32 size;
    hmi_s32 file_position;
    hmi_s32 result_value;
    hmi_s32 index;
    hmi_s32 error;

    value_28 = -1;
    size = 0x30;
    index = 0;
    error = 0;
    if (name == 0)
        return 2;
    found = 0;
    status = 0;
    while (hmi_detect.record_index <= hmi_detect_file_header.record_count
            && found == 0) {
        file_position = sub_716AB(hmi_detect.file_handle, 0, 1);
        sub_7560B(hmi_detect.file_handle, &hmi_detect_info, size);
        value_24 = hmi_detect_info.value_24;
        value_28 = hmi_detect_info.value_28;
        if ((hmi_detect_info.flags_2d & 0x80) != 0) {
            sub_7165A(hmi_detect.file_handle, hmi_detect.memory_a, value_24,
                &result_value);
            hmi_driver_op2_copy_selectors(hmi_detect.memory_b, hmi_detect.memory_a,
                (void __far *)&hmi_detect.data_selector);
            if ((hmi_detect.flags_67 & 1) != 0) {
                hmi_driver_op4_tables(hmi_detect.memory_b, hmi_detect.memory_a);
                index = 0;
                while (hmi_driver_copy_record(index) != 0) {
                    hmi_driver_copy_record(index);
                    error = sub_79038();
                    hmi_driver_set_table_callback(index, error);
                    index++;
                }
            }
            status = hmi_driver_op0(hmi_detect.memory_b, hmi_detect.memory_a);
            if (status != 0) {
                hmi_detect.unknown_14 = file_position;
                hmi_driver_op2_copy_selectors(hmi_detect.memory_b, hmi_detect.memory_a,
                    (void __far *)name);
                hmi_detect.table = (void __far *)name;
                *(hmi_s32 __far *)output = status;
                return 0;
            }
        } else {
            sub_716AB(hmi_detect.file_handle, value_24, 1);
        }
        hmi_detect.record_index++;
    }
    return 7;
}

hmi_s32 sub_72DB5(hmi_s32 __far *output)
{
    hmi_s32 size;
    hmi_s32 index;

    /* PROVEN DOS store at 1:72dc9; purpose remains OPEN. */
    size = 0x30;
    if (output == 0)
        return 2;
    sub_716AB(hmi_detect.file_handle, hmi_detect.unknown_14, 0);
    index = 0;
    while (((hmi_detect_request __far *)hmi_detect.table)->resources[index] != -1) {
        hmi_windows_vds_lock(((hmi_detect_request __far *)hmi_detect.table)->resources[index]);
        index++;
    }
    hmi_driver_op1_detect_results(hmi_detect.memory_b, hmi_detect.memory_a);
    index = 0;
    while (((hmi_detect_request __far *)hmi_detect.table)->resources[index] != -1) {
        hmi_windows_vds_unlock(((hmi_detect_request __far *)hmi_detect.table)->resources[index]);
        index++;
    }
    output[0] = hmi_results.values[0];
    output[2] = hmi_results.values[1];
    output[1] = hmi_results.values[2];
    output[3] = hmi_results.values[3];
    if ((hmi_detect.flags_66 & 1) == 0) {
        if (hmi_results.values[2] > 1 && hmi_results.values[2] < 0x10)
            return 0;
        else
            return 0x11;
    }
    return 0;
}

hmi_s32 sub_72F3D(hmi_detect_results __far *input)
{
    hmi_u32 value_00;
    hmi_u32 value_04;
    hmi_u32 value_08;
    hmi_s32 size;
    hmi_s32 status;

    /* PROVEN DOS store at 1:72f51; purpose remains OPEN. */
    size = 0x30;
    if (input == 0)
        return 2;
    value_00 = input->values[0];
    value_08 = input->values[2];
    value_04 = input->values[1];
    if ((hmi_detect.flags_66 & 1) == 0) {
        if (value_04 < 2 || value_04 > 0x0f)
            return 0x11;
    }
    hmi_windows_vds_lock(input->values[2]);
    status = hmi_driver_op3_parameters(hmi_detect.memory_b, value_00, value_04,
        value_08, hmi_detect.memory_a);
    hmi_windows_vds_unlock(input->values[2]);
    return status;
}

void hmi_detect_end(void) {}
