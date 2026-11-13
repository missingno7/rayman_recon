/*
 * The Display screen of the options menu (port; user decision 2026-11-13): the port's presentation settings in the
 * game's own menu. It replaces the DOS video-mode screen (PCI / Mode X / VESA, meaningless in the port, and Mode X
 * is not provided), which Graphics Details' "video mode" row opened (menuEtape 10); derive/menu.patch routes that
 * row here, also from the in-game options. Built with the historical units on the original menu framework (the
 * screen functions, rows, plasma highlight, the in-game background and frame), like the video-mode screen it
 * replaces (src/menu.c INIT_VIDEOMODE_SCREEN .. END_VIDEOMODE_SCREEN). Each change applies at once (the view width
 * at the next frame boundary) and is saved in the settings file (platform/display.h, game/entry.c); nothing here
 * changes game state beyond the menu's own variables.
 *
 * Rows: widescreen (off / 16:9 / 21:9 / fit), pixels (square / 4:3), fullscreen (on / off; not on Android, where the
 * game is always fullscreen), exit. Left, right and the validate button change the selected row's value.
 */
#include "rtypes.h"
#include "game.h"

#include "platform/display.h"

#include <stdio.h>
#include <string.h>

/* The menu framework's state (src/menu.c declarations, same types). */
extern display_item_t menu_to_display[9];
extern char title[100];
extern s16 position, option_exit, nbre_options, basex, compteur, max_compteur, delai_repetition, repetition;
extern s16 debut_titre, debut_options, debut_sortie, ecart_options;
extern s16 word_E4CEC_x, word_E4CEE_h, word_E4CF0_y, word_E4CF4_w;
extern u8 first_option, byte_E4CFC, button_released, menuEtape, OptionGame, MENU_RETURN;
extern char *language_txt[];
extern void (*pINIT_SCREEN)(void);
extern void (*pINIT_AFFICHE_SCREEN)(void);
extern void (*pAFFICHE_SCREEN)(void);
extern void (*pDO_COMMANDE)(void);
extern void (*pEND_SCREEN)(void);

void general_init_screen(s16 title_percent, s16 options_percent, s16 exit_percent);
void TestCompteur(void);
void TestButtonReleased(void);
void display_box_text_plasma(display_item_t *box, u8 a2);
void display_text(const char *text, s16 x, s16 y, u8 font_size, u8 color);
void INIT_TXT_BOX(display_item_t *box);
void DISPLAY_FOND_MENU(void);
void CadreTrans(s16 x, s16 y, s16 width, s16 height);
void FonduOption(s16 x, s16 y, s16 w, s16 h, u8 mode);
void FonduPixel(s16 x, s16 y, s16 w, s16 h);
void PlaySnd_old(s16 sound_id);
u8 ValidButPressed(void);   /* bool = u8 (src/menu_local.h) */
u8 SelectButPressed(void);
s16 upjoy(void);
s16 downjoy(void);
s16 leftjoy(void);
s16 rightjoy(void);

void port_INIT_DISPLAY_SCREEN(void);

#ifdef __ANDROID__
#define ROWS 2   /* always fullscreen */
#else
#define ROWS 3
#endif
#define ROW_COLOR 5
#define EXIT_COLOR 3

/* The value shown in Graphics Details' "video mode" row. */
const char *port_display_value_text(void)
{
    return port_display_mode_name(port_display_mode());
}

/* "/text/", as the language file writes menu texts: display_text centres it on x and INIT_TXT_BOX measures the
   line its slashes close (src/affiche.c, src/basic.c). */
static void row_text(int row, char *out, size_t size)
{
    switch (row) {
    case 0: snprintf(out, size, "/widescreen %s/", port_display_mode_name(port_display_mode())); break;
    case 1: snprintf(out, size, "/pixels %s/", port_display_square() ? "square" : "4:3"); break;
    default: snprintf(out, size, "/fullscreen %s/", port_display_fullscreen() ? "on" : "off"); break;
    }
}

static void refresh_row(int row)
{
    row_text(row, menu_to_display[row].text, sizeof menu_to_display[row].text);
    INIT_TXT_BOX(&menu_to_display[row]);
}

/* INIT_AFFICHE_VIDEOMODE_SCREEN's layout: centred rows, the exit at debut_sortie, the menu box grown or shrunk from
   the previous screen's (FonduOption in game, FonduPixel in the menus). */
static void init_affiche(void)
{
    s16 i, x, y, w, h, left, top, width, height;
    snprintf(title, sizeof title, "/display/");
    for (i = first_option; i <= first_option + nbre_options; ++i) {
        menu_to_display[i].xpos = 160;
        menu_to_display[i].font_size = 2;
        menu_to_display[i].is_fond = 1;
        menu_to_display[i].field_D5 = 0;
        if (i == option_exit) {
            strcpy(menu_to_display[i].text, language_txt[232]);
            menu_to_display[i].ypos = debut_sortie;
            menu_to_display[i].color = EXIT_COLOR;
        } else {
            row_text(i, menu_to_display[i].text, sizeof menu_to_display[i].text);
            menu_to_display[i].color = ROW_COLOR;
            menu_to_display[i].ypos = (i - first_option) * (ecart_options + 15) + debut_options;
        }
        INIT_TXT_BOX(&menu_to_display[i]);
    }
    w = byte_E4CFC * 2;
    x = basex;
    h = menu_to_display[option_exit].ypos - debut_titre + 35;
    y = debut_titre - 25;
    left = x < word_E4CEC_x ? x : word_E4CEC_x;
    top = y < word_E4CF0_y ? y : word_E4CF0_y;
    width = word_E4CEC_x + word_E4CF4_w > x + w ? word_E4CEC_x + word_E4CF4_w - left : x + w - left;
    height = word_E4CF0_y + word_E4CEE_h > y + h ? word_E4CF0_y + word_E4CEE_h - top : y + h - top;
    word_E4CEC_x = x;
    word_E4CF0_y = y;
    word_E4CF4_w = w;
    word_E4CEE_h = h;
    if (OptionGame)
        FonduOption(left, top, width, height, 1);
    else
        FonduPixel(left, top, width, height);
}

static void affiche(void)
{
    s16 i;
    DISPLAY_FOND_MENU();
    if (OptionGame)
        CadreTrans(word_E4CEC_x, word_E4CF0_y, word_E4CF4_w, word_E4CEE_h);
    display_text(title, 160, debut_titre, 1, 1);
    for (i = first_option; i <= first_option + nbre_options; ++i) {
        if (i != position)
            display_text(menu_to_display[i].text, menu_to_display[i].xpos, menu_to_display[i].ypos, 2,
                         menu_to_display[i].color);
        else
            display_box_text_plasma(&menu_to_display[i], 1);
    }
}

static void commande(void)
{
    int step = 0;
    TestCompteur();
    if (SelectButPressed())
        MENU_RETURN = 1;
    if (button_released || (compteur > delai_repetition && compteur % repetition == 0)) {
        if (upjoy()) {
            PlaySnd_old(0x44);
            position = position == first_option ? option_exit : position - 1;
        } else if (downjoy()) {
            PlaySnd_old(0x44);
            position = position == option_exit ? first_option : position + 1;
        }
    }
    if (position < option_exit && button_released && !upjoy() && !downjoy()) {
        if (leftjoy())
            step = -1;
        else if (rightjoy() || ValidButPressed())
            step = 1;
    }
    if (step) {
        switch (position) {
        case 0: port_display_set_mode((port_display_mode() + PORT_WIDE_COUNT + step) % PORT_WIDE_COUNT); break;
        case 1: port_display_set_square(!port_display_square()); break;
        default: port_display_set_fullscreen(!port_display_fullscreen()); break;
        }
        refresh_row(position);
    }
    TestButtonReleased();
}

/* Back to Graphics Details, as END_VIDEOMODE_SCREEN. The settings are already applied. */
static void end_screen(void)
{
    menuEtape = 9;
}

void port_INIT_DISPLAY_SCREEN(void)
{
    pINIT_SCREEN = port_INIT_DISPLAY_SCREEN;
    pINIT_AFFICHE_SCREEN = init_affiche;
    pAFFICHE_SCREEN = affiche;
    pDO_COMMANDE = commande;
    pEND_SCREEN = end_screen;
    button_released = 1;
    compteur = 0;
    max_compteur = 100;
    delai_repetition = 12;
    repetition = 6;
    first_option = 0;
    nbre_options = ROWS;
    option_exit = ROWS;
    position = 0;
    general_init_screen(15, 68, 5);
    byte_E4CFC = 0x8c;
    basex = 160 - byte_E4CFC;
}
