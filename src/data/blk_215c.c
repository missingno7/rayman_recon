/* Initialization-only partial proposal. Full central BSS remains OPEN.
 * Whole _DATA and CONST verified; no complete original TU claim.
 * See NOTES.md before using the conditional replacement registry. */
/* RESEARCH _DATA partition 0215c..05ec4; boundaries STRONG/OPEN. */

/* data_215c, 2:0215c. */
/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

typedef struct archive_header_t {
    s32 offset;
    s32 size;
    u8 xor_byte;
    u8 checksum_byte;
    u16 reserved;
} archive_header_t;

//9215C
archive_header_t HeaderFilevignet[75] = {
        {0, 42275, 5, 253}, // 0
        {42275, 36462, 145, 81}, // 1
        {78737, 36152, 208, 209}, // 2
        {114889, 28818, 129, 241}, // 3
        {143707, 46061, 192, 26}, // 4
        {189768, 65113, 59, 169}, // 5
        {254881, 61578, 122, 21}, // 6
        {316459, 50466, 14, 223}, // 7
        {366925, 74739, 69, 202}, // 8
        {441664, 80357, 200, 179}, // 9
        {522021, 70496, 106, 158}, // 10
        {592517, 32003, 16, 240}, // 11
        {624520, 22540, 157, 59}, // 12
        {647060, 24561, 221, 81}, // 13
        {671621, 60596, 60, 233}, // 14
        {732217, 33238, 95, 109}, // 15
        {765455, 31569, 200, 105}, // 16
        {797024, 22153, 98, 77}, // 17
        {819177, 30899, 226, 214}, // 18
        {850076, 21609, 33, 253}, // 19
        {871685, 77649, 175, 199}, // 20
        {949334, 76571, 255, 79}, // 21
        {1025905, 92627, 115, 181}, // 22
        {1118532, 51216, 158, 97}, // 23
        {1169748, 42076, 203, 254}, // 24
        {1211824, 54578, 85, 83}, // 25
        {1266402, 38474, 185, 82}, // 26
        {1304876, 48091, 228, 82}, // 27
        {1352967, 37440, 150, 4}, // 28
        {1390407, 9177, 1, 225}, // 29
        {1399584, 16215, 121, 97}, // 30
        {1415799, 16215, 26, 100}, // 31
        {1432014, 23972, 180, 235}, // 32
        {1455986, 30360, 81, 64}, // 33
        {1486346, 55090, 128, 213}, // 34
        {1541436, 54064, 165, 17}, // 35
        {1595500, 73264, 225, 55}, // 36
        {1668764, 72077, 196, 210}, // 37
        {1740841, 133683, 114, 67}, // 38
        {1874524, 133683, 252, 153}, // 39
        {2008207, 63971, 154, 15}, // 40
        {2072178, 24933, 68, 50}, // 41
        {2097111, 50885, 82, 197}, // 42
        {2147996, 45348, 75, 151}, // 43
        {2193344, 33633, 139, 71}, // 44
        {2226977, 62831, 34, 119}, // 45
        {2289808, 18921, 208, 47}, // 46
        {2308729, 37610, 203, 107}, // 47
        {2346339, 53687, 187, 96}, // 48
        {2400026, 21443, 213, 68}, // 49
        {2421469, 26623, 86, 136}, // 50
        {2448092, 17234, 220, 97}, // 51
        {2465326, 26265, 121, 159}, // 52
        {2491591, 23760, 114, 125}, // 53
        {2515351, 15276, 209, 19}, // 54
        {2530627, 18626, 231, 31}, // 55
        {2549253, 17447, 5, 33}, // 56
        {2566700, 21935, 235, 231}, // 57
        {2588635, 24099, 64, 176}, // 58
        {2612734, 22162, 246, 40}, // 59
        {2634896, 13699, 255, 247}, // 60
        {2648595, 18002, 8, 23}, // 61
        {2666597, 23196, 253, 150}, // 62
        {2689793, 23083, 16, 198}, // 63
        {2712876, 13216, 145, 86}, // 64
        {2726092, 41228, 37, 210}, // 65
        {2767320, 26633, 206, 20}, // 66
        {2793953, 16768, 86, 238}, // 67
        {2810721, 26848, 91, 151}, // 68
        {2837569, 26694, 176, 104}, // 69
        {2864263, 18229, 171, 146}, // 70
        {2882492, 45240, 222, 238}, // 71
        {2927732, 19114, 33, 36}, // 72
        {2946846, 47407, 97, 52}, // 73
        {2994253, 14885, 77, 155}, // 74
};

//924E0
archive_header_t language_infos[] = {
        {0, 4234, 48, 180},      // English
        {4234, 4713, 130, 161},  // French
        {8947, 4903, 207, 92},   // German
        {13850, 2511, 208, 192}, // Japanese
        {16361, 2366, 149, 20},  // Chinese
};

s16 nb_des = 0; //9251C
s16 nb_fix_des = 0; //9251E
s16 dhspeed = 0; //92520
s16 dvspeed = 0; //92522

/* data_coll, 2:02524. */
/*
 * Collision/zone tables: part of the central data translation unit (GOG v1.21 obj2 2:02524-2:035fa).
 * Declarations and initialisers from Rayverse data.c (whose values were taken from the PC binary); every byte and
 * the placement are verified against the oracle by tools/verify.py (data segment check). The file name and the
 * split are invented: the original data TU is larger and its boundaries are not known yet.
 * Comments //9xxxx are Rayverse's PC addresses (obj2 offset + 0x90000).
 */
#include "obj.h"
#include "map.h"
#include "game.h"

//92524
u8 ecroule_rubis_order[24] = {2, 1, 3, 0, 4, 7, 6, 5, 1, 3, 2, 0, 4, 7, 6, 5, 4, 1, 3, 2, 0, 7, 6, 5};

// 0x9253C
block_flags_t block_flags[64] = {   /* initialisers fill .raw (first member) */
        0x00, 0x20, 0x42, 0xC2, 0x42, 0x42, 0xC2, 0xC2, 0x04, 0x0B, //0-9
        0x00, 0x00, 0x00, 0x00, 0x03, 0x13, 0x00, 0x00, 0x4A, 0xCA, //10-19
        0x4A, 0x4A, 0xCA, 0xCA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //20-29
        0x0B, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, //30-39
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, //40-49
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, //50-59
        0x20, 0x20, 0x20, 0x20, //60-63
};

//9257C
obj_type_flags_t flags[262] = {
        0x23038028, 0x02030004, 0x42020000, 0x42218021, 0x42020004, 0x02020020, 0x26471008, 0x02020008, 0x20431100,
        0x23038028, 0x02018018, 0x4A000005, 0x03028028, 0x4222800B, 0x03028028, 0x4222800B, 0x02028104, 0x02028104,
        0x02038004, 0x4A200001, 0x03039128, 0x02028020, 0x02038100, 0x02038040, 0x02038000, 0x02020000, 0x02024000, 0x202C000,
        0x02024100, 0x02024000, 0x20100000, 0x42000001, 0x02028008, 0x4222B009, 0x02038100, 0x03038028, 0x42239009,
        0x02030000, 0x42038001, 0x42238001, 0x03039008, 0x02020008, 0x02120000, 0x03038028, 0x42228009, 0x02028108, 0x3038098,
        0x22031008, 0x02028004, 0x02028000, 0x0300B098, 0x42239009, 0x42239019, 0x02039028, 0x02028000, 0x02028020, 0x3038028,
        0x03028028, 0x22030204, 0x42230005, 0x03038028, 0x03038028, 0x42239009, 0x0202B100, 0x42008000, 0x03038028,
        0x42228009, 0x02020104, 0x0202C000, 0x23031008, 0x03038028, 0x43038009, 0x03230028, 0x4203B009, 0x03038098,
        0x48200005, 0x02038000, 0x03220028, 0x43639009, 0x42238009, 0x02030000, 0x03138820, 0x22120000, 0x42200009,
        0x02020000, 0x03020000, 0x02028100, 0x42020100, 0x42020100, 0x02038020, 0x42238009, 0x42238009, 0x42238009,
        0x42238001, 0x42000011, 0x42020000, 0x03028000, 0x43278019, 0x0303B098, 0x42000004, 0x03138028, 0x02020008,
        0x42238009, 0x02028000, 0x02028000, 0x02028008, 0x02038108, 0x02020008, 0x4212B001, 0x32060000, 0x42220001,
        0x02028000, 0x43231009, 0x02028008, 0x02038000, 0x4202C001, 0x03038028, 0x03038028, 0x03038028, 0x42238009, 0x3039038,
        0x42238009, 0x43239009, 0x230B9028, 0x42120000, 0x02038800, 0x02038008, 0x02038008, 0x02038008, 0x02078008,
        0x42631005, 0x02038000, 0x03028028, 0x02030000, 0x20431100, 0x42228009, 0x42020001, 0x22120000, 0x02038900,
        0x42220005, 0x02038100, 0x42130000, 0x22120200, 0x4A000011, 0x42018200, 0x02028008, 0x42230009, 0x03038028,
        0x42038000, 0x02030004, 0x030284B8, 0x22038008, 0x0303C020, 0x03038030, 0x02038900, 0x02038900, 0x03038008, 0x2000010,
        0x42030000, 0x02030800, 0x02030800, 0x42120000, 0x42020100, 0x02020000, 0x02120000, 0x23038028, 0x02020000,
        0x20431000, 0x0303B000, 0x0313B800, 0x4A228009, 0x42030005, 0x031B8028, 0x0223B000, 0x03029028, 0x02020008,
        0x12060008, 0x26471100, 0x12060008, 0x12060008, 0x03138098, 0x43038020, 0x03038008, 0x03038028, 0x03029028,
        0x03038008, 0x42220001, 0x03038498, 0x02020008, 0x02038108, 0x4203F019, 0x02020008, 0x43229019, 0x4302B009,
        0x4322B009, 0x03031028, 0x4222B009, 0x42020005, 0x03138498, 0x03038020, 0x03138408, 0x03138008, 0x42030005,
        0x42030009, 0x42028019, 0x42028019, 0x42020001, 0x00018000, 0x4A020001, 0x42139009, 0x42139009, 0x42430001,
        0x03028028, 0x0303B098, 0x42831000, 0x4A228011, 0x02028000, 0x03038008, 0x03039008, 0x02028010, 0x0202A010, 0x42020001,
        0x42220001, 0x03028008, 0x43029009, 0x03029028, 0x03029028, 0x0200B098, 0x02038000, 0x02020008, 0x02020008, 0x4302A008,
        0x4302A008, 0x42228009, 0x42020008, 0x02030008, 0x02020000, 0x0203C000, 0x42000005, 0x02230028, 0x12060008,
        0x12060008, 0x12060008, 0x0206C008, 0x42229019, 0x02028000, 0x42038000, 0x02038000, 0x42029009, 0x4202A004,
        0x02020008, 0x42020004, 0x42020004, 0x42020004, 0x02038000, 0x02038000, 0x4203B001, 0x0203B000, 0x02038800,
        0x02038000, 0x02038000, 0x02038000,
};

u8 joy_done = 0; //92994 (DOS: in _DATA, so explicitly initialised; Android has it in .bss)

//92996
s16 zonediffx[262] = {
        0, 0, 0, 0, 0, -50, 0, 0, -80, 0, 0, 0, 250, 0, 250, 0, -60, -60, 0, 0, 60, 0, 0, 0, 0, 0, -60, 0, 0, 0,
        0, -50, 0, 0, 0, 0, 0, 0, -50, 0, 0, 0, -50, 0, 0, 0, 0, 0, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0, -50, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 0, 150, 0, 0, 0, 0, 0, 0, -50, 0, 0, 0, 0, 0, 0, -50, 0, 0, 0, 0, 450,
        -50, 0, 100, 0, -50, 200, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 150, -50,
        0, 0, 0, 0, 0, 0, 0, 0, 0, -80, 0, 0, 0, 0, 0, 0, -50, -50, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, -50, -50, -50, 0, 0, -80, 0, 0, 0, 0, 200, 40, 200, 0, 0, 0, -50, -50, 0, 0, 0, 0, 200, 0, 0, 0,
        0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 450, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 200, 200, 0, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -50, -50, -50, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
};

//92BA2
s16 zonediffy[262] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, -50, 0, 0, 0, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 300, 0, 0, 0, 0, 0, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 120, -50, 0, 150, 0, 0, 200, 200, 0, 0, 0, 0, 0, 350, -50, 0, 300,
        0, 0, 200, 140, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 0, -50, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 120, -50, -50, 0, 0, 0, 100, 300, 0, 200, 0, 0, 120, 0, 0, 0, 0, 0, 0, 150, 150,
        0, -50, -50, 0, 0, 0, 0, 120, 120, 0, 0, 120, 200, 200, 0, 0, 0, 20, 20, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 150, 0, 50, 0, 0, 0, 0, 0, 350, 0, 0, 0, 0, 0, 150, 0, 0, 0, 0, 0, 50, 100, 150, 0,
        0, 0, 0, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 20, 0, 0, 0, 0, 0, 0, 50, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
};

//92DAE
zdc_t zdc_tab[200] = {
        {84, 44, 13, 17, 4, 255}, // 0
        {3, 4, 3, 13, 3, 0}, // 1
        {12, 11, 16, 16, 3, 4}, // 2
        {7, 6, 26, 25, 1, 255}, // 3
        {2, 1, 8, 13, 3, 2}, // 4
        {76, 63, 10, 22, 1, 255}, // 5
        {5, 8, 12, 8, 3, 0}, // 6
        {6, -10, 12, 35, 3, 2}, // 7
        {1, 7, 21, 5, 3, 4}, // 8
        {71, 49, 18, 6, 1, 255}, // 9
        {6, -10, 12, 35, 3, 2}, // 10
        {1, 7, 21, 5, 3, 4}, // 11
        {71, 49, 18, 6, 1, 255}, // 12
        {-21, -19, 64, 64, 3, 0}, // 13
        {105, 168, 48, 60, 1, 255}, // 14
        {97, 129, 2, 2, 1, 255}, // 15
        {8, 6, 20, 20, 3, 0}, // 16
        {6, 5, 10, 11, 3, 0}, // 17
        {74, 73, 13, 14, 1, 255}, // 18
        {9, 10, 16, 16, 3, 0}, // 19
        {14, 1, 14, 44, 3, 0}, // 20
        {9, 10, 16, 16, 3, 0}, // 21
        {8, 6, 20, 20, 3, 0}, // 22
        {64, 48, 32, 32, 1, 255}, // 23
        {0, 0, 16, 16, 3, 5}, // 24
        {17, 1, 9, 9, 3, 7}, // 25
        {24, 64, 112, 96, 1, 255}, // 26
        {11, 10, 16, 16, 3, 0}, // 27
        {16, 26, 48, 38, 3, 0}, // 28
        {11, 11, 16, 16, 3, 0}, // 29
        {1, -7, 12, 34, 3, 2}, // 30
        {8, 6, 20, 20, 3, 0}, // 31
        {1, -8, 13, 30, 3, 0}, // 32
        {6, 3, 11, 2, 3, 0}, // 33
        {17, 13, 58, 76, 3, 2}, // 34
        {15, 14, 23, 59, 3, 5}, // 35
        {63, 77, 21, 10, 4, 255}, // 36
        {8, 5, 18, 23, 3, 3}, // 37
        {7, 14, 6, 7, 3, 0}, // 38
        {0, 2, 208, 6, 3, 3}, // 39
        {79, 52, 6, 19, 1, 255}, // 40
        {71, 56, 19, 18, 1, 255}, // 41
        {71, 56, 19, 18, 1, 255}, // 42
        {75, 58, 12, 12, 1, 255}, // 43
        {6, 4, 24, 25, 6, 4}, // 44
        {2, 4, 18, 14, 3, 5}, // 45
        {8, 8, 49, 51, 3, 6}, // 46
        {18, 9, 16, 16, 3, 1}, // 47
        {19, 2, 14, 68, 3, 3}, // 48
        {-3, -2, 11, 7, 3, 10}, // 49
        {10, 6, 16, 19, 3, 0}, // 50
        {11, 7, 14, 17, 3, 1}, // 51
        {10, 8, 17, 16, 3, 2}, // 52
        {24, 24, 54, 53, 3, 4}, // 53
        {76, 60, 8, 9, 1, 255}, // 54
        {21, 20, 56, 56, 3, 0}, // 55
        {63, 40, 43, 45, 1, 255}, // 56
        {123, 130, 43, 45, 1, 255}, // 57
        {168, 140, 32, 48, 1, 255}, // 58
        {2, -9, 8, 28, 3, 2}, // 59
        {2, -9, 8, 28, 3, 2}, // 60
        {2, -9, 8, 28, 3, 2}, // 61
        {10, 6, 19, 11, 3, 0}, // 62
        {10, 6, 19, 11, 3, 1}, // 63
        {2, 20, 32, 37, 3, 2}, // 64
        {11, 13, 59, 12, 3, 3}, // 65
        {18, 14, 41, 41, 3, 14}, // 66
        {18, 14, 41, 41, 3, 15}, // 67
        {0, 0, 19, 32, 6, 10}, // 68
        {4, 0, 19, 32, 6, 11}, // 69
        {74, 73, 13, 14, 1, 255}, // 70
        {70, 68, 14, 12, 1, 255}, // 71
        {15, 3, 12, 42, 3, 0}, // 72
        {109, 51, 54, 56, 1, 255}, // 73
        {111, 152, 51, 23, 1, 255}, // 74
        {79, 83, 116, 38, 1, 255}, // 75
        {50, 114, 52, 15, 1, 255}, // 76
        {71, 140, 54, 21, 1, 255}, // 77
        {71, 140, 54, 21, 1, 0}, // 78
        {7, 6, 26, 25, 1, 255}, // 79
        {68, 53, 33, 34, 1, 255}, // 80
        {4, 1, 16, 28, 3, 2}, // 81
        {9, 9, 36, 9, 3, 0}, // 82
        {64, 48, 32, 32, 1, 255}, // 83
        {0, 0, 16, 16, 3, 5}, // 84
        {17, 1, 9, 9, 3, 7}, // 85
        {24, 64, 112, 96, 1, 255}, // 86
        {3, 3, 12, 12, 3, 4}, // 87
        {7, 4, 13, 11, 3, 6}, // 88
        {4, 3, 17, 17, 3, 7}, // 89
        {12, 11, 31, 28, 3, 11}, // 90
        {15, 20, 22, 47, 3, 12}, // 91
        {13, -6, 53, 33, 3, 16}, // 92
        {6, 7, 30, 14, 3, 17}, // 93
        {71, 49, 18, 6, 1, 255}, // 94
        {0, 0, 75, 80, 3, 2}, // 95
        {79, 52, 6, 19, 1, 255}, // 96
        {7, 5, 19, 19, 1, 255}, // 97
        {2, 1, 8, 13, 3, 2}, // 98
        {76, 63, 10, 22, 1, 255}, // 99
        {7, 6, 26, 25, 1, 255}, // 100
        {0, 2, 208, 6, 3, 3}, // 101
        {4, 4, 16, 16, 3, 0}, // 102
        {-5, -12, 27, 42, 3, 5}, // 103
        {5, 5, 17, 16, 3, 6}, // 104
        {2, -1, 2, 27, 3, 1}, // 105
        {71, 11, 14, 106, 4, 255}, // 106
        {4, -6, 2, 22, 3, 3}, // 107
        {71, 11, 14, 106, 4, 255}, // 108
        {4, -6, 2, 22, 3, 3}, // 109
        {71, 11, 14, 106, 4, 255}, // 110
        {4, -6, 2, 22, 3, 3}, // 111
        {17, 13, 58, 76, 3, 2}, // 112
        {15, 14, 23, 59, 3, 5}, // 113
        {79, 52, 6, 19, 1, 255}, // 114
        {17, 13, 58, 76, 3, 2}, // 115
        {15, 14, 23, 59, 3, 5}, // 116
        {-5, -12, 27, 42, 3, 5}, // 117
        {5, 5, 17, 16, 3, 6}, // 118
        {79, 52, 6, 19, 1, 255}, // 119
        {18, 7, 32, 29, 3, 2}, // 120
        {7, 0, 26, 23, 3, 5}, // 121
        {9, 11, 33, 13, 1, 255}, // 122
        {9, 11, 33, 13, 1, 255}, // 123
        {-6, -129, 22, 139, 3, 3}, // 124
        {16, 4, 8, 8, 3, 0}, // 125
        {-8, 4, 8, 8, 3, 1}, // 126
        {0, 4, 8, 8, 3, 2}, // 127
        {4, 4, 8, 8, 3, 4}, // 128
        {4, 4, 8, 8, 3, 5}, // 129
        {4, 12, 23, 6, 3, 0}, // 130
        {7, 12, 24, 8, 3, 1}, // 131
        {4, 12, 23, 6, 3, 0}, // 132
        {7, 12, 24, 8, 3, 1}, // 133
        {14, 5, 93, 12, 3, 8}, // 134
        {3, 2, 22, 5, 3, 1}, // 135
        {12, -40, 60, 153, 1, 255}, // 136
        {72, -40, 60, 153, 1, 255}, // 137
        {105, 168, 48, 60, 1, 255}, // 138
        {168, 140, 32, 48, 1, 255}, // 139
        {10, 6, 21, 16, 3, 0}, // 140
        {5, 4, 12, 12, 3, 3}, // 141
        {6, 4, 12, 12, 3, 6}, // 142
        {11, 7, 23, 18, 3, 8}, // 143
        {7, 2, 16, 9, 3, 2}, // 144
        {12, 7, 47, 15, 3, 4}, // 145
        {15, 7, 56, 17, 3, 5}, // 146
        {4, 2, 6, 50, 3, 7}, // 147
        {10, 22, 56, 7, 3, 8}, // 148
        {22, 18, 22, 50, 3, 6}, // 149
        {16, 2, 43, 30, 3, 1}, // 150
        {-5, -12, 27, 42, 3, 5}, // 151
        {5, 5, 17, 16, 3, 6}, // 152
        {-5, -12, 27, 42, 3, 5}, // 153
        {5, 5, 17, 16, 3, 6}, // 154
        {64, 48, 32, 32, 1, 255}, // 155
        {0, 0, 16, 16, 3, 5}, // 156
        {17, 1, 9, 9, 3, 7}, // 157
        {24, 64, 112, 96, 1, 255}, // 158
        {2, 2, 2, 40, 3, 1}, // 159
        {1, 1, 31, 2, 3, 1}, // 160
        {-13, -13, 48, 43, 3, 3}, // 161
        {5, 2, 30, 17, 3, 7}, // 162
        {-13, -13, 48, 43, 3, 3}, // 163
        {5, 2, 30, 17, 3, 7}, // 164
        {13, 4, 7, 8, 3, 8}, // 165
        {0, 80, 128, 1, 1, 255}, // 166
        {64, 0, 32, 128, 1, 255}, // 167
        {6, 5, 7, 17, 3, 1}, // 168
        {7, -8, 9, 34, 3, 3}, // 169
        {71, 33, 14, 95, 4, 255}, // 170
        {4, 4, 2, 19, 3, 2}, // 171
        {71, 33, 14, 95, 4, 255}, // 172
        {4, 4, 2, 19, 3, 2}, // 173
        {71, 33, 14, 95, 4, 255}, // 174
        {4, 4, 2, 19, 3, 2}, // 175
        {84, 42, 13, 98, 4, 255}, // 176
        {5, 1, 2, 19, 3, 0}, // 177
        {16, 4, 8, 8, 3, 0}, // 178
        {-8, 4, 8, 8, 3, 1}, // 179
        {0, 4, 8, 8, 3, 2}, // 180
        {4, 4, 8, 8, 3, 4}, // 181
        {4, 4, 8, 8, 3, 5}, // 182
        {11, 186, 56, 34, 1, 255}, // 183
        {16, -24, 2, 81, 3, 0}, // 184
};

//933EE
u16 type_zdc[262] = {
        0, 0, 0, 0, 0, 0, 4096, 2050, 2051, 4100, 2054, 0, 4103,
        2057, 4106, 2060, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 2061, 0, 2062, 2063, 0, 0, 2064, 2065, 0, 0, 2066,
        2067, 2068, 0, 0, 2069, 0, 2070, 0, 0, 8215, 2075, 2076,
        2077, 0, 0, 0, 0, 0, 0, 0, 2078, 0, 0, 0, 0, 0, 0, 0,
        2079, 0, 0, 2080, 2081, 4130, 0, 0, 4132, 2086, 0, 0,
        2087, 0, 0, 0, 0, 0, 0, 0, 0, 2088, 2089, 2090, 0, 0,
        2091, 6188, 0, 6191, 0, 0, 8242, 2102, 0, 0, 0, 0, 2103,
        0, 2104, 0, 2105, 2106, 0, 0, 0, 2107, 2108, 2109, 0,
        16446, 0, 2118, 2119, 2120, 0, 2121, 6218, 4173, 0,
        0, 0, 0, 0, 2127, 0, 0, 2128, 0, 0, 2129, 0, 0, 0, 0,
        2130, 0, 8275, 0, 0, 14423, 2142, 2143, 0, 0, 0, 2144,
        0, 0, 0, 0, 2145, 0, 0, 0, 4194, 0, 2148, 2149, 0, 2150,
        0, 0, 0, 4199, 2153, 4202, 0, 4204, 4206, 4208, 0, 2162,
        4211, 4213, 2167, 0, 4216, 2170, 2171, 0, 2172, 10365,
        4226, 4228, 0, 0, 0, 0, 0, 0, 0, 0, 2182, 0, 0, 2183,
        0, 0, 2184, 2185, 0, 2186, 2187, 0, 0, 0, 0, 8332, 0,
        0, 0, 0, 12432, 2198, 4247, 4249, 8347, 0, 2207, 2208,
        4257, 4259, 2213, 2214, 0, 2215, 0, 0, 4264, 4266, 4268,
        4270, 4272, 10418, 0, 0, 0, 2231, 0, 2232, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
};

/* data_35fc, 2:035fc. */
/* ObjectsFonctions: DOS GOG v1.21 2:035fc..2:04a74 (5240 bytes).
 * Block split/name INVENTED; function and field names RAYVERSE.
 * DOS: 262 records, stride 20; every 4-byte slot has a code relocation.
 * Indexing: command 1:48325 (+0), poing 1:1d7a0 (+4),
 * rayman collision 1:1ec54/1:1ed1a (+8), detection zone 1:48292 (+12).
 * +16 is a function pointer; its u_turn role/signature is RAYVERSE semantic
 * evidence (no direct DOS access found). Existing callers use the same type.
 * DOS correction: type 182 command = DO_NOTE_CMD (1:65b0c), not DO_ONE_CMD.
 * 2:035fa..2:035fc are preceding alignment bytes, not table members.
 * See NOTES.md for evidence, boundaries, and verification commands.
 */
#include "obj.h"

typedef struct obj_procs_t {
    void (*command)(obj_t *obj);
    void (*poing_collision)(obj_t *obj, s16 sprite);
    void (*rayman_collision)(obj_t *obj);
    void (*rayman_in_zone)(obj_t *obj);
    void (*u_turn)(obj_t *obj);
} obj_procs_t;


extern void BAT_ray_in_zone(obj_t *obj);
extern void BombExplosion(obj_t *obj);
extern void DO_2_PARTS_CYMBAL(obj_t *obj);
extern void DO_BAG1_COMMAND(obj_t *obj);
extern void DO_BAT_COMMAND(obj_t *obj);
extern void DO_BAT_POING_COLLISION(obj_t *obj, s16 sprite);
extern void DO_BB1_PLAT_CMD(obj_t *obj);
extern void DO_BBL_COMMAND(obj_t *obj);
extern void DO_BBMONT2_COMMAND(obj_t *obj);
extern void DO_BBMONT2_MARCHE(obj_t *obj);
extern void DO_BBMONT3_COMMAND(obj_t *obj);
extern void DO_BBMONT4_COMMAND(obj_t *obj);
extern void DO_BBMONT_COMMAND(obj_t *obj);
extern void DO_BBMONT_TOUCHE(obj_t *obj, s16 sprite);
extern void DO_BLKTOON_COMMAND(obj_t *obj);
extern void DO_BLKTOON_EYES_CMD(obj_t *obj);
extern void DO_BOUEE_JOE_COMMAND(obj_t *obj);
extern void DO_CAGE(obj_t *obj);
extern void DO_CAGE2(obj_t *obj);
extern void DO_CB_BALL_COMMAND(obj_t *obj);
extern void DO_CB_BRIK_COMMAND(obj_t *obj);
extern void DO_CCL_COMMAND(obj_t *obj);
extern void DO_CFUMEE_COMMAND(obj_t *obj);
extern void DO_CLOWN_TNT2_COMMAND(obj_t *obj);
extern void DO_CLOWN_TNT3_COMMAND(obj_t *obj);
extern void DO_CLOWN_TNT_COMMAND(obj_t *obj);
extern void DO_COLL_RAY_CYMBALE(obj_t *obj);
extern void DO_CORDE_COMMAND(obj_t *obj);
extern void DO_COU_COMMAND(obj_t *obj);
extern void DO_CYMBAL_COMMAND(obj_t *obj);
extern void DO_DARD_PLAFOND_ALWAYS(obj_t *obj);
extern void DO_DARK2_SORT_COMMAND(obj_t *obj);
extern void DO_DARK2_TOONS_COMMAND(obj_t *obj);
extern void DO_DARK_COMMAND(obj_t *obj);
extern void DO_DARK_PHASE2_COMMAND(obj_t *obj);
extern void DO_DARK_POING_COLLISION(obj_t *obj, s16 sprite);
extern void DO_DARK_REACT(obj_t *obj);
extern void DO_DARK_SORT_COLLISION(obj_t *obj);
extern void DO_DARK_SORT_COMMAND(obj_t *obj);
extern void DO_DROP_COMMAND(obj_t *obj);
extern void DO_EAU_QUI_MONTE(obj_t *obj);
extern void DO_ECLAIR_COMMAND(obj_t *obj);
extern void DO_ENSEIGNE_COMMAND(obj_t *obj);
extern void DO_FALLING_OBJ_CMD(obj_t *obj);
extern void DO_FEE(obj_t *obj);
extern void DO_FLASH_COMMAND(obj_t *obj);
extern void DO_HYB_BBF2_LAS(obj_t *obj);
extern void DO_HYB_BBF2_POING_COLLISION(obj_t *obj, s16 sprite);
extern void DO_IDC_COMMAND(obj_t *obj);
extern void DO_JOE_COMMAND(obj_t *obj);
extern void DO_LEV_POING_COLLISION(obj_t *obj, s16 sprite);
extern void DO_MEDAILLON_TOON(obj_t *obj);
extern void DO_MITE2_COMMAND(obj_t *obj);
extern void DO_MIT_COMMAND(obj_t *obj);
extern void DO_MOVE_MARACAS_COMMAND(obj_t *obj);
extern void DO_MOVING_PLATFORM_COMMAND(obj_t *obj);
extern void DO_MOVING_WITH_INDICATOR_COMMAND(obj_t *obj);
extern void DO_MST_CHANGE_COMMAND(obj_t *obj);
extern void DO_MST_SCROLL_COMMAND(obj_t *obj);
extern void DO_MUSICIEN(obj_t *obj);
extern void DO_NGW_POING_COLLISION(obj_t *obj, s16 sprite);
extern void DO_NOMOVE_STONEWOMAN_COMMAND(obj_t *obj);
extern void DO_NOTE_CMD(obj_t *obj);
extern void DO_NOTE_TOUCHEE(obj_t *obj, s16 sprite);
extern void DO_NOVA2_COMMAND(obj_t *obj);
extern void DO_ONE_CMD(obj_t *obj);
extern void DO_ONE_NGW_COMMAND(obj_t *obj);
extern void DO_ONE_NGW_RING_COMMAND(obj_t *obj);
extern void DO_ONE_PAR_COMMAND(obj_t *obj);
extern void DO_ONE_PINK_CMD(obj_t *obj);
extern void DO_ONE_STONECHIP_COMMAND(obj_t *obj);
extern void DO_PAC_COMMAND(obj_t *obj);
extern void DO_PAR_BOMB_COMMAND(obj_t *obj);
extern void DO_PAR_POING_COLLISION(obj_t *obj, s16 sprite);
extern void DO_PETIT_COUTEAU_COMMAND(obj_t *obj);
extern void DO_PHOTOGRAPHE_CMD(obj_t *obj);
extern void DO_PIEDS_RAYMAN(obj_t *obj);
extern void DO_PIRATE_POELLE(obj_t *obj);
extern void DO_PIRATE_POELLE_POING_COLLISION(obj_t *obj, s16 sprite);
extern void DO_PMA_COMMAND(obj_t *obj);
extern void DO_PMA_POING_COLLISION(obj_t *obj, s16 sprite);
extern void DO_PNG_COLL_STONEMAN(obj_t *obj, s16 sprite);
extern void DO_PNG_COLL_STONEWOMAN(obj_t *obj, s16 sprite);
extern void DO_POELLE_COMMAND(obj_t *obj);
extern void DO_POING(obj_t *obj);
extern void DO_POISSON_BLEU_CMD(obj_t *obj);
extern void DO_POISSON_VERT_CMD(obj_t *obj);
extern void DO_PTI_ESQUIVE(obj_t *obj);
extern void DO_PYRANHA(obj_t *obj);
extern void DO_RAY_POS_CMD(obj_t *obj);
extern void DO_REDUCTEUR(obj_t *obj);
extern void DO_ROLL_EYES(obj_t *obj);
extern void DO_SAXO2_COMMAND(obj_t *obj);
extern void DO_SAXO3_COMMAND(obj_t *obj);
extern void DO_SAXO3_DEBUT(obj_t *obj);
extern void DO_SAXO_COMMAND(obj_t *obj);
extern void DO_SAXO_MARCHE(obj_t *obj);
extern void DO_SKO(obj_t *obj);
extern void DO_SKO_HIT(obj_t *obj, s16 sprite);
extern void DO_SLOPEY_PLAT_CMD(obj_t *obj);
extern void DO_SPIDER_PLAFOND(obj_t *obj);
extern void DO_SPIDER_PLAFOND_POING_COLLISION(obj_t *obj, s16 sprite);
extern void DO_SPIDER_TIR(obj_t *obj);
extern void DO_STONEBOMB_COMMAND(obj_t *obj);
extern void DO_STONEDOG_COMMAND(obj_t *obj);
extern void DO_STONEMAN1_TIR(obj_t *obj);
extern void DO_STONEMAN2_TIR(obj_t *obj);
extern void DO_STONEWOMAN_COMMAND(obj_t *obj);
extern void DO_STONE_EXPLOSION(obj_t *obj, s16 sprite);
extern void DO_TARZAN(obj_t *obj);
extern void DO_TEN_COMMAND(obj_t *obj);
extern void DO_TIRE_BOUCHON_COMMAND(obj_t *obj);
extern void DO_TOTEM_COMMAND(obj_t *obj);
extern void DO_TOTEM_TOUCHE(obj_t *obj, s16 sprite);
extern void DO_TRP_COMMAND(obj_t *obj);
extern void DO_VITRAIL_COMMAND(obj_t *obj);
extern void DO_WLKNOT_COMMAND(obj_t *obj);
extern void DoAnnuleDarkSortRaymanCollision(obj_t *obj);
extern void DoAudioStartRaymanCollision(obj_t *obj);
extern void DoBadGuy1PoingCollision(obj_t *obj, s16 sprite);
extern void DoBadGuy1RaymanZDD(obj_t *obj);
extern void DoBadGuy23PoingCollision(obj_t *obj, s16 sprite);
extern void DoBadGuy23RaymanZDD(obj_t *obj);
extern void DoBlackToonPoingCollision(obj_t *obj, s16 sprite);
extern void DoBlackToonRaymanZDD(obj_t *obj);
extern void DoCagePoingCollision(obj_t *obj, s16 sprite);
extern void DoCaisseClairePoingCollision(obj_t *obj, s16 sprite);
extern void DoCaisseClaireRaymanZDD(obj_t *obj);
extern void DoChasseurPoingCollision(obj_t *obj, s16 sprite);
extern void DoChasseurRaymanZDD(obj_t *obj);
extern void DoFallingObjPoingCollision(obj_t *obj, s16 sprite);
extern void DoFeeRaymanZDD(obj_t *obj);
extern void DoFishPoingCollision(obj_t *obj, s16 sprite);
extern void DoFlammeCommand(obj_t *obj);
extern void DoFlammeRaymanCollision(obj_t *obj);
extern void DoGeneBadGuyPoingCollision(obj_t *obj, s16 sprite);
extern void DoGeneBadGuyRaymanZDD(obj_t *obj);
extern void DoGeneratingDoorRaymanCollision(obj_t *obj);
extern void DoGraineRaymanCollision(obj_t *obj);
extern void DoGrapBonusRaymanCollision(obj_t *obj);
extern void DoHybBBF2LasRaymanCollision(obj_t *obj);
extern void DoJaugeUpRaymanCollision(obj_t *obj);
extern void DoJoeRaymanZDD(obj_t *obj);
extern void DoKillingEyesPoingCollision(obj_t *obj, s16 sprite);
extern void DoLidolPinkRaymanZDD(obj_t *obj);
extern void DoMOSAMSPoingCollision(obj_t *obj, s16 sprite);
extern void DoMite2PoingCollision(obj_t *obj, s16 sprite);
extern void DoMitePoingCollision(obj_t *obj, s16 sprite);
extern void DoMorningStarPoingCollision(obj_t *obj, s16 sprite);
extern void DoMstShakyFruitRaymanZDD(obj_t *obj);
extern void DoMusicienRaymanInZDD(obj_t *obj);
extern void DoNeigeRaymanCollision(obj_t *obj);
extern void DoOneUpPoingCollision(obj_t *obj, s16 sprite);
extern void DoOneUpRaymanCollision(obj_t *obj);
extern void DoPTGRAPPINPoingCollision(obj_t *obj, s16 sprite);
extern void DoPancarteRaymanCollision(obj_t *obj);
extern void DoPetitCouteauPoingCollision(obj_t *obj, s16 sprite);
extern void DoPiMusPoingCollision(obj_t *obj, s16 sprite);
extern void DoPiPoingCollision(obj_t *obj, s16 sprite);
extern void DoPlatformPoingCollision(obj_t *obj, s16 sprite);
extern void DoPoingCollisionDefault(obj_t *obj, s16 sprite);
extern void DoPoingPowerupRaymanCollision(obj_t *obj);
extern void DoPoissonBleuPoingCollision(obj_t *obj, s16 sprite);
extern void DoPoissonBleuRaymanZDD(obj_t *obj);
extern void DoPowerupRaymanCollision(obj_t *obj);
extern void DoPrisePoingCollision(obj_t *obj, s16 sprite);
extern void DoRaymanBag1Collision(obj_t *obj);
extern void DoRaymanCollisionDefault(obj_t *obj);
extern void DoRaymanInZDDDefault(obj_t *obj);
extern void DoReducteurRaymanCollision(obj_t *obj);
extern void DoSTOSKOPoingCollision(obj_t *obj, s16 sprite);
extern void DoSignPostRaymanCollision(obj_t *obj);
extern void DoSpiderPoingCollision(obj_t *obj, s16 sprite);
extern void DoSpiderRaymanZDD(obj_t *obj);
extern void DoStalagRaymanZDD(obj_t *obj);
extern void DoStoneDogPoingCollision(obj_t *obj, s16 sprite);
extern void DoStoneDogRaymanZDD(obj_t *obj);
extern void DoStoneMan1et2RaymanZDD(obj_t *obj);
extern void DoSuperHelicoRaymanCollision(obj_t *obj);
extern void DoTarzanPoingCollision(obj_t *obj, s16 sprite);
extern void DoTntClownPoingCollision(obj_t *obj, s16 sprite);
extern void DoTntClownRaymanZDD(obj_t *obj);
extern void DoTrompetteRaymanZDD(obj_t *obj);
extern void DoTrumpetPoingCollision(obj_t *obj, s16 sprite);
extern void DoWatAndBigClownPoingCollision(obj_t *obj, s16 sprite);
extern void DoWatAndBigClownRaymanZDD(obj_t *obj);
extern void DoWizRaymanCollision(obj_t *obj);
extern void NGW_REACT_TO_RAY_IN_ZONE(obj_t *obj);
extern void ObjectUTurnDefault(obj_t *obj);
extern void PAR_REACT_TO_RAY_IN_ZONE(obj_t *obj);
extern void PIRATE_POELLE_REACT(obj_t *obj);
extern void SKO_ray_in_zone(obj_t *obj);
extern void SPIDER_PLAFOND_REACT(obj_t *obj);
extern void TEST_WIZARD(obj_t *obj);
extern void ToonDonnePoing(obj_t *obj);
extern void blkUTurn(obj_t *obj);
extern void doBBF2command(obj_t *obj);
extern void doBlKRayRaymanCollision(obj_t *obj);
extern void doBlackRaymanCommand(obj_t *obj);
extern void doHerseCommand(obj_t *obj);
extern void doMOSAMScommand(obj_t *obj);
extern void doMereDenisCommand(obj_t *obj);
extern void doMereDenisHit(obj_t *obj, s16 sprite);
extern void doMoskitoCommand(obj_t *obj);
extern void doMoskitoHit(obj_t *obj, s16 sprite);
extern void doSTOSKOcommand(obj_t *obj);
extern void doShipCommand(obj_t *obj);
extern void mereDenisBigLaserCommand(obj_t *obj);
extern void mereDenisBombCommand(obj_t *obj);

obj_procs_t ObjectsFonctions[262] = {
    {DO_ONE_CMD, DoBadGuy1PoingCollision, DoRaymanCollisionDefault, DoBadGuy1RaymanZDD, ObjectUTurnDefault}, // 0: livingstone
    {DO_ONE_CMD, DoPlatformPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 1: platform
    {DO_ONE_CMD, DoPoingCollisionDefault, DoPowerupRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 2: energy point
    {DO_ONE_PINK_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoLidolPinkRaymanZDD, ObjectUTurnDefault}, // 3: electoon
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 4: scenery
    {TEST_WIZARD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 5: magician
    {DO_ONE_CMD, DoFallingObjPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 6: yin with pin
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 7: swinging spiky fruit
    {DO_FALLING_OBJ_CMD, DoFallingObjPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 8: plum 1
    {DO_ONE_CMD, DoBadGuy23PoingCollision, DoRaymanCollisionDefault, DoBadGuy23RaymanZDD, ObjectUTurnDefault}, // 9: small livingstone
    {DO_PYRANHA, DoFishPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 10: piranha
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 11: hit effect
    {DO_ONE_CMD, DoChasseurPoingCollision, DoRaymanCollisionDefault, DoChasseurRaymanZDD, ObjectUTurnDefault}, // 12: hunter 1
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 13: hunter bullet
    {DO_ONE_CMD, DoChasseurPoingCollision, DoRaymanCollisionDefault, DoChasseurRaymanZDD, ObjectUTurnDefault}, // 14: hunter 2
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 15:
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 16: falling platform
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 17: rising platform
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 18:
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 19: splash
    {DO_TEN_COMMAND, DoGeneBadGuyPoingCollision, DoRaymanCollisionDefault, DoGeneBadGuyRaymanZDD, ObjectUTurnDefault}, // 20: tentacle
    {DO_PHOTOGRAPHE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 21: photographer
    {DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 22: moving platform
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 23: rayman
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 24:
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 25: ejecting platform
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 26: disappearing cloud
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 27: trampoline cloud
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 28: blinking cloud
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 29:
    {DO_ONE_CMD, DoPoingCollisionDefault, DoAudioStartRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 30: audio start
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 31: 1-up collected
    {DO_DARK_PHASE2_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 32: Mr Dark 2nd phase
    {DO_DARK2_SORT_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 33: Mr Dark fireballs
    {DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 34: horizontal nenuphar 2
    {DO_STONEMAN1_TIR, DO_PNG_COLL_STONEMAN, DoRaymanCollisionDefault, DoStoneMan1et2RaymanZDD, ObjectUTurnDefault}, // 35: stoneman (throwing)
    {DO_STONEBOMB_COMMAND, DO_STONE_EXPLOSION, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 36: stoneman lava ball
    {DO_TARZAN, DoTarzanPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 37: tarayzan
    {DO_ONE_CMD, DoPoingCollisionDefault, DoGraineRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 38: magic seed
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 39: planted nenuphar
    {DO_STONEDOG_COMMAND, DoStoneDogPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 40: stone dog
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 41: prickly
    {DO_ONE_CMD, DoPoingCollisionDefault, DoSignPostRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 42: exit sign
    {DO_STONEMAN2_TIR, DO_PNG_COLL_STONEMAN, DoRaymanCollisionDefault, DoStoneMan1et2RaymanZDD, ObjectUTurnDefault}, // 43: stoneman (shooting)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 44: stoneman flame
    {DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 45: moving prickly
    {DO_BBMONT_COMMAND, DO_BBMONT_TOUCHE, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 46: Mr Stone
    {DO_STONEBOMB_COMMAND, DO_STONE_EXPLOSION, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 47: bouncing lava ball
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 48: torch
    {DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 49: moving flat stone
    {doMoskitoCommand, doMoskitoHit, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 50: moskito (Bzzit)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 51: moskito fruit small
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 52: moskito fruit medium
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoMstShakyFruitRaymanZDD, ObjectUTurnDefault}, // 53: shaky fruit
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 54: medaillon
    {DO_MUSICIEN, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoMusicienRaymanInZDD, ObjectUTurnDefault}, // 55: musician
    {DO_NOMOVE_STONEWOMAN_COMMAND, DO_PNG_COLL_STONEWOMAN, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 56: nonmoving stoneman
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoStalagRaymanZDD, ObjectUTurnDefault}, // 57: stake
    {DO_CAGE, DoCagePoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 58: cage
    {DO_CAGE2, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 59: opened cage
    {DO_ONE_CMD, DoWatAndBigClownPoingCollision, DoRaymanCollisionDefault, DoWatAndBigClownRaymanZDD, ObjectUTurnDefault}, // 60: big clown
    {DO_ONE_CMD, DoWatAndBigClownPoingCollision, DoRaymanCollisionDefault, DoWatAndBigClownRaymanZDD, ObjectUTurnDefault}, // 61: big water clown
    {DO_DROP_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 62: water balloon
    {DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 63: platform starts moving
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 64: autoscroll
    {DO_SPIDER_TIR, DoSpiderPoingCollision, DoRaymanCollisionDefault, DoSpiderRaymanZDD, ObjectUTurnDefault}, // 65: spider
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 66: spider dart
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 67: swinging flower
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 68: big boing plat
    {DO_STONEBOMB_COMMAND, DO_STONE_EXPLOSION, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 69: big bouncing boulder
    {DO_TRP_COMMAND, DoTrumpetPoingCollision, DoRaymanCollisionDefault, DoTrompetteRaymanZDD, ObjectUTurnDefault}, // 70: trumpet
    {DO_NOTE_CMD, DO_NOTE_TOUCHEE, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 71: note
    {DO_ONE_NGW_COMMAND, DO_NGW_POING_COLLISION, DoRaymanCollisionDefault, NGW_REACT_TO_RAY_IN_ZONE, ObjectUTurnDefault}, // 72: pirate with earring
    {DO_ONE_NGW_RING_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 73: earring
    {DO_SAXO_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 74: Mr Sax
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 75: bounce glitter
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 76: kildoor
    {DO_ONE_PAR_COMMAND, DO_PAR_POING_COLLISION, DoRaymanCollisionDefault, PAR_REACT_TO_RAY_IN_ZONE, ObjectUTurnDefault}, // 77: bomber pirate
    {DO_PAR_BOMB_COMMAND, DoPoingCollisionDefault, BombExplosion, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 78: pirate bomb
    {DO_ONE_STONECHIP_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 79: lava ball fragment
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 80: bigstone
    {DO_CYMBAL_COMMAND, DoPoingCollisionDefault, DO_COLL_RAY_CYMBALE, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 81: cymbal
    {DO_ONE_CMD, DoPoingCollisionDefault, DoJaugeUpRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 82: big powerup
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 83: explosion / dust effect
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 84: tibetan juggler
    {DO_ROLL_EYES, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 85: eyes
    {DO_MOVE_MARACAS_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 86: maracas
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 87: tambourin platform1
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 88: tambourin platform2
    {DO_JOE_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoJoeRaymanZDD, ObjectUTurnDefault}, // 89: Joe
    {DO_ONE_CMD, DO_NOTE_TOUCHEE, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 90: Mr Sax wrong note
    {DO_ONE_CMD, DO_NOTE_TOUCHEE, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 91: note bomb 1
    {DO_NOTE_CMD, DO_NOTE_TOUCHEE, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 92: note bomb 2
    {DO_NOTE_CMD, DO_NOTE_TOUCHEE, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 93: punched wrong note
    {DO_POING, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 94: fist
    {DO_ONE_CMD, DoPoingCollisionDefault, DoPoingPowerupRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 95: fist pickup
    {DO_TOTEM_COMMAND, DO_TOTEM_TOUCHE, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 96: Mr Stone totem
    {DO_BBL_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 97: Mr Stone boulder
    {doMereDenisCommand, doMereDenisHit, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 98: Space Mama
    {DO_RAY_POS_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 99: Rayman start pos
    {DO_MIT_COMMAND, DoMitePoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 100: moth
    {DO_ONE_CMD, DoMorningStarPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 101: swinging prickly
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 102: tiny wrong note
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 103: large eyed fish
    {DO_POISSON_VERT_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 104: large nosed fish
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 105: bouncing crystal rock 1
    {DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 106: bouncing crystal rock 2
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 107: big spiky ball
    {DO_DARK2_TOONS_COMMAND, DoPoingCollisionDefault, ToonDonnePoing, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 108: Mr Dark electoons
    {DO_ONE_CMD, DoPiPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 109: boulder
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 110: destroyed boulder
    {DO_ONE_CMD, DoPiMusPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 111: crushed guitar
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 112: washing machine
    {DO_BAG1_COMMAND, DoPoingCollisionDefault, DoRaymanBag1Collision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 113: mallets
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 114:
    {DO_BB1_PLAT_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 115: Mr Stone cloud
    {DO_CLOWN_TNT_COMMAND, DoTntClownPoingCollision, DoRaymanCollisionDefault, DoTntClownRaymanZDD, ObjectUTurnDefault}, // 116: TNT clown
    {DO_CLOWN_TNT2_COMMAND, DoTntClownPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 117: TNT clown burst
    {DO_CLOWN_TNT3_COMMAND, DoTntClownPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 118: TNT clown kamikaze
    {DO_ONE_CMD, DoPoingCollisionDefault, BombExplosion, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 119: clown TNT
    {DO_BAT_COMMAND, DO_BAT_POING_COLLISION, DoRaymanCollisionDefault, BAT_ray_in_zone, ObjectUTurnDefault}, // 120: red drummer
    {DO_ECLAIR_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 121: Mr Stone flame
    {DO_STONEDOG_COMMAND, DoStoneDogPoingCollision, DoRaymanCollisionDefault, DoStoneDogRaymanZDD, ObjectUTurnDefault}, // 122: Mr Stone stone dog
    {DO_BLKTOON_COMMAND, DoBlackToonPoingCollision, DoRaymanCollisionDefault, DoBlackToonRaymanZDD, blkUTurn}, // 123: antitoon
    {DO_ONE_CMD, DoPoingCollisionDefault, DoPancarteRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 124: return sign
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 125: candy platform
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 126: fork
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 127: swiss knife
    {DO_TIRE_BOUCHON_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 128: corkscrew
    {DO_PETIT_COUTEAU_COMMAND, DoPetitCouteauPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 129: small knife
    {DO_BLKTOON_EYES_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 130: antitoon eyes
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 131: mallets spring
    {DO_POISSON_BLEU_CMD, DoPoissonBleuPoingCollision, DoRaymanCollisionDefault, DoPoissonBleuRaymanZDD, ObjectUTurnDefault}, // 132: large mouthed fish
    {DO_ONE_CMD, DoPoingCollisionDefault, DoSuperHelicoRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 133: supercopter
    {DO_FALLING_OBJ_CMD, DoFallingObjPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 134: plum2
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 135: flame Mr Stone
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 136: tiny Rayman
    {DO_REDUCTEUR, DoPoingCollisionDefault, DoReducteurRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 137: fairy
    {DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 138: sprinkler (4 heads)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 139: maracas flying
    {DO_MOVING_PLATFORM_COMMAND, DoPTGRAPPINPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 140: ring
    {DO_ONE_CMD, DoPoingCollisionDefault, DoNeigeRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 141: snow
    {DO_ONE_CMD, DoOneUpPoingCollision, DoOneUpRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 142: extralife
    {DO_NOVA2_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 143: item blink
    {DO_ONE_PINK_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 144: lidolpink2
    {DO_ROLL_EYES, DoKillingEyesPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 145: bad eyes
    {DO_FLASH_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 146: eyes bolt
    {DO_MST_SCROLL_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DO_MST_CHANGE_COMMAND, ObjectUTurnDefault}, // 147: moskito huge spiky fruit
    {DO_ONE_CMD, DoPoingCollisionDefault, DoGrapBonusRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 148: grap bonus
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 149: treble clef
    {DO_SKO, DO_SKO_HIT, DoRaymanCollisionDefault, SKO_ray_in_zone, ObjectUTurnDefault}, // 150: Mr Skops
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 151: bullet
    {DO_CCL_COMMAND, DoCaisseClairePoingCollision, DoRaymanCollisionDefault, DoCaisseClaireRaymanZDD, ObjectUTurnDefault}, // 152: snare drum
    {DO_FEE, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoFeeRaymanZDD, ObjectUTurnDefault}, // 153: Betilla
    {DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 154: sprinkler (4 heads, moving)
    {DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 155: sprinkler (3 heads)
    {DO_WLKNOT_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 156: walk note 1
    {DO_EAU_QUI_MONTE, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 157: water
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 158: palette change
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 159: tibetan 6 platforms
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 160: tibetan 2 platforms
    {DO_ONE_CMD, DoPoingCollisionDefault, DoWizRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 161: ting
    {DO_MOVING_WITH_INDICATOR_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 162: flying saucer platform
    {DO_IDC_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 163: circuit box
    {DO_ONE_CMD, DoPoingCollisionDefault, DoGeneratingDoorRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 164: gendoor
    {DO_PTI_ESQUIVE, DoBadGuy23PoingCollision, DoRaymanCollisionDefault, DoBadGuy23RaymanZDD, ObjectUTurnDefault}, // 165: evil livingstone
    {DO_ONE_CMD, DO_LEV_POING_COLLISION, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 166: lever
    {DO_FALLING_OBJ_CMD, DoFallingObjPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 167: plum 3
    {DO_2_PARTS_CYMBAL, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 168: cymbal1 single
    {DO_2_PARTS_CYMBAL, DoPoingCollisionDefault, DO_COLL_RAY_CYMBALE, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 169: cymbal2
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 170: Mr Skops beam
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 171: Rayman Bzzit cutscene
    {DO_STONEWOMAN_COMMAND, DO_PNG_COLL_STONEWOMAN, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 172: stoneman (aggressive)
    {doShipCommand, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 173: viking ship
    {DO_PIRATE_POELLE, DO_PIRATE_POELLE_POING_COLLISION, DoRaymanCollisionDefault, PIRATE_POELLE_REACT, ObjectUTurnDefault}, // 174: space pirate (left)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 175: punaise platform
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 176: pencil pointing down
    {DO_ONE_CMD, DoFallingObjPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 177: yin1
    {doHerseCommand, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 178: pencil pointing down (moving)
    {doHerseCommand, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 179: pencil pointing down (wave)
    {DO_SAXO2_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 180: Mr Sax (chasing)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DO_SAXO_MARCHE, ObjectUTurnDefault}, // 181: Mr Sax walk speed
    {DO_NOTE_CMD, DO_NOTE_TOUCHEE, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 182: wrong note stationary punchable
    {DO_SAXO3_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DO_SAXO3_DEBUT, ObjectUTurnDefault}, // 183: Mr Sax (first encounter)
    {DO_PIRATE_POELLE, DO_PIRATE_POELLE_POING_COLLISION, DoRaymanCollisionDefault, PIRATE_POELLE_REACT, ObjectUTurnDefault}, // 184: space pirate (right)
    {DO_WLKNOT_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 185: walk note 2
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 186: pirate pan flying away
    {DO_PMA_COMMAND, DO_PMA_POING_COLLISION, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 187: Viking Mama
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 188: red crystal platform
    {DO_MOVING_PLATFORM_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 189: red crystal platform (moving)
    {DO_COU_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 190: Viking Mama sword
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 191: pencil (falling)
    {mereDenisBigLaserCommand, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 192: Space Mama beam
    {mereDenisBombCommand, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 193: Space Mama pot
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 194: Space Mama pot part
    {DO_SPIDER_PLAFOND, DO_SPIDER_PLAFOND_POING_COLLISION, DoRaymanCollisionDefault, SPIDER_PLAFOND_REACT, ObjectUTurnDefault}, // 195: spider (ceiling)
    {DO_DARD_PLAFOND_ALWAYS, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 196: spider homing dart
    {DO_MEDAILLON_TOON, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 197: medaillon
    {DO_BBMONT2_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 198: Mr Stone (chase)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DO_BBMONT2_MARCHE, ObjectUTurnDefault}, // 199: Mr Stone (chase, walking)
    {DO_BBMONT3_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 200: BB13
    {DO_BBMONT4_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 201: BB14
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 202: stars on Rayman
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 203: washing machine 2
    {doBlackRaymanCommand, DoPoingCollisionDefault, doBlKRayRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 204: dark Rayman
    {DO_ONE_CMD, DoPoingCollisionDefault, doBlKRayRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 205: dark Rayman fist
    {DO_PIEDS_RAYMAN, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 206: Rayman's feet
    {DO_POELLE_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 207: frying pan
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 208: dust circle
    {DoFlammeCommand, DoPoingCollisionDefault, DoFlammeRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 209: Mr Dark flame (left)
    {DoFlammeCommand, DoPoingCollisionDefault, DoFlammeRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 210: Mr Dark flame (right)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 211: Mr Stone swinging totem
    {DO_DARK_COMMAND, DO_DARK_POING_COLLISION, DoRaymanCollisionDefault, DO_DARK_REACT, ObjectUTurnDefault}, // 212: Mr Dark
    {doMereDenisCommand, doMereDenisHit, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 213: Space Mama (with washing machine)
    {DO_BOUEE_JOE_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 214: buoy
    {DO_DARK_SORT_COMMAND, DoPoingCollisionDefault, DO_DARK_SORT_COLLISION, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 215: Mr Dark spell fireball
    {DO_ENSEIGNE_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 216: Eat at Joe's sign
    {DO_MITE2_COMMAND, DoMite2PoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 217: moth 2
    {doMOSAMScommand, DoMOSAMSPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 218: Spaxkito
    {DO_CORDE_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 219: flat stone rope
    {DO_PAC_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 220: flat stone (with ropes)
    {DO_CFUMEE_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 221: rope dust
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 222: flat stone (detached rope)
    {doSTOSKOcommand, DoSTOSKOPoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 223: Stoneskops
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 224: Stoneskops claw
    {DO_PIRATE_POELLE, DO_PIRATE_POELLE_POING_COLLISION, DoRaymanCollisionDefault, PIRATE_POELLE_REACT, ObjectUTurnDefault}, // 225: pan pirate (45 degrees)
    {DO_PIRATE_POELLE, DO_PIRATE_POELLE_POING_COLLISION, DoRaymanCollisionDefault, PIRATE_POELLE_REACT, ObjectUTurnDefault}, // 226: pan pirate 2 (45 degrees)
    {doMoskitoCommand, doMoskitoHit, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 227: Moskito
    {DO_ONE_CMD, DoPrisePoingCollision, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 228: electric plug
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 229: punaise (facing down)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 230: punaise (horizontal)
    {doBBF2command, DO_HYB_BBF2_POING_COLLISION, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 231: Moskitomama (right)
    {doBBF2command, DO_HYB_BBF2_POING_COLLISION, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 232: Moskitomama (left)
    {DO_HYB_BBF2_LAS, DoPoingCollisionDefault, DoHybBBF2LasRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 233: Moskitomama beam
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 234: lava
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 235: punaise
    {DO_ONE_CMD, DoPoingCollisionDefault, DoAnnuleDarkSortRaymanCollision, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 236: cancel Mr Dark spell
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 237: eraser
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 238: Betilla text fist
    {DO_ONE_PAR_COMMAND, DO_PAR_POING_COLLISION, DoRaymanCollisionDefault, PAR_REACT_TO_RAY_IN_ZONE, ObjectUTurnDefault}, // 239: viking ship nest
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 240: pencil (pointing up)
    {doHerseCommand, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 241: pencil (pointing up, moving)
    {doHerseCommand, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 242: pencil (pointing up, wave)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 243: stylo
    {mereDenisBigLaserCommand, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 244: Space Mama second beam
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 245: Joe's beach
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 246: Mr Dark separate rope
    {DO_VITRAIL_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 247: vitrail
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 248: Mr Skops claw
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 249: theatre curtain
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 250: punaise (up down)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 251: theatre water (front)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 252: theatre water (back)
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 253: planches
    {DO_SLOPEY_PLAT_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 254: slopey plat
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 255: invalid
    {DO_CB_BRIK_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 256: breakout brick 20
    {DO_CB_BALL_COMMAND, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 257: breakout game
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 258: nougat
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 259: EDU letter
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 260: EDU digit
    {DO_ONE_CMD, DoPoingCollisionDefault, DoRaymanCollisionDefault, DoRaymanInZDDDefault, ObjectUTurnDefault}, // 261: EDU road sign
};

/* data_4a74, 2:04a74. */
/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

u8 nb_levels_in_world[7] = {0, 22, 18, 13, 13, 12, 4}; //94A74
s16 pente[16] = {0, 3, 7, 10, 13, 16, 19, 22, 25, 28, 31, 34, 37, 40, 42, 45}; //94A7C

//94A9C
u8 N_anim[256] = {
        8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5,
        5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

s16 bagDy[20] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -60, -26, 8, 32, 75, 32, 8, -26, -60, -1}; //94B9C
s16 bagW[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 32, 48, 48, 48, 48, 48, 32, 32, 0}; //94BC4
s16 bagH[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 32, 48, 48, 60, 48, 48, 32, 32, 0}; //94BEC

//94C14
s16 expsin[63] = {
        0, 855, 825, 0, -769, -742, -1, 690, 666, 0, -621, -599, -1, 557, 537, 0,
        -501, -484, -1, 449, 434, 0, -405, -391, -1, 363, 350, 0, -327, -315, -1, 292,
        282, 0, -264, -255, -1, 236, 228, 0, -213, -206, -1, 190, 184, 0, -172, -166,
        -1, 153, 148, 0, -139, -134, -1, 124, 120, 0, -112, -108, -1, 80, 32640,
};

//94C92
s16 expsin2[49] = {
        3, 0, -3, 0, 3, 0, -3, 0, 3, 0, -3, 0, 3, 0, -3, 0,
        3, 0, -3, 0, 3, 0, -3, 0, 3, 0, -3, 0, 3, 0, -3, 0,
        3, 0, -3, 0, 3, 0, -3, 0, 3, 0, -3, 0, 3, 0, -3, 0, 255
};

s16 EauDy[14] = {0, -3, -6, -8, -9, -10, -9, -7, -5, -2, 2, 6, 7, 4}; //94CF4
s16 DecEau[4] = {9, 0, 7, 5}; //94D10

//94D18
u8 angletab[65] = {
        128, 126, 125, 124, 122, 121, 120, 119, 117, 116, 115,
        114, 112, 111, 110, 109, 108, 106, 105, 104, 103, 102,
        101, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88,
        87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 78, 77, 76,
        75, 74, 73, 73, 72, 71, 70, 70, 69, 68, 68, 67, 66,
        65, 65, 64, 64
};

/* data_4d5c, 2:04d5c. */
/* Typed initializers with original TU string literals. See NOTES.md. */
#include "game.h"


world_info_t t_world_info[24] = {
    {29, 264, 18, 0, 0, 1, {1}, 0, 1, 1, 2, 0, 0, 0, ""},
    {100, 254, 2, 1, 0, 4, {0}, 0, 1, 5, 0, 0, 0, 0, ""},
    {90, 212, 2, 1, 3, 2, {0}, 0, 1, 9, 2, 0, 0, 0, ""},
    {47, 200, 3, 3, 3, 2, {0}, 0, 1, 12, 1, 0, 0, 0, ""},
    {180, 215, 5, 19, 1, 4, {0}, 0, 2, 1, 2, 0, 0, 0, ""},
    {185, 175, 8, 4, 6, 8, {0}, 0, 2, 7, 0, 0, 0, 0, ""},
    {135, 160, 20, 6, 7, 5, {0}, 0, 2, 12, 2, 0, 0, 0, ""},
    {91, 143, 7, 7, 7, 6, {0}, 0, 2, 14, 3, 0, 0, 0, ""},
    {212, 133, 9, 5, 5, 8, {0}, 0, 3, 1, 1, 0, 0, 0, ""},
    {209, 92, 21, 8, 9, 10, {0}, 0, 3, 3, 0, 0, 0, 0, ""},
    {257, 80, 10, 11, 9, 10, {0}, 0, 3, 6, 1, 0, 0, 0, ""},
    {234, 235, 10, 11, 11, 12, {0}, 0, 4, 1, 1, 0, 0, 0, ""},
    {278, 190, 13, 11, 11, 12, {0}, 0, 4, 5, 5, 0, 0, 0, ""},
    {290, 145, 22, 12, 13, 14, {0}, 0, 4, 8, 1, 0, 0, 0, ""},
    {335, 180, 13, 15, 13, 14, {0}, 0, 5, 1, 0, 0, 0, 0, ""},
    {312, 228, 14, 23, 15, 16, {0}, 0, 5, 3, 5, 0, 0, 0, ""},
    {360, 220, 17, 16, 15, 16, {0}, 0, 5, 9, 2, 0, 0, 0, ""},
    {357, 80, 17, 16, 17, 17, {0}, 0, 6, 1, 2, 0, 0, 0, ""},
    {10, 212, 18, 0, 18, 18, {0}, 0, 7, 20, 5, 0, 0, 0, ""},
    {182, 266, 4, 19, 19, 19, {0}, 0, 7, 20, 3, 0, 0, 0, ""},
    {145, 97, 20, 6, 20, 20, {0}, 0, 7, 20, 0, 0, 0, 0, ""},
    {190, 45, 21, 9, 21, 21, {0}, 0, 7, 20, 2, 0, 0, 0, ""},
    {276, 115, 22, 13, 22, 22, {0}, 0, 7, 20, 1, 0, 0, 0, ""},
    {306, 267, 15, 23, 23, 23, {0}, 0, 7, 20, 1, 0, 0, 0, ""},
};

/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

u8 mapCd[132] = {
        14, 14, 9, 14, 14, 5, 14, 9, 14, 14, 14, 14, 14, 14, 14, 5, 9, 15, 15, 15, 15, 15,
        17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 9, 17, 17, 17, 7, 7, 15, 15, 0, 0, 0, 0,
        16, 16, 16, 16, 16, 16, 16, 16, 16, 6, 9, 15, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        13, 13, 13, 4, 13, 13, 13, 13, 13, 13, 4, 15, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 3, 3, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        11, 11, 11, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0
};

//94FBC
s32 timeCd[132] = {
        293, 293, 81, 293, 293, 151, 293, 81,
        293, 293, 293, 293, 293, 293, 293, 151,
        81, 70, 70, 70, 70, 70, 315, 315,
        315, 315, 315, 315, 315, 315, 315, 315,
        81, 315, 315, 315, 149, 149, 70, 70,
        0, 0, 0, 0, 206, 206, 206, 206,
        206, 206, 206, 206, 206, 148, 81, 70,
        70, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 258, 258, 258, 151, 258, 258,
        258, 258, 258, 258, 151, 70, 70, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        282, 282, 282, 282, 282, 282, 282, 282,
        282, 146, 146, 70, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 216, 216,
        216, 148, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0
};

/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

u8 let_shadow = 0; //951D0

/* Typed initializers with original TU string literals. See NOTES.md. */
#include "game.h"


/* INIT_CREDITS 1:5af91..5aff6: 12-byte copies, cmd byte at +9.
 * STRONG capacity: source ends at DemoRecord 2:05828; the destination
 * credits 2:3d140 ends at save_ray_copy 2:3d794. Both extents are 135 * 12.
 * Slots 115..134 are implicitly zero-initialised reserve records. */
credit_t credits_init[135] = {
    {"", 160, 110, 1, 4, 2, 0},
    {"", 160, 112, 2, 102, 1, 0},
    {"/michel ancel/", 160, 0, 2, 0, 1, 0},
    {"/frederic houde/", 160, 0, 2, 3, 1, 0},
    {"", 160, 113, 2, 102, 2, 0},
    {"/alexandra steible/", 160, 0, 2, 3, 2, 0},
    {"", 160, 115, 2, 102, 5, 0},
    {"/michel ancel/", 160, 0, 2, 0, 5, 0},
    {"/serge hascoet/", 160, 0, 2, 0, 5, 0},
    {"", 160, 111, 2, 0, 5, 0},
    {"/bruno bouvret/", 160, 0, 2, 0, 5, 0},
    {"/gunther galipot/", 160, 0, 2, 0, 5, 0},
    {"/sacha gentilhomme/", 160, 0, 2, 0, 5, 0},
    {"/michael guez/", 160, 0, 2, 0, 5, 0},
    {"/christophe thibaut/", 160, 0, 2, 3, 5, 0},
    {"", 160, 116, 2, 102, 0, 0},
    {"/alexandra steible/", 160, 0, 2, 0, 0, 0},
    {"", 160, 111, 2, 0, 0, 0},
    {"/kamal aitmihoub/", 160, 0, 2, 0, 0, 0},
    {"/david gilson/", 160, 0, 2, 0, 0, 0},
    {"/sophie esturgie/", 160, 0, 2, 0, 0, 0},
    {"/jacques exertie/", 160, 0, 2, 0, 0, 0},
    {"/gregoire pons/", 160, 0, 2, 0, 0, 0},
    {"/veronique rondello/", 160, 0, 2, 0, 0, 0},
    {"/beatrice sauterau/", 160, 0, 2, 0, 0, 0},
    {"/philippe vindolet", 160, 0, 2, 0, 0, 0},
    {"/christian volckman/", 160, 0, 2, 3, 0, 0},
    {"", 160, 117, 2, 102, 1, 0},
    {"/eric pelatan/", 160, 0, 2, 0, 1, 0},
    {"/sylvaine jenny/", 160, 0, 2, 3, 1, 0},
    {"", 160, 114, 2, 102, 0, 0},
    {"/jean christophe alessandri/", 160, 0, 2, 0, 0, 0},
    {"/sophie ancel/", 160, 0, 2, 0, 0, 0},
    {"/olivier bechard/", 160, 0, 2, 0, 0, 0},
    {"/nicolas bocquillon/", 160, 0, 2, 0, 0, 0},
    {"/jean marc geffroy/", 160, 0, 2, 0, 0, 0},
    {"/laurent rettig/", 160, 0, 2, 0, 0, 0},
    {"/olivier soleil/", 160, 0, 2, 3, 0, 0},
    {"", 160, 118, 2, 102, 5, 0},
    {"/bertrand helias/", 160, 0, 2, 0, 5, 0},
    {"/yann le tensorer/", 160, 0, 2, 0, 5, 0},
    {"/daniel palix/", 160, 0, 2, 0, 5, 0},
    {"", 160, 111, 2, 0, 5, 0},
    {"/cyril cauchois/", 160, 0, 2, 0, 5, 0},
    {"/olivier couvreur/", 160, 0, 2, 0, 5, 0},
    {"/antoine dodens/", 160, 0, 2, 0, 5, 0},
    {"/antonio ferreira/", 160, 0, 2, 0, 5, 0},
    {"/philippe fuentes/", 160, 0, 2, 0, 5, 0},
    {"/benoit germain/", 160, 0, 2, 0, 5, 0},
    {"/christophe giraud/", 160, 0, 2, 0, 5, 0},
    {"/frederic houde/", 160, 0, 2, 0, 5, 0},
    {"/francois mahieu/", 160, 0, 2, 0, 5, 0},
    {"/christian mihalache/", 160, 0, 2, 0, 5, 0},
    {"/sebastien morin/", 160, 0, 2, 0, 5, 0},
    {"/bogdan radusi/", 160, 0, 2, 0, 5, 0},
    {"/daniel raviart/", 160, 0, 2, 0, 5, 0},
    {"/olivier rozier/", 160, 0, 2, 0, 5, 0},
    {"/jozef skrabo/", 160, 0, 2, 0, 5, 0},
    {"/gilles vanwalleghem/", 160, 0, 2, 3, 5, 0},
    {"", 160, 119, 2, 102, 0, 0},
    {"/stephane bellanger/", 160, 0, 2, 0, 0, 0},
    {"/didier lord/", 160, 0, 2, 0, 0, 0},
    {"", 160, 111, 2, 0, 0, 0},
    {"/olivier amiaud/", 160, 0, 2, 0, 0, 0},
    {"/sylvain brunet/", 160, 0, 2, 0, 0, 0},
    {"/nathalie drouet/", 160, 0, 2, 0, 0, 0},
    {"/dominique dumont/", 160, 0, 2, 0, 0, 0},
    {"/kamel feunas/", 160, 0, 2, 0, 0, 0},
    {"/remy gazel/", 160, 0, 2, 0, 0, 0},
    {"/didier leglise/", 160, 0, 2, 0, 0, 0},
    {"/jean marc litchmann/", 160, 0, 2, 0, 0, 0},
    {"/frederic louvre/", 160, 0, 2, 0, 0, 0},
    {"/guillaume michel/", 160, 0, 2, 0, 0, 0},
    {"/olivier mortier/", 160, 0, 2, 0, 0, 0},
    {"/frederic prados/", 160, 0, 2, 0, 0, 0},
    {"/stephane ronse/", 160, 0, 2, 0, 0, 0},
    {"/rene de wael/", 160, 0, 2, 3, 0, 0},
    {"", 160, 120, 2, 102, 1, 0},
    {"/emmanuelle cosso/", 160, 0, 2, 0, 1, 0},
    {"", 160, 111, 2, 0, 1, 0},
    {"/mitsuyuki bando/", 160, 0, 2, 0, 1, 0},
    {"/chris benard/", 160, 0, 2, 0, 1, 0},
    {"/patrice dozier/", 160, 0, 2, 0, 1, 0},
    {"/douglas rand/", 160, 0, 2, 0, 1, 0},
    {"/steven perkinson/", 160, 0, 2, 0, 1, 0},
    {"/christian stonner/", 160, 0, 2, 3, 1, 0},
    {"", 160, 121, 2, 102, 1, 0},
    {"/serge hascoet/", 160, 0, 2, 0, 1, 0},
    {"", 160, 111, 2, 0, 1, 0},
    {"/bruno barone/", 160, 0, 2, 0, 1, 0},
    {"/thomas belmont/", 160, 0, 2, 0, 1, 0},
    {"/nicolas bocquillon/", 160, 0, 2, 0, 1, 0},
    {"/bruno bouvret/", 160, 0, 2, 0, 1, 0},
    {"/christophe cavelan/", 160, 0, 2, 0, 1, 0},
    {"/gunther galipot/", 160, 0, 2, 0, 1, 0},
    {"/sacha gentilhomme/", 160, 0, 2, 0, 1, 0},
    {"/marc gioan/", 160, 0, 2, 0, 1, 0},
    {"/michael guez/", 160, 0, 2, 0, 1, 0},
    {"/benoit macon/", 160, 0, 2, 0, 1, 0},
    {"/lionel rico/", 160, 0, 2, 0, 1, 0},
    {"/emmanuel texandier/", 160, 0, 2, 0, 1, 0},
    {"/christophe thibaut/", 160, 0, 2, 0, 1, 0},
    {"/dorian thibaut/", 160, 0, 2, 3, 1, 0},
    {"", 160, 122, 2, 102, 2, 0},
    {"/agnes haegel/", 160, 0, 2, 3, 2, 0},
    {"", 160, 123, 2, 102, 5, 0},
    {"/ludimedia : gerard guillemot/", 160, 0, 2, 2, 5, 0},
    {"", 160, 124, 2, 102, 5, 0},
    {"/ubi studios : michel guillemot/", 160, 0, 2, 2, 5, 0},
    {"", 160, 125, 2, 102, 5, 0},
    {"/ubi soft :  yves guillemot/", 160, 0, 2, 4, 5, 0},
    {"", 160, 126, 2, 0, 1, 0},
    {"", 160, 127, 2, 6, 1, 0},
    {"", 160, 128, 1, 0, 1, 0},
    {"rayman", 200, 0, 2, 255, 2, 0},
};

/* data_5828, 2:05828. */
/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

u8 DemoRecord[1500] = {
        0, 41, 32, 8, 0, 19, 32, 27, 0, 30, 4, 2, 0, 60, 32, 11, 0, 16, 4, 12, 0, 69, 4, 7, 36, 20, 32, 27, 0, 74,
        128, 42, 160, 4, 32, 8, 36, 11, 32, 124, 36, 19, 32, 58, 0, 28, 16, 46, 0, 13, 128, 23, 144, 1, 16, 23, 144,
        1, 128, 31, 144, 2, 16, 52, 0, 33, 128, 3, 144, 1, 16, 23, 144, 1, 128, 27, 144, 3, 16, 74, 0, 68, 128, 31,
        144, 1, 16, 4, 20, 12, 16, 14, 0, 8, 160, 29, 128, 2, 144, 1, 128, 202, 144, 13, 16, 35, 0, 17, 32, 36, 0,
        19, 32, 13, 160, 1, 128, 22, 160, 25, 32, 38, 36, 14, 32, 29, 0, 16, 128, 25, 144, 17, 128, 1, 144, 1, 128,
        12, 160, 48, 32, 35, 36, 4, 32, 117, 36, 9, 32, 15, 0, 85, 16, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, 32, 12, 36, 19, 4, 2, 20, 1, 16, 9, 20, 1, 4, 12, 0, 26, 4, 5, 0,
        43, 16, 7, 0, 7, 32, 7, 0, 2, 4, 3, 0, 2, 16, 1, 80, 1, 16, 6, 0, 7, 32, 8, 0, 75, 4, 5, 0, 63, 16, 6, 0,
        14, 32, 9, 0, 9, 4, 6, 0, 12, 32, 55, 0, 5, 16, 2, 80, 28, 16, 1, 4, 5, 0, 20, 16, 7, 0, 8, 32, 5, 0, 56, 4,
        4, 0, 14, 16, 8, 0, 34, 32, 6, 0, 25, 84, 4, 80, 23, 16, 21, 0, 8, 32, 22, 0, 15, 32, 11, 0, 1, 4, 6, 0,
        13, 16, 6, 0, 82, 4, 4, 0, 88, 32, 4, 36, 4, 32, 44, 0, 10, 16, 2, 80, 23, 16, 2, 0, 13, 4, 4, 0, 10, 16,
        3, 0, 10, 32, 4, 0, 16, 16, 5, 0, 52, 4, 4, 0, 51, 16, 4, 0, 13, 32, 3, 0, 27, 4, 5, 0, 71, 4, 4, 0, 81, 4,
        4, 0, 33, 16, 12, 0, 19, 32, 6, 0, 5, 4, 7, 0, 66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 29, 32, 50, 36, 8, 32, 13, 33, 5, 32, 8, 0, 12, 32, 1, 36, 8, 32, 7, 0, 9, 32, 6, 36, 5, 32,
        54, 36, 2, 4, 10, 0, 7, 32, 3, 0, 17, 32, 3, 34, 3, 38, 1, 102, 6, 98, 1, 34, 32, 2, 20, 34, 4, 38, 8, 34,
        15, 38, 10, 34, 22, 0, 24, 32, 8, 0, 71, 32, 5, 36, 5, 32, 27, 0, 5, 4, 6, 0, 4, 4, 7, 0, 13, 34, 137, 38,
        13, 34, 21, 2, 2, 0, 28, 32, 8, 36, 7, 32, 25, 0, 12, 32, 3, 36, 3, 32, 2, 0, 12, 32, 23, 36, 8, 32, 7, 36,
        1, 4, 6, 0, 22, 32, 9, 36, 11, 32, 34, 36, 3, 32, 21, 36, 10, 32, 16, 0, 27, 32, 8, 36, 8, 32, 12, 36, 13,
        32, 75, 0, 70, 2, 39, 34, 5, 38, 6, 34, 18, 38, 14, 34, 43, 2, 1, 0, 16, 32, 6, 0, 45, 4, 18, 0, 9, 4, 18,
        0, 21, 1, 87, 0, 22, 4, 11, 0, 14, 4, 35, 0, 43, 1, 34, 0, 27, 4, 13, 0, 26, 4, 19, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 40, 32, 74, 0, 3, 32, 37, 36, 12, 32, 7, 0, 21, 1, 9, 0, 14, 128, 29, 0, 30, 32, 10,
        36, 26, 32, 11, 0, 34, 32, 16, 36, 9, 32, 19, 0, 16, 32, 16, 0, 4, 1, 61, 0, 28, 128, 53, 0, 12, 1, 32, 129,
        3, 128, 47, 0, 3, 1, 27, 129, 4, 128, 52, 0, 15, 32, 9, 36, 8, 32, 23, 0, 32, 32, 7, 0, 9, 4, 20, 0, 17,
        1, 7, 0, 17, 128, 36, 0, 29, 32, 1, 36, 14, 32, 21, 0, 31, 32, 2, 36, 30, 32, 9, 0, 15, 16, 8, 0, 4, 1,
        41, 0, 44, 16, 14, 20, 7, 16, 44, 0, 18, 4, 31, 36, 7, 32, 34, 0, 6, 32, 2, 36, 5, 32, 15, 0, 46, 16, 24,
        0, 12, 32, 6, 0, 33, 32, 5, 36, 5, 32, 20, 36, 14, 32, 16, 0, 52, 32, 16, 0, 14, 128, 15, 160, 19, 164, 255,
        164, 34, 160, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 188, 32, 16, 0, 176, 2, 14, 0, 42, 4, 23, 0, 21, 4, 2, 36, 17, 32,
        2, 0, 29, 4, 1, 36, 8, 32, 2, 0, 37, 32, 13, 0, 10, 2, 11, 0, 118, 4, 15, 0, 43, 4, 10, 36, 4, 32, 2, 0,
        43, 16, 20, 20, 19, 16, 39, 20, 7, 16, 31, 0, 21, 2, 14, 0, 41, 4, 24, 0, 19, 32, 5, 0, 88, 4, 18, 36, 12,
        32, 1, 0, 102, 4, 5, 36, 7, 32, 11, 0, 6, 32, 1, 0, 26, 2, 10, 0, 52, 4, 12, 0, 25, 16, 4, 0, 23, 4, 2,
        20, 2, 4, 6, 0, 97, 4, 2, 20, 2, 84, 10, 20, 1, 16, 22, 0, 8, 1, 5, 0, 27, 16, 69, 0, 7, 32, 52, 0, 10,
        2, 10, 0, 46, 4, 16, 0, 16, 32, 9, 0, 10, 4, 6, 36, 14, 32, 5, 0, 4, 16, 8, 0, 2, 1, 6, 0, 165, 32, 33,
        0, 5, 32, 20, 0, 12, 2, 8, 0, 38, 4, 17, 0, 25, 16, 4, 20, 5, 4, 2, 0, 32, 20, 9, 16, 8, 0, 30, 16, 74,
        0, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 84, 16, 37, 0, 26, 16, 3, 0, 10, 32, 6, 0, 58, 16, 13,
        0, 9, 32, 3, 0, 255, 0, 11, 32, 3, 36, 17, 32, 19, 0, 23, 16, 7, 80, 2, 84, 31, 80, 17, 16, 1, 0, 17, 32,
        3, 0, 139, 32, 4, 36, 15, 32, 19, 0, 45, 16, 3, 80, 3, 84, 32, 80, 9, 16, 4, 0, 49, 32, 2, 0, 115, 4, 24,
        0, 53, 4, 24, 0, 74, 32, 8, 36, 30, 32, 11, 36, 18, 32, 3, 0, 25, 32, 39, 0, 68, 4, 14, 0, 7, 32, 5, 0,
        80, 4, 19, 20, 9, 4, 1, 0, 32, 16, 4, 0, 17, 32, 4, 0, 34, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
};

s16 DemoRecordSize[6] = {154, 192, 200, 172, 200, 120}; //95E04
s16 DemoRecordWorld[6] = {2, 3, 4, 5, 1, 5}; //95E10
s16 DemoRecordMap[6] = {7, 7, 1, 4, 9, 10}; //95E1C

/* data_5e28, 2:05e28. */
/* GOG v1.21 _DATA 2:05e28. Typed reconstruction; see NOTES.md.
 * Block split and dt_str_* names INVENTED. No literal pointer addresses. */
#include "rtypes.h"


/* Physical VGA aperture; oracle contains 0x000a0000 with NO fixup. */
u8 *DisplayBufferNormal = (u8 *)0x000a0000UL;

/* data_5e2c, 2:05e2c. */
/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

enum {
    MODE_TEXT = 255,
    SC_RIGHT = 0x4D, SC_UP = 0x48, SC_LEFT = 0x4B, SC_DOWN = 0x50,
    SC_ALT = 0x38, SC_CONTROL = 0x1D, SC_X = 0x2D
};

u8 ModeVideoActuel = MODE_TEXT; //95E2C
u8 GameModeVideo = MODE_TEXT; //95E2D
s32 JumAmpDefMul = 0; //95E30
s32 JumAmpDefPlus = 0; //95E34
u8 key_right = SC_RIGHT; //95E38
u8 key_up = SC_UP; //95E39
u8 key_left = SC_LEFT; //95E3A
u8 key_down = SC_DOWN; //95E3B
u8 key_fist = SC_ALT; //95E3C
u8 key_jump = SC_CONTROL; //95E3D
u8 key_action = SC_X; //95E3E
u8* tab_key[7] = {&key_left, &key_up, &key_right, &key_down, &key_jump, &key_fist, &key_action}; //95E40
u8 tab_key_sav[7] = {SC_LEFT, SC_UP, SC_RIGHT, SC_DOWN, SC_CONTROL, SC_ALT, SC_X}; //95E5C
u8 OptionGame = 0; //95E63
u8 GoMenu = 0; //95E64
u16 fixon = 1; //95E66      // scores enabled
u16 fixontemp = 0; //95E68
u16 SizeScreen = 0; //95E6A
u8 Frequence = 0; //95E6C
u8 BackgroundOptionOn = 1; //95E6D
u8 BackgroundOn = 1; //95E6E

u16 tRefRam2VramNormalFix[8] = {255, 255, 255, 255, 255, 255, 255, 255}; //95E70
u16 tRefRam2VramNormal[8] = {255, 255, 255, 255, 255, 255, 255, 255}; //95E80
u16 tRefTransFondNormal[8] = {255, 255, 255, 255, 255, 255, 255, 255}; //95E90
u16 tRefSpriteNormal[2] = {255, 255}; //95EA0
u16 tRefRam2VramX[2] = {255, 255}; //95EA4
u16 tRefVram2VramX[2] = {255, 255}; //95EA8
u16 tRefSpriteX[2] = {255, 255}; //95EAC
u8 ExitMenu = 0; //95EB0
u8 ReInitPlasma = 0; //95EB1
u8 Main_Control = 0; //95EB2
s16 During_The_Menu = 1; //95EB4
u8 OptionMusicCdActive = 1; //95EB6
u8 text_input_buffer[9] = {0}; //95EB7
u8 PC_TingsToGet_Col = 1; //95EC0
u8 PC_TingsToGet_ColAdd = 1; //95EC1
