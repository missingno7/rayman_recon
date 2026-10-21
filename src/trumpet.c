#include "inline_helpers.h"
/*
 * Trumpet enemy (GOG v1.21 object 1:6e420-1:6e760). File name invented (Rayverse trumpet.c). Names: Rayverse/Android.
 * Status per function: config/units.json.
 */
#include "obj.h"
#include "inline_helpers.h"

extern obj_t ray;
extern obj_t *poing_obj;
extern s16 ray_zdc_x;
extern s16 ray_zdc_y;
extern s16 ray_zdc_w;
extern s16 ray_zdc_h;
extern s16 ray_wind_force;
extern s16 decalage_en_cours;

void set_main_and_sub_etat(obj_t *obj, u8 main_etat, u8 sub_etat);
void SET_X_SPEED(obj_t *obj);
void CALC_MOV_ON_BLOC(obj_t *obj);
void GET_ANIM_POS(obj_t *obj, s16 *x, s16 *y, s16 *w, s16 *h);
s16 inter_box(s16 x_1, s16 y_1, s16 w_1, s16 h_1,
              s16 x_2, s16 y_2, s16 w_2, s16 h_2);
void skipToLabel(obj_t *obj, u8 label, u8 skip_label_cmd);
void recale_position(obj_t *obj);
void obj_hurt(obj_t *obj);
void DO_TRP_ATTAK(obj_t *obj);

#define GO_LEFT 0
#define GO_RIGHT 1
#define GO_WAIT 2
#define GO_UP 3
#define GO_DOWN 4
#define GO_SUBSTATE 5
#define GO_SKIP 6
#define GO_ADD 7
#define GO_STATE 8
#define GO_PREPARELOOP 9
#define GO_DOLOOP 10
#define GO_LABEL 11
#define GO_GOTO 12
#define GO_GOSUB 13
#define GO_RETURN 14
#define GO_BRANCHTRUE 15
#define GO_BRANCHFALSE 16
#define GO_TEST 17
#define GO_SETTEST 18
#define GO_WAITSTATE 19
#define GO_SPEED 20
#define GO_X 21
#define GO_Y 22
#define RESERVED_GO_SKIP 23
#define RESERVED_GO_GOTO 24
#define RESERVED_GO_GOSUB 25
#define RESERVED_GO_GOTOT 26
#define RESERVED_GO_GOTOF 27
#define RESERVED_GO_SKIPT 28
#define RESERVED_GO_SKIPF 29
#define GO_NOP 30


void DO_TRP_COMMAND(obj_t *obj)
{
    if (obj->main_etat == 0 && obj->sub_etat == 1) {
        obj->speed_x = 0;
        obj->speed_y = -8;
    } else {
        switch (obj->cmd) {
        case GO_SPEED:
            obj->speed_x = obj->iframes_timer;
            obj->speed_y = obj->follow_id;
            break;
        case GO_LEFT:
        case GO_RIGHT:
            if (obj->cmd == GO_LEFT)
                obj->flags.flip_x = 0;
            else
                obj->flags.flip_x = 1;

            if ((obj->main_etat == 0 && obj->sub_etat == 0) ||
                (obj->main_etat == 1 && obj->sub_etat == 0)) {
                set_main_and_sub_etat(obj, 1, 0);
                SET_X_SPEED(obj);
                CALC_MOV_ON_BLOC(obj);
            }
            break;
        case GO_WAIT:
            CALC_MOV_ON_BLOC(obj);
            break;
        /* DOS 1:6e44f..6e457: keep the reference-source upper empty cases. */
        case GO_X:
        case GO_Y:
        case RESERVED_GO_SKIP:
        case RESERVED_GO_GOTO:
        case RESERVED_GO_GOSUB:
        case RESERVED_GO_GOTOT:
        case RESERVED_GO_GOTOF:
        case RESERVED_GO_SKIPT:
        case RESERVED_GO_SKIPF:
        case GO_NOP:
        default:
            break;
        }
    }
    DO_TRP_ATTAK(obj);
}

void trompetteWind(obj_t *obj, u8 a2)
{
    s16 trp_x;
    s16 trp_y;
    s16 trp_w;
    s16 trp_h;
    s16 unk_1;

    obj->timer = 100;
    GET_ANIM_POS(obj, &trp_x, &trp_y, &trp_w, &trp_h);
    if (obj->flags.flip_x)
        trp_x += trp_w - 16;
    else
        trp_x -= 76 + trp_w;

    trp_w += 92;
    trp_h = ashr16(trp_h, 1) + 2;
    trp_y += ashr16(trp_h, 2);
    if (inter_box(ray_zdc_x, ray_zdc_h, ray_zdc_w, ray_zdc_y,
                  trp_x, trp_y, trp_w, trp_h)) {
        ashr16(ray_zdc_w, 1);
        ashr16(ray_zdc_y, 1);
        unk_1 = 7;
        if ((!obj->flags.flip_x && a2 == 0) ||
            (obj->flags.flip_x && a2 == 1))
            unk_1 = -unk_1;
    } else {
        unk_1 = 0;
    }

    if (unk_1 != 0) {
        ray_wind_force += unk_1;
        if (ray.main_etat != 5)
            decalage_en_cours = ashl16(ray_wind_force, 8);
    }
}

void DO_TRP_ATTAK(obj_t *obj)
{

    if (obj->timer != 0)
        obj->timer--;

    if (obj->main_etat == 0) {
        if ((obj->sub_etat == 2 && obj->anim_frame >= 10) || obj->sub_etat == 4)
            trompetteWind(obj, 0);
        else if ((obj->sub_etat == 6 && obj->anim_frame >= 10) || obj->sub_etat == 7)
            trompetteWind(obj, 1);
    }
}

void trompetteAtter(obj_t *obj)
{
    skipToLabel(obj, 8, 1);
    recale_position(obj);
}

void DoTrumpetPoingCollision(obj_t *obj, s16 sprite)
{
    obj_hurt(obj);
    if (poing_obj->speed_x > 0)
        obj->flags.flip_x = 0;
    else if (poing_obj->speed_x < 0)
        obj->flags.flip_x = 1;

    if (obj->hit_points != 0) {
        obj->y -= 2;
        skipToLabel(obj, 4, 1);
    } else {
        set_main_and_sub_etat(obj, 0, 1);
        obj->flags.read_commands = 0;
        obj->cmd = GO_WAIT;
    }
}

void DoTrompetteRaymanZDD(obj_t *obj)
{
    if (obj->timer == 0 &&
        ((obj->main_etat == 0 && obj->sub_etat == 0) ||
         (obj->main_etat == 1 && obj->sub_etat == 0))) {
        skipToLabel(obj, 5, 1);
    }
}
