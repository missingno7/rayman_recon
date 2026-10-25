/*
 * Test entries mirroring PcMain (src/pcmain.c, function PcMain, DOS 1:083f0). Compiled with the historical units
 * (same flags and pre-include). Every call below is the original one in the original order, except the steps
 * marked PORT, which port/docs/DIVERGENCES.md lists with their reason. tests/test_port_mirror.py pins the PcMain
 * body this file was written against.
 */
#include "game.h"
#include "obj.h"
#include "palette.h"

#include "game/port_main.h"

/* src/pcmain.c 40-43 */
#define MODE_NORMAL 0
#define SCREEN_HEIGHT 200
#define SCREEN_WIDTH 320

/* Globals (types: docs/semantics registries). */
extern u8 TheProcesseur, input_mode, OptionMusicCdActive, MusicCdActive, NBRE_SAVE, fin_de_rayman, fin_du_jeu;
extern u8 ModeDemo, Frequence, GameModeVideo, byte_CFA2A, BackgroundOn, ExitMenu, CarteSonAutorisee, SonLimite;
extern u8 VGA_FREQ, P486, byte_CFA29, JeuCracker;
extern s16 new_world, new_level, num_world, num_level, num_world_choice, NumDemo, During_The_Menu;
extern status_bar_t status_bar;
extern rgb_palette_t rvb_plan3;

u8 DetectCpu(void);
void sub_5BB78(void);
void sub_5BB94(void);
void sub_5BAE0(void);
void sub_5BAD8(void);
void MakeMyRand(int set_seed);
void Init_Clavier(void);
void ToDoAtExit(void);
void InitMemoryVariable(void);
void sprite_clipping(s32 xmin, s32 xmax, s32 ymin, s32 ymax);
void INIT_MOTEUR_BEGIN(void);
void FIRST_INIT(void);
void DO_UBI_LOGO(void);
void DO_GROS_RAYMAN(void);
void INIT_EXTENSIONS(void);
void LOAD_CONFIG(void);
void PC_init_cheats(void);
void InitPcSoundCard(void);
void stop_cd(void);
void InitMusic(void);
void verify_mscdex(void);
void check_saved_drive_letter(void);
u8 verify_cdrom_response(void);
void DEPART_INIT_LOOP(void);
void INIT_WORLD_INFO(void);
void set_default_Bloc_clipping(void);
void DO_NEW_MENUS(void);
void InitDemoJeu(void);
void FinDemoJeu(void);
void SPECIAL_INIT(void);
void default_sprite_clipping(void);
void DO_WORLD_MAP(void);
void DEPART_WORLD(void);
s32 LoadBnkWorld(s16 world);
void WaitNSynchro(s32 n_frames);
void speaker_enable(void);
void DEPART_LEVEL(void);
void init_divers_level_PC(u8 *a1);
u8 get_casse_brique_active(void);
void InitClipping(void);
void INIT_MOTEUR_LEVEL(s32 new_level_flag);
void init_fee(void);
void init_moustique(void);
void InitPaletteSpecialPC(void);
void fade_out(s16 speed, rgb_palette_t *palette);
void INIT_MOTEUR_DEAD(void);
void INIT_RAY_ON_MS(void);
void START_LEVEL_ANIM(void);
u8 IsBackgroundOn(void);
void InitModeNormalWithFrequency(u8 freq);
void NewFrequency(u8 frequency);
void INIT_GAME_MODE_NORMAL(void);
void DO_MAIN_LOOP_PC_NORMAL(u8 *key_state);
void FIN_GAME_MODE_NORMAL(void);
void stop_all_snd(void);
void DO_CONTINUE(void);
void DO_VICTOIRE(void);
void INIT_CONTINUE(void);
void DONE_MOTEUR_LEVEL(void);
void FIN_DEAD_LOOP(void);
void FIN_MAP_LOOP(void);
void FIN_GAME_LOOP(void);
void init_arg(s32 argc, char **argv);
u8 detect_dos_version(u8 *major_version, u8 *minor_version);
u8 detect_windows(u8 *first_result, u8 *second_result);
extern u8 DosDetect, HVerDos, LVerDos, WindowsLance, NVersionWindows, HVersionWindows;
extern char saved_argv[255];
void rayman_dos_main(int argc, char **argv);

/* The DOS command line the port runs as: the game installed in C:\RAYMAN (PORT: argv[0] is only compared with
   the CD drive letter by check_saved_drive_letter, and printed). */
static char *dos_argv[] = {"C:\\RAYMAN\\RAYMAN.EXE", NULL};

int port_run_dos_main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    rayman_dos_main(1, dos_argv);
    return 0;
}

/* main() before PcMain (src/pcmain.c 989-1007). */
static void mirror_main_prologue(void)
{
    /* PORT: unlink("c:\\raytmp.tmp") has no file to remove; AfficheEntete writes the DOS text screen. */
    DosDetect = detect_dos_version(&HVerDos, &LVerDos);
    WindowsLance = detect_windows(&NVersionWindows, &HVersionWindows);
    OptionMusicCdActive = 1;
    MusicCdActive = 1;
    sub_5BAD8();
    /* PORT: processor-flag and debug-register writes have no native counterpart (compat inline helpers). */
    init_arg(1, dos_argv);
    strcpy(saved_argv, dos_argv[0]);
}

enum { RUN_MENUS, RUN_DEMO };

/* PcMain (src/pcmain.c 1009-1144). */
static void mirror_pcmain(int run, int demo)
{
    u8 level_init = 0;
    TheProcesseur = DetectCpu();
    sub_5BB78();
    sub_5BB94();
    /* PORT: the BIOS-tick debug-register watch (1:08442..08481) is omitted (docs/port/PLATFORM_OPEN.md). */
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
        if (run == RUN_DEMO) {
            /* TEST ENTRY: what the menu does after its idle timeout (menu.c DO_COMMANDE_GENERAL), with the
               demo chosen by the caller. */
            NumDemo = (s16)demo;
            InitDemoJeu();
        } else {
            DO_NEW_MENUS();
        }
        /* PORT: the intro movie (playVideo "intro.dat") is not ported yet. */
        while (!fin_du_jeu && status_bar.lives >= 0 && new_world) {
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
                    /* PORT: Mode X is not presented by the port; the normal-mode branch always runs. */
                    InitClipping();
                    InitModeNormalWithFrequency(VGA_FREQ);
                    NewFrequency(Frequence);
                    INIT_GAME_MODE_NORMAL();
                    During_The_Menu = 0;
                    DO_MAIN_LOOP_PC_NORMAL(&level_init);
                    During_The_Menu = 1;
                    FIN_GAME_MODE_NORMAL();
                    if (CarteSonAutorisee)
                        stop_all_snd();
                    InitModeNormalWithFrequency(VGA_FREQ);
                    sprite_clipping(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
                    START_LEVEL_ANIM();
                    if (!ExitMenu) {
                        DO_CONTINUE();
                        DO_VICTOIRE();
                    } else
                        INIT_CONTINUE();
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
        if (ModeDemo) {
            FinDemoJeu();
            if (run == RUN_DEMO)
                return;
        }
    }
}

int port_run_demo(int demo)
{
    mirror_main_prologue();
    mirror_pcmain(RUN_DEMO, demo);
    return 0;
}

int port_run_menus(void)
{
    mirror_main_prologue();
    mirror_pcmain(RUN_MENUS, 0);
    return 0;
}
