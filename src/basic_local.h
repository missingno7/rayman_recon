/* Worker-local declarations for basic.c. Macros/constants follow Rayverse; shared
   constants (BTYP_*, GO_*) should move to a common header when other units need them. */
/* Local Rayverse compatibility types/constants for this candidate TU. */
#ifndef BASIC_WORKER_H
#define BASIC_WORKER_H
typedef unsigned char bool;
#ifndef true
#define true 1
#define false 0
#endif
#define ASSERT(expr)
#define COUNT(x) (sizeof(x) / sizeof((x)[0]))
#define Abs(value) ((value) >= 0 ? (value) : -(value))
#define get_eta(obj) (&(obj)->eta[(obj)->main_etat][(obj)->sub_etat])
#define ANIMMODE_RESET_IF_NEW 1
#define BTYP_CHDIR 1
#define BTYP_LIANE 12
#define BTYP_NONE 0
#define BTYP_SLIPPERY 30
#define BTYP_SLIPPERY_LEFT1_30 22
#define BTYP_SLIPPERY_LEFT2_30 23
#define BTYP_SLIPPERY_LEFT_45 19
#define BTYP_SLIPPERY_RIGHT1_30 20
#define BTYP_SLIPPERY_RIGHT2_30 21
#define BTYP_SLIPPERY_RIGHT_45 18
#define BTYP_SOLID 15
#define BTYP_SOLID_LEFT1_30 6
#define BTYP_SOLID_LEFT2_30 7
#define BTYP_SOLID_LEFT_45 3
#define BTYP_SOLID_PASSTHROUGH 14
#define BTYP_SOLID_RIGHT1_30 4
#define BTYP_SOLID_RIGHT2_30 5
#define BTYP_SOLID_RIGHT_45 2
#define GO_LEFT 0
#define GO_RIGHT 1
#define MODE_5_CASSE_BRIQUE 5
#define TYPE_161_WIZ 161
#define TYPE_170_RAYON 170
#define TYPE_23_RAYMAN 23
#define TYPE_58_CAGE 58
#define TYPE_75_PAILLETTE 75
#define TYPE_BADGUY1 0
#define TYPE_BADGUY2 9
#define TYPE_BADGUY3 165
#define TYPE_BIG_CLOWN 60
#define TYPE_BLACKTOON1 123
#define TYPE_CAISSE_CLAIRE 152
#define TYPE_FALLING_YING_OUYE 6
#define TYPE_GENEBADGUY 20
#define TYPE_LIDOLPINK 3
#define TYPE_LIDOLPINK2 144
#define TYPE_MITE 100
#define TYPE_MITE2 217
#define TYPE_MOVE_MARTEAU 106
#define TYPE_MOVE_OUYE 45
#define TYPE_MOVE_PLAT 22
#define TYPE_MOVE_RUBIS 189
#define TYPE_MOVE_START_PLAT 49
#define TYPE_PIRATE_POELLE 174
#define TYPE_PIRATE_POELLE_D 184
#define TYPE_PIRATE_P_45 225
#define TYPE_PIRATE_P_D_45 226
#define TYPE_PT_GRAPPIN 140
#define TYPE_RAYMAN 23
#define TYPE_ROULETTE 138
#define TYPE_ROULETTE2 154
#define TYPE_ROULETTE3 155
#define TYPE_SPIDER 65
#define TYPE_SPIDER_PLAFOND 195
#define TYPE_STONEMAN1 35
#define TYPE_STONEMAN2 43
#define TYPE_TROMPETTE 70
#define TYPE_WALK_NOTE_1 156
#define TYPE_WAT_CLOWN 61
#define eta_flags_0x10_anim_reverse 0x10
#define flags1_0x10_move_x 0x1000
#define flags1_0x20_move_y 0x2000
extern u8 horloge[25];
extern level_t level;
extern u32 saveobj[16];
#include "map.h"
#include "game.h"
extern obj_type_flags_t flags[262];
extern block_flags_t block_flags[64];
extern s16 PROJ_CENTER_X;
extern s16 PROJ_CENTER_Y;
extern u8 zoom_mode;
extern u8 input_mode;
extern s16 num_world;
extern obj_t ray;
extern s16 xmap;
extern s16 ymap;
extern map_data_t mp;
extern rayevts_t RayEvts;
extern u8 ray_last_ground_btyp;
extern level_obj_t level_obj;
extern active_objects_t actobj;
extern finbosslevel_t finBosslevel;
extern u8 first_boss_meet;
extern s16 ray_mode;
extern poing_t poing;
extern font_t alpha_numbers;
extern font_t alpha2;
extern font_t alpha;
extern display_item_t text_to_display[11];

bool EOA(obj_t* obj);
void save_objects_flags(void);
void restore_objects_flags(void);
void snapToSprite(obj_t* obj_1, obj_t* obj_2, u8 obj_2_spr, s32 x_offs, u16 y_offs);
obj_t* findfirstObject(u16 obj_type);
obj_t* findfirstInactiveObject(u16 obj_type);
s16 sinYspeed(obj_t* obj, s16 a2, s16 a3, s16* a4);
void set_proj_center(s16 x, s16 y);
s16 get_proj_dist(s16 scale, s16 outer_dim);
s32 get_proj_dist2(s16 scale, s16 a2);
s16 get_proj_x(s16 scale, s16 a2);
s16 get_proj_y(s16 scale, s16 a2);
void set_zoom_mode(u8 mode);
s16 inverse_proj_x(s16 scale, s16 a2);
s16 inverse_proj_y(s16 scale, s16 a2);
s32 vblToEOA(obj_t* obj, u8 a2);
void GET_ANIM_POS(obj_t* obj, s16* x, s16* y, s16* w, s16* h);
void add_actobj(s16 a1);
void set_sub_etat(obj_t* obj, u8 sub_etat);
void set_main_etat(obj_t* obj, u8 etat);
void set_main_and_sub_etat(obj_t* obj, u8 etat, u8 sub_etat);
s16 get_center_x(obj_t* obj);
s16 get_center_y(obj_t* obj);
s16 on_block_chdir(obj_t* obj, s16 center_x, s16 center_y);
void CALC_FOLLOW_SPRITE_SPEED(obj_t* obj, anim_t* anim_1, anim_t* anim_2, s16 anim_frame_2);
s16 GET_SPRITE_POS(obj_t* obj, s16 index, s16* x, s16* y, s16* w, s16* h);
void GET_RAY_ZDC(obj_t* obj, s16* x, s16* y, s16* w, s16* h);
void GET_BB1_ZDCs(obj_t* obj, s16 *x_1, s16 *y_1, s16 *w_1, s16 *h_1, s16 *x_2, s16 *y_2, s16 *w_2, s16 *h_2);
void calc_obj_dir(obj_t* obj);
s16 OBJ_IN_ZONE(obj_t* obj);
void calc_obj_pos(obj_t* obj);
void makeUturn(obj_t* obj);
u8 BTYP(s16 tile_x, s16 tile_y);
void calc_btyp_square(obj_t* obj);
void DO_OBJ_REBOND_EN_X(obj_t* obj);
u8 calc_btyp(obj_t* obj);
void init_obj_in_the_air(obj_t* obj);
void make_my_fruit_go_down(obj_t* obj, s16 x_accel);
void switchOff(obj_t* obj);
void obj_hurt(obj_t* target);
void Projectil_to_RM(obj_t* obj, s16* out_spd_x, s16* out_spd_y, s16 new_spd_x, s16 new_spd_y);
void del_actobj(obj_t* obj);
s16 calc_let_Width(u8 font_size, u32 num_let);
s16 calc_let_Width2(u8 font_size, u32 num_let);
void INIT_TEXT_TO_DISPLAY(void);
s32 deter_num_let(u8 c, const char* next_chars);
void calc_num_let_spr(u8 a1, u8* a2);
s16 calc_largmax_text(const char* text, s16 char_index, s16 space_width, s16 char_spacing, u8 font_size);
void INIT_TXT_BOX(display_item_t* box);
void Deter_Option_Caract(char* text, s16 key, s16 offset);
void SwapAB(s16* a, s16* b);
void Bresenham(void (*func)(s16, s16), s16 origin_x, s16 origin_y, s16 dest_x, s16 dest_y, s16 param_6, s16 percent);
void init_finBossLevel(void);
void Change_Wait_Anim(void);
void add_alwobj(obj_t* obj);
void del_alwobj(s16 obj_index);
void FatalError(char* message);
u8 sprite_of_ref(obj_t* obj, u8 ref);

void set2bits(u32 *x, u8 index, u32 value);
void read2bits(s32 *value, u8 pos, s32 *b1, s32 *b0);
void InitTextMode(void);
void PrintMemInfo(void);
s16 bloc_floor(s16 btype, s16 x, s16 y);
s16 instantSpeed(s16 speed);
void skipToLabel(obj_t *obj, u8 label, u8 skip_label_cmd);
s16 sinus(s16 x);
void stop_cd(void);
void print_once(const char *message);
#endif

