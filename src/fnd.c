#include "obj.h"
#include "game.h"
#include "map.h"
/* tools/seed.py compat prelude: PS1 helper macros as defined in rayman-ps1-decomp include/common.h;
   each guarded separately so a unit's own (DOS-verified) definition wins */
#include <string.h>
#include <stdlib.h>
#ifndef FLG
#define FLG(x) (1 << x)
#endif
#ifndef LEN
#define LEN(x) (sizeof(x) / sizeof(x[0]))
#endif
#ifndef MAX_1
#define MAX_1(x, y) ((x) > (y) ? (x) : (y))
#endif
#ifndef MAX_2
#define MAX_2(x, y) if (x < y) x = y;
#endif
#ifndef MAX_3
#define MAX_3(x, y) if (y > x) x = y;
#endif
#ifndef MIN_1
#define MIN_1(x, y) ((x) < (y) ? (x) : (y))
#endif
#ifndef MIN_2
#define MIN_2(x, y) if (x > y) x = y;
#endif
#ifndef MIN_3
#define MIN_3(x, y) if (y < x) x = y;
#endif
#ifndef ABS
#define ABS(x) (x >= 0 ? (x) : -(x))
#endif
#ifndef SGN
#define SGN(x) (x >= 0 ? (x > 0 ? 1 : 0) : -1)
#endif
#ifndef ashl16
#define ashl16(v, b) ((v) << (b))
#endif
#ifndef ashr16
#define ashr16(v, b) ((v) >> (b))
#endif

#define MODE_NORMAL 0
extern u8 *DrawBufferNormal;
extern u8 GameModeVideo;
extern u8 *PLAN0BIT;
extern s8 Plan0NumPcx[10];
extern map_data_t mp;
extern s8 no_fnd;
extern s16 num_level;
extern s16 num_world;
extern u16 plan0_height;
extern s16 plan0_width;
extern u8 sp_y;
extern u8 type_fnd;
extern u8 type_fndscrX;
extern s16 yp0;
extern s16 ymap;
extern s16 ymapmax;
void Init_Bande(u8 fnd, u16 width, u16 height, u8* source_buf, u8* dest_buf);
void LoadPlan0InVignet(s32 resource_id);
void LOAD_FND(void);
void INIT_FND(void);

void LOAD_FND(void) {
    if (Plan0NumPcx[no_fnd] != -1) {
        LoadPlan0InVignet(Plan0NumPcx[no_fnd]);
    }
    if (GameModeVideo == MODE_NORMAL) {
        Init_Bande(no_fnd, plan0_width, plan0_height, PLAN0BIT, DrawBufferNormal);
    }
}

/* W-SWITCH: keep the repeated per-level bodies separate. Grouping their labels
   changes Watcom's global caching/register allocation, even though the emitted
   bodies merge (DOS 1:39098..391c3). */
void INIT_FND(void) {
    sp_y = 0;
    if (16 * mp.height < plan0_height)
        type_fnd = 3;
    else
        type_fnd = 0;
    type_fndscrX = 0;
    switch (num_world) {
        case 1: {
            switch (num_level) {
                /* These empty slots are present in the DOS level jump table at 1:38f60. */
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    break;
                case 6: {
                    type_fndscrX = 1;
                    sp_y = 2;
                    type_fnd = 1;
                } break;
                case 9: {
                    type_fndscrX = 1;
                    sp_y = 2;
                    type_fnd = 1;
                } break;
                case 7: {
                    type_fnd = 3;
                } break;
                case 10: {
                    type_fnd = 3;
                } break;
                case 8:
                    break;
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                    break;
                case 16: {
                    type_fndscrX = 1;
                    type_fnd = 3;
                } break;
                case 22: {
                    type_fndscrX = 1;
                    type_fnd = 3;
                } break;
                case 17:
                case 18:
                case 19:
                case 20:
                case 21:
                    break;
                default: break;
            }
        } break;
        case 2: {
            switch (num_level) {
                /* Dense cases 1..18 are shown by the table at 1:38fb8. */
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                    break;
                case 7:
                    type_fnd = 1;
                    sp_y = 4;
                    break;
                case 8:
                    type_fnd = 1;
                    sp_y = 4;
                    break;
                case 9:
                    type_fnd = 1;
                    sp_y = 4;
                    break;
                case 10:
                    type_fnd = 1;
                    sp_y = 4;
                    break;
                case 11:
                    type_fnd = 1;
                    sp_y = 4;
                    break;
                case 18:
                    type_fnd = 1;
                    sp_y = 4;
                    break;
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:
                case 17:
                    break;
                default: break;
            }
        } break;
        case 3: {
            switch (num_level) {
                /* Dense cases 1..13 are shown by the table at 1:39000. */
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                    break;
                case 7: {
                    type_fnd = 1;
                    sp_y = 4;
                } break;
                case 8:
                case 9:
                case 11:
                case 12:
                case 13:
                    break;
                case 10:
                    /* DOS table slot 1:39024 targets 1:39110, after the
                       type_fndscrX assignment at 1:3910e. */
                    type_fnd = 3;
                    break;
                default: break;
            }
        } break;
        case 4: {
            switch (num_level) {
                /* Dense cases 1..13 are shown by the table at 1:39034. */
                case 1:
                case 2:
                case 3:
                    break;
                case 4: {
                    type_fndscrX = 1;
                    type_fnd = 3;
                } break;
                case 11: {
                    type_fndscrX = 1;
                    type_fnd = 3;
                } break;
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 12:
                case 13:
                    break;
                default: break;
            }
        } break;
        case 5: {
            switch (num_level) {
                /* Dense cases 1..12 are shown by the table at 1:39068. */
                case 1:
                case 2:
                    break;
                case 3: {
                    type_fndscrX = 1;
                    type_fnd = 3;
                } break;
                case 11: {
                    type_fndscrX = 1;
                    type_fnd = 3;
                } break;
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 12:
                    break;
                default: break;
            }
        } break;
        case 6: {
            /* DOS 1:39179..3918f: unsigned lower/range guards for 1..3,
               followed by the level-4 case; no jump table for this world. */
            switch (num_level) {
                case 1:
                case 2:
                case 3:
                    break;
                case 4:
                    type_fndscrX = 1;
                    type_fnd = 3;
                    break;
                default: break;
            }
        } break;
        default:break;
    }
    if (GameModeVideo == MODE_NORMAL) {
        type_fndscrX = 0;
    }
    LOAD_FND();
}

void calc_off_fnd(void)
{
    switch (type_fnd) {
    case 0:
        yp0 = ymap * (plan0_height - 216) / ymapmax;
        break;
    case 1:
        yp0 = (ymap / sp_y) % plan0_height;
        break;
    case 2:
        yp0 = 0;
        break;
    case 3:
        yp0 = plan0_height - 216;
        break;
    default: break;
    }
}
