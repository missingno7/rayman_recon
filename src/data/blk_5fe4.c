/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

s16 no_sound = 0; //95FE4
u8 InvadersMode = 0; //95FE6
u8 BonusMode = 0; //95FE7
u8 BonusBrik = 20; //95FE8
u8 BonusBrikType = 0; //95FE9
u8 NbClassicBrik = 0; //95FEA
u8 NbBrikTouched = 0; //95FEB
s16 CasseBriqueOffsetX[7] = {0, 0, 6, 2, 5, 4, 8}; //95FEC
s16 CasseBriqueOffsetY[7] = {16, 16, 8, 8, 16, 16, 8}; //95FFA
u8 LastTouched = 20; //96008
s16 NoCB = 0; //9600A
s16 NoCBInit = 0; //9600C
s16 tempo = 500; //9600E
s16 CBoffsetX = 0; //96010
s16 CBoffsetY = 0; //96012
s16 Xdir = 0; //96014
s16 Ydir = 0; //96016
