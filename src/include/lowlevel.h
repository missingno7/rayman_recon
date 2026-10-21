/* Hand-written assembly routines of the low-level drawing layer (1:00000..1:06f80) called from C.
   Their register ABIs are proven from DOS call sites; the pragmas declare the ABI only (no code). */
#ifndef LOWLEVEL_H
#define LOWLEVEL_H

#include "rtypes.h"

/* 1:061fa; DOS callers load EAX/EBX/ECX/EDX = xmin/xmax/ymin/ymax (jumelle, map_ngawe INIT_GAME_MODE_NORMAL) */
void sprite_clipping(s32 xmin, s32 xmax, s32 ymin, s32 ymax);
#pragma aux sprite_clipping parm [eax] [ebx] [ecx] [edx] modify exact [eax ebx ecx edx]

/* 1:04c19: ESI source, EDI destination, EAX/EBX source x/y, ECX/EDX destination x/y, three callee-popped stack
   arguments (RET 12) - proven by jumelle DisplayJumellesFondNormal */
void DisplayAnyPictureNormal(u8 *source_buffer, u8 *dest_buffer, s32 source_x, s32 source_y,
                             s32 dest_x, s32 dest_y, s32 stride, s32 width, s32 height);
#pragma aux DisplayAnyPictureNormal parm [esi] [edi] [eax] [ebx] [ecx] [edx] modify [eax ebx ecx edx esi edi]

/* 1:04c8e: clears 64000 bytes at EDI (rep stosd; EDI not restored) - map_ngawe INIT_GAME_MODE_NORMAL */
void N_CLRSCR(u8 *buffer);
#pragma aux N_CLRSCR parm [edi] modify [edi]

/* 1:061bd: writes the current clip box through four pointers (EAX/EBX/ECX/EDX); preserves registers - display */
void get_sprite_clipping(s32 *xmin, s32 *xmax, s32 *ymin, s32 *ymax);
#pragma aux get_sprite_clipping parm [eax] [ebx] [ecx] [edx] modify exact []

/* 1:06f5d: target in EBX, current value returned in ECX, modifies EAX/ECX - display WORLD_CHOICE */
s32 wait_until(s32 target);
#pragma aux wait_until parm [ebx] value [ecx] modify exact [eax ecx]

/* 1:06f78: no arguments, preserves registers (calls set_speaker_on) - display */
void speaker_enable(void);
#pragma aux speaker_enable parm [] modify exact []

/* Tile blitters 1:04e64/1:05041/1:053c6/1:0555e: ESI source, EDI destination (+EAX/EBX clip x/y); preserve
   registers - map_ngawe DRAW_MAP (docs/reference/lowlevel_abi.h) */
void Display_Bloc(u8 *source, u8 *dest);
#pragma aux Display_Bloc parm [esi] [edi] modify exact []
void Display_Bloc_Plein(u8 *source, u8 *dest);
#pragma aux Display_Bloc_Plein parm [esi] [edi] modify exact []
void Display_Bloc_Clippe(u8 *source, u8 *dest, s32 x, s32 y);
#pragma aux Display_Bloc_Clippe parm [esi] [edi] [eax] [ebx] modify exact []
void Display_Bloc_Plein_Clippe(u8 *source, u8 *dest, s32 x, s32 y);
#pragma aux Display_Bloc_Plein_Clippe parm [esi] [edi] [eax] [ebx] modify exact []

#endif
