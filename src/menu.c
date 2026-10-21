/* 1:39290..1:40b60: menu.c + misc_source.c (options/general menus, key config, fades, message boxes). Names RAYVERSE/ANDROID; seeded by tools/seed.py, worker tu_menu. INIT_GENERAL_CHOICE contains a DOS self-modifying trampoline (1:39ed1, OPEN). */
#include "obj.h"
#include "game.h"
#include "lowlevel.h"
#include "map.h"
#include "menu_local.h"
#include "menu_inline.h"
#include <string.h>
/* DOS 1:3fd62/3fe4b: intrinsic dword fill, not a memset_ call. */
#pragma intrinsic(memset)
#include <stdlib.h>
#include <conio.h>
#include <i86.h>

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
extern u8 WindowsLance;
extern s16 display_mode;
void set_vga_frequency(u8 frequency);
void InitPcSoundCard(void);
extern const char **key_descriptions;
#include "menu_keys_local.h"
extern s32 Bloc_lim_H1;
extern s32 Bloc_lim_H2;
extern s32 Bloc_lim_W1;
extern s32 Bloc_lim_W2;
extern u8 *DrawBufferNormal;
extern u8 *EffetBufferNormal;
extern void (*pAFFICHE_SCREEN)(void);
extern u8 GameModeVideo;
extern s16 SizeScreen;
extern s16 TabH1[4];
extern s16 TabH2[4];
extern s16 TabW1[4];
extern s16 TabW2[4];
extern u8 button_released;
extern u8 but_options[5];
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
extern s8 byte_49290[4];
extern u8 current_pal_id;
extern char *language_txt[300];
extern char title[100];
extern s16 max_compteur;
extern u8 msg_box_being_displayed;
extern s16 nbre_options;
extern s16 num_level;
extern s16 num_world;
extern u8 BackgroundOptionOn;
extern u8 BackgroundOn;
extern u8 ScrollDiffOn;
extern u16 fixon;
extern u8 GoMenu;
extern u8 byte_96884;
extern u8 dead_time;
extern u8 in_pause;
extern s32 MapTimePause;
extern s16 sortie_options;
extern s16 wFondu;
extern s16 xFondu;
extern s16 yFondu;
extern s16 ProchainEclair;
void FireBox(s16 x, s16 y, s16 width, s16 height);
void N_CLRSCR(u8* buffer);
void PlasmaBox(s16 x, s16 y, s16 width, s16 height, u8 a5);
void DISPLAY_BLACKBOX(s16 x, s16 y, s16 width, s16 height, s16 font_size, u8 is_fond);
void CadreTrans(s16 x, s16 y, s16 width, s16 height);
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
extern u8 ModeNormalAutorise;
extern u8 first_credit;
extern u8 Mode_Pad;
extern u8 Port_Pad;
extern u8 is_joypad_mode;
extern u8 something_with_cfg_25_default_00_joy;
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
extern u8 byte_E4CFF;
extern u8 byte_E4CFE;
extern s16 VideoSpeed;
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
extern u8 input_mode;
extern s16 joyx0;
extern s16 joyy0;
extern u8 language;
extern s16 loop_nb_trames;
extern s16 loop_timing;
extern mem_t main_mem_fix;
extern mem_t main_mem_level;
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
extern s16 notbut[4];
extern void (*pCOMMANDE_BOX)(void);
extern void (*pDO_COMMANDE)(void);
extern void (*pEND_SCREEN)(void);
extern void (*sEND_SCREEN)(void);
extern void (*pINIT_AFFICHE_SCREEN)(void);
extern void (*pINIT_SCREEN)(void);
extern void (*pINIT_SCROLL)(void);
extern void (*pLOAD_SCREEN)(void);
extern u8 pos_YN;
extern s16 position;
extern s16 action;
extern s16 positionx2;
extern s16 positiony2;
extern s16 positiony;
extern s16 repetition;
extern s16 joy_buttonA1;
extern rgb_palette_t rvb[3];
extern rgb_palette_t rvb_E40AC;
extern rgb_palette_t rvb_menu_save;
extern rgb_palette_t rvb_options_in_game;
extern rgb_palette_t rvb_plan3;
extern rgb_palette_t* rvb_special;
extern u16 tRefRam2VramNormalFix[2][4];
extern u16 tRefRam2VramNormal[2][4];
extern u16 tRefTransFondNormal[2][4];
extern u16 tRefSpriteNormal[2];
extern u16 tRefRam2VramX[2];
extern u16 tRefVram2VramX[2];
extern s16 tRefSpriteX[2];
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
void CaptureVideo(u8* source_buf, u8 grayscale);
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
void gauge(s16 x, s16 y, s16 width, s16 height, s16 minimum, s16 maximum, s16 value);
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
void SaveOptionsOnDisk(void);
void LoadLanguageTxt(mem_t* mem, s32 language_index);
extern u32 plan0bit_nb_bytes;
extern u32 plan0bit_length;
extern u8 *PLAN0BIT;
void sub_1268A(u8 *plan, s32 offset, s32 width);
#pragma aux sub_1268A parm [esi] [ebx] [edx]
bool LoadOptionsOnDisk(void);
void LoadPlan3InVignet(mem_t* mem, s32 resource_id);
void align_mem_desc(mem_t* desc);
void POINTEUR_BOUTONS_OPTIONS_BIS(void);
void PlaySnd_old(s16 sound_id);
void RESET_ALL_TOUCHE(void);
void RESET_TOUCHE(u8 key);
void SWAP_BUFFERS(void);
bool SelectButPressed(void);
void SetCompteurTrameAudio(void);
void SetDeviceID(s32 device_id);
s32 sub_3E780(void);
void SetDma(s32 dma);
void SetIrq(s32 irq);
void SetPalette(rgb_palette_t* palette, u8 first_color, s32 num_colors);
void SetParam(s32 param);
void SetPort_(s32 port);
void SetVolumeSound(s16 volume);
bool ValidButPressed(void);
void readinput_center_joystick(s16* xcentre, s16* ycentre);
void advance_frame(void);
s32 calc_largmax_text(const char* text, s16 char_index, s32 space_width, s32 char_spacing, u8 font_size);
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
void WaitNSynchro(s32 count);
void readinput_calibrate_joystick(s16* xmax, s16* xmin, s16* ymax, s16* ymin);
u8 TOUCHE(u8 key);
void update_pad_limits(s16* xmax, s16* xmin, s16* ymax, s16* ymin, s16* xcentre, s16* ycentre);
s16 upjoy(void);
s16 but0pressed(void);
s16 but1pressed(void);
s16 but2pressed(void);
s16 but3pressed(void);

/* In-range function prototypes, source-derived unless manually corrected. */
void check_key_table(void);
void LOAD_CONFIG(void);
void DO_NEW_MENUS(void);
void INIT_KEY_SCREEN(void);
void INIT_AFFICHE_KEY_SCREEN(void);
void AFFICHE_KEY_SCREEN(void);
void DO_COMMANDE_KEY(void);
void END_KEY_SCREEN(void);
void INIT_PAD_SCREEN(void);
void INIT_GRAPHIC_SCREEN(void);
void INIT_AFFICHE_GRAPHIC_SCREEN(void);
void AFFICHE_GRAPHIC_SCREEN(void);
void DO_COMMANDE_GRAPHIC(void);
void END_GRAPHIC_SCREEN(void);
void INIT_VIDEOMODE_SCREEN(void);
void INIT_AFFICHE_VIDEOMODE_SCREEN(void);
void AFFICHE_VIDEOMODE_SCREEN(void);
void DO_COMMANDE_VIDEOMODE(void);
void INIT_BUTTON_SCREEN(void);
void INIT_AFFICHE_BUTTON_SCREEN(void);
void AFFICHE_BUTTON_JOYSTICK(void);
void DO_BUTTON_COMMANDE(void);
void END_BUTTON_COMMAND(void);
void INIT_AFFICHE_PAD_SCREEN(void);
void AFFICHE_PAD_SCREEN(void);
void DO_COMMANDE_PAD(void);
void END_PAD_SCREEN(void);
void END_VIDEOMODE_SCREEN(void);
void INIT_CALIBRATE_JOYSTICK(s16* xmax, s16* xmin, s16* ymax, s16* ymin);
void AFFICHE_CALIBRATE_JOYSTICK(void);
s16 calibrate_joystick_prg(u32 a1);
s16 center_joystick_prg(u32 a1);
void DO_OPTIONS_IN_GAME(void);
void INIT_GENERAL_CHOICE(void);
void AFFICHE_ECRAN_GENERAL(void);
void DO_COMMANDE_GENERAL(void);
void END_GENERAL_SCREEN(void);
void INIT_OPTIONS_CHOICE(void);
void MAIN_CALIBRATE_JOYSTICK(void);
char *itoa_dos(s32 value, char *buffer, s32 radix);
s16 CalcSpeed(void);
void InitParamGraphicsScreen(void);
void INIT_AFFICHE_ECRAN_OPTIONS(void);
void AFFICHE_ECRAN_OPTIONS(void);
void AFFICHE_ECRAN_SAVE(void);
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
    s32 valid = 1;
    s32 i;
    for (i = 0; i < 7; ++i) {
        if (*tab_key[i] > SC_DELETE || (key_descriptions_qwerty[*tab_key[i]] == 0 && key_descriptions_azerty[*tab_key[i]] == 0))
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
        language = 0;
        LoadLanguageTxt(&main_mem_fix, 0); // English
        if ((s16)tRefRam2VramNormalFix[0][0] >= 128 && xpadmax == -1) {
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
        if (DeviceID != 999 && CarteSonAutorisee) {
            SetPort_((u16)Port);
            SetIrq(Irq);
            SetDma(Dma);
            SetParam(Param);
            SetDeviceID(DeviceID);
            if (sub_3E780()) {
                CarteSonAutorisee = 0;
            } else {
                LoadBnkFixe();
                CarteSonAutorisee = 1;
                InitSnd();
            }
        } else {
            SetDeviceID(DeviceID);
            CarteSonAutorisee = 0;
        }

        if (JoystickPresent()) {
            if (xpadmax < 0 || Mode_Pad != is_joypad_mode ||
                Port_Pad != something_with_cfg_25_default_00_joy ||
                (notbut[0] != 0 && notbut[0] != 1) ||
                (notbut[1] != 0 && notbut[1] != 1) ||
                (notbut[2] != 0 && notbut[2] != 1) ||
                (notbut[3] != 0 && notbut[3] != 1)) {
                pLOAD_SCREEN = LOAD_OPTIONS_SCREEN;
                CalcTab();
                DO_FADE_OUT();
                MAIN_CALIBRATE_JOYSTICK();
            } else {
                Main_Control = 2;
                update_pad_limits(&xpadmax, &xpadmin, &ypadmax, &ypadmin,
                                  &xpadcentre, &ypadcentre);
            }
        } else {
            if (xpadmax == -1) {
                xpadmax = -2;
            }
        }
    } else {
        // We didn't load a .cfg file -> load defaults
        if (CarteSonAutorisee) {
            CarteSonAutorisee = 0;
            LoadBnkFixe();
            InitSnd();
        }
        language = 0; // English
        LoadLanguageTxt(&main_mem_fix, 0);
        if (JoystickPresent()) {
            pLOAD_SCREEN = LOAD_OPTIONS_SCREEN;
            CalcTab();
            DO_FADE_OUT();
            MAIN_CALIBRATE_JOYSTICK();
        } else {
            xpadmax = -2;
        }
        options_jeu.music_enabled = 13;
        options_jeu.sound_volume = 18;
        options_jeu.is_stereo = 1;
        options_jeu.jump = 1;
        options_jeu.fist = 0;
        options_jeu.action = 2;
        raj_env_sound(options_jeu.sound_volume);
        GameModeVideo = 0;
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
    if (GameModeVideo == MODE_NORMAL) {
        if (is_fee || (num_world == world_6_cake && num_level == 4) || get_casse_brique_active()) {
            set_xywhmap(TabW1[0], TabW2[0], TabH1[0], TabH2[0]);
            sprite_clipping(Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2);
        } else {
            set_xywhmap(TabW1[SizeScreen], TabW2[SizeScreen], TabH1[SizeScreen], TabH2[SizeScreen]);
            sprite_clipping(Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2);
            N_CLRSCR(DrawBufferNormal);
        }
    } else {
        default_sprite_clipping();
    }
}
/* DOS 1:3980c; menu.c:223 */
/* DOS 1:3980c; menu.c:223 */
u8 IsBackgroundOn(void)
{
    return (num_world == 1 && num_level == 16) ||
        (num_world == 2 && num_level == 16) ||
        (num_world == 3 && num_level == 10) ||
        (num_world == 4 && num_level == num_world) ||
        (num_world == 4 && num_level == 11) ||
        (num_world == 5 && num_level == 3) ||
        (num_world == 5 && num_level == 11) ||
        (num_world == 6 && num_level == 4) || BackgroundOptionOn;
}

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
                } else {
                    pLOAD_SCREEN = NULL;
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
    SaveOptionsOnDisk();

}

/* DOS 1:39b7c; menu.c:320 */
void DO_OPTIONS_IN_GAME(void)
{
    u8 button_released_saved;
    u8 SizeScreen_saved;

    button_released_saved = button_released;
    SetCompteurTrameAudio();
    MENU_SUITE = 0;
    MENU_RETURN = 0;
    menuEtape = 4;
    byte_E4CFD = 1;
    if (GoMenu == 1) {
        if (byte_96884 == 64) {
            set_xywhmap(TabW1[0], TabW2[0], TabH1[0], TabH2[0]);
            sprite_clipping(Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2);
            ++GoMenu;
        } else {
            GoMenu = 0;
        }
    } else if (GoMenu == 2 || GameModeVideo == MODE_X) {
        if ((num_world == 6 && num_level == 4) || is_fee) {
            SizeScreen_saved = SizeScreen;
            SizeScreen = 0;
        }
        memcpy(&rvb_options_in_game, &rvb[0], 768);
        Keyflush();
        default_key();
        OptionGame = 1;
        GoMenu = 0;
        stop_cd();
        cdTime = 0;
        if (CarteSonAutorisee) {
            start_freeze_snd();
        }
        while (menuEtape != 0) {
            raj_env_sound(options_jeu.sound_volume);
            raj_env_stereo(options_jeu.is_stereo);
            raj_env_audio(options_jeu.music_enabled);
            RESET_ALL_TOUCHE();
            switch (menuEtape) {
            case 4:
                pINIT_SCREEN = INIT_OPTIONS_CHOICE;
                pLOAD_SCREEN = NULL;
                if (MENU_RETURN) {
                    menuEtape = 4;
                }
                break;
            case 7:
                pINIT_SCREEN = INIT_KEY_SCREEN;
                if (MENU_RETURN) {
                    menuEtape = 4;
                }
                break;
            case 8:
                pINIT_SCREEN = INIT_PAD_SCREEN;
                if (MENU_RETURN) {
                    menuEtape = 4;
                }
                break;
            case 9:
                pINIT_SCREEN = INIT_GRAPHIC_SCREEN;
                if (MENU_RETURN) {
                    menuEtape = 4;
                }
                break;
            default:
                break;
            }
            DO_MENU();
            MENU_RETURN = 0;
        }
        OptionGame = 0;
            new_key();
            if (GameModeVideo == MODE_NORMAL) {
                set_xywhmap(TabW1[SizeScreen], TabW2[SizeScreen], TabH1[SizeScreen], TabH2[SizeScreen]);
                sprite_clipping(Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2);
                N_CLRSCR(DrawBufferNormal);
            } else if (P486 == 1) {
                sub_1268A(PLAN0BIT, plan0bit_nb_bytes, plan0bit_length >> 2);
            }
            if (CarteSonAutorisee) {
                stop_freeze_snd();
            }
            in_pause = 0;
            ProchainEclair = 1;
            numero_palette_special = 0;
            MapTimePause = 0;
            if ((num_world == 6 && num_level == 4) || is_fee) {
                SizeScreen = SizeScreen_saved;
            }
    }
    byte_96884 = dead_time;
    button_released = button_released_saved;
}

/* DOS 1:39e98; menu.c:426 */
void INIT_GENERAL_CHOICE(void) {
    u8 color_table[4] = {0, 0, 0, 0};
    u8 i;
    /* OPEN: DOS 1:39eb7..39edf self-modifying trampoline is not expressible
       under the permitted inline-instruction rule; no replacement bytes added. */
    pINIT_AFFICHE_SCREEN = NULL;
    pINIT_SCROLL = NULL;
    pDO_COMMANDE = DO_COMMANDE_GENERAL;
    pEND_SCREEN = END_GENERAL_SCREEN;
    pAFFICHE_SCREEN = AFFICHE_ECRAN_GENERAL;
    first_option = 0;
    option_exit = 3;
    nbre_options = 4;
    basex = 160;
    for (i = 0; i < nbre_options; ++i) {
        strcpy(menu_to_display[i].text, language_txt[190 + i]); // "music"
        menu_to_display[i].color = color_table[i];
        switch(i) {
            default: break;
            case 0: {
                strcpy(menu_to_display[i].text, language_txt[109]); // "/start/"
                menu_to_display[i].ypos = 30;
            } break;
            case 1: {
                strcpy(menu_to_display[i].text, language_txt[181]); // "/options/"
                menu_to_display[i].ypos = 60;
            } break;
            case 2: {
                strcpy(menu_to_display[i].text, language_txt[110]); // "/credits/"
                menu_to_display[i].ypos = 155;
            } break;
            case 3: {
                strcpy(menu_to_display[i].text, language_txt[154]); // "/exit/"
                menu_to_display[i].ypos = 185;
            } break;
        }
        menu_to_display[i].xpos = basex;
        menu_to_display[i].font_size = 1;
        menu_to_display[i].is_fond = 1;
        menu_to_display[i].field_D5 = 0;
        INIT_TXT_BOX(&menu_to_display[i]);
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
    u8 i;
    DISPLAY_FOND_MENU();
    for (i = 0; i < nbre_options; ++i) {
        if (i == positiony) {
            display_box_text_plasma(menu_to_display + i, 1);
        } else {
            display_text(menu_to_display[i].text, menu_to_display[i].xpos, menu_to_display[i].ypos, menu_to_display[i].font_size, menu_to_display[i].color);
        }
    }
}

/* DOS 1:3a178; menu.c:490 */
void DO_COMMANDE_GENERAL(void) {
    TestCompteur();
    ++TempsDemo;
    if (TempsDemo >= (u16)RunTimeDemo && !fin_du_jeu) {
        InitDemoJeu();
    }
    if (SelectButPressed()) {
        RESET_TOUCHE(SC_ESCAPE);
        if (confirmation_msg(0)) {
            MENU_RETURN = 1;
        }
    }
    if (upjoy() && (button_released || (compteur > delai_repetition && compteur % repetition == 0))) {
        PlaySnd_old(68);
        if (positiony > first_option) {
            --positiony;
        } else {
            positiony = option_exit;
        }
        TempsDemo = 0;
    }
    if (downjoy() && (button_released || (compteur > delai_repetition && compteur % repetition == 0))) {
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
                loop_timing = 4;
                LoadPlan3InVignet(&main_mem_level, 61);
                INIT_TEXT_TO_DISPLAY();
                DO_FADE_OUT();
                INIT_FADE_IN();
                DISPLAY_CREDITS_MENU();
                align_mem_desc(&main_mem_level);
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
            endsynchro();
            synchro();
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
    if (!WindowsLance && (menu_read_dr0() != 0 || menu_read_dr1() != 0 ||
                         menu_read_dr2() != 0 || menu_read_dr3() != 0)) {
        menu_dos_interrupt(0x4c00);
    }
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
    switch (language) {
    case 0: byte_E4CFC = 75; break;
    case 1: byte_E4CFC = 90; break;
    case 2: byte_E4CFC = 90; break;
    }
    basex = 160 - byte_E4CFC;
    if (OptionGame) {
        position = 6;
        option_exit = 6;
        nbre_options = 6;
        first_option = 0;
    } else {
        position = 5;
        option_exit = 5;
        nbre_options = 5;
        first_option = 0;
    }
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
    u8 v28[9] = {5, 5, 5, 5, 5, 5, 3, 0}; //0x5050505;
    char v39[3] = "on";
    char v32[4] = "off";
//    u8 v29[4] = {5, 5, 3, 0}; // 0x30505; //dword_49294[1]
    s16 i;
    menu_to_display[9].is_fond = 1;
    menu_to_display[9].font_size = 2;
    menu_to_display[9].field_D5 = 0;
    menu_to_display[9].color = 1;
    for (i = first_option; i <= first_option + nbre_options; ++i) {

        menu_to_display[i].font_size = 2;
        menu_to_display[i].field_D5 = 0;
        menu_to_display[i].is_fond = 1;
        if (i == option_exit) {
            strcpy(menu_to_display[i].text, language_txt[232]); // /exit/
            menu_to_display[i].xpos = (SCREEN_WIDTH / 2);
            menu_to_display[i].ypos = debut_sortie;
            menu_to_display[i].color = v28[6];
        } else {
            menu_to_display[i].color = v28[i];
            strcpy(menu_to_display[i].text, language_txt[190 + i]); // { "music", "sound", "/setup keys/", "/graphics details/", "/quit game/" }
            switch(i) {
                case 0: {
                    // Text "music" offsetted to the left with "on off" to the right
                    s32 text_width = calc_largmax_text(menu_to_display[i].text, -1, 8, 1, 2) + 16 +
                                     calc_largmax_text(v39, -1, 8, 1, 2) + calc_largmax_text(v32, -1, 8, 1, 2);
                    menu_to_display[i].xpos = (SCREEN_WIDTH / 2) - (text_width >> 1);
                } break;
                case 1: {
                    // Text "sound" offsetted to the left
                    s32 text_width = calc_largmax_text(menu_to_display[i].text, -1, 8, 1, 2) + 16 +
                             20 * calc_largmax_text("|", -1, 8, 1, 2);
                    menu_to_display[i].xpos = (SCREEN_WIDTH / 2) - (text_width >> 1);
                } break;
                case 2:
                case 3:
                case 4:
                case 5:
                    // Text centered
                    menu_to_display[i].xpos = (SCREEN_WIDTH / 2);
                    break;
                default:
                    break;
            }
            menu_to_display[i].ypos = i * (ecart_options + 15) + debut_options;
        }
        INIT_TXT_BOX((&menu_to_display[i]));
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
void AFFICHE_ECRAN_OPTIONS(void)
{
    u8 option_index;
    u8 i;
    s32 v5;
    s32 v6;
    s32 v18;
    s32 v8;
    s32 draw_x;
    s16 x;
    s16 y;
    u8 color;
    char on_text[3] = "on";
    char off_text[4] = "off";
    const char *nonselected_text;
    const char *other_text;
    display_item_t *to_display;

    DISPLAY_FOND_MENU();
    if (OptionGame) {
        CadreTrans(word_E4CEC_x, word_E4CF0_y, word_E4CF4_w, word_E4CEE_h);
    }
    display_text(language_txt[181], SCREEN_WIDTH / 2, debut_titre, 1, 1);
    for (option_index = first_option; option_index <= first_option + nbre_options; ++option_index) {
        x = menu_to_display[option_index].xpos;
        y = menu_to_display[option_index].ypos;
        if (option_index == position) {
            display_box_text_plasma(menu_to_display + option_index, 1);
        } else if (option_index == option_exit) {
            display_text(language_txt[232], x, y, 2, menu_to_display[option_index].color);
        } else {
            display_text(language_txt[190 + option_index], x, y, 2, menu_to_display[option_index].color);
        }
        if (option_index == 0) {
            v5 = calc_largmax_text(menu_to_display[0].text, -1, 8, 1, 2);
            v6 = v5 + x;
            if (options.music_enabled) {
                nonselected_text = off_text;
                v18 = v6 + calc_largmax_text(on_text, -1, 8, 1, 2) + 32;
                v8 = v6 + 16;
                other_text = on_text;
            } else {
                nonselected_text = on_text;
                v18 = v6 + 16;
                v8 = v6 + calc_largmax_text(on_text, -1, 8, 1, 2) + 32;
                other_text = off_text;
            }
            display_text(nonselected_text, v18, y, 2, 5);
            if (position == option_index) {
                to_display = text_to_display;
                strcpy(to_display->text, other_text);
                to_display->xpos = v8;
                to_display->ypos = menu_to_display[0].ypos;
                INIT_TXT_BOX(to_display);
                display_box_text_plasma(to_display, 0);
            } else {
                display_text(other_text, v8, y, 2, 5);
            }
        } else if (option_index == 1) {
            draw_x = x + calc_largmax_text(menu_to_display[1].text, -1, 8, 1, 2);
            draw_x += 16;
            DrawBlackBorderBox(draw_x, y - 14, 15, 20 * ecart_barre + 1, 1);
            for (i = 0; i < options.sound_volume; ++i) {
                if (i >= (2 * max_sound) / 3) {
                    color = 1;
                } else if (i >= max_sound / 3) {
                    color = 3;
                } else {
                    color = 5;
                }
                display_text("|", draw_x + ecart_barre * i,
                             debut_options + ecart_options + 15, 2, color);
            }
        }
    }
}

/* DOS 1:3af28; menu.c:803 */
void DO_COMMANDE_OPTIONS(void) {
    TestCompteur();
    if (SelectButPressed()) {
        MENU_RETURN = 1;
    }
    if (upjoy() && !rightjoy() && !leftjoy() &&
            (button_released || (compteur > delai_repetition && (compteur % repetition) == 0))
    ) {
        PlaySnd_old(68);
        if (position == first_option) {
            position = option_exit;
        } else {
            --position;
        }
    }
    if (downjoy() && !rightjoy() && !leftjoy() &&
        (button_released || (compteur > delai_repetition && (compteur % repetition) == 0))
    ) {
        PlaySnd_old(68);
        if (position == option_exit) {
            position = first_option;
        } else {
            ++position;
        }
    }
    if (position == 0 && button_released && (ValidButPressed() || rightjoy() || leftjoy())) {
        ((s16*)&options)[position] = !((s16*)&options)[position];
        if (((s16*)&options)[position]) {
            SetCompteurTrameAudio();
            cdTime = 0;
            if (OptionMusicCdActive) {
                MusicCdActive = 1;
            }
        } else {
            SetCompteurTrameAudio();
            stop_cd();
            if (OptionMusicCdActive) {
                MusicCdActive = 0;
            }
        }
    }
    if (position == 1 && rightjoy()) {
        if (options.sound_volume < max_sound) {
            if (button_released || (compteur > delai_repetition && (compteur % delai_barre) == 0)) {
                PlaySnd_old(68);
                ++options.sound_volume;
                raj_env_sound(options.sound_volume);
            }
        }
    }
    if (position == 1 && leftjoy()) {
        if (options.sound_volume > 0) {
            if (button_released || (compteur > delai_repetition && (compteur % delai_barre) == 0)) {
                PlaySnd_old(68);
                --options.sound_volume;
                raj_env_sound(options.sound_volume);
            }
        }
    }
    if (position == 3 && ValidButPressed()) {
        if (JoystickPresent()) {
            byte_E4CD9 = options.sound_volume;
            byte_E4CD8 = options.music_enabled;
            byte_96883 = 1;
            menuEtape = 8; // go to setup pad
            sortie_options = 1;
        } else {
            display_box_msg(&msg_to_display[0]); // no joystick connected
        }
    } else if (position == 2 && ValidButPressed()) {
        byte_E4CD9 = options.sound_volume;
        byte_E4CD8 = options.music_enabled;
        byte_96883 = 1;
        menuEtape = 7; // go to setup keys
        sortie_options = 1;
        TestButtonReleased();
        return;
    } else if (position == 4 && ValidButPressed()) {
        byte_E4CD9 = options.sound_volume;
        byte_E4CD8 = options.music_enabled;
        byte_96883 = 1;
        menuEtape = 9; // go to graphics details
        sortie_options = 1;
        TestButtonReleased();
        return;
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
void INIT_KEY_SCREEN(void) {
    s16 i;
    if (language == 1) {
        key_descriptions = key_descriptions_azerty;
    } else {
        key_descriptions = key_descriptions_qwerty;
    }
    set_special_key_descriptions();
    pINIT_AFFICHE_SCREEN = INIT_AFFICHE_KEY_SCREEN;
    pAFFICHE_SCREEN = AFFICHE_KEY_SCREEN;
    pDO_COMMANDE = DO_COMMANDE_KEY;
    pEND_SCREEN = END_KEY_SCREEN;
    position = 8;
    nbre_options = 8;
    option_exit = 8;
    for (i = 0; (s32)i < 7; ++i) {
        ((s16*)&options)[i] = tab_key_sav[i];
    }
    byte_E4CFF = 0;
    compteur = 0;
    max_compteur = 100;
    delai_repetition = 12;
    button_released = 1;
    repetition = 6;
    switch (language) {
    case 0: byte_E4CFC = 90; break;
    case 1: byte_E4CFC = 105; break;
    case 2: byte_E4CFC = 120; break;
    }
    basex = 160 - byte_E4CFC;
    general_init_screen(15, 68, 5);
}

/* DOS 1:3b514; menu.c:916 */
void INIT_AFFICHE_KEY_SCREEN(void) {
    s16 i;
    u8 language_offset;
    s16 new_x, new_y, new_w, new_h;
    s16 left, top, right, bottom, width, height;
    u8 color[10] = {5, 5, 5, 5, 5, 5, 5, 5, 3, 0};

    switch (language) {
    case 0:
        language_offset = 20;
        break;
    case 1:
        language_offset = 25;
        break;
    case 2:
        language_offset = 45;
        break;
    }

    strcpy(title + 1, language_txt[233]);
    i = (s16)strlen(language_txt[233]);
    title[i + 1] = '/';
    title[i + 2] = 0;

    for (i = 0; i <= nbre_options; ++i) {

        if (i == 8) {
            strcpy(menu_to_display[i].text, language_txt[232]);
            menu_to_display[i].xpos = 160;
            menu_to_display[i].ypos = debut_sortie;
        } else {
            strcpy(menu_to_display[i].text, language_txt[182 + i]);
            if (i == 7) {
                menu_to_display[i].xpos = 160;
            } else {
                menu_to_display[i].xpos = (s16)(basex + language_offset);
            }
            menu_to_display[i].ypos = (s16)(i * (ecart_options + 15) + debut_options);
        }
        menu_to_display[i].color = (s8)color[i];
        menu_to_display[i].font_size = 2;
        menu_to_display[i].field_D5 = 0;
        menu_to_display[i].is_fond = 1;
        INIT_TXT_BOX((&menu_to_display[i]));
    }

    text_to_display[0].is_fond = 1;
    text_to_display[0].xpos = (s16)(debut_titre + 80);
    text_to_display[0].field_D5 = 0;
    text_to_display[0].font_size = 2;

    msg_to_display[0].xpos = 160;
    msg_to_display[0].ypos = 100;
    msg_to_display[0].font_size = 2;
    msg_to_display[0].field_D5 = 0;
    msg_to_display[0].is_fond = 1;
    strcpy(msg_to_display[0].text, language_txt[222]);
    INIT_TXT_BOX(&msg_to_display[0]);

    new_x = (s16)(basex - 2);
    new_y = (s16)(debut_titre - 25);
    new_w = (s16)(byte_E4CFC * 2);
    new_h = (s16)(menu_to_display[8].ypos - debut_titre + 35);

    left = (word_E4CEC_x < new_x) ? word_E4CEC_x : new_x;
    top = (word_E4CF0_y < new_y) ? word_E4CF0_y : new_y;
    right = ((s16)(word_E4CEC_x + word_E4CF4_w) > (s16)(new_x + new_w)) ?
            (s16)(word_E4CEC_x + word_E4CF4_w) : (s16)(new_x + new_w);
    bottom = ((s16)(word_E4CF0_y + word_E4CEE_h) > (s16)(new_y + new_h)) ?
             (s16)(word_E4CF0_y + word_E4CEE_h) : (s16)(new_y + new_h);
    width = (s16)(right - left);
    height = (s16)(bottom - top);

    word_E4CEC_x = left;
    word_E4CF0_y = top;
    word_E4CF4_w = width;
    word_E4CEE_h = height;
    if (OptionGame) {
        FonduOption(left, top, width, height, 1);
    } else {
        FonduPixel(left, top, width, height);
    }
}

/* DOS 1:3b8a4; menu.c:921 */
void AFFICHE_KEY_SCREEN(void) {
    s16 xpos, ypos;
    u8 i;
    u8 language_offset;

    switch (language) {
    case 0:
        language_offset = 62;
        break;
    case 1:
        language_offset = 70;
        break;
    case 2:
        language_offset = 75;
        break;
    }

    DISPLAY_FOND_MENU();
    if (OptionGame) {
        CadreTrans(word_E4CEC_x, word_E4CF0_y, word_E4CF4_w, word_E4CEE_h);
    }
    display_text(title, 160, debut_titre, 1, 1);

    for (i = 0; i <= nbre_options; ++i) {

        xpos = menu_to_display[i].xpos;
        ypos = menu_to_display[i].ypos;
        if (i != position) {
            display_text(menu_to_display[i].text, xpos, ypos, 2, menu_to_display[i].color);
        } else {
            display_box_text_plasma(&menu_to_display[position], 1);
        }

        if (i < nbre_options - 1) {
            if (byte_E4CFF && i == position) {
                strcpy(text_to_display[0].text, key_descriptions[((s16*)&options)[position]]);
                text_to_display[0].color = menu_to_display[position].color;
                text_to_display[0].ypos = menu_to_display[position].ypos;
                text_to_display[0].xpos = (s16)(xpos + language_offset);
                INIT_TXT_BOX(&text_to_display[0]);
                display_box_text_plasma(&text_to_display[0], 0);
            } else {
                display_text(key_descriptions[((s16*)&options)[i]], (s16)(xpos + language_offset), menu_to_display[i].ypos, 2, menu_to_display[i].color);
            }
        }
    }
}

/* DOS 1:3bacc; menu.c:926 */
void DO_COMMANDE_KEY(void) {
    s16 i;
    s16 j;
    s16 found_duplicate;

    TestCompteur();
    if (!byte_E4CFF) {
        if (SelectButPressed()) {
            MENU_RETURN = 1;
        }

        if (!rightjoy() && !leftjoy() &&
                (button_released || (compteur > delai_repetition && compteur % repetition == 0))) {
            if (upjoy()) {
                PlaySnd_old(0x44);
                if (position == 0) {
                    position = 8;
                } else {
                    --position;
                }
            } else if (downjoy()) {
                PlaySnd_old(0x44);
                if (position == 8) {
                    position = 0;
                } else {
                    ++position;
                }
            }
        }

        if (position == 7) {
            if (ValidButPressed() && button_released) {
                ((s16*)&options)[0] = SC_LEFT;
                ((s16*)&options)[1] = SC_UP;
                ((s16*)&options)[2] = SC_RIGHT;
                ((s16*)&options)[3] = SC_DOWN;
                ((s16*)&options)[4] = SC_CONTROL;
                ((s16*)&options)[5] = SC_ALT;
                ((s16*)&options)[6] = SC_X;
            }
        } else if (position >= 0 && position <= 6) {
            if (ValidButPressed() && button_released) {
                byte_E4CFF = 1;
                RESET_ALL_TOUCHE();
                TestButtonReleased();
                return;
            }
        } else if (position == 8 && ValidButPressed()) {
            found_duplicate = 0;
            for (i = 0; i < nbre_options && !found_duplicate; ++i) {
                for (j = i + 1; j < nbre_options; ++j) {
                    if (((s16*)&options)[i] == ((s16*)&options)[j]) {
                        found_duplicate = 1;
                        break;
                    }
                }
            }
            if (found_duplicate) {
                display_box_msg(&msg_to_display[0]);
                TestButtonReleased();
                return;
            }
        }

        TestButtonReleased();
    } else {
        for (i = 0; i < 128; ++i) {
            if (TOUCHE((u8)i) && key_descriptions[i] != NULL) {
                byte_E4CFF = 0;
                ((s16*)&options)[position] = i;
                RESET_ALL_TOUCHE();
            }
        }
        TestButtonReleased();
    }
}

/* DOS 1:3bd50; menu.c:931 */
void END_KEY_SCREEN(void) {
    s16 i;
    menuEtape = 4;
    if (!MENU_RETURN) {
        for (i = 0; i < nbre_options - 1; ++i) {
            tab_key_sav[i] = ((u8*)&options)[i * 2];
        }
    } else {
        byte_96883 = 0;
        if (!OptionGame) {
            DO_FADE_OUT();
        }
    }
}

/* DOS 1:3bda4; menu.c:936 */
void INIT_BUTTON_SCREEN(void) {
    pINIT_SCREEN = INIT_BUTTON_SCREEN;
    pINIT_AFFICHE_SCREEN = INIT_AFFICHE_BUTTON_SCREEN;
    pAFFICHE_SCREEN = AFFICHE_BUTTON_JOYSTICK;
    pDO_COMMANDE = DO_BUTTON_COMMANDE;
    pEND_SCREEN = END_BUTTON_COMMAND;
    notbut[3] = 0;
    notbut[2] = 0;
    notbut[1] = 0;
    notbut[0] = 0;
}

/* DOS 1:3be04; menu.c:941 */
void INIT_AFFICHE_BUTTON_SCREEN(void) {
    s32 i;
    u8 color[5] = {5, 5, 5, 5, 3};
    compteur = 0;
    button_released = 1;
    position = 0;
    first_option = 0;
    delai_repetition = 12;
    max_compteur = 100;
    repetition = 6;
    nbre_options = 4;
    option_exit = 4;
    for (i = 0; i <= 4; ++i) {
        but_options[i] = 0;
        menu_to_display[i].font_size = 2;
        menu_to_display[i].is_fond = 1;
        menu_to_display[i].field_D5 = 0;
        if (i == 4) {
            menu_to_display[i].ypos = 155;
            menu_to_display[i].xpos = 160;
            strcpy(menu_to_display[i].text, language_txt[232]);
            menu_to_display[i].color = color[4];
        } else {
            menu_to_display[i].xpos = 120;
            strcpy(menu_to_display[i].text, language_txt[226 + i]);
            menu_to_display[i].color = color[i];
            menu_to_display[i].ypos = (s16)(70 + i * 20);
        }
        INIT_TXT_BOX(&menu_to_display[i]);
    }
}

/* DOS 1:3bf60; menu.c:946 */
void AFFICHE_BUTTON_JOYSTICK(void) {
    s16 x, y;
    u8 color[5] = {5, 5, 5, 5, 3};

    DISPLAY_FOND_MENU();
    if (OptionGame) {
        CadreTrans(word_E4CEC_x, word_E4CF0_y, word_E4CF4_w, word_E4CEE_h);
    }
    display_box_text_plasma(&text_to_display[0], 1);

    {
    u8 i;
    for (i = 0; i <= 4; ++i) {
        x = menu_to_display[i].xpos;
        y = menu_to_display[i].ypos;

        if (i < 4) {
            if (but_options[i]) {
                PlasmaBox(192, 59 + 20 * i, 34, 9, 0);
            }
            if (notbut[i] == 2) {
                DISPLAY_BLACKBOX(188, 55 + 20 * i, 40, 16, 0, 2);
                display_text("ok", 233, y, 2, color[i]);
            } else {
                DISPLAY_BLACKBOX(188, 55 + 20 * i, 40, 16, 0, 1);
            }
        }

        if (i != position) {
            display_text(menu_to_display[i].text, x, y, 2, color[i]);
        } else {
            menu_to_display[i].color = color[i];
            INIT_TXT_BOX(&menu_to_display[i]);
            display_box_text_plasma(&menu_to_display[i], 0);
        }
    }

    }
    display_text(language_txt[230], 160, 175, 2, 5);
}

/* DOS 1:3c138; menu.c:951 */
void DO_BUTTON_COMMANDE(void) {
    TestCompteur();

    if (joy_buttonA1 == 1) {
        if (notbut[0] == 0) {
            if (position == option_exit) {
                joy_buttonA1 = notbut[0];
            }
        }
    }

    if (SelectButPressed()) {
        MENU_RETURN = 1;
    }

    if (button_released || (compteur > delai_repetition && compteur % repetition == 0)) {
                if (upjoy()) {
                    PlaySnd_old(0x44);
                    if (position == first_option) {
                        position = option_exit;
                    } else {
                        --position;
                    }
                } else if (downjoy()) {
                    PlaySnd_old(0x44);
                    if (position == option_exit) {
                        position = first_option;
                    } else {
                        ++position;
                    }
                }
    }

    input_mode = 0;
    if (but0pressed() && position != option_exit) {
        if (but_options[0] == 0) {
            PlaySnd_old(0x44);
        }
        but_options[0] = 1;
    } else {
        if (but_options[0] != 0) {
            notbut[0] = 2;
        }
        but_options[0] = 0;
    }

    if (but1pressed() && position != option_exit) {
        if (but_options[1] == 0) {
            PlaySnd_old(0x44);
        }
        but_options[1] = 1;
    } else {
        if (but_options[1] != 0) {
            notbut[1] = 2;
        }
        but_options[1] = 0;
    }

    if (but2pressed() && position != option_exit) {
        if (but_options[2] == 0) {
            PlaySnd_old(0x44);
        }
        but_options[2] = 1;
    } else {
        if (but_options[2] != 0) {
            notbut[2] = 2;
        }
        but_options[2] = 0;
    }

    if (but3pressed() && position != option_exit) {
        if (but_options[3] == 0) {
            PlaySnd_old(0x44);
        }
        but_options[3] = 1;
    } else {
        if (but_options[3] != 0) {
            notbut[3] = 2;
        }
        but_options[3] = 0;
    }

    input_mode = 1;
    TestButtonReleased();
}

/* DOS 1:3c390; menu.c:956 */
void END_BUTTON_COMMAND(void) {
    s16 i;
    for (i = 0; i < 4; ++i) {
        notbut[i] = notbut[i] == 0;
    }
    MENU_RETURN = 0;
    sortie_options = 0;
    pEND_SCREEN = NULL;
}

/* DOS 1:3c3d0; menu.c:961 */
void INIT_PAD_SCREEN(void) {
    pINIT_AFFICHE_SCREEN = INIT_AFFICHE_PAD_SCREEN;
    pAFFICHE_SCREEN = AFFICHE_PAD_SCREEN;
    pDO_COMMANDE = DO_COMMANDE_PAD;
    pEND_SCREEN = END_PAD_SCREEN;
    options.sound_volume = options_jeu.jump;
    options.SizeScreen = options_jeu.fist;
    options.Frequence = options_jeu.action;
    position = 5;
    nbre_options = 5;
    option_exit = 5;
    compteur = 0;
    first_option = 0;
    button_released = 1;
    max_compteur = 100;
    delai_repetition = 12;
    repetition = 6;
    switch (language) {
    case 0: byte_E4CFC = 65; break;
    case 1: byte_E4CFC = 105; break;
    case 2: byte_E4CFC = 105; break;
    }
    basex = 160 - byte_E4CFC;
    general_init_screen(20, 60, 5);
}

/* DOS 1:3c4d0; menu.c:966 */
void INIT_AFFICHE_PAD_SCREEN(void) {
    u8 i;
    s16 row;
    s16 text_width;
    s16 letter_width;
    s16 old_x, old_y, old_w, old_h;
    s16 new_x, new_y, new_w, new_h;
    s16 left, top, right, bottom, width, height;
    u8 first;
    u8 color[6] = {5, 5, 5, 5, 5, 3};

    strcpy(title, language_txt[234]);
    first = first_option;
    for (i = first; i <= first + nbre_options; ++i) {

        row = i - first;
        menu_to_display[i].color = (s8)color[row];
        menu_to_display[i].ypos = (s16)(row * (ecart_options + 15) + debut_options);
        if (i == option_exit) {
            strcpy(menu_to_display[i].text, language_txt[232]);
            menu_to_display[i].xpos = 160;
            menu_to_display[i].ypos = debut_sortie;
        } else if (i == 0) {
            strcpy(menu_to_display[i].text, language_txt[208]);
            menu_to_display[i].xpos = 160;
        } else {
            strcpy(menu_to_display[i].text, language_txt[185 + i]);
            if (first == 4) {
                menu_to_display[i].xpos = 160;
            } else {
                text_width = calc_largmax_text(menu_to_display[i].text, -1, 8, 1, 2);
                letter_width = calc_largmax_text("a", -1, 8, 1, 2);
                menu_to_display[i].xpos = (s16)(160 - ((text_width + letter_width + 16) / 2));
            }
        }
        menu_to_display[i].font_size = 2;
        menu_to_display[i].field_D5 = 0;
        menu_to_display[i].is_fond = 1;
        INIT_TXT_BOX((&menu_to_display[i]));
    }

    new_x = basex;
    new_y = (s16)(debut_titre - 25);
    new_w = (s16)(byte_E4CFC * 2);
    new_h = (s16)(menu_to_display[option_exit].ypos - debut_titre + 35);

    left = (word_E4CEC_x < new_x) ? word_E4CEC_x : new_x;
    top = (word_E4CF0_y < new_y) ? word_E4CF0_y : new_y;
    right = ((s16)(word_E4CEC_x + word_E4CF4_w) > (s16)(new_x + new_w)) ?
            (s16)(word_E4CEC_x + word_E4CF4_w) : (s16)(new_x + new_w);
    bottom = ((s16)(word_E4CF0_y + word_E4CEE_h) > (s16)(new_y + new_h)) ?
             (s16)(word_E4CF0_y + word_E4CEE_h) : (s16)(new_y + new_h);
    width = (s16)(right - left);
    height = (s16)(bottom - top);
    word_E4CEC_x = left;
    word_E4CF0_y = top;
    word_E4CF4_w = width;
    word_E4CEE_h = height;

    if (OptionGame) {
        FonduOption(left, top, width, height, 1);
    } else {
        FonduPixel(left, top, width, height);
    }
}

/* DOS 1:3c854; menu.c:971 */
void AFFICHE_PAD_SCREEN(void) {
    u8 i;
    s16 x, y;
    const char* button_labels[4] = {"a/", "b/", "c/", "d/"};

    DISPLAY_FOND_MENU();
    if (OptionGame) {
        CadreTrans(word_E4CEC_x, word_E4CF0_y, word_E4CF4_w, word_E4CEE_h);
    }
    display_text(title, 160, debut_titre, 1, 1);

    for (i = first_option; i <= first_option + nbre_options; ++i) {

        x = menu_to_display[i].xpos;
        y = menu_to_display[i].ypos;
        if (i != position) {
            display_text(menu_to_display[i].text, x, y, 2, menu_to_display[i].color);
        } else {
            display_box_text_plasma(&menu_to_display[position], 1);
        }

        if (i > 0 && i < 4) {
            s32 label_width = calc_largmax_text(menu_to_display[i].text, -1, 8, 1, 2);
            const char* button_label = button_labels[((s16*)&options)[i]];
            display_text(button_label, (s16)(x + (label_width + 16)),
                         y, 2, menu_to_display[i].color);
        }
    }
}

/* DOS 1:3c9d8; menu.c:976 */
void DO_COMMANDE_PAD(void) {
    u8 selected_button;
    u8 button_index;
    s16 old_button;

    TestCompteur();
    selected_button = 4;
    if (SelectButPressed()) {
        MENU_RETURN = 1;
    }

    if (!button_released &&
        (compteur <= delai_repetition || compteur % repetition != 0)) {
        goto pad_actions;
    }
    if (upjoy()) {
        PlaySnd_old(0x44);
        if (position == first_option) {
            position = option_exit;
        } else {
            --position;
        }
    } else if (downjoy()) {
        PlaySnd_old(0x44);
        if (position == option_exit) {
            position = first_option;
        } else {
            ++position;
        }
    }

pad_actions:
    if (position == 0 && ValidButPressed()) {
        input_mode = 0;
        if (!OptionGame) {
            DO_FADE_OUT();
        }
        MAIN_CALIBRATE_JOYSTICK();
        if (Main_Control == 0) {
            Mode_Pad = 0xff;
            is_joypad_mode = 0xff;
            xpadmax = -1;
            xpadmin = -1;
            ypadmax = -1;
            ypadmin = -1;
            xpadcentre = -1;
            ypadcentre = -1;
            notbut[0] = -1;
            notbut[1] = -1;
            notbut[2] = -1;
            notbut[3] = -1;
            sortie_options = 1;
            if (!OptionGame) {
                DO_FADE_OUT();
                INIT_OPTIONS_CHOICE();
                INIT_AFFICHE_ECRAN_OPTIONS();
                INIT_FADE_IN();
            } else {
                INIT_OPTIONS_CHOICE();
                INIT_AFFICHE_ECRAN_OPTIONS();
            }
        }
        input_mode = 1;
        readinput();
        TestButtonReleased();
        return;
    }

    if (position >= 1 && position <= 3 && button_released) {
        input_mode = 0;
        if (but1pressed() && ((s16*)&options)[position] != 1) {
            selected_button = 1;
        } else if (but0pressed() && ((s16*)&options)[position] != 0) {
            selected_button = 0;
        } else if (but2pressed() && ((s16*)&options)[position] != 2) {
            selected_button = 2;
        } else if (but3pressed() && ((s16*)&options)[position] != 3) {
            selected_button = 3;
        }

        if (selected_button != 4) {
            if (selected_button == ((s16*)&options)[1] ||
                selected_button == ((s16*)&options)[2] ||
                selected_button == ((s16*)&options)[3]) {
                PlaySnd_old(0x44);
                button_index = 1;
                while (((s16*)&options)[button_index] != selected_button &&
                       button_index <= 3) {
                    ++button_index;
                }
                old_button = ((s16*)&options)[position];
                ((s16*)&options)[button_index] = old_button;
            }
            ((s16*)&options)[position] = selected_button;
        }
    } else if (position == 4 && ValidButPressed() && button_released) {
        ((s16*)&options)[1] = 1;
        ((s16*)&options)[2] = 0;
        ((s16*)&options)[3] = 2;
    }
    TestButtonReleased();
}

/* DOS 1:3cd38; menu.c:981 */
void END_PAD_SCREEN(void) {
    menuEtape = 4;
    if (!MENU_RETURN) {
        options_jeu.jump = options.sound_volume;
        options_jeu.fist = options.SizeScreen;
        options_jeu.action = options.Frequence;
        POINTEUR_BOUTONS_OPTIONS_BIS();
    } else {
        byte_96883 = 0;
        if (!OptionGame) {
            DO_FADE_OUT();
        }
    }
}

/* DOS 1:3cd94; menu.c:986 */
void InitParamGraphicsScreen(void) {
    button_released = 1;
    compteur = 0;
    max_compteur = 100;
    delai_repetition = 12;
    repetition = 6;
    if (options.sound_volume == 0 && ModeNormalAutorise) {
        if (OptionGame) {
            if ((num_world != 6 || num_level != 4) && !is_fee) {
                position = 5;
                nbre_options = 4;
                first_option = 2;
                option_exit = 5;
            } else {
                position = 4;
                nbre_options = 3;
                option_exit = 5;
                first_option = 3;
            }
            switch (language) {
            case 0: byte_E4CFC = 110; break;
            case 1: byte_E4CFC = 110; break;
            case 2: byte_E4CFC = 120; break;
            }
            basex = 160 - byte_E4CFC;
            general_init_screen(20, 60, 5);
            --nbre_options;
        } else {
            position = 7;
            nbre_options = 7;
            option_exit = 7;
            first_option = 1;
            switch (language) {
            case 0: byte_E4CFC = 120; break;
            case 1: byte_E4CFC = 120; break;
            case 2: byte_E4CFC = 125; break;
            }
            basex = 160 - byte_E4CFC;
            general_init_screen(15, 70, 5);
            --nbre_options;
        }
    } else {
        if (FondAutorise == 1) {
            if (OptionGame) {
                position = 5;
                nbre_options = 4;
                first_option = 3;
                option_exit = 6;
                switch (language) {
                case 0: byte_E4CFC = 110; break;
                case 1: byte_E4CFC = 110; break;
                case 2: byte_E4CFC = 120; break;
                }
            } else {
                first_option = 1;
                position = 5;
                nbre_options = 5;
                option_exit = 6;
                switch (language) {
                case 0: byte_E4CFC = 110; break;
                case 1: byte_E4CFC = 110; break;
                case 2: byte_E4CFC = 120; break;
                }
            }
            basex = 160 - byte_E4CFC;
            general_init_screen(20, 60, 5);
        } else if (FondAutorise == 2) {
            position = 5;
            nbre_options = 4;
            option_exit = 6;
            first_option = 3;
            switch (language) {
            case 0: byte_E4CFC = 110; break;
            case 1: byte_E4CFC = 110; break;
            case 2: byte_E4CFC = 120; break;
            }
            basex = 160 - byte_E4CFC;
            general_init_screen(20, 60, 5);
        }
    }
}


/* DOS 1:3d094; menu.c:991 */
void INIT_GRAPHIC_SCREEN(void) {
    pINIT_SCREEN = INIT_GRAPHIC_SCREEN;
    pINIT_AFFICHE_SCREEN = INIT_AFFICHE_GRAPHIC_SCREEN;
    pAFFICHE_SCREEN = AFFICHE_GRAPHIC_SCREEN;
    pDO_COMMANDE = DO_COMMANDE_GRAPHIC;
    pEND_SCREEN = END_GRAPHIC_SCREEN;

    if (GameModeVideo == 0) {
        byte_E4CFE = 2;
        options.sound_volume = 0;
    } else if (GameModeVideo == 1 && P486 == 0) {
        byte_E4CFE = 1;
        options.sound_volume = 1;
    } else {
        byte_E4CFE = 1;
        options.sound_volume = 2;
    }

    options.Frequence = (s16)SizeScreen;
    options.fixon = Frequence;
    options.BackgroundOn = (s16)fixon;
    options.ScrollDiffOn = BackgroundOptionOn;
    options.field_E = ScrollDiffOn;
    VideoSpeed = (s16)CalcSpeed();
    InitParamGraphicsScreen();
}

/* DOS 1:3d178; menu.c:996 */
void INIT_AFFICHE_GRAPHIC_SCREEN(void) {
    s16 i;
    s16 last_option;
    s16 new_x;
    s16 new_y;
    s16 new_w;
    s16 new_h;
    s16 left;
    s16 top;
    s16 right;
    s16 bottom;
    u8 colors[8] = {5, 5, 5, 5, 5, 5, 5, 3};


    strcpy(title, language_txt[235]);
    last_option = (s16)(first_option + nbre_options);
    for (i = 0; i <= last_option; ++i) {

        menu_to_display[i].font_size = 2;
        menu_to_display[i].field_D5 = 1;
        menu_to_display[i].is_fond = 1;
        menu_to_display[i].color = colors[i];
        if (i == option_exit) {
            strcpy(menu_to_display[i].text, language_txt[232]);
            menu_to_display[i].xpos = 160;
            menu_to_display[i].ypos = debut_sortie;
        } else {
            strcpy(menu_to_display[i].text, language_txt[196 + i]);
            menu_to_display[i].xpos = (s16)(basex + 10);
            menu_to_display[i].ypos = (s16)(debut_options +
                (i - first_option) * (ecart_options + 15));
        }
        INIT_TXT_BOX((&menu_to_display[i]));
    }

    new_x = basex;
    new_y = (s16)(debut_titre - 25);
    new_w = (s16)(byte_E4CFC * 2);
    new_h = (s16)(menu_to_display[option_exit].ypos - debut_titre + 35);
    left = word_E4CEC_x < new_x ? word_E4CEC_x : new_x;
    top = word_E4CF0_y < new_y ? word_E4CF0_y : new_y;
    right = (s16)(word_E4CEC_x + word_E4CF4_w);
    if (new_x + new_w > right) {
        right = (s16)(new_x + new_w);
    }
    bottom = (s16)(word_E4CF0_y + word_E4CEE_h);
    if (new_y + new_h > bottom) {
        bottom = (s16)(new_y + new_h);
    }
    word_E4CEC_x = new_x;
    word_E4CF0_y = new_y;
    word_E4CF4_w = new_w;
    word_E4CEE_h = new_h;
    if (OptionGame) {
        FonduOption(left, top, (s16)(right - left),
                    (s16)(bottom - top), 1);
    } else {
        FonduPixel(left, top, (s16)(right - left), (s16)(bottom - top));
    }
}

/* DOS 1:3d55c; menu.c:1001 */
void AFFICHE_GRAPHIC_SCREEN(void) {
    char on_text[3] = "on";
    char off_text[4] = "off";
    char max_text[4] = "max";
    char sixty_text[3] = "60";
    char fifty_text[3] = "50";
    const char *mode_names[3] = {"pci1", "pci2", "vesa"};
    s16 x, y, selected_x, other_x, third_x;
    const char *selected_text, *other_text, *third_text;
    u8 option_index, control_index;
    display_item_t *item;

    DISPLAY_FOND_MENU();
    if (OptionGame) {
        CadreTrans(word_E4CEC_x, word_E4CF0_y, word_E4CF4_w, word_E4CEE_h);
    }
    display_text(title, 160, debut_titre, 1, 1);
    display_text(language_txt[196], basex + 10, menu_to_display[0].ypos, 2,
                 menu_to_display[0].color);
    gauge(byte_E4CFC + 22, menu_to_display[0].ypos - 12, 128, 12, 128, 256, VideoSpeed);

    option_index = first_option;
    control_index = first_option - 1;
    for (; option_index <= option_exit; ++option_index, ++control_index) {
        if (options.sound_volume != 0 && option_index == 2) continue;
        if (OptionGame && options.sound_volume == 0 && option_index == 6) continue;
        y = menu_to_display[option_index].ypos;
        x = menu_to_display[option_index].xpos;
        item = &menu_to_display[option_index];
        if (option_index != position) {
            display_text(item->text, x, y, 2, menu_to_display[option_index].color);
        } else {
            display_box_text_plasma(item, 1);
        }
        switch (control_index) {
        case 0:
            display_text(mode_names[options.sound_volume], byte_E4CFC + 118, y, 2,
                         menu_to_display[option_index].color);
            break;
        case 1:
            if (options.sound_volume == 0) {
                gauge(byte_E4CFC + 70, y - 12, 80, 12, 0, 3, 3 - options.SizeScreen);
            }
            break;
        case 2:
            if ((options.Frequence == 0 && GameModeVideo == 0) ||
                ((options.Frequence == 0 || options.Frequence == 70 ||
                  options.Frequence == 80 || options.Frequence == 100) && GameModeVideo == 1)) {
                other_text = fifty_text;
                selected_text = sixty_text;
                third_text = max_text;
                other_x = byte_E4CFC + 134;
                selected_x = byte_E4CFC + 102;
                third_x = byte_E4CFC + 54;
            } else if (options.Frequence == 1) {
                other_text = sixty_text;
                selected_text = fifty_text;
                third_text = max_text;
                selected_x = byte_E4CFC + 134;
                other_x = byte_E4CFC + 102;
                third_x = byte_E4CFC + 54;
            } else {
                other_text = fifty_text;
                selected_text = max_text;
                third_text = sixty_text;
                other_x = byte_E4CFC + 134;
                third_x = byte_E4CFC + 102;
                selected_x = byte_E4CFC + 54;
            }
            display_text(other_text, other_x, y, 2, 5);
            if (options.sound_volume == 0) display_text(third_text, third_x, y, 2, 5);
            if (option_index == position) {
                strcpy(text_to_display[0].text, selected_text);
                text_to_display[0].xpos = selected_x;
                text_to_display[0].ypos = y;
                INIT_TXT_BOX(&text_to_display[0]);
                display_box_text_plasma(&text_to_display[0], 0);
            } else {
                display_text(selected_text, selected_x, y, 2, 1);
            }
            break;
        case 5:
            if (OptionGame || options.BackgroundOn == 0 || options.sound_volume != 0) break;
            /* The DOS cases intentionally fall through through the shared guards. */
        case 4:
            if (OptionGame && options.sound_volume == 0) break;
            if (FondAutorise == 0) break;
        case 3:
            if (((s16 *)&options)[option_index]) {
                other_text = off_text;
                selected_text = on_text;
                other_x = byte_E4CFC + 134;
                selected_x = byte_E4CFC + 102;
            } else {
                selected_text = off_text;
                other_text = on_text;
                selected_x = byte_E4CFC + 134;
                other_x = byte_E4CFC + 102;
            }
            display_text(other_text, other_x, y, 2, 5);
            if (option_index == position) {
                strcpy(text_to_display[0].text, selected_text);
                text_to_display[0].xpos = selected_x;
                text_to_display[0].ypos = y;
                INIT_TXT_BOX(&text_to_display[0]);
                display_box_text_plasma(&text_to_display[0], 0);
            } else {
                display_text(selected_text, selected_x, y, 2, 1);
            }
            break;
        }
    }
}


/* DOS 1:3da8c; menu.c:1006 */
void DO_COMMANDE_GRAPHIC(void) {
    u8 changed;
    s16 selected_mode;

    TestCompteur();
    changed = 0;
    menuEtape = 4;
    if (SelectButPressed()) {
        MENU_RETURN = 1;
    }
    if (button_released ||
        (compteur > delai_repetition && compteur % repetition == 0)) {
        if (upjoy()) {
            PlaySnd_old(0x44);
            if (position == first_option) {
                position = option_exit;
            } else if (options.sound_volume != 0 && position == 3) {
                position = 1;
            } else {
                --position;
            }
        } else if (downjoy()) {
            PlaySnd_old(0x44);
            if (position == option_exit) {
                position = first_option;
            } else if (options.sound_volume != 0 && position == 1) {
                position = 3;
            } else {
                ++position;
            }
        }
    }

    if (options.sound_volume == 0 && position == 2 &&
        !downjoy() && !upjoy() && button_released) {
        if (leftjoy() && options.SizeScreen < 3) {
            ++options.SizeScreen;
            changed = 1;
        } else if (rightjoy()) {
            if (options.SizeScreen > 0) {
                --options.SizeScreen;
            } else {
                options.SizeScreen = 3;
            }
            changed = 1;
        }
        if (changed) {
            set_xywhmap(TabW1[options.SizeScreen], TabW2[options.SizeScreen],
                        TabH1[options.SizeScreen], TabH2[options.SizeScreen]);
        }
    }

    if (position == 6 && !OptionGame && button_released &&
        (ValidButPressed() || rightjoy() || leftjoy())) {
        options.ScrollDiffOn = (s16)(options.ScrollDiffOn == 0);
        changed = 1;
    }

    if (position == 3 && !downjoy() && !upjoy() && button_released) {
        selected_mode = options.Frequence;
        if (rightjoy() || ValidButPressed()) {
            if (GameModeVideo == 1 && selected_mode < byte_E4CFE) {
                ++selected_mode;
            } else {
                selected_mode = 0;
            }
            options.Frequence = selected_mode;
            NewFrequency((u8)selected_mode);
            changed = 1;
        } else if (leftjoy()) {
            if (selected_mode > 0) {
                --selected_mode;
            } else {
                selected_mode = byte_E4CFE;
            }
            options.Frequence = selected_mode;
            NewFrequency((u8)selected_mode);
            changed = 1;
        }
    }

    if (position >= 4 && position < option_exit && position != 6 &&
        button_released && (ValidButPressed() || rightjoy() || leftjoy())) {
        ((s16*)&options)[position] =
            (s16)(((s16*)&options)[position] == 0);
        changed = 1;
    }

    if (position == 5 && !OptionGame && options.BackgroundOn == 0 &&
        options.SizeScreen == 0) {
        options.SizeScreen = 1;
        byte_E4CFE = 1;
        P486 = 0;
        if (options.Frequence == 2) {
            options.Frequence = 0;
        }
        InitParamGraphicsScreen();
        position = 5;
        INIT_AFFICHE_GRAPHIC_SCREEN();
        FonduPixel(18, (s16)(debut_titre + 14), 270,
                   (s16)(menu_to_display[option_exit].ypos - debut_titre - 36));
        changed = 1;
    }

    if (position == 1 && ValidButPressed() && button_released) {
        menuEtape = 10;
        sortie_options = 1;
    }
    if (changed) {
        VideoSpeed = (s16)CalcSpeed();
    }
    TestButtonReleased();
}

/* DOS 1:3df88; menu.c:1011 */
void END_GRAPHIC_SCREEN(void) {
    if (!MENU_RETURN) {
        SizeScreen = options.SizeScreen;
        Frequence = (u8)options.Frequence;
        fixon = (u16)options.fixon;
        BackgroundOptionOn = (u8)options.BackgroundOn;
        BackgroundOn = IsBackgroundOn();
        ScrollDiffOn = (u8)options.ScrollDiffOn;
        switch (options.sound_volume) {
        case 0:
            GameModeVideo = 0;
            P486 = 0;
            break;
        case 1:
            GameModeVideo = 1;
            P486 = 0;
            break;
        case 2:
            GameModeVideo = 1;
            P486 = 1;
            break;
        }
    } else {
        byte_96883 = 0;
        if (GameModeVideo == MODE_NORMAL) {
            set_xywhmap(TabW1[SizeScreen], TabW2[SizeScreen], TabH1[SizeScreen], TabH2[SizeScreen]);
        }
        WaitNSynchro(25);
        NewFrequency(Frequence);
        if (!OptionGame) {
            DO_FADE_OUT();
        }
    }
}

/* DOS 1:3e0a0; menu.c:1016 */
void INIT_VIDEOMODE_SCREEN(void) {
    pINIT_SCREEN = INIT_VIDEOMODE_SCREEN;
    pINIT_AFFICHE_SCREEN = INIT_AFFICHE_VIDEOMODE_SCREEN;
    pAFFICHE_SCREEN = AFFICHE_VIDEOMODE_SCREEN;
    pDO_COMMANDE = DO_COMMANDE_VIDEOMODE;
    pEND_SCREEN = END_VIDEOMODE_SCREEN;
    button_released = 1;
    compteur = 0;
    max_compteur = 100;
    delai_repetition = 12;
    repetition = 6;
    position = 3;
    first_option = 0;
    nbre_options = 3;
    option_exit = 3;
    general_init_screen(15, 68, 5);
    options.music_enabled = 0;
    options.sound_volume = 0;
    options.SizeScreen = 0;
    if (GameModeVideo == MODE_NORMAL) {
        options.music_enabled = 1;
    } else if (GameModeVideo == 1 && !P486) {
        options.sound_volume = 1;
    } else {
        options.SizeScreen = 1;
    }
    switch (language) {
    case 0: byte_E4CFC = 0x8c; break;
    case 1: byte_E4CFC = 0x8c; break;
    case 2: byte_E4CFC = 0x8c; break;
    }
    basex = 160 - byte_E4CFC;
}

/* DOS 1:3e1cc; menu.c:1021 */
void INIT_AFFICHE_VIDEOMODE_SCREEN(void) {
    s16 i;
    s16 left;
    s16 top;
    s16 right;
    s16 bottom;
    s16 new_x;
    s16 new_y;
    s16 new_w;
    s16 new_h;
    u8 color[4] = {5, 5, 5, 3};


    text_to_display[0].font_size = 2;
    text_to_display[0].field_D5 = 0;
    text_to_display[0].is_fond = 1;
    text_to_display[0].color = 1;

    strcpy(title, language_txt[203]);
    for (i = 0; i <= first_option + nbre_options; ++i) {

        menu_to_display[i].xpos = 160;
        menu_to_display[i].font_size = 2;
        menu_to_display[i].is_fond = 1;
        menu_to_display[i].field_D5 = 1;
        if (i == option_exit) {
            strcpy(menu_to_display[i].text, language_txt[232]);
            menu_to_display[i].ypos = debut_sortie;
            menu_to_display[i].color = color[3];
        } else {
            strcpy(menu_to_display[i].text, language_txt[223 + i]);
            menu_to_display[i].ypos = (s16)(debut_options + i * (ecart_options + 15));
            menu_to_display[i].color = color[i];
        }
        INIT_TXT_BOX((&menu_to_display[i]));
    }

    new_x = basex;
    new_y = (s16)(debut_titre - 25);
    new_w = (s16)(byte_E4CFC * 2);
    new_h = (s16)(menu_to_display[option_exit].ypos - debut_titre + 35);
    left = word_E4CEC_x < new_x ? word_E4CEC_x : new_x;
    top = word_E4CF0_y < new_y ? word_E4CF0_y : new_y;
    right = word_E4CEC_x + word_E4CF4_w;
    if (new_x + new_w > right) {
        right = (s16)(new_x + new_w);
    }
    bottom = word_E4CF0_y + word_E4CEE_h;
    if (new_y + new_h > bottom) {
        bottom = (s16)(new_y + new_h);
    }
    word_E4CEC_x = new_x;
    word_E4CF0_y = new_y;
    word_E4CF4_w = new_w;
    word_E4CEE_h = new_h;
    if (OptionGame) {
        FonduOption(left, top, (s16)(right - left),
                    (s16)(bottom - top), 1);
    } else {
        FonduPixel(left, top, (s16)(right - left), (s16)(bottom - top));
    }
}

/* DOS 1:3e470; menu.c:1026 */
void AFFICHE_VIDEOMODE_SCREEN(void) {
    u8 i;
    s16 x, y;
    u8 colors[4] = {5, 5, 5, 3};
    u8 color;


    DISPLAY_FOND_MENU();
    display_text(title, 160, debut_titre, 1, 1);
    for (i = first_option; i <= nbre_options + first_option; ++i) {
        x = menu_to_display[i].xpos;
        y = menu_to_display[i].ypos;

        if (i < 3 && ((s16*)&options)[i] != 0) {
            color = 1;
        } else {
            color = colors[i];
        }
        if (i != position) {
            display_text(menu_to_display[i].text, x, y, 2, color);
        } else {
            menu_to_display[i].color = color;
            INIT_TXT_BOX(&menu_to_display[i]);
            display_box_text_plasma(&menu_to_display[i], 1);
        }
    }
}

/* DOS 1:3e574; menu.c:1031 */
void DO_COMMANDE_VIDEOMODE(void) {
    u8 active_option;

    TestCompteur();
    if (SelectButPressed()) {
        MENU_RETURN = 1;
    }
    if (!button_released &&
        (compteur <= delai_repetition || compteur % repetition != 0)) {
        goto video_action;
    }
    if (upjoy()) {
        PlaySnd_old(0x44);
        if (position == first_option) {
            position = option_exit;
        } else {
            --position;
        }
    } else if (downjoy()) {
        PlaySnd_old(0x44);
        if (position == option_exit) {
            position = first_option;
        } else {
            ++position;
        }
    }

video_action:
    if (position >= 0 && position <= 2 && button_released &&
        (ValidButPressed() || rightjoy() || leftjoy())) {
        for (active_option = 0; ((s16*)&options)[active_option] == 0; ++active_option) {
        }
        if (active_option != position) {
            ((s16*)&options)[position] = ((s16*)&options)[position] == 0;
            ((s16*)&options)[active_option] = ((s16*)&options)[active_option] == 0;
        }
    }
    TestButtonReleased();
}

/* DOS 1:3e708; menu.c:1036 */
void END_VIDEOMODE_SCREEN(void) {
    if (!MENU_RETURN) {
        if (options.music_enabled != 0) {
            GameModeVideo = 0;
            P486 = 0;
            BackgroundOptionOn = 1;
        } else if (options.sound_volume != 0) {
            GameModeVideo = 1;
            P486 = 0;
            if (Frequence == 2) {
                Frequence = 0;
                NewFrequency(Frequence);
            }
        } else {
            GameModeVideo = 1;
            P486 = 1;
            if (Frequence == 2) {
                Frequence = 0;
                NewFrequency(Frequence);
            }
        }
    }
    menuEtape = 9;
}

/* DOS 1:3e790; menu.c:1041 */
void INIT_CALIBRATE_JOYSTICK(s16* xmax, s16* xmin, s16* ymax, s16* ymin) {
    *xmax = 200;
    *xmin = 0;
    *ymax = 200;
    *ymin = 0;
    memcpy(text_to_display[0].text, language_txt[236], 70);
    text_to_display[0].xpos = 160;
    text_to_display[0].ypos = 36;
    text_to_display[0].font_size = 1;
    text_to_display[0].field_D5 = 0;
    text_to_display[0].is_fond = 1;
    text_to_display[0].color = 0;
    INIT_TXT_BOX(&text_to_display[0]);
}

/* DOS 1:3e804; menu.c:1046 */
void AFFICHE_CALIBRATE_JOYSTICK(void) {
    u8 i;
    char number[12];

    DISPLAY_FOND_MENU();
    if (OptionGame) {
        CadreTrans(word_E4CEC_x, word_E4CF0_y, word_E4CF4_w, word_E4CEE_h);
    }
    display_box_text_plasma(&text_to_display[0], 1);
    display_text("/......./", 155, 98, 1, 5);
    for (i = 0; i < 7; ++i) {
        display_text(".", 150, (s16)(74 + i * 8), 1, 5);
    }
    itoa_dos(xpadmax / 10, number, 10);
    display_text(number, 198, 98, 2, 5);
    itoa_dos(xpadmin / 10, number, 10);
    display_text(number, 105, 98, 2, 5);
    itoa_dos(ypadmin / 10, number, 10);
    display_text(number, 150, 64, 2, 5);
    itoa_dos(ypadmax / 10, number, 10);
    display_text(number, 150, 138, 2, 5);
    display_text(language_txt[134], 160, 160, 2, 5);
}

/* DOS 1:3e99c; menu.c:1051 */
s16 calibrate_joystick_prg(u32 a1) {
    (void)a1;
    readinput_calibrate_joystick(&xpadmax, &xpadmin, &ypadmax, &ypadmin);
    AFFICHE_CALIBRATE_JOYSTICK();
    {
        u32 done = 0;
        done += ValidButPressed();
        if (TOUCHE(1)) {
            Main_Control = 0;
            done = 1;
        }
        return done;
    }
}

/* DOS 1:3e9e8; menu.c:1056 */
void AFFICHE_CENTER_JOYSTICK(void) {
    char number[12];

    DISPLAY_FOND_MENU();
    if (OptionGame) {
        CadreTrans(word_E4CEC_x, word_E4CF0_y, word_E4CF4_w, word_E4CEE_h);
    }
    display_box_text_plasma(&text_to_display[0], 1);
    display_text(language_txt[133], 160, 100, 2, 5);
    itoa_dos((xpadcentre / 20) * 2, number, 10);
    display_text(number, 130, 120, 2, 5);
    itoa_dos((ypadcentre / 20) * 2, number, 10);
    display_text(number, 180, 120, 2, 5);
    display_text(language_txt[135], 160, 160, 2, 5);
}

/* DOS 1:3eae8; menu.c:1061 */
s16 center_joystick_prg(u32 a1) {
    (void)a1;
    readinput_center_joystick(&xpadcentre, &ypadcentre);
    AFFICHE_CENTER_JOYSTICK();
    {
        u32 done = 0;
        done += ValidButPressed();
        if (TOUCHE(1)) {
            Main_Control = 0;
            done = 1;
        }
        return done;
    }
}

/* DOS 1:3eb28; menu.c:1066 */
void MAIN_CALIBRATE_JOYSTICK(void) {
    s16 x, h, y, w;
    s16 left, top, width, height;
    DoCdRap();
    if (pLOAD_SCREEN) {
        pLOAD_SCREEN();
        memcpy(&menu_rvb, &rvb[current_pal_id], 768);
        memcpy(&rvb[current_pal_id], &rvb_plan3, 768);
        InitPlasma(0);
    }
    INIT_CALIBRATE_JOYSTICK(&xpadmin, &xpadmax, &ypadmin, &ypadmax);
    if (!OptionGame) {
        INIT_FADE_IN();
    } else {
        switch (language) {
        case 0: x = 70; y = 8; w = 176; h = 190; break;
        case 1: x = 57; y = 8; w = 200; h = 190; break;
        case 2: x = 50; y = 8; w = 216; h = 190; break;
        }
        left = word_E4CEC_x <= x ? word_E4CEC_x : x;
        top = word_E4CF0_y <= y ? word_E4CF0_y : y;
        width = word_E4CEC_x + word_E4CF4_w > x + w ?
            word_E4CEC_x + word_E4CF4_w - left : x + w - left;
        height = word_E4CF0_y + word_E4CEE_h > y + h ?
            word_E4CF0_y + word_E4CEE_h - top : y + h - top;
        word_E4CEC_x = x;
        word_E4CF0_y = y;
        word_E4CF4_w = w;
        word_E4CEE_h = h;
        pAFFICHE_SCREEN = AFFICHE_CALIBRATE_JOYSTICK;
        FonduOption(left, top, width, height, 1);
    }
    if (!Main_Control) goto calibration_end;
    SYNCHRO_LOOP(calibrate_joystick_prg);
    if (!OptionGame) {
        DO_FADE_OUT();
        INIT_FADE_IN();
    } else {
        switch (language) {
        case 0: x = 70; y = 8; w = 176; h = 175; break;
        case 1: x = 61; y = 8; w = 194; h = 175; break;
        case 2: x = 50; y = 8; w = 215; h = 175; break;
        }
        left = word_E4CEC_x <= x ? word_E4CEC_x : x;
        top = word_E4CF0_y <= y ? word_E4CF0_y : y;
        width = word_E4CEC_x + word_E4CF4_w > x + w ?
            word_E4CEC_x + word_E4CF4_w - left : x + w - left;
        height = word_E4CF0_y + word_E4CEE_h > y + h ?
            word_E4CF0_y + word_E4CEE_h - top : y + h - top;
        word_E4CEC_x = x;
        word_E4CF0_y = y;
        word_E4CF4_w = w;
        word_E4CEE_h = h;
        if (Main_Control) {
            pAFFICHE_SCREEN = AFFICHE_CENTER_JOYSTICK;
            FonduOption(left, top, width, height, 1);
        }
    }
    if (!Main_Control) goto calibration_end;
    SYNCHRO_LOOP(center_joystick_prg);
    if (!OptionGame) {
        DO_FADE_OUT();
        INIT_FADE_IN();
    } else {
        switch (language) {
        case 0: x = 60; y = 8; w = 192; h = 187; break;
        case 1: x = 60; y = 8; w = 192; h = 187; break;
        case 2: x = 50; y = 8; w = 214; h = 190; break;
        }
        left = word_E4CEC_x > x ? x : word_E4CEC_x;
        top = word_E4CF0_y <= y ? word_E4CF0_y : y;
        width = word_E4CEC_x + word_E4CF4_w > x + w ?
            word_E4CEC_x + word_E4CF4_w - left : x + w - left;
        height = word_E4CF0_y + word_E4CEE_h > y + h ?
            word_E4CF0_y + word_E4CEE_h - top : y + h - top;
        word_E4CEC_x = x;
        word_E4CF0_y = y;
        word_E4CF4_w = w;
        word_E4CEE_h = h;
        if (Main_Control) {
            INIT_AFFICHE_BUTTON_SCREEN();
            pAFFICHE_SCREEN = AFFICHE_BUTTON_JOYSTICK;
            FonduOption(left, top, width, height, 1);
        }
    }
    sEND_SCREEN = pEND_SCREEN;
    update_pad_limits(&xpadmax, &xpadmin, &ypadmax, &ypadmin, &xpadcentre, &ypadcentre);
    if (Main_Control) {
        pINIT_SCREEN = INIT_BUTTON_SCREEN;
        if (!OptionGame) DO_MENU();
        else DO_INTER_MENU();
        pEND_SCREEN = sEND_SCREEN;
        if (!OptionGame) {
            DO_FADE_OUT();
            INIT_PAD_SCREEN();
            INIT_AFFICHE_PAD_SCREEN();
            INIT_FADE_IN();
        } else {
            INIT_PAD_SCREEN();
            INIT_AFFICHE_PAD_SCREEN();
        }
        if (pLOAD_SCREEN) memcpy(&rvb[current_pal_id], &menu_rvb, 768);
    }
calibration_end:
    Keyflush();
}


/* DOS 1:3f17c; menu.c:1071 */
s16 menu_prg(u32 a1) {
    bool pad_limits_changed = false;
    s16 need_exit;
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
        update_pad_limits(&xpadmax, &xpadmin, &ypadmax, &ypadmin, &xpadcentre, &ypadcentre);
    }
    if (pAFFICHE_SCREEN) {
        pAFFICHE_SCREEN();
    }
    if (ReInitPlasma) {
        ReInitPlasma = 0;
        endsynchro();
        synchro();
        InitPlasma(1);
        SWAP_BUFFERS();
    }
    need_exit = fin_du_jeu || sortie_options || MENU_RETURN || ModeDemo;
    if (need_exit) return need_exit;
    {
        if (pDO_COMMANDE && nb_fade == 0) {
            pDO_COMMANDE();
        }
        if (ValidButPressed() && position == option_exit) {
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
void DO_INTER_MENU(void) {
    if (pINIT_SCREEN != 0) {
        (*pINIT_SCREEN)();
    }
    if (pINIT_AFFICHE_SCREEN != 0) {
        (*pINIT_AFFICHE_SCREEN)();
    }
    SYNCHRO_LOOP(menu_prg);
    if (pEND_SCREEN != 0) {
        (*pEND_SCREEN)();
    }
}

/* DOS 1:3f318; menu.c:1129 */
void DO_MENU(void) {
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
            (num_world == world_4_image && num_level == 9) ||
            (num_world == world_5_cave && num_level == 4)
    ) {
        memcpy(&menu_rvb, &rvb_special[numero_palette_special], 768);
    } else {
        memcpy(&menu_rvb, &rvb[current_pal_id], 768);
        memcpy(&rvb[current_pal_id], &rvb_plan3, 768);
    }
    InitGauge(); // TODO
    InitPlasma(0); // TODO
    SYNCHRO_LOOP(menu_prg);
    if (pEND_SCREEN) {
        pEND_SCREEN();
    }
    memcpy(&rvb[current_pal_id], &menu_rvb, 768);
}

/* DOS 1:3f488; menu.c:1161 */
void default_key(void) {
    s16 i;
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
s16 CalcSpeed(void) {
    s16 speed;
    s16 sprite_flag;
    s32 sprite_speed;
    if (options.sound_volume == 0) {
        if (options.Frequence == 2) {
            speed = 0;
        } else {
            speed = tRefSpriteNormal[options.Frequence];
            sprite_flag = options.fixon;
            if (sprite_flag) {
                speed += tRefRam2VramNormalFix[options.Frequence][options.SizeScreen];
            } else {
                speed += tRefRam2VramNormal[options.Frequence][options.SizeScreen];
            }
            if (options.ScrollDiffOn) {
                speed += tRefTransFondNormal[options.Frequence][options.SizeScreen] * 2;
            } else {
                speed += tRefTransFondNormal[options.Frequence][options.SizeScreen];
            }
        }
    } else if (!P486) {
        sprite_speed = tRefSpriteX[options.Frequence];
        speed = sprite_speed >> 1;
        speed += sprite_speed >> 1;
        sprite_flag = options.fixon;
        if (sprite_flag) {
            speed += sprite_speed >> 3;
        }
        if (options.BackgroundOn) {
            speed += tRefRam2VramX[options.Frequence];
        }
    } else {
        sprite_speed = tRefSpriteX[options.Frequence];
        speed = sprite_speed >> 1;
        speed += sprite_speed >> 1;
        if (options.fixon) {
            speed += sprite_speed >> 3;
        }
        if (options.BackgroundOn) {
            speed += tRefVram2VramX[options.Frequence];
        }
    }
    if (VideoSpeed > 0x100) return 0x80;
    if (speed < 0x80) return 0x100;
    return 0x100 - speed + 0x80;
}


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
    if (pos_YN == 0) {
        display_text(language_txt[147], dialog_display_item_ptr->xpos - 25, dialog_display_item_ptr->centered_y_pos + dialog_display_item_ptr->height, 2, 1);
        display_text(language_txt[148], dialog_display_item_ptr->xpos + 35, dialog_display_item_ptr->centered_y_pos + dialog_display_item_ptr->height, 2, 0);
    } else {
        display_text(language_txt[147], dialog_display_item_ptr->xpos - 25, dialog_display_item_ptr->centered_y_pos + dialog_display_item_ptr->height, 2, 0);
        display_text(language_txt[148], dialog_display_item_ptr->xpos + 35, dialog_display_item_ptr->centered_y_pos + dialog_display_item_ptr->height, 2, 1);
    }
}

/* DOS 1:3fac4; menu.c:1315 */
void DO_YESNOBIS(void) {
    TestCompteur();
    if ((leftjoy() || rightjoy()) && !downjoy() && !upjoy()) {
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
#define COPY_CONFIRMATION_TEXT(index) strcpy(msg_to_display[1].text, language_txt[index])
u8 confirmation_msg(u8 which_message) {
    msg_to_display[1].xpos = 160;
    msg_to_display[1].ypos = 100;
    msg_to_display[1].font_size = 2;
    msg_to_display[1].is_fond = 2;
    msg_to_display[1].field_D5 = 0;
    switch (which_message) {
        case 0:
            pos_YN = 1;
            if (OptionGame) {
                COPY_CONFIRMATION_TEXT(131);
            } else {
                COPY_CONFIRMATION_TEXT(132);
            }
            INIT_TXT_BOX(&msg_to_display[1]);
            display_box_msg_commande(&msg_to_display[1], DO_YESNOBIS);
            break;
        case 1:
            pos_YN = 1;
            pAFFICHE_SCREEN = AFFICHE_ECRAN_SAVE;
            COPY_CONFIRMATION_TEXT(129);
            INIT_TXT_BOX(&msg_to_display[1]);
            display_box_msg_commande(&msg_to_display[1], DO_YESNOBIS);
            break;
        case 2:
            pos_YN = 1;
            pAFFICHE_SCREEN = AFFICHE_ECRAN_SAVE;
            COPY_CONFIRMATION_TEXT(130);
            INIT_TXT_BOX(&msg_to_display[1]);
            display_box_msg_commande(&msg_to_display[1], DO_YESNOBIS);
            break;
        case 3: {
            s32 i;
            default_key();
            pos_YN = 1;
            memcpy(&rvb_menu_save, &rvb[0], 768);
            InitMenuPalette();
            CaptureVideo(display_buffer, 1);
            DO_FADE_OUT();
            memset(display_buffer, 0, 64000);
            INIT_FADE_IN();
            memcpy(&rvb[0], &MenuPalette, 768);
            pAFFICHE_SCREEN = DISPLAY_FOND_MENU;
            COPY_CONFIRMATION_TEXT(130);
            INIT_TXT_BOX(&msg_to_display[1]);
            display_box_msg_commande(&msg_to_display[1], DO_YESNOBIS);
            DO_FADE_OUT();
            memcpy(&rvb[0], &rvb_menu_save, 768);
            INIT_FADE_IN();
            new_key();
            break;
        }
        case 4: {
            s32 i;
            default_key();
            pos_YN = 1;
            memcpy(&rvb_menu_save, &rvb[0], 768);
            InitMenuPalette();
            CaptureVideo(display_buffer, 1);
            DO_FADE_OUT();
            memset(display_buffer, 0, 64000);
            INIT_FADE_IN();
            memcpy(&rvb[0], &MenuPalette, 768);
            pAFFICHE_SCREEN = DISPLAY_FOND_MENU;
            COPY_CONFIRMATION_TEXT(131);
            INIT_TXT_BOX(&msg_to_display[1]);
            display_box_msg_commande(&msg_to_display[1], DO_YESNOBIS);
            DO_FADE_OUT();
            memcpy(&rvb[0], &rvb_menu_save, 768);
            INIT_FADE_IN();
            ReInitPlasma = 0;
            new_key();
            break;
        }
        default:
            break;
    }
    return pos_YN == 0;
}
#undef COPY_CONFIRMATION_TEXT

/* DOS 1:3fef0; menu.c:1413 */
void SYNCHRO_LOOP_PALETTE(synchro_loop_palette_func_t func) {
    do {
        endsynchro();
        synchro();
        menu_palette_index(0x3c8, 0);
        menu_palette_disable();
        menu_palette_stream(0x3c9, (const u8 *)&rvb_E40AC, 256 * 3);
        menu_palette_enable();
        SWAP_BUFFERS();
    } while (func() == 0);
}

/* DOS 1:3ff3c; menu.c:1426 */
s16 apparition_prg(u32 a1) {
    s16 yoff, row_skip, xoff, dst_y, dst_x;
    u8 *src, *dst_row;
    row_skip = 320 - wFondu;
    DoCdRap();
    DISPLAY_FOND_MENU();
    src = xFondu + (EffetBufferNormal + 64000) + yFondu * 320;
    for (yoff = -hFondu / 2; yoff < hFondu / 2; ++yoff) {
        dst_y = 100 + yoff * EtapeFondu;
        dst_row = dst_y * 320 + DrawBufferNormal;
        if (dst_y > Bloc_lim_H1 && dst_y < Bloc_lim_H2) {
            for (xoff = -wFondu / 2; xoff < wFondu / 2; ++xoff, ++src) {
                dst_x = 160 + EtapeFondu * xoff;
                if (dst_x > Bloc_lim_W1 && dst_x < Bloc_lim_W2) {
                    dst_row[dst_x] = *src;
                }
            }
            src += row_skip;
        } else {
            src += 320;
        }
    }
    if (InOut) {
        --EtapeFondu;
        return EtapeFondu == 1;
    } else {
        if (EtapeFondu == 16) INIT_FADE_OUT();
        ++EtapeFondu;
        return EtapeFondu == 32;
    }
}

/* DOS 1:400fc; menu.c:1484 */
s16 fondu_prg(u32 a1) {
    u16 row_skip = 320 - wFondu;
    u16 i, j;
    u8 *dst = DrawBufferNormal + xFondu + yFondu * 320;
    u8 *src = EffetBufferNormal + 64000 + xFondu;
    for (j = 0; j < hFondu; ++j) {
        for (i = 0; i < wFondu; ++i, ++dst, ++src) {
            if (*dst != *src) {
                *dst = (*dst * (16 - EtapeFondu) + *src) / (17 - EtapeFondu);
            }
        }
        dst += row_skip;
        src += row_skip;
    }
    ++EtapeFondu;
    return EtapeFondu == 16;
}

/* DOS 1:401e8; menu.c:1518 */
s16 change_couleur_prg(void) {
    s16 done = 1;
    s16 i;
    for (i = 0; i < 256; ++i) {
        if (rvb_E40AC.colors[i].r != rvb_menu_save.colors[i].r) {
            done = 0;
            if (rvb_E40AC.colors[i].r > rvb_menu_save.colors[i].r + 2) {
                rvb_E40AC.colors[i].r -= 2;
            } else if (rvb_E40AC.colors[i].r < rvb_menu_save.colors[i].r - 2) {
                rvb_E40AC.colors[i].r += 2;
            } else {
                rvb_E40AC.colors[i].r = rvb_menu_save.colors[i].r;
            }
        }
        if (rvb_menu_save.colors[i].g != rvb_E40AC.colors[i].g) {
            done = 0;
            if (rvb_E40AC.colors[i].g > rvb_menu_save.colors[i].g + 2) {
                rvb_E40AC.colors[i].g -= 2;
            } else if (rvb_E40AC.colors[i].g < rvb_menu_save.colors[i].g - 2) {
                rvb_E40AC.colors[i].g += 2;
            } else {
                rvb_E40AC.colors[i].g = rvb_menu_save.colors[i].g;
            }
        }
        if (rvb_E40AC.colors[i].b != rvb_menu_save.colors[i].b) {
            done = 0;
            if (rvb_E40AC.colors[i].b > rvb_menu_save.colors[i].b + 2) {
                rvb_E40AC.colors[i].b -= 2;
            } else if (rvb_E40AC.colors[i].b < rvb_menu_save.colors[i].b - 2) {
                rvb_E40AC.colors[i].b += 2;
            } else {
                rvb_E40AC.colors[i].b = rvb_menu_save.colors[i].b;
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
    xFondu = x - (x & 3) + 4;
    yFondu = y;
    wFondu = w - (w & 3) + 6;
    hFondu = h + 2;

    if (mode == 0) {
        if (GameModeVideo == MODE_NORMAL) {
            CaptureVideo(draw_buffer, 1);
            if (RayEvts.firefly) {
                set_xywhmap(TabW1[0], TabW2[0], TabH1[0], TabH2[0]);
                sprite_clipping(Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2);
            }
            DO_FADE_OUT();
            set_xywhmap(TabW1[SizeScreen], TabW2[SizeScreen], TabH1[SizeScreen], TabH2[SizeScreen]);
            sprite_clipping(Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2);
            N_CLRSCR(DrawBufferNormal);
        } else {
            CaptureVideo(display_buffer, 1);
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
            u8 gray = ((MenuPalette.colors[i].r + MenuPalette.colors[i].g + MenuPalette.colors[i].b) >> 2) / 3 + 128;
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
    u16 row_skip, width, row, column, phase;
    u8 *src, *dst;
    width = wFondu;
    row_skip = 320 - width;
    src = DrawBufferNormal + xFondu + yFondu * 320;
    dst = EffetBufferNormal + 64000 + xFondu;
    phase = 0;
    for (row = 0; row < hFondu; ++row) {
        for (column = 0; column < wFondu; ++column) {
            if (phase == EtapeFondu) {
                if (*src != *dst) *src = *dst;
                phase = 0;
            }
            ++src;
            ++dst;
            ++phase;
        }
        src += row_skip;
        dst += row_skip;
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

