#include "render/world.h"

#include <string.h>

static port_rect intersect(port_rect a, const port_surface *d)
{
    if (a.x0 < d->x0) a.x0 = d->x0;
    if (a.y0 < d->y0) a.y0 = d->y0;
    if (a.x1 > d->x0 + d->width) a.x1 = d->x0 + d->width;
    if (a.y1 > d->y0 + d->height) a.y1 = d->y0 + d->height;
    return a;
}

static uint8_t *at(const port_surface *d, int x, int y)
{
    return d->pixels + (y - d->y0) * d->pitch + (x - d->x0);
}

static void mark(const port_surface *d, int x, int y, int n)
{
    if (d->written)
        memset(d->written + (y - d->y0) * d->pitch + (x - d->x0), 1, (size_t)n);
}

static int floor_mod(int v, int m) { v %= m; return v < 0 ? v + m : v; }

/* Columns beyond the stored background image (reachable only in a wider view) are read by wrapping one half of the
   image: valid when the image is two identical halves (horizontal bands scroll through a doubled image; the shipped
   column backgrounds are their 320 columns twice). Checked once per image instead of assumed; otherwise those
   columns are not drawn. */
static int image_halves_equal(const port_scene *s)
{
    static const uint8_t *checked;
    static int checked_w, checked_h, equal;
    int y, half = s->plan_width / 2;
    if (s->plan0 == checked && s->plan_width == checked_w && s->plan_height == checked_h)
        return equal;
    checked = s->plan0;
    checked_w = s->plan_width;
    checked_h = s->plan_height;
    equal = s->plan0 && half > 0 && !(s->plan_width & 1);
    for (y = 0; equal && y < s->plan_height; y++)
        equal = !memcmp(s->plan0 + (size_t)y * s->plan_width, s->plan0 + (size_t)y * s->plan_width + half,
                        (size_t)half);
    return equal;
}

/* DrawSpriteDiffNormal, EXTENDED: 4-pixel units; a unit whose four image bytes are all zero is skipped, otherwise
   each byte becomes (dest & mask) | image, clipped per pixel to the rectangle. */
static void diff_sprite_clipped(const port_scene_bg_sprite *sp, int x, int y, port_surface *d, port_rect c)
{
    int units = sp->width / 4, r, u, k;
    for (r = 0; r < sp->height; r++) {
        int py = y + r;
        if (py < c.y0 || py >= c.y1)
            continue;
        for (u = 0; u < units; u++) {
            const uint8_t *img = sp->image + r * sp->width + 4 * u, *msk = sp->mask + r * sp->width + 4 * u;
            if (!(img[0] | img[1] | img[2] | img[3]))
                continue;
            for (k = 0; k < 4; k++) {
                int px = x + 4 * u + k;
                if (px >= c.x0 && px < c.x1) {
                    uint8_t *p = at(d, px, py);
                    *p = (uint8_t)((*p & msk[k]) | img[k]);
                    mark(d, px, py, 1);
                }
            }
        }
    }
}

/* DrawSpriteDiffNormal, ORIGINAL (ASM_SPEC clip_sprite_on_screen, diff variant; port/video/lowlevel_normal.c):
   the sprite clip box in force, cropped in whole units from the left (the crop distance is rounded down to 4, so
   up to 3 pixels left of the box are drawn), right edge rounded up to whole units, and the original's two gaps:
   a left-cropped sprite is not clipped on the right, and a top-cropped sprite is not clipped at the bottom. The
   surface bounds still apply. */
static void diff_sprite_original(const port_scene_bg_sprite *sp, int16_t x16, int16_t y16, const int32_t *box,
                                 port_surface *d)
{
    uint32_t w = sp->width, h = sp->height, end, units, rows, r, u;
    int32_t x = x16, y = y16, k;
    size_t skip_bytes = 0;
    int cropped_left = x < box[0];
    if (cropped_left) {
        uint32_t distance = (uint32_t)box[0] - (uint32_t)x;
        if (w <= distance)
            return;
        distance &= 0xfffc;
        x = (int32_t)((uint32_t)x + distance);
        skip_bytes = distance;
        w = (w - distance) & 255;
    }
    end = w + (uint32_t)x;
    if (cropped_left) {
        units = w / 4;
    } else if (end > (uint32_t)box[1]) {
        if ((uint32_t)x >= (uint32_t)box[1])
            return;
        units = ((((uint32_t)box[1] - (uint32_t)x) & 255) + 3) / 4;
    } else {
        units = w / 4;
    }
    rows = h;
    if (y < box[2]) {
        uint32_t skip;
        if ((int32_t)(h + (uint32_t)y) <= box[2])
            return;
        skip = ((uint32_t)box[2] - (uint32_t)y) & 65535;
        rows = (h - skip) & 255;
        skip_bytes += (size_t)sp->width * skip;
        y = box[2];
        if ((uint32_t)y > (uint32_t)box[3])
            return;
    } else if (h + (uint32_t)y > (uint32_t)box[3]) {
        if ((uint32_t)y >= (uint32_t)box[3])
            return;
        rows = ((uint32_t)box[3] - (uint32_t)y) & 255;
    }
    if (!units)
        return;
    for (r = 0; r < rows; r++)
        for (u = 0; u < units; u++) {
            const uint8_t *img = sp->image + skip_bytes + r * sp->width + 4 * u;
            const uint8_t *msk = sp->mask + skip_bytes + r * sp->width + 4 * u;
            if (!(img[0] | img[1] | img[2] | img[3]))
                continue;
            for (k = 0; k < 4; k++) {
                int32_t px = x + 4 * (int32_t)u + k, py = y + (int32_t)r;
                if (px >= d->x0 && px < d->x0 + d->width && py >= d->y0 && py < d->y0 + d->height) {
                    uint8_t *p = at(d, px, py);
                    *p = (uint8_t)((*p & msk[k]) | img[k]);
                    mark(d, px, py, 1);
                }
            }
        }
}

static void diff_sprite(const port_scene *s, const port_scene_bg_sprite *sp, int x, int y, port_surface *d,
                        port_rect c, int fidelity)
{
    if (fidelity == PORT_RENDER_ORIGINAL)
        diff_sprite_original(sp, (int16_t)x, (int16_t)y, s->sprite_clip, d);
    else
        diff_sprite_clipped(sp, x, y, d, c);
}

/* Instances of a sprite repeating every `period` pixels along x (BANDS) or y (COLUMNS). The original computes
   the position along the scroll axis as s16, subtracts one period when it exceeds the screen size (320 or 200),
   and draws there when the sprite reaches past the viewport start and one period further when that is still
   before the viewport end. ALL draws every instance meeting the rectangle instead. */
static void repeated_sprite(const port_scene *s, const port_scene_bg_sprite *sp, int along, int across, int vertical,
                            int period, port_surface *d, port_rect c, int fidelity)
{
    int size = vertical ? sp->height : sp->width;
    int view0 = vertical ? s->view_y0 : s->view_x0, view1 = vertical ? s->view_y1 : s->view_x1;
    int lo = vertical ? c.y0 : c.x0, hi = vertical ? c.y1 : c.x1, pos;
    if ((int16_t)along > (vertical ? 200 : 320))
        along = (int16_t)(along - period);
    if (fidelity == PORT_RENDER_ORIGINAL) {
        if (along + size > view0)
            diff_sprite(s, sp, vertical ? across : along, vertical ? along : across, d, c, fidelity);
        if (along + period < view1)
            diff_sprite(s, sp, vertical ? across : along + period, vertical ? along + period : across, d, c,
                        fidelity);
        return;
    }
    for (pos = along - period * ((along + size - lo) / period + 1); pos < hi; pos += period)
        if (pos + size > lo)
            diff_sprite(s, sp, vertical ? across : pos, vertical ? pos : across, d, c, fidelity);
}

static const port_scene_band *band_at(const port_scene *s, int v)
{
    int i;
    for (i = 0; i < s->nb_bands; i++)
        if (s->bands[i].length && v >= s->bands[i].offset && v < s->bands[i].offset + s->bands[i].length)
            return &s->bands[i];
    return NULL;
}

void port_render_background(const port_scene *s, port_surface *dst, port_rect clip, int fidelity)
{
    port_rect c = intersect(clip, dst);
    int x, y, i;
    if (c.x0 >= c.x1 || c.y0 >= c.y1)
        return;
    if (s->background == PORT_SCENE_BG_CLEAR) {
        for (y = c.y0; y < c.y1; y++) {
            memset(at(dst, c.x0, y), 0, (size_t)(c.x1 - c.x0));
            mark(dst, c.x0, y, c.x1 - c.x0);
        }
        return;
    }
    if (s->background == PORT_SCENE_BG_BANDS) {
        /* Display_Back_Screen copies band rows: screen row y shows image row y + bg_shift, column scroll + x. The
           image holds two periods; columns outside it (only reachable outside the original image) wrap by one. */
        int period = s->plan_width / 2;
        if (period <= 0)
            return;
        int xa = c.x0, xb = c.x1;
        if (fidelity == PORT_RENDER_ORIGINAL) {   /* a narrow viewport's band copy can stop short of its right edge */
            xa = xa > s->bg_x0 ? xa : s->bg_x0;
            xb = xb < s->bg_x1 ? xb : s->bg_x1;
        }
        for (y = c.y0 > s->bg_top ? c.y0 : s->bg_top; y < c.y1 && xa < xb; y++) {
            int row = y + s->bg_shift;
            const port_scene_band *b = band_at(s, row);
            const uint8_t *src;
            uint8_t *out;
            if (!b || row < 0 || row >= s->plan_height)
                continue;
            src = s->plan0 + (size_t)row * s->plan_width;
            out = at(dst, xa, y);
            for (x = xa; x < xb; x++, out++) {
                int col = b->scroll + x;
                if (col < 0 || col >= s->plan_width) {
                    if (!image_halves_equal(s))
                        continue;
                    col = floor_mod(col, period);
                }
                *out = src[col];
            }
            mark(dst, xa, y, xb - xa);
        }
        for (i = 0; i < s->nb_sprites; i++) {
            const port_scene_bg_sprite *sp = &s->sprites[i];
            if (s->bands[sp->band].length)
                repeated_sprite(s, sp, sp->x - s->bands[sp->band].scroll, sp->y - s->bg_shift, 0, period, dst, c,
                                fidelity);
        }
        return;
    }
    if (s->background == PORT_SCENE_BG_COLUMNS) {
        /* Vertical bands: column x of band b shows image column x, row (scroll + y) wrapped by the image height
           (the original copies the rows below the wrap point a second time from row 0). EXTENDED reads columns
           outside the original image through half the image width, if the image is two equal halves. */
        int half = s->plan_width / 2;
        for (y = c.y0 > s->bg_top ? c.y0 : s->bg_top; y < c.y1; y++)
            for (x = c.x0; x < c.x1; x++) {
                int col = x;
                const port_scene_band *b;
                int row;
                if (fidelity == PORT_RENDER_EXTENDED && (x < 0 || x >= 320)) {
                    if (half <= 0 || !image_halves_equal(s))
                        continue;
                    col = floor_mod(x, half);
                }
                b = band_at(s, col);
                if (!b || col < 0 || col >= s->plan_width)
                    continue;
                row = floor_mod(b->scroll + y, s->plan_height);
                *at(dst, x, y) = s->plan0[(size_t)row * s->plan_width + col];
                mark(dst, x, y, 1);
            }
        for (i = 0; i < s->nb_sprites; i++) {
            const port_scene_bg_sprite *sp = &s->sprites[i];
            if (s->bands[sp->band].length)
                repeated_sprite(s, sp, sp->y - s->bands[sp->band].scroll, sp->x, 1, s->plan_height, dst, c,
                                fidelity);
        }
    }
}

void port_render_front(const port_scene *s, port_surface *dst, port_rect clip, int fidelity)
{
    port_rect c = intersect(clip, dst);
    int i;
    if (c.x0 >= c.x1 || c.y0 >= c.y1)
        return;
    for (i = 0; i < s->nb_sprites; i++) {
        const port_scene_bg_sprite *sp = &s->sprites[i];
        if (s->bands[sp->band].length)
            continue;
        if (s->front == PORT_SCENE_BG_BANDS && s->plan_width / 2 > 0)
            repeated_sprite(s, sp, sp->x - s->bands[sp->band].scroll, s->front_y - sp->height, 0, s->plan_width / 2,
                            dst, c, fidelity);
        else if (s->front == PORT_SCENE_BG_COLUMNS && s->plan_height > 0)
            repeated_sprite(s, sp, sp->y - s->bands[sp->band].scroll, sp->x, 1, s->plan_height, dst, c, fidelity);
    }
}

/* DRAW_MAP / Display_Bloc[_Plein][_Clippe]: transparency 1 copies the 16x16 texture; 2 reads one 2-bit kind per
   row from the dword at +512 (MSB first): bit 0 skips the row, bit 1 copies it, 0 combines (dest & tex[256+i]) |
   tex[i]. The original's 4-pixel clip rounding only writes outside the clip rectangle. DRAW_MAP draws nothing
   when its grid for the viewport is a single tile wide or high (src/map_ngawe.c 81). */
void port_render_map(const port_scene *s, port_surface *dst, port_rect clip, int fidelity)
{
    port_rect c = intersect(clip, dst);
    int r, col, y, x;
    if (c.x0 >= c.x1 || c.y0 >= c.y1)
        return;
    if (fidelity == PORT_RENDER_ORIGINAL && (s->original_map_grid_w <= 1 || s->original_map_grid_h <= 1))
        return;
    for (r = 0; r < PORT_SCENE_TILE_ROWS; r++)
        for (col = 0; col < PORT_SCENE_TILE_COLS; col++) {
            const port_scene_tile *t = &s->tiles[r][col];
            int tx = 16 * (s->tile_col0 + col) - s->xmap + 8, ty = 16 * (s->tile_row0 + r) - s->ymap;
            uint32_t flags;
            int xa, xb;
            if (!t->transparency || tx >= c.x1 || tx + 16 <= c.x0 || ty >= c.y1 || ty + 16 <= c.y0)
                continue;
            flags = t->transparency == 1 ? 0xaaaaaaaau
                                         : (uint32_t)t->texture[512] | (uint32_t)t->texture[513] << 8 |
                                               (uint32_t)t->texture[514] << 16 | (uint32_t)t->texture[515] << 24;
            xa = tx < c.x0 ? c.x0 : tx;
            xb = tx + 16 > c.x1 ? c.x1 : tx + 16;
            for (y = 0; y < 16; y++) {
                unsigned kind = (flags >> (30 - 2 * y)) & 3;
                const uint8_t *row = t->texture + 16 * y;
                uint8_t *out;
                if (ty + y < c.y0 || ty + y >= c.y1 || (kind & 1))
                    continue;
                out = at(dst, xa, ty + y);
                for (x = xa; x < xb; x++, out++)
                    *out = (kind & 2) ? row[x - tx] : (uint8_t)((*out & row[256 + x - tx]) | row[x - tx]);
                mark(dst, xa, ty + y, xb - xa);
            }
        }
}
