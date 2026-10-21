/* display: original object 1:240f0..1:25f49. Worker gfin_display; see NOTES.md for measured status.
 * Function/global names RAYVERSE/ANDROID. Diagnostic local names and font_t are INVENTED. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "lowlevel.h"
#include "inline_helpers.h"
#include <string.h>
#include <stdlib.h>


extern u8 PROC_EXIT;
extern u8 display_Vignet;
extern u8 fin_continue;
extern u8 let_shadow;
extern s16 loop_timing;
extern s16 new_world;
extern obj_t ray;
extern mem_t main_mem_world;
extern u8 PROC_EXIT;
extern s16 xmap;
extern s16 ymap;
extern s16 loop_timing;
extern u8 fin_continue;
extern void (*anim_func)(void);
extern void horloges(u8 ticks);
extern void test_EXIT(void);
extern s16 but0pressed(void);
extern s16 but1pressed(void);
extern s16 but2pressed(void);
extern s16 but3pressed(void);
void CLRSCR(void);
void DISPLAY_CONTINUE_SPR(void);
void DISPLAY_FOND3(void);
void DISPLAY_FOND_CONTINUE(void);
void DISPLAY_JUMELLES(void);
void DISPLAY_JUMELLES_FOND(s16 x1, s16 y1, s16 x2, s16 y2);
void DISPLAY_PLAT_WAY(void);
void DISPLAY_PTS_WAY(void);
void DO_CHEMIN(void);
void DO_CREDITS(void);
void DO_FADE(void);
void DO_FADE_OUT(void);
void MAIN_CONTINUE_PRG(void);
s32 PS1_InitPAD(void);
s32 PS1_IsInputtingCheatCode(s32 param_1);
void RESTORE_PALETTE(void);
s32 but0pressed__CROSS(void);
s32 but2pressed__SELECT(void);
void charge_fix(void);
void display(obj_t *obj);
void put_env_point_way(void);
void readinput(void);
s32 continue_fonction(void);
void DO_CONTINUE(void);
s16 select_level_prg(u32 a1);
void CLEAR_STAGE_NAMES(void);
s16 WORLD_CHOICE(u32 a1);
s16 display_credits_prg(u32 a1);
void END_LOADER_ANIM(void);
void DO_GROS_RAYMAN(void);
s32 loader_anim_prg(u32 a1);
void START_LEVEL_ANIM(void);
s16 saisie_nom_prg(u32 a1);
s16 selection_save_option_prg(u32 a1);
void DO_SAVE_CHOICE(void);
void AFFICHE_ECRAN_SAVE(void);
s16 SELECT_LEVEL(s16 original_level_choice);
void DISPLAY_STAGE_NAMES(void);
void DO_WORLD_MAP(void);
void DISPLAY_TXT_VIGNET(void);
s16 display_vignet_prg(u32 a1);
void DISPLAY_GAME_VIGNET(void);
s32 display_anim_victoire(void);
void DISPLAY_CREDITS(void);
void DISPLAY_CREDITS_MENU(void);
void DISPLAY_PROTOON_BACK(void);
void DO_ANIM_VICTOIRE(void);
void DO_VICTOIRE(void);
void InitDemoJeu(void);
void FinDemoJeu(void);

extern u8 button_released, ButtonReleasedSav1, ButtonReleasedSav2, ButtonReleasedSav3;
extern u8 ReInitPlasma, MENU_RETURN, ModeDemo, ButtonReleasedSav3;
extern s16 action, sortie_save, fin_saisie_nom, fichier_selectionne, fichier_existant;
extern s16 realisation_effectuee, level_select, inter_select;
extern u8 save_ray[3][4], save_ray_copy[4];
extern s32 TempsDemo;
extern u16 RunTimeDemo;
extern s16 compteur, compteur_clignote, max_compteur;
extern s16 loop_nb_trames;
extern u8 new_txt_fee, Vignet_To_Display;
extern u8 colour, language, VGA_FREQ;
extern u8 need_timer;
extern u16 word_CF7EE;
extern s32 JumellePosX, JumellePosY, RayonJumelle;
extern u8 *DrawBufferNormal, *EffetBufferNormal, *draw_buffer;
extern u8 PositionStageNameCalcule;
extern u8 NBRE_SAVE;
extern s16 basex, debut_titre, debut_sortie, debut_options, ecarty, ecartx;
extern s16 positionx2, positionx, positiony, clignotement;
extern s16 coeffktxt, ktxtenx, ktxteny, rotationtxt, delai_repetition;
extern char *language_txt[300];
extern display_item_t menu_to_display[10];
extern void SAISIE_NOM(void);
extern void DO_COMMANDE_SAVE(void);
extern void SELECTION_SAVE_OPTION(void);
extern void REALISATION_ACTION(void);
extern void INIT_SAVE_CONTINUE(void);
extern void INIT_SAVE_CHOICE(void);
extern void INIT_AFFICHE_ECRAN_SAVE(void);
extern void SaveGameOnDisk(s16 slot);
extern void InitPlasma(u8 enabled);
extern void SWAP_BUFFERS(void);
extern void DoCdRap(void);
extern void block_free(mem_t *mem);
extern s16 LoadPlan2InVignet(mem_t *mem, s16 index);
extern void DISPLAY_FOND_SELECT(void);
extern s16 rightjoy(void);
extern s16 leftjoy(void);
extern s16 PROJ_CENTER_X;
extern s16 xwldmapsave, ywldmapsave;
extern void INIT_TXT_BOX(display_item_t *box);
extern void display_box_text_plasma(display_item_t *box, u8 arg);
extern void display_deform_text(const char *text, s16 x, s16 y, u8 font,
                                s8 color, s16 rotation, s16 kx, s16 ky);
extern s16 cosinus(s32 angle);
extern void DISPLAY_SAVE_SPRITES(s16 x, s16 save_index);
extern void DISPLAY_SAVE_POING(void);
extern void DO_STAGE_NAMES(void);
extern void PC_do_cheats_in_world_map(void);
extern void DO_RAYMAN_IN_WLD_MAP(void);
extern void DisplayJumellesFondNormal(void);
extern void DisplayJumellesNormal(s32 x, s32 y, s32 radius, s32 radius_y,
                                  u8 *effect_buffer, u8 *draw_buffer);
extern void test_Keyb_on_wldmap(void);
extern void display_box_text(display_item_t *box);
extern void DISPLAY_BLACKBOX(s16 x, s16 y, s16 width, s16 height,
                             s16 font_size, u8 is_fond);
extern void INIT_CHEMIN(void);
extern void INIT_RAY(u8 level);
extern void DefaultJumelleVariable(void);
extern void DETER_WORLD_AND_LEVEL(void);
extern void FIN_WORLD_CHOICE(void);
extern u8 get_casse_brique_active(void);
extern void PlaySnd_old(s16 sound);
extern void WaitNSynchro(s16 count);
extern void mute_snd(void);
extern void SAVE_PLAN3(void);
extern void RESTORE_PLAN3(void);
extern void INIT_VIGNET(void);
extern void PlayTchatchVignette(s32 arg);
extern void FreeTchatchVignette(void);
extern void InitModeNormalWithFrequency(s32 frequency);
extern void InitModeXWithFrequency(s32 frequency);
extern rgb_palette_t rvb_Vig[8];
extern u8 CarteSonAutorisee, ModeVideoActuel;
extern s16 ProchainEclair, numero_palette_special;
extern s16 WidthNworld, HeightNworld, plan2_height;
extern s32 plan2_width;
extern u8 chemin_percent;
extern s32 map_time;
extern s16 scroll_start_x, scroll_end_x, scroll_start_y, scroll_end_y;
extern s16 num_world, num_level, num_world_choice, num_level_choice;
extern u8 world_index, menuEtape;
extern u8 JeuCracker, fin_de_rayman, ALL_WORLD;
extern u8 You_Win;
extern rayevts_t SauveRayEvtsDemo;
extern s16 NumDemo;
extern s16 DemoRecordSize[6], DemoRecordWorld[6], DemoRecordMap[6];
extern u8 DemoRecord[1500];
extern u8 GameModeVideo, Frequence;
extern mem_t demo_video_mem;
extern u8 save_zone[2592];
extern options_jeu_t options_jeu_save, options_jeu;
extern record_t record;
extern obj_t *star_ray_dev, *star_ray_der;
extern u8 MenuCredits;
extern u16 nb_fade;
extern void INIT_CREDITS(void);
extern u32 clock_ticks(void);
extern void SAVE_PLAN3(void);
extern void RESTORE_PLAN3(void);
extern void INIT_TEXT_TO_DISPLAY(void);
extern void LOAD_CREDITS_VIGNET(mem_t *mem1, mem_t *mem2);
extern void LOAD_VIGNET_GAME(mem_t *mem);
extern void INIT_ANIM_VICTOIRE(void);
extern void DO_RAYMAN_WINS(void);
extern void playVideo(mem_t *mem, const char *filename, s16 video_id);
extern void POINTEUR_BOUTONS_OPTIONS_BIS(void);
extern void INIT_HORLOGES(void);
extern void block_free(mem_t *mem);

extern credit_t credits[1722];
extern u8 first_credit;
extern u8 last_credit;
extern void display_text(const char *text, s16 x, s16 y, u8 font, u8 color);
extern void DoCdCredits(void);
extern u8 ValidButPressed(void);
extern u8 SelectButPressed(void);
extern u8 StartButPressed(void);
extern void DoCdRap(void);
extern void DO_LOADER_ANIM(void);
extern obj_t bigray;
extern mem_t main_mem_tmp;
extern mem_t main_mem_level;
extern rgb_palette_t rvb_plan3;
extern rgb_palette_t rvb_pres;
extern status_bar_t status_bar;
extern void (*DrawSpriteNormalEtX)(void);
extern void (*DrawSpriteFlipNormalEtX)(void);
extern void DrawSpriteNormal(void);
extern void DrawSpriteFlipNormal(void);
extern void DrawSpriteNormal256(void);
extern void DrawSpriteFlipNormal256(void);
extern s32 LOAD_BIG_RAYMAN(mem_t *mem);
extern void LOAD_SAVE_SCREEN(mem_t *mem);
extern void SAVE_PALETTE(rgb_palette_t *palette);
extern void start_cd_gros_rayman(void);
extern void INIT_FADE_IN(void);
extern void INIT_LOADER_ANIM(void);
extern void INIT_LEVEL_ANIM(void);
extern void FIN_LEVEL_ANIM(void);
extern void EFFACE_VIDEO(void);
extern void stop_cd(void);
extern void init_memory(mem_t *mem, u32 capacity);
extern void SetCompteurTrameAudio(void);
extern void sub_5bbe8(void);
extern void PC_clear_text_input_buffer(void);
extern void InitPcSoundCard(void);
extern void free_main_mem(mem_t *mem);
extern void LOAD_ALL_FIX(void);
extern void stop_all_snd(void);
extern void LOAD_CONTINUE_SCREEN(mem_t *mem);
extern void INIT_CONTINUE(void);
extern void FIN_CONTINUE_PRG(void);
extern void start_cd_gameover(void);
extern s16 new_level;
extern void align_mem_desc(mem_t *mem);
extern u8 fin_du_jeu;
extern u8 fin_dark;
extern rayevts_t RayEvts;
extern finbosslevel_t finBosslevel;
extern void SYNCHRO_LOOP(s32 (*scene)(u32));


#include "display_local.h"

s32 display_inter_anim(u32 a1)
{
    horloges(1);
    CLRSCR();
    readinput();
    display2(&ray);
    DO_ANIM(&ray);
    test_EXIT();
    if (but0pressed() || but1pressed() || but2pressed() || but3pressed())
        PROC_EXIT = 1;
    anim_func();
    return PROC_EXIT;
}

s32 continue_fonction(void)
{
    u8 done = 0;

    CLRSCR();
    DISPLAY_FOND_CONTINUE();
    DISPLAY_CONTINUE_SPR();
    MAIN_CONTINUE_PRG();
    readinput();

    if (loop_timing == -1 || fin_continue != 0)
        done = 1;
    return done;
}

void DO_CONTINUE(void)
{
    if (status_bar.lives == 0 && fin_du_jeu == 1 && !fin_dark) {
        align_mem_desc(&main_mem_level);
        LOAD_CONTINUE_SCREEN(&main_mem_level);
        SAVE_PALETTE(&rvb_plan3);
        EFFACE_VIDEO();
        start_cd_gameover();
        INIT_FADE_IN();
        INIT_CONTINUE();
        SYNCHRO_LOOP((s32 (*)(u32))continue_fonction);
        DO_FADE_OUT();
        FIN_CONTINUE_PRG();
        stop_cd();
        stop_all_snd();
        RESTORE_PALETTE();
        align_mem_desc(&main_mem_level);
        new_level = 1;
    }
}

s16 saisie_nom_prg(u32 a1)
{
    readinput();
    button_released = ButtonReleasedSav3;
    SAISIE_NOM();
    ButtonReleasedSav3 = button_released;
    if (ReInitPlasma) {
        ReInitPlasma = 0;
        endsynchro();
        synchro();
        InitPlasma(1);
        AFFICHE_ECRAN_SAVE();
        SWAP_BUFFERS();
    } else {
        AFFICHE_ECRAN_SAVE();
    }
    AFFICHE_ECRAN_SAVE();

    if (action == 3)
        sortie_save = 1;

    {
        u8 done;
        if (fin_saisie_nom != 0)
            done = 1;
        else if (MENU_RETURN != 0)
            done = 1;
        else
            done = 0;
        if (MENU_RETURN) {
            if (action == 1)
                strcpy(save_ray[fichier_selectionne - 1], save_ray_copy);
            else
                strcpy(save_ray[fichier_selectionne - 1], "");
        }
        return done;
    }
}

s16 selection_save_option_prg(u32 a1)
{
    s32 done;

    readinput();
    DoCdRap();
    button_released = ButtonReleasedSav1;
    DO_COMMANDE_SAVE();
    ButtonReleasedSav1 = button_released;
    if (ReInitPlasma) {
        ReInitPlasma = 0;
        endsynchro();
        synchro();
        InitPlasma(1);
        AFFICHE_ECRAN_SAVE();
        SWAP_BUFFERS();
    } else {
        AFFICHE_ECRAN_SAVE();
    }

    readinput();
    button_released = ButtonReleasedSav2;
    SELECTION_SAVE_OPTION();
    ButtonReleasedSav2 = button_released;
    REALISATION_ACTION();

    if ((action == 1 && fichier_selectionne != 0) ||
        (action == 3 && !fichier_existant && fichier_selectionne != 0)) {
        if (!fin_saisie_nom) {
            INIT_AFFICHE_ECRAN_SAVE();
            SYNCHRO_LOOP(saisie_nom_prg);
        }
        if (!MENU_RETURN)
            SaveGameOnDisk((u8)fichier_selectionne);
    }
    if (realisation_effectuee)
        INIT_SAVE_CONTINUE();
    ++TempsDemo;
    action = 0;
    if (TempsDemo >= RunTimeDemo && !sortie_save && !fin_du_jeu)
        InitDemoJeu();

    done = fin_du_jeu || sortie_save || ModeDemo || MENU_RETURN;
    return done;
}

void DO_SAVE_CHOICE(void)
{
    let_shadow = 1;
    LOAD_SAVE_SCREEN(&main_mem_level);
    SAVE_PALETTE(&rvb_plan3);
    InitPlasma(0);
    INIT_FADE_IN();
    INIT_SAVE_CHOICE();
    INIT_SAVE_CONTINUE();
    TempsDemo = 0;
    INIT_AFFICHE_ECRAN_SAVE();
    ButtonReleasedSav1 = 1;
    ButtonReleasedSav2 = 1;
    ButtonReleasedSav3 = 0;
    compteur = 0;
    max_compteur = 100;
    xwldmapsave = 0;
    ywldmapsave = 0;
    SYNCHRO_LOOP(selection_save_option_prg);
    DO_FADE_OUT();
    RESTORE_PALETTE();
    align_mem_desc(&main_mem_level);
}

void AFFICHE_ECRAN_SAVE(void)
{
    s32 blink_period = 30;
    s16 save_index;
    s16 start_xpos;
    s16 erase_xpos;
    s16 copy_xpos;
    s16 cur_x;
    char temp[4];

    if (compteur < max_compteur && !button_released) {
        ++compteur;
    } else {
        if (!button_released)
            compteur = delai_repetition + 1;
        else
            compteur = 0;
    }
    if ((s16)blink_period > compteur_clignote)
        ++compteur_clignote;
    else
        compteur_clignote = 0;

    DISPLAY_FOND3();
    display_text(language_txt[96], 160, debut_titre, 1, 0);
    if (language == 1) {
        copy_xpos = 53;
        erase_xpos = 145;
        start_xpos = 255;
    } else {
        copy_xpos = 53;
        erase_xpos = 159;
        start_xpos = 265;
    }

    if (positionx2 == 1) {
        strcpy(menu_to_display[0].text, language_txt[97]);
        menu_to_display[0].xpos = copy_xpos;
    } else {
        display_text(language_txt[97], copy_xpos, debut_sortie, 1, 5);
    }
    if (positionx2 == 2) {
        strcpy(menu_to_display[0].text, language_txt[98]);
        menu_to_display[0].xpos = erase_xpos;
    } else {
        display_text(language_txt[98], erase_xpos, debut_sortie, 1, 5);
    }
    if (positionx2 == 3) {
        strcpy(menu_to_display[0].text, language_txt[99]);
        menu_to_display[0].xpos = start_xpos;
    } else {
        display_text(language_txt[99], start_xpos, debut_sortie, 1, 5);
    }

    INIT_TXT_BOX(&menu_to_display[0]);
    display_box_text_plasma(&menu_to_display[0], 1);

    for (save_index = 0; NBRE_SAVE > save_index; ++save_index) {
        if (strcmp(save_ray[save_index], "")) {
            for (cur_x = 1; cur_x <= strlen(save_ray[save_index]); ++cur_x) {
                if (cur_x != positionx || positiony != save_index + 1 || !clignotement) {
                    strcpy(temp, save_ray[save_index]);
                    temp[cur_x] = '\0';
                    if (temp[cur_x - 1] == '~')
                        display_deform_text("~", basex + ecartx * (cur_x - 1) - 10,
                            debut_options + save_index * (ecarty + 23), 1, 2,
                            rotationtxt, ktxtenx, ktxteny);
                    else if (temp[cur_x - 1] == ' ')
                        /* DOS 1:2484b passes 2:30c0d, the NUL of a "z" literal (CONST "" "~" "z" "level "), not the shared "" */
                        display_text("z" + 1, basex + ecartx * (cur_x - 1),
                            debut_options + save_index * (ecarty + 23), 1, 2);
                    else
                        display_text(temp + cur_x - 1, basex + ecartx * (cur_x - 1),
                            debut_options + save_index * (ecarty + 23), 1, colour);
                } else {
                    s32 delta;
                    strcpy(temp, save_ray[save_index]);
                    temp[cur_x] = '\0';
                    ktxtenx = 4096;
                    if (coeffktxt >= 512) coeffktxt = 0;
                    delta = (cosinus(coeffktxt + 256) >> 7) + 5;
                    ktxteny = 8 * cosinus(coeffktxt);
                    coeffktxt += delta;
                    if (temp[cur_x - 1] == '~')
                        display_deform_text("~", basex + ecartx * (cur_x - 1) - 10,
                            debut_options + save_index * (ecarty + 23), 1, 2,
                            rotationtxt, ktxtenx, ktxteny);
                    else
                        display_deform_text(temp + cur_x - 1, basex + ecartx * (cur_x - 1) + 1,
                            debut_options + save_index * (ecarty + 23) + 1, 1, colour,
                            rotationtxt, ktxtenx, ktxteny);
                }
            }
            DISPLAY_SAVE_SPRITES(basex + 3 * (ecartx + 11) - 30, save_index);
        } else {
            let_shadow = 1;
            display_text(language_txt[155], basex,
                debut_options + save_index * (ecarty + 23), 1, 2);
        }
    }
    DISPLAY_SAVE_POING();
}

s16 select_level_prg(u32 a1)
{
    s32 num[6] = {0x1100, 0x1000, 11, 11, 11, 4};
    char level_text[12];

    readinput();
    DISPLAY_FOND_SELECT();
    display_text("level ", 100, 100, 0, colour);
    itoa(level_select, level_text, 10);
    display_text(level_text, 200, 100, 0, colour);

    if (rightjoy() && inter_select == 0)
        ++level_select;
    else if (leftjoy() && inter_select == 0)
        --level_select;

    if (level_select > num[num_world_choice - 1])
        level_select = num[num_world_choice - 1];
    if (level_select < 1)
        level_select = 1;
    ++inter_select;
    if (inter_select >= 4)
        inter_select = 0;

    return but0pressed() || but1pressed() || but2pressed() || but3pressed();
}

s16 SELECT_LEVEL(s16 original_level_choice)
{
    level_select = original_level_choice;
    inter_select = 0;
    LoadPlan2InVignet(&main_mem_level, 50);
    SAVE_PALETTE(&rvb_pres);
    INIT_FADE_IN();
    SYNCHRO_LOOP(select_level_prg);
    DO_FADE_OUT();
    RESTORE_PALETTE();
    return level_select;
}

void CLEAR_STAGE_NAMES(void)
{
    if (map_time == 1) {
        text_to_display[2].text[0] = '\0';
        text_to_display[4].text[0] = '\0';
        map_time = 2;
        return;
    }
    if (text_to_display[2].text[0] != '\0')
        DISPLAY_BLACKBOX(text_to_display[2].centered_x_pos,
                         text_to_display[2].centered_y_pos - 2,
                         text_to_display[2].width,
                         text_to_display[2].height + 2, 255, 0);
    if (text_to_display[1].text[0] != '\0')
        DISPLAY_BLACKBOX(text_to_display[1].centered_x_pos,
                         text_to_display[1].centered_y_pos - 2,
                         text_to_display[1].width,
                         text_to_display[1].height + 2, 255, 0);
    if (text_to_display[4].text[0] != '\0')
        DISPLAY_BLACKBOX(text_to_display[4].centered_x_pos,
                         text_to_display[4].centered_y_pos - 2,
                         text_to_display[4].width,
                         text_to_display[4].height + 2, 255, 0);
    if (text_to_display[3].text[0] != '\0')
        DISPLAY_BLACKBOX(text_to_display[3].centered_x_pos,
                         text_to_display[3].centered_y_pos - 2,
                         text_to_display[3].width,
                         text_to_display[3].height + 2, 255, 0);
}

void DISPLAY_STAGE_NAMES(void)
{
    s32 i, letter;
    s32 width;
    sprite_t *sprite;
    s16 draw_x, draw_y;
    DO_STAGE_NAMES();
    sprite_clipping(0, 320, 0, 200);
    if (PositionStageNameCalcule != 1) {
        if (text_to_display[2].text[0] != '\0')
            display_text(text_to_display[2].text, text_to_display[2].xpos,
                         text_to_display[2].ypos, text_to_display[2].font_size,
                         (u8)text_to_display[2].color);
        if (text_to_display[1].text[0] != '\0')
            display_text(text_to_display[1].text, text_to_display[1].xpos,
                         text_to_display[1].ypos, text_to_display[1].font_size,
                         (u8)text_to_display[1].color);
        if (text_to_display[4].text[0] != '\0')
            display_text(text_to_display[4].text, text_to_display[4].xpos,
                         text_to_display[4].ypos, text_to_display[4].font_size,
                         (u8)text_to_display[4].color);
        if (text_to_display[3].text[0] != '\0')
            display_text(text_to_display[3].text, text_to_display[3].xpos,
                         text_to_display[3].ypos, text_to_display[3].font_size,
                         (u8)text_to_display[3].color);
    }
    if (PositionStageNameCalcule) {
        for (i = 0; i < LongueurStageNameWorld; ++i) {
            if (TexteStageNameWorld[i] == '/' || TexteStageNameWorld[i] == ' ')
                letter = 0;
            else
                letter = deter_num_let((u8)TexteStageNameWorld[i], &TexteStageNameWorld[i + 1]);
            if (letter) {
                width = calc_let_Width((u8)TailleStageNameWorld, letter);
                sprite = &alpha2.sprites[letter + 41];
                draw_y = (s16)((PositionYStageNameWorld[i] >> 4) - sprite->outer_height);
                draw_x = (s16)(PositionXStageNameWorld[i] >> 4);
                DrawSpriteColorNormalEtX(draw_x,
                    (u8)CouleurStageNameWorld,
                    draw_y,
                    (u16)(width + (sprite->outer_height << 8)), draw_buffer,
                    alpha2.img_buffer + sprite->offset_in_atlas);
            }
        }
        for (i = 0; i < LongueurStageNameLevel; ++i) {
            if (TexteStageNameLevel[i] == '/' || TexteStageNameLevel[i] == ' ')
                letter = 0;
            else
                letter = deter_num_let((u8)TexteStageNameLevel[i], &TexteStageNameLevel[i + 1]);
            if (letter) {
                width = calc_let_Width((u8)TailleStageNameLevel, letter);
                sprite = &alpha.sprites[letter];
                draw_y = (s16)((PositionYStageNameLevel[i] >> 4) - sprite->outer_height);
                draw_x = (s16)(PositionXStageNameLevel[i] >> 4);
                DrawSpriteColorNormalEtX(draw_x,
                    (u8)CouleurStageNameLevel,
                    draw_y,
                    (u16)(width + (sprite->outer_height << 8)), draw_buffer,
                    alpha.img_buffer + sprite->offset_in_atlas);
            }
        }
    }
}

s16 WORLD_CHOICE(u32 a1)
{
    s32 next_clock_target;
    s16 result;

    PC_do_cheats_in_world_map();
    if (need_timer)
        next_clock_target = word_CF7EE + a1;

    let_shadow = 0;
    fill(DrawBufferNormal);
    draw_buffer = EffetBufferNormal;

    DO_RAYMAN_IN_WLD_MAP();
    DO_CHEMIN();
    DisplayJumellesFondNormal();
    display2(&ray);
    DISPLAY_PLAT_WAY();

    DisplayJumellesNormal(JumellePosX, JumellePosY, RayonJumelle,
                          3 * RayonJumelle / 4, EffetBufferNormal,
                          DrawBufferNormal);
    draw_buffer = DrawBufferNormal;

    DISPLAY_STAGE_NAMES();
    readinput();
    DoCdRap();
    test_Keyb_on_wldmap();
    if (PROC_EXIT)
        PlaySnd_old(77);

    result = new_world;
    if (new_world || PROC_EXIT || get_casse_brique_active())
        result = 1;

    if (need_timer) {
        if (!wait_until(next_clock_target)) {
            DO_RAYMAN_IN_WLD_MAP();
            DO_CHEMIN();
        }
    }
    return result;
}

void DO_WORLD_MAP(void)
{
    sub_5bbe8();
    PC_clear_text_input_buffer();
    SetCompteurTrameAudio();
    EFFACE_VIDEO();
    INIT_RAY((u8)new_level);
    readinput();
    map_time = 0;
    if (!ModeDemo) {
        LoadPlan2InVignet(&main_mem_level, 48);
        WidthNworld = plan2_width;
        HeightNworld = plan2_height;
        chemin_percent = 1;
        DISPLAY_PTS_WAY();
    }
    SAVE_PALETTE(&rvb_pres);
    INIT_FADE_IN();
    INIT_CHEMIN();
    if (xwldmapsave == 0 && ywldmapsave == 0) {
        xmap = (scroll_end_x + scroll_start_x) / 2;
        ymap = (scroll_end_y + scroll_start_y) / 2;
        DefaultJumelleVariable();
    }
    if (!ModeDemo) {
        InitPcSoundCard();
        SYNCHRO_LOOP(WORLD_CHOICE);
    }

    if (JeuCracker && num_world_choice > 3) {
        fin_de_rayman = 1;
        new_world = 1;
        new_level = 1;
        fin_du_jeu = 1;
        PROC_EXIT = 1;
    }
    if (PROC_EXIT) {
        fin_du_jeu = 1;
        fichier_selectionne = 0;
        world_index = num_world_choice;
        menuEtape = 4;
        num_world_choice = num_world;
        num_level_choice = num_level;
        num_world = 0;
        PlaySnd_old(77);
    } else {
        for (;;) {
            if (!but0pressed() && !but1pressed() && !but2pressed() && !but3pressed())
                break;
            if (ModeDemo)
                break;
            readinput();
        }
        DETER_WORLD_AND_LEVEL();
        if (!get_casse_brique_active() && ALL_WORLD && !ModeDemo)
            num_level_choice = SELECT_LEVEL(num_level_choice);
        if (!ModeDemo) {
            PlaySnd_old(69);
            WaitNSynchro(5);
        }
    }
    FIN_WORLD_CHOICE();
    stop_cd();
}

void DISPLAY_TXT_VIGNET(void)
{
    u32 i;
    for (i = new_txt_fee; (text_to_display + 1)[i].text[0]; ++i) {
        if (i > new_txt_fee + 1) break;
        display_box_text((text_to_display + 1) + i);
    }
}

s16 display_vignet_prg(u32 a1)
{
    s16 done;

    readinput();
    DISPLAY_FOND3();
    DISPLAY_TXT_VIGNET();
    ++loop_nb_trames;
    if (loop_nb_trames == 60) {
        loop_nb_trames = 0;
        --loop_timing;
    }

    done = ((ValidButPressed() || StartButPressed()) && loop_timing <= 3) ||
           loop_timing <= 0;
    if (done) {
        if ((text_to_display + 1)[new_txt_fee + 2].text[0] == '\0') {
            new_txt_fee = 10;
        } else {
            loop_timing = 5;
            new_txt_fee += 2;
            done = 0;
        }
    }
    return done && new_txt_fee == 10;
}

void DISPLAY_GAME_VIGNET(void)
{
    s32 xmin, xmax, ymin, ymax;
    s32 need_restore_mode_x = 0;

    mute_snd();
    ProchainEclair = 1;
    numero_palette_special = 0;
    DO_FADE_OUT();
    get_sprite_clipping(&xmin, &xmax, &ymin, &ymax);
    sprite_clipping(0, 320, 0, 200);
    display_Vignet = 0;
    SAVE_PALETTE(rvb_Vig);
    SAVE_PLAN3();
    if (ModeVideoActuel == 1) {
        need_restore_mode_x = 1;
        InitModeNormalWithFrequency(VGA_FREQ);
    }
    INIT_FADE_IN();
    INIT_VIGNET();
    if (CarteSonAutorisee && language == 0)
        PlayTchatchVignette(0);
    SYNCHRO_LOOP(display_vignet_prg);
    if (CarteSonAutorisee && language == 0)
        FreeTchatchVignette();
    DO_FADE_OUT();
    EFFACE_VIDEO();
    Vignet_To_Display = 0;
    PROC_EXIT = 0;
    if (need_restore_mode_x)
        InitModeXWithFrequency(VGA_FREQ);
    RESTORE_PALETTE();
    RESTORE_PLAN3();
    sprite_clipping(xmin, xmax, ymin, ymax);
    INIT_FADE_IN();
}

void DISPLAY_TXT_CREDITS(void)
{
    u32 i;

    for (i = first_credit; i <= last_credit; ++i) {
        display_text(credits[i].text, credits[i].x_pos,
                     credits[i].y_pos, credits[i].font,
                     credits[i].color);
    }
}

s16 display_credits_prg(u32 a1)
{
    s16 prev_exit;

    readinput();
    DoCdCredits();
    DO_CREDITS();
    DISPLAY_FOND3();
    DISPLAY_TXT_CREDITS();
    prev_exit = PROC_EXIT;
    PROC_EXIT = 0;

    if (ValidButPressed() || SelectButPressed())
    {
        PROC_EXIT = 1;
        display_Vignet = 7;
    }

    if (PROC_EXIT || prev_exit)
        return 1;
    return 0;
}

s32 display_anim_victoire(void)
{
    horloges(1);
    CLRSCR();
    readinput();
    display2(&ray);
    DO_ANIM(&ray);
    DO_RAYMAN_WINS();
    return PROC_EXIT;
}

void DISPLAY_CREDITS(void)
{
    stop_cd();
    INIT_CREDITS();
    while (display_Vignet <= 7) {
        SAVE_PALETTE(&rvb_Vig[display_Vignet]);
        SAVE_PLAN3();
        INIT_FADE_IN();
        SYNCHRO_LOOP((s32 (*)(u32))display_credits_prg);
        DO_FADE_OUT();
        PROC_EXIT = 0;
        ++display_Vignet;
        RESTORE_PALETTE();
        RESTORE_PLAN3();
    }
    stop_cd();
    INIT_FADE_IN();
}

void DISPLAY_CREDITS_MENU(void)
{
    u32 tick = 0;
    u8 sound_state = 0;
    u8 do_fade = 1;

    stop_cd();
    SAVE_PALETTE(&rvb_plan3);
    MenuCredits = 1;
    INIT_CREDITS();
    MenuCredits = 0;

    while (display_Vignet <= 7) {
        do {
        speaker_enable();
        endsynchro();
        synchro();
        if (sound_state != need_timer)
            tick = clock_ticks();
        if (do_fade) {
            DO_FADE();
            if ((u16)sound_state == nb_fade)
                do_fade = 0;
        }
        SWAP_BUFFERS();
        } while (!display_credits_prg(tick));
        ++display_Vignet;
        PROC_EXIT = 0;
    }
    DO_FADE_OUT();
    RESTORE_PALETTE();
    stop_cd();
    INIT_FADE_IN();
}

void DISPLAY_PROTOON_BACK(void)
{
    display_Vignet = 0;
    SAVE_PALETTE(&rvb_Vig[0]);
    SAVE_PLAN3();
    INIT_FADE_IN();
    loop_nb_trames = 0;
    loop_timing = 4;
    new_txt_fee = 0;
    INIT_TEXT_TO_DISPLAY();
    SYNCHRO_LOOP(display_vignet_prg);
    LOAD_CREDITS_VIGNET(&main_mem_world, &main_mem_level);
    DO_FADE_OUT();
    RESTORE_PALETTE();
    RESTORE_PLAN3();
}

void DO_ANIM_VICTOIRE(void)
{
    INIT_FADE_IN();
    INIT_ANIM_VICTOIRE();
    anim_func = DO_RAYMAN_WINS;
    SYNCHRO_LOOP(display_inter_anim);
    DO_FADE_OUT();
}

void DO_VICTOIRE(void)
{
    star_ray_der = 0;
    star_ray_dev = 0;
    if (You_Win) {
        if (num_level_choice > 1 && Frequence != 0x46)
            playVideo(&demo_video_mem, "conclu.dat", 21);
        num_level = 4;
        num_world = 6;
        align_mem_desc(&main_mem_level);
        block_free(&main_mem_world);
        LOAD_VIGNET_GAME(&main_mem_level);
        DISPLAY_PROTOON_BACK();
        DISPLAY_CREDITS();
    }
    PROC_EXIT = 0;
}

void InitDemoJeu(void)
{
    DO_FADE_OUT();
    if ((GameModeVideo == 1 && DemoRecordWorld[NumDemo] == 5 && DemoRecordMap[NumDemo] == 10) ||
        (GameModeVideo == 0 && SizeScreen && DemoRecordWorld[NumDemo] == 1 && DemoRecordMap[NumDemo] == 9)) {
        ++NumDemo;
        if (NumDemo == 6) NumDemo = 0;
    }
    ModeDemo = 1;
    SauveRayEvtsDemo = RayEvts;
    RayEvts.poing = 1;
    RayEvts.hang = 1;
    RayEvts.helico = 1;
    RayEvts.super_helico = 1;
    TempsDemo = 0;
    RayEvts.handstand_dash = 0;
    RayEvts.handstand = 0;
    if (DemoRecordWorld[NumDemo] == 1 && DemoRecordMap[NumDemo] == 9)
        RayEvts.magicseed = 1;
    else
        RayEvts.magicseed = 0;
    RayEvts.grap = 1;
    RayEvts.run = 1;
    RayEvts.tiny = 0;
    if (num_world == 5 && num_level == 4)
        RayEvts.firefly = 1;
    else
        RayEvts.firefly = 0;
    options_jeu_save = options_jeu;
    RayEvts.force_run = 0;
    RayEvts.reverse = 0;
    options_jeu.jump = 1;
    options_jeu.fist = 0;
    options_jeu.action = 2;
    options_jeu.field_14 = 2;
    POINTEUR_BOUTONS_OPTIONS_BIS();
    finBosslevel.helped_joe_1 = 1;
    finBosslevel.helped_musician = 1;
    new_world = 1;
    new_level = 1;
    num_world = 0;
    num_level = 0;
    num_world_choice = DemoRecordWorld[NumDemo];
    ray.hit_points = 2;
    status_bar.max_hitp = 2;
    status_bar.num_wiz = 0;
    num_level_choice = DemoRecordMap[NumDemo];
    record.length = DemoRecordSize[NumDemo];
    poing.sub_etat = 1;
    record.is_playing = 1;
    record.current_offset = 0;
    record.data = &DemoRecord[NumDemo * 250];
    status_bar.lives = 3;
    memset(save_zone, 0, sizeof(save_zone));
    t_world_info[num_world_choice].nb_cages = 0;
    INIT_HORLOGES();
}

void FinDemoJeu(void)
{
    DO_FADE_OUT();
    if (record.repeat_index == record.repeat_length)
        RunTimeDemo = 180;
    else
        RunTimeDemo = 1800;
    ModeDemo = 0;
    TempsDemo = 0;
    INIT_RAY(1);
    RayEvts = SauveRayEvtsDemo;
    record.is_playing = 0;
    record.current_offset = 0;
    record.length = 0;
    record.data = 0;
    new_world = 0;
    new_level = 0;
    num_world = 0;
    num_level = 0;
    num_world_choice = 0;
    num_level_choice = 0;
    finBosslevel.helped_joe_1 = 0;
    finBosslevel.helped_musician = 0;
    options_jeu = options_jeu_save;
    POINTEUR_BOUTONS_OPTIONS_BIS();
    ++NumDemo;
    if (NumDemo == 6)
        NumDemo = 0;
}

 s32 loader_anim_prg(u32 a1)
{
    readinput();
    if (ValidButPressed() || StartButPressed())
        PROC_EXIT = 1;
    horloges(1);
    DoCdRap();
    DISPLAY_FOND3();
    display2(&bigray);
    DO_LOADER_ANIM();
    if (PROC_EXIT && nb_fade == 0)
        return 1;
    return 0;
}

void START_LOADER_ANIM(void)
{
    LOAD_BIG_RAYMAN(&main_mem_tmp);
    LOAD_SAVE_SCREEN(&main_mem_tmp);
    SAVE_PALETTE(&rvb_plan3);
    start_cd_gros_rayman();
    INIT_FADE_IN();
    INIT_LOADER_ANIM();
    DrawSpriteNormalEtX = DrawSpriteNormal256;
    DrawSpriteFlipNormalEtX = DrawSpriteFlipNormal256;
    SYNCHRO_LOOP(loader_anim_prg);
    DrawSpriteNormalEtX = DrawSpriteNormal;
    DrawSpriteFlipNormalEtX = DrawSpriteFlipNormal;
}

void END_LOADER_ANIM(void) {
    DO_FADE_OUT();
    RESTORE_PALETTE();
}

void DO_GROS_RAYMAN(void)
{
    init_memory(&main_mem_tmp, 0x41a00);
    SetCompteurTrameAudio();
    START_LOADER_ANIM();
    free_main_mem(&main_mem_tmp);
    PROC_EXIT = 0;
    LOAD_ALL_FIX();
    END_LOADER_ANIM();
}

void START_LEVEL_ANIM(void)
{
    if (fin_du_jeu != 1) {
        u8 saved_proc_exit;

        INIT_LEVEL_ANIM();
        saved_proc_exit = PROC_EXIT;
        if (!PROC_EXIT) {
            EFFACE_VIDEO();
            stop_cd();
            INIT_FADE_IN();
            SYNCHRO_LOOP(display_inter_anim);
            FIN_LEVEL_ANIM();
            PROC_EXIT = saved_proc_exit;
        }
    }
}

void PouvoirsParMap(u8 world, u8 level)
{
    if (!RayEvts.poing) {
        RayEvts.poing = world == 1 && level <= 3 ? 0 : 1;
    }
    if (!RayEvts.hang)
        RayEvts.hang = world == 1 && level <= 8 ? 0 : 1;
    if (!RayEvts.grap)
        RayEvts.grap = world == 1 && level <= 17 ? 0 : 1;
    if (!RayEvts.helico)
        RayEvts.helico = world < 2 || (world == 2 && level <= 11) ? 0 : 1;
    if (!RayEvts.run)
        RayEvts.run = world < 3 || (world == 3 && level <= 11) ? 0 : 1;
    finBosslevel.helped_musician = world < 3 || (world == 3 && level <= 6) ? 0 : 1;
    finBosslevel.helped_joe_2 = world < 5 || (world == 5 && level <= 6) ? 0 : 1;
}
