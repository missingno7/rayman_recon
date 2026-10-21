/* neige.c (1:4b7a0..1:4c4a0: snow/rain effects). Names RAYVERSE; seeded by tools/seed.py, workers tu_moteur + tu_moteur-luna. */
#include "obj.h"
#include "game.h"
#include "map.h"

typedef struct pix_gerbe_item_t {
    s16 x_pos;
    s16 y_pos;
    s16 speed_x;
    s16 speed_y;
    u8 y_accel;
    u8 unk_1;
} pix_gerbe_item_t;
typedef struct pix_gerbe_t {
    pix_gerbe_item_t items[64];
    s16 is_active;
} pix_gerbe_t;
typedef struct obj_procs_t {
    void (*command)(obj_t* obj);
    void (*poing_collision)(obj_t* obj, s16 sprite);
    void (*rayman_collision)(obj_t* obj);
    void (*rayman_in_zone)(obj_t* obj);
    void (*u_turn)(obj_t* obj);
} obj_procs_t;
#define flags0_1_always (1 << 0)
#define flags0_2_balle (1 << 1)
#define flags0_0x20_has_detect_zone (1 << 5)
#define flags0_0x80_boss (1 << 7)
#define flags1_1_keep_linked_objects_active (1 << 8)
#define flags1_0x10_move_x (1 << 12)
#define flags1_0x20_move_y (1 << 13)
#define flags1_0x40_special_platform (1 << 14)
#define flags1_0x80_read_cmd (1 << 15)
#define flags2_1_check_tile_type (1 << 16)
#define flags2_2_fall_in_water (1 << 17)
#define flags2_8_can_jump (1 << 19)
#define flags2_0x10_do_not_check_ray_collision (1 << 20)
#define flags2_0x20_kill_if_outside_active_zone (1 << 21)
#define flags2_0x40_uturn_on_block (1 << 22)
#define flags2_0x80_increase_speed_x (1 << 23)
#define flags3_2_die_in_water (1 << 25)
#define flags3_4_stop_moving_up_when_hit_block (1 << 26)
#define flags3_8_switch_off (1 << 27)
#define flags3_0x20_link_requires_gendoor (1 << 29)
#define flags3_0x40_no_link (1 << 30)

#define flags0_1_always (1 << 0)
#define flags0_2_balle (1 << 1)
#define flags0_0x20_has_detect_zone (1 << 5)
#define flags0_0x80_boss (1 << 7)
#define flags1_1_keep_linked_objects_active (1 << 8)
#define flags1_0x10_move_x (1 << 12)
#define flags1_0x20_move_y (1 << 13)
#define flags1_0x40_special_platform (1 << 14)
#define flags1_0x80_read_cmd (1 << 15)
#define flags2_1_check_tile_type (1 << 16)
#define flags2_2_fall_in_water (1 << 17)
#define flags2_8_can_jump (1 << 19)
#define flags2_0x10_do_not_check_ray_collision (1 << 20)
#define flags2_0x20_kill_if_outside_active_zone (1 << 21)
#define flags2_0x40_uturn_on_block (1 << 22)
#define flags2_0x80_increase_speed_x (1 << 23)
#define flags3_2_die_in_water (1 << 25)
#define flags3_4_stop_moving_up_when_hit_block (1 << 26)
#define flags3_8_switch_off (1 << 27)
#define flags3_0x20_link_requires_gendoor (1 << 29)
#define flags3_0x40_no_link (1 << 30)

typedef struct flocon_t {
    s16 field_0;
    s16 field_2;
    s16 field_4;
    u8 field_6;
    u8 field_7;

} flocon_t;
#define Abs(x) abs(x)
#define MIN(x,y) ((x)<(y)?(x):(y))
#define MAX(x,y) ((x)>(y)?(x):(y))
#define COUNT(x) (sizeof(x)/sizeof((x)[0]))
#define ashr32(v,b) ((v)<0?-((-(v))>>(b)):((v)>>(b)))
#define sgn(x) ((x)<0?-1:((x)>0))
#define get_eta(obj) (&(obj)->eta[(obj)->main_etat][(obj)->sub_etat])
void init_allowed_time(void);
void SET_ACTIVE_FLAG(s16 screen_x, s16 screen_y, obj_t* obj);
s32 DO_PESANTEUR(obj_t* obj);
void DO_ANIM(obj_t* obj);
void DO_STONEBOMB_REBOND(obj_t* obj);
void DO_THROWN_BOMB_REBOND(obj_t* obj, s16 pesanteur, s16 a3);
void DO_FRUIT_REBOND(obj_t* obj, s16 pesanteur, s16 a3);
void OBJ_IN_THE_AIR(obj_t* obj);
void test_fall_in_water(obj_t* obj);
void MOVE_OBJECT(obj_t* obj);
void DO_RAY_IN_ZONE(obj_t* obj);
void DO_ONE_OBJECT(obj_t* obj);
void build_active_table(void);
void DO_CLING_ANIMS(void);
void DO_OBJECTS(void);
void move_up_ray(void);
void recale_ray_pos(void);
void correct_gendoor_link(u8 a1);
void correct_link(void);
void INIT_RAY(u8 new_lvl);
void STOPPE_RAY_EN_XY(void);
void INIT_MOTEUR(u8 new_lvl);
void INIT_MOTEUR_LEVEL(u8 a1);
void restore_gendoor_link(void);
void DO_MOTEUR(void);
void DO_MOTEUR_GELE(void);
void DoFirstFlocons(void);
void init_flocons(void);
void do_flocons(s16 x, s16 y, s16 x_old, s16 y_old);

#include <stdlib.h>
#ifndef LEN
#define LEN(x) (sizeof(x) / sizeof(x[0]))
#endif

#define ACTIVE_ALIVE 0
#define ACTIVE_DEAD 1
#define BLOCK_FULLY_SOLID 0
#define BTYP_NONE 0
#define BTYP_RESSORT 9
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
#define MODE_0_NONE 0
#define MODE_1_RAYMAN 1
#define MODE_2_RAY_ON_MS 2
#define MODE_3_MORT_DE_RAYMAN 3
#define MODE_4_MORT_DE_RAYMAN_ON_MS 4
#define MODE_5_CASSE_BRIQUE 5
#define MODE_RAY_ON_MS 2
#define OBJ0_BOSS 7
#define OBJ0_KEEP_ACTIVE 4
#define TYPE_208_LANDING_SMOKE 208
#define TYPE_23_RAYMAN 23
#define TYPE_64_SCROLL 64
#define TYPE_BATTEUR_FOU 120
#define TYPE_BBL 97
#define TYPE_BLACK_FIST 205
#define TYPE_BLACK_RAY 204
#define TYPE_CORDE 219
#define TYPE_COUTEAU 190
#define TYPE_DARK_SORT 215
#define TYPE_DESTROYING_DOOR 76
#define TYPE_EAU 157
#define TYPE_FEE 153
#define TYPE_FISH 10
#define TYPE_GENERATING_DOOR 164
#define TYPE_NOVA2 143
#define TYPE_PAILLETTE 75
#define TYPE_PIERREACORDE 220
#define TYPE_POING 94
#define TYPE_RAYMAN 23
#define TYPE_SMA_GRAND_LASER 192
#define TYPE_SMA_PETIT_LASER 244
#define TYPE_STONEDOG2 122
#define world_5_cave 5
#define world_6_cake 6
extern s16 DemoRecordMap[6];
extern s16 DemoRecordWorld[6];
extern u8 First_Hist;
extern u8 First_Menu;
extern u8 IsBossThere;
extern s16 LEFT_MAP_BORDER;
extern s16 MapAvecPluieOuNeige;
extern u8 NbTramesEcrase;
extern u8 NewMs;
extern s16 NumDemo;
extern s16 NumScrollObj;
extern u8 RAY_MODE_SPEED;
extern s16 RIGHT_MAP_BORDER;
extern rayevts_t RayEvts;
extern s16 RunTimeDemo;
extern s16 SNSEQ_len[64];
extern s16 SNSEQ_list[256];
extern s16 SNSEQ_list_ptr;
extern s16 SNSEQ_no;
extern s16 SNSEQ_ptr;
extern s16 VENT_X;
extern s16 VENT_Y;
extern u8 You_Win;
extern active_objects_t actobj;
extern s16 bateau_obj_id;
extern block_flags_t block_flags[64];
extern s16 bonus_map;
extern u8 dead_time;
extern s16 decalage_en_cours;
extern s16 departlevel;
extern s16 dhspeed;
extern s16 display_mode;
extern u8 dontdoit;
extern s16 dvspeed;
extern s16 eau_obj_id;
extern s16 expsin[63];
extern s16 expsin2[49];
extern u8 fade;
extern s16 fee_obj_id;
extern u8 fin_continue;
extern u8 fin_de_rayman;
extern u8 first_boss_meet;
extern obj_type_flags_t flags[262];
extern s16 floc_ind[8];
extern u8 gele;
extern u8 gerbe;
extern s16 h_scroll_speed;
extern u8 horloge[25];
extern s32 left_time;
extern level_t level;
extern u8 life_becoz_wiz;
extern s16 *link_init;
extern s32 map_time;
extern map_data_t mp;
extern u8 nb_continue;
extern s16 nb_floc[8];
extern s16 new_level;
extern s16 new_world;
extern s16 num_level;
extern s16 num_world;
extern u8 oldPrio;
extern u16 ot;
extern s16 pierreAcorde_obj_id;
extern poing_t poing;
extern obj_t *poing_obj;
extern obj_t ray;
extern u8 rayOnMsWasThere;
extern s16 ray_mode;
extern s16 rayman_obj_id;
extern obj_t raytmp;
extern record_t record;
extern s16 reduced_rayman_id;
extern obj_t rms;
extern save_state_t save2;
extern s16 screen_trembling;
extern s16 screen_trembling2;
extern s16 screen_trembling3;
extern u8 scrollLocked;
extern s16 scroll_end_x;
extern s16 scroll_end_y;
extern s16 scroll_obj_id[50];
extern s16 scroll_start_x;
extern s16 scroll_start_y;
extern s16 scroll_x;
extern s16 scroll_y;
extern s16 special_ray_mov_win_x_left;
extern s16 special_ray_mov_win_x_right;
extern obj_t *star_ray_der;
extern obj_t *star_ray_dev;
extern status_bar_t status_bar;
extern s16 tot_nb_flocs[8];
extern s16 v_scroll_speed;
extern s16 xmap;
extern s16 xmap_old;
extern s16 xmapmax;
extern s16 ymap;
extern s16 ymap_old;
extern s16 zonediffx[262];
extern s16 zonediffy[262];
void Change_Wait_Anim(void);
void DO_FADE(void);
void DO_MORT_DE_RAY(void);
void DO_NOVA(obj_t* obj);
void DO_PERFECT_BONUS_MAP(void);
void DO_RAYMAN(void);
void DO_RAY_CASSE_BRIQUE(void);
void DO_RAY_ON_MS(void);
void DO_WIZ_AFTER_BONUS_MAP(void);
s16 GET_SPRITE_POS(obj_t* obj, s16 index, s16* x, s16* y, s16* w, s16* h);
void INIT_FADE_IN(void);
void INIT_FADE_OUT(void);
void PlaySnd(s16 snd, s16 obj_id);
void SHOW_COLLISIONS_ZONES(void);
void add_alwobj(obj_t* obj);

void calc_obj_dir(obj_t* obj);
void calc_obj_pos(obj_t* obj);
u8 calc_typ_trav(obj_t* obj, u8 a2);
void do_pix_gerbes(void);
void doneGameSave(void);
s16 downjoy(void);
s16 get_next_bonus_level(u8 lvl);
void init_bonus_perfect(void);
void init_calcbloc_func(void);
s16 leftjoy(void);
short myRand(short max);
void recale_position(obj_t *obj);
s16 rightjoy(void);
void saveGameState(obj_t *save_obj, save_state_t *save_state);
void set_main_and_sub_etat(obj_t* obj, u8 etat, u8 sub_etat);
void set_proj_center(s16 x, s16 y);
void set_sub_etat(obj_t* obj, u8 sub_etat);
void set_zoom_mode(u8 mode);

void skipToLabel(obj_t* obj, u8 label, u8 skip_label_cmd);
void start_cd_gros_rayman(void);
s16 upjoy(void);
s16 x_pos(u8 param_1, u8 nb_couteau);
s16 y_pos(s16 param_1, s16 param_2);
extern s16 allowed_time[192];
#define ANIMMODE_NONE 0
#define ANIMMODE_RESET 2
#define ANIMMODE_RESET_IF_NEW 1
#define TYPE_10_FISH 10
#define TYPE_STONEBOMB2 47
#define TYPE_TAMBOUR1 87
#define TYPE_TAMBOUR2 88
#define eta_flags_0x10_anim_reverse 16
extern u16 ray_old_main_etat;
extern u16 ray_old_sub_etat;
#define TYPE_FALLING_OBJ 8
#define TYPE_GRAINE 38
#define TYPE_STONEBOMB3 69
#define TYPE_FALLING_OBJ2 134
#define TYPE_FALLING_OBJ3 167
#define TYPE_FALLING_YING 177
#define TYPE_FALLING_YING_OUYE 6
#define TYPE_LIDOLPINK 3
#define TYPE_STONEDOG 40
#define TYPE_NOTE1 91
#define TYPE_TROMPETTE 70
#define TYPE_HYBRIDE_MOSAMS 218
extern u8 button_released;
#define TYPE_BLACKTOON1 123
#define TYPE_WASHING_MACHINE 112
#define TYPE_DROP 62
#define TYPE_MITE 100
#define TYPE_MITE2 217
#define TYPE_STONEWOMAN 172
#define TYPE_BADGUY1 0
#define TYPE_TNT_BOMB 119
#define TYPE_BADGUY2 9
#define TYPE_BADGUY3 165
#define TYPE_STONEBOMB 36
#define TYPE_PIRATE_BOMB 78
#define TYPE_MST_SHAKY_FRUIT 53
#define TYPE_MST_FRUIT1 51
#define TYPE_MST_FRUIT2 52
#define TYPE_BLACKTOON_EYES 130
#define TYPE_CAGE 58
#define TYPE_BONNE_NOTE 93
#define TYPE_NOTE0 90
#define TYPE_NOTE2 92
#define TYPE_PIRATE_GUETTEUR 77
#define TYPE_PIRATE_GUETTEUR2 239
#define TYPE_PIRATE_NGAWE 72
#define TYPE_SUPERHELICO 133
#define TYPE_BB1 46
#define TYPE_BB12 198
#define TYPE_BB13 200
#define TYPE_BIG_CLOWN 60
#define TYPE_NOTE3 182
#define TYPE_SPIDER 65
#define TYPE_WAT_CLOWN 61
#define TYPE_BALLE1 13
#define TYPE_BALLE2 15
#define TYPE_BOUT_TOTEM 211
#define TYPE_MAMA_PIRATE 187
#define TYPE_SAXO 74
#define TYPE_SAXO2 180
#define TYPE_SPIDER_PLAFOND 195
#define TYPE_BIG_BOING_PLAT 68
#define TYPE_BNOTE 102
#define TYPE_BOING_PLAT 27
#define TYPE_BULLET 151
#define TYPE_CORDEBAS 222
#define TYPE_DARK 212
#define TYPE_DARK_PHASE2 32
#define TYPE_MARACAS_BAS 139
#define TYPE_MOSKITO 50
#define TYPE_MOSKITO2 227
#define TYPE_MST_SCROLL 147
#define TYPE_PI_BOUM 110
#define TYPE_POI1 103
#define TYPE_POI2 104
#define BTYP_WATER 10
#define SCREEN_WIDTH 320
#define TYPE_52_MST_FRUIT2 52
#define TYPE_94_POING 94
#define TYPE_POI3 132
#define TYPE_RING 73
#define TYPE_SCORPION 150
#define TYPE_SPACE_MAMA 98
#define TYPE_SPACE_MAMA2 213
#define GO_NOP 30
extern obj_procs_t ObjectsFonctions[262];
extern level_obj_t level_obj;
#define TYPE_161_WIZ 161
#define TYPE_164_GENERATING_DOOR 164
#define TYPE_33_DARK2_SORT 33
#define TYPE_83_EXPLOSION 83
extern s16 compteur_attente;
extern u8 dans_la_map_monde;
extern s16 debug_obj_id;
extern s32 debug_obj_xmin;
extern s16 id_Cling_1up;
extern s16 id_Cling_Pow;
extern obj_t *lidol_source_obj;
extern u8 lidol_to_allocate;
extern u8 ray_Suphelico_bis;
extern s16 ray_zdc_h;
#define world_1_jungle 1
extern u8 ModeDemo;
extern u8 fin_dark;
extern u8 in_air_because_hit;
extern u8 no_ray_landing_smoke;
extern s16 pos_stack[10];
extern u8 ray_in_fee_zone;
extern u8 ray_last_ground_btyp;
extern u8 ray_pos_in_stack;
extern u8 ray_se_noie;
extern u8 ray_stack_is_full;
extern s16 remoteRayXToReach;
extern save_state_t save1;
#define MODE_NORMAL 0
#define TYPE_99_RAY_POS 99
extern u8 GameModeVideo;
extern u8 RaymanDansUneMapDuJeu;
extern u8 Vignet_To_Display;
extern finbosslevel_t finBosslevel;
extern s16 joe_exp_probleme;
extern s16 loop_time;
extern pix_gerbe_t pix_gerbe[8];
extern s32 pixels_enfonce;
extern s16 rubis_list_calculated;
extern u8 sko_enfonce_enable;
extern s16 sko_last_action;
extern s16 sko_rayon_on;
extern s16 ymapmax;
extern u8 TextDark2_Affiche;
extern u8 boss_mort;
extern u8 current_pal_id;
extern u8 fin_boss;
extern u8 fin_du_jeu;
extern s16 helico_time;
extern u8 in_pause;
extern s16 jump_time;
extern u8 nb_wiz_collected;
extern u8 pal_id_at_wizard;
extern s16 ray_X_main_pos_init;
extern s16 ray_Y_main_pos_init;
extern s16 ray_wind_force;
extern u8 save_save;
extern s16 weather_wind;
#define MODE_X 1
extern s32 Bloc_lim_H1;
extern s32 Bloc_lim_H1_Aff;
extern s32 Bloc_lim_H2;
extern s32 Bloc_lim_H2_Aff;
extern s32 Bloc_lim_W1;
extern s32 Bloc_lim_W1_Aff;
extern s32 Bloc_lim_W2;
extern s32 Bloc_lim_W2_Aff;
extern u16 SizeScreen;
extern s16 num_level_choice;
extern s16 num_world_choice;
extern u8 type_fndscrX;
extern s16 xmapsave;
extern s16 ymapsave;
#define SCREEN_HEIGHT 200
#define world_3_mountain 3
extern u8 ModeVideoActuel;
extern s16 ProchainEclair;
extern flocon_t *flocon_tab;
extern s16 invpx0[8];
extern s16 invpx320[8];
extern s16 invpy0[8];
extern s16 invpy200[8];
extern s16 numero_palette_special;
u8 EOA(obj_t* obj);
s16 OBJ_IN_ZONE(obj_t* obj);
u8 BTYP(s16 tile_x, s16 tile_y);
void BombExplosion(obj_t* obj);
void CALC_FOLLOW_SPRITE_SPEED(obj_t* obj, anim_t* anim_1, anim_t* anim_2, s16 anim_frame_2);
void Clown_Music_Atter(obj_t* obj);
void DO_BALLE(obj_t* obj);
void DO_BBL_REBOND(obj_t* obj);
void DO_BBMONT2_ATTER(obj_t* obj);
void DO_BBMONT3_ATTER(obj_t* obj);
void DO_BBMONT_ATTER(obj_t *obj);
void DO_COU_ATTER(obj_t* obj);
void DO_EXPLOSE_NOTE1(obj_t* obj);
void DO_FADE_OUT(void);
void DO_LUCIOLE(void);
void DO_MEDAILLON_TOON_GELE(void);
void DO_NOTE_REBOND(obj_t* obj);
void DO_PMA_ATTER(obj_t* obj);
void DO_SAXO2_ATTER(obj_t* obj);
void DO_SAXO_ATTER(obj_t* obj);
void DO_SPECIAL_PLATFORM(obj_t* obj);
void DO_STONE_EXPLOSION(obj_t* obj, s16 sprite);
void DO_TOTBT_REBOND(obj_t *obj);
void FatalError(char* message);
void GET_OBJ_CMD(obj_t *obj);
void INIT_LUCIOLE(void);
void INIT_OBJECTS(u8 a1);
void PC_do_cheats(void);
void RAY_HIT(u8 hurt, obj_t* obj);
void REINIT_OBJECT(obj_t* obj);
void Ray_RayEcrase(void);
void Reset_air_speed(u8 is_rolling_speed);
void SET_DETECT_ZONE_FLAG(obj_t* obj);
void SET_RAY_DIST(obj_t* obj);
void TEST_SIGNPOST(void);
void actualize_palette(u8 new_pal_id);
void allocate_splash(obj_t* obj);
void allocate_toons(obj_t *src_obj, u8 count);
u8 calc_btyp(obj_t* obj);
void calc_left_time(void);
void change_audio_track_moskito_acc(void);
void change_audio_track_moskito_dec(void);
void del_alwobj(s16 obj_index);
s16 get_allowed_time(void);
s16 get_proj_x(s16 scale, s16 a2);
s16 get_proj_y(s16 scale, s16 a2);
void initGameSave(void);
/* This object declares inverse projection returns wide (W-PROTO);
 * both stores consume AX. See init_flocons in NOTES.md. */
s32 inverse_proj_x(s16 scale, s16 a2);
s32 inverse_proj_y(s16 scale, s16 a2);
void ray_jump(void);
void restoreGameState(save_state_t *save_state);
void restore_objects_flags(void);
void set_xymap(void);
void set_xymapini(void);
void special_flags_init(void);
void switchOff(obj_t* obj);
void trompetteAtter(obj_t *obj);

extern rayevts_t SauveRayEvts;
void Ray_RayEcrase(void);
void init_finBossLevel(void);
extern mem_t main_mem_level;
void align_mem_desc(mem_t *mem);
void INIT_HORLOGES(void);
void horloges(u8 ticks);
void fades(void);
s16 frapsol(s16 param_1);
void DO_SCREEN_TREMBLE(void);
void DO_SCREEN_TREMBLE3(void);
void DO_SCREEN_TREMBLE2(void);
void DO_SCROLL(s16* h_speed, s16* v_speed);
void allocateLandingSmoke(obj_t* in_obj);
s32 instantSpeed(s16 speed);
void SET_X_SPEED(obj_t* obj);
void make_active2(obj_t* obj, u8 do_nova);
void make_active(obj_t* obj, u8 do_nova);
u8 in_action_zone(s32 in_x, s32 in_y, obj_t *obj, u8 param_4);
void kill_obj(obj_t *obj);
void freezeAnim(obj_t *obj, u8 param_2);
s16 prof_in_bloc(obj_t* obj);
void do_boing(obj_t *obj, u8 main_etat, u8 sub_etat);
u8 underSlope(obj_t* obj);
void Drop_Atter(obj_t *obj);
void BadGuyAtter(obj_t *obj);
void MiteAtter(obj_t *obj);
void LidolPinkAtter(obj_t *obj);
void stoneDogAtter(obj_t *obj);
void stoneDogBounces(obj_t* obj);
void Spider_Atter(obj_t *obj);
void NormalAtter(obj_t *obj);
void DoRaymanInZDDDefault(obj_t* obj);
void Add_One_RAY_lives(void);
void DO_OBJECTS_ANIMS(void);
void MOVE_OBJECTS(void);
void RECALE_ALL_OBJECTS(void);
s16 RayCoince(s16 dir);
void move_down_ray(void);
void RAY_TO_THE_RIGHT(void);
void RAY_TO_THE_LEFT(void);
void DO_FIXE(void);
void deactivate_ship_links(void);
s32 linkListHoldsAGendoor(obj_t *obj);
void suppressFromLinkList(obj_t* obj);
void INIT_RAY_BEGIN(void);
s32 is_icy_pente(u8 btyp);
void DO_PLACE_RAY(void);
void DO_AUTO_SCROLL(void);
void INIT_MOTEUR_BEGIN(void);
void INIT_MOTEUR_WORLD(void);
void DONE_MOTEUR_LEVEL(void);
void INIT_MOTEUR_DEAD(void);
void INIT_RAY_ON_MS(void);
void DO_MOTEUR2(void);
void RAY_REVERSE_COMMANDS(void);
void RAY_DEMIRAY(void);
void add_one_floc(void);
void add_256_flocs(void);
void sub_one_floc(void);
void set_snow_sequence(s16 param_1, s16 param_2);
void set_SNSEQ_list(s16 param_1);
void DO_SNOW_SEQUENCE(void);
void DoNeigeRaymanCollision(obj_t* obj);


/* Public prototypes retained across the inferred object boundary. */
extern void INIT_HORLOGES(void);
extern void horloges(u8 ticks);
extern void init_allowed_time(void);
extern void fades(void);
extern s16 frapsol(s16 param_1);
extern void DO_SCREEN_TREMBLE(void);
extern void DO_SCREEN_TREMBLE3(void);
extern void DO_SCREEN_TREMBLE2(void);
extern void DO_SCROLL(s16* h_speed, s16* v_speed);
extern void allocateLandingSmoke(obj_t* in_obj);
extern s32 instantSpeed(s16 speed);
extern void SET_X_SPEED(obj_t* obj);
extern void make_active2(obj_t* obj, u8 do_nova);
extern void make_active(obj_t* obj, u8 do_nova);
extern u8 in_action_zone(s32 in_x, s32 in_y, obj_t *obj, u8 param_4);
extern void kill_obj(obj_t *obj);
extern void SET_ACTIVE_FLAG(s16 screen_x, s16 screen_y, obj_t* obj);
extern s32 DO_PESANTEUR(obj_t* obj);
extern void freezeAnim(obj_t *obj, u8 param_2);
extern void DO_ANIM(obj_t* obj);
extern s16 prof_in_bloc(obj_t* obj);
extern void do_boing(obj_t *obj, u8 main_etat, u8 sub_etat);
extern u8 underSlope(obj_t *obj);
extern void DO_STONEBOMB_REBOND(obj_t* obj);
extern void DO_THROWN_BOMB_REBOND(obj_t* obj, s16 pesanteur, s16 a3);
extern void DO_FRUIT_REBOND(obj_t* obj, s16 pesanteur, s16 a3);
extern void Drop_Atter(obj_t *obj);
extern void BadGuyAtter(obj_t *obj);
extern void MiteAtter(obj_t *obj);
extern void LidolPinkAtter(obj_t *obj);
extern void stoneDogAtter(obj_t *obj);
extern void stoneDogBounces(obj_t* obj);
extern void Spider_Atter(obj_t *obj);
extern void NormalAtter(obj_t *obj);
extern void OBJ_IN_THE_AIR(obj_t* obj);
extern void test_fall_in_water(obj_t* obj);
extern void MOVE_OBJECT(obj_t* obj);
extern void DO_RAY_IN_ZONE(obj_t* obj);
extern void DoRaymanInZDDDefault(obj_t* obj);
extern void DO_ONE_OBJECT(obj_t* obj);
extern void build_active_table(void);
extern void Add_One_RAY_lives(void);
extern void DO_CLING_ANIMS(void);
extern void DO_OBJECTS_ANIMS(void);
extern void DO_OBJECTS(void);
extern void MOVE_OBJECTS(void);
extern void RECALE_ALL_OBJECTS(void);
extern s16 RayCoince(s16 dir);
extern void move_up_ray(void);
extern void move_down_ray(void);
extern void recale_ray_pos(void);
extern void RAY_TO_THE_RIGHT(void);
extern void RAY_TO_THE_LEFT(void);
extern void DO_FIXE(void);
extern void deactivate_ship_links(void);
extern s32 linkListHoldsAGendoor(obj_t *obj);
extern void correct_gendoor_link(u8 a1);
extern void suppressFromLinkList(obj_t* obj);
extern void correct_link(void);
extern void INIT_RAY_BEGIN(void);
extern void INIT_RAY(u8 new_lvl);
extern s32 is_icy_pente(u8 btyp);
extern void STOPPE_RAY_EN_XY(void);
extern void DO_PLACE_RAY(void);
extern void DO_AUTO_SCROLL(void);
extern void INIT_MOTEUR(u8 new_lvl);
extern void INIT_MOTEUR_BEGIN(void);
extern void INIT_MOTEUR_WORLD(void);
extern void INIT_MOTEUR_LEVEL(u8 a1);
extern void restore_gendoor_link(void);
extern void DONE_MOTEUR_LEVEL(void);
extern void INIT_MOTEUR_DEAD(void);
extern void INIT_RAY_ON_MS(void);
extern void DO_MOTEUR(void);
extern void DO_MOTEUR2(void);
extern void RAY_REVERSE_COMMANDS(void);
extern void RAY_DEMIRAY(void);
extern void Ray_RayEcrase(void);
extern void DO_MOTEUR_GELE(void);
extern void DoFirstFlocons(void);
extern void add_one_floc(void);
extern void add_256_flocs(void);
extern void sub_one_floc(void);
extern void init_flocons(void);
extern void do_flocons(s16 x, s16 y, s16 x_old, s16 y_old);
extern void set_snow_sequence(s16 param_1, s16 param_2);
extern void set_SNSEQ_list(s16 param_1);
extern void DO_SNOW_SEQUENCE(void);
extern void DoNeigeRaymanCollision(obj_t* obj);


void DoFirstFlocons(void) {
    if (MapAvecPluieOuNeige && save1.is_just_saved && save1.save_obj_id == -1 && !fin_continue) {
    s32 i;

 for (i = 0; i < 100; ++i) {
            do_flocons(xmap + myRand(i), ymap + myRand(i), xmap_old, ymap_old);
            DO_SNOW_SEQUENCE();
            horloges(0);
            xmap_old = xmap;
            ymap_old = ymap;
        }
    }
}

void add_one_floc(void)
{
    s16 unk_1;
    s16 i;
    s16 done;

    unk_1 = myRand(511);
    i = LEN(floc_ind) - 1;
    done = 0;

    while (!done)
    {
        if (floc_ind[i] < unk_1)
        {
            done = 1;
            nb_floc[i]++;
        }
        i--;
    }

    if (nb_floc[i + 1] > tot_nb_flocs[i + 1])
        nb_floc[i + 1]--;
}

void add_256_flocs(void)
{
    /* THEY LIED TO US, IT'S REALLY 251! D: */
    nb_floc[0] = 10;
    nb_floc[1] = 15;
    nb_floc[2] = 25;
    nb_floc[3] = 30;
    nb_floc[4] = 35;
    nb_floc[5] = 40;
    nb_floc[6] = 43;
    nb_floc[7] = 53;
}

void sub_one_floc(void)
{
    s16 unk_1;
    s16 i;
    s16 done;

    unk_1 = myRand(511);
    i = LEN(nb_floc) - 1;
    done = 0;

    do
    {
        if (floc_ind[i] <= unk_1 && nb_floc[i] > 0)
        {
            nb_floc[i]--;
            done = 1;
        }
        i--;
    } while (!done && i >= 0);

    if (i == -1)
    {
        i = LEN(nb_floc) - 1;
        done = 0;
        do
        {
            if (nb_floc[i] > 0)
            {
                nb_floc[i]--;
                done = 1;
            }
            i--;
        } while (!done && i >= 0);
    }
}

void init_flocons(void) {
    s16 flc_i;
    s16 i;
    s16 unk_2;
    s16 j;
    s16 unk_3;
    s16 unk_4;
    s16 unk_5;
    s16 unk_6;

    /* 3B488 8015FC88 -O2 -msoft-float */

 /* param_1 into get_proj_* is sometimes ray.scale, so similar? */

    tot_nb_flocs[0] = 20;
    tot_nb_flocs[1] = 30;
    tot_nb_flocs[2] = 50;
    tot_nb_flocs[3] = 60;
    tot_nb_flocs[4] = 70;
    tot_nb_flocs[5] = 80;
    tot_nb_flocs[6] = 96;
    tot_nb_flocs[7] = 106;
    flc_i = 0;
    i = 0;
    unk_2 = -64;
    while (i < 8)
    {
        nb_floc[i] = 0;
        floc_ind[i] = flc_i;
        {
        /* DOS 4ba3d..4ba43 keeps a word scale snapshot for the retry loop. */
        s16 scale = unk_2;

        invpy0[i] = inverse_proj_y(unk_2, 0);
        invpy200[i] = inverse_proj_y(unk_2, SCREEN_HEIGHT);
        invpx0[i] = inverse_proj_x(unk_2, 0);
        invpx320[i] = inverse_proj_x(unk_2, SCREEN_WIDTH);
        for (j = 0; j < tot_nb_flocs[i]; j++)
        {
            do {
                    unk_3 = myRand(700);
                    unk_4 = myRand(400) - 150;
                    unk_5 = get_proj_x(scale, unk_3);
                    unk_6 = get_proj_y(scale, unk_4);
                
            } while (unk_5 >= 320 || unk_5 <= 0 || unk_6 >= 200 || unk_6 <= 0);
            flocon_tab[flc_i].field_0 = unk_3;
            flocon_tab[flc_i].field_2 = unk_4;
            flocon_tab[flc_i].field_4 = scale;
            flc_i++;
        }
        }
        unk_2 += 32;
        i++;
    }
    VENT_X = 0;
    VENT_Y = 0;
}

void do_flocons(s16 x, s16 y, s16 x_old, s16 y_old) {
    s16 unk_x_1;
    s16 unk_y_1;
    s32 remove_1;
    s32 remove_2;
    s16 unk_x_2;
    s16 unk_y_2;
    s16 i;
    s16 unk_1;
    s16 flc_i_1;
    s16 unk_x_3;
    s16 unk_y_3;
    s16 x_0;
    s16 x_320;
    s16 y_0;
    s16 y_200;
    flocon_t *cur_floc;
    s16 unk_2;
    s16 flc_i_2;
    s16 prev_vy;
    s16 prev_vx;

    /* 3B718 8015FF18 -O2 -msoft-float */

 /* TODO: ??? */

    prev_vy = VENT_Y;
    prev_vx = VENT_X;

    VENT_Y += 4;
    unk_x_1 = VENT_X / 4;
    unk_y_1 = VENT_Y / 4;
    remove_1 = VENT_X - (VENT_X / 4 * 4);
    unk_x_2 = remove_1;
    remove_2 = VENT_Y - (VENT_Y / 4 * 4);
    unk_y_2 = remove_2;
    if (Abs(unk_x_2) == 1 && horloge[4] == 0)
        unk_x_1 += sgn(VENT_X);

    if (Abs(unk_x_2) == 2 && horloge[2] == 0)
        unk_x_1 += sgn(VENT_X);

    if (Abs(unk_x_2) == 3 && horloge[4] < 3)
        unk_x_1 += sgn(VENT_X);

    if (Abs(unk_y_2) == 1 && horloge[4] == 0)
        unk_y_1 += sgn(VENT_Y);

    if (Abs(unk_y_2) == 2 && horloge[2] == 0)
        unk_y_1 += sgn(VENT_Y);

    if (Abs(unk_y_2) == 3 && horloge[4] < 3)
        unk_y_1 += sgn(VENT_Y);

    set_proj_center(SCREEN_WIDTH / 2, 170);
    i = 0;
    unk_1 = -64;
    while (unk_1 < 32 * 6)
    {
        flc_i_1 = floc_ind[i];
        unk_y_3 = y_old - y;
        unk_y_3 += unk_y_1;
        unk_x_3 = x_old - x + unk_x_1;
        y_0 = invpy0[i];
        y_200 = invpy200[i];
        x_0 = invpx0[i];
        x_320 = invpx320[i];

        cur_floc = &flocon_tab[flc_i_1];
        unk_2 = flc_i_1 + nb_floc[i];
        flc_i_2 = flc_i_1;
        while (flc_i_2 < unk_2)
        {
            cur_floc->field_0 += unk_x_3;
            cur_floc->field_2 += unk_y_3;

            if (cur_floc->field_0 > x_320)
                cur_floc->field_0 = x_0;
            else if (cur_floc->field_0 < x_0)
                cur_floc->field_0 = x_320;

            if (cur_floc->field_2 > y_200)
                cur_floc->field_2 = y_0;
            else if (cur_floc->field_2 < y_0)
                cur_floc->field_2 = y_200;

            cur_floc++;
            flc_i_2++;
        }
        unk_1 += 32;
        i++;
    }

    VENT_Y = prev_vy;
    VENT_X = prev_vx;
    if (num_world != 1)
        weather_wind = VENT_X / 8;
}

void set_snow_sequence(s16 param_1, s16 param_2)
{
    SNSEQ_no = param_1;
    SNSEQ_ptr = 0;
    SNSEQ_len[(s16) param_1] = param_2;
}

void set_SNSEQ_list(s16 param_1)
{
    s16 *cur_sn;

    cur_sn = SNSEQ_list;
    MapAvecPluieOuNeige = 1; /* TODO: can't tell if this should be in dec or hex (flags?) */

    if (param_1 == 0)
    {
        *cur_sn++ = 4;
        *cur_sn++ = 2;
        *cur_sn++ = 0x80;
        *cur_sn++ = 0;
        *cur_sn++ = 0x7FFF;
    }
    else if (param_1 == 1)
    {
        *cur_sn++ = 4;
        *cur_sn++ = 3;
        *cur_sn++ = 0x80;
        *cur_sn++ = 0;
        *cur_sn++ = 0x7FFF;
    }
    else if (param_1 == 2)
    {
        *cur_sn++ = 4;
        *cur_sn++ = 3;
        *cur_sn++ = 0x0200;
        *cur_sn++ = 0;
        *cur_sn++ = 0x7FFF;
    }
    else if (param_1 == 3)
    {
        if (VENT_Y != 8)
            *cur_sn++ = 4;
        else
            *cur_sn++ = 2;

        if (VENT_Y > 8)
        {
            *cur_sn++ = 8;
            *cur_sn++ = (VENT_Y * 2) - 0x10;
        }
        else if (VENT_Y < 8)
        {
            *cur_sn++ = 9;
            *cur_sn++ = 0x10 - (VENT_Y * 2);
        }
        *cur_sn++ = 0;
        *cur_sn++ = 0x7FFF;
    }
    else if (param_1 == 4)
    {
        if (VENT_Y != 0)
            *cur_sn++ = 4;
        else
            *cur_sn++ = 2;

        if (VENT_Y > 0)
        {
            *cur_sn++ = 8;
            *cur_sn++ = (VENT_Y * 2);
        }
        else if (VENT_Y < 0)
        {
            *cur_sn++ = 9;
            *cur_sn++ = (-VENT_Y * 2);
        }
        *cur_sn++ = 0;
        *cur_sn++ = 0x7FFF;
    }
    else if (param_1 == 5)
    {
        if (VENT_X != -8)
            *cur_sn++ = 4;
        else
            *cur_sn++ = 2;

        if (VENT_X > -8)
        {
            *cur_sn++ = 6;
            *cur_sn++ = (VENT_X * 2) + 0x10;
        }
        else if (VENT_X < -8)
        {
            *cur_sn++ = 7;
            *cur_sn++ = -0x10 - (VENT_X * 2);
        }
        *cur_sn++ = 0;
        *cur_sn++ = 0x7FFF;
    }
    else if (param_1 == 6)
    {
        if (VENT_X != 8)
            *cur_sn++ = 4;
        else
            *cur_sn++ = 2;

        if (VENT_X > 8)
        {
            *cur_sn++ = 6;
            *cur_sn++ = (VENT_X * 2) - 0x10;
        }
        else if (VENT_X < 8)
        {
            *cur_sn++ = 7;
            *cur_sn++ = 0x10 - (VENT_X * 2);
        }
        *cur_sn++ = 0;
        *cur_sn++ = 0x7FFF;
    }
    else if (param_1 == 7)
    {
        if (VENT_X != -16)
            *cur_sn++ = 4;
        else
            *cur_sn++ = 2;

        if (VENT_X > -16)
        {
            *cur_sn++ = 6;
            *cur_sn++ = (VENT_X * 2) + 0x20;
        }
        else if (VENT_X < -0x10)
        {
            *cur_sn++ = 7;
            *cur_sn++ = -0x20 - (VENT_X * 2);
        }
        *cur_sn++ = 0;
        *cur_sn++ = 0x7FFF;
    }
    else if (param_1 == 8)
    {
        if (VENT_X != 16)
            *cur_sn++ = 4;
        else
            *cur_sn++ = 2;

        if (VENT_X > 16)
        {
            *cur_sn++ = 6;
            *cur_sn++ = (VENT_X * 2) - 0x20;
        }
        else if (VENT_X < 16)
        {
            *cur_sn++ = 7;
            *cur_sn++ = 0x20 - (VENT_X * 2);
        }
        *cur_sn++ = 0;
        *cur_sn++ = 0x7FFF;
    }
    else if (param_1 == 9)
    {
        if (VENT_X != 0)
            *cur_sn++ = 4;
        else
            *cur_sn++ = 2;

        if (VENT_X > 0)
        {
            *cur_sn++ = 6;
            *cur_sn++ = ((u16) VENT_X * 2);
        }
        else if (VENT_X < 0)
        {
            *cur_sn++ = 7;
            *cur_sn++ = (-(u16) VENT_X * 2);
        }
        *cur_sn++ = 0;
        *cur_sn++ = 0x7FFF;
    }
    else if (param_1 == 10)
    {
        if (VENT_Y != -16)
            *cur_sn++ = 6;
        else
            *cur_sn++ = 4;

        if (VENT_Y > -16)
        {
            *cur_sn++ = 8;
            *cur_sn++ = (VENT_Y * 2) + 0x20;
        }
        else if (VENT_Y < -0x10)
        {
            *cur_sn++ = 9;
            *cur_sn++ = -0x20 - (VENT_Y * 2);
        }
        *cur_sn++ = 0;
        *cur_sn++ = 0x0080;
        *cur_sn++ = 9;
        *cur_sn++ = 0x0020;
    }
    else if (param_1 == 11)
    {
        *cur_sn++ = 4;
        *cur_sn++ = 10;
        *cur_sn++ = 1;
        *cur_sn++ = 0;
        *cur_sn++ = 0x7FFF;
    }

    SNSEQ_list_ptr = 1;
    set_snow_sequence(SNSEQ_list[1], SNSEQ_list[2]);
    SNSEQ_list_ptr += 2;
}

void DO_SNOW_SEQUENCE(void)
{
    s16 first;
    s16 second;

    switch (SNSEQ_no)
    {
    case 0:
        break;
    case 1:
        VENT_X = 0;
        VENT_Y = 0;
        break;
    case 2:
        add_one_floc();
        break;
    case 3:
        sub_one_floc();
        break;
    case 6:
        if (horloge[2] == 0)
            VENT_X--;
        break;
    case 4:
        VENT_X--;
        break;
    case 7:
        if (horloge[2] == 0)
            VENT_X++;
        break;
    case 5:
        VENT_X++;
        break;
    case 8:
        if (horloge[2] == 0)
            VENT_Y--;
        break;
    case 9:
        if (horloge[2] == 0)
            VENT_Y++;
        break;
    case 10:
        VENT_X = -8;
        VENT_Y = 8;
        add_256_flocs();
    }

    if (++SNSEQ_ptr == SNSEQ_len[SNSEQ_no])
    {
        first = SNSEQ_list[SNSEQ_list_ptr];
        SNSEQ_list_ptr++;
        second = SNSEQ_list[SNSEQ_list_ptr];
        SNSEQ_list_ptr++;

        set_snow_sequence(first, second);
        if (SNSEQ_list_ptr == SNSEQ_list[0])
            SNSEQ_list_ptr = 0;
    }
}

void DoNeigeRaymanCollision(obj_t* obj) {
    obj->flags.alive = 0;
    switch (obj->sub_etat) {
        case 0x20:  // 32
            set_SNSEQ_list(0);
            break;
        case 0x15:  // 21
            set_SNSEQ_list(1);
            break;
        case 0x16:  // 22
            set_SNSEQ_list(2);
            break;
        case 0x17:  // 23
            set_SNSEQ_list(3);
            break;
        case 0x18:  // 24
            set_SNSEQ_list(4);
            break;
        case 0x19:  // 25
            set_SNSEQ_list(5);
            break;
        case 0x1A:  // 26
            set_SNSEQ_list(6);
            break;
        case 0x1B:  // 27
            set_SNSEQ_list(7);
            break;
        case 0x1C:  // 28
            set_SNSEQ_list(8);
            break;
        case 0x1D:  // 29
            set_SNSEQ_list(9);
            break;
        case 0x1E:  // 30
            set_SNSEQ_list(10);
            break;

        default:
            break;
    }
}
