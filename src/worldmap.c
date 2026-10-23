#include "type_island_abi.h"
/* worldmap.c (1:57860..1:5ba70). Names RAYVERSE/ANDROID; seeded by tools/seed.py, workers tu_worldmap + gfin_worldmap (census-guided finish pass, 20 -> 46/50). text_to_display items are [1..10] here (Rayverse [0..9] is a port difference). See config/units.json for statuses. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "inline_helpers.h"
#include <string.h>
#include <stdlib.h>
extern u8 JeuCracker;
extern char save_ray_copy[4];
extern obj_t clockobj;
extern save_state_t save2;
extern obj_t rms;
extern u8 byte_CFA3E;
extern u8 NewMs;
extern s16 ray_mode;
u8 ValidButPressed(void);
u8 StartButPressed(void);
s16 rightjoy(void);
s16 leftjoy(void);
s16 upjoy(void);
s16 downjoy(void);
u8 confirmation_msg(u8 message);
void SaveGameOnDisk(u8 slot);
void LoadGameOnDisk(u8 slot);
void LoadInfoGame(u8 slot);
void calc_obj_pos(obj_t *obj);
void MAIN_NO_MORE_CONTINUE_PRG(void);
void *temp_malloc(s32 size);
void temp_free(void *ptr);
void check_cd_for_save(void); /* INVENTED: DOS 1:5bc40 */
u32 deter_num_let(u8 c, const char *next_chars);
s16 calc_largmax_text(const char *text, s32 char_index, s16 space_width, s16 spacing, u8 font_size); /* DOS 1:5838d..1:583ab */
s16 myRand(s16 max);
extern sprite_t *alpha, *alpha2;
extern char TexteStageNameLevel[], TexteStageNameWorld[];
extern s32 VitesseYStageNameLevel[30];
extern s32 VitesseXStageNameWorld[30];
extern s32 VitesseXStageNameLevel[30];
extern s32 PositionYStageNameWorld[50];
extern s32 VitesseYStageNameWorld[30];
extern s32 PositionXStageNameLevel[50];
extern s32 PositionXStageNameWorld[50];
extern s32 PositionYStageNameLevel[50];
extern s32 TailleStageNameLevel;
extern s32 CouleurStageNameLevel;
extern s32 TailleStageNameWorld;
extern s32 CouleurStageNameWorld;
extern s32 LongueurStageNameWorld;
extern s32 LongueurStageNameLevel;
void Deter_Option_Caract(char *text, s16 key, s16 offset);
#define bool u8
#ifndef COUNT
#define COUNT(x) (sizeof(x) / sizeof((x)[0]))
#endif
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define world_1_jungle 1
#define world_2_music 2
#define world_3_mountain 3
#define world_4_image 4

#define GO_LEFT 0
#define world_5_cave 5
#define world_6_cake 6
extern s32 LargeurJumelle, HauteurJumelle;
extern u8 ModeDemo;
extern u8 NBRE_SAVE;
extern u8 PROC_EXIT;
extern rayevts_t RayEvts;
extern rayevts_t SauveRayEvts;
extern save_state_t save1;
extern s32 TempsDemo;
extern u8 You_Win;
extern s16 action;
extern obj_t mapobj[25];
extern display_item_t menu_to_display[9];
extern credit_t credits[135];
extern credit_t credits_init[];
extern options_jeu_t options_jeu;
extern u8 ALL_WORLD, new_txt_fee, MenuCredits, fade;
extern u8 horloge[25];
extern char save_ray[3][4];
extern s16 num_world, num_level, new_level;
extern u8 old_num_world;
extern u8 MENU_RETURN, dans_la_map_monde, gele, Nb_total_cages;
extern u8 ModeAutoJumelle, PositionStageNameCalcule, ChangeJumelleSizeOK;
extern s16 special_ray_mov_win_x_left, special_ray_mov_win_x_right;
extern s16 scroll_x, scroll_y;
extern status_bar_t status_bar;
extern poing_t poing;
extern u8 nb_continue, fin_continue, joy_done, Etape_History;
extern s16 departlevel, positiony;
extern world_info_save_t wi_save_zone[24];
extern u8 save_zone[2592];
extern loadinforay_t LoadInfoRay[3];
extern u8 bonus_perfect[24];
extern s16 rotationtxt, ktxtenx, ktxteny, coeffktxt, nbre_options;
extern u8 button_released;
extern s16 positionx2, positiony2, compteur, max_compteur;
extern s16 delai_repetition, repetition, basex, sortie_save;
extern s16 debut_sortie, ecartx, ecarty, ecart_options;
extern s16 loop_nb_trames, loop_timing;
extern u8 first_credit, last_credit, display_Vignet, menuEtape, dontdoit;
extern s16 nb_credits_lines;
extern s16 *anim_sequence;
extern void (*anim_func)(void);
extern s16 moust1_sequence[9], moust2_sequence[9], pst1_sequence[13], pst2_sequence[13], worry_sequence[12];
extern s16 car_sequence[33];
extern s16 affiche_bon_ecran;
extern obj_t bigray;
extern u8 chemin_percent;
extern s16 clignotement;
extern s16 compteur_clignote;
extern u8 dark_phase;
extern u8 dir_on_wldmap;
extern s16 fichier_a_copier;
extern s16 fichier_existant;
extern s16 fichier_selectionne;
extern finbosslevel_t finBosslevel;
extern u8 fin_dark;
extern u8 fin_du_jeu;
extern s16 fin_saisie_nom;
extern s16 h_scroll_speed;
extern char *language_txt[];
extern s16 new_world;
extern u8 nouvelle_partie;
extern s16 num_level_choice;
extern s16 num_world_choice;
extern s16 positionx;
extern obj_t ray;
extern u8 ray_on_poelle;
extern obj_t raylittle;
extern obj_t raytmp;
extern s16 realisation_effectuee;
extern s16 selection_effectuee;
extern world_info_t t_world_info[24];
extern s16 v_scroll_speed;
extern u8 world_index;
extern s16 xmap;
extern s16 xmapinit;
extern s16 xwldmapsave;
extern s16 ymap;
extern s16 ymapinit;
extern s16 ywldmapsave;
void INIT_TXT_BOX(display_item_t *box);
void CalcObjPosInWorldMap(obj_t *obj);
void INIT_RAY_BEGIN(void);
void display_text(const char *text, s16 x, s16 y, u8 font_size, s8 color);
void set_zoom_mode(s16 mode);
void set_main_etat(obj_t *obj, u8 main_etat);
void set_sub_etat(obj_t *obj, u8 sub_etat);
void set_main_and_sub_etat(obj_t *obj, u8 main_etat, u8 sub_etat);
u8 EOA(obj_t *obj);
u8 SelectButPressed(void);
void DO_LEVEL_ANIM(void);
void DISPLAY_PTS_TO_PLAN2(s16 origin_x, s16 origin_y, s16 dest_x, s16 dest_y, s16 percent);
void DO_ANIM(obj_t* obj);
void DO_FADE_OUT(void);
void DoScrollInWorldMap(s16 h_speed, s16 v_speed);
void INIT_FADE_IN(void);
void MoveRayInWorldMap(void);
void RecaleRayPosInJumelle(void);
void enter_casse_brique(void);
void horloges(s32 ticks);
void set_proj_center(s16 x, s16 y);
void TEST_DISPLAY_PTS_WAY(s16 world, s16 connected_world, s16 xpos, s16 ypos);
void INIT_LITTLE_RAY(void);
void RESTORE_RAY(void);
void RESPOND_TO_UP(void);
void RESPOND_TO_DOWN(void);
void RESPOND_TO_RIGHT(void);
void RESPOND_TO_LEFT(void);
void DO_CHEMIN(void);
void INIT_PASTILLES_SAUVE(void);
void PASTILLES_SAUVE_SAVED(s16 world);
void FIN_WORLD_CHOICE(void);
void DETER_WORLD_AND_LEVEL(void);
void DO_NOT_SAVE_CHOICE(void);
void INIT_SAVE_CONTINUE(void);
void INIT_LOADER_ANIM(void);
void SPECIAL_INIT(void);
void FIN_LEVEL_ANIM(void);

void TEST_DISPLAY_PTS_WAY(s16 world, s16 connected_world, s16 xpos, s16 ypos) {
    if (connected_world != world) {
        if ((t_world_info[connected_world].state & 2) == 0) {
            if (t_world_info[connected_world].state & 1) {
                DISPLAY_PTS_TO_PLAN2(xpos, ypos, t_world_info[connected_world].xpos, t_world_info[connected_world].ypos, 128);
            } else if (t_world_info[connected_world].state & 4) {
                DISPLAY_PTS_TO_PLAN2(xpos, ypos, t_world_info[connected_world].xpos, t_world_info[connected_world].ypos, chemin_percent);
            }
        }
    }
}

void DISPLAY_PTS_WAY(void) {
    s16 i, x, y;
    world_info_t *wi = t_world_info;
    for (i = 0; i < 24; ++i, ++wi) wi->state &= ~2;
    wi = t_world_info;
    for (i = 0; i < 24; ++i, ++wi) {
        x = wi->xpos;
        y = wi->ypos;
        if (wi->state & 1) {
            TEST_DISPLAY_PTS_WAY(i, wi->index_up, x, y);
            TEST_DISPLAY_PTS_WAY(i, wi->index_down, x, y);
            TEST_DISPLAY_PTS_WAY(i, wi->index_right, x, y);
            TEST_DISPLAY_PTS_WAY(i, wi->index_left, x, y);
            wi->state |= 2;
        }
    }
}

void DISPLAY_PLAT_WAY(void) {
    s16 i;
    world_info_t *wi = t_world_info;
    obj_t *obj = mapobj;
    for (i = 0; i < 24; ++i, ++wi, ++obj) {
        if ((wi->state & 1) || i == 17) {
            s32 x = obj->screen_x + obj->offset_bx + 8;
            if (x > -8 && x < LargeurJumelle + 14) {
                s32 y = obj->screen_y + obj->offset_by;
                if (y > -10 && y < HauteurJumelle + 12) display2(obj);
            }
            if (chemin_percent != 0 && i != 17) {
                if (t_world_info[wi->index_up].state & 4)
                    DISPLAY_PTS_TO_PLAN2(wi->xpos, wi->ypos, t_world_info[wi->index_up].xpos, t_world_info[wi->index_up].ypos, chemin_percent);
                if (t_world_info[wi->index_down].state & 4)
                    DISPLAY_PTS_TO_PLAN2(wi->xpos, wi->ypos, t_world_info[wi->index_down].xpos, t_world_info[wi->index_down].ypos, chemin_percent);
                if (t_world_info[wi->index_left].state & 4)
                    DISPLAY_PTS_TO_PLAN2(wi->xpos, wi->ypos, t_world_info[wi->index_left].xpos, t_world_info[wi->index_left].ypos, chemin_percent);
                if (t_world_info[wi->index_right].state & 4)
                    DISPLAY_PTS_TO_PLAN2(wi->xpos, wi->ypos, t_world_info[wi->index_right].xpos, t_world_info[wi->index_right].ypos, chemin_percent);
            }
        }
    }
}

void DO_MEDAILLONS(void) {
    s16 i;
    bool unlocked = 0;
    if (chemin_percent < 128 && (horloge[2] != 0 || ALL_WORLD)) {
        ++chemin_percent;
    }
    {
    obj_t *obj = mapobj;
    world_info_t *world_info = t_world_info;
    for (i = 0; i < 24; ++obj, ++i, ++world_info) {
        if ((world_info->state & 4) || (world_info->state & 2)) {
            CalcObjPosInWorldMap(obj);

            if (chemin_percent == 128 && (world_info->state & 4) &&
                ((t_world_info[world_info->index_left].state & 2) ||
                (t_world_info[world_info->index_right].state & 2) ||
                (t_world_info[world_info->index_up].state & 2) ||
                (t_world_info[world_info->index_down].state & 2))
            ) {
                // set unlocked
                world_info->state |= 1;
                world_info->state &= ~6;
                set_sub_etat(obj, 46);
                unlocked = 1;
            }

            if (i == 17 && (obj->sub_etat != 59 && obj->sub_etat != 46) && (world_info->state & 1)) {
                set_sub_etat(obj, 59);
            } else if ((i == 18 || i == 19 || i == 20 || i == 21 || i == 22 || i == 23) &&
                    (obj->sub_etat != 58 && obj->sub_etat != 46) &&
                    (world_info->state & 1)
            ) {
                set_sub_etat(obj, 58);
            } else {
                u8 nb_cages = world_info->nb_cages;
                if (nb_cages > 5 && obj->sub_etat == 51 && EOA(obj)) {
                    set_sub_etat(obj, 52);
                } else if (nb_cages > 4 && obj->sub_etat == 50 && EOA(obj)) {
                    set_sub_etat(obj, 51);
                } else if (nb_cages > 3 && obj->sub_etat == 49 && EOA(obj)) {
                    set_sub_etat(obj, 50);
                } else if (nb_cages > 2 && obj->sub_etat == 48 && EOA(obj)) {
                    set_sub_etat(obj, 49);
                } else if (nb_cages > 1 && obj->sub_etat == 47 && EOA(obj)) {
                    set_sub_etat(obj, 48);
                }
            }

            DO_ANIM(obj);
        }
    }
    }
    if (unlocked) {
        world_info_t *world_info;
        chemin_percent = 0;
        world_info = t_world_info;
        for (i = 0; i < 24; ++i, ++world_info) {
            if (!(world_info->state & 2) && (world_info->state & 1) == 1) {
                world_info->state |= 2;
            }
        }
    }
}

void INIT_LEVEL_STAGE_NAME(void) {
    memcpy(text_to_display[1].text, t_world_info[num_world_choice].text, 48);
    text_to_display[1].xpos = 450;
    text_to_display[1].ypos = 193;
    text_to_display[1].font_size = 2;
    text_to_display[1].is_fond = 0;
    text_to_display[1].field_D5 = 0;
    INIT_TXT_BOX(&text_to_display[1]);
    text_to_display[1].centered_y_pos -= 6;
    text_to_display[1].width += 10;
    text_to_display[1].height += 6;
    text_to_display[1].color = t_world_info[num_world_choice].color;
}

void INIT_WORLD_STAGE_NAME(void) {
    char *name = (char *)temp_malloc(75);
    s8 new_color;
    switch (t_world_info[num_world_choice].world) {
        case 7: {
            new_color = t_world_info[num_world_choice].color;
            switch(fichier_selectionne) {
                case 0: {
                    strcpy(text_to_display[3].text, language_txt[161]); // /password/
                } break;
                case 1: {
                    strcpy(name, "/");
                    strcat(name, save_ray[0]);
                    strcat(name, "/");
                    memcpy(text_to_display[3].text, name, 48);
                } break;
                case 2: {
                    strcpy(name, "/");
                    strcat(name, save_ray[1]);
                    strcat(name, "/");
                    memcpy(text_to_display[3].text, name, 48);
                } break;
                case 3: {
                    strcpy(name, "/");
                    strcat(name, save_ray[2]);
                    strcat(name, "/");
                    memcpy(text_to_display[3].text, name, 48);
                } break;
                default: break;
            }
        } break;
        case 1: {
            strcpy(text_to_display[3].text, language_txt[24]); // /the dream forest/
            new_color = 5;
        } break;
        case 2: {
            strcpy(text_to_display[3].text, language_txt[25]); // /band land/
            new_color = 3;
        } break;
        case 3: {
            strcpy(text_to_display[3].text, language_txt[26]); // /blue mountains/
            new_color = 0;
        } break;
        case 4: {
            strcpy(text_to_display[3].text, language_txt[27]); // /picture city/
            new_color = 2;
        } break;
        case 5: {
            strcpy(text_to_display[3].text, language_txt[28]); // /the caves of skops/
            new_color = 1;
        } break;
        case 6: {
            strcpy(text_to_display[3].text, language_txt[29]); // /candy chateau/
            new_color = 1;
        } break;
        default: break;
    }
    text_to_display[3].font_size = 1;
    text_to_display[3].xpos = 450;
    text_to_display[3].ypos = 30;
    text_to_display[3].color = new_color;
    text_to_display[3].is_fond = 0;
    text_to_display[3].field_D5 = 0;
    INIT_TXT_BOX(&text_to_display[3]);
    text_to_display[3].width += 10;
    text_to_display[3].height += 2;
    temp_free(name);
}

void INIT_STAGE_NAME(void) {
    INIT_TEXT_TO_DISPLAY();
    INIT_LEVEL_STAGE_NAME();
    INIT_WORLD_STAGE_NAME();
    text_to_display[2].xpos = 160;
    text_to_display[2].ypos = 250;
    text_to_display[2].field_D5 = 0;
    text_to_display[2].is_fond = 0;
    text_to_display[4].xpos = 160;
    text_to_display[4].ypos = -50;
    text_to_display[4].field_D5 = 0;
    text_to_display[4].is_fond = 0;
}

void CHANGE_STAGE_NAMES(void) {
    memcpy(&text_to_display[2], &text_to_display[1], sizeof(display_item_t));
    INIT_LEVEL_STAGE_NAME();
    if (t_world_info[num_world_choice].world != t_world_info[old_num_world].world) {
        memcpy(&text_to_display[4], &text_to_display[3], sizeof(display_item_t));
        INIT_WORLD_STAGE_NAME();
    }
}

/* Nonnegative-first absolute value: DOS 1:58751..1:58757, 1:58845..1:5884b. */
#define ABS_STAGE(x) ((x) >= 0 ? (x) : -(x))

void DO_STAGE_NAMES(void) {
  s32 rand_speed;
  s32 distance;
  s32 x;
  s32 letter;
  sprite_t *sprite_world;
  s32 spacing;
  u8 finished;
  s32 space_width;
  s32 start_level_x;
  s32 speed;
  display_item_t *item_level;
  s32 i;
  display_item_t *item_world;
  s32 start_world_x;
  sprite_t *sprite_level;
  if (!ModeAutoJumelle)
  {
    if (text_to_display[1].xpos > 160)
    {
      text_to_display[1].xpos -= 6;
      text_to_display[1].centered_x_pos -= 6;
    }
    if (text_to_display[3].xpos > 160)
    {
      text_to_display[3].xpos -= 6;
      text_to_display[3].centered_x_pos -= 6;
    }
    if (text_to_display[2].ypos < 293)
    {
      text_to_display[2].ypos += 2;
      text_to_display[2].centered_y_pos += 2;
    }
    if (text_to_display[4].ypos > (-70))
    {
      text_to_display[4].ypos -= 2;
      text_to_display[4].centered_y_pos -= 2;
    }
  }
  else
    if (!PositionStageNameCalcule)
  {
    item_level = &text_to_display[1];
    strcpy(TexteStageNameLevel, item_level->text);
    CouleurStageNameLevel = (u8) item_level->color;
    TailleStageNameLevel = item_level->font_size;
    spacing = 1;
    space_width = 8;
    LongueurStageNameLevel = strlen(item_level->text);
    start_level_x = item_level->xpos;
    for (i = 0; i < LongueurStageNameLevel; ++i)
    {
      letter = deter_num_let(item_level->text[i], &item_level->text[i + 1]);
      if (item_level->text[i] == '/')
      {
        x = start_level_x - (calc_largmax_text(item_level->text, i, space_width, spacing, item_level->font_size) >> 1);
        letter = 0;
      }
      else
        if (item_level->text[i] == ' ')
      {
        x += space_width;
        letter = 0;
      }
      if (letter)
      {
        sprite_level = &alpha[letter];
        PositionXStageNameLevel[i] = x << 4;
        PositionYStageNameLevel[i] = item_level->ypos << 4;
        VitesseXStageNameLevel[i] = 0;
        while (!VitesseXStageNameLevel[i])
          VitesseXStageNameLevel[i] = (((i - (LongueurStageNameLevel / 2)) * 32) / (LongueurStageNameLevel / 2)) + (myRand(32) - 16);

        {
          s32 random_y;
          random_y = myRand(32);
          VitesseYStageNameLevel[i] = (-16) - random_y;
        }
        x += (sprite_level->inner_width + (sprite_level->sprite_pos.raw & 15)) - spacing;
      }
    }

    item_world = &text_to_display[3];
    strcpy(TexteStageNameWorld, item_world->text);
    CouleurStageNameWorld = (u8) item_world->color;
    TailleStageNameWorld = item_world->font_size;
    space_width = 10;
    spacing = 3;
    LongueurStageNameWorld = strlen(item_world->text);
    start_world_x = item_world->xpos;
    for (i = 0; i < LongueurStageNameWorld; ++i)
    {
      letter = deter_num_let(item_world->text[i], &item_world->text[i + 1]);
      if (item_world->text[i] == '/')
      {
        x = start_world_x - (calc_largmax_text(item_world->text, i, space_width, spacing, item_world->font_size) >> 1);
        letter = 0;
      }
      else
        if (item_world->text[i] == ' ')
      {
        x += space_width;
        letter = 0;
      }
      if (letter)
      {
        letter += 41;
        sprite_world = &alpha2[letter];
        PositionXStageNameWorld[i] = x << 4;
        PositionYStageNameWorld[i] = item_world->ypos << 4;
        VitesseXStageNameWorld[i] = 0;
        while (!VitesseXStageNameWorld[i])
          VitesseXStageNameWorld[i] = (((i - (LongueurStageNameWorld / 2)) * 32) / (LongueurStageNameWorld / 2)) + (myRand(32) - 16);

        {
          s32 random_y;
          random_y = myRand(32);
          VitesseYStageNameWorld[i] = (-16) - random_y;
        }
        x += (sprite_world->inner_width + (sprite_world->sprite_pos.raw & 15)) - spacing;
      }
    }

    PositionStageNameCalcule = 1;
    strcpy(text_to_display[4].text, text_to_display[3].text);
    text_to_display[4].color = text_to_display[3].color;
    text_to_display[3].ypos = 30;
    text_to_display[3].xpos = 450;
    strcpy(text_to_display[2].text, text_to_display[1].text);
    text_to_display[1].xpos = 450;
    text_to_display[1].ypos = 193;
    text_to_display[2].color = text_to_display[1].color;
  }
  if (PositionStageNameCalcule == 1)
  {
    finished = 1;
    for (i = 0; i < LongueurStageNameWorld; ++i)
      if ((TexteStageNameWorld[i] != '/') && (TexteStageNameWorld[i] != ' '))
    {
      PositionYStageNameWorld[i] += VitesseYStageNameWorld[i];
      if (PositionYStageNameWorld[i] >= 3200)
      {
        PositionYStageNameWorld[i] = 3200;
        if (((VitesseYStageNameWorld[i] >= 0) ? (VitesseYStageNameWorld[i]) : (-VitesseYStageNameWorld[i])) < 8)
        {
          VitesseYStageNameWorld[i] = (-64) - myRand(32);
        }
        else
        {
          VitesseYStageNameWorld[i] = -VitesseYStageNameWorld[i];
        }
      }
      VitesseYStageNameWorld[i]++;
      PositionXStageNameWorld[i] += VitesseXStageNameWorld[i];
      if ((PositionStageNameCalcule == 1) && ((PositionXStageNameWorld[i] <= 0) || (PositionXStageNameWorld[i] >= 4880)))
      {
        VitesseXStageNameWorld[i] = -VitesseXStageNameWorld[i];
      }
      else
        if ((PositionXStageNameWorld[i] > (-320)) && (PositionXStageNameWorld[i] < 5120))
      {
        finished = 0;
      }
    }

    for (i = 0; i < LongueurStageNameLevel; ++i)
      if ((TexteStageNameLevel[i] != '/') && (TexteStageNameLevel[i] != ' '))
    {
      PositionYStageNameLevel[i] += VitesseYStageNameLevel[i];
      if (PositionYStageNameLevel[i] >= 3200)
      {
        PositionYStageNameLevel[i] = 3200;
        if (((VitesseYStageNameLevel[i] >= 0) ? (VitesseYStageNameLevel[i]) : (-VitesseYStageNameLevel[i])) < 8)
        {
          VitesseYStageNameLevel[i] = (-64) - myRand(32);
        }
        else
        {
          VitesseYStageNameLevel[i] = -VitesseYStageNameLevel[i];
        }
      }
      VitesseYStageNameLevel[i]++;
      PositionXStageNameLevel[i] += VitesseXStageNameLevel[i];
      if ((PositionStageNameCalcule == 1) && ((PositionXStageNameLevel[i] <= 0) || (PositionXStageNameLevel[i] >= 4960)))
      {
        VitesseXStageNameLevel[i] = -VitesseXStageNameLevel[i];
      }
      else
        if ((PositionXStageNameLevel[i] > (-320)) && (PositionXStageNameLevel[i] < 5120))
      {
        finished = 0;
      }
    }

    if (finished)
    {
      PositionStageNameCalcule = 0;
    }
  }
  else
    if (PositionStageNameCalcule == 2)
  {
    for (i = 0; i < LongueurStageNameWorld; ++i)
      if ((TexteStageNameWorld[i] != '/') && (TexteStageNameWorld[i] != ' '))
    {
      distance = PositionYStageNameWorld[i] - 1600;
      if (distance < 0)
      {
        distance = -distance;
      }
      {
        s32 perm_value_18;
        perm_value_18 = PositionXStageNameWorld[i] - 2560;
        speed = perm_value_18;
      }
      if (speed < 0)
      {
        speed = -speed;
      }
      if (distance > speed)
      {
        speed = PositionYStageNameWorld[i] - 1600;
      }
      else
      {
        speed = PositionXStageNameWorld[i] - 2560;
      }
      if (speed < 0)
      {
        speed = -speed;
      }
      distance = speed;
      {
        s32 perm_value_22;
        perm_value_22 = myRand(64) + 32;
        rand_speed = perm_value_22;
      }
      VitesseXStageNameWorld[i] = ((PositionXStageNameWorld[i] - 2560) * rand_speed) / distance;
      VitesseYStageNameWorld[i] = ((PositionYStageNameWorld[i] - 1600) * rand_speed) / distance;
    }

    for (i = 0; i < LongueurStageNameLevel; ++i)
      if ((TexteStageNameLevel[i] != '/') && (TexteStageNameLevel[i] != ' '))
    {
      distance = PositionYStageNameLevel[i] - 1600;
      if (distance < 0)
      {
        distance = -distance;
      }
      speed = PositionXStageNameLevel[i] - 2560;
      if (speed < 0)
      {
        speed = -speed;
      }
      if (distance > speed)
      {
        speed = PositionYStageNameLevel[i] - 1600;
      }
      else
      {
        speed = PositionXStageNameLevel[i] - 2560;
      }
      if (speed < 0)
      {
        speed = -speed;
      }
      distance = speed;
      rand_speed = myRand(64) + 32;
      VitesseXStageNameLevel[i] = ((PositionXStageNameLevel[i] - 2560) * rand_speed) / distance;
      VitesseYStageNameLevel[i] = ((PositionYStageNameLevel[i] - 1600) * rand_speed) / distance;
    }

    PositionStageNameCalcule = 3;
  }
  else
    if (PositionStageNameCalcule == 3)
  {
    finished = 1;
    for (i = 0; i < LongueurStageNameWorld; ++i)
      if ((TexteStageNameWorld[i] != '/') && (TexteStageNameWorld[i] != ' '))
    {
      PositionYStageNameWorld[i] += VitesseYStageNameWorld[i];
      PositionXStageNameWorld[i] += VitesseXStageNameWorld[i];
      if ((((PositionXStageNameWorld[i] > (-320)) && (PositionXStageNameWorld[i] < 5120)) && (PositionYStageNameWorld[i] > (-320))) && (PositionYStageNameWorld[i] < 3200))
      {
        finished = 0;
      }
    }

    for (i = 0; i < LongueurStageNameLevel; ++i)
      if ((TexteStageNameLevel[i] != '/') && (TexteStageNameLevel[i] != ' '))
    {
      PositionYStageNameLevel[i] += VitesseYStageNameLevel[i];
      PositionXStageNameLevel[i] += VitesseXStageNameLevel[i];
      if ((((PositionXStageNameLevel[i] > (-320)) && (PositionXStageNameLevel[i] < 5120)) && (PositionYStageNameLevel[i] > (-320))) && (PositionYStageNameLevel[i] < 3200))
      {
        finished = 0;
      }
    }

    if (finished)
    {
      PositionStageNameCalcule = 0;
    }
  }

}

void INIT_WORLD_INFO(void) {
    s16 i;
    for (i = 0; i < 24; ++i) {
        t_world_info[i].text = language_txt[i];
    }
    num_world = 0;
    num_level = 0;
    new_world = 1;
    new_level = 1;
    world_index = 0;
    xwldmapsave = 0;
    ywldmapsave = 0;
    dir_on_wldmap = 1;
    You_Win = 0;
    fin_du_jeu = 0;
    INIT_PASTILLES_SAUVE();
    MENU_RETURN = 0;
}

void INIT_LITTLE_RAY(void)
{
    memcpy(&raytmp, &ray, sizeof(obj_t));
    ray.scale = 0x100;
    ray.sprites = raylittle.sprites;
    ray.img_buffer = raylittle.img_buffer;
}

void RESTORE_RAY(void)
{
    ray.scale = 0;
    ray.sprites = raytmp.sprites;
    ray.img_buffer = raytmp.img_buffer;
    ray.nb_sprites = raytmp.nb_sprites;
}

void INIT_CHEMIN(void) {
    s16 i;
    u8 world_ind;
    obj_t *medaillon;
    PROC_EXIT = 0;
    dans_la_map_monde = 1;
    gele = 0;
    if (!ModeDemo) {
        new_world = 0;
        num_world_choice = world_index;
    }
    old_num_world = num_world_choice;
    INIT_LITTLE_RAY();
    set_main_and_sub_etat(&ray, 0, 0);
    ray.x = t_world_info[num_world_choice].xpos - ray.offset_bx + 4;
    ray.y = t_world_info[num_world_choice].ypos - ray.offset_by + 8;
    ray.speed_x = 0;
    ray.speed_y = 0;
    set_zoom_mode(0);
    chemin_percent = 0;
    world_ind = 0;
    Nb_total_cages = 0;
    {
    i = 0;
    medaillon = &mapobj[world_ind];
    while (i < 24) {
        medaillon->type = 54;
        medaillon->init_x = t_world_info[world_ind].xpos - 78;
        medaillon->init_y = t_world_info[world_ind].ypos - 64;
        medaillon->init_main_etat = 5;
        if (t_world_info[world_ind].state & 4) {
            medaillon->init_sub_etat = 46;
        } else {
            if (t_world_info[world_ind].nb_cages != 0) {
                Nb_total_cages += t_world_info[world_ind].nb_cages;
                if (t_world_info[world_ind].nb_cages == 6) {
                    medaillon->init_sub_etat = 52;
                } else {
                    medaillon->init_sub_etat = 47;
                }
            } else if (i == 17) {
                medaillon->sub_etat = 59;
                medaillon->init_sub_etat = 59;
            } else {
                medaillon->init_sub_etat = 39;
            }
        }
        medaillon->scale = 0;
        medaillon->offset_bx = 80;
        medaillon->offset_by = 64;
        obj_init(medaillon);
        CalcObjPosInWorldMap(medaillon);
        medaillon->anim_frame = i % medaillon->animations[medaillon->eta[medaillon->main_etat][medaillon->sub_etat].anim_index].frames_count;
        world_ind++;
        if (world_ind > 24) world_ind = 24;
        ++medaillon;
        ++i;
    }

    }
    // unlock Mr Dark's Dare
    if ((t_world_info[17].state & 1) == 0 && Nb_total_cages >= 102) {
        t_world_info[17].state |= 4;
    }

    xmapinit = xmap;
    ymapinit = ymap;
    xmap = xwldmapsave;
    ymap = ywldmapsave;
    ChangeJumelleVariable();
    scroll_x = -1;
    scroll_y = -1;
    special_ray_mov_win_x_left = 30;
    special_ray_mov_win_x_right = 30;
    ray.flags.flip_x = dir_on_wldmap & 1;

    // If it's a new game, create the new save file on disk
    if (nouvelle_partie) {
        if (NBRE_SAVE != 0) {
            SaveGameOnDisk(fichier_selectionne);
        }
        nouvelle_partie = 0;
    }

    INIT_PASTILLES_SAUVE();
    INIT_STAGE_NAME();

}

void RESPOND_TO_UP(void)
{
    num_world_choice = t_world_info[num_world_choice].index_up;
    ChangeJumelleSizeOK &= ~4;
    ModeAutoJumelle = 0;
    if (PositionStageNameCalcule == 1)
        PositionStageNameCalcule = 2;
}

void RESPOND_TO_DOWN(void)
{
    num_world_choice = t_world_info[num_world_choice].index_down;
    ChangeJumelleSizeOK &= ~4;
    ModeAutoJumelle = 0;
    if (PositionStageNameCalcule == 1)
        PositionStageNameCalcule = 2;
}

void RESPOND_TO_RIGHT(void)
{
    num_world_choice = t_world_info[num_world_choice].index_right;
    ChangeJumelleSizeOK &= ~4;
    ModeAutoJumelle = 0;
    if (PositionStageNameCalcule == 1)
        PositionStageNameCalcule = 2;
}

void RESPOND_TO_LEFT(void)
{
    num_world_choice = t_world_info[num_world_choice].index_left;
    ChangeJumelleSizeOK &= ~4;
    ModeAutoJumelle = 0;
    if (PositionStageNameCalcule == 1)
        PositionStageNameCalcule = 2;
}

/* DOS uses conditional negation, e.g. 1:59283..1:59293. */
#define ABS_PROMOTED(x) ((s32)(x) >= 0 ? (s32)(x) : -(s32)(x))

void DO_RAYMAN_IN_WLD_MAP(void) {
  s16 diff_x;
  s16 diff_y;
  s16 xspeed;
  s16 yspeed;
  s16 move_x;
  s16 move_y;
  if (old_num_world == num_world_choice)
  {
    if (ValidButPressed())
    {
      PositionStageNameCalcule = 0;
      ModeAutoJumelle = 0;
      if ((((((num_world_choice == 18) || (num_world_choice == 19)) || (num_world_choice == 20)) || (num_world_choice == 21)) || (num_world_choice == 22)) || (num_world_choice == 23))
      {
        if (((language_txt[150] == t_world_info[num_world_choice].text) && (NBRE_SAVE != 0)) && (strcmp(text_to_display[3].text, language_txt[161]) != 0))
        {
          if (confirmation_msg(3))
          {
            SaveGameOnDisk(fichier_selectionne);
            PASTILLES_SAUVE_SAVED(num_world_choice);
            CHANGE_STAGE_NAMES();
          }
        }
      }
      else
      {
        new_world = 1;
      }
      ChangeJumelleSizeOK &= ~4;
    }
    else
      if (rightjoy())
    {
      RESPOND_TO_RIGHT();
    }
    else
      if (leftjoy())
    {
      RESPOND_TO_LEFT();
    }
    else
      if (downjoy())
    {
      RESPOND_TO_DOWN();
    }
    else
      if (upjoy())
    {
      RESPOND_TO_UP();
    }
    if (num_world_choice != old_num_world)
    {
      if (t_world_info[num_world_choice].state & 1)
      {
        ray.timer = 0;
        set_main_and_sub_etat(&ray, 1, 0);
        CHANGE_STAGE_NAMES();
      }
      else
      {
        num_world_choice = old_num_world;
      }
    }
  }
  else
  {
{s16 perm_value_0;perm_value_0 = t_world_info[num_world_choice].xpos - t_world_info[old_num_world].xpos;diff_x = perm_value_0;}
    diff_y = t_world_info[num_world_choice].ypos - t_world_info[old_num_world].ypos;
    if (diff_x < 0)
    {
      ray.flags.flip_x = 0;
    }
    else
    {
      ray.flags.flip_x = 1;
    }
    if ((diff_x != 0) && (diff_y != 0))
    {
      if (!(((((s32) diff_y) >= 0) ? ((s32) diff_y) : (-((s32) diff_y))) > ((((s32) diff_x) >= 0) ? ((s32) diff_x) : (-((s32) diff_x)))))
      {
        xspeed = ray.timer;
        yspeed = (((s32) ((ray.timer * diff_y) / diff_x)) >= 0) ? ((s32) ((ray.timer * diff_y) / diff_x)) : (-((s32) ((ray.timer * diff_y) / diff_x)));
      }
      else
      {
        xspeed = (((s32) ((ray.timer * diff_x) / diff_y)) >= 0) ? ((s32) ((ray.timer * diff_x) / diff_y)) : (-((s32) ((ray.timer * diff_x) / diff_y)));
        yspeed = ray.timer;
      }
    }
    else
      if (diff_x == 0)
    {
      yspeed = ray.timer;
    }
    else
      if (diff_y == 0)
    {
      xspeed = ray.timer;
    }
    ++ray.timer;
    move_x = xspeed * sgn(diff_x);
    move_y = yspeed * sgn(diff_y);
    ray.speed_x = ((t_world_info[old_num_world].xpos + move_x) - ray.offset_bx) - ((s16) ray.x);
    ray.speed_y = (((t_world_info[old_num_world].ypos + move_y) - ray.offset_by) - ((s16) ray.y)) + 8;
    if ((((((s32) xspeed) >= 0) ? ((s32) xspeed) : (-((s32) xspeed))) >= ((((s32) diff_x) >= 0) ? ((s32) diff_x) : (-((s32) diff_x)))) && (((((s32) yspeed) >= 0) ? ((s32) yspeed) : (-((s32) yspeed))) >= ((((s32) diff_y) >= 0) ? ((s32) diff_y) : (-((s32) diff_y)))))
    {
      old_num_world = num_world_choice;
      if (ray.main_etat != 0)
      {
        set_main_and_sub_etat(&ray, 0, 0);
        ray.speed_x = 0;
        ray.speed_y = 0;
        ray.x = t_world_info[num_world_choice].xpos - ray.offset_bx;
        ray.y = (t_world_info[num_world_choice].ypos - ray.offset_by) + 8;
      }
    }
  }

}

void DO_CHEMIN(void) {
    horloges(1);
    RecaleRayPosInJumelle();
    DoScrollInWorldMap(h_scroll_speed, v_scroll_speed);
    MoveRayInWorldMap();
    /* PROVEN DOS 1:594aa: both facing branches set the projection center. */
    if (ray.flags.flip_x)
        set_proj_center(ray.screen_x + ray.offset_bx, ray.screen_y + ray.offset_by);
    else
        set_proj_center(ray.screen_x + ray.offset_bx, ray.screen_y + ray.offset_by);
    DO_ANIM(&ray);
    DO_MEDAILLONS();
}

void INIT_PASTILLES_SAUVE(void) {
    char* text = (NBRE_SAVE != 0) ? language_txt[150] /*/save game/*/: language_txt[152]; /*/no save available/*/
    t_world_info[18].text = text;
    t_world_info[19].text = text;
    t_world_info[20].text = text;
    t_world_info[21].text = text;
    t_world_info[22].text = text;
    t_world_info[23].text = text;
}

void PASTILLES_SAUVE_SAVED(s16 world) {
    INIT_PASTILLES_SAUVE();
    t_world_info[world].text = language_txt[151]; // /game saved/
}

void FIN_WORLD_CHOICE(void) {
    DO_FADE_OUT();
    xwldmapsave = xmap;
    ywldmapsave = ymap;
    xmap = xmapinit;
    ymap = ymapinit;
    dir_on_wldmap = ray.flags.flip_x;
    RESTORE_RAY();
    INIT_PASTILLES_SAUVE();
    PROC_EXIT = 0;
    if (ray.hit_points == 0xff) {
        ray.hit_points = 0;
    }
}

void DETER_WORLD_AND_LEVEL(void) {
    world_index = num_world_choice;
    if (!ModeDemo) {
        num_level_choice = t_world_info[num_world_choice].level;
        num_world_choice = t_world_info[num_world_choice].world;
        enter_casse_brique();
    }
    RayEvts.firefly = 0;
    if (num_world_choice == world_5_cave && num_level_choice == 3 && finBosslevel.helped_joe_1) {
        RayEvts.firefly = 1;
        ++num_level_choice;
    } else if (num_world_choice == world_5_cave && num_level_choice == 4 && finBosslevel.helped_joe_1 && ModeDemo) {
        RayEvts.firefly = 1;
    } else if (num_world_choice == world_6_cake && finBosslevel.mr_dark) {
        You_Win = 1;
        fin_du_jeu = 1;
        new_world = 1;
        fin_dark = 1;
    }
}

void INIT_NEW_GAME(void) {
    s16 i;
    new_level = 1;
    new_world = 1;
    status_bar.lives = 3;
    ray.hit_points = 2;
    ray.flags.hurtbyfist = 0;
    poing.sub_etat = 1;
    departlevel = 1;
    fin_du_jeu = 0;
    nb_continue = 9;
    memset(wi_save_zone, 0, sizeof(wi_save_zone));
    memset(save_zone, 0, sizeof(save_zone));
    INIT_RAY_BEGIN();
    t_world_info[0].state |= 1;
    t_world_info[0].state &= ~6;
    t_world_info[0].nb_cages = 0;
    for (i = 1; i < 24; ++i) {
        t_world_info[i].state &= ~7;
        t_world_info[i].nb_cages = 0;
    }
    LoadInfoRay[positiony - 1].lives = 3;
    LoadInfoRay[positiony - 1].tings = 0;
    LoadInfoRay[positiony - 1].cages = 0;
    LoadInfoRay[positiony - 1].continues = 9;
    for (i = 0; i < 23; ++i) bonus_perfect[i] = 0;
    init_finBossLevel();
    INIT_WORLD_INFO();
}

void DO_NOT_SAVE_CHOICE(void) {
    fichier_existant = 0;
    fichier_selectionne = 0;
    TempsDemo = 0;
    NBRE_SAVE = 0;
    nouvelle_partie = 1;
    INIT_NEW_GAME();
}

void INIT_SAVE_CHOICE(void) {
    s16 i;
    if (!JeuCracker) check_cd_for_save();
    rotationtxt = 0;
    ktxtenx = 4096;
    ktxteny = 4096;
    coeffktxt = 1;
    nbre_options = 3;
    positiony = 1;
    positionx2 = 3;
    positiony2 = NBRE_SAVE + 1;
    button_released = 1;
    compteur = 0;
    max_compteur = 100;
    nouvelle_partie = 0;
    delai_repetition = 12;
    repetition = 6;
    basex = 40;
    sortie_save = 0;
    if (NBRE_SAVE) for (i = 0; i < NBRE_SAVE; ++i) {
        LoadInfoGame(i + 1);
    }
}

void INIT_SAVE_CONTINUE(void) {
    selection_effectuee = 0;
    realisation_effectuee = 0;
    fichier_selectionne = 0;
    fichier_a_copier = 0;
    action = 0;
    fin_saisie_nom = 0;
    affiche_bon_ecran = 0;
    clignotement = 0;
    compteur_clignote = 0;
    positionx = 1;
}

void DO_COMMANDE_SAVE(void) {
    if (rightjoy() && !upjoy() && !downjoy() && !fichier_a_copier && (button_released || (compteur > delai_repetition && (compteur % repetition == 0)))) {
        if (positionx2 == 3) positionx2 = 1;
        else positionx2++;
        TempsDemo = 0;
        PlaySnd_old(68);
    }
    if (leftjoy() && !upjoy() && !downjoy() && !fichier_a_copier && (button_released || (compteur > delai_repetition && (compteur % repetition == 0)))) {
        if (positionx2 == 1) positionx2 = 3;
        else positionx2--;
        TempsDemo = 0;
        PlaySnd_old(68);
    }

    if (upjoy() && !leftjoy() && !rightjoy() && (button_released || (compteur > delai_repetition && (compteur % repetition == 0)))) {
        if (positiony <= 1) positiony = NBRE_SAVE;
        else positiony--;
        TempsDemo = 0;
        PlaySnd_old(68);
    }
    if (downjoy() && !leftjoy() && !rightjoy() && (button_released || (compteur > delai_repetition && (compteur % repetition == 0)))) {
        if (NBRE_SAVE == positiony) positiony = 1;
        else positiony++;
        TempsDemo = 0;
        PlaySnd_old(68);
    }

    if (!rightjoy() && !leftjoy() && !downjoy() && !upjoy()) {
        compteur = 0;
        button_released = 1;
    } else button_released = 0;

    if (SelectButPressed() && button_released) {
        MENU_RETURN = 1;
        PlaySnd_old(77);
    }
}

void SELECTION_SAVE_OPTION(void) {
    TestCompteur();
    if (positionx2 == 1 && ValidButPressed()) {
        if (button_released || (compteur > delai_repetition && compteur % repetition == 0)) {
            action = 1;
            PlaySnd_old(69);
        }
    }
    if (positionx2 == 2 && ValidButPressed()) {
        if (button_released || (compteur > delai_repetition && compteur % repetition == 0)) {
            action = 2;
            PlaySnd_old(69);
        }
    }
    if (positionx2 == 3 && ValidButPressed()) {
        if (button_released || (compteur > delai_repetition && compteur % repetition == 0)) {
            action = 3;
            PlaySnd_old(69);
            WaitNSynchro(20);
        }
    }
    TestButtonReleased();
}

void INIT_AFFICHE_ECRAN_SAVE(void) {
    display_item_t *to_display;
    nbre_options = 4;
    general_init_screen(20, 65, 5);
    basex = 50;
    to_display = menu_to_display;
    to_display->font_size = 1;
    to_display->is_fond = 1;
    to_display->field_D5 = 0;
    to_display->ypos = debut_sortie;
    to_display->color = 5;
    ecartx = 21;
    ecarty = ecart_options;
}

void SAISIE_NOM(void) {
    TestCompteur();
    if (!fichier_existant && action != 1) {
        strcpy(save_ray[positiony - 1], "aaa");
        fichier_existant = 1;
    }
    if (affiche_bon_ecran) {
        fin_saisie_nom = 1;
    }

    if (ValidButPressed() && button_released) {
        if (save_ray[positiony - 1][positionx - 1] == '~') {
            if (positionx != 1) {
                save_ray[positiony - 1][positionx - 1] = 'a';
                --positionx;
                PlaySnd_old(68);
            }
        } else if (positionx < 3) {
            ++positionx;
            PlaySnd_old(68);
        } else {
            PlaySnd_old(68);
            affiche_bon_ecran = 1;
        }
        rotationtxt = 0;
        ktxtenx = 4096;
        ktxteny = 4096;
        coeffktxt = 1;
    }

    if (upjoy() && !rightjoy() && !leftjoy() && (button_released || (compteur % 5 == 0))) {
        if (save_ray[positiony - 1][positionx - 1] == 'z') save_ray[positiony - 1][positionx - 1] = ' ';
        else if (save_ray[positiony - 1][positionx - 1] == ' ') save_ray[positiony - 1][positionx - 1] = '~';
        else if (save_ray[positiony - 1][positionx - 1] == '~') save_ray[positiony - 1][positionx - 1] = 'a';
        else save_ray[positiony - 1][positionx - 1]++;
        rotationtxt = 0;
        ktxtenx = 4096;
        ktxteny = 4096;
        coeffktxt = 1;
        PlaySnd_old(68);
    }

    if (downjoy() && !rightjoy() && !leftjoy() && (button_released || (compteur % 5 == 0))) {
        if (save_ray[positiony - 1][positionx - 1] == 'a') save_ray[positiony - 1][positionx - 1] = '~';
        else if (save_ray[positiony - 1][positionx - 1] == '~') save_ray[positiony - 1][positionx - 1] = ' ';
        else if (save_ray[positiony - 1][positionx - 1] == ' ') save_ray[positiony - 1][positionx - 1] = 'z';
        else save_ray[positiony - 1][positionx - 1]--;
        rotationtxt = 0;
        ktxtenx = 4096;
        ktxteny = 4096;
        coeffktxt = 1;
        PlaySnd_old(68);
    }

    if (!upjoy() && !downjoy() && !rightjoy() && !leftjoy() && !affiche_bon_ecran && !fin_saisie_nom) {
        clignotement = 1;
    }

    if (SelectButPressed()) {
        MENU_RETURN = 1;
        PlaySnd_old(77);
    }
    TestButtonReleased();
}

void REALISATION_ACTION(void) {
    u8 do_copy = 0;
    switch(action) {
        default: break;
        case 1: {
            // Copy
            if (fichier_a_copier) {
                if (strcmp(save_ray[fichier_a_copier - 1], "") != 0) {
                    if (strcmp(save_ray[positiony - 1], "") == 0 || confirmation_msg(2)) do_copy = 1;
                }
                if (do_copy) {
                    fichier_selectionne = positiony;
                    strcpy(save_ray_copy, save_ray[fichier_selectionne - 1]);
                    strcpy(save_ray[fichier_selectionne - 1], save_ray[fichier_a_copier - 1]);
                    LoadInfoRay[fichier_selectionne - 1].lives = LoadInfoRay[fichier_a_copier - 1].lives;
                    LoadInfoRay[fichier_selectionne - 1].tings = LoadInfoRay[fichier_a_copier - 1].tings;
                    LoadInfoRay[fichier_selectionne - 1].cages = LoadInfoRay[fichier_a_copier - 1].cages;
                    LoadInfoRay[fichier_selectionne - 1].continues = LoadInfoRay[fichier_a_copier - 1].continues;
                    LoadGameOnDisk(fichier_a_copier);
                    realisation_effectuee = 1;
                } else realisation_effectuee = 1;
            } else if (strcmp(save_ray[positiony - 1], "") != 0) fichier_a_copier = positiony;
        } break;
        case 2: {
            // Erase
            if (strcmp(save_ray[positiony-1], "") == 0) {
                break;
            }
            if (confirmation_msg(1)) {
                fichier_selectionne = positiony;
                REMOVE_FICHIER();
                save_ray[fichier_selectionne-1][0] = ""[0];
            }
            realisation_effectuee = 1;
        } break;
        case 3: {
            // Start
            fichier_selectionne = positiony;
            if (strcmp("", save_ray[fichier_selectionne-1]) == 0) {
                fichier_existant = 0;
                nouvelle_partie = 1;
                INIT_NEW_GAME();
            } else {
                fichier_existant = 1;
                LoadGameOnDisk(fichier_selectionne);
                sortie_save = 1;
                new_world = 1;
            }
        } break;
    }
}

void DISPLAY_SYMBOLE(s16 button, s16 x, s16 y, u8 font_size, u8 unused)
{
    switch (button) {
    case 0: display_text("a", x, y, font_size, 2); break;
    case 1: display_text("b", x, y, font_size, 1); break;
    case 2: display_text("d", x, y, font_size, 5); break;
    case 3: display_text("c", x, y, font_size, 1); break;
    }
}

void INIT_CONTINUE(void)
{
    obj_t *mapobj_1;

    loop_nb_trames = 0;
    loop_timing = 10;
    fin_continue = 0;
    compteur = 0;
    joy_done = 0;
    byte_CFA3E = 0;
    if (ray_mode == 4 || RayEvts.tiny) {
        ray = rms;
        NewMs = 1;
        ray_mode = 1;
        RayEvts.tiny = 0;
    }
    ray.y = 135 - ray.offset_by;
    ray.anim_frame = 0;
    if (nb_continue != 0)
    {
        set_main_and_sub_etat(&ray, 3, 24);
        ray.x = 225 - ray.offset_bx;
    }
    else
    {
        ray.x = 80 - ray.offset_bx;
        set_main_etat(&ray, 3);
        loop_timing = 0xff;
        if (horloge[20] > 10)
            set_sub_etat(&ray, 52);
        else
            set_sub_etat(&ray, 51);
    }

    ray.flags.flip_x = 0;
    ray.speed_x = 0;
    ray.speed_y = 0;
    set_zoom_mode(0);
    clockobj.x = 80;
    clockobj.y = 130;
    set_main_and_sub_etat(&clockobj, 0, 0);
    mapobj_1 = mapobj;
    mapobj_1->screen_x = 120;
    mapobj_1->screen_y = 110;
    mapobj->offset_bx = 80;
    mapobj->offset_by = 64;
    mapobj->flags.alive = 0;
    mapobj->timer = 0;
    set_main_and_sub_etat(mapobj, 5, 20);
    xmapinit = xmap;
    ymapinit = ymap;
    xmap = 0;
    ymap = 0;
    PROC_EXIT = 0;
}

void CHEAT_MODE_CONTINUE(void)
{
    compteur = 1;
    if (upjoy() && joy_done != 1) joy_done += 1;
    else if (downjoy() && joy_done != 3) joy_done += 2;
    else if (rightjoy() && joy_done != 7) joy_done += 4;
    else if (leftjoy() && joy_done != 15) joy_done += 8;
    else compteur = 0;
    switch (byte_CFA3E) {
    case 0:
        if (joy_done == 1 && nb_continue <= 3) byte_CFA3E++;
        else if (compteur) { byte_CFA3E = 0; joy_done = 0; }
        break;
    case 1:
        if (joy_done == 3) { byte_CFA3E++; joy_done = 3; }
        else if (compteur) { byte_CFA3E = 0; joy_done = 0; }
        break;
    case 2:
        if (joy_done == 7) { byte_CFA3E++; joy_done = 7; }
        else if (compteur) { byte_CFA3E = 0; joy_done = 0; }
        break;
    case 3:
        if (joy_done == 15) {
            mapobj->flags.alive = 1;
            joy_done = 0;
            mapobj->timer = 128;
            nb_continue = 10;
        } else if (compteur) { byte_CFA3E = 0; joy_done = 0; }
        break;
    }
}

void MAIN_CONTINUE_PRG(void)
{
    u8 flag_set;

    horloges(1);
    if (ValidButPressed() || StartButPressed() || SelectButPressed())
    {
        if (ray.sub_etat == 25 || ray.sub_etat == 26 || ray.sub_etat == 27)
        {
            if (mapobj->timer != 128)
                mapobj->flags.alive = 1;
            start_cd_continue();
            PlaySnd_old(216);
            if (ray.sub_etat != 27)
                set_main_and_sub_etat(&ray, 3, 31);
            else
            {
                set_main_and_sub_etat(&ray, 3, 29);
                ray.flags.flip_x = 1;
            }
            ray.anim_frame = 0;
            set_main_and_sub_etat(&clockobj, 0, 1);
        }
    }
    SET_X_SPEED(&ray);
    ray.speed_x = instantSpeed(ray.speed_x);
    switch (ray.sub_etat)
    {
    case 25:
        if (ray.x + ray.offset_bx < 70)
        {
            set_main_and_sub_etat(&ray, 3, 26);
            ray.anim_frame = 0;
        }
        break;
    case 26:
        if (ray.anim_frame > 6)
            ray.speed_x = 0;
        break;
    case 27:
        if (++loop_nb_trames == 60)
        {
            loop_nb_trames = 0;
            loop_timing--;
        }
        break;
    case 31:
        if (EOA(&ray))
            ray.flags.flip_x = 1;
        break;
    case 29:
        ray.flags.flip_x = 1;
        if ((ray.x + ray.offset_bx > 180) && EOA(&ray))
        {
            set_sub_etat(&ray, 28);
            ray.anim_frame = 0;
        }
        else
        {
            if (ray.anim_frame < 64)
                ray.speed_x = 0;
        }
        break;
    case 30:
        if (ray.x + ray.offset_bx > 175 && EOA(&ray))
        {
            set_sub_etat(&ray, 28);
            ray.anim_frame = 0;
        }
        break;
    case 28:
        if (ray.anim_frame > 40) {
            fin_continue = 1;
            save1.save_obj_id = -1;
        }
        if (ray.anim_frame > 16)
            ray.speed_x = 0;
        break;
    default:
        break;
    }

    if (horloge[2] != 0 || ray.sub_etat != 25)
    {
        ray.x = ray.x + ray.speed_x;
        calc_obj_pos(&ray);
        DO_ANIM(&ray);
    }

    calc_obj_pos(&clockobj);
    DO_ANIM(&clockobj);
    if (mapobj->flags.alive)
    {
        DO_ANIM(mapobj);
        if (EOA(mapobj))
        {
            if (mapobj->timer != 128)
                nb_continue--;
            mapobj->flags.alive = 0;
        }
    }
    if (loop_timing != 0xFF)
        CHEAT_MODE_CONTINUE();
    else
        MAIN_NO_MORE_CONTINUE_PRG();
}

void FIN_CONTINUE_PRG(void)
{
    if (loop_timing == 0xff)
    {
        fin_continue = 0;
        fin_du_jeu = 1;
        menuEtape = 0;
    }
    else if (loop_timing != -1)
    {
        status_bar.lives = 3;
        ray.hit_points = 2;
        status_bar.max_hitp = 2;
        fin_du_jeu = 0;
    }
    else
        menuEtape = 0;
    xmap = xmapinit;
    ymap = ymapinit;
}

void MAIN_NO_MORE_CONTINUE_PRG(void);

void INIT_NO_MORE_CONTINUE(void) {
    ray.screen_x = 80 - ray.offset_bx;
    ray.screen_y = 135 - ray.offset_by;
    set_main_etat(&ray, 3);
    if (horloge[20] > 10)
        set_sub_etat(&ray, 0x34);
    else
        set_sub_etat(&ray, 0x33);
    ray.flags.flip_x = 0;
    PROC_EXIT = 0;
    anim_func = MAIN_NO_MORE_CONTINUE_PRG;
}

void MAIN_NO_MORE_CONTINUE_PRG(void)
{
    PROC_EXIT = SelectButPressed() != 0;
    if (EOA(&ray))
        PROC_EXIT = 1;

    if (PROC_EXIT) {
        fin_continue = 1;
        save1.save_obj_id = -1;
    }
}

void INIT_VIGNET(void) {
    s32 i;
    loop_nb_trames = 0;
    loop_timing = 5;
    new_txt_fee = 0;
    INIT_TEXT_TO_DISPLAY();
    if (You_Win) {
        INIT_CREDITS(); //Todo
    } else {
      switch (num_world) {
        case 1: {
            strcpy(text_to_display[1].text, language_txt[30]); // /tarayzan gives rayman a magic seed/
            strcpy(text_to_display[2].text, language_txt[31]); // /to grow plants use the $ button/
            strcpy(text_to_display[3].text, language_txt[32]); // (empty string)
            strcpy(text_to_display[4].text, language_txt[33]); // (empty string)
            Deter_Option_Caract(text_to_display[2].text, options_jeu.action, 255);
            Deter_Option_Caract(text_to_display[3].text, options_jeu.action, 255);
            Deter_Option_Caract(text_to_display[4].text, options_jeu.action, 255);
        } break;
        case 3: {
            strcpy(text_to_display[1].text, language_txt[34]); // /the musician gives rayman/
            strcpy(text_to_display[2].text, language_txt[35]); // /a super helicopter power/
            strcpy(text_to_display[3].text, language_txt[36]); // /press the $ button to use it/
            strcpy(text_to_display[4].text, language_txt[37]); // /press $ again to go higher/
            Deter_Option_Caract(text_to_display[3].text, options_jeu.jump, 255);
            Deter_Option_Caract(text_to_display[4].text, options_jeu.jump, 255);
        } break;
        case 4: {
            start_cd_gros_rayman();
            strcpy(text_to_display[1].text, language_txt[38]); // /mr dark kidnaps betilla the fairy!/
        } break;
        case 5: {
            if (num_level == 3) {
                strcpy(text_to_display[1].text, language_txt[39]); // /joe offers rayman a firefly /
                strcpy(text_to_display[2].text, language_txt[40]); // /to light up the dark/
                strcpy(text_to_display[3].text, language_txt[41]); // (empty string)
                strcpy(text_to_display[4].text, language_txt[42]); // (empty string)
            }
            if (num_level == 11) {
                strcpy(text_to_display[1].text, language_txt[43]); // /rayman please help me! hurry!/
            }
        } break;
        default: break;
    }

      for (i = 0; i != 10; i += 2) {
        if ((&text_to_display[1])[i].text[0]) {
          (&text_to_display[1])[i].xpos = 160;
          (&text_to_display[1])[i].ypos = 173;
          (&text_to_display[1])[i].font_size = 2;
        }
        if ((&text_to_display[1])[i+1].text[0]) {
          (&text_to_display[1])[i+1].xpos = 160;
          (&text_to_display[1])[i+1].ypos = 188;
          (&text_to_display[1])[i+1].font_size = 2;
        }
      }
    }
}

void INIT_CREDITS(void)
{
    s16 i = -1;

    dans_la_map_monde = 0;
    if (!MenuCredits) SaveGameOnDisk(fichier_selectionne);
    first_credit = 0;
    last_credit = 0;
    nb_credits_lines = 0;
    PROC_EXIT = 0;
    display_Vignet = 0;
    menuEtape = 0;
    do
    {
        i++;
        memcpy(&credits[i], &credits_init[i], sizeof(credit_t));
        if (credits[i].y_pos != 0) credits[i].text = language_txt[credits[i].y_pos];
        credits[i].y_pos = 220;
    } while (credits[i].cmd != 0xff);
}

void DO_CREDITS(void)
{
    s32 i;
    s32 scroll_step = 1;
    s16 last_cmd;
    s16 unk_1;

    if (You_Win)
    {
        horloges(scroll_step);
        if (horloge[2] == 0)
        {
            for (i = first_credit; i <= last_credit; i++)
                credits[i].y_pos -= scroll_step;

            nb_credits_lines += scroll_step;
            last_cmd = credits[last_credit].cmd;
            if (last_cmd == 0)
            {
                switch (credits[last_credit].font)
                {
                case 0:
                    unk_1 = 36;
                    break;
                case 1:
                    unk_1 = 23;
                    break;
                case 2:
                    unk_1 = 15;
                    break;
                }
            }
            else if (last_cmd == 0xFF)
                unk_1 = 160;
            else if (last_cmd > 100)
                unk_1 = last_cmd - 85;
            else
                unk_1 = last_cmd * 15;

            if (unk_1 <= nb_credits_lines)
            {
                nb_credits_lines = 0;
                if (last_cmd == 0xFF) PROC_EXIT = 1;
                else {
                    last_credit = last_credit + 1;
                    if (last_credit % 15 == 0) PROC_EXIT = 1;
                }
            }
            if (credits[first_credit].y_pos < -4)
                first_credit++;
        }
    }
}

void INIT_ANIM_VICTOIRE(void) {
    PROC_EXIT = 0;
    set_main_and_sub_etat(&ray, 3, 0x27);
    ray.screen_x = 160 - ray.offset_bx;
    fin_dark = 0;
}

void DO_RAYMAN_WINS(void) {
    display_text("/you have done it !/", 160, ray.screen_y + ray.offset_hy - 60, 2, 0);
    display_text("/you have saved the world !/", 160, ray.screen_y + ray.offset_hy - 30, 2, 0);
    if (ray.anim_frame > 0x80)
        PROC_EXIT = 1;
}

void INIT_LOADER_ANIM(void)
{
    bigray.offset_bx = 200;
    bigray.offset_by = 240;
    bigray.screen_x = 120;
    bigray.screen_y = -72;
    bigray.flags.flip_x = 0;
    bigray.main_etat = 1;
    bigray.sub_etat = 0;
    bigray.cmd = 0;
}

void DO_LOADER_ANIM(void) {
    u8 anim_speed = bigray.eta[bigray.main_etat][bigray.sub_etat].anim_speed & 15;
    bigray.speed_x = 0;
    if (anim_speed != 0 && horloge[anim_speed] == 0) {
        SET_X_SPEED(&bigray);
    }
    bigray.screen_x += bigray.speed_x;
    if (PROC_EXIT == 1) {
        bigray.cmd = 5;
        set_main_and_sub_etat(&bigray, 0, 2);
        bigray.timer = 2;
        bigray.flags.flip_x = 0;
        PROC_EXIT = 0;
        bigray.screen_x = 160 - bigray.offset_bx - 16;
    }

    {
        switch(bigray.cmd) {
            default: break;
            case 0: {
                if (bigray.screen_x + bigray.offset_bx < -100) {
                    ++bigray.cmd;
                    set_main_and_sub_etat(&bigray, 1, 1);
                    bigray.screen_x = (-bigray.offset_bx) - 60;
                    bigray.flags.flip_x = 1;
                }
            } break;
            case 1: {
                if (bigray.screen_x + bigray.offset_bx > 400) {
                    ++bigray.cmd;
                    set_main_and_sub_etat(&bigray, 1, 2);
                    bigray.anim_frame = 0;
                    bigray.screen_x = -bigray.offset_bx + 350;
                    bigray.flags.flip_x = 0;
                }
            } break;
            case 2: {
                if (bigray.main_etat == 0 && bigray.sub_etat == 0) {
                    ++bigray.cmd;
                }
            } break;
            case 3: {
                if (EOA(&bigray)) {
                    ++bigray.cmd;
                    set_main_and_sub_etat(&bigray, 1, 3);
                    bigray.screen_x = -60 - bigray.offset_bx;
                    bigray.flags.flip_x = 1;
                }
            } break;
            case 4: {
                if (bigray.screen_x + bigray.offset_bx > 400) {
                    ++bigray.cmd;
                    set_main_and_sub_etat(&bigray, 0, 1);
                    bigray.screen_x = 160 - bigray.offset_bx - 16;
                    bigray.flags.flip_x = 0;
                }
            } break;
            case 5: {
                if (bigray.main_etat == 0 && bigray.sub_etat == 2) {
                    if (bigray.timer == 0) {
                        PROC_EXIT = 1;
                    } else {
                        --bigray.timer;
                    }
                }
            } break;
        }
    }
    DO_ANIM(&bigray);
}

void SPECIAL_INIT(void) {
    dark_phase = 0;
    if (ray_on_poelle == 1) {
        RayEvts = SauveRayEvts;
        ray_on_poelle = 0;
    }
}

void INIT_LEVEL_ANIM(void) {
    anim_sequence = NULL;
    switch (num_world) {
        case world_1_jungle: {
            if (num_level == 16 && (new_level == 1 || new_world == 1)) {
                anim_sequence = car_sequence;
            }
            if (num_level == 7 && (new_level == 1 || new_world == 1)) {
                anim_sequence = moust2_sequence;
                dontdoit = 0;
            } else if (num_level == 7 && !dontdoit) {
                anim_sequence = moust1_sequence;
                if (!fin_du_jeu) {
                    dontdoit = 1;
                }
            }
        } break;
        case world_2_music: {
            if (num_level == 16 && new_world == 1) {
                anim_sequence = pst1_sequence;
            }
        } break;
        case world_3_mountain: {
            if (num_level == 10 && new_level == 1) {
                anim_sequence = pst1_sequence;
            }
        } break;
        case world_4_image: {
            if (num_level == 11 && (new_level == 1 || new_world == 1)) {
                anim_sequence = pst2_sequence;
            }
        } break;
        case world_5_cave: {
            if (num_level == 11 && (new_level == 1 || new_world == 1)) {
                anim_sequence = worry_sequence;
            }
        } break;
        default: break;
    }
    PROC_EXIT = 0;
    if (!anim_sequence || fin_du_jeu) {
        PROC_EXIT = 1;
    } else {
        memcpy(&raytmp, &ray, sizeof(obj_t));
        ray.cmd = GO_LEFT;
        ray.screen_y = 168 - ray.offset_by;
        ray.speed_x = 0;
        anim_func = DO_LEVEL_ANIM;
    }
}

void DO_LEVEL_ANIM(void) {
    s16 prev_cmd, current_cmd;
    eta_t **eta_table = ray.eta;
    u8 horloge_index;
    s16 dx;
    do {
        prev_cmd = ray.cmd;
        switch(anim_sequence[ray.cmd]) {
            case 1: {
                ray.flags.flip_x = 1;
                ++ray.cmd;
            } break;
            case 0: {
                ray.flags.flip_x = 0;
                ++ray.cmd;
            } break;
            case 8: {
                set_main_etat(&ray, anim_sequence[++ray.cmd]);
                set_sub_etat(&ray, anim_sequence[++ray.cmd]);
                if (ray.flags.flip_x) ray.speed_x = eta_table[ray.main_etat][ray.sub_etat].speed_x_right;
                else ray.speed_x = eta_table[ray.main_etat][ray.sub_etat].speed_x_left;
                ++ray.cmd;
            } break;
            case 21: {
                ray.screen_x = anim_sequence[++ray.cmd];
                ++ray.cmd;
            } break;
            case 19: {
                if (EOA(&ray) && ray.anim_index == ray.eta[ray.main_etat][ray.sub_etat].anim_index) {
                    ++ray.cmd;
                }
            } break;
            case 17: {
                if (ray.anim_frame == anim_sequence[ray.cmd + 1]) {
                    ray.cmd += 2;
                }
            } break;
            case 18: {
                if ((ray.screen_x >= anim_sequence[ray.cmd + 1] && ray.flags.flip_x) ||
                    (ray.screen_x <= anim_sequence[ray.cmd + 1] && !ray.flags.flip_x)
                ) {
                    ray.cmd += 2;
                }
            } break;
            case 20: {
                ray.speed_x = anim_sequence[++ray.cmd];
                ++ray.cmd;
            } break;
            case 14: {
                set_main_etat(&ray, 0);
                set_sub_etat(&ray, 0);
            } break;
            default: break;
        }
        current_cmd = ray.cmd;
    } while (prev_cmd != current_cmd);

    horloge_index = ((ray.animations[ray.anim_index].layers_per_frame & 0xC000) >> 14) + 1;
    dx = instantSpeed(ray.speed_x);
    if (horloge[horloge_index] == 0) {
        ray.screen_x += dx;
    }
    if (ray.main_etat == 0 && ray.sub_etat == 0) {
        PROC_EXIT = 1;
    }
}

void FIN_LEVEL_ANIM(void) {
    PROC_EXIT = 0;
    memcpy(&ray, &raytmp, sizeof(obj_t));
    DO_FADE_OUT();
    INIT_FADE_IN();
}






