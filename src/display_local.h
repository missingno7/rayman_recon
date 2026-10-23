#include "type_island_abi.h"
/* Local DOS declarations; proposals and evidence in header_proposal.md. */
#ifndef GFIN_DISPLAY_LOCAL_H
#define GFIN_DISPLAY_LOCAL_H
extern s16 SizeScreen;
extern poing_t poing;
extern world_info_t t_world_info[24];

/* Font resources: DOS loads sprite table at +0 and atlas at +4; Android alpha/alpha2 each 12 bytes. */
extern font_t alpha, alpha2;
extern s32 PositionXStageNameWorld[50], PositionYStageNameWorld[50];
extern s32 PositionXStageNameLevel[50], PositionYStageNameLevel[50];
extern s32 LongueurStageNameWorld, LongueurStageNameLevel;
extern s32 TailleStageNameWorld, TailleStageNameLevel, CouleurStageNameWorld, CouleurStageNameLevel;
extern char TexteStageNameWorld[], TexteStageNameLevel[];
extern s32 deter_num_let(u8 c, char *next);
extern s32 calc_let_Width(u8 font, u32 letter);
typedef void stage_draw_t(s32 x, s32 color, s32 y, u16 size, u8 *dest, u8 *pixels);
#pragma aux stage_draw_t parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi]
extern island_draw_t *DrawSpriteColorNormalEtX;

#endif
