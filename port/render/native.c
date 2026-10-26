#include "render/native.h"

#include <string.h>

const char *port_pass_name(int pass)
{
    static const char *names[PORT_PASS_COUNT] = {
        "loop", "frame_start", "background", "rear_weather", "map", "stars", "objects", "particles", "front",
        "front_weather", "firefly", "fairy_text", "hud", "crackers", "programmer", "time", "vignette", "options",
        "pause", "keys"};
    return pass >= 0 && pass < PORT_PASS_COUNT ? names[pass] : "?";
}

const char *port_prim_name(int prim)
{
    static const char *names[PORT_PRIM_COUNT] = {
        "sprite", "sprite_diff", "tile", "plan0_copy", "pixel", "weather", "black_box", "fond_box", "border_box",
        "picture", "clear_borders", "clear_screen", "buffer_copy", "deform", "effect", "dword_copy"};
    return prim >= 0 && prim < PORT_PRIM_COUNT ? names[prim] : "?";
}

const char *port_op_name(int kind)
{
    static const char *names[PORT_OP_KIND_COUNT] = {
        "clear", "background", "map", "front", "sprite", "clear_borders", "pixel", "weather", "black_box",
        "clear_outside", "firefly", "unsupported"};
    return kind >= 0 && kind < PORT_OP_KIND_COUNT ? names[kind] : "?";
}

int port_native_supports(const port_op *op) { return op->kind != PORT_OP_UNSUPPORTED; }

static void put(port_surface *d, int32_t x, int32_t y, uint8_t v)
{
    if (x >= d->x0 && x < d->x0 + d->width && y >= d->y0 && y < d->y0 + d->height) {
        d->pixels[(y - d->y0) * d->pitch + (x - d->x0)] = v;
        if (d->written)
            d->written[(y - d->y0) * d->pitch + (x - d->x0)] = 1;
    }
}

static void fill(port_surface *d, int32_t x, int32_t y, int32_t n, uint8_t v)
{
    int32_t i;
    for (i = 0; i < n; i++)
        put(d, x + i, y, v);
}

/* DrawSprite*Normal* (ASM_SPEC clip_sprite_on_screen, port/video/lowlevel_normal.c): the clip box arithmetic
   with its unsigned upper comparisons, the left crop that advances the source only for unmirrored sprites, the
   right crop that advances it only for mirrored ones, and marker-run decoding from the cropped start (a crop can
   land inside a run, which the original then reads as data). Rows step by the uncropped width. */
static void sprite(port_surface *d, const port_op *op)
{
    int flip = op->flags & PORT_SPRITE_FLIP, raw = op->flags & PORT_SPRITE_RAW;
    uint8_t or_color = (op->flags & PORT_SPRITE_COLOR) ? (uint8_t)(op->color * 8) : 0;
    uint32_t size = op->size, w = size & 255, h = (size >> 8) & 255, stride = w, end;
    int32_t x = op->x, y = op->y;
    const int32_t *box = op->clip;
    const uint8_t *src = op->image;
    uint32_t row, i;
    if (!(op->flags & PORT_SPRITE_NOCLIP)) {
        end = w;
        if (x < box[0]) {
            uint32_t distance = (uint32_t)box[0] - (uint32_t)x;
            if (w <= distance)
                return;
            distance &= 0xffff;
            end = w - distance;
            size = (size & ~255u) | ((w - distance) & 255);
            if (!flip)
                src += distance;
            x = box[0];
        }
        end += (uint32_t)x;
        if (end > (uint32_t)box[1]) {
            if ((uint32_t)x >= (uint32_t)box[1])
                return;
            end = (uint32_t)box[1] - (uint32_t)x;
            size = (size & ~255u) | (end & 255);
            if (flip)
                src += stride - end;
        }
        end = (size >> 8) & 255;
        if (y < box[2]) {
            uint32_t skip;
            end += (uint32_t)y;
            if ((int32_t)end <= box[2])
                return;
            skip = ((uint32_t)box[2] - (uint32_t)y) & 65535;
            size = (size & ~0xff00u) | (((h - skip) & 255) << 8);
            src += (size_t)stride * skip;
            y = box[2];
            end = (size >> 8) & 255;
        }
        end += (uint32_t)y;
        if (end > (uint32_t)box[3]) {
            if ((uint32_t)y >= (uint32_t)box[3])
                return;
            end = (uint32_t)box[3] - (uint32_t)y;
            size = (size & ~0xff00u) | ((end & 255) << 8);
        }
        if (!(size & 255))
            return;
    }
    w = size & 255;
    h = (size >> 8) & 255;
    for (row = 0; row < h; row++) {
        const uint8_t *s = src + (size_t)row * stride;
        for (i = 0; i < w;) {
            uint8_t v = s[i];
            if (raw) {
                if (v)
                    put(d, x + (int32_t)(flip ? w - 1 - i : i), y + (int32_t)row, v);
                i++;
            } else if (v < 160) {
                put(d, x + (int32_t)(flip ? w - 1 - i : i), y + (int32_t)row, (uint8_t)(v | or_color));
                i++;
            } else {
                i += v - 160u;
            }
        }
    }
}

/* draw_flocon*_Normal / draw_pluie*_Normal stamps (ASM_SPEC): the anchor is tested against the bounds set by
   flocon_clipping (upper bounds compared unsigned, inclusive); the whole stamp is then written. */
static void weather(port_surface *d, const port_op *op)
{
    static const struct { const char *pattern; int w, h; } stamps[] = {
        {"7", 1, 1}, {"7867", 2, 2}, {".6.866.6.", 3, 3}, {"3", 1, 1}, {"5", 1, 1},
        {"..66...7887.688886688886.7887...66..", 6, 6}, {".66.68866886.66.", 4, 4}, {".55.57755775.55.", 4, 4},
        {".45.", 2, 2}, {".4253236.", 3, 3}, {"..32.324543.46..", 4, 4}};
    const int32_t *b = op->clip;
    int r, c;
    if (op->flags >= sizeof stamps / sizeof stamps[0])
        return;
    if (op->x < b[2] || (uint32_t)op->x > (uint32_t)b[3] || op->y < b[0] || (uint32_t)op->y > (uint32_t)b[1])
        return;
    for (r = 0; r < stamps[op->flags].h; r++)
        for (c = 0; c < stamps[op->flags].w; c++) {
            char v = stamps[op->flags].pattern[r * stamps[op->flags].w + c];
            if (v != '.')
                put(d, op->x + c, op->y + r, (uint8_t)(v - '0'));
        }
}

/* plot2line (src/luciole.c): zero the spans of rows centre_y -/+ row_offset outside [cx - inner, cx + inner] and
   inside the outer radius, clamped to the full viewport. */
static void firefly_rows(port_surface *d, const port_op *op, int32_t inner, int32_t row_offset)
{
    const int32_t *g = op->clip;   /* W1, W2, H1, H2 of the full viewport */
    int32_t radius = (int16_t)op->size + op->h, cx = op->x, cy = op->y;
    int32_t outer_left = cx - radius, outer_right = cx + radius, inner_left = cx - inner, inner_right = cx + inner;
    int32_t top = cy - row_offset, bottom = cy + row_offset;
    if (outer_left < g[0])
        outer_left = g[0];
    if (inner_left > g[1])
        inner_left = g[1];
    if (outer_right > g[1])
        outer_right = g[1];
    if (inner_right < g[0])
        inner_right = g[0];
    if (inner_left - outer_left > 0) {
        if (top >= g[2] && top < g[3])
            fill(d, outer_left, top, inner_left - outer_left, 0);
        if (bottom >= g[2] && bottom < g[3])
            fill(d, outer_left, bottom, inner_left - outer_left, 0);
    }
    if (outer_right - inner_right > 0) {
        if (top >= g[2] && top < g[3])
            fill(d, inner_right, top, outer_right - inner_right, 0);
        if (bottom >= g[2] && bottom < g[3])
            fill(d, inner_right, bottom, outer_right - inner_right, 0);
    }
}

/* aff_luciole (src/luciole.c), radius rayon_luciole - 4: full rows from the radius to ADDLUCLIP beyond it, then the
   midpoint circle's spans. */
static void firefly(port_surface *d, const port_op *op)
{
    int32_t radius = (int16_t)op->size - 4, x = radius, i, y, dd, err;
    for (i = 0, y = radius; i <= op->h; ++i, ++y)
        firefly_rows(d, op, 0, y);
    if (x < 0)
        return;
    i = 0;
    y = 0;
    dd = 1;
    err = 2 * x - 1;
    do {
        firefly_rows(d, op, i, x);
        firefly_rows(d, op, x, i);
        ++i;
        y += dd;
        dd += 2;
        if (x <= y) {
            --x;
            y -= err;
            err -= 2;
        }
    } while (i <= x);
}

void port_native_draw(const port_frame *f, int first, int last, port_surface *draw, port_surface *display)
{
    int i;
    int32_t y;
    for (i = first; i < last; i++) {
        const port_op *op = &f->ops[i];
        port_surface *dst = op->target ? display : draw;
        port_rect view = {op->clip[0], op->clip[2], op->clip[1], op->clip[3]};
        port_rect all;
        if (!dst)
            continue;
        all.x0 = dst->x0;
        all.y0 = dst->y0;
        all.x1 = dst->x0 + dst->width;
        all.y1 = dst->y0 + dst->height;
        switch (op->kind) {
        case PORT_OP_CLEAR:
            /* N_CLRSCR: 64000 bytes; the loop's clear: 16008 dwords, 32 bytes beyond the 200 rows */
            for (y = 0; y < op->w / 320; y++)
                fill(dst, 0, y, 320, 0);
            fill(dst, 0, op->w / 320, op->w % 320, 0);
            break;
        case PORT_OP_BACKGROUND:
            port_render_background(&f->world, dst, view, PORT_RENDER_ORIGINAL);
            break;
        case PORT_OP_MAP:
            port_render_map(&f->world, dst, view, PORT_RENDER_ORIGINAL);
            break;
        case PORT_OP_FRONT:
            port_render_front(&f->world, dst, all, PORT_RENDER_ORIGINAL);
            break;
        case PORT_OP_SPRITE:
            sprite(dst, op);
            break;
        case PORT_OP_CLEAR_BORDERS:
            /* clear_borders_Normal: 4 bytes at the row start and at +right, on height rounded down to even */
            for (y = 0; y < op->h / 2 * 2; y++) {
                fill(dst, op->x, op->y + y, 4, 0);
                fill(dst, op->x + op->w, op->y + y, 4, 0);
            }
            break;
        case PORT_OP_PIXEL:
            put(dst, op->x, op->y, op->color);
            break;
        case PORT_OP_WEATHER:
            weather(dst, op);
            break;
        case PORT_OP_CLEAR_OUTSIDE:
            /* ClearBorder (src/box.c 408): rows above y0 and from y1, then columns left of x0 and from x1 */
            for (y = 0; y < op->clip[2]; y++)
                fill(dst, 0, y, 320, 0);
            for (y = op->clip[3]; y < 200; y++)
                fill(dst, 0, y, 320, 0);
            for (y = op->clip[2]; y < op->clip[3]; y++) {
                fill(dst, 0, y, op->clip[0], 0);
                fill(dst, op->clip[1], y, 320 - op->clip[1], 0);
            }
            break;
        case PORT_OP_FIREFLY:
            firefly(dst, op);
            break;
        case PORT_OP_BLACK_BOX:
            for (y = 0; y < op->h; y++)
                fill(dst, op->x, op->y + y, op->w, 0);
            break;
        default:
            break;
        }
    }
}
