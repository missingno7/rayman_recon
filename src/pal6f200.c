/* DOS 1:6f200..1:6f3b0; names INVENTED. Both functions STRICT. */
#include "rtypes.h"

extern void clear_memory(void *destination, s32 value, s32 length);
extern void sub_564f8(u32 service, void *registers);
extern s32 int386(u32 interrupt_number, void *interrupt_registers,
                  void *output_registers);
extern u8 byte_CFA14;

u8 detect_windows(u8 *first_result, u8 *second_result)
{
    union { u16 words[11]; u8 bytes[22]; } request;
    u8 code;

    *first_result = 0;
    *second_result = 0;
    clear_memory(&request, 0, 0x16);
    request.words[0] = 0x1600;
    sub_564f8(0x2f, &request);
    code = request.bytes[0];

    switch (code) {
    case 1:
    case 0xff:
        goto special_result;
    case 0:
    case 0x80:
        goto fallback;
    default:
        goto normal_result;
    }

special_result:
    *first_result = 2;
    *second_result = 0;
    return 1;

fallback:
    clear_memory(&request, 0, 0x16);
    request.words[0] = 0x4680;
    sub_564f8(0x2f, &request);
    if (request.bytes[0] == 0x80)
        return 0;

    clear_memory(&request, 0, 0x16);
    request.words[0] = 0x1605;
    request.words[2] = 0;
    request.words[4] = 0;
    request.words[1] = 0;
    request.words[3] = 1;
    sub_564f8(0x2f, &request);
    if (request.words[2] == 0) {
        clear_memory(&request, 0, 0x16);
        request.words[0] = 0x1606;
        sub_564f8(0x2f, &request);
        return 0x81;
    }
    return 0x82;

normal_result:
    *first_result = request.bytes[0];
    *second_result = request.bytes[1];
    return 0x83;
}

u8 detect_dos_version(u8 *first_result, u8 *second_result)
{
    u8 input_registers[0x1c];
    u8 output_registers[0x1c];

    *first_result = 0;
    *second_result = 0;
    clear_memory(input_registers, 0, 0x1c);
    clear_memory(output_registers, 0, 0x1c);
    *(u32 *)input_registers = 0x3000;
    int386(0x21, input_registers, output_registers);

    *first_result = output_registers[0];
    *second_result = output_registers[1];
    if (*first_result == 10 || *first_result == 20)
        byte_CFA14 = 1;
    else
        byte_CFA14 = 0;
    return output_registers[5];
}

