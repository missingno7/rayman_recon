/* effect.c part 1 (1:27b90..1:28a30: pause/screen effects; sub_* = Rayverse placeholder names, PC-only). Seeded by tools/seed.py, worker tu_effect. Some functions not yet reconstructed. */
#include "obj.h"
#include "game.h"
#include "map.h"
#include "lowlevel.h"
#include <conio.h>
#include <string.h>

s16 dummy_scene_func(u32 par_0);
extern u8 byte_964CA;
extern u8 byte_964C8;
extern u8 effect_choice;
extern u8 *DrawBufferNormal;
extern s16 myRand(s16 max);
extern u8 sub_38DCC(s32 x, s32 y, s32 scale, s32 width, u8 *arg5, u8 *arg6);
extern u8 effect_helper_6ee54(s32 a1, s32 a2, s32 a3, s32 a4, u8 *a5, u8 *a6);
extern u8 effect_helper_1623c(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, u8 *a6, u8 *a7);
extern u8 effect_helper_346b4(s32 a1, s32 a2);
extern u8 effect_helper_634f0(s32 a1, s32 a2);
extern u8 effect_helper_50738(s32 a1, s32 a2);
extern u8 effect_helper_6c7cc(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, u8 *a6, u8 *a7);
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
    u32 i;

    readinput();
    for (i = 0; i < 0x3e80; ++i)
        ((u32 *)DrawBufferNormal)[i] = 0;
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
    u32 i;

    readinput();
    for (i = 0; i < 0x3e80; ++i)
        ((u32 *)DrawBufferNormal)[i] = 0;
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
    u32 i;

    readinput();
    for (i = 0; i < 0x3e80; ++i)
        ((u32 *)DrawBufferNormal)[i] = 0;
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
    u32 i;

    readinput();
    for (i = 0; i < 0x3e80; ++i)
        ((u32 *)DrawBufferNormal)[i] = 0;
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
    u8 effect_local;
    u8 value_local;
    s32 scale = 1;

    effect_local = effect;
    value_local = value;
    if (effect_local == 9) {
        effect_local = sub_38030();
        if (effect_local == 2 || effect_local == 4)
            effect_local = sub_38054();
        else if (effect_local == 6)
            effect_local = sub_38064();
    }
    if (effect_local == 4)
        effect_local = 2;

    scale *= (s8)byte_964CA;
    byte_964C8 = value_local;
    effect_choice = effect_local;
    switch (effect_local) {
    case 1:
        return sub_38DCC(8, 8, scale, 120, arg3, arg4);
    case 2:
        return effect_helper_6ee54(20, 10, scale, 400, arg3, arg4);
    case 3:
        return effect_helper_1623c(20, 8, 61, scale * 2, 50, arg3, arg4);
    case 4:
        if (value_local == 13 || value_local == 100) {
            value_local = sub_38054();
            byte_964C8 = value_local;
        }
        return effect_helper_346b4(byte_964C8, scale * 2);
    case 5:
        return effect_helper_634f0(22, scale * 2);
    case 6:
        if (value_local == 16 || value_local == 100) {
            value_local = sub_38064();
            byte_964C8 = value_local;
        }
        return effect_helper_50738(byte_964C8, scale * 4);
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

/* sub_38258: no seed source */

/* sub_38290: no seed source */

extern u8 GameModeVideo;
extern u8 *EffetBufferNormal;
extern u8 *display_buffer;
extern u8 *draw_buffer;
extern char *language_txt[304];
extern void display_text(const char *text, s16 x, s16 y, u8 font_size, s8 color);

void SaveScreen(u8 *src, u8 *dst)
{
    if (GameModeVideo == 1) {
        u8 plane;

        plane = 0;
        do {
            u32 plane_offset;

            plane_offset = plane * 0x3e80;
            outpw(0x3ce, (plane << 8) | 4);
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
        u8 plane_mask;
        u8 plane;

        plane_mask = GameModeVideo;
        plane = 0;
        do {
            outpw(0x3c4, (plane_mask << 8) | 2);
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

s16 dummy_scene_func(u32 par_0) {
    return 1;
}

/* Do_Effect_Pause_Simple: no seed source */

/* Do_Effect_Pause_unknown: no seed source */

/* Do_Effect_Pause: no seed source */

/* sub_38A30: no seed source */

/* sub_38B58: no seed source */

/* sub_38DA0: no seed source */

/* sub_38DCC: no seed source */

/* sub_38ED4: no seed source */

/* sub_391C0: no seed source */
