/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

enum dos_scancode_enum {
	SC_ESCAPE       = 0x01,
	SC_1            = 0x02,
	SC_2            = 0x03,
	SC_3            = 0x04,
	SC_4            = 0x05,
	SC_5            = 0x06,
	SC_6            = 0x07,
	SC_7            = 0x08,
	SC_8            = 0x09,
	SC_9            = 0x0A,
	SC_0            = 0x0B,
	SC_MINUS        = 0x0C,
	SC_EQUALS       = 0x0D,
	SC_BACKSPACE    = 0x0E,
	SC_TAB          = 0x0F,
	SC_Q            = 0x10,
	SC_W            = 0x11,
	SC_E            = 0x12,
	SC_R            = 0x13,
	SC_T            = 0x14,
	SC_Y            = 0x15,
	SC_U            = 0x16,
	SC_I            = 0x17,
	SC_O            = 0x18,
	SC_P            = 0x19,
	SC_LEFTBRACKET  = 0x1A,
	SC_RIGHTBRACKET = 0x1B,
	SC_ENTER        = 0x1C,
	SC_CONTROL      = 0x1D,
	SC_A            = 0x1E,
	SC_S            = 0x1F,
	SC_D            = 0x20,
	SC_F            = 0x21,
	SC_G            = 0x22,
	SC_H            = 0x23,
	SC_J            = 0x24,
	SC_K            = 0x25,
	SC_L            = 0x26,
	SC_SEMICOLON    = 0x27,
	SC_QUOTE        = 0x28,
	SC_TILDE        = 0x29,
	SC_LSHIFT       = 0x2A,
	SC_BACKSLASH    = 0x2B,
	SC_Z            = 0x2C,
	SC_X            = 0x2D,
	SC_C            = 0x2E,
	SC_V            = 0x2F,
	SC_B            = 0x30,
	SC_N            = 0x31,
	SC_M            = 0x32,
	SC_COMMA        = 0x33,
	SC_PERIOD       = 0x34,
	SC_SLASH        = 0x35,
	SC_RSHIFT       = 0x36,
	SC_MULTIPLY     = 0x37,
	SC_ALT          = 0x38,
	SC_SPACE        = 0x39,
	SC_CAPSLOCK     = 0x3A,
	SC_F1           = 0x3B,
	SC_F2           = 0x3C,
	SC_F3           = 0x3D,
	SC_F4           = 0x3E,
	SC_F5           = 0x3F,
	SC_F6           = 0x40,
	SC_F7           = 0x41,
	SC_F8           = 0x42,
	SC_F9           = 0x43,
	SC_F10          = 0x44,
	SC_NUMLOCK      = 0x45,
	SC_SCROLLLOCK   = 0x46,
	SC_HOME         = 0x47,
	SC_UP           = 0x48,
	SC_PAGEUP       = 0x49,
	SC_LEFT         = 0x4B,
	SC_RIGHT        = 0x4D,
	SC_PLUS         = 0x4E,
	SC_END          = 0x4F,
	SC_DOWN         = 0x50,
	SC_PAGEDOWN     = 0x51,
	SC_INSERT       = 0x52,
	SC_DELETE       = 0x53,
	SC_F11          = 0x57,
	SC_F12          = 0x58,
};

u8 cheat_codes[325] = {
#define _(x) x^0x96
        _(SC_BACKSPACE), _(SC_L), _(SC_Q), _(SC_Y), _(SC_Q), _(SC_R), _(SC_B), _(SC_C), _(SC_TAB),
        _(SC_BACKSPACE), _(SC_L), _(SC_A), _(SC_Y), _(SC_A), _(SC_R), _(SC_B), _(SC_C), _(SC_TAB), // [TAB]cbrayal[BACKSPACE] (enter breakout before defeating Mr Dark)

        _(SC_BACKSPACE), _(SC_L), _(SC_E), _(SC_V), _(SC_E), _(SC_L), _(SC_Q), _(SC_TAB),
        _(SC_BACKSPACE), _(SC_L), _(SC_E), _(SC_V), _(SC_E), _(SC_L), _(SC_A), _(SC_TAB), // [TAB]alevel[BACKSPACE] (level select)

        _(SC_S), _(SC_N), _(SC_E), _(SC_L),
        _(SC_S), _(SC_N), _(SC_E), _(SC_L), // lens (lens effect)

        _(SC_0), _(SC_1), _(SC_A), _(SC_E), _(SC_R), _(SC_F),
        _(SC_0), _(SC_1), _(SC_Q), _(SC_E), _(SC_R), _(SC_F), // freq10 (100 Hz refresh rate)

        _(SC_0), _(SC_8), _(SC_A), _(SC_E), _(SC_R), _(SC_F),
        _(SC_0), _(SC_8), _(SC_Q), _(SC_E), _(SC_R), _(SC_F), // freq80 (80 Hz refresh rate)

        _(SC_BACKSPACE), _(SC_Y), _(SC_Q), _(SC_R), _(SC_E), _(SC_V), _(SC_O), 0xb1, _(SC_TAB),
        _(SC_BACKSPACE), _(SC_Y), _(SC_A), _(SC_R), _(SC_E), _(SC_V), _(SC_O), 0xb1, _(SC_TAB), // [TAB];overay[BACKSPACE] (toggle free movement)

        _(SC_Y), _(SC_Q), _(SC_R), _(SC_B), _(SC_C),
        _(SC_Y), _(SC_A), _(SC_R), _(SC_B), _(SC_C), // cbray (enter breakout minigame after defeating Mr Dark)

        _(SC_D), _(SC_L), _(SC_R), _(SC_O), _(SC_Z), _(SC_L), _(SC_Q),
        _(SC_D), _(SC_L), _(SC_R), _(SC_O), _(SC_W), _(SC_L), _(SC_A), // alworld (unlock all levels)

        _(SC_S), _(SC_E), _(SC_V), _(SC_I), _(SC_L), _(SC_Y), _(SC_Q), _(SC_R),
        _(SC_S), _(SC_E), _(SC_V), _(SC_I), _(SC_L), _(SC_Y), _(SC_A), _(SC_R), // raylives (99 lives)

        _(SC_R), _(SC_E), _(SC_Z), _(SC_O), _(SC_P),
        _(SC_R), _(SC_E), _(SC_W), _(SC_O), _(SC_P), // power (all powers)

        _(SC_T), _(SC_N), _(SC_I), _(SC_O), _(SC_P), _(SC_Y), _(SC_Q), _(SC_R),
        _(SC_T), _(SC_N), _(SC_I), _(SC_O), _(SC_P), _(SC_Y), _(SC_A), _(SC_R), // raypoint (full health)

        _(SC_T), _(SC_S), _(SC_I), _(SC_F), _(SC_D), _(SC_L), _(SC_O), _(SC_G),
        _(SC_T), _(SC_S), _(SC_I), _(SC_F), _(SC_D), _(SC_L), _(SC_O), _(SC_G), //goldfist (all fist powerups)

        _(SC_W), _(SC_I), _(SC_Z), _(SC_Y), _(SC_Q), _(SC_R),
        _(SC_Z), _(SC_I), _(SC_W), _(SC_Y), _(SC_A), _(SC_R), //raywiz (10 tings)

        _(SC_P), _(SC_Q), 0xb1, _(SC_N), _(SC_I), _(SC_Z),
        _(SC_P), _(SC_A), _(SC_M), _(SC_N), _(SC_I), _(SC_W), //winmap (skip level)

        _(SC_R), _(SC_E), _(SC_I), _(SC_V), _(SC_I), _(SC_L), _(SC_O), _(SC_TAB),
        _(SC_R), _(SC_E), _(SC_I), _(SC_V), _(SC_I), _(SC_L), _(SC_O), _(SC_TAB), //[TAB]olivier (enter breakout after defeating Mr Dark)

        _(SC_S), _(SC_I), _(SC_O), _(SC_C), _(SC_N), _(SC_Q), _(SC_R), _(SC_F), _(SC_TAB),
        _(SC_S), _(SC_I), _(SC_O), _(SC_C), _(SC_N), _(SC_A), _(SC_R), _(SC_F), _(SC_TAB), //[TAB]francois (unlock all levels)

        _(SC_D), _(SC_N), _(SC_Q), _(SC_R), _(SC_T), _(SC_R), _(SC_E), _(SC_B), _(SC_TAB),
        _(SC_D), _(SC_N), _(SC_A), _(SC_R), _(SC_T), _(SC_R), _(SC_E), _(SC_B), _(SC_TAB), //[TAB]bertrand (99 lives)

        _(SC_T), _(SC_I), _(SC_O), _(SC_N), _(SC_E), _(SC_B), _(SC_TAB),
        _(SC_T), _(SC_I), _(SC_O), _(SC_N), _(SC_E), _(SC_B), _(SC_TAB), //[TAB]benoit (all powers)

        _(SC_T), _(SC_S), _(SC_I), _(SC_R), _(SC_H), _(SC_C), _(SC_TAB),
        _(SC_T), _(SC_S), _(SC_I), _(SC_R), _(SC_H), _(SC_C), _(SC_TAB), //[TAB]christ (full health)

        _(SC_E), _(SC_N), _(SC_I), _(SC_O), _(SC_T), _(SC_N), _(SC_Q), _(SC_TAB),
        _(SC_E), _(SC_N), _(SC_I), _(SC_O), _(SC_T), _(SC_N), _(SC_A), _(SC_TAB), //[TAB]antoine (all fist powerups)

        _(SC_O), _(SC_J), _(SC_O), _(SC_J), _(SC_TAB),
        _(SC_O), _(SC_J), _(SC_O), _(SC_J), _(SC_TAB), //[TAB]jojo (10 tings)

        _(SC_L), _(SC_I), _(SC_R), _(SC_Y), _(SC_C), _(SC_TAB),
        _(SC_L), _(SC_I), _(SC_R), _(SC_Y), _(SC_C), _(SC_TAB), // [TAB]cyril (skip level)

        0xb1, _(SC_Q), _(SC_R), _(SC_G), _(SC_O), _(SC_R), _(SC_P), _(SC_TAB),
        _(SC_M), _(SC_A), _(SC_R), _(SC_G), _(SC_O), _(SC_R), _(SC_P), _(SC_TAB), // [TAB]program (hidden message)

        0,
#undef _
};


u8 effect_choice = 0; //964C9
u8 byte_964CA = 1; //964CA
u16 effect_speed = 0; //964CC
u8 effect_pause_bloque = 0; //964CE
