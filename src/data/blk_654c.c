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

s32 prev_Bloc_lim_W1 = 0; //9654C

//96550
archive_header_t snd8b_header_infos[7] = {
        {0, 2048, 77, 195},
        {2048, 2048, 217, 193},
        {4096, 2048, 36, 142},
        {6144, 2048, 250, 22},
        {8192, 2048, 103, 73},
        {10240, 2048, 171, 183},
        {12288, 2048, 99, 222},
};

//965A4
archive_header_t snd8b_data_infos[7] = {
        {0, 119104, 192, 60},
        {119104, 139852, 148, 104},
        {258956, 290280, 41, 149},
        {549236, 202780, 237, 23},
        {752016, 200344, 36, 180},
        {952360, 193264, 243, 59},
        {1145624, 185736, 248, 51},
};

//965F8
archive_header_t sndvig_infos[9] = {
        {0, 83700, 77, 96},
        {83700, 134576, 217, 38},
        {218276, 118290, 36, 67},
        {336566, 83104, 250, 122},
        {419670, 35392, 103, 177},
        {455062, 125648, 171, 164},
        {580710, 98378, 99, 219},
        {679088, 25472, 71, 84},
        {704560, 68350, 135, 79},
};
