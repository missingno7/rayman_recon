#include "inline_helpers.h"
/* Worker d_effect1; partial finishing candidate, see NOTES.md.
 * effect.c part 1 (1:27b90..1:28a30: pause/screen effects; sub_* = Rayverse placeholder names, PC-only). Seeded by tools/seed.py, worker tu_effect. dummy_prg is a zero-size compiler alias of dummy_scene_func (identical adjacent bodies, docs/COMPILER.md). */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "lowlevel.h"
/* INVENTED helper name. ORIGINAL_INLINE: VGA indexed register output at DOS 1:282db and 1:28355. */
extern void write_vga_register(u16 port, u16 value);
#pragma aux write_vga_register = "out dx, ax" parm [dx] [ax] modify exact []

#include <string.h>
#pragma intrinsic(memset)

s16 dummy_scene_func(u32 par_0);
extern u8 byte_964CA;
extern u8 byte_964C8;
extern u8 effect_choice;
extern u8 *DrawBufferNormal;
extern s16 myRand(s16 max);
extern u8 sub_38DCC(u16 x, u16 y, s16 scale, u16 width, u8 *arg5, u8 *arg6);
extern u8 effect_helper_6ee54(u16 a1, u16 a2, s16 a3, u16 a4, u8 *a5, u8 *a6);
extern u8 effect_helper_1623c(u16 a1, u16 a2, u16 a3, s16 a4, u16 a5, u8 *a6, u8 *a7);
/* DOS 1:346c2..346d0 consumes both EBX source and ECX destination. */
extern u8 effect_helper_346b4(u8 a1, s16 a2, u8 *source, u8 *destination);
/* DOS 1:634fd..63512 consumes both pointers; 1:63538 reads the u16 first argument. */
extern u8 effect_helper_634f0(u16 a1, s16 a2, u8 *source, u8 *destination);
extern u8 effect_helper_50738(u8 a1, s16 a2, u8 *a3, u8 *a4);
extern u8 effect_helper_6c7cc(u16 a1, u16 a2, u16 a3, s16 a4, u16 a5, u8 *a6, u8 *a7);
extern rgb_palette_t rvb_plan3;
extern void SAVE_PALETTE(rgb_palette_t *palette);
extern void INIT_FADE_IN(void);
extern void DO_FADE_OUT(void);
extern void RESTORE_PALETTE(void);
extern void EFFACE_VIDEO(void);
extern void SYNCHRO_LOOP(s16 (*scene)(u32));
extern void readinput(void);
extern s16 but0pressed(void);
extern s16 but1pressed(void);
extern s16 but2pressed(void);
extern s16 but3pressed(void);
extern u8 TOUCHE(u32 key);
extern s16 sub_38ED4(void);
extern void sub_391C0(void);
extern s16 effect_helper_6ef5c(void);
extern void effect_helper_6f0e0(void);
extern s16 effect_helper_16404(void);
extern void effect_helper_168c8(void);
extern s16 effect_helper_348a4(void);
extern void effect_helper_34760(void);
extern s16 effect_helper_63614(void);
extern void effect_helper_63838(void);
extern s16 effect_helper_507f0(void);
extern void effect_helper_50aa8(void);
extern s16 effect_helper_6c8d4(void);
extern void effect_helper_6c9bc(void);
extern void effect_cleanup_506fc(void);
extern u8 CarteSonAutorisee;
extern u8 FondAutorise;
extern u8 VGA_FREQ;
extern u8 MusicCdActive;
extern u8 in_pause;
extern u8 effect_pause_bloque;
extern s16 effect_speed;
extern s16 effect_pause_count;
extern u16 effect_pause_timer;
extern s32 effect_pause_counter;
extern s32 cdTimeStart;
extern s32 MapTimePause;
extern u32 effect_audio_clock_start;
extern u32 effect_audio_clock_end;
extern u8 *DisplayBufferNormal;
extern u8 AudioStatusByte(void);
extern void effect_set_audio_status(u8 status);
extern void effect_restore_audio_status(u8 status);
extern void start_freeze_snd(void);
extern void stop_freeze_snd(void);
extern void PC_do_cheats(void);
extern void endsynchro(void);
extern void synchro(void);
extern void sub_2748(u32 *clock_value);
#pragma aux sub_2748 parm [esi]
extern void effect_copy_pause_band(u8 *source, u8 *destination, u32 width);
extern void effect_activate(s32 effect, s16 speed);
s16 Do_Effect_Pause_unknown(u32 par_0);
s16 sub_37B90(u32 par_0);
s16 sub_37C40(u32 par_0);
s16 sub_37CDC(u32 par_0);
s16 sub_37D90(u32 par_0);
s16 sub_37E44(u32 par_0);
s16 sub_37EE0(u32 par_0);
s16 sub_37F94(u32 par_0);
u8 sub_380A4(u8 effect, u8 value, u8 *arg3, u8 *arg4);

s16 sub_37B90(u32 par_0)
{
    s16 done;

    readinput();
    fill(DrawBufferNormal);
    if (sub_38ED4() == 0 || but0pressed() != 0 || but1pressed() != 0 ||
        but2pressed() != 0 || but3pressed() != 0 || TOUCHE(0x39) != 0)
        done = 1;
    else
        done = 0;
    sub_391C0();
    return done;
}

void sub_37C00(void)
{
    SAVE_PALETTE(&rvb_plan3);
    if (byte_964CA == 1)
        INIT_FADE_IN();
    SYNCHRO_LOOP(sub_37B90);
    if ((s8)byte_964CA == -1)
        DO_FADE_OUT();
    RESTORE_PALETTE();
}

s16 sub_37C40(u32 par_0)
{
    s16 done;

    readinput();
    if (effect_helper_6ef5c() == 0 || but0pressed() != 0 || but1pressed() != 0 ||
        but2pressed() != 0 || but3pressed() != 0 || TOUCHE(0x39) != 0)
        done = 1;
    else
        done = 0;
    effect_helper_6f0e0();
    return done;
}

void sub_37C98(void)
{
    SAVE_PALETTE(&rvb_plan3);
    if (byte_964CA == 1) {
        EFFACE_VIDEO();
        INIT_FADE_IN();
    }
    SYNCHRO_LOOP(sub_37C40);
    if ((s8)byte_964CA == -1)
        DO_FADE_OUT();
    RESTORE_PALETTE();
}

s16 sub_37CDC(u32 par_0)
{
    s16 done;

    readinput();
    fill(DrawBufferNormal);
    if (effect_helper_16404() == 0 || but0pressed() != 0 || but1pressed() != 0 ||
        but2pressed() != 0 || but3pressed() != 0 || TOUCHE(0x39) != 0)
        done = 1;
    else
        done = 0;
    effect_helper_168c8();
    return done;
}

void sub_37D4C(void)
{
    SAVE_PALETTE(&rvb_plan3);
    if (byte_964CA == 1) {
        EFFACE_VIDEO();
        INIT_FADE_IN();
    }
    SYNCHRO_LOOP(sub_37CDC);
    if ((s8)byte_964CA == -1)
        DO_FADE_OUT();
    RESTORE_PALETTE();
}

s16 sub_37D90(u32 par_0)
{
    s16 done;

    readinput();
    fill(DrawBufferNormal);
    if (effect_helper_348a4() == 0 || but0pressed() != 0 || but1pressed() != 0 ||
        but2pressed() != 0 || but3pressed() != 0 || TOUCHE(0x39) != 0)
        done = 1;
    else
        done = 0;
    effect_helper_34760();
    return done;
}

void sub_37E00(void)
{
    SAVE_PALETTE(&rvb_plan3);
    if (byte_964CA == 1) {
        EFFACE_VIDEO();
        INIT_FADE_IN();
    }
    SYNCHRO_LOOP(sub_37D90);
    if ((s8)byte_964CA == -1)
        DO_FADE_OUT();
    RESTORE_PALETTE();
}

s16 sub_37E44(u32 par_0)
{
    s16 done;

    readinput();
    if (effect_helper_63614() == 0 || but0pressed() != 0 || but1pressed() != 0 ||
        but2pressed() != 0 || but3pressed() != 0 || TOUCHE(0x39) != 0)
        done = 1;
    else
        done = 0;
    effect_helper_63838();
    return done;
}

void sub_37E9C(void)
{
    SAVE_PALETTE(&rvb_plan3);
    if (byte_964CA == 1) {
        EFFACE_VIDEO();
        INIT_FADE_IN();
    }
    SYNCHRO_LOOP(sub_37E44);
    if ((s8)byte_964CA == -1)
        DO_FADE_OUT();
    RESTORE_PALETTE();
}

s16 sub_37EE0(u32 par_0)
{
    s16 done;

    readinput();
    fill(DrawBufferNormal);
    if (effect_helper_507f0() == 0 || but0pressed() != 0 || but1pressed() != 0 ||
        but2pressed() != 0 || but3pressed() != 0 || TOUCHE(0x39) != 0)
        done = 1;
    else
        done = 0;
    effect_helper_50aa8();
    return done;
}

void sub_37F50(void)
{
    SAVE_PALETTE(&rvb_plan3);
    if (byte_964CA == 1) {
        EFFACE_VIDEO();
        INIT_FADE_IN();
    }
    SYNCHRO_LOOP(sub_37EE0);
    if ((s8)byte_964CA == -1)
        DO_FADE_OUT();
    RESTORE_PALETTE();
}

s16 sub_37F94(u32 par_0)
{
    s16 done;

    readinput();
    if (effect_helper_6c8d4() == 0 || but0pressed() != 0 || but1pressed() != 0 ||
        but2pressed() != 0 || but3pressed() != 0 || TOUCHE(0x39) != 0)
        done = 1;
    else
        done = 0;
    effect_helper_6c9bc();
    return done;
}

void sub_37FEC(void)
{
    SAVE_PALETTE(&rvb_plan3);
    if (byte_964CA == 1) {
        EFFACE_VIDEO();
        INIT_FADE_IN();
    }
    SYNCHRO_LOOP(sub_37F94);
    if ((s8)byte_964CA == -1)
        DO_FADE_OUT();
    RESTORE_PALETTE();
}

u8 sub_38030(void)
{
    if (byte_964CA == 1)
        return (u8)(myRand(6) + 2);
    return (u8)(myRand(7) + 1);
}

u8 sub_38054(void)
{
    return (u8)(myRand(11) + 1);
}

u8 sub_38064(void)
{
    return (u8)(myRand(14) + 1);
}

u8 sub_38074(void)
{
    return (u8)(myRand(6) + 1);
}

u8 sub_380A4(u8 effect, u8 value, u8 *arg3, u8 *arg4)
{
    s16 scale = 1;


    if (effect == 9) {
        effect = sub_38030();
        if (effect == 2 || effect == 4)
            value = sub_38054();
        else if (effect == 6)
            value = sub_38064();
    }
    if (effect == 4)
        effect = 2;

    scale *= (s16)(s8)byte_964CA;
    byte_964C8 = value;
    effect_choice = effect;
    switch (effect) {
    case 1:
        return sub_38DCC(8, 8, scale, 120, arg3, arg4);
    case 3:
        return effect_helper_6ee54(20, 10, scale, 400, arg3, arg4);
    case 5:
        return effect_helper_1623c(20, 8, 61, scale * 2, 50, arg3, arg4);
    case 2:
        if (value == 13 || value == 100) {
            byte_964C8 = sub_38054();
        }
        return effect_helper_346b4(byte_964C8, scale * 2, arg3, arg4);
    case 8:
        return effect_helper_634f0(22, scale * 2, arg3, arg4);
    case 6:
        if (value == 16 || value == 100) {
            byte_964C8 = sub_38064();
        }
        return effect_helper_50738(byte_964C8, scale * 4, arg3, arg4);
    case 7:
        return effect_helper_6c7cc(160, 100, 48, scale * 4, 1000, arg3, arg4);
    default:
        return 0;
    }
}

u8 sub_38208(u8 effect, u8 value, u8 *unused_1, u8 *unused_2)
{
    byte_964CA = 1;
    return sub_380A4(effect, value, unused_1, unused_2);
}

u8 sub_38220(u8 effect, u8 value, u8 *unused_1, u8 *unused_2)
{
    byte_964CA = 0xff;
    return sub_380A4(effect, value, unused_1, unused_2);
}

extern void effect_cleanup_34688(void);
extern void effect_cleanup_6ee28(void);
extern void effect_cleanup_16200(void);
extern void effect_cleanup_506fc(void);
extern void effect_cleanup_6c7a0(void);
extern void effect_cleanup_634c4(void);
extern void sub_38DA0(void);

void sub_38258(void)
{
    switch (effect_choice) {
    case 1: sub_37C00(); return;
    case 2: sub_37E00(); return;
    case 3: sub_37C98(); return;
    case 5: sub_37D4C(); return;
    case 6: sub_37F50(); return;
    case 7: sub_37FEC(); return;
    case 8: sub_37E9C(); return;
    /* No action for selector 4 or an unsupported selector. */
    case 4:
    default: return;
    }
}

void sub_38290(void)
{
    switch (effect_choice) {
    case 1: sub_38DA0(); return;
    case 2: effect_cleanup_34688(); return;
    case 3: effect_cleanup_6ee28(); return;
    case 5: effect_cleanup_16200(); return;
    case 6: effect_cleanup_506fc(); return;
    case 7: effect_cleanup_6c7a0(); return;
    case 8: effect_cleanup_634c4(); return;
    /* No action for selector 4 or an unsupported selector. */
    case 4:
    default: return;
    }
}

extern u8 GameModeVideo;
extern u8 *EffetBufferNormal;
extern u8 *display_buffer;
extern u8 *draw_buffer;
extern char *language_txt[304];
extern void display_text(const char *text, s16 x, s16 y, u8 font_size, s8 color);

typedef union vga_register {
    u16 word;
    struct { u8 index; u8 value; } byte;
} vga_register_t;

void SaveScreen(u8 *src, u8 *dst)
{
    if (GameModeVideo == 1) {
        u8 plane;
        vga_register_t reg;

        plane = 0;
        do {
            u32 plane_offset;

            plane_offset = plane * 0x3e80;
            reg.byte.value = plane;
            reg.byte.index = 4;
            write_vga_register(0x3ce, reg.word);
            memcpy(dst + plane_offset, src, 0x3e80);
            ++plane;
        } while (plane < 4);
    } else {
        memcpy(dst, src, 0xfa00);
    }
}

void RestoreScreen(u8 *src, u8 *dst)
{
    if (GameModeVideo == 1) {
        u8 plane;
        u8 plane_mask;
        vga_register_t reg;

        plane_mask = GameModeVideo;
        plane = 0;
        do {
            reg.byte.value = plane_mask;
            reg.byte.index = 2;
            write_vga_register(0x3c4, reg.word);
            memcpy(dst, src + plane * 0x3e80, 0x3e80);
            plane_mask <<= 1;
            ++plane;
        } while (plane < 4);
    } else {
        memcpy(dst, src, 0xfa00);
    }
}

void DO_AFFICHE_PAUSE(void)
{
    s32 xmin;
    s32 xmax;
    s32 ymin;
    s32 ymax;

    get_sprite_clipping(&xmin, &xmax, &ymin, &ymax);
    sprite_clipping(0, 320, 0, 200);
    display_text(language_txt[160], 160, 100, 2, 1);
    sprite_clipping(xmin, xmax, ymin, ymax);
}

/* The DOS public at 1:28400 aliases dummy_scene_func. */
s16 dummy_prg(u32 par_0)
{
    return 1;
}

s16 dummy_scene_func(u32 par_0)
{
    return 1;
}

void Do_Effect_Pause_Simple(void)
{
    if (FondAutorise == 2) {
        DO_AFFICHE_PAUSE();
        SYNCHRO_LOOP(dummy_scene_func);
        {
            u8 frame = 0;
            while (frame < 12) {
                endsynchro();
                ++frame;
                synchro();
            }
        }
        do {
            endsynchro();
            synchro();
        } while (in_pause != 0);
        return;
    }

    SaveScreen(display_buffer, EffetBufferNormal);
    RestoreScreen(EffetBufferNormal, draw_buffer);
    DO_AFFICHE_PAUSE();
    SYNCHRO_LOOP(dummy_scene_func);
    for (;;) {
        u8 touch_count = 0;

        effect_speed = 15;
        while (effect_speed != 0 && touch_count <= 7) {
            endsynchro();
            synchro();
            if (TOUCHE(0x36) != 0 && TOUCHE(0x2a) != 0)
                ++touch_count;
            --effect_speed;
        }
        if (touch_count >= 12)
            touch_count = 0;
        if (touch_count != 0) {
            effect_pause_bloque ^= 1;
            if (GameModeVideo == 0) {
                RestoreScreen(EffetBufferNormal, draw_buffer);
                if (effect_pause_bloque == 0)
                    DO_AFFICHE_PAUSE();
            }
            SYNCHRO_LOOP(dummy_scene_func);
        }
        PC_do_cheats();
        if (in_pause == 0)
            break;
    }
    if (GameModeVideo == 0) {
        RestoreScreen(EffetBufferNormal, draw_buffer);
        SYNCHRO_LOOP(dummy_scene_func);
    }
}

s16 Do_Effect_Pause_unknown(u32 par_0)
{
    u8 effect;
    s32 duration;

    PC_do_cheats();
    /* Keep the cadence calculation for both initial-phase comparisons. */
    duration = VGA_FREQ * 7;
    if (effect_pause_count < duration) {
        ++effect_pause_count;
        effect_copy_pause_band(EffetBufferNormal, DrawBufferNormal, 0xa0);
        effect_copy_pause_band(EffetBufferNormal + 0xc800,
                               DrawBufferNormal + 0xa0, 0xa0);
        if (TOUCHE(0x36) != 0 && TOUCHE(0x2a) != 0) {
            ++effect_pause_counter;
            if (effect_pause_counter >= 5) {
                effect_pause_counter = 0;
                effect_pause_bloque ^= 1;
            }
        } else {
            effect_pause_counter = 0;
        }
        if (effect_pause_bloque == 0)
            DO_AFFICHE_PAUSE();
        goto finalize;
    } else if (effect_pause_count == duration) {
        ++effect_pause_count;
        effect_pause_timer = 0;
        effect_pause_bloque = 0;
        effect_pause_counter = 0;
    }

    if (effect_pause_bloque == 0) {
        if (effect_pause_counter >= 20 && TOUCHE(0x36) != 0 &&
            TOUCHE(0x2a) != 0)
            effect_pause_bloque = 1;
        if (effect_pause_timer == 0) {
            fill(DrawBufferNormal);
            effect_pause_counter = (u16)effect_helper_507f0();
            effect_helper_50aa8();
            DO_AFFICHE_PAUSE();
            if (effect_pause_counter == 0) {
                effect_speed = (u16)(0 - effect_speed);
                effect = sub_38074();
                byte_964C8 = effect;
                effect_activate(effect, (s16)effect_speed);
                if ((s16)effect_speed < 0) {
                    effect_copy_pause_band(EffetBufferNormal,
                                           DrawBufferNormal, 0xa0);
                    effect_copy_pause_band(EffetBufferNormal + 0xc800,
                                           DrawBufferNormal + 0xa0, 0xa0);
                    DO_AFFICHE_PAUSE();
                    effect_pause_timer = VGA_FREQ;
                }
            }
        } else {
            --effect_pause_timer;
        }
    } else if (effect_pause_counter != 0 || (s16)effect_speed >= 0) {
        fill(DrawBufferNormal);
        effect_pause_counter = (u16)effect_helper_507f0();
        effect_helper_50aa8();
        if (effect_pause_counter != 0)
            goto finalize;
        effect_speed = (u16)(0 - effect_speed);
        effect = sub_38074();
        byte_964C8 = effect;
        effect_activate(effect, (s16)effect_speed);
    } else {
        effect_copy_pause_band(EffetBufferNormal, DrawBufferNormal, 0xa0);
        effect_copy_pause_band(EffetBufferNormal + 0xc800,
                               DrawBufferNormal + 0xa0, 0xa0);
        endsynchro();
        synchro();
        {
            /* DOS 1:28810 and 1:288f2 read destination before source. */
            u8 *display = DisplayBufferNormal;
            u8 *draw = DrawBufferNormal;
            copy(display, draw, 0xfa00);
        }
        for (;;) {
            if (TOUCHE(0x36) != 0 && TOUCHE(0x2a) != 0)
                break;
            if (in_pause == 0)
                break;
            PC_do_cheats();
        }
        effect_pause_bloque = 0;
        effect_pause_timer = 0;
        if (in_pause != 0) {
            effect_pause_counter = 0;
            effect_speed = -4;
            effect = sub_38074();
            byte_964C8 = effect;
            effect_activate(effect, (s16)effect_speed);
        }
    }

finalize:
    if (in_pause == 0) {
        effect_copy_pause_band(EffetBufferNormal, DrawBufferNormal, 0xa0);
        effect_copy_pause_band(EffetBufferNormal + 0xc800,
                               DrawBufferNormal + 0xa0, 0xa0);
        endsynchro();
        synchro();
        {
            /* DOS 1:28810 and 1:288f2 read destination before source. */
            u8 *display = DisplayBufferNormal;
            u8 *draw = DrawBufferNormal;
            copy(display, draw, 0xfa00);
        }
    }
    return (in_pause == 0);
}

void Do_Effect_Pause(void)
{
    u8 effect;

    effect_pause_bloque = 0;
    if (CarteSonAutorisee != 0)
        start_freeze_snd();
    if (MusicCdActive != 0) {
        sub_2748(&effect_audio_clock_start);
        effect_set_audio_status(AudioStatusByte());
    }
    if (GameModeVideo == 0) {
        effect = sub_38074();
        byte_964C8 = effect;
        effect_speed = -4;
        if (effect_helper_50738(effect, -4, DrawBufferNormal,
                                DrawBufferNormal) != 0) {
            effect_pause_count = 0;
            effect_pause_timer = 0;
            SYNCHRO_LOOP(Do_Effect_Pause_unknown);
            effect_cleanup_506fc();
        } else {
            Do_Effect_Pause_Simple();
        }
    } else {
        Do_Effect_Pause_Simple();
    }
    if (MusicCdActive != 0) {
        sub_2748(&effect_audio_clock_end);
        cdTimeStart += (s32)(effect_audio_clock_end - effect_audio_clock_start);
        effect_restore_audio_status(AudioStatusByte());
    }
    if (CarteSonAutorisee != 0)
        stop_freeze_snd();
    in_pause = 0;
    MapTimePause = 0;
}

