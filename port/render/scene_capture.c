/*
 * Render snapshot capture (port/docs/RENDERER.md). Compiled with the historical units (same flags, pre-include
 * and headers), because it reads their state with their types. It only READS game state: it calls no game draw
 * function and writes no game variable.
 *
 * Observation point: DRAW_MAP's call in DO_MAIN_LOOP_PC_NORMAL (src/pcmain.c 545) is redirected at link time
 * (-Wl,--wrap=DRAW_MAP) to __wrap_DRAW_MAP, which captures and then calls the unchanged DRAW_MAP. At that
 * point the frame's engine updates, camera, band scrolls (Calcul_Deplacement_Bande) and background are done.
 *
 * The exactness gate (--verify-render) renders the snapshot's background and map with render/world.c and
 * compares them with the draw buffer the original code produced, before and after the original DRAW_MAP.
 */
#include "game.h"
#include "obj.h"
#include "map.h"
#include "screen_types.h"

#include "render/capture.h"
#include "render/world.h"

#include <stdio.h>
#include <string.h>

extern s16 xmap, ymap, ymapmax, num_world, num_level, rideau_obj_id, MapAvecPluieOuNeige;
extern s32 Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2, map_time;
extern u8 BackgroundOn, FondAutorise, RaymanDansUneMapDuJeu;
extern u8 *PLAN0BIT, *DrawBufferNormal;
extern s32 BufferNormalDeplt;
extern u16 plan0_width, plan0_height;
extern bande_t Bande[300];
extern def_sprite_t Sprite[20];
extern u8 Type_Scroll[60];
extern u16 NbBande, NbSprite, Num_Fond, Val_Div_Scroll_Y, Val_Add_Scroll_Y;
extern obj_t *ScrollDiffSprites;
extern u8 *Scroll_Masque[20];
extern big_map_t BIG_MAP;
extern level_t level;
extern obj_t ray;
extern int RandomIndex;
extern short RandArray[256];

void __real_DRAW_MAP(u8 *draw_buf, big_map_t *big_map);

static port_scene pending;
static int pending_new;
static int verify;
static struct {
    long frames, background_frames, map_frames, mismatched_frames, unwritten_px, background_skipped_weather;
    long long background_px, map_px, background_bad, map_bad;
    int reported;
} stats;

static void capture(port_scene *s)
{
    u8 type_scroll = Type_Scroll[Num_Fond + 10 * (num_world - 1)];
    s16 col0, row0;
    int r, c, i;
    s->seq++;
    s->xmap = xmap;
    s->ymap = ymap;
    s->view_x0 = (int16_t)Bloc_lim_W1;
    s->view_x1 = (int16_t)Bloc_lim_W2;
    s->view_y0 = (int16_t)Bloc_lim_H1;
    s->view_y1 = (int16_t)Bloc_lim_H2;
    /* DO_MAIN_LOOP_PC_NORMAL (src/pcmain.c 525-531): bands when BackgroundOn && FondAutorise == 1, else a clear. */
    if (!(BackgroundOn && FondAutorise == 1))
        s->background = PORT_SCENE_BG_CLEAR;
    else if (type_scroll == 0 || type_scroll == 2)
        s->background = PORT_SCENE_BG_BANDS;
    else
        s->background = PORT_SCENE_BG_COLUMNS;
    s->plan0 = PLAN0BIT;
    s->plan_width = plan0_width;
    s->plan_height = plan0_height;
    s->nb_bands = s->nb_sprites = 0;
    if (s->background == PORT_SCENE_BG_BANDS) {
        /* Display_Back_Screen (src/skops.c 200-243) with w1..h2 = Bloc_lim: curtain clip, then vertical offset. */
        u16 h1 = (u16)Bloc_lim_H1, h2 = (u16)Bloc_lim_H2;
        s16 excess_height = (s16)(plan0_height - 200), shift;
        if (num_world == 4 /* world_4_image */ && (num_level == 4 || num_level == 11)) {
            obj_t *rideau = level.objects + rideau_obj_id;
            if (rideau->is_active) {
                if (h1 < rideau->y - 110 - (200 - h2)) {
                    h1 = (u16)(rideau->y - 110 - (200 - h2));
                    if ((s16)h1 < 0)
                        h1 = 0;
                }
                if (h1 >= h2)
                    h1 = h2 - 1;
            }
        }
        if (Type_Scroll[10 * (num_world - 1) + Num_Fond] != 0)
            shift = (s16)(excess_height + (200 - h2));
        else if (num_world == 3 /* world_3_mountain */ && num_level == 10)
            shift = (s16)((ymap >> 1) + (200 - h2) + 11);
        else
            shift = (s16)(Val_Add_Scroll_Y + ymap / Val_Div_Scroll_Y);
        if (shift > 200 - h2 + excess_height)
            shift = (s16)(excess_height + (200 - h2));
        s->bg_shift = shift;
        s->bg_top = (int16_t)h1;
        /* Each band row is copied from logical x w1 - BufferNormalDeplt (the draw buffer's alignment shift), w2 - w1
           bytes rounded up to 8 (Copy_Plan0Diff_To_Buf): the whole viewport unless it is narrow. */
        s->bg_x0 = (int16_t)(Bloc_lim_W1 - BufferNormalDeplt);
        s->bg_x1 = (int16_t)(s->bg_x0 + (((s16)(Bloc_lim_W2 - Bloc_lim_W1) + 7) / 8) * 8);
        /* Display_Sprite_On_Front (src/skops.c 382-399) */
        s->front_y = (int16_t)(2 * ymapmax - 2 * ymap + 200);
    } else if (s->background == PORT_SCENE_BG_COLUMNS) {
        /* Display_Back_Screen's vertical branch (src/skops.c 303-345): rows from h1, no vertical offset. */
        s->bg_shift = 0;
        s->bg_top = (int16_t)Bloc_lim_H1;
        s->front_y = 0;
    }
    if (s->background != PORT_SCENE_BG_CLEAR) {
        s->nb_bands = NbBande < PORT_SCENE_MAX_BANDS ? NbBande : PORT_SCENE_MAX_BANDS;
        for (i = 0; i < s->nb_bands; i++) {
            s->bands[i].offset = (int16_t)Bande[i].offset;
            s->bands[i].length = (int16_t)Bande[i].length;
            s->bands[i].scroll = (int16_t)Bande[i].field_6;
        }
        s->nb_sprites = NbSprite < PORT_SCENE_MAX_BG_SPRITES ? NbSprite : PORT_SCENE_MAX_BG_SPRITES;
        for (i = 0; i < s->nb_sprites; i++) {
            sprite_t *sp = ScrollDiffSprites->sprites + i + 1;
            s->sprites[i].x = Sprite[i].x;
            s->sprites[i].y = Sprite[i].y;
            s->sprites[i].band = Sprite[i].bande_index;
            s->sprites[i].width = sp->outer_width;
            s->sprites[i].height = sp->outer_height;
            s->sprites[i].image = ScrollDiffSprites->img_buffer + sp->offset_in_atlas;
            s->sprites[i].mask = Scroll_Masque[i + 1];
        }
    }
    /* Map window around the camera (update_display_map: tile column c is at screen x 16c - xmap + 8). */
    col0 = (s16)((xmap - 8 - PORT_SCENE_MAX_PAD) >> 4);
    row0 = (s16)(ymap >> 4);
    s->tile_col0 = col0;
    s->tile_row0 = row0;
    s->original_map_grid_w = BIG_MAP.display_map->width_in_tiles;
    s->original_map_grid_h = BIG_MAP.display_map->height_in_tiles;
    for (r = 0; r < PORT_SCENE_TILE_ROWS; r++)
        for (c = 0; c < PORT_SCENE_TILE_COLS; c++) {
            int col = col0 + c, row = row0 + r;
            port_scene_tile *t = &s->tiles[r][c];
            if (col < 0 || row < 0 || col >= BIG_MAP.width || row >= BIG_MAP.height) {
                t->transparency = 0;
                t->texture = NULL;
            } else {
                int index = row * BIG_MAP.width + col;
                t->transparency = BIG_MAP.map[index].transparency;
                t->texture = BIG_MAP.tile_texture_offsets[index];
            }
        }
}

/* The original draw buffer as a surface: logical pixel (x, y) is DrawBufferNormal[320 y + x]. */
static void read_draw_buffer(uint8_t *out)
{
    memcpy(out, DrawBufferNormal, 64000);
}

static long compare(const uint8_t *want, const uint8_t *got, const uint8_t *only, port_rect r, const char *what)
{
    long bad = 0;
    int x, y;
    for (y = r.y0; y < r.y1; y++)
        for (x = r.x0; x < r.x1; x++) {
            int i = 320 * y + x;
            if (only && !only[i])
                continue;
            if (want[i] != got[i]) {
                if (!bad && stats.reported < 5) {
                    stats.reported++;
                    fprintf(stderr, "render verify: %s differs at frame %ld (%d,%d): original %u, semantic %u "
                            "(world %d level %d, camera %d,%d)\n", what, stats.frames, x, y, want[i], got[i],
                            num_world, num_level, xmap, ymap);
                }
                bad++;
            }
        }
    return bad;
}

void __wrap_DRAW_MAP(u8 *draw_buf, big_map_t *big_map)
{
    static uint8_t before[64000], after[64000], semantic[64000], written[64000];
    port_rect view;
    capture(&pending);
    pending_new = 1;
    stats.frames++;
    if (verify)
        read_draw_buffer(before);
    __real_DRAW_MAP(draw_buf, big_map);
    if (!verify)
        return;
    read_draw_buffer(after);
    view.x0 = pending.view_x0;
    view.x1 = pending.view_x1;
    view.y0 = pending.view_y0;
    view.y1 = pending.view_y1;
    {
        port_surface s = {semantic, 320, 0, 0, 320, 200, written};
        long bad_bg = 0, bad_map, n = 0;
        int x, y;
        /* 1. Background alone against the buffer before DRAW_MAP (the pixels the background wrote). Rear rain/snow
           is drawn between the background and DRAW_MAP, so those frames have no background reference. */
        if (MapAvecPluieOuNeige) {
            stats.background_skipped_weather++;
        } else {
            memset(semantic, 0, sizeof semantic);
            memset(written, 0, sizeof written);
            port_render_background(&pending, &s, view, PORT_RENDER_ORIGINAL);
            bad_bg = compare(before, semantic, written, view, "background");
            for (y = view.y0; y < view.y1; y++)
                for (x = view.x0; x < view.x1; x++)
                    n += written[320 * y + x];
            stats.background_px += n;
            stats.unwritten_px += (long)(view.x1 - view.x0) * (view.y1 - view.y0) - n;
            stats.background_frames++;
        }
        /* 2. Map over the original's own pre-map image against the buffer after DRAW_MAP. */
        memcpy(semantic, before, sizeof semantic);
        s.written = NULL;
        port_render_map(&pending, &s, view, PORT_RENDER_ORIGINAL);
        bad_map = compare(after, semantic, NULL, view, "map");
        stats.map_px += (long)(view.x1 - view.x0) * (view.y1 - view.y0);
        stats.map_frames++;
        stats.background_bad += bad_bg;
        stats.map_bad += bad_map;
        if (bad_bg || bad_map)
            stats.mismatched_frames++;
    }
}

const port_scene *port_scene_take_pending(void)
{
    if (!pending_new)
        return NULL;
    pending_new = 0;
    return &pending;
}

void port_scene_set_verify(int on) { verify = on; }

void port_scene_verify_report(void)
{
    if (!verify)
        return;
    printf("render verify: frames=%ld mismatched_frames=%ld background_frames=%ld background_px=%lld "
           "background_bad=%lld map_frames=%ld map_px=%lld map_bad=%lld unwritten_px=%ld "
           "background_skipped_weather=%ld\n",
           stats.frames, stats.mismatched_frames, stats.background_frames, stats.background_px,
           stats.background_bad, stats.map_frames, stats.map_px, stats.map_bad, stats.unwritten_px,
           stats.background_skipped_weather);
}

/* FNV-1a over the gameplay state the presentation must never change: camera, clock, random table position and
   contents, Rayman and every level object from is_active through field_83 (no pointers in that range). */
/* is_active .. field_83: every non-pointer field, without the host's tail padding (same bytes on every host). */
#define OBJ_STATE_BYTES (offsetof(obj_t, field_83) + 1 - offsetof(obj_t, is_active))

uint64_t port_scene_state_digest(void)
{
    uint64_t h = 1469598103934665603ULL;
    const unsigned char *p;
    size_t n, i;
    int o;
#define MIX(ptr, len) for (p = (const unsigned char *)(ptr), n = (len), i = 0; i < n; i++) \
        h = (h ^ p[i]) * 1099511628211ULL
    MIX(&xmap, sizeof xmap);
    MIX(&ymap, sizeof ymap);
    MIX(&map_time, sizeof map_time);
    MIX(&RandomIndex, sizeof RandomIndex);
    MIX(RandArray, sizeof RandArray);
    /* Objects only while a level map runs (DO_MAIN_LOOP_PC_NORMAL sets the flag on entry, clears it on exit):
       around level changes `level` still names released memory. */
    if (RaymanDansUneMapDuJeu) {
        MIX(&ray.is_active, OBJ_STATE_BYTES);
        for (o = 0; level.objects && o < level.nb_objects; o++)
            MIX(&level.objects[o].is_active, OBJ_STATE_BYTES);
    }
#undef MIX
    return h;
}
