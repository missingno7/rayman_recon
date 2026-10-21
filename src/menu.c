/* 1:39290..1:40b60: menu.c + misc_source.c (options/general menus, key config, fades, message boxes). Names RAYVERSE/ANDROID; seeded by tools/seed.py, worker tu_menu. 38 functions not yet reconstructed. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "menu_local.h"
#include <string.h>
#include <stdlib.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define SC_ESCAPE 1
#define world_1_jungle 1
#define world_2_music 2
#define world_4_image 4
#define world_5_cave 5
typedef s16 (*synchro_loop_palette_func_t)(void);
#define MODE_NORMAL 0
#define MODE_X 1
#define SC_ALT 56
#define SC_BACKSPACE 14
#define SC_CAPSLOCK 58
#define SC_CONTROL 29
#define SC_DELETE 83
#define SC_X 45
#define SC_DOWN 80
#define SC_END 79
#define SC_ENTER 28
#define SC_HOME 71
#define SC_INSERT 82
#define SC_LEFT 75
#define SC_PAGEDOWN 81
#define SC_PAGEUP 73
#define SC_RIGHT 77
#define SC_SPACE 57
#define SC_TAB 15
#define SC_UP 72
#define world_6_cake 6
extern u8 *tab_key[7];
extern u8 use_sync;
extern s16 display_mode;
void set_vga_frequency(u8 frequency);
void InitPcSoundCard(void);
extern const char **key_descriptions;
extern const char *key_descriptions_azerty[128];
extern const char *key_descriptions_qwerty[128];
extern s32 Bloc_lim_H1;
extern s32 Bloc_lim_H2;
extern s32 Bloc_lim_W1;
extern s32 Bloc_lim_W2;
extern u8 *DrawBufferNormal;
extern u8 *EffetBufferNormal;
extern void (*pAFFICHE_SCREEN)(void);
extern u8 GameModeVideo;
extern u16 SizeScreen;
extern s16 TabH1[4];
extern s16 TabH2[4];
extern s16 TabW1[4];
extern s16 TabW2[4];
extern u8 button_released;
extern s16 compteur;
extern s16 debut_options;
extern s16 debut_sortie;
extern s16 debut_titre;
extern s16 delai_repetition;
extern u8 *draw_buffer;
extern s16 ecart_options;
extern s16 hFondu;
extern s16 EtapeFondu;
extern u8 is_fee;
extern u8 byte_49290[4];
extern u8 current_pal_id;
extern char *language_txt[300];
extern s16 max_compteur;
extern u8 msg_box_being_displayed;
extern s16 nbre_options;
extern s16 num_level;
extern s16 num_world;
extern s16 sortie_options;
extern s16 wFondu;
extern s16 xFondu;
extern s16 yFondu;
void FireBox(s16 x, s16 y, s16 width, s16 height);
void N_CLRSCR(u8* buffer);
void PlasmaBox(s16 x, s16 y, s16 width, s16 height, u8 a5);
void SYNCHRO_LOOP(scene_func_t scene_func);
void default_sprite_clipping(void);
void display_box_text(display_item_t* box);
void display_text(const char* text, s16 x, s16 y, u8 font_size, u8 color);
u8 get_casse_brique_active(void);
void set_xywhmap(s16 xmin, s16 xmax, s16 ymin, s16 ymax);
void sprite_clipping(s32 xmin, s32 xmax, s32 ymin, s32 ymax);
void InitClipping(void);
void NewFrequency(u8 frequency);
s16 FonduPixel_prg(u32 arg);
void set_special_key_descriptions(void);
void general_init_screen(s16 a1, s16 a2, s16 a3);
void TestCompteur(void);
void display_box_text_plasma(display_item_t* box, u8 a2);
void display_box_text_fire(display_item_t* a1);
void FonduPixel(s16 x, s16 y, s16 w, s16 h);

/* DOS-addressed data declarations from Rayverse data.c; Android sizes checked where exported. */



/* DOS-addressed globals referenced by non-open candidate bodies. */
extern u8 CarteSonAutorisee;
extern s32 DeviceID;
extern s32 Dma;
extern u8 ExitMenu;
extern u8 First_Hist;
extern u8 FondAutorise;
extern u8 Frequence;
extern u8 InOut;
extern s32 Irq;
extern u8 MENU_RETURN;
extern u8 MENU_SUITE;
extern u8 Main_Control;
extern rgb_palette_t MenuPalette;
extern u8 ModeDemo;
extern u8 MusicCdActive;
extern u8 OptionGame;
extern u8 OptionMusicCdActive;
extern u8 P486;
extern s32 Param;
extern s32 Port;
extern rayevts_t RayEvts;
extern u8 ReInitPlasma;
extern s16 RunTimeDemo;
extern s32 TempsDemo;
extern u8 VGA_FREQ;
extern u8 You_Win;
extern s16 basex;
extern u8 byte_96883;
extern u8 byte_E4CD8;
extern u8 byte_E4CD9;
extern u8 byte_E4CFC;
extern u8 byte_E4CFD;
extern s32 cdTime;
extern s16 choix_menu;
extern u8 dans_la_map_monde;
extern s16 delai_barre;
extern s16 delai_stereo;
extern display_item_t* dialog_display_item_ptr;
extern u8* display_buffer;
extern s16 ecart_barre;
extern u8 fin_de_rayman;
extern u8 fin_du_jeu;
extern u8 first_option;
extern s16 joyx0;
extern s16 joyy0;
extern u8 language;
extern s16 loop_nb_trames;
extern s16 loop_timing;
extern mem_t* main_mem_fix;
extern mem_t* main_mem_level;
extern s16 max_sound;
extern u8 menuEtape;
extern rgb_palette_t menu_rvb;
extern display_item_t menu_to_display[10];
extern display_item_t msg_to_display[2];
extern u16 nb_fade;
extern u8 new_txt_fee;
extern s16 new_world;
extern s16 numero_palette_special;
extern s16 option_exit;
extern options_t options;
extern options_jeu_t options_jeu;
extern void (*pCOMMANDE_BOX)(void);
extern void (*pDO_COMMANDE)(void);
extern void (*pEND_SCREEN)(void);
extern void (*pINIT_AFFICHE_SCREEN)(void);
extern void (*pINIT_SCREEN)(void);
extern void (*pINIT_SCROLL)(void);
extern void (*pLOAD_SCREEN)(void);
extern u8 pos_YN;
extern s16 position;
extern s16 positiony;
extern s16 repetition;
extern rgb_palette_t rvb[3];
extern rgb_palette_t rvb_E40AC;
extern rgb_palette_t rvb_menu_save;
extern rgb_palette_t rvb_options_in_game;
extern rgb_palette_t rvb_plan3;
extern rgb_palette_t* rvb_special;
extern u16 tRefRam2VramNormalFix[8];
extern u8 tab_key_sav[7];
extern s16 word_E4CEC_x;
extern s16 word_E4CEE_h;
extern s16 word_E4CF0_y;
extern s16 word_E4CF4_w;
extern s16 xpadcentre;
extern s16 xpadmax;
extern s16 xpadmin;
extern u8 yesno_finished;
extern s16 ypadcentre;
extern s16 ypadmax;
extern s16 ypadmin;

/* External callee prototypes from Rayverse proto.h; widths are provisional. */
void CalcTab(void);
void CaptureVideo(u8* source_buf, u8* dest_buf, u8 grayscale);
void DISPLAY_CREDITS_MENU(void);
void DISPLAY_FOND_MENU(void);
void DO_FADE(void);
void DO_FADE_OUT(void);
void DO_SAVE_CHOICE(void);
void DoCdRap(void);
void INIT_FADE_IN(void);
void INIT_FADE_OUT(void);
void INIT_TEXT_TO_DISPLAY(void);
void INIT_TXT_BOX(display_item_t* box);
void InitDemoJeu(void);
void InitFire(void);
void InitGauge(void);
void InitMenuPalette(void);
void InitModeNormalWithFrequency(u8 freq);
void InitModeXWithFrequency(u8 freq);
void InitPlasma(u8 need_set_palette);
bool InitSnd(void);
bool JoystickPresent(void);
void Keyflush(void);
void LOAD_GENERAL_SCREEN(void);
void LOAD_OPTIONS_SCREEN(void);
void LoadBnkFixe(void);
void LoadLanguageTxt(mem_t* mem, s32 language_index);
bool LoadOptionsOnDisk(void);
void LoadPlan3InVignet(mem_t* mem, s32 resource_id);
void POINTEUR_BOUTONS_OPTIONS_BIS(void);
void PlaySnd_old(s16 sound_id);
void RESET_ALL_TOUCHE(void);
void RESET_TOUCHE(u8 key);
void SWAP_BUFFERS(void);
bool SelectButPressed(void);
void SetCompteurTrameAudio(void);
void SetDeviceID(s32 device_id);
void SetDma(s32 dma);
void SetIrq(s32 irq);
void SetPalette(rgb_palette_t* palette, u8 first_color, s32 num_colors);
void SetParam(s32 param);
void SetPort_(s32 port);
void SetVolumeSound(s16 volume);
bool ValidButPressed(void);
void advance_frame(void);
s16 calc_largmax_text(const char* text, s16 char_index, s16 space_width, s16 char_spacing, u8 font_size);
s16 downjoy(void);
void endsynchro(void);
s16 leftjoy(void);
void raj_env_audio(s16 enabled);
void raj_env_sound(s16 volume);
bool raj_env_stereo(s16 stereo);
void readinput(void);
s16 rightjoy(void);
void stop_cd(void);
void synchro(void);
void update_pad_limits(s16* xmax, s16* xmin, s16* ymax, s16* ymin, s16* xcentre, s16* ycentre);
s16 upjoy(void);

/* In-range function prototypes, source-derived unless manually corrected. */
void check_key_table(void);
void LOAD_CONFIG(void);
void DO_NEW_MENUS(void);
void INIT_KEY_SCREEN(void);
void INIT_PAD_SCREEN(void);
void INIT_GRAPHIC_SCREEN(void);
void INIT_VIDEOMODE_SCREEN(void);
void DO_OPTIONS_IN_GAME(void);
void INIT_GENERAL_CHOICE(void);
void AFFICHE_ECRAN_GENERAL(void);
void DO_COMMANDE_GENERAL(void);
void END_GENERAL_SCREEN(void);
void INIT_OPTIONS_CHOICE(void);
void INIT_AFFICHE_ECRAN_OPTIONS(void);
void AFFICHE_ECRAN_OPTIONS(void);
void DO_COMMANDE_OPTIONS(void);
void END_OPTIONS_SCREEN(void);
s16 menu_prg(u32 a1);
void DO_MENU(void);
void default_key(void);
void new_key(void);
void TestButtonReleased(void);
s16 display_box_msg_prg(u32 a1);
void display_box_msg(display_item_t* box);
void display_box_msg_commande(display_item_t* box, void (*commande_box_func)(void));
void AfficheYesNo(void);
void DO_YESNOBIS(void);
u8 confirmation_msg(u8 which_message);
void SYNCHRO_LOOP_PALETTE(synchro_loop_palette_func_t func);
s16 apparition_prg(u32 a1);
s16 fondu_prg(u32 a1);
s16 change_couleur_prg(void);
void FonduOption(s16 x, s16 y, s16 w, s16 h, u8 mode);

/* DOS 1:39308; menu.c:106 */
void check_key_table(void) {
    u8 valid = 1;
    s32 i;
    for (i = 0; i < 7; ++i) {
        u8 key = *tab_key[i];
        if (key > SC_DELETE || key_descriptions_qwerty[key] == 0 || key_descriptions_azerty[key] == 0)
            valid = 0;
    }
    if (!valid) {
        *tab_key[0] = SC_LEFT;
        *tab_key[1] = SC_UP;
        *tab_key[2] = SC_RIGHT;
        *tab_key[3] = SC_DOWN;
        *tab_key[4] = SC_CONTROL;
        *tab_key[5] = SC_ALT;
        *tab_key[6] = SC_X;
    }
}

/* DOS 1:39388; menu.c:128 */
void LOAD_CONFIG(void) {
    if (LoadOptionsOnDisk()) {
        LoadLanguageTxt(main_mem_fix, 0); // English
        if (tRefRam2VramNormalFix[0] >= 128 && xpadmax == -1) {
            // NOTE: this may occur if a RAYMAN.CFG is present that contains values that meet this condition.
            // TODO: figure out why this causes Mode X to be used instead of normal mode.
            GameModeVideo = MODE_X;
            P486 = 0;
        }
        if (FondAutorise == 2) {
            GameModeVideo = MODE_X;
            P486 = 1;
        }
        POINTEUR_BOUTONS_OPTIONS_BIS();

        // STUB: this section seems to be related to sound card initialization
        if (CarteSonAutorisee && DeviceID != 999) {
            SetPort_(Port);
            SetIrq(Irq);
            SetDma(Dma);
            SetParam(Param);
            SetDeviceID(DeviceID);
            //if (sub_3E780()) {} // seems to do something with locking memory regions (DOS protected mode stuff)

        } else {
            SetDeviceID(DeviceID);
            CarteSonAutorisee = 0;
        }

        if (CarteSonAutorisee) {
            LoadBnkFixe();
            InitSnd();
        }

        if (JoystickPresent()) {
            // stub: init joypad
        } else {
            if (xpadmax == -1) {
                xpadmax = -2;
            }
        }

        // stub
    } else {
        // We didn't load a .cfg file -> load defaults
        if (CarteSonAutorisee) {
            LoadBnkFixe();
            InitSnd();
        }
        language = 0; // English
        LoadLanguageTxt(main_mem_fix, language);
        if (JoystickPresent()) {
            pLOAD_SCREEN = LOAD_OPTIONS_SCREEN;
            CalcTab();
            DO_FADE_OUT();
            MAIN_CALIBRATE_JOYSTICK();
        } else {
            xpadmax = -2;
        }
        options_jeu.music_enabled = 2;
        options_jeu.sound_volume = 18;
        options_jeu.is_stereo = 1;
        options_jeu.jump = 1;
        options_jeu.fist = 0;
        options_jeu.action = 2;
        SetVolumeSound((options_jeu.sound_volume * 127) / 20);
        GameModeVideo = MODE_NORMAL;
        POINTEUR_BOUTONS_OPTIONS_BIS();
    }

    key_descriptions = key_descriptions_azerty;
    set_special_key_descriptions();
    key_descriptions = key_descriptions_qwerty;
    set_special_key_descriptions();
    check_key_table();
}

/* DOS 1:39638; menu.c:204 */
void NewFrequency(u8 frequency) {
    switch (frequency) {
    case 0:
        use_sync = 1;
        set_vga_frequency(60);
        display_mode = 0;
        break;
    case 1:
        use_sync = 1;
        display_mode = 0;
        set_vga_frequency(50);
        break;
    case 2:
        use_sync = 0;
        set_vga_frequency(60);
        break;
    case 0x46:
        use_sync = 1;
        display_mode = 0;
        set_vga_frequency(70);
        InitPcSoundCard();
        return;
    case 0x50:
        use_sync = 1;
        display_mode = 0;
        set_vga_frequency(80);
        InitPcSoundCard();
        return;
    case 0x64:
        use_sync = 1;
        display_mode = 0;
        set_vga_frequency(100);
        break;
    default:
        break;
    }
    InitPcSoundCard();
}

/* DOS 1:39720; menu.c:209 */
void InitClipping(void) {
    if (GameModeVideo != MODE_NORMAL) {
        default_sprite_clipping();
    } else if (is_fee || (num_world == world_6_cake && num_level == 4) || get_casse_brique_active()) {
        set_xywhmap(TabW1[0], TabW2[0], TabH1[0], TabH2[0]);
        sprite_clipping(Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2);
    } else {
        set_xywhmap(TabW1[SizeScreen], TabW2[SizeScreen], TabH1[SizeScreen], TabH2[SizeScreen]);
        sprite_clipping(Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2);
        N_CLRSCR(DrawBufferNormal);
    }
}

/* DOS 1:3980c; menu.c:223 */
/* IsBackgroundOn: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:398a4; misc_source.c:64 */
void set_special_key_descriptions(void) {
    key_descriptions[SC_BACKSPACE] = language_txt[212]; // backspace
    key_descriptions[SC_TAB] = language_txt[213]; // tab
    key_descriptions[SC_ENTER] = language_txt[214]; // return
    key_descriptions[SC_CONTROL] = language_txt[210]; // ctrl
    key_descriptions[SC_ALT] = language_txt[211]; // alt
    key_descriptions[SC_SPACE] = language_txt[209]; // space
    key_descriptions[SC_CAPSLOCK] = language_txt[215]; // caps lock
    key_descriptions[SC_HOME] = language_txt[216]; // home
    key_descriptions[SC_UP] = language_txt[205]; // up arrow
    key_descriptions[SC_PAGEUP] = language_txt[217]; // page up
    key_descriptions[SC_LEFT] = language_txt[204]; // left arrow
    key_descriptions[SC_RIGHT] = language_txt[206]; // right arrow
    key_descriptions[SC_END] = language_txt[218]; // end
    key_descriptions[SC_DOWN] = language_txt[207]; // down arrow
    key_descriptions[SC_PAGEDOWN] = language_txt[219]; // page down
    key_descriptions[SC_INSERT] = language_txt[220]; // insert
    key_descriptions[SC_DELETE] = language_txt[221]; // del
}

/* DOS 1:39998; menu.c:234 */
void DO_NEW_MENUS(void) {
    s16 NewMenusArgs = 0;
    dans_la_map_monde = 0;
    SetCompteurTrameAudio();
    current_pal_id = 0;
    choix_menu = 0;
    fin_du_jeu = 0;
    MENU_SUITE = 0;
    MENU_RETURN = 0;
    menuEtape = 0;
    if (OptionMusicCdActive) {
        MusicCdActive = options_jeu.music_enabled;
        if (!MusicCdActive) {
            stop_cd();
        }
    }
    Keyflush(); //TODO
    CalcTab();
    default_key();
    while (!ModeDemo && !fin_du_jeu && !NewMenusArgs) {
        raj_env_sound(options_jeu.sound_volume);
        raj_env_stereo(options_jeu.is_stereo);
        raj_env_audio(options_jeu.music_enabled);
        switch(menuEtape) {
            default: break;
            case 0: {
                // Main menu screen
                pINIT_SCREEN = INIT_GENERAL_CHOICE;
                pLOAD_SCREEN = LOAD_GENERAL_SCREEN;
                INIT_FADE_IN();
                DO_MENU();
            } break;
            case 3: {
                // Save selection screen
                DO_SAVE_CHOICE();
                if (!MENU_RETURN) {
                    fin_du_jeu = 0;
                    NewMenusArgs = 1;
                    new_world = 1;
                }
            } break;
            case 4: {
                // Options screen
                pINIT_SCREEN = INIT_OPTIONS_CHOICE;
                if (pLOAD_SCREEN) {
                    byte_E4CFD = 1;
                    pLOAD_SCREEN = LOAD_OPTIONS_SCREEN;
                }
                DO_MENU();
            } break;
            case 7: {
                // Setup keys screen (in options)
                pINIT_SCREEN = INIT_KEY_SCREEN;
                pLOAD_SCREEN = NULL;
                DO_MENU();
            } break;
            case 8: {
                // Setup pad screen (in options)
                pINIT_SCREEN = INIT_PAD_SCREEN;
                pLOAD_SCREEN = NULL;
                DO_MENU();
            } break;
            case 9: {
                // Graphics details screen (in options)
                pINIT_SCREEN = INIT_GRAPHIC_SCREEN;
                pLOAD_SCREEN = NULL;
                DO_MENU();
            } break;
            case 10: {
                pINIT_SCREEN = INIT_VIDEOMODE_SCREEN;
                pLOAD_SCREEN = NULL;
                DO_MENU();
            } break;
        }
        RESET_ALL_TOUCHE();
        if (MENU_RETURN) {
            menuEtape = 0;
            MENU_RETURN = 0;
        }
    }
    new_key();
    //SaveOptionsOnDisk();

}

/* DOS 1:39b7c; menu.c:320 */
/* DO_OPTIONS_IN_GAME: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:39e98; menu.c:426 */
void INIT_GENERAL_CHOICE(void) {
    u8 max_options = 0;
    s32 i;
    pINIT_AFFICHE_SCREEN = NULL;
    pINIT_SCROLL = NULL;
    pDO_COMMANDE = DO_COMMANDE_GENERAL;
    pEND_SCREEN = END_GENERAL_SCREEN;
    pAFFICHE_SCREEN = AFFICHE_ECRAN_GENERAL;
    option_exit = 3;
    nbre_options = 4;
    basex = 160;
    for (i = 0; i < nbre_options; ++i) {
        display_item_t* display_item = menu_to_display + i;
        strcpy(display_item->text, language_txt[190]); // "music"
        display_item->color = byte_49290[i];
        switch(i) {
            default: break;
            case 0: {
                strcpy(display_item->text, language_txt[109]); // "/start/"
                display_item->ypos = 30;
            } break;
            case 1: {
                strcpy(display_item->text, language_txt[181]); // "/options/"
                display_item->ypos = 60;
            } break;
            case 2: {
                strcpy(display_item->text, language_txt[110]); // "/credits/"
                display_item->ypos = 155;
            } break;
            case 3: {
                strcpy(display_item->text, language_txt[154]); // "/exit/"
                display_item->ypos = 185;
            } break;
        }
        display_item->xpos = basex;
        display_item->font_size = 1;
        display_item->is_fond = 1;
        display_item->field_D5 = 0;
        INIT_TXT_BOX(display_item);
    }
    positiony = 0;
    button_released = 1;
    compteur = 0;
    max_compteur = 100;
    delai_repetition = 12;
    repetition = 6;
    choix_menu = 0;
    sortie_options = 0;

}

/* DOS 1:3a0dc; menu.c:477 */
void AFFICHE_ECRAN_GENERAL(void) {
    s32 i;
    DISPLAY_FOND_MENU();
    for (i = 0; i < nbre_options; ++i) {
        display_item_t* to_display = menu_to_display + i;
        if (i == positiony) {
            display_box_text_plasma(to_display, 1);
        } else {
            display_text(to_display->text, to_display->xpos, to_display->ypos, to_display->font_size, to_display->color);
        }
    }
}

/* DOS 1:3a178; menu.c:490 */
void DO_COMMANDE_GENERAL(void) {
    TestCompteur();
    ++TempsDemo;
    if (TempsDemo >= RunTimeDemo && !fin_du_jeu) {
        InitDemoJeu();
    }
    if (SelectButPressed()) {
        RESET_TOUCHE(SC_ESCAPE);
        if (confirmation_msg(0)) {
            MENU_RETURN = 1;
        }
    }
    if (upjoy() && (button_released || (delai_repetition < compteur && compteur % repetition == 0))) {
        PlaySnd_old(68);
        if (positiony <= first_option) {
            positiony = option_exit;
        } else {
            --positiony;
        }
        TempsDemo = 0;
    }
    if (downjoy() && (button_released || (delai_repetition < compteur && compteur % repetition == 0))) {
        PlaySnd_old(68);
        if (positiony < option_exit) {
            ++positiony;
        } else {
            positiony = first_option;
        }
        TempsDemo = 0;
    }
    if (ValidButPressed() && button_released) {
        PlaySnd_old(69);
        switch(positiony) {
            default: break;
            case 0: {
                TempsDemo = 0;
                sortie_options = 1;
                menuEtape = 3;
            } break;
            case 1: {
                TempsDemo = 0;
                sortie_options = 1;
                menuEtape = 4;
            } break;
            case 2: {
                You_Win = 1;
                loop_nb_trames = 0;
                new_txt_fee = 0;
                LoadPlan3InVignet(main_mem_level, 61);
                loop_timing = 4;
                INIT_TEXT_TO_DISPLAY();
                DO_FADE_OUT();
                INIT_FADE_IN();
                DISPLAY_CREDITS_MENU();
                block_free(main_mem_level);
                You_Win = 0;
            } break;
            case 3: {
                if (confirmation_msg(0)) {
                    MENU_RETURN = 1;
                }
            } break;
        }
    }
    TestButtonReleased();
}

/* DOS 1:3a3d8; menu.c:558 */
void END_GENERAL_SCREEN(void) {
    if (MENU_RETURN) {
        fin_du_jeu = 1;
        fin_de_rayman = 1;
        if (pAFFICHE_SCREEN) {
            pAFFICHE_SCREEN();
            advance_frame();
            InitPlasma(1);
            SWAP_BUFFERS();
        }
    } else if (!ModeDemo) {
        RunTimeDemo = 1800;
        if (First_Hist == 2) {
            First_Hist = 0;
        }
        DO_FADE_OUT();
        INIT_FADE_IN();
    }
}

/* DOS 1:3a44c; menu.c:579 */
void INIT_OPTIONS_CHOICE(void) {
    /*if (WindowsLance == 0) {
        if (__readdr(0) || __readdr(1) || __readdr(2) || __readdr(3)) { exit(); }
    }*/
    pINIT_AFFICHE_SCREEN = INIT_AFFICHE_ECRAN_OPTIONS;
    pAFFICHE_SCREEN = AFFICHE_ECRAN_OPTIONS;
    pDO_COMMANDE = DO_COMMANDE_OPTIONS;
    pEND_SCREEN = END_OPTIONS_SCREEN;
    delai_repetition = 12;
    repetition = 6;
    max_sound = 20;
    ecart_barre = 4;
    compteur = 0;
    max_compteur = 100;
    button_released = 1;
    delai_barre = 3;
    delai_stereo = 8;
    if (language != 0) {
        byte_E4CFC = 90;
    } else {
        byte_E4CFC = 75;
    }
    basex = 160 - byte_E4CFC;
    if (OptionGame) {
        position = 6;
        option_exit = 6;
        nbre_options = 6;
    } else {
        position = 5;
        option_exit = 5;
        nbre_options = 5;
    }
    first_option = 0;
    general_init_screen(20, 60, 5);
    if (byte_96883) {
        options.sound_volume = byte_E4CD9;
        byte_96883 = 0;
        options.music_enabled = byte_E4CD8;
    } else {
        options.music_enabled = options_jeu.music_enabled;
        options.sound_volume = options_jeu.sound_volume;
    }
    ExitMenu = 0;
    sortie_options = 0;
}

/* DOS 1:3a628; menu.c:626 */
void INIT_AFFICHE_ECRAN_OPTIONS(void) {
    s32 i;
    char v39[4];
    char* v32 = "off";
    u8 v28[8] = {5, 5, 5, 5, 5, 5, 3, 0}; //0x5050505;
//    u8 v29[4] = {5, 5, 3, 0}; // 0x30505; //dword_49294[1]
    u32 v30 = 0; //dword_49294[2];
    menu_to_display[9].is_fond = 1;
    menu_to_display[9].font_size = 2;
    menu_to_display[9].field_D5 = 0;
    menu_to_display[9].color = 1;
    memcpy(v39, "on", 3);
    for (i = first_option; i <= first_option + nbre_options; ++i) {
        display_item_t* to_display = menu_to_display + i;
        to_display->font_size = 2;
        to_display->field_D5 = 0;
        to_display->is_fond = 1;
        if (i == option_exit) {
            strcpy(to_display->text, language_txt[232]); // /exit/
            to_display->xpos = (SCREEN_WIDTH / 2);
            to_display->ypos = debut_sortie;
            to_display->color = v28[6];
        } else {
            to_display->color = v28[i];
            strcpy(to_display->text, language_txt[190 + i]); // { "music", "sound", "/setup keys/", "/graphics details/", "/quit game/" }
            switch(i) {
                case 0: {
                    // Text "music" offsetted to the left with "on off" to the right
                    s32 text_width = calc_largmax_text(to_display->text, -1, 8, 1, 2) + 16 +
                                     calc_largmax_text(v39, -1, 8, 1, 2) + calc_largmax_text(v32, -1, 8, 1, 2);
                    to_display->xpos = (SCREEN_WIDTH / 2) - (text_width >> 1);
                } break;
                case 1: {
                    // Text "sound" offsetted to the left
                    s32 text_width = calc_largmax_text(to_display->text, -1, 8, 1, 2) + 16 +
                             20 * calc_largmax_text("|", -1, 8, 1, 2);
                    to_display->xpos = (SCREEN_WIDTH / 2) - (text_width >> 1);
                } break;
                case 2:
                case 3:
                case 4:
                case 5:
                    // Text centered
                    to_display->xpos = (SCREEN_WIDTH / 2);
                    break;
                default:
                    break;
            }
            to_display->ypos = i * (ecart_options + 15) + debut_options;
        }
        INIT_TXT_BOX(to_display);
    }

    strcpy(msg_to_display[0].text, language_txt[136]); ///no joystick detected !/check if cable is connected/
    msg_to_display[0].xpos = (SCREEN_WIDTH / 2);
    msg_to_display[0].ypos = (SCREEN_HEIGHT / 2);
    msg_to_display[0].font_size = 2;
    msg_to_display[0].is_fond = 2;
    msg_to_display[0].field_D5 = 0;
    INIT_TXT_BOX(&msg_to_display[0]);
    msg_to_display[1].xpos = (SCREEN_WIDTH / 2);
    msg_to_display[1].ypos = (SCREEN_HEIGHT / 2);
    msg_to_display[1].font_size = 2;
    msg_to_display[1].is_fond = 2;
    msg_to_display[1].field_D5 = 0;

    if (byte_E4CFD) {
        if (OptionGame) {
            word_E4CEC_x = basex;
            word_E4CF4_w = 2 * byte_E4CFC;
            word_E4CF0_y = debut_titre - 25;
            word_E4CEE_h = menu_to_display[option_exit].ypos - debut_titre + 35;
            FonduOption(word_E4CEC_x, word_E4CF0_y, word_E4CF4_w, word_E4CEE_h, 0);
        }
    } else {
        s16 v21, v42_hi, v22_x, v23_y, v24, unk_w;
        s32 v37, v33, v25, v34, v26, v31, v38, v36, v27_h;
        v21 = debut_titre - 25;
        v42_hi = menu_to_display[option_exit].ypos - debut_titre + 35;
        v22_x = (basex >= word_E4CEC_x) ? word_E4CEC_x : basex;
        v23_y = word_E4CF0_y;
        if (v21 < word_E4CF0_y) {
            v23_y = debut_titre - 25;
        }
        v24 = word_E4CF4_w + word_E4CEC_x;
        v37 = basex;
        v33 = v24;
        v25 = basex + 2 * byte_E4CFC;
        v34 = (v25 >= v24) ? v25 - v22_x : v24 - v22_x;
        unk_w = v34;
        v26 = word_E4CEE_h + word_E4CF0_y;
        v31 = v21;
        v38 = v42_hi;
        v36 = v42_hi + v21;
        if (v26 <= v36) {
            v26 = v36;
        }
        v27_h = v26 - v23_y;
        word_E4CEC_x = basex;
        word_E4CF0_y = debut_titre - 25;
        word_E4CEE_h = v42_hi;
        word_E4CF4_w = 2 * byte_E4CFC;
        if (OptionGame) {
            FonduOption(v22_x, v23_y, unk_w, v27_h, 1); //TODO

        } else {
            FonduPixel(v22_x, v23_y, unk_w, v27_h); //TODO
        }
    }
    byte_E4CFD = 0;
}

/* DOS 1:3ab34; menu.c:736 */
/* AFFICHE_ECRAN_OPTIONS: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3af28; menu.c:803 */
void DO_COMMANDE_OPTIONS(void) {
    TestCompteur();
    if (SelectButPressed()) {
        MENU_RETURN = 1;
    }
    if (upjoy() && !rightjoy() && !leftjoy() &&
            (button_released || (delai_repetition < compteur && (compteur % repetition) == 0))
    ) {
        PlaySnd_old(68);
        if (position == first_option) {
            position = option_exit;
        } else {
            --position;
        }
    }
    if (downjoy() && !rightjoy() && !leftjoy() &&
        (button_released || (delai_repetition < compteur && (compteur % repetition) == 0))
    ) {
        PlaySnd_old(68);
        if (position == option_exit) {
            position = first_option;
        } else {
            ++position;
        }
    }
    if (position == 0 && button_released && (ValidButPressed() || rightjoy() || leftjoy())) {
        options.music_enabled = !options.music_enabled;
        if (options.music_enabled) {
            SetCompteurTrameAudio();
            cdTime = 0;
            if (OptionMusicCdActive) {
                MusicCdActive = 1;
            }
        } else {
            stop_cd();
            if (OptionMusicCdActive) {
                MusicCdActive = 0;
            }
        }
    } else if (position == 1 && rightjoy()) {
        if (options.sound_volume < max_sound) {
            if (button_released || (delai_repetition < compteur && (compteur % repetition) == 0)) {
                PlaySnd_old(68);
                ++options.sound_volume;
                raj_env_sound(options.sound_volume);
            }
        }
    } else if (position == 1 && leftjoy()) {
        if (options.sound_volume > 0) {
            if (button_released || (delai_repetition < compteur && (compteur % repetition) == 0)) {
                PlaySnd_old(68);
                --options.sound_volume;
                raj_env_sound(options.sound_volume);
            }
        }
    } else if (position == 3 && ValidButPressed()) {
        if (JoystickPresent()) {
            byte_E4CD8 = options.music_enabled;
            byte_E4CD9 = options.sound_volume;
            byte_96883 = 1;
            menuEtape = 8; // go to setup pad
            sortie_options = 1;
        } else {
            display_box_msg(&msg_to_display[0]); // no joystick connected
        }
    } else if (position == 2 && ValidButPressed()) {
        byte_E4CD8 = options.music_enabled;
        byte_E4CD9 = options.sound_volume;
        menuEtape = 7; // go to setup keys
        sortie_options = 1;
        byte_96883 = 1;
    } else if (position == 4 && ValidButPressed()) {
        byte_E4CD8 = options.music_enabled;
        byte_E4CD9 = options.sound_volume;
        menuEtape = 9; // go to graphics details
        byte_96883 = 1;
        sortie_options = 1;
    } else if (position == 5 && ValidButPressed() && button_released && OptionGame) {
        if (confirmation_msg(0)) {
            ExitMenu = 1;
            fin_du_jeu = 1;
            new_world = 1;
        }

    }
    TestButtonReleased();
}

/* DOS 1:3b334; menu.c:892 */
void END_OPTIONS_SCREEN(void) {
    if ((position == option_exit && MENU_RETURN != 1) || fin_du_jeu) {
        options_jeu.music_enabled = options.music_enabled;
        options_jeu.sound_volume = options.sound_volume;
    }
    if (position == option_exit || MENU_RETURN == 1 || fin_du_jeu) {
        if (OptionMusicCdActive) {
            MusicCdActive = options_jeu.music_enabled;
        }
        menuEtape = 0;
        if (OptionGame) {
            FonduOption(word_E4CEC_x, word_E4CF0_y, word_E4CF4_w, word_E4CEE_h, 2);
        } else {
            DO_FADE_OUT();
        }
    }
}

/* DOS 1:3b3ec; menu.c:911 */
/* INIT_KEY_SCREEN: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3b514; menu.c:916 */
/* INIT_AFFICHE_KEY_SCREEN: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3b8a4; menu.c:921 */
/* AFFICHE_KEY_SCREEN: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3bacc; menu.c:926 */
/* DO_COMMANDE_KEY: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3bd50; menu.c:931 */
/* END_KEY_SCREEN: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3bda4; menu.c:936 */
/* INIT_BUTTON_SCREEN: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3be04; menu.c:941 */
/* INIT_AFFICHE_BUTTON_SCREEN: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3bf60; menu.c:946 */
/* AFFICHE_BUTTON_JOYSTICK: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3c138; menu.c:951 */
/* DO_BUTTON_COMMANDE: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3c390; menu.c:956 */
/* END_BUTTON_COMMAND: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3c3d0; menu.c:961 */
/* INIT_PAD_SCREEN: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3c4d0; menu.c:966 */
/* INIT_AFFICHE_PAD_SCREEN: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3c854; menu.c:971 */
/* AFFICHE_PAD_SCREEN: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3c9d8; menu.c:976 */
/* DO_COMMANDE_PAD: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3cd38; menu.c:981 */
/* END_PAD_SCREEN: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3cd94; menu.c:986 */
/* InitParamGraphicsScreen: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3d094; menu.c:991 */
/* INIT_GRAPHIC_SCREEN: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3d178; menu.c:996 */
/* INIT_AFFICHE_GRAPHIC_SCREEN: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3d55c; menu.c:1001 */
/* AFFICHE_GRAPHIC_SCREEN: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3da8c; menu.c:1006 */
/* DO_COMMANDE_GRAPHIC: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3df88; menu.c:1011 */
/* END_GRAPHIC_SCREEN: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3e0a0; menu.c:1016 */
/* INIT_VIDEOMODE_SCREEN: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3e1cc; menu.c:1021 */
/* INIT_AFFICHE_VIDEOMODE_SCREEN: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3e470; menu.c:1026 */
/* AFFICHE_VIDEOMODE_SCREEN: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3e574; menu.c:1031 */
/* DO_COMMANDE_VIDEOMODE: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3e708; menu.c:1036 */
/* END_VIDEOMODE_SCREEN: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3e790; menu.c:1041 */
/* INIT_CALIBRATE_JOYSTICK: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3e804; menu.c:1046 */
/* AFFICHE_CALIBRATE_JOYSTICK: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3e99c; menu.c:1051 */
/* calibrate_joystick_prg: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3e9e8; menu.c:1056 */
/* AFFICHE_CENTER_JOYSTICK: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3eae8; menu.c:1061 */
/* center_joystick_prg: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3eb28; menu.c:1066 */
/* MAIN_CALIBRATE_JOYSTICK: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3f17c; menu.c:1071 */
s16 menu_prg(u32 a1) {
    bool pad_limits_changed = false;
    s16 need_exit = 0;
    (void)a1;
    readinput();
    DoCdRap();
    if (Main_Control) {
        if (joyx0 < xpadmin) {
            xpadmin = joyx0;
            pad_limits_changed = true;
        } else if (joyx0 > xpadmax) {
            xpadmax = joyx0;
            pad_limits_changed = true;
        }
        if (joyy0 < ypadmin) {
            ypadmin = joyy0;
            pad_limits_changed = true;
        } else if (joyy0 > ypadmax) {
            ypadmax = joyy0;
            pad_limits_changed = true;
        }
    }
    if (pad_limits_changed) {
        // NOTE: this is not present in the Android version.
        update_pad_limits(&xpadmax, &xpadmin, &ypadmax, &ypadmin, &xpadcentre, &ypadcentre);
    }
    if (pAFFICHE_SCREEN) {
        pAFFICHE_SCREEN();
    }
    if (ReInitPlasma) {
        ReInitPlasma = 0;
        advance_frame();
        InitPlasma(1);
        SWAP_BUFFERS();
    }
    if (fin_du_jeu || sortie_options || MENU_RETURN || ModeDemo) {
        need_exit = 1;
    }
    if (!need_exit) {
        if (pDO_COMMANDE && nb_fade == 0) {
            pDO_COMMANDE();
        }
        if (ValidButPressed() && option_exit == position) {
            sortie_options = 1;
            PlaySnd_old(69);
        }
        if (MENU_RETURN) {
            PlaySnd_old(77);
        }
    }
    return need_exit;
}

/* DOS 1:3f2e0; menu.c:1124 */
/* DO_INTER_MENU: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3f318; menu.c:1129 */
void DO_MENU(void) {
    rgb_palette_t* palette;
    if (pINIT_SCREEN) {
        pINIT_SCREEN();
    }
    if (pINIT_AFFICHE_SCREEN) {
        pINIT_AFFICHE_SCREEN();
    }
    if (pLOAD_SCREEN) {
        pLOAD_SCREEN();
    }
    if ((num_world == world_1_jungle && num_level == 9) ||
            (num_world == world_2_music && num_level == 4) ||
            (num_world == world_4_image && num_level == 4) ||
            (num_world == world_5_cave && num_level == 4)
    ) {
        palette = rvb_special + numero_palette_special;
    } else {
        palette = rvb + current_pal_id;
        rvb[current_pal_id] = rvb_plan3;
    }
    menu_rvb = *palette;
    InitGauge(); // TODO
    InitPlasma(0); // TODO
    SYNCHRO_LOOP(menu_prg);
    if (pEND_SCREEN) {
        pEND_SCREEN();
    }
    menu_rvb = rvb[current_pal_id];
}

/* DOS 1:3f488; menu.c:1161 */
void default_key(void) {
    s32 i;
    check_key_table();
    for (i = 0; i < 7; ++i) {
        tab_key_sav[i] = *(tab_key[i]);
    }
    *(tab_key[0]) = SC_LEFT;
    *(tab_key[1]) = SC_UP;
    *(tab_key[2]) = SC_RIGHT;
    *(tab_key[3]) = SC_DOWN;
    *(tab_key[4]) = SC_CONTROL;
    *(tab_key[5]) = SC_ALT;
    *(tab_key[6]) = SC_X;
}

/* DOS 1:3f4ec; menu.c:1176 */
void new_key(void) {
    s16 i;
    for (i = 0; i < 7; ++i) {
        *(tab_key[i]) = tab_key_sav[i];
    }
}

/* DOS 1:3f50c; menu.c:1183 */
/* CalcSpeed: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3f698; menu.c:1189 */
void general_init_screen(s16 a1, s16 a2, s16 a3) {
    debut_titre = ((a1 * 200 / 100) / 2) + 11;
    ecart_options = ((a2 * 200 / 100) - 15 * nbre_options) / (nbre_options + 1);
    debut_options = ecart_options + 15 + (a1 * 200 / 100);
    debut_sortie = ((a3 * 200 / 100) / 2) + 11 + (a1 * 200 / 100) + (a2 * 200 / 100);
    sortie_options = 0;
}

/* DOS 1:3f76c; menu.c:1198 */
void TestCompteur(void) {
    if (compteur < max_compteur && !button_released) {
        ++compteur;
    } else if (!button_released) {
        compteur = delai_repetition + 1;
    } else {
        compteur = 0;
    }
}

/* DOS 1:3f7b0; menu.c:1211 */
void TestButtonReleased(void) {
    if (!upjoy() && !downjoy() && !rightjoy() && !leftjoy() && !ValidButPressed()) {
        button_released = 1;
        compteur = 0;
    } else {
        button_released = 0;
    }
}

/* DOS 1:3f800; menu.c:1221 */
void display_box_text_plasma(display_item_t* box, u8 a2) {
    if (!msg_box_being_displayed) {
        PlasmaBox(box->centered_x_pos, box->centered_y_pos, box->width, box->height, a2);
        display_box_text(box);
    } else {
        display_text(box->text, box->xpos, box->ypos, box->font_size, box->color);
    }
}

/* DOS 1:3f878; menu.c:1231 */
void display_box_text_fire(display_item_t* a1) {
    FireBox(a1->centered_x_pos, a1->centered_y_pos, a1->width, a1->height);
    display_box_text(a1);
}

/* DOS 1:3f8b4; menu.c:1237 */
s16 display_box_msg_prg(u32 a1) {
    yesno_finished = 0;
    readinput();
    if (pAFFICHE_SCREEN) {
        pAFFICHE_SCREEN();
    }
    display_box_text_fire(dialog_display_item_ptr);
    if (pCOMMANDE_BOX) {
        pCOMMANDE_BOX();
    }
    if (SelectButPressed() || yesno_finished) {
        return 1;
    } else {
        return 0;
    }
}

/* DOS 1:3f904; menu.c:1255 */
void display_box_msg(display_item_t* box) {
    void (*affiche_screen)(void) = pAFFICHE_SCREEN;
    dialog_display_item_ptr = box;
    if (affiche_screen) {
        pAFFICHE_SCREEN();
    }
    endsynchro();
    synchro();
    InitFire();
    SWAP_BUFFERS();
    msg_box_being_displayed = 1;
    pCOMMANDE_BOX = NULL;
    SYNCHRO_LOOP(display_box_msg_prg);
    msg_box_being_displayed = 0;
    ReInitPlasma = 1;
    RESET_ALL_TOUCHE();
}

/* DOS 1:3f960; menu.c:1272 */
void display_box_msg_commande(display_item_t* box, void (*commande_box_func)(void)) {

    dialog_display_item_ptr = box;
    pCOMMANDE_BOX = commande_box_func;
    msg_box_being_displayed = 1;
    endsynchro();
    synchro();
    InitFire();

    do {
        if (pAFFICHE_SCREEN) {
            pAFFICHE_SCREEN();
        }
        display_box_text_fire(dialog_display_item_ptr);
        AfficheYesNo();
        endsynchro();
        synchro();
        DO_FADE();
        SWAP_BUFFERS();
        readinput();
    } while (ValidButPressed() || nb_fade != 0);

    SYNCHRO_LOOP(display_box_msg_prg);
    msg_box_being_displayed = 0;
    ReInitPlasma = 1;
    RESET_ALL_TOUCHE();
}

/* DOS 1:3f9f0; menu.c:1303 */
void AfficheYesNo(void) {
    display_item_t* to_display = dialog_display_item_ptr;
    if (pos_YN == 0) {
        display_text(language_txt[147], to_display->xpos - 25, to_display->centered_y_pos + to_display->height, 2, 1);
        display_text(language_txt[148], to_display->xpos + 35, to_display->centered_y_pos + to_display->height, 2, 0);
    } else {
        display_text(language_txt[147], to_display->xpos - 25, to_display->centered_y_pos + to_display->height, 2, 0);
        display_text(language_txt[148], to_display->xpos + 35, to_display->centered_y_pos + to_display->height, 2, 1);
    }
}

/* DOS 1:3fac4; menu.c:1315 */
void DO_YESNOBIS(void) {
    TestCompteur();
    if ((leftjoy() || rightjoy()) && !(downjoy() || upjoy())) {
        if (button_released || (compteur > delai_repetition && (compteur % repetition == 0))) {
            pos_YN = !pos_YN;
            PlaySnd_old(68);
        }
    }
    AfficheYesNo();
    if (SelectButPressed()) {
        pos_YN = 1;
        yesno_finished = 1;
    }
    if (ValidButPressed() && button_released) {
        yesno_finished = 1;
    }
    TestButtonReleased();
}

/* DOS 1:3fb94; menu.c:1335 */
/* confirmation_msg: OPEN - no usable DOS C body; Rayverse body depends on a port-only state object or is a stub. */

/* DOS 1:3fef0; menu.c:1413 */
void SYNCHRO_LOOP_PALETTE(synchro_loop_palette_func_t func) {
    s16 ended = 0;
    u32 timer = 0;
    do {
        endsynchro();
        synchro();
        SetPalette(&rvb_E40AC, 0, 256);
        timer = 0;
        ended = func();
    } while(!ended);
}

/* DOS 1:3ff3c; menu.c:1426 */
s16 apparition_prg(u32 a1) {
    s32 w, h, src_row_skip, yoff, yoff_end;
    u8* src;
    (void)a1;
    w = wFondu;
    h = hFondu;
    src_row_skip = SCREEN_WIDTH - w;

    DoCdRap();
    DISPLAY_FOND_MENU();

    src = EffetBufferNormal + (SCREEN_WIDTH * SCREEN_HEIGHT) + yFondu * SCREEN_WIDTH + xFondu;

    yoff = -((h-1)/2);
    yoff_end = h/2;

    while (yoff < yoff_end) {
        s32 dst_y = (SCREEN_HEIGHT/2) + EtapeFondu * yoff;

        if (dst_y > Bloc_lim_H1 && dst_y < Bloc_lim_H2) {
            u8* dst_row = (u8*)DrawBufferNormal + dst_y * SCREEN_WIDTH;

            s32 xoff = -((w-1)/2);
            s32 xoff_end = w/2;
            u8* s = src;
            while (xoff < xoff_end) {
                s32 dst_x = (SCREEN_WIDTH/2) + EtapeFondu * xoff;

                if (dst_x > Bloc_lim_W1 && dst_x < Bloc_lim_W2) {
                    dst_row[dst_x] = *s;
                }
                ++xoff;
                ++s;
            }

            src += w + src_row_skip; // advance exactly one source scanline
        } else {
            src += SCREEN_WIDTH; // skip full source scanline
        }

        ++yoff;
    }

    // Update EtapeFondu / decide end
    if (InOut != 0) {
        // decreasing until 1
        --EtapeFondu;
        return (EtapeFondu == 1) ? 1 : 0;
    } else {
        // increasing until 32; at 16 trigger INIT_FADE_OUT
        if (EtapeFondu == 16) {
            INIT_FADE_OUT();
        }
        ++EtapeFondu;
        return (EtapeFondu == 32) ? 1 : 0;
    }
}

/* DOS 1:400fc; menu.c:1484 */
s16 fondu_prg(u32 a1) {
    s32 i, j;
    s32 x = xFondu;
    s32 y = yFondu;
    s32 w = wFondu;
    s32 h = hFondu;

    s32 row_skip = SCREEN_WIDTH - w;

    u8* dst = DrawBufferNormal + y * SCREEN_WIDTH + x;
    u8* src = EffetBufferNormal + (SCREEN_WIDTH * SCREEN_HEIGHT) + y * SCREEN_WIDTH + x;

    s32 E = (s16)EtapeFondu;
    s32 mul = 16 - E;
    s32 div = 17 - E;

    (void)a1;
    for (j = 0; j < h; ++j) {
        for (i = 0; i < w; ++i) {
            u8 a = dst[i];
            u8 b = src[i];
            if (a != b) {
                dst[i] = (u8)((a * mul + b) / div);
            }
        }
        dst += w + row_skip;
        src += w + row_skip;
    }

    EtapeFondu = (s16)(E + 1);
    return (EtapeFondu == 16) ? 1 : 0;
}

/* DOS 1:401e8; menu.c:1518 */
s16 change_couleur_prg(void) {
    s32 i, c;
    s16 done = 1;
    for (i = 0; i < 256; ++i) {
        rgb_t* rgb_cur = rvb_E40AC.colors + i;
        rgb_t* rgb_goal = rvb_menu_save.colors + i;
        for (c = 0; c < 3; ++c) {
            u8* cur = ((u8*)rgb_cur) + c;
            u8 goal = ((u8*)rgb_goal)[c];
            if (*cur == goal) {
                continue;
            }
            done = 0;
            if (*cur > goal + 2) {
                *cur -= 2;
            } else if (*cur + 2 < goal) {
                *cur += 2;
            } else {
                *cur = goal;
            }
        }
    }
    return done;
}

/* DOS 1:402fc; menu.c:1543 */
void FonduOption(s16 x, s16 y, s16 w, s16 h, u8 mode) {
    s32 i, j;
    u8* row;
    switch(mode) {
        case 1:
            EtapeFondu = 0;
            break;
        case 2:
            EtapeFondu = 1;
            InOut = 0;
            break;
        case 0:
            EtapeFondu = 32;
            InOut = 1;
            break;
    }
    xFondu = (x & ~3) + 4;
    yFondu = y;
    wFondu = (w & ~3) + 6;
    hFondu = h + 2;

    if (mode == 0) {
        if (GameModeVideo == MODE_NORMAL) {
            CaptureVideo(draw_buffer, EffetBufferNormal, 1);
            if (RayEvts.firefly) {
                set_xywhmap(TabW1[0], TabW2[0], TabH1[0], TabH2[0]);
                sprite_clipping(Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2);
            }
            DO_FADE_OUT();
            set_xywhmap(TabW1[SizeScreen], TabW2[SizeScreen], TabH1[SizeScreen], TabH2[SizeScreen]);
            sprite_clipping(Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2);
            N_CLRSCR(DrawBufferNormal);
        } else {
            CaptureVideo(display_buffer, EffetBufferNormal, 1);
            DO_FADE_OUT();
            InitModeNormalWithFrequency(VGA_FREQ);
            NewFrequency(Frequence);
        }

        if (
            (num_world == 1 && num_level == 9) ||
            (num_world == 2 && num_level == 4) ||
            (num_world == 4 && num_level == 9) ||
            (num_world == 5 && num_level == 4)
        ) {
            rvb[current_pal_id] = rvb_special[15];
        }

        InitMenuPalette();
        draw_buffer = DrawBufferNormal;
        DISPLAY_FOND_MENU();
    } else if (mode == 1) {
        for (i = 0; i < 256; ++i) {
            rgb_t rgb = MenuPalette.colors[i];
            u8 gray = ((rgb.r + rgb.g + rgb.b) >> 2) / 3 + 128;
            rvb_menu_save.colors[i] = MenuPalette.colors[gray];
        }
        rvb_E40AC = MenuPalette;
        SYNCHRO_LOOP_PALETTE(change_couleur_prg);

        // pixel remap to menu gray index
        row = DrawBufferNormal + SCREEN_WIDTH * yFondu + xFondu;
        for (j = 0; j < hFondu; ++j) {
            u8* pos = row;
            for (i = 0; i < wFondu; ++i) {
                u8 src = *pos;
                rgb_t rgb = MenuPalette.colors[src];
                u8 gray = ((rgb.r + rgb.g + rgb.b) >> 2) / 3 + 128;
                *pos++ = gray;
            }
            row += SCREEN_WIDTH;
        }

    }

    // Draw menu to offscreen area
    draw_buffer = EffetBufferNormal + SCREEN_WIDTH * SCREEN_HEIGHT;
    pAFFICHE_SCREEN();
    draw_buffer = DrawBufferNormal;

    if (mode == 1) {
        // pixel remap to menu gray index
        row = (EffetBufferNormal + SCREEN_WIDTH * SCREEN_HEIGHT) + SCREEN_WIDTH * yFondu + xFondu;
        for (j = 0; j < hFondu; ++j) {
            u8* pos = row;
            for (i = 0; i < wFondu; ++i) {
                u8 src = *pos;
                rgb_t rgb = MenuPalette.colors[src];
                u8 gray = ((rgb.r + rgb.g + rgb.b) >> 2) / 3 + 128;
                *pos++ = gray;
            }
            row += SCREEN_WIDTH;
        }
    }

    if (mode == 0) {
        rvb_menu_save = rvb[current_pal_id];
        rvb[current_pal_id] = MenuPalette;
        INIT_FADE_IN();
        SYNCHRO_LOOP(apparition_prg);
        rvb[current_pal_id] = rvb_menu_save;
    } else if (mode == 1) {
        SYNCHRO_LOOP(fondu_prg);
        for (i = 0; i < 256; ++i) {
            rgb_t rgb = MenuPalette.colors[i];
            u8 gray = ((rgb.r + rgb.g + rgb.b) >> 2) / 3 + 128;
            rvb_E40AC.colors[i] = MenuPalette.colors[gray];
        }
        rvb_menu_save = MenuPalette;
        pAFFICHE_SCREEN();
        SYNCHRO_LOOP_PALETTE(change_couleur_prg);

    } else if (mode == 2) {
        // NOTE: disabled this, because the palette seems to get messed up in DO_MENU(). I don't know why that does not
        // happen in the original game? Anyway, it still seems to work fine with the line disabled.
//        rvb_menu_save = rvb[current_pal_id];

        rvb[current_pal_id] = MenuPalette;
        SYNCHRO_LOOP(apparition_prg);
        if (GameModeVideo == MODE_X) {
            InitModeXWithFrequency(VGA_FREQ);
            NewFrequency(Frequence);
        }

        if (
            (num_world == 1 && num_level == 9) ||
            (num_world == 2 && num_level == 4) ||
            (num_world == 4 && num_level == 9) ||
            (num_world == 5 && num_level == 4)
        ) {
            rvb[current_pal_id] = rvb_special[numero_palette_special];
        } else {
            rvb[current_pal_id] = rvb_menu_save;
        }

        if (current_pal_id != 0) {
            rvb[0] = rvb_options_in_game;
        }
        INIT_FADE_IN();

    }
}

/* DOS 1:40a38; menu.c:1685 */
s16 FonduPixel_prg(u32 arg) {
    s32 line_offset = 320 - wFondu;
    s16 row = 0;
    s32 phase = 0;
    s16 column;
    u8 *src = DrawBufferNormal + (320 * yFondu) + line_offset;
    u8 *dst = EffetBufferNormal + 64000 + xFondu;

    while (row < hFondu) {
        column = 0;
        while (column < wFondu) {
            if (phase == EtapeFondu) {
                if (*src != *dst)
                    *src = *dst;
                phase = 0;
            }
            ++column;
            ++src;
            ++dst;
            ++phase;
        }
        ++row;
        src += line_offset;
        dst += line_offset;
    }
    --EtapeFondu;
    return EtapeFondu == 0;
}

/* DOS 1:40b0c; menu.c:1691 */
void FonduPixel(s16 x, s16 y, s16 w, s16 h) {
    xFondu = x;
    yFondu = y;
    wFondu = w;
    hFondu = h;
    draw_buffer = EffetBufferNormal + 64000;
    EtapeFondu = 16;
    pAFFICHE_SCREEN();
    draw_buffer = DrawBufferNormal;
    SYNCHRO_LOOP(FonduPixel_prg);
}

