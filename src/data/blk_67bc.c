/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "rtypes.h"

extern u8 TestKeyWeapon(void);
extern u8 TestKeyJump(void);
extern u8 TestKeyAction(void);

u8 (*TEST_KEY_BUTTON0)(void) = TestKeyWeapon; //967BC
u8 (*TEST_KEY_BUTTON1)(void) = TestKeyJump; //967C0
u8 (*TEST_KEY_BUTTON2)(void) = TestKeyAction; //967C4
u8 (*TEST_KEY_BUTTON3)(void) = TestKeyAction; //967C8
