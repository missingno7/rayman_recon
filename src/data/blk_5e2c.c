/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

enum {
    MODE_TEXT = 255,
    SC_RIGHT = 0x4D, SC_UP = 0x48, SC_LEFT = 0x4B, SC_DOWN = 0x50,
    SC_ALT = 0x38, SC_CONTROL = 0x1D, SC_X = 0x2D
};

u8 ModeVideoActuel = MODE_TEXT; //95E2C
u8 GameModeVideo = MODE_TEXT; //95E2D
s32 JumAmpDefMul = 0; //95E30
s32 JumAmpDefPlus = 0; //95E34
u8 key_right = SC_RIGHT; //95E38
u8 key_up = SC_UP; //95E39
u8 key_left = SC_LEFT; //95E3A
u8 key_down = SC_DOWN; //95E3B
u8 key_fist = SC_ALT; //95E3C
u8 key_jump = SC_CONTROL; //95E3D
u8 key_action = SC_X; //95E3E
u8* tab_key[7] = {&key_left, &key_up, &key_right, &key_down, &key_jump, &key_fist, &key_action}; //95E40
u8 tab_key_sav[7] = {SC_LEFT, SC_UP, SC_RIGHT, SC_DOWN, SC_CONTROL, SC_ALT, SC_X}; //95E5C
u8 OptionGame = 0; //95E63
u8 GoMenu = 0; //95E64
u16 fixon = 1; //95E66      // scores enabled
u16 fixontemp = 0; //95E68
u16 SizeScreen = 0; //95E6A
u8 Frequence = 0; //95E6C
u8 BackgroundOptionOn = 1; //95E6D
u8 BackgroundOn = 1; //95E6E

u16 tRefRam2VramNormalFix[8] = {255, 255, 255, 255, 255, 255, 255, 255}; //95E70
u16 tRefRam2VramNormal[8] = {255, 255, 255, 255, 255, 255, 255, 255}; //95E80
u16 tRefTransFondNormal[8] = {255, 255, 255, 255, 255, 255, 255, 255}; //95E90
u16 tRefSpriteNormal[2] = {255, 255}; //95EA0
u16 tRefRam2VramX[2] = {255, 255}; //95EA4
u16 tRefVram2VramX[2] = {255, 255}; //95EA8
u16 tRefSpriteX[2] = {255, 255}; //95EAC
u8 ExitMenu = 0; //95EB0
u8 ReInitPlasma = 0; //95EB1
u8 Main_Control = 0; //95EB2
s16 During_The_Menu = 1; //95EB4
u8 OptionMusicCdActive = 1; //95EB6
u8 text_input_buffer[9] = {0}; //95EB7
u8 PC_TingsToGet_Col = 1; //95EC0
u8 PC_TingsToGet_ColAdd = 1; //95EC1
