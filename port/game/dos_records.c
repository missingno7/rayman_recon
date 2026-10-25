/*
 * Serialized DOS records -> native runtime structures.
 *
 * Level files store object instances as the 132-byte DOS obj_t image (docs/port/FILE_FORMATS.md "Event/object
 * section"). Natively obj_t holds 8-byte pointers, so its layout differs; this decoder copies every non-pointer
 * field from its DOS offset (docs/semantics/generated/semantics_dos.h, generated from the checked registry) and
 * hands the four pointer-shaped words back to the loader, which rebinds them exactly as DOS did.
 */
#include "game/dos_records.h"

#include <stddef.h>
#include <string.h>

#include "semantics_dos.h"

#if DOS_OBJ_T_SIZE != 132
#error "DOS obj_t record size changed in the semantic registry"
#endif

#define FIELD(name, NAME) {DOS_OBJ_T_##NAME##_OFFSET, offsetof(obj_t, name), sizeof(((obj_t *)0)->name)}

static const struct { unsigned dos, native, size; } fields[] = {
    FIELD(cmd_contexts, CMD_CONTEXTS), FIELD(field_1C, FIELD_1C), FIELD(link_has_gendoor, LINK_HAS_GENDOOR),
    FIELD(is_active, IS_ACTIVE), FIELD(x, X), FIELD(y, Y), FIELD(active_flag, ACTIVE_FLAG), FIELD(id, ID),
    FIELD(screen_x, SCREEN_X), FIELD(screen_y, SCREEN_Y), FIELD(sub_speed, SUB_SPEED), FIELD(init_x, INIT_X),
    FIELD(init_y, INIT_Y), FIELD(speed_x, SPEED_X), FIELD(speed_y, SPEED_Y), FIELD(nb_sprites, NB_SPRITES),
    FIELD(cmd_offset, CMD_OFFSET), FIELD(nb_cmd, NB_CMD), FIELD(follow_id, FOLLOW_ID), FIELD(follow_y, FOLLOW_Y),
    FIELD(follow_x, FOLLOW_X), FIELD(param, PARAM), FIELD(link, LINK), FIELD(ray_dist, RAY_DIST),
    FIELD(iframes_timer, IFRAMES_TIMER), FIELD(test_block_index, TEST_BLOCK_INDEX), FIELD(scale, SCALE),
    FIELD(zdc, ZDC), FIELD(active_timer, ACTIVE_TIMER), FIELD(type, TYPE), FIELD(btypes, BTYPES),
    FIELD(unused_padding, UNUSED_PADDING), FIELD(offset_bx, OFFSET_BX), FIELD(offset_by, OFFSET_BY),
    FIELD(anim_index, ANIM_INDEX), FIELD(anim_frame, ANIM_FRAME), FIELD(sub_etat, SUB_ETAT),
    FIELD(main_etat, MAIN_ETAT), FIELD(init_sub_etat, INIT_SUB_ETAT), FIELD(init_main_etat, INIT_MAIN_ETAT),
    FIELD(cmd, CMD), FIELD(gravity_value_1, GRAVITY_VALUE_1), FIELD(gravity_value_2, GRAVITY_VALUE_2),
    FIELD(change_anim_mode, CHANGE_ANIM_MODE), FIELD(offset_hy, OFFSET_HY), FIELD(follow_sprite, FOLLOW_SPRITE),
    FIELD(hit_points, HIT_POINTS), FIELD(init_hit_points, INIT_HIT_POINTS), FIELD(init_flag, INIT_FLAG),
    FIELD(hit_sprite, HIT_SPRITE), FIELD(detect_zone, DETECT_ZONE), FIELD(detect_zone_flag, DETECT_ZONE_FLAG),
    FIELD(cmd_context_depth, CMD_CONTEXT_DEPTH), FIELD(config, CONFIG), FIELD(display_prio, DISPLAY_PRIO),
    FIELD(timer, TIMER), FIELD(anim_count, ANIM_COUNT), FIELD(flags, FLAGS), FIELD(field_83, FIELD_83),
};

static s32 word_at(const u8 *rec, unsigned offset)
{
    return (s32)((u32)rec[offset] | (u32)rec[offset + 1] << 8 | (u32)rec[offset + 2] << 16 |
                 (u32)rec[offset + 3] << 24);
}

void port_decode_obj_record(obj_t *obj, const u8 record[132], port_obj_words_t *words)
{
    unsigned i;
    memset(obj, 0, sizeof *obj);
    /* Little-endian host: DOS multi-byte fields are copied byte for byte (bit-field bytes included). */
    for (i = 0; i < sizeof fields / sizeof fields[0]; i++)
        memcpy((u8 *)obj + fields[i].native, record + fields[i].dos, fields[i].size);
    words->sprites = word_at(record, DOS_OBJ_T_SPRITES_OFFSET);
    words->animations = word_at(record, DOS_OBJ_T_ANIMATIONS_OFFSET);
    words->img_buffer = word_at(record, DOS_OBJ_T_IMG_BUFFER_OFFSET);
    words->eta = word_at(record, DOS_OBJ_T_ETA_OFFSET);
}

size_t port_read_obj_records(obj_t *objects, s16 count, port_obj_words_t *words, FILE *fp)
{
    u8 record[132];
    s16 i;
    size_t n = 0;
    for (i = 0; i < count; i++) {
        memset(record, 0, sizeof record);
        if (fread(record, sizeof record, 1, fp) == 1)
            n++;
        port_decode_obj_record(&objects[i], record, &words[i]);
    }
    return n;
}
