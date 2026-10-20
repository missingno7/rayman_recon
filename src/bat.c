/*
 * Bat enemy (GOG v1.21 object 1:0fa40-1:100b0). File name invented (Rayverse bat.c). Names: Rayverse/Android.
 * obj_t/eta_t are partial layouts (padding for unknown fields) until a shared, verified header exists.
 * Status per function: config/units.json (DO_BAT_FLASH, bat_dir, DO_BAT_COMMAND not yet STRICT).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed long s32;

typedef struct eta_t {
    s8 speed_x_right;
    s8 speed_x_left;
    u8 anim_index;
    u8 next_main_etat;
    u8 next_sub_etat;
    u8 anim_speed;
    u8 sound_index;
    u8 flags;
} eta_t;

typedef struct obj_flags_t {
    unsigned char hurtbyfist : 1;
    unsigned char command_test : 1;
    unsigned char alive : 1;
    unsigned char flip_x : 1;
    unsigned char read_commands : 1;
    unsigned char follow_enabled : 1;
    unsigned char flag_0x40 : 1;
    unsigned char anim_changed : 1;
} obj_flags_t;

typedef struct obj_t {
    u8 _00[0x0c];
    eta_t **eta;
    u8 _10[0x18];
    s32 x;
    s32 y;
    u8 _30[0x10];
    s16 speed_x;
    s16 speed_y;
    u8 _44[6];
    s16 follow_id;
    u8 _4c[8];
    s16 ray_dist;
    s16 iframes_timer;
    u8 _58[0x10];
    u8 offset_bx;
    u8 offset_by;
    u8 anim_index;
    u8 anim_frame;
    u8 sub_etat;
    u8 main_etat;
    u8 init_sub_etat;
    u8 init_main_etat;
    u8 cmd;
    u8 gravity_value_1;
    u8 gravity_value_2;
    u8 change_anim_mode;
    s8 offset_hy;
    u8 follow_sprite;
    u8 hit_points;
    u8 init_hit_points;
    u8 init_flag;
    u8 hit_sprite;
    u8 detect_zone;
    u8 detect_zone_flag;
    s8 cmd_context_depth;
    u8 config;
    u8 display_prio;
    u8 timer;
    u8 anim_count;
    obj_flags_t flags;
} obj_t;

extern obj_t ray;
extern u8 horloge[25];
extern s16 bat_nb_frap;
extern u8 N_anim[256];
extern s16 ray_zdc_x;
extern s16 ray_zdc_w;
extern s16 ray_zdc_h;
extern s16 ray_zdc_y;
extern u8 byte_CFA14;

void DO_REDEYE_FIRE(s16 x, s16 y, s16 direction);
u8 GET_SPRITE_POS(obj_t *obj, s16 index, s16 *x, s16 *y, s16 *w, s16 *h);
void skipToLabel(obj_t *obj, u8 label, u8 skip_label_cmd);
void set_sub_etat(obj_t *obj, u8 sub_etat);
u8 test_allowed(obj_t *obj, s16 center_x, s16 center_y);
u8 on_block_chdir(obj_t *obj, s16 center_x, s16 center_y);
s16 myRand(s16 max);

#define GO_SPEED 20
#define Abs(value) ((value) >= 0 ? (value) : -(value))
#define ashr16(value, amount) \
    ((value) < 0 ? -((-(value)) >> (amount)) : ((value) >> (amount)))

void DO_BAT_FLASH(s16 in_x, s16 in_y)
{
    s16 fire_par_3;
    s16 unk_1;
    s16 unk_x;
    s16 unk_y;
    s16 diff_x;
    s16 diff_y;
    u8 x_gt_0;
    u8 y_gt_0;
    u8 half_shift;

    unk_x = in_x;
    unk_y = in_y;
    diff_x = ray_zdc_x + (ray_zdc_w >> 1) - unk_x;
    diff_y = ray_zdc_h + (ray_zdc_y >> 1) - unk_y;
    x_gt_0 = diff_x > 0;
    y_gt_0 = diff_y > 0;
    half_shift = 1;

    if (!x_gt_0)
        diff_x = -diff_x;
    if (!y_gt_0)
        diff_y = -diff_y;

    if (diff_x > 200) {
        diff_x = ashr16(diff_x, half_shift);
        diff_y = ashr16(diff_y, half_shift);
    }

    if (diff_y == 0) {
        if (x_gt_0)
            fire_par_3 = 27;
        else if (diff_x != 0)
            fire_par_3 = 9;
        else
            fire_par_3 = -1;
    } else {
        unk_1 = diff_x * 45 / diff_y;
        if (x_gt_0) {
            if (y_gt_0) {
                if (unk_1 >= 256)
                    fire_par_3 = 27;
                else
                    fire_par_3 = 27 + N_anim[unk_1];
            } else {
                if (unk_1 >= 256)
                    fire_par_3 = 0;
                else
                    fire_par_3 = N_anim[unk_1];
            }
        } else if (y_gt_0) {
            if (unk_1 >= 256)
                fire_par_3 = 18;
            else
                fire_par_3 = 18 + N_anim[unk_1];
        } else {
            if (unk_1 >= 256)
                fire_par_3 = 9;
            else
                fire_par_3 = 9 + N_anim[unk_1];
        }
    }

    DO_REDEYE_FIRE(unk_x, unk_y, fire_par_3);
}

void DO_BAT_LEFT_FLASH(obj_t *obj)
{
    s16 spr_x;
    s16 spr_y;
    s16 spr_w;
    s16 spr_h;

    GET_SPRITE_POS(obj, 14, &spr_x, &spr_y, &spr_w, &spr_h);
    DO_BAT_FLASH(spr_x + (spr_w >> 1), spr_y + (spr_h >> 1));
}

void DO_BAT_RIGHT_FLASH(obj_t *obj)
{
    s16 spr_x;
    s16 spr_y;
    s16 spr_w;
    s16 spr_h;

    GET_SPRITE_POS(obj, 14, &spr_x, &spr_y, &spr_w, &spr_h);
    DO_BAT_FLASH(spr_x + (spr_w >> 1), spr_y + (spr_h >> 1));
}

void DO_BAT_LEFT_RIGHT_FLASH(obj_t *obj)
{
    s16 spr_x;
    s16 spr_y;
    s16 spr_w;
    s16 spr_h;

    GET_SPRITE_POS(obj, 15, &spr_x, &spr_y, &spr_w, &spr_h);
    DO_BAT_FLASH(spr_x + (spr_w >> 1), spr_y + (spr_h >> 1));
    GET_SPRITE_POS(obj, 14, &spr_x, &spr_y, &spr_w, &spr_h);
    DO_BAT_FLASH(spr_x + (spr_w >> 1), spr_y + (spr_h >> 1));
}

s16 bat_dir(obj_t *obj)
{
    return (ray.x + ray.offset_bx - obj->x - obj->offset_bx) > 0;
}

void DO_BAT_COMMAND(obj_t *obj)
{
    u8 offs_bx;
    u8 offs_by;
    eta_t *cmd_eta;
    s16 obj_sub_etat;
    u8 horl_0;

    obj_sub_etat = obj->sub_etat;
    cmd_eta = obj->eta[obj->main_etat];
    horl_0 = horloge[cmd_eta[obj->sub_etat].anim_speed & 0x0f] == 0;

    switch (obj_sub_etat) {
    default:
        break;
    case 2:
        if (obj->anim_frame == 14 && horl_0)
            DO_BAT_LEFT_RIGHT_FLASH(obj);
        break;
    case 5:
        if (obj->anim_frame == 22) {
            if (obj->hit_points == 0)
                skipToLabel(obj, 8, 1);
            else if (myRand(1) == 0)
                skipToLabel(obj, 2, 1);
            else
                skipToLabel(obj, 3, 1);
            return;
        }
        break;
    case 3:
    case 4:
        if (obj->anim_frame == 19 && horl_0) {
            bat_nb_frap++;
            if (obj_sub_etat == 4)
                DO_BAT_RIGHT_FLASH(obj);
            if (obj_sub_etat == 3)
                DO_BAT_LEFT_FLASH(obj);
        }
        break;
    case 6:
    case 7:
        if (obj->hit_points == 0)
            skipToLabel(obj, 8, 1);
        if (obj->anim_frame == 20) {
            if (obj->hit_points == 0)
                skipToLabel(obj, 8, 1);
            else {
                switch (bat_nb_frap) {
                case 1:
                    set_sub_etat(obj, 11);
                    break;
                case 3:
                    set_sub_etat(obj, 11);
                    bat_nb_frap = 0;
                    break;
                default:
                    if (obj_sub_etat == 6)
                        skipToLabel(obj, 3, 1);
                    else
                        skipToLabel(obj, 2, 1);
                    break;
                }
            }
        }
        break;
    case 8:
        if (bat_dir(obj))
            skipToLabel(obj, 4, 1);
        else
            skipToLabel(obj, 5, 1);
        break;
    case 11:
        if (bat_dir(obj))
            skipToLabel(obj, 6, 1);
        else
            skipToLabel(obj, 7, 1);
        break;
    case 9:
    case 10:
    case 12:
    case 13:
        if (obj->cmd == GO_SPEED) {
            switch (obj_sub_etat) {
            case 10:
            case 13:
                obj->speed_x = -Abs(obj->iframes_timer);
                break;
            case 12:
            case 9:
                obj->speed_x = Abs(obj->iframes_timer);
                break;
            }
            obj->speed_y = obj->follow_id;
        }

        offs_bx = obj->offset_bx;
        offs_by = obj->offset_by;
        if (test_allowed(obj, offs_bx, offs_by) &&
            on_block_chdir(obj, offs_bx, offs_by)) {
            switch (obj_sub_etat) {
            case 10:
                obj->sub_etat = 9;
                break;
            case 9:
                obj->sub_etat = 10;
                break;
            case 13:
                obj->sub_etat = 12;
                break;
            case 12:
                obj->sub_etat = 13;
                break;
            }
        }
        break;
    }
}

void BAT_ray_in_zone(obj_t *obj)
{
    if (obj->sub_etat == 0 && byte_CFA14 == 0) {
        bat_nb_frap = 0;
        obj->flags.flip_x = 0;
        skipToLabel(obj, 1, 1);
    }
}

void DO_BAT_POING_COLLISION(obj_t *bat_obj, s16 sprite)
{
}

s32 bat_get_eject_sens(obj_t *obj)
{
    ray.iframes_timer = 40;
    return bat_dir(obj) ? 1 : -1;
}
