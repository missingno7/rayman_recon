/*
 * Screen-effect capture (port/docs/RENDERER.md "Screen effects"). Compiled with the historical units. Each wrapper
 * stands for one pixel pass of an effect unit, reached at link time from another unit (-Wl,--wrap, CMakeLists.txt
 * RAY_RENDER_EFFECTS): it records the pass's inputs as a native operation (render/effects.h), then calls the
 * original exactly once, with the recorder's primitive and memory observation suspended, since the operation stands
 * for everything the pass writes. The effect's own state update stays in the original.
 */
#include "rtypes.h"
#include "game.h"
#include "palette.h"

#include "render/effect_layouts.h"
#include "render/effects.h"
#include "render/record.h"

#include <stddef.h>
#include <string.h>

static int buffer(const void *p, port_effect_buffer *b)
{
    b->pad = 0;
    b->target = port_record_target(p, &b->offset);
    return b->target < PORT_TARGET_COUNT;
}

/* effect633e0.c: page curl. */
extern port_effect_column_state_t *effect_state_55760;
void __real_effect_helper_63838(void);
void __wrap_effect_helper_63838(void)
{
    const port_effect_column_state_t *s = effect_state_55760;
    port_effect_curl e;
    int recorded = 0;
    memset(&e, 0, sizeof e);
    if (s && s->width <= 40 && buffer(s->source, &e.source) && buffer(s->destination, &e.destination)) {
        e.mode = s->mode;
        e.initialized = s->initialized;
        e.delta = s->delta;
        e.field_0c = s->field_0c;
        e.field_10 = s->field_10;
        e.field_14 = s->field_14;
        e.width = s->width;
        memcpy(e.table, s->table, (size_t)s->width * 2 * sizeof e.table[0]);
        recorded = port_record_effect(PORT_EFFECT_CURL, &e, sizeof e);
    }
    if (recorded)
        port_record_claim(1);
    __real_effect_helper_63838();
    if (recorded)
        port_record_claim(0);
}

/* box.c PlasmaBox: advances the plasma phases when asked, widens the box by 3 pixels per side, clamps it (with the
   original's two slips: a box above the screen grows by x instead of y, and the bottom is compared with the screen
   width), and calls Plasma (same unit). After it returns, its phase globals hold the values Plasma received; the
   geometry is recomputed here with its expressions (src/box.c 145-160). */
extern u8 *draw_buffer;
extern u8 byte_DA43C[128 * 128];
extern u8 plasma_palette_color_index;
extern s16 word_95FB0, word_DE73E, word_DE740, word_DE742, word_DE744, word_DE746, word_DE748, word_DE74A, word_DE74C;
void __real_PlasmaBox(s16 x, s16 y, s16 width, s16 height, u8 advance_phases);
void __wrap_PlasmaBox(s16 x, s16 y, s16 width, s16 height, u8 advance_phases)
{
    static port_effect_plasma e;
    int recorded = 0;
    port_record_claim(1);
    __real_PlasmaBox(x, y, width, height, advance_phases);
    port_record_claim(0);
    x -= 3;
    y -= 3;
    width += 6;
    height += 6;
    if (x < 0) {
        width += x;
        x = 0;
    } else if (x + width > 320) {
        width = 320 - x;
    }
    if (y < 0) {
        y = 0;
        height += x;
    } else if (y + height > 320) {
        height = 200 - y;
    }
    if (buffer(draw_buffer, &e.destination)) {
        e.x = x;
        e.y = y;
        e.width = width;
        e.height = height;
        e.phase_x[0] = word_DE74C;
        e.phase_y[0] = word_DE74A;
        e.phase_x[1] = word_DE748;
        e.phase_y[1] = word_DE746;
        e.phase_x[2] = word_DE744;
        e.phase_y[2] = word_DE742;
        e.phase_x[3] = word_DE740;
        e.phase_y[3] = word_DE73E;
        e.bias = word_95FB0;
        e.color_index = plasma_palette_color_index;
        memcpy(e.table, byte_DA43C, sizeof e.table);
        recorded = port_record_effect(PORT_EFFECT_PLASMA, &e, sizeof e);
    }
    if (!recorded)
        port_record_unsupported_effect(draw_buffer);
}

/* jumelle.c DisplayJumellesNormal (called by display.c WORLD_CHOICE): copies the two circles of the world map's
   binocular view from the effect buffer into the draw buffer, through JumelleZoomDef when zoom is active; then, when
   asked, recomputes the zoom table (PrepareJumelleZoom), which the next frame uses. The inputs are read before. */
extern u8 JumelleZoomActif;
extern s32 JumelleXMin, JumelleYMin;
extern s32 *JumelleZoomDef;
void __real_DisplayJumellesNormal(s32 x, s32 y, s32 rayon, s32 circle2_offset, u8 *effet_buf, u8 *draw_buf);
void __wrap_DisplayJumellesNormal(s32 x, s32 y, s32 rayon, s32 circle2_offset, u8 *effet_buf, u8 *draw_buf)
{
    static port_effect_binoculars e;
    size_t size = offsetof(port_effect_binoculars, zoom_table);
    int recorded = 0;
    if (buffer(effet_buf, &e.effect) && buffer(draw_buf, &e.draw)) {
        e.x = x;
        e.y = y;
        e.radius = rayon;
        e.circle2_offset = circle2_offset;
        e.x_min = JumelleXMin;
        e.y_min = JumelleYMin;
        e.zoom = JumelleZoomActif != 0;
        if (e.zoom && JumelleZoomDef) {
            memcpy(e.zoom_table, JumelleZoomDef, sizeof e.zoom_table);
            size = sizeof e;
        }
        recorded = (!e.zoom || JumelleZoomDef) && port_record_effect(PORT_EFFECT_BINOCULARS, &e, size);
    }
    if (!recorded)
        port_record_unsupported_effect(draw_buf);
    port_record_claim(1);
    __real_DisplayJumellesNormal(x, y, rayon, circle2_offset, effet_buf, draw_buf);
    port_record_claim(0);
}

/* effect6c3b0.c effect_helper_6c9bc (called by effect1.c): the moving-lens transition's pixel pass. */
extern port_effect_state_6c_t *effect_state_6c;
void __real_effect_helper_6c9bc(void);
void __wrap_effect_helper_6c9bc(void)
{
    static port_effect_lens e;
    const port_effect_state_6c_t *s = effect_state_6c;
    int recorded = 0;
    if (s && s->width <= 256 && s->height <= 256 && buffer(s->screen, &e.screen) && buffer(s->output, &e.output)) {
        size_t map_bytes = (size_t)2 * s->width * s->height;
        e.x = s->x;
        e.y = s->y;
        e.radius = s->radius;
        e.width = s->width;
        e.height = s->height;
        memcpy(e.map, s->map, map_bytes);
        recorded = port_record_effect(PORT_EFFECT_LENS, &e, offsetof(port_effect_lens, map) + map_bytes);
    }
    if (!recorded && s)
        port_record_unsupported_effect(s->output);
    if (recorded)
        port_record_claim(1);
    __real_effect_helper_6c9bc();
    if (recorded)
        port_record_claim(0);
}

/* ---- menus ---- */

extern u8 *EffetBufferNormal, *DrawBufferNormal;
extern s32 Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2;
extern u8 ModeVideoActuel, current_pal_id;
extern rgb_palette_t rvb[3];

static void record_grey(const u8 *source, u8 *destination, int width, int height, const void *palette)
{
    static port_effect_grey e;
    if (buffer(source, &e.source) && buffer(destination, &e.destination)) {
        e.width = width;
        e.height = height;
        memcpy(e.palette, palette, sizeof e.palette);
        if (port_record_effect(PORT_EFFECT_GREY, &e, sizeof e))
            return;
    }
    port_record_unsupported_effect(destination);
}

/* box.c CaptureVideo (normal mode, greyscale): the built frame through the current palette into the effect buffer,
   the in-game menu's background. Mode X and the plain copy (a memcpy, recorded) are left to the original. */
void __real_CaptureVideo(u8 *source_buf, u8 grayscale);
void __wrap_CaptureVideo(u8 *source_buf, u8 grayscale)
{
    int grey = ModeVideoActuel != 1 /* MODE_X */ && grayscale;
    if (grey) {
        record_grey(source_buf, EffetBufferNormal, 320, 200, &rvb[current_pal_id]);
        port_record_claim(1);
    }
    __real_CaptureVideo(source_buf, grayscale);
    if (grey)
        port_record_claim(0);
}

/* menu.c FonduOption, mode 1 (derivative hook, port/derive/menu.patch): the option rectangle greyed in place in the
   draw buffer and in the menu picture, through MenuPalette. */
void port_record_menu_grey(u8 *rectangle, s16 width, s16 height, const void *palette)
{
    record_grey(rectangle, rectangle, width, height, palette);
}

/* box.c CadreTrans (called from the menus): x and width aligned up to 4, then the dword pass and a border box (the
   border box is a recorded primitive). The alignment is recomputed with its expressions (src/box.c 326-328). */
extern u8 *draw_buffer;
void __real_CadreTrans(s16 x, s16 y, s16 w, s16 h);
void __wrap_CadreTrans(s16 x, s16 y, s16 w, s16 h)
{
    static port_effect_frame e;
    s16 ax = (s16)(x + 4 - (x & 3)), aw = (s16)(w + 4 - (w & 3));
    if (buffer(draw_buffer, &e.destination)) {
        e.x = ax;
        e.y = y;
        e.width = aw;
        e.height = h;
        if (!port_record_effect(PORT_EFFECT_FRAME, &e, sizeof e))
            port_record_unsupported_effect(draw_buffer);
    } else {
        port_record_unsupported_effect(draw_buffer);
    }
    __real_CadreTrans(x, y, w, h);
}

/* frame.c DISPLAY_FOND_MENU (called by the menus): outside the game a recorded memcpy; in game (OptionGame) the
   viewport rows copied from the effect buffer with a C loop, between clears outside the viewport (recorded, and
   disjoint from it, so the copy is recorded first). */
extern u8 OptionGame;
void __real_DISPLAY_FOND_MENU(void);
void __wrap_DISPLAY_FOND_MENU(void)
{
    static port_effect_rect e;
    if (OptionGame && Bloc_lim_W2 > Bloc_lim_W1 && Bloc_lim_H2 > Bloc_lim_H1) {
        int64_t at = Bloc_lim_W1 + (int64_t)Bloc_lim_H1 * 320;
        if (buffer(EffetBufferNormal + at, &e.source) && buffer(draw_buffer + at, &e.destination)) {
            e.width = Bloc_lim_W2 - Bloc_lim_W1;
            e.height = Bloc_lim_H2 - Bloc_lim_H1;
            if (!port_record_effect(PORT_EFFECT_COPY_RECT, &e, sizeof e))
                port_record_unsupported_effect(draw_buffer);
        } else {
            port_record_unsupported_effect(draw_buffer);
        }
    }
    __real_DISPLAY_FOND_MENU();
}

/* box.c FireBox (called by the menus' message boxes): the box widened and clamped like PlasmaBox's (src/box.c
   220-238, the same two slips), then Fire (same unit, a tail call): it advances the fire field p1 (consuming random
   numbers) and copies its first `height` rows into draw_buffer. The original runs once; its output rows are read
   from the field afterwards (src/box.c 210-216). */
extern s16 p1[70][300];
void __real_FireBox(s16 x, s16 y, s16 width, s16 height);
void __wrap_FireBox(s16 x, s16 y, s16 width, s16 height)
{
    static port_effect_fire e;
    int recorded = 0;
    port_record_claim(1);
    __real_FireBox(x, y, width, height);
    port_record_claim(0);
    x -= 3;
    y -= 3;
    width += 6;
    height += 6;
    if (x < 0) {
        width += x;
        x = 0;
    } else if (x + width > 320) {
        width = 320 - x;
    }
    if (y < 0) {
        y = 0;
        height += x;
    } else if (y + height > 320) {
        height = 200 - y;
    }
    if (width > 0 && height > 0 && width <= 300 && height <= 70 &&
        buffer(draw_buffer + x + 320 * y, &e.destination)) {
        s16 i, j;
        e.width = width;
        e.height = height;
        for (i = 0; i < height; ++i)
            for (j = 0; j < width; ++j)
                e.field[i * width + j] = (u8)p1[i][j];
        recorded = port_record_effect(PORT_EFFECT_FIRE, &e, offsetof(port_effect_fire, field) + (size_t)width * height);
    }
    if (!recorded && width > 0 && height > 0)
        port_record_unsupported_effect(draw_buffer);
}
