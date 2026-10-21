/* moteur.c (1:45480..1:4b7a0: engine loop, timers, scroll, object update dispatch). Names RAYVERSE/ANDROID; seeded by tools/seed.py, workers tu_moteur (sol) + tu_moteur-luna. Split at 1:4b7a0 from matching end/start (STRONG). */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "inline_helpers.h"

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
#include "inline_helpers.h"
#define get_eta(obj) (&(obj)->eta[(obj)->main_etat][(obj)->sub_etat])
void init_allowed_time(void);
void SET_ACTIVE_FLAG(s16 screen_x, s16 screen_y, obj_t* obj);
s16 DO_PESANTEUR(obj_t* obj);
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
void INIT_MOTEUR_LEVEL(s32 a1);
void restore_gendoor_link(void);
void DO_MOTEUR(void);
void DO_MOTEUR_GELE(void);
void DoFirstFlocons(void);
void init_flocons(void);
void do_flocons(s16 x, s16 y, s16 x_old, s16 y_old);

#include <string.h>
#include <stdlib.h>
#include <conio.h>
#ifndef MAX_2
#define MAX_2(x, y) if (x < y) x = y;
#endif
#ifndef MIN_2
#define MIN_2(x, y) if (x > y) x = y;
#endif
#ifndef MIN_3
#define MIN_3(x, y) if (y < x) x = y;
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
extern s16 ray_old_main_etat;
extern s16 ray_old_sub_etat;
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
extern s16 ray_zdc_y;
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
s16 inverse_proj_x(s16 scale, s16 a2);
s16 inverse_proj_y(s16 scale, s16 a2);
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
s16 in_action_zone(s16 in_x, s16 in_y, obj_t *obj, u8 param_4);
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
s16 linkListHoldsAGendoor(obj_t *obj);
void suppressFromLinkList(obj_t* obj);
void INIT_RAY_BEGIN(void);
s16 is_icy_pente(u8 btyp);
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
extern s16 in_action_zone(s16 in_x, s16 in_y, obj_t *obj, u8 param_4);
extern void kill_obj(obj_t *obj);
extern void SET_ACTIVE_FLAG(s16 screen_x, s16 screen_y, obj_t* obj);
extern s16 DO_PESANTEUR(obj_t* obj);
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
extern s16 linkListHoldsAGendoor(obj_t *obj);
extern void correct_gendoor_link(u8 a1);
extern void suppressFromLinkList(obj_t* obj);
extern void correct_link(void);
extern void INIT_RAY_BEGIN(void);
extern void INIT_RAY(u8 new_lvl);
extern s16 is_icy_pente(u8 btyp);
extern void STOPPE_RAY_EN_XY(void);
extern void DO_PLACE_RAY(void);
extern void DO_AUTO_SCROLL(void);
extern void INIT_MOTEUR(u8 new_lvl);
extern void INIT_MOTEUR_BEGIN(void);
extern void INIT_MOTEUR_WORLD(void);
extern void INIT_MOTEUR_LEVEL(s32 a1);
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


void INIT_HORLOGES(void) { s32 i; for(i=0;i<25;i++) horloge[i]=0; }

void horloges(u8 ticks)
{
    s32 i;

    for (i = 0; i < 25; i++)
    {
        horloge[i]++;
        if (horloge[i] >= i)
            horloge[i] = 0;
    }
    map_time += ticks;
}

void init_allowed_time(void)
{
    s16 world = 1;
    s16 index;
    s16 *p;
    do {
        index = (world - 1) * 32;
        p = &allowed_time[index];
        switch (world) {
        case 1:
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = 30;
            *p++ = 45;
            *p++ = 35;
            *p++ = 25;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            break;
        case 2:
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = 40;
            *p++ = 25;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            break;
        case 3:
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = 45;
            *p++ = 20;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            break;
        case 4:
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = 35;
            *p++ = 50;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            break;
        case 5:
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = 35;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            break;
        case 6:
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            *p++ = -2;
            break;
        }
        world++;
    } while (world <= 6);
}

void fades(void) {
    if (map_time == 0) {
        INIT_FADE_IN();
    }
    if (inrange(dead_time, 32, display_mode + 33)) {
        if (fade == 65) {
            INIT_FADE_OUT();
        }
    }
    if ((fade & 0x40) == 0) {
        DO_FADE();
    }
}

s16 frapsol(s16 param_1)
{
    return ashr16(expsin[param_1], 7);
}

void DO_SCREEN_TREMBLE(void)
{
    s16 unk_1;

    if (screen_trembling != 0)
    {
        unk_1 = frapsol(screen_trembling);
        screen_trembling++;
        if (unk_1 == 0xFF)
        {
            unk_1 = 0;
            screen_trembling = 0;
        }
        ymap += unk_1;
    }
}

void DO_SCREEN_TREMBLE3(void)
{
    s16 unk_1;

    unk_1 = expsin2[screen_trembling3];

    if (unk_1 != 0xFF)
    {
        screen_trembling3++;
        ymap += unk_1;
    }
    else
        screen_trembling3 = 0;
}

void DO_SCREEN_TREMBLE2(void)
{
    s16 unk_1;

    unk_1 = expsin2[screen_trembling2];

    if (unk_1 != 0xFF)
    {
        screen_trembling2++;
        xmap += unk_1;
    }
    else
        screen_trembling2 = 0;
}

void DO_SCROLL(s16* h_speed, s16* v_speed) {
    if (*v_speed == 255) {
        *v_speed = 0;
    }
    if (num_world == world_6_cake && num_level == 4) {
        *h_speed = 0;
    }
    if (num_world == world_5_cave && num_level == 11) {
        *h_speed = 0;
    }
    if (num_world == world_5_cave && num_level == 3) {
        *h_speed = 0;
        *v_speed = 0;
    }
    DO_SCREEN_TREMBLE(); //TODO
    if (screen_trembling2 > 0) {
        DO_SCREEN_TREMBLE2(); //TODO
    }
    if (screen_trembling3 > 0) {
        DO_SCREEN_TREMBLE3(); //TODO
    }

    ymap += *v_speed;
    xmap += *h_speed;

    if (xmap < scroll_start_x) {
        *h_speed += scroll_start_x - xmap;
        xmap = scroll_start_x;
        dhspeed = 0;
    } else if (xmap > scroll_end_x) {
        *h_speed -= xmap - scroll_end_x;
        xmap = scroll_end_x;
        dhspeed = 0;
    }

    if (ymap < scroll_start_y) {
        *v_speed += scroll_start_y - ymap;
        ymap = scroll_start_y;
        dvspeed = 0;
    } else if (ymap > scroll_end_y) {
        *v_speed -= ymap - scroll_end_y;
        ymap = scroll_end_y;
        dvspeed = 0;
    }

    calc_obj_pos(&ray);
}

void allocateLandingSmoke(obj_t* in_obj) {
    s16 i;
    s16 j;
    obj_t * cur_obj;
    s16 nb_objs;
    s32 pos;
    s32 offset_y;

    /* DOS 1:45df6..45e01: load the y offset before the y coordinate. */

    for (i = 0; i <= 2; i += 2)
    {
        cur_obj = &level.objects[0];
        nb_objs = level.nb_objects;
        j = 0;
        while (j < nb_objs)
        {
            if (cur_obj->type == TYPE_208_LANDING_SMOKE && !cur_obj->is_active) {
                cur_obj->flags.alive = 1;
                cur_obj->is_active = 1;
                add_alwobj(cur_obj);
                cur_obj->active_timer = 0;
                cur_obj->active_flag = ACTIVE_ALIVE;
                set_main_and_sub_etat(cur_obj, 0, i != 0);
                pos = in_obj->x;
                pos += in_obj->offset_bx;
                pos -= cur_obj->offset_bx;
                cur_obj->x = pos;
                offset_y = in_obj->offset_by;
                pos = in_obj->y;
                pos += offset_y;
                pos -= cur_obj->offset_by;
                cur_obj->y = pos;
                cur_obj->display_prio = i != 0 ? 2 : 7;
                calc_obj_pos(cur_obj);
                break;
            }
            cur_obj++;
            j++;
        }
    }
}

s32 instantSpeed(s16 speed) {
    s32 spd_shr;
    s32 spd_abs_max;

    spd_shr = ashr16(speed, 4);
    spd_abs_max = Abs(speed) & 0xF;
    if (spd_abs_max != 0) {
        spd_shr += sgn(speed) * (ashr32(map_time * spd_abs_max, 4) - ashr32(map_time * spd_abs_max - spd_abs_max, 4));
    }
    return spd_shr;
}

void SET_X_SPEED(obj_t* obj) {
    s16 xspeed;
    u8 horloge_index;

    horloge_index = ((obj->animations[obj->anim_index].layers_per_frame & 0xC000) >> 14) + 1;
    if (horloge[horloge_index] == 0) {
        eta_t *eta;

        eta = get_eta(obj);
        switch (obj->flags.flip_x) {
        case 0: xspeed = eta->speed_x_left * horloge_index; break;
        case 1: xspeed = eta->speed_x_right * horloge_index; break;
        }
    } else {
        xspeed = 0;
    }
    if (obj->type == TYPE_23_RAYMAN && RayEvts.tiny) {
        xspeed = ashr16(xspeed, 1);
    }
    obj->speed_x = xspeed;
}

void make_active2(obj_t* obj, u8 do_nova) {
    obj->is_active = 1;
    obj->field_1C = 1;
    obj->active_flag = 0;
    if (do_nova) {
        DO_NOVA(obj);
    }
}

void make_active(obj_t* obj, u8 do_nova) {
    if (obj->flags.alive) {
        obj->is_active = 1;
        obj->field_1C = 1;
        obj->active_flag = 0;
        if (do_nova) {
            DO_NOVA(obj);
        }
    }
}

s16 in_action_zone(s16 in_x, s16 in_y, obj_t *obj, u8 param_4)
{
    s16 unk_1;
    s16 zdiff_x;
    s16 zdiff_y;
    u8 offset_bx;

    unk_1 = 0;
    zdiff_x = zonediffx[ot];
    zdiff_y = zonediffy[ot];
    offset_bx = obj->offset_bx;

    if (flags[ot].flags0 & 0x10)
    {
        if (flags[ot].flags0 & 0x80 && IsBossThere)
            unk_1 = 1;
        else
        {
            switch (ot)
            {
            case TYPE_FEE:
            case TYPE_POING:
            case TYPE_BATTEUR_FOU:
            case TYPE_BLACK_RAY:
            case TYPE_BLACK_FIST:
            case TYPE_COUTEAU:
            case TYPE_DARK_SORT:
            case TYPE_NOVA2:
                unk_1 = 1;
                break;
            case TYPE_FISH:
            case TYPE_BBL:
                if (in_x > -284 && in_x < 444)
                    unk_1 = 1;
                break;
            case TYPE_PIERREACORDE:
                if (((s16) in_y + zdiff_y > -50) && in_x > -284 && in_x < 500)
                {
                    if (level.objects[pierreAcorde_obj_id].hit_points == 0)
                    {
                        level.objects[pierreAcorde_obj_id].hit_points = 10;
                        set_main_and_sub_etat(&level.objects[pierreAcorde_obj_id], 2, 4);
                    }
                }
            case TYPE_CORDE:
                if (level.objects[pierreAcorde_obj_id].hit_points > 0)
                    unk_1 = 1;
                break;
            case TYPE_EAU:
                if (in_x > -284 && in_x < 500)
                    unk_1 = 1;
                break;
            case TYPE_STONEDOG2:
                if ((in_x > -284 && in_x < 444) && (in_y > -100 && in_y < 250))
                    unk_1 = 1;
                break;
            case TYPE_SMA_PETIT_LASER:
            case TYPE_SMA_GRAND_LASER:
                offset_bx = 128;
                break;

            }
        }
    }

    if (unk_1 == 0)
    {
        if (param_4)
        {
            zdiff_x += 60;
            zdiff_y += 60;
        }

        if (
            ((s16) in_x < -284 - zdiff_x) || ((s16) in_x > zdiff_x + 444) ||
            ((s16) in_y < -144 - zdiff_y) || ((s16) in_y > zdiff_y + 304) ||
            (obj->y + obj->offset_by < -30) || (obj->y > mp.height * 16 + 30) ||
            (obj->x + offset_bx * 2 < 0) || (obj->x > mp.width * 16)
        )
            unk_1 = 0;
        else
            unk_1 = 1;
        }

    if (unk_1 == 1)
    {
        if (flags[ot].flags0 & 0x80)
        {
            IsBossThere = 1;
            if (
                (!first_boss_meet && obj->hit_points != 0 && scrollLocked) ||
                (first_boss_meet && obj->hit_points == 0)
            )
                Change_Wait_Anim();
        }
    }
    return unk_1 == 1;
}

void kill_obj(obj_t *obj)
{
    obj->x = -32000;
    obj->y = -32000;
    obj->active_flag = ACTIVE_DEAD;
    obj->flags.alive = 0;
}

void SET_ACTIVE_FLAG(s16 screen_x, s16 screen_y, obj_t* obj) {
    u8 was_active;

    ot = obj->type;
    was_active = obj->is_active;
    obj->is_active = 0;
    if (obj->flags.alive) {
        if (in_action_zone(screen_x, screen_y, obj, was_active)) {
            if (obj->active_timer == 0 &&
                    ((flags[ot].raw & flags1_1_keep_linked_objects_active) ||
                     obj->active_flag < 2 ||
                     (obj->active_flag != 2 && ot == TYPE_10_FISH))) {
                obj->is_active = was_active;
                if ((flags[ot].raw & flags0_1_always) || (flags[ot].raw & flags3_0x40_no_link)) {
                    make_active2(obj, !obj->is_active && (flags[ot].raw & flags1_1_keep_linked_objects_active));
                } else {
    s16 id;

                    id = obj->id;
                    do {
    obj_t * cur_obj;

                        cur_obj = level.objects + id;
                        make_active(cur_obj, !cur_obj->is_active && (flags[obj->type].raw & flags1_1_keep_linked_objects_active));
                        id = link_init[cur_obj->id];
                    } while (id != obj->id);
                }
            } else {
                if ((s16)obj->active_timer > 0)
                    --obj->active_timer;
            }
        } else {
            s16 active_flag;
            active_flag = obj->active_flag;
            if (active_flag != 1) {
                obj->active_timer = 0;
                if (flags[ot].raw & flags2_0x20_kill_if_outside_active_zone) {
                    kill_obj(obj);
                } else if (active_flag == 2) {
                    obj->active_flag = 1;
                } else if (active_flag == 0 || active_flag == 4) {
                    if (flags[ot].raw & flags0_1_always) {
                        REINIT_OBJECT(obj);
                    } else {
    u8 dont_reinit_linked_objects;

                        obj->active_flag = 4;
                        obj->is_active = 1;

                        dont_reinit_linked_objects = 0;
                        {
                            s16 id;
                            u16 type;
                            type = ot;
                            id = obj->id;
                            if (type != TYPE_10_FISH) {
                                do {
                                    obj_t *cur_obj;
                                    cur_obj = level.objects + id;
                                    if (cur_obj->active_flag != 4) {
                                        dont_reinit_linked_objects = 1;
                                        break;
                                    }
                                    id = link_init[cur_obj->id];
                                } while (id != obj->id);
                            }
                        }

                        if (obj->y + obj->offset_by < -30) {
                            obj->is_active = 0;
                        }

                        if (!dont_reinit_linked_objects) {
    s16 id;

                            id = obj->id;
                            do {
    obj_t * cur_obj;

                                cur_obj = level.objects + id;
                                REINIT_OBJECT(cur_obj);
                                id = link_init[cur_obj->id];
                            } while (id != obj->id);
                        }
                    }
                }
            }
        }
    }
}

s16 DO_PESANTEUR(obj_t* obj) {
    s16 y_accel;
    s16 res;
    u8 anim_speed_div;

    /* 2C214 80150A14 -O2 -msoft-float */

    res = 0;
    y_accel = 0;
    anim_speed_div = obj->eta[obj->main_etat][obj->sub_etat].anim_speed;
    anim_speed_div >>= 4;

    if (anim_speed_div != 0)
    {
        if (obj->type == TYPE_TAMBOUR1 || obj->type == TYPE_TAMBOUR2)
        {
            if (obj->config > 0)
                obj->config--;
            else if (obj->gravity_value_1 == 0)
            {
                res = 1;
                obj->speed_y += res;
            }
        }
        else
        {
            switch (anim_speed_div)
            {
                case 6:
                    if (horloge[2] == 0)
                    {
                        y_accel = 1;
                        res = 1;
                    }
                    break;
                case 1:
                    if (obj->gravity_value_1 == 0)
                    {
                        y_accel = 1;
                        res = 1;
                    }
                    break;
                case 2:
                    if (obj->gravity_value_2 == 0)
                    {
                        y_accel = 1;
                        res = 1;
                    }
                    break;
                case 3:
                    if (obj->speed_y < 1)
                        y_accel = 1;
                    else if (obj->speed_y > 1)
                        y_accel = -1;
                    break;
                case 4:
                    if (obj->speed_y > -2)
                        y_accel = -1;
                    else if (obj->speed_y < -2)
                        y_accel = 1;
                    break;
                case 5:
                    if (obj->gravity_value_1 == 0)
                        y_accel = -1;
                    break;
                case 10:
                    obj->gravity_value_1++;
                    if (obj->gravity_value_1 >= obj->gravity_value_2)
                    {
                        obj->gravity_value_1 = 0;
                        y_accel = 1;
                        res = 1;
                    }
                    break;
                case 11:
                    obj->gravity_value_1++;
                    if (obj->gravity_value_1 >= obj->gravity_value_2)
                    {
                        obj->gravity_value_1 = 0;
                        y_accel = -1;
                        res = 1;
                    }
                    break;
            }
        }

        if (flags[obj->type].raw & flags1_0x20_move_y)
            y_accel = ashl16(y_accel, 4);
        obj->speed_y += y_accel;
    }
    return res;
}

void freezeAnim(obj_t *obj, u8 param_2)
{
    s16 count;
    s32 count_capped;

    count = obj->animations[obj->anim_index].frames_count - 1;

    if (obj->eta[obj->main_etat][obj->sub_etat].flags & 0x10)
    {
        /* duplicate... */
        count_capped = min32(count, param_2);
        obj->anim_frame = count - count_capped + 1;
    }
    else
    {
        count_capped = min32(count, param_2);
        obj->anim_frame = count_capped;
    }
}

void DO_ANIM(obj_t* obj) {
    s32 new_frame;
    s16 prev_anim_index;
    s16 prev_anim_frame;
    eta_t * eta;
    anim_t * prev_anim;
    u8 anim_speed;
    anim_t * anim;

    prev_anim_frame = obj->anim_frame;
    prev_anim_index = obj->anim_index;
    eta = get_eta(obj);
    prev_anim = obj->animations + obj->anim_index;
    anim_speed = eta->anim_speed & 15;
    if (anim_speed != 0 && horloge[anim_speed] == 0) {
        switch (eta->flags_bits.bit4) {
        case 0: ++obj->anim_frame; break;
        default: --obj->anim_frame; break;
        }
    }
    obj->anim_index = eta->anim_index;


    anim = obj->animations + obj->anim_index;
    switch (obj->change_anim_mode) {
    case ANIMMODE_RESET_IF_NEW:
        if (obj->anim_index != prev_anim_index) {
            if (!(eta->flags & eta_flags_0x10_anim_reverse))
                obj->anim_frame = 0;
            else
                obj->anim_frame = anim->frames_count - 1;
        }
        if (obj->is_active)
            PlaySnd(get_eta(obj)->sound_index, obj->id);
        break;
    case ANIMMODE_RESET:
        if (!(eta->flags & eta_flags_0x10_anim_reverse))
            obj->anim_frame = 0;
        else
            obj->anim_frame = anim->frames_count - 1;
        if (obj->is_active)
            PlaySnd(get_eta(obj)->sound_index, obj->id);
        break;
    }
    if (obj->anim_frame >= anim->frames_count || obj->anim_frame == 255) {
        // animation ended
        obj->sub_etat = eta->next_sub_etat;
        obj->main_etat = eta->next_main_etat;
        eta = get_eta(obj);
        obj->anim_index = eta->anim_index;
        anim = obj->animations + obj->anim_index;
        if ((obj->type == TYPE_23_RAYMAN && ray_old_sub_etat == 8 && (ray_old_main_etat == 2 || ray_old_main_etat == 6) && obj->sub_etat != 8) ||
                (ray_old_sub_etat == 61 && ray_old_main_etat == 0 && obj->sub_etat != 61)
        ) {
            if (ray.iframes_timer > 60 && !RayEvts.squashed) {
                ray.iframes_timer = 60;
            }
        }
        if (eta->flags & eta_flags_0x10_anim_reverse)
            new_frame = anim->frames_count - 1;
        else
            new_frame = 0;
        obj->anim_frame = new_frame;
        if (obj->is_active != 0) {
            PlaySnd(get_eta(obj)->sound_index, obj->id);
        }
    }
    obj->change_anim_mode = ANIMMODE_NONE;
    if (obj->flags.follow_enabled) {
        CALC_FOLLOW_SPRITE_SPEED(obj, anim, prev_anim, prev_anim_frame);
    }
    obj->flags.anim_changed = (obj->anim_frame != prev_anim_frame || obj->anim_index != prev_anim_index) ? 1 : 0;
}

s16 prof_in_bloc(obj_t* obj) {
    return (obj->y + obj->offset_by) & 0xF;
}

void do_boing(obj_t *obj, u8 main_etat, u8 sub_etat)
{
    if (obj->speed_y <= 1)
    {
        obj->speed_y = 0;
        set_main_and_sub_etat(obj, main_etat, sub_etat);
        if (main_etat != 2)
            recale_position(obj);
    }
    else
    {
        if (horloge[2] == 0)
        {
            switch (obj->btypes[0])
            {
            case 8:
            case 12:
            case 24:
            case 25:
                break;
            case BTYP_SOLID_RIGHT_45:
            case BTYP_SLIPPERY_RIGHT_45:
                obj->speed_y = 1 - (obj->speed_y >> 1);
                obj->speed_x -= 2;
                break;
            case BTYP_SOLID_LEFT_45:
            case BTYP_SLIPPERY_LEFT_45:
                obj->speed_y = 1 - (obj->speed_y >> 1);
                obj->speed_x += 2;
                break;
            case BTYP_SOLID_RIGHT1_30:
            case BTYP_SOLID_RIGHT2_30:
            case BTYP_SLIPPERY_RIGHT1_30:
            case BTYP_SLIPPERY_RIGHT2_30:
                obj->speed_y = 1 - ((obj->speed_y + 1) >> 1);
                obj->speed_x--;
                break;
            case BTYP_SOLID_LEFT1_30:
            case BTYP_SOLID_LEFT2_30:
            case BTYP_SLIPPERY_LEFT1_30:
            case BTYP_SLIPPERY_LEFT2_30:
                obj->speed_y = 1 - ((obj->speed_y + 1) >> 1);
                obj->speed_x++;
                break;
            case BTYP_SOLID_PASSTHROUGH:
            case BTYP_SOLID:
            case BTYP_SLIPPERY:
                obj->speed_y = -((obj->speed_y + 1) >> 1);
                break;
            case BTYP_RESSORT:
                obj->speed_y = -((obj->speed_y + 3) >> 1);
                break;
            }
        }

        if (obj->speed_y < 0)
            set_sub_etat(obj, 0);
    }
}

u8 underSlope(obj_t *obj) { return (block_flags[obj->btypes[0]].raw & 1) && (block_flags[obj->btypes[3]].raw & 0x40); }

void DO_STONEBOMB_REBOND(obj_t* obj) {
    s16 new_spd_x;
    s16 new_spd_y;

    /* 2CAAC 801512AC -O2 -msoft-float */

    if (obj->type == TYPE_STONEBOMB2) {
        new_spd_x = 32;
        new_spd_y = -4;
    }
    else if (obj->type == TYPE_STONEBOMB3) {
        new_spd_x = 48;
        new_spd_y = -6;
    }

    if (underSlope(obj))
        obj->btypes[0] = obj->btypes[3];

    switch (obj->btypes[0]) {
        case BTYP_SOLID_LEFT1_30:
        case BTYP_SOLID_LEFT2_30:
        case BTYP_SLIPPERY_LEFT1_30:
        case BTYP_SLIPPERY_LEFT2_30:
            obj->speed_y = new_spd_y;
            obj->speed_x = new_spd_x;
            break;
        case BTYP_SOLID_RIGHT1_30:
        case BTYP_SOLID_RIGHT2_30:
        case BTYP_SLIPPERY_RIGHT1_30:
        case BTYP_SLIPPERY_RIGHT2_30:
            obj->speed_y = new_spd_y;
            obj->speed_x = -new_spd_x;
            break;
        case BTYP_SOLID_LEFT_45:
        case BTYP_SLIPPERY_LEFT_45:
            obj->speed_y = new_spd_y + 2;
            obj->speed_x = new_spd_x + 16;
            break;
        case BTYP_SOLID_RIGHT_45:
        case BTYP_SLIPPERY_RIGHT_45:
            obj->speed_y = new_spd_y + 2;
            obj->speed_x = -16 - new_spd_x;
            break;
        case BTYP_SOLID_PASSTHROUGH:
        case BTYP_SOLID:
        case BTYP_SLIPPERY:
            obj->speed_y = new_spd_y;
            break;
        case BTYP_RESSORT:
            obj->speed_y = new_spd_y - 3;
            break;
    }
    obj->gravity_value_1 = 0;
    obj->gravity_value_2 = 2;
}

void DO_THROWN_BOMB_REBOND(obj_t* obj, s16 pesanteur, s16 a3) {
    u8 under;
    u8 btyp;
    u8 anim_speed;
    s32 mul_45;
    s32 mul_30;
    u8 div_45;
    u8 div_30;
    u16 old_type;

    /* 2CBC4 801513C4 -O2 -msoft-float */

    mul_45 = 5;
    mul_30 = 3;
    div_45 = 1;
    div_30 = 1;

    if (obj->main_etat == 2) {
        if (obj->sub_etat == 1) {
            DO_STONE_EXPLOSION(obj, -1);
            return;
        }
        if (obj->sub_etat == 0) {
            old_type = (u16)obj->type;
            obj->type = TYPE_STONEBOMB2;
            DO_STONEBOMB_REBOND(obj);
            obj->type = old_type;
            return;
        }
    }

    if (obj->speed_y >= 0) {
        under = underSlope(obj);
        if (under)
            btyp = obj->btypes[3];
        else
            btyp = obj->btypes[0];

        switch (btyp) {
            case BTYP_NONE:
            case BTYP_SLIPPERY:
                break;
            case BTYP_SOLID_RIGHT_45:
            case BTYP_SLIPPERY_RIGHT_45:
                if (obj->speed_y == 0)
                    obj->speed_y++;
                obj->speed_x -= mul_45 * obj->speed_y / div_45;
                break;
            case BTYP_SOLID_LEFT_45:
            case BTYP_SLIPPERY_LEFT_45:
                if (obj->speed_y == 0)
                    obj->speed_y++;
                obj->speed_x += mul_45 * obj->speed_y / div_45;
                break;
            case BTYP_SOLID_RIGHT1_30:
            case BTYP_SOLID_RIGHT2_30:
            case BTYP_SLIPPERY_RIGHT1_30:
            case BTYP_SLIPPERY_RIGHT2_30:
                if (obj->speed_y == 0)
                    obj->speed_y++;
                obj->speed_x -= mul_30 * obj->speed_y / div_30;
                break;
            case BTYP_SOLID_LEFT1_30:
            case BTYP_SOLID_LEFT2_30:
            case BTYP_SLIPPERY_LEFT1_30:
            case BTYP_SLIPPERY_LEFT2_30:
                if (obj->speed_y == 0)
                    obj->speed_y++;
                obj->speed_x += mul_30 * obj->speed_y / div_30;
                break;
            case BTYP_RESSORT:
                if (obj->speed_y == 0)
                    obj->speed_y++;
                break;
        }
        if (a3 > 0) {
            if (obj->speed_y >= 2) {
                obj->speed_y = a3 - obj->speed_y;
                if (pesanteur)
                    obj->speed_y++;
            }
            else
                obj->speed_y = 0;
        }
        else
            obj->speed_y = -3;

        if (under) {
            while (BTYP((obj->x + obj->offset_bx) >> 4, (obj->y + obj->offset_by) >> 4) == obj->btypes[0]) {
                obj->y--;
            }

            recale_position(obj);
            calc_btyp(obj);
        } else if ((block_flags[obj->btypes[0]].raw & 1) && prof_in_bloc(obj) >= 4 && obj->speed_y < 3) {
            if (obj->speed_y == 0)
                obj->speed_y = 1;
        }
        else
            recale_position(obj);
    }
    obj->gravity_value_1 = 0;
    anim_speed = obj->eta[obj->main_etat][obj->sub_etat].anim_speed >> 4;
    if (!(anim_speed == 10 || anim_speed == 11))
        obj->gravity_value_2 = 0;
}

void DO_FRUIT_REBOND(obj_t* obj, s16 pesanteur, s16 a3) {
    s16 gravity;
    s16 bounce;
    u8 under;
    u8 btyp;
    u8 anim_speed;
    s16 accel_x;
    s16 speed_x_bnd;
    s16 mul_45;
    s16 mul_30;
    s16 div_30;
    u16 obj_type;

    /* 2D008 80151808 -O2 -msoft-float */

    gravity = pesanteur;
    bounce = a3;
    if (obj->type == TYPE_GRAINE) {
        accel_x = 1;
        speed_x_bnd = 1;
        mul_45 = 1;
        mul_30 = 1;
        div_30 = 2;
    }
    else {
        accel_x = 5;
        speed_x_bnd = 4;
        mul_45 = 5;
        mul_30 = 3;
        div_30 = 1;
    }

    if (obj->speed_y >= 0) {
        if (obj->speed_x > 0)
            obj->speed_x -= accel_x;
        else if (obj->speed_x < 0)
            obj->speed_x += accel_x;

        if (inrange(obj->speed_x, -speed_x_bnd, speed_x_bnd))
            obj->speed_x = 0;

        under = underSlope(obj);
        if (under)
            btyp = obj->btypes[3];
        else
            btyp = obj->btypes[0];

        switch (btyp) {
            case BTYP_SOLID_PASSTHROUGH:
            case BTYP_SOLID:
            case BTYP_SLIPPERY:
            case 8:
            case 12:
            case 24:
            case 25:
                break;
            case BTYP_SOLID_RIGHT_45:
            case BTYP_SLIPPERY_RIGHT_45:
                if (obj->speed_y == 0)
                    obj->speed_y++;
                obj->speed_x -= mul_45 * obj->speed_y;
                break;
            case BTYP_SOLID_LEFT_45:
            case BTYP_SLIPPERY_LEFT_45:
                if (obj->speed_y == 0)
                    obj->speed_y++;
                obj->speed_x += mul_45 * obj->speed_y;
                break;
            case BTYP_SOLID_RIGHT1_30:
            case BTYP_SOLID_RIGHT2_30:
            case BTYP_SLIPPERY_RIGHT1_30:
            case BTYP_SLIPPERY_RIGHT2_30:
                if (obj->speed_y == 0)
                    obj->speed_y++;
                obj->speed_x -= mul_30 * obj->speed_y / div_30;
                break;
            case BTYP_SOLID_LEFT1_30:
            case BTYP_SOLID_LEFT2_30:
            case BTYP_SLIPPERY_LEFT1_30:
            case BTYP_SLIPPERY_LEFT2_30:
                if (obj->speed_y == 0)
                    obj->speed_y++;
                obj->speed_x += mul_30 * obj->speed_y / div_30;
                break;
            case BTYP_RESSORT:
                if (obj->speed_y == 0)
                    obj->speed_y++;
                break;
        }

        if (bounce > 0) {
            if (obj->speed_y >= 2) {
                obj->speed_y = bounce - obj->speed_y;
                if (gravity)
                    obj->speed_y++;
            } else {
                obj->speed_y = 0;
                obj->hit_points = 2;
            }
        } else {
            obj_type = (u16)obj->type;
            if (obj_type == TYPE_FALLING_OBJ || obj_type == TYPE_FALLING_OBJ2 || obj_type == TYPE_FALLING_OBJ3 ||
                obj_type == TYPE_FALLING_YING || obj_type == TYPE_FALLING_YING_OUYE
            ) {
                if (ray.follow_id == obj->id) {
                    if (RayEvts.tiny)
                        obj->speed_y = -4;
                    else
                        obj->speed_y = -3;
                }
                else
                    obj->speed_y = -5;

                if (obj->type == TYPE_FALLING_YING_OUYE)
                    obj->speed_y++;
            }
            else
                obj->speed_y = -3;
        }

        if (under) {
            while (BTYP((obj->x + obj->offset_bx) >> 4, (obj->y + obj->offset_by) >> 4) == obj->btypes[0]) {
                obj->y--;
            }

            calc_btyp(obj);
            recale_position(obj);
        } else if ((block_flags[obj->btypes[0]].raw & 1) && prof_in_bloc(obj) >= 4 && obj->speed_y < 3) {
            if (obj->speed_y == 0)
                obj->speed_y = 1;
        }
        else
            recale_position(obj);
    }

    obj->gravity_value_1 = 0;
    anim_speed = obj->eta[obj->main_etat][obj->sub_etat].anim_speed >> 4;
    if (!(anim_speed == 10 || anim_speed == 11))
        obj->gravity_value_2 = 0;
}

void Drop_Atter(obj_t *obj)
{
    set_main_and_sub_etat(obj, 2, 3);
    obj->speed_y = 0;
    obj->speed_x = 0;
}

void BadGuyAtter(obj_t *obj)
{
    recale_position(obj);
    if (obj->hit_points != obj->init_hit_points)
    {
        set_main_and_sub_etat(obj, 1,
            (obj->eta[obj->main_etat][obj->sub_etat].flags & 0x40) ? 2 : 11);
    }
    else
    {
        set_main_and_sub_etat(obj, 1, 0);
        obj->flags.read_commands = 1;
    }

    calc_obj_dir(obj);
    skipToLabel(obj, obj->flags.flip_x ? 3 : 2, 1);
}

void MiteAtter(obj_t *obj)
{
    if (obj->speed_y > 0)
    {
        recale_position(obj);
        skipToLabel(obj, 2, 1);
        if (obj->follow_id > 500)
            obj->follow_id = 0;
    }
}

void LidolPinkAtter(obj_t *obj)
{
    if (gerbe && obj->sub_etat == 1)
        do_boing(obj, 1, 0);
    else if (obj->sub_etat != 0)
    {
        skipToLabel(obj, 1, 1);
        obj->speed_y = 0;
        recale_position(obj);
    }
}

void stoneDogAtter(obj_t *obj)
{
    if (obj->sub_etat == 2)
    {
        skipToLabel(obj, 4, 1);
        recale_position(obj);
    }
}

void stoneDogBounces(obj_t* obj) {
    s32 x;
    s32 y;
    s32 block_l;
    s32 block_r;

    /* 2D7D0 80151FD0 -O2 -msoft-float */
    x = obj->x + obj->offset_bx;
    y = obj->y + obj->offset_by;
    block_l = mp.map[((x - 2) >> 4) + ((y >> 4) * mp.width)].tile_type;
    block_r = mp.map[((x + 2) >> 4) + ((y >> 4) * mp.width)].tile_type;

    if (obj->main_etat == 2 && !(block_flags[obj->btypes[0]].raw & 2) &&
            ((block_flags[block_l].raw & 1 && !(obj->flags.flip_x)) || (block_flags[block_r].raw & 1 && obj->flags.flip_x))
    ) {
        if (obj->speed_y < 0) {
            if (obj->flags.flip_x)
                skipToLabel(obj, 6, 1);
            else
                skipToLabel(obj, 7, 1);
        } else {
            obj->x += obj->speed_x / 16;
            skipToLabel(obj, 4, 1);
            obj->flags.flip_x ^= 1;
        }
    }
}

void Spider_Atter(obj_t *obj)
{
    set_main_and_sub_etat(obj, 1, 0);
    if (obj->flags.flip_x)
        skipToLabel(obj, 3, 1);
    else
        skipToLabel(obj, 1, 1);
}

void NormalAtter(obj_t *obj)
{
    set_main_and_sub_etat(obj, obj->init_main_etat, obj->init_sub_etat);
    recale_position(obj);
}

void OBJ_IN_THE_AIR(obj_t* obj) {
    s16 pesanteur;
    s16 spr_w;
    s16 spr_x;
    s16 spr_y;
    s16 spr_h;
    u8 anim_speed_div;

    /* 2DA58 80152258 -O2 -msoft-float */

    anim_speed_div = obj->eta[obj->main_etat][obj->sub_etat].anim_speed;
    anim_speed_div >>= 4;

    if (!(anim_speed_div == 11 || anim_speed_div == 10))
    {
        obj->gravity_value_1++;
        if (obj->gravity_value_1 >= 3)
            obj->gravity_value_1 = 0;

        obj->gravity_value_2++;
        if (obj->gravity_value_2 >= 4)
            obj->gravity_value_2 = 0;
    }

    pesanteur = DO_PESANTEUR(obj);
    if (obj->type == TYPE_LIDOLPINK)
    {
        if (obj->speed_y >= 0 && obj->sub_etat == 0)
            set_sub_etat(obj, 1);

        if (!gerbe && obj->speed_y >= 3 && obj->sub_etat == 1)
        {
            set_sub_etat(obj, 2);
            obj->speed_x = 0;
        }
    }
    else if (obj->type == TYPE_STONEDOG || obj->type == TYPE_STONEDOG2)
    {
        stoneDogBounces(obj);
        if (obj->main_etat == 2)
        {
            if (obj->speed_y > 0)
            {
                if (obj->sub_etat != 2)
                    set_sub_etat(obj, 2);
            }
            else if (obj->speed_y < 0 && (block_flags[(u8) calc_typ_trav(obj, 1)].raw & 1))
                obj->speed_y = 0;
        }
    }
    else if (obj->type == TYPE_NOTE1)
    {
        if (--obj->iframes_timer == 0)
            DO_EXPLOSE_NOTE1(obj);
    }

    if ((flags[obj->type].raw & flags3_4_stop_moving_up_when_hit_block) && obj->speed_y < 0)
    {
        if (obj->flags.follow_enabled)
        {
            GET_SPRITE_POS(obj, obj->follow_sprite, &spr_x, &spr_y, &spr_w, &spr_h);
            spr_x = obj->x + obj->offset_bx;
            spr_y = spr_y + obj->offset_hy;
        }
        else
        {
            spr_x = obj->x + obj->offset_bx;
            spr_y = obj->y + obj->offset_hy;
        }

        if (block_flags[BTYP(spr_x >> 4, (spr_y + obj->speed_y) >> 4)].raw & 0x10)
            obj->speed_y = 0;
    }
    if ((flags[obj->type].raw & flags2_1_check_tile_type) && (block_flags[obj->btypes[0]].raw & 2))
    {
        switch (obj->type)
        {
            case TYPE_TROMPETTE:
                trompetteAtter(obj);
                break;
            case TYPE_HYBRIDE_MOSAMS:
                calc_obj_dir(obj);
                recale_position(obj);
                skipToLabel(obj, 3, 1);
                if (ray.main_etat != 2)
                {
                    allocateLandingSmoke(obj);
                    set_main_and_sub_etat(&ray, 0, 0);
                    button_released = 1;
                    ray_jump();
                    ray.speed_y = -8;
                }
                screen_trembling = 1;
                break;
            case TYPE_BLACKTOON1:
                switch (obj->follow_sprite)
                {
                    case 3:
                    case 5:
                    case 6:
                        break;
                    case 7:
                        obj->config = 2;
                        calc_obj_dir(obj);
                    case 1:
                    case 4:
                        set_main_and_sub_etat(obj, 1, 0);
                        if (obj->flags.flip_x)
                            skipToLabel(obj, 3, 1);
                        else
                            skipToLabel(obj, 2, 1);

                        recale_position(obj);
                        obj->speed_y = 0;
                        break;
                    case 2:
                        set_main_and_sub_etat(obj, 0, 0);
                        recale_position(obj);
                        obj->speed_y = 0;
                        break;
                }
                break;
            case TYPE_WASHING_MACHINE:
                if (obj->speed_y > 0)
                {
                    recale_position(obj);
                    obj->speed_y = -obj->speed_y;
                }
                break;
            case TYPE_DROP:
                Drop_Atter(obj);
                break;
            case TYPE_MITE:
            case TYPE_MITE2:
                MiteAtter(obj);
                break;
            case TYPE_STONEWOMAN:
                if (obj->speed_y >= 0)
                {
                    skipToLabel(obj, 16, 1);
                    obj->config = 0;
                }
                break;
            case TYPE_TNT_BOMB:
                PlaySnd(187, obj->id);
                BombExplosion(obj);
                break;
            case TYPE_BADGUY1:
            case TYPE_BADGUY2:
            case TYPE_BADGUY3:
                BadGuyAtter(obj);
                break;
            case TYPE_STONEBOMB2:
            case TYPE_STONEBOMB3:
                if (obj->speed_y != 0)
                    PlaySnd(200, obj->id);
                DO_STONEBOMB_REBOND(obj);
                break;
            case TYPE_STONEBOMB:
                if (obj->speed_y != 0)
                    PlaySnd(200, obj->id);
                DO_THROWN_BOMB_REBOND(obj, pesanteur, 1);
                break;
            case TYPE_STONEDOG:
            case TYPE_STONEDOG2:
                stoneDogAtter(obj);
                break;
            case TYPE_PIRATE_BOMB:
                DO_FRUIT_REBOND(obj, pesanteur, 1);
                break;
            case TYPE_MST_SHAKY_FRUIT:
                if (obj->param != 0)
                {
                    PlaySnd(45, obj->id);
                    obj->speed_y = 6;
                    DO_FRUIT_REBOND(obj, 1, 1);
                    obj->param--;
                }
                break;
            case TYPE_MST_FRUIT1:
                if (obj->param != 0)
                {
                    PlaySnd(45, obj->id);
                    obj->speed_y = 7;
                    DO_FRUIT_REBOND(obj, 1, 1);
                    obj->param--;
                }
                break;
            case TYPE_MST_FRUIT2:
                if (obj->param != 0)
                {
                    PlaySnd(45, obj->id);
                    obj->speed_y = 6;
                    DO_THROWN_BOMB_REBOND(obj, 1, 1);
                    obj->param--;
                }
                break;
            case TYPE_FALLING_OBJ:
            case TYPE_FALLING_OBJ2:
            case TYPE_FALLING_OBJ3:
            case TYPE_FALLING_YING:
            case TYPE_FALLING_YING_OUYE:
            case TYPE_GRAINE:
            case TYPE_BLACKTOON_EYES:
                switch (obj->type)
                {
                    case TYPE_FALLING_OBJ:
                    case TYPE_FALLING_OBJ3:
                    case TYPE_FALLING_OBJ2:
                        PlaySnd(45, obj->id);
                        break;
                    case TYPE_FALLING_YING:
                    case TYPE_FALLING_YING_OUYE:
                        PlaySnd(140, obj->id);
                        break;
                    case TYPE_GRAINE:
                    case TYPE_BLACKTOON_EYES:
                        PlaySnd(0, obj->id);
                        break;
                }
                DO_FRUIT_REBOND(obj, pesanteur, 0);
                break;
            case TYPE_CAGE:
                set_main_and_sub_etat(obj, 0, 11);
                obj->speed_y = 0;
                break;
            case TYPE_LIDOLPINK:
                LidolPinkAtter(obj);
                break;
            case TYPE_PIRATE_GUETTEUR:
            case TYPE_PIRATE_GUETTEUR2:
                if (obj->sub_etat == 2)
                    set_main_and_sub_etat(obj, 0, 7);
                else if (obj->speed_y >= 0)
                    set_main_and_sub_etat(obj, 0, 10);
                recale_position(obj);
                obj->speed_y = 0;
                obj->speed_x = 0;
                break;
            case TYPE_PIRATE_NGAWE:
                recale_position(obj);
                set_main_and_sub_etat(obj, 0, 5);
                obj->speed_y = 0;
                obj->speed_x = 0;
                break;
            case TYPE_SUPERHELICO:
                obj->speed_x = 0;
                obj->speed_y = 0;
                break;
            case TYPE_NOTE0:
            case TYPE_NOTE1:
            case TYPE_NOTE2:
            case TYPE_BONNE_NOTE:
            case TYPE_NOTE3:
                DO_NOTE_REBOND(obj);
                break;
            case TYPE_BBL:
                DO_BBL_REBOND(obj);
                break;
            case TYPE_BIG_CLOWN:
            case TYPE_WAT_CLOWN:
                Clown_Music_Atter(obj);
                break;
            case TYPE_SPIDER:
                Spider_Atter(obj);
                break;
            case TYPE_BB1:
                if (obj->iframes_timer < 10)
                    DO_BBMONT_ATTER(obj);
                break;
            case TYPE_BB12:
                DO_BBMONT2_ATTER(obj);
                break;
            case TYPE_BB13:
                DO_BBMONT3_ATTER(obj);
                break;
            case TYPE_SAXO2:
                DO_SAXO2_ATTER(obj);
                break;
            case TYPE_SAXO:
                DO_SAXO_ATTER(obj);
                break;
            case TYPE_MAMA_PIRATE:
                DO_PMA_ATTER(obj);
                break;
            case TYPE_COUTEAU:
                DO_COU_ATTER(obj);
                break;
            case TYPE_BOUT_TOTEM:
                DO_TOTBT_REBOND(obj);
                break;
            case TYPE_SPIDER_PLAFOND:
                if (obj->main_etat == 2 && obj->speed_y > 0)
                {
                    set_main_and_sub_etat(obj, 0, 29);
                    obj->speed_x = 0;
                    obj->speed_y = 0;
                    obj->flags.flip_x = ray.x > obj->x ? 1 : 0;
                }
                break;
            case TYPE_BALLE1:
            case TYPE_BALLE2:
            case TYPE_BIG_BOING_PLAT:
            case TYPE_BLACK_FIST:
            case TYPE_BLACK_RAY:
            case TYPE_BOING_PLAT:
            case TYPE_BULLET:
            case TYPE_CORDEBAS:
            case TYPE_DARK_PHASE2:
            case TYPE_MARACAS_BAS:
            case TYPE_MOSKITO:
            case TYPE_MOSKITO2:
            case TYPE_MST_SCROLL:
            case TYPE_PI_BOUM:
            case TYPE_POI1:
            case TYPE_POI2:
            case TYPE_POI3:
            case TYPE_RING:
            case TYPE_SPACE_MAMA:
            case TYPE_SPACE_MAMA2:
            case TYPE_TAMBOUR1:
            case TYPE_TAMBOUR2:
                break;
            /* DOS 1:47937..47960, 1:4789e..478cd and 1:477b2..477c8
               distinguish these existing no-op cases from adjacent ranges. */
            case TYPE_BNOTE:
                break;
            case TYPE_SCORPION:
                break;
            case TYPE_DARK:
                break;
            default:
                NormalAtter(obj);
                break;
        }
    }
    if (flags[obj->type].raw & flags1_0x20_move_y)
    {
        MIN_2(obj->speed_y, 96);
        MAX_2(obj->speed_y, -128)
    }
    else
    {
        MIN_2(obj->speed_y, 6);
        MAX_2(obj->speed_y, -8)
    }
}

void test_fall_in_water(obj_t* obj) {
    if (obj->btypes[0] == BTYP_WATER && !(obj->type == TYPE_23_RAYMAN && ray.main_etat == 6)) {
        if (obj->type == TYPE_52_MST_FRUIT2) {
            obj->flags.alive = 0;
        } else {
            if (flags[obj->type].raw & flags2_2_fall_in_water) {
                if (!obj->flags.flag_0x40) {
                    allocate_splash(obj);
                    obj->speed_y = 0;
                    obj->speed_x = obj->speed_y;
                }
                if (flags[obj->type].raw & flags3_2_die_in_water) {
                    obj->is_active = 0;
                    obj->y = ymap + 484;
                }
            }
        }
    }
}

void MOVE_OBJECT(obj_t* obj) {
    if (obj->speed_x != 0 || obj->speed_y != 0) {
    s32 need_set_flag_0x100;

        if ((flags[obj->type].raw & flags2_0x80_increase_speed_x) && obj->config == 1 && horloge[20] == 0) {
            obj->speed_x = obj->speed_x <= 0 ? obj->speed_x + 1 : obj->speed_x - 1;
            if (obj->speed_x == 0) {
                obj->config = 0;
            }
        }

        if (obj->type == TYPE_94_POING) {
            poing.y_16 += ashl16(obj->speed_y, 4);
            obj->y = poing.y_16 >> 4;
            obj->x += obj->speed_x;
            need_set_flag_0x100 = 0;
        } else {
    s32 dx;
    s32 dy;

            dx = 0;
            dy = 0;

            if (flags[obj->type].raw & flags1_0x20_move_y) {
                dy = instantSpeed(obj->speed_y);
            } else {
                dy = obj->speed_y;
            }

            if (flags[obj->type].raw & flags1_0x10_move_x) {
                dx = instantSpeed(obj->speed_x);
            } else {
                dx = obj->speed_x;
            }

            if (flags[obj->type].raw & flags2_0x40_uturn_on_block) {
    s32 x1;
    s32 x2;
    s32 y;

                x1 = obj->x + obj->offset_bx;
                x2 = x1 + dx;
                y = obj->y + obj->offset_by - 8;
                if ((!(block_flags[BTYP(x1 >> 4, y >> 4)].raw & 0x10) && (block_flags[BTYP(x2 >> 4, y >> 4)].raw & 0x10)) ||
                        !inrange(x2, 0, xmapmax + SCREEN_WIDTH - 16)) {
                    dx = -dx;
                    obj->speed_x = -obj->speed_x;
                }
            }

            obj->x += dx;
            obj->y += dy;
            need_set_flag_0x100 = (dx != 0) || (dy != 0);
        }

        obj->flags.flag_0x100 = need_set_flag_0x100;
        test_fall_in_water(obj);
    } else {
        obj->flags.flag_0x100 = 0;
    }
}

void DO_RAY_IN_ZONE(obj_t* obj) {
    if (OBJ_IN_ZONE(obj)) {
        ObjectsFonctions[obj->type].rayman_in_zone(obj);
    }
}

void DoRaymanInZDDDefault(obj_t* obj) {
    //nullsub
}

void DO_ONE_OBJECT(obj_t* obj) {
    if (flags[ot].raw & flags0_2_balle) {
        DO_BALLE(obj);
    }
    if (flags[ot].raw & flags0_0x20_has_detect_zone) {
        SET_DETECT_ZONE_FLAG(obj);
    }
    if (flags[ot].raw & flags2_1_check_tile_type) {
        calc_btyp(obj);
    }
    if (flags[ot].raw & flags1_0x80_read_cmd) {
        GET_OBJ_CMD(obj);
//        if (obj->id == debug_obj_id) {
//            printf("obj %d [%02d]: cmd %d\n", obj->id, obj->cmd_offset, obj->cmd);
//        }
    } else {
        obj->cmd = GO_NOP;
    }
    ObjectsFonctions[ot].command(obj);
    if (obj->main_etat == 2) {
        OBJ_IN_THE_AIR(obj);
    }
    if (obj->flags.follow_enabled) {
        SET_RAY_DIST(obj);
    } else {
        obj->ray_dist = 10000;
    }
    if (flags[ot].raw & flags1_0x40_special_platform) {
        DO_SPECIAL_PLATFORM(obj);
    }
    if (flags[ot].raw & flags0_0x20_has_detect_zone) {
        DO_RAY_IN_ZONE(obj);
    }
}

void build_active_table(void)
{
    s32 min_x, min_y, max_x, max_y;
    s32 half_x, half_y;
    s32 i;
    s32 added;
    s16 *out;
    s16 *out_start;
    obj_t *obj;

    actobj.objects[0] = -1;
    actobj.num_active_objects = 0;
    min_x = xmap;
    min_y = ymap;
    max_x = min_x + 1320;
    max_y = min_y + 1200;
    half_x = (max_x - min_x) >> 1;
    half_y = (max_y - min_y) >> 1;
    min_x = min_x + half_x - 500;
    min_y = min_y + half_y - 500;

    added = 0;
    i = 0;
    obj = &level.objects[level_obj.obj_ids[i]];
    while (i < level_obj.nb_objects) {
        obj->field_1C = 0;
        i++;
        obj = &level.objects[level_obj.obj_ids[i]];
    }
    out = &actobj.objects[actobj.num_active_objects];
    out_start = out;
    i = level_obj.nb_objects - 1;
    obj = &level.objects[level_obj.obj_ids[i]];
    while (i >= 0) {
        if (obj->field_1C == 0) {
            if ((obj->x - min_x < 0 ? -(obj->x - min_x) : obj->x - min_x) < half_x &&
                (obj->y - min_y < 0 ? -(obj->y - min_y) : obj->y - min_y) < half_y) {
                SET_ACTIVE_FLAG(obj->x - xmap + 8, obj->y - ymap, obj);
            } else if (flags[obj->type].flags0 & 0x10) {
                SET_ACTIVE_FLAG(obj->x - xmap + 8, obj->y - ymap, obj);
            } else {
                obj->is_active = 0;
            }
        }
        i--;
        obj = &level.objects[level_obj.obj_ids[i]];
    }
    i = level_obj.nb_objects - 1;
    obj = &level.objects[level_obj.obj_ids[i]];
    while (i >= 0) {
        if (obj->is_active) {
            obj->screen_x = obj->x - xmap + 8;
            obj->screen_y = obj->y - ymap;
            *out++ = obj->id;
            added++;
            if (added > 100)
                FatalError("Error On MAX_ACTOBJ.");
        } else if (flags[obj->type].flags0 & 1) {
            del_alwobj(obj->id);
        }
        i--;
        obj = &level.objects[level_obj.obj_ids[i]];
    }
    *out = -1;
    actobj.num_active_objects += out - out_start;
}

void Add_One_RAY_lives(void) {
    ++status_bar.lives;
    if (status_bar.lives > 99) {
        status_bar.lives = 99;
    }
    if (status_bar.max_hitp == 4) {
        ray.hit_points = status_bar.max_hitp;
    } else {
        ray.hit_points = 2;
    }
}

void DO_CLING_ANIMS(void) {
    obj_t *obj;
    if (id_Cling_1up != -1) {

        obj = level.objects + id_Cling_1up;
        if (obj->timer > 0) {
            --obj->timer;
            obj->anim_frame = 0;
        }
        DO_ANIM(obj);
        if (EOA(obj)) {
            id_Cling_1up = -1;
            if (ray_mode != MODE_3_MORT_DE_RAYMAN && ray_mode != MODE_4_MORT_DE_RAYMAN_ON_MS) {
                Add_One_RAY_lives();
            }
            obj->flags.alive = 0;
        }
    }

    if (id_Cling_Pow != -1) {

        obj = level.objects + id_Cling_Pow;
        if (obj->timer > 0) {
            --obj->timer;
            obj->anim_frame = 0;
        }
        DO_ANIM(obj);
        if (EOA(obj)) {
            id_Cling_Pow = -1;
            obj->flags.alive = 0;
        }
    }
}

void DO_OBJECTS_ANIMS(void)
{
    s16 i;
    obj_t *cur_obj;

    i = 0;
    cur_obj = &level.objects[actobj.objects[i]];

    while (i < actobj.num_active_objects)
    {
        if (!(cur_obj->type == TYPE_BLACK_RAY || cur_obj->type == TYPE_BLACK_FIST))
            DO_ANIM(cur_obj);
        i++;
        cur_obj = &level.objects[actobj.objects[i]];
    }
}

void DO_OBJECTS(void) {
    s16 i;
    obj_t *obj;

    if (id_Cling_1up != -1 || id_Cling_Pow != -1) {
        DO_CLING_ANIMS();
    }
    if (lidol_to_allocate != 0) {
        allocate_toons(lidol_source_obj, 7);
        lidol_to_allocate = 0;
    }
    i = 0;
    obj = &level.objects[actobj.objects[i]];
    while (i < actobj.num_active_objects) {
        ot = obj->type;

        if ((flags[ot].raw & flags3_8_switch_off) ||
            (ot == TYPE_161_WIZ && obj->sub_etat == 23) ||
            (ot == TYPE_83_EXPLOSION && obj->sub_etat == 1) ||
            (ot == TYPE_33_DARK2_SORT && obj->sub_etat != 35)
        ) {
            switchOff(obj);
        }
        if (ot != TYPE_161_WIZ && obj->is_active) {
            DO_ONE_OBJECT(obj);
            if (!(flags[ot].raw & flags0_0x80_boss) && obj->hit_points == 0 && (get_eta(obj)->flags & 8)) {
                ++obj->hit_points;
            }
        }
        i++;
        obj = &level.objects[actobj.objects[i]];
    }
}

void MOVE_OBJECTS(void)
{
    s16 i;
    obj_t *cur_obj;

    i = 0;
    cur_obj = &level.objects[actobj.objects[i]];

    while (i < actobj.num_active_objects)
    {
        MOVE_OBJECT(cur_obj);
        i++;
        cur_obj = &level.objects[actobj.objects[i]];
    }
}

void RECALE_ALL_OBJECTS(void)
{
    s16 x;
    s16 y;
    s16 w;
    s16 h;
    s16 i;
    obj_t *cur_obj;

    i = 0;
    cur_obj = &level.objects[actobj.objects[0]];

    if (i < actobj.num_active_objects) {
        do {
            i++;
            calc_obj_pos(cur_obj);
            cur_obj = level.objects + actobj.objects[i];
        } while (i < actobj.num_active_objects);
    }

    if (ray.follow_id != -1)
    {
        cur_obj = &level.objects[ray.follow_id];
        if (cur_obj->is_active)
        {
            GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &x, &y, &w, &h);
            ray.y = y + cur_obj->offset_hy - ray.offset_by;
            if (cur_obj->type == 254) {
                s32 dx;

                dx = (ray.x + ray.offset_bx) - ((x >> 1) + w);
                ray.y -= ashr16((s16)((s8)cur_obj->hit_points * dx), 4);
            }
            calc_obj_pos(&ray);
        }
    }
}

s16 RayCoince(s16 dir) {
    s16 x_pos;
    s16 y_pos;
    s32 map_ind;
    s32 res;

    /* 30A64 80155264 -O2 -msoft-float */

    res = 0;

    if (ray.flags.flip_x)
        x_pos = ray.x + 70;
    else
        x_pos = ray.x + 58;
    y_pos = ray.y + 32;

    switch (dir)
    {
        case 2:
            if (y_pos % 16 < 3)
            {
                map_ind = (x_pos >> 4) + (mp.width * (y_pos >> 4));
                if (block_flags[mp.map[map_ind].tile_type].raw & 1)
                    res = 1;
                if (block_flags[mp.map[++map_ind].tile_type].raw & 1)
                    res = 1;
                if (block_flags[mp.map[++map_ind].tile_type].raw & 1)
                    res = 1;
            }
            break;
        case 3:
            if (y_pos % 16 > 12)
            {
                map_ind = (x_pos >> 4) + (mp.width * ((y_pos + 16 * 4) >> 4));
                if (block_flags[mp.map[map_ind].tile_type].raw & 1)
                    res = 1;
                if (block_flags[mp.map[++map_ind].tile_type].raw & 1)
                    res = 1;
                if (block_flags[mp.map[++map_ind].tile_type].raw & 1)
                    res = 1;
            }
            break;
        case 0:
            if (x_pos % 16 < 3)
            {
                map_ind = ((x_pos >> 4) + (mp.width * ((y_pos + 16) >> 4))) - 1;
                if (block_flags[mp.map[map_ind].tile_type].raw & 1)
                    res = 1;
                if (block_flags[mp.map[map_ind += mp.width].tile_type].raw & 1)
                    res = 1;
                if (block_flags[mp.map[map_ind += mp.width].tile_type].raw & 1)
                    res = 1;
            }
            break;
        case 1:
            if (x_pos % 16 > 12)
            {
                map_ind = ((x_pos >> 4) + (mp.width * ((y_pos + 16) >> 4))) + 3;
                if (block_flags[mp.map[map_ind].tile_type].raw & 1)
                    res = 1;
                if (block_flags[mp.map[map_ind += mp.width].tile_type].raw & 1)
                    res = 1;
                if (block_flags[mp.map[map_ind += mp.width].tile_type].raw & 1)
                    res = 1;
            }
            break;
    }

    return res;
}

void move_up_ray(void) {
    eta_t *eta_row;
    u8 main_state;
    s16 unk_y_1;

    /* 30E34 80155634 -O2 -msoft-float */
    if (ray_mode == MODE_2_RAY_ON_MS && RayCoince(2)) {
        ray.speed_y = 0;
    }

    main_state = ray.main_etat;
    eta_row = ray.eta[main_state];
    unk_y_1 = ((eta_row[ray.sub_etat].flags & 0x40) || RayEvts.tiny) ? 40 : 25;

    ray.y += ray.speed_y;
    if (ray.y < scroll_start_y - 20 - unk_y_1) {
        ray.y = scroll_start_y - 20 - unk_y_1;
        ray.speed_y = 0;
        if (ray.follow_id != -1) {
            RAY_HIT(ray.iframes_timer == -1, NULL);
        }
    }
    calc_obj_pos(&ray);
    v_scroll_speed = 0;
}

void move_down_ray(void)
{
    if (ray_mode == MODE_RAY_ON_MS && RayCoince(3))
        ray.speed_y = 0;

    ray.y += ray.speed_y;
    calc_obj_pos(&ray);
    if (ray.main_etat == 2)
    {
        if (scroll_end_y - ymap > ray.speed_y) {
            if (ray.screen_y < 100) {
                v_scroll_speed = 0;
                return;
            }
            v_scroll_speed = ray.speed_y;
            return;
        }
        v_scroll_speed = 0;
    }
}

void recale_ray_pos(void) {
    /* 3103C 8015583C -O2 -msoft-float */
    //NOTE: is this function really called RecaleRayPosInJumelle in the PS1 version?? maybe not right?
    if (ray.main_etat == 3 && ray.sub_etat == 23) {
        return;
    }
    if (scroll_y == -1) {
    s16 unk_y_1;

        unk_y_1 = ((ray_zdc_y >> 1) - ray.offset_by) + 114; // 136 in the PS1 version
        if ((ray.main_etat == 2 && !(ray.sub_etat == 15 && ray_Suphelico_bis)) || screen_trembling) {
            if ((ymap != scroll_end_y && (unk_y_1 < ray.screen_y - ray.speed_y) && ray.speed_y > 0) ||
                (ymap != scroll_start_y && (-ray.offset_hy - 16 >= ray.screen_y) && ray.speed_y < 0))
            {
                if (ray.speed_y <= 16)
                    v_scroll_speed = ray.speed_y;
                else
                    v_scroll_speed = 0;
            }
        } else {
            if (v_scroll_speed != 255 || decalage_en_cours != 0)
            {
    s16 unk_y_2;
    s32 v_scr_temp;
    s32 spd_y_abs_1;

                unk_y_2 = ray.screen_y - unk_y_1;
                v_scr_temp = v_scroll_speed = ashr16(unk_y_2, 2);
                spd_y_abs_1 = Abs(ray.speed_y);
                if (Abs(v_scr_temp) >= spd_y_abs_1) {
    s16 spd_y_abs_2;

                    spd_y_abs_1 = max32(spd_y_abs_1, 3);
                    spd_y_abs_2 = spd_y_abs_1;
                    if (v_scr_temp > 0) {
                        v_scroll_speed = min32(v_scroll_speed, (s16) spd_y_abs_1);
                    } else if (v_scr_temp < 0) {
                        v_scr_temp = max32(v_scr_temp, -spd_y_abs_2);
                        v_scroll_speed = v_scr_temp;
                    } else {
                        if (unk_y_2 > 0)
                            v_scroll_speed = 1;
                        else if (unk_y_2 < 0)
                            v_scroll_speed = -1;
                    }
                }
            } else if (ray.main_etat != 1) {
                if (ray.screen_y < unk_y_1 + 48) {
                    v_scroll_speed = -4;
                } else {
                    v_scroll_speed = 0;
                }

            }
        }
    }

    if (scroll_x == -1) {
    s16 unk_x_1;
    s16 unk_x_2;

        unk_x_1 = (112 - ray.offset_bx) - special_ray_mov_win_x_left;
        unk_x_2 = special_ray_mov_win_x_right - (ray.offset_bx - 208);
        if (decalage_en_cours > 0 || ray.speed_x > 0) {
    s16 unk_x_3;

            unk_x_3 = ashr16(ray.screen_x - unk_x_1, 2);
            if (unk_x_3 >= dhspeed) {
                if (unk_x_3 > dhspeed) {
                    dhspeed++;
                }
            } else {
                dhspeed--;
            }
        } else if (decalage_en_cours < 0 || ray.speed_x < 0) {
    s16 unk_x_3;

            unk_x_3 = ashr16(ray.screen_x - unk_x_2, 2);
            if (unk_x_3 > dhspeed) {
                dhspeed++;
            } else if (unk_x_3 < dhspeed) {
                dhspeed--;
            }
        } else {
            if (ray.flags.flip_x) {
    s16 unk_x_3;

                unk_x_3 = ashr16(ray.screen_x - unk_x_1, 2);
                if (unk_x_3 >= dhspeed) {
                    if (unk_x_3 > dhspeed) {
                        dhspeed++;
                    }
                } else {
                    dhspeed--;
                }
            } else {
    s16 unk_x_3;

                unk_x_3 = ashr16(ray.screen_x - unk_x_2, 2);
                if (unk_x_3 > dhspeed) {
                    dhspeed++;
                } else if (unk_x_3 < dhspeed) {
                    dhspeed--;
                }
            }
        }

        if (dans_la_map_monde) {
            if (Abs(dhspeed) > 4) {
                dhspeed = dhspeed > 0 ? 4 : -4;
            }
        }

        h_scroll_speed += ashr16(dhspeed, 2);
        if ((unk_x_2 > ray.screen_x && ray.speed_x < 0) || (unk_x_1 < ray.screen_x && ray.speed_x > 0)) {
            h_scroll_speed += ray.speed_x;
        }
    }

    if (scroll_y == -1 && ray.follow_id != -1)
    {
    obj_t * other_obj;

        other_obj = &level.objects[ray.follow_id];
        if (flags[other_obj->type].raw & flags1_0x20_move_y) {
            v_scroll_speed += instantSpeed(other_obj->speed_y);
        } else {
            v_scroll_speed += other_obj->speed_y;
        }
        v_scroll_speed += other_obj->follow_y;
    }
}

void RAY_TO_THE_RIGHT(void) {
    s16 border;
    /* 31690 80155E90 -O2 -msoft-float */
    if (ray_mode == MODE_2_RAY_ON_MS && RayCoince(1)) {
        ray.speed_x = 0;
        decalage_en_cours = 0;
    }
    ray.x += ray.speed_x;
    border = RIGHT_MAP_BORDER;
    if (border < ray.x) {
        ray.x = border;
        ray.speed_x = 0;
        decalage_en_cours = 0;
        if (ray.main_etat != 2) {
            ray.speed_y = 0;
        }
    }
    calc_obj_pos(&ray);
}

void RAY_TO_THE_LEFT(void) {
    if (ray_mode == MODE_2_RAY_ON_MS && RayCoince(0)) {
        ray.speed_x = 0;
        decalage_en_cours = 0;
    }
    ray.x += ray.speed_x;
    if (scroll_x == -1) {
        s16 border = LEFT_MAP_BORDER;
        if (ray.x < border) {
            ray.x = border;
            decalage_en_cours = 0;
            if (ray.main_etat != 2) {
                ray.speed_y = 0;
            }
        }
    }
}

void DO_FIXE(void) {
    if (status_bar.max_hitp == 2) {
        status_bar.hp_sprites[1] = ray.hit_points;
        status_bar.hp_sprites[0] = 0;
    } else {
        if (ray.hit_points <= 2) {
            status_bar.hp_sprites[1] = ray.hit_points;
            status_bar.hp_sprites[0] = 0;
        } else {
            status_bar.hp_sprites[1] = 2;
            status_bar.hp_sprites[0] = ray.hit_points - 2;
        }
    }
    if (status_bar.lives == -1) {
        status_bar.lives_digits[1] = 0;
        status_bar.lives_digits[0] = 0;
    } else {
        status_bar.lives_digits[1] = status_bar.lives % 10;
        status_bar.lives_digits[0] = status_bar.lives / 10;
    }
    status_bar.num_wiz = MIN(99, status_bar.num_wiz);
    status_bar.wiz_digits[0] = status_bar.num_wiz / 10;
    status_bar.wiz_digits[1] = status_bar.num_wiz % 10;
}

void deactivate_ship_links(void) {
    obj_t *bateau_obj;
    s16 linked;

    /* 31D3C 8015653C -O2 -msoft-float */

    if (bateau_obj_id != -1) {
        bateau_obj = &level.objects[bateau_obj_id];
        if (bateau_obj->link_has_gendoor) {
            linked = link_init[bateau_obj->id];
            while (linked != bateau_obj->id) {
                level.objects[linked].flags.alive = 0;
                linked = link_init[linked];
            }
        }
    }
}

s16 linkListHoldsAGendoor(obj_t *obj)
{
    u8 res;
    s16 linked;

    res = 0;
    linked = link_init[obj->id];

    while (linked != obj->id)
    {
        if (level.objects[linked].type == TYPE_GENERATING_DOOR)
        {
            res = 1;
            break;
        }
        linked = link_init[linked];
    }
    return res;
}

void correct_gendoor_link(u8 a1) {
    s16 i;

 for (i = 0; i < level.nb_objects; ++i) {
    obj_t * obj;

        obj = level.objects + i;
        if (obj->type == TYPE_164_GENERATING_DOOR) {
    s16 link;
    s16 last_link;

            link = link_init[i];
            last_link = i;

            // NOTE: I'm not too sure about this part, might need checking
            while (link != i && link != last_link) {
                if (obj->flags.alive && obj->type == TYPE_164_GENERATING_DOOR) {
                    level.objects[link].flags.alive = 0;
                }
                last_link = link;
                link = link_init[link];
            }

            if (link == i) {
                link_init[last_link] = link_init[i];
                if (last_link == link_init[i]) {
                    level.objects[last_link].link_has_gendoor = 0;
                }
                obj->link = link_init[i];
                link_init[i] = i;
                obj->link_has_gendoor = 0;
            }

            if (a1 && !obj->flags.alive) {
    s16 init_link;
    s16 linked_obj_id;

                init_link = obj->link;

                do {
                    linked_obj_id = link_init[init_link];
                    suppressFromLinkList(level.objects + linked_obj_id);
                } while (init_link != linked_obj_id);
            }

        }
    }

}

void suppressFromLinkList(obj_t* obj) {
    s16 id;
    s16 linked_id;

    id = obj->id;
    linked_id = link_init[id];
    if (linked_id != id) {
        do {
            id = linked_id;
            linked_id = link_init[id];
        } while (linked_id != obj->id);
    }
    link_init[id] = link_init[obj->id];
    if (id == link_init[id]) {
        level.objects[id].link_has_gendoor = 0;
    }
    link_init[obj->id] = obj->id;
    obj->link_has_gendoor = 0;
}

void correct_link(void) {
    s32 i;

 for (i = 0; i < level.nb_objects; ++i) {
    obj_t * obj;

        obj = level.objects + i;
        if (link_init[i] == i) {
            obj->link_has_gendoor = 0;
        } else {
            if ((flags[obj->type].raw & flags3_0x40_no_link) ||
                    ((flags[obj->type].raw & flags3_0x20_link_requires_gendoor) && !linkListHoldsAGendoor(obj))) {
                suppressFromLinkList(obj);
            } else {
                obj->link_has_gendoor = 1;
            }
        }
    }
}

void INIT_RAY_BEGIN(void) {
    RayEvts.poing = 0;
    RayEvts.hang = 0;
    RayEvts.helico = 0;
    RayEvts.handstand_dash = 0;
    RayEvts.handstand = 0;
    RayEvts.grap = 0;
    RayEvts.run = 0;
    RayEvts.firefly = 0;
    RayEvts.force_run = 0;
    RayEvts.reverse = 0;
    RayEvts.squashed = 0;
    ray.flags.hurtbyfist = 0;
    status_bar.max_hitp = 2;
    status_bar.num_wiz = 0;
    fin_continue = 0;
}

void INIT_RAY(u8 new_lvl) {
    s32 i;

    gele = 0;
    compteur_attente = 0;
    ray_mode = (ray.main_etat == 6) ? MODE_2_RAY_ON_MS : MODE_1_RAYMAN;
    if (RayEvts.tiny) {
        rms.hit_points = ray.hit_points;
        ray = rms;
    }
    xmap = 0;
    ymap = 0;
    ray.type = TYPE_23_RAYMAN;
    ray.screen_x = 0;
    ray.screen_y = 0;
    ray.speed_x = 0;
    ray.speed_y = 0;
    ray.x = 100;
    ray.offset_hy = 20;
    ray.y = 10;
    ray.anim_index = 0;
    ray.anim_frame = 0;
    ray.main_etat = 2;
    ray.sub_etat = 2;
    ray.config = 0;
    ray.timer = 0;
    ray.offset_bx = 80;
    ray.offset_by = 78;
    ray.follow_id = -1;
    ray.link = -1;
    ray.iframes_timer = -1;
    ray.is_active = 1;
    ray.flags.alive = 1;
    ray.flags.flip_x = 1;
    ray.flags.flag_0x40 = 0;
    ray_stack_is_full = 0;
    no_ray_landing_smoke = 0;
    in_air_because_hit = 0;
    remoteRayXToReach = -32000;
    ray_last_ground_btyp = 1;
    ray_pos_in_stack = 0;
    ray_se_noie = 0;
    ray_in_fee_zone = 0;
    fin_dark = 0;
 for (i = 0; i < 10; ++i) {
        pos_stack[i] = ray.x;
    }
    poing.is_active = 0;
    poing.is_charging = 0;
    if (new_lvl) {
        RayEvts.super_helico = 0;
    }
    if (!(num_world == world_1_jungle && num_level == 9) && !ModeDemo && !record.is_recording) {
        RayEvts.magicseed = 0;
    }
    if (RayEvts.squashed) {
        RayEvts.tiny = RayEvts.squashed;
        Ray_RayEcrase(); //TODO
    }
    RayEvts.tiny = 0;
    RayEvts.force_run = 0;
    if (save1.rayevts_reverse && save1.save_obj_id != -1) {
        RayEvts.reverse = 0;
        if (num_world == world_6_cake && num_level == 3) {
            RAY_REVERSE_COMMANDS();
        }
    } else {
        //TODO: figure out what is happening here (seems to be wrong right now)
//        RayEvts.reverse = 1;
//        RAY_REVERSE_COMMANDS();
    }

    if (level.objects && level.nb_objects > 0) {
    obj_t * obj;
        s32 i;
    s32 found;

        obj = level.objects;
        found = 0;
 for (i = 0; i < level.nb_objects; ++i) {
            if (obj->type == TYPE_99_RAY_POS) {
                found = 1;
                break;
            }
            ++obj;
        }
        if (found) {
            if (!fin_continue) {
                ray.flags.alive = 0;
                ray.is_active = 0;
            }
            xmap = obj->x + obj->offset_bx - 160;
            ymap = obj->y - 10;
            if (GameModeVideo != MODE_NORMAL) {
                if (xmap >= xmapmax) {
                    xmap = xmapmax;
                }
                if (ymap >= ymapmax) {
                    ymap = ymapmax;
                }
                if (xmap < 0) {
                    xmap = 0;
                }
                if (ymap < 0) {
                    ymap = 0;
                }
            } else {
                set_xymap();
            }
            ray.x = obj->x + obj->offset_bx - ray.offset_bx;
            ray.y = obj->y + obj->offset_by - ray.offset_by;
        }
    }

    if (RaymanDansUneMapDuJeu) {
        calc_btyp(&ray);
    } else {
    s32 i;

        i = 0;
        do {
            ++i;
            ray.btypes[i - 1] = 0;
        } while (i < 5);
    }
}

s16 is_icy_pente(u8 btyp) {
    s32 result;

    result = (block_flags[btyp].raw & 0x40) != 0 && (block_flags[btyp].raw & 8) != 0;
    return result;
}

void STOPPE_RAY_EN_XY(void) {
    if (ray.main_etat != 7) {
    s16 x;
    s16 y;
    u8 btyp_1;
    u8 btyp_2;
    u8 btyp_3;
    u8 btyp_4;
    s16 stop;

        x = ray.offset_bx + ray.x;
        y = ray.offset_by + ray.y;
        if (block_flags[ray.btypes[0]].raw & 0x40) {
            y -= 8;
        }

        x += ray.speed_x * 2;
        btyp_1 = BTYP(x >> 4, (y - 8) >> 4);
        btyp_2 = BTYP(x >> 4, (y - 24) >> 4);
        btyp_3 = BTYP(x >> 4, (y - 40) >> 4);
        btyp_4 = BTYP(x >> 4, (y - 56) >> 4);

        stop = 0;
        if (block_flags[btyp_1].raw & 0x10)
            stop = !is_icy_pente(btyp_2);
        if (!(ray.eta[ray.main_etat][ray.sub_etat].flags & 0x40) && !RayEvts.tiny) {
            if (!stop && (((block_flags[btyp_2].raw & 0x10) && !is_icy_pente(btyp_3)) ||
                     ((block_flags[btyp_3].raw & 0x10) && !is_icy_pente(btyp_4)))
            ) {
                stop = 1;
            }
        }

        if (stop)
        {
            ray.speed_x = 0;
            decalage_en_cours = 0;
            ray.nb_cmd = 0;
            Reset_air_speed(1);
            Reset_air_speed(0);
        }
    }

}

void DO_PLACE_RAY(void) {
    h_scroll_speed = 0;
    v_scroll_speed = 0;
    ray.speed_x = 0;
    ray.speed_y = 0;

    if (rightjoy()) {
        ray.speed_x += RAY_MODE_SPEED;
    } else if (leftjoy()) {
        ray.speed_x -= RAY_MODE_SPEED;
    }
    if (downjoy()) {
        ray.speed_y += RAY_MODE_SPEED>>1;
    } else if (upjoy()) {
        ray.speed_y -= RAY_MODE_SPEED>>1;
    }

    if (ray.speed_y > 0) {
        move_down_ray();
    } else if (ray.speed_y < 0) {
        move_up_ray();
    }
    if (ray.speed_x < 0) {
        RAY_TO_THE_LEFT();
    } else if (ray.speed_x > 0) {
        RAY_TO_THE_RIGHT();
    }
    calc_obj_pos(&ray);
    h_scroll_speed = ray.speed_x;
    v_scroll_speed = ray.speed_y;
    SHOW_COLLISIONS_ZONES();
}

void DO_AUTO_SCROLL(void) {
    s16 i;
    obj_t * scr_obj;
    s16 scr_x;
    s16 ray_y;
    s16 ray_x;

    /* DOS 1:49fc1..49fd9: ray_x is the stack short; ray_y stays in DI.
       Read eau_obj_id only in case 3 (1:4a0b7 and 1:4a110). */

    ray_x = ray.x;
    ray_y = ray.y;

    scroll_x = 0;
    scroll_y = -1;
    for (i = 0; i < NumScrollObj; i++) {
        scr_obj = &level.objects[scroll_obj_id[i]];
        if (scr_obj->type != TYPE_64_SCROLL)
            break;

        switch (scr_obj->hit_points) {
            case 0:
                if (scr_obj->x <= ray_x)
                {
                    scroll_x++;
                    scr_obj->x = scr_obj->init_x - 200;
                }
                break;
            case 1:
                if (scr_obj->x <= ray_x)
                {
                    scroll_x--;
                    scr_obj->x = scr_obj->init_x - 200;
                }
                break;
            case 2:
                if (scr_obj->y <= ray_y)
                    scroll_y = -scroll_y;
                else
                    scr_obj->y = 10000;
                break;
            case 3:
                if (eau_obj_id == -1 || (level.objects[eau_obj_id].hit_points == 0 && level.objects[eau_obj_id].param == 0)) {
                    if (scr_obj->y <= ray_y)
                    {
                        if (Abs(scroll_y) == 1)
                            scroll_y = 2;
                        else
                            scroll_y = -scroll_y;
                    }
                    else
                        scr_obj->y = 10000;

                    if (eau_obj_id != -1 && scroll_y != 2)
                        level.objects[eau_obj_id].iframes_timer = 2;
                }
                break;
        }
    }

    if (scroll_x <= 0)
        scroll_x--;

    scr_x = scroll_x;
    if (scr_x > 0)
        h_scroll_speed = scr_x;
    else if (scr_x < -1)
        h_scroll_speed = scr_x + 1;

    if (scroll_y == 1)
        v_scroll_speed = -1;
    else
    {
        if (scroll_y == 2)
        {
            if (horloge[2] != 0)
                v_scroll_speed = -1;
            else
                v_scroll_speed = 0;
        }
        if (scroll_y == -2)
            scroll_y = -1;
    }
}

void INIT_MOTEUR(u8 new_lvl) {
    s16 i;
    obj_t *obj;

    i = 0;
    obj = level.objects;
    if (i < level.nb_objects) {
        do {
            ++obj;
            ++i;
            obj[-1].field_1C = 0;
        } while (i < level.nb_objects);
    }

    memset(pix_gerbe, 0, sizeof(pix_gerbe));

    if (!fin_continue) {
        init_flocons();
    }
    if (num_world == world_1_jungle && num_level == 9) {
        set_SNSEQ_list(11);
    }
    scrollLocked = 0;
    loop_time = 0;
    dans_la_map_monde = 0;
    sko_rayon_on = 0;
    rubis_list_calculated = 0;
    Vignet_To_Display = 0;
    sko_last_action = 0;
    sko_enfonce_enable = 0;
    joe_exp_probleme = finBosslevel.helped_joe_2;
    pixels_enfonce = 0;
    if (num_world == world_5_cave && num_level == 10) {
        mp.height = 32;
        scroll_end_y = 312;
        ymapmax = 312;
    }
    in_pause = 0;
    map_time = 0;
    left_time = -2;
    INIT_HORLOGES();
    ray.id = -1;
    actobj.objects[0] = -1;
    actobj.num_active_objects = 0;
    xmap = 0;
    ymap = 0;

    if (GameModeVideo != MODE_NORMAL) {
        ymapmax = 16 * mp.height - 200;
        xmapmax = 16 * mp.width - 305;
        if (xmapmax < 0) {
            xmapmax = 0;
        }
        if (ymapmax < 0) {
            ymapmax = 0;
        }
        scroll_start_x = 0;
        scroll_end_x = xmapmax;
        scroll_start_y = 0;
        scroll_end_y = ymapmax;
    } else {
        set_xymapini();
    }
    v_scroll_speed = 0;
    h_scroll_speed = 0;
    dvspeed = 0;
    dhspeed = 0;
    jump_time = 0;
    gerbe = 0;
    decalage_en_cours = 0;
    ray_wind_force = 0;
    screen_trembling = 0;
    helico_time = -1;
    weather_wind = 0;
    lidol_to_allocate = 0;
    fin_boss = 0;
    boss_mort = 0;
    TextDark2_Affiche = 0;
    if (!fin_du_jeu) {
        dead_time = 64;
    }
    INIT_RAY(new_lvl);
    if (!bonus_map && departlevel && save1.is_just_saved) {
    s16 saved_tings;

        saved_tings = status_bar.num_wiz;
        restoreGameState(&save1);
        status_bar.num_wiz = saved_tings;
    } else {
        actualize_palette(0); // NOTE: Added to prevent wrong palette being used at start of level (is there a bug elsewhere?)
    }
    if (departlevel) {
        nb_wiz_collected = 0;
    } else {
        save1.is_just_saved = save_save;
        ray.x = ray_X_main_pos_init;
        ray.y = ray_Y_main_pos_init;
        ray.is_active = 1;
        ray.flags.alive = 1;
        current_pal_id = pal_id_at_wizard + 1;
        actualize_palette(pal_id_at_wizard);
        xmap = xmapsave;
        ymap = ymapsave;
    }
    INIT_OBJECTS(new_lvl);
    correct_link();
    deactivate_ship_links();
    special_flags_init();
    if (num_world_choice == world_5_cave && num_level_choice == 4 && finBosslevel.helped_joe_1) {
        RayEvts.firefly = 1;
    } else {
        RayEvts.firefly = 0;
    }
    if (RayEvts.firefly) {
        INIT_LUCIOLE(); //TODO
    }
    DO_OBJECTS();
    if (mp.width <= 20 && (SizeScreen == 0 || GameModeVideo == MODE_X)) {
        scroll_start_x = scroll_end_x;
        xmap = scroll_end_x;
    }
    if (type_fndscrX != 0) {
        scroll_end_x = scroll_start_x;
        xmap = scroll_start_x;
    }
}

void INIT_MOTEUR_BEGIN(void) {
    init_allowed_time();
    init_bonus_perfect();
    init_finBossLevel();
    init_calcbloc_func();
    new_level = 1;
    new_world = 1;
    status_bar.lives = 3;
    ray.hit_points = 2;
    poing.sub_etat = 1;
    departlevel = 1;
    nb_continue = 9;
    gele = 0;
    You_Win = 0;
    set_proj_center(160, 170);
    INIT_RAY_BEGIN();
    scroll_x = -1;
    scroll_y = -1;
    special_ray_mov_win_x_left = 0;
    special_ray_mov_win_x_right = 0;
    fin_de_rayman = 0;
    NumDemo = myRand(5);
    if (DemoRecordWorld[NumDemo] == 5 && DemoRecordMap[NumDemo] == 10) {
        ++NumDemo;
        if (NumDemo == 6) {
            NumDemo = 0;
        }
    }
    First_Hist = 1;
    First_Menu = 1;
    left_time = -2;
    life_becoz_wiz = 0;
    dontdoit = 0;
    RunTimeDemo = 1800;
}

void INIT_MOTEUR_WORLD(void)
{
    new_world = 0;
    new_level = 1;
}

void INIT_MOTEUR_LEVEL(s32 a1) {
    Bloc_lim_W1_Aff = Bloc_lim_W1;
    Bloc_lim_W2_Aff = Bloc_lim_W2;
    Bloc_lim_H1_Aff = Bloc_lim_H1;
    Bloc_lim_H2_Aff = Bloc_lim_H2;
    if (!fin_du_jeu) {
    u8 need_save_game_state;

        bonus_map = (get_allowed_time() != -2);
        need_save_game_state = (!bonus_map && departlevel && !fin_continue);
        if (need_save_game_state) {
            initGameSave();
            MapAvecPluieOuNeige = 0;
        }
        INIT_MOTEUR(a1); //TODO
        if (need_save_game_state) {
            saveGameState(NULL, &save1);
        }
        if (!departlevel) {
            departlevel = 1;
            restore_objects_flags();
        } else {
            nb_wiz_collected = 0;
        }
        correct_gendoor_link(0);
        new_level = 0;
        ray_mode = MODE_1_RAYMAN;
        RAY_MODE_SPEED = 16;
        build_active_table();
    }
}

void restore_gendoor_link(void) {
    obj_t * temp_v1;
    obj_t * var_v0_2;
    s16 var_v0_1;
    s32 temp_a0_3;
    s32 var_a0;
    s16 temp_a0_1;
    u16 temp_a0_2;
    s16 var_a1;
    u32 var_v1;
    u8 temp_a1;
    s16 test_1;
    s32 i;

    /* 34E30 80159630 -O2 -msoft-float */ //NB: nonmatching

    test_1 = level.nb_objects;
 for (i = 0; i < level.nb_objects; ++i) {
    obj_t * obj;

        obj = level.objects + i;
        if (obj->type == TYPE_164_GENERATING_DOOR)
        {
            if (obj->flags.alive) {
                var_a1 = obj->link;
                do
                {
                    temp_a0_1 = var_a1;
                    var_a1 = link_init[var_a1];
                } while (var_a1 != obj->link);
                link_init[obj->id] = var_a1;
                link_init[temp_a0_1] = obj->id;
                obj->link_has_gendoor = 1;
                level.objects[temp_a0_1].link_has_gendoor = 1;
                continue;
            }
            temp_a0_2 = (u16) obj->id;
            var_a0 = temp_a0_2 << 0x10;
            if ((s16) temp_a0_2 != save1.link_init[temp_a0_2])
            {
                do
                {
                    temp_a0_3 = var_a0 >> 0x10;
                    temp_a1 = save1.link_init[temp_a0_3];
                    link_init[temp_a0_3] = temp_a1;
                    if (temp_a0_3 != temp_a1)
                    {
                        level.objects[temp_a0_3].link_has_gendoor = 1;
                    }
                    else
                    {
                        level.objects[temp_a0_3].link_has_gendoor = 0;
                    }
                    var_a0 = temp_a1 << 0x10;
                } while (obj->id != (s16) temp_a1);
            }
        }
    }
}

void DONE_MOTEUR_LEVEL(void) {
    /* 34FEC 801597EC -O2 -msoft-float */
    restore_gendoor_link();
    if (!bonus_map && departlevel && get_next_bonus_level(num_level) == 0 && !fin_continue) {
        doneGameSave();
    }
    if (rayman_obj_id != -1 && (new_level || new_world)) {
        raytmp = ray;
        ray = rms;
        INIT_RAY(1);
        ray.hit_points = raytmp.hit_points;
        ray.init_hit_points = raytmp.init_hit_points;
        ray.hit_sprite = raytmp.hit_sprite;
        set_main_and_sub_etat(&ray, 0, 0);
        ray_mode = MODE_0_NONE;
    }

    if (first_boss_meet) {
        Change_Wait_Anim();
    }

    if (fee_obj_id != -1) {
        record.is_playing = 0;
    }
    align_mem_desc(&main_mem_level);
}

void INIT_MOTEUR_DEAD(void) {
    if (dead_time == 0) {
        restore_gendoor_link();
        INIT_MOTEUR(0);
        correct_gendoor_link(0);
    }
    if (!bonus_map && departlevel) {
        saveGameState(NULL, &save2);
        life_becoz_wiz = 0;
    }
}

void INIT_RAY_ON_MS(void) {
    if (NewMs) {
        if (rayman_obj_id != -1) {
            rms = ray;
            ray = level.objects[rayman_obj_id];
            INIT_RAY(1);
            ray.hit_points = rms.hit_points;
            ray.init_hit_points = rms.init_hit_points;
            ray.hit_sprite = rms.hit_sprite;
            set_main_and_sub_etat(&ray, 6, 0);
            ray_mode = MODE_2_RAY_ON_MS;
            NewMs = 0;
        } else {
            if (rayOnMsWasThere) {
                INIT_RAY(1);
                NewMs = 0;
            }
        }
    }
}

void DO_MOTEUR(void) {
    if (ModeDemo != 0) {
    s32 has_joystick_input;

        has_joystick_input = inp(0x201);
        if (!(has_joystick_input & 0x10) || !(has_joystick_input & 0x20) || !(has_joystick_input & 0x40) || !(has_joystick_input & 0x80) || ModeDemo == 3 || ModeDemo == 4) {
            new_world = 1;
            DO_FADE_OUT();
            ModeDemo = 2;
        }
    }
    PC_do_cheats();
    horloges(1);
    calc_left_time();
    ray_old_main_etat = ray.main_etat;
    ray_old_sub_etat = ray.sub_etat;
    xmap_old = xmap;
    ymap_old = ymap;

    if (ModeVideoActuel == MODE_X) {
        RIGHT_MAP_BORDER = xmap + 320 - ray.offset_bx - 36;
        LEFT_MAP_BORDER = xmap + 20 - ray.offset_bx;
    } else {
        RIGHT_MAP_BORDER = xmap + Bloc_lim_W2 - ray.offset_bx - 36;
        LEFT_MAP_BORDER = xmap + Bloc_lim_W1 + 20 - ray.offset_bx;
    }

    if (ray.follow_id != -1) {
        level.objects[ray.follow_id].display_prio = oldPrio;
    }

    if (RayEvts.firefly) {
        DO_LUCIOLE(); //TODO
    }

    if (num_world != world_3_mountain) {
        ray_wind_force = weather_wind;
    } else {
        ray_wind_force = 0;
    }

    DO_OBJECTS();
}

void DO_MOTEUR2(void) {
    DO_WIZ_AFTER_BONUS_MAP();
    DO_PERFECT_BONUS_MAP();
    DO_OBJECTS_ANIMS();
    if (ray.flags.alive) {
        if (dead_time != 64) {
            ray.follow_id = -1;
        }
        switch(ray_mode) {
            case MODE_1_RAYMAN: {
                DO_RAYMAN();
            } break;
            case MODE_2_RAY_ON_MS: {
                if (ray.main_etat != 6) {
                    set_main_and_sub_etat(&ray, 6, 0);
                }
                DO_RAY_ON_MS();
            } break;
            case MODE_3_MORT_DE_RAYMAN:
            case MODE_4_MORT_DE_RAYMAN_ON_MS: {
                DO_MORT_DE_RAY();
            } break;
            case MODE_5_CASSE_BRIQUE: {
                DO_RAY_CASSE_BRIQUE(); //TODO
            } break;
            default: {
                // NOTE: ray_mode is set to negative when the free movement cheat is enabled.
                DO_PLACE_RAY();
            } break;
        }
        if (NumScrollObj <= 0 || ray_mode <= MODE_0_NONE) {
            scroll_x = -1;
            scroll_y = -1;
        } else {
            DO_AUTO_SCROLL();
        }
        MOVE_OBJECTS();
        calc_obj_pos(&ray);
        if (scroll_y != -1 && v_scroll_speed == 255) {
            v_scroll_speed = 0;
        }
        if (ray_mode > MODE_0_NONE) {
            recale_ray_pos();
        }
        if (ray_mode == MODE_2_RAY_ON_MS) {
            if (scroll_x != -1)
            {
                scroll_x = ray.speed_x;
                ray.speed_x = h_scroll_speed;
                if (h_scroll_speed > 0)
                    RAY_TO_THE_RIGHT();
                else if (h_scroll_speed < 0)
                    RAY_TO_THE_LEFT();
                ray.speed_x = scroll_x;
            }
            if (ray.sub_etat != 14)
            {
                switch (ray.sub_etat)
                {
                    case 0:
                        if (Abs(ray.speed_x) >= 2 || Abs(h_scroll_speed) >= 2)
                            set_sub_etat(&ray, 6);
                        break;
                    case 1:
                        if (Abs(ray.speed_x) < 2 && Abs(h_scroll_speed) < 2)
                            set_sub_etat(&ray, 7);
                        else
                        {
                            if (Abs(h_scroll_speed) >= 5 && (xmap < xmapmax - 100))
                            {
                                set_sub_etat(&ray, 2);
                                start_cd_gros_rayman(); // change_audio_track_moskito_acc();
                            }
                        }
                        break;
                    case 5:
                        if (Abs(h_scroll_speed) < 5)
                        {
                            set_sub_etat(&ray, 3);
                            start_cd_gros_rayman(); // change_audio_track_moskito_dec();
                        }
                        break;
                }
                if (xmap == xmapmax)
                    scroll_start_x = xmap;
            }
            poing_obj->x += h_scroll_speed;
            poing_obj->y += v_scroll_speed;
        }
        DO_SCROLL(&h_scroll_speed, &v_scroll_speed);
        build_active_table();
        RECALE_ALL_OBJECTS();
        if (ray.follow_id != -1) {
    obj_t * v20;

            v20 = level.objects + ray.follow_id;
            oldPrio = v20->display_prio;
            if (oldPrio > 3) {
                v20->display_prio = 3;
            }
        }
    } else {
        MOVE_OBJECTS();
        scroll_y = -1;
        build_active_table();
        if (dead_time != 0) {
            // TODO: solve ray.flags.alive set to 0 at start of game?
            --dead_time;
        }
    }
    DO_FIXE();
    if (MapAvecPluieOuNeige) {
        do_flocons(xmap, ymap, xmap_old, ymap_old);
        DO_SNOW_SEQUENCE();
    }
    do_pix_gerbes();
    if (RayEvts.tiny) {
        set_zoom_mode(0);
        if (RayEvts.squashed && (ray.iframes_timer == -1 || ray.scale & ~0x100)) {
            if (ray.main_etat != 5) {
                if (NbTramesEcrase != 0) {
                    --NbTramesEcrase;
                } else {
                    ray.scale = max32(0, ray.scale - 5);
                }
            }
        } else {
            ray.scale = 256;
            NbTramesEcrase = 200;
        }
        if (ray.flags.flip_x) {
            set_proj_center(ray.screen_x - 8 + ray.offset_bx, ray.screen_y + ray.offset_by);
        } else {
            set_proj_center(ray.screen_x + ray.offset_bx, ray.screen_y + ray.offset_by);
        }
    }
}

void RAY_REVERSE_COMMANDS(void) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    if (RayEvts.reverse == 0) {
        RayEvts.reverse = 1;
        if (star_ray_der != NULL) {
            star_ray_der->is_active = 1;
            star_ray_der->flags.alive = 1;
            star_ray_der->display_prio = 3;
        }
        if (star_ray_dev != NULL) {
            star_ray_dev->is_active = 1;
            star_ray_dev->flags.alive = 1;
            star_ray_dev->display_prio = 1;
        }
    } else {
        RayEvts.reverse = 0;
        if (star_ray_der != NULL) {
            star_ray_der->is_active = 0;
            star_ray_der->flags.alive = 0;
            star_ray_der->display_prio = 3;
        }
        if (star_ray_dev != NULL) {
            star_ray_dev->is_active = 0;
            star_ray_dev->flags.alive = 0;
            star_ray_dev->display_prio = 3;
        }
    }
}

void RAY_DEMIRAY(void) {
    RayEvts.tiny = !RayEvts.tiny;
    if (RayEvts.tiny) {
        rms = ray;
        ray = level.objects[reduced_rayman_id];
        ray.animations = rms.animations;
        PlaySnd(206, -1);
    } else {
        raytmp = rms;
        rms = ray;
        ray = raytmp;
        PlaySnd(207, -1);
    }
    ray.type = TYPE_RAYMAN;
    ray.screen_x = rms.screen_x;
    ray.screen_y = rms.screen_y;
    ray.speed_x = rms.speed_x;
    ray.speed_y = rms.speed_y;
    ray.x = rms.x;
    ray.y = rms.y;
    ray.offset_bx = rms.offset_bx;
    ray.offset_by = rms.offset_by;
    ray.offset_hy = rms.offset_hy;
    ray.anim_index = rms.anim_index;
    ray.anim_frame = rms.anim_frame;
    ray.flags.flip_x = rms.flags.flip_x;
    ray.param = rms.param;
    ray.main_etat = rms.main_etat;
    ray.sub_etat = rms.sub_etat;
    ray.follow_id = rms.follow_id;
    ray.flags.alive = 1;
    ray.is_active = 1;
    ray.link = rms.link;
    ray.iframes_timer = rms.iframes_timer;
    ray.config = rms.config;
    ray.timer = rms.timer;
    ray.hit_points = rms.hit_points;
    ray.init_hit_points = rms.init_hit_points;
    ray.hit_sprite = rms.hit_sprite;

    if (block_flags[calc_typ_trav(&ray, 2)].raw & 0x10) {
        set_main_and_sub_etat(&ray, 0, 15);
    }
}

void Ray_RayEcrase(void)
{
    if (RayEvts.tiny == RayEvts.squashed) {
        RayEvts.squashed = !RayEvts.squashed;
        RayEvts.tiny = RayEvts.squashed;
        if (RayEvts.squashed) {
            rms = ray;
            ray = level.objects[reduced_rayman_id];
            ray.animations = rms.animations;
            ray.img_buffer = rms.img_buffer;
            ray.sprites = rms.sprites;
            SauveRayEvts = RayEvts;
            RayEvts.run = 0;
            RayEvts.force_run = 0;
            RayEvts.reverse = 0;
            RayEvts.squashed = 1;
            RayEvts.tiny = RayEvts.squashed;
            RayEvts.hang = 0;
            RayEvts.helico = 0;
            RayEvts.super_helico = 0;
            RayEvts.handstand_dash = 0;
            RayEvts.handstand = 0;
            RayEvts.grap = 0;
        } else {
            raytmp = rms;
            rms = ray;
            RayEvts = SauveRayEvts;
            ray = raytmp;
            RayEvts.squashed = 0;
            RayEvts.tiny = RayEvts.squashed;
        }
        ray.screen_x = rms.screen_x;
        ray.screen_y = rms.screen_y;
        ray.speed_x = rms.speed_x;
        ray.speed_y = rms.speed_y;
        ray.x = rms.x;
        ray.y = rms.y;
        ray.offset_bx = rms.offset_bx;
        ray.offset_by = rms.offset_by;
        ray.offset_hy = rms.offset_hy;
        ray.anim_index = rms.anim_index;
        ray.anim_frame = rms.anim_frame;
        ray.flags.flip_x = rms.flags.flip_x;
        ray.param = rms.param;
        ray.main_etat = rms.main_etat;
        ray.sub_etat = rms.sub_etat;
        ray.follow_id = rms.follow_id;
        ray.type = TYPE_RAYMAN;
        ray.flags.alive = 1;
        ray.is_active = 1;
        ray.link = rms.link;
        ray.iframes_timer = rms.iframes_timer;
        ray.config = rms.config;
        ray.timer = rms.timer;
        ray.hit_points = rms.hit_points;
        ray.init_hit_points = rms.init_hit_points;
        ray.hit_sprite = rms.hit_sprite;

        if (block_flags[calc_typ_trav(&ray, 2)].raw & 0x10) {
            set_main_and_sub_etat(&ray, 0, 15);
        }
    }
}

void DO_MOTEUR_GELE(void) {
    if (gele != 3) {
    s16 i;
    obj_t *obj;

        if (gele == 2) {
            horloges(1);
            TEST_SIGNPOST();
            h_scroll_speed = 0;
            v_scroll_speed = 0;
            DO_ANIM(&ray);
        } else {
            DO_MEDAILLON_TOON_GELE();
        }
        if (MapAvecPluieOuNeige) {
            do_flocons(xmap, ymap, xmap_old, ymap_old);
            DO_SNOW_SEQUENCE();
            ProchainEclair = 1;
            numero_palette_special = 0;
        }
    i = 0;
        obj = &level.objects[actobj.objects[i]];
        while (i < actobj.num_active_objects) {
            if ((flags[obj->type].raw & flags3_8_switch_off) || (obj->type == TYPE_161_WIZ && obj->sub_etat == 23) ||
                    (obj->type == TYPE_83_EXPLOSION && obj->sub_etat == 1) ||
                    (obj->type == TYPE_33_DARK2_SORT && obj->sub_etat != 35)) {
                DO_ANIM(obj);
                switchOff(obj);
            }
            i++;
            obj = &level.objects[actobj.objects[i]];
        }
    }
    build_active_table();
}
