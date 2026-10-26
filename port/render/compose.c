#include "render/compose.h"
#include "render/record.h"
#include "render/world.h"

#include <string.h>

static int pad;
static port_scene shown;
static int shown_valid;
static uint8_t swapped[64000];
static uint8_t wide[(320 + 2 * PORT_SCENE_MAX_PAD) * 200];
static long drawn_count, black_count;
static const uint8_t *last_frame;
static int last_width;

void port_render_set_pad(int value)
{
    pad = value < 0 ? 0 : value > PORT_SCENE_MAX_PAD ? PORT_SCENE_MAX_PAD : value;
}

int port_render_pad(void) { return pad; }

void port_render_on_swap(const uint8_t *display)
{
    /* DO_MAIN_LOOP_PC_NORMAL swaps the frame built in its previous iteration; its DRAW_MAP captured the snapshot.
       A swap without a capture since the last one (another scene) has no snapshot. */
    const port_scene *s = port_scene_take_pending();
    shown_valid = s != NULL;
    if (s)
        shown = *s;
    if (pad)
        memcpy(swapped, display, sizeof swapped);
}

/* Margins read background columns beyond the original image by wrapping one half of the background image: the
   image is stored as two identical halves (horizontal bands scroll through it; the column backgrounds of the
   shipped levels are the same 320 columns twice). Checked once per image instead of assumed. */
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

/* The original full-screen viewport (the firefly levels, the breakout game and others narrow it; their margins
   stay black, as the original shows nothing beyond the narrowed view either), and a background that can extend. */
static int supported(const port_scene *s)
{
    if (s->view_x0 != 4 || s->view_x1 != 320 || s->view_y0 != 0 || s->view_y1 != 200)
        return 0;
    if (s->background == PORT_SCENE_BG_CLEAR)
        return 1;
    return (s->background == PORT_SCENE_BG_BANDS || s->background == PORT_SCENE_BG_COLUMNS) &&
           image_halves_equal(s);
}

const uint8_t *port_render_compose(const uint8_t *display, int *width)
{
    int w = 320 + 2 * pad, y;
    if (!pad) {
        *width = last_width = 320;
        return last_frame = display;
    }
    memset(wide, 0, (size_t)w * 200);
    if (shown_valid && supported(&shown) && !memcmp(display, swapped, sizeof swapped)) {
        port_surface s = {wide, w, -pad, 0, w, 200, NULL};
        port_rect sides[2] = {{-pad, 0, 0, 200}, {320, 0, 320 + pad, 200}};
        int i;
        for (i = 0; i < 2; i++) {
            port_render_background(&shown, &s, sides[i], PORT_RENDER_EXTENDED);
            port_render_map(&shown, &s, sides[i], PORT_RENDER_EXTENDED);
            port_render_front(&shown, &s, sides[i], PORT_RENDER_EXTENDED);
        }
        drawn_count++;
    } else {
        black_count++;
    }
    for (y = 0; y < 200; y++)
        memcpy(wide + (size_t)y * w + pad, display + 320 * y, 320);
    *width = last_width = w;
    return last_frame = wide;
}

const uint8_t *port_render_last(int *width)
{
    *width = last_width;
    return last_frame;
}

void port_render_counts(long *drawn, long *black)
{
    *drawn = drawn_count;
    *black = black_count;
}
