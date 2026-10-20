/* input.c (1:31ab0..1:32930: keyboard/joystick/demo record). Names RAYVERSE/ANDROID (sub_* = Rayverse placeholder names); worker tu_input first pass. See config/units.json for statuses. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include <conio.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define INPUT_READ 255
#define SC_ESCAPE 1
#define SC_CONTROL 0x1d
#define SC_NUMLOCK 0x45

typedef s32 (*button_test_fn)(void);
typedef u8 (*key_test_fn)(void);
typedef void (*input_fn)(void);
typedef s32 (*swap_func_t)(u8 *source_buf, u8 *dest_buf, s32 width, s32 height);

typedef struct options_jeu_t {
    button_test_fn test_fire1;
    button_test_fn test_fire0;
    button_test_fn test_button4;
    button_test_fn test_button3;
    u16 jump;
    u16 fist;
    u16 field_14;
    u16 action;
    u16 music_enabled;
    u16 sound_volume;
    u16 is_stereo;
    u16 unknown_1e;
} options_jeu_t;

typedef struct record_t {
    s32 current_offset;
    s32 repeat_length;
    s32 repeat_index;
    s32 length;
    u8 *data;
    u8 is_recording;
    u8 is_playing;
    u8 is_finished;
    u8 unknown_17;
} record_t;

typedef void (__interrupt __far *keyboard_handler_fn)(void);

extern u8 Touche_Enfoncee[128];
extern u8 key_left;
extern u8 key_right;
extern u8 key_down;
extern u8 key_up;
extern u8 key_action;
extern u8 key_fist;
extern u8 key_jump;
extern s16 word_E0CD0;
extern s16 word_E0CBA;
extern s16 word_E0CD4;
extern s16 word_E0CD8;
extern options_jeu_t options_jeu;
extern record_t record;
extern u16 nb_fade;
extern u8 Main_Control;
extern u8 input_mode;
extern u8 ModeDemo;
extern u8 fade;
extern u8 need_check_cheats;
extern u8 text_input_buffer[9];
extern u8 dans_la_map_monde;
extern u8 ModeAutoJumelle;
extern u8 ChangeJumelleSizeOK;
extern u8 PositionStageNameCalcule;
extern u8 byte_967CC;
extern u8 RaymanDansUneMapDuJeu;
extern u8 in_pause;
extern u8 WindowsLance;
extern u8 need_timer;
extern u8 something_with_cfg_25_default_00_joy;
extern u8 is_joypad_mode;
extern u8 VGA_FREQ;
extern s16 joyy0;
extern s16 joyx0;
extern s16 xpadmax;
extern s16 ypadmax;
extern s16 ypadcentre;
extern s16 xpadcentre;
extern s16 xpadmin;
extern s16 ypadmin;
extern s16 joy_buttonB1;
extern s16 joy_buttonB2;
extern s16 joy_buttonA1;
extern s16 joy_buttonA2;
extern s16 notbut[4];
extern s16 joy_rec_up;
extern s16 joy_rec_right;
extern s16 joy_rec_down;
extern s16 joy_rec_left;
extern s16 bux11;
extern s16 bux10;
extern s16 bux01;
extern s16 bux00;
extern s16 auto_joy_left;
extern s16 auto_joy_up;
extern s16 auto_joy_down;
extern s16 auto_joy_right;
extern input_fn Read_Input_Func;
extern keyboard_handler_fn saved_keyboard_interrupt_handler;
extern key_test_fn TEST_KEY_BUTTON0;
extern key_test_fn TEST_KEY_BUTTON1;
extern key_test_fn TEST_KEY_BUTTON2;
extern key_test_fn TEST_KEY_BUTTON3;
extern swap_func_t Swap_And_Test_Joystick;
extern void get_joy_input2_windows1(void);
extern void get_joy_input2_dos1(void);
extern void get_joy_input2_windows2(void);
extern void get_joy_input2_dos2(void);
extern s32 get_joy_input1_dos1(u8 *source_buf, u8 *dest_buf, s32 width, s32 height);
extern s32 get_joy_input1_dos2(u8 *source_buf, u8 *dest_buf, s32 width, s32 height);
extern s32 dos_game_io_port_something(void);
extern s32 dos_game_io_port_something2(void);
extern u8 *unidentified_record_allocator(s32 size);
extern void InitTextMode(void);
extern s16 InitModeXWithFrequency(u8 frequency);
extern void DO_FADE_OUT(void);
extern rgb_palette_t current_rvb;
extern const char input_file_not_found[20];
extern const char record_default_name[5];
extern const char record_greeting[48];
extern const char record_prompt[31];
extern const char record_suffix[5];
extern s16 new_world;
extern s16 reduced_rayman_id;

u8 TestKeyWeapon(void);
u8 TOUCHE(u8 key);
u8 TestKeyJump(void);
u8 TestKeyAction(void);
u16 leftjoy(void);
u16 rightjoy(void);
u16 downjoy(void);
u16 upjoy(void);
s32 but0pressed(void);
s32 but1pressed(void);
s32 but2pressed(void);
s32 but3pressed(void);
s32 butX0pressed(void);
s32 butX1pressed(void);
void sub_4212C(s16 value);
void readinput(void);
void readinput_calibrate_joystick(s16 *xmax, s16 *xmin, s16 *ymax, s16 *ymin);
void readinput_center_joystick(s16 *xcentre, s16 *ycentre);
void update_pad_limits(s16 *xmax, s16 *xmin, s16 *ymax, s16 *ymin,
                       s16 *xcentre, s16 *ycentre);
void sub_4225C(record_t *rec);
void read_record(const char *filename, record_t *rec);
void load_record(record_t *rec);
void sub_42408(u8 buttons);
void do_record(record_t *rec);
void __interrupt __far PC_keyboard_interrupt_handler(void);
void RESET_TOUCHE(u8 key);
void RESET_ALL_TOUCHE(void);
void Init_Clavier(void);
void Reset_Clavier(void);
u8 JoystickPresent(void);

void POINTEUR_BOUTONS_OPTIONS_BIS(void)
{
    switch (options_jeu.jump) {
    case 0:
        options_jeu.test_fire1 = but0pressed;
        TEST_KEY_BUTTON0 = TestKeyJump;
        break;
    case 1:
        options_jeu.test_fire1 = but1pressed;
        TEST_KEY_BUTTON1 = TestKeyJump;
        break;
    case 2:
        options_jeu.test_fire1 = but2pressed;
        TEST_KEY_BUTTON2 = TestKeyJump;
        break;
    case 3:
        options_jeu.test_fire1 = but3pressed;
        TEST_KEY_BUTTON3 = TestKeyJump;
        break;
    default:
        break;
    }

    switch (options_jeu.fist) {
    case 0:
        options_jeu.test_fire0 = but0pressed;
        TEST_KEY_BUTTON0 = TestKeyWeapon;
        break;
    case 1:
        options_jeu.test_fire0 = but1pressed;
        TEST_KEY_BUTTON1 = TestKeyWeapon;
        break;
    case 2:
        options_jeu.test_fire0 = but2pressed;
        TEST_KEY_BUTTON2 = TestKeyWeapon;
        break;
    case 3:
        options_jeu.test_fire0 = but3pressed;
        TEST_KEY_BUTTON3 = TestKeyWeapon;
        break;
    default:
        break;
    }

    switch (options_jeu.field_14) {
    case 0:
        options_jeu.test_button4 = but0pressed;
        break;
    case 1:
        options_jeu.test_button4 = but1pressed;
        break;
    case 2:
        options_jeu.test_button4 = but2pressed;
        break;
    case 3:
        options_jeu.test_button4 = but3pressed;
        break;
    default:
        break;
    }

    switch (options_jeu.action) {
    case 0:
        options_jeu.test_button3 = but0pressed;
        TEST_KEY_BUTTON0 = TestKeyAction;
        break;
    case 1:
        options_jeu.test_button3 = but1pressed;
        TEST_KEY_BUTTON1 = TestKeyAction;
        break;
    case 2:
        options_jeu.test_button3 = but2pressed;
        TEST_KEY_BUTTON2 = TestKeyAction;
        break;
    case 3:
        options_jeu.test_button3 = but3pressed;
        TEST_KEY_BUTTON3 = TestKeyAction;
        break;
    default:
        break;
    }
}

u8 TestKeyWeapon(void)
{
    return TOUCHE(key_fist);
}

u8 TOUCHE(u8 key)
{
    return Touche_Enfoncee[key];
}

u8 TestKeyJump(void)
{
    return TOUCHE(key_jump);
}

u8 TestKeyAction(void)
{
    return TOUCHE(key_action);
}

u16 leftjoy(void)
{
    s16 result = 0;

    if (Main_Control == 2)
        result = joyx0 <= auto_joy_left;
    if (input_mode == 1)
        result |= TOUCHE(key_left) && !TOUCHE(key_right);
    if (record.is_playing)
        result = joy_rec_left;
    return result;
}

u16 rightjoy(void)
{
    s16 result = 0;

    if (Main_Control == 2)
        result = joyx0 >= auto_joy_right;
    if (input_mode == 1)
        result |= TOUCHE(key_right) && !TOUCHE(key_left);
    if (record.is_playing)
        result = joy_rec_right;
    return result;
}

u16 downjoy(void)
{
    s16 result = 0;

    if (Main_Control == 2)
        result = joyy0 >= auto_joy_down;
    if (input_mode == 1)
        result |= TOUCHE(key_down) && !TOUCHE(key_up);
    if (record.is_playing)
        result = joy_rec_down;
    return result;
}

u16 upjoy(void)
{
    s16 result = 0;

    if (Main_Control == 2)
        result = joyy0 <= auto_joy_up;
    if (input_mode == 1)
        result |= TOUCHE(key_up) && !TOUCHE(key_down);
    if (record.is_playing)
        result = joy_rec_up;
    return result;
}

s32 but0pressed(void)
{
    s32 result = 0;

    if (nb_fade == 0) {
        if (!Main_Control || notbut[0] == 1) {
            result = input_mode == 1 && TEST_KEY_BUTTON0();
            if (ModeDemo)
                result |= joy_buttonA1 == 1;
        } else {
            result = joy_buttonA1 == 1 || (input_mode == 1 && TEST_KEY_BUTTON0());
        }
    }
    return result;
}

s32 but1pressed(void)
{
    s32 result = 0;

    if (nb_fade == 0) {
        if (!Main_Control || notbut[1] == 1) {
            result = input_mode == 1 && TEST_KEY_BUTTON1();
            if (ModeDemo)
                result |= joy_buttonA2 == 1;
        } else {
            result = joy_buttonA2 == 1 || (input_mode == 1 && TEST_KEY_BUTTON1());
        }
    }
    return result;
}

s32 but2pressed(void)
{
    s32 result = 0;

    if (nb_fade == 0) {
        if (!Main_Control || notbut[2] == 1) {
            result = input_mode == 1 && TEST_KEY_BUTTON2();
            if (ModeDemo)
                result |= joy_buttonB1 == 1;
        } else {
            result = joy_buttonB1 == 1 || (input_mode == 1 && TEST_KEY_BUTTON2());
        }
    }
    return result;
}

s32 but3pressed(void)
{
    s32 result = 0;

    if (nb_fade == 0) {
        if (!Main_Control || notbut[3] == 1) {
            result = input_mode == 1 && TEST_KEY_BUTTON3();
            if (ModeDemo)
                result |= joy_buttonA1 == 1;
        } else {
            result = joy_buttonB2 == 1 || (input_mode == 1 && TEST_KEY_BUTTON3());
        }
    }
    return result;
}

s32 butX0pressed(void)
{
    s32 result = 0;

    if (nb_fade == 0)
        result = bux01 == 1 || (input_mode == 1 && TEST_KEY_BUTTON2());
    return result;
}

s32 butX1pressed(void)
{
    s32 result = 0;

    if (nb_fade == 0)
        result = bux11 == 1;
    return result;
}

void sub_4212C(s16 value)
{
    word_E0CD0 = value;
}

void readinput(void)
{
    Read_Input_Func();
}

void readinput_calibrate_joystick(s16 *xmax, s16 *xmin, s16 *ymax, s16 *ymin)
{
    Read_Input_Func();

    if (joyx0 < *xmin)
        *xmin = joyx0;
    if (joyy0 < *ymin)
        *ymin = joyy0;
    if (joyx0 > *xmax)
        *xmax = joyx0;
    if (joyy0 > *ymax)
        *ymax = joyy0;
}

void readinput_center_joystick(s16 *xcentre, s16 *ycentre)
{
    if (Main_Control) {
        Read_Input_Func();
        *xcentre = joyx0;
        *ycentre = joyy0;
        word_E0CD4 = joyx0;
        word_E0CD8 = joyy0;
        Main_Control = 2;
    }
}

void update_pad_limits(s16 *xmax, s16 *xmin, s16 *ymax, s16 *ymin,
                       s16 *xcentre, s16 *ycentre)
{
    auto_joy_left = (((*xcentre - *xmin) * 2) / 5) + *xmin;
    auto_joy_right = *xmax - (((*xmax - *xcentre) * 2) / 5);
    auto_joy_up = (((*ycentre - *ymin) * 2) / 5) + *ymin;
    auto_joy_down = *ymax - (((*ymax - *ycentre) * 2) / 5);
}

void sub_4225C(record_t *rec)
{
    s32 progress;

    if (rec->current_offset == 0) {
        progress = 100;
    } else if (rec->is_recording) {
        progress = (100 * (20000 - rec->current_offset)) / 20000;
    } else {
        progress = (100 * (rec->length - rec->current_offset)) / rec->length;
    }
    word_E0CBA = (s16)progress;
}

void read_record(const char *filename, record_t *rec)
{
    s32 file_size;
    s16 handle;

    handle = open(filename, O_BINARY, S_IREAD | S_IWRITE);
    if (handle == -1) {
        printf(input_file_not_found, filename);
        getch();
        return;
    }

    file_size = (s16)filelength(handle);
    rec->data = unidentified_record_allocator(file_size);
    rec->length = file_size;
    read(handle, rec->data, file_size);
    close(handle);
}

void load_record(record_t *rec)
{
    char filename[100];
    register s16 index;
    register u8 character;

    InitTextMode();
    printf(record_greeting);
    printf(record_prompt, record_default_name);

    index = 0;
    do {
        character = getche();
        filename[index++] = (char)character;
    } while (character != '\r' && character != SC_ESCAPE);
    filename[--index] = '\0';

    if (filename[0] == '\0')
        strcpy(filename, record_default_name);
    strcat(filename, record_suffix);

    read_record(filename, rec);
    InitModeXWithFrequency(VGA_FREQ);

    _disable();
    outp(0x3c8, 0);
    {
        s32 i;
        for (i = 0; i < 256 * 3; ++i)
            outp(0x3c9, ((u8 *)&current_rvb)[i]);
    }
    _enable();
}

void sub_42408(u8 buttons)
{
    s16 a2 = 0;

    joy_buttonB2 = 0;
    joy_buttonB1 = 0;
    joy_buttonA1 = 0;

    if (buttons & 1)
        joy_buttonA1 = 1;
    if (buttons & 2)
        joy_buttonB1 = 1;
    if (buttons & 4)
        a2 = 1;
    if (buttons & 8)
        a2 = 1;
    joy_buttonA2 = a2;

    joy_rec_left = (buttons & 0x10) != 0;
    joy_rec_right = (buttons & 0x20) != 0;
    joy_rec_up = (buttons & 0x40) != 0;
    joy_rec_down = (buttons & 0x80) != 0;
}

void do_record(record_t *rec)
{
    s32 current_offset;

    bux00 = joy_buttonA1;
    bux01 = joy_buttonA2;
    bux10 = joy_buttonB1;
    bux11 = joy_buttonB2;

    current_offset = rec->current_offset;
    if (rec->is_playing) {
        if (current_offset == 0) {
            if (reduced_rayman_id == -1 && ModeDemo == 0)
                load_record(rec);
            rec->repeat_index = 0;
            current_offset++;
            sub_42408(rec->data[current_offset - 1]);
            rec->is_finished = 0;
            rec->current_offset = current_offset;
            rec->repeat_length = rec->data[current_offset];
        } else {
            rec->repeat_index++;
            if (rec->repeat_index == rec->repeat_length) {
                if (rec->is_finished || current_offset + 1 >= rec->length) {
                    rec->is_playing = 0;
                    if (ModeDemo == 1) {
                        ModeDemo = 2;
                        new_world = 1;
                        DO_FADE_OUT();
                    }
                } else {
                    current_offset += 2;
                    rec->repeat_index = 0;
                    rec->repeat_length = rec->data[current_offset];
                    rec->current_offset = current_offset;
                }
            }
            sub_42408(rec->data[current_offset - 1]);
        }
    }
}

void __interrupt __far PC_keyboard_interrupt_handler(void)
{
    u8 scancode;
    u8 state;
    u8 value;
    u8 v3;
    s32 i;

    v3 = 0;
    _disable();
    value = (u8)inp(0x21);
    outp(0x21, value | 2);
    scancode = (u8)inp(0x60);

    if (ModeDemo == 1 && (fade & 0x40) != 0) {
        if (scancode == SC_ESCAPE)
            ModeDemo = 4;
        else
            ModeDemo = 3;
    }

    if (!(scancode & 0x80) && (scancode ^ 0x96) != text_input_buffer[0]) {
        need_check_cheats = 1;
        for (i = 8; i > 0; --i)
            text_input_buffer[i] = text_input_buffer[i - 1];
        text_input_buffer[0] = scancode ^ 0x96;
    }

    if (dans_la_map_monde) {
        ModeAutoJumelle = 0;
        ChangeJumelleSizeOK &= ~4;
        if (PositionStageNameCalcule == 1)
            PositionStageNameCalcule = 2;
    }

    if (byte_967CC != 0) {
        if (byte_967CC <= 1) {
            if (scancode == SC_CONTROL)
                byte_967CC = 2;
            else
                byte_967CC = 0;
        } else if (byte_967CC == 2) {
            if (scancode == SC_NUMLOCK && RaymanDansUneMapDuJeu)
                in_pause = !in_pause;
            byte_967CC = 0;
        } else {
            byte_967CC = 0;
        }
    } else if (scancode == 0xe0) {
        byte_967CC = 1;
    } else {
        v3 = 1;
    }

    if (v3) {
        state = (scancode & 0x80) ^ 0x80;
        Touche_Enfoncee[scancode & 0x7f] = state;
    }

    value = (u8)inp(0x61);
    outp(0x61, value | 0x80);
    outp(0x61, value);
    outp(0x20, 0x20);
    value = (u8)inp(0x21);
    outp(0x21, value & 0xfd);
    _enable();
}

void RESET_TOUCHE(u8 key)
{
    Touche_Enfoncee[key] = 0;
}

void RESET_ALL_TOUCHE(void)
{
    u8 index;

    for (index = 0; index < 128; ++index)
        Touche_Enfoncee[index] = 0;
}

void Init_Clavier(void)
{
    RESET_ALL_TOUCHE();
    saved_keyboard_interrupt_handler = _dos_getvect(9);
    if (input_mode == 1)
        _dos_setvect(9, PC_keyboard_interrupt_handler);
}

void Reset_Clavier(void)
{
    _dos_setvect(9, saved_keyboard_interrupt_handler);
}

u8 JoystickPresent(void)
{
    u8 mode;
    u8 port;
    input_fn read_fn;

    if (need_timer != 0 && WindowsLance != 0) {
        mode = 1;
        port = WindowsLance;
        something_with_cfg_25_default_00_joy = port;
        is_joypad_mode = mode;
        Swap_And_Test_Joystick = get_joy_input1_dos1;
        read_fn = get_joy_input2_dos1;
    } else {
        mode = 0;
        port = 0;
        something_with_cfg_25_default_00_joy = 0;
        is_joypad_mode = 0;
        read_fn = get_joy_input2_windows1;
    }
    Read_Input_Func = read_fn;

    if (dos_game_io_port_something() == 0 &&
        dos_game_io_port_something2() == 0) {
        Main_Control = 0;
        joyx0 = 0;
        joyy0 = 0;
        joy_buttonB1 = 0;
        joy_buttonB2 = 0;
        joy_buttonA1 = 0;
        joy_buttonA2 = 0;
        bux00 = 0;
        bux01 = 0;
        bux10 = 0;
        bux11 = 0;
        joy_rec_up = 0;
        joy_rec_right = 0;
        joy_rec_down = 0;
        joy_rec_left = 0;
        word_E0CD0 = 0;
        word_E0CD4 = 0;
        word_E0CD8 = 0;
        auto_joy_left = 0;
        auto_joy_up = 0;
        auto_joy_right = 0;
        auto_joy_down = 0;
    } else {
        if (Main_Control == 0)
            Main_Control = 1;

        if (dos_game_io_port_something() != 0) {
            if (need_timer != 0 && WindowsLance != 0) {
                mode = 1;
                port = WindowsLance;
                Swap_And_Test_Joystick = get_joy_input1_dos1;
                read_fn = get_joy_input2_dos1;
            } else {
                mode = 0;
                port = 0;
                read_fn = get_joy_input2_windows1;
            }
        } else if (need_timer != 0 && WindowsLance != 0) {
            mode = 1;
            port = 1;
            Swap_And_Test_Joystick = get_joy_input1_dos2;
            read_fn = get_joy_input2_dos2;
        } else {
            mode = 0;
            port = 1;
            read_fn = get_joy_input2_windows2;
        }

        is_joypad_mode = mode;
        something_with_cfg_25_default_00_joy = port;
        Read_Input_Func = read_fn;
    }

    return 1;
}
