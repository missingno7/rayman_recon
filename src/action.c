/* Original action object: DOS v1.21 1:08890..08ce0.
 * Names RAYVERSE/ANDROID. Contiguous layout; four functions STRICT.
 * Speed interpolation remains NEAR; ordinary absolute-value expressions feed max32.
 * See NOTES.md for measured results and evidence. */
#include "obj.h"
#include "map.h"
#include "inline_helpers.h"

#define BLOCK_SIZE 16
#define MODE_NORMAL 0

extern s16 bossXToReach, bossYToReach;
extern s32 bossSpeedFactor, alternateBossSpeedFactor;
extern u8 currentBossActionIsOver;
extern u8 horloge[25];
extern block_flags_t block_flags[64];
extern s16 bossScrollStartX, bossScrollEndX, bossScrollStartY, bossScrollEndY;
extern s16 xmapmax, ymapmax;
extern s16 scroll_start_x, scroll_end_x, floorLine;
extern s32 Bloc_lim_H1, Bloc_lim_H2, Bloc_lim_W1;
extern u8 GameModeVideo;

u8 BTYP(s16 tile_x, s16 tile_y);
s16 dist_to_bloc_floor(s16 btyp, s16 x, s32 y);

/* Names and meanings: docs/semantics/action.json. */
void setBossReachingSpeeds(obj_t *obj, u8 horloge_ind, u8 blend_x, u8 blend_y)
{
    s16 gap_x_q4;
    s16 gap_y_q4;
    s32 speed_limit_q4;
    s32 max_gap_q4;
    s32 travel_steps_q4;
    s16 target_speed_x;
    s16 target_speed_y;
    s32 factor;
    s32 diff_y_wide;
    if ((bossXToReach != -32000) && (bossYToReach != -32000))
    {
        factor = alternateBossSpeedFactor;
        speed_limit_q4 = obj->eta[obj->main_etat][obj->sub_etat].speed_x_right * (bossSpeedFactor + factor);
        speed_limit_q4 /= 0x4000;
        gap_x_q4 = (bossXToReach - obj->x) * 16;
        gap_y_q4 = bossYToReach - obj->y; gap_y_q4 *= 16;
        if ((gap_x_q4 != 0 || gap_y_q4 != 0) && speed_limit_q4 != 0)
        {
            diff_y_wide = gap_y_q4;
            max_gap_q4 = max32((s32)gap_x_q4 >= 0 ? (s32)gap_x_q4 : -(s32)gap_x_q4,
                          diff_y_wide < 0 ? -diff_y_wide : diff_y_wide);
            speed_limit_q4 = min32((s16)max_gap_q4, speed_limit_q4);
            travel_steps_q4 = ashl16(max_gap_q4, 4) / speed_limit_q4;
            target_speed_x = ashl16(gap_x_q4, 4) / travel_steps_q4;
            target_speed_y = ashl16(gap_y_q4, 4) / travel_steps_q4;
            if (horloge[horloge_ind] == 0)
            {
                obj->speed_x = (obj->speed_x * (s16)(0xFF - blend_x) + (target_speed_x * blend_x)) / 255;
                if (obj->main_etat == 2)
                {
                    if (diff_y_wide <= speed_limit_q4)
                        blend_y = 0xFF;
                }
                obj->speed_y = (obj->speed_y * (s16)(0xFF - blend_y) + (target_speed_y * blend_y)) / 255;
            }
        }
        else
        {
            obj->speed_x = 0;
            obj->speed_y = 0;
        }
    }
    else
    {
        obj->speed_x = 0;
        obj->speed_y = 0;
    }
}

s16 testActionEnd(obj_t *obj)
{
    s16 res = 0;

    if (bossXToReach == -32000 && bossYToReach == -32000) {
        if (obj->nb_cmd == 0)
            res = 1;
    } else if (obj->x == bossXToReach && obj->y == bossYToReach) {
        res = 1;
    }
    if (res)
        currentBossActionIsOver = 1;
    return res;
}

s16 firstFloorBelow(obj_t* obj) {
    s16 x = obj->offset_bx + obj->x;
    s16 y = obj->offset_by + obj->y;
    u8 btyp = BTYP(x >> 4, y >> 4);

    // Move down until reaching a solid block or too far down
    while (!block_flags[btyp].bits.solid && y <= ymapmax + 200) {
        y += BLOCK_SIZE;
        btyp = BTYP(x >> 4, y >> 4);
    }

    return (y & ~0xf) + dist_to_bloc_floor(btyp, x & 0xf, 0);
}

void adjustBossScrollLocker(void) {

    if (GameModeVideo == MODE_NORMAL) {
        if (bossScrollStartX < -Bloc_lim_W1 + 8) {
            bossScrollStartX = -Bloc_lim_W1 + 8;
        }
        if (bossScrollStartX > xmapmax + 8) {
            bossScrollStartX = xmapmax + 8;
        }
        if (bossScrollEndX < -Bloc_lim_W1 + 8) {
            bossScrollEndX = -Bloc_lim_W1 + 8;
        }
        if (bossScrollEndX > xmapmax + 8) {
            bossScrollEndX = xmapmax + 8;
        }
    } else {
        if (bossScrollStartX < 0) {
            bossScrollStartX = 0;
        }
        if (bossScrollStartX > xmapmax) {
            bossScrollStartX = xmapmax;
        }
        if (bossScrollEndX < 0) {
            bossScrollEndX = 0;
        }
        if (bossScrollEndX > xmapmax) {
            bossScrollEndX = xmapmax;
        }
    }

    if (bossScrollStartY < -Bloc_lim_H1) {
        bossScrollStartY = -Bloc_lim_H1;
    }
    if (bossScrollStartY > ymapmax) {
        bossScrollStartY = ymapmax;
    }
    if (bossScrollEndY < -Bloc_lim_H1) {
        bossScrollEndY = -Bloc_lim_H1;
    }
    if (bossScrollEndY > ymapmax) {
        bossScrollEndY = ymapmax;
    }

}

void setBossScrollLimits(obj_t* obj) {
    bossScrollStartX = scroll_start_x;
    bossScrollEndX = scroll_end_x;
    floorLine = firstFloorBelow(obj);
    bossScrollStartY = floorLine - (s16)(Bloc_lim_H2 - Bloc_lim_H1) - BLOCK_SIZE;
    bossScrollEndY = ymapmax;
    adjustBossScrollLocker();
}



