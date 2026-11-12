/*
 * Semantic capture (port/docs/RENDERER.md). Compiled with the historical units (same flags, pre-include and
 * headers), because it reads their state with their types. It only READS game state: it calls no game draw function
 * a second time and writes no game variable.
 *
 * The passes that DO_MAIN_LOOP_PC_NORMAL (src/pcmain.c) calls are redirected at link time (-Wl,--wrap, see
 * port/CMakeLists.txt) to the wrappers below. Each wrapper tells the frame recorder (render/record.c) which pass
 * the following primitives belong to, then calls the unchanged original exactly once. For the background, map and
 * foreground band sprites it first captures the layer's game state into the frame's semantic payload (render/scene.h)
 * and adds the layer operation that stands for the primitives the original then calls.
 */
#include "game.h"
#include "obj.h"
#include "map.h"
#include "screen_types.h"
#include "lowlevel.h"

#include "render/capture.h"
#include "render/record.h"
#include "platform/pad.h"

#include <stddef.h>
#include <string.h>

extern s16 xmap, ymap, ymapmax, num_world, num_level, rideau_obj_id;
extern s32 Bloc_lim_W1, Bloc_lim_W2, Bloc_lim_H1, Bloc_lim_H2, map_time;
extern u8 BackgroundOn, FondAutorise, RaymanDansUneMapDuJeu;
extern u8 *PLAN0BIT;
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
extern rayevts_t RayEvts;
extern s16 x_luc, y_luc, rayon_luciole, ADDLUCLIP;
extern s32 Bloc_lim_W1_Glob, Bloc_lim_W2_Glob, Bloc_lim_H1_Glob, Bloc_lim_H2_Glob;
extern int RandomIndex;
extern short RandArray[256];

static void capture_view(port_scene *s)
{
    s->xmap = xmap;
    s->ymap = ymap;
    s->view_x0 = (int16_t)Bloc_lim_W1;
    s->view_x1 = (int16_t)Bloc_lim_W2;
    s->view_y0 = (int16_t)Bloc_lim_H1;
    s->view_y1 = (int16_t)Bloc_lim_H2;
    s->map_width = 16 * BIG_MAP.width;
    /* the firefly's lit box before set_luciole cut it to the viewport (y_luc holds the screen x) */
    s->firefly = RayEvts.firefly ? 1 : 0;
    s->lit_x0 = y_luc - rayon_luciole;
    s->lit_x1 = y_luc + rayon_luciole;
    s->glob_x0 = Bloc_lim_W1_Glob;
    s->glob_x1 = Bloc_lim_W2_Glob;
}

/* Bands and band sprites: level data plus the scroll positions Calcul_Deplacement_Bande left in Bande[].field_6. */
static void capture_bands(port_scene *s)
{
    s32 clip[4];
    int i;
    s->plan0 = PLAN0BIT;
    s->plan_width = plan0_width;
    s->plan_height = plan0_height;
    s->nb_bands = NbBande < PORT_SCENE_MAX_BANDS ? NbBande : PORT_SCENE_MAX_BANDS;
    for (i = 0; i < s->nb_bands; i++) {
        s->bands[i].offset = (int16_t)Bande[i].offset;
        s->bands[i].length = (int16_t)Bande[i].length;
        s->bands[i].scroll = (int16_t)Bande[i].field_6;
        /* Calcul_Deplacement_Bande (src/skops.c): how the scroll position follows the camera's x. */
        s->bands[i].cam_mul = 0;
        s->bands[i].cam_div = 1;
        if (Type_Scroll[Num_Fond + 10 * (num_world - 1)] == 0) {
            if (Bande[i].length == 0)
                s->bands[i].cam_mul = 2;
            else if ((Bande[i].field_4 == 0 || Bande[i].field_4 == 4 || Bande[i].field_4 == 5) && Bande[i].field_8) {
                s->bands[i].cam_mul = 1;
                s->bands[i].cam_div = (int16_t)Bande[i].field_8;
            }
        }
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
    get_sprite_clipping(&clip[0], &clip[1], &clip[2], &clip[3]);
    for (i = 0; i < 4; i++)
        s->sprite_clip[i] = clip[i];
}

/* Display_Back_Screen (src/skops.c 200-345) with w1..h2 = Bloc_lim. */
static void capture_background(port_scene *s)
{
    u8 type_scroll = Type_Scroll[Num_Fond + 10 * (num_world - 1)];
    capture_view(s);
    capture_bands(s);
    s->background = type_scroll == 0 || type_scroll == 2 ? PORT_SCENE_BG_BANDS : PORT_SCENE_BG_COLUMNS;
    if (s->background == PORT_SCENE_BG_BANDS) {
        /* curtain clip, then vertical offset */
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
    } else {
        /* vertical branch (src/skops.c 303-345): rows from h1, no vertical offset */
        s->bg_shift = 0;
        s->bg_top = (int16_t)Bloc_lim_H1;
    }
}

/* Display_Sprite_On_Front (src/skops.c 381-440): by the level's scroll type, whether or not the background is on. */
static void capture_front(port_scene *s)
{
    u8 type_scroll = Type_Scroll[Num_Fond + 10 * (num_world - 1)];
    capture_view(s);
    capture_bands(s);
    s->front = type_scroll == 0 || type_scroll == 2 ? PORT_SCENE_BG_BANDS : PORT_SCENE_BG_COLUMNS;
    s->front_y = (int16_t)(2 * ymapmax - 2 * ymap + 200);
}

/* Map window around the camera (update_display_map: tile column c is at screen x 16c - xmap + 8), as far as the
   wider view can reach on either side. */
static void capture_map(port_scene *s)
{
    s16 col0 = (s16)((xmap - 8 - PORT_SCENE_MAX_REACH) >> 4), row0 = (s16)(ymap >> 4);
    int r, c;
    capture_view(s);
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

static void layer(int kind)
{
    port_record_layer(kind, Bloc_lim_W1, Bloc_lim_H1, Bloc_lim_W2, Bloc_lim_H2);
}

/* ---- pass wrappers ---- */

void __real_CLEAR_FIXE_LUCIOLE(void);
void __wrap_CLEAR_FIXE_LUCIOLE(void)
{
    int previous = port_record_pass_begin(PORT_PASS_FRAME_START);
    port_record_gameplay();
    __real_CLEAR_FIXE_LUCIOLE();
    port_record_pass_end(previous);
    /* DO_MAIN_LOOP_PC_NORMAL (src/pcmain.c 529-539) clears the draw buffer next when the background is off (its
       memset is recorded as a fill); the layer payload notes it for the margin compositor. */
    if (RaymanDansUneMapDuJeu && !(BackgroundOn && FondAutorise == 1)) {
        port_record_world()->background = PORT_SCENE_BG_CLEAR;
    }
}

void __real_Display_Back_Screen(u16 plan_width, u16 plan_height, u16 w1, u16 h1, u16 w2, u16 h2);
void __wrap_Display_Back_Screen(u16 plan_width, u16 plan_height, u16 w1, u16 h1, u16 w2, u16 h2)
{
    int previous = port_record_pass_begin(PORT_PASS_BACKGROUND);
    capture_background(port_record_world());
    layer(PORT_OP_BACKGROUND);
    __real_Display_Back_Screen(plan_width, plan_height, w1, h1, w2, h2);
    port_record_pass_end(previous);
}

void __real_DRAW_MAP(u8 *draw_buf, big_map_t *big_map);
void __wrap_DRAW_MAP(u8 *draw_buf, big_map_t *big_map)
{
    int previous = port_record_pass_begin(PORT_PASS_MAP);
    capture_map(port_record_world());
    layer(PORT_OP_MAP);
    __real_DRAW_MAP(draw_buf, big_map);
    port_record_pass_end(previous);
}

void __real_Display_Sprite_On_Front(u16 plan_width, u16 plan_height, u16 w1, u16 h1, u16 w2, u16 h2);
void __wrap_Display_Sprite_On_Front(u16 plan_width, u16 plan_height, u16 w1, u16 h1, u16 w2, u16 h2)
{
    int previous = port_record_pass_begin(PORT_PASS_FRONT);
    capture_front(port_record_world());
    layer(PORT_OP_FRONT);
    __real_Display_Sprite_On_Front(plan_width, plan_height, w1, h1, w2, h2);
    port_record_pass_end(previous);
}

/* Display_and_free_luciole (src/luciole.c 269-292) draws the darkness itself (aff_luciole). Its inputs are read
   after it ran: the ADDLUCLIP it computed, the centre (aff_luciole is called as (y_luc, x_luc): y_luc holds the
   screen x) and the full viewport, which it restored into Bloc_lim from Bloc_lim_*_Glob. */
void __real_Display_and_free_luciole(u8 *draw_buf);
void __wrap_Display_and_free_luciole(u8 *draw_buf)
{
    int previous = port_record_pass_begin(PORT_PASS_FIREFLY);
    __real_Display_and_free_luciole(draw_buf);
    if (RayEvts.firefly)
        port_record_firefly(y_luc, x_luc, rayon_luciole, ADDLUCLIP, Bloc_lim_W1_Glob, Bloc_lim_W2_Glob,
                            Bloc_lim_H1_Glob, Bloc_lim_H2_Glob);
    port_record_pass_end(previous);
}

/* ClearBorder (src/box.c 408-428) clears outside the viewport with its own C loops, into draw_buffer, which its
   callers point at the display or the draw buffer (DISPLAY_FIXE at fixontemp == 1, DISPLAY_FOND_MENU). */
extern u8 *draw_buffer;
void __real_ClearBorder(s16 lim_H1, s16 lim_H2, s16 lim_W1, s16 lim_W2);
void __wrap_ClearBorder(s16 lim_H1, s16 lim_H2, s16 lim_W1, s16 lim_W2)
{
    port_record_clear_outside(draw_buffer, lim_W1, lim_H1, lim_W2, lim_H2);
    __real_ClearBorder(lim_H1, lim_H2, lim_W1, lim_W2);
}

#define PASS_VOID(name, pass) \
    void __real_##name(void); \
    void __wrap_##name(void) \
    { \
        int previous = port_record_pass_begin(pass); \
        __real_##name(); \
        port_record_pass_end(previous); \
    }
#define PASS_ARG(name, pass, type) \
    void __real_##name(type a); \
    void __wrap_##name(type a) \
    { \
        int previous = port_record_pass_begin(pass); \
        __real_##name(a); \
        port_record_pass_end(previous); \
    }

PASS_VOID(display_flocons_behind, PORT_PASS_REAR_WEATHER)
PASS_VOID(display_grp_stars, PORT_PASS_STARS)
/* Wider view, presentation only: objects the game does not draw because they are outside their activation zone
   (in_action_zone, src/moteur.c: origin screen x within [-284 - zonediffx, 444 + zonediffx]), which a view wider
   than the zone shows as late appearance on one side and early disappearance of wide sprites on the other. Drawn
   (render/native.c, margins only, behind the active objects) for the types measured as static: over a camera
   sweep of 59 levels at pad 100 (RENDERER.md 9, "Inactive static objects") each never moved, changed animation
   or flipped while active, and showed no jump, animation change or flip when switching on or off (at least 3
   such transitions). Reads game state only, as display2 (src/affiche.c 391) would draw the object. */
static const u8 inactive_drawable[262] = {
    [1] = 1, [2] = 1, [4] = 1, [5] = 1, [8] = 1, [16] = 1, [21] = 1, [27] = 1, [41] = 1, [42] = 1, [53] = 1,
    [58] = 1, [63] = 1, [67] = 1, [82] = 1, [85] = 1, [87] = 1, [88] = 1, [95] = 1, [101] = 1, [105] = 1,
    [109] = 1, [124] = 1, [142] = 1, [154] = 1, [155] = 1, [158] = 1, [160] = 1, [163] = 1, [164] = 1, [167] = 1,
    [175] = 1, [176] = 1, [188] = 1, [229] = 1, [230] = 1, [234] = 1, [237] = 1, [240] = 1, [243] = 1};
extern s16 zonediffx[262];
extern obj_type_flags_t flags[262];
s16 get_proj_x(s16 scale, s16 unprojected_x);
s16 get_proj_y(s16 scale, s16 unprojected_y);
s16 get_proj_dist(s16 scale, s16 outer_dim);

static void capture_inactive_objects(void)
{
    int o;
    if (!port_record_pad() || !RaymanDansUneMapDuJeu || !level.objects)
        return;
    for (o = 0; o < level.nb_objects; o++) {
        obj_t *obj = &level.objects[o];
        s32 screen_x = obj->x - xmap + 8, screen_y = obj->y - ymap, zd;
        anim_t *anim;
        anim_layer_t *layer;
        s16 layers_per_frame;
        int i;
        if (obj->type >= 262 || !inactive_drawable[obj->type] || !obj->flags.alive || obj->is_active ||
            (flags[obj->type].flags0 & 0x90) || obj->display_prio < 1 || obj->display_prio > 7 ||
            !obj->animations || !obj->sprites || !obj->img_buffer || obj->anim_index == 255)
            continue;
        zd = zonediffx[obj->type];
        if (screen_x >= -284 - zd && screen_x <= 444 + zd)
            continue;   /* inside the zone: the game keeps it inactive for its own reasons */
        anim = obj->animations + obj->anim_index;
        layers_per_frame = anim->layers_per_frame & 0x3FFF;
        if (!anim->layers || obj->anim_frame >= anim->frames_count)
            continue;
        layer = anim->layers + obj->anim_frame * layers_per_frame;
        for (i = 0; i < layers_per_frame; i++, layer++) {
            sprite_t *sprite;
            s16 x, w, h;
            if (!layer->sprite_index)
                continue;
            sprite = obj->sprites + layer->sprite_index;
            if (!sprite->id)
                continue;
            if (!obj->flags.flip_x)
                x = (s16)(layer->x + screen_x);
            else
                x = (s16)((obj->scale == 256 && i == 5 && obj->anim_index <= 16 && obj->anim_index >= 14 ? -16 : 0) +
                          obj->offset_bx * 2 - layer->x - sprite->outer_width + screen_x);
            w = get_proj_dist(obj->scale, sprite->outer_width);
            h = get_proj_dist(obj->scale, sprite->outer_height);
            port_record_extra_sprite(obj->flags.flip_x ^ layer->mirrored, get_proj_x(obj->scale, x),
                                     sprite->color >> 4, get_proj_y(obj->scale, (s16)(layer->y + screen_y)),
                                     (u16)((h << 8) + w), obj->img_buffer + sprite->offset_in_atlas);
        }
    }
}

/* The boss bar's sprites, so that the renderer can tell its operations from the objects' (render/native.c): the
   images display_bar_boss passes (fist object sprites 60, 61 and sprite_of_ref of each boss icon) and its rows
   (Bloc_lim_H2 - 19, - 20, - 30 in the normal video mode, projected like display_sprite does). */
extern obj_t *poing_obj;
extern s32 Bloc_lim_H2;
u8 sprite_of_ref(obj_t *obj, u8 ref);
static void capture_boss_bar(port_scene *s)
{
    static const u8 icons[] = {44, 108, 109, 110, 111, 112, 113};
    int i, n = 0;
    s->nb_bossbar_images = 0;
    if (!poing_obj || !poing_obj->sprites || !poing_obj->img_buffer || poing_obj->nb_sprites <= 61)
        return;
    s->bossbar_images[n++] = poing_obj->img_buffer + poing_obj->sprites[60].offset_in_atlas;
    s->bossbar_images[n++] = poing_obj->img_buffer + poing_obj->sprites[61].offset_in_atlas;
    for (i = 0; i < (int)sizeof icons; i++) {
        u8 index = sprite_of_ref(poing_obj, icons[i]);
        if (index < poing_obj->nb_sprites)
            s->bossbar_images[n++] = poing_obj->img_buffer + poing_obj->sprites[index].offset_in_atlas;
    }
    s->nb_bossbar_images = n;
    s->bossbar_y[0] = get_proj_y(poing_obj->scale, (s16)(Bloc_lim_H2 - 19));
    s->bossbar_y[1] = get_proj_y(poing_obj->scale, (s16)(Bloc_lim_H2 - 20));
    s->bossbar_y[2] = get_proj_y(poing_obj->scale, (s16)(Bloc_lim_H2 - 30));
}

void __real_DISPLAY_ALL_OBJECTS(void);
void __wrap_DISPLAY_ALL_OBJECTS(void)
{
    int previous = port_record_pass_begin(PORT_PASS_OBJECTS);
    capture_boss_bar(port_record_world());
    capture_inactive_objects();
    __real_DISPLAY_ALL_OBJECTS();
    port_record_pass_end(previous);
}
PASS_VOID(display_pix_gerbes, PORT_PASS_PARTICLES)
PASS_VOID(display_flocons_before, PORT_PASS_FRONT_WEATHER)
PASS_VOID(DISPLAY_TEXT_FEE, PORT_PASS_FAIRY_TEXT)
PASS_ARG(DISPLAY_FIXE, PORT_PASS_HUD, s32)
PASS_VOID(DO_DARK2_AFFICHE_TEXT, PORT_PASS_HUD_TEXT)
PASS_VOID(DisplayCrackers, PORT_PASS_CRACKERS)
PASS_VOID(DisplayProgrammerMessage, PORT_PASS_PROGRAMMER)
PASS_ARG(display_time, PORT_PASS_TIME, s32)
PASS_VOID(DISPLAY_GAME_VIGNET, PORT_PASS_VIGNETTE)
PASS_VOID(DO_OPTIONS_IN_GAME, PORT_PASS_OPTIONS)
PASS_VOID(Do_Effect_Pause, PORT_PASS_PAUSE)
PASS_ARG(HANDLE_KEY, PORT_PASS_KEYS, u8 *)

/* Scene labels: the scene functions PcMain (game/port_main.c) calls, for the renderer's per-scene statistics. */
#define SCENE_VOID(name) \
    void __real_##name(void); \
    void __wrap_##name(void) \
    { \
        const char *previous = port_record_scene(#name); \
        __real_##name(); \
        port_record_scene(previous); \
    }
#define SCENE_ARG(name, type) \
    void __real_##name(type a); \
    void __wrap_##name(type a) \
    { \
        const char *previous = port_record_scene(#name); \
        __real_##name(a); \
        port_record_scene(previous); \
    }

SCENE_VOID(DO_UBI_LOGO)
SCENE_VOID(DO_GROS_RAYMAN)
SCENE_VOID(InitDemoJeu)
SCENE_VOID(DO_NEW_MENUS)
/* Development and test scenarios: RAYMAN_START_LEVEL="W L" makes the first world-map visit choose world W,
   level L (the map runs as usual; only its choice is replaced when it returns). */
void __real_DO_WORLD_MAP(void);
void __wrap_DO_WORLD_MAP(void)
{
    extern s16 num_world_choice, num_level_choice;
    static int used;
    const char *previous = port_record_scene("DO_WORLD_MAP");
    const char *want = getenv("RAYMAN_START_LEVEL");
    int w, l;
    __real_DO_WORLD_MAP();
    if (!used && want && sscanf(want, "%d %d", &w, &l) == 2 && w >= 1 && w <= 6 && l >= 1 && l <= 30) {
        num_world_choice = (s16)w;
        num_level_choice = (s16)l;
    }
    used = 1;
    port_record_scene(previous);
}
SCENE_VOID(DEPART_WORLD)
SCENE_VOID(DEPART_LEVEL)
SCENE_VOID(START_LEVEL_ANIM)
SCENE_VOID(INIT_GAME_MODE_NORMAL)
SCENE_ARG(DO_MAIN_LOOP_PC_NORMAL, u8 *)
SCENE_VOID(FIN_GAME_MODE_NORMAL)
SCENE_VOID(DO_CONTINUE)
SCENE_VOID(DO_VICTOIRE)
SCENE_VOID(FinDemoJeu)
SCENE_VOID(FIN_GAME_LOOP)
SCENE_VOID(FIN_MAP_LOOP)
SCENE_VOID(FIN_DEAD_LOOP)
SCENE_ARG(INIT_MOTEUR_LEVEL, s32)

s32 __real_playVideo(const char *path, const char *filename, u8 audio_track);
s32 __wrap_playVideo(const char *path, const char *filename, u8 audio_track)
{
    const char *previous = port_record_scene("playVideo");
    s32 result = __real_playVideo(path, filename, audio_track);
    port_record_scene(previous);
    return result;
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

/* Input context for the virtual pad and touch HUD (platform/pad.h, port/docs/TOUCH_CONTROLS.md). Read from the
   thread that polls SDL events while the game runs: single-byte flags and a scene label pointer, only read. The
   Android release switches its HUD at the same points (setGameState: world map / level; displayGameVignetInit,
   levelAnimInit, DO_FEE_ETAPE: HUD hidden; INIT_RAY_ON_MS: Ray on the mosquito). */
extern u8 GoMenu, OptionGame, is_fee, NewMs, Vignet_To_Display;   /* OptionGame: src/menu.c in-level menu loop */
extern u8 key_right, key_up, key_left, key_down, key_fist, key_jump, key_action;

int port_game_input_context(uint8_t keys[PORT_KEY_COUNT], unsigned *powers)
{
    const char *scene = port_record_current_scene();
    rayevts_t evts = RayEvts;
    keys[PORT_KEY_RIGHT] = key_right;
    keys[PORT_KEY_UP] = key_up;
    keys[PORT_KEY_LEFT] = key_left;
    keys[PORT_KEY_DOWN] = key_down;
    keys[PORT_KEY_FIST] = key_fist;
    keys[PORT_KEY_JUMP] = key_jump;
    keys[PORT_KEY_ACTION] = key_action;
    *powers = (evts.poing ? PORT_POWER_FIST : 0) | (evts.helico ? PORT_POWER_HELICO : 0) |
              (evts.super_helico ? PORT_POWER_SUPER_HELICO : 0) | (evts.run ? PORT_POWER_RUN : 0) |
              (evts.magicseed ? PORT_POWER_SEED : 0);
    if (!scene)
        return PORT_CONTEXT_MENU;
    if (!strcmp(scene, "DO_WORLD_MAP"))
        return PORT_CONTEXT_WORLDMAP;
    if (!strcmp(scene, "START_LEVEL_ANIM"))
        return PORT_CONTEXT_LEVEL_NO_HUD;
    if (!strcmp(scene, "DO_MAIN_LOOP_PC_NORMAL") && RaymanDansUneMapDuJeu) {
        if (GoMenu || OptionGame)
            return PORT_CONTEXT_MENU;   /* the in-level menu Esc opened (Android: its pause popup) */
        if (is_fee || Vignet_To_Display)
            return PORT_CONTEXT_LEVEL_NO_HUD;
        return NewMs ? PORT_CONTEXT_MOSKITO : PORT_CONTEXT_GAMEPLAY;
    }
    return PORT_CONTEXT_MENU;
}
