/* Original pcmain object 1:06f80..1:08890, GOG DOS v1.21.
 * Names RAYVERSE/ANDROID except names explicitly marked INVENTED.
 * Partial matching reconstruction; see NOTES.md for remaining byte differences. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "palette.h"
#include "action_local.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <i86.h>
#include <io.h>
#include <conio.h>
#include "original_inline.h"

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
extern u8 VGA_FREQ;
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
u32 get_avail_memory(void);
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
void InitData(s16 x, s16 y, s16 origin_x, s16 origin_y);
void update_map(s32 x, s32 y, s32 width, s32 height);
void calc_off_fnd(void);
void sub_3C3BC(s16 x, s16 y);
void init_arg(s32 argc, char** argv);
void PrintDosInfo(void);
void PcMain(void);
void DO_MAIN_LOOP_PC_NORMAL(u8* key_state);
void DO_MAIN_LOOP_PC_X(u8* key_state);
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
#pragma aux sprite_clipping parm [eax] [ebx] [ecx] [edx] modify exact [eax ebx ecx edx]
s16 test_allowed(obj_t* obj, s16 center_x, s16 center_y);
void InitMemoryVariable(void);
void wait_milliseconds(u32 duration);
void AfficheEntete(void);
void main_Ray(int argc, char** argv);
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

/* External handwritten drawing/timer ABIs: docs/reference/lowlevel_abi.h,
 * proven by DOS callers 1:0723c, 072bf, 073e8, 074b4, 0764e. */
typedef s32 (pc_swap_func_t)(u8 *source, u8 *destination, s32 width, s32 height);
#pragma aux pc_swap_func_t parm [esi] [edi] [ecx] [eax] modify exact []
extern pc_swap_func_t *Swap_And_Test_Joystick;
void Swap_To_Screen(u8 *, u8 *, s32, s32);
#pragma aux Swap_To_Screen parm [esi] [edi] [ecx] [eax] modify exact []
void clear_borders_Normal(u8 *, s32, s32);
#pragma aux clear_borders_Normal parm [edi] [ecx] [edx] modify exact []
void flocon_clipping(s32, s32, s32, s32);
#pragma aux flocon_clipping parm [eax] [ebx] [ecx] [edx] modify exact []
s32 clock_ticks(void);
#pragma aux clock_ticks parm [] value [eax] modify exact [eax]
s32 wait_until(s32);
#pragma aux wait_until parm [ebx] value [ecx] modify exact [eax ecx]
void synchro(void);
#pragma aux synchro parm [] modify exact []
void endsynchro(void);
#pragma aux endsynchro parm [] modify exact []
void speaker_enable(void);
#pragma aux speaker_enable parm [] modify exact []
void free_luciole(void);
extern u8 is_joypad_mode;
extern s16 word_CF7E6;
s16 rightjoy(void), leftjoy(void), downjoy(void), upjoy(void);
s16 but0pressed(void), but1pressed(void), but2pressed(void), but3pressed(void);
/* DOS 1:687e8 saves ECX and consumes EAX/EDX/EBX: three arguments.
 * The caller hoists ECX=0 for Display_Back_Screen across this call. */
void Calcul_Deplacement_Bande(s16 x, u16 width, u16 height);
void Display_Back_Screen(u16, u16, u16, u16, u16, u16);
void Display_Sprite_On_Front(u16, u16, u16, u16, u16, u16);
void sub_3B580(void);
u8 TOUCHE(u8 key);
u8 get_casse_brique_ON(void);
u8 get_casse_brique_active(void);
void set_casse_brique_ON(u8 value);
void build_map(s16 x, s16 y);
void sub_3BE20(s16 x, s16 y);
void sub_3BEE0(s16 x, s16 y);
void LoadPlan0InVignet(s32 resource_id);
u8 *allocate_PLAN0(mem_t *mem_world, u16 width, u16 height);
extern s8 Plan0NumPcx[10];
extern u8 no_fnd;
extern u8 *PLAN0;
extern s32 cdTime;
/* Handwritten callees: docs/reference/lowlevel_abi.h, and DOS 1:07a54..07aba. */
void asm_04504(u8 *buffer);
#pragma aux asm_04504 parm [edi] modify exact []
void asm_0456e(u8 *buffer);
#pragma aux asm_0456e parm [edi] modify exact []
void sub_1268A(u8 *source, s32 row_stride, u32 plane_stride);
#pragma aux sub_1268A parm [esi] [ebx] [edx] modify exact [ebx ecx esi edi]
s32 DetectCpu(void);
#pragma aux DetectCpu parm [] value [eax] modify exact [eax ebx ecx]
void sub_5BB78(void);
void sub_5BB94(void);
void sub_5BAE0(void);
void verify_mscdex(void);
void check_saved_drive_letter(void);
u8 verify_cdrom_response(void);
void InitPcSoundCard(void);
void ToDoAtExit(void);
void DoEffectBloodOut(void);
s32 playVideo(const char *path, const char *filename, u8 video_id);
extern u8 input_mode;

void InitData(s16 x, s16 y, s16 origin_x, s16 origin_y) {
    s32 offset;
    s32 mask; /* INVENTED local: keep the computed plane mask separate from its store. */
    init_data_state.x = x & 0x0F;
    init_data_state.y = y & 0x0F;
    offset = (s32)draw_buffer - init_data_state.x / 4 -
                        ((init_data_state.y * 5) << 4);
    init_data_state.c4 = offset + 1;
    if (type_fndscrX)
        init_data_state.c8 = 0;
    else
        init_data_state.c8 = origin_x + 16 - init_data_state.x;
    init_data_state.cc = origin_y + 16 - init_data_state.y;
    init_data_state.d0 = (s32)PLAN0BIT + init_data_state.c8 / 4 +
        init_data_state.c8 % 4 * ((u32)plan0bit_length >> 2) +
        init_data_state.cc * plan0bit_nb_bytes;
    mask = 1 << (3 - (init_data_state.x & 3));
    planar_copy_offset = mask;
}

void DO_GROS_MOTEUR_NORMAL(u8 need_update_display_map) {
    if (CarteSonAutorisee && gele != 1)
        manage_snd();
    do_record(&record);
    if (!gele) {
        DO_MOTEUR();
        if (!gele)
            DO_MOTEUR2();
    } else
        DO_MOTEUR_GELE();
    get_luciole();
    if (!need_update_display_map)
        update_display_map(&BIG_MAP);
}

/* DOS disassembly reconstructs this loop; remaining call ABI work is recorded in NOTES.md. */
void DO_MAIN_LOOP_PC_NORMAL(u8* key_state) {
    u8 end_of_frame;
    u8 display_count;
    s32 ticks;
    s32 deadline;
    s32 offset;
    RaymanDansUneMapDuJeu = 1;
    DoFirstFlocons();
    fin_continue = 0;
    MapTimePause = 0;
    ProchainEclair = 1;
    numero_palette_special = 0;
    if (MusicCdActive)
        TestCdLoop();
    while (dead_time != 0 && !new_level && !new_world && ModeDemo != 2) {
        speaker_enable();
        if (use_sync)
            synchro();
        if (need_timer)
            ticks = clock_ticks();
        if (need_timer)
            deadline = ticks + (u16)word_CF7E6;
        fades();
        if (dead_time >= 32 && !gele)
            DoPaletteSpecialPC();
        DO_SWAP_PALETTE();
        if (!fixon && !fixontemp) {
            offset = (Bloc_lim_H1 << 8) + (Bloc_lim_H1 << 6) + Bloc_lim_W1;
            if (is_joypad_mode == 1 && Main_Control)
                Swap_And_Test_Joystick(DrawBufferNormal + offset, DisplayBufferNormal + offset,
                                      Bloc_lim_W2 - Bloc_lim_W1, Bloc_lim_H2 - Bloc_lim_H1);
            else
                Swap_To_Screen(DrawBufferNormal + offset, DisplayBufferNormal + offset,
                               Bloc_lim_W2 - Bloc_lim_W1, Bloc_lim_H2 - Bloc_lim_H1);
        } else {
            if (is_joypad_mode == 1 && Main_Control)
                Swap_And_Test_Joystick(DrawBufferNormal, DisplayBufferNormal, 320, Bloc_lim_H2);
            else
                Swap_To_Screen(DrawBufferNormal, DisplayBufferNormal, 320, Bloc_lim_H2);
        }
        if (need_timer)
            clock_ticks();
        if (MusicCdActive)
            TestCdLoop();
        if (is_joypad_mode == 0)
            readinput();
        display_count = display_mode;
        do {
            DO_GROS_MOTEUR_NORMAL(0);
            end_of_frame = !new_level && !new_world && !fin_du_jeu;
        } while (end_of_frame && --display_count != 255);
        if (need_timer)
            clock_ticks();
        CLEAR_FIXE_LUCIOLE();
        if (BackgroundOn && FondAutorise == 1) {
            Calcul_Deplacement_Bande(xmap, plan0_width, plan0_height);
            Display_Back_Screen(plan0_width, plan0_height,
                                Bloc_lim_W1, Bloc_lim_H1, Bloc_lim_W2, Bloc_lim_H2);
        } else {
            /* DOS 1:073af uses REP STOSD for 16008 dwords (W-COPY OPEN). */
            memset(draw_buffer, 0, 16008 * 4);
        }
        if (MapAvecPluieOuNeige) {
            flocon_clipping(Bloc_lim_H1, Bloc_lim_H2 - 6,
                            Bloc_lim_W1, Bloc_lim_W2 - 4);
            display_flocons_behind();
        }
        DRAW_MAP(DrawBufferNormal, &BIG_MAP);
        display_grp_stars();
        if (need_timer)
            clock_ticks();
        DISPLAY_ALL_OBJECTS();
        if (need_timer)
            clock_ticks();
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
        if (need_timer && use_sync && end_of_frame && !wait_until(deadline)) {
            DO_GROS_MOTEUR_NORMAL(1);
            if (ray.iframes_timer != -1)
                ray.iframes_timer++;
            free_luciole();
            if (use_sync)
                endsynchro();
        }
    }
    RaymanDansUneMapDuJeu = 0;
}

void DO_GROS_MOTEUR_X(void) {
    s16 x_rem;
    if (CarteSonAutorisee && gele != 1)
        manage_snd();
    do_record(&record);
    if (!gele) {
        DO_MOTEUR();
        if (!gele)
            DO_MOTEUR2();
    } else
        DO_MOTEUR_GELE();
    if (!gele) {
        update_map(xmap, ymap, xmap_old, ymap_old);
    }
    if (FondAutorise == 1) {
        calc_off_fnd();
        if (yp0 < 0)
            yp0 += plan0_height;
    } else
        yp0 = 0;
    if (BackgroundOn && FondAutorise == 1) {
        x_rem = (xmap / 3) % (u16)plan0_width;
        InitData(xmap, ymap, (s16)x_rem, yp0);
    } else {
        InitData(xmap, ymap, 0, 0);
    }
    sub_3C3BC(xmap, ymap);
}

/* DOS 1:077e0..07cb5: the VGA path has its own background reload policy. */
void DO_MAIN_LOOP_PC_X(u8* key_state) {
    u8 end_of_frame;
    u8 reload_background = 0;
    u8 display_count;
    s32 ticks;
    s32 deadline;
    RaymanDansUneMapDuJeu = 1;
    DoFirstFlocons();
    fin_continue = 0;
    MapTimePause = 0;
    ProchainEclair = 1;
    numero_palette_special = 0;
    if (MusicCdActive)
        TestCdLoop();
    while (dead_time != 0 && !new_level && !new_world && ModeDemo != 2) {
        speaker_enable();
        sub_3B580();
        if (need_timer)
            ticks = clock_ticks();
        if (need_timer)
            deadline = ticks + (u16)word_CF7E6;
        fades();
        if (dead_time >= 32 && !gele)
            DoPaletteSpecialPC();
        DO_SWAP_PALETTE();
        if (MusicCdActive)
            TestCdLoop();
        if (map_time == 1)
            build_map(xmap, ymap);
        readinput();
        display_count = display_mode;
        do {
            DO_GROS_MOTEUR_X();
            end_of_frame = !new_level && !new_world && !fin_du_jeu;
        } while (end_of_frame && --display_count != 255);
        if (need_timer)
            clock_ticks();
        if (BackgroundOn && FondAutorise == 1)
            sub_3BE20(xmap, ymap);
        else if (BackgroundOn && FondAutorise == 2)
            sub_3BE20(xmap, ymap);
        else {
            /* DOS 1:07995..079aa uses OUT DX,AX and REP STOSD, 4000 dwords.
             * Port I/O is intrinsic; the fill's exact plain-C lowering is OPEN. */
            outpw(0x3ce, 0x0a04);
            memset(draw_buffer, 0, 4000 * 4);
        }
        if (MapAvecPluieOuNeige)
            display_flocons_behind();
        sub_3BEE0(xmap, ymap);
        display_grp_stars();
        if (need_timer)
            clock_ticks();
        DISPLAY_ALL_OBJECTS();
        if (need_timer)
            clock_ticks();
        display_pix_gerbes();
        if (MapAvecPluieOuNeige)
            display_flocons_before();
        if (is_fee)
            DISPLAY_TEXT_FEE();
        else
            DISPLAY_FIXE(left_time);
        if (JeuCracker)
            DisplayCrackers();
        if (MessageProgram)
            DisplayProgrammerMessage();
        display_time(left_time);
        if (P486)
            asm_0456e(draw_buffer);
        else
            asm_04504(draw_buffer);
        if (Vignet_To_Display) {
            DISPLAY_GAME_VIGNET();
            if (BackgroundOn && FondAutorise == 2)
                reload_background = 1;
            else if (BackgroundOn && P486 == 1)
                sub_1268A(PLAN0BIT, plan0bit_nb_bytes, (u32)plan0bit_length >> 2);
        }
        if (TOUCHE(SC_ESCAPE) && nb_fade == 0 &&
            !get_casse_brique_ON() && !ModeDemo) {
            DO_OPTIONS_IN_GAME();
            if (BackgroundOn && FondAutorise == 2)
                reload_background = 1;
        }
        if (get_casse_brique_ON() && !get_casse_brique_active()) {
            set_casse_brique_ON(0);
            RESET_TOUCHE(SC_ESCAPE);
        }
        if (reload_background) {
            if (Plan0NumPcx[(s8)no_fnd] != -1) {
                stop_cd();
                PLAN0 = allocate_PLAN0(NULL, 0, 0);
                LoadPlan0InVignet(Plan0NumPcx[(s8)no_fnd]);
                sub_1268A(PLAN0BIT, plan0bit_nb_bytes, (u32)plan0bit_length >> 2);
                cdTime = 0;
            }
            reload_background = 0;
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
            } else
                in_pause = 0;
        }
        HANDLE_KEY(key_state);
        if (need_timer && use_sync && end_of_frame && !wait_until(deadline)) {
            DO_GROS_MOTEUR_X();
            if (ray.iframes_timer != -1)
                ++ray.iframes_timer;
            if (use_sync)
                endsynchro();
        }
    }
    RaymanDansUneMapDuJeu = 0;
}

void init_arg(s32 argc, char** argv) {
    s32 i;
    char* eq;

    strcpy(demo_video_mem, ".\\");
    if (argc > 1) {
        for (i = 1; i < argc; ++i) {
            if (strncmp(strupr(argv[i]), "SAVE", 4) == 0) {
                eq = strchr(argv[i], '=');
                if (eq == NULL)
                    FatalError("Missing = in rayman's argument (save=pathname).\n");
                eq = strchr(argv[i], '=');
                strcpy(demo_video_mem, eq + 1);
                strcat(demo_video_mem, "\\");
            }
            if (strcmp(strupr(argv[i]), "FREQ70") == 0)
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
    switch (HVerDos) {
    case 10:
        printf("OS/2 v1.x");
        break;
    case 20:
        if (LVerDos == 30)
            printf("OS/2 Warp v3.0");
        else
            printf("OS/2 v2.x");
        break;
    default:
        printf("Dos v%1d.%1d", HVerDos, LVerDos);
        break;
    }
    printf("\n");
}

void InitMemoryVariable(void) {
    u32 available_memory;
    s32 i;

    available_memory = get_avail_memory();
    printf("\nLeft memory available = %.3f Mo.\n\n",
           (double)available_memory / 1048576.0);

    switch (TheProcesseur) {
    case 3:
        printf("386 processor detected.\n");
        printf("Rayman requires a 486 or more.\n");
        break;
    case 4:
        printf("486 processor detected.\n");
        break;
    default:
        if (TheProcesseur == 5)
            printf("Pentium processor detected.\n");
        else if (TheProcesseur > 5)
            printf("Pentium %1d processor detected.\n", TheProcesseur);
        else
            printf("Unknown processor detected.\n");
        break;
    }
    switch (WindowsLance) {
    case 0:
        printf("No windows detected.\n");
        break;
    case 0x81:
        printf("Windows real mode detected.\n");
        break;
    case 0x82:
        printf("Windows standard mode detected.\n");
        break;
    case 1:
        printf("Windows/386 V 2.x detected.\n");
        break;
    case 0x83:
        if (NVersionWindows == 4)
            /* DOS 1:07ffd..0800f passes both version arguments even for this literal. */
            printf("Windows 95 detected.\n", NVersionWindows, HVersionWindows);
        else
            printf("Windows V %d.%d detected.\n", NVersionWindows, HVersionWindows);
        break;
    }

    PrintDosInfo();
    P486 = 0;
    if (available_memory < 0x200000) {
        FatalError("Not enough memory.");
    } else if (available_memory < 0x300000) {
        ModeNormalAutorise = 0;
        JumelleEffetAutorise = 1;
        CarteSonAutorisee = 1;
        SonLimite = 1;
        ToutSpriteAutorise = 0;
        P486 = 1;
        FondAutorise = 2;
        TailleMainMemTmp = 0x17400;
        TailleMainMemFix = 0x4D800;
        TailleMainMemWorld = 0x62C00;
        TailleMainMemLevel = 0x5E000;
        TailleMainMemSprite = 0x12C000;
    } else {
        ModeNormalAutorise = 1;
        JumelleEffetAutorise = 1;
        CarteSonAutorisee = 1;
        SonLimite = 0;
        ToutSpriteAutorise = 1;
        FondAutorise = 1;
        TailleMainMemTmp = 0x22000;
        TailleMainMemFix = 0x4D800;
        TailleMainMemWorld = 0xF4C00;
        TailleMainMemLevel = 0x87C00;
        TailleMainMemSprite = 0xDF400;
    }

    {
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
    }
    /* DOS 1:081ec..08225: key test belongs to the loop condition. */
    for (i = 0; i < 40 && !TOUCHE(1); i++)
        wait_milliseconds(100);
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
    const char* copyright;
    u8 *second_line;
    s32 i;
    s32 j;

    InitTextMode();
    video = (u8*)0xB8000;
    title = "RAYMAN (US) v1.21";
    copyright = "(C) 1994-1995 UBI STUDIOS, FRANCE";
    printf("\n\n");
    for (i = 0; i < 80; i++)
        video[i * 2 + 1] = 0x1E;
    for (i = 0; i < strlen(title); i++)
        video[i * 2] = title[i];
    second_line = video + 160;
    for (j = 0; j < strlen(copyright); j++) {
        (second_line - 2 * strlen(copyright))[j * 2] = copyright[j];
        (second_line - 2 * strlen(copyright))[j * 2 + 1] |= 0x80;
    }
}

/* DOS-only initialization; descriptive helper names below are INVENTED. */
u8 detect_dos_version(u8 *major, u8 *minor);
u8 detect_windows(u8 *major, u8 *minor);
void sub_5BAD8(void);
extern u8 HVerDos, LVerDos;
extern char saved_argv[255];
void main_Ray(int argc, char** argv) {
    unlink("c:\\raytmp.tmp");
    AfficheEntete();
    DosDetect = detect_dos_version(&HVerDos, &LVerDos);
    WindowsLance = detect_windows(&NVersionWindows, &HVersionWindows);
    OptionMusicCdActive = 1;
    MusicCdActive = 1;
    sub_5BAD8();
    write_processor_flags(read_processor_flags() & 0xfffefeffUL);
    write_debug_address0(0);
    write_debug_address1(0);
    write_debug_address2(0);
    write_debug_address3(0);
    write_debug_status(read_debug_status() & 0xffff1ff0UL);
    write_debug_control(read_debug_control() & 0xde00);
    init_arg(argc, argv);
    strcpy(saved_argv, argv[0]);
    PcMain();
}

void PcMain(void) {
    u8 level_init = 0;
    TheProcesseur = DetectCpu();
    sub_5BB78();
    sub_5BB94();
    /* OPEN: DOS 1:08442..08481 debug-register/timer block is not reconstructed.
     * It arms DR0/DR7 and waits for two BIOS tick changes, then disarms them.
     * Do not replace its memory reads or arithmetic with pragma code. */
    sub_5BAE0();
    MakeMyRand(1);
    printf("CD ROM's Initialization.\n");
    InitMusic();
    input_mode = 1;
    Init_Clavier();
    atexit(ToDoAtExit);
    OptionMusicCdActive = MusicCdActive;
    verify_mscdex();
    check_saved_drive_letter();
    if (verify_cdrom_response())
        MusicCdActive = OptionMusicCdActive;
    InitMemoryVariable();
    sub_5BAE0();
    sprite_clipping(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
    INIT_MOTEUR_BEGIN();
    FIRST_INIT();
    DO_UBI_LOGO();
    DO_GROS_RAYMAN();
    INIT_EXTENSIONS();
    LOAD_CONFIG();
    PC_init_cheats();
    InitPcSoundCard();
    NBRE_SAVE = 3;
    if (MusicCdActive)
        stop_cd();
    while (!fin_de_rayman) {
        DEPART_INIT_LOOP();
        INIT_WORLD_INFO();
        set_default_Bloc_clipping();
        DO_NEW_MENUS();
        if (!fin_de_rayman && !ModeDemo && Frequence != 'F')
            playVideo(demo_video_mem, "intro.dat", 20);
        while (!fin_du_jeu && status_bar.lives > -1 && new_world) {
            MakeMyRand(1);
            SPECIAL_INIT();
            default_sprite_clipping();
            DO_WORLD_MAP();
            sprite_clipping(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
            DEPART_WORLD();
            if (CarteSonAutorisee && !SonLimite)
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
                    if (GameModeVideo == MODE_NORMAL) {
                        InitClipping();
                        InitModeNormalWithFrequency(VGA_FREQ);
                        NewFrequency(Frequence);
                        INIT_GAME_MODE_NORMAL();
                        During_The_Menu = 0;
                        DO_MAIN_LOOP_PC_NORMAL(&level_init);
                        During_The_Menu = 1;
                        FIN_GAME_MODE_NORMAL();
                    } else {
                        default_sprite_clipping();
                        InitModeXWithFrequency(VGA_FREQ);
                        NewFrequency(Frequence);
                        build_map(xmap, ymap);
                        if (P486 == 1)
                            sub_1268A(PLAN0BIT, plan0bit_nb_bytes, (u32)plan0bit_length >> 2);
                        During_The_Menu = 0;
                        DO_MAIN_LOOP_PC_X(&level_init);
                        During_The_Menu = 1;
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
    DoEffectBloodOut();
    if (CarteSonAutorisee) {
        stop_all_snd();
        DigiMusicDone();
    }
    END_GAME();
}
