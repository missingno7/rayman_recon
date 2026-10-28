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
#include "render/effects.h"

int port_record_deform(const port_deform *e);
#include "video/lowlevel_normal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern uint8_t *DrawBufferNormal, *DrawBufferNormalIni, *DisplayBufferNormal, *EffetBufferNormal;

enum { TARGET_DRAW = PORT_TARGET_DRAW, TARGET_DISPLAY = PORT_TARGET_DISPLAY, TARGET_EFFECT = PORT_TARGET_EFFECT,
       TARGET_OTHER = PORT_TARGET_COUNT };

static port_frame rec;
static int pass = PORT_PASS_LOOP;
static int native_mode, verify;
static int32_t weather_bounds[4];
static void (*swap_hook)(void);
static const char *scene = "start";

/* Native surfaces. The draw surface has margins: the original's 320-byte rows let a write past a row's end land
   at the start of the next row; here it lands beyond the image, where the original's next operations (the border
   clear) leave no visible difference. */
#define MARGIN 32
#define MAX_PAD PORT_SCENE_MAX_PAD
#define DRAW_W (320 + 2 * (MARGIN + MAX_PAD))
#define DRAW_H (200 + 2 * MARGIN)
static uint8_t native_draw_px[DRAW_W * DRAW_H], native_display_px[(320 + 2 * MAX_PAD) * 200],
    native_effect_px[128000];
static port_surface native_draw = {native_draw_px, 320 + 2 * MARGIN, -MARGIN, -MARGIN, 320 + 2 * MARGIN, DRAW_H, NULL};
static port_surface native_display = {native_display_px, 320, 0, 0, 320, 200, NULL};
/* The view: `pad` columns beyond the original 320 on each side (render/native.h). */
static int pad;
static port_surface native_effect = {native_effect_px, 320, 0, 0, 320, 400, NULL};
static port_surface *const native[PORT_TARGET_COUNT] = {&native_draw, &native_display, &native_effect};

/* Verification: per-pass check surfaces start from the original buffers at each pass boundary. */
static uint8_t check_draw_px[64000], check_display_px[64000], check_effect_px[128000];
static port_surface check_draw = {check_draw_px, 320, 0, 0, 320, 200, NULL};
static port_surface check_display = {check_display_px, 320, 0, 0, 320, 200, NULL};
static port_surface check_effect = {check_effect_px, 320, 0, 0, 320, 400, NULL};
static port_surface *const check[PORT_TARGET_COUNT] = {&check_draw, &check_display, &check_effect};
static int segment_first, segment_pass;
static int native_drawn;     /* operations of the frame already drawn into the native surfaces */
static int claim_depth;      /* inside a wrapper that records a semantic operation for what it draws */

/* Bytes of copy sources captured when recorded (the source may be a scratch buffer the game reuses later in the
   frame); released when the frame is done. */
static uint8_t arena[8 << 20];
static size_t arena_used;

static struct {
    long frames[2], complete[2], draw_exact[2], display_exact[2], complete_exact[2], effect_exact[2];
    long long draw_bad_px[2], display_bad_px[2];
    long segments[PORT_PASS_COUNT], bad_segments[PORT_PASS_COUNT];
    long long bad_px[PORT_PASS_COUNT];
    long unsupported[PORT_PASS_COUNT][PORT_PRIM_COUNT];
    long display_writes, overflow, origin_moves, presents, present_exact, volatile_images;
    long ops[2][PORT_PASS_COUNT][PORT_OP_KIND_COUNT];
    long effects[PORT_EFFECT_COUNT];
    int reported;
} st;

/* Frames per scene label (render/scene_capture.c), in order of first appearance. */
static struct scene_stats {
    const char *name;
    long frames, complete, exact;
} scenes[48];
static int nb_scenes;

const char *port_record_scene(const char *name)
{
    const char *previous = scene;
    scene = name;
    return previous;
}

static struct scene_stats *scene_stats(void)
{
    int i;
    for (i = 0; i < nb_scenes; i++)
        if (scenes[i].name == scene)
            return &scenes[i];
    if (nb_scenes == (int)(sizeof scenes / sizeof scenes[0]))
        return NULL;
    scenes[nb_scenes].name = scene;
    return &scenes[nb_scenes++];
}

/* The allocation of each target: the draw buffer is 64064 bytes from DrawBufferNormalIni (or DrawBufferNormal
   before the first level), its logical origin DrawBufferNormal moves within it (update_display_map). */
static int target_range(int target, uintptr_t *start, uintptr_t *end, uintptr_t *origin)
{
    switch (target) {
    case TARGET_DRAW:
        if (!DrawBufferNormal)
            return 0;
        *start = (uintptr_t)(DrawBufferNormalIni ? DrawBufferNormalIni : DrawBufferNormal);
        *end = *start + 64064;
        *origin = (uintptr_t)DrawBufferNormal;
        return 1;
    case TARGET_DISPLAY:
        if (!DisplayBufferNormal)
            return 0;
        *start = *origin = (uintptr_t)DisplayBufferNormal;
        *end = *start + 64000;
        return 1;
    case TARGET_EFFECT:
        if (!EffetBufferNormal)
            return 0;
        *start = *origin = (uintptr_t)EffetBufferNormal;
        *end = *start + 128000;
        return 1;
    }
    return 0;
}

/* The target whose allocation holds [p, p + n), and the linear offset of p from its origin. */
static int region(const void *p, size_t n, int64_t *offset)
{
    uintptr_t a = (uintptr_t)p, start, end, origin;
    int target;
    for (target = 0; target < TARGET_OTHER; target++)
        if (target_range(target, &start, &end, &origin) && a >= start && n <= end - start && a + n <= end) {
            *offset = (int64_t)(a - origin);
            return target;
        }
    return TARGET_OTHER;
}

/* Does [p, p + n) overlap any target? */
static int touches_video(const void *p, size_t n)
{
    uintptr_t a = (uintptr_t)p, start, end, origin;
    int target;
    for (target = 0; target < TARGET_OTHER; target++)
        if (target_range(target, &start, &end, &origin) && a < end && a + n > start)
            return 1;
    return 0;
}

static int locate(const uint8_t *p, int32_t *x, int32_t *y)
{
    int64_t off;
    int target = region(p, 1, &off);
    if (target == TARGET_OTHER)
        return target;
    *x = (int32_t)(off >= 0 ? off % 320 : 319 - (-off - 1) % 320);
    *y = (int32_t)(off >= 0 ? off / 320 : -1 - (-off - 1) / 320);
    return target;
}

static port_op *append(int kind, int target);
static void unsupported(int prim, const uint8_t *dest);

static int claimed(void)
{
    return claim_depth || pass == PORT_PASS_BACKGROUND || pass == PORT_PASS_MAP || pass == PORT_PASS_FRONT;
}

void port_record_claim(int on) { claim_depth += on ? 1 : -1; }

static const uint8_t *keep(const void *source, size_t n)
{
    uint8_t *p;
    if (n > sizeof arena - arena_used)
        return NULL;
    p = arena + arena_used;
    memcpy(p, source, n);
    arena_used += n;
    return p;
}

/* Verification diagnostics: the first memory operations the recorder cannot represent, located relative to the
   video buffers. */
static void note(const char *what, const void *dest, const void *source, size_t n)
{
    static int notes;
    if (!verify || notes >= 20)
        return;
    notes++;
    fprintf(stderr, "render note: %s of %zu bytes in %s, pass %s, frame %ld: dest draw%+lld display%+lld, source draw%+lld"
            " display%+lld\n", what, n, scene, port_pass_name(pass), st.frames[0] + st.frames[1],
            (long long)((intptr_t)dest - (intptr_t)DrawBufferNormal), (long long)((intptr_t)dest - (intptr_t)DisplayBufferNormal),
            (long long)((intptr_t)source - (intptr_t)DrawBufferNormal),
            (long long)((intptr_t)source - (intptr_t)DisplayBufferNormal));
}

static void record_fill(void *dest, uint8_t value, size_t n)
{
    int64_t off;
    int target;
    port_op *op;
    if (!n || !touches_video(dest, n) || claimed())
        return;
    target = region(dest, n, &off);
    if (target == TARGET_OTHER || n > INT32_MAX) {
        note("fill", dest, NULL, n);
        unsupported(PORT_PRIM_MEMORY, NULL);
        return;
    }
    op = append(PORT_OP_FILL, target);
    if (op) {
        op->x = (int32_t)off;
        op->w = (int32_t)n;
        op->color = value;
    }
}

static void record_copy(void *dest, const void *source, size_t n)
{
    int64_t off, src_off = 0;
    int target, source_target;
    port_op *op;
    if (!n || claimed())
        return;
    if (!touches_video(dest, n)) {
        /* a copy out of a video buffer into game memory: the game keeps original pixels (read-back) */
        if (touches_video(source, n)) {
            note("read-back", dest, source, n);
            unsupported(PORT_PRIM_READBACK, NULL);
        }
        return;
    }
    target = region(dest, n, &off);
    source_target = touches_video(source, n) ? region(source, n, &src_off) : -1;
    if (target == TARGET_OTHER || source_target == TARGET_OTHER || n > INT32_MAX) {
        note("copy", dest, source, n);
        unsupported(PORT_PRIM_MEMORY, NULL);
        return;
    }
    op = append(PORT_OP_COPY, target);
    if (!op)
        return;
    op->x = (int32_t)off;
    op->w = (int32_t)n;
    if (source_target >= 0) {
        op->flags = 1;
        op->color = (uint8_t)source_target;
        op->y = (int32_t)src_off;
    } else if (!(op->image = keep(source, n))) {
        op->kind = PORT_OP_UNSUPPORTED;
        op->flags = PORT_PRIM_MEMORY;
        rec.unsupported++;
        st.unsupported[pass][PORT_PRIM_MEMORY]++;
    }
}

/* The historical units' memcpy/memmove/memset (compat/include/port_watcom.h): copies and fills into the video
   buffers are drawing operations; every call then runs the C library's. */
void *port_memcpy(void *dest, const void *source, size_t n)
{
    record_copy(dest, source, n);
    return memcpy(dest, source, n);
}

void *port_memmove(void *dest, const void *source, size_t n)
{
    record_copy(dest, source, n);
    return memmove(dest, source, n);
}

void *port_memset(void *dest, int value, size_t n)
{
    record_fill(dest, (uint8_t)value, n);
    return memset(dest, value, n);
}

/* Operations of the frame that wrote the display outside a swap (the movie player copies each decoded frame to
   the display and presents it without swapping: outside gameplay frames such a frame ends at the presentation). */
static int display_ops;
static int swapping;

/* The draw buffer's origin as the operations recorded so far assume it. */
static uint8_t *recorded_origin;

static port_op *append_raw(int kind, int target);

/* Record a move of the draw buffer's origin; called before every operation and at every pass boundary, so the
   move belongs to the segment in which the engine made it. */
static void note_origin(void)
{
    if (DrawBufferNormal != recorded_origin) {
        if (recorded_origin && DrawBufferNormal) {
            intptr_t delta = (intptr_t)(DrawBufferNormal - recorded_origin);
            port_op *op = delta > -64064 && delta < 64064 ? append_raw(PORT_OP_ORIGIN, TARGET_DRAW) : NULL;
            if (op)
                op->x = (int32_t)delta;
        }
        recorded_origin = DrawBufferNormal;
    }
}

static port_op *append(int kind, int target)
{
    note_origin();
    return append_raw(kind, target);
}

static port_op *append_raw(int kind, int target)
{
    port_op *op;
    if (target == TARGET_DISPLAY)
        rec.display_writes++;
    if (rec.nb_ops >= PORT_FRAME_MAX_OPS) {
        rec.overflow++;
        return NULL;
    }
    if (target == TARGET_DISPLAY && !swapping)
        display_ops++;
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
    int32_t x = 0, y = 0;
    int target = dest ? locate(dest, &x, &y) : TARGET_DRAW;
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
static void note_origin(void);

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
        op->clip[0] = x0;
        op->clip[1] = x1;
        op->clip[2] = y0;
        op->clip[3] = y1;
    }
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

int port_record_target(const void *p, int64_t *offset) { return region(p, 1, offset); }

void port_record_unsupported_effect(const void *p) { unsupported(PORT_PRIM_EFFECT, (const uint8_t *)p); }

static port_op *payload_op(int kind, const void *payload, size_t size)
{
    port_op *op;
    size_t aligned = (arena_used + 7) & ~(size_t)7;
    if (claimed() || aligned > sizeof arena || size > sizeof arena - aligned)
        return NULL;
    if (!(op = append(kind, TARGET_DRAW)))
        return NULL;
    arena_used = aligned;
    op->image = keep(payload, size);
    return op;
}

int port_record_deform(const port_deform *e)
{
    port_op *op = payload_op(PORT_OP_DEFORM, e, sizeof *e);
    if (op)
        op->w = e->slot;
    return op != NULL;
}

int port_record_effect(int effect, const void *payload, size_t size)
{
    port_op *op;
    size_t aligned = (arena_used + 7) & ~(size_t)7;
    if (claimed() || aligned > sizeof arena || size > sizeof arena - aligned)
        return 0;
    if (!(op = append(PORT_OP_EFFECT, TARGET_DRAW)))
        return 0;
    arena_used = aligned;
    op->flags = (uint8_t)effect;
    op->image = keep(payload, size);
    return 1;
}

/* ---- primitive wrappers ---- */

#define SPRITE_WRAP(name, variant) \
    void __real_##name(int32_t x, int32_t c, int32_t y, uint32_t size, uint8_t *d, uint8_t *s); \
    void __wrap_##name(int32_t x, int32_t c, int32_t y, uint32_t size, uint8_t *d, uint8_t *s) \
    { \
        record_sprite(variant, x, c, y, size, d, s); \
        __real_##name(x, c, y, size, d, s); \
    }

/* Scratch buffers (render/effects.h): game memory the deform rasterizer writes and sprites then read. Registered by
   address when first written; the native renderer keeps its own version in the matching slot. */
static struct { uintptr_t start; size_t size; } scratch_ranges[PORT_SCRATCH_SLOTS];

/* The slot whose range holds [p, p + n); with `create`, the range starting at p is registered or grown to n bytes
   (the footprint the deform rasterizer fills). */
static int scratch_slot(const void *p, size_t n, int create)
{
    uintptr_t a = (uintptr_t)p;
    int i;
    for (i = 0; i < PORT_SCRATCH_SLOTS; i++) {
        if (!scratch_ranges[i].size)
            continue;
        if (create && a == scratch_ranges[i].start) {
            if (n > scratch_ranges[i].size)
                scratch_ranges[i].size = n;
            return i;
        }
        if (!create && a >= scratch_ranges[i].start && a + n <= scratch_ranges[i].start + scratch_ranges[i].size)
            return i;
    }
    if (create && n && n <= PORT_SCRATCH_BYTES)
        for (i = 0; i < PORT_SCRATCH_SLOTS; i++)
            if (!scratch_ranges[i].size) {
                scratch_ranges[i].start = a;
                scratch_ranges[i].size = n;
                return i;
            }
    return -1;
}

static uint32_t image_hash(const uint8_t *p, size_t n)
{
    uint32_t h = 2166136261u;
    size_t i;
    for (i = 0; i < n; i++)
        h = (h ^ p[i]) * 16777619u;
    return h | 1;
}

static void record_sprite(int variant, int32_t x, int32_t c, int32_t y, uint32_t size, uint8_t *d, uint8_t *s)
{
    int32_t ox, oy;
    int target = locate(d, &ox, &oy);
    port_op *op;
    if (target == TARGET_OTHER || claimed())
        return;
    op = append(PORT_OP_SPRITE, target);
    if (!op)
        return;
    op->flags = (uint8_t)variant;
    op->color = (uint8_t)c;
    op->x = x;
    op->y = y;
    op->base = 320 * oy + ox;   /* the drawers take a buffer and coordinates relative to it */
    op->size = size;
    get_sprite_clipping(&op->clip[0], &op->clip[1], &op->clip[2], &op->clip[3]);
    /* Sprites are rows of `width` bytes. The zero-transparent 256-colour sprites are the loader animation's, decoded
       into a buffer the loader reuses for its next picture within the same frame: their bytes are kept. Other
       sprites come from the level's and the fixed atlases, unchanged until a load; verification checks that. */
    if (!(variant & PORT_SPRITE_RAW) && (op->w = scratch_slot(s, (size_t)(size & 255) * ((size >> 8) & 255), 0)) >= 0) {
        /* a deformed sprite: its image is the native scratch slot the deform operation wrote */
        op->flags |= PORT_SPRITE_SCRATCH;
        op->h = (int32_t)((uintptr_t)s - scratch_ranges[op->w].start);
        op->image = NULL;
    } else if (variant & PORT_SPRITE_RAW) {
        if (!(op->image = keep(s, (size_t)(size & 255) * ((size >> 8) & 255) + 1))) {
            op->kind = PORT_OP_UNSUPPORTED;
            op->flags = PORT_PRIM_SPRITE;
            rec.unsupported++;
            st.unsupported[pass][PORT_PRIM_SPRITE]++;
        }
    } else {
        op->image = s;
        if (verify)
            op->check = image_hash(s, (size_t)(size & 255) * ((size >> 8) & 255));
    }
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

/* The box drawers address buffer + 320 y + x: the operation keeps that linear offset. */
static port_op *record_box(int kind, uint8_t *buffer, int32_t x, int32_t y, int32_t height, int32_t width)
{
    int64_t off;
    int target = region(buffer, 1, &off);
    port_op *op;
    if (target == TARGET_OTHER || claimed() || !(op = append(kind, target)))
        return NULL;
    op->x = (int32_t)(off + (int64_t)y * 320 + x);
    op->w = width;
    op->h = height;
    return op;
}

void __real_DrawFondBoxNormal(uint8_t *buffer, int32_t x, int32_t y, int32_t height, int32_t width, int32_t type);
void __wrap_DrawFondBoxNormal(uint8_t *buffer, int32_t x, int32_t y, int32_t height, int32_t width, int32_t type)
{
    port_op *op = record_box(PORT_OP_FOND_BOX, buffer, x, y, height, width);
    if (op)
        op->flags = (uint8_t)(type == 2 ? 2 : 1);
    __real_DrawFondBoxNormal(buffer, x, y, height, width, type);
}
void __real_DrawBorderBoxNormal(uint8_t *buffer, int32_t x, int32_t y, int32_t height, int32_t width, uint16_t colors);
void __wrap_DrawBorderBoxNormal(uint8_t *buffer, int32_t x, int32_t y, int32_t height, int32_t width, uint16_t colors)
{
    port_op *op = record_box(PORT_OP_BORDER_BOX, buffer, x, y, height, width);
    if (op)
        op->size = colors;
    __real_DrawBorderBoxNormal(buffer, x, y, height, width, colors);
}
void __real_DisplayAnyPictureNormal(uint8_t *source, uint8_t *dest, int32_t sx, int32_t sy, int32_t dx, int32_t dy,
                                    int32_t stride, int32_t width, int32_t height);
void __wrap_DisplayAnyPictureNormal(uint8_t *source, uint8_t *dest, int32_t sx, int32_t sy, int32_t dx, int32_t dy,
                                    int32_t stride, int32_t width, int32_t height)
{
    int32_t ox, oy, row;
    int target = locate(dest, &ox, &oy);
    if (target != TARGET_OTHER && !claimed()) {
        port_op *op;
        if (width <= 0 || height <= 0 || (size_t)width * (size_t)height > sizeof arena - arena_used ||
            touches_video(source + (ptrdiff_t)sy * stride + sx, 1)) {
            unsupported(PORT_PRIM_PICTURE, dest);
        } else if ((op = append(PORT_OP_PICTURE, target)) != NULL) {
            op->x = ox + dx;
            op->y = oy + dy;
            op->w = width;
            op->h = height;
            op->image = arena + arena_used;
            for (row = 0; row < height; row++)
                keep(source + (ptrdiff_t)(sy + row) * stride + sx, (size_t)width);
        }
    }
    __real_DisplayAnyPictureNormal(source, dest, sx, sy, dx, dy, stride, width, height);
}

/* Asset decoders that write straight into video memory (the menus' PCX pictures go to the effect buffer): the
   decoder is game code, not drawing, and runs once; its output, read from the buffer when it returns, becomes one
   image operation. No drawing primitive has touched those bytes. */
static void record_asset(uint8_t *buffer, size_t n)
{
    int64_t off;
    int target;
    port_op *op;
    if (!n || claimed())
        return;
    target = region(buffer, n, &off);
    if (target == TARGET_OTHER) {
        if (touches_video(buffer, n)) {
            note("asset", buffer, NULL, n);
            unsupported(PORT_PRIM_MEMORY, NULL);
        }
        return;
    }
    if (!(op = append(PORT_OP_COPY, target)))
        return;
    op->x = (int32_t)off;
    op->w = (int32_t)n;
    if (!(op->image = keep(buffer, n))) {
        op->kind = PORT_OP_UNSUPPORTED;
        op->flags = PORT_PRIM_MEMORY;
        rec.unsupported++;
        st.unsupported[pass][PORT_PRIM_MEMORY]++;
    }
}

void port_record_asset(void *buffer, size_t n) { record_asset((uint8_t *)buffer, n); }

/* File reads straight into video memory (the movie player reads its compressed frames into the effect buffer):
   the bytes come from the file, an image operation stands for them. */
int __real_port_read(int fd, void *buf, unsigned count);
int __wrap_port_read(int fd, void *buf, unsigned count)
{
    int n = __real_port_read(fd, buf, count);
    if (n > 0)
        record_asset((uint8_t *)buf, (size_t)n);
    return n;
}
int32_t __real__dos_read(int32_t fd, void *buffer, uint32_t count, uint32_t *actual);
int32_t __wrap__dos_read(int32_t fd, void *buffer, uint32_t count, uint32_t *actual)
{
    int32_t r = __real__dos_read(fd, buffer, count, actual);
    if (!r && actual && *actual)
        record_asset((uint8_t *)buffer, *actual);
    return r;
}

/* src/load.c LoadPcxOptions: decodes a PCX picture of width x height bytes into buffer. */
uint8_t *__real_LoadPcxOptions(uint8_t *buffer, int32_t resource_id, uint16_t *width, uint16_t *height);
uint8_t *__wrap_LoadPcxOptions(uint8_t *buffer, int32_t resource_id, uint16_t *width, uint16_t *height)
{
    uint8_t *result;
    claim_depth++;
    result = __real_LoadPcxOptions(buffer, resource_id, width, height);
    claim_depth--;
    record_asset(buffer, (size_t)*width * *height);
    return result;
}

/* Original inline helpers (compat/src/inline_helpers.c): 64000-byte clears and forward copies. */
void __real_fill(void *destination);
void __wrap_fill(void *destination)
{
    record_fill(destination, 0, 64000);
    __real_fill(destination);
}
void __real_fill_cld(void *destination);
void __wrap_fill_cld(void *destination)
{
    record_fill(destination, 0, 64000);
    __real_fill_cld(destination);
}
void __real_copy(const void *source, void *destination, uint32_t count);
void __wrap_copy(const void *source, void *destination, uint32_t count)
{
    record_copy(destination, source, count);
    __real_copy(source, destination, count);
}
void __real_copy_guarded(const void *source, void *destination, uint32_t count);
void __wrap_copy_guarded(const void *source, void *destination, uint32_t count)
{
    record_copy(destination, source, count);
    __real_copy_guarded(source, destination, count);
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
    record_fill(buffer, 0, 64000);
    __real_N_CLRSCR(buffer);
}
void __real_ClearDrawAndDisplayBufferNormal(uint8_t *draw, uint8_t *display);
void __wrap_ClearDrawAndDisplayBufferNormal(uint8_t *draw, uint8_t *display)
{
    record_fill(draw, 0, 64000);
    record_fill(display, 0, 64000);
    __real_ClearDrawAndDisplayBufferNormal(draw, display);
}
/* compat/src/dos_clib.c: the Watcom-library memory clear. */
void __real_clear_memory(void *destination, int value, int length);
void __wrap_clear_memory(void *destination, int value, int length)
{
    if (length > 0)
        record_fill(destination, (uint8_t)value, (size_t)length);
    __real_clear_memory(destination, value, length);
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
    port_deform e;
    int32_t ox, oy;
    if (locate(dest, &ox, &oy) != TARGET_OTHER) {
        unsupported(PORT_PRIM_DEFORM, dest);
    } else if (!claimed()) {
        memset(&e, 0, sizeof e);
        memcpy(e.geometry, geometry, sizeof e.geometry);
        e.source = source;
        e.slot = scratch_slot(dest, (size_t)(4 * ((geometry->dest_width >> 2) + 1)) * (size_t)geometry->dest_height, 1);
        if (e.slot < 0 || !port_record_deform(&e))
            unsupported(PORT_PRIM_DEFORM, NULL);
    }
    __real_draw_sprite_deform(source, dest, geometry);
}
/* The segment rasterizer's texture must be video memory the native renderer owns (the effects stage it in the
   effect buffer); otherwise the segment is not drawn natively. */
void __real_draw_effect_segment(uint8_t *dest, uint8_t *descriptor);
void __wrap_draw_effect_segment(uint8_t *dest, uint8_t *descriptor)
{
    port_effect_segment e;
    uint8_t *texture;
    int recorded = 0;
    memset(&e, 0, sizeof e);
    memcpy(&texture, descriptor, sizeof texture);
    if (!claimed()) {
        e.texture.target = region(texture, 65536, &e.texture.offset);
        e.destination.target = region(dest, 1, &e.destination.offset);
        if (e.texture.target != TARGET_OTHER && e.destination.target != TARGET_OTHER) {
            memcpy(e.vertices, descriptor + sizeof texture, sizeof e.vertices);
            memcpy(e.uv, descriptor + sizeof texture + sizeof e.vertices, sizeof e.uv);
            recorded = port_record_effect(PORT_EFFECT_SEGMENT, &e, sizeof e);
        } else {
            note("segment texture", dest, texture, 65536);
        }
    }
    if (!recorded)
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
                    fprintf(stderr, "render verify: %s (%s, %s) differs in frame %ld at (%d,%d): original %u, native %u\n",
                            what, scene, port_pass_name(p), st.frames[0] + st.frames[1], x, y, w[x], g[x]);
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
    note_origin();
    if (!verify || !DrawBufferNormal)
        return;
    port_native_draw(&rec, segment_first, rec.nb_ops, check, 0);
    if (segment_origin == DrawBufferNormal)
        bad = compare_rect(DrawBufferNormal, &check_draw, Bloc_lim_W1, Bloc_lim_H1, Bloc_lim_W2, Bloc_lim_H2,
                           segment_pass);
    else
        st.origin_moves++;
    bad += compare_rect(DisplayBufferNormal, &check_display, 0, 0, 320, 200, segment_pass);
    if (EffetBufferNormal)
        bad += compare_rect(EffetBufferNormal, &check_effect, 0, 0, 320, 400, segment_pass);
    st.segments[segment_pass]++;
    if (bad) {
        st.bad_segments[segment_pass]++;
        st.bad_px[segment_pass] += bad;
    }
    memcpy(check_draw_px, DrawBufferNormal, 64000);
    memcpy(check_display_px, DisplayBufferNormal, 64000);
    if (EffetBufferNormal)
        memcpy(check_effect_px, EffetBufferNormal, 128000);
    segment_origin = DrawBufferNormal;
    segment_first = rec.nb_ops;
}

/* Draw the operations recorded since the last flush into the native surfaces (at swaps and presents). */
/* Debugging aid (RAYMAN_RENDER_EXPLAIN="FRAME X Y [TARGET]"): in frame FRAME, print every native operation that
   changes pixel (X, Y) of the target's surface (0 draw, 1 display, 2 effect buffer). */
static void explain(int first, int last)
{
    static long frame = -2, x, y, target;
    int i;
    if (frame == -2) {
        const char *e = getenv("RAYMAN_RENDER_EXPLAIN");
        frame = -1;
        if (e && sscanf(e, "%ld %ld %ld %ld", &frame, &x, &y, &target) < 3)
            frame = -1;
    }
    if (frame < 0 || st.frames[0] + st.frames[1] != frame || target < 0 || target >= TARGET_OTHER) {
        port_native_draw(&rec, first, last, native, pad);
        return;
    }
    if (getenv("RAYMAN_RENDER_EXPLAIN_ALL"))
        for (i = first; i < last; i++)
            fprintf(stderr, "render explain: frame %ld op %d %s/%s target %u x %d y %d w %d h %d size %u color %u "
                    "flags %u\n", frame, i, port_pass_name(rec.ops[i].pass), port_op_name(rec.ops[i].kind),
                    rec.ops[i].target, rec.ops[i].x, rec.ops[i].y, rec.ops[i].w, rec.ops[i].h, rec.ops[i].size,
                    rec.ops[i].color, rec.ops[i].flags);
    for (i = first; i < last; i++) {
        const port_surface *s = native[target];
        const uint8_t *p = s->pixels + (y - s->y0) * s->pitch + (x - s->x0);
        uint8_t before = *p;
        port_native_draw(&rec, i, i + 1, native, pad);
        if (*p != before)
            fprintf(stderr, "render explain: frame %ld op %d %s/%s (target %u) changes (%ld,%ld): %u -> %u\n",
                    frame, i, port_pass_name(rec.ops[i].pass), port_op_name(rec.ops[i].kind), rec.ops[i].target, x, y,
                    before, *p);
    }
}

/* Verification: a game-owned image must still hold the bytes it held when the operation was recorded. */
static void check_images(int first, int last)
{
    int i;
    for (i = first; i < last; i++) {
        const port_op *op = &rec.ops[i];
        if (op->kind == PORT_OP_SPRITE && op->check &&
            image_hash(op->image, (size_t)(op->size & 255) * ((op->size >> 8) & 255)) != op->check) {
            st.volatile_images++;
            if (st.reported < 24) {
                st.reported++;
                fprintf(stderr, "render verify: sprite image of op %d (%s, %s) changed before it was drawn in frame %ld\n",
                        i, scene, port_pass_name(op->pass), st.frames[0] + st.frames[1]);
            }
        }
    }
}

static void flush(void)
{
    if (verify && native_drawn < rec.nb_ops)
        check_images(native_drawn, rec.nb_ops);
    if ((native_mode || verify) && native_drawn < rec.nb_ops) {
        explain(native_drawn, rec.nb_ops);
        native_drawn = rec.nb_ops;
    }
}

static void swap_begin(const uint8_t *source, const uint8_t *dest, int32_t width, int32_t height)
{
    int32_t sx, sy, dx, dy;
    port_op *op;
    swapping++;
    boundary();
    if (locate(source, &sx, &sy) == TARGET_DRAW && locate(dest, &dx, &dy) == TARGET_DISPLAY &&
        (op = append(PORT_OP_SWAP, TARGET_DISPLAY)) != NULL) {
        op->x = sx;
        op->y = sy;
        op->clip[0] = dx;
        op->clip[1] = dy;
        op->w = width / 4 * 4;
        op->h = height;
    } else {
        unsupported(PORT_PRIM_BUFFER_COPY, dest);
    }
    flush();
}

/* Debugging aid (RAYMAN_RENDER_TRAP="FRAME X Y"): called when frame FRAME starts with the address of draw-buffer
   pixel (X, Y), so a debugger can watch who writes it. */
void port_record_trap(volatile uint8_t *pixel);
void __attribute__((noipa)) port_record_trap(volatile uint8_t *pixel) { (void)pixel; }

static void trap_check(void)
{
    static long frame = -2, x, y, target;
    if (frame == -2) {
        const char *e = getenv("RAYMAN_RENDER_TRAP");
        frame = -1;
        if (e && sscanf(e, "%ld %ld %ld %ld", &frame, &x, &y, &target) < 3)
            frame = -1;
    }
    if (frame >= 0 && st.frames[0] + st.frames[1] == frame) {
        uint8_t *base = target == TARGET_DISPLAY ? DisplayBufferNormal : target == TARGET_EFFECT ? EffetBufferNormal
                                                                                               : DrawBufferNormal;
        if (base)
            port_record_trap(base + 320 * y + x);
    }
}

/* The end of a frame: verification, statistics, and a fresh frame. */
static void end_frame(void)
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
        {
            struct scene_stats *s = scene_stats();
            if (s) {
                s->frames++;
                s->complete += complete;
                s->exact += complete && !bad_draw && !bad_display;
            }
        }
        st.draw_bad_px[g] += bad_draw;
        st.display_bad_px[g] += bad_display;
        if (EffetBufferNormal)
        {
            /* the effect buffer is off-screen: a difference there is reported, not counted against the frame */
            int i = 0;
            while (i < 128000 && native_effect_px[i] == EffetBufferNormal[i])
                i++;
            if (i == 128000)
                st.effect_exact[g]++;
            else if (st.reported < 24) {
                st.reported++;
                fprintf(stderr, "render verify: effect buffer (%s) differs in frame %ld at (%d,%d): original %u, native %u\n",
                        scene, st.frames[0] + st.frames[1] - 1, i % 320, i / 320, EffetBufferNormal[i],
                        native_effect_px[i]);
            }
        }
        memcpy(check_draw_px, DrawBufferNormal, 64000);
        memcpy(check_display_px, DisplayBufferNormal, 64000);
        if (EffetBufferNormal)
            memcpy(check_effect_px, EffetBufferNormal, 128000);
        segment_origin = DrawBufferNormal;
    }
    {
        int i;
        for (i = 0; i < rec.nb_ops; i++) {
            st.ops[g][rec.ops[i].pass][rec.ops[i].kind]++;
            if (rec.ops[i].kind == PORT_OP_EFFECT && rec.ops[i].flags < PORT_EFFECT_COUNT)
                st.effects[rec.ops[i].flags]++;
        }
    }
    st.display_writes += rec.display_writes;
    st.overflow += rec.overflow;
    rec.seq++;
    rec.gameplay = 0;
    rec.nb_ops = rec.overflow = rec.unsupported = rec.display_writes = 0;
    native_drawn = 0;
    arena_used = 0;
    segment_first = 0;
    display_ops = 0;
    if (verify)
        trap_check();
}

static void swap_end(void)
{
    swapping--;
    end_frame();
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

void port_record_set_native(int on) { native_mode = on; }
int port_record_native(void) { return native_mode; }
void port_record_set_pad(int value)
{
    pad = value < 0 ? 0 : value > MAX_PAD ? MAX_PAD : value;
    native_draw.x0 = -(MARGIN + pad);
    native_draw.width = native_draw.pitch = 320 + 2 * (MARGIN + pad);
    native_display.x0 = -pad;
    native_display.width = native_display.pitch = 320 + 2 * pad;
    memset(native_draw_px, 0, sizeof native_draw_px);
    memset(native_display_px, 0, sizeof native_display_px);
}

int port_record_pad(void) { return pad; }

const uint8_t *port_record_presented(int *width)
{
    flush();
    if (verify && DisplayBufferNormal) {
        st.presents++;
        st.present_exact += !memcmp(native_display_px, DisplayBufferNormal, 64000);
    }
    /* Scenes that write the display directly and never swap (the FLC movie player): a presentation ends their
       frame. */
    if (!swapping && !rec.gameplay && display_ops) {
        boundary();
        end_frame();
    }
    *width = native_display.width;
    return native_mode ? native_display_px : NULL;
}
void port_record_set_verify(int on) { verify = on; }
int port_record_verify(void) { return verify; }

void port_record_report(void)
{
    int p, k, g;
    if (!verify)
        return;
    printf("render frames: gameplay=%ld gameplay_complete=%ld gameplay_draw_exact=%ld gameplay_display_exact=%ld "
           "gameplay_complete_exact=%ld gameplay_draw_bad_px=%lld gameplay_display_bad_px=%lld other=%ld "
           "other_complete=%ld other_draw_exact=%ld other_display_exact=%ld overflow=%ld display_writes=%ld "
           "origin_moves=%ld presents=%ld present_exact=%ld "
           "effect_exact=%ld/%ld volatile_images=%ld\n",
           st.frames[1], st.complete[1], st.draw_exact[1], st.display_exact[1], st.complete_exact[1],
           st.draw_bad_px[1], st.display_bad_px[1], st.frames[0], st.complete[0], st.draw_exact[0],
           st.display_exact[0], st.overflow, st.display_writes, st.origin_moves, st.presents, st.present_exact, st.effect_exact[1],
           st.effect_exact[0], st.volatile_images);
    for (p = 0; p < PORT_PASS_COUNT; p++)
        if (st.segments[p])
            printf("render pass %s: segments=%ld bad_segments=%ld bad_px=%lld\n", port_pass_name(p), st.segments[p],
                   st.bad_segments[p], st.bad_px[p]);
    for (k = 1; k < PORT_EFFECT_COUNT; k++)
        if (st.effects[k])
            printf("render effect %s: %ld\n", port_effect_name(k), st.effects[k]);
    for (k = 0; k < nb_scenes; k++)
        printf("render scene %s: frames=%ld complete=%ld exact=%ld\n", scenes[k].name, scenes[k].frames,
               scenes[k].complete, scenes[k].exact);
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
