/* Generated DOS ABI facts; native runtime structs need not be packed. */
#ifndef RAYMAN_SEMANTICS_DOS_H
#define RAYMAN_SEMANTICS_DOS_H
#define DOS_OBJ_T_SIZE 132
#define DOS_ETA_T_SIZE 8
#define DOS_SPRITE_T_SIZE 12
#define DOS_ANIM_T_SIZE 12
#define DOS_ANIM_FRAME_T_SIZE 4
#define DOS_ANIM_LAYER_T_SIZE 4
#define DOS_OBJ_FLAGS_T_SIZE 2
#define DOS_CMD_CONTEXT_T_SIZE 4
#define DOS_LEVEL_T_SIZE 6
#define DOS_LEVEL_OBJ_T_SIZE 6
#define DOS_ACTIVE_OBJECTS_T_SIZE 202
#define DOS_OBJ_TYPE_FLAGS_T_SIZE 4
#define DOS_MAP_DATA_T_SIZE 12
#define DOS_MAP_TILE_T_SIZE 6
#define DOS_BLOCK_FLAGS_T_SIZE 1
#define DOS_RAYEVTS_T_SIZE 2
#define DOS_FINBOSSLEVEL_T_SIZE 2
#define DOS_POING_T_SIZE 20
#define DOS_FONT_T_SIZE 10
#define DOS_DISPLAY_ITEM_T_SIZE 216
/* PROVEN: Runtime sprite-table pointer; serialized instance slot is a signed16 descriptor index carried in32bits. */
#define DOS_OBJ_T_SPRITES_OFFSET 0x0
#define DOS_OBJ_T_SPRITES_SIZE 4
/* PROVEN: Runtime animation-table pointer; serialized instance slot is a signed16 descriptor index carried in32bits. */
#define DOS_OBJ_T_ANIMATIONS_OFFSET 0x4
#define DOS_OBJ_T_ANIMATIONS_SIZE 4
/* PROVEN: Runtime atlas pointer; serialized instance slot is a signed16 descriptor index carried in32bits. */
#define DOS_OBJ_T_IMG_BUFFER_OFFSET 0x8
#define DOS_OBJ_T_IMG_BUFFER_SIZE 4
/* PROVEN: Runtime main-state pointer table; serialized instance slot is a signed16 bank index carried in32bits. */
#define DOS_OBJ_T_ETA_OFFSET 0xc
#define DOS_OBJ_T_ETA_SIZE 4
/* PROVEN: Runtime object command byte stream, replacing an ignored serialized producer pointer. */
#define DOS_OBJ_T_CMDS_OFFSET 0x10
#define DOS_OBJ_T_CMDS_SIZE 4
/* PROVEN: Runtime signed16 command-label offset table, replacing an ignored serialized producer pointer. */
#define DOS_OBJ_T_CMD_LABELS_OFFSET 0x14
#define DOS_OBJ_T_CMD_LABELS_SIZE 4
/* STRONG: Object script stream/context member cmd_contexts. */
#define DOS_OBJ_T_CMD_CONTEXTS_OFFSET 0x18
#define DOS_OBJ_T_CMD_CONTEXTS_SIZE 4
/* PROVEN: Overloaded32-bit activation/restore scratch marker. */
#define DOS_OBJ_T_FIELD_1C_OFFSET 0x1c
#define DOS_OBJ_T_FIELD_1C_SIZE 4
/* STRONG: Marker enabling linked-group operations, also selecting the reinitialization delay. */
#define DOS_OBJ_T_LINK_HAS_GENDOOR_OFFSET 0x20
#define DOS_OBJ_T_LINK_HAS_GENDOOR_SIZE 4
/* PROVEN: Object participation flag saved as the high bit of each two-bit snapshot entry. */
#define DOS_OBJ_T_IS_ACTIVE_OFFSET 0x24
#define DOS_OBJ_T_IS_ACTIVE_SIZE 4
/* PROVEN: Signed32 live-instance world-pixel x; resource-holder wldobj records reuse the same member as atlas byte length. */
#define DOS_OBJ_T_X_OFFSET 0x28
#define DOS_OBJ_T_X_SIZE 4
/* PROVEN: Signed32 world pixel y origin in live object movement. */
#define DOS_OBJ_T_Y_OFFSET 0x2c
#define DOS_OBJ_T_Y_SIZE 4
/* PROVEN: Activation lifecycle code used by zone transitions: 0 activated/default,1 dead/outside,2 reinitialised awaiting departure,4 waiting for the linked group to leave. */
#define DOS_OBJ_T_ACTIVE_FLAG_OFFSET 0x30
#define DOS_OBJ_T_ACTIVE_FLAG_SIZE 4
/* PROVEN: Object identifier stored in the active and always-object id lists. */
#define DOS_OBJ_T_ID_OFFSET 0x34
#define DOS_OBJ_T_ID_SIZE 2
/* PROVEN: Stored screen-space origin coordinate screen_x after camera subtraction. */
#define DOS_OBJ_T_SCREEN_X_OFFSET 0x36
#define DOS_OBJ_T_SCREEN_X_SIZE 2
/* PROVEN: Stored screen-space origin coordinate screen_y after camera subtraction. */
#define DOS_OBJ_T_SCREEN_Y_OFFSET 0x38
#define DOS_OBJ_T_SCREEN_Y_SIZE 2
/* UNKNOWN: Type-dependent signed16 scratch used for a water-motion residual and another type's horizontal distance. */
#define DOS_OBJ_T_SUB_SPEED_OFFSET 0x3a
#define DOS_OBJ_T_SUB_SPEED_SIZE 2
/* PROVEN: Signed16 reset x captured from the low16bits of live x by first_obj_init. */
#define DOS_OBJ_T_INIT_X_OFFSET 0x3c
#define DOS_OBJ_T_INIT_X_SIZE 2
/* PROVEN: Signed16 reset y captured from the low16bits of live y by first_obj_init. */
#define DOS_OBJ_T_INIT_Y_OFFSET 0x3e
#define DOS_OBJ_T_INIT_Y_SIZE 2
/* PROVEN: Signed16 horizontal speed, interpreted per type axis flag as integer pixels/call or sixteenths of a pixel/call. */
#define DOS_OBJ_T_SPEED_X_OFFSET 0x40
#define DOS_OBJ_T_SPEED_X_SIZE 2
/* PROVEN: Signed16 vertical speed, interpreted per type axis flag as integer pixels/call or sixteenths of a pixel/call. */
#define DOS_OBJ_T_SPEED_Y_OFFSET 0x42
#define DOS_OBJ_T_SPEED_Y_SIZE 2
/* PROVEN: Signed16 sprite count in obj_t, including resource holders. */
#define DOS_OBJ_T_NB_SPRITES_OFFSET 0x44
#define DOS_OBJ_T_NB_SPRITES_SIZE 2
/* STRONG: Object script stream/context member cmd_offset. */
#define DOS_OBJ_T_CMD_OFFSET_OFFSET 0x46
#define DOS_OBJ_T_CMD_OFFSET_SIZE 2
/* STRONG: Type-dependent script count or Rayman airborne-inertia selector. */
#define DOS_OBJ_T_NB_CMD_OFFSET 0x48
#define DOS_OBJ_T_NB_CMD_SIZE 2
/* STRONG: Multipurpose signed16 slot: support/object ID in some actors, timing/command parameter in others. */
#define DOS_OBJ_T_FOLLOW_ID_OFFSET 0x4a
#define DOS_OBJ_T_FOLLOW_ID_SIZE 2
/* STRONG: Type-dependent vertical follower displacement or grapple radius. */
#define DOS_OBJ_T_FOLLOW_Y_OFFSET 0x4c
#define DOS_OBJ_T_FOLLOW_Y_SIZE 2
/* STRONG: Type-dependent horizontal follower displacement or grapple angle. */
#define DOS_OBJ_T_FOLLOW_X_OFFSET 0x4e
#define DOS_OBJ_T_FOLLOW_X_SIZE 2
/* PROVEN: Type-specific signed scratch parameter, including Rayman edge classification in calc_btyp. */
#define DOS_OBJ_T_PARAM_OFFSET 0x50
#define DOS_OBJ_T_PARAM_SIZE 2
/* UNKNOWN: Multipurpose signed16 scratch; initialization includes paired-object links and unrelated timers/parameters. */
#define DOS_OBJ_T_LINK_OFFSET 0x52
#define DOS_OBJ_T_LINK_SIZE 2
/* STRONG: Type-dependent Rayman tile index or support-distance code with several geometric conventions. */
#define DOS_OBJ_T_RAY_DIST_OFFSET 0x54
#define DOS_OBJ_T_RAY_DIST_SIZE 2
/* STRONG: Multipurpose signed16 timer/state slot, including invulnerability, platform delays and command parameters. */
#define DOS_OBJ_T_IFRAMES_TIMER_OFFSET 0x56
#define DOS_OBJ_T_IFRAMES_TIMER_SIZE 2
/* PROVEN: Last row-major map tile index used by the changed-block test. */
#define DOS_OBJ_T_TEST_BLOCK_INDEX_OFFSET 0x58
#define DOS_OBJ_T_TEST_BLOCK_INDEX_SIZE 2
/* PROVEN: Projection-depth parameter whose denominator is signed_scale+256. */
#define DOS_OBJ_T_SCALE_OFFSET 0x5a
#define DOS_OBJ_T_SCALE_SIZE 2
/* PROVEN: Packed reference to an object collision-zone run. */
#define DOS_OBJ_T_ZDC_OFFSET 0x5c
#define DOS_OBJ_T_ZDC_SIZE 2
/* PROVEN: Object reactivation-delay counter. */
#define DOS_OBJ_T_ACTIVE_TIMER_OFFSET 0x5e
#define DOS_OBJ_T_ACTIVE_TIMER_SIZE 2
/* PROVEN: Unsigned object type discriminator indexing flags and selecting behaviour. */
#define DOS_OBJ_T_TYPE_OFFSET 0x60
#define DOS_OBJ_T_TYPE_SIZE 2
/* PROVEN: Five cached collision block types in centre,left,right,above,below order. */
#define DOS_OBJ_T_BTYPES_OFFSET 0x62
#define DOS_OBJ_T_BTYPES_SIZE 5
/* UNKNOWN: Neutral obj_t.unused_padding storage/view; its intended meaning is not established by this pilot. */
#define DOS_OBJ_T_UNUSED_PADDING_OFFSET 0x67
#define DOS_OBJ_T_UNUSED_PADDING_SIZE 1
/* PROVEN: Horizontal object anchor offset used for collision probes and reflection centre. */
#define DOS_OBJ_T_OFFSET_BX_OFFSET 0x68
#define DOS_OBJ_T_OFFSET_BX_SIZE 1
/* PROVEN: Vertical object anchor offset used for foot/block probes and projection centre. */
#define DOS_OBJ_T_OFFSET_BY_OFFSET 0x69
#define DOS_OBJ_T_OFFSET_BY_SIZE 1
/* PROVEN: Current animation selector or current frame selector (anim_index). */
#define DOS_OBJ_T_ANIM_INDEX_OFFSET 0x6a
#define DOS_OBJ_T_ANIM_INDEX_SIZE 1
/* PROVEN: Current animation selector or current frame selector (anim_frame). */
#define DOS_OBJ_T_ANIM_FRAME_OFFSET 0x6b
#define DOS_OBJ_T_ANIM_FRAME_SIZE 1
/* PROVEN: Object-type- and main-state-specific substate-table index. */
#define DOS_OBJ_T_SUB_ETAT_OFFSET 0x6c
#define DOS_OBJ_T_SUB_ETAT_SIZE 1
/* PROVEN: Object-type-specific main state-table index; Rayman's controller dispatches eight observed state families. */
#define DOS_OBJ_T_MAIN_ETAT_OFFSET 0x6d
#define DOS_OBJ_T_MAIN_ETAT_SIZE 1
/* PROVEN: Substate reset byte captured by first_obj_init and modified by some type-specific defaults. */
#define DOS_OBJ_T_INIT_SUB_ETAT_OFFSET 0x6e
#define DOS_OBJ_T_INIT_SUB_ETAT_SIZE 1
/* PROVEN: Main-state reset byte captured by first_obj_init. */
#define DOS_OBJ_T_INIT_MAIN_ETAT_OFFSET 0x6f
#define DOS_OBJ_T_INIT_MAIN_ETAT_SIZE 1
/* STRONG: Current script command discriminator. */
#define DOS_OBJ_T_CMD_OFFSET 0x70
#define DOS_OBJ_T_CMD_SIZE 1
/* STRONG: First type-dependent gravity/control phase byte. */
#define DOS_OBJ_T_GRAVITY_VALUE_1_OFFSET 0x71
#define DOS_OBJ_T_GRAVITY_VALUE_1_SIZE 1
/* STRONG: Second type-dependent gravity/control phase byte. */
#define DOS_OBJ_T_GRAVITY_VALUE_2_OFFSET 0x72
#define DOS_OBJ_T_GRAVITY_VALUE_2_SIZE 1
/* PROVEN: Animation transition mode set to1 by the state setters. */
#define DOS_OBJ_T_CHANGE_ANIM_MODE_OFFSET 0x73
#define DOS_OBJ_T_CHANGE_ANIM_MODE_SIZE 1
/* STRONG: Unsigned vertical upper-body/collision anchor offset. */
#define DOS_OBJ_T_OFFSET_HY_OFFSET 0x74
#define DOS_OBJ_T_OFFSET_HY_SIZE 1
/* PROVEN: Selected animation layer for following, also overloaded as Blacktoon behaviour selector. */
#define DOS_OBJ_T_FOLLOW_SPRITE_OFFSET 0x75
#define DOS_OBJ_T_FOLLOW_SPRITE_SIZE 1
/* PROVEN: Unsigned hit-point byte in obj_hurt, with type-specific reuse including signed deformation angle input. */
#define DOS_OBJ_T_HIT_POINTS_OFFSET 0x76
#define DOS_OBJ_T_HIT_POINTS_SIZE 1
/* PROVEN: Reset byte captured from hit_points by first_obj_init. */
#define DOS_OBJ_T_INIT_HIT_POINTS_OFFSET 0x77
#define DOS_OBJ_T_INIT_HIT_POINTS_SIZE 1
/* UNKNOWN: Serialized byte with initializer writes of7; no intended common meaning or typed reader is established. */
#define DOS_OBJ_T_INIT_FLAG_OFFSET 0x78
#define DOS_OBJ_T_INIT_FLAG_SIZE 1
/* PROVEN: Collision layer selector with three special dispatch codes. */
#define DOS_OBJ_T_HIT_SPRITE_OFFSET 0x79
#define DOS_OBJ_T_HIT_SPRITE_SIZE 1
/* PROVEN: Type-dependent unsigned8 detection-range parameter with initialization defaults. */
#define DOS_OBJ_T_DETECT_ZONE_OFFSET 0x7a
#define DOS_OBJ_T_DETECT_ZONE_SIZE 1
/* PROVEN: Detection-zone classification tested by OBJ_IN_ZONE. */
#define DOS_OBJ_T_DETECT_ZONE_FLAG_OFFSET 0x7b
#define DOS_OBJ_T_DETECT_ZONE_FLAG_SIZE 1
/* STRONG: Object script stream/context member cmd_context_depth. */
#define DOS_OBJ_T_CMD_CONTEXT_DEPTH_OFFSET 0x7c
#define DOS_OBJ_T_CMD_CONTEXT_DEPTH_SIZE 1
/* PROVEN: Type-specific byte configuration tested for Blacktoon edge-turning. */
#define DOS_OBJ_T_CONFIG_OFFSET 0x7d
#define DOS_OBJ_T_CONFIG_SIZE 1
/* PROVEN: Object display bucket,distinct from activation/list membership. */
#define DOS_OBJ_T_DISPLAY_PRIO_OFFSET 0x7e
#define DOS_OBJ_T_DISPLAY_PRIO_SIZE 1
/* UNKNOWN: Multipurpose byte countdown/state scratch with common reset0 and actor-specific defaults. */
#define DOS_OBJ_T_TIMER_OFFSET 0x7f
#define DOS_OBJ_T_TIMER_SIZE 1
/* PROVEN: Unsigned8 animation count loaded into resource-holder records. */
#define DOS_OBJ_T_ANIM_COUNT_OFFSET 0x80
#define DOS_OBJ_T_ANIM_COUNT_SIZE 1
/* PROVEN: Packed two-byte per-object flag record. */
#define DOS_OBJ_T_FLAGS_OFFSET 0x81
#define DOS_OBJ_T_FLAGS_SIZE 2
/* UNKNOWN: Neutral obj_t.field_83 storage/view; its intended meaning is not established by this pilot. */
#define DOS_OBJ_T_FIELD_83_OFFSET 0x83
#define DOS_OBJ_T_FIELD_83_SIZE 1
/* STRONG: Signed state-selected horizontal speed for the speed_x_right facing. */
#define DOS_ETA_T_SPEED_X_RIGHT_OFFSET 0x0
#define DOS_ETA_T_SPEED_X_RIGHT_SIZE 1
/* STRONG: Signed state-selected horizontal speed for the speed_x_left facing. */
#define DOS_ETA_T_SPEED_X_LEFT_OFFSET 0x1
#define DOS_ETA_T_SPEED_X_LEFT_SIZE 1
/* PROVEN: Animation id selected by this main/substate entry. */
#define DOS_ETA_T_ANIM_INDEX_OFFSET 0x2
#define DOS_ETA_T_ANIM_INDEX_SIZE 1
/* PROVEN: State indices selected when an animation crosses its frame bounds. */
#define DOS_ETA_T_NEXT_MAIN_ETAT_OFFSET 0x3
#define DOS_ETA_T_NEXT_MAIN_ETAT_SIZE 1
/* PROVEN: State indices selected when an animation crosses its frame bounds. */
#define DOS_ETA_T_NEXT_SUB_ETAT_OFFSET 0x4
#define DOS_ETA_T_NEXT_SUB_ETAT_SIZE 1
/* PROVEN: Packed state timing/control byte: low nibble selects the animation clock, high nibble selects the airborne gravity/control mode. */
#define DOS_ETA_T_ANIM_SPEED_OFFSET 0x5
#define DOS_ETA_T_ANIM_SPEED_SIZE 1
/* PROVEN: Overlapping nibble/bit alias container for the corresponding eta byte. */
#define DOS_ETA_T_ANIM_SPEED_BITS_OFFSET 0x5
#define DOS_ETA_T_ANIM_SPEED_BITS_SIZE 1
/* PROVEN: Low nibble alias of the animation timing byte. */
#define DOS_ETA_T_ANIM_SPEED_BITS_LOW_OFFSET 0x5
#define DOS_ETA_T_ANIM_SPEED_BITS_LOW_SIZE 1
#define DOS_ETA_T_ANIM_SPEED_BITS_LOW_MASK 0x0f
/* PROVEN: High-nibble gravity/control selector consumed by DO_PESANTEUR and specialized state logic. */
#define DOS_ETA_T_ANIM_SPEED_BITS_HIGH_OFFSET 0x5
#define DOS_ETA_T_ANIM_SPEED_BITS_HIGH_SIZE 1
#define DOS_ETA_T_ANIM_SPEED_BITS_HIGH_MASK 0xf0
/* STRONG: State-selected sound event index. */
#define DOS_ETA_T_SOUND_INDEX_OFFSET 0x6
#define DOS_ETA_T_SOUND_INDEX_SIZE 1
/* PROVEN: State behaviour bitset, including reverse playback0x10 and Rayman rectangle selection0x40. */
#define DOS_ETA_T_FLAGS_OFFSET 0x7
#define DOS_ETA_T_FLAGS_SIZE 1
/* PROVEN: Overlapping nibble/bit alias container for the corresponding eta byte. */
#define DOS_ETA_T_FLAGS_BITS_OFFSET 0x7
#define DOS_ETA_T_FLAGS_BITS_SIZE 1
/* STRONG: State permission bit tested by Rayman jump initiation and the stonewoman jump handler. */
#define DOS_ETA_T_FLAGS_BITS_BIT0_OFFSET 0x7
#define DOS_ETA_T_FLAGS_BITS_BIT0_SIZE 1
#define DOS_ETA_T_FLAGS_BITS_BIT0_MASK 0x01
/* STRONG: State permission bit gating fist preparation in RAY_PREPARE_FIST. */
#define DOS_ETA_T_FLAGS_BITS_BIT1_OFFSET 0x7
#define DOS_ETA_T_FLAGS_BITS_BIT1_SIZE 1
#define DOS_ETA_T_FLAGS_BITS_BIT1_MASK 0x02
/* STRONG: State permission bit gating helicopter activation. */
#define DOS_ETA_T_FLAGS_BITS_BIT2_OFFSET 0x7
#define DOS_ETA_T_FLAGS_BITS_BIT2_SIZE 1
#define DOS_ETA_T_FLAGS_BITS_BIT2_MASK 0x04
/* STRONG: State participation bit for damage/contact eligibility, with actor-dependent readers. */
#define DOS_ETA_T_FLAGS_BITS_BIT3_OFFSET 0x7
#define DOS_ETA_T_FLAGS_BITS_BIT3_SIZE 1
#define DOS_ETA_T_FLAGS_BITS_BIT3_MASK 0x08
/* PROVEN: Reverse animation playback flag0x10, selecting decrement and last-frame initialization. */
#define DOS_ETA_T_FLAGS_BITS_ANIM_REVERSE_OFFSET 0x7
#define DOS_ETA_T_FLAGS_BITS_ANIM_REVERSE_SIZE 1
#define DOS_ETA_T_FLAGS_BITS_ANIM_REVERSE_MASK 0x10
/* STRONG: State gate for Rayman contact dispatch and collision-zone traversal. */
#define DOS_ETA_T_FLAGS_BITS_BIT5_OFFSET 0x7
#define DOS_ETA_T_FLAGS_BITS_BIT5_SIZE 1
#define DOS_ETA_T_FLAGS_BITS_BIT5_MASK 0x20
/* PROVEN: Context-dependent state bit selecting Rayman's low collision shape and participating in generating-door inset encoding. */
#define DOS_ETA_T_FLAGS_BITS_BIT6_OFFSET 0x7
#define DOS_ETA_T_FLAGS_BITS_BIT6_SIZE 1
#define DOS_ETA_T_FLAGS_BITS_BIT6_MASK 0x40
/* STRONG: Low-weight bit of generating-door collision inset selection, paired with bit6. */
#define DOS_ETA_T_FLAGS_BITS_BIT7_OFFSET 0x7
#define DOS_ETA_T_FLAGS_BITS_BIT7_SIZE 1
#define DOS_ETA_T_FLAGS_BITS_BIT7_MASK 0x80
/* PROVEN: Unsigned32 byte offset of sprite pixels relative to the atlas base. */
#define DOS_SPRITE_T_OFFSET_IN_ATLAS_OFFSET 0x0
#define DOS_SPRITE_T_OFFSET_IN_ATLAS_SIZE 4
/* PROVEN: Sprite reference id, with0 treated as an absent sprite in GET_SPRITE_POS. */
#define DOS_SPRITE_T_ID_OFFSET 0x4
#define DOS_SPRITE_T_ID_SIZE 1
/* STRONG: Outer sprite rectangle dimension outer_width. */
#define DOS_SPRITE_T_OUTER_WIDTH_OFFSET 0x5
#define DOS_SPRITE_T_OUTER_WIDTH_SIZE 1
/* STRONG: Outer sprite rectangle dimension outer_height. */
#define DOS_SPRITE_T_OUTER_HEIGHT_OFFSET 0x6
#define DOS_SPRITE_T_OUTER_HEIGHT_SIZE 1
/* PROVEN: Returned inner sprite rectangle dimension inner_width. */
#define DOS_SPRITE_T_INNER_WIDTH_OFFSET 0x7
#define DOS_SPRITE_T_INNER_WIDTH_SIZE 1
/* PROVEN: Returned inner sprite rectangle dimension inner_height. */
#define DOS_SPRITE_T_INNER_HEIGHT_OFFSET 0x8
#define DOS_SPRITE_T_INNER_HEIGHT_SIZE 1
/* PROVEN: Overlapping raw/nibble views of the sprite inset byte. */
#define DOS_SPRITE_T_SPRITE_POS_OFFSET 0x9
#define DOS_SPRITE_T_SPRITE_POS_SIZE 1
/* PROVEN: Overlapping raw/nibble views of the sprite inset byte. */
#define DOS_SPRITE_T_SPRITE_POS_RAW_OFFSET 0x9
#define DOS_SPRITE_T_SPRITE_POS_RAW_SIZE 1
/* PROVEN: Overlapping raw/nibble views of the sprite inset byte. */
#define DOS_SPRITE_T_SPRITE_POS_BITS_OFFSET 0x9
#define DOS_SPRITE_T_SPRITE_POS_BITS_SIZE 1
/* PROVEN: Packed nonnegative sprite inset component sprite_pos.bits.x. */
#define DOS_SPRITE_T_SPRITE_POS_BITS_X_OFFSET 0x9
#define DOS_SPRITE_T_SPRITE_POS_BITS_X_SIZE 1
#define DOS_SPRITE_T_SPRITE_POS_BITS_X_MASK 0x0f
/* PROVEN: Packed nonnegative sprite inset component sprite_pos.bits.y. */
#define DOS_SPRITE_T_SPRITE_POS_BITS_Y_OFFSET 0x9
#define DOS_SPRITE_T_SPRITE_POS_BITS_Y_SIZE 1
#define DOS_SPRITE_T_SPRITE_POS_BITS_Y_MASK 0xf0
/* PROVEN: Raw/bitfield view of the sprite color control byte. */
#define DOS_SPRITE_T_COLOR_OFFSET 0xa
#define DOS_SPRITE_T_COLOR_SIZE 1
/* PROVEN: Raw/bitfield view of the sprite color_bits control byte. */
#define DOS_SPRITE_T_COLOR_BITS_OFFSET 0xa
#define DOS_SPRITE_T_COLOR_BITS_SIZE 1
/* UNKNOWN: Neutral sprite_t.color_bits.unused storage/view; its intended meaning is not established by this pilot. */
#define DOS_SPRITE_T_COLOR_BITS_UNUSED_OFFSET 0xa
#define DOS_SPRITE_T_COLOR_BITS_UNUSED_SIZE 1
#define DOS_SPRITE_T_COLOR_BITS_UNUSED_MASK 0x0f
/* STRONG: High-nibble palette selection. */
#define DOS_SPRITE_T_COLOR_BITS_PALETTE_INDEX_OFFSET 0xa
#define DOS_SPRITE_T_COLOR_BITS_PALETTE_INDEX_SIZE 1
#define DOS_SPRITE_T_COLOR_BITS_PALETTE_INDEX_MASK 0xf0
/* PROVEN: Raw/bitfield view of the sprite flags control byte. */
#define DOS_SPRITE_T_FLAGS_OFFSET 0xb
#define DOS_SPRITE_T_FLAGS_SIZE 1
/* PROVEN: Raw/bitfield view of the sprite flags_bits control byte. */
#define DOS_SPRITE_T_FLAGS_BITS_OFFSET 0xb
#define DOS_SPRITE_T_FLAGS_BITS_SIZE 1
/* UNKNOWN: Neutral sprite_t.flags_bits.unused0 storage/view; its intended meaning is not established by this pilot. */
#define DOS_SPRITE_T_FLAGS_BITS_UNUSED0_OFFSET 0xb
#define DOS_SPRITE_T_FLAGS_BITS_UNUSED0_SIZE 1
#define DOS_SPRITE_T_FLAGS_BITS_UNUSED0_MASK 0x03
/* PROVEN: Sprite orientation bit used by single-sprite drawing. */
#define DOS_SPRITE_T_FLAGS_BITS_FLIP_X_OFFSET 0xb
#define DOS_SPRITE_T_FLAGS_BITS_FLIP_X_SIZE 1
#define DOS_SPRITE_T_FLAGS_BITS_FLIP_X_MASK 0x04
/* UNKNOWN: Neutral sprite_t.flags_bits.unused3 storage/view; its intended meaning is not established by this pilot. */
#define DOS_SPRITE_T_FLAGS_BITS_UNUSED3_OFFSET 0xb
#define DOS_SPRITE_T_FLAGS_BITS_UNUSED3_SIZE 1
#define DOS_SPRITE_T_FLAGS_BITS_UNUSED3_MASK 0xf8
/* PROVEN: Runtime pointer to the allocated animation layer payload. */
#define DOS_ANIM_T_LAYERS_OFFSET 0x0
#define DOS_ANIM_T_LAYERS_SIZE 4
/* PROVEN: Runtime frame-rectangle pointer, with serialized32-bit-1 sentinel indicating no rectangle payload. */
#define DOS_ANIM_T_FRAMES_OFFSET 0x4
#define DOS_ANIM_T_FRAMES_SIZE 4
/* PROVEN: Packed animation header: low14 bits are layer count per frame, top2 bits encode the horizontal-speed clock period minus one. */
#define DOS_ANIM_T_LAYERS_PER_FRAME_OFFSET 0x8
#define DOS_ANIM_T_LAYERS_PER_FRAME_SIZE 2
/* PROVEN: Number of frames in this animation. */
#define DOS_ANIM_T_FRAMES_COUNT_OFFSET 0xa
#define DOS_ANIM_T_FRAMES_COUNT_SIZE 2
/* PROVEN: Unsigned offset of the animation bounding rectangle (x). */
#define DOS_ANIM_FRAME_T_X_OFFSET 0x0
#define DOS_ANIM_FRAME_T_X_SIZE 1
/* PROVEN: Unsigned offset of the animation bounding rectangle (y). */
#define DOS_ANIM_FRAME_T_Y_OFFSET 0x1
#define DOS_ANIM_FRAME_T_Y_SIZE 1
/* PROVEN: Animation bounding rectangle dimension width. */
#define DOS_ANIM_FRAME_T_WIDTH_OFFSET 0x2
#define DOS_ANIM_FRAME_T_WIDTH_SIZE 1
/* PROVEN: Animation bounding rectangle dimension height. */
#define DOS_ANIM_FRAME_T_HEIGHT_OFFSET 0x3
#define DOS_ANIM_FRAME_T_HEIGHT_SIZE 1
/* PROVEN: Layer horizontal mirror selector combined with object facing. */
#define DOS_ANIM_LAYER_T_MIRRORED_OFFSET 0x0
#define DOS_ANIM_LAYER_T_MIRRORED_SIZE 1
/* PROVEN: Unsigned object-relative layer offset component x. */
#define DOS_ANIM_LAYER_T_X_OFFSET 0x1
#define DOS_ANIM_LAYER_T_X_SIZE 1
/* PROVEN: Unsigned object-relative layer offset component y. */
#define DOS_ANIM_LAYER_T_Y_OFFSET 0x2
#define DOS_ANIM_LAYER_T_Y_SIZE 1
/* PROVEN: Index into the owning object sprite table for this layer. */
#define DOS_ANIM_LAYER_T_SPRITE_INDEX_OFFSET 0x3
#define DOS_ANIM_LAYER_T_SPRITE_INDEX_SIZE 1
/* STRONG: Fist-hit interaction flag. */
#define DOS_OBJ_FLAGS_T_HURTBYFIST_OFFSET 0x0
#define DOS_OBJ_FLAGS_T_HURTBYFIST_SIZE 1
#define DOS_OBJ_FLAGS_T_HURTBYFIST_MASK 0x01
/* STRONG: Stored result of the current script test. */
#define DOS_OBJ_FLAGS_T_COMMAND_TEST_OFFSET 0x0
#define DOS_OBJ_FLAGS_T_COMMAND_TEST_SIZE 1
#define DOS_OBJ_FLAGS_T_COMMAND_TEST_MASK 0x02
/* PROVEN: Low saved object-state bit, cleared at timed animation completion. */
#define DOS_OBJ_FLAGS_T_ALIVE_OFFSET 0x0
#define DOS_OBJ_FLAGS_T_ALIVE_SIZE 1
#define DOS_OBJ_FLAGS_T_ALIVE_MASK 0x04
/* PROVEN: Horizontal object reflection/facing flag. */
#define DOS_OBJ_FLAGS_T_FLIP_X_OFFSET 0x0
#define DOS_OBJ_FLAGS_T_FLIP_X_SIZE 1
#define DOS_OBJ_FLAGS_T_FLIP_X_MASK 0x08
/* PROVEN: Per-object command mode used to select turn labels. */
#define DOS_OBJ_FLAGS_T_READ_COMMANDS_OFFSET 0x0
#define DOS_OBJ_FLAGS_T_READ_COMMANDS_SIZE 1
#define DOS_OBJ_FLAGS_T_READ_COMMANDS_MASK 0x10
/* STRONG: Enable support/follow-distance processing for the object. */
#define DOS_OBJ_FLAGS_T_FOLLOW_ENABLED_OFFSET 0x0
#define DOS_OBJ_FLAGS_T_FOLLOW_ENABLED_SIZE 1
#define DOS_OBJ_FLAGS_T_FOLLOW_ENABLED_MASK 0x20
/* STRONG: Water-entry/splash suppression flag in the generic movement path; also cleared by many spawned-object initializers. */
#define DOS_OBJ_FLAGS_T_FLAG_0X40_OFFSET 0x0
#define DOS_OBJ_FLAGS_T_FLAG_0X40_SIZE 1
#define DOS_OBJ_FLAGS_T_FLAG_0X40_MASK 0x40
/* STRONG: Flag used by animation update when the selected animation changes. */
#define DOS_OBJ_FLAGS_T_ANIM_CHANGED_OFFSET 0x0
#define DOS_OBJ_FLAGS_T_ANIM_CHANGED_SIZE 1
#define DOS_OBJ_FLAGS_T_ANIM_CHANGED_MASK 0x80
/* PROVEN: Ninth object flag written as movement-occurrence bookkeeping, with one original uninitialised producer. */
#define DOS_OBJ_FLAGS_T_FLAG_0X100_OFFSET 0x1
#define DOS_OBJ_FLAGS_T_FLAG_0X100_SIZE 1
#define DOS_OBJ_FLAGS_T_FLAG_0X100_MASK 0x01
/* STRONG: Saved script command-context component cmd_offset. */
#define DOS_CMD_CONTEXT_T_CMD_OFFSET_OFFSET 0x0
#define DOS_CMD_CONTEXT_T_CMD_OFFSET_SIZE 2
/* STRONG: Saved script command-context component count. */
#define DOS_CMD_CONTEXT_T_COUNT_OFFSET 0x2
#define DOS_CMD_CONTEXT_T_COUNT_SIZE 2
/* PROVEN: Base pointer of the packed level object array. */
#define DOS_LEVEL_T_OBJECTS_OFFSET 0x0
#define DOS_LEVEL_T_OBJECTS_SIZE 4
/* PROVEN: Count bounding the level object array. */
#define DOS_LEVEL_T_NB_OBJECTS_OFFSET 0x4
#define DOS_LEVEL_T_NB_OBJECTS_SIZE 2
/* PROVEN: Object-id sequence pointer shared by the level_obj activation-candidate descriptor and the separate level_alw always-type pool descriptor. */
#define DOS_LEVEL_OBJ_T_OBJ_IDS_OFFSET 0x0
#define DOS_LEVEL_OBJ_T_OBJ_IDS_SIZE 4
/* PROVEN: Signed16 counted length of the id sequence in either level_obj or level_alw. */
#define DOS_LEVEL_OBJ_T_NB_OBJECTS_OFFSET 0x4
#define DOS_LEVEL_OBJ_T_NB_OBJECTS_SIZE 2
/* PROVEN: Counted active-object id array with an additional -1 terminator written at the next slot. */
#define DOS_ACTIVE_OBJECTS_T_OBJECTS_OFFSET 0x0
#define DOS_ACTIVE_OBJECTS_T_OBJECTS_SIZE 200
/* PROVEN: Active-object list member num_active_objects. */
#define DOS_ACTIVE_OBJECTS_T_NUM_ACTIVE_OBJECTS_OFFSET 0xc8
#define DOS_ACTIVE_OBJECTS_T_NUM_ACTIVE_OBJECTS_SIZE 2
/* PROVEN: Overlapping raw/byte/nibble view raw of per-type behaviour flags. */
#define DOS_OBJ_TYPE_FLAGS_T_RAW_OFFSET 0x0
#define DOS_OBJ_TYPE_FLAGS_T_RAW_SIZE 4
/* PROVEN: Overlapping raw/byte/nibble view flags0 of per-type behaviour flags. */
#define DOS_OBJ_TYPE_FLAGS_T_FLAGS0_OFFSET 0x0
#define DOS_OBJ_TYPE_FLAGS_T_FLAGS0_SIZE 1
/* PROVEN: Overlapping raw/byte/nibble view flags1 of per-type behaviour flags. */
#define DOS_OBJ_TYPE_FLAGS_T_FLAGS1_OFFSET 0x1
#define DOS_OBJ_TYPE_FLAGS_T_FLAGS1_SIZE 1
/* PROVEN: Overlapping raw/byte/nibble view flags1_lo of per-type behaviour flags. */
#define DOS_OBJ_TYPE_FLAGS_T_FLAGS1_LO_OFFSET 0x1
#define DOS_OBJ_TYPE_FLAGS_T_FLAGS1_LO_SIZE 1
#define DOS_OBJ_TYPE_FLAGS_T_FLAGS1_LO_MASK 0x0f
/* PROVEN: Select fractional speed conversion for the corresponding object movement axis. */
#define DOS_OBJ_TYPE_FLAGS_T_SPEED_X_USES_SIXTEENTHS_OFFSET 0x1
#define DOS_OBJ_TYPE_FLAGS_T_SPEED_X_USES_SIXTEENTHS_SIZE 1
#define DOS_OBJ_TYPE_FLAGS_T_SPEED_X_USES_SIXTEENTHS_MASK 0x10
/* PROVEN: Select fractional speed conversion for the corresponding object movement axis. */
#define DOS_OBJ_TYPE_FLAGS_T_SPEED_Y_USES_SIXTEENTHS_OFFSET 0x1
#define DOS_OBJ_TYPE_FLAGS_T_SPEED_Y_USES_SIXTEENTHS_SIZE 1
#define DOS_OBJ_TYPE_FLAGS_T_SPEED_Y_USES_SIXTEENTHS_MASK 0x20
/* PROVEN: Overlapping raw/byte/nibble view flags1_hi of per-type behaviour flags. */
#define DOS_OBJ_TYPE_FLAGS_T_FLAGS1_HI_OFFSET 0x1
#define DOS_OBJ_TYPE_FLAGS_T_FLAGS1_HI_SIZE 1
#define DOS_OBJ_TYPE_FLAGS_T_FLAGS1_HI_MASK 0xc0
/* PROVEN: Overlapping raw/byte/nibble view flags2 of per-type behaviour flags. */
#define DOS_OBJ_TYPE_FLAGS_T_FLAGS2_OFFSET 0x2
#define DOS_OBJ_TYPE_FLAGS_T_FLAGS2_SIZE 1
/* PROVEN: Overlapping raw/byte/nibble view flags3 of per-type behaviour flags. */
#define DOS_OBJ_TYPE_FLAGS_T_FLAGS3_OFFSET 0x3
#define DOS_OBJ_TYPE_FLAGS_T_FLAGS3_SIZE 1
/* PROVEN: Map dimension width in16-pixel tiles. */
#define DOS_MAP_DATA_T_WIDTH_OFFSET 0x0
#define DOS_MAP_DATA_T_WIDTH_SIZE 2
/* PROVEN: Map dimension height in16-pixel tiles. */
#define DOS_MAP_DATA_T_HEIGHT_OFFSET 0x2
#define DOS_MAP_DATA_T_HEIGHT_SIZE 2
/* STRONG: Number of tile records used by full-map processing. */
#define DOS_MAP_DATA_T_LENGTH_OFFSET 0x4
#define DOS_MAP_DATA_T_LENGTH_SIZE 4
/* PROVEN: Pointer to row-major six-byte tile records. */
#define DOS_MAP_DATA_T_MAP_OFFSET 0x8
#define DOS_MAP_DATA_T_MAP_SIZE 4
/* STRONG: Index of a visual16x16 tile texture. */
#define DOS_MAP_TILE_T_TEXTURE_ID_OFFSET 0x0
#define DOS_MAP_TILE_T_TEXTURE_ID_SIZE 2
/* PROVEN: Collision/behaviour type byte returned by BTYP. */
#define DOS_MAP_TILE_T_TILE_TYPE_OFFSET 0x2
#define DOS_MAP_TILE_T_TILE_TYPE_SIZE 1
/* UNKNOWN: Raw serialized map byte+3 with no intended meaning assigned. */
#define DOS_MAP_TILE_T_FIELD_3_OFFSET 0x3
#define DOS_MAP_TILE_T_FIELD_3_SIZE 1
/* PROVEN: Computed texture coverage classification:0 opaque,1 empty,2 mixed. */
#define DOS_MAP_TILE_T_TRANSPARENCY_OFFSET 0x4
#define DOS_MAP_TILE_T_TRANSPARENCY_SIZE 1
/* UNKNOWN: Raw serialized map byte+5 with no intended meaning assigned. */
#define DOS_MAP_TILE_T_FIELD_5_OFFSET 0x5
#define DOS_MAP_TILE_T_FIELD_5_SIZE 1
/* PROVEN: Byte/bitfield views of block-type behaviour flags. */
#define DOS_BLOCK_FLAGS_T_RAW_OFFSET 0x0
#define DOS_BLOCK_FLAGS_T_RAW_SIZE 1
/* PROVEN: Byte/bitfield views of block-type behaviour flags. */
#define DOS_BLOCK_FLAGS_T_BITS_OFFSET 0x0
#define DOS_BLOCK_FLAGS_T_BITS_SIZE 1
/* UNKNOWN: Observed raw bit0 used by underSlope, stone-dog side probes and mounted Rayman side-strip probes; its universal collision-class meaning remains unresolved. */
#define DOS_BLOCK_FLAGS_T_BITS_UNUSED0_OFFSET 0x0
#define DOS_BLOCK_FLAGS_T_BITS_UNUSED0_SIZE 1
#define DOS_BLOCK_FLAGS_T_BITS_UNUSED0_MASK 0x01
/* PROVEN: Bit1 tested as the supporting/solid classification in block selection. */
#define DOS_BLOCK_FLAGS_T_BITS_SOLID_OFFSET 0x0
#define DOS_BLOCK_FLAGS_T_BITS_SOLID_SIZE 1
#define DOS_BLOCK_FLAGS_T_BITS_SOLID_MASK 0x02
/* UNKNOWN: Three-bit view of raw bits2..4; moteur tests bit3 together with bit6 for icy slopes, and bit4 for ceiling/wall blocking. Bit2 meaning is unresolved. */
#define DOS_BLOCK_FLAGS_T_BITS_UNUSED2_OFFSET 0x0
#define DOS_BLOCK_FLAGS_T_BITS_UNUSED2_SIZE 1
#define DOS_BLOCK_FLAGS_T_BITS_UNUSED2_MASK 0x1c
/* PROVEN: Bit5 of the probed block flags, returned by the turn-marker test. */
#define DOS_BLOCK_FLAGS_T_BITS_CHDIR_OFFSET 0x0
#define DOS_BLOCK_FLAGS_T_BITS_CHDIR_SIZE 1
#define DOS_BLOCK_FLAGS_T_BITS_CHDIR_MASK 0x20
/* STRONG: Two-bit slope classification/orientation view used during Rayman tile landing. */
#define DOS_BLOCK_FLAGS_T_BITS_UNUSED1_OFFSET 0x0
#define DOS_BLOCK_FLAGS_T_BITS_UNUSED1_SIZE 1
#define DOS_BLOCK_FLAGS_T_BITS_UNUSED1_MASK 0xc0
/* STRONG: Enable the fist-throwing ability. */
#define DOS_RAYEVTS_T_POING_OFFSET 0x0
#define DOS_RAYEVTS_T_POING_SIZE 1
#define DOS_RAYEVTS_T_POING_MASK 0x01
/* STRONG: Enable the ledge-hanging ability. */
#define DOS_RAYEVTS_T_HANG_OFFSET 0x0
#define DOS_RAYEVTS_T_HANG_SIZE 1
#define DOS_RAYEVTS_T_HANG_MASK 0x02
/* STRONG: Enable normal/enhanced helicopter response (helico). */
#define DOS_RAYEVTS_T_HELICO_OFFSET 0x0
#define DOS_RAYEVTS_T_HELICO_SIZE 1
#define DOS_RAYEVTS_T_HELICO_MASK 0x04
/* STRONG: Enable normal/enhanced helicopter response (super_helico). */
#define DOS_RAYEVTS_T_SUPER_HELICO_OFFSET 0x0
#define DOS_RAYEVTS_T_SUPER_HELICO_SIZE 1
#define DOS_RAYEVTS_T_SUPER_HELICO_MASK 0x08
/* UNKNOWN: Neutral rayevts_t.handstand_dash storage/view; its intended meaning is not established by this pilot. */
#define DOS_RAYEVTS_T_HANDSTAND_DASH_OFFSET 0x0
#define DOS_RAYEVTS_T_HANDSTAND_DASH_SIZE 1
#define DOS_RAYEVTS_T_HANDSTAND_DASH_MASK 0x10
/* UNKNOWN: Neutral rayevts_t.handstand storage/view; its intended meaning is not established by this pilot. */
#define DOS_RAYEVTS_T_HANDSTAND_OFFSET 0x0
#define DOS_RAYEVTS_T_HANDSTAND_SIZE 1
#define DOS_RAYEVTS_T_HANDSTAND_MASK 0x20
/* STRONG: Enable the level-specific magic-seed ability. */
#define DOS_RAYEVTS_T_MAGICSEED_OFFSET 0x0
#define DOS_RAYEVTS_T_MAGICSEED_SIZE 1
#define DOS_RAYEVTS_T_MAGICSEED_MASK 0x40
/* STRONG: Enable grabbing/swinging through the grappling interaction. */
#define DOS_RAYEVTS_T_GRAP_OFFSET 0x0
#define DOS_RAYEVTS_T_GRAP_SIZE 1
#define DOS_RAYEVTS_T_GRAP_MASK 0x80
/* STRONG: Enable Rayman running behaviour. */
#define DOS_RAYEVTS_T_RUN_OFFSET 0x1
#define DOS_RAYEVTS_T_RUN_SIZE 1
#define DOS_RAYEVTS_T_RUN_MASK 0x01
/* STRONG: Rayman small-size mode/power bit. */
#define DOS_RAYEVTS_T_TINY_OFFSET 0x1
#define DOS_RAYEVTS_T_TINY_SIZE 1
#define DOS_RAYEVTS_T_TINY_MASK 0x02
/* STRONG: Enable the firefly/light companion mode. */
#define DOS_RAYEVTS_T_FIREFLY_OFFSET 0x1
#define DOS_RAYEVTS_T_FIREFLY_SIZE 1
#define DOS_RAYEVTS_T_FIREFLY_MASK 0x04
/* PROVEN: Two-bit forced-running lifecycle state. */
#define DOS_RAYEVTS_T_FORCE_RUN_OFFSET 0x1
#define DOS_RAYEVTS_T_FORCE_RUN_SIZE 1
#define DOS_RAYEVTS_T_FORCE_RUN_MASK 0x18
/* PROVEN: Two-bit reversed-controls lifecycle state whose nonzero values swap horizontal input directions. */
#define DOS_RAYEVTS_T_REVERSE_OFFSET 0x1
#define DOS_RAYEVTS_T_REVERSE_SIZE 1
#define DOS_RAYEVTS_T_REVERSE_MASK 0x60
/* STRONG: Reduced/squashed Rayman mode coupled to tiny and object/ability-state swapping. */
#define DOS_RAYEVTS_T_SQUASHED_OFFSET 0x1
#define DOS_RAYEVTS_T_SQUASHED_SIZE 1
#define DOS_RAYEVTS_T_SQUASHED_MASK 0x80
/* STRONG: Progression flag for completion of the bzzit encounter. */
#define DOS_FINBOSSLEVEL_T_BZZIT_OFFSET 0x0
#define DOS_FINBOSSLEVEL_T_BZZIT_SIZE 1
#define DOS_FINBOSSLEVEL_T_BZZIT_MASK 0x01
/* STRONG: Progression flag for completion of the moskito encounter. */
#define DOS_FINBOSSLEVEL_T_MOSKITO_OFFSET 0x0
#define DOS_FINBOSSLEVEL_T_MOSKITO_SIZE 1
#define DOS_FINBOSSLEVEL_T_MOSKITO_MASK 0x02
/* STRONG: Progression flag for completion of the mr_sax encounter. */
#define DOS_FINBOSSLEVEL_T_MR_SAX_OFFSET 0x0
#define DOS_FINBOSSLEVEL_T_MR_SAX_SIZE 1
#define DOS_FINBOSSLEVEL_T_MR_SAX_MASK 0x04
/* STRONG: Progression flag for completion of the mr_stone encounter. */
#define DOS_FINBOSSLEVEL_T_MR_STONE_OFFSET 0x0
#define DOS_FINBOSSLEVEL_T_MR_STONE_SIZE 1
#define DOS_FINBOSSLEVEL_T_MR_STONE_MASK 0x08
/* STRONG: Progression flag for completion of the viking_mama encounter. */
#define DOS_FINBOSSLEVEL_T_VIKING_MAMA_OFFSET 0x0
#define DOS_FINBOSSLEVEL_T_VIKING_MAMA_SIZE 1
#define DOS_FINBOSSLEVEL_T_VIKING_MAMA_MASK 0x10
/* STRONG: Progression flag for completion of the space_mama encounter. */
#define DOS_FINBOSSLEVEL_T_SPACE_MAMA_OFFSET 0x0
#define DOS_FINBOSSLEVEL_T_SPACE_MAMA_SIZE 1
#define DOS_FINBOSSLEVEL_T_SPACE_MAMA_MASK 0x20
/* STRONG: Progression flag for completion of the mr_skops encounter. */
#define DOS_FINBOSSLEVEL_T_MR_SKOPS_OFFSET 0x0
#define DOS_FINBOSSLEVEL_T_MR_SKOPS_SIZE 1
#define DOS_FINBOSSLEVEL_T_MR_SKOPS_MASK 0x40
/* STRONG: Progression flag for completion of the mr_dark encounter. */
#define DOS_FINBOSSLEVEL_T_MR_DARK_OFFSET 0x0
#define DOS_FINBOSSLEVEL_T_MR_DARK_SIZE 1
#define DOS_FINBOSSLEVEL_T_MR_DARK_MASK 0x80
/* UNKNOWN: Neutral finbosslevel_t.crazy_drummer storage/view; its intended meaning is not established by this pilot. */
#define DOS_FINBOSSLEVEL_T_CRAZY_DRUMMER_OFFSET 0x1
#define DOS_FINBOSSLEVEL_T_CRAZY_DRUMMER_SIZE 1
#define DOS_FINBOSSLEVEL_T_CRAZY_DRUMMER_MASK 0x01
/* STRONG: Progression flag for the corresponding help event (helped_joe_1). */
#define DOS_FINBOSSLEVEL_T_HELPED_JOE_1_OFFSET 0x1
#define DOS_FINBOSSLEVEL_T_HELPED_JOE_1_SIZE 1
#define DOS_FINBOSSLEVEL_T_HELPED_JOE_1_MASK 0x02
/* STRONG: Progression flag for the corresponding help event (helped_joe_2). */
#define DOS_FINBOSSLEVEL_T_HELPED_JOE_2_OFFSET 0x1
#define DOS_FINBOSSLEVEL_T_HELPED_JOE_2_SIZE 1
#define DOS_FINBOSSLEVEL_T_HELPED_JOE_2_MASK 0x04
/* STRONG: Progression flag for the corresponding help event (helped_musician). */
#define DOS_FINBOSSLEVEL_T_HELPED_MUSICIAN_OFFSET 0x1
#define DOS_FINBOSSLEVEL_T_HELPED_MUSICIAN_SIZE 1
#define DOS_FINBOSSLEVEL_T_HELPED_MUSICIAN_MASK 0x08
/* PROVEN: Fist signed32 vertical position accumulator scaled by16, separately maintained from integer obj.y. */
#define DOS_POING_T_Y_16_OFFSET 0x0
#define DOS_POING_T_Y_16_SIZE 4
/* PROVEN: Launch-time snapshot of charge shifted left eight, written alongside the outbound charge budget. A further live consumer is not established. */
#define DOS_POING_T_FIELD_4_OFFSET 0x4
#define DOS_POING_T_FIELD_4_SIZE 2
/* PROVEN: Stored positive horizontal fist speed used to launch and constrain return speed. */
#define DOS_POING_T_SPEED_X_OFFSET 0x6
#define DOS_POING_T_SPEED_X_SIZE 2
/* PROVEN: Fist charge counter before throwing, then scaled outbound budget decremented during flight. */
#define DOS_POING_T_CHARGE_OFFSET 0x8
#define DOS_POING_T_CHARGE_SIZE 2
/* PROVEN: Temporary throw-range denominator then decrement applied to the scaled outbound charge budget. */
#define DOS_POING_T_FIELD_A_OFFSET 0xa
#define DOS_POING_T_FIELD_A_SIZE 2
/* STRONG: Saved/selected fist upgrade substate copied into fist objects and save data. */
#define DOS_POING_T_SUB_ETAT_OFFSET 0xc
#define DOS_POING_T_SUB_ETAT_SIZE 1
/* PROVEN: Select the homing-return phase of fist flight. */
#define DOS_POING_T_IS_RETURNING_OFFSET 0xd
#define DOS_POING_T_IS_RETURNING_SIZE 1
/* PROVEN: Enable fist update/collision processing. */
#define DOS_POING_T_IS_ACTIVE_OFFSET 0xe
#define DOS_POING_T_IS_ACTIVE_SIZE 1
/* PROVEN: Current fist damage applied by obj_hurt. */
#define DOS_POING_T_DAMAGE_OFFSET 0xf
#define DOS_POING_T_DAMAGE_SIZE 1
/* STRONG: Fist charge-preparation flag. */
#define DOS_POING_T_IS_CHARGING_OFFSET 0x10
#define DOS_POING_T_IS_CHARGING_SIZE 1
/* STRONG: Fist impact/explosion interaction state. */
#define DOS_POING_T_IS_BOUM_OFFSET 0x11
#define DOS_POING_T_IS_BOUM_SIZE 1
/* UNKNOWN: Neutral poing_t.field_12 storage/view; its intended meaning is not established by this pilot. */
#define DOS_POING_T_FIELD_12_OFFSET 0x12
#define DOS_POING_T_FIELD_12_SIZE 1
/* UNKNOWN: Neutral poing_t.field_13 storage/view; its intended meaning is not established by this pilot. */
#define DOS_POING_T_FIELD_13_OFFSET 0x13
#define DOS_POING_T_FIELD_13_SIZE 1
/* PROVEN: Base sprite table for the corresponding font bank. */
#define DOS_FONT_T_SPRITES_OFFSET 0x0
#define DOS_FONT_T_SPRITES_SIZE 4
/* STRONG: Font image buffer or sprite count (img_buffer) copied from loaded object descriptors. */
#define DOS_FONT_T_IMG_BUFFER_OFFSET 0x4
#define DOS_FONT_T_IMG_BUFFER_SIZE 4
/* STRONG: Font image buffer or sprite count (nb_sprites) copied from loaded object descriptors. */
#define DOS_FONT_T_NB_SPRITES_OFFSET 0x8
#define DOS_FONT_T_NB_SPRITES_SIZE 2
/* PROVEN: Fixed200-byte text buffer interpreted by the game glyph decoder. */
#define DOS_DISPLAY_ITEM_T_TEXT_OFFSET 0x0
#define DOS_DISPLAY_ITEM_T_TEXT_SIZE 200
/* PROVEN: Computed text-box rectangle component centered_x_pos. */
#define DOS_DISPLAY_ITEM_T_CENTERED_X_POS_OFFSET 0xc8
#define DOS_DISPLAY_ITEM_T_CENTERED_X_POS_SIZE 2
/* PROVEN: Computed text-box rectangle component centered_y_pos. */
#define DOS_DISPLAY_ITEM_T_CENTERED_Y_POS_OFFSET 0xca
#define DOS_DISPLAY_ITEM_T_CENTERED_Y_POS_SIZE 2
/* PROVEN: Computed text-box rectangle component width. */
#define DOS_DISPLAY_ITEM_T_WIDTH_OFFSET 0xcc
#define DOS_DISPLAY_ITEM_T_WIDTH_SIZE 2
/* PROVEN: Computed text-box rectangle component height. */
#define DOS_DISPLAY_ITEM_T_HEIGHT_OFFSET 0xce
#define DOS_DISPLAY_ITEM_T_HEIGHT_SIZE 2
/* PROVEN: Text anchor coordinate xpos. */
#define DOS_DISPLAY_ITEM_T_XPOS_OFFSET 0xd0
#define DOS_DISPLAY_ITEM_T_XPOS_SIZE 2
/* PROVEN: Text anchor coordinate ypos. */
#define DOS_DISPLAY_ITEM_T_YPOS_OFFSET 0xd2
#define DOS_DISPLAY_ITEM_T_YPOS_SIZE 2
/* PROVEN: Font selector controlling bank and line metrics. */
#define DOS_DISPLAY_ITEM_T_FONT_SIZE_OFFSET 0xd4
#define DOS_DISPLAY_ITEM_T_FONT_SIZE_SIZE 1
/* PROVEN: Observed filled-background/text-offset selector in box rendering;broader intended label unresolved. */
#define DOS_DISPLAY_ITEM_T_FIELD_D5_OFFSET 0xd5
#define DOS_DISPLAY_ITEM_T_FIELD_D5_SIZE 1
/* PROVEN: Box drawing/position mode passed to DISPLAY_BLACKBOX and used to offset text by +/-1. */
#define DOS_DISPLAY_ITEM_T_IS_FOND_OFFSET 0xd6
#define DOS_DISPLAY_ITEM_T_IS_FOND_SIZE 1
/* STRONG: Text palette/color selector passed to display_text. */
#define DOS_DISPLAY_ITEM_T_COLOR_OFFSET 0xd7
#define DOS_DISPLAY_ITEM_T_COLOR_SIZE 1
#define DOS_STATUS_BAR_T_SIZE 10
#define DOS_SAVE_STATE_T_SIZE 604
#define DOS_RECORD_T_SIZE 24
#define DOS_MEM_T_SIZE 12
#define DOS_PIX_GERBE_ITEM_T_SIZE 10
#define DOS_PIX_GERBE_T_SIZE 642
#define DOS_OBJ_PROCS_T_SIZE 20
#define DOS_FLOCON_T_SIZE 8
/* PROVEN: Signed life counter with -1 display sentinel and upper gameplay cap99. */
#define DOS_STATUS_BAR_T_LIVES_OFFSET 0x0
#define DOS_STATUS_BAR_T_LIVES_SIZE 2
/* PROVEN: Tens then units decimal display digits of lives. */
#define DOS_STATUS_BAR_T_LIVES_DIGITS_OFFSET 0x2
#define DOS_STATUS_BAR_T_LIVES_DIGITS_SIZE 2
/* PROVEN: Two HUD health-sprite selectors, slot1 for the first two health points and slot0 for extra health. */
#define DOS_STATUS_BAR_T_HP_SPRITES_OFFSET 0x4
#define DOS_STATUS_BAR_T_HP_SPRITES_SIZE 2
/* PROVEN: Collected tings count used by the HUD and bonus/checkpoint rules. */
#define DOS_STATUS_BAR_T_NUM_WIZ_OFFSET 0x6
#define DOS_STATUS_BAR_T_NUM_WIZ_SIZE 1
/* PROVEN: Tens then units decimal display digits for the tings count. */
#define DOS_STATUS_BAR_T_WIZ_DIGITS_OFFSET 0x7
#define DOS_STATUS_BAR_T_WIZ_DIGITS_SIZE 2
/* PROVEN: HUD maximum health class, normally2 or4. */
#define DOS_STATUS_BAR_T_MAX_HITP_OFFSET 0x9
#define DOS_STATUS_BAR_T_MAX_HITP_SIZE 1
/* PROVEN: Eight32-bit alive masks for types141/164/179/242 only;index object ID>>5,bit1<<(ID&31). Other bits are not cleared by capture. */
#define DOS_SAVE_STATE_T_TRIGGERED_OBJECTS_OFFSET 0x0
#define DOS_SAVE_STATE_T_TRIGGERED_OBJECTS_SIZE 32
/* PROVEN: Saved eight weather-layer flake counts. */
#define DOS_SAVE_STATE_T_NB_FLOC_OFFSET 0x20
#define DOS_SAVE_STATE_T_NB_FLOC_SIZE 16
/* PROVEN: Saved VENT_X weather/wind value. */
#define DOS_SAVE_STATE_T_VENT_X_OFFSET 0x30
#define DOS_SAVE_STATE_T_VENT_X_SIZE 2
/* PROVEN: Saved VENT_Y weather/wind value. */
#define DOS_SAVE_STATE_T_VENT_Y_OFFSET 0x32
#define DOS_SAVE_STATE_T_VENT_Y_SIZE 2
/* PROVEN: Saved horizontal camera origin. */
#define DOS_SAVE_STATE_T_X_MAP_OFFSET 0x34
#define DOS_SAVE_STATE_T_X_MAP_SIZE 2
/* PROVEN: Saved vertical camera origin. */
#define DOS_SAVE_STATE_T_Y_MAP_OFFSET 0x36
#define DOS_SAVE_STATE_T_Y_MAP_SIZE 2
/* PROVEN: Saved Rayman world x narrowed to signed16. */
#define DOS_SAVE_STATE_T_RAY_X_POS_OFFSET 0x38
#define DOS_SAVE_STATE_T_RAY_X_POS_SIZE 2
/* PROVEN: Saved Rayman world y narrowed to signed16. */
#define DOS_SAVE_STATE_T_RAY_Y_POS_OFFSET 0x3a
#define DOS_SAVE_STATE_T_RAY_Y_POS_SIZE 2
/* PROVEN: Saved Rayman screen x. */
#define DOS_SAVE_STATE_T_RAY_SCREEN_X_OFFSET 0x3c
#define DOS_SAVE_STATE_T_RAY_SCREEN_X_SIZE 2
/* PROVEN: Saved Rayman screen y. */
#define DOS_SAVE_STATE_T_RAY_SCREEN_Y_OFFSET 0x3e
#define DOS_SAVE_STATE_T_RAY_SCREEN_Y_SIZE 2
/* PROVEN: Saved Rayman facing flag widened to u16. */
#define DOS_SAVE_STATE_T_RAY_FLIP_X_OFFSET 0x40
#define DOS_SAVE_STATE_T_RAY_FLIP_X_SIZE 2
/* PROVEN: Saved checkpoint-object world x narrowed to signed16. */
#define DOS_SAVE_STATE_T_SAVE_OBJ_X_POS_OFFSET 0x42
#define DOS_SAVE_STATE_T_SAVE_OBJ_X_POS_SIZE 2
/* PROVEN: Saved checkpoint-object world y narrowed to signed16. */
#define DOS_SAVE_STATE_T_SAVE_OBJ_Y_POS_OFFSET 0x44
#define DOS_SAVE_STATE_T_SAVE_OBJ_Y_POS_SIZE 2
/* PROVEN: Checkpoint-object id, with-1 when saveGameState receives no object. */
#define DOS_SAVE_STATE_T_SAVE_OBJ_ID_OFFSET 0x46
#define DOS_SAVE_STATE_T_SAVE_OBJ_ID_SIZE 2
/* PROVEN: Snapshot of level.nb_objects successor IDs;restore applies only when save_state==&save1 and sets link_has_gendoor by successor!=self. */
#define DOS_SAVE_STATE_T_LINK_INIT_OFFSET 0x48
#define DOS_SAVE_STATE_T_LINK_INIT_SIZE 512
/* PROVEN: Saved checkpoint-object detection-zone classification. */
#define DOS_SAVE_STATE_T_SAVE_OBJ_DETECT_ZONE_FLAG_OFFSET 0x248
#define DOS_SAVE_STATE_T_SAVE_OBJ_DETECT_ZONE_FLAG_SIZE 1
/* PROVEN: Saved checkpoint-object hurtbyfist bit, despite the neutral numbered spelling. */
#define DOS_SAVE_STATE_T_SAVE_OBJ_FLAG_1_OFFSET 0x249
#define DOS_SAVE_STATE_T_SAVE_OBJ_FLAG_1_SIZE 1
/* PROVEN: Saved five Rayman tile-collision sample types. */
#define DOS_SAVE_STATE_T_RAY_COLL_BTYPE_OFFSET 0x24a
#define DOS_SAVE_STATE_T_RAY_COLL_BTYPE_SIZE 5
/* PROVEN: Saved Rayman animation selector. */
#define DOS_SAVE_STATE_T_RAY_ANIM_INDEX_OFFSET 0x24f
#define DOS_SAVE_STATE_T_RAY_ANIM_INDEX_SIZE 1
/* PROVEN: Saved Rayman animation frame selector. */
#define DOS_SAVE_STATE_T_RAY_ANIM_FRAME_OFFSET 0x250
#define DOS_SAVE_STATE_T_RAY_ANIM_FRAME_SIZE 1
/* PROVEN: Saved Rayman main state. */
#define DOS_SAVE_STATE_T_RAY_MAIN_ETAT_OFFSET 0x251
#define DOS_SAVE_STATE_T_RAY_MAIN_ETAT_SIZE 1
/* PROVEN: Saved Rayman substate. */
#define DOS_SAVE_STATE_T_RAY_SUB_ETAT_OFFSET 0x252
#define DOS_SAVE_STATE_T_RAY_SUB_ETAT_SIZE 1
/* PROVEN: [sem_objs; moteur.c; PROVEN] Saved fist power/substate selection.
[sem_flow; moteur.c; PROVEN] Checkpoint fist state;restoring save1 degrades5/12->3 and1/3/8/10->1 by mutating saved value before applying to fist. */
#define DOS_SAVE_STATE_T_POING_SUB_ETAT_OFFSET 0x253
#define DOS_SAVE_STATE_T_POING_SUB_ETAT_SIZE 1
/* PROVEN: Saved two-bit RayEvts.reverse value widened to a byte. */
#define DOS_SAVE_STATE_T_RAYEVTS_REVERSE_OFFSET 0x254
#define DOS_SAVE_STATE_T_RAYEVTS_REVERSE_SIZE 1
/* PROVEN: Saved RayEvts.super_helico bit. */
#define DOS_SAVE_STATE_T_RAYEVTS_SUPER_HELICO_OFFSET 0x255
#define DOS_SAVE_STATE_T_RAYEVTS_SUPER_HELICO_SIZE 1
/* PROVEN: Saved RayEvts.poing ability bit. */
#define DOS_SAVE_STATE_T_RAYEVTS_POING_OFFSET 0x256
#define DOS_SAVE_STATE_T_RAYEVTS_POING_SIZE 1
/* PROVEN: Saved palette selector. */
#define DOS_SAVE_STATE_T_CURRENT_PAL_ID_OFFSET 0x257
#define DOS_SAVE_STATE_T_CURRENT_PAL_ID_SIZE 1
/* PROVEN: Saved entry/death sequence counter. */
#define DOS_SAVE_STATE_T_DEAD_TIME_OFFSET 0x258
#define DOS_SAVE_STATE_T_DEAD_TIME_SIZE 1
/* PROVEN: Checkpoint-valid marker set after a save image is filled. */
#define DOS_SAVE_STATE_T_IS_JUST_SAVED_OFFSET 0x259
#define DOS_SAVE_STATE_T_IS_JUST_SAVED_SIZE 1
/* PROVEN: Saved HUD tings count. */
#define DOS_SAVE_STATE_T_STATUS_BAR_TINGS_OFFSET 0x25a
#define DOS_SAVE_STATE_T_STATUS_BAR_TINGS_SIZE 1
/* UNKNOWN: Trailing checkpoint byte with no typed reader/writer found. */
#define DOS_SAVE_STATE_T_FIELD_25B_OFFSET 0x25b
#define DOS_SAVE_STATE_T_FIELD_25B_SIZE 1
/* PROVEN: Byte offset of current repeat-length byte after startup;first frame consumes data0 buttons and leaves offset1. Subsequent runs advance2. */
#define DOS_RECORD_T_CURRENT_OFFSET_OFFSET 0x0
#define DOS_RECORD_T_CURRENT_OFFSET_SIZE 4
/* PROVEN: Second byte of current input pair;repeat_index equality ends run. Zero length is not automatically treated as immediate termination. */
#define DOS_RECORD_T_REPEAT_LENGTH_OFFSET 0x4
#define DOS_RECORD_T_REPEAT_LENGTH_SIZE 4
/* PROVEN: Incrementing32-bit index into current repeated run;startup/new run resets0. */
#define DOS_RECORD_T_REPEAT_INDEX_OFFSET 0x8
#define DOS_RECORD_T_REPEAT_INDEX_SIZE 4
/* PROVEN: REC file length narrowed through signed16 by read_record then stored in record;used to bound next pair. */
#define DOS_RECORD_T_LENGTH_OFFSET 0xc
#define DOS_RECORD_T_LENGTH_SIZE 4
/* PROVEN: Allocated packed input pairs(button byte,repeat-length byte);no native pointer is serialized by read_record. */
#define DOS_RECORD_T_DATA_OFFSET 0x10
#define DOS_RECORD_T_DATA_SIZE 4
/* PROVEN: Nonzero while demo input recording is enabled. */
#define DOS_RECORD_T_IS_RECORDING_OFFSET 0x14
#define DOS_RECORD_T_IS_RECORDING_SIZE 1
/* PROVEN: Nonzero while recorded demo input is being played. */
#define DOS_RECORD_T_IS_PLAYING_OFFSET 0x15
#define DOS_RECORD_T_IS_PLAYING_SIZE 1
/* PROVEN: Request to stop demo playback at the current repeat-run boundary. */
#define DOS_RECORD_T_IS_FINISHED_OFFSET 0x16
#define DOS_RECORD_T_IS_FINISHED_SIZE 1
/* UNKNOWN: Declared record_t.unknown_17 storage; moteur only clears its enclosing storage or never accesses it. */
#define DOS_RECORD_T_UNKNOWN_17_OFFSET 0x17
#define DOS_RECORD_T_UNKNOWN_17_SIZE 1
/* PROVEN: Used-byte bump offset relative to arena data, despite the alignment-oriented field name. */
#define DOS_MEM_T_ALIGN_OFFSET 0x0
#define DOS_MEM_T_ALIGN_SIZE 4
/* PROVEN: Arena base allocation pointer, with the live bump offset stored separately in align. */
#define DOS_MEM_T_DATA_OFFSET 0x4
#define DOS_MEM_T_DATA_SIZE 4
/* PROVEN: Total arena byte capacity against which the bump offset is checked. */
#define DOS_MEM_T_CAPACITY_OFFSET 0x8
#define DOS_MEM_T_CAPACITY_SIZE 4
/* PROVEN: Particle x position relative to the current displayed scene. */
#define DOS_PIX_GERBE_ITEM_T_X_POS_OFFSET 0x0
#define DOS_PIX_GERBE_ITEM_T_X_POS_SIZE 2
/* PROVEN: Particle y position relative to the current displayed scene. */
#define DOS_PIX_GERBE_ITEM_T_Y_POS_OFFSET 0x2
#define DOS_PIX_GERBE_ITEM_T_Y_POS_SIZE 2
/* PROVEN: Horizontal particle displacement per update. */
#define DOS_PIX_GERBE_ITEM_T_SPEED_X_OFFSET 0x4
#define DOS_PIX_GERBE_ITEM_T_SPEED_X_SIZE 2
/* PROVEN: Vertical particle displacement per update. */
#define DOS_PIX_GERBE_ITEM_T_SPEED_Y_OFFSET 0x6
#define DOS_PIX_GERBE_ITEM_T_SPEED_Y_SIZE 2
/* PROVEN: Increment of the particle vertical speed each update. */
#define DOS_PIX_GERBE_ITEM_T_Y_ACCEL_OFFSET 0x8
#define DOS_PIX_GERBE_ITEM_T_Y_ACCEL_SIZE 1
/* PROVEN: Observed particle-active high bit plus drawing color bias in low7 bits. */
#define DOS_PIX_GERBE_ITEM_T_UNK_1_OFFSET 0x9
#define DOS_PIX_GERBE_ITEM_T_UNK_1_SIZE 1
/* PROVEN: 64-item particle storage view. */
#define DOS_PIX_GERBE_T_ITEMS_OFFSET 0x0
#define DOS_PIX_GERBE_T_ITEMS_SIZE 640
/* PROVEN: 320-word alias of the same640-byte particle item storage. */
#define DOS_PIX_GERBE_T_WORDS_OFFSET 0x0
#define DOS_PIX_GERBE_T_WORDS_SIZE 640
/* PROVEN: Signed16 active marker for a particle batch. */
#define DOS_PIX_GERBE_T_IS_ACTIVE_OFFSET 0x280
#define DOS_PIX_GERBE_T_IS_ACTIVE_SIZE 2
/* PROVEN: Object command/update callback. */
#define DOS_OBJ_PROCS_T_COMMAND_OFFSET 0x0
#define DOS_OBJ_PROCS_T_COMMAND_SIZE 4
/* PROVEN: Object fist-collision callback receiving a signed16 sprite selector. */
#define DOS_OBJ_PROCS_T_POING_COLLISION_OFFSET 0x4
#define DOS_OBJ_PROCS_T_POING_COLLISION_SIZE 4
/* PROVEN: Object Rayman-collision callback. */
#define DOS_OBJ_PROCS_T_RAYMAN_COLLISION_OFFSET 0x8
#define DOS_OBJ_PROCS_T_RAYMAN_COLLISION_SIZE 4
/* PROVEN: Object Rayman-in-detection-zone callback. */
#define DOS_OBJ_PROCS_T_RAYMAN_IN_ZONE_OFFSET 0xc
#define DOS_OBJ_PROCS_T_RAYMAN_IN_ZONE_SIZE 4
/* STRONG: Fifth callback slot, initialized with turn-handler function pointers. */
#define DOS_OBJ_PROCS_T_U_TURN_OFFSET 0x10
#define DOS_OBJ_PROCS_T_U_TURN_SIZE 4
/* PROVEN: Weather particle x coordinate before perspective projection. */
#define DOS_FLOCON_T_UNPROJECTED_X_OFFSET 0x0
#define DOS_FLOCON_T_UNPROJECTED_X_SIZE 2
/* PROVEN: Weather particle y coordinate before perspective projection. */
#define DOS_FLOCON_T_UNPROJECTED_Y_OFFSET 0x2
#define DOS_FLOCON_T_UNPROJECTED_Y_SIZE 2
/* PROVEN: Perspective depth/scale parameter stored per weather particle and passed to projection by the drawing readers. */
#define DOS_FLOCON_T_FIELD_4_OFFSET 0x4
#define DOS_FLOCON_T_FIELD_4_SIZE 2
/* UNKNOWN: Declared flocon_t.field_6 storage; moteur only clears its enclosing storage or never accesses it. */
#define DOS_FLOCON_T_FIELD_6_OFFSET 0x6
#define DOS_FLOCON_T_FIELD_6_SIZE 1
/* UNKNOWN: Declared flocon_t.field_7 storage; moteur only clears its enclosing storage or never accesses it. */
#define DOS_FLOCON_T_FIELD_7_OFFSET 0x7
#define DOS_FLOCON_T_FIELD_7_SIZE 1
#define DOS_ZDC_T_SIZE 8
#define DOS_OPTIONS_JEU_T_SIZE 32
#define DOS_RAY_STACK_T_SIZE 18
/* PROVEN: Collision-zone horizontal offset in the selected coordinate frame. */
#define DOS_ZDC_T_X_POS_OFFSET 0x0
#define DOS_ZDC_T_X_POS_SIZE 2
/* PROVEN: Collision-zone vertical offset in the selected coordinate frame. */
#define DOS_ZDC_T_Y_POS_OFFSET 0x2
#define DOS_ZDC_T_Y_POS_SIZE 2
/* PROVEN: Unsigned horizontal span of this collision rectangle. */
#define DOS_ZDC_T_WIDTH_OFFSET 0x4
#define DOS_ZDC_T_WIDTH_SIZE 1
/* PROVEN: Unsigned vertical span of this collision rectangle. */
#define DOS_ZDC_T_HEIGHT_OFFSET 0x5
#define DOS_ZDC_T_HEIGHT_SIZE 1
/* PROVEN: Collision-zone mode flags. */
#define DOS_ZDC_T_FLAGS_OFFSET 0x6
#define DOS_ZDC_T_FLAGS_SIZE 1
/* PROVEN: Animation-layer anchor index and collision-result code. */
#define DOS_ZDC_T_SPRITE_OFFSET 0x7
#define DOS_ZDC_T_SPRITE_SIZE 1
/* STRONG: Current jump input-test callback. */
#define DOS_OPTIONS_JEU_T_TEST_FIRE1_OFFSET 0x0
#define DOS_OPTIONS_JEU_T_TEST_FIRE1_SIZE 4
/* STRONG: Current fist input-test callback. */
#define DOS_OPTIONS_JEU_T_TEST_FIRE0_OFFSET 0x4
#define DOS_OPTIONS_JEU_T_TEST_FIRE0_SIZE 4
/* STRONG: Current empty fourth-action input-test callback. */
#define DOS_OPTIONS_JEU_T_TEST_BUTTON4_OFFSET 0x8
#define DOS_OPTIONS_JEU_T_TEST_BUTTON4_SIZE 4
/* STRONG: Current action/run/seed input-test callback. */
#define DOS_OPTIONS_JEU_T_TEST_BUTTON3_OFFSET 0xc
#define DOS_OPTIONS_JEU_T_TEST_BUTTON3_SIZE 4
/* PROVEN: Physical/logical button selector for test_fire1. */
#define DOS_OPTIONS_JEU_T_JUMP_OFFSET 0x10
#define DOS_OPTIONS_JEU_T_JUMP_SIZE 2
/* PROVEN: Physical/logical button selector for test_fire0. */
#define DOS_OPTIONS_JEU_T_FIST_OFFSET 0x12
#define DOS_OPTIONS_JEU_T_FIST_SIZE 2
/* PROVEN: Physical/logical button selector for test_button4. */
#define DOS_OPTIONS_JEU_T_FIELD_14_OFFSET 0x14
#define DOS_OPTIONS_JEU_T_FIELD_14_SIZE 2
/* PROVEN: Physical/logical button selector for test_button3. */
#define DOS_OPTIONS_JEU_T_ACTION_OFFSET 0x16
#define DOS_OPTIONS_JEU_T_ACTION_SIZE 2
/* PROVEN: Persistent music enable value;menu toggles logical zero/nonzero and controls MusicCdActive only when OptionMusicCdActive permits. Historical default13 is retained rather than normalized. */
#define DOS_OPTIONS_JEU_T_MUSIC_ENABLED_OFFSET 0x18
#define DOS_OPTIONS_JEU_T_MUSIC_ENABLED_SIZE 2
/* PROVEN: Persistent sound-volume value passed to raj_env_sound;menu working copy adjusts0..max_sound. CFG stores16 bits;historical default18. */
#define DOS_OPTIONS_JEU_T_SOUND_VOLUME_OFFSET 0x1a
#define DOS_OPTIONS_JEU_T_SOUND_VOLUME_SIZE 2
/* PROVEN: Persistent stereo setting passed to raj_env_stereo;CFG stores16 bits,default1. */
#define DOS_OPTIONS_JEU_T_IS_STEREO_OFFSET 0x1c
#define DOS_OPTIONS_JEU_T_IS_STEREO_SIZE 2
/* UNKNOWN: Unsigned16 option slot outside the Rayman/collision field-access scope. */
#define DOS_OPTIONS_JEU_T_UNKNOWN_1E_OFFSET 0x1e
#define DOS_OPTIONS_JEU_T_UNKNOWN_1E_SIZE 2
/* PROVEN: Snapshot of Rayman x_pos. */
#define DOS_RAY_STACK_T_X_POS_OFFSET 0x0
#define DOS_RAY_STACK_T_X_POS_SIZE 2
/* PROVEN: Snapshot of Rayman y_pos. */
#define DOS_RAY_STACK_T_Y_POS_OFFSET 0x2
#define DOS_RAY_STACK_T_Y_POS_SIZE 2
/* PROVEN: Snapshot of fist x_pos. */
#define DOS_RAY_STACK_T_POING_X_POS_OFFSET 0x4
#define DOS_RAY_STACK_T_POING_X_POS_SIZE 2
/* PROVEN: Snapshot of fist y_pos. */
#define DOS_RAY_STACK_T_POING_Y_POS_OFFSET 0x6
#define DOS_RAY_STACK_T_POING_Y_POS_SIZE 2
/* PROVEN: Snapshot of Rayman main_etat. */
#define DOS_RAY_STACK_T_MAIN_ETAT_OFFSET 0x8
#define DOS_RAY_STACK_T_MAIN_ETAT_SIZE 1
/* PROVEN: Snapshot of Rayman sub_etat. */
#define DOS_RAY_STACK_T_SUB_ETAT_OFFSET 0x9
#define DOS_RAY_STACK_T_SUB_ETAT_SIZE 1
/* PROVEN: Snapshot of Rayman anim_index. */
#define DOS_RAY_STACK_T_ANIM_INDEX_OFFSET 0xa
#define DOS_RAY_STACK_T_ANIM_INDEX_SIZE 1
/* PROVEN: Snapshot of Rayman anim_frame. */
#define DOS_RAY_STACK_T_ANIM_FRAME_OFFSET 0xb
#define DOS_RAY_STACK_T_ANIM_FRAME_SIZE 1
/* PROVEN: Snapshot of Rayman flip_x. */
#define DOS_RAY_STACK_T_FLIP_X_OFFSET 0xc
#define DOS_RAY_STACK_T_FLIP_X_SIZE 1
/* PROVEN: Snapshot of fist anim_index. */
#define DOS_RAY_STACK_T_POING_ANIM_INDEX_OFFSET 0xd
#define DOS_RAY_STACK_T_POING_ANIM_INDEX_SIZE 1
/* PROVEN: Snapshot of fist anim_frame. */
#define DOS_RAY_STACK_T_POING_ANIM_FRAME_OFFSET 0xe
#define DOS_RAY_STACK_T_POING_ANIM_FRAME_SIZE 1
/* PROVEN: Snapshot of fist flip_x. */
#define DOS_RAY_STACK_T_POING_FLIP_X_OFFSET 0xf
#define DOS_RAY_STACK_T_POING_FLIP_X_SIZE 1
/* PROVEN: Validity byte for the optional fist snapshot. */
#define DOS_RAY_STACK_T_POING_IS_ACTIVE_OFFSET 0x10
#define DOS_RAY_STACK_T_POING_IS_ACTIVE_SIZE 1
/* PROVEN: Low8 bits of Rayman scale at the history sample. */
#define DOS_RAY_STACK_T_SCALE_OFFSET 0x11
#define DOS_RAY_STACK_T_SCALE_SIZE 1
#define DOS_WORLD_INFO_T_SIZE 20
#define DOS_RGB_T_SIZE 3
#define DOS_RGB_PALETTE_T_SIZE 768
#define DOS_ARCHIVE_HEADER_LOCAL_T_SIZE 12
#define DOS_PCX_HEADER_LOCAL_T_SIZE 128
#define DOS_X_TEXTURE_LOCAL_T_SIZE 256
#define DOS_LEVEL_LOCAL_T_SIZE 6
#define DOS_LEVEL_OBJ_LOCAL_T_SIZE 6
#define DOS_DISPLAY_MAP_LOCAL_T_SIZE 8
#define DOS_BIG_MAP_LOCAL_T_SIZE 30
/* PROVEN: World-map node center coordinate;used to place medallions and compute path interpolation. */
#define DOS_WORLD_INFO_T_XPOS_OFFSET 0x0
#define DOS_WORLD_INFO_T_XPOS_SIZE 2
/* PROVEN: World-map node center coordinate;used to place medallions and compute path interpolation. */
#define DOS_WORLD_INFO_T_YPOS_OFFSET 0x2
#define DOS_WORLD_INFO_T_YPOS_SIZE 2
/* PROVEN: Neighbor node index for the named direction;self index means no movement. Shipped table stays0..23. */
#define DOS_WORLD_INFO_T_INDEX_UP_OFFSET 0x4
#define DOS_WORLD_INFO_T_INDEX_UP_SIZE 1
/* PROVEN: Neighbor node index for the named direction;self index means no movement. Shipped table stays0..23. */
#define DOS_WORLD_INFO_T_INDEX_DOWN_OFFSET 0x5
#define DOS_WORLD_INFO_T_INDEX_DOWN_SIZE 1
/* PROVEN: Neighbor node index for the named direction;self index means no movement. Shipped table stays0..23. */
#define DOS_WORLD_INFO_T_INDEX_LEFT_OFFSET 0x6
#define DOS_WORLD_INFO_T_INDEX_LEFT_SIZE 1
/* PROVEN: Neighbor node index for the named direction;self index means no movement. Shipped table stays0..23. */
#define DOS_WORLD_INFO_T_INDEX_RIGHT_OFFSET 0x7
#define DOS_WORLD_INFO_T_INDEX_RIGHT_SIZE 1
/* PROVEN: [sem_objs; special.c; UNKNOWN] Declared world_info_t.state storage; no intended meaning is assigned from its spelling alone.
[sem_flow; bit flags; PROVEN] Access/animation byte:bit0 accessible,bit1 drawing traversal/neighbor mark,bit2 pending unlock;high5 retained. Runtime bit2 is serialized as save-byte bit1. */
#define DOS_WORLD_INFO_T_STATE_OFFSET 0x8
#define DOS_WORLD_INFO_T_STATE_SIZE 1
/* PROVEN: Bit0 allows navigation/entry and ordinary medallion/path drawing. */
#define DOS_WORLD_INFO_T_IS_UNLOCKED_OFFSET 0x8
#define DOS_WORLD_INFO_T_IS_UNLOCKED_SIZE 1
#define DOS_WORLD_INFO_T_IS_UNLOCKED_MASK 0x01
/* PROVEN: Bit1 scratch traversal/neighbor mark:DISPLAY_PTS_WAY clears all then marks processed accessible nodes;DO_MEDAILLONS also marks accessible nodes after unlock. Not a separate persistent completion flag. */
#define DOS_WORLD_INFO_T_STATE_BIT1_OFFSET 0x8
#define DOS_WORLD_INFO_T_STATE_BIT1_SIZE 1
#define DOS_WORLD_INFO_T_STATE_BIT1_MASK 0x02
/* PROVEN: Bit2 pending unlock;at progress128 and marked neighbor,convert to accessible and clear bits1/2. Saved in packed save bit1. */
#define DOS_WORLD_INFO_T_IS_UNLOCKING_OFFSET 0x8
#define DOS_WORLD_INFO_T_IS_UNLOCKING_SIZE 1
#define DOS_WORLD_INFO_T_IS_UNLOCKING_MASK 0x04
/* UNKNOWN: Declared world_info_t.state_rest storage; no intended meaning is assigned from its spelling alone. */
#define DOS_WORLD_INFO_T_STATE_REST_OFFSET 0x8
#define DOS_WORLD_INFO_T_STATE_REST_SIZE 1
#define DOS_WORLD_INFO_T_STATE_REST_MASK 0xf8
/* PROVEN: Collected-cage count of one world-map stage record. */
#define DOS_WORLD_INFO_T_NB_CAGES_OFFSET 0x9
#define DOS_WORLD_INFO_T_NB_CAGES_SIZE 1
/* PROVEN: Actual game world selected by node;shipped playable nodes1..6,save nodes18..23 carry7. */
#define DOS_WORLD_INFO_T_WORLD_OFFSET 0xa
#define DOS_WORLD_INFO_T_WORLD_SIZE 1
/* PROVEN: Actual starting level selected by node;save nodes carry20 and take separate save UI route. */
#define DOS_WORLD_INFO_T_LEVEL_OFFSET 0xb
#define DOS_WORLD_INFO_T_LEVEL_SIZE 1
/* STRONG: Signed stage-title text color copied to text display state;exact uses retained. */
#define DOS_WORLD_INFO_T_COLOR_OFFSET 0xc
#define DOS_WORLD_INFO_T_COLOR_SIZE 1
/* UNKNOWN: Declared world_info_t.field_D storage; no intended meaning is assigned from its spelling alone. */
#define DOS_WORLD_INFO_T_FIELD_D_OFFSET 0xd
#define DOS_WORLD_INFO_T_FIELD_D_SIZE 1
/* UNKNOWN: Declared world_info_t.field_E storage; no intended meaning is assigned from its spelling alone. */
#define DOS_WORLD_INFO_T_FIELD_E_OFFSET 0xe
#define DOS_WORLD_INFO_T_FIELD_E_SIZE 1
/* UNKNOWN: Declared world_info_t.field_F storage; no intended meaning is assigned from its spelling alone. */
#define DOS_WORLD_INFO_T_FIELD_F_OFFSET 0xf
#define DOS_WORLD_INFO_T_FIELD_F_SIZE 1
/* PROVEN: Localized node label pointer assigned from language_txt;save nodes replace it with150/151/152 status strings. */
#define DOS_WORLD_INFO_T_TEXT_OFFSET 0x10
#define DOS_WORLD_INFO_T_TEXT_SIZE 4
/* STRONG: First component in each three-byte RGB palette entry. */
#define DOS_RGB_T_R_OFFSET 0x0
#define DOS_RGB_T_R_SIZE 1
/* STRONG: Second component in each three-byte RGB palette entry. */
#define DOS_RGB_T_G_OFFSET 0x1
#define DOS_RGB_T_G_SIZE 1
/* STRONG: Third component in each three-byte RGB palette entry. */
#define DOS_RGB_T_B_OFFSET 0x2
#define DOS_RGB_T_B_SIZE 1
/* PROVEN: 256 three-byte palette entries. */
#define DOS_RGB_PALETTE_T_COLORS_OFFSET 0x0
#define DOS_RGB_PALETTE_T_COLORS_SIZE 768
/* PROVEN: Absolute file byte offset of one encoded archive payload. */
#define DOS_ARCHIVE_HEADER_LOCAL_T_OFFSET_OFFSET 0x0
#define DOS_ARCHIVE_HEADER_LOCAL_T_OFFSET_SIZE 4
/* PROVEN: Encoded payload byte length including PCX header/body/palette where applicable. */
#define DOS_ARCHIVE_HEADER_LOCAL_T_SIZE_OFFSET 0x4
#define DOS_ARCHIVE_HEADER_LOCAL_T_SIZE_SIZE 4
/* PROVEN: Per-entry XOR decode byte. */
#define DOS_ARCHIVE_HEADER_LOCAL_T_XOR_BYTE_OFFSET 0x8
#define DOS_ARCHIVE_HEADER_LOCAL_T_XOR_BYTE_SIZE 1
/* PROVEN: Modulo256 sum of the encoded payload bytes. */
#define DOS_ARCHIVE_HEADER_LOCAL_T_CHECKSUM_BYTE_OFFSET 0x9
#define DOS_ARCHIVE_HEADER_LOCAL_T_CHECKSUM_BYTE_SIZE 1
/* UNKNOWN: Two table bytes at+10 not read as archive fields by the scoped loaders. */
#define DOS_ARCHIVE_HEADER_LOCAL_T_RESERVED_OFFSET 0xa
#define DOS_ARCHIVE_HEADER_LOCAL_T_RESERVED_SIZE 2
/* UNKNOWN: Declared pcx_header_local_t.header_byte storage; no intended meaning is assigned from its spelling alone. */
#define DOS_PCX_HEADER_LOCAL_T_HEADER_BYTE_OFFSET 0x0
#define DOS_PCX_HEADER_LOCAL_T_HEADER_BYTE_SIZE 1
/* UNKNOWN: Declared pcx_header_local_t.software_version storage; no intended meaning is assigned from its spelling alone. */
#define DOS_PCX_HEADER_LOCAL_T_SOFTWARE_VERSION_OFFSET 0x1
#define DOS_PCX_HEADER_LOCAL_T_SOFTWARE_VERSION_SIZE 1
/* UNKNOWN: Declared pcx_header_local_t.encoding_method storage; no intended meaning is assigned from its spelling alone. */
#define DOS_PCX_HEADER_LOCAL_T_ENCODING_METHOD_OFFSET 0x2
#define DOS_PCX_HEADER_LOCAL_T_ENCODING_METHOD_SIZE 1
/* UNKNOWN: Declared pcx_header_local_t.bits_per_bitplane storage; no intended meaning is assigned from its spelling alone. */
#define DOS_PCX_HEADER_LOCAL_T_BITS_PER_BITPLANE_OFFSET 0x3
#define DOS_PCX_HEADER_LOCAL_T_BITS_PER_BITPLANE_SIZE 1
/* PROVEN: Decoded16-bit horizontal lower bound used in width computation. */
#define DOS_PCX_HEADER_LOCAL_T_MIN_X_OFFSET 0x4
#define DOS_PCX_HEADER_LOCAL_T_MIN_X_SIZE 2
/* PROVEN: Decoded16-bit vertical lower bound used in height computation. */
#define DOS_PCX_HEADER_LOCAL_T_MIN_Y_OFFSET 0x6
#define DOS_PCX_HEADER_LOCAL_T_MIN_Y_SIZE 2
/* PROVEN: Decoded16-bit horizontal upper bound used in width computation. */
#define DOS_PCX_HEADER_LOCAL_T_MAX_X_OFFSET 0x8
#define DOS_PCX_HEADER_LOCAL_T_MAX_X_SIZE 2
/* PROVEN: Decoded16-bit vertical upper bound used in height computation. */
#define DOS_PCX_HEADER_LOCAL_T_MAX_Y_OFFSET 0xa
#define DOS_PCX_HEADER_LOCAL_T_MAX_Y_SIZE 2
/* UNKNOWN: Declared pcx_header_local_t.hor_dpi storage; no intended meaning is assigned from its spelling alone. */
#define DOS_PCX_HEADER_LOCAL_T_HOR_DPI_OFFSET 0xc
#define DOS_PCX_HEADER_LOCAL_T_HOR_DPI_SIZE 2
/* UNKNOWN: Declared pcx_header_local_t.ver_dpi storage; no intended meaning is assigned from its spelling alone. */
#define DOS_PCX_HEADER_LOCAL_T_VER_DPI_OFFSET 0xe
#define DOS_PCX_HEADER_LOCAL_T_VER_DPI_SIZE 2
/* UNKNOWN: Declared pcx_header_local_t.ega_palette storage; no intended meaning is assigned from its spelling alone. */
#define DOS_PCX_HEADER_LOCAL_T_EGA_PALETTE_OFFSET 0x10
#define DOS_PCX_HEADER_LOCAL_T_EGA_PALETTE_SIZE 48
/* UNKNOWN: Declared pcx_header_local_t.reserved1 storage; no intended meaning is assigned from its spelling alone. */
#define DOS_PCX_HEADER_LOCAL_T_RESERVED1_OFFSET 0x40
#define DOS_PCX_HEADER_LOCAL_T_RESERVED1_SIZE 1
/* UNKNOWN: Declared pcx_header_local_t.n_color_planes storage; no intended meaning is assigned from its spelling alone. */
#define DOS_PCX_HEADER_LOCAL_T_N_COLOR_PLANES_OFFSET 0x41
#define DOS_PCX_HEADER_LOCAL_T_N_COLOR_PLANES_SIZE 1
/* PROVEN: Number of expanded bytes required by each RLE scanline loop. */
#define DOS_PCX_HEADER_LOCAL_T_BYTES_PER_LINE_OFFSET 0x42
#define DOS_PCX_HEADER_LOCAL_T_BYTES_PER_LINE_SIZE 2
/* UNKNOWN: Declared pcx_header_local_t.palette_mode storage; no intended meaning is assigned from its spelling alone. */
#define DOS_PCX_HEADER_LOCAL_T_PALETTE_MODE_OFFSET 0x44
#define DOS_PCX_HEADER_LOCAL_T_PALETTE_MODE_SIZE 2
/* UNKNOWN: Declared pcx_header_local_t.source_hor_res storage; no intended meaning is assigned from its spelling alone. */
#define DOS_PCX_HEADER_LOCAL_T_SOURCE_HOR_RES_OFFSET 0x46
#define DOS_PCX_HEADER_LOCAL_T_SOURCE_HOR_RES_SIZE 2
/* UNKNOWN: Declared pcx_header_local_t.source_ver_res storage; no intended meaning is assigned from its spelling alone. */
#define DOS_PCX_HEADER_LOCAL_T_SOURCE_VER_RES_OFFSET 0x48
#define DOS_PCX_HEADER_LOCAL_T_SOURCE_VER_RES_SIZE 2
/* UNKNOWN: Declared pcx_header_local_t.reserved2 storage; no intended meaning is assigned from its spelling alone. */
#define DOS_PCX_HEADER_LOCAL_T_RESERVED2_OFFSET 0x4a
#define DOS_PCX_HEADER_LOCAL_T_RESERVED2_SIZE 54
/* PROVEN: One256-byte ModeX tile-pixel record. */
#define DOS_X_TEXTURE_LOCAL_T_PIXELS_OFFSET 0x0
#define DOS_X_TEXTURE_LOCAL_T_PIXELS_SIZE 256
/* PROVEN: Runtime pointer to132-byte live level-instance records. */
#define DOS_LEVEL_LOCAL_T_OBJECTS_OFFSET 0x0
#define DOS_LEVEL_LOCAL_T_OBJECTS_SIZE 4
/* PROVEN: Signed16 live level-instance count. */
#define DOS_LEVEL_LOCAL_T_NB_OBJECTS_OFFSET 0x4
#define DOS_LEVEL_LOCAL_T_NB_OBJECTS_SIZE 2
/* PROVEN: Runtime pointer to a signed16 index list. */
#define DOS_LEVEL_OBJ_LOCAL_T_OBJ_IDS_OFFSET 0x0
#define DOS_LEVEL_OBJ_LOCAL_T_OBJ_IDS_SIZE 4
/* PROVEN: Signed16 number of populated object-index list entries. */
#define DOS_LEVEL_OBJ_LOCAL_T_NB_OBJECTS_OFFSET 0x4
#define DOS_LEVEL_OBJ_LOCAL_T_NB_OBJECTS_SIZE 2
/* UNKNOWN: Declared display_map_local_t.x storage; no intended meaning is assigned from its spelling alone. */
#define DOS_DISPLAY_MAP_LOCAL_T_X_OFFSET 0x0
#define DOS_DISPLAY_MAP_LOCAL_T_X_SIZE 2
/* UNKNOWN: Declared display_map_local_t.y storage; no intended meaning is assigned from its spelling alone. */
#define DOS_DISPLAY_MAP_LOCAL_T_Y_OFFSET 0x2
#define DOS_DISPLAY_MAP_LOCAL_T_Y_SIZE 2
/* UNKNOWN: Declared display_map_local_t.width_in_tiles storage; no intended meaning is assigned from its spelling alone. */
#define DOS_DISPLAY_MAP_LOCAL_T_WIDTH_IN_TILES_OFFSET 0x4
#define DOS_DISPLAY_MAP_LOCAL_T_WIDTH_IN_TILES_SIZE 1
/* UNKNOWN: Declared display_map_local_t.height_in_tiles storage; no intended meaning is assigned from its spelling alone. */
#define DOS_DISPLAY_MAP_LOCAL_T_HEIGHT_IN_TILES_OFFSET 0x5
#define DOS_DISPLAY_MAP_LOCAL_T_HEIGHT_IN_TILES_SIZE 1
/* UNKNOWN: Declared display_map_local_t.tile_index storage; no intended meaning is assigned from its spelling alone. */
#define DOS_DISPLAY_MAP_LOCAL_T_TILE_INDEX_OFFSET 0x6
#define DOS_DISPLAY_MAP_LOCAL_T_TILE_INDEX_SIZE 2
/* UNKNOWN: Declared big_map_local_t.map storage; no intended meaning is assigned from its spelling alone. */
#define DOS_BIG_MAP_LOCAL_T_MAP_OFFSET 0x0
#define DOS_BIG_MAP_LOCAL_T_MAP_SIZE 4
/* UNKNOWN: Declared big_map_local_t.tile_texture_offsets storage; no intended meaning is assigned from its spelling alone. */
#define DOS_BIG_MAP_LOCAL_T_TILE_TEXTURE_OFFSETS_OFFSET 0x4
#define DOS_BIG_MAP_LOCAL_T_TILE_TEXTURE_OFFSETS_SIZE 4
/* UNKNOWN: Declared big_map_local_t.display_map storage; no intended meaning is assigned from its spelling alone. */
#define DOS_BIG_MAP_LOCAL_T_DISPLAY_MAP_OFFSET 0x8
#define DOS_BIG_MAP_LOCAL_T_DISPLAY_MAP_SIZE 4
/* UNKNOWN: Declared big_map_local_t.map_blocks storage; no intended meaning is assigned from its spelling alone. */
#define DOS_BIG_MAP_LOCAL_T_MAP_BLOCKS_OFFSET 0xc
#define DOS_BIG_MAP_LOCAL_T_MAP_BLOCKS_SIZE 4
/* UNKNOWN: Declared big_map_local_t.field_10 storage; no intended meaning is assigned from its spelling alone. */
#define DOS_BIG_MAP_LOCAL_T_FIELD_10_OFFSET 0x10
#define DOS_BIG_MAP_LOCAL_T_FIELD_10_SIZE 4
/* UNKNOWN: Declared big_map_local_t.field_14 storage; no intended meaning is assigned from its spelling alone. */
#define DOS_BIG_MAP_LOCAL_T_FIELD_14_OFFSET 0x14
#define DOS_BIG_MAP_LOCAL_T_FIELD_14_SIZE 2
/* UNKNOWN: Declared big_map_local_t.width storage; no intended meaning is assigned from its spelling alone. */
#define DOS_BIG_MAP_LOCAL_T_WIDTH_OFFSET 0x16
#define DOS_BIG_MAP_LOCAL_T_WIDTH_SIZE 2
/* UNKNOWN: Declared big_map_local_t.height storage; no intended meaning is assigned from its spelling alone. */
#define DOS_BIG_MAP_LOCAL_T_HEIGHT_OFFSET 0x18
#define DOS_BIG_MAP_LOCAL_T_HEIGHT_SIZE 2
/* UNKNOWN: Declared big_map_local_t.field_1A storage; no intended meaning is assigned from its spelling alone. */
#define DOS_BIG_MAP_LOCAL_T_FIELD_1A_OFFSET 0x1a
#define DOS_BIG_MAP_LOCAL_T_FIELD_1A_SIZE 2
/* UNKNOWN: Declared big_map_local_t.field_1C storage; no intended meaning is assigned from its spelling alone. */
#define DOS_BIG_MAP_LOCAL_T_FIELD_1C_OFFSET 0x1c
#define DOS_BIG_MAP_LOCAL_T_FIELD_1C_SIZE 2
#define DOS_LOADINFORAY_T_SIZE 4
#define DOS_CREDIT_T_SIZE 12
#define DOS_INIT_DATA_STATE_T_SIZE 20
#define DOS_DEFORM_BLIT_T_SIZE 52
#define DOS_GRP_STAR_T_SIZE 4
#define DOS_FRAME_CONTEXT_T_SIZE 32
/* PROVEN: Cached save-slot life count. */
#define DOS_LOADINFORAY_T_LIVES_OFFSET 0x0
#define DOS_LOADINFORAY_T_LIVES_SIZE 1
/* PROVEN: Cached save-slot ting count. */
#define DOS_LOADINFORAY_T_TINGS_OFFSET 0x1
#define DOS_LOADINFORAY_T_TINGS_SIZE 1
/* PROVEN: Cached total cages across the save progress records. */
#define DOS_LOADINFORAY_T_CAGES_OFFSET 0x2
#define DOS_LOADINFORAY_T_CAGES_SIZE 1
/* PROVEN: Cached save-slot continue count. */
#define DOS_LOADINFORAY_T_CONTINUES_OFFSET 0x3
#define DOS_LOADINFORAY_T_CONTINUES_SIZE 1
/* PROVEN: Current credit line string pointer. */
#define DOS_CREDIT_T_TEXT_OFFSET 0x0
#define DOS_CREDIT_T_TEXT_SIZE 4
/* PROVEN: Credit line drawing x anchor. */
#define DOS_CREDIT_T_X_POS_OFFSET 0x4
#define DOS_CREDIT_T_X_POS_SIZE 2
/* PROVEN: Credit line text baseline. */
#define DOS_CREDIT_T_Y_POS_OFFSET 0x6
#define DOS_CREDIT_T_Y_POS_SIZE 2
/* PROVEN: Credit line font selector. */
#define DOS_CREDIT_T_FONT_OFFSET 0x8
#define DOS_CREDIT_T_FONT_SIZE 1
/* UNKNOWN: Credit control byte consumed by credits logic outside the display loop. */
#define DOS_CREDIT_T_CMD_OFFSET 0x9
#define DOS_CREDIT_T_CMD_SIZE 1
/* PROVEN: Credit line color-drawer selector. */
#define DOS_CREDIT_T_COLOR_OFFSET 0xa
#define DOS_CREDIT_T_COLOR_SIZE 1
/* UNKNOWN: Credit record trailing byte not read by the display loop. */
#define DOS_CREDIT_T_UNKNOWN_B_OFFSET 0xb
#define DOS_CREDIT_T_UNKNOWN_B_SIZE 1
/* PROVEN: Low four bits of the current camera x coordinate. */
#define DOS_INIT_DATA_STATE_T_X_OFFSET 0x0
#define DOS_INIT_DATA_STATE_T_X_SIZE 2
/* PROVEN: Low four bits of the current camera y coordinate. */
#define DOS_INIT_DATA_STATE_T_Y_OFFSET 0x2
#define DOS_INIT_DATA_STATE_T_Y_SIZE 2
/* PROVEN: Absolute DOS destination pointer for the top-left partial tile. */
#define DOS_INIT_DATA_STATE_T_DRAW_ORIGIN_OFFSET 0x4
#define DOS_INIT_DATA_STATE_T_DRAW_ORIGIN_SIZE 4
/* PROVEN: Background source x coordinate adjusted for the current tile phase. */
#define DOS_INIT_DATA_STATE_T_SCROLL_X_OFFSET 0x8
#define DOS_INIT_DATA_STATE_T_SCROLL_X_SIZE 4
/* PROVEN: Background source y coordinate adjusted for the current tile phase. */
#define DOS_INIT_DATA_STATE_T_SCROLL_Y_OFFSET 0xc
#define DOS_INIT_DATA_STATE_T_SCROLL_Y_SIZE 4
/* PROVEN: Absolute DOS background source pointer. */
#define DOS_INIT_DATA_STATE_T_SOURCE_ORIGIN_OFFSET 0x10
#define DOS_INIT_DATA_STATE_T_SOURCE_ORIGIN_SIZE 4
/* PROVEN: Horizontal edge1 direction as a destination byte step. */
#define DOS_DEFORM_BLIT_T_X_STEP_1_OFFSET 0x0
#define DOS_DEFORM_BLIT_T_X_STEP_1_SIZE 4
/* PROVEN: Vertical edge1 direction as a destination row step. */
#define DOS_DEFORM_BLIT_T_ROW_STEP_1_OFFSET 0x4
#define DOS_DEFORM_BLIT_T_ROW_STEP_1_SIZE 4
/* PROVEN: Absolute horizontal length of edge1. */
#define DOS_DEFORM_BLIT_T_DX_1_OFFSET 0x8
#define DOS_DEFORM_BLIT_T_DX_1_SIZE 4
/* PROVEN: Absolute vertical length of edge1,with zero replaced by1. */
#define DOS_DEFORM_BLIT_T_DY_1_OFFSET 0xc
#define DOS_DEFORM_BLIT_T_DY_1_SIZE 4
/* PROVEN: Horizontal edge2 direction as a destination byte step. */
#define DOS_DEFORM_BLIT_T_X_STEP_2_OFFSET 0x10
#define DOS_DEFORM_BLIT_T_X_STEP_2_SIZE 4
/* PROVEN: Vertical edge2 direction as a destination row step. */
#define DOS_DEFORM_BLIT_T_ROW_STEP_2_OFFSET 0x14
#define DOS_DEFORM_BLIT_T_ROW_STEP_2_SIZE 4
/* PROVEN: Absolute horizontal length of edge2. */
#define DOS_DEFORM_BLIT_T_DX_2_OFFSET 0x18
#define DOS_DEFORM_BLIT_T_DX_2_SIZE 4
/* PROVEN: Absolute vertical length of edge2,with zero replaced by1. */
#define DOS_DEFORM_BLIT_T_DY_2_OFFSET 0x1c
#define DOS_DEFORM_BLIT_T_DY_2_SIZE 4
/* PROVEN: Deformed bounding rectangle width and scratch row pitch. */
#define DOS_DEFORM_BLIT_T_DEST_WIDTH_OFFSET 0x20
#define DOS_DEFORM_BLIT_T_DEST_WIDTH_SIZE 4
/* PROVEN: Deformed bounding rectangle height. */
#define DOS_DEFORM_BLIT_T_DEST_HEIGHT_OFFSET 0x24
#define DOS_DEFORM_BLIT_T_DEST_HEIGHT_SIZE 4
/* PROVEN: First transformed corner offset within the scratch bounding rectangle. */
#define DOS_DEFORM_BLIT_T_DEST_OFFSET_OFFSET 0x28
#define DOS_DEFORM_BLIT_T_DEST_OFFSET_SIZE 4
/* PROVEN: Undeformed outer sprite width and source row pitch. */
#define DOS_DEFORM_BLIT_T_SOURCE_WIDTH_OFFSET 0x2c
#define DOS_DEFORM_BLIT_T_SOURCE_WIDTH_SIZE 4
/* PROVEN: Undeformed outer sprite height. */
#define DOS_DEFORM_BLIT_T_SOURCE_HEIGHT_OFFSET 0x30
#define DOS_DEFORM_BLIT_T_SOURCE_HEIGHT_SIZE 4
/* PROVEN: Elapsed calls for one grapple-star slot since its last reroll. */
#define DOS_GRP_STAR_T_TIMER_OFFSET 0x0
#define DOS_GRP_STAR_T_TIMER_SIZE 1
/* PROVEN: Last randomized grapple-star reroll threshold. */
#define DOS_GRP_STAR_T_LENGTH_OFFSET 0x1
#define DOS_GRP_STAR_T_LENGTH_SIZE 1
/* PROVEN: Random diagonal displacement of this grapple star. */
#define DOS_GRP_STAR_T_DIST_OFFSET 0x2
#define DOS_GRP_STAR_T_DIST_SIZE 1
/* PROVEN: Index into the local grapple-star sprite sequence. */
#define DOS_GRP_STAR_T_SPRITE_TABLE_INDEX_OFFSET 0x3
#define DOS_GRP_STAR_T_SPRITE_TABLE_INDEX_SIZE 1
/* PROVEN: Phase-dependent context word0. */
#define DOS_FRAME_CONTEXT_T_PLANE_SIZE_OFFSET 0x0
#define DOS_FRAME_CONTEXT_T_PLANE_SIZE_SIZE 4
/* PROVEN: Phase-dependent context word4. */
#define DOS_FRAME_CONTEXT_T_SOURCE_END_OFFSET 0x4
#define DOS_FRAME_CONTEXT_T_SOURCE_END_SIZE 4
/* PROVEN: Phase-dependent context word8. */
#define DOS_FRAME_CONTEXT_T_RUN_STEP_OFFSET 0x8
#define DOS_FRAME_CONTEXT_T_RUN_STEP_SIZE 4
/* PROVEN: Background source scanline pitch supplied to tile helpers. */
#define DOS_FRAME_CONTEXT_T_SOURCE_STRIDE_OFFSET 0xc
#define DOS_FRAME_CONTEXT_T_SOURCE_STRIDE_SIZE 4
/* PROVEN: Three background scanline pitches. */
#define DOS_FRAME_CONTEXT_T_SOURCE_THREE_ROWS_OFFSET 0x10
#define DOS_FRAME_CONTEXT_T_SOURCE_THREE_ROWS_SIZE 4
/* PROVEN: Pointer to the final level map record. */
#define DOS_FRAME_CONTEXT_T_LAST_TILE_OFFSET 0x14
#define DOS_FRAME_CONTEXT_T_LAST_TILE_SIZE 4
/* PROVEN: One-past background buffer endpoint. */
#define DOS_FRAME_CONTEXT_T_BACKGROUND_END_OFFSET 0x18
#define DOS_FRAME_CONTEXT_T_BACKGROUND_END_SIZE 4
/* PROVEN: Full background data length across all four planes. */
#define DOS_FRAME_CONTEXT_T_BACKGROUND_LENGTH_OFFSET 0x1c
#define DOS_FRAME_CONTEXT_T_BACKGROUND_LENGTH_SIZE 4
#define DOS_SPECIAL_BOOL_T_SIZE 4
#define DOS_PLATFORM_OBJECT_TYPE_T_SIZE 4
#define DOS_PLATFORM_BOOL_T_SIZE 1
#define DOS_CALCBLOC_FUNC_T_SIZE 4
#define DOS_CALCBLOC1_FUNC_T_SIZE 4
#define DOS_NGAWE_BYTE_PAIR_T_SIZE 2
/* UNKNOWN: Unused unsigned byte member x; coordinate meaning is not established. */
#define DOS_NGAWE_BYTE_PAIR_T_X_OFFSET 0x0
#define DOS_NGAWE_BYTE_PAIR_T_X_SIZE 1
/* UNKNOWN: Unused unsigned byte member y; coordinate meaning is not established. */
#define DOS_NGAWE_BYTE_PAIR_T_Y_OFFSET 0x1
#define DOS_NGAWE_BYTE_PAIR_T_Y_SIZE 1
#define DOS_CPTR_T_SIZE 12
/* PROVEN: Decode and store the arguments of this opcode. */
#define DOS_CPTR_T_READ_OFFSET 0x0
#define DOS_CPTR_T_READ_SIZE 4
/* PROVEN: Skip the arguments of this opcode without applying its handler. */
#define DOS_CPTR_T_SKIP_OFFSET 0x4
#define DOS_CPTR_T_SKIP_SIZE 4
/* PROVEN: Handle the decoded command; nonzero continues immediate decoding, zero yields to the object updater. */
#define DOS_CPTR_T_HANDLE_OFFSET 0x8
#define DOS_CPTR_T_HANDLE_SIZE 4
#define DOS_DISPLAY_MAP_T_SIZE 8
/* PROVEN: Screen x of the first visible tile, including its partially clipped portion. */
#define DOS_DISPLAY_MAP_T_X_OFFSET 0x0
#define DOS_DISPLAY_MAP_T_X_SIZE 2
/* PROVEN: Screen y of the first visible tile, including its partially clipped portion. */
#define DOS_DISPLAY_MAP_T_Y_OFFSET 0x2
#define DOS_DISPLAY_MAP_T_Y_SIZE 2
/* PROVEN: Ceiling-rounded number of visible tile columns. */
#define DOS_DISPLAY_MAP_T_WIDTH_IN_TILES_OFFSET 0x4
#define DOS_DISPLAY_MAP_T_WIDTH_IN_TILES_SIZE 1
/* PROVEN: Ceiling-rounded number of visible tile rows. */
#define DOS_DISPLAY_MAP_T_HEIGHT_IN_TILES_OFFSET 0x5
#define DOS_DISPLAY_MAP_T_HEIGHT_IN_TILES_SIZE 1
/* PROVEN: Row-major index of the first visible map tile, stored as unsigned16. */
#define DOS_DISPLAY_MAP_T_TILE_INDEX_OFFSET 0x6
#define DOS_DISPLAY_MAP_T_TILE_INDEX_SIZE 2
#define DOS_BIG_MAP_T_SIZE 32
/* PROVEN: Map tile record array shared with mp.map. */
#define DOS_BIG_MAP_T_MAP_OFFSET 0x0
#define DOS_BIG_MAP_T_MAP_SIZE 4
/* PROVEN: Per-map-cell Normal texture pointers, despite the offsets spelling. */
#define DOS_BIG_MAP_T_TILE_TEXTURE_OFFSETS_OFFSET 0x4
#define DOS_BIG_MAP_T_TILE_TEXTURE_OFFSETS_SIZE 4
/* PROVEN: Pointer to the prepared visible tile rectangle. */
#define DOS_BIG_MAP_T_DISPLAY_MAP_OFFSET 0x8
#define DOS_BIG_MAP_T_DISPLAY_MAP_SIZE 4
/* PROVEN: Base of the packed Normal tile texture storage. */
#define DOS_BIG_MAP_T_MAP_BLOCKS_OFFSET 0xc
#define DOS_BIG_MAP_T_MAP_BLOCKS_SIZE 4
/* UNKNOWN: No value-consuming role for this declared tail member is established by the assigned map units. */
#define DOS_BIG_MAP_T_FIELD_10_OFFSET 0x10
#define DOS_BIG_MAP_T_FIELD_10_SIZE 4
/* UNKNOWN: No value-consuming role for this declared tail member is established by the assigned map units. */
#define DOS_BIG_MAP_T_FIELD_14_OFFSET 0x14
#define DOS_BIG_MAP_T_FIELD_14_SIZE 2
/* PROVEN: Map width in 16-pixel tiles. */
#define DOS_BIG_MAP_T_WIDTH_OFFSET 0x16
#define DOS_BIG_MAP_T_WIDTH_SIZE 2
/* PROVEN: Map height in 16-pixel tiles. */
#define DOS_BIG_MAP_T_HEIGHT_OFFSET 0x18
#define DOS_BIG_MAP_T_HEIGHT_SIZE 2
/* UNKNOWN: No value-consuming role for this declared tail member is established by the assigned map units. */
#define DOS_BIG_MAP_T_FIELD_1A_OFFSET 0x1a
#define DOS_BIG_MAP_T_FIELD_1A_SIZE 2
/* UNKNOWN: No value-consuming role for this declared tail member is established by the assigned map units. */
#define DOS_BIG_MAP_T_FIELD_1C_OFFSET 0x1c
#define DOS_BIG_MAP_T_FIELD_1C_SIZE 2
/* UNKNOWN: No value-consuming role for this declared tail member is established by the assigned map units. */
#define DOS_BIG_MAP_T_FIELD_1E_OFFSET 0x1e
#define DOS_BIG_MAP_T_FIELD_1E_SIZE 2
#define DOS_BB_DATA_T_SIZE 16
#define DOS_BB_ATTACK_ENTRY_T_SIZE 4
/* STRONG: Walking-animation selector and encounter movement scratch; 2/3 select walking substates 1/2. */
#define DOS_BB_DATA_T_SPEED_X_OFFSET 0x0
#define DOS_BB_DATA_T_SPEED_X_SIZE 2
/* STRONG: Signed per-encounter progress/countdown scratch; initialized to -1 and tested by movement/landing phases. */
#define DOS_BB_DATA_T_FIELD1_0X2_OFFSET 0x2
#define DOS_BB_DATA_T_FIELD1_0X2_SIZE 2
/* STRONG: Mr Stone/totem damage phase: zero normal, one requested, two reacting, three defeated. */
#define DOS_BB_DATA_T_FIELD2_0X4_OFFSET 0x4
#define DOS_BB_DATA_T_FIELD2_0X4_SIZE 1
/* STRONG: Saved interrupted main state in the damage response. */
#define DOS_BB_DATA_T_SAVED_MAIN_ETAT_OFFSET 0x5
#define DOS_BB_DATA_T_SAVED_MAIN_ETAT_SIZE 1
/* STRONG: Saved interrupted substate in the damage response. */
#define DOS_BB_DATA_T_SAVED_SUB_ETAT_OFFSET 0x6
#define DOS_BB_DATA_T_SAVED_SUB_ETAT_SIZE 1
/* UNKNOWN: No intended meaning established for bb_data_t._pad_7. Observed layout and typed accesses are retained without a rename. */
#define DOS_BB_DATA_T__PAD_7_OFFSET 0x7
#define DOS_BB_DATA_T__PAD_7_SIZE 1
/* STRONG: Saved script command during damage, also reused as a signed defeat countdown. */
#define DOS_BB_DATA_T_SAVED_CMD_OFFSET 0x8
#define DOS_BB_DATA_T_SAVED_CMD_SIZE 2
/* STRONG: Sprite-6 contact x in the main fight; reused as phase scratch in later encounters. */
#define DOS_BB_DATA_T_SPRITE6_X_OFFSET 0xa
#define DOS_BB_DATA_T_SPRITE6_X_SIZE 2
/* STRONG: Sprite-6 contact y in the main fight; reused as scroll/phase scratch in later encounters. */
#define DOS_BB_DATA_T_SPRITE6_Y_OFFSET 0xc
#define DOS_BB_DATA_T_SPRITE6_Y_SIZE 2
/* STRONG: Multipurpose attack/intro counter, including successive overhead projectile positions. */
#define DOS_BB_DATA_T_FIELD8_0XE_OFFSET 0xe
#define DOS_BB_DATA_T_FIELD8_0XE_SIZE 1
/* UNKNOWN: No intended meaning established for bb_data_t._pad_15. Observed layout and typed accesses are retained without a rename. */
#define DOS_BB_DATA_T__PAD_15_OFFSET 0xf
#define DOS_BB_DATA_T__PAD_15_SIZE 1
/* STRONG: Mr Stone attack selector; 255 terminates the row. */
#define DOS_BB_ATTACK_ENTRY_T_ATTACK_OFFSET 0x0
#define DOS_BB_ATTACK_ENTRY_T_ATTACK_SIZE 2
/* STRONG: Wait/repetition count loaded into WaitForFinAtan for the selected attack. */
#define DOS_BB_ATTACK_ENTRY_T_WAIT_FOR_FIN_ATAN_OFFSET 0x2
#define DOS_BB_ATTACK_ENTRY_T_WAIT_FOR_FIN_ATAN_SIZE 1
/* UNKNOWN: No intended meaning established for bb_attack_entry_t.reserved. Observed layout and typed accesses are retained without a rename. */
#define DOS_BB_ATTACK_ENTRY_T_RESERVED_OFFSET 0x3
#define DOS_BB_ATTACK_ENTRY_T_RESERVED_SIZE 1
#define DOS_COUTEAUX_INFO_T_SIZE 20
#define DOS_PMA_BLOCK_FLAGS_T_SIZE 1
/* STRONG: Knife target horizontal coordinate; staging readers can add camera x and attack aiming writers use Rayman coordinates. */
#define DOS_COUTEAUX_INFO_T_X_POS_OFFSET 0x0
#define DOS_COUTEAUX_INFO_T_X_POS_SIZE 2
/* STRONG: Knife target vertical coordinate; staging readers can add camera y and attack aiming writers use Rayman coordinates. */
#define DOS_COUTEAUX_INFO_T_Y_POS_OFFSET 0x2
#define DOS_COUTEAUX_INFO_T_Y_POS_SIZE 2
/* STRONG: Knife launch delay/state latch, counting down to one for preparation and zero for travel. */
#define DOS_COUTEAUX_INFO_T_FIELD2_0X4_OFFSET 0x4
#define DOS_COUTEAUX_INFO_T_FIELD2_0X4_SIZE 2
/* STRONG: Knife return delay; the next knife waits for the preceding entry to reach zero. */
#define DOS_COUTEAUX_INFO_T_FIELD3_0X6_OFFSET 0x6
#define DOS_COUTEAUX_INFO_T_FIELD3_0X6_SIZE 2
/* STRONG: Fifteen-step preparation/flicker countdown used before restoring launch velocity. */
#define DOS_COUTEAUX_INFO_T_FIELD4_0X8_OFFSET 0x8
#define DOS_COUTEAUX_INFO_T_FIELD4_0X8_SIZE 2
/* STRONG: Saved horizontal knife launch velocity restored after the preparation pause. */
#define DOS_COUTEAUX_INFO_T_FIELD5_0XA_OFFSET 0xa
#define DOS_COUTEAUX_INFO_T_FIELD5_0XA_SIZE 2
/* STRONG: Saved vertical knife launch velocity restored after the preparation pause. */
#define DOS_COUTEAUX_INFO_T_FIELD6_0XC_OFFSET 0xc
#define DOS_COUTEAUX_INFO_T_FIELD6_0XC_SIZE 2
/* STRONG: Index of the level object bound to this knife slot. */
#define DOS_COUTEAUX_INFO_T_FIELD7_0XE_OFFSET 0xe
#define DOS_COUTEAUX_INFO_T_FIELD7_0XE_SIZE 2
/* STRONG: Saved directional animation frame restored during the preparation pause. */
#define DOS_COUTEAUX_INFO_T_RECORD_FLAG_OFFSET 0x10
#define DOS_COUTEAUX_INFO_T_RECORD_FLAG_SIZE 1
/* STRONG: Knife arrival/completion flag counted by check_couteaux; distinct from object activation. */
#define DOS_COUTEAUX_INFO_T_ACTIVE_OFFSET 0x11
#define DOS_COUTEAUX_INFO_T_ACTIVE_SIZE 1
/* STRONG: Numeric per-knife launch-mode selector; 0/1/2/3 choose different directional/ballistic setup. */
#define DOS_COUTEAUX_INFO_T_FIELD9_0X12_OFFSET 0x12
#define DOS_COUTEAUX_INFO_T_FIELD9_0X12_SIZE 1
/* UNKNOWN: No intended meaning established for couteaux_info_t.dos_tail. Observed layout and typed accesses are retained without a rename. */
#define DOS_COUTEAUX_INFO_T_DOS_TAIL_OFFSET 0x13
#define DOS_COUTEAUX_INFO_T_DOS_TAIL_SIZE 1
/* UNKNOWN: No intended meaning established for pma_block_flags_t.raw. Observed layout and typed accesses are retained without a rename. */
#define DOS_PMA_BLOCK_FLAGS_T_RAW_OFFSET 0x0
#define DOS_PMA_BLOCK_FLAGS_T_RAW_SIZE 1
/* UNKNOWN: No intended meaning established for pma_block_flags_t.bits. Observed layout and typed accesses are retained without a rename. */
#define DOS_PMA_BLOCK_FLAGS_T_BITS_OFFSET 0x0
#define DOS_PMA_BLOCK_FLAGS_T_BITS_SIZE 1
/* UNKNOWN: No intended meaning established for pma_block_flags_t.bits.unused0. Observed layout and typed accesses are retained without a rename. */
#define DOS_PMA_BLOCK_FLAGS_T_BITS_UNUSED0_OFFSET 0x0
#define DOS_PMA_BLOCK_FLAGS_T_BITS_UNUSED0_SIZE 1
#define DOS_PMA_BLOCK_FLAGS_T_BITS_UNUSED0_MASK 0x01
/* UNKNOWN: No intended meaning established for pma_block_flags_t.bits.solid. Observed layout and typed accesses are retained without a rename. */
#define DOS_PMA_BLOCK_FLAGS_T_BITS_SOLID_OFFSET 0x0
#define DOS_PMA_BLOCK_FLAGS_T_BITS_SOLID_SIZE 1
#define DOS_PMA_BLOCK_FLAGS_T_BITS_SOLID_MASK 0x02
/* UNKNOWN: No intended meaning established for pma_block_flags_t.bits.unused1. Observed layout and typed accesses are retained without a rename. */
#define DOS_PMA_BLOCK_FLAGS_T_BITS_UNUSED1_OFFSET 0x0
#define DOS_PMA_BLOCK_FLAGS_T_BITS_UNUSED1_SIZE 1
#define DOS_PMA_BLOCK_FLAGS_T_BITS_UNUSED1_MASK 0xfc
#define DOS_BANDE_T_SIZE 24
#define DOS_DEF_BANDE_T_SIZE 8
#define DOS_DEF_SPRITE_T_SIZE 8
/* STRONG: Band extent along the splitting axis; zero marks the foreground-sprite band. */
#define DOS_BANDE_T_LENGTH_OFFSET 0x0
#define DOS_BANDE_T_LENGTH_SIZE 2
/* STRONG: Cumulative band starting offset along the splitting axis. */
#define DOS_BANDE_T_OFFSET_OFFSET 0x2
#define DOS_BANDE_T_OFFSET_SIZE 2
/* STRONG: Scroll update mode: zero parallax, 1/2 autonomous signs, 4/5 combined parallax and autonomous motion. */
#define DOS_BANDE_T_FIELD_4_OFFSET 0x4
#define DOS_BANDE_T_FIELD_4_SIZE 1
/* UNKNOWN: No intended meaning established for bande_t._pad_5. Observed layout and typed accesses are retained without a rename. */
#define DOS_BANDE_T__PAD_5_OFFSET 0x5
#define DOS_BANDE_T__PAD_5_SIZE 1
/* STRONG: Current wrapped scroll displacement in pixels. */
#define DOS_BANDE_T_FIELD_6_OFFSET 0x6
#define DOS_BANDE_T_FIELD_6_SIZE 2
/* STRONG: Camera parallax divisor copied from def_bande_t.field_2. */
#define DOS_BANDE_T_FIELD_8_OFFSET 0x8
#define DOS_BANDE_T_FIELD_8_SIZE 1
/* STRONG: Autonomous scroll increment in sixteenths copied from def_bande_t.field_4. */
#define DOS_BANDE_T_FIELD_9_OFFSET 0x9
#define DOS_BANDE_T_FIELD_9_SIZE 1
/* UNKNOWN: No intended meaning established for bande_t._pad_0a. Observed layout and typed accesses are retained without a rename. */
#define DOS_BANDE_T__PAD_0A_OFFSET 0xa
#define DOS_BANDE_T__PAD_0A_SIZE 2
/* STRONG: Source band-start pointer, with row/column addressing selected by scroll mode. */
#define DOS_BANDE_T_SOURCE_BUFFER_POS_OFFSET 0xc
#define DOS_BANDE_T_SOURCE_BUFFER_POS_SIZE 4
/* STRONG: Destination band-start pointer, using 320-byte screen rows in horizontal mode. */
#define DOS_BANDE_T_DRAW_BUFFER_POS_OFFSET 0x10
#define DOS_BANDE_T_DRAW_BUFFER_POS_SIZE 4
/* STRONG: Autonomous scroll accumulator in sixteenths, wrapped by sixteen times the plane extent. */
#define DOS_BANDE_T_FIELD_14_OFFSET 0x14
#define DOS_BANDE_T_FIELD_14_SIZE 2
/* UNKNOWN: No intended meaning established for bande_t._pad_16. Observed layout and typed accesses are retained without a rename. */
#define DOS_BANDE_T__PAD_16_OFFSET 0x16
#define DOS_BANDE_T__PAD_16_SIZE 2
/* STRONG: Initial band extent copied to the runtime band. */
#define DOS_DEF_BANDE_T_LENGTH_OFFSET 0x0
#define DOS_DEF_BANDE_T_LENGTH_SIZE 2
/* STRONG: Initial camera parallax divisor copied to bande_t.field_8. */
#define DOS_DEF_BANDE_T_FIELD_2_OFFSET 0x2
#define DOS_DEF_BANDE_T_FIELD_2_SIZE 1
/* UNKNOWN: No intended meaning established for def_bande_t._pad_3. Observed layout and typed accesses are retained without a rename. */
#define DOS_DEF_BANDE_T__PAD_3_OFFSET 0x3
#define DOS_DEF_BANDE_T__PAD_3_SIZE 1
/* STRONG: Initial autonomous scroll increment copied to bande_t.field_9. */
#define DOS_DEF_BANDE_T_FIELD_4_OFFSET 0x4
#define DOS_DEF_BANDE_T_FIELD_4_SIZE 1
/* UNKNOWN: No intended meaning established for def_bande_t._pad_5. Observed layout and typed accesses are retained without a rename. */
#define DOS_DEF_BANDE_T__PAD_5_OFFSET 0x5
#define DOS_DEF_BANDE_T__PAD_5_SIZE 1
/* STRONG: Initial scroll update mode copied to bande_t.field_4. */
#define DOS_DEF_BANDE_T_FIELD_6_OFFSET 0x6
#define DOS_DEF_BANDE_T_FIELD_6_SIZE 1
/* UNKNOWN: No intended meaning established for def_bande_t._pad_7. Observed layout and typed accesses are retained without a rename. */
#define DOS_DEF_BANDE_T__PAD_7_OFFSET 0x7
#define DOS_DEF_BANDE_T__PAD_7_SIZE 1
/* STRONG: Background sprite horizontal placement copied into the runtime Sprite table. */
#define DOS_DEF_SPRITE_T_X_OFFSET 0x0
#define DOS_DEF_SPRITE_T_X_SIZE 2
/* STRONG: Background sprite vertical placement copied into the runtime Sprite table. */
#define DOS_DEF_SPRITE_T_Y_OFFSET 0x2
#define DOS_DEF_SPRITE_T_Y_SIZE 2
/* STRONG: Index of the scrolling band whose displacement and layering apply to the sprite. */
#define DOS_DEF_SPRITE_T_BANDE_INDEX_OFFSET 0x4
#define DOS_DEF_SPRITE_T_BANDE_INDEX_SIZE 2
/* UNKNOWN: No intended meaning established for def_sprite_t._pad_6. Observed layout and typed accesses are retained without a rename. */
#define DOS_DEF_SPRITE_T__PAD_6_OFFSET 0x6
#define DOS_DEF_SPRITE_T__PAD_6_SIZE 2
#define DOS_BOOL_SIZE 1
#define DOS_SAX_ATTACK_ENTRY_T_SIZE 4
#define DOS_SAX_NOTE_ENTRY_T_SIZE 10
#define DOS_SAX_DATA_T_SIZE 18
/* STRONG: Next note or boss-action selector returned by PrepareAtak. */
#define DOS_SAX_ATTACK_ENTRY_T_NEXT_NOTE_OFFSET 0x0
#define DOS_SAX_ATTACK_ENTRY_T_NEXT_NOTE_SIZE 1
/* STRONG: Delay loaded as time+1 after a type-74 note shot. */
#define DOS_SAX_ATTACK_ENTRY_T_TIME_OFFSET 0x1
#define DOS_SAX_ATTACK_ENTRY_T_TIME_SIZE 1
/* STRONG: Nonzero attack-row terminator that resets IndexAtak. */
#define DOS_SAX_ATTACK_ENTRY_T_END_OFFSET 0x2
#define DOS_SAX_ATTACK_ENTRY_T_END_SIZE 2
/* STRONG: Note type offset added to ninety; also initializes the note substate. */
#define DOS_SAX_NOTE_ENTRY_T_TYPE_OFFSET 0x0
#define DOS_SAX_NOTE_ENTRY_T_TYPE_SIZE 1
/* UNKNOWN: No intended meaning established for sax_note_entry_t.dos_alignment. Observed layout and typed accesses are retained without a rename. */
#define DOS_SAX_NOTE_ENTRY_T_DOS_ALIGNMENT_OFFSET 0x1
#define DOS_SAX_NOTE_ENTRY_T_DOS_ALIGNMENT_SIZE 1
/* STRONG: Horizontal launch magnitude, negated for the opposite facing. */
#define DOS_SAX_NOTE_ENTRY_T_SPEED_X_OFFSET 0x2
#define DOS_SAX_NOTE_ENTRY_T_SPEED_X_SIZE 2
/* STRONG: Initial vertical launch velocity. */
#define DOS_SAX_NOTE_ENTRY_T_SPEED_Y_OFFSET 0x4
#define DOS_SAX_NOTE_ENTRY_T_SPEED_Y_SIZE 2
/* STRONG: Value assigned to note iframes_timer; type 92 uses it as a split count. */
#define DOS_SAX_NOTE_ENTRY_T_INITIAL_IFRAMES_OFFSET 0x6
#define DOS_SAX_NOTE_ENTRY_T_INITIAL_IFRAMES_SIZE 2
/* UNKNOWN: No intended meaning established for sax_note_entry_t.field4_0x8. Observed layout and typed accesses are retained without a rename. */
#define DOS_SAX_NOTE_ENTRY_T_FIELD4_0X8_OFFSET 0x8
#define DOS_SAX_NOTE_ENTRY_T_FIELD4_0X8_SIZE 2
/* STRONG: Mr Sax horizontal position scratch used by encounter code. */
#define DOS_SAX_DATA_T_X_POS_OFFSET 0x0
#define DOS_SAX_DATA_T_X_POS_SIZE 2
/* STRONG: Mr Sax vertical position scratch used by encounter code. */
#define DOS_SAX_DATA_T_Y_POS_OFFSET 0x2
#define DOS_SAX_DATA_T_Y_POS_SIZE 2
/* STRONG: World x of the twenty-pixel-wide reflected-note hit box derived from sprite 2. */
#define DOS_SAX_DATA_T_NOTE_BOX_COLL_X_OFFSET 0x4
#define DOS_SAX_DATA_T_NOTE_BOX_COLL_X_SIZE 2
/* STRONG: World y of the 140-pixel-high reflected-note hit box derived from sprite 5 plus twenty. */
#define DOS_SAX_DATA_T_NOTE_BOX_COLL_Y_OFFSET 0x6
#define DOS_SAX_DATA_T_NOTE_BOX_COLL_Y_SIZE 2
/* STRONG: Adjusted sprite-2 horizontal firing anchor. */
#define DOS_SAX_DATA_T_SPRITE2_X_OFFSET 0x8
#define DOS_SAX_DATA_T_SPRITE2_X_SIZE 2
/* STRONG: Adjusted sprite-2 vertical firing anchor. */
#define DOS_SAX_DATA_T_SPRITE2_Y_OFFSET 0xa
#define DOS_SAX_DATA_T_SPRITE2_Y_SIZE 2
/* STRONG: Damage coordination latch: one requests a hit, two denotes a reaction, zero clears it. */
#define DOS_SAX_DATA_T_COUP_OFFSET 0xc
#define DOS_SAX_DATA_T_COUP_SIZE 1
/* STRONG: HP value restored by INIT_SAXO for type 74. */
#define DOS_SAX_DATA_T_SAVED_HP_OFFSET 0xd
#define DOS_SAX_DATA_T_SAVED_HP_SIZE 1
/* STRONG: Interrupted main state restored after the hit animation. */
#define DOS_SAX_DATA_T_FIELD8_0XE_OFFSET 0xe
#define DOS_SAX_DATA_T_FIELD8_0XE_SIZE 1
/* STRONG: Interrupted substate restored after the hit animation. */
#define DOS_SAX_DATA_T_FIELD9_0XF_OFFSET 0xf
#define DOS_SAX_DATA_T_FIELD9_0XF_SIZE 1
/* STRONG: Signed encounter countdown/action scratch, including main-fight defeat timing. */
#define DOS_SAX_DATA_T_FIELD10_0X10_OFFSET 0x10
#define DOS_SAX_DATA_T_FIELD10_0X10_SIZE 2
#define DOS_VITRAUX_INFO_T_SIZE 10
/* STRONG: Window object's horizontal center captured by init_vitraux. */
#define DOS_VITRAUX_INFO_T_X_POS_OFFSET 0x0
#define DOS_VITRAUX_INFO_T_X_POS_SIZE 2
/* STRONG: Window object's bottom position captured by init_vitraux. */
#define DOS_VITRAUX_INFO_T_Y_POS_OFFSET 0x2
#define DOS_VITRAUX_INFO_T_Y_POS_SIZE 2
/* STRONG: Byte level-object index bound to the window slot. */
#define DOS_VITRAUX_INFO_T_OBJ_ID_OFFSET 0x4
#define DOS_VITRAUX_INFO_T_OBJ_ID_SIZE 1
/* STRONG: Slot index written during window registration. */
#define DOS_VITRAUX_INFO_T_INDEX_OFFSET 0x5
#define DOS_VITRAUX_INFO_T_INDEX_SIZE 1
/* UNKNOWN: No intended meaning established for vitraux_info_t.field_6. Observed layout and typed accesses are retained without a rename. */
#define DOS_VITRAUX_INFO_T_FIELD_6_OFFSET 0x6
#define DOS_VITRAUX_INFO_T_FIELD_6_SIZE 1
/* STRONG: Remaining display repeats, decremented whenever the 100-tick period expires. */
#define DOS_VITRAUX_INFO_T_MODE_OFFSET 0x7
#define DOS_VITRAUX_INFO_T_MODE_SIZE 1
/* STRONG: Window display countdown, reloaded to one hundred on each repeat. */
#define DOS_VITRAUX_INFO_T_VALUE_OFFSET 0x8
#define DOS_VITRAUX_INFO_T_VALUE_SIZE 2
#define DOS_OBJTYPE_SIZE 1
#define DOS_MITE_POING_LOCAL_T_SIZE 20
/* UNKNOWN: No intended meaning established for mite_poing_local_t.y_16. Observed layout and typed accesses are retained without a rename. */
#define DOS_MITE_POING_LOCAL_T_Y_16_OFFSET 0x0
#define DOS_MITE_POING_LOCAL_T_Y_16_SIZE 4
/* UNKNOWN: No intended meaning established for mite_poing_local_t.field_4. Observed layout and typed accesses are retained without a rename. */
#define DOS_MITE_POING_LOCAL_T_FIELD_4_OFFSET 0x4
#define DOS_MITE_POING_LOCAL_T_FIELD_4_SIZE 2
/* UNKNOWN: No intended meaning established for mite_poing_local_t.speed_x. Observed layout and typed accesses are retained without a rename. */
#define DOS_MITE_POING_LOCAL_T_SPEED_X_OFFSET 0x6
#define DOS_MITE_POING_LOCAL_T_SPEED_X_SIZE 2
/* UNKNOWN: No intended meaning established for mite_poing_local_t.charge. Observed layout and typed accesses are retained without a rename. */
#define DOS_MITE_POING_LOCAL_T_CHARGE_OFFSET 0x8
#define DOS_MITE_POING_LOCAL_T_CHARGE_SIZE 2
/* UNKNOWN: No intended meaning established for mite_poing_local_t.field_A. Observed layout and typed accesses are retained without a rename. */
#define DOS_MITE_POING_LOCAL_T_FIELD_A_OFFSET 0xa
#define DOS_MITE_POING_LOCAL_T_FIELD_A_SIZE 2
/* UNKNOWN: No intended meaning established for mite_poing_local_t.sub_etat. Observed layout and typed accesses are retained without a rename. */
#define DOS_MITE_POING_LOCAL_T_SUB_ETAT_OFFSET 0xc
#define DOS_MITE_POING_LOCAL_T_SUB_ETAT_SIZE 1
/* UNKNOWN: No intended meaning established for mite_poing_local_t.is_returning. Observed layout and typed accesses are retained without a rename. */
#define DOS_MITE_POING_LOCAL_T_IS_RETURNING_OFFSET 0xd
#define DOS_MITE_POING_LOCAL_T_IS_RETURNING_SIZE 1
/* UNKNOWN: No intended meaning established for mite_poing_local_t.is_active. Observed layout and typed accesses are retained without a rename. */
#define DOS_MITE_POING_LOCAL_T_IS_ACTIVE_OFFSET 0xe
#define DOS_MITE_POING_LOCAL_T_IS_ACTIVE_SIZE 1
/* UNKNOWN: No intended meaning established for mite_poing_local_t.damage. Observed layout and typed accesses are retained without a rename. */
#define DOS_MITE_POING_LOCAL_T_DAMAGE_OFFSET 0xf
#define DOS_MITE_POING_LOCAL_T_DAMAGE_SIZE 1
/* UNKNOWN: No intended meaning established for mite_poing_local_t.is_charging. Observed layout and typed accesses are retained without a rename. */
#define DOS_MITE_POING_LOCAL_T_IS_CHARGING_OFFSET 0x10
#define DOS_MITE_POING_LOCAL_T_IS_CHARGING_SIZE 1
/* UNKNOWN: No intended meaning established for mite_poing_local_t.is_boum. Observed layout and typed accesses are retained without a rename. */
#define DOS_MITE_POING_LOCAL_T_IS_BOUM_OFFSET 0x11
#define DOS_MITE_POING_LOCAL_T_IS_BOUM_SIZE 1
/* UNKNOWN: No intended meaning established for mite_poing_local_t.field_12. Observed layout and typed accesses are retained without a rename. */
#define DOS_MITE_POING_LOCAL_T_FIELD_12_OFFSET 0x12
#define DOS_MITE_POING_LOCAL_T_FIELD_12_SIZE 1
/* UNKNOWN: No intended meaning established for mite_poing_local_t.field_13. Observed layout and typed accesses are retained without a rename. */
#define DOS_MITE_POING_LOCAL_T_FIELD_13_OFFSET 0x13
#define DOS_MITE_POING_LOCAL_T_FIELD_13_SIZE 1
#define DOS_CB_COLLISION_T_SIZE 8
#define DOS_CASSE_BRIQUE_ENTRY_T_SIZE 8
/* STRONG: Horizontal ball-to-brick separation retained for the chosen contact. */
#define DOS_CB_COLLISION_T_DX_OFFSET 0x0
#define DOS_CB_COLLISION_T_DX_SIZE 2
/* STRONG: Vertical ball-to-brick separation retained for the chosen contact. */
#define DOS_CB_COLLISION_T_DY_OFFSET 0x2
#define DOS_CB_COLLISION_T_DY_SIZE 2
/* STRONG: Whether the chosen contact reflects the ball's horizontal velocity. */
#define DOS_CB_COLLISION_T_REFLECT_X_OFFSET 0x4
#define DOS_CB_COLLISION_T_REFLECT_X_SIZE 1
/* STRONG: Whether the chosen contact reflects the ball's vertical velocity. */
#define DOS_CB_COLLISION_T_REFLECT_Y_OFFSET 0x5
#define DOS_CB_COLLISION_T_REFLECT_Y_SIZE 1
/* STRONG: Index of the contacted runtime brick slot. */
#define DOS_CB_COLLISION_T_INDEX_OFFSET 0x6
#define DOS_CB_COLLISION_T_INDEX_SIZE 1
/* UNKNOWN: No intended meaning established for cb_collision_t.reserved. Observed layout and typed accesses are retained without a rename. */
#define DOS_CB_COLLISION_T_RESERVED_OFFSET 0x7
#define DOS_CB_COLLISION_T_RESERVED_SIZE 1
/* STRONG: Brick x offset within its formation, read from animation layout. */
#define DOS_CASSE_BRIQUE_ENTRY_T_X_OFFSET 0x0
#define DOS_CASSE_BRIQUE_ENTRY_T_X_SIZE 2
/* STRONG: Brick y offset within its formation, read from animation layout. */
#define DOS_CASSE_BRIQUE_ENTRY_T_Y_OFFSET 0x2
#define DOS_CASSE_BRIQUE_ENTRY_T_Y_SIZE 2
/* STRONG: Index of the pooled level object used by this brick slot. */
#define DOS_CASSE_BRIQUE_ENTRY_T_OBJECT_INDEX_OFFSET 0x4
#define DOS_CASSE_BRIQUE_ENTRY_T_OBJECT_INDEX_SIZE 2
/* STRONG: Saved layout brick kind derived from the animation sprite ID. */
#define DOS_CASSE_BRIQUE_ENTRY_T_KIND_OFFSET 0x6
#define DOS_CASSE_BRIQUE_ENTRY_T_KIND_SIZE 1
/* UNKNOWN: No intended meaning established for casse_brique_entry_t.reserved. Observed layout and typed accesses are retained without a rename. */
#define DOS_CASSE_BRIQUE_ENTRY_T_RESERVED_OFFSET 0x7
#define DOS_CASSE_BRIQUE_ENTRY_T_RESERVED_SIZE 1
#define DOS_OPTIONS_T_SIZE 20
/* PROVEN: Overloaded menu working slot. Keys use first7 signed16 slots as left/up/right/down/fist/jump/action. Audio uses music/volume;pad slots1/2/3 are jump/fist/action. Graphics slots1..6 are video selector,clip preset,frequency,fixon,background,differential;video menu slots0/1/2 are one-hot normal/ModeX/P486 choices. Interpret only in active screen context. */
#define DOS_OPTIONS_T_MUSIC_ENABLED_OFFSET 0x0
#define DOS_OPTIONS_T_MUSIC_ENABLED_SIZE 2
/* PROVEN: Overloaded menu working slot. Keys use first7 signed16 slots as left/up/right/down/fist/jump/action. Audio uses music/volume;pad slots1/2/3 are jump/fist/action. Graphics slots1..6 are video selector,clip preset,frequency,fixon,background,differential;video menu slots0/1/2 are one-hot normal/ModeX/P486 choices. Interpret only in active screen context. */
#define DOS_OPTIONS_T_SOUND_VOLUME_OFFSET 0x2
#define DOS_OPTIONS_T_SOUND_VOLUME_SIZE 2
/* PROVEN: Overloaded menu working slot. Keys use first7 signed16 slots as left/up/right/down/fist/jump/action. Audio uses music/volume;pad slots1/2/3 are jump/fist/action. Graphics slots1..6 are video selector,clip preset,frequency,fixon,background,differential;video menu slots0/1/2 are one-hot normal/ModeX/P486 choices. Interpret only in active screen context. */
#define DOS_OPTIONS_T_SIZESCREEN_OFFSET 0x4
#define DOS_OPTIONS_T_SIZESCREEN_SIZE 2
/* PROVEN: Overloaded menu working slot. Keys use first7 signed16 slots as left/up/right/down/fist/jump/action. Audio uses music/volume;pad slots1/2/3 are jump/fist/action. Graphics slots1..6 are video selector,clip preset,frequency,fixon,background,differential;video menu slots0/1/2 are one-hot normal/ModeX/P486 choices. Interpret only in active screen context. */
#define DOS_OPTIONS_T_FREQUENCE_OFFSET 0x6
#define DOS_OPTIONS_T_FREQUENCE_SIZE 2
/* PROVEN: Overloaded menu working slot. Keys use first7 signed16 slots as left/up/right/down/fist/jump/action. Audio uses music/volume;pad slots1/2/3 are jump/fist/action. Graphics slots1..6 are video selector,clip preset,frequency,fixon,background,differential;video menu slots0/1/2 are one-hot normal/ModeX/P486 choices. Interpret only in active screen context. */
#define DOS_OPTIONS_T_FIXON_OFFSET 0x8
#define DOS_OPTIONS_T_FIXON_SIZE 2
/* PROVEN: Overloaded menu working slot. Keys use first7 signed16 slots as left/up/right/down/fist/jump/action. Audio uses music/volume;pad slots1/2/3 are jump/fist/action. Graphics slots1..6 are video selector,clip preset,frequency,fixon,background,differential;video menu slots0/1/2 are one-hot normal/ModeX/P486 choices. Interpret only in active screen context. */
#define DOS_OPTIONS_T_BACKGROUNDON_OFFSET 0xa
#define DOS_OPTIONS_T_BACKGROUNDON_SIZE 2
/* PROVEN: Overloaded menu working slot. Keys use first7 signed16 slots as left/up/right/down/fist/jump/action. Audio uses music/volume;pad slots1/2/3 are jump/fist/action. Graphics slots1..6 are video selector,clip preset,frequency,fixon,background,differential;video menu slots0/1/2 are one-hot normal/ModeX/P486 choices. Interpret only in active screen context. */
#define DOS_OPTIONS_T_SCROLLDIFFON_OFFSET 0xc
#define DOS_OPTIONS_T_SCROLLDIFFON_SIZE 2
/* UNKNOWN: Declared signed16 working-record storage;no value-consuming use established. */
#define DOS_OPTIONS_T_FIELD_E_OFFSET 0xe
#define DOS_OPTIONS_T_FIELD_E_SIZE 2
/* UNKNOWN: Declared signed16 working-record storage;no value-consuming use established. */
#define DOS_OPTIONS_T_FIELD_10_OFFSET 0x10
#define DOS_OPTIONS_T_FIELD_10_SIZE 2
/* UNKNOWN: Declared signed16 working-record storage;no value-consuming use established. */
#define DOS_OPTIONS_T_FIELD_12_OFFSET 0x12
#define DOS_OPTIONS_T_FIELD_12_SIZE 2
#define DOS_SYNCHRO_LOOP_PALETTE_FUNC_T_SIZE 4
#define DOS_WORLD_INFO_SAVE_T_SIZE 1
/* PROVEN: Whole serialized byte;field writes preserve unrelated high3 bits. */
#define DOS_WORLD_INFO_SAVE_T_RAW_OFFSET 0x0
#define DOS_WORLD_INFO_SAVE_T_RAW_SIZE 1
/* PROVEN: Packed bit0 copies runtime accessible flag. */
#define DOS_WORLD_INFO_SAVE_T_IS_UNLOCKED_OFFSET 0x0
#define DOS_WORLD_INFO_SAVE_T_IS_UNLOCKED_SIZE 1
#define DOS_WORLD_INFO_SAVE_T_IS_UNLOCKED_MASK 0x01
/* PROVEN: Packed bit1 copies runtime pending bit2;not runtime scratch bit1. */
#define DOS_WORLD_INFO_SAVE_T_IS_UNLOCKING_OFFSET 0x0
#define DOS_WORLD_INFO_SAVE_T_IS_UNLOCKING_SIZE 1
#define DOS_WORLD_INFO_SAVE_T_IS_UNLOCKING_MASK 0x02
/* PROVEN: Packed bits2..4 store low3 cage-count bits,ordinary game count0..6. */
#define DOS_WORLD_INFO_SAVE_T_NB_CAGES_OFFSET 0x0
#define DOS_WORLD_INFO_SAVE_T_NB_CAGES_SIZE 1
#define DOS_WORLD_INFO_SAVE_T_NB_CAGES_MASK 0x1c
/* UNKNOWN: High3 bits retained by store/retrieve field operations;no semantic consumer established. */
#define DOS_WORLD_INFO_SAVE_T_UNUSED_OFFSET 0x0
#define DOS_WORLD_INFO_SAVE_T_UNUSED_SIZE 1
#define DOS_WORLD_INFO_SAVE_T_UNUSED_MASK 0xe0
#define DOS_BUTTON_TEST_FN_SIZE 4
#define DOS_KEY_TEST_FN_SIZE 4
#define DOS_INPUT_FN_SIZE 4
#define DOS_SWAP_FUNC_T_SIZE 4
#define DOS_KEYBOARD_HANDLER_FN_SIZE 6
#define DOS_DOS_VECTOR_T_SIZE 6
#define DOS_TEMP_MEM_ENTRY_T_SIZE 12
/* PROVEN: Block byte capacity;zero for unused descriptor. */
#define DOS_TEMP_MEM_ENTRY_T_CAPACITY_OFFSET 0x0
#define DOS_TEMP_MEM_ENTRY_T_CAPACITY_SIZE 4
/* PROVEN: Block start pointer;unused descriptor points to pool end. */
#define DOS_TEMP_MEM_ENTRY_T_DATA_OFFSET 0x4
#define DOS_TEMP_MEM_ENTRY_T_DATA_SIZE 4
/* PROVEN: Nonzero means live block;zero allows best-fit allocation/coalescing. */
#define DOS_TEMP_MEM_ENTRY_T_ALLOCATED_OFFSET 0x8
#define DOS_TEMP_MEM_ENTRY_T_ALLOCATED_SIZE 1
#define DOS_DPMI_MEMORY_INFO_T_SIZE 48
/* PROVEN: First output dword of INT31h0500h;returned by get_avail_memory. */
#define DOS_DPMI_MEMORY_INFO_T_LARGEST_FREE_BLOCK_OFFSET 0x0
#define DOS_DPMI_MEMORY_INFO_T_LARGEST_FREE_BLOCK_SIZE 4
/* UNKNOWN: Remaining11 DPMI output dwords;not consumed by this unit. Do not invent per-element names. */
#define DOS_DPMI_MEMORY_INFO_T_REMAINING_INFO_OFFSET 0x4
#define DOS_DPMI_MEMORY_INFO_T_REMAINING_INFO_SIZE 44
#define DOS_MEM_PEAK_T_SIZE 8
/* PROVEN: Largest observed arena cursor. */
#define DOS_MEM_PEAK_T_USED_OFFSET 0x0
#define DOS_MEM_PEAK_T_USED_SIZE 4
/* PROVEN: Current num_world when cursor peak was replaced. */
#define DOS_MEM_PEAK_T_WORLD_OFFSET 0x4
#define DOS_MEM_PEAK_T_WORLD_SIZE 2
/* PROVEN: Current num_level when cursor peak was replaced. */
#define DOS_MEM_PEAK_T_LEVEL_OFFSET 0x6
#define DOS_MEM_PEAK_T_LEVEL_SIZE 2
#define DOS_MEM_USAGE_STATS_T_SIZE 40
/* PROVEN: Adjusted capacity of pooled temporary arena. */
#define DOS_MEM_USAGE_STATS_T_TEMP_CAPACITY_OFFSET 0x0
#define DOS_MEM_USAGE_STATS_T_TEMP_CAPACITY_SIZE 4
/* PROVEN: Peak cursor plus world/level location for the named arena. */
#define DOS_MEM_USAGE_STATS_T_FIX_OFFSET 0x4
#define DOS_MEM_USAGE_STATS_T_FIX_SIZE 8
/* PROVEN: Peak cursor plus world/level location for the named arena. */
#define DOS_MEM_USAGE_STATS_T_WORLD_OFFSET 0xc
#define DOS_MEM_USAGE_STATS_T_WORLD_SIZE 8
/* PROVEN: Peak cursor plus world/level location for the named arena. */
#define DOS_MEM_USAGE_STATS_T_LEVEL_OFFSET 0x14
#define DOS_MEM_USAGE_STATS_T_LEVEL_SIZE 8
/* PROVEN: Peak cursor plus world/level location for the named arena. */
#define DOS_MEM_USAGE_STATS_T_SPRITE_OFFSET 0x1c
#define DOS_MEM_USAGE_STATS_T_SPRITE_SIZE 8
/* PROVEN: Historical diagnostic high-water value;fallback updates compare pool capacity+single request,not sum of all live heap fallbacks. */
#define DOS_MEM_USAGE_STATS_T_TEMP_PEAK_OFFSET 0x24
#define DOS_MEM_USAGE_STATS_T_TEMP_PEAK_SIZE 4
#endif

