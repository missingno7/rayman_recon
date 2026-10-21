/* 1:06f80..1:0cee0: action.c, affiche.c, araignee.c, pcmain.c (init, main loops, display, spider); four original objects by DOS boundaries (build/workers/tu_action/source_objects.md) - kept in one file until split. Names RAYVERSE/ANDROID; seeded by tools/seed.py, worker tu_action. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "palette.h"
#include "action_local.h"
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
#define SCREEN_HEIGHT 240
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
s16 GET_SPRITE_POS(obj_t* obj, s16 index, s16* x, s16* y, s16* w, s16* h);
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

void InitData(s16 x, s16 y, s16 origin_x, s16 origin_y) {
    init_data_state.x = x & 0x0F;
    init_data_state.y = y & 0x0F;
    init_data_state.c4 = (s32)draw_buffer - init_data_state.x / 4 -
                        ((init_data_state.y * 5) << 4) + 1;
    if (type_fndscrX)
        init_data_state.c8 = 0;
    else
        init_data_state.c8 = origin_x + 16 - init_data_state.x;
    init_data_state.cc = origin_y + 16 - init_data_state.y;
    init_data_state.d0 = (s32)PLAN0BIT + init_data_state.c8 / 4 +
        init_data_state.c8 % 4 * ((u32)plan0bit_length >> 2) +
        init_data_state.cc * plan0bit_nb_bytes;
    planar_copy_offset = 1 << (3 - (init_data_state.x & 3));
}

void DO_GROS_MOTEUR_NORMAL(u8 need_update_display_map) {
    if (CarteSonAutorisee && gele != 1)
        manage_snd();
    do_record(&record);
    if (gele)
        DO_MOTEUR_GELE();
    else {
        DO_MOTEUR();
        if (!gele)
            DO_MOTEUR2();
    }
    get_luciole();
    if (need_update_display_map)
        update_display_map(&BIG_MAP);
}

/* DOS disassembly reconstructs this loop; remaining call ABI work is recorded in NOTES.md. */
void DO_MAIN_LOOP_PC_NORMAL(u8* key_state) {
    u8 end_of_frame;
    RaymanDansUneMapDuJeu = 1;
    DoFirstFlocons();
    fin_continue = 0;
    MapTimePause = 0;
    ProchainEclair = 0;
    numero_palette_special = 0;
    if (MusicCdActive)
        TestCdLoop();
    while (dead_time != 0 && !new_level && !new_world && ModeDemo != 2) {
        speaker_enable();
        if (use_sync)
            synchro();
        if (need_timer)
            clock_ticks();
        fades();
        if (dead_time >= 32 && !gele)
            DoPaletteSpecialPC();
        DO_SWAP_PALETTE();
        if (fixon || fixontemp) {
            if (Mode_Pad == 1 && Main_Control)
                Swap_And_Test_Joystick(DrawBufferNormal, DisplayBufferNormal, 320, Bloc_lim_H2);
            else
                Swap_To_Screen(DrawBufferNormal, DisplayBufferNormal, 320, Bloc_lim_H2);
        } else {
            Swap_To_Screen(DrawBufferNormal, DisplayBufferNormal, 320, Bloc_lim_H2);
        }
        if (need_timer)
            clock_ticks();
        if (MusicCdActive)
            TestCdLoop();
        if (Mode_Pad == 0)
            readinput();
        DO_GROS_MOTEUR_NORMAL((u8)display_mode);
        if (need_timer)
            clock_ticks();
        CLEAR_FIXE_LUCIOLE();
        if (BackgroundOn && FondAutorise == 1) {
            Calcul_Deplacement_Bande(xmap, plan0_width, plan0_height);
            Display_Back_Screen(plan0_width, plan0_height,
                                Bloc_lim_W1, Bloc_lim_H1, Bloc_lim_W2, Bloc_lim_H2);
        } else {
            memset(draw_buffer, 0, 320 * 200);
        }
        if (MapAvecPluieOuNeige) {
            flocon_clipping(Bloc_lim_H1, Bloc_lim_H2 - 6,
                            Bloc_lim_W1, Bloc_lim_W2 - 4);
            display_flocons_behind();
        }
        DRAW_MAP(DrawBufferNormal, &BIG_MAP);
        display_grp_stars();
        DISPLAY_ALL_OBJECTS();
        display_pix_gerbes();
        Display_Sprite_On_Front(plan0_width, plan0_height,
                                Bloc_lim_W1, Bloc_lim_H1, Bloc_lim_W2, Bloc_lim_H2);
        if (MapAvecPluieOuNeige)
            display_flocons_before();
        Display_and_free_luciole(DrawBufferNormal);
        clear_borders_Normal(DrawBufferNormal + Bloc_lim_W1 + (Bloc_lim_H1 << 8) - 4 +
                             (Bloc_lim_H1 << 6), Bloc_lim_H2 - Bloc_lim_H1,
                             Bloc_lim_W2 - Bloc_lim_W1 + 4);
        if (is_fee)
            DISPLAY_TEXT_FEE();
        else
            DISPLAY_FIXE(left_time);
        if (JeuCracker)
            DisplayCrackers();
        if (MessageProgram)
            DisplayProgrammerMessage();
        display_time(left_time);
        if (Vignet_To_Display)
            DISPLAY_GAME_VIGNET();
        if (TOUCHE(SC_ESCAPE) && nb_fade == 0 && !GoMenu &&
            !get_casse_brique_ON() && !ModeDemo && !gele && dead_time == 64)
            GoMenu = 1;
        DO_OPTIONS_IN_GAME();
        if (get_casse_brique_ON() && !get_casse_brique_active()) {
            set_casse_brique_ON(0);
            RESET_TOUCHE(SC_ESCAPE);
        }
        if (rightjoy() || leftjoy() || downjoy() || upjoy() ||
            but0pressed() || but1pressed() || but2pressed() || but3pressed())
            MapTimePause = 0;
        ++MapTimePause;
        if (MapTimePause >= 21600)
            in_pause = 1;
        if (in_pause && !GoMenu) {
            if (fade == 65) {
                Do_Effect_Pause();
                MapTimePause = 0;
            } else {
                in_pause = 0;
            }
        }
        HANDLE_KEY(key_state);
        end_of_frame = 1;
        if (need_timer && use_sync && end_of_frame)
            end_of_frame = 1;
        endsynchro();
    }
    RaymanDansUneMapDuJeu = 0;
}

void DO_GROS_MOTEUR_X(void) {
    s32 x_rem;
    if (CarteSonAutorisee && gele != 1)
        manage_snd();
    do_record(&record);
    if (gele)
        DO_MOTEUR_GELE();
    else {
        DO_MOTEUR();
        if (!gele)
            DO_MOTEUR2();
    }
    if (!gele) {
        update_map(xmap, ymap, xmap_old, ymap_old);
    }
    if (FondAutorise == 1) {
        calc_off_fnd();
        if (yp0 < 0)
            yp0 += plan0_height;
        else
            yp0 = 0;
    }
    if (BackgroundOn) {
        x_rem = (xmap / 3) % plan0_width;
        InitData((u8)xmap, (u8)ymap, (s16)x_rem, plan0_width);
    } else {
        InitData((u8)xmap, (u8)ymap, 0, 0);
    }
    sub_3C3BC(xmap, ymap);
}

void DO_MAIN_LOOP_PC_X(void) {
    u8 end_of_frame;
    DoFirstFlocons();
    if (MusicCdActive)
        TestCdLoop();
    while (dead_time != 0 && !new_level && !new_world && ModeDemo != 2) {
        speaker_enable();
        if (use_sync)
            synchro();
        if (need_timer)
            clock_ticks();
        fades();
        if (dead_time >= 32 && !gele)
            DoPaletteSpecialPC();
        DO_SWAP_PALETTE();
        if (MusicCdActive)
            TestCdLoop();
        build_map();
        if (need_timer)
            clock_ticks();
        if (Mode_Pad == 0)
            readinput();
        DO_GROS_MOTEUR_X();
        if (need_timer)
            clock_ticks();
        sub_3BE20();
        if (BackgroundOn)
            sub_3BEE0();
        display_grp_stars();
        DISPLAY_ALL_OBJECTS();
        display_pix_gerbes();
        if (MapAvecPluieOuNeige) {
            display_flocons_behind();
            display_flocons_before();
        }
        if (is_fee)
            DISPLAY_TEXT_FEE();
        else
            DISPLAY_FIXE(left_time);
        if (JeuCracker)
            DisplayCrackers();
        if (MessageProgram)
            DisplayProgrammerMessage();
        display_time(left_time);
        if (Vignet_To_Display)
            DISPLAY_GAME_VIGNET();
        HANDLE_KEY(NULL);
        end_of_frame = 1;
        if (need_timer && use_sync && end_of_frame)
            wait_until();
        if (!fin_continue && new_level)
            LoadPlan0InVignet();
        endsynchro();
    }
}

void init_arg(s32 argc, char** argv) {
    s32 i;
    char* eq;

    strcpy(demo_video_mem, ".\\");
    if (argc > 1) {
        for (i = 1; i < argc; ++i) {
            if (strncmp(strupr(argv[i]), "SAVE", 4) == 0) {
                eq = argv[i];
                while (*eq != '=' && *eq != '\0')
                    ++eq;
                if (*eq == '\0')
                    FatalError("Missing = in rayman's argument (save=pathname).\n");
                eq = argv[i];
                while (*eq != '=' && *eq != '\0')
                    ++eq;
                strcpy(demo_video_mem, eq + 1);
                strcat(demo_video_mem, "\\");
            }
            if (strcmp(strupr(argv[i]), "FREQ70") != 0)
                Frequence = 'F';
        }
    }
}

void PrintDosInfo(void) {
    switch (DosDetect) {
    case 0:
        printf("IBM");
        break;
    case 1:
        printf("Compaq");
        break;
    case 4:
        printf("AT&T");
        break;
    case 6:
    case 0x4d:
        printf("Hewlett-Packard");
        break;
    case 0x0d:
        printf("Packard-Bell");
        break;
    case 0x16:
        printf("DEC");
        break;
    case 0x23:
        printf("Olivetti");
        break;
    case 0xee:
        printf("DR");
        break;
    case 0xff:
        printf("Microsoft");
        break;
    default:
        printf("Unknown");
        break;
    }
    printf(" ");
    if (HVerDos < 10) {
        printf("Dos v%1d.%1d", HVerDos, LVerDos);
    } else if (HVerDos <= 10) {
        printf("OS/2 v1.x");
    } else if (HVerDos == 20) {
        if (LVerDos == 30)
            printf("OS/2 Warp v3.0");
        else
            printf("OS/2 v2.x");
    } else {
        printf("Dos v%1d.%1d", HVerDos, LVerDos);
    }
    printf("\n");
}

void InitMemoryVariable(void) {
    s32 available_memory;
    s16 i;

    available_memory = get_avail_memory();
    printf("\nLeft memory available = %.3f Mo.\n\n",
           (double)available_memory / 1048576.0);

    if (TheProcesseur >= 3) {
        if (TheProcesseur == 3) {
            printf("386 processor detected.\n");
            printf("Rayman requires a 486 or more.\n");
        } else if (TheProcesseur == 4) {
            printf("486 processor detected.\n");
        } else if (TheProcesseur == 5) {
            printf("Pentium processor detected.\n");
        } else {
            printf("Pentium %1d processor detected.\n", TheProcesseur);
        }
    } else {
        printf("Unknown processor detected.\n");
    }

    if (WindowsLance == 0) {
        printf("No windows detected.\n");
    } else if (WindowsLance == 1) {
        printf("Windows real mode detected.\n");
    } else if (WindowsLance == 0x81) {
        printf("Windows standard mode detected.\n");
    } else if (WindowsLance == 0x82) {
        printf("Windows/386 V 2.x detected.\n");
    } else if (WindowsLance == 0x83) {
        if (NVersionWindows == 4)
            printf("Windows 95 detected.\n");
        else
            printf("Windows V %d.%d detected.\n", HVersionWindows, NVersionWindows);
    } else if (WindowsLance <= 0x80) {
        printf("Windows V %d.%d detected.\n", HVersionWindows, NVersionWindows);
    }

    PrintDosInfo();
    P486 = 0;
    if (available_memory < 2000000) {
        FatalError("Not enough memory.");
    } else if (available_memory < 3000000) {
        ModeNormalAutorise = 0;
        ToutSpriteAutorise = 0;
        FondAutorise = 2;
        JumelleEffetAutorise = 1;
        CarteSonAutorisee = 1;
        SonLimite = 1;
        P486 = 1;
        TailleMainMemTmp = 0x17400;
        TailleMainMemWorld = 0x62C00;
        TailleMainMemLevel = 0x5E000;
        TailleMainMemSprite = 0x12C000;
        TailleMainMemFix = 0x4D800;
    } else {
        ModeNormalAutorise = 1;
        ToutSpriteAutorise = 1;
        FondAutorise = 1;
        JumelleEffetAutorise = 1;
        CarteSonAutorisee = 1;
        SonLimite = 0;
        P486 = 0;
        TailleMainMemTmp = 0x22000;
        TailleMainMemWorld = 0xF4C00;
        TailleMainMemLevel = 0x87C00;
        TailleMainMemSprite = 0xDF400;
        TailleMainMemFix = 0x4D800;
    }

    for (;;) {
        printf("\n");
        if (ModeNormalAutorise)
            printf("Normal and X mode available.\n");
        else
            printf("Only X mode available.\n");
        if (JumelleEffetAutorise)
            printf("Zoom effect available.\n");
        else
            printf("No zoom effect.\n");
        if (CarteSonAutorisee) {
            if (MusicCdActive)
                printf("Sound and CD music.\n");
            else
                printf("Only sound, no CD music.\n");
        } else if (MusicCdActive) {
            printf("Only CD music, no sound.\n");
        } else {
            printf("no CD music, no sound.\n");
        }
        if (FondAutorise)
            printf("Background available.\n");
        else
            printf("No background.\n");
        printf("\n");
        printf("save path : %s\n", demo_video_mem);
        if (TOUCHE(1))
            break;
        for (i = 0; i < 40; i++)
            delay(100);
    }
    RESET_ALL_TOUCHE();
    printf("\n");
    AllocVariablesAutorisee();
}

void AllocVariablesAutorisee(void) {
    MessageProgram = 0;
    rvb_special = (rgb_palette_t*)JumelleCalloc(16, sizeof(rgb_palette_t));
    if (!rvb_special)
        FatalError("Allocation error in variables.\n");
    flocon_tab = (flocon_t*)JumelleCalloc(512, sizeof(flocon_t));
    if (!flocon_tab)
        FatalError("Allocation error in variables.\n");
    GameModeVideo = (ModeNormalAutorise == 0);
    MenuCredits = 0;
}

void AfficheEntete(void) {
    u8* video;
    const char* title;
    u8* second_line;
    s32 i;

    InitTextMode();
    video = (u8*)0xB8000;
    title = "RAYMAN (US) v1.21";
    printf("\n\n");
    for (second_line = video; second_line != video + 160; second_line += 2)
        second_line[1] = 0x1E;
    for (i = 0; i < strlen(title); i++)
        video[i * 2] = title[i];
    second_line = video + 160;
    for (i = 0; i < strlen("(C) 1994-1995 UBI STUDIOS, FRANCE"); i++) {
        second_line[-(s32)(2 * strlen("(C) 1994-1995 UBI STUDIOS, FRANCE")) + i * 2] =
            "(C) 1994-1995 UBI STUDIOS, FRANCE"[i];
        second_line[-(s32)(2 * strlen("(C) 1994-1995 UBI STUDIOS, FRANCE")) + i * 2 + 1] |= 0x80;
    }
}

int main_Ray(int argc, char** argv) {
    //unlink("C:\\raytmp.tmp");
    AfficheEntete();
    WindowsLance = 131; //detect_windows();
    HVersionWindows = 4;
    NVersionWindows = 0;
    OptionMusicCdActive = 1;
    MusicCdActive = 1;
    init_arg(argc, argv);
    PcMain();
    return 0;
}

void PcMain(void) {
    u8 level_init;
    DetectCpu();
    PrintDosInfo();
    InitMusic();
    Init_Clavier();
    InitMemoryVariable();
    sprite_clipping(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
    INIT_MOTEUR_BEGIN();
    FIRST_INIT();
    DO_UBI_LOGO();
    DO_GROS_RAYMAN();
    INIT_EXTENSIONS();
    LOAD_CONFIG();
    PC_init_cheats();
    NBRE_SAVE = 3;
    if (MusicCdActive)
        stop_cd();
    level_init = 0;
    while (!fin_de_rayman) {
        DEPART_INIT_LOOP();
        INIT_WORLD_INFO();
        set_default_Bloc_clipping();
        DO_NEW_MENUS();
        while (!fin_du_jeu && status_bar.lives > -1 && new_world) {
            MakeMyRand(1);
            SPECIAL_INIT();
            default_sprite_clipping();
            DO_WORLD_MAP();
            sprite_clipping(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
            DEPART_WORLD();
            if (!SonLimite)
                LoadBnkWorld(num_world_choice);
            while (!fin_du_jeu && !new_world && new_level) {
                WaitNSynchro(5);
                speaker_enable();
                DEPART_LEVEL();
                init_divers_level_PC(&level_init);
                if ((GameModeVideo == MODE_NORMAL && num_world == 6 && num_level == 4) ||
                    get_casse_brique_active())
                    InitClipping();
                INIT_MOTEUR_LEVEL(new_level);
                init_fee();
                init_moustique();
                InitPaletteSpecialPC();
                if (byte_CFA2A != 0)
                    fade_out(2, &rvb_plan3);
                while (!(fin_du_jeu || new_level || new_world)) {
                    WaitNSynchro(15);
                    INIT_MOTEUR_DEAD();
                    INIT_RAY_ON_MS();
                    START_LEVEL_ANIM();
                    BackgroundOn = IsBackgroundOn();
                    if (GameModeVideo != MODE_NORMAL) {
                        default_sprite_clipping();
                        InitModeXWithFrequency(VGA_FREQ);
                        NewFrequency(Frequence);
                        During_The_Menu = 0;
                        DO_MAIN_LOOP_PC_X();
                        During_The_Menu = 1;
                    } else {
                        InitClipping();
                        InitModeNormalWithFrequency(VGA_FREQ);
                        NewFrequency(Frequence);
                        INIT_GAME_MODE_NORMAL();
                        During_The_Menu = 0;
                        DO_MAIN_LOOP_PC_NORMAL(NULL);
                        During_The_Menu = 1;
                        FIN_GAME_MODE_NORMAL();
                    }
                    if (CarteSonAutorisee)
                        stop_all_snd();
                    InitModeNormalWithFrequency(VGA_FREQ);
                    sprite_clipping(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
                    START_LEVEL_ANIM();
                    if (ExitMenu)
                        INIT_CONTINUE();
                    else {
                        DO_CONTINUE();
                        DO_VICTOIRE();
                    }
                    ExitMenu = 0;
                }
                DONE_MOTEUR_LEVEL();
                FIN_DEAD_LOOP();
            }
            if (MusicCdActive)
                stop_cd();
            FIN_MAP_LOOP();
            if (ModeDemo)
                fin_du_jeu = 1;
        }
        FIN_GAME_LOOP();
        if (ModeDemo)
            FinDemoJeu();
    }
    if (CarteSonAutorisee) {
        stop_all_snd();
        DigiMusicDone();
    }
    END_GAME();
}

void setBossReachingSpeeds(obj_t *obj, u8 horloge_ind, u8 unk_x, u8 unk_y)
{
    s16 diff_x_1;
    s16 diff_y_1;
    s32 diff_x_abs;
    s32 diff_y_abs;
    s32 unk_1;
    s16 unk_2;
    s32 unk_3;
    s16 unk_4;
    s16 unk_5;

    if ((bossXToReach != -32000) && (bossYToReach != -32000))
    {
        unk_1 = obj->eta[obj->main_etat][obj->sub_etat].speed_x_right * (bossSpeedFactor + alternateBossSpeedFactor);
        if (unk_1 < 0)
            unk_1 += 0x3FFF;
        unk_1 = unk_1 >> 0xE;
        diff_x_1 = (bossXToReach - obj->x) * 16;
        diff_y_1 = (bossYToReach - obj->y) * 16;
        if (!(diff_x_1 == 0 && diff_y_1 == 0) && unk_1 != 0)
        {
            diff_x_abs = abs(diff_x_1);
            diff_y_abs = abs(diff_y_1);
            unk_2 = MAX_1(diff_x_abs, diff_y_abs);
            unk_1 = MIN_1(unk_2, unk_1);
            unk_3 = ashl16(unk_2, 4) / unk_1;
            unk_4 = ashl16(diff_x_1, 4) / unk_3;
            unk_5 = ashl16(diff_y_1, 4) / unk_3;
            if (horloge[horloge_ind] == 0)
            {
                obj->speed_x = (obj->speed_x * (0xFF - unk_x) + (unk_4 * unk_x)) / 255;
                if (obj->main_etat == 2)
                {
                    if (unk_1 >= diff_y_1)
                        unk_y = 0xFF;
                }
                obj->speed_y = (obj->speed_y * (0xFF - unk_y) + (unk_5 * unk_y)) / 255;
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
    while (!(block_flags[btyp] >> BLOCK_SOLID & 1) && y <= ymapmax + 200) {
        y += BLOCK_SIZE;
        btyp = BTYP(x >> 4, y >> 4);
    }

    return dist_to_bloc_floor(btyp, x & 0xf, 0) + (y & ~0xf);
}

void adjustBossScrollLocker(void) {
    s16 new_bossScrollStartX = bossScrollStartX;
    s16 new_bossScrollEndX = bossScrollEndX;

    if (GameModeVideo != MODE_NORMAL) {
        if (bossScrollStartX < 0) {
            new_bossScrollStartX = 0;
        }
        if (new_bossScrollStartX > xmapmax) {
            new_bossScrollStartX = xmapmax;
        }
        if (bossScrollEndX < 0) {
            new_bossScrollEndX = 0;
        }
        if (new_bossScrollEndX > xmapmax) {
            new_bossScrollEndX = xmapmax;
        }
    } else {
        if (bossScrollStartX < 8 - Bloc_lim_W1) {
            new_bossScrollStartX = 8 - Bloc_lim_W1;
        }
        if (new_bossScrollStartX > xmapmax + 8) {
            new_bossScrollStartX = xmapmax + 8;
        }
        if (bossScrollEndX < 8 - Bloc_lim_W1) {
            new_bossScrollEndX = 8 - Bloc_lim_W1;
        }
        if (new_bossScrollEndX > xmapmax + 8) {
            new_bossScrollEndX = xmapmax + 8;
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

    bossScrollStartX = new_bossScrollStartX;
    bossScrollEndX = new_bossScrollEndX;
}

void setBossScrollLimits(obj_t* obj) {
    bossScrollStartX = scroll_start_x;
    bossScrollEndX = scroll_end_x;
    floorLine = firstFloorBelow(obj);
    bossScrollStartY = Bloc_lim_H1 - Bloc_lim_H2 + floorLine - BLOCK_SIZE;
    bossScrollEndY = ymapmax;
    adjustBossScrollLocker();
}

void display2(obj_t* obj) {
    anim_t* anim;
    u16 layers_per_frame;
    anim_layer_t* layer;
    s32 layer_index;
    s32 proj_y;
    sprite_t* sprite;
    s16 x;
    s32 proj_x;
    s32 proj_height;
    s32 proj_width;
    vec2b_t proj_size;
    u8 sprite_field_A;
    draw_func_t* draw_func;
    u8* image_data;

    anim = obj->animations + obj->anim_index;
    layers_per_frame = anim->layers_per_frame & 0x3FFF;
    layer = anim->layers + (obj->anim_frame * layers_per_frame);
    for (layer_index = 0; layer_index < layers_per_frame; ++layer_index) {
        proj_y = get_proj_y(obj->scale, layer->y + obj->screen_y);
        if (layer->sprite_index != 0) {
            sprite = obj->sprites + layer->sprite_index;
            if (sprite->id != 0) {
                if (obj->flags.flip_x) {
                    if (obj->scale == 256 && layer_index == 5 &&
                        obj->anim_index >= 14 && obj->anim_index <= 16)
                        x = -16;
                    else
                        x = 0;
                    x += obj->offset_bx * 2 - layer->x - sprite->outer_width + obj->screen_x;
                } else {
                    x = layer->x + obj->screen_x;
                }
                if (obj->flags.flip_x ^ layer->mirrored)
                    draw_func = DrawSpriteFlipNormalEtX;
                else
                    draw_func = DrawSpriteNormalEtX;
                proj_x = get_proj_x(obj->scale, x);
                proj_height = get_proj_dist(obj->scale, sprite->outer_height);
                proj_width = get_proj_dist(obj->scale, sprite->outer_width);
                proj_size.width = (u8)proj_width;
                proj_size.height = (u8)proj_height;
                sprite_field_A = sprite->color >> 4;
                image_data = obj->img_buffer + sprite->offset_in_atlas;
                draw_func(proj_x, sprite_field_A, proj_y, proj_size,
                          draw_buffer, image_data);
            }
        }
        ++layer;
    }
}

void display_sprite(obj_t *param_1, u8 sprite, s16 x, s16 y, u8 param_5)
{
    draw_sprite(&param_1->sprites[sprite], x, y, param_5);
}

void display_sprite_NoClip(obj_t* obj, u8 sprite_index, s16 x, s16 y, u8 flipped) {
    sprite_t* sprite = obj->sprites + sprite_index;
    draw_func_t* draw_func;
    s32 proj_x;
    s32 proj_y;
    s32 proj_height;
    s32 proj_width;
    vec2b_t proj_size;
    u8* image_data;

    if (((sprite->flags & 4) != 0) == flipped)
        draw_func = DrawSpriteFlipNoClipNormalEtX;
    else
        draw_func = DrawSpriteNoClipNormalEtX;
    proj_x = get_proj_x(obj->scale, x);
    proj_y = get_proj_y(obj->scale, y);
    proj_height = get_proj_dist(obj->scale, sprite->outer_height);
    proj_width = get_proj_dist(obj->scale, sprite->outer_width);
    proj_size.width = (u8)proj_width;
    proj_size.height = (u8)proj_height;
    image_data = obj->img_buffer + sprite->offset_in_atlas;
    draw_func(proj_x, sprite->color, proj_y, proj_size, draw_buffer, image_data);
}

void DISPLAY_POING(void)
{
    if (poing.is_active)
        display(&level.objects[poing_obj_id]);
}

void DISPLAY_CLING(void) {
    /* 15FB8 8013A7B8 -O2 -msoft-float */
    obj_t *c_1up;
    obj_t *c_pow;
    s32 xmin, xmax, ymin, ymax;

    // NOTE: there are some differences here from the PS1 version
    s16 extra_y_offset;
    if (GameModeVideo == MODE_X && P486 == 1) {
        extra_y_offset = 16;
    } else {
        extra_y_offset = 0;
    };

    id_Cling_Old = 1;
    if (id_Cling_1up != -1) {
        c_1up = &level.objects[id_Cling_1up];
        if (c_1up->timer == 0) {
            c_1up->screen_x = 61 - c_1up->offset_bx;
            c_1up->screen_y = extra_y_offset + 5 - c_1up->offset_hy;
            get_sprite_clipping(&xmin, &xmax, &ymin, &ymax);
            sprite_clipping(0, 100, 0, 100);
            display2(c_1up);
            sprite_clipping(xmin, xmax, ymin, ymax);
        }
    }
    if (id_Cling_Pow != -1) {
        c_pow = &level.objects[id_Cling_Pow];
        if (c_pow->timer == 0) {
            c_pow->screen_x = 66 - c_pow->offset_bx;
            c_pow->screen_y = extra_y_offset + 14 - c_pow->offset_hy;
            get_sprite_clipping(&xmin, &xmax, &ymin, &ymax);
            sprite_clipping(0, 100, 0, 100);
            display2(c_pow);
            sprite_clipping(xmin, xmax, ymin, ymax);
        }
    }
}

void display_bar_boss(obj_t *boss_obj)
{
    u8 hp;
    obj_t *poing_obj;
    u8 init_hp;
    s32 unk_1;
    s32 unk_2;
    s32 unk_3;
    s32 disp_mode;

    if (scrollLocked)
    {
        hp = boss_obj->hit_points;
        if (hp != 0)
        {
            poing_obj = &level.objects[poing_obj_id];
            init_hp = boss_obj->init_hit_points;
            unk_1 = 6;
            unk_2 = (29 - init_hp) * unk_1;
            unk_3 = 20 - unk_2;
            disp_mode = 1;
            display_sprite(poing_obj, 61, unk_3 - (init_hp - hp) * unk_1, 217, disp_mode);
            display_sprite(poing_obj, 60, 20 - unk_2 + 1, 216, disp_mode);
            switch (boss_obj->type)
            {
            case TYPE_MOSKITO:
                display_sprite(poing_obj, 62, 0, 206, disp_mode);
                break;
            case TYPE_MOSKITO2:
                display_sprite(poing_obj, 62, 0, 206, disp_mode);
                break;
            case TYPE_BB1:
                display_sprite(poing_obj, 106, 0, 206, disp_mode);
                break;
            case TYPE_BB12:
                display_sprite(poing_obj, 106, 0, 206, disp_mode);
                break;
            case TYPE_SPACE_MAMA:
                display_sprite(poing_obj, 108, 0, 206, disp_mode);
                break;
            case TYPE_SPACE_MAMA2:
                display_sprite(poing_obj, 108, 0, 206, disp_mode);
                break;
            case TYPE_MAMA_PIRATE:
                display_sprite(poing_obj, 111, 0, 206, disp_mode);
                break;
            case TYPE_SCORPION:
                display_sprite(poing_obj, 107, 0, 206, disp_mode);
                break;
            case TYPE_HYB_BBF2_D:
                display_sprite(poing_obj, 110, 0, 206, disp_mode);
                break;
            case TYPE_HYBRIDE_STOSKO:
                display_sprite(poing_obj, 110, 0, 206, disp_mode);
                break;
            case TYPE_HYBRIDE_MOSAMS:
                display_sprite(poing_obj, 110, 0, 206, disp_mode);
                break;
            case TYPE_DARK:
                display_sprite(poing_obj, 110, 0, 206, disp_mode);
                break;
            case TYPE_SAXO:
            case TYPE_SAXO2:
                display_sprite(poing_obj, 109, 0, 206, disp_mode);
                break;
            }
        }
    }
}

void DisplayCrackers(void) {
    display_text("/limited version/", 160, 100, 0, 0);
    display_text("/install rayman with cdrom/", 160, 150, 2, 1);
}

void DisplayProgrammerMessage(void) {
    display_text("/hi master/", 160, 100, 0, 0);
    display_text("/happy to see you/", 160, 150, 2, 1);
}

void DISPLAY_FIXE(s32 time) {
    s16 height;
    s16 width;
    obj_t* obj;
    s32 draw_y;

    if (ray_mode == MODE_5_CASSE_BRIQUE) {
        if (cb_ball_obj_id != -1)
            DISPLAY_FIXE_CB(level.objects + cb_ball_obj_id);
    } else {
        DO_DARK2_AFFICHE_TEXT();
        if (dead_time != 64 && !fixontemp)
            fixontemp = 300;
        if (fixontemp > 0) {
            if (fixontemp == 1 && GameModeVideo == MODE_NORMAL) {
                draw_buffer = display_buffer;
                ClearBorder(Bloc_lim_H1, Bloc_lim_H2, Bloc_lim_W1, Bloc_lim_W2);
                draw_buffer = DrawBufferNormal;
                ClearBorder(Bloc_lim_H1, Bloc_lim_H2, Bloc_lim_W1, Bloc_lim_W2);
            }
            --fixontemp;
        }
        if (fixon || fixontemp != 0 || time == 0) {
            height = 35;
            width = 77;
            if (GameModeVideo == MODE_NORMAL && (Bloc_lim_W1 > 16 || Bloc_lim_H1 > 5)) {
                if (id_Cling_Old) {
                    height = 45;
                    width = 85;
                }
                if (Bloc_lim_H1 - 5 >= height)
                    DISPLAY_BLACKBOX(13, 0, width, height, -1, 0);
                else {
                    DISPLAY_BLACKBOX(13, 0, width, Bloc_lim_H1, -1, 0);
                    DISPLAY_BLACKBOX(13, Bloc_lim_H1, Bloc_lim_W1 - 15,
                                     height + 5 - Bloc_lim_H1, -1, 0);
                }
                if (Bloc_lim_H1 - 5 >= 23)
                    DISPLAY_BLACKBOX(241, 4, 68, 23, -1, 0);
                else {
                    DISPLAY_BLACKBOX(241, 4, 68, Bloc_lim_H1 - 4, -1, 0);
                    DISPLAY_BLACKBOX(Bloc_lim_W2, Bloc_lim_H1, 312 - Bloc_lim_W2,
                                     28 - Bloc_lim_H1, -1, 0);
                }
            }
            obj = level.objects + sbar_obj_id;
            if (time == -2) {
                draw_y = (GameModeVideo == MODE_NORMAL && P486 == 1) ? 16 : 0;
                display_sprite_NoClip(obj, 27, 16, draw_y + 5, 1);
                display_sprite_NoClip(obj, 56, 244, draw_y + 5, 1);
                display_sprite_NoClip(obj, 28 + status_bar.lives_digits[0], 55, draw_y + 5, 1);
                display_sprite_NoClip(obj, 28 + status_bar.lives_digits[1], 70, draw_y + 5, 1);
                if (ray.hit_points != (u8)-1 && !fin_du_jeu) {
                    display_sprite_NoClip(obj, 17 + status_bar.hp_sprites[1],
                                          35, draw_y + 27, 1);
                    if (status_bar.max_hitp == 4)
                        display_sprite_NoClip(obj, 20 + status_bar.hp_sprites[0],
                                              59, draw_y + 27, 1);
                }
                display_sprite_NoClip(obj, 28 + status_bar.wiz_digits[0],
                                      276, draw_y + 5, 1);
                display_sprite_NoClip(obj, 28 + status_bar.wiz_digits[1],
                                      290, draw_y + 5, 1);
            } else {
                display_sprite_NoClip(obj, 56, Bloc_lim_W1 + 14, Bloc_lim_H2 - 32, 1);
                display_sprite_NoClip(obj, 28 + status_bar.wiz_digits[0],
                                      Bloc_lim_W1 + 46, Bloc_lim_H2 - 32, 1);
                display_sprite_NoClip(obj, 28 + status_bar.wiz_digits[1],
                                      Bloc_lim_W1 + 60, Bloc_lim_H2 - 32, 1);
            }
            if (id_Cling_1up != -1 || id_Cling_Pow != -1)
                DISPLAY_CLING();
        }
    }
}

void DISPLAY_POINT(s16 x, s16 y) {
    DrawWldPointPlan2Normal(PLAN2BIT, x, y - 2);
}

void DISPLAY_PTS_TO(s32 x1, s32 y1, s32 x2, s32 y2, s16 percentage)
{
    x1 += 52 - xmap;
    x2 += 52 - xmap;
    y1 += 35 - ymap;
    y2 += 35 - ymap;

    Bresenham(
        DISPLAY_POINT,
        x1, y1,
        x2, y2,
        7,
        percentage
    );
}

void DISPLAY_PTS_TO_PLAN2(s16 origin_x, s16 origin_y, s16 dest_x, s16 dest_y, s16 percent) {
    //TODO: figure out why too many points are being drawn
    Bresenham(DISPLAY_POINT, origin_x + 8, origin_y, dest_x + 8, dest_y, 7, percent);
}

void DISPLAY_CYMBALE(obj_t* obj, u8* draw_buf, u8 a3) {
    u16 begin;
    u16 end;
    u16 i;
    sprite_t* sprite;
    s16 x;
    s16 y;
    s32 proj_x;
    s32 proj_y;
    s32 proj_height;
    vec2b_t proj_size;
    u8* image_data;
    anim_t* anim = &obj->animations[obj->anim_index];
    anim_layer_t* layer = &anim->layers[(anim->layers_per_frame & 0x3FFF) * obj->anim_frame];

    if (!a3) {
        end = 5;
        begin = 3;
    } else {
        end = 2;
        begin = 0;
    }
    i = begin;
    for (; i <= end; i++) {
        sprite = &obj->sprites[layer[i].sprite_index];
        y = layer[i].y + obj->screen_y;
        if (sprite->id != 0) {
            x = layer[i].x + obj->screen_x;
            proj_y = get_proj_y(obj->scale, y);
            proj_x = get_proj_x(obj->scale, x);
            proj_height = get_proj_dist(obj->scale, sprite->outer_height);
            proj_size.width = (u8)get_proj_dist(obj->scale, sprite->outer_width);
            proj_size.height = (u8)proj_height;
            image_data = obj->img_buffer + sprite->offset_in_atlas;
            DrawSpriteNormalEtX(proj_x, sprite->color, proj_y, proj_size,
                                draw_buf, image_data);
        }
    }
}

void DISPLAY_ALL_OBJECTS(void) {
    s16 current_display_prio;
    s16 i;
    s16 j;
    obj_t* obj;
    s32 xmin;
    s32 xmax;
    s32 ymin;
    s32 ymax;
    s32 hp;
    s32 angle;
    s32 remainder;
    s32 divisor;

    for (current_display_prio = 7; current_display_prio >= 1; --current_display_prio) {
        for (i = 0; i < actobj.num_active_objects; ++i) {
            obj = level.objects + actobj.objects[i];
            if (current_display_prio == 1 &&
                (flags[obj->type].flags0 & 0x80) != 0)
                display_bar_boss(obj);
            if (obj->display_prio != current_display_prio)
                continue;
            if (obj->type == 0x9D && num_world == 5 && num_level == 8 && obj->x >= 300) {
                display2(obj);
                continue;
            }
            if (obj->type == 0x51) {
                DISPLAY_CYMBALE(obj, draw_buffer, 1);
            } else if (obj->type == 0xFE) {
                hp = (s8)obj->hit_points;
                angle = pente[hp < 0 ? -hp : hp];
                if (hp > 0)
                    angle = -angle;
                angle = (angle << 9) / 360;
                DISPLAY_DEFORMATION(obj, (s16)angle, 0x1000, 0x1000);
            } else if (obj->type == 0xFC || obj->type == 0xFB || obj->type == 0xAD) {
                get_sprite_clipping(&xmin, &xmax, &ymin, &ymax);
                sprite_clipping(xmin, xmax, ymin, Bloc_lim_H2 - 40);
                display2(obj);
                sprite_clipping(xmin, xmax, ymin, ymax);
            } else {
                display2(obj);
            }
        }
        if (current_display_prio == 3) {
            divisor = 2 * display_mode + 2;
            remainder = ray.iframes_timer % divisor;
            if (gele || remainder <= display_mode ||
                (ray.iframes_timer > 90 && !RayEvts.squashed)) {
                if (ray.flags.alive && ray.is_active) {
                    display2(&ray);
                    DISPLAY_POING();
                }
            }
        } else if (current_display_prio == 2 && nb_cymbal_in_map != 0) {
            for (j = 0; j < nb_cymbal_in_map; ++j) {
                obj = level.objects + cymbal_obj_id[j];
                if (obj->id != 0)
                    DISPLAY_CYMBALE(obj, draw_buffer, 0);
            }
        }
    }
    for (i = 0; i < actobj.num_active_objects; ++i) {
        obj = level.objects + actobj.objects[i];
        if (obj->display_prio == 0 && obj->type == 0xF5) {
            display2(obj);
            break;
        }
    }
}

void display_flocons_behind(void) {
    s16 previous_center_x;
    s16 previous_center_y;
    s16 layer;
    s16 group;
    s16 depth;
    s16 index;
    s16 end_index;
    s32 multiplier;
    s32 center_x;
    s32 center_y;
    flocon_t* entry;
    void (*draw_func)(s16, s16);

    previous_center_x = PROJ_CENTER_X;
    previous_center_y = PROJ_CENTER_Y;
    set_proj_center(160, 170);
    layer = 6;
    group = 3;
    depth = 32;
    while (depth < 192) {
        --layer;
        multiplier = 0x10000 / (depth + 256);
        center_x = PROJ_CENTER_X - multiplier * PROJ_CENTER_X / 256;
        center_y = PROJ_CENTER_Y - multiplier * PROJ_CENTER_Y / 256;
        index = floc_ind[group];
        entry = flocon_tab + index;
        end_index = index + nb_floc[group];
        if (num_world != 1) {
            if (layer == 5) draw_func = draw_flocon5_Normal;
            else if (layer == 4) draw_func = draw_flocon4_Normal;
            else if (layer == 3) draw_func = draw_flocon3_Normal;
            else if (layer == 2) draw_func = draw_flocon2_Normal;
            else draw_func = draw_flocon1_Normal;
        } else {
            if (layer == 5) draw_func = draw_pluie6_Normal;
            else if (layer >= 3) draw_func = draw_pluie5_Normal;
            else draw_func = draw_pluie4_Normal;
        }
        for (; index < end_index; ++index, ++entry)
            draw_func((s16)((multiplier * entry->field_0 >> 8) + center_x),
                      (s16)((multiplier * entry->field_2 >> 8) + center_y));
        ++group;
        depth += 32;
    }
    PROJ_CENTER_X = previous_center_x;
    PROJ_CENTER_Y = previous_center_y;
}

void display_flocons_before(void) {
    s16 previous_center_x;
    s16 previous_center_y;
    s16 group;
    s16 depth;
    s16 index;
    s16 end_index;
    s32 multiplier;
    s32 center_x;
    s32 center_y;
    flocon_t* entry;
    void (*draw_func)(s16, s16);

    previous_center_x = PROJ_CENTER_X;
    previous_center_y = PROJ_CENTER_Y;
    set_proj_center(160, 170);
    group = 0;
    depth = -64;
    while (depth < 32) {
        index = floc_ind[group];
        end_index = index + nb_floc[group];
        entry = flocon_tab + index;
        multiplier = 0x10000 / (depth + 256);
        center_x = PROJ_CENTER_X - multiplier * PROJ_CENTER_X / 256;
        center_y = PROJ_CENTER_Y - multiplier * PROJ_CENTER_Y / 256;
        if (num_world != 1)
            draw_func = group == 0 ? draw_flocon7_Normal : draw_flocon6_Normal;
        else
            draw_func = draw_pluie7_Normal;
        for (; index < end_index; ++index, ++entry)
            draw_func((s16)((multiplier * entry->field_0 >> 8) + center_x),
                      (s16)((multiplier * entry->field_2 >> 8) + center_y));
        ++group;
        depth += 32;
    }
    PROJ_CENTER_X = previous_center_x;
    PROJ_CENTER_Y = previous_center_y;
}

void display_pix_gerbes(void) {
    u8 color;
    s16 i;
    s16 j;
    pix_gerbe_item_t* cur_item;
    u8 unk_1;
    s16 spd_y;

    i = 0;
    do {
        if (pix_gerbe[i].is_active) {
            cur_item = &pix_gerbe[i].items[0];
            j = 0;
            do {
                unk_1 = cur_item->unk_1;
                if (unk_1 > 127 && cur_item->y_pos > (Bloc_lim_H1_Aff << 6)) {
                    spd_y = (u8)cur_item->speed_y;
                    if (spd_y > 127)
                        color = 88;
                    else
                        color = (unk_1 & 127) + ((spd_y >> 5) + 4);
                    fplotNormalETX(draw_buffer, cur_item->x_pos >> 6,
                                   cur_item->y_pos >> 6, color);
                }
                cur_item++;
                ++j;
            } while (j < LEN(pix_gerbe[i].items));
        }
        ++i;
    } while (i < LEN(pix_gerbe));
}

void DISPLAY_BLACKBOX(s16 x, s16 y, s16 width, s16 height, s16 font_size, u8 is_fond) {
    s32 clip_height = height;
    s32 clip_width = width;
    s32 clip_x = x;
    s32 clip_y = y;
    if (x < 0) {
        clip_x = 0;
        clip_width = x + width;
    } else if (x > 320) {
        clip_x = 320;
    }
    if (clip_width + clip_x > 320) {
        clip_width = 320 - clip_x;
    }
    if (y < 0) {
        clip_y = 0;
        clip_height = y + height;
    } else if (y > 200) {
        clip_y = 200;
    }
    if (clip_height + clip_y > 200) {
        clip_height = 200 - clip_y;
    }

    if (clip_width > 0 && clip_height > 0) {
        if (font_size == -1) {
            if (ModeVideoActuel == MODE_X) {
                //draw_borderbox();
            } else {
                if (is_fond == 0) {
                    DrawBlackBoxNormal(draw_buffer, clip_x, clip_y, clip_height, clip_width);
                } else {
                    DrawFondBoxNormal(draw_buffer, clip_x, clip_y, clip_height, clip_width, is_fond);
                }
            }
        }
        if (is_fond == 1) {
            if (ModeVideoActuel != MODE_X) {
                DrawBorderBoxNormal(draw_buffer, clip_x, clip_y, clip_height, clip_width, 0x2026);
            }
        } else if (is_fond == 2) {
            if (ModeVideoActuel != MODE_X) {
                DrawBorderBoxNormal(draw_buffer, clip_x, clip_y, clip_height, clip_width, 0x2020);
            } else {
                //draw_borderbox();
            }
        }
    }
}

void display_text(const char* text, s16 x, s16 y, u8 font_size, u8 color) {
    s32 line_height;
    s16 char_spacing;
    s16 space_width;
    s32 num_let;
    s32 current_x;
    sprite_t* sprite;
    s32 i;
    const char* pos;
    char c;
    s32 let_width;
    vec2b_t size;

    if (font_size == 2) {
        line_height = 15;
        char_spacing = 1;
        space_width = 8;
    } else if (font_size == 1) {
        line_height = 23;
        char_spacing = 3;
        space_width = 10;
    } else {
        line_height = 36;
        char_spacing = 3;
        space_width = 12;
    }
    current_x = x;
    sprite = NULL;
    num_let = 0;
    if (text != NULL) {
        for (i = 0; text[i] != 0; i++) {
            pos = text + i;
            c = *pos;
            if (c == '/') {
                current_x = x - calc_largmax_text(text, (s16)i, space_width,
                                                 char_spacing, font_size) / 2;
                if (i > 1)
                    y += line_height;
                num_let = 0;
            } else if (c == ' ') {
                num_let = 0;
                current_x += space_width;
            } else {
                num_let = deter_num_let(c, pos + 1);
            }
            if (num_let != 0) {
                let_width = calc_let_Width(font_size, num_let);
                if (num_let < 1000) {
                    if (font_size <= 1) {
                        if (font_size == 1)
                            num_let += 41;
                        sprite = alpha2.sprites + num_let;
                        size.width = (u8)let_width;
                        size.height = sprite->outer_height;
                        DrawSpriteColorNormalEtX(current_x, color,
                            y - sprite->outer_height,
                            size, draw_buffer,
                            alpha2.img_buffer + sprite->offset_in_atlas);
                    } else if (font_size == 2) {
                        sprite = alpha.sprites + num_let;
                        size.width = (u8)let_width;
                        size.height = sprite->outer_height;
                        DrawSpriteColorNormalEtX(current_x, color,
                            y - sprite->outer_height,
                            size, draw_buffer,
                            alpha.img_buffer + sprite->offset_in_atlas);
                    }
                } else {
                    sprite = alpha_numbers.sprites + (num_let - 1000);
                    size.width = (u8)let_width;
                    size.height = sprite->outer_height;
                    DrawSpriteColorNormalEtX(current_x, color,
                        y - sprite->outer_height,
                        size, draw_buffer,
                        alpha_numbers.img_buffer + sprite->offset_in_atlas);
                    i += 3;
                }
                current_x += (sprite->sprite_pos.raw & 0xF) + sprite->inner_width - char_spacing;
            }
        }
    }
}

void display_deform_text(const char* text, s16 x, s16 y, u8 font_size, s8 color, s16 rotation, s16 enx, s16 eny) {
    s32 line_height;
    s16 char_spacing;
    s16 space_width;
    s32 index;
    s32 number;
    s32 current_x;
    s32 current_y;
    sprite_t* sprite;
    font_t* font;
    u8 sprite_index;

    if (font_size == 2) {
        line_height = 15;
        char_spacing = 1;
        space_width = 8;
    } else if (font_size == 1) {
        line_height = 23;
        char_spacing = 3;
        space_width = 10;
    } else {
        line_height = 36;
        char_spacing = 3;
        space_width = 12;
    }
    current_x = x;
    current_y = y;
    sprite = NULL;
    if (text != NULL && text[0] != '\0') {
        for (index = 0; text[index] != '\0'; ++index) {
            if (text[index] == '/') {
                current_x = x - calc_largmax_text(text, (s16)index,
                    space_width, char_spacing, font_size) / 2;
                if (index > 1)
                    current_y += line_height;
                number = 0;
            } else if (text[index] == ' ') {
                current_x += space_width;
                number = 0;
            } else {
                number = deter_num_let((u8)text[index], text + index + 1);
            }
            if (number != 0) {
                calc_let_Width(font_size, number);
                calc_num_let_spr(font_size, &sprite_index);
                if (font_size <= 1) {
                    font = &alpha2;
                    sprite = font->sprites + sprite_index;
                } else if (font_size == 2) {
                    font = &alpha;
                    sprite = font->sprites + sprite_index;
                } else {
                    font = &alpha_numbers;
                    sprite = font->sprites + sprite_index;
                }
                DISPLAY_DEFORM_SPRITE(sprite,
                    font->img_buffer + sprite->offset_in_atlas,
                    (s16)current_x,
                    (s16)(current_y - sprite->outer_height),
                    rotation, enx, eny);
                current_x += (sprite->sprite_pos.raw & 0x0f) +
                    sprite->inner_width - char_spacing;
            }
        }
    }
}

void display_text_sin(const char* text, s16 x, s16 y, u8 font_size, s8 color, u8 phase) {
    s32 line_height;
    s16 char_spacing;
    s16 space_width;
    s32 index;
    s32 number;
    s32 angle;
    s32 vertical_offset;
    s32 let_width;
    s32 current_x;
    s32 current_y;
    sprite_t* sprite;
    font_t* font;
    vec2b_t size;

    if (font_size == 2) {
        line_height = 15;
        char_spacing = 1;
        space_width = 8;
    } else if (font_size == 1) {
        line_height = 23;
        char_spacing = 3;
        space_width = 10;
    } else {
        line_height = 36;
        char_spacing = 3;
        space_width = 12;
    }
    current_x = x;
    current_y = y;
    if (text != NULL && text[0] != '\0') {
        for (index = 0; text[index] != '\0'; ++index) {
            if (text[index] == '/') {
                current_x = x - calc_largmax_text(text, (s16)index, space_width,
                                                  char_spacing, font_size) / 2;
                if (index > 1)
                    current_y += line_height;
                number = 0;
            } else if (text[index] == ' ') {
                current_x += space_width;
                number = 0;
            } else {
                number = deter_num_let((u8)text[index], text + index + 1);
            }
            if (number != 0) {
                let_width = calc_let_Width(font_size, number);
                calc_num_let_spr(font_size, (u8*)&number);
                angle = cosinus(phase + index * 32);
                if (angle < 0)
                    vertical_offset = -((-angle) >> 5);
                else
                    vertical_offset = angle >> 5;
                if (number >= 1000) {
                    font = &alpha_numbers;
                    sprite = font->sprites + (number - 1000);
                    size.width = (u8)let_width;
                    size.height = sprite->outer_height;
                    DrawSpriteColorNormalEtX(current_x, color,
                        current_y - sprite->outer_height + vertical_offset, size,
                        draw_buffer, font->img_buffer + sprite->offset_in_atlas);
                    index += 3;
                } else if (font_size <= 1) {
                    font = &alpha2;
                    sprite = font->sprites + number;
                    size.width = (u8)let_width;
                    size.height = sprite->outer_height;
                    DrawSpriteColorNormalEtX(current_x, color,
                        current_y - sprite->outer_height + vertical_offset, size,
                        draw_buffer, font->img_buffer + sprite->offset_in_atlas);
                } else if (font_size == 2) {
                    font = &alpha;
                    sprite = font->sprites + number;
                    size.width = (u8)let_width;
                    size.height = sprite->outer_height;
                    DrawSpriteColorNormalEtX(current_x, color,
                        current_y - sprite->outer_height + vertical_offset, size,
                        draw_buffer, font->img_buffer + sprite->offset_in_atlas);
                }
                current_x += (sprite->sprite_pos.bits.x + sprite->inner_width) - char_spacing;
            }
        }
    }
}

void display_box_text(display_item_t* box) {
    s16 blackbox_x = (s16)(box->centered_x_pos - 3);
    s16 blackbox_y = (s16)(box->centered_y_pos - 3);
    s16 blackbox_width = (s16)(box->width + 6);
    s16 blackbox_height = (s16)(box->height + 6);
    if (box->field_D5) {
        DISPLAY_BLACKBOX(blackbox_x, blackbox_y, blackbox_width, blackbox_height, 255, box->is_fond);
        if (box->is_fond == 1) {
            display_text(box->text, box->xpos - 1, box->ypos - 1, box->font_size, (u8)box->color);
        } else if (box->is_fond == 2) {
            display_text(box->text, box->xpos + 1, box->ypos + 1, box->font_size, (u8)box->color);
        } else {
            display_text(box->text, box->xpos, box->ypos, box->font_size, (u8)box->color);
        }
    } else {
        if (box->is_fond != 0) {
            DISPLAY_BLACKBOX(blackbox_x, blackbox_y, blackbox_width, blackbox_height, 254, box->is_fond);
        }
        display_text(box->text, box->xpos, box->ypos, box->font_size, (u8)box->color);
    }
}

void CLRSCR(void) {
    if (ModeVideoActuel == MODE_X) {
        display_emptypicture();
    } else {
        memset(DrawBufferNormal, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
    }
}

void display_etoile(s16 in_x, s16 in_y) {
    u8 loc_star_spr[8] = {21, 22, 23, 24, 21, 25, 22, 21};
    grp_star_t* star;
    u8 rand_res;
    s16 draw_x;
    s16 draw_y;
    sprite_t* sprite;
    vec2b_t proj_size;

    star = &grp_stars[current_star];
    star->timer++;
    if (star->timer > star->length) {
        rand_res = myRand(7);
        star->timer = 0;
        star->length = myRand(5);
        star->dist = rand_res;
        star->sprite_table_index = rand_res;
    }
    draw_x = star->dist + (in_x - xmap) - 8;
    draw_y = star->dist + (in_y - ymap);
    if (draw_x > 0 && SCREEN_WIDTH - draw_x > 0 && draw_y > 0 && SCREEN_HEIGHT - draw_y > 0) {
        sprite = poing_obj->sprites + loc_star_spr[star->sprite_table_index];
        proj_size.width = (u8)sprite->outer_width;
        proj_size.height = (u8)sprite->outer_height;
        DrawSpriteNormalEtX(draw_x, sprite->color >> 4, draw_y, proj_size,
                            draw_buffer, poing_obj->img_buffer + sprite->offset_in_atlas);
    }
    if (current_star < LEN(grp_stars))
        current_star++;
}

void display_grp_stars(void)
{
    obj_t *grapped;
    s16 x; s16 y; s16 w; s16 h;
    s16 cen_x;
    s16 grapped_x;

    if (ray.main_etat == 7 && (ray.flags.alive))
    {
        current_star = 0;
        grapped = &level.objects[id_obj_grapped];
        GET_SPRITE_POS(&ray, 1, &x, &y, &w, &h);

        cen_x = x + (w >> 1);
        grapped_x = grapped->offset_bx + grapped->x;
        Bresenham(
            display_etoile,
            cen_x, (s16) (y + (h >> 1) - 6),
            grapped_x, (s16) (grapped->offset_hy + grapped->y),
            5,
            100
        );
    }
}

void DISPLAY_TEXT_FEE(void) {
    /* 19D20 8013E520 -O2 -msoft-float */
    obj_t* obj;
    display_item_t ttd;
    s16 cen_x;
    s16 obj_x;
    s16 marg_x;

    DISPLAY_BLACKBOX(0, 0, 320, 16, 255, 0);
    DISPLAY_BLACKBOX(0, 160, 320, 40, 255, 0);
    if (display_txt_fee != 255)
    {
        obj = &level.objects[png_or_fee_id];
        memcpy(&ttd, (text_to_display + 1) + display_txt_fee, sizeof(ttd));
        if (ttd.text[0] != '\0')
        {
            if (obj->flags.alive)
            {
                s16 y_fee = 190;
                obj->screen_y = y_fee - obj->offset_by;
                cen_x = ttd.centered_x_pos;
                obj_x = obj->offset_bx + obj->screen_x;
                marg_x = 8;
                if (cen_x < obj_x - marg_x)
                {
                    if (cen_x + ttd.width > obj_x)
                    {
                        ttd.text[(s16) ((obj_x - (cen_x + marg_x)) / 7)] = '\0';
                        display_box_text(&ttd);
                    }
                    else
                        display_box_text(&ttd);
                }
                else if (cen_x + ttd.width <= obj_x)
                    display_box_text(&ttd);

                DISPLAY_BLACKBOX(obj_x, obj->screen_y + obj->offset_by - 20, 30, 18, 255, 0);
                display2(obj);
            }
            else
                display_box_text(&ttd);
        }
    }

    if (old_txt_fee != 255) {
        memcpy(&ttd, (text_to_display + 1) + old_txt_fee, sizeof(ttd));
        if (ttd.text[0] != '\0')
            display_box_text(&ttd);
    }
}

void DISPLAY_SAVE_SPRITES(s16 x, s16 save_index) {
    s16 y;
    loadinforay_t* loadinfo;
    s32 sprite_index;
    u16 percentage;

    y = save_index * (ecarty + 23) + debut_options - 23;
    loadinfo = LoadInfoRay + save_index;
    sprite_index = 27;
    display_sprite(&div_obj, 27, x, y, 1);
    x += div_obj.sprites[sprite_index].outer_width;
    sprite_index = (loadinfo->lives / 10) % 10 + 28;
    display_sprite(&div_obj, (u8)sprite_index, x, y, 1);
    x += div_obj.sprites[sprite_index].outer_width;
    sprite_index = loadinfo->lives % 10 + 28;
    display_sprite(&div_obj, (u8)sprite_index, x, y, 1);
    x += div_obj.sprites[28].outer_width + 10;
    sprite_index = 57;
    display_sprite(&mapobj[0], (u8)sprite_index, x, y, 1);
    x += mapobj[0].sprites[sprite_index].outer_width;
    sprite_index = loadinfo->continues % 10 + 28;
    display_sprite(&div_obj, (u8)sprite_index, x, y, 1);
    x += div_obj.sprites[37].outer_width + 10;
    percentage = (u16)(loadinfo->cages * 100 / 102);
    sprite_index = percentage / 100 + 28;
    display_sprite(&div_obj, (u8)sprite_index, x, y, 1);
    x += div_obj.sprites[sprite_index].outer_width;
    sprite_index = (percentage / 10) % 10 + 28;
    display_sprite(&div_obj, (u8)sprite_index, x, y, 1);
    x += div_obj.sprites[sprite_index].outer_width;
    sprite_index = percentage % 10 + 28;
    display_sprite(&div_obj, (u8)sprite_index, x, y, 1);
    x += div_obj.sprites[sprite_index].outer_width;
    display_text("%", x, debut_options + save_index * (ecarty + 23), 1, 2);
}

void DISPLAY_SAVE_POING(void) {
    display_sprite(&mapobj[0], 2, 10, (positiony - 1) * (ecarty + 23) + debut_options - 23, 0);
    if (fichier_a_copier) {
        display_sprite(&mapobj[0], 1, 10, (fichier_a_copier - 1) * (ecarty + 23) + debut_options - 23, 0);
    }
}

void DISPLAY_OPTIONS_POING(void) {
    s16 x;
    s16 y;
    s16 menu_index;
    if (position == option_exit) {
        menu_index = ypadmin;
        x = menu_to_display[menu_index].centered_x_pos - 30;
        y = debut_sortie - 13;
    } else {
        menu_index = compteur;
        x = menu_to_display[menu_index].centered_x_pos - 30;
        y = menu_to_display[menu_index].centered_y_pos;
    }
    display_sprite(&mapobj[0], 2, x, y, 0);
}

void DISPLAY_YESNO_POING(void) {
    s16 y;
    y = 117 + (pos_YN - 1) * 22;
    display_sprite(&mapobj[0], 2, 120, y, 0);
}

void display_time(s32 time) {
    // NOTE(Falcury): based on the PC version (the PS1 version is slightly different)
    obj_t *sbar_obj;
    char text[24];
    u8 nb_wiz_text[8];
    s32 time_div;
    u32 col;
    u32 col_add;

    sbar_obj = &level.objects[sbar_obj_id];
    if (time != -2) {
        u8 seconds = time / 60 % 10;
        u8 tens = time / 600 % 10;
        u8 hundreds = time / 6000 % 10;
        s16 x_offset = 0;

        display_text(language_txt[159], Bloc_lim_W2 - 52, Bloc_lim_H2 - 32, 2, 5); // /time/
        if (hundreds != 0) {
            display_sprite(sbar_obj, hundreds + 28, Bloc_lim_W2 - 50, Bloc_lim_H2 - 30, 1);
            x_offset = 15;
        }
        display_sprite(sbar_obj, tens + 28, Bloc_lim_W2 - 50 + x_offset, Bloc_lim_H2 - 30, 1);
        display_sprite(sbar_obj, seconds + 28, Bloc_lim_W2 - 35 + x_offset, Bloc_lim_H2 - 30, 1);

        if (map_time > 100) {
            if (map_time <= 160 && map_time > 120) {
                strcpy(text, "/go !/");
                display_text(text, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, 5);
            }
        } else {
            ray.is_active = 0;
            sprintf(text, language_txt[156], nb_wiz); // /%d tings to get/
            if (horloge[8] < 4) {
                col = PC_TingsToGet_Col;
                col_add = PC_TingsToGet_ColAdd;
                PC_TingsToGet_Col = col + col_add;
                if (PC_TingsToGet_Col >= 6) {
                    PC_TingsToGet_ColAdd *= -1;
                    PC_TingsToGet_Col = 5;
                } else if (PC_TingsToGet_Col == 0) {
                    PC_TingsToGet_ColAdd *= -1;
                    PC_TingsToGet_Col = 1;
                }

                display_text(text, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2, PC_TingsToGet_Col);
            }
        }
    }
}

void DISPLAY_CONTINUE_SPR(void) {
    u8 tens;
    u8 ones;
    tens = nb_continue / 10;
    ones = nb_continue % 10;
    if (loop_timing == 0x00FF) {
        if (language == 0)
            display_text("/game over/", 181, 182, 2, 2);
        else
            display_text("/game over/", 181, 182, 2, 2);
    } else {
        display_sprite(&div_obj, 27, 146, 162, 1);
        DISPLAY_BLACKBOX(146, 162, 22, 34, -1, 0);
        display_sprite(&div_obj, tens + 28, 188, 162, 1);
        display_sprite(&div_obj, ones + 28, 203, 162, 1);
        display_sprite(&div_obj, (u8)(loop_timing / 10 + 28), 52, 162, 1);
        display_sprite(&div_obj, (u8)(loop_timing % 10 + 28), 67, 162, 1);
    }
    display2(&ray);
    display2(&clock_obj);
    if (mapobj[0].flags.alive)
        display2(&mapobj[0]);
}

void AFFICHE_SPRITE_DEFORME(sprite_t* sprite, u8* image, s16 x0, s16 y0,
                            s16 x1, s16 y1, s16 x2, s16 y2, s16 x3, s16 y3) {
    deform_blit_t geometry;
    s32 min_x;
    s32 max_x;
    s32 min_y;
    s32 max_y;
    vec2b_t size;

    min_x = x0;
    max_x = x0;
    min_y = y0;
    max_y = y0;
    if (x1 < min_x) min_x = x1;
    if (x2 < min_x) min_x = x2;
    if (x3 < min_x) min_x = x3;
    if (x1 > max_x) max_x = x1;
    if (x2 > max_x) max_x = x2;
    if (x3 > max_x) max_x = x3;
    if (y1 < min_y) min_y = y1;
    if (y2 < min_y) min_y = y2;
    if (y3 < min_y) min_y = y3;
    if (y1 > max_y) max_y = y1;
    if (y2 > max_y) max_y = y2;
    if (y3 > max_y) max_y = y3;
    geometry.corner[0] = x0;
    geometry.corner[1] = y0;
    geometry.corner[2] = x1;
    geometry.corner[3] = y1;
    geometry.corner[4] = x2;
    geometry.corner[5] = y2;
    geometry.corner[6] = x3;
    geometry.corner[7] = y3;
    geometry.dest_width = max_x - min_x + 1;
    geometry.dest_height = max_y - min_y + 1;
    geometry.dest_offset = min_y * 320 + min_x;
    geometry.source_width = sprite->outer_width;
    geometry.source_height = sprite->outer_height;
    draw_sprite_deform(image + sprite->offset_in_atlas, new_sprite_deform, &geometry);
    size.width = (u8)geometry.dest_width;
    size.height = (u8)geometry.dest_height;
    DrawSpriteNormalEtX(min_x, sprite->color >> 4, min_y, size,
                        draw_buffer, new_sprite_deform);
}

void DISPLAY_DEFORMATION(obj_t* obj, s16 angle, s16 scale_x, s16 scale_y) {
    anim_t* anim;
    anim_layer_t* layer;
    sprite_t* sprite;
    u16 layers_per_frame;
    s32 layer_index;
    s16 sprite_x;
    s16 sprite_y;
    s16 sprite_width;
    s16 sprite_height;
    s16 origin_x;
    s16 origin_y;

    anim = obj->animations + obj->anim_index;
    layers_per_frame = anim->layers_per_frame & 0x3FFF;
    layer = anim->layers + obj->anim_frame * layers_per_frame;
    origin_x = obj->screen_x;
    origin_y = obj->screen_y;
    if (obj->flags.follow_enabled) {
        GET_SPRITE_POS(obj, obj->follow_sprite, &sprite_x, &sprite_y,
                       &sprite_width, &sprite_height);
        origin_x = (s16)(sprite_x - xmap + 8 + (sprite_width >> 1));
        origin_y = (s16)(sprite_y - ymap + obj->offset_by - obj->offset_hy);
    }
    for (layer_index = 0; layer_index < layers_per_frame; ++layer_index, ++layer) {
        if (layer->sprite_index != 0) {
            sprite = obj->sprites + layer->sprite_index;
            if (sprite->id != 0)
                DISPLAY_DEFORM_SPRITE(sprite, obj->img_buffer,
                    origin_x + layer->x, origin_y + layer->y,
                    angle, scale_x, scale_y);
        }
    }
}

void DISPLAY_DEFORM_SPRITE(sprite_t* sprite, u8* image, s16 x, s16 y,
                           s16 angle, s16 scale_x, s16 scale_y) {
    s16 center_x;
    s16 center_y;
    s16 x_points[4];
    s16 y_points[4];
    s16 left;
    s16 top;
    s16 right;
    s16 bottom;
    s16 fourth_x;
    s16 fourth_y;

    left = sprite->sprite_pos.bits.x;
    top = sprite->sprite_pos.bits.y;
    center_x = x + left + (sprite->inner_width >> 1);
    center_y = y + top + 1 + (sprite->inner_height >> 1);
    x_points[0] = (s16)(x - center_x);
    y_points[0] = (s16)(y - center_y);
    x_points[1] = (s16)(x + sprite->outer_width - center_x);
    y_points[1] = y_points[0];
    x_points[2] = x_points[0];
    y_points[2] = (s16)(y + sprite->outer_height - center_y);
    CALCUL_DEFORMATION(&x_points[0], &y_points[0], angle, scale_x, scale_y);
    CALCUL_DEFORMATION(&x_points[1], &y_points[1], angle, scale_x, scale_y);
    CALCUL_DEFORMATION(&x_points[2], &y_points[2], angle, scale_x, scale_y);
    fourth_x = (s16)(x_points[1] + x_points[2] - x_points[0]);
    fourth_y = (s16)(y_points[1] + y_points[2] - y_points[0]);
    x_points[0] += center_x;
    x_points[1] += center_x;
    x_points[2] += center_x;
    fourth_x += center_x;
    y_points[0] += center_y;
    y_points[1] += center_y;
    y_points[2] += center_y;
    fourth_y += center_y;
    AFFICHE_SPRITE_DEFORME(sprite, image,
        x_points[0], y_points[0], x_points[1], y_points[1],
        x_points[2], y_points[2], fourth_x, fourth_y);
}

void CALCUL_DEFORMATION(s16* x, s16* y, s16 angle, s16 scale_x, s16 scale_y) {
    s16 sin_value;
    s16 cos_value;
    s32 scaled_x;
    s32 scaled_y;

    scaled_x = ((s32)*x * scale_x) >> 12;
    scaled_y = ((s32)*y * scale_y) >> 12;
    sinus_cosinus(angle, &sin_value, &cos_value);
    *x = (s16)((scaled_x * cos_value - scaled_y * sin_value) >> 9);
    *y = (s16)((scaled_x * sin_value + scaled_y * cos_value) >> 9);
}

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
    u8 sub_etat;
    s16 poing_x; s16 poing_y_1; s16 poing_y_2;
    s16 spipl_x; s16 spipl_y_1; s16 spipl_y_2;
    s16 i;
    obj_t *cur_obj;
    s16 nb_objs;
    s16 spipl_spr_x; s16 spipl_spr_y; s16 spipl_spr_w; s16 spipl_spr_h;
    u8 new_flip;
    s16 cen_x = get_center_x(spipl_obj);
    s16 cen_y = get_center_y(spipl_obj);

    if (
        (
            spipl_obj->main_etat == 0 &&
            (sub_etat = spipl_obj->sub_etat, sub_etat == 27 || sub_etat == 24 || sub_etat == 30 || sub_etat == 11)
        ) ||
        (spipl_obj->main_etat == 1 && spipl_obj->sub_etat == 2)
    ) {
        if (poing.is_active) {
            poing_x = poing_obj->x + poing_obj->offset_bx;
            poing_y_1 = poing_obj->y + poing_obj->offset_by;
            poing_y_2 = poing_obj->y + poing_obj->offset_hy;

            spipl_x = spipl_obj->x + spipl_obj->offset_bx;
            spipl_y_1 = spipl_obj->y + spipl_obj->offset_by + 5;
            spipl_y_2 = spipl_obj->y + spipl_obj->offset_hy - 5;
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
            i = 0;
            cur_obj = &level.objects[i];
            nb_objs = level.nb_objects;
            while (i < nb_objs) {
                if ((cur_obj->type == TYPE_DARD_PLAFOND) && !cur_obj->is_active) {
                    GET_SPRITE_POS(spipl_obj, 2, &spipl_spr_x, &spipl_spr_y, &spipl_spr_w, &spipl_spr_h);
                    cur_obj->speed_x = 0;
                    cur_obj->speed_y = 32;
                    cur_obj->cmd_offset = -1;
                    cur_obj->flags.flip_x = 0;

                    cur_obj->init_x =
                    cur_obj->x =
                            spipl_spr_x - cur_obj->offset_bx + (spipl_spr_w >> 1);

                    cur_obj->init_y =
                    cur_obj->y =
                            spipl_spr_y - cur_obj->offset_by + (spipl_spr_h >> 1);

                    cur_obj->is_active = 1;
                    cur_obj->flags.alive = 1;
                    add_alwobj(cur_obj);
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
        (sub_etat = spipl_obj->sub_etat, sub_etat == 20 || sub_etat == 13 || sub_etat == 23)
    ) {
        new_flip = ray.x > spipl_obj->x;
        spipl_obj->flags.flip_x = new_flip;
    }
}

void DO_SPIDER_PLAFOND_POING_COLLISION(obj_t* obj, s16 sprite) {
    // NOTE(Falcury): the PS1 and PC versions seem functionally identical.
    s32 main_etat;
    u8 new_sub_etat;

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
            new_sub_etat = 19;
        } else {
            new_sub_etat = 18;
        }
        set_main_and_sub_etat(obj, 0, new_sub_etat);
        obj->speed_y = -8;
        obj->speed_x = 0;
        obj->flags.read_commands = 0;
    } else {
        main_etat = obj->main_etat;
        if ((main_etat == 1 && obj->sub_etat == 2) ||
            (main_etat == 0 &&
             (obj->sub_etat == 11 || obj->sub_etat == 29 ||
              obj->sub_etat == 25 || obj->sub_etat == 26 ||
              obj->sub_etat == 27 || obj->sub_etat == 24 ||
              obj->sub_etat == 30))) {
            set_main_and_sub_etat(obj, 0, 14);
            obj->speed_y = 0;
            obj->speed_x = 0;
        } else if (main_etat == 0 &&
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

    if (obj->main_etat == 0 && (obj->sub_etat == 12 || obj->sub_etat == 13))
        set_sub_etat(obj, 20);
    else if (
        (obj->main_etat == 0 && (obj->sub_etat == 11 || obj->sub_etat == 30)) ||
        (obj->main_etat == 1 && obj->sub_etat == 2)
    ) {
        prev_flip = obj->flags.flip_x;
        prev_flip &= 1;
        if ((ray.x > obj->x && prev_flip != 1) || /* ??? */
            (ray.x <= obj->x && prev_flip != 0)
        ) {
            new_flip = ray.x > obj->x;
            obj->flags.flip_x = new_flip;
            obj->speed_x = 0;
            obj->speed_y = 0;
            set_main_and_sub_etat(obj, 0, 24);
        }
    }
}

void DO_DARD_PLAFOND_ALWAYS(obj_t* obj) {
    s16 dx;
    s16 dy;
    s16 distance;
    s16 ray_x;
    s16 ray_y;
    s16 obj_x;
    s16 obj_y;
    s16 speed_x;
    s16 speed_y;
    s16 sign_x;
    s16 sign_y;
    s16 abs_speed_x;
    s16 abs_speed_y;

    ray_x = (s16)((ray.x >> 16) + ray.offset_bx);
    ray_y = (s16)((ray.y >> 16) + ray.offset_by);
    obj_x = (s16)((obj->x >> 16) + obj->offset_bx);
    obj_y = (s16)((obj->y >> 16) + obj->offset_by);
    if (obj->cmd == 1) {
        if (ray_x > obj_x)
            obj->flags.flip_x = 0;
        else if (ray_x < obj_x)
            obj->flags.flip_x = 1;
    } else {
        dx = (s16)(ray_x - obj_x);
        if (position_ray)
            dy = (s16)(ray_y - obj_y);
        else
            dy = (s16)(ray.y / 65536 + ((u8)ray.offset_by >> 1) - obj_y);
        distance = (s16)(abs(dx) + abs(dy));
        if (ray_y > (s16)((obj->y >> 16) + obj->offset_by - obj->offset_hy) &&
            ((ray_x < obj_x && obj->flags.flip_x) ||
             (ray_x > obj_x && !obj->flags.flip_x))) {
            if (distance != 0) {
                dx = (s16)(((s32)dx << 5) / distance);
                dy = (s16)(((s32)dy << 5) / distance);
            }
            speed_y = obj->speed_y;
            if (dy > speed_y) sign_y = 1;
            else if (dy < speed_y) sign_y = -1;
            else sign_y = 0;
            obj->speed_y = (s16)(speed_y + sign_y);
            if (obj->speed_y < 0) obj->speed_y = 0;
            if (obj->speed_y > 32) obj->speed_y = 32;
            if (obj->speed_y != 0) {
                speed_x = obj->speed_x;
                if (dx > speed_x) sign_x = 1;
                else if (dx < speed_x) sign_x = -1;
                else sign_x = 0;
                obj->speed_x = (s16)(speed_x + sign_x);
                if (obj->speed_x < -32) obj->speed_x = -32;
                if (obj->speed_x > 32) obj->speed_x = 32;
            } else if (obj->flags.flip_x) {
                if (obj->speed_x < dx && obj->speed_x < 32)
                    ++obj->speed_x;
                else if (obj->speed_x > dx && obj->speed_x > -32)
                    --obj->speed_x;
            } else {
                if (obj->speed_x < dx && obj->speed_x < 32)
                    ++obj->speed_x;
                else if (obj->speed_x > dx && obj->speed_x > -32)
                    --obj->speed_x;
            }
        } else {
            obj->speed_x = (s16)((obj->speed_x << 1));
            obj->speed_y = (s16)((obj->speed_y << 1));
            if (obj->speed_x > 32) obj->speed_x = 32;
            if (obj->speed_x < -32) obj->speed_x = -32;
            if (obj->speed_y > 32) obj->speed_y = 32;
            if (obj->speed_y < -32) obj->speed_y = -32;
        }
        if (obj->speed_x == 0 && obj->speed_y == 0)
            obj->speed_x = obj->flags.flip_x ? -1 : 1;
        abs_speed_x = (s16)abs(obj->speed_x);
        abs_speed_y = (s16)abs(obj->speed_y);
        if (abs_speed_x * 3 < abs_speed_y * 4)
            obj->anim_frame = 0;
        else if (abs_speed_y * 3 < abs_speed_x * 4)
            obj->anim_frame = 9;
        else if (obj->speed_x < 0)
            obj->anim_frame = 10;
        else
            obj->anim_frame = 11;
        if (obj->speed_x > 0 && obj->flags.flip_x)
            obj->flags.flip_x = 0;
        else if (obj->speed_x < 0 && !obj->flags.flip_x)
            obj->flags.flip_x = 1;
    }
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
                    if (!(block_flags[calc_typ_travd(obj, 0)] >> BLOCK_FULLY_SOLID & 1))
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
    s16 spi_x;
    s16 spi_y;
    s16 spi_width;
    s16 spi_height;
    s16 i;
    s16 nb_objects;
    obj_t* cur_obj;

    i = 0;
    cur_obj = level.objects;
    nb_objects = level.nb_objects;
    while (i < nb_objects) {
        if (cur_obj->type == 0x42 && cur_obj->is_active == 0) {
            cur_obj->flags.flip_x = spi_obj->flags.flip_x;
            cur_obj->speed_y = 0;
            if (cur_obj->flags.flip_x)
                cur_obj->speed_x = -3;
            else
                cur_obj->speed_x = 3;
            GET_SPRITE_POS(spi_obj, 3, &spi_x, &spi_y, &spi_width, &spi_height);
            cur_obj->x = (spi_x + (spi_width >> 1) - cur_obj->offset_bx) << 16;
            cur_obj->y = (spi_y + (spi_height >> 1) - cur_obj->offset_by) << 16;
            cur_obj->init_x = (s16)(cur_obj->x >> 16);
            cur_obj->init_y = (s16)(cur_obj->y >> 16);
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
