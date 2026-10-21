/* 1:25f50..1:27b90: cheat.c + display.c tail (level/vignette loading, init/dead/map loops, PC cheats, exit). Names RAYVERSE/ANDROID (sub_* = Rayverse placeholders); seeded by tools/seed.py, worker tu_cheat. Four interrupt/protected-mode handlers (sub_36FAC..sub_37134) not yet reconstructed (Watcom __interrupt C or assembly: OPEN). */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "lowlevel.h"
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <i86.h>

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
void LoadPcxOptions(u8* buffer, s32 resource_id, s16* width, s16* height, rgb_palette_t* pal_to_save);
void RAY_DEMIRAY(void);
void Reset_Clavier(void);
u8 TOUCHE(u8 key);
s16 but1pressed(void);
void do_record(record_t *rec);
s32 clock_ticks(void);
void *unidentified_record_allocator(s32 size);
void release_record(void *memory);
s16 CalcSpeed(void);
u8 confirmation_msg(s32 msg);
extern void exit();
void fade_out(s16 speed, rgb_palette_t* palette);
u8 get_casse_brique_active(void);
void load_big_ray(mem_t* buffer);
void readinput(void);
u8 sub_38220(s32 a1, s32 a2, u8* a3, u8* a4);
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
u8 StartButPressed(void);
u8 PauseButPressed(void);
u8 ExitButPressed(void);
u8 CancelButPressed(void);
u8 SelectButPressed(void);
void ToDoAtExit(void);
void PC_clear_text_input_buffer(void);

extern s32 plan2bit_length;
extern s32 plan2bit_nb_bytes;
extern s32 plan3bit_length;
extern s32 plan3bit_nb_bytes;
extern u8 *PLAN2BIT;
extern u8 *PLAN3BIT;
extern u8 *PLANVIGBIT[7];
extern s32 planVigbit_length[7];
extern s32 planVigbit_nb_bytes[7];
extern rgb_palette_t rvb_Vig[7];
extern rgb_palette_t rvb_pres;
extern rgb_palette_t rvb_plan3;
extern rgb_palette_t current_rvb;
extern u16 nb_fade;
extern void start_fade_in(s16 speed, rgb_palette_t *palette);
extern void do_fade(rgb_palette_t *from, rgb_palette_t *to);
extern s16 display_mode;
extern u8 need_timer;
extern s16 word_CF7E6;
extern s16 word_CF7EE;
extern s16 word_CF7F0;
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
extern char level_filename[20];
extern mem_t main_mem_fix;
extern mem_t main_mem_sprite;
extern u16 path_has_changed;
extern mem_t demo_video_mem;
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
extern char *game_title;
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
extern u8 cheat_codes[324];
extern u8 chemin_percent;
extern u8 dans_la_map_monde;
extern u8 dead_time;
extern u8* draw_buffer;
extern finbosslevel_t finBosslevel;
extern u8 fin_boss;
extern u8 gele;
extern s16 joy_buttonA1;
extern u8 language;
extern char* language_txt[300];
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
    s32 *times;
    s32 *intervals;
    s32 sum;
    s32 average;
    s32 low_limit;
    s32 high_limit;
    s32 interval;
    s16 i;
    s16 j;
    s16 outliers;
    u8 valid;

    times = (s32*)unidentified_record_allocator(0xfa0);
    if (times != 0) {
        for (i = 0; i < 1000; ++i) {
            for (j = 0; j < 500; ++j) {
            }
            times[i] = clock_ticks();
        }
        intervals = (s32*)unidentified_record_allocator(0xf9c);
        if (intervals != 0) {
            valid = 1;
            sum = 0;
            for (i = 0; i < 999; ++i) {
                interval = times[i + 1] - times[i];
                intervals[i] = interval;
                if (interval <= 0) {
                    valid = 0;
                    break;
                }
                sum += interval;
            }
            if (valid) {
                average = sum / 999;
                low_limit = (average * 3) / 4;
                high_limit = (average * 4) / 3;
                outliers = 0;
                for (i = 0; i < 999; ++i) {
                    if (intervals[i] < low_limit || intervals[i] > high_limit) {
                        ++outliers;
                    }
                }
                need_timer = (outliers < 10);
            }
            release_record(intervals);
        }
        release_record(times);
    }
}

void sub_3609C(void) {
    s32 start_ticks;
    s32 end_ticks;
    s16 i;
    s16 sample_count;
    s16 frame_time;

    if (need_timer == 0) {
        return;
    }
    endsynchro();
    start_ticks = clock_ticks();
    wait_until(start_ticks + 3000);
    synchro();
    start_ticks = clock_ticks();
    sample_count = VGA_FREQ / 2;
    for (i = 0; i < sample_count; ++i) {
        endsynchro();
        wait_until(clock_ticks() + 3000);
        synchro();
    }
    end_ticks = clock_ticks();
    frame_time = (s16)((end_ticks - start_ticks) / i);
    word_CF7F0 = frame_time;
    word_CF7EE = (s16)((frame_time * 31) / 32);
    word_CF7E6 = (s16)(word_CF7EE + display_mode * frame_time);
}

void InitPcSoundCard(void) {
    s32 start_ticks;
    s32 end_ticks;
    s32 limit;
    u8 failed;
    u8 attempts;

    _disable();
    display_mode = 0;
    failed = 0;
    attempts = 0;
    speaker_enable();
    sub_35F70();
    while (need_timer != 0 && failed == 0) {
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
    _enable();
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
    DrawBufferNormal = (u8 *)malloc(64064);
    EffetBufferNormal = (u8*)malloc(128000);
    if (!DrawBufferNormal || !EffetBufferNormal) {
        FatalError("Memory error in draw_buffer.\n");
    }
    InitMatriceJumelle();
    if (Frequence == 70) {
        VGA_FREQ = Frequence;
    } else {
        VGA_FREQ = 60;
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
    if (record.current_offset != 0 || record.is_playing || record.is_recording) {
        return;
    }
    do_record(&record); //TODO
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
    //stub: there is a DRM check here
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
    s16 vignet = -1;
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
            } else if (num_level == 11) {
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

void LOAD_CREDITS_VIGNET(mem_t *mem1, mem_t *mem2, s16 unused)
{
    s32 vignette_ids[8] = {52, 58, 55, 53, 56, 54, 59, 57};
    s32 i;

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
    if (num_world_choice == num_world) {
        if (get_casse_brique_active()) {
            num_level = 0;
            new_world = 1;
        } else {
            new_world = 0;
        }
        new_level = 1;
    } else {
        num_level = 0;
        new_world = 1;
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
    //sub_6BAE0();
    new_world = 0;
    num_world = num_world_choice;
    new_level = 1;
    num_level = num_level_choice;
    if (!fin_du_jeu) {

        if (byte_CFA2A) {
            s16 text_x;
            s16 text_y;
            text_x = 160;
            text_y = 190;
            if (get_casse_brique_active()) {
                text_x = 240;
                text_y = 160;
            }
            memset(draw_buffer, 0, 320*200);
            start_fade_in(2, &rvb_plan3);
            display_text(language_txt[149], text_x, text_y, 2, 0); // /loading../
            synchro();
            endsynchro();
            SWAP_BUFFERS();
            do {
                synchro();
                endsynchro();
                do_fade(&rvb_plan3, &current_rvb);
            } while (nb_fade != 0);
        }

        if (MusicCdActive) {
            cdTime = 0;
        }

        file_level(num_level, level_filename);
        block_free(&main_mem_level);
        SpriteWorldBlocksFree(&main_mem_sprite);
        LOAD_VIGNET_GAME(&main_mem_level);
        load_level(&main_mem_level, num_world, level_filename);
        INIT_FND();

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
    s16 width = 0;
    s16 height = 0;
    LoadPcxOptions(EffetBufferNormal, 13, &width, &height, &rvb_plan3);
    //LoadPcxPaletteInVignet(13, &rvb_plan3); // opening twice no longer needed (we saved the palette in LoadPcxOptions)
}

void LOAD_OPTIONS_SCREEN(void) {
    s16 width = 0;
    s16 height = 0;
    LoadPcxOptions(EffetBufferNormal, 12, &width, &height, &rvb_plan3);
    //LoadPcxPaletteInVignet(12, &rvb_plan3);
}

void HANDLE_KEY(u8* a1) {
    if (input_mode != 1) {
        if (kbhit() && !ModeDemo) {
            *a1 = (u8)getch();
        } else {
            *a1 = 255;
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
        DEPART_INIT_LOOP();
        return;
    }
    doneGameSave();
    block_free(&main_mem_world);
    block_free(&main_mem_sprite);
}

void FIN_MAP_LOOP(void) {
    if (RayEvts.tiny) {
        RAY_DEMIRAY();
    }
    textmode();
    if (JeuCracker) {
        exit(0);
    }
}

void FIN_DEAD_LOOP(void) {
    //nullsub
}

void END_GAME(void) {
    //sub_6BB48();
    InitTextMode();
    if (record.is_recording) {
        Reset_Clavier();
        record.is_finished = 1;
        do_record(&record); // TODO
    }
    if (fin_du_jeu) {
        block_free(&main_mem_level);
        block_free(&main_mem_world);
        block_free(&main_mem_sprite);
        block_free(&main_mem_fix);
        //free(DrawBufferNormal); // not needed, backed by global_app_state.game.draw_buffer.memory
        if (FondAutorise != 2) {
            free(EffetBufferNormal);
        }

    }
    block_free(&temp_mem_buf);
    //sub_6BB48();
    //exit(0);
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
    rvb_save = current_rvb;
    current_rvb = rvb_plan3;
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

/* sub_36FAC: no seed source */

/* sub_36FBB: no seed source */

/* sub_370E8: no seed source */

/* sub_37134: no seed source */

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
    printf("\n%s\n", game_title);
}

void PC_clear_text_input_buffer(void) {
    memset(text_input_buffer, 0, sizeof(text_input_buffer));
}

void PC_init_cheats(void) {
#if 0
    // debug: decode cheat code table (see data.c)
    u8 decoded_cheat_codes[327];
    for (s32 i = 0; i < sizeof(cheat_codes); ++i) {
        decoded_cheat_codes[i] = cheat_codes[i] ^ 0x96;
    }

    u8 letters[] = {SC_A, SC_B, SC_C, SC_D, SC_E, SC_F, SC_G, SC_H, SC_I, SC_J, SC_K, SC_L, SC_M, SC_N, SC_O,
                    SC_P, SC_Q, SC_R, SC_S, SC_T, SC_U, SC_V, SC_W, SC_X, SC_Y, SC_Z};
    u8 numbers[] = {SC_0, SC_1, SC_2, SC_3, SC_4, SC_5, SC_6, SC_7, SC_8, SC_9};

    s32 num_lines = (327 / 10) + 1;
    for (s32 line = 0; line < num_lines; ++line) {
        s32 i = line * 10;
        s32 line_len = (327 - i >= 10) ? 10 : 327 - i;
        for (s32 j = 0; j < line_len; ++j) {
            u8 c = decoded_cheat_codes[i+j];
            bool printed = 0;
            for (s32 k = 0; k < COUNT(letters); ++k) {
                if (c == letters[k]) {
                    printf("_(SC_%c), ", 'A'+k);
                    printed = 1;
                    break;
                }
            }
            if (!printed) {
                for (s32 k = 0; k < COUNT(numbers); ++k) {
                    if (c == numbers[k]) {
                        printf("_(SC_%c), ", '0'+k);
                        printed = 1;
                        break;
                    }
                }
            }
            if (!printed && c == SC_TAB) {
                printf("_(SC_TAB), ");
                printed = 1;
            }
            if (!printed && c == SC_BACKSPACE) {
                printf("_(SC_BACKSPACE), ");
                printed = 1;
            }
            if (!printed && c == SC_SPACE) {
                printf("_(SC_SPACE), ");
                printed = 1;
            }
            if (!printed) {
                printf("_(0x%x), ", c);
//                printf("%d, ", cheat_codes[i+j]);
            }
        }
        putc('\n', stdout);
    }
#endif


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
    u8* world_info_state;
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
        if (JumAmpDefMul) {
            JumAmpDefMul = 0;
            JumAmpDefPlus = 0;
        } else {
            JumAmpDefMul = 26;
            JumAmpDefPlus = 32150;
        }
        JumelleZoomActif = 1;
        ParamZoomChange = 1;
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
        world_info_state = &t_world_info[0].state;
        for (i = 0; i < 0x1e0; i += 20) {
            if ((world_info_state[i] & 1) == 0 && world_info_state[i + 2] != world_6_cake) {
                world_info_state[i] |= 4;
                chemin_percent = 0;
            }
        }
        // This cheat will normally not unlock Mr Dark's Dare.
        // However, there's an exception: if you are standing on Crystal Palace, Mr Dark's Dare *WILL* unlock!
        if (num_world_choice == 14) {
            for (i = 0; i < 0x1e0; i += 20) {
                if ((world_info_state[i] & 1) == 0) {
                    world_info_state[i] |= 4;
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
            Frequence = 80;
            NewFrequency(80);
        } else {
            Frequence = saved_frequence;
            NewFrequency(saved_frequence);
        }
    }
    else if (memcmp(text_input_buffer, cheat_code_freq80, 6) == 0) {
        if (Frequence != 80) {
            if (Frequence != 100) {
                saved_frequence = Frequence;
            }
            Frequence = 100;
            NewFrequency(100);
        } else {
            Frequence = saved_frequence;
            NewFrequency(saved_frequence);
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
    else if (memcmp(text_input_buffer, cheat_code_golden_fist, 8) == 0 || memcmp(text_input_buffer, cheat_code_golden_fist_alt, 5) == 0) {
        if (!RayEvts.squashed && RayEvts.poing) {
            poing_obj->init_sub_etat = 12;
            poing.sub_etat = poing_obj->init_sub_etat;
            save1.poing_sub_etat = poing_obj->init_sub_etat;
            poing_obj->sub_etat = poing_obj->init_sub_etat;
        }
    }
    else if (memcmp(text_input_buffer, cheat_code_10_wiz, 6) == 0 || memcmp(text_input_buffer, cheat_code_10_wiz_alt, 5) == 0) {
        status_bar.num_wiz = (status_bar.num_wiz + 10 < 99) ? status_bar.num_wiz + 10 : 99;
    }
    else if (memcmp(text_input_buffer, cheat_code_win_map, 6) == 0 || memcmp(text_input_buffer, cheat_code_win_map_alt, 6) == 0) {
        if (ray.main_etat == 0 && ray.speed_x == 0 && ray.speed_y == 0 && ray.follow_id == -1) {
            fin_boss = 1;
        }
    }
    else if (memcmp(text_input_buffer, cheat_code_free_movement, 9) == 0) {
        ray.follow_id = -1;
        ray_mode = -ray_mode;
        dead_time = 64;
        set_main_and_sub_etat(&ray, 2, 2);
    }
    else if (memcmp(text_input_buffer, cheat_code_programmer_message, 9) == 0) {
        MessageProgram = 1 - MessageProgram;
    }

}
