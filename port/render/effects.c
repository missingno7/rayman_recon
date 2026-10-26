#include "render/effects.h"

/* Video memory is 320-byte rows: linear offset n of a buffer is pixel (n mod 320, n div 320) of its surface. */
static port_surface *surface_of(const port_effect_buffer *b, port_surface *const *surfaces)
{
    return b->target >= 0 && b->target < PORT_TARGET_COUNT ? surfaces[b->target] : NULL;
}

static void put(port_surface *d, int64_t n, uint8_t v)
{
    int64_t y = n >= 0 ? n / 320 : -1 - (-n - 1) / 320, x = n - 320 * y;
    if (d && x >= d->x0 && x < d->x0 + d->width && y >= d->y0 && y < d->y0 + d->height)
        d->pixels[(y - d->y0) * d->pitch + (x - d->x0)] = v;
}

static uint8_t get(const port_surface *d, int64_t n)
{
    int64_t y = n >= 0 ? n / 320 : -1 - (-n - 1) / 320, x = n - 320 * y;
    if (!d || x < d->x0 || x >= d->x0 + d->width || y < d->y0 || y >= d->y0 + d->height)
        return 0;
    return d->pixels[(y - d->y0) * d->pitch + (x - d->x0)];
}

static int32_t s32_of(uint32_t n);

static int32_t magnitude(int32_t v) { return v < 0 ? -v : v; }

/* The arc table outside its 2 * width entries is never read by the original's loops for valid states. */
static int32_t arc(const port_effect_curl *e, int32_t i)
{
    return i >= 0 && i < 2 * e->width && i < 80 ? e->table[i] : 0;
}

/* effect_helper_63838 (src/effect633e0.c), the pixel part: mode 3 clears the destination; modes 0-2 remap the
   columns of each row through the arc table, filling the uncovered strip with 0 or the source. */
static void curl(const port_effect_curl *e, port_surface *const *surfaces)
{
    port_surface *dst = surface_of(&e->destination, surfaces);
    const port_surface *src = surface_of(&e->source, surfaces);
    int64_t s = e->source.offset, d = e->destination.offset;
    int32_t x, y, sx, step = magnitude(e->delta);
    if (e->mode == 3) {
        for (x = 0; x < 64000; x++)
            put(dst, d + x, 0);
        return;
    }
    if (e->mode == 0 && e->delta > 0 && e->initialized == 1)
        for (x = 0; x < 64000; x++)
            put(dst, d + x, get(src, s + x));
    for (y = 0; y < 200; y++, s += 320, d += 320) {
        switch (e->mode) {
        case 0:
            for (x = 0; x < e->field_0c; x++)
                put(dst, d + e->field_14 + x, get(src, s + e->field_10 + arc(e, e->width - x)));
            if (e->delta > 0) {
                for (x = e->field_0c; x < step + e->field_0c; x++)
                    if (e->field_14 + x < 320)
                        put(dst, d + e->field_14 + x, 0);
            } else {
                for (x = -1; x < step - 1; x++)
                    put(dst, d + e->field_14 - x, get(src, s + e->field_14 - x));
            }
            break;
        case 1:
            for (x = e->field_0c; x < e->width; x++) {
                sx = e->field_10 + arc(e, e->width + x);
                if (sx > 319)
                    sx = 319;
                if (get(src, s + sx) == 0)
                    sx = x <= 0 ? e->field_14 + x : e->field_10 - arc(e, e->width + x) - 1;
                put(dst, d + e->field_14 + x, get(src, s + sx));
            }
            if (e->delta > 0) {
                for (x = e->width; x < step + e->width; x++)
                    put(dst, d + e->field_14 + x, 0);
            } else if (e->width < e->field_14) {
                for (x = -e->width - step; x < e->field_0c; x++)
                    put(dst, d + e->field_14 + x, get(src, s + e->field_14 + x));
            }
            break;
        case 2:
            for (x = e->field_0c; x < e->width; x++) {
                sx = e->field_10 + arc(e, e->width + x);
                if (get(src, s + sx) == 0)
                    sx = x < 0 ? e->field_14 + x : e->field_10 - arc(e, e->width + x);
                put(dst, d + e->field_14 + x, get(src, s + sx));
            }
            if (e->delta > 0)
                for (x = e->width; x < step + e->width; x++)
                    put(dst, d + e->field_14 + x, 0);
            break;
        default:
            return;
        }
    }
}

static int32_t s32_of(uint32_t n) { return n <= INT32_MAX ? (int32_t)n : -1 - (int32_t)(UINT32_MAX - n); }
static uint32_t low_byte(uint32_t v, uint32_t x) { return (v & 0xffffff00u) | (x & 255); }
static uint32_t high_byte(uint32_t v, uint32_t x) { return (v & 0xffff00ffu) | ((x & 255) << 8); }
static int64_t floor_q16(int64_t v) { return v >= 0 ? v / 65536 : -1 - ((-1 - v) / 65536); }

/* draw_effect_segment (ASM_SPEC, port/video/lowlevel_normal.c): edges into per-row extremes with their (u, v), then
   each row's span with the original's coupled modular UV accumulators. The per-row tables start empty for every
   call, as the original's do (it resets each row it draws, and the edges only touch drawn rows). */
static void segment(const port_effect_segment *e, port_surface *const *surfaces)
{
    static const unsigned ends[4][3] = {{0, 3, 0}, {3, 2, 1}, {2, 1, 0}, {1, 0, 1}};
    port_surface *dst = surface_of(&e->destination, surfaces);
    const port_surface *tex = surface_of(&e->texture, surfaces);
    const int32_t *verts = e->vertices;
    const uint8_t *uv = e->uv;
    uint16_t left[200], right[200];
    uint8_t lu[200], lv[200], ru[200], rv[200];
    uint32_t du = (uint8_t)(uv[6] - uv[0]), dv = (uint8_t)(uv[3] - uv[1]);
    int32_t miny = verts[1], maxy = verts[1], first, last, y;
    unsigned i, edge;
    for (i = 0; i < 200; i++) {
        left[i] = 65535;
        right[i] = 0;
        lu[i] = lv[i] = ru[i] = rv[i] = 0;
    }
    for (i = 1; i < 4; i++) {
        if (verts[2 * i + 1] < miny)
            miny = verts[2 * i + 1];
        if (verts[2 * i + 1] > maxy)
            maxy = verts[2 * i + 1];
    }
    if (miny >= 200 || maxy < 0)
        return;
    first = miny < 0 ? 0 : miny;
    last = maxy > 199 ? 199 : maxy;
    for (edge = 0; edge < 4; edge++) {
        unsigned a = ends[edge][0], b = ends[edge][1], axis = ends[edge][2];
        int32_t xa = verts[2 * b], ya = verts[2 * b + 1], xb = verts[2 * a], yb = verts[2 * a + 1];
        int32_t ta = uv[2 * b + axis], constant = uv[2 * b + 1 - axis];
        int32_t delta = edge == 0 ? -(int32_t)du : edge == 1 ? -(int32_t)dv : edge == 2 ? (int32_t)du : (int32_t)dv;
        int64_t count, dx, qx, qt;
        if ((ya < 0 && yb < 0) || (ya >= 200 && yb >= 200))
            continue;
        if (yb < ya) {
            int32_t swap = xa;
            xa = xb;
            xb = swap;
            swap = ya;
            ya = yb;
            yb = swap;
            ta = uv[2 * a + axis];
            delta = -delta;
        }
        count = (int64_t)yb - ya + 1;
        dx = (int64_t)xb - xa;
        qx = (dx + (dx >= 0 ? 1 : -1)) * 65536 / count;
        qt = (int64_t)delta * 65536 / count;
        for (y = ya < 0 ? 0 : ya; y <= (yb > 199 ? 199 : yb); y++) {
            int64_t n = (int64_t)y - ya;
            uint16_t biased = (uint16_t)floor_q16(((int64_t)xa + 32000) * 65536 + qx * n);
            uint8_t tt = (uint8_t)floor_q16((int64_t)ta * 65536 + qt * n);
            uint8_t uu = axis == 0 ? tt : (uint8_t)constant, vv = axis == 0 ? (uint8_t)constant : tt;
            if (left[y] > biased) {
                left[y] = biased;
                lu[y] = uu;
                lv[y] = vv;
            }
            if (right[y] < biased) {
                right[y] = biased;
                ru[y] = uu;
                rv[y] = vv;
            }
        }
    }
    for (y = first; y <= last; y++) {
        uint32_t l = left[y], r = right[y];
        if (l < 32320 && r >= 32000) {
            uint32_t width = r - l + 1, a, d, b, fraction = 0, col;
            int64_t divisor = s32_of(width << 16);
            uint32_t qv = (uint32_t)(((int64_t)s32_of((uint32_t)rv[y] - lv[y] + 1) * INT64_C(4294967296)) / divisor);
            uint32_t qu = (uint32_t)(((int64_t)s32_of((uint32_t)ru[y] - lu[y] + 1) * INT64_C(4294967296)) / divisor);
            int32_t x = (int32_t)l - 32000;
            a = ((qv & 65535) << 16) | (qu >> 16);
            d = ((qu & 65535) << 16) | ((qv >> 16) & 255);
            b = (((uint32_t)lv[y] << 8) + lu[y]) & 65535;
            if (x < 0) {
                uint32_t skip = (uint32_t)-x, carry;
                uint64_t product = (uint64_t)a * skip, total = (uint64_t)b + (uint32_t)product;
                width -= skip;
                carry = (uint32_t)(total >> 32);
                b = (uint32_t)total;
                b = high_byte(b, (b >> 8) + (uint8_t)(product >> 32) + carry);
                product = (uint64_t)d * skip;
                b = low_byte(b, (b & 255) + (uint8_t)(product >> 32));
                b = high_byte(b, (b >> 8) + (uint8_t)product);
                fraction = (uint32_t)product;
                x = 0;
            }
            if (width > (uint32_t)(320 - x))
                width = (uint32_t)(320 - x);
            for (col = 0; col < width; col++) {
                uint64_t total;
                put(dst, e->destination.offset + (int64_t)y * 320 + x + col, get(tex, e->texture.offset + (b & 65535)));
                total = (uint64_t)fraction + d;
                fraction = (uint32_t)total;
                total = (uint64_t)b + a + (total >> 32);
                b = (uint32_t)total;
                b = high_byte(b, (b >> 8) + (d & 255) + (uint32_t)(total >> 32));
            }
        }
    }
}

/* The four phase bytes as the original's little-endian dword, stepped by `+= 0x01010101; &= 0x7f7f7f7f` (a byte
   0xFF carries into the next one before the mask). */
static uint32_t pack(const int32_t *phase)
{
    return (uint32_t)(uint8_t)phase[0] | (uint32_t)(uint8_t)phase[1] << 8 | (uint32_t)(uint8_t)phase[2] << 16 |
           (uint32_t)(uint8_t)phase[3] << 24;
}

static uint32_t step(uint32_t phases) { return (phases + 0x01010101u) & 0x7f7f7f7fu; }

/* Plasma (src/box.c): per row and column the phases step; the pixel is the byte sum of four table samples and the
   bias, divided by 16, plus the palette base. */
static void plasma(const port_effect_plasma *e, port_surface *const *surfaces)
{
    port_surface *dst = surface_of(&e->destination, surfaces);
    int64_t row = e->destination.offset + e->x + (int64_t)320 * e->y;
    uint16_t j, i, width = (uint16_t)e->width, height = (uint16_t)e->height;
    uint32_t yy = pack(e->phase_y), xx;
    for (j = 0; j < height; ++j, row += 320) {
        yy = step(yy);
        xx = pack(e->phase_x);
        for (i = 0; i < width; ++i) {
            uint8_t value;
            xx = step(xx);
            value = (uint8_t)(e->table[128 * (yy & 255) + (xx & 255)] + e->bias);
            value = (uint8_t)(value + e->table[128 * ((yy >> 8) & 255) + ((xx >> 8) & 255)]);
            value = (uint8_t)(value + e->table[128 * ((yy >> 16) & 255) + ((xx >> 16) & 255)]);
            value = (uint8_t)(value + e->table[128 * (yy >> 24) + (xx >> 24)]);
            put(dst, row + i, (uint8_t)((value >> 4) + e->color_index));
        }
    }
}

static uint8_t scratch[PORT_SCRATCH_SLOTS][PORT_SCRATCH_BYTES];

uint8_t *port_native_scratch(int slot) { return slot >= 0 && slot < PORT_SCRATCH_SLOTS ? scratch[slot] : NULL; }

/* draw_sprite_deform (ASM_SPEC 1:04828, port/video/lowlevel_normal.c) into a scratch slot: fill the footprint with
   161 (a one-pixel skip marker for the sprite drawer), then walk the destination with the integer stepping of the
   geometry and write source samples where the byte is still 161 (descending) or 129 (ascending). */
void port_native_deform(const port_deform *e)
{
    uint8_t *dest = port_native_scratch(e->slot);
    const int32_t *p = e->geometry;
    const uint8_t *source = e->source;
    int32_t A = p[0], B = p[1], C = p[2], D = p[3], E = p[4], F = p[5], G = p[6], H = p[7];
    uint32_t sw = (uint32_t)p[11], sh = (uint32_t)p[12], fill_bytes, rowstep, limit, goal, step, k;
    uint32_t outer_error = 0, outer_count = 0, sample_y = 0, innerstep;
    int descending = C > D;
    int64_t row_dest = p[10];
    size_t row_source = 0;
    if (!dest || !source)
        return;
    fill_bytes = (uint32_t)(4 * ((p[8] >> 2) + 1)) * (uint32_t)p[9];
    for (k = 0; k < fill_bytes && k < PORT_SCRATCH_BYTES; k++)
        dest[k] = 161;
    rowstep = (sh << 16) / (uint32_t)(G + H);
    limit = (uint32_t)(descending ? C : D);
    goal = (uint32_t)(descending ? H : G);
    step = (sw << 16) / limit;
    innerstep = step;
    for (;;) {
        uint32_t error = 0, sample_x = 0;
        int64_t d = row_dest;
        for (k = 0; k < limit; k++) {
            uint8_t value = source[row_source + (sample_x >> 16)];
            if (d >= 0 && d < PORT_SCRATCH_BYTES && dest[d] == (descending ? 161 : 129))
                dest[d] = value;
            sample_x += innerstep;
            error += (uint32_t)(descending ? D : C);
            if (k + 1 < limit) {
                d += descending ? A : B;
                if (descending ? error > (uint32_t)C : s32_of(error) > 0) {
                    d += descending ? B : A;
                    error -= (uint32_t)(descending ? C : D);
                }
            }
        }
        outer_count++;
        outer_error += (uint32_t)(descending ? G : H);
        row_dest += descending ? F : E;
        sample_y += rowstep;
        row_source = (size_t)sw * (sample_y >> 16);
        innerstep = step;
        if (outer_count >= goal)
            break;
        if (descending ? s32_of(outer_error) > 0 : s32_of(outer_error) > G) {
            row_dest += descending ? E - F : F - E;
            outer_count--;
            outer_error -= (uint32_t)(G + H);
            innerstep = step - 64;
        }
    }
}

/* One span of plot2linejumelle: n bytes to draw offset d from effect offset s, through the zoom table from index z
   when zoom is active (an index outside the table reads offset 0). */
static void binocular_span(const port_effect_binoculars *e, port_surface *const *surfaces, int64_t d, int64_t s,
                           int64_t z, int32_t n)
{
    port_surface *dst = surface_of(&e->draw, surfaces);
    const port_surface *src = surface_of(&e->effect, surfaces);
    int32_t i;
    for (i = 0; i < n; i++) {
        int64_t from = s + i;
        if (e->zoom)
            from += z + i >= 0 && z + i < 29025 ? e->zoom_table[z + i] : 0;
        put(dst, e->draw.offset + d + i, get(src, e->effect.offset + from));
    }
}

/* plot2linejumelle (src/jumelle.c): the rows jumelle_y -/+ plot_y of both circles, as one middle span when the
   circles overlap there, else one span per circle, each clamped to the screen width. */
static void binocular_rows(const port_effect_binoculars *e, port_surface *const *surfaces, int32_t plot_x,
                           int32_t plot_y)
{
    int32_t jx = e->x, jy = e->y, off2 = e->circle2_offset;
    int32_t v53 = jy - 64, v7 = jx - plot_x - off2, v55 = 320 * e->y_min + e->x_min, v56 = jx - 112, row, k;
    if (plot_x > off2) {
        int32_t n = 2 * (plot_x + off2);
        if (v7 < 0) {
            n += v7;
            v7 = 0;
        }
        if (v7 + n >= 320)
            n = 320 - v7 - 1;
        if (n > 0)
            for (k = 0; k < 2; k++) {
                row = k ? jy + plot_y : jy - plot_y;
                if (row >= 0 && row < 200)
                    binocular_span(e, surfaces, 320 * (int64_t)row + v7, 320 * (int64_t)row + v7 - v55,
                                   (448 * (int64_t)(row - v53)) / 2 + (v7 - v56), n);
            }
    } else {
        int32_t n1 = 2 * plot_x, n2 = 2 * plot_x, x2 = off2 + jx - plot_x;
        if (v7 < 0) {
            n1 += v7;
            v7 = 0;
        }
        if (v7 + n1 >= 320)
            n1 = 320 - v7 - 1;
        if (x2 < 0) {
            n2 += x2;
            x2 = 0;
        }
        if (n2 + x2 >= 320)
            n2 = 320 - x2 - 1;
        for (k = 0; k < 2; k++) {
            row = k ? plot_y + jy : jy - plot_y;
            if (row < 0 || row >= 200)
                continue;
            if (n1 > 0)
                binocular_span(e, surfaces, 320 * (int64_t)row + v7, 320 * (int64_t)row + v7 - v55,
                               (448 * (int64_t)(row - v53)) / 2 + (v7 - v56), n1);
            if (n2 > 0)
                binocular_span(e, surfaces, 320 * (int64_t)row + x2, 320 * (int64_t)row + x2 - v55,
                               (448 * (int64_t)(row - v53)) / 2 + (x2 - v56), n2);
        }
    }
}

/* DisplayJumellesNormal (src/jumelle.c): midpoint circle of the radius, two row pairs per step. */
static void binoculars(const port_effect_binoculars *e, port_surface *const *surfaces)
{
    int32_t plot_x = 0, decision = 0, plot_y = e->radius;
    if (plot_x <= plot_y) {
        int32_t east_step = 2 * plot_x + 1, south_step = 2 * plot_y - 1;
        do {
            binocular_rows(e, surfaces, plot_x, plot_y);
            binocular_rows(e, surfaces, plot_y, plot_x);
            decision += east_step;
            east_step += 2;
            ++plot_x;
            if (decision >= plot_y) {
                decision -= south_step;
                south_step -= 2;
                --plot_y;
            }
        } while (plot_x <= plot_y);
    }
}

/* effect_helper_6c9bc (src/effect6c3b0.c): the screen copied to the output (64000 bytes), then the lens square
   resampled through its displacement map. */
static void lens(const port_effect_lens *e, port_surface *const *surfaces)
{
    port_surface *dst = surface_of(&e->output, surfaces);
    const port_surface *src = surface_of(&e->screen, surfaces);
    int64_t start, s, d;
    int32_t x, y, m = 0;
    for (x = 0; x < 64000; x++)
        put(dst, e->output.offset + x, get(src, e->screen.offset + x));
    start = (int64_t)(e->x - e->radius) + (int64_t)(e->y - e->radius) * 320;
    for (y = 0; y < e->height; ++y) {
        s = e->screen.offset + start + (int64_t)y * 320;
        d = e->output.offset + start + (int64_t)y * 320;
        for (x = 0; x < e->width; ++x, m += 2)
            put(dst, d + x, get(src, s + x + e->map[m] + e->map[m + 1] * 320));
    }
}

static void grey(const port_effect_grey *e, port_surface *const *surfaces)
{
    port_surface *dst = surface_of(&e->destination, surfaces);
    const port_surface *src = surface_of(&e->source, surfaces);
    int32_t x, y;
    for (y = 0; y < e->height; y++)
        for (x = 0; x < e->width; x++) {
            uint8_t c = get(src, e->source.offset + (int64_t)y * 320 + x);
            put(dst, e->destination.offset + (int64_t)y * 320 + x,
                (uint8_t)((e->palette[3 * c] + e->palette[3 * c + 1] + e->palette[3 * c + 2]) / 3 + 128));
        }
}

/* CadreTrans (src/box.c): each little-endian dword of the rectangle gains 0x40404040 (bytes carry into the next
   one), a zero dword becomes 0xc8c8c8c8. */
static void frame(const port_effect_frame *e, port_surface *const *surfaces)
{
    port_surface *dst = surface_of(&e->destination, surfaces);
    int32_t row, word, k;
    for (row = 0; row < e->height; row++)
        for (word = 0; word < e->width / 4; word++) {
            int64_t at = e->destination.offset + e->x + (int64_t)320 * (e->y + row) + 4 * word;
            uint32_t v = 0;
            for (k = 0; k < 4; k++)
                v |= (uint32_t)get(dst, at + k) << (8 * k);
            v = v ? v + 0x40404040u : 0xc8c8c8c8u;
            for (k = 0; k < 4; k++)
                put(dst, at + k, (uint8_t)(v >> (8 * k)));
        }
}

/* apparition_prg (src/menu.c): rows yoff = -h/2 .. h/2 - 1 land at y = 100 + yoff * step and columns at
   x = 160 + xoff * step, kept strictly inside the viewport; the source pointer walks the picture rectangle. */
static void zoom_in(const port_effect_fade *e, port_surface *const *surfaces)
{
    port_surface *dst = surface_of(&e->draw, surfaces);
    const port_surface *src = surface_of(&e->picture, surfaces);
    int64_t s = e->picture.offset + e->x + (int64_t)e->y * 320;
    int32_t yoff, xoff, row_skip = 320 - e->width;
    for (yoff = -e->height / 2; yoff < e->height / 2; ++yoff) {
        int16_t y = (int16_t)(100 + yoff * e->step);
        if (y > e->view_y0 && y < e->view_y1) {
            for (xoff = -e->width / 2; xoff < e->width / 2; ++xoff, ++s) {
                int16_t x = (int16_t)(160 + e->step * xoff);
                if (x > e->view_x0 && x < e->view_x1)
                    put(dst, e->draw.offset + (int64_t)y * 320 + x, get(src, s));
            }
            s += row_skip;
        } else {
            s += 320;
        }
    }
}

/* fondu_prg (src/menu.c): differing pixels move towards the picture, (d * (16 - step) + p) / (17 - step). */
static void blend(const port_effect_fade *e, port_surface *const *surfaces)
{
    port_surface *dst = surface_of(&e->draw, surfaces);
    const port_surface *src = surface_of(&e->picture, surfaces);
    int32_t i, j;
    for (j = 0; j < e->height; j++)
        for (i = 0; i < e->width; i++) {
            int64_t at = (int64_t)(e->y + j) * 320 + e->x + i;
            uint8_t d = get(dst, e->draw.offset + at), p = get(src, e->picture.offset + at);
            if (d != p)
                put(dst, e->draw.offset + at, (uint8_t)((d * (16 - e->step) + p) / (17 - e->step)));
        }
}

/* FonduPixel_prg (src/menu.c): a phase counter running across the whole rectangle copies the picture's pixel each
   time it reaches the step. */
static void dissolve(const port_effect_fade *e, port_surface *const *surfaces)
{
    port_surface *dst = surface_of(&e->draw, surfaces);
    const port_surface *src = surface_of(&e->picture, surfaces);
    uint16_t row, column, phase = 0, width = (uint16_t)e->width, height = (uint16_t)e->height;
    for (row = 0; row < height; ++row)
        for (column = 0; column < width; ++column) {
            if (phase == (uint16_t)e->step) {
                int64_t at = (int64_t)(e->y + row) * 320 + e->x + column;
                uint8_t p = get(src, e->picture.offset + at);
                if (get(dst, e->draw.offset + at) != p)
                    put(dst, e->draw.offset + at, p);
                phase = 0;
            }
            ++phase;
        }
}

static void copy_rect(const port_effect_rect *e, port_surface *const *surfaces)
{
    port_surface *dst = surface_of(&e->destination, surfaces);
    const port_surface *src = surface_of(&e->source, surfaces);
    int32_t x, y;
    for (y = 0; y < e->height; y++)
        for (x = 0; x < e->width; x++)
            put(dst, e->destination.offset + (int64_t)y * 320 + x, get(src, e->source.offset + (int64_t)y * 320 + x));
}

static void fire(const port_effect_fire *e, port_surface *const *surfaces)
{
    port_surface *dst = surface_of(&e->destination, surfaces);
    int32_t x, y;
    for (y = 0; y < e->height; y++)
        for (x = 0; x < e->width; x++)
            put(dst, e->destination.offset + (int64_t)y * 320 + x, e->field[y * e->width + x]);
}

const char *port_effect_name(int effect)
{
    static const char *names[PORT_EFFECT_COUNT] = {
        "?", "curl", "segment", "plasma", "binoculars", "lens", "grey", "frame", "zoom_in", "blend", "dissolve",
        "copy_rect", "fire"};
    return effect > 0 && effect < PORT_EFFECT_COUNT ? names[effect] : "?";
}

void port_native_effect(const port_op *op, port_surface *const *surfaces)
{
    switch (op->flags) {
    case PORT_EFFECT_CURL:
        curl((const port_effect_curl *)(const void *)op->image, surfaces);
        break;
    case PORT_EFFECT_SEGMENT:
        segment((const port_effect_segment *)(const void *)op->image, surfaces);
        break;
    case PORT_EFFECT_PLASMA:
        plasma((const port_effect_plasma *)(const void *)op->image, surfaces);
        break;
    case PORT_EFFECT_BINOCULARS:
        binoculars((const port_effect_binoculars *)(const void *)op->image, surfaces);
        break;
    case PORT_EFFECT_LENS:
        lens((const port_effect_lens *)(const void *)op->image, surfaces);
        break;
    case PORT_EFFECT_GREY:
        grey((const port_effect_grey *)(const void *)op->image, surfaces);
        break;
    case PORT_EFFECT_FRAME:
        frame((const port_effect_frame *)(const void *)op->image, surfaces);
        break;
    case PORT_EFFECT_ZOOM_IN:
        zoom_in((const port_effect_fade *)(const void *)op->image, surfaces);
        break;
    case PORT_EFFECT_BLEND:
        blend((const port_effect_fade *)(const void *)op->image, surfaces);
        break;
    case PORT_EFFECT_DISSOLVE:
        dissolve((const port_effect_fade *)(const void *)op->image, surfaces);
        break;
    case PORT_EFFECT_COPY_RECT:
        copy_rect((const port_effect_rect *)(const void *)op->image, surfaces);
        break;
    case PORT_EFFECT_FIRE:
        fire((const port_effect_fire *)(const void *)op->image, surfaces);
        break;
    default:
        break;
    }
}
