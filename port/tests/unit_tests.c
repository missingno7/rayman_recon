/*
 * Native unit tests of the port's compat layer: the original inline helpers, the Watcom C-library behaviour,
 * the PIT/retrace clock model and the DOS record decoder. Expected values come from the documented instruction
 * sequences (src/include/inline_helpers.h) and the linked library code (DOS addresses in compat/src).
 */
#include "platform/display.h"
#include "platform/config.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "rtypes.h"
#include "obj.h"
#include "compat/src/pc_hardware.h"
#include "game/dos_records.h"
#include "platform/clock.h"
#include "video/vga.h"

s16 ashr16(s16 value, u8 shift);
s32 ashl16(s16 value, u8 shift);
s32 ashr32_inline(s32 value, u8 shift);
s32 abs32_inline(s32 value);
s32 sgn(s32 value);
s32 inrange(s32 value, s32 lower, s32 upper);
void copy(const void *source, void *destination, u32 count);
void palette_output(s32 first, s32 last, const void *palette);
int port_rand(void);
void port_srand(unsigned seed);
char *port_itoa(int value, char *buf, int radix);
double effect_math_curve(double x);
void speaker_enable(void);
s32 clock_ticks(void);
s32 wait_until(s32 target);

static int failures;
#define CHECK(cond) do { if (!(cond)) { printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); failures++; } } while (0)

static void test_inline_helpers(void)
{
    CHECK(ashr16(100, 3) == 12);
    CHECK(ashr16(-100, 3) == -12);          /* rounds toward zero (neg/sar/neg) */
    CHECK(ashr16(-1, 1) == 0);
    CHECK(ashr16(-32768, 0) == -32768);     /* 16-bit negation of -32768 is itself */
    CHECK(ashr16(-32768, 1) == 16384);       /* neg 8000h = 8000h, sar = C000h, neg = 4000h */
    CHECK(ashr16(1000, 20) == 0);           /* counts above 15 shift out every bit */
    CHECK(ashl16(-2, 4) == -32);            /* cwde before the shift */
    CHECK(ashr32_inline(-5, 1) == -3);      /* arithmetic shift rounds down */
    CHECK(abs32_inline(-7) == 7);
    CHECK(add_row320(5, 3) == 965 && add_row320(-4, -1) == -324 && add_row320(0, 199) == 63680);
    CHECK(abs32_inline((s32)0x80000000u) == (s32)0x80000000u);
    CHECK(sgn(-9) == -1 && sgn(0) == 0 && sgn(9) == 1);
    CHECK(inrange(5, 5, 6) && !inrange(7, 5, 6) && inrange(-3, -4, -3));
}

static void test_forward_copy(void)
{
    /* rep movsb (count & 3) then rep movsd: an overlapping forward copy repeats the first bytes. */
    u8 buf[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    u8 ref[16];
    u32 i;
    memcpy(ref, buf, sizeof ref);
    for (i = 0; i < 1; i++)
        ref[1 + i] = ref[i];
    for (i = 0; i < 2; i++) {
        u8 t[4];
        memcpy(t, ref + 1 + 4 * i, 4);
        memcpy(ref + 2 + 4 * i, t, 4);
    }
    copy(buf, buf + 1, 9);
    CHECK(memcmp(buf, ref, sizeof buf) == 0);
}

static void test_clib(void)
{
    char s[40];
    port_srand(1);
    CHECK(port_rand() == 16838);            /* Watcom/ANSI LCG, seed 1 (DOS 2:2d9b4) */
    CHECK(port_rand() == 5758);
    CHECK(port_rand() == 10113);
    CHECK(strcmp(port_itoa(-42, s, 10), "-42") == 0);
    CHECK(strcmp(port_itoa(-1, s, 16), "ffffffff") == 0);
    CHECK(effect_math_curve(1.0) == 0.0);
    CHECK(fabs(effect_math_curve(-1.0) - 3.141592653589793) < 1e-15);
    CHECK(fabs(effect_math_curve(0.5) - acos(0.5)) < 1e-12);
}

static void test_timing(void)
{
    s32 a, b, target;
    port_clock_init(PORT_CLOCK_DETERMINISTIC);
    speaker_enable();
    a = clock_ticks();
    b = clock_ticks();
    CHECK(b > a);                           /* two latches never return the same count */
    target = b + 5000;
    CHECK(wait_until(target) >= 1);
    CHECK(clock_ticks() > target);
    a = clock_ticks();
    CHECK(wait_until(a - 10) == 0);         /* already past: the first poll succeeds */
    /* 70000 PIT ticks span more than one 16-bit counter period; the epoch logic must follow. */
    a = clock_ticks();
    CHECK(wait_until(a + 70000) >= 1);
    CHECK(clock_ticks() > a + 70000);
}

static void test_dac(void)
{
    u8 pal[768];
    int i;
    for (i = 0; i < 768; i++)
        pal[i] = (u8)(i & 63);
    memset(port_vga_dac, 0, 768);
    palette_output(2, 3, pal);
    CHECK(port_vga_dac[5] == 0 && port_vga_dac[6] == pal[6] && port_vga_dac[11] == pal[11] && port_vga_dac[12] == 0);
}

static void test_obj_record(void)
{
    u8 rec[132];
    obj_t o;
    port_obj_words_t w;
    int i;
    for (i = 0; i < 132; i++)
        rec[i] = (u8)(i * 7 + 3);
    rec[0x28] = 0x78; rec[0x29] = 0x56; rec[0x2a] = 0x34; rec[0x2b] = 0x12;   /* x */
    rec[0x60] = 0x2a; rec[0x61] = 0x01;                                       /* type */
    rec[0x81] = 0x0c; rec[0x82] = 0x01;                                       /* alive, flip_x, flag_0x100 */
    rec[0x00] = 0xff; rec[0x01] = 0xff; rec[0x02] = 0x05; rec[0x03] = 0x00;   /* sprites word 0x0005ffff */
    port_decode_obj_record(&o, rec, &w);
    CHECK(o.x == 0x12345678);
    CHECK(o.type == 0x012a);
    CHECK(o.flags.alive == 1 && o.flags.flip_x == 1 && o.flags.hurtbyfist == 0 && o.flags.flag_0x100 == 1);
    CHECK(o.anim_count == rec[0x80] && o.field_83 == rec[0x83] && o.offset_bx == rec[0x68]);
    CHECK((s16)w.sprites == -1);            /* the loader keeps the low 16 bits (MOVSX) */
    CHECK(o.sprites == NULL && o.cmds == NULL);
}

/* PalPlasma and Def_Bande (src/data/blk_5f70.c, blk_80fc.c, typed source) compiled natively must hold exactly the
   DOS bytes (FNV-1a of GOG v1.21 obj2 2:05f70 and 2:080fc): the native layouts of rgb_t (3 bytes) and def_bande_t
   (8 bytes) equal the DOS ones. */
extern unsigned char PalPlasma[], Def_Bande[];
static const unsigned long long port_asset_fnv_PalPlasma = 0x026cfe363c27e07aULL, port_asset_fnv_Def_Bande = 0x118957a1e740c726ULL;

static unsigned long long fnv(const unsigned char *p, size_t n)
{
    unsigned long long h = 1469598103934665603ULL;
    while (n--)
        h = (h ^ *p++) * 1099511628211ULL;
    return h;
}

static void test_assets(void)
{
    CHECK(fnv(PalPlasma, 48) == port_asset_fnv_PalPlasma);
    CHECK(fnv(Def_Bande, 144000) == port_asset_fnv_Def_Bande);
}

static void write_text(const char *path, const char *text)
{
    FILE *f = fopen(path, "wb");
    fputs(text, f);
    fclose(f);
}

static void test_config(void)
{
    static port_config c, d;
    const char *path = "config-test.json";
    remove(path);
    CHECK(port_config_load(path, &c) == 0);
    port_config_defaults(&c);
    CHECK(c.widescreen == PORT_WIDE_FIT && !c.fullscreen && c.vsync && !c.window_scale && !strcmp(c.cd, "auto") && !c.save_dir[0]);
    c.vsync = 0;
    c.widescreen = PORT_WIDE_21_9;
    c.window_scale = 3;
    strcpy(c.save_dir, "D:\\Saves \"x\"");
    strcpy(c.extra_data_dirs[0], "D:/Games/Rayman");
    c.extra_data_dir_count = 1;
    CHECK(port_config_save(path, &c));
    CHECK(port_config_load(path, &d) == 1);
    CHECK(d.widescreen == PORT_WIDE_21_9 && d.fullscreen == 0 && d.vsync == 0 && d.window_scale == 3 && !strcmp(d.cd, "auto"));
    CHECK(!strcmp(d.save_dir, c.save_dir) && d.extra_data_dir_count == 1 &&
          !strcmp(d.extra_data_dirs[0], c.extra_data_dirs[0]));
    /* A partial file with an unknown setting and a path typed with single backslashes: loads, reported incomplete. */
    write_text(path, "{ \"future\": {\"a\": [1, 2.5, null]}, \"fullscreen\": true, \"cd\": \"E:\", "
                     "\"save_dir\": \"D:\\Games\\x\" }");
    CHECK(port_config_load(path, &d) == 2);
    CHECK(d.fullscreen == 1 && d.widescreen == PORT_WIDE_FIT && d.vsync == 1 && !strcmp(d.cd, "E:") &&
          !strcmp(d.save_dir, "D:\\Games\\x"));
    /* A wrong type and malformed text are rejected and leave the defaults. */
    write_text(path, "{ \"widescreen\": 1 }");
    CHECK(port_config_load(path, &d) == -1 && d.widescreen == PORT_WIDE_FIT && !strcmp(d.cd, "auto"));
    write_text(path, "{ \"widescreen\": true");
    CHECK(port_config_load(path, &d) == -1);
    /* The wider view's mode: names, and the booleans of earlier versions (true = as wide as the window or display). */
    write_text(path, "{ \"widescreen\": \"16:9\" }");
    CHECK(port_config_load(path, &d) == 2 && d.widescreen == PORT_WIDE_16_9);
    write_text(path, "{ \"widescreen\": true }");
    CHECK(port_config_load(path, &d) == 2 && d.widescreen == PORT_WIDE_FIT);
    write_text(path, "{ \"widescreen\": false }");
    CHECK(port_config_load(path, &d) == 2 && d.widescreen == PORT_WIDE_OFF);
    write_text(path, "{ \"widescreen\": \"wide\" }");
    CHECK(port_config_load(path, &d) == -1);
    /* The view width per mode and pixel shape (platform/display.c). */
    port_display_init(PORT_WIDE_16_9, 1, 0, -1, 0);
    CHECK(port_display_pad() == 18 && port_display_rows() == 200);
    port_display_set_square(0);
    CHECK(port_display_pad() == 54 && port_display_rows() == 240 && port_display_take_changed() && !port_display_take_changed());
    port_display_set_mode(PORT_WIDE_21_9);
    CHECK(port_display_pad() == 120);
    port_display_set_square(1);
    CHECK(port_display_pad() == 74);
    port_display_set_mode(PORT_WIDE_FIT);
    port_display_set_output(3120, 1440);
    CHECK(port_display_pad() == 57);
    port_display_set_output(0, 0);
    CHECK(port_display_pad() == 18);
    port_display_init(PORT_WIDE_FIT, 1, 0, 40, 0);
    CHECK(port_display_pad() == 40);
    port_display_set_mode(PORT_WIDE_OFF);
    CHECK(port_display_pad() == 0);
    port_display_init(PORT_WIDE_16_9, 1, 0, -1, 1);
    CHECK(port_display_pad() == 0 && !port_display_adjustable());
    remove(path);
}

int main(void)
{
    test_config();
    test_assets();
    test_inline_helpers();
    test_forward_copy();
    test_clib();
    test_timing();
    test_dac();
    test_obj_record();
    printf("%s (%d failures)\n", failures ? "FAILED" : "OK", failures);
    return failures != 0;
}
