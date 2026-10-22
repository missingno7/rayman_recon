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

/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

s16 CBdata[323] = {
    0, 576, 0, 656, 0, 736, 0, 816, 0, 896, 0, 976,
    0, 1056, 0, 1136, 0, -44, 573, -50, 653, -57, 733, -64,
    812, -71, 892, -78, 972, -85, 1051, -92, 1131, -99, -90, 567,
    -100, 646, -113, 724, -127, 803, -141, 882, -155, 961, -169, 1039,
    -183, 1118, -197, -137, 556, -149, 633, -169, 710, -190, 788, -211,
    865, -231, 942, -252, 1020, -273, 1097, -294, -186, 541, -197, 616,
    -224, 691, -251, 766, -279, 841, -306, 917, -333, 992, -361, 1067,
    -388, -238, 522, -243, 594, -277, 667, -311, 739, -344, 812, -378,
    884, -412, 957, -446, 1029, -480, -295, 498, -287, 568, -327, 637,
    -367, 706, -407, 775, -447, 845, -487, 914, -527, 983, -567, -358,
    471, -330, 537, -376, 602, -422, 668, -468, 733, -513, 799, -559,
    865, -605, 930, -651, -429, 441, -370, 502, -421, 563, -473, 625,
    -524, 686, -575, 747, -627, 808, -678, 870, -730, -511, 407, -407,
    463, -463, 520, -520, 576, -576, 633, -633, 690, -690, 746, -746,
    803, -803, -610, 370, -441, 421, -502, 473, -563, 524, -625, 575,
    -686, 627, -747, 678, -808, 730, -870, -731, 330, -471, 376, -537,
    422, -602, 468, -668, 513, -733, 559, -799, 605, -865, 651, -930,
    -886, 288, -498, 328, -568, 368, -637, 408, -706, 448, -775, 488,
    -845, 528, -914, 568, -983, -1097, 243, -522, 277, -594, 311, -667,
    344, -739, 378, -812, 412, -884, 446, -957, 480, -1029, -1406, 197,
    -541, 224, -616, 251, -691, 279, -766, 306, -841, 333, -917, 361,
    -992, 388, -1067, -1910, 149, -556, 169, -633, 190, -710, 211, -788,
    231, -865, 252, -942, 273, -1020, 294, -1097, -2903, 100, -567, 113,
    -646, 127, -724, 141, -803, 155, -882, 169, -961, 183, -1039, 197,
    -1118, -5852, 50, -573, 57, -653, 64, -733, 71, -812, 78, -892,
    85, -972, 92, -1051, 99, -1131, 0, 0, -576, 0, -656, 0,
    -736, 0, -816, 0, -896, 0, -976, 0, -1056, 0, -1136,
}; //96018

/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

s16 speed_ball_angle = 18; //9629E
u8 speed_ball_level = 0; //962A0
s16 speed_ball_x = 0; //962A2
s16 speed_ball_y = 0; //962A4

/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

s32 ball_x = 0; //962A8
s32 ball_y = 0; //962AC
s32 dist2 = 441; //962B0
u8 nb_max_collis = 0; //962B4
u8 cbout = 0; //962B5
u8 undestroy = 0; //962B6
u8 upscreen = 0; //962B7
u8 casse_brique_active = 0; //962B8
u8 CasseBriqueON = 0; //962B9
