/* Hand-written assembly routines of the low-level drawing layer (1:00000..1:06f80) called from C.
   Their register ABIs are proven from DOS call sites; the pragmas declare the ABI only (no code). */
#ifndef LOWLEVEL_H
#define LOWLEVEL_H

#include "rtypes.h"

/* 1:061fa; DOS callers load EAX/EBX/ECX/EDX = xmin/xmax/ymin/ymax (jumelle, map_ngawe INIT_GAME_MODE_NORMAL) */
void sprite_clipping(s32 xmin, s32 xmax, s32 ymin, s32 ymax);
#pragma aux sprite_clipping parm [eax] [ebx] [ecx] [edx]

/* 1:04c19: ESI source, EDI destination, EAX/EBX source x/y, ECX/EDX destination x/y, three callee-popped stack
   arguments (RET 12) - proven by jumelle DisplayJumellesFondNormal */
void DisplayAnyPictureNormal(u8 *source_buffer, u8 *dest_buffer, s32 source_x, s32 source_y,
                             s32 dest_x, s32 dest_y, s32 stride, s32 width, s32 height);
#pragma aux DisplayAnyPictureNormal parm [esi] [edi] [eax] [ebx] [ecx] [edx] modify [eax ebx ecx edx esi edi]

/* 1:04c8e: clears 64000 bytes at EDI (rep stosd; EDI not restored) - map_ngawe INIT_GAME_MODE_NORMAL */
void N_CLRSCR(u8 *buffer);
#pragma aux N_CLRSCR parm [edi] modify [edi]

#endif
