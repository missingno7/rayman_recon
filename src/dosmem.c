/* Candidate for obj1:56460..56670. These are DOS DPMI wrappers (INVENTED names). */
#include "rtypes.h"
#include <i86.h>

extern u8 dpmi_workspace[0x5c];
extern void clear_memory(u8 *destination, s32 value, s32 bytes);

#define DPMI16(offset) (*(u16 *)(dpmi_workspace + (offset)))
#define DPMI32(offset) (*(u32 *)(dpmi_workspace + (offset)))

u16 dos_allocate_conventional_block(u16 bytes, u32 *selector_and_segment)
{
    u32 paragraphs;

    DPMI32(0) = 0x100;
    paragraphs = ((s32)bytes + 15) / 16;
    DPMI32(4) = paragraphs;
    int386(0x31, (union REGS *)dpmi_workspace, (union REGS *)dpmi_workspace);
    if ((dpmi_workspace[0x18] & 1) != 0) {
        *selector_and_segment = 0;
        return 0;
    }
    *selector_and_segment = (u32)DPMI16(0) << 16;
    return DPMI16(0x0c);
}

u8 dos_free_conventional_block(u16 handle)
{
    DPMI16(0) = 0x101;
    DPMI16(0x0c) = handle;
    int386(0x31, (union REGS *)dpmi_workspace, (union REGS *)dpmi_workspace);
    return (dpmi_workspace[0x18] & 1) != 0;
}

void sub_564f8(u8 interrupt_number, s16 *registers)
{
    clear_memory(dpmi_workspace, 0, 0x1c);
    clear_memory(dpmi_workspace + 0x50, 0, 0x0c);
    clear_memory(dpmi_workspace + 0x1c, 0, 0x34);

    DPMI32(0x38) = (u16)registers[0];
    DPMI32(0x2c) = (u16)registers[1];
    DPMI32(0x34) = (u16)registers[2];
    DPMI32(0x30) = (u16)registers[3];
    DPMI32(0x20) = (u16)registers[4];
    DPMI32(0x1c) = (u16)registers[5];
    DPMI32(0x3c) = (u16)registers[6];
    DPMI32(0x48) = (u16)registers[7];
    DPMI32(0x40) = (u16)registers[8];
    DPMI16(0x3e) = (u16)registers[9];
    DPMI16(0x4c) = (u16)registers[10];

    DPMI16(0) = 0x0300;
    dpmi_workspace[4] = interrupt_number;
    dpmi_workspace[5] = 0;
    DPMI32(0x14) = (u32)(dpmi_workspace + 0x1c);
    DPMI16(0x50) = FP_SEG((void __far *)dpmi_workspace);
    int386x(0x31, (union REGS *)dpmi_workspace, (union REGS *)dpmi_workspace,
            (struct SREGS *)(dpmi_workspace + 0x50));

    registers[0] = (s16)DPMI16(0x38);
    registers[1] = (s16)DPMI16(0x2c);
    registers[2] = (s16)DPMI16(0x34);
    registers[3] = (s16)DPMI16(0x30);
    registers[4] = (s16)DPMI16(0x20);
    registers[5] = (s16)DPMI16(0x1c);
    registers[6] = (s16)DPMI16(0x3c);
    registers[7] = (s16)DPMI16(0x48);
    registers[8] = (s16)DPMI16(0x40);
    registers[9] = (s16)DPMI16(0x3e);
    registers[10] = (s16)DPMI16(0x4c);
}
