/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

typedef struct sax_note_entry_t {
    s16 type;
    s16 speed_x;
    s16 speed_y;
    s16 initial_iframes;
    s16 field4;
} sax_note_entry_t;

typedef struct sax_attack_entry_t {
    u8 next_note;
    u8 time;
    s16 end;
} sax_attack_entry_t;

//97F7C
sax_note_entry_t atak[7] = {
        {0, 1, -1, 0, 0},  // 0
        {1, 1, -3, 60, 0}, // 1
        {2, 2, -2, 0, 0},  // 2
        {1, 3, -4, 30, 0}, // 3
        {1, 2, -4, 30, 0}, // 4
        {1, 2, -3, 42, 0}, // 5
        {1, 1, -4, 80, 0}, // 6
};

//97FC2
sax_attack_entry_t SerieDatak[4][11] = {
        {{1, 1, 0}, {3, -1, 0}, {1, 1, 0}, {0, 0, 1}}, // 0
        {{9, 0, 0}, {1, 0, 0}, {3, -1, 0}, {5, 0, 0}, {0, 0, 1}}, // 1
        {{2, 1, 0}, {7, 0, 0}, {2, 0, 0}, {8, 0, 0}, {1, -1, 0}, {0, 0, 1}}, // 2
        {{3, -1, 0}, {4, -1, 0}, {6, 1, 0}, {9, 0, 0}, {2, 0, 0}, {2, 0, 0}, {9, 0, 0}, {0, 0, 0}, {9, 0, 1}}, // 3
};

