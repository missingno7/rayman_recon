/*
 * Virtual pad: SDL gamepad + touch overlay -> pad buttons -> the game's keys (platform/pad.h).
 *
 * Mapping (port/docs/TOUCH_CONTROLS.md "Native button ids"): the Android release's native layer writes fixed keys
 * (A Space, B Enter, X X, Back Esc, arrows) and its own menus used the pad directly. The port runs the DOS menus,
 * so it maps by context:
 *   levels:            A key_jump, B key_fist, X key_action, Start/Back Esc (pause), d-pad key_up/down/left/right
 *   world map:         A Space (validate), Start Enter, Back Esc, d-pad as in levels
 *   DOS menus/screens: A Enter, B Esc, Start Enter, Back Esc, X Space, d-pad as in levels (tap-to-key)
 * Using the game's configured keys keeps the mapping right when the player redefines them in the options.
 */
#include "platform/pad.h"
#include "platform/host.h"

#include <SDL3/SDL.h>
#include <stdio.h>
#include <string.h>

#define SC_ESC 0x01
#define SC_ENTER 0x1c
#define SC_SPACE 0x39
#define TAP_NS 80000000ULL   /* longer than the game's 1/60 s frame: the press is always seen */

static uint8_t held[PORT_PAD_SOURCES][PORT_PAD_BUTTONS];
static uint64_t tap_until[PORT_PAD_SOURCES][PORT_PAD_BUTTONS];
static uint8_t injected[128];
/* A press stays down at least TAP_NS: a short tap would otherwise make and break between two of the game's
   once-per-frame key reads and never be seen. */
static uint8_t was_on[PORT_PAD_BUTTONS];
static uint64_t min_until[PORT_PAD_BUTTONS];
static int context = PORT_CONTEXT_MENU;
static unsigned powers;
static SDL_Gamepad *gamepad;
static SDL_JoystickID gamepad_id;
static int16_t axis_x, axis_y;
/* The pad has been used (a button or the stick moved past 0.5). SDL also reports keyboards with d-pad keys as
   gamepads (the emulator's qwerty2); the touch overlay is hidden only for a pad the player actually uses. */
static int gamepad_used;

void port_pad_init(void)
{
    SDL_InitSubSystem(SDL_INIT_GAMEPAD);
}

static int map_button(SDL_GamepadButton b)
{
    switch (b) {
    case SDL_GAMEPAD_BUTTON_SOUTH: return PORT_PAD_A;
    case SDL_GAMEPAD_BUTTON_EAST: return PORT_PAD_B;
    case SDL_GAMEPAD_BUTTON_WEST: return PORT_PAD_X;
    case SDL_GAMEPAD_BUTTON_NORTH: return PORT_PAD_Y;
    case SDL_GAMEPAD_BUTTON_START: return PORT_PAD_START;
    case SDL_GAMEPAD_BUTTON_BACK: return PORT_PAD_BACK;
    case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER: return PORT_PAD_LB;
    case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER: return PORT_PAD_RB;
    case SDL_GAMEPAD_BUTTON_LEFT_STICK: return PORT_PAD_L3;
    case SDL_GAMEPAD_BUTTON_RIGHT_STICK: return PORT_PAD_R3;
    case SDL_GAMEPAD_BUTTON_DPAD_UP: return PORT_PAD_UP;
    case SDL_GAMEPAD_BUTTON_DPAD_DOWN: return PORT_PAD_DOWN;
    case SDL_GAMEPAD_BUTTON_DPAD_LEFT: return PORT_PAD_LEFT;
    case SDL_GAMEPAD_BUTTON_DPAD_RIGHT: return PORT_PAD_RIGHT;
    default: return 0;
    }
}

int port_pad_event(const void *event)
{
    const SDL_Event *e = (const SDL_Event *)event;
    int b;
    switch (e->type) {
    case SDL_EVENT_GAMEPAD_ADDED:
        if (!gamepad && (gamepad = SDL_OpenGamepad(e->gdevice.which)) != NULL) {
            gamepad_id = e->gdevice.which;
            printf("Gamepad connected: %s\n", SDL_GetGamepadName(gamepad));
        }
        return 1;
    case SDL_EVENT_GAMEPAD_REMOVED:
        if (gamepad && e->gdevice.which == gamepad_id) {
            printf("Gamepad disconnected\n");
            SDL_CloseGamepad(gamepad);
            gamepad = NULL;
            gamepad_used = 0;
            axis_x = axis_y = 0;
            port_pad_clear(PORT_PAD_FROM_GAMEPAD);
        }
        return 1;
    case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
    case SDL_EVENT_GAMEPAD_BUTTON_UP:
        if (gamepad && e->gbutton.which == gamepad_id && (b = map_button((SDL_GamepadButton)e->gbutton.button)) != 0)
        {
            held[PORT_PAD_FROM_GAMEPAD][b] = e->gbutton.down;
            gamepad_used = 1;
        }
        return 1;
    case SDL_EVENT_GAMEPAD_AXIS_MOTION:
        if (gamepad && e->gaxis.which == gamepad_id) {
            if (e->gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX) axis_x = e->gaxis.value;
            if (e->gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY) axis_y = e->gaxis.value;
            if (e->gaxis.value > 16384 || e->gaxis.value < -16384)
                gamepad_used = 1;
        }
        return 1;
    default:
        return 0;
    }
}

int port_pad_gamepad_connected(void) { return gamepad != NULL && gamepad_used; }

void port_pad_set(int source, int button, int on)
{
    if (source >= 0 && source < PORT_PAD_SOURCES && button > 0 && button < PORT_PAD_BUTTONS)
        held[source][button] = (uint8_t)(on != 0);
}

void port_pad_clear(int source)
{
    memset(held[source], 0, sizeof held[source]);
}

void port_pad_tap(int source, int button)
{
    if (source >= 0 && source < PORT_PAD_SOURCES && button > 0 && button < PORT_PAD_BUTTONS)
        tap_until[source][button] = SDL_GetTicksNS() + TAP_NS;
}

int port_pad_context(unsigned *p)
{
    if (p) *p = powers;
    return context;
}

static int is_level(int c)
{
    return c == PORT_CONTEXT_GAMEPLAY || c == PORT_CONTEXT_MOSKITO || c == PORT_CONTEXT_LEVEL_NO_HUD;
}

/* Scan code a held pad button stands for in this context; 0 = none. */
static uint8_t key_for(int button, const uint8_t keys[PORT_KEY_COUNT])
{
    switch (button) {
    case PORT_PAD_UP: return keys[PORT_KEY_UP];
    case PORT_PAD_DOWN: return keys[PORT_KEY_DOWN];
    case PORT_PAD_LEFT: return keys[PORT_KEY_LEFT];
    case PORT_PAD_RIGHT: return keys[PORT_KEY_RIGHT];
    case PORT_PAD_BACK: return SC_ESC;
    default: break;
    }
    if (is_level(context)) {
        switch (button) {
        case PORT_PAD_A: return keys[PORT_KEY_JUMP];
        case PORT_PAD_B: return keys[PORT_KEY_FIST];
        case PORT_PAD_X: return keys[PORT_KEY_ACTION];
        case PORT_PAD_START: return SC_ESC;
        default: return 0;
        }
    }
    if (context == PORT_CONTEXT_WORLDMAP) {
        switch (button) {
        case PORT_PAD_A: return SC_SPACE;
        case PORT_PAD_START: return SC_ENTER;
        default: return 0;
        }
    }
    switch (button) {   /* DOS menus and screens */
    case PORT_PAD_A: return SC_ENTER;
    case PORT_PAD_B: return SC_ESC;
    case PORT_PAD_X: return SC_SPACE;
    case PORT_PAD_START: return SC_ENTER;
    default: return 0;
    }
}

void port_pad_update(void)
{
    uint8_t keys[PORT_KEY_COUNT], want[128];
    uint8_t on[PORT_PAD_BUTTONS];
    uint64_t now = SDL_GetTicksNS();
    int s, b, k;
    context = port_game_input_context(keys, &powers);
    memset(on, 0, sizeof on);
    for (s = 0; s < PORT_PAD_SOURCES; s++)
        for (b = 1; b < PORT_PAD_BUTTONS; b++)
            if (held[s][b] || tap_until[s][b] > now)
                on[b] = 1;
    /* Left stick (RaymanGameManager.UpdateLoop: y < -0.40 down, y > 0.50 up, |x| > 0.25; SDL's y grows down). */
    if (axis_y > (int16_t)(0.40f * 32767)) on[PORT_PAD_DOWN] = 1;
    else if (axis_y < (int16_t)(-0.50f * 32767)) on[PORT_PAD_UP] = 1;
    if (axis_x > (int16_t)(0.25f * 32767)) on[PORT_PAD_RIGHT] = 1;
    else if (axis_x < (int16_t)(-0.25f * 32767)) on[PORT_PAD_LEFT] = 1;
    for (b = 1; b < PORT_PAD_BUTTONS; b++) {
        if (on[b] && !was_on[b])
            min_until[b] = now + TAP_NS;
        was_on[b] = on[b];
        if (min_until[b] > now)
            on[b] = 1;
    }
    memset(want, 0, sizeof want);
    for (b = 1; b < PORT_PAD_BUTTONS; b++) {
        uint8_t code = on[b] ? key_for(b, keys) : 0;
        if (code && code < 128)
            want[code] = 1;
    }
    for (k = 1; k < 128; k++)
        if (want[k] != injected[k]) {
            port_host_inject_scancode((uint8_t)(want[k] ? k : (k | 0x80)));
            injected[k] = want[k];
        }
}
