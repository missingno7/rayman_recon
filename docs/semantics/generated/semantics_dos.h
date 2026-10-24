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
/* PROVEN: Pointer to the object-specific sprites table. */
#define DOS_OBJ_T_SPRITES_OFFSET 0x0
#define DOS_OBJ_T_SPRITES_SIZE 4
/* PROVEN: Pointer to the object-specific animations table. */
#define DOS_OBJ_T_ANIMATIONS_OFFSET 0x4
#define DOS_OBJ_T_ANIMATIONS_SIZE 4
/* STRONG: Object sprite pixel-atlas buffer. */
#define DOS_OBJ_T_IMG_BUFFER_OFFSET 0x8
#define DOS_OBJ_T_IMG_BUFFER_SIZE 4
/* PROVEN: Pointer to the object-specific eta table. */
#define DOS_OBJ_T_ETA_OFFSET 0xc
#define DOS_OBJ_T_ETA_SIZE 4
/* STRONG: Object script stream/context member cmds. */
#define DOS_OBJ_T_CMDS_OFFSET 0x10
#define DOS_OBJ_T_CMDS_SIZE 4
/* STRONG: Object script stream/context member cmd_labels. */
#define DOS_OBJ_T_CMD_LABELS_OFFSET 0x14
#define DOS_OBJ_T_CMD_LABELS_SIZE 4
/* STRONG: Object script stream/context member cmd_contexts. */
#define DOS_OBJ_T_CMD_CONTEXTS_OFFSET 0x18
#define DOS_OBJ_T_CMD_CONTEXTS_SIZE 4
/* PROVEN: Overloaded 32-bit scratch mark: set by make_active/make_active2 and tested to suppress repeat linked activation; also set during save restoration for gate types179/242 and tested by INIT_OBJECTS. */
#define DOS_OBJ_T_FIELD_1C_OFFSET 0x1c
#define DOS_OBJ_T_FIELD_1C_SIZE 4
/* STRONG: Marker enabling linked-group operations; normally set for eligible non-self successor groups, not a proof that a generating door exists. */
#define DOS_OBJ_T_LINK_HAS_GENDOOR_OFFSET 0x20
#define DOS_OBJ_T_LINK_HAS_GENDOOR_SIZE 4
/* PROVEN: Object participation flag saved as the high bit of each two-bit snapshot entry. */
#define DOS_OBJ_T_IS_ACTIVE_OFFSET 0x24
#define DOS_OBJ_T_IS_ACTIVE_SIZE 4
/* PROVEN: Signed32 world pixel x origin in live object movement; loader also reuses object records as descriptors. */
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
/* STRONG: Initial position component init_x used by reset and relative-displacement logic. */
#define DOS_OBJ_T_INIT_X_OFFSET 0x3c
#define DOS_OBJ_T_INIT_X_SIZE 2
/* STRONG: Initial position component init_y used by reset and relative-displacement logic. */
#define DOS_OBJ_T_INIT_Y_OFFSET 0x3e
#define DOS_OBJ_T_INIT_Y_SIZE 2
/* PROVEN: Signed16 horizontal speed, interpreted per type axis flag as integer pixels/call or sixteenths of a pixel/call. */
#define DOS_OBJ_T_SPEED_X_OFFSET 0x40
#define DOS_OBJ_T_SPEED_X_SIZE 2
/* PROVEN: Signed16 vertical speed, interpreted per type axis flag as integer pixels/call or sixteenths of a pixel/call. */
#define DOS_OBJ_T_SPEED_Y_OFFSET 0x42
#define DOS_OBJ_T_SPEED_Y_SIZE 2
/* PROVEN: Count bounding the object sprite-table search. */
#define DOS_OBJ_T_NB_SPRITES_OFFSET 0x44
#define DOS_OBJ_T_NB_SPRITES_SIZE 2
/* STRONG: Object script stream/context member cmd_offset. */
#define DOS_OBJ_T_CMD_OFFSET_OFFSET 0x46
#define DOS_OBJ_T_CMD_OFFSET_SIZE 2
/* STRONG: Type-dependent script count or Rayman airborne-inertia selector. */
#define DOS_OBJ_T_NB_CMD_OFFSET 0x48
#define DOS_OBJ_T_NB_CMD_SIZE 2
/* STRONG: Type-dependent support/object id or reused vertical command parameter. */
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
/* UNKNOWN: Type-dependent signed16 scratch with proven Rayman fall, grapple-direction, door-id and initialization roles. */
#define DOS_OBJ_T_LINK_OFFSET 0x52
#define DOS_OBJ_T_LINK_SIZE 2
/* STRONG: Type-dependent Rayman tile index or support-distance code with several geometric conventions. */
#define DOS_OBJ_T_RAY_DIST_OFFSET 0x54
#define DOS_OBJ_T_RAY_DIST_SIZE 2
/* STRONG: Type-dependent signed16 response timer or reused command/state storage; Rayman uses -1 for vulnerability. */
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
/* PROVEN: Delay inhibiting reactivation, counted down while the living object is in its action zone. */
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
/* STRONG: Reset value for init_sub_etat. */
#define DOS_OBJ_T_INIT_SUB_ETAT_OFFSET 0x6e
#define DOS_OBJ_T_INIT_SUB_ETAT_SIZE 1
/* STRONG: Reset value for init_main_etat. */
#define DOS_OBJ_T_INIT_MAIN_ETAT_OFFSET 0x6f
#define DOS_OBJ_T_INIT_MAIN_ETAT_SIZE 1
/* STRONG: Current script command discriminator. */
#define DOS_OBJ_T_CMD_OFFSET 0x70
#define DOS_OBJ_T_CMD_SIZE 1
/* STRONG: Type-dependent gravity phase byte. */
#define DOS_OBJ_T_GRAVITY_VALUE_1_OFFSET 0x71
#define DOS_OBJ_T_GRAVITY_VALUE_1_SIZE 1
/* STRONG: Second type-dependent gravity phase byte or high-nibble10/11 period. */
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
/* STRONG: Reset value for init_hit_points. */
#define DOS_OBJ_T_INIT_HIT_POINTS_OFFSET 0x77
#define DOS_OBJ_T_INIT_HIT_POINTS_SIZE 1
/* UNKNOWN: Neutral obj_t.init_flag storage/view; its intended meaning is not established by this pilot. */
#define DOS_OBJ_T_INIT_FLAG_OFFSET 0x78
#define DOS_OBJ_T_INIT_FLAG_SIZE 1
/* PROVEN: Collision layer selector with three special dispatch codes. */
#define DOS_OBJ_T_HIT_SPRITE_OFFSET 0x79
#define DOS_OBJ_T_HIT_SPRITE_SIZE 1
/* STRONG: Type-dependent size/range parameter used by the detection-zone computation. */
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
/* STRONG: Object drawing-order bucket selector. */
#define DOS_OBJ_T_DISPLAY_PRIO_OFFSET 0x7e
#define DOS_OBJ_T_DISPLAY_PRIO_SIZE 1
/* UNKNOWN: Type-dependent byte countdown/state scratch with multiple independently established roles. */
#define DOS_OBJ_T_TIMER_OFFSET 0x7f
#define DOS_OBJ_T_TIMER_SIZE 1
/* STRONG: Number of animations in the object descriptor. */
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
/* STRONG: Pixel offset of this sprite in its object atlas buffer. */
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
/* PROVEN: Sprite horizontal flip bit XORed with object flip to select the bounds reflection. */
#define DOS_SPRITE_T_FLAGS_BITS_FLIP_X_OFFSET 0xb
#define DOS_SPRITE_T_FLAGS_BITS_FLIP_X_SIZE 1
#define DOS_SPRITE_T_FLAGS_BITS_FLIP_X_MASK 0x04
/* UNKNOWN: Neutral sprite_t.flags_bits.unused3 storage/view; its intended meaning is not established by this pilot. */
#define DOS_SPRITE_T_FLAGS_BITS_UNUSED3_OFFSET 0xb
#define DOS_SPRITE_T_FLAGS_BITS_UNUSED3_SIZE 1
#define DOS_SPRITE_T_FLAGS_BITS_UNUSED3_MASK 0xf8
/* PROVEN: Pointer to frame-major animation layers. */
#define DOS_ANIM_T_LAYERS_OFFSET 0x0
#define DOS_ANIM_T_LAYERS_SIZE 4
/* PROVEN: Pointer to one bounding rectangle per animation frame. */
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
/* STRONG: Layer horizontal mirror selector used by the renderer. */
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
/* UNKNOWN: Neutral map_tile_t.field_3 storage/view; its intended meaning is not established by this pilot. */
#define DOS_MAP_TILE_T_FIELD_3_OFFSET 0x3
#define DOS_MAP_TILE_T_FIELD_3_SIZE 1
/* PROVEN: Computed texture coverage classification:0 opaque,1 empty,2 mixed. */
#define DOS_MAP_TILE_T_TRANSPARENCY_OFFSET 0x4
#define DOS_MAP_TILE_T_TRANSPARENCY_SIZE 1
/* UNKNOWN: Neutral map_tile_t.field_5 storage/view; its intended meaning is not established by this pilot. */
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
/* UNKNOWN: Neutral poing_t.field_4 storage/view; its intended meaning is not established by this pilot. */
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
/* PROVEN: Byte selecting the palette255 box branch instead of the ordinary palette254/no-box branch. */
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
/* PROVEN: Alive-bit snapshot for object types141,164,179,242 indexed by object id bit. */
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
/* PROVEN: Snapshot of live circular successor ids, stored as u16. */
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
/* PROVEN: Saved fist power/substate selection. */
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
/* STRONG: Current byte cursor into the recorded demo stream. */
#define DOS_RECORD_T_CURRENT_OFFSET_OFFSET 0x0
#define DOS_RECORD_T_CURRENT_OFFSET_SIZE 4
/* STRONG: Length of the current repeated demo-input run. */
#define DOS_RECORD_T_REPEAT_LENGTH_OFFSET 0x4
#define DOS_RECORD_T_REPEAT_LENGTH_SIZE 4
/* STRONG: Progress within the current repeated demo-input run. */
#define DOS_RECORD_T_REPEAT_INDEX_OFFSET 0x8
#define DOS_RECORD_T_REPEAT_INDEX_SIZE 4
/* STRONG: Recorded demo stream length used by input playback. */
#define DOS_RECORD_T_LENGTH_OFFSET 0xc
#define DOS_RECORD_T_LENGTH_SIZE 4
/* STRONG: Demo stream byte buffer pointer. */
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
/* UNKNOWN: Declared pix_gerbe_item_t.x_pos storage; moteur only clears its enclosing storage or never accesses it. */
#define DOS_PIX_GERBE_ITEM_T_X_POS_OFFSET 0x0
#define DOS_PIX_GERBE_ITEM_T_X_POS_SIZE 2
/* UNKNOWN: Declared pix_gerbe_item_t.y_pos storage; moteur only clears its enclosing storage or never accesses it. */
#define DOS_PIX_GERBE_ITEM_T_Y_POS_OFFSET 0x2
#define DOS_PIX_GERBE_ITEM_T_Y_POS_SIZE 2
/* UNKNOWN: Declared pix_gerbe_item_t.speed_x storage; moteur only clears its enclosing storage or never accesses it. */
#define DOS_PIX_GERBE_ITEM_T_SPEED_X_OFFSET 0x4
#define DOS_PIX_GERBE_ITEM_T_SPEED_X_SIZE 2
/* UNKNOWN: Declared pix_gerbe_item_t.speed_y storage; moteur only clears its enclosing storage or never accesses it. */
#define DOS_PIX_GERBE_ITEM_T_SPEED_Y_OFFSET 0x6
#define DOS_PIX_GERBE_ITEM_T_SPEED_Y_SIZE 2
/* UNKNOWN: Declared pix_gerbe_item_t.y_accel storage; moteur only clears its enclosing storage or never accesses it. */
#define DOS_PIX_GERBE_ITEM_T_Y_ACCEL_OFFSET 0x8
#define DOS_PIX_GERBE_ITEM_T_Y_ACCEL_SIZE 1
/* UNKNOWN: Declared pix_gerbe_item_t.unk_1 storage; moteur only clears its enclosing storage or never accesses it. */
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
/* UNKNOWN: Declared flocon_t.field_0 storage; moteur only clears its enclosing storage or never accesses it. */
#define DOS_FLOCON_T_FIELD_0_OFFSET 0x0
#define DOS_FLOCON_T_FIELD_0_SIZE 2
/* UNKNOWN: Declared flocon_t.field_2 storage; moteur only clears its enclosing storage or never accesses it. */
#define DOS_FLOCON_T_FIELD_2_OFFSET 0x2
#define DOS_FLOCON_T_FIELD_2_SIZE 2
/* UNKNOWN: Declared flocon_t.field_4 storage; moteur only clears its enclosing storage or never accesses it. */
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
/* UNKNOWN: Unsigned16 option slot outside the Rayman/collision field-access scope. */
#define DOS_OPTIONS_JEU_T_MUSIC_ENABLED_OFFSET 0x18
#define DOS_OPTIONS_JEU_T_MUSIC_ENABLED_SIZE 2
/* UNKNOWN: Unsigned16 option slot outside the Rayman/collision field-access scope. */
#define DOS_OPTIONS_JEU_T_SOUND_VOLUME_OFFSET 0x1a
#define DOS_OPTIONS_JEU_T_SOUND_VOLUME_SIZE 2
/* UNKNOWN: Unsigned16 option slot outside the Rayman/collision field-access scope. */
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
#endif

