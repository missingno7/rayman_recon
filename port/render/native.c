#include "render/native.h"
#include "render/effects.h"

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
        "picture", "clear_borders", "clear_screen", "buffer_copy", "deform", "effect", "dword_copy", "memory",
        "readback"};
    return prim >= 0 && prim < PORT_PRIM_COUNT ? names[prim] : "?";
}

const char *port_op_name(int kind)
{
    static const char *names[PORT_OP_KIND_COUNT] = {
        "background", "map", "front", "sprite", "clear_borders", "pixel", "weather", "black_box",
        "fill", "copy", "picture", "swap", "border_box", "fond_box", "deform", "effect", "origin", "clear_outside", "firefly", "unsupported"};
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

/* Video memory is 320-byte rows: linear offset n is pixel (n mod 320, n div 320), rounding towards minus infinity. */
static void put_linear(port_surface *d, int64_t n, uint8_t v)
{
    int64_t y = n >= 0 ? n / 320 : -1 - (-n - 1) / 320;
    put(d, (int32_t)(n - 320 * y), (int32_t)y, v);
}

static uint8_t get_linear(const port_surface *d, int64_t n)
{
    int64_t y = n >= 0 ? n / 320 : -1 - (-n - 1) / 320, x = n - 320 * y;
    if (x < d->x0 || x >= d->x0 + d->width || y < d->y0 || y >= d->y0 + d->height)
        return 0;
    return d->pixels[(y - d->y0) * d->pitch + (x - d->x0)];
}

static uint8_t get_pixel(const port_surface *d, int32_t x, int32_t y)
{
    if (x < d->x0 || x >= d->x0 + d->width || y < d->y0 || y >= d->y0 + d->height)
        return 0;
    return d->pixels[(y - d->y0) * d->pitch + (x - d->x0)];
}

static void fill(port_surface *d, int32_t x, int32_t y, int32_t n, uint8_t v)
{
    int32_t i;
    for (i = 0; i < n; i++)
        put(d, x + i, y, v);
}

/* A sprite pixel at (x, y) relative to the buffer the drawer was given: base + 320 y + x. */
static void put_sprite(port_surface *d, int32_t base, int32_t x, int32_t y, uint8_t v)
{
    if (base)
        put_linear(d, (int64_t)base + (int64_t)y * 320 + x, v);
    else
        put(d, x, y, v);
}

/* DrawSprite*Normal* (ASM_SPEC clip_sprite_on_screen, port/video/lowlevel_normal.c): the clip box arithmetic
   with its unsigned upper comparisons, the left crop that advances the source only for unmirrored sprites, the
   right crop that advances it only for mirrored ones, and marker-run decoding from the cropped start (a crop can
   land inside a run, which the original then reads as data). Rows step by the uncropped width. */
/* Wider view: a horizontal bound pair that is the original full viewport ([0..8, 312..320]) widens to the window. */
static int full_width(int32_t x0, int32_t x1) { return x0 <= 8 && x1 >= 312; }

/* The wider view's window onto the world, in the game's screen coordinates (world x = screen x + xmap - 8):
   [-pad, 320 + pad) moved by the presentation offset dx, so that it stays inside the level map. The game's camera
   is not touched; at a level edge the window stops and the original 4:3 area moves off centre. A map narrower than
   the window is centred, and the window is cut to the map (black beyond it). */
typedef struct view_window { int32_t x0, x1, dx; } view_window;

static view_window world_window(const port_scene *s, int pad)
{
    view_window w = {-pad, 320 + pad, 0};
    if (s->map_width > 0) {
        int32_t lo = 8 - s->xmap + pad, hi = s->map_width - 312 - pad - s->xmap;
        if (lo <= hi)
            w.dx = lo > 0 ? lo : hi < 0 ? hi : 0;
        else
            w.dx = (lo + hi) / 2;
        w.x0 += w.dx;
        w.x1 += w.dx;
        if (w.x0 < 8 - s->xmap)
            w.x0 = 8 - s->xmap;
        if (w.x1 > s->map_width + 8 - s->xmap)
            w.x1 = s->map_width + 8 - s->xmap;
    }
    return w;
}

/* The passes that draw the world (moved with the window); the others are screen-space (HUD, text). */
static int world_pass(int pass)
{
    switch (pass) {
    case PORT_PASS_BACKGROUND:
    case PORT_PASS_REAR_WEATHER:
    case PORT_PASS_MAP:
    case PORT_PASS_STARS:
    case PORT_PASS_OBJECTS:
    case PORT_PASS_PARTICLES:
    case PORT_PASS_FRONT:
    case PORT_PASS_FRONT_WEATHER:
    case PORT_PASS_FIREFLY:
        return 1;
    default:
        return 0;
    }
}

static void sprite(port_surface *d, const port_op *op, const view_window *win)
{
    int flip = op->flags & PORT_SPRITE_FLIP, raw = op->flags & PORT_SPRITE_RAW;
    uint8_t or_color = (op->flags & PORT_SPRITE_COLOR) ? (uint8_t)(op->color * 8) : 0;
    uint32_t size = op->size, w = size & 255, h = (size >> 8) & 255, stride = w, end;
    /* A box widened to the window runs its arithmetic shifted right by -window.x0, so that its unsigned comparisons
       see non-negative values as in the original view; pixels are written back shifted. */
    int widen = win && full_width(op->clip[0], op->clip[1]);
    int32_t shift = widen ? -win->x0 : 0;
    int32_t x = op->x + shift, y = op->y;
    int32_t box[4] = {widen ? 0 : op->clip[0], widen ? win->x1 - win->x0 : op->clip[1], op->clip[2], op->clip[3]};
    const uint8_t *src = op->image;
    if (op->flags & PORT_SPRITE_SCRATCH) {
        uint8_t *slot = port_native_scratch(op->w);
        if (!slot || (uint32_t)op->h + w * h > PORT_SCRATCH_BYTES)
            return;
        src = slot + op->h;
    }
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
                    put_sprite(d, op->base, x - shift + (int32_t)(flip ? w - 1 - i : i), y + (int32_t)row, v);
                i++;
            } else if (v < 160) {
                put_sprite(d, op->base, x - shift + (int32_t)(flip ? w - 1 - i : i), y + (int32_t)row,
                           (uint8_t)(v | or_color));
                i++;
            } else {
                i += v - 160u;
            }
        }
    }
}

/* draw_flocon*_Normal / draw_pluie*_Normal stamps (ASM_SPEC): the anchor is tested against the bounds set by
   flocon_clipping (upper bounds compared unsigned, inclusive); the whole stamp is then written. */
static void weather(port_surface *d, const port_op *op, const view_window *win)
{
    static const struct { const char *pattern; int w, h; } stamps[] = {
        {"7", 1, 1}, {"7867", 2, 2}, {".6.866.6.", 3, 3}, {"3", 1, 1}, {"5", 1, 1},
        {"..66...7887.688886688886.7887...66..", 6, 6}, {".66.68866886.66.", 4, 4}, {".55.57755775.55.", 4, 4},
        {".45.", 2, 2}, {".4253236.", 3, 3}, {"..32.324543.46..", 4, 4}};
    const int32_t *b = op->clip;
    int r, c;
    if (op->flags >= sizeof stamps / sizeof stamps[0])
        return;
    if (win && full_width(b[2], b[3])) {
        /* wider view: the left and right bounds move to the window (compared signed: anchors left of 0 exist) */
        if (op->x < win->x0 || op->x > win->x1 - (320 - b[3]) || op->y < b[0] || (uint32_t)op->y > (uint32_t)b[1])
            return;
    } else if (op->x < b[2] || (uint32_t)op->x > (uint32_t)b[3] || op->y < b[0] || (uint32_t)op->y > (uint32_t)b[1]) {
        return;
    }
    for (r = 0; r < stamps[op->flags].h; r++)
        for (c = 0; c < stamps[op->flags].w; c++) {
            char v = stamps[op->flags].pattern[r * stamps[op->flags].w + c];
            if (v != '.')
                put(d, op->x + c, op->y + r, (uint8_t)(v - '0'));
        }
}

/* plot2line (src/luciole.c): zero the spans of rows centre_y -/+ row_offset outside [cx - inner, cx + inner] and
   inside the outer radius, clamped to the full viewport. */
static void firefly_rows(port_surface *d, const port_op *op, const int32_t *g, int32_t inner, int32_t row_offset)
{
    /* g: W1, W2, H1, H2 of the full viewport */
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
static void firefly(port_surface *d, const port_op *op, const view_window *win)
{
    int32_t radius = (int16_t)op->size - 4, x = radius, i, y, dd, err;
    int32_t g[4] = {op->clip[0], op->clip[1], op->clip[2], op->clip[3]};
    if (win && full_width(g[0], g[1])) {
        g[0] = win->x0;
        g[1] = win->x1;
    }
    for (i = 0, y = radius; i <= op->h; ++i, ++y)
        firefly_rows(d, op, g, 0, y);
    if (x < 0)
        return;
    i = 0;
    y = 0;
    dd = 1;
    err = 2 * x - 1;
    do {
        firefly_rows(d, op, g, i, x);
        firefly_rows(d, op, g, x, i);
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

/* DrawBorderBoxNormal (ASM_SPEC; port/video/lowlevel_normal.c): light left column and top row, dark bottom and right
   edges plus the extra shadow row and column, in the original's write order. */
static void border_box(port_surface *d, const port_op *op)
{
    int64_t base = op->x;
    uint8_t light = (uint8_t)op->size, dark = (uint8_t)(op->size >> 8);
    int32_t row, col;
    for (row = 0; row < op->h; row++)
        put_linear(d, base + (int64_t)row * 320, light);
    for (col = 0; col < op->w - 1; col++)
        put_linear(d, base + (int64_t)(op->h - 1) * 320 + 1 + col, dark);
    for (col = 0; col < op->w; col++)
        put_linear(d, base + (int64_t)op->h * 320 + col, dark);
    for (col = 0; col < op->w; col++)
        put_linear(d, base + col, light);
    for (row = 1; row < op->h; row++)
        put_linear(d, base + (int64_t)row * 320 + op->w - 1, dark);
    for (row = 0; row < op->h; row++)
        put_linear(d, base + (int64_t)row * 320 + op->w, dark);
}

/* DrawFondBoxNormal: each pixel becomes its diagonal neighbour (+321, or -321 walking backwards for type 2) shifted by
   -5 (+5), and when that crosses a 32-colour bank, the bank bits with the low bits cleared (set). Reads the surface
   it is modifying, in the original order. */
static void fond_box(port_surface *d, const port_op *op)
{
    int back = op->flags == 2, step = back ? -1 : 1;
    int64_t at = op->x;
    int32_t row, col;
    if (back)
        at += (int64_t)(op->h - 1) * 320 + op->w - 1;
    for (row = 0; row < op->h; row++) {
        for (col = 0; col < op->w; col++) {
            uint8_t v = get_linear(d, at + (back ? -321 : 321)), out = (uint8_t)(v + (back ? 5 : -5));
            uint8_t cross = (uint8_t)((v ^ out) & 224);
            if (cross)
                out = (uint8_t)(cross | (back ? 31 : 0));
            put_linear(d, at, out);
            at += step;
        }
        at += (int64_t)step * (320 - op->w);
    }
}

/* A row of the surface, from x0 for n pixels, filled with v. */
static void clear_row(port_surface *d, int32_t y, uint8_t v)
{
    if (y >= d->y0 && y < d->y0 + d->height)
        memset(d->pixels + (y - d->y0) * d->pitch, v, (size_t)d->width);
}

void port_native_draw(const port_frame *f, int first, int last, port_surface *const *surfaces, int pad)
{
    port_surface *draw = surfaces[PORT_TARGET_DRAW], *display = surfaces[PORT_TARGET_DISPLAY];
    int i;
    int32_t y;
    for (i = first; i < last; i++) {
        const port_op *op = &f->ops[i];
        port_surface *dst = op->target < PORT_TARGET_COUNT ? surfaces[op->target] : NULL;
        port_surface moved;
        port_rect view = {op->clip[0], op->clip[2], op->clip[1], op->clip[3]};
        port_rect all;
        /* the wider view applies to the gameplay frame's draw buffer */
        int wide = pad && f->gameplay && op->target == PORT_TARGET_DRAW ? pad : 0;
        int fidelity = PORT_RENDER_ORIGINAL;
        view_window window, *win = NULL;
        if (!dst)
            continue;
        if (wide && world_pass(op->pass)) {
            /* the world through the window: drawn into the surface seen dx columns further right */
            window = world_window(&f->world, wide);
            win = &window;
            moved = *dst;
            moved.x0 += window.dx;
            dst = &moved;
        }
        all.x0 = dst->x0;
        all.y0 = dst->y0;
        all.x1 = dst->x0 + dst->width;
        all.y1 = dst->y0 + dst->height;
        if (win && full_width(view.x0, view.x1)) {
            view.x0 = win->x0;
            view.x1 = win->x1;
            fidelity = PORT_RENDER_EXTENDED;
        }
        switch (op->kind) {
        case PORT_OP_BACKGROUND:
            if (win) {
                /* the window beyond a narrow map's edges: black */
                int32_t full_x0 = -wide + win->dx, full_x1 = 320 + wide + win->dx;
                for (y = 0; y < 200; y++) {
                    fill(dst, full_x0, y, win->x0 - full_x0, 0);
                    fill(dst, win->x1, y, full_x1 - win->x1, 0);
                }
            }
            port_render_background(&f->world, dst, view, fidelity);
            break;
        case PORT_OP_MAP:
            port_render_map(&f->world, dst, view, fidelity);
            break;
        case PORT_OP_FRONT:
            if (win) {
                port_rect window_rect = {win->x0, 0, win->x1, 200};
                port_render_front(&f->world, dst, window_rect, PORT_RENDER_EXTENDED);
            } else {
                port_render_front(&f->world, dst, all, PORT_RENDER_ORIGINAL);
            }
            break;
        case PORT_OP_SPRITE:
            sprite(dst, op, op->base ? NULL : win);
            break;
        case PORT_OP_CLEAR_BORDERS:
            /* clear_borders_Normal: 4 bytes at the row start and at +right, on height rounded down to even. Not in
               the wider view: there the world continues past the original's left border. */
            if (wide)
                break;
            for (y = 0; y < op->h / 2 * 2; y++) {
                fill(dst, op->x, op->y + y, 4, 0);
                fill(dst, op->x + op->w, op->y + y, 4, 0);
            }
            break;
        case PORT_OP_PIXEL:
            put(dst, op->x, op->y, op->color);
            break;
        case PORT_OP_WEATHER:
            weather(dst, op, win);
            break;
        case PORT_OP_FILL:
            if (wide && op->x == 0 && op->w >= 64000) {
                /* the whole buffer cleared: the whole view */
                for (y = 0; y < 200; y++)
                    clear_row(dst, y, op->color);
                break;
            }
            for (y = 0; y < op->w; y++)
                put_linear(dst, (int64_t)op->x + y, op->color);
            break;
        case PORT_OP_COPY:
            if (op->flags) {
                const port_surface *src = op->color < PORT_TARGET_COUNT ? surfaces[op->color] : NULL;
                if (src)
                    for (y = 0; y < op->w; y++)
                        put_linear(dst, (int64_t)op->x + y, get_linear(src, (int64_t)op->y + y));
            } else {
                for (y = 0; y < op->w; y++)
                    put_linear(dst, (int64_t)op->x + y, op->image[y]);
            }
            break;
        case PORT_OP_PICTURE:
            for (y = 0; y < op->h; y++) {
                int32_t x;
                for (x = 0; x < op->w; x++)
                    put(dst, op->x + x, op->y + y, op->image[(size_t)y * op->w + x]);
            }
            break;
        case PORT_OP_SWAP:
            if (!draw || !display)
                break;
            if (pad && f->gameplay && op->x <= 4 && op->w >= 316 && op->x == op->clip[0] && op->y == op->clip[1]) {
                /* the gameplay view, its full width */
                for (y = 0; y < op->h; y++) {
                    int32_t x;
                    for (x = -pad; x < 320 + pad; x++)
                        put(display, x, op->clip[1] + y, get_pixel(draw, x, op->y + y));
                }
                break;
            }
            if (pad)   /* screen-space scenes and narrowed views keep the original 320 columns */
                for (y = 0; y < 200; y++) {
                    fill(display, -pad, y, pad, 0);
                    fill(display, 320, y, pad, 0);
                }
            for (y = 0; y < op->h; y++) {
                int32_t x;
                for (x = 0; x < op->w; x++)
                    put_linear(display, (int64_t)(op->clip[1] + y) * 320 + op->clip[0] + x,
                               get_linear(draw, (int64_t)(op->y + y) * 320 + op->x + x));
            }
            break;
        case PORT_OP_BORDER_BOX:
            border_box(dst, op);
            break;
        case PORT_OP_FOND_BOX:
            fond_box(dst, op);
            break;
        case PORT_OP_DEFORM:
            port_native_deform((const port_deform *)(const void *)op->image);
            break;
        case PORT_OP_EFFECT:
            port_native_effect(op, surfaces);
            break;
        case PORT_OP_ORIGIN:
            if (pad && op->target == PORT_TARGET_DRAW) {
                /* the wider view keeps rows apart: the leftover pixels move left by delta within their row */
                int32_t x;
                for (y = dst->y0; y < dst->y0 + dst->height; y++) {
                    if (op->x > 0)
                        for (x = dst->x0; x < dst->x0 + dst->width; x++)
                            put(dst, x, y, get_pixel(dst, x + op->x, y));
                    else
                        for (x = dst->x0 + dst->width - 1; x >= dst->x0; x--)
                            put(dst, x, y, get_pixel(dst, x + op->x, y));
                }
                break;
            }
            /* logical byte n is now the old logical byte n + delta (the 320-byte rows of the buffer) */
            if (op->x > 0) {
                int64_t n;
                for (n = -(int64_t)320 * 32; n < (int64_t)320 * 232; n++)
                    put_linear(dst, n, get_linear(dst, n + op->x));
            } else if (op->x < 0) {
                int64_t n;
                for (n = (int64_t)320 * 232 - 1; n >= -(int64_t)320 * 32; n--)
                    put_linear(dst, n, get_linear(dst, n + op->x));
            }
            break;
        case PORT_OP_CLEAR_OUTSIDE:
            /* ClearBorder (src/box.c 408): rows above y0 and from y1, then columns left of x0 and from x1. In the
               wider view the rows span it, and a full-width viewport has no columns outside it. */
            if (wide && full_width(op->clip[0], op->clip[1])) {
                for (y = 0; y < op->clip[2]; y++)
                    clear_row(dst, y, 0);
                for (y = op->clip[3]; y < 200; y++)
                    clear_row(dst, y, 0);
                break;
            }
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
            firefly(dst, op, win);
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
