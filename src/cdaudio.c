/* Strict prefix candidate, 1:193b0..1:1940d; the larger object is incomplete. */
#include "rtypes.h"

extern u8 byte_DEEFB;
extern u8 byte_DEEFC;
extern u8 byte_DEEFD;
extern u8 byte_DEF00;
extern void sub_564f8(s32 command, s32 value);

u8 AudioStatusByte(void)
{
    return byte_DEEFB;
}

u8 sub_193b8(void)
{
    return byte_DEF00;
}

u8 sub_193c0(void)
{
    return byte_DEEFC;
}

u8 sub_193c8(void)
{
    return byte_DEEFD;
}

void sub_193d0(u8 value)
{
    byte_DEEFB = value;
}

void sub_193d8(u8 value)
{
    byte_DEF00 = value;
}

void sub_193e0(u8 value)
{
    byte_DEEFC = value;
}

void sub_193e8(u8 value)
{
    byte_DEEFD = value;
}

void sub_193f0(s32 value)
{
    sub_564f8(0x2f, value);
}

u16 sub_19400(u16 value)
{
    if (value & 0x8000)
        return value & 0xff;
    return 0x100;
}
