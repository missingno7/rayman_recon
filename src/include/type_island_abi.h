#ifndef TYPE_ISLAND_ABI_H
#define TYPE_ISLAND_ABI_H
#include "rtypes.h"
/* Existing external assembly contracts only; no inline code. */
typedef void island_draw_t(s32, s32, s32, u32, u8 *, u8 *);
#pragma aux island_draw_t parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi]
typedef void island_snow_t(u8 *, s32, s32);
#pragma aux island_snow_t parm [edi] [eax] [ebx] modify exact [eax ebx]
typedef void island_copy_t(u8 *, u8 *, u16, s16, s16);
#pragma aux island_copy_t parm [esi] [edi] [edx] [ebx] [ecx] modify exact []
typedef s32 island_swap_t(u8 *, u8 *, s32, s32);
#pragma aux island_swap_t parm [esi] [edi] [ecx] [eax] modify exact []
#endif
