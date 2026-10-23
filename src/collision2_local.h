#include "type_island_abi.h"
#ifndef COLLISION2_LOCAL_H
#define COLLISION2_LOCAL_H
#include "obj.h"
#include "game.h"
#include "map.h"
#include "rtypes.h"
#include "inline_helpers.h"
#define true 1
#define false 0
#define LEN(x) (sizeof(x) / sizeof((x)[0]))
#define Abs(x) ((x) >= 0 ? (x) : -(x))
extern map_data_t mp;
extern u8 EOA(obj_t *obj);
extern void set_sub_etat(obj_t *obj, u8 sub_etat);
extern s16 get_center_x(obj_t *obj);
extern s16 get_center_y(obj_t *obj);
extern s16 on_block_chdir(obj_t *obj, s16 center_x, s16 center_y);
extern rayevts_t RayEvts;
extern s16 ray_mode;
extern obj_t ray;
extern s16 id_obj_grapped;
extern poing_t poing;
extern obj_t *poing_obj;
#define get_eta(obj) (&(obj)->eta[(obj)->main_etat][(obj)->sub_etat])
typedef struct ray_stack_t {
    s16 x_pos; s16 y_pos; s16 poing_x_pos; s16 poing_y_pos;
    u8 main_etat; u8 sub_etat; u8 anim_index; u8 anim_frame; u8 flip_x;
    u8 poing_anim_index; u8 poing_anim_frame; u8 poing_flip_x; u8 poing_is_active; u8 scale;
} ray_stack_t;
#define ANIMMODE_NONE 0
#define BTYP_WATER 10
#define GO_DOWN 4
#define GO_LEFT 0
#define GO_NOP 30
#define GO_RIGHT 1
#define GO_SPEED 20
#define GO_UP 3
#define GO_WAIT 2
#define MODE_3_MORT_DE_RAYMAN 3
#define MODE_NORMAL 0
#define TYPE_0_BADGUY1 0
#define TYPE_10_FISH 10
#define TYPE_123_BLACKTOON1 123
#define TYPE_145_KILLING_EYES 145
#define TYPE_157_EAU 157
#define TYPE_172_STONEWOMAN 172
#define TYPE_178_HERSE_BAS 178
#define TYPE_187_MAMA_PIRATE 187
#define TYPE_1_PLATFORM 1
#define TYPE_24_INTERACTPLT 24
#define TYPE_72_PIRATE_NGAWE 72
#define TYPE_77_PIRATE_GUETTEUR 77
#define TYPE_BIG_CLOWN 60
#define TYPE_CHASSEUR1 12
#define TYPE_CHASSEUR2 14
#define TYPE_FALLING_OBJ 8
#define TYPE_FALLING_OBJ2 134
#define TYPE_FALLING_OBJ3 167
#define TYPE_FALLING_YING 177
#define TYPE_FALLING_YING_OUYE 6
#define TYPE_HERSE_HAUT 241
#define TYPE_WAT_CLOWN 61
#define SCREEN_HEIGHT 200
extern block_flags_t block_flags[64];
extern u8 N_anim[256];
extern u8 GameModeVideo;
extern level_t level;
extern s32 Bloc_lim_H2;
extern obj_t* poing_obj;
extern s16* link_init;
extern ray_stack_t rayStack[101];
extern rayevts_t SauveRayEvts;
extern s16 scroll_start_y;
extern s16 scroll_end_y;
extern s16 scroll_start_x;
extern s16 scroll_end_x;
extern s16 num_level;
extern s16 eau_obj_id;
extern s16 ymapmax;
extern s16 num_world;
extern s16 pierreAcorde_obj_id;
extern s16 xmap;
extern s16 ymap;
extern s16 scroll_y;
extern s16 black_fist_obj_id;
extern s16 scroll_x;
extern u8 horloge[25];
extern u8 ray_on_poelle;
extern u8 dead_time;
extern u8 ray_stack_is_full;
extern u8 black_pos_in_stack;
extern u8 ray_pos_in_stack;
#endif
