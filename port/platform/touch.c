/*
 * Touch controls of the Android release (port/docs/TOUCH_CONTROLS.md), feeding the virtual pad (platform/pad.h).
 *
 * GUI space as in the Android scripts: x -1 (left) .. +1 (right), y -1 (bottom) .. +1 (top); stick distances in x
 * units (a vertical offset is divided by the aspect ratio). Levels: floating stick on the left (x < -0.2), Jump
 * (A) at (0.86, -0.73), Fist (B) above it, Action (X) left of it, Pause at (0, 0.8). World map: fixed 4-way cross
 * at (-0.69, -0.6), radius 0.35, and the GO button (A) bottom right. DOS menus and other screens (tap-to-key, the
 * port's own: the Android release had different menus): the same cross, OK (A) bottom right, Back (B) top left.
 * With a gamepad connected the overlay is hidden and ignores touches, as in the Android HUD.
 *
 * Runs on the thread that polls SDL events and presents (platform/host.c).
 */
#include "platform/touch.h"
#include "platform/pad.h"

#include <SDL3/SDL.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FINGERS 10
#define STICK_RADIUS 0.07f          /* PakTouchJoystick.Radius */
#define STICK_ZONE_X (-0.2f)        /* RayJoystick.PointInActivationArea (gameplay) */
#define CROSS_X (-0.69f)            /* RayJoystick.DPadCenter */
#define CROSS_Y (-0.6f)
#define CROSS_RADIUS 0.35f          /* RayJoystick.DPadRadius */
/* Button half-size in pixels relative to the screen height. OPEN in the spec (the Android size comes from the
   texture's pixel size x 0.495); chosen to match the release's look on 16:9..20:9 phones. */
#define BUTTON_HALF 0.105f

enum { EL_JUMP, EL_FIST, EL_ACTION, EL_PAUSE, EL_OK, EL_BACK, EL_COUNT };

typedef struct element {
    float cx, cy, half;                       /* centre and half-size, pixels */
    float left, right, top, bottom;           /* hit box scales (RaymanActionButton: 0.70 each unless overridden) */
    int button;                               /* pad button */
    int tap;                                  /* acts on release (pause) instead of while held */
    int visible;
    const char *label;
    SDL_FingerID finger;                      /* finger holding it; 0 = none */
    int pressed;
} element;

static int enabled;                           /* overlay active on this platform */
static int width = 1, height = 1;             /* render output size */
static element el[EL_COUNT];
static int ctx = -1;
static unsigned powers;

/* stick / cross */
static SDL_FingerID stick_finger;
static float stick_ox, stick_oy;              /* origin, GUI units */
static float stick_vx, stick_vy;              /* value, unit disc */
static int fingers_down;                      /* any finger: a level without HUD takes a tap anywhere as A */

void port_touch_init(int on)
{
    enabled = on;
}

int port_touch_enabled(void) { return enabled; }

static float aspect(void) { return (float)width / (float)height; }
static float to_gx(float px) { return px / width * 2.0f - 1.0f; }
static float to_gy(float py) { return 1.0f - py / height * 2.0f; }
static float px_of(float gx) { return (gx + 1.0f) * 0.5f * width; }
static float py_of(float gy) { return (1.0f - gy) * 0.5f * height; }

static int overlay_active(void)
{
    return enabled && !port_pad_gamepad_connected();
}

static int is_level(int c) { return c == PORT_CONTEXT_GAMEPLAY || c == PORT_CONTEXT_MOSKITO; }
static int has_cross(int c) { return c == PORT_CONTEXT_WORLDMAP || c == PORT_CONTEXT_MENU; }

static void set_element(int i, float cx, float cy, float half, int button, const char *label)
{
    element *e = &el[i];
    e->cx = cx;
    e->cy = cy;
    e->half = half;
    e->left = e->right = e->top = e->bottom = 0.70f;
    e->button = button;
    e->tap = 0;
    e->label = label;
}

/* Layout and visibility for the context (PakRaymanHUD, MenuGUI.startWorldMap). */
static void layout(void)
{
    float h = BUTTON_HALF * height, jx = px_of(0.86f), jy = py_of(-0.73f);
    int c = ctx, i;
    set_element(EL_JUMP, jx, jy, h, PORT_PAD_A, "A");
    el[EL_JUMP].right = 1.0f;
    /* Fist: anchor (0, 1.6) of Jump, pivot (0, -1): its bottom edge 1.6 half-sizes above Jump's centre. */
    set_element(EL_FIST, jx, jy - 1.6f * h - h, h, PORT_PAD_B, "B");
    el[EL_FIST].right = el[EL_FIST].top = 1.0f;
    /* Action: anchor (-1.6, 0), pivot (1, 0): its right edge 1.6 half-sizes left of Jump's centre. */
    set_element(EL_ACTION, jx - 1.6f * h - h, jy, h, PORT_PAD_X, "X");
    set_element(EL_PAUSE, px_of(0.0f), py_of(0.8f), 0.6f * h, PORT_PAD_BACK, "II");
    el[EL_PAUSE].tap = 1;
    /* GO / OK: anchor (0.92, -1), pivot (1, -1): bottom-right corner there. */
    set_element(EL_OK, px_of(0.92f) - h, py_of(-1.0f) - h - 0.08f * height, h, PORT_PAD_A, "OK");
    set_element(EL_BACK, px_of(-0.92f) + 0.6f * h, py_of(0.8f), 0.6f * h, PORT_PAD_B, "<");
    for (i = 0; i < EL_COUNT; i++)
        el[i].visible = 0;
    if (is_level(c)) {
        el[EL_JUMP].visible = c == PORT_CONTEXT_GAMEPLAY;
        el[EL_JUMP].label = (powers & PORT_POWER_SUPER_HELICO) ? "A+" : (powers & PORT_POWER_HELICO) ? "A^" : "A";
        el[EL_FIST].visible = (powers & PORT_POWER_FIST) != 0;
        el[EL_ACTION].visible = c == PORT_CONTEXT_GAMEPLAY;   /* Grimace is always available */
        el[EL_ACTION].label = (powers & PORT_POWER_SEED) ? "X*" : (powers & PORT_POWER_RUN) ? "X>" : "X";
        el[EL_PAUSE].visible = 1;
    } else if (c == PORT_CONTEXT_WORLDMAP) {
        el[EL_OK].visible = 1;
        el[EL_OK].label = "GO";
    } else if (c == PORT_CONTEXT_MENU) {
        el[EL_OK].visible = 1;
        el[EL_BACK].visible = 1;
    }
}

static int inside(const element *e, float x, float y)
{
    return x >= e->cx - e->half * e->left && x <= e->cx + e->half * e->right &&
           y >= e->cy - e->half * e->top && y <= e->cy + e->half * e->bottom;
}

static void release_all(void)
{
    int i;
    for (i = 0; i < EL_COUNT; i++) {
        el[i].finger = 0;
        el[i].pressed = 0;
    }
    stick_finger = 0;
    stick_vx = stick_vy = 0;
    port_pad_clear(PORT_PAD_FROM_TOUCH);
    /* fingers_down keeps counting: a finger already on the glass still lifts later */
}

/* Stick value for a finger at GUI (gx, gy): offset from the origin, y in x units, / radius, clamped to 1. */
static void stick_value(float gx, float gy)
{
    float dx = (gx - stick_ox) / STICK_RADIUS, dy = (gy - stick_oy) / aspect() / STICK_RADIUS;
    float len = sqrtf(dx * dx + dy * dy);
    if (len > 1.0f) {
        dx /= len;
        dy /= len;
    }
    stick_vx = dx;
    stick_vy = dy;
}

static int stick_claims(float gx, float gy)
{
    if (is_level(ctx))
        return gx < STICK_ZONE_X && gx <= 0.0f;
    if (has_cross(ctx)) {
        float dx = gx - CROSS_X, dy = (gy - CROSS_Y) / aspect();
        return dx * dx + dy * dy < CROSS_RADIUS * CROSS_RADIUS;
    }
    return 0;
}

static void finger_down(SDL_FingerID id, float x, float y)
{
    float gx = to_gx(x), gy = to_gy(y);
    int i;
    fingers_down++;
    if (!stick_finger && stick_claims(gx, gy)) {
        stick_finger = id;
        if (is_level(ctx)) {            /* floating: the stick's centre is where the thumb lands */
            stick_ox = gx;
            stick_oy = gy;
            stick_vx = stick_vy = 0;
        } else {                        /* fixed cross: the value counts at once */
            stick_ox = CROSS_X;
            stick_oy = CROSS_Y;
            stick_value(gx, gy);
        }
        return;
    }
    for (i = 0; i < EL_COUNT; i++)
        if (el[i].visible && !el[i].finger && inside(&el[i], x, y)) {
            el[i].finger = id;
            el[i].pressed = 1;
            return;
        }
}

static void finger_move(SDL_FingerID id, float x, float y)
{
    int i;
    if (id == stick_finger) {
        stick_value(to_gx(x), to_gy(y));
        return;
    }
    for (i = 0; i < EL_COUNT; i++)
        if (el[i].finger == id)
            el[i].pressed = inside(&el[i], x, y);
}

static void finger_up(SDL_FingerID id, float x, float y)
{
    int i;
    if (fingers_down > 0)
        fingers_down--;
    if (id == stick_finger) {
        stick_finger = 0;
        stick_vx = stick_vy = 0;
        return;
    }
    for (i = 0; i < EL_COUNT; i++)
        if (el[i].finger == id) {
            if (el[i].tap && inside(&el[i], x, y))
                port_pad_tap(PORT_PAD_FROM_TOUCH, el[i].button);   /* PauseButton.OnButtonReleased */
            el[i].finger = 0;
            el[i].pressed = 0;
        }
}

int port_touch_event(const void *event)
{
    const SDL_Event *e = (const SDL_Event *)event;
    float x, y;
    if (e->type != SDL_EVENT_FINGER_DOWN && e->type != SDL_EVENT_FINGER_MOTION && e->type != SDL_EVENT_FINGER_UP &&
        e->type != SDL_EVENT_FINGER_CANCELED)
        return 0;
    if (!overlay_active())
        return 1;
    x = e->tfinger.x * width;
    y = e->tfinger.y * height;
    if (e->type == SDL_EVENT_FINGER_DOWN)
        finger_down(e->tfinger.fingerID, x, y);
    else if (e->type == SDL_EVENT_FINGER_MOTION)
        finger_move(e->tfinger.fingerID, x, y);
    else
        finger_up(e->tfinger.fingerID, x, y);
    return 1;
}

void port_touch_update(int output_width, int output_height)
{
    int c, i;
    unsigned p;
    if (output_width > 0 && output_height > 0) {
        width = output_width;
        height = output_height;
    }
    c = port_pad_context(&p);
    if (!overlay_active()) {
        if (ctx != -1) {
            release_all();
            ctx = -1;
        }
        return;
    }
    if (c != ctx) {                     /* OnSetHUDContext: everything lets go */
        static const char *const names[] = {"menu", "worldmap", "gameplay", "moskito", "level-no-hud"};
        printf("Touch HUD context: %s (%dx%d)\n", c >= 0 && c < 5 ? names[c] : "?", width, height);
        release_all();
        ctx = c;
    }
    powers = p;
    layout();
    for (i = 0; i < EL_COUNT; i++)
        if (!el[i].tap)
            port_pad_set(PORT_PAD_FROM_TOUCH, el[i].button, 0);
    for (i = 0; i < EL_COUNT; i++)
        if (el[i].visible && el[i].pressed && !el[i].tap)
            port_pad_set(PORT_PAD_FROM_TOUCH, el[i].button, 1);
    /* Vignette, level start, fairy (HUD hidden, port's tap-to-key): a finger anywhere holds A. */
    if (ctx == PORT_CONTEXT_LEVEL_NO_HUD && fingers_down > 0)
        port_pad_set(PORT_PAD_FROM_TOUCH, PORT_PAD_A, 1);
    /* Stick to d-pad (RaymanGameManager.UpdateLoop): y < -0.40 down, else y > 0.50 up; x > 0.33 right, else
       x < -0.33 left. */
    port_pad_set(PORT_PAD_FROM_TOUCH, PORT_PAD_DOWN, stick_finger && stick_vy < -0.40f);
    port_pad_set(PORT_PAD_FROM_TOUCH, PORT_PAD_UP, stick_finger && stick_vy >= -0.40f && stick_vy > 0.50f);
    port_pad_set(PORT_PAD_FROM_TOUCH, PORT_PAD_RIGHT, stick_finger && stick_vx > 0.33f);
    port_pad_set(PORT_PAD_FROM_TOUCH, PORT_PAD_LEFT, stick_finger && stick_vx <= 0.33f && stick_vx < -0.33f);
}

/* A fingerprint of what the overlay shows; the presenter redraws the last frame when it changes. */
static uint64_t drawn_state = ~0ULL;

static uint64_t overlay_state(void)
{
    uint64_t h = 1469598103934665603ULL;
    int i;
#define MIXV(v) h = (h ^ (uint64_t)(v)) * 1099511628211ULL
    MIXV(overlay_active());
    MIXV(ctx + 1);
    MIXV(width);
    MIXV(height);
    MIXV(stick_finger != 0);
    MIXV((int)(stick_vx * 64));
    MIXV((int)(stick_vy * 64));
    MIXV((int)(stick_ox * 512));
    MIXV((int)(stick_oy * 512));
    for (i = 0; i < EL_COUNT; i++) {
        MIXV(el[i].visible);
        MIXV(el[i].pressed);
        MIXV((uintptr_t)el[i].label);
    }
#undef MIXV
    return h;
}

int port_touch_needs_redraw(void)
{
    return enabled && overlay_state() != drawn_state;
}

/* ---- drawing ------------------------------------------------------------------------------------------- */
static void disc(SDL_Renderer *r, float cx, float cy, float rad, SDL_FColor col)
{
    enum { N = 40 };
    SDL_Vertex v[N + 1];
    int idx[N * 3], i;
    v[0].position.x = cx;
    v[0].position.y = cy;
    v[0].color = col;
    for (i = 0; i < N; i++) {
        float a = (float)i * 6.2831853f / N;
        v[i + 1].position.x = cx + rad * cosf(a);
        v[i + 1].position.y = cy + rad * sinf(a);
        v[i + 1].color = col;
        idx[3 * i] = 0;
        idx[3 * i + 1] = 1 + i;
        idx[3 * i + 2] = 1 + (i + 1) % N;
    }
    SDL_RenderGeometry(r, NULL, v, N + 1, idx, N * 3);
}

static void ring(SDL_Renderer *r, float cx, float cy, float rad, float thick, SDL_FColor col)
{
    enum { N = 48 };
    SDL_Vertex v[2 * N];
    int idx[N * 6], i;
    for (i = 0; i < N; i++) {
        float a = (float)i * 6.2831853f / N, c = cosf(a), s = sinf(a);
        v[2 * i].position.x = cx + rad * c;
        v[2 * i].position.y = cy + rad * s;
        v[2 * i + 1].position.x = cx + (rad - thick) * c;
        v[2 * i + 1].position.y = cy + (rad - thick) * s;
        v[2 * i].color = v[2 * i + 1].color = col;
    }
    for (i = 0; i < N; i++) {
        int a = 2 * i, b = 2 * ((i + 1) % N);
        idx[6 * i] = a; idx[6 * i + 1] = a + 1; idx[6 * i + 2] = b;
        idx[6 * i + 3] = b; idx[6 * i + 4] = a + 1; idx[6 * i + 5] = b + 1;
    }
    SDL_RenderGeometry(r, NULL, v, 2 * N, idx, N * 6);
}

static void label(SDL_Renderer *r, float cx, float cy, const char *text, float scale, float alpha)
{
    float w = (float)strlen(text) * SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * scale;
    SDL_SetRenderScale(r, scale, scale);
    SDL_SetRenderDrawColor(r, 255, 255, 255, (Uint8)(alpha * 255));
    SDL_RenderDebugText(r, (cx - w / 2) / scale, (cy - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * scale / 2) / scale, text);
    SDL_SetRenderScale(r, 1.0f, 1.0f);
}

static SDL_FColor white(float a)
{
    SDL_FColor c = {1.0f, 1.0f, 1.0f, a};
    return c;
}

void port_touch_draw(void *renderer)
{
    SDL_Renderer *r = (SDL_Renderer *)renderer;
    float unit = (float)width * 0.5f;   /* one GUI x unit in pixels */
    int i;
    drawn_state = overlay_state();
    if (!overlay_active() || ctx < 0)
        return;
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    if (is_level(ctx)) {
        /* Stick (PakVisualJoystick): base and knob, alpha 0.80 in use, 0.10 idle; idle base at (-0.69, -0.6). */
        float a = stick_finger ? 0.80f : 0.10f;
        float bx = px_of(stick_finger ? stick_ox : CROSS_X), by = py_of(stick_finger ? stick_oy : CROSS_Y);
        float rad = STICK_RADIUS * unit;
        ring(r, bx, by, rad * 1.6f, rad * 0.12f, white(a));
        disc(r, bx + stick_vx * rad, by - stick_vy * rad, rad * 0.75f, white(a * 0.7f));
    } else if (has_cross(ctx)) {
        /* Cross (DPadVisual): the arm of the rounded direction opaque, the others alpha 0.5. */
        float cx = px_of(CROSS_X), cy = py_of(CROSS_Y), arm = 0.11f * height;
        int dx = 0, dy = 0;
        static const int dirs[4][2] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};
        if (stick_finger) {
            dx = (int)lroundf(stick_vx);
            dy = (int)lroundf(stick_vy);
            if (dx) dy = 0;
        }
        disc(r, cx, cy, arm * 0.45f, white(0.25f));
        for (i = 0; i < 4; i++) {
            float a = (dirs[i][0] == dx && dirs[i][1] == dy && stick_finger) ? 1.0f : 0.5f;
            disc(r, cx + dirs[i][0] * arm, cy - dirs[i][1] * arm, arm * 0.45f, white(a * 0.6f));
        }
    }
    for (i = 0; i < EL_COUNT; i++) {
        const element *e = &el[i];
        float a;
        if (!e->visible)
            continue;
        /* RaymanActionButton: idle alpha 0.7, pressed 1.0; Pause idle 0.4. */
        a = e->pressed ? 1.0f : (i == EL_PAUSE ? 0.4f : 0.7f);
        disc(r, e->cx, e->cy, e->half, white(a * 0.30f));
        ring(r, e->cx, e->cy, e->half, e->half * 0.08f, white(a));
        label(r, e->cx, e->cy, e->label, e->half / 14.0f, a);
    }
}
