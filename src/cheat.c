#include "type_island_abi.h"
#include "inline_helpers.h"
/* cheat object, DOS v1.21, worker w4_cheatload. Names inherited RAYVERSE/ANDROID;
 * sub_* names are reference placeholders. Timer ISR reconstructed from DOS.
 * Partial candidate: see NOTES.md for remaining byte/layout differences. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "lowlevel.h"
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <i86.h>
#include <dos.h>
#include "cheat_inline.h"


/* ORIGINAL_INLINE: DOS 1:26164, 1:26fbb, 1:270c5, CLI; no general register changes. */
void inhibit_interrupts(void);
#pragma aux inhibit_interrupts = "cli" modify exact []
/* ORIGINAL_INLINE: DOS 1:26222 and 1:270cc, STI; no CLD or register changes. */
void permit_interrupts(void);
#pragma aux permit_interrupts = "sti" modify exact []

#define MODE_TEXT 255
#define MODE_X 1
#define MODE_5_CASSE_BRIQUE 5
#define world_6_cake 6
#define SC_ENTER 28
#define SC_ESCAPE 1
#define SC_NUMLOCK 69
#define SC_TAB 15
#define SC_SPACE 57
typedef u8 bool;
extern u8 *DrawBufferNormal;
extern u8 *DrawBufferNormalIni;
extern u8 *EffetBufferNormal;
extern u8 JeuCracker;
extern u8 ModeVideoActuel;
extern u8 MusicCdActive;
extern rayevts_t RayEvts;
extern rgb_palette_t current_rvb;
extern u8 *display_buffer;
extern s16 fichier_selectionne;
extern u8 fin_du_jeu;
extern u8 input_mode;
extern s16 num_level;
extern s16 num_world;
extern record_t record;
extern rgb_palette_t rvb_plan3;
extern rgb_palette_t rvb_save;
extern u8 text_input_buffer[9];
void DISPLAY_FOND3(void);
void DoneMusic(void);
void InitTextMode(void);
void LoadPcxOptions(u8* buffer, s32 resource_id, s16* width, s16* height);
void LoadPcxPaletteInVignet(s16 resource_id, u8* palette);
void align_mem_desc(mem_t* mem);
void load_level(mem_t* mem, mem_t* sprites, char* filename);
void sub_5BAE0(void);
void sub_5bbe8(void);
void sub_5BF58(void);
void INIT_FND(void);
void free_main_mem(mem_t* mem);
void JumelleFreeArray(s32** array);
void sub_5BB48(void);
void sub_6F740(void);
void RAY_DEMIRAY(void);
void Reset_Clavier(void);
u8 TOUCHE(u8 key);
s16 but1pressed(void);
void do_record(record_t *rec);
s32 clock_ticks(void);
u8 *temp_malloc(s32 size);
void temp_free(u8 *memory);
void *sub_6FBB4(s32 size);
s16 CalcSpeed(void);
u8 confirmation_msg(s32 msg);
extern void exit();
void fade_out(s16 speed, rgb_palette_t* palette);
u8 get_casse_brique_active(void);
void load_big_ray(mem_t* buffer);
void readinput(void);
u8 sub_38220(s32 a1, s32 a2, u8* a3, u8* a4);
u8 sub_38208(s32 a1, s32 a2, u8* a3, u8* a4);
void sub_38258(void);
void sub_38290(void);
void textmode(void);
void InitModeNormalWithFrequency(u8 frequency);
void init_divers_level_PC(u8* a1);
void file_level(s16 level_id, char* filename);
void world_level(s16 world_id, char* filename);
void LOAD_VIGNET_PC(mem_t* mem, s16 world);
void LoadPlan2InVignet(mem_t* mem, s32 resource_id);
void SwapPlan2PlanVignInVignet(mem_t* mem, s32 resource_id, s16 vig_index);
void DISPLAY_AND_FADE_PLAN3(void);
void LOAD_SAVE_SCREEN(mem_t* mem);
void LOAD_PERFTIME_SCREEN(mem_t* mem);
void LOAD_PERFECT_SCREEN(mem_t* mem);
void LOAD_CONTINUE_SCREEN(mem_t* mem);
void DISPLAY_FOND_CONTINUE(void);
void LOAD_VIGNET_GAME(mem_t* mem);
void REMOVE_FICHIER(void);
s32 TestSave(void);
void LOAD_GENERAL_SCREEN(void);
void LOAD_OPTIONS_SCREEN(void);
void DEPART_INIT_LOOP(void);
void DEPART_DEAD_LOOP(void);
void FIN_MAP_LOOP(void);
void FIN_DEAD_LOOP(void);
void INIT_EXTENSIONS(void);
void END_WORLD_VIGNET(void);
s16 LOAD_BIG_RAYMAN(mem_t* mem);
void __interrupt __far sub_36FAC(void);
u8 StartButPressed(void);
u8 PauseButPressed(void);
u8 ExitButPressed(void);
u8 CancelButPressed(void);
u8 SelectButPressed(void);
void ToDoAtExit(void);
void PC_clear_text_input_buffer(void);

extern s32 plan2bit_length;
extern u32 plan2bit_nb_bytes;
extern s32 plan3bit_length;
extern s32 plan3bit_nb_bytes;
extern u8 *PLAN2BIT;
extern u8 *PLAN3BIT;
extern u8 *PLANVIGBIT[8];
extern s32 planVigbit_length[8];
extern s32 planVigbit_nb_bytes[8];
extern rgb_palette_t rvb_Vig[8];
extern rgb_palette_t rvb_pres;
extern rgb_palette_t rvb_plan3;
extern rgb_palette_t current_rvb;
extern u16 nb_fade;
extern void start_fade_in(s16 speed, rgb_palette_t *palette);
extern void do_fade(rgb_palette_t *from, rgb_palette_t *to);
extern s16 display_mode;
extern u8 need_timer;
extern u16 word_CF7E6;
extern u16 word_CF7EE;
extern u16 word_CF7F0;
extern s32 timer_count;
extern s32 timer_step;
extern s32 timer_phase;
extern void (__interrupt __far *old_timer)(void);
extern u8 MessageProgram;
extern u8 * cheat_code_10_wiz;
extern u8 * cheat_code_10_wiz_alt;
extern u8 * cheat_code_5hp;
extern u8 * cheat_code_5hp_alt;
extern u8 * cheat_code_99_lives;
extern u8 * cheat_code_99_lives_alt;
extern u8 * cheat_code_all_powers;
extern u8 * cheat_code_all_powers_alt;
extern u8 * cheat_code_breakout;
extern u8 * cheat_code_breakout_alt;
extern u8 * cheat_code_breakout_before_victory;
extern u8 * cheat_code_free_movement;
extern u8 * cheat_code_freq100;
extern u8 * cheat_code_freq80;
extern u8 * cheat_code_golden_fist;
extern u8 * cheat_code_golden_fist_alt;
extern u8 * cheat_code_lens;
extern u8 * cheat_code_level_select;
extern u8 * cheat_code_programmer_message;
extern u8 * cheat_code_unlock_all_levels;
extern u8 * cheat_code_unlock_all_levels_alt;
extern u8 * cheat_code_win_map;
extern u8 * cheat_code_win_map_alt;
extern s32 saved_frequence;
extern u8 byte_CFA2A;
extern u8 drm_byte_CFA27;
extern char level_filename[20];
extern mem_t main_mem_fix;
extern mem_t main_mem_sprite;
extern u16 path_has_changed;
extern char demo_video_mem[100];
extern char world_filename[20];
extern u8 FondAutorise;
extern u8 ModeNormalAutorise;
extern u8 P486;
extern u32 TailleMainMemTmp;
extern u8 freeze;
extern mem_t temp_mem_buf;
extern u8 use_sync;
void init_all_memory(u32 size);
void sub_4212C(s32 value);
void FatalError(const char *message);
extern u8 ALL_WORLD;
extern u8 Frequence;
extern u8 GameModeVideo;
extern s32 JumAmpDefMul;
extern s32 JumAmpDefPlus;
extern u8 JumelleZoomActif;
extern u8 ModeDemo;
extern u8 PROC_EXIT;
extern u8 ParamZoomChange;
extern u8 PositionStageNameCalcule;
extern u8 RaymanDansUneMapDuJeu;
extern u8 VGA_FREQ;
extern u8 You_Win;
extern s32 cdTime;
extern u8 cheat_codes[325];
extern u8 chemin_percent;
extern u8 dans_la_map_monde;
extern u8 dead_time;
extern u8* draw_buffer;
extern finbosslevel_t finBosslevel;
extern u8 fin_boss;
extern u8 gele;
extern s16 joy_buttonA1;
extern u8 language;
extern char* language_txt[];
extern mem_t main_mem_level;
extern mem_t main_mem_world;
extern u8 need_check_cheats;
extern s16 new_level;
extern s16 new_world;
extern s16 num_level_choice;
extern s16 num_world_choice;
extern poing_t poing;
extern obj_t* poing_obj;
extern obj_t ray;
extern s16 ray_mode;
extern save_state_t save1;
extern status_bar_t status_bar;
extern world_info_t t_world_info[24];
extern s16 word_E0CD0;

void sub_35F70(void) {
    s32 *intervals;
    s32 *times;
    s32 sum = 0;
    s32 average;
    s32 low_limit;
    s32 high_limit;
    s16 i;
    s16 j;
    u8 valid;

    times = (u32*)temp_malloc(0xfa0);
    if (times != 0) {
        for (i = 0; i < 1000; ++i) {
            for (j = 0; j < 500; ++j) {
            }
            times[i] = clock_ticks();
        }
        intervals = (s32*)temp_malloc(0xf9c);
        if (intervals != 0) {
            valid = 1;
            for (i = 0; i < 999; ++i) {
                intervals[i] = times[i + 1] - times[i];
                if (intervals[i] <= 0) {
                    valid = 0;
                    break;
                }
                sum += intervals[i];
            }
            if (valid) {
                average = sum / 999;
                high_limit = (average * 4) / 3;
                low_limit = (average * 3) / 4;
                for (i = 0; i < 999; ++i) {
                    if (!inrange(intervals[i], low_limit, high_limit)) {
                        ++valid;
                    }
                }
            }
            need_timer = (valid < 10);
            temp_free(intervals);
        }
        temp_free(times);
    }
}

void sub_3609C(void) {
    s16 i;
    u32 start_ticks;
    u16 product; /* Only the low half reaches the final 16-bit timing sum. */

    if (need_timer == 0) {
        return;
    }
    endsynchro();
    start_ticks = clock_ticks();
    wait_until(start_ticks + 3000);
    synchro();
    start_ticks = clock_ticks();
    for (i = 0; i < VGA_FREQ / 2; ++i) {
        endsynchro();
        wait_until(clock_ticks() + 3000);
        synchro();
    }
    word_CF7F0 = (u16)((clock_ticks() - start_ticks) / i);
    word_CF7EE = (word_CF7F0 * 31U) / 32;
    product = (u32)word_CF7F0 * display_mode;
    word_CF7E6 = word_CF7EE + product;
}

void InitPcSoundCard(void) {
    u32 start_ticks;
    u32 end_ticks;
    u8 attempts = 0;
    u8 failed = 0;
    s32 limit;

    inhibit_interrupts();
    display_mode = 0;
    speaker_enable();
    sub_35F70();
    if (need_timer != 0) while (failed == 0) {
        sub_3609C();
        endsynchro();
        start_ticks = clock_ticks();
        wait_until(start_ticks + 3000);
        synchro();
        start_ticks = clock_ticks();
        limit = start_ticks + word_CF7EE;
        wait_until(limit);
        synchro();
        end_ticks = clock_ticks();
        if (end_ticks - start_ticks < (word_CF7F0 * 101) / 100) {
            failed = 1;
        } else {
            ++attempts;
        }
        if (attempts > 2) {
            display_mode = (CalcSpeed() <= 190);
            need_timer = 0;
            failed = 1;
        }
    }
    permit_interrupts();
}

void FIRST_INIT(void) {
    init_all_memory(TailleMainMemTmp);
    InitTextMode();
    freeze = 0;
    record.is_recording = 0;
    record.is_playing = 0;
    gele = 0;
    JoystickPresent();
    sub_4212C(3);
    DrawBufferNormal = (u8 *)sub_6FBB4(64064);
    EffetBufferNormal = (u8*)sub_6FBB4(128000);
    if (!DrawBufferNormal || !EffetBufferNormal) {
        FatalError("Memory error in draw_buffer.\n");
    }
    InitMatriceJumelle();
    {
        u8 frequency = Frequence;
        /* COMPILER_STEERED: retain the frequency value before applying the fallback. */
        VGA_FREQ = frequency;
        if (frequency != 70) VGA_FREQ = 60;
    }
    InitModeNormalWithFrequency(VGA_FREQ);
    use_sync = 1;
    set_speaker_on();
    sub_35F70();
    path_has_changed = 0;
}

void init_divers_level_PC(u8* a1) {
    if (fin_du_jeu) {
        return;
    }
    *a1 = 255;
    readinput();
    if (record.current_offset == 0 && (record.is_playing || record.is_recording)) {
        do_record(&record); // TODO
    }
}

void file_level(s16 level_id, char* filename) {
    char number[12];
    strcpy(filename, "RAY");
    ltoa(level_id, number, 10);
    strcat(filename, number);
    strcat(filename, ".LEV");
}

void world_level(s16 world_id, char* filename) {
    char number[12];
    strcpy(filename, "RAY");
    ltoa(world_id, number, 10);
    strcat(filename, number);
    strcat(filename, ".WLD");
}

void LoadPlan3InVignet(mem_t *mem, s32 resource_id)
{
    s32 saved_plan2bit_length = plan2bit_length;
    s32 saved_plan2bit_nb_bytes = plan2bit_nb_bytes;
    u8 *saved_plan2bit = PLAN2BIT;
    rgb_palette_t saved_palette;

    memcpy(&saved_palette, &rvb_pres, sizeof(saved_palette));
    LoadPlan2InVignet(mem, resource_id);
    PLAN3BIT = PLAN2BIT;
    plan3bit_length = plan2bit_length;
    plan3bit_nb_bytes = plan2bit_nb_bytes;
    memcpy(&rvb_plan3, &rvb_pres, 768);
    PLAN2BIT = saved_plan2bit;
    plan2bit_length = saved_plan2bit_length;
    plan2bit_nb_bytes = saved_plan2bit_nb_bytes;
    memcpy(&rvb_pres, &saved_palette, sizeof(saved_palette));
}

void DISPLAY_AND_FADE_PLAN3(void)
{
    start_fade_in(2, &rvb_plan3);
    endsynchro();
    SWAP_BUFFERS();
    synchro();
    DISPLAY_FOND3();
    while (nb_fade != 0) {
        endsynchro();
        synchro();
        do_fade(&rvb_plan3, &current_rvb);
    }
}

void LOAD_VIGNET_PC(mem_t* mem, s16 world) {
    if (get_casse_brique_active()) {
        LoadPlan3InVignet(mem, 64);
    } else {
        switch(world) {
            case 1: {
                LoadPlan3InVignet(mem, 68);
            } break;
            case 2: {
                LoadPlan3InVignet(mem, 70);
            } break;
            case 3: {
                LoadPlan3InVignet(mem, 69);
            } break;
            case 4: {
                LoadPlan3InVignet(mem, 66);
            } break;
            case 5: {
                LoadPlan3InVignet(mem, 63);
            } break;
            case 6: {
                LoadPlan3InVignet(mem, 62);
            } break;
            default: {
                LoadPlan3InVignet(mem, 61);
            } break;
        }
    }
}

void LOAD_SAVE_SCREEN(mem_t* mem) {
    LoadPlan3InVignet(mem, 12);
}

void LOAD_PERFTIME_SCREEN(mem_t* mem) {
    LoadPlan3InVignet(mem, 73);
}

void LOAD_PERFECT_SCREEN(mem_t* mem) {
    LoadPlan3InVignet(mem, 71);
}

void LOAD_CONTINUE_SCREEN(mem_t* mem) {
    LoadPlan3InVignet(mem, 11);
    /* DRM check (DOS 1:2661c..26641): exits unless the CD drive is a valid read-only disk. */
    exit_if_not_cd_drive(0x3600, drm_byte_CFA27 - 0x40);
}

void DISPLAY_FOND_CONTINUE(void) {
    DisplayAnyPictureNormal(PLAN3BIT, draw_buffer, 0, 0, 0, 0, 320, 320, 137);
}

void SwapPlan2PlanVignInVignet(mem_t *mem, s32 resource_id, s16 vig_index)
{
    u8 *saved_plan2bit = PLAN2BIT;
    s32 saved_plan2bit_length = plan2bit_length;
    s32 saved_plan2bit_nb_bytes = plan2bit_nb_bytes;
    rgb_palette_t saved_palette;

    memcpy(&saved_palette, &rvb_pres, sizeof(saved_palette));
    LoadPlan2InVignet(mem, resource_id);
    PLANVIGBIT[vig_index] = PLAN2BIT;
    planVigbit_length[vig_index] = plan2bit_length;
    planVigbit_nb_bytes[vig_index] = plan2bit_nb_bytes;
    memcpy(&rvb_Vig[vig_index], &rvb_pres, sizeof(rgb_palette_t));
    PLAN2BIT = saved_plan2bit;
    plan2bit_length = saved_plan2bit_length;
    plan2bit_nb_bytes = saved_plan2bit_nb_bytes;
    memcpy(&rvb_pres, &saved_palette, sizeof(saved_palette));
}

void LOAD_VIGNET_GAME(mem_t* mem) {
    s32 vignet = -1;
    switch(num_world) {
        case 1: {
            if (num_level == 9) {
                vignet = 74;
            }
        } break;
        case 3: {
            if (num_level == 6) {
                vignet = 60;
            }
        } break;
        case 4: {
            if (num_level == 11) {
                vignet = 72;
            }
        } break;
        case 5: {
            if (num_level == 3) {
                vignet = 67;
            }
            if (num_level == 11) {
                vignet = 51;
            }
        } break;
        case 6: {
            if (num_level == 4) {
                vignet = 65;
            }
        } break;
    }
    if (vignet != -1) {
        SwapPlan2PlanVignInVignet(mem, vignet, 0);
    }
}

void LOAD_CREDITS_VIGNET(mem_t *mem1, mem_t *mem2)
{
    s32 vignette_ids[8] = {52, 58, 55, 53, 56, 54, 59, 57};
    u32 i;

    for (i = 0; i <= 3; ++i)
        SwapPlan2PlanVignInVignet(mem1, vignette_ids[i], (s16)i);
    for (i = 4; i <= 7; ++i)
        SwapPlan2PlanVignInVignet(mem2, vignette_ids[i], (s16)i);
}

void DEPART_WORLD(void) {
    if (MusicCdActive) {
        stop_cd();
    }
    if (!ModeNormalAutorise) {
        GameModeVideo = MODE_X;
    }
    if (FondAutorise == 2) {
        P486 = 1;
    }
    if (num_world_choice != num_world) {
        num_level = 0;
        new_world = 1;
        new_level = 1;
    } else {
        if (get_casse_brique_active()) {
            num_level = 0;
            new_world = 1;
        } else {
            new_world = 0;
        }
        new_level = 1;
    }
    if (You_Win == 1 && num_world_choice == world_6_cake) {
        DO_VICTOIRE(); //TODO
    } else if (!fin_du_jeu && new_world != 0) {
        START_WORLD_VIGNET();
        block_free(&main_mem_world);
        SpriteFixeBlocksFree(&main_mem_sprite);
        world_level(num_world_choice, world_filename);
        load_world(&main_mem_world, &main_mem_sprite, world_filename);
        SpriteWorldOffset(&main_mem_sprite);
        INIT_MOTEUR_WORLD();
        END_WORLD_VIGNET();
    }
}

void DEPART_LEVEL(void) {
    if (MusicCdActive) {
        stop_cd();
    }
    sub_5BAE0();
    num_world = num_world_choice;
    new_world = 0;
    new_level = 1;
    num_level = num_level_choice;
    if (!fin_du_jeu) {

        if (byte_CFA2A) {
            s16 text_x;
            s16 text_y;
            s32 i;
            text_x = 160;
            text_y = 190;
            if (get_casse_brique_active()) {
                text_x = 240;
                text_y = 160;
            }
            start_fade_in(2, &rvb_plan3);
            fill(draw_buffer);
            display_text(language_txt[149], text_x, text_y, 2, 0); // /loading../
            synchro();
            endsynchro();
            SWAP_BUFFERS();
            while (nb_fade != 0) {
                endsynchro();
                synchro();
                do_fade(&rvb_plan3, &current_rvb);
            }
        }

        sub_5bbe8();
        if (MusicCdActive) {
            cdTime = 0;
        }

        file_level(num_level, level_filename);
        align_mem_desc(&main_mem_level);
        SpriteWorldBlocksFree(&main_mem_sprite);
        LOAD_VIGNET_GAME(&main_mem_level);
        load_level(&main_mem_level, &main_mem_sprite, level_filename);
        INIT_FND();

        if (num_world >= 2 && num_level >= 7) {
            sub_5BF58();
        }

    }
}

void REMOVE_FICHIER(void) {
    char filename[52];
    strcpy(filename, (char*)&demo_video_mem);
    strcat(filename, "RAYMAN");
    itoa(fichier_selectionne, filename + strlen(filename), 10);
    strcat(filename, ".SAV");
    remove(filename);
}

s32 TestSave(void) {
    return 3;
}

void test_EXIT(void) {
    if (input_mode == 1) {
        if (TOUCHE(SC_ESCAPE)) {
            PROC_EXIT = 1;
        }
    } else if (kbhit() && (u8)getch() == 27) {
        PROC_EXIT = 1;
    }
}

void test_Keyb_on_wldmap(void) {
    if (input_mode == 1) {
        if (TOUCHE(SC_ESCAPE) && nb_fade == 0 && confirmation_msg(4)) {
            PROC_EXIT = 1;
        }
    } else if (kbhit() && (u8)getch() == 27) {
        PROC_EXIT = 1;
    }
}

void Keyflush(void) {
    while (kbhit()) {
        getch();
    }
}

void LOAD_GENERAL_SCREEN(void) {
    s16 width;
    s16 height;
    LoadPcxOptions(EffetBufferNormal, 13, &width, &height);
    LoadPcxPaletteInVignet(13, (u8*)&rvb_plan3);
}

void LOAD_OPTIONS_SCREEN(void) {
    s16 width;
    s16 height;
    LoadPcxOptions(EffetBufferNormal, 12, &width, &height);
    LoadPcxPaletteInVignet(12, (u8*)&rvb_plan3);
}

/* Local meanings: docs/semantics/cheat.json, function:HANDLE_KEY. */
void HANDLE_KEY(u8* key_result) {
    if (input_mode != 1) {
        if (kbhit() && !ModeDemo) {
            *key_result = (u8)getch();
        } else {
            *key_result = 255;
        }
    }
    return;
}

void DISPLAY_FOND_SELECT(void) {
    if (ModeVideoActuel) {
        InitModeNormalWithFrequency(VGA_FREQ);
    }
    DisplayAnyPictureNormal(PLAN2BIT, draw_buffer, 0, 0, 0, 0, 320, 320, 200);
}

void DEPART_INIT_LOOP(void) {
    //nullsub
}

void DEPART_DEAD_LOOP(void) {
    //nullsub
}

void FIN_GAME_LOOP(void) {
    if (!fin_du_jeu) {
        return;
    }
    doneGameSave();
    block_free(&main_mem_world);
    SpriteFixeBlocksFree(&main_mem_sprite);
}

void FIN_MAP_LOOP(void) {
    s32 cracked = JeuCracker;
    if (RayEvts.tiny) {
        RAY_DEMIRAY();
    }
    exit_if_cracked(cracked);
}

void FIN_DEAD_LOOP(void) {
}

void END_GAME(void) {
    sub_5BB48();
    InitTextMode();
    if (record.is_recording) {
        Reset_Clavier();
        record.is_finished = 1;
        do_record(&record); // TODO
    }
    if (fin_du_jeu) {
        free_main_mem(&main_mem_level);
        free_main_mem(&main_mem_world);
        free_main_mem(&main_mem_sprite);
        free_main_mem(&main_mem_fix);
        JumelleFreeArray((s32**)&DrawBufferNormalIni);
        if (FondAutorise != 2) {
            JumelleFreeArray((s32**)&EffetBufferNormal);
        }
    }
    sub_6F740();
    sub_5BB48();
    exit(0);
}

void INIT_EXTENSIONS(void) {
    //nullsub
}

void DISPLAY_LOADING(void) {
    s16 text_x = 160;
    s16 text_y = 190;
    DISPLAY_FOND3();
    if (get_casse_brique_active()) {
        text_x = 240;
        text_y = 160;
    }
    display_text(language_txt[149], text_x, text_y, 2, 0); // /loading../
}

void START_WORLD_VIGNET(void) {
    LOAD_VIGNET_PC(&main_mem_level, num_world_choice);
    memcpy(&rvb_save, &current_rvb, 768);
    memcpy(&current_rvb, &rvb_plan3, 768);
    start_fade_in(2, &rvb_plan3);
    synchro();
    endsynchro();
    SWAP_BUFFERS();
    DISPLAY_LOADING();

    // Do loading screen effect (TODO)
    if (sub_38208(9, 100, DrawBufferNormal, DrawBufferNormal)) {
        sub_38258();
        sub_38290();
        DISPLAY_LOADING();
    }

    endsynchro();
    synchro();
    SWAP_BUFFERS();
    while (nb_fade != 0) {
        endsynchro();
        synchro();
        do_fade(&rvb_plan3, &current_rvb);
    }
}

void END_WORLD_VIGNET(void) {
    // Do loading screen effect (TODO)
    if (sub_38220(9, 100, display_buffer, DrawBufferNormal)) {
        sub_38258();
        sub_38290();
    }
    fade_out(2, &rvb_plan3);
    memcpy(&current_rvb, &rvb_save, 768);
}

/* DOS sub_36FAC is one 316-byte ISR; sub_36FBB labels its body at +15.
 * STRICT: shared min32 sites 1:27048, 1:2706c, 1:27090; direct component
 * expressions preserve the DOS blue-channel allocation and scheduling. */
/* Timer and palette arithmetic: DOS 1:26fbb..2709f.
 * Signed IDIV and JL/JG prove these existing dword bindings are signed.
 * Helper names are INVENTED (Android lookup: absent). */
void __interrupt __far sub_36FAC(void)
{
    s32 i;
    inhibit_interrupts();
    ++timer_phase;
    if (timer_phase % 10 != 0) {
        timer_step += timer_count;
        if (timer_step >= 128) {
            timer_step = 128;
            timer_count = -timer_count;
        } else if (timer_step <= 32) {
            timer_step = 32;
            timer_count = -timer_count;
        }
        for (i = 1; i < 9; ++i) {
            current_rvb.colors[i].r = min32(63, ((s32)rvb_plan3.colors[i].r * timer_step) >> 6);
            current_rvb.colors[i].g = min32(63, ((s32)rvb_plan3.colors[i].g * timer_step) >> 6);
            current_rvb.colors[i].b = min32(63, ((s32)rvb_plan3.colors[i].b * timer_step) >> 6);
        }
        synchro();
        palette_output(1, 8, &current_rvb);
    }
    _chain_intr(old_timer);
    _enable();
}

void sub_370E8(void)
{
    timer_phase = 0;
    timer_step = 0x40;
    timer_count = 4;
    old_timer = _dos_getvect(8);
    _dos_setvect(8, (void (__interrupt __far *)(void))sub_36FAC);
}

void sub_37134(void)
{
    _dos_setvect(8, old_timer);
}

s16 LOAD_BIG_RAYMAN(mem_t* mem) {
    load_big_ray(mem);
    return 1;
}

bool ValidButPressed(void) {
    bool result = 0;
    if (nb_fade == 0) {
        result = (joy_buttonA1 == 1 || (input_mode == 1 && (TOUCHE(SC_ENTER) || TOUCHE(SC_SPACE))));
    }
    return result;
}

u8 StartButPressed(void) {
    u8 result = 0;
    if (input_mode == 1) {
        if (TOUCHE(SC_ENTER)) {
            result = 1;
        }
    } else if (but1pressed()) {
        result = 1;
    }
    return result;
}

u8 PauseButPressed(void) {
    u8 result = 0;
    if (input_mode == 1) {
        if (TOUCHE(SC_NUMLOCK)) {
            result = 1;
        }
    } else if (kbhit()) {
        u8 key = (u8)getch();
        if (key == 'p' || key == 'P') {
            result = 1;
        }
    }
    return result;
}

u8 ExitButPressed(void) {
    u8 result = 0;
    if (input_mode == 1) {
        if (TOUCHE(SC_ESCAPE)) {
            result = 1;
        }
    } else if (kbhit()) {
        if ((u8)getch() == 27) {
            result = 1;
        }
    }
    return result;
}

u8 CancelButPressed(void) {
    u8 result = 0;
    if (input_mode == 1) {
        if (TOUCHE(SC_TAB)) {
            result = 1;
        }
    } else if (kbhit()) {
        if ((u8)getch() == 9) {
            result = 1;
        }
    }
    return result;
}

u8 SelectButPressed(void) {
    u8 result = 0;
    if (input_mode == 1) {
        if (TOUCHE(SC_ESCAPE)) {
            result = 1;
        }
    } else if (kbhit()) {
        if ((u8)getch() == 27) {
            result = 1;
        }
    }
    return result;
}

void ToDoAtExit(void) {
    Reset_Clavier();
    set_speaker_off();
    if (MusicCdActive) {
        DoneMusic();
    }
    if (ModeVideoActuel != MODE_TEXT) {
        InitTextMode();
    }
    printf("\n%s\n", language_txt[231]);
}

void PC_clear_text_input_buffer(void) {
    memset(text_input_buffer, 0, sizeof(text_input_buffer));
}

void PC_init_cheats(void) {



    if (language == 1 /* french keyboard layout */) {
        cheat_code_breakout = &cheat_codes[84];
        cheat_code_breakout_before_victory = cheat_codes;
        cheat_code_unlock_all_levels = &cheat_codes[94];
        cheat_code_level_select = &cheat_codes[18];
        cheat_code_lens = &cheat_codes[34];
        cheat_code_freq100 = &cheat_codes[42];
        cheat_code_freq80 = &cheat_codes[54];
        cheat_code_99_lives = &cheat_codes[108];
        cheat_code_5hp = &cheat_codes[134];
        cheat_code_10_wiz = &cheat_codes[166];
        cheat_code_all_powers = &cheat_codes[124];
        cheat_code_golden_fist = &cheat_codes[150];
        cheat_code_free_movement = &cheat_codes[66];
        cheat_code_win_map = &cheat_codes[178];
        cheat_code_breakout_alt = &cheat_codes[190];
        cheat_code_unlock_all_levels_alt = &cheat_codes[206];
        cheat_code_99_lives_alt = &cheat_codes[224];
        cheat_code_5hp_alt = &cheat_codes[256];
        cheat_code_10_wiz_alt = &cheat_codes[286];
        cheat_code_all_powers_alt = &cheat_codes[242];
        cheat_code_golden_fist_alt = &cheat_codes[270];
        cheat_code_win_map_alt = &cheat_codes[296];
        cheat_code_programmer_message = &cheat_codes[308];
    } else /* regular keyboard layout */{
        cheat_code_breakout = &cheat_codes[89];
        cheat_code_breakout_before_victory = &cheat_codes[9];
        cheat_code_unlock_all_levels = &cheat_codes[101];
        cheat_code_level_select = &cheat_codes[26];
        cheat_code_lens = &cheat_codes[38];
        cheat_code_freq100 = &cheat_codes[48];
        cheat_code_freq80 = &cheat_codes[60];
        cheat_code_99_lives = &cheat_codes[116];
        cheat_code_5hp = &cheat_codes[142];
        cheat_code_10_wiz = &cheat_codes[172];
        cheat_code_all_powers = &cheat_codes[129];
        cheat_code_golden_fist = &cheat_codes[158];
        cheat_code_free_movement = &cheat_codes[75];
        cheat_code_win_map = &cheat_codes[184];
        cheat_code_breakout_alt = &cheat_codes[198];
        cheat_code_unlock_all_levels_alt = &cheat_codes[215];
        cheat_code_99_lives_alt = &cheat_codes[233];
        cheat_code_5hp_alt = &cheat_codes[263];
        cheat_code_10_wiz_alt = &cheat_codes[291];
        cheat_code_all_powers_alt = &cheat_codes[249];
        cheat_code_golden_fist_alt = &cheat_codes[278];
        cheat_code_win_map_alt = &cheat_codes[302];
        cheat_code_programmer_message = &cheat_codes[316];
    }

}

void PC_do_cheats_in_world_map(void) {
    s32 i;
    if (!need_check_cheats) {
        return;
    }
    need_check_cheats = 0;
    if (!dans_la_map_monde || JeuCracker) {
        return;
    }
    if (memcmp(text_input_buffer, cheat_code_breakout, 5) == 0 || memcmp(text_input_buffer, cheat_code_breakout_alt, 8) == 0) {
        if (finBosslevel.mr_dark) {
            PositionStageNameCalcule = 0;
            set_casse_brique_active();
        }
    }
    else if (memcmp(text_input_buffer, cheat_code_breakout_before_victory, 9) == 0) {
        PositionStageNameCalcule = 0;
        set_casse_brique_active();
    }
    else if (memcmp(text_input_buffer, cheat_code_lens, 4) == 0) {
        if (!JumAmpDefMul) {
            JumAmpDefMul = 26;
            JumAmpDefPlus = 32150;
            JumelleZoomActif = 1;
            ParamZoomChange = 1;
        } else {
            JumAmpDefMul = 0;
            JumAmpDefPlus = 0;
            JumelleZoomActif = 1;
            ParamZoomChange = 1;
        }
    }
    else if (memcmp(text_input_buffer, cheat_code_all_powers, 5) == 0 || memcmp(text_input_buffer, cheat_code_all_powers_alt, 7) == 0) {
        RayEvts.poing = 1;
        RayEvts.hang = 1;
        RayEvts.helico = 1;
        RayEvts.grap = 1;
        RayEvts.run = 1;
    }
    else if (memcmp(text_input_buffer, cheat_code_unlock_all_levels, 7) == 0 || memcmp(text_input_buffer, cheat_code_unlock_all_levels_alt, 9) == 0) {
        ALL_WORLD = 0;
        for (i = 0; i < 24; ++i) {
            if (!t_world_info[i].is_unlocked && t_world_info[i].world != world_6_cake) {
                t_world_info[i].is_unlocking = 1;
                chemin_percent = 0;
            }
        }
        // This cheat will normally not unlock Mr Dark's Dare.
        // However, there's an exception: if you are standing on Crystal Palace, Mr Dark's Dare *WILL* unlock!
        if (num_world_choice == 14) {
            for (i = 0; i < 24; ++i) {
                    if (!t_world_info[i].is_unlocked) {
                    t_world_info[i].is_unlocking = 1;
                    chemin_percent = 0;
                }
            }
        }
    }
    else if (memcmp(text_input_buffer, cheat_code_level_select, 8) == 0) {
        ALL_WORLD = 1;
    }
    else if (memcmp(text_input_buffer, cheat_code_freq100, 6) == 0) {
        if (Frequence != 100) {
            if (Frequence != 80) {
                saved_frequence = Frequence;
            }
            Frequence = 100;
            NewFrequency(100);
        } else {
            Frequence = saved_frequence;
            NewFrequency(Frequence);
        }
    }
    else if (memcmp(text_input_buffer, cheat_code_freq80, 6) == 0) {
        if (Frequence != 80) {
            if (Frequence != 100) {
                saved_frequence = Frequence;
            }
            Frequence = 80;
            NewFrequency(80);
        } else {
            Frequence = saved_frequence;
            NewFrequency(Frequence);
        }
    }
}

void PC_do_cheats(void) {
    if (!need_check_cheats) {
        return;
    }
    need_check_cheats = 0;
    if (!RaymanDansUneMapDuJeu || num_world == world_6_cake || JeuCracker || ray_mode == MODE_5_CASSE_BRIQUE) {
        return;
    }

    if (memcmp(text_input_buffer, cheat_code_99_lives, 8) == 0 || memcmp(text_input_buffer, cheat_code_99_lives_alt, 9) == 0) {
        status_bar.lives = 99;
    }
    else if (memcmp(text_input_buffer, cheat_code_5hp, 8) == 0 || memcmp(text_input_buffer, cheat_code_5hp_alt, 7) == 0) {
        ray.hit_points = 4;
        status_bar.max_hitp = 4;
    }
    else if (memcmp(text_input_buffer, cheat_code_all_powers, 5) == 0 || memcmp(text_input_buffer, cheat_code_all_powers_alt, 7) == 0) {
        if (!RayEvts.squashed) {
            RayEvts.poing = 1;
            RayEvts.hang = 1;
            RayEvts.helico = 1;
            RayEvts.grap = 1;
            RayEvts.run = 1;
        }
    }
    else if (memcmp(text_input_buffer, cheat_code_golden_fist, 8) == 0 || memcmp(text_input_buffer, cheat_code_golden_fist_alt, 8) == 0) {
        if (!RayEvts.squashed && RayEvts.poing) {
            u8 state;
            poing_obj->init_sub_etat = 12;
            state = poing_obj->init_sub_etat;
            poing.sub_etat = state;
            save1.poing_sub_etat = state;
            poing_obj->sub_etat = state;
        }
    }
    else if (memcmp(text_input_buffer, cheat_code_10_wiz, 6) == 0 || memcmp(text_input_buffer, cheat_code_10_wiz_alt, 5) == 0) {
        s32 limit = 99;
        s32 count = status_bar.num_wiz + 10;
        limit = min32(limit, count);
        status_bar.num_wiz = limit;
    }
    else if (memcmp(text_input_buffer, cheat_code_win_map, 6) == 0 || memcmp(text_input_buffer, cheat_code_win_map_alt, 6) == 0) {
        if (ray.main_etat == 0 && ray.speed_x == 0 && ray.speed_y == 0 && ray.follow_id == -1) {
            fin_boss = 1;
        }
    }
    else if (memcmp(text_input_buffer, cheat_code_free_movement, 9) == 0) {
        ray_mode = -ray_mode;
        ray.follow_id = -1;
        dead_time = 64;
        set_main_and_sub_etat(&ray, 2, 2);
    }
    else if (memcmp(text_input_buffer, cheat_code_programmer_message, 8) == 0) {
        MessageProgram = 1 - MessageProgram;
    }

}
