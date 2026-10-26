/*
 * Frame recorder (render/record.h, port/docs/RENDERER.md). The drawing primitives of port/video/lowlevel_normal.c
 * (and the joystick-polling swaps of compat/src/gameport.c) are wrapped at link time (-Wl,--wrap, CMakeLists.txt):
 * each wrapper appends the operation to the frame being recorded, then calls the original primitive, which keeps
 * building the original draw buffer as the reference. At the swap the native renderer draws the recorded frame
 * into its own surfaces; in native mode only those are presented.
 *
 * Inside the passes captured as semantic layers (background, map, foreground band sprites) the primitives are
 * not recorded: the layer operation stands for them, and the verification compares it with what they drew.
 */
#include "render/record.h"
#include "render/native.h"
#include "video/lowlevel_normal.h"

#include <stdio.h>
#include <string.h>

extern uint8_t *DrawBufferNormal, *DisplayBufferNormal;

enum { TARGET_DRAW, TARGET_DISPLAY, TARGET_OTHER };

static port_frame rec;
static int pass = PORT_PASS_LOOP;
static int native_mode, verify;
static int32_t weather_bounds[4];
static int pending_world;
static void (*swap_hook)(void);

/* Native surfaces. The draw surface has margins: the original's 320-byte rows let a write past a row's end land
   at the start of the next row; here it lands beyond the image, where the original's next operations (the border
   clear) leave no visible difference. */
#define MARGIN 32
#define DRAW_W (320 + 2 * MARGIN)
#define DRAW_H (200 + 2 * MARGIN)
static uint8_t native_draw_px[DRAW_W * DRAW_H], native_display_px[64000];
static port_surface native_draw = {native_draw_px, DRAW_W, -MARGIN, -MARGIN, DRAW_W, DRAW_H, NULL};
static port_surface native_display = {native_display_px, 320, 0, 0, 320, 200, NULL};

/* Verification: per-pass check surfaces start from the original buffers at each pass boundary. */
static uint8_t check_draw_px[64000], check_display_px[64000];
static port_surface check_draw = {check_draw_px, 320, 0, 0, 320, 200, NULL};
static port_surface check_display = {check_display_px, 320, 0, 0, 320, 200, NULL};
static int segment_first, segment_pass;

static struct {
    long frames[2], complete[2], draw_exact[2], display_exact[2], complete_exact[2];
    long long draw_bad_px[2], display_bad_px[2];
    long segments[PORT_PASS_COUNT], bad_segments[PORT_PASS_COUNT];
    long long bad_px[PORT_PASS_COUNT];
    long unsupported[PORT_PASS_COUNT][PORT_PRIM_COUNT];
    long display_writes, overflow, origin_moves;
    long ops[2][PORT_PASS_COUNT][PORT_OP_KIND_COUNT];
    int reported;
} st;

static int locate(const uint8_t *p, int32_t *x, int32_t *y)
{
    uintptr_t a = (uintptr_t)p;
    intptr_t off;
    int target;
    if (DrawBufferNormal && a >= (uintptr_t)DrawBufferNormal - 64000 && a < (uintptr_t)DrawBufferNormal + 128000) {
        target = TARGET_DRAW;
        off = (intptr_t)(a - (uintptr_t)DrawBufferNormal);
    } else if (DisplayBufferNormal && a >= (uintptr_t)DisplayBufferNormal &&
               a < (uintptr_t)DisplayBufferNormal + 64000) {
        target = TARGET_DISPLAY;
        off = (intptr_t)(a - (uintptr_t)DisplayBufferNormal);
    } else {
        return TARGET_OTHER;
    }
    *x = (int32_t)(off >= 0 ? off % 320 : 319 - (-off - 1) % 320);
    *y = (int32_t)(off >= 0 ? off / 320 : -1 - (-off - 1) / 320);
    return target;
}

static int claimed(void)
{
    return pass == PORT_PASS_BACKGROUND || pass == PORT_PASS_MAP || pass == PORT_PASS_FRONT;
}

static port_op *append(int kind, int target)
{
    port_op *op;
    if (target == TARGET_DISPLAY)
        rec.display_writes++;
    if (rec.nb_ops >= PORT_FRAME_MAX_OPS) {
        rec.overflow++;
        return NULL;
    }
    op = &rec.ops[rec.nb_ops++];
    memset(op, 0, sizeof *op);
    op->kind = (uint8_t)kind;
    op->pass = (uint8_t)pass;
    op->target = (uint8_t)target;
    op->source = -1;
    return op;
}

static void unsupported(int prim, const uint8_t *dest)
{
    int32_t x, y;
    int target = locate(dest, &x, &y);
    port_op *op;
    if (target == TARGET_OTHER || claimed())
        return;
    op = append(PORT_OP_UNSUPPORTED, target);
    rec.unsupported++;
    st.unsupported[pass][prim]++;
    if (op) {
        op->flags = (uint8_t)prim;
        op->x = x;
        op->y = y;
    }
}

/* ---- passes and semantic layers (render/scene_capture.c) ---- */

static void boundary(void);

int port_record_pass_begin(int p)
{
    int previous = pass;
    boundary();
    pass = p;
    segment_pass = p;
    return previous;
}

void port_record_pass_end(int previous)
{
    boundary();
    pass = previous;
    segment_pass = previous;
}

void port_record_gameplay(void) { rec.gameplay = 1; }

port_scene *port_record_world(void) { return &rec.world; }

void port_record_layer(int kind, int x0, int y0, int x1, int y1)
{
    port_op *op = append(kind, TARGET_DRAW);
    if (op) {
        op->pass = (uint8_t)(kind == PORT_OP_CLEAR ? PORT_PASS_BACKGROUND : pass);
        op->w = kind == PORT_OP_CLEAR ? 16008 * 4 : 0;
        op->clip[0] = x0;
        op->clip[1] = x1;
        op->clip[2] = y0;
        op->clip[3] = y1;
    }
    if (kind == PORT_OP_MAP && rec.gameplay)
        pending_world = 1;
}

void port_record_firefly(int center_x, int center_y, int rayon, int addluclip, int w1, int w2, int h1, int h2)
{
    port_op *op = append(PORT_OP_FIREFLY, TARGET_DRAW);
    if (op) {
        op->x = center_x;
        op->y = center_y;
        op->size = (uint32_t)rayon;
        op->h = addluclip;
        op->clip[0] = w1;
        op->clip[1] = w2;
        op->clip[2] = h1;
        op->clip[3] = h2;
    }
}

void port_record_clear_outside(const uint8_t *target, int x0, int y0, int x1, int y1)
{
    int32_t x, y;
    int where = locate(target, &x, &y);
    port_op *op;
    if (where == TARGET_OTHER || claimed())
        return;
    if (x || y) {
        unsupported(PORT_PRIM_CLEAR_SCREEN, target);
        return;
    }
    op = append(PORT_OP_CLEAR_OUTSIDE, where);
    if (op) {
        op->clip[0] = x0;
        op->clip[1] = x1;
        op->clip[2] = y0;
        op->clip[3] = y1;
    }
}

/* ---- primitive wrappers ---- */

#define SPRITE_WRAP(name, variant) \
    void __real_##name(int32_t x, int32_t c, int32_t y, uint32_t size, uint8_t *d, uint8_t *s); \
    void __wrap_##name(int32_t x, int32_t c, int32_t y, uint32_t size, uint8_t *d, uint8_t *s) \
    { \
        record_sprite(variant, x, c, y, size, d, s); \
        __real_##name(x, c, y, size, d, s); \
    }

static void record_sprite(int variant, int32_t x, int32_t c, int32_t y, uint32_t size, uint8_t *d, uint8_t *s)
{
    int32_t ox, oy;
    int target = locate(d, &ox, &oy);
    port_op *op;
    if (target == TARGET_OTHER || claimed())
        return;
    if (ox || oy) {   /* the sprite drawers take the buffer base and separate coordinates */
        unsupported(PORT_PRIM_SPRITE, d);
        return;
    }
    op = append(PORT_OP_SPRITE, target);
    if (!op)
        return;
    op->flags = (uint8_t)variant;
    op->color = (uint8_t)c;
    op->x = x;
    op->y = y;
    op->size = size;
    get_sprite_clipping(&op->clip[0], &op->clip[1], &op->clip[2], &op->clip[3]);
    op->image = s;
}

SPRITE_WRAP(DrawSpriteNormalNoClip, PORT_SPRITE_NOCLIP)
SPRITE_WRAP(DrawSpriteNormal, 0)
SPRITE_WRAP(DrawSpriteNormal256, PORT_SPRITE_RAW)
SPRITE_WRAP(DrawSpriteFlipNormalNoClip, PORT_SPRITE_FLIP | PORT_SPRITE_NOCLIP)
SPRITE_WRAP(DrawSpriteFlipNormal, PORT_SPRITE_FLIP)
SPRITE_WRAP(DrawSpriteFlipNormal256, PORT_SPRITE_FLIP | PORT_SPRITE_RAW)
SPRITE_WRAP(DrawSpriteColorNormal, PORT_SPRITE_COLOR)
SPRITE_WRAP(DrawSpriteColorFlipNormal, PORT_SPRITE_COLOR | PORT_SPRITE_FLIP)

void __real_DrawSpriteDiffNormal(int16_t x, int16_t y, uint16_t size, uint8_t *mask, uint8_t *dest, uint8_t *source);
void __wrap_DrawSpriteDiffNormal(int16_t x, int16_t y, uint16_t size, uint8_t *mask, uint8_t *dest, uint8_t *source)
{
    unsupported(PORT_PRIM_SPRITE_DIFF, dest);
    __real_DrawSpriteDiffNormal(x, y, size, mask, dest, source);
}

void __real_Display_Bloc(uint8_t *source, uint8_t *dest);
void __wrap_Display_Bloc(uint8_t *source, uint8_t *dest)
{
    unsupported(PORT_PRIM_TILE, dest);
    __real_Display_Bloc(source, dest);
}
void __real_Display_Bloc_Plein(uint8_t *source, uint8_t *dest);
void __wrap_Display_Bloc_Plein(uint8_t *source, uint8_t *dest)
{
    unsupported(PORT_PRIM_TILE, dest);
    __real_Display_Bloc_Plein(source, dest);
}
void __real_Display_Bloc_Clippe(uint8_t *source, uint8_t *dest, int32_t x, int32_t y);
void __wrap_Display_Bloc_Clippe(uint8_t *source, uint8_t *dest, int32_t x, int32_t y)
{
    unsupported(PORT_PRIM_TILE, dest);
    __real_Display_Bloc_Clippe(source, dest, x, y);
}
void __real_Display_Bloc_Plein_Clippe(uint8_t *source, uint8_t *dest, int32_t x, int32_t y);
void __wrap_Display_Bloc_Plein_Clippe(uint8_t *source, uint8_t *dest, int32_t x, int32_t y)
{
    unsupported(PORT_PRIM_TILE, dest);
    __real_Display_Bloc_Plein_Clippe(source, dest, x, y);
}

void __real_Copy_Plan0Diff_To_Buf(uint8_t *source, uint8_t *dest, int32_t width, int32_t height, int32_t pitch);
void __wrap_Copy_Plan0Diff_To_Buf(uint8_t *source, uint8_t *dest, int32_t width, int32_t height, int32_t pitch)
{
    unsupported(PORT_PRIM_PLAN0_COPY, dest);
    __real_Copy_Plan0Diff_To_Buf(source, dest, width, height, pitch);
}
void __real_asm_04d55(uint8_t *source, uint8_t *dest, int32_t width, int32_t height, int32_t pitch);
void __wrap_asm_04d55(uint8_t *source, uint8_t *dest, int32_t width, int32_t height, int32_t pitch)
{
    unsupported(PORT_PRIM_PLAN0_COPY, dest);
    __real_asm_04d55(source, dest, width, height, pitch);
}

void __real_fplot_Normal(uint8_t *dest, int32_t x, int32_t y, int32_t color);
void __wrap_fplot_Normal(uint8_t *dest, int32_t x, int32_t y, int32_t color)
{
    int32_t ox, oy;
    int target = locate(dest, &ox, &oy);
    if (target != TARGET_OTHER && !claimed()) {
        port_op *op = append(PORT_OP_PIXEL, target);
        if (op) {
            /* the pixel lands at dest + 320 y + x */
            int64_t off = (int64_t)oy * 320 + ox + (int64_t)y * 320 + x;
            op->x = (int32_t)(off >= 0 ? off % 320 : 319 - (-off - 1) % 320);
            op->y = (int32_t)(off >= 0 ? off / 320 : -1 - (-off - 1) / 320);
            op->color = (uint8_t)color;
        }
    }
    __real_fplot_Normal(dest, x, y, color);
}

void __real_flocon_clipping(int32_t top, int32_t bottom, int32_t left, int32_t right);
void __wrap_flocon_clipping(int32_t top, int32_t bottom, int32_t left, int32_t right)
{
    weather_bounds[0] = top;
    weather_bounds[1] = bottom;
    weather_bounds[2] = left;
    weather_bounds[3] = right;
    __real_flocon_clipping(top, bottom, left, right);
}

static void record_weather(int stamp, uint8_t *dest, int32_t x, int32_t y)
{
    int32_t ox, oy;
    int target = locate(dest, &ox, &oy);
    port_op *op;
    if (target == TARGET_OTHER || claimed())
        return;
    if (ox || oy) {
        unsupported(PORT_PRIM_WEATHER, dest);
        return;
    }
    op = append(PORT_OP_WEATHER, target);
    if (op) {
        op->flags = (uint8_t)stamp;
        op->x = x;
        op->y = y;
        memcpy(op->clip, weather_bounds, sizeof weather_bounds);
    }
}

#define WEATHER_WRAP(name, stamp) \
    void __real_##name(uint8_t *d, int32_t x, int32_t y); \
    void __wrap_##name(uint8_t *d, int32_t x, int32_t y) { record_weather(stamp, d, x, y); __real_##name(d, x, y); }
WEATHER_WRAP(draw_flocon1_Normal, 0)
WEATHER_WRAP(draw_flocon2_Normal, 1)
WEATHER_WRAP(draw_flocon3_Normal, 2)
WEATHER_WRAP(draw_flocon4_Normal, 3)
WEATHER_WRAP(draw_flocon5_Normal, 4)
WEATHER_WRAP(draw_flocon6_Normal, 5)
WEATHER_WRAP(draw_flocon7_Normal, 6)
WEATHER_WRAP(draw_normal_5755, 7)
WEATHER_WRAP(draw_pluie5_Normal, 8)
WEATHER_WRAP(draw_pluie6_Normal, 9)
WEATHER_WRAP(draw_pluie7_Normal, 10)

void __real_DrawBlackBoxNormal(uint8_t *buffer, int32_t x, int32_t y, int32_t height, int32_t width);
void __wrap_DrawBlackBoxNormal(uint8_t *buffer, int32_t x, int32_t y, int32_t height, int32_t width)
{
    int32_t ox, oy;
    int target = locate(buffer, &ox, &oy);
    if (target != TARGET_OTHER && !claimed()) {
        port_op *op = append(PORT_OP_BLACK_BOX, target);
        if (op) {
            op->x = ox + x;
            op->y = oy + y;
            op->w = width;
            op->h = height;
        }
    }
    __real_DrawBlackBoxNormal(buffer, x, y, height, width);
}

void __real_DrawFondBoxNormal(uint8_t *buffer, int32_t x, int32_t y, int32_t height, int32_t width, int32_t type);
void __wrap_DrawFondBoxNormal(uint8_t *buffer, int32_t x, int32_t y, int32_t height, int32_t width, int32_t type)
{
    unsupported(PORT_PRIM_FOND_BOX, buffer);
    __real_DrawFondBoxNormal(buffer, x, y, height, width, type);
}
void __real_DrawBorderBoxNormal(uint8_t *buffer, int32_t x, int32_t y, int32_t height, int32_t width, uint16_t colors);
void __wrap_DrawBorderBoxNormal(uint8_t *buffer, int32_t x, int32_t y, int32_t height, int32_t width, uint16_t colors)
{
    unsupported(PORT_PRIM_BORDER_BOX, buffer);
    __real_DrawBorderBoxNormal(buffer, x, y, height, width, colors);
}
void __real_DisplayAnyPictureNormal(uint8_t *source, uint8_t *dest, int32_t sx, int32_t sy, int32_t dx, int32_t dy,
                                    int32_t stride, int32_t width, int32_t height);
void __wrap_DisplayAnyPictureNormal(uint8_t *source, uint8_t *dest, int32_t sx, int32_t sy, int32_t dx, int32_t dy,
                                    int32_t stride, int32_t width, int32_t height)
{
    unsupported(PORT_PRIM_PICTURE, dest);
    __real_DisplayAnyPictureNormal(source, dest, sx, sy, dx, dy, stride, width, height);
}

void __real_clear_borders_Normal(uint8_t *dest, int32_t height, int32_t right);
void __wrap_clear_borders_Normal(uint8_t *dest, int32_t height, int32_t right)
{
    int32_t ox, oy;
    int target = locate(dest, &ox, &oy);
    if (target != TARGET_OTHER && !claimed()) {
        port_op *op = append(PORT_OP_CLEAR_BORDERS, target);
        if (op) {
            op->x = ox;
            op->y = oy;
            op->w = right;
            op->h = height;
        }
    }
    __real_clear_borders_Normal(dest, height, right);
}

void __real_N_CLRSCR(uint8_t *buffer);
void __wrap_N_CLRSCR(uint8_t *buffer)
{
    int32_t ox, oy;
    int target = locate(buffer, &ox, &oy);
    if (target != TARGET_OTHER && !claimed()) {
        if (ox || oy) {
            unsupported(PORT_PRIM_CLEAR_SCREEN, buffer);
        } else {
            port_op *op = append(PORT_OP_CLEAR, target);
            if (op)
                op->w = 64000;
        }
    }
    __real_N_CLRSCR(buffer);
}
void __real_ClearDrawAndDisplayBufferNormal(uint8_t *draw, uint8_t *display);
void __wrap_ClearDrawAndDisplayBufferNormal(uint8_t *draw, uint8_t *display)
{
    unsupported(PORT_PRIM_CLEAR_SCREEN, draw);
    unsupported(PORT_PRIM_CLEAR_SCREEN, display);
    __real_ClearDrawAndDisplayBufferNormal(draw, display);
}
static void swap_begin(const uint8_t *source, const uint8_t *dest, int32_t width, int32_t height);
static void swap_end(void);
/* Copying the whole draw buffer to the display is the swap of the scenes outside the gameplay loop (SYNCHRO_LOOP's
   SWAP_BUFFERS); other copies between the buffers are not drawn natively yet. */
void __real_DisplayBufferModeNormal(uint8_t *source, uint8_t *dest);
void __wrap_DisplayBufferModeNormal(uint8_t *source, uint8_t *dest)
{
    int32_t x, y;
    if (locate(source, &x, &y) == TARGET_DRAW && !x && !y && locate(dest, &x, &y) == TARGET_DISPLAY && !x && !y) {
        swap_begin(source, dest, 320, 200);
        __real_DisplayBufferModeNormal(source, dest);
        swap_end();
        return;
    }
    unsupported(PORT_PRIM_BUFFER_COPY, dest);
    __real_DisplayBufferModeNormal(source, dest);
}
void __real_draw_sprite_deform(uint8_t *source, uint8_t *dest, deform_blit_t *geometry);
void __wrap_draw_sprite_deform(uint8_t *source, uint8_t *dest, deform_blit_t *geometry)
{
    unsupported(PORT_PRIM_DEFORM, dest);
    __real_draw_sprite_deform(source, dest, geometry);
}
void __real_draw_effect_segment(uint8_t *dest, uint8_t *descriptor);
void __wrap_draw_effect_segment(uint8_t *dest, uint8_t *descriptor)
{
    unsupported(PORT_PRIM_EFFECT, dest);
    __real_draw_effect_segment(dest, descriptor);
}
void __real_copy_dwords(uint32_t *source, uint32_t *dest, int32_t count);
void __wrap_copy_dwords(uint32_t *source, uint32_t *dest, int32_t count)
{
    unsupported(PORT_PRIM_DWORD_COPY, (uint8_t *)dest);
    __real_copy_dwords(source, dest, count);
}

/* ---- swap: native frame, presentation, verification ---- */

static long compare(const uint8_t *want, int want_pitch, const port_surface *got, const char *what, int p)
{
    long bad = 0;
    int x, y;
    for (y = 0; y < 200; y++) {
        const uint8_t *w = want + y * want_pitch;
        const uint8_t *g = got->pixels + (y - got->y0) * got->pitch - got->x0;
        if (!memcmp(w, g, 320))
            continue;
        for (x = 0; x < 320; x++)
            if (w[x] != g[x]) {
                if (!bad && st.reported < 12) {
                    st.reported++;
                    fprintf(stderr, "render verify: %s (%s) differs in frame %ld at (%d,%d): original %u, native %u\n",
                            what, port_pass_name(p), st.frames[0] + st.frames[1], x, y, w[x], g[x]);
                }
                bad++;
            }
    }
    return bad;
}

/* Per-pass diagnostics: the pass's operations drawn over the original buffers as they were when it began, against
   the original buffers when it ends. The draw buffer is compared inside the viewport (Bloc_lim): outside it the
   original leaves scratch pixels that a later pass of the same frame overwrites (band rows start up to 4 pixels left
   of the viewport, tiles are clipped in 4-pixel units, the border clear follows); the full-frame check at the swap
   compares everything. A segment during which the engine moved the draw buffer's origin (update_display_map shifts
   DrawBufferNormal by the camera's alignment) has no draw-buffer reference. */
static const uint8_t *segment_origin;

static long compare_rect(const uint8_t *want, const port_surface *got, int x0, int y0, int x1, int y1, int p)
{
    long bad = 0;
    int x, y;
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 > 320) x1 = 320;
    if (y1 > 200) y1 = 200;
    for (y = y0; y < y1; y++)
        for (x = x0; x < x1; x++)
            if (want[320 * y + x] != got->pixels[(y - got->y0) * got->pitch + x - got->x0]) {
                if (!bad && st.reported < 12) {
                    st.reported++;
                    fprintf(stderr, "render verify: pass %s differs in frame %ld at (%d,%d): original %u, native %u\n",
                            port_pass_name(p), st.frames[0] + st.frames[1], x, y, want[320 * y + x],
                            got->pixels[(y - got->y0) * got->pitch + x - got->x0]);
                }
                bad++;
            }
    return bad;
}

static void boundary(void)
{
    long bad = 0;
    if (!verify || !DrawBufferNormal)
        return;
    port_native_draw(&rec, segment_first, rec.nb_ops, &check_draw, &check_display);
    if (segment_origin == DrawBufferNormal)
        bad = compare_rect(DrawBufferNormal, &check_draw, Bloc_lim_W1, Bloc_lim_H1, Bloc_lim_W2, Bloc_lim_H2,
                           segment_pass);
    else
        st.origin_moves++;
    bad += compare_rect(DisplayBufferNormal, &check_display, 0, 0, 320, 200, segment_pass);
    st.segments[segment_pass]++;
    if (bad) {
        st.bad_segments[segment_pass]++;
        st.bad_px[segment_pass] += bad;
    }
    memcpy(check_draw_px, DrawBufferNormal, 64000);
    memcpy(check_display_px, DisplayBufferNormal, 64000);
    segment_origin = DrawBufferNormal;
    segment_first = rec.nb_ops;
}

static void swap_native(const uint8_t *source, const uint8_t *dest, int32_t width, int32_t height)
{
    int32_t sx, sy, dx, dy, row, n = (width / 4) * 4;
    if (!native_mode && !verify)
        return;
    port_native_draw(&rec, 0, rec.nb_ops, &native_draw, &native_display);
    if (locate(source, &sx, &sy) != TARGET_DRAW || locate(dest, &dx, &dy) != TARGET_DISPLAY)
        return;
    for (row = 0; row < height; row++) {
        int32_t i;
        for (i = 0; i < n; i++) {
            int32_t x = sx + i, y = sy + row, tx = dx + i, ty = dy + row;
            if (x >= -MARGIN && x < 320 + MARGIN && y >= -MARGIN && y < 200 + MARGIN && tx >= 0 && tx < 320 &&
                ty >= 0 && ty < 200)
                native_display_px[ty * 320 + tx] = native_draw_px[(y + MARGIN) * DRAW_W + x + MARGIN];
        }
    }
}

static void swap_begin(const uint8_t *source, const uint8_t *dest, int32_t width, int32_t height)
{
    boundary();
    swap_native(source, dest, width, height);
}

static void swap_end(void)
{
    int g = rec.gameplay ? 1 : 0;
    if (verify && DrawBufferNormal) {
        long bad_draw = compare(DrawBufferNormal, 320, &native_draw, "frame draw buffer", PORT_PASS_LOOP);
        long bad_display = compare(DisplayBufferNormal, 320, &native_display, "frame display", PORT_PASS_LOOP);
        int complete = !rec.unsupported && !rec.overflow;
        st.frames[g]++;
        st.complete[g] += complete;
        st.draw_exact[g] += !bad_draw;
        st.display_exact[g] += !bad_display;
        st.complete_exact[g] += complete && !bad_draw && !bad_display;
        st.draw_bad_px[g] += bad_draw;
        st.display_bad_px[g] += bad_display;
        memcpy(check_draw_px, DrawBufferNormal, 64000);
        memcpy(check_display_px, DisplayBufferNormal, 64000);
        segment_origin = DrawBufferNormal;
    }
    {
        int i;
        for (i = 0; i < rec.nb_ops; i++)
            st.ops[g][rec.ops[i].pass][rec.ops[i].kind]++;
    }
    st.display_writes += rec.display_writes;
    st.overflow += rec.overflow;
    rec.seq++;
    rec.gameplay = 0;
    rec.nb_ops = rec.overflow = rec.unsupported = rec.display_writes = 0;
    segment_first = 0;
    pending_world = 0;
    if (swap_hook)
        swap_hook();
}

void port_record_set_swap_hook(void (*hook)(void)) { swap_hook = hook; }

void __real_Swap_To_Screen(uint8_t *source, uint8_t *dest, int32_t width, int32_t height);
void __wrap_Swap_To_Screen(uint8_t *source, uint8_t *dest, int32_t width, int32_t height)
{
    swap_begin(source, dest, width, height);
    __real_Swap_To_Screen(source, dest, width, height);
    swap_end();
}

int32_t __real_get_joy_input1_dos1(uint8_t *source, uint8_t *dest, int32_t width, int32_t height);
int32_t __wrap_get_joy_input1_dos1(uint8_t *source, uint8_t *dest, int32_t width, int32_t height)
{
    int32_t r;
    swap_begin(source, dest, width, height);
    r = __real_get_joy_input1_dos1(source, dest, width, height);
    swap_end();
    return r;
}
int32_t __real_get_joy_input1_dos2(uint8_t *source, uint8_t *dest, int32_t width, int32_t height);
int32_t __wrap_get_joy_input1_dos2(uint8_t *source, uint8_t *dest, int32_t width, int32_t height)
{
    int32_t r;
    swap_begin(source, dest, width, height);
    r = __real_get_joy_input1_dos2(source, dest, width, height);
    swap_end();
    return r;
}

const port_scene *port_scene_take_pending(void)
{
    if (!pending_world)
        return NULL;
    pending_world = 0;
    return &rec.world;
}

void port_record_set_native(int on) { native_mode = on; }
int port_record_native(void) { return native_mode; }
const uint8_t *port_record_presented(void) { return native_mode ? native_display_px : NULL; }
void port_record_set_verify(int on) { verify = on; }

void port_record_report(void)
{
    int p, k, g;
    if (!verify)
        return;
    printf("render frames: gameplay=%ld gameplay_complete=%ld gameplay_draw_exact=%ld gameplay_display_exact=%ld "
           "gameplay_complete_exact=%ld gameplay_draw_bad_px=%lld gameplay_display_bad_px=%lld other=%ld "
           "other_complete=%ld other_draw_exact=%ld other_display_exact=%ld overflow=%ld display_writes=%ld\n",
           st.frames[1], st.complete[1], st.draw_exact[1], st.display_exact[1], st.complete_exact[1],
           st.draw_bad_px[1], st.display_bad_px[1], st.frames[0], st.complete[0], st.draw_exact[0],
           st.display_exact[0], st.overflow, st.display_writes, st.origin_moves);
    for (p = 0; p < PORT_PASS_COUNT; p++)
        if (st.segments[p])
            printf("render pass %s: segments=%ld bad_segments=%ld bad_px=%lld\n", port_pass_name(p), st.segments[p],
                   st.bad_segments[p], st.bad_px[p]);
    for (g = 1; g >= 0; g--)
        for (p = 0; p < PORT_PASS_COUNT; p++)
            for (k = 0; k < PORT_OP_KIND_COUNT; k++)
                if (st.ops[g][p][k])
                    printf("render ops %s %s/%s: %ld\n", g ? "gameplay" : "other", port_pass_name(p),
                           port_op_name(k), st.ops[g][p][k]);
    for (p = 0; p < PORT_PASS_COUNT; p++)
        for (k = 0; k < PORT_PRIM_COUNT; k++)
            if (st.unsupported[p][k])
                printf("render unsupported %s/%s: %ld\n", port_pass_name(p), port_prim_name(k), st.unsupported[p][k]);
}
