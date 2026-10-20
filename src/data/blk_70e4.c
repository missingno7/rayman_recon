/* Part of the central data TU (GOG v1.21 obj2 _DATA). Block file and split INVENTED; values verified against the
   oracle by tools/verify.py (data segment check). From build/workers/data_rv (Rayverse data.c + DOS corrections). */
#include "obj.h"

typedef struct cptr_t {
    u8 (*read)(obj_t*);
    u8 (*skip)(obj_t*);
    u8 (*handle)(obj_t*);
} cptr_t;

extern u8 handle_10_GO_DOLOOP(obj_t*);
extern u8 handle_11_GO_LABEL(obj_t*);
extern u8 handle_12_GO_GOTO(obj_t*);
extern u8 handle_13_GO_GOSUB(obj_t*);
extern u8 handle_14_GO_RETURN(obj_t*);
extern u8 handle_15_GO_BRANCHTRUE(obj_t*);
extern u8 handle_16_GO_BRANCHFALSE(obj_t*);
extern u8 handle_17_GO_TEST(obj_t*);
extern u8 handle_18_GO_SETTEST(obj_t*);
extern u8 handle_19_GO_WAITSTATE(obj_t*);
extern u8 handle_21_GO_X(obj_t*);
extern u8 handle_22_GO_Y(obj_t*);
extern u8 handle_25_RESERVED_GO_GOSUB(obj_t*);
extern u8 handle_26_RESERVED_GO_BRANCHTRUE(obj_t*);
extern u8 handle_27_RESERVED_GO_BRANCHFALSE(obj_t*);
extern u8 handle_28_RESERVED_GO_SKIPTRUE(obj_t*);
extern u8 handle_29_RESERVED_GO_SKIPFALSE(obj_t*);
extern u8 handle_31_GO_SKIPTRUE(obj_t*);
extern u8 handle_32_GO_SKIPFALSE(obj_t*);
extern u8 handle_33_INVALID_CMD(obj_t*);
extern u8 handle_5_GO_SUBSTATE(obj_t*);
extern u8 handle_6_GO_SKIP(obj_t*);
extern u8 handle_8_GO_STATE(obj_t*);
extern u8 handle_9_GO_PREPARELOOP(obj_t*);
extern u8 handle_RESERVED_GO_SKIP_and_RESERVED_GO_GOTO(obj_t*);
extern u8 handle_SELF_HANDLED(obj_t*);
extern u8 readGoXYargs(obj_t*);
extern u8 readInvalidArg(obj_t*);
extern u8 readNoArg(obj_t*);
extern u8 readOneArg(obj_t*);
extern u8 readSpeedArgs(obj_t*);
extern u8 readTestArgs(obj_t*);
extern u8 skipGoXYArgs(obj_t*);
extern u8 skipInvalidArg(obj_t*);
extern u8 skipNoArg(obj_t*);
extern u8 skipOneArg(obj_t*);
extern u8 skipSpeedArgs(obj_t*);
extern u8 skipTestArgs(obj_t*);

cptr_t cptr_tab[] = {
        {readOneArg, skipOneArg, handle_SELF_HANDLED}, // 0
        {readOneArg, skipOneArg, handle_SELF_HANDLED}, // 1
        {readOneArg, skipOneArg, handle_SELF_HANDLED}, // 2
        {readOneArg, skipOneArg, handle_SELF_HANDLED}, // 3
        {readOneArg, skipOneArg, handle_SELF_HANDLED}, // 4
        {readOneArg, skipOneArg, handle_5_GO_SUBSTATE}, // 5
        {readOneArg, skipOneArg, handle_6_GO_SKIP}, // 6
        {readOneArg, skipOneArg, handle_SELF_HANDLED}, // 7
        {readOneArg, skipOneArg, handle_8_GO_STATE}, // 8
        {readOneArg, skipOneArg, handle_9_GO_PREPARELOOP}, // 9
        {readNoArg, skipNoArg, handle_10_GO_DOLOOP}, // 10
        {readOneArg, skipOneArg, handle_11_GO_LABEL}, // 11
        {readOneArg, skipOneArg, handle_12_GO_GOTO}, // 12
        {readOneArg, skipOneArg, handle_13_GO_GOSUB}, // 13
        {readNoArg, skipNoArg, handle_14_GO_RETURN}, // 14
        {readOneArg, skipOneArg, handle_15_GO_BRANCHTRUE}, // 15
        {readOneArg, skipOneArg, handle_16_GO_BRANCHFALSE}, // 16
        {readTestArgs, skipTestArgs, handle_17_GO_TEST}, // 17
        {readOneArg, skipOneArg, handle_18_GO_SETTEST}, // 18
        {readOneArg, skipOneArg, handle_19_GO_WAITSTATE}, // 19
        {readSpeedArgs, skipSpeedArgs, handle_SELF_HANDLED}, // 20
        {readGoXYargs, skipGoXYArgs, handle_21_GO_X}, // 21
        {readGoXYargs, skipGoXYArgs, handle_22_GO_Y}, // 22
        {readOneArg, skipOneArg, handle_RESERVED_GO_SKIP_and_RESERVED_GO_GOTO}, // 23
        {readOneArg, skipOneArg, handle_RESERVED_GO_SKIP_and_RESERVED_GO_GOTO}, // 24
        {readOneArg, skipOneArg, handle_25_RESERVED_GO_GOSUB}, // 25
        {readOneArg, skipOneArg, handle_26_RESERVED_GO_BRANCHTRUE}, // 26
        {readOneArg, skipOneArg, handle_27_RESERVED_GO_BRANCHFALSE}, // 27
        {readOneArg, skipOneArg, handle_28_RESERVED_GO_SKIPTRUE}, // 28
        {readOneArg, skipOneArg, handle_29_RESERVED_GO_SKIPFALSE}, // 29
        {readOneArg, skipOneArg, handle_SELF_HANDLED}, // 30
        {readOneArg, skipOneArg, handle_31_GO_SKIPTRUE}, // 31
        {readOneArg, skipOneArg, handle_32_GO_SKIPFALSE}, // 32
        {readInvalidArg, skipInvalidArg, handle_33_INVALID_CMD}, // 33
};
