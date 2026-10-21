/* 1:06f80..1:0cee0: action.c, affiche.c, araignee.c, pcmain.c (init, main loops, display, spider); four original objects by DOS boundaries (build/workers/tu_action/source_objects.md) - kept in one file until split. Names RAYVERSE/ANDROID; seeded by tools/seed.py, worker tu_action. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "palette.h"
#include "action_local.h"
#include "inline_helpers.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <i86.h>

void display_etoile(s16 x, s16 y);
void display_text(const char *text, s16 x, s16 y, u8 font_size, u8 color);
#ifndef LEN
#define LEN(x) (sizeof(x) / sizeof(x[0]))
#endif
#ifndef MAX_1
#define MAX_1(x, y) ((x) > (y) ? (x) : (y))
#endif
#ifndef MIN_1
#define MIN_1(x, y) ((x) < (y) ? (x) : (y))
#endif
#ifndef ashl16
#define ashl16(v, b) ((v) << (b))
#endif

#define BLOCK_FULLY_SOLID 0
#define BLOCK_SIZE 16
#define BLOCK_SOLID 1
#define GO_LEFT 0
#define GO_RIGHT 1
#define GO_WAIT 2
#define MODE_NORMAL 0
#define MODE_X 1
#define SCREEN_HEIGHT 200
#define SCREEN_WIDTH 320
#define MODE_5_CASSE_BRIQUE 5
#define SC_ESCAPE 1
#define TYPE_BB1 46
#define TYPE_BB12 198
#define TYPE_DARD_PLAFOND 196
#define TYPE_DARK 212
#define TYPE_HYBRIDE_MOSAMS 218
#define TYPE_HYBRIDE_STOSKO 223
#define TYPE_HYB_BBF2_D 231
#define TYPE_MAMA_PIRATE 187
#define TYPE_MOSKITO 50
#define TYPE_MOSKITO2 227
#define TYPE_SAXO 74
#define TYPE_SAXO2 180
#define TYPE_SCORPION 150
#define TYPE_SPACE_MAMA 98
#define TYPE_SPACE_MAMA2 213
extern s32 Bloc_lim_H1;
extern s32 Bloc_lim_H1_Aff;
extern s32 Bloc_lim_H2;
extern s32 Bloc_lim_W1;
extern s32 Bloc_lim_W2;
extern u8 CarteSonAutorisee;
extern u8 *DrawBufferNormal;
extern u8 FondAutorise;
extern u8 GameModeVideo;
extern u8 HVersionWindows;
extern u8 JumelleEffetAutorise;
extern u8 ModeNormalAutorise;
extern u8 ModeVideoActuel;
extern u8 MusicCdActive;
extern u8 NVersionWindows;
extern u8 OptionMusicCdActive;
extern u8 P486;
extern u8 PC_TingsToGet_Col;
extern u8 PC_TingsToGet_ColAdd;
extern u8 *PLAN2BIT;
extern u8 *PLAN0BIT;
extern u8 SonLimite;
extern u8 TheProcesseur;
extern u32 TailleMainMemFix;
extern u32 TailleMainMemLevel;
extern u32 TailleMainMemSprite;
extern u32 TailleMainMemTmp;
extern u32 TailleMainMemWorld;
extern u8 ToutSpriteAutorise;
extern u8 WindowsLance;
extern s32 alternateBossSpeedFactor;
extern u8 block_flags[64];
extern s16 bossScrollEndX;
extern s16 bossScrollEndY;
extern s16 bossScrollStartX;
extern s16 bossScrollStartY;
extern s32 bossSpeedFactor;
extern s16 bossXToReach;
extern s16 bossYToReach;
extern u8 currentBossActionIsOver;
extern s16 current_star;
extern s16 debut_options;
extern u8 display_txt_fee;
extern u8 *draw_buffer;
extern u8 *display_buffer;
extern u8 MenuCredits;
extern u8 MessageProgram;
extern s16 ecarty;
extern s16 fichier_a_copier;
extern s16 cb_ball_obj_id;
extern s16 ray_mode;
extern s16 id_Cling_Old;
extern s16 fixon;
extern s16 fixontemp;
extern u8 dead_time;
extern u8 fin_du_jeu;
extern status_bar_t status_bar;
extern u8 nb_cymbal_in_map;
extern s16 cymbal_obj_id[2];
extern s16 NumScrollObj;
extern s16 scroll_obj_id[50];
extern s16 display_mode;
extern s16 num_world;
extern s16 num_level;
extern active_objects_t actobj;
extern obj_type_flags_t flags[262];
extern s16 pente[16];
extern rayevts_t RayEvts;
extern s16 floorLine;
extern u8 horloge[25];
extern s16 id_Cling_1up;
extern s16 id_Cling_Pow;
extern s16 id_obj_grapped;
extern char *language_txt[300];
extern level_t level;
extern s32 map_time;
extern s32 plan0bit_length;
extern u32 plan0bit_nb_bytes;
extern u8 type_fndscrX;
extern u8 DosDetect;
extern u8 HVerDos;
extern u8 LVerDos;
extern char demo_video_mem[100];
extern s32 planar_copy_offset;
extern obj_t mapobj[25];
extern u8 nb_wiz;
extern u8 old_txt_fee;
extern s16 png_or_fee_id;
extern poing_t poing;
extern obj_t *poing_obj;
extern s16 poing_obj_id;
extern u8 position_ray;
extern s16 positiony;
extern obj_t ray;
extern s16 sbar_obj_id;
extern u8 scrollLocked;
extern s16 scroll_end_x;
extern s16 scroll_start_x;
extern display_item_t text_to_display[11];
extern display_item_t menu_to_display[10];
extern loadinforay_t LoadInfoRay[3];
extern obj_t div_obj;
extern obj_t mapobj[25];
extern obj_t clock_obj;
extern s16 option_exit;
extern s16 ypadmin;
extern s16 compteur;
extern s16 position;
extern s16 debut_sortie;
extern s16 debut_options;
extern s16 ecarty;
extern s16 fichier_a_copier;
extern s16 positiony;
extern s16 loop_timing;
extern u8 nb_continue;
extern u8 pos_YN;
extern s16 floc_ind[8];
extern s16 nb_floc[8];
extern s16 PROJ_CENTER_X;
extern s16 PROJ_CENTER_Y;
extern s16 num_world;
extern u8 language;
extern u8 BackgroundOn;
extern s16 plan0_width;
extern s16 plan0_height;
extern s16 yp0;
extern s16 VGA_FREQ;
extern s16 During_The_Menu;
extern u8 fin_de_rayman;
extern u8 fin_du_jeu;
extern u8 ExitMenu;
extern u8 RaymanDansUneMapDuJeu;
extern u8 fin_continue;
extern s32 MapTimePause;
extern s16 ProchainEclair;
extern s16 numero_palette_special;
extern s16 new_level;
extern s16 new_world;
extern u8 ModeDemo;
extern u8 use_sync;
extern u8 need_timer;
extern u8 Mode_Pad;
extern u8 Main_Control;
extern u8 *DisplayBufferNormal;
extern u8 dead_time;
extern s16 MapAvecPluieOuNeige;
extern u8 is_fee;
extern s32 left_time;
extern u8 JeuCracker;
extern u8 MessageProgram;
extern u8 Vignet_To_Display;
extern s16 nb_fade;
extern u8 GoMenu;
extern u8 fade;
extern u8 in_pause;
extern s16 num_level;
extern s16 num_world_choice;
extern u8 Frequence;
extern u8 NBRE_SAVE;
extern u8 SonLimite;
extern s16 xmap;
extern s16 xmapmax;
extern s16 xmap_old;
extern s16 ymap;
extern s16 ymapmax;
extern s16 ymap_old;
extern rgb_palette_t rvb_plan3;
extern u8 byte_CFA2A;
extern rgb_palette_t *rvb_special;
extern flocon_t *flocon_tab;
extern record_t record;
extern big_map_t BIG_MAP;
extern u8 gele;
extern font_t alpha_numbers;
extern init_data_state_t init_data_state;
extern font_t alpha;
extern font_t alpha2;
extern u8* new_sprite_deform;
extern grp_star_t grp_stars[30];
extern pix_gerbe_t pix_gerbe[8];
extern draw_func_t *DrawSpriteNormalEtX;
extern draw_func_t *DrawSpriteFlipNormalEtX;
extern draw_func_t *DrawSpriteNoClipNormalEtX;
extern draw_func_t *DrawSpriteFlipNoClipNormalEtX;
extern draw_func_t *DrawSpriteColorNormalEtX;
extern fplot_func_t *fplotNormalETX;
s32 calc_let_Width(u8 font_size, s32 num_let);
s32 deter_num_let(u8 c, const char* next_chars);
s16 calc_largmax_text(const char* text, s16 char_index, s16 space_width,
                      s16 char_spacing, u8 font_size);
s16 get_proj_x(s16 scale, s16 x);
s16 get_proj_y(s16 scale, s16 y);
s16 get_proj_dist(s16 scale, s16 distance);
u8 BTYP(s16 tile_x, s16 tile_y);
void Bresenham(void (*func)(s16, s16), s16 origin_x, s16 origin_y,
               s16 dest_x, s16 dest_y, s16 param_6, s16 percent);
void CALC_MOV_ON_BLOC(obj_t* obj);
void DrawBlackBoxNormal(u8* buffer, s32 x, s32 y, s32 height, s32 width);
void DrawBorderBoxNormal(u8* buffer, s32 x, s32 y, s32 height, s32 width, u16 colors);
void DrawFondBoxNormal(u8* buffer, s32 x, s32 y, s32 height, s32 width, u8 fond_type);
void DrawWldPointPlan2Normal(u8* buffer, s32 x, s32 y);
u8 GET_SPRITE_POS(obj_t* obj, s16 index, s16* x, s16* y, s16* w, s16* h);
void InitTextMode(void);
void AllocVariablesAutorisee(void);
void* JumelleCalloc(u32 count, u32 size);
void FatalError(char* message);
s32 get_avail_memory(void);
void calc_obj_pos(obj_t* obj);
void set_proj_center(s16 x, s16 y);
void draw_flocon7_Normal(s16 x, s16 y);
void draw_flocon6_Normal(s16 x, s16 y);
void draw_flocon5_Normal(s16 x, s16 y);
void draw_flocon4_Normal(s16 x, s16 y);
void draw_flocon3_Normal(s16 x, s16 y);
void draw_flocon2_Normal(s16 x, s16 y);
void draw_flocon1_Normal(s16 x, s16 y);
void draw_pluie7_Normal(s16 x, s16 y);
void draw_pluie6_Normal(s16 x, s16 y);
void draw_pluie5_Normal(s16 x, s16 y);
void draw_pluie4_Normal(s16 x, s16 y);
void InitData(u8 x, u8 y, s16 origin_x, s16 origin_y);
void update_map(s32 x, s32 y, s32 width, s32 height);
void calc_off_fnd(void);
void sub_3C3BC(s16 x, s16 y);
void init_arg(s32 argc, char** argv);
void PrintDosInfo(void);
void PcMain(void);
void DO_MAIN_LOOP_PC_NORMAL(u8* key_state);
void DO_MAIN_LOOP_PC_X(void);
void DO_GROS_MOTEUR_X(void);
void DISPLAY_SAVE_SPRITES(s16 x, s16 save_index);
void DISPLAY_OPTIONS_POING(void);
void DISPLAY_YESNO_POING(void);
void DISPLAY_CONTINUE_SPR(void);
void calc_num_let_spr(u8 font_size, u8* sprite_index);
void draw_sprite_deform(u8* source, u8* destination, deform_blit_t* geometry);
#pragma aux draw_sprite_deform parm [esi] [edi] [ecx]
void AFFICHE_SPRITE_DEFORME(sprite_t* sprite, u8* image, s16 x0, s16 y0,
                            s16 x1, s16 y1, s16 x2, s16 y2, s16 x3, s16 y3);
void DISPLAY_DEFORM_SPRITE(sprite_t* sprite, u8* image, s16 x, s16 y,
                           s16 angle, s16 scale_x, s16 scale_y);
void DISPLAY_DEFORMATION(obj_t* obj, s16 angle, s16 scale_x, s16 scale_y);
void CALCUL_DEFORMATION(s16* x, s16* y, s16 angle, s16 scale_x, s16 scale_y);
void sinus_cosinus(s16 angle, s16* sine, s16* cosine);
void manage_snd(void);
void do_record(record_t* rec);
void DO_MOTEUR_GELE(void);
void DO_MOTEUR(void);
void DO_MOTEUR2(void);
void get_luciole(void);
void update_display_map(big_map_t* map);
void DO_DARK2_AFFICHE_TEXT(void);
void DISPLAY_FIXE_CB(obj_t* obj);
void ClearBorder(s32 h1, s32 h2, s32 w1, s32 w2);
void DISPLAY_BLACKBOX(s16 x, s16 y, s16 width, s16 height, s16 font_size, u8 is_fond);
void DISPLAY_DEFORMATION(obj_t* obj, s16 a2, s16 a3, s16 a4);
s32 cosinus(s32 x);
void SET_X_SPEED(obj_t* obj);
void TEST_FIN_BLOC(obj_t* obj);
void add_alwobj(obj_t* obj);
u8 calc_typ_travd(obj_t* obj, u8 a2);
void display(obj_t *obj);
void display_emptypicture(void);
s16 dist_to_bloc_floor(s16 btyp, s16 x, s32 y);
void draw_sprite(sprite_t *sprite, s16 x, s16 y, u8 param_4);
s16 get_center_x(obj_t* obj);
s16 get_center_y(obj_t* obj);
void get_sprite_clipping(s32* xmin, s32* xmax, s32* ymin, s32* ymax);
void obj_hurt(obj_t* target);
s16 on_block_chdir(obj_t* obj, s16 center_x, s16 center_y);
void pushToLabel(obj_t *obj, u8 label, u8 skip_label_cmd);
void set_main_and_sub_etat(obj_t* obj, u8 etat, u8 sub_etat);
void set_sub_etat(obj_t* obj, u8 sub_etat);
void skipToLabel(obj_t* obj, u8 label, u8 skip_label_cmd);
void sprite_clipping(s32 xmin, s32 xmax, s32 ymin, s32 ymax);
s16 test_allowed(obj_t* obj, s16 center_x, s16 center_y);
void InitMemoryVariable(void);
void AfficheEntete(void);
int main_Ray(int argc, char** argv);
void setBossReachingSpeeds(obj_t *obj, u8 horloge_ind, u8 unk_x, u8 unk_y);
s16 testActionEnd(obj_t *obj);
s16 firstFloorBelow(obj_t* obj);
void adjustBossScrollLocker(void);
void setBossScrollLimits(obj_t* obj);
void display_sprite(obj_t *param_1, u8 sprite, s16 x, s16 y, u8 param_5);
void DISPLAY_POING(void);
void DISPLAY_CLING(void);
void display_bar_boss(obj_t *boss_obj);
void DisplayCrackers(void);
void DisplayProgrammerMessage(void);
void DISPLAY_POINT(s16 x, s16 y);
void DISPLAY_PTS_TO(s32 x1, s32 y1, s32 x2, s32 y2, s16 percentage);
void DISPLAY_PTS_TO_PLAN2(s16 origin_x, s16 origin_y, s16 dest_x, s16 dest_y, s16 percent);
void display_flocons_behind(void);
void display_flocons_before(void);
void DISPLAY_BLACKBOX(s16 x, s16 y, s16 width, s16 height, s16 font_size, u8 is_fond);
void display_deform_text(const char* text, s16 x, s16 y, u8 font_size, s8 color, s16 rotation, s16 enx, s16 eny);
void display_text_sin(const char* text, s16 x, s16 y, u8 font_size, s8 color, u8 a6);
void display_box_text(display_item_t* box);
void CLRSCR(void);
void display_grp_stars(void);
void DISPLAY_TEXT_FEE(void);
void DISPLAY_SAVE_POING(void);
void display_time(s32 time);
void get_spi_zdc(obj_t *param_1, s16 *param_2, s16 *param_3, s16 *param_4, s16 *param_5);
void DO_SPIDER_PLAFOND(obj_t* spipl_obj);
void DO_SPIDER_PLAFOND_POING_COLLISION(obj_t* obj, s16 sprite);
void SPIDER_PLAFOND_REACT(obj_t* obj);
void DO_SPIDER_COMMAND(obj_t* obj);
void DO_SPIDER_TIR(obj_t *obj);
void DoSpiderPoingCollision(obj_t* other_obj, s16 sprite);
void DoSpiderRaymanZDD(obj_t* obj);

void get_spi_zdc(obj_t *param_1, s16 *param_2, s16 *param_3, s16 *param_4, s16 *param_5)
{
    if (param_1->eta[param_1->main_etat][param_1->sub_etat].flags & 0x40)
    {
        *param_2 = param_1->x + 69;
        *param_3 = param_1->y + 38;
        *param_4 = 21;
        *param_5 = 30;
    }
    else if (param_1->eta[param_1->main_etat][param_1->sub_etat].flags & 0x80)
    {
        *param_2 = param_1->x + 69;
        *param_3 = param_1->y + 86;
        *param_4 = 34;
        *param_5 = 10;
    }
    else
    {
        *param_2 = param_1->x + 71;
        *param_3 = param_1->y + 71;
        *param_4 = 29;
        *param_5 = 19;
    }
}

void DO_SPIDER_PLAFOND(obj_t* spipl_obj) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    s16 poing_x;
    s16 spipl_x;
    s16 cen_x;
    s16 cen_y;
    s16 poing_y_1; s16 poing_y_2;
    s16 spipl_y_1; s16 spipl_y_2;
    s16 i;
    obj_t *cur_obj;
    s16 nb_objs;
    u8 new_flip;

    cen_x = get_center_x(spipl_obj);
    cen_y = get_center_y(spipl_obj);

    if (
        (
            spipl_obj->main_etat == 0 &&
            (spipl_obj->sub_etat == 27 || spipl_obj->sub_etat == 24 || spipl_obj->sub_etat == 30 || spipl_obj->sub_etat == 11)
        ) ||
        (spipl_obj->main_etat == 1 && spipl_obj->sub_etat == 2)
    ) {
        if (poing.is_active) {
            poing_x = (s16)poing_obj->x + poing_obj->offset_bx;
            poing_y_1 = (s16)poing_obj->y + poing_obj->offset_by;
            poing_y_2 = (s16)poing_obj->y + poing_obj->offset_hy;

            spipl_x = (s16)spipl_obj->x + spipl_obj->offset_bx;
            spipl_y_1 = (s16)spipl_obj->y + spipl_obj->offset_by + 5;
            spipl_y_2 = (s16)spipl_obj->y + spipl_obj->offset_hy - 5;
            if (
                (poing_y_1 <= spipl_y_1 || poing_y_2 <= spipl_y_1) &&
                (poing_y_1 >= spipl_y_2 || poing_y_2 >= spipl_y_2) &&
                (poing_x <= spipl_x + 60) && (poing_x >= spipl_x - 60)
            ) {
                spipl_obj->speed_x = 0;
                spipl_obj->speed_y = 0;
                set_main_and_sub_etat(spipl_obj, 0, 25);
            }
        }
    } else if (spipl_obj->main_etat == 0 && spipl_obj->sub_etat == 26 && !poing.is_active) {
        set_sub_etat(spipl_obj, 27);
    }

    if (spipl_obj->main_etat == 1 && spipl_obj->sub_etat == 2) {
        SET_X_SPEED(spipl_obj);
        if (block_flags[calc_typ_travd(spipl_obj, 0)] >> BLOCK_FULLY_SOLID & 1) {
            spipl_obj->flags.flip_x = 1 - spipl_obj->flags.flip_x;
            set_main_and_sub_etat(spipl_obj, 0, 24);
            SET_X_SPEED(spipl_obj);
        }
        CALC_MOV_ON_BLOC(spipl_obj);
    }

    if (on_block_chdir(spipl_obj, cen_x, cen_y) && test_allowed(spipl_obj, cen_x, cen_y)) {
        if (spipl_obj->main_etat == 1 && spipl_obj->sub_etat == 2) {
            set_main_and_sub_etat(spipl_obj, 0, 21);
            spipl_obj->speed_x = 0;
            spipl_obj->speed_y = 0;
            spipl_obj->flags.flip_x = 1 - spipl_obj->flags.flip_x;
        } else if (
            spipl_obj->main_etat == 2 &&
            (spipl_obj->sub_etat == 3 || spipl_obj->sub_etat == 1 || spipl_obj->sub_etat == 2)
        ) {
            set_main_and_sub_etat(spipl_obj, 0, 12);
            spipl_obj->speed_x = 0;
            spipl_obj->speed_y = 0;
        }
    } else {
        if (spipl_obj->main_etat == 2 && spipl_obj->sub_etat == 1 &&
            spipl_obj->anim_frame == 0
        ) {
            spipl_obj->speed_x = 0;
            spipl_obj->speed_y = -10;
        }
        else if (
            spipl_obj->main_etat == 0 && spipl_obj->sub_etat == 20 &&
            spipl_obj->anim_frame == 27
        ) {
            s16 spipl_spr_x; s16 spipl_spr_y; s16 spipl_spr_w; s16 spipl_spr_h;
            i = 0;
            cur_obj = &level.objects[i];
            nb_objs = level.nb_objects;
            while (i < nb_objs) {
                if ((cur_obj->type == TYPE_DARD_PLAFOND) && !cur_obj->is_active) {
                    GET_SPRITE_POS(spipl_obj, 2, &spipl_spr_x, &spipl_spr_y, &spipl_spr_w, &spipl_spr_h);
                    cur_obj->flags.flip_x = 0;

                    cur_obj->init_x =
                    cur_obj->x =
                            spipl_spr_x - cur_obj->offset_bx + (spipl_spr_w >> 1);

                    cur_obj->init_y =
                    cur_obj->y =
                            spipl_spr_y - cur_obj->offset_by + (spipl_spr_h >> 1);

                    cur_obj->speed_x = 0;
                    cur_obj->speed_y = 32;
                    cur_obj->is_active = 1;
                    cur_obj->flags.alive = 1;
                    add_alwobj(cur_obj);
                    cur_obj->cmd_offset = -1;
                    cur_obj->flags.read_commands = 1;
                    cur_obj->nb_cmd = 0;
                    if (ray.eta[ray.main_etat][ray.sub_etat].flags & 0x40)
                        position_ray = 1;
                    else
                        position_ray = 0;

                    DO_DARD_PLAFOND_ALWAYS(cur_obj);
                    break;
                }
                cur_obj++;
                i++;
            }
        }
    }

    if (spipl_obj->main_etat == 0 &&
        (spipl_obj->sub_etat == 20 || spipl_obj->sub_etat == 13 || spipl_obj->sub_etat == 23)
    ) {
        new_flip = ray.x > spipl_obj->x;
        spipl_obj->flags.flip_x = new_flip;
    }
}

void DO_SPIDER_PLAFOND_POING_COLLISION(obj_t* obj, s16 sprite) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.

    if (--obj->hit_points == 0) {
        if (
            (
                obj->main_etat == 0 &&
                (obj->sub_etat == 12 || obj->sub_etat == 13 ||
                 obj->sub_etat == 23 || obj->sub_etat == 20)
            ) ||
            (
                obj->main_etat == 2 &&
                (obj->sub_etat == 2 || obj->sub_etat == 3 ||
                 obj->sub_etat == 28 || obj->sub_etat == 7)
            )
        ) {
            set_main_and_sub_etat(obj, 0, 19);
        } else {
            set_main_and_sub_etat(obj, 0, 18);
        }
        obj->speed_y = -8;
        obj->speed_x = 0;
        obj->flags.read_commands = 0;
    } else {
        if ((obj->main_etat == 1 && obj->sub_etat == 2) ||
            (obj->main_etat == 0 &&
             (obj->sub_etat == 11 || obj->sub_etat == 29 ||
              obj->sub_etat == 26 || obj->sub_etat == 25 ||
              obj->sub_etat == 27 || obj->sub_etat == 24 ||
              obj->sub_etat == 30))) {
            set_main_and_sub_etat(obj, 0, 14);
            obj->speed_x = obj->speed_y = 0;
        } else if (obj->main_etat == 0 &&
                   (obj->sub_etat == 12 || obj->sub_etat == 13 ||
                    obj->sub_etat == 23 || obj->sub_etat == 20)) {
            if (obj->x < ray.x) {
                obj->flags.flip_x = 1;
            } else {
                obj->flags.flip_x = 0;
            }
            set_main_and_sub_etat(obj, 0, 17);
        }
    }
}

void SPIDER_PLAFOND_REACT(obj_t* obj) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    s32 prev_flip;
    u8 new_flip;

    if (obj->main_etat == 0 && (obj->sub_etat == 13 || obj->sub_etat == 12))
        set_sub_etat(obj, 20);
    else if (
        (obj->main_etat == 0 && obj->sub_etat == 11) ||
        (obj->main_etat == 0 && obj->sub_etat == 30) ||
        (obj->main_etat == 1 && obj->sub_etat == 2)
    ) {
        prev_flip = obj->flags.flip_x != (ray.x > obj->x);
        if (prev_flip) {
            obj->speed_x = 0;
            new_flip = ray.x > obj->x;
            obj->flags.flip_x = new_flip;
            obj->speed_y = 0;
            set_main_and_sub_etat(obj, 0, 24);
        }
    }
}

/* DOS 1:0c75c..0cac3. Narrow trajectory deltas, full-coordinate pursuit tests.
 * Abs/Sign/Min/Max spellings are ordinary C; helper lowering remains OPEN. */
#define SPIDER_ABS(x) ((x) >= 0 ? (x) : -(x))
#define SPIDER_MIN(a,b) ((a) < (b) ? (a) : (b))
#define SPIDER_MAX(a,b) ((a) > (b) ? (a) : (b))
void DO_DARD_PLAFOND_ALWAYS(obj_t* obj) {
    s16 dx, dy, distance;
    s16 abs_speed_x;
    s32 speed;
    if (obj->cmd != 1) {
        dx = (s16)ray.x + ray.offset_bx - ((s16)obj->x + obj->offset_bx);
        if (position_ray)
            dy = (s16)ray.y + ray.offset_by - ((s16)obj->y + obj->offset_by);
        else
            dy = ray.y + (ray.offset_by >> 1) - (obj->y + obj->offset_by);
        distance = SPIDER_ABS((s32)dx) + SPIDER_ABS((s32)dy);
        if (ray.y + ray.offset_by > obj->y + (obj->offset_by - obj->offset_hy) &&
            ((ray.x + ray.offset_bx > obj->x + obj->offset_bx && !obj->flags.flip_x) ||
             (ray.x + ray.offset_bx < obj->x + obj->offset_bx && obj->flags.flip_x))) {
            if (distance > 0) {
                dx = ((s32)dx << 5) / distance;
                dy = ((s32)dy << 5) / distance;
            }
            speed = obj->speed_y + sgn(dy - obj->speed_y);
            if (speed > 32) speed = 32;
            if (speed < 0) speed = 0;
            obj->speed_y = speed;
            if (obj->speed_y != 0) {
                speed = obj->speed_x + sgn(dx - obj->speed_x);
                if (speed > 32) speed = 32;
                if (speed < -32) speed = -32;
                obj->speed_x = speed;
            } else if (obj->flags.flip_x) {
                speed = obj->speed_x - (dx > obj->speed_x);
                if (speed > 32) speed = 32;
                if (speed < -32) speed = -32;
                obj->speed_x = speed;
            } else {
                speed = obj->speed_x + (dx < obj->speed_x);
                if (speed > 32) speed = 32;
                if (speed < -32) speed = -32;
                obj->speed_x = speed;
            }
        } else {
            speed = obj->speed_x * 2;
            if (speed > 32) speed = 32;
            if (speed < -32) speed = -32;
            obj->speed_x = speed;
            speed = obj->speed_y * 2;
            if (speed > 32) speed = 32;
            obj->speed_y = speed;
        }
    } else {
        if (ray.x + ray.offset_bx > obj->x + obj->offset_bx)
            obj->flags.flip_x = 0;
        else if (ray.x + ray.offset_bx < obj->x + obj->offset_bx)
            obj->flags.flip_x = 1;
    }
    if (obj->speed_x == 0 && obj->speed_y == 0)
        obj->speed_x = obj->flags.flip_x ? -1 : 1;
    abs_speed_x = SPIDER_ABS((s32)obj->speed_x);
    if (abs_speed_x * 3 < obj->speed_y)
        obj->anim_frame = 0;
    else if (abs_speed_x * 4 < obj->speed_y * 3)
        obj->anim_frame = 0;
    else if (abs_speed_x * 3 < obj->speed_y * 4)
        obj->anim_frame = 11;
    else if (abs_speed_x < obj->speed_y * 3)
        obj->anim_frame = 10;
    else
        obj->anim_frame = 9;
    if (obj->speed_x > 0 && obj->flags.flip_x)
        obj->flags.flip_x = 0;
    else if (obj->speed_x < 0 && !obj->flags.flip_x)
        obj->flags.flip_x = 1;
}

void DO_SPIDER_COMMAND(obj_t* obj) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    if (obj->sub_etat == 7) {
        obj->speed_y = -8;
        obj->speed_x = 0;
        obj->flags.read_commands = 0;
        return;
    }

    if ((obj->main_etat == 0) && (obj->sub_etat == 0))
        obj->link = 0;

    switch (obj->cmd) {
        case GO_LEFT:
        case GO_RIGHT:
            if (obj->cmd == GO_LEFT)
                obj->flags.flip_x = 0;
            else
                obj->flags.flip_x = 1;

            SET_X_SPEED(obj);
            obj->param = 1;
            if (obj->main_etat == 0) {
                set_main_and_sub_etat(obj, 1, 0);
            }

            if (obj->main_etat == 1) {
                if (block_flags[calc_typ_travd(obj, 0)] & 1) {
                    if (obj->cmd == GO_LEFT) {
                        obj->flags.flip_x = 1;
                        skipToLabel(obj, 2, 1);
                    } else {
                        obj->flags.flip_x = 0;
                        skipToLabel(obj, 0, 1);
                    }
                }
                SET_X_SPEED(obj);
                CALC_MOV_ON_BLOC(obj);
            }
            break;
        case GO_WAIT:
            switch (obj->main_etat) {
                case 0:
                    if (obj->sub_etat == 4)
                        obj->link = 0;

                    if (obj->sub_etat == 8)
                        TEST_FIN_BLOC(obj);
                    break;
                case 1:
                    if (!(block_flags[calc_typ_travd(obj, 0)] & 1))
                        SET_X_SPEED(obj);
                    else
                        obj->speed_x = 0;

                    CALC_MOV_ON_BLOC(obj);
            }
            break;
    }
}

void DO_SPIDER_TIR(obj_t *obj)
{
    DO_SPIDER_COMMAND(obj);
    if ((obj->main_etat == 0) && (obj->sub_etat == 3) && (obj->link == 0))
    {
        allocateDard(obj);
        obj->link = 1;
    }
}

void allocateDard(obj_t* spi_obj) {
    s16 spi_y;
    s16 spi_width;
    s16 spi_x;
    s16 spi_height;
    s16 i;
    s16 nb_objects;
    s16 speed;
    obj_t* cur_obj;

    i = 0;
    cur_obj = level.objects;
    nb_objects = level.nb_objects;
    while (i < nb_objects) {
        if (cur_obj->type == 0x42 && cur_obj->is_active == 0) {
            cur_obj->flags.flip_x = spi_obj->flags.flip_x;
            cur_obj->speed_y = 0;
            if (cur_obj->flags.flip_x) speed = -3;
            else speed = 3;
            cur_obj->speed_x = speed;
            GET_SPRITE_POS(spi_obj, 3, &spi_x, &spi_y, &spi_width, &spi_height);
            cur_obj->x = (spi_width >> 1) + spi_x - cur_obj->offset_bx;
            cur_obj->y = (spi_height >> 1) + spi_y - cur_obj->offset_by;
            cur_obj->init_x = (s16)cur_obj->x;
            cur_obj->init_y = (s16)cur_obj->y;
            cur_obj->display_prio = 4;
            cur_obj->main_etat = 0;
            cur_obj->sub_etat = 9;
            skipToLabel(cur_obj, cur_obj->flags.flip_x, 1);
            calc_obj_pos(cur_obj);
            cur_obj->is_active = 1;
            cur_obj->flags.alive = 1;
            add_alwobj(cur_obj);
            cur_obj->anim_frame = 9;
            cur_obj->flags.flag_0x40 = 0;
            break;
        }
        ++cur_obj;
        ++i;
    }
}

void DoSpiderPoingCollision(obj_t* other_obj, s16 sprite) {
    // NOTE(Falcury): PS1 implementation is located in DO_POING_COLLISION(). Seems to be functionally identical.
    obj_hurt(other_obj);
    if (other_obj->hit_points == 0) {
        set_main_and_sub_etat(other_obj, 0, 7);
        other_obj->anim_frame = 0;
        other_obj->flags.read_commands = 0;
    } else {
        if (other_obj->main_etat == 0 &&
            (other_obj->sub_etat == 2 || other_obj->sub_etat == 3 ||
             other_obj->sub_etat == 4 || other_obj->sub_etat == 5)
        ) {
            other_obj->cmd_context_depth = -1;
        }

        other_obj->param = 0;
        if (poing_obj->speed_x > 0) {
            skipToLabel(other_obj, 3, 1);
            other_obj->flags.flip_x = 1;
        } else {
            skipToLabel(other_obj, 1, 1);
            other_obj->flags.flip_x = 0;
        }
        pushToLabel(other_obj, 6, 1);
        other_obj->anim_frame = 0;
        pushToLabel(other_obj, 7, 1);
    }
}

void DoSpiderRaymanZDD(obj_t* obj) {
    // NOTE(Falcury): PS1 implementation is located in DO_RAY_IN_ZONE(). Seems to be functionally identical.
    if (obj->main_etat == 1 && obj->sub_etat == 0 && obj->link == 0) {
        obj->anim_frame = 0;
        if (!(obj->flags.flip_x)) {
            obj->flags.flip_x = 1;
            skipToLabel(obj, 3, 1);
        } else {
            obj->flags.flip_x = 0;
            skipToLabel(obj, 1, 1);
        }
        pushToLabel(obj, 5, 1);
    }
}

