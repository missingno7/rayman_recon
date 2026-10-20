/* objupdate.c, part 1 (1:50ad0..1:512f0): object command reader/skipper/handlers.
   Names RAYVERSE (objupdate.c); bodies seeded from PS1/Rayverse by tools/seed.py, finished by worker tu_objcmd. */
#include "obj.h"
#include "game.h"
#include "map.h"

typedef u8 (*cmd_fn_t)(obj_t *obj);
typedef struct cptr_t {
    cmd_fn_t read;
    cmd_fn_t skip;
    cmd_fn_t handle;
} cptr_t;
extern cptr_t cptr_tab[34];

#define ANIMMODE_RESET_IF_NEW 1
#define GO_LABEL 11
#define GO_NOP 30
#define GO_WAIT 2
s16 OBJ_IN_ZONE(obj_t* obj);
void calc_obj_dir(obj_t* obj);
short myRand(short max);
void set_main_etat(obj_t* obj, u8 etat);
void set_sub_etat(obj_t* obj, u8 sub_etat);
s32 vblToEOA(obj_t* obj, u8 a2);
void popCmdContext(obj_t* obj);
s16 char2short(u8 x);
u8 readNoArg(obj_t *obj);
u8 skipNoArg(obj_t *obj);
u8 readOneArg(obj_t *obj);
u8 readTestArgs(obj_t *obj);
u8 readGoXYargs(obj_t* obj);
u8 readSpeedArgs(obj_t *obj);
u8 readInvalidArg(obj_t *obj);
u8 readOneCommand(obj_t *obj);
u8 skipOneCommand(obj_t *obj);
void GET_OBJ_CMD(obj_t *obj);
u8 skipOneArg(obj_t *obj);
u8 skipTestArgs(obj_t *obj);
u8 skipGoXYArgs(obj_t *obj);
u8 skipSpeedArgs(obj_t *obj);
u8 handle_SELF_HANDLED(obj_t *obj);
u8 skipInvalidArg(obj_t* obj);
u8 handle_11_GO_LABEL(obj_t* obj);
u8 handle_19_GO_WAITSTATE(obj_t* obj);
u8 handle_25_RESERVED_GO_GOSUB(obj_t* obj);
u8 handle_RESERVED_GO_SKIP_and_RESERVED_GO_GOTO(obj_t *obj);
u8 handle_26_RESERVED_GO_BRANCHTRUE(obj_t* obj);
u8 handle_27_RESERVED_GO_BRANCHFALSE(obj_t* obj);
u8 handle_28_RESERVED_GO_SKIPTRUE(obj_t* obj);
u8 handle_29_RESERVED_GO_SKIPFALSE(obj_t* obj);
u8 handle_21_GO_X(obj_t* obj);
u8 handle_22_GO_Y(obj_t* obj);
u8 handle_12_GO_GOTO(obj_t* obj);
u8 handle_8_GO_STATE(obj_t* obj);
u8 handle_5_GO_SUBSTATE(obj_t* obj);
u8 handle_9_GO_PREPARELOOP(obj_t* obj);
u8 handle_13_GO_GOSUB(obj_t* obj);
u8 handle_14_GO_RETURN(obj_t* obj);
u8 handle_33_INVALID_CMD(obj_t* obj);
u8 handle_15_GO_BRANCHTRUE(obj_t* obj);
u8 handle_16_GO_BRANCHFALSE(obj_t* obj);
u8 handle_18_GO_SETTEST(obj_t* obj);
u8 handle_17_GO_TEST(obj_t* obj);
void pushCmdContext(obj_t *obj, u8 count);
void skipToLabel(obj_t* obj, u8 label, u8 skip_label_cmd);
void pushToLabel(obj_t *obj, u8 label, u8 skip_label_cmd);

void popCmdContext(obj_t* obj) {
    obj->cmd_offset = obj->cmd_contexts[(u8)obj->cmd_context_depth].cmd_offset;
    --obj->cmd_context_depth;
}

s16 char2short(u8 x) {
    s16 result;

    if (x > 0x7F)
        result = -(256 - x);
    else
        result = x;

    return result;
}

u8 readNoArg(obj_t *obj)
{
    return 0;
}

u8 skipNoArg(obj_t *obj)
{
    return 0;
}

u8 readOneArg(obj_t *obj)
{
    obj->cmd_offset++;
    obj->nb_cmd = obj->cmds[obj->cmd_offset];
    return 0;
}

u8 readTestArgs(obj_t *obj)
{
    obj->nb_cmd = obj->cmds[++obj->cmd_offset];
    switch (obj->nb_cmd)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        obj->param = obj->cmds[++obj->cmd_offset];
        break;
    case 70:
    case 71:
    case 72:
        break;
    }

    return 0;
}

u8 readGoXYargs(obj_t* obj) {
    ++obj->cmd_offset;
    obj->nb_cmd = char2short(obj->cmds[obj->cmd_offset]);

    ++obj->cmd_offset;
    obj->link = char2short(obj->cmds[obj->cmd_offset]);
    return 0;
}

u8 readSpeedArgs(obj_t *obj)
{
    obj->cmd_offset++;
    obj->nb_cmd = obj->cmds[obj->cmd_offset];

    obj->cmd_offset++;
    obj->iframes_timer = char2short(obj->cmds[obj->cmd_offset]);

    obj->cmd_offset++;
    obj->follow_id = char2short(obj->cmds[obj->cmd_offset]);
    return 0;
}

u8 readInvalidArg(obj_t *obj)
{
    obj->cmd_offset = -1;
    readOneCommand(obj);
    return 1;
}

u8 skipOneArg(obj_t *obj)
{
    obj->cmd_offset++;
    return 0;
}

u8 skipTestArgs(obj_t *obj)
{
    switch (obj->cmds[++obj->cmd_offset])
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        obj->cmd_offset++;
        break;
    case 70:
    case 71:
    case 72:
        break;
    }

    return 0;
}

u8 skipGoXYArgs(obj_t *obj)
{
    obj->cmd_offset += 2;
    return 0;
}

u8 skipSpeedArgs(obj_t *obj)
{
    obj->cmd_offset += 3;
    return 0;
}

u8 handle_SELF_HANDLED(obj_t *obj)
{
    return 0;
}

u8 skipInvalidArg(obj_t* obj) {
    obj->cmd_offset = -1;
    skipOneCommand(obj);
    return 1;
}

u8 handle_11_GO_LABEL(obj_t* obj) {
    return 1;
}

u8 handle_19_GO_WAITSTATE(obj_t* obj) {
    obj->change_anim_mode = ANIMMODE_RESET_IF_NEW;
    obj->cmd = GO_WAIT;
    obj->nb_cmd = vblToEOA(obj, obj->nb_cmd) - 1;
    return 0;
}

u8 handle_25_RESERVED_GO_GOSUB(obj_t* obj) {
    pushCmdContext(obj, 1);
    obj->cmd_offset = obj->cmd_labels[obj->nb_cmd];
    return 1;
}

u8 handle_RESERVED_GO_SKIP_and_RESERVED_GO_GOTO(obj_t *obj)
{
    obj->cmd_offset = obj->cmd_labels[obj->nb_cmd];
    return 1;
}

u8 handle_26_RESERVED_GO_BRANCHTRUE(obj_t* obj) {
    if (obj->flags.command_test) {
        obj->cmd_offset = obj->cmd_labels[obj->nb_cmd];
    }
    return 1;
}

u8 handle_27_RESERVED_GO_BRANCHFALSE(obj_t* obj) {
    if (!obj->flags.command_test) {
        obj->cmd_offset = obj->cmd_labels[obj->nb_cmd];
    }
    return 1;
}

u8 handle_28_RESERVED_GO_SKIPTRUE(obj_t* obj) {
    if (obj->flags.command_test) {
        obj->cmd_offset = obj->cmd_labels[obj->nb_cmd];
    }
    return 1;
}

u8 handle_29_RESERVED_GO_SKIPFALSE(obj_t* obj) {
    if (!obj->flags.command_test) {
        obj->cmd_offset = obj->cmd_labels[obj->nb_cmd];
    }
    return 1;
}

u8 handle_21_GO_X(obj_t* obj) {
    obj->x = obj->nb_cmd * 100 + obj->link;
    return 1;
}

u8 handle_22_GO_Y(obj_t* obj) {
    obj->y = obj->nb_cmd * 100 + obj->link;
    return 1;
}

u8 handle_12_GO_GOTO(obj_t* obj) {
    skipToLabel(obj, obj->nb_cmd, 1);
    return 0;
}

u8 handle_8_GO_STATE(obj_t* obj) {
    set_main_etat(obj, obj->nb_cmd);
    obj->speed_x = 0;
    obj->speed_y = 0;
    return 1;
}

u8 handle_5_GO_SUBSTATE(obj_t* obj) {
    set_sub_etat(obj, obj->nb_cmd);
    obj->speed_x = 0;
    obj->speed_y = 0;
    return 1;
}

u8 handle_6_GO_SKIP(obj_t* obj)
{
    s16 length;
    s16 i;

    length = obj->nb_cmd;
    for (i = 0; i < length; i++)
        skipOneCommand(obj);
    return 1;
}

u8 handle_9_GO_PREPARELOOP(obj_t* obj) {
    pushCmdContext(obj, obj->nb_cmd);
    return 1;
}

u8 handle_13_GO_GOSUB(obj_t* obj) {
    pushCmdContext(obj, 1);
    skipToLabel(obj, obj->nb_cmd, 1);
    return 0;
}

u8 handle_14_GO_RETURN(obj_t* obj) {
    popCmdContext(obj);
    return 1;
}

u8 handle_10_GO_DOLOOP(obj_t *obj)
{
    if (--obj->cmd_contexts[(u8)obj->cmd_context_depth].count > 0)
        obj->cmd_offset = obj->cmd_contexts[(u8)obj->cmd_context_depth].cmd_offset;
    else
        --obj->cmd_context_depth;
    return 1;
}

u8 handle_33_INVALID_CMD(obj_t* obj) {
    obj->cmd_offset = -1;
    return 1;
}

u8 handle_15_GO_BRANCHTRUE(obj_t* obj) {
    if (obj->flags.command_test) {
        skipToLabel(obj, obj->nb_cmd, 1);
        return 0;
    } else {
        return 1;
    }
}

u8 handle_16_GO_BRANCHFALSE(obj_t* obj) {
    if (!obj->flags.command_test) {
        skipToLabel(obj, obj->nb_cmd, 1);
        return 0;
    } else {
        return 1;
    }
}

u8 handle_31_GO_SKIPTRUE(obj_t *obj)
{
    s16 length;
    s16 i;

    if (obj->flags.command_test) {
        length = obj->nb_cmd;
        for (i = 0; i < length; i++)
            skipOneCommand(obj);
    }
    return 1;
}

u8 handle_32_GO_SKIPFALSE(obj_t *obj)
{
    s16 length;
    s16 i;

    if (!obj->flags.command_test) {
        length = obj->nb_cmd;
        for (i = 0; i < length; i++)
            skipOneCommand(obj);
    }
    return 1;
}

u8 handle_18_GO_SETTEST(obj_t* obj) {
    // NOTE: this doesn't make sense to me (?), the flag gets set only if obj->nb_cmd is uneven? (did they intend > 0?)
    obj->flags.command_test = obj->nb_cmd & 1;
    return 1;
}

u8 handle_17_GO_TEST(obj_t* obj) {
    u8 saved_flags;

    switch(obj->nb_cmd) {
    case 70:
        if (OBJ_IN_ZONE(obj))
            obj->flags.command_test = 1;
        else
            obj->flags.command_test = 0;
        break;
    case 71:
        if (obj->flags.hurtbyfist)
            obj->flags.command_test = 1;
        else
            obj->flags.command_test = 0;
        break;
    case 72:
        if (!obj->flags.read_commands)
            obj->flags.command_test = 1;
        else
            obj->flags.command_test = 0;
        break;
    case 0:
        if (obj->flags.flip_x == obj->param)
            obj->flags.command_test = 1;
        else
            obj->flags.command_test = 0;
        break;
    case 1:
        if (myRand(obj->param) == 0)
            obj->flags.command_test = 1;
        else
            obj->flags.command_test = 0;
        break;
    case 2:
        saved_flags = *((u8 *)&obj->flags);
        calc_obj_dir(obj);
        saved_flags <<= 4;
        saved_flags >>= 7;
        if (obj->flags.flip_x == obj->param)
            obj->flags.command_test = 1;
        else
            obj->flags.command_test = 0;
        obj->flags.flip_x = saved_flags;
        break;
    case 3:
        if (obj->main_etat == obj->param)
            obj->flags.command_test = 1;
        else
            obj->flags.command_test = 0;
        break;
    case 4:
        if (obj->sub_etat == obj->param)
            obj->flags.command_test = 1;
        else
            obj->flags.command_test = 0;
        break;
    }
    return 1;
}

u8 readOneCommand(obj_t *obj)
{
    ++obj->cmd_offset;
    obj->cmd = obj->cmds[obj->cmd_offset];
    return cptr_tab[obj->cmd].read(obj);
}

u8 skipOneCommand(obj_t *obj)
{
    ++obj->cmd_offset;
    obj->cmd = obj->cmds[obj->cmd_offset];
    return cptr_tab[obj->cmd].skip(obj);
}

void GET_OBJ_CMD(obj_t *obj)
{
    if (obj->cmds) {
        if (obj->flags.read_commands && obj->nb_cmd-- == 0) {
            do {
                readOneCommand(obj);
            } while (cptr_tab[obj->cmd].handle(obj));
        }
    } else {
        obj->cmd = GO_NOP;
    }
}

void pushCmdContext(obj_t *obj, u8 count)
{
    ++obj->cmd_context_depth;
    obj->cmd_contexts[(u8)obj->cmd_context_depth].cmd_offset = obj->cmd_offset;
    obj->cmd_contexts[(u8)obj->cmd_context_depth].count = count;
}

void skipToLabel(obj_t* obj, u8 label, u8 skip_label_cmd) {
    s16 initial_offset;
    u8 initial_read_commands;

    initial_offset = obj->cmd_offset;
    initial_read_commands = obj->flags.read_commands;
    do {
        skipOneCommand(obj);
        if (obj->cmd_offset == initial_offset)
            break;
    } while (obj->cmd != GO_LABEL || obj->cmds[obj->cmd_offset] != label);

    if (skip_label_cmd) {
        if (initial_offset != obj->cmd_offset) {
            obj->nb_cmd = 0;
            obj->flags.read_commands = 1;
            GET_OBJ_CMD(obj);
            obj->flags.read_commands = initial_read_commands;
        }
    } else {
        obj->cmd = GO_NOP;
    }
}

void pushToLabel(obj_t *obj, u8 label, u8 skip_label_cmd)
{
    pushCmdContext(obj, 1);
    skipToLabel(obj, label, skip_label_cmd);
}
