/*
 * Bit and trigonometry helpers (GOG v1.21 object 1:4def0-1:4e160, 624 bytes; all 12 functions STRICT).
 * File name invented (Rayverse mathutil.c). Names: Rayverse/Android. PS1 counterparts (bit_util.c, ray_5D190.c)
 * need source-shape changes for the DOS code; sinus/abs_sinus_cosinus differ in logic (docs/VERSIONS.md).
 */
#include "rtypes.h"

extern s16 costab[257];

void set2bits(u32 *x, u8 index, u32 value)
{
    u32 shift = (15 - index) * 2;
    u32 mask = 3 << shift;
    u32 shifted_value = value << shift;

    *x = *x & (0xffffffff - mask);
    *x |= shifted_value;
}

void read2bits(s32 *value, u8 pos, s32 *b1, s32 *b0)
{
    s32 bits;
    s32 shift;

    bits = *value;
    shift = (0xF - pos) << 1;

    bits &= 3 << shift;
    bits >>= shift;
    *b1 = (bits & (1 << 1)) >> 1;
    *b0 = bits & 1;
}

void setbit(u8 *buffer, u16 index)
{
    s16 byte_index;

    byte_index = index >> 3;
    buffer[byte_index] |= 1 << (index & 7);
}

void clearbit(u8 *buffer, u16 index)
{
    s16 byte_index;

    byte_index = index >> 3;
    buffer[byte_index] &= 0xff - (1 << (index & 7));
}

u8 getbit(u8 *buffer, u16 index)
{
    s16 byte_index;

    byte_index = index >> 3;
    return buffer[byte_index] & (1 << (index & 7));
}

s32 reflexion(s32 a1, s32 unused, s32 a3)
{
    return 2 * a1 - a3;
}

s16 sinus(s16 x);

s16 cosinus(s16 x)
{
    s16 index;

    x += 128;
    x &= 511;
    if (x < 256) {
        index = x - 128;
        if (index < 0)
            index = -index;
        return costab[index];
    } else {
        index = x - 384;
        if (index < 0)
            index = -index;
        return -costab[index];
    }
}

s16 sinus(s16 x)
{
    s16 index;

    x &= 511;
    if (x < 256) {
        index = x - 128;
        if (index < 0)
            index = -index;
        return costab[index];
    } else {
        index = x - 384;
        if (index < 0)
            index = -index;
        return -costab[index];
    }
}

void sinus_cosinus(s16 x, s16 *sinus_result, s16 *cosinus_result)
{
    x &= (512-1);
    if (x < 256) {
        s16 index = x - 128;
        if (index < 0) {
            index = -index;
            *cosinus_result = (s16)costab[128 - index];
        } else {
            *cosinus_result = (s16)-costab[128 - index];
        }
        *sinus_result = costab[index];
    } else {
        s16 index = x - 384;
        if (index < 0) {
            index = -index;
            *cosinus_result = (s16)-costab[128 - index];
        } else {
            *cosinus_result = (s16)costab[128 - index];
        }
        *sinus_result = (s16)-costab[index];
    }
}

s16 abs_sinus(s16 x)
{
    s16 t = (u8)x - 128;
    if (t < 0) {
        t = -t;
    }
    return costab[t];
}

s16 abs_cosinus(s32 x)
{
    s16 t;

    t = x + 128;
    t = (u8)t - 128;
    if (t < 0) {
        t = -t;
    }
    return costab[t];
}

void abs_sinus_cosinus(s16 tab_index, s16 *sinus_result, s16 *cosinus_result)
{
    s16 t = (u8)tab_index - 128;

    if (t < 0)
        t = -t;
    *sinus_result = costab[t];
    *cosinus_result = costab[128 - t];
}
