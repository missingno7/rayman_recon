# Semantic registry

Confidence describes the stated meaning, separately from STRICT matching.

| Registry / Entity | Level | Meaning | Units | DOS layout/address |
|---|---|---|---|---|
| basic.json / function:EOA | PROVEN | Report the terminal animation frame only on a zero phase of its selected animation clock. | see parameters/result contract | 1:0d074 |
| basic.json / local:EOA:on_last_frame:1 | PROVEN | Whether the current frame is the direction-dependent terminal frame. | boolean |  |
| basic.json / local:EOA:eta:1 | PROVEN | Current main/substate descriptor. | pointer |  |
| basic.json / local:EOA:anim:1 | PROVEN | Current object animation descriptor. | pointer |  |
| basic.json / local:EOA:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / function:save_objects_flags | PROVEN | Pack alive and is_active into two bits per level object. | see parameters/result contract | 1:0d0ec |
| basic.json / local:save_objects_flags:i:1 | PROVEN | Loop or append/removal slot in the object/list sequence used by this helper. | object/list index |  |
| basic.json / local:save_objects_flags:nb_objects:1 | PROVEN | Local snapshot of the current level object count. | objects |  |
| basic.json / local:save_objects_flags:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / function:restore_objects_flags | PROVEN | Restore packed alive and active bits for every level object except type 58. | see parameters/result contract | 1:0d15c |
| basic.json / local:restore_objects_flags:high_bit:1 | PROVEN | Decoded active flag bit. | boolean |  |
| basic.json / local:restore_objects_flags:low_bit:1 | PROVEN | Decoded alive flag bit. | boolean |  |
| basic.json / local:restore_objects_flags:i:1 | PROVEN | Loop or append/removal slot in the object/list sequence used by this helper. | object/list index |  |
| basic.json / local:restore_objects_flags:nb_objects:1 | PROVEN | Local snapshot of the current level object count. | objects |  |
| basic.json / local:restore_objects_flags:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / local:restore_objects_flags:word_index:1 | PROVEN | Snapshot word index i divided by16. | word index |  |
| basic.json / local:restore_objects_flags:bit_index:1 | PROVEN | Object slot within a two-bit snapshot word. | two-bit slot |  |
| basic.json / function:snapToSprite | PROVEN | Compute follower speeds towards a selected sprite of the other object at its predicted next animation frame. | see parameters/result contract | 1:0d204 |
| basic.json / local:snapToSprite:obj_2_frames_count:1 | PROVEN | Predicted animation frame count narrowed to a byte. | frames |  |
| basic.json / local:snapToSprite:obj_2_eta_2:1 | PROVEN | Predicted transition state descriptor. | pointer |  |
| basic.json / local:snapToSprite:prev_frame:1 | PROVEN | Saved actual target animation field restored after the predictive bounds query. | animation/frame index |  |
| basic.json / local:snapToSprite:prev_index:1 | PROVEN | Saved actual target animation field restored after the predictive bounds query. | animation/frame index |  |
| basic.json / local:snapToSprite:obj_2_x:1 | PROVEN | Predicted followed-sprite x coordinate. | pixels |  |
| basic.json / local:snapToSprite:obj_2_y:1 | PROVEN | Predicted followed-sprite y coordinate. | pixels |  |
| basic.json / local:snapToSprite:obj_2_w:1 | PROVEN | Predicted followed-sprite width output. | pixels |  |
| basic.json / local:snapToSprite:obj_2_h:1 | PROVEN | Predicted followed-sprite height output. | pixels |  |
| basic.json / local:snapToSprite:new_spd_x:1 | PROVEN | Signed16 residual x distance then converted follower x speed. | pixels then object speed units |  |
| basic.json / local:snapToSprite:new_spd_y:1 | PROVEN | Signed16 residual y distance then converted follower y speed. | pixels then object speed units |  |
| basic.json / local:snapToSprite:obj_2_eta_1:1 | PROVEN | Current state descriptor of the followed object. | pointer |  |
| basic.json / local:snapToSprite:obj_2_anim_index:1 | PROVEN | Predicted followed-object animation selector. | animation index |  |
| basic.json / local:snapToSprite:obj_2_anim_frame:1 | PROVEN | Predicted followed-object frame selector. | frame index |  |
| basic.json / local:snapToSprite:perm_value_7:1 | PROVEN | First frame of the predicted next animation, last frame when playing backwards. | frame index |  |
| basic.json / local:snapToSprite:first_flags:1 | PROVEN | Byte1 type flags snapshot for the follower; interpretation depends on the enclosing axis block. | bitset |  |
| basic.json / local:snapToSprite:second_flags:1 | PROVEN | Byte1 type flags snapshot for the followed object; interpretation depends on the enclosing axis block. | bitset |  |
| basic.json / local:snapToSprite:first_flags:2 | PROVEN | Byte1 type flags snapshot for the follower; interpretation depends on the enclosing axis block. | bitset |  |
| basic.json / local:snapToSprite:second_flags:2 | PROVEN | Byte1 type flags snapshot for the followed object; interpretation depends on the enclosing axis block. | bitset |  |
| basic.json / local:snapToSprite:obj_1:1 | PROVEN | Follower object whose position and speeds are updated. | pointer |  |
| basic.json / local:snapToSprite:obj_2:1 | PROVEN | Followed object whose predicted sprite position is sampled. | pointer |  |
| basic.json / local:snapToSprite:obj_2_spr:1 | PROVEN | Animation layer index to sample on the followed object. | layer index |  |
| basic.json / local:snapToSprite:x_offs:1 | PROVEN | Horizontal follower placement offset from the sampled sprite. | pixels |  |
| basic.json / local:snapToSprite:y_offs:1 | PROVEN | Unsigned16 vertical follower placement offset from the sampled sprite. | pixels |  |
| basic.json / function:findfirstObject | PROVEN | Return the first object of the requested type, or the pointer just past the level object array. | see parameters/result contract | 1:0d4c4 |
| basic.json / local:findfirstObject:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / local:findfirstObject:i:1 | PROVEN | Loop or append/removal slot in the object/list sequence used by this helper. | object/list index |  |
| basic.json / local:findfirstObject:nb_objs:1 | PROVEN | Local snapshot of the current level object count. | objects |  |
| basic.json / local:findfirstObject:obj_type:1 | PROVEN | Object type discriminator sought by the linear search. | object type id |  |
| basic.json / function:findfirstInactiveObject | PROVEN | Return the first inactive object of the requested type, or the pointer just past the level object array. | see parameters/result contract | 1:0d4f4 |
| basic.json / local:findfirstInactiveObject:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / local:findfirstInactiveObject:i:1 | PROVEN | Loop or append/removal slot in the object/list sequence used by this helper. | object/list index |  |
| basic.json / local:findfirstInactiveObject:nb_objs:1 | PROVEN | Local snapshot of the current level object count. | objects |  |
| basic.json / local:findfirstInactiveObject:obj_type:1 | PROVEN | Object type discriminator sought by the linear search. | object type id |  |
| basic.json / function:sinYspeed | PROVEN | Advance a 12-bit phase and return the sine target displacement minus current displacement from init_y. | see parameters/result contract | 1:0d528 |
| basic.json / local:sinYspeed:unk_1:1 | PROVEN | Wrapped updated sine phase. | 1/4096 cycle |  |
| basic.json / local:sinYspeed:diff_y:1 | PROVEN | Signed16 current y minus initial y. | pixels |  |
| basic.json / local:sinYspeed:unk_2:1 | PROVEN | Target vertical displacement from the sine product shifted by9. | pixels |  |
| basic.json / local:sinYspeed:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / local:sinYspeed:a2:1 | PROVEN | Advance added to the stored phase before wrapping to12 bits. | 1/4096 cycle |  |
| basic.json / local:sinYspeed:a3:1 | PROVEN | Signed sine amplitude multiplier. | pixels |  |
| basic.json / local:sinYspeed:a4:1 | PROVEN | Pointer to the mutable phase accumulator. | 1/4096 cycle |  |
| basic.json / function:set_proj_center | PROVEN | Store the signed-16 projection centre coordinates. | see parameters/result contract | 1:0d560 |
| basic.json / local:set_proj_center:x:1 | PROVEN | Coordinate value consumed by this helper computation. | pixels |  |
| basic.json / local:set_proj_center:y:1 | PROVEN | Coordinate value consumed by this helper computation. | pixels |  |
| basic.json / function:get_proj_dist | PROVEN | Return a dimension unchanged with zoom disabled, otherwise apply the integer projection factor. | see parameters/result contract | 1:0d570 |
| basic.json / local:get_proj_dist:temp:1 | PROVEN | Projected dimension product before division by256. | 1/256 pixel |  |
| basic.json / local:get_proj_dist:scale:1 | PROVEN | Signed projection depth added to256 in the scale denominator. | projection depth units |  |
| basic.json / local:get_proj_dist:outer_dim:1 | PROVEN | Signed dimension to optionally project. | pixels |  |
| basic.json / function:get_proj_dist2 | PROVEN | Scale a signed dimension by the integer projection factor without consulting zoom_mode. | see parameters/result contract | 1:0d5b0 |
| basic.json / local:get_proj_dist2:scale:1 | PROVEN | Signed projection depth added to256 in the scale denominator. | projection depth units |  |
| basic.json / local:get_proj_dist2:a2:1 | PROVEN | Signed dimension to project using arithmetic-shift rounding. | pixels |  |
| basic.json / function:get_proj_x | PROVEN | Project a signed-16 horizontal coordinate about PROJ_CENTER_X. | see parameters/result contract | 1:0d5d8 |
| basic.json / local:get_proj_x:temp:1 | PROVEN | Projection factor times x displacement from the centre. | 1/256 pixel |  |
| basic.json / local:get_proj_x:scale:1 | PROVEN | Signed projection depth added to256 in the scale denominator. | projection depth units |  |
| basic.json / local:get_proj_x:a2:1 | PROVEN | Horizontal coordinate to project. | pixels |  |
| basic.json / function:get_proj_y | PROVEN | Project a signed-16 vertical coordinate about PROJ_CENTER_Y. | see parameters/result contract | 1:0d614 |
| basic.json / local:get_proj_y:temp:1 | PROVEN | Projection factor times y displacement from the centre. | 1/256 pixel |  |
| basic.json / local:get_proj_y:scale:1 | PROVEN | Signed projection depth added to256 in the scale denominator. | projection depth units |  |
| basic.json / local:get_proj_y:a2:1 | PROVEN | Vertical coordinate to project. | pixels |  |
| basic.json / function:set_zoom_mode | PROVEN | Store the byte controlling the optional projection branch of get_proj_dist. | see parameters/result contract | 1:0d650 |
| basic.json / local:set_zoom_mode:mode:1 | PROVEN | Byte stored into zoom_mode. | zero/nonzero mode |  |
| basic.json / function:inverse_proj_x | PROVEN | Map a horizontal coordinate back through the quantised projection factor. | see parameters/result contract | 1:0d658 |
| basic.json / local:inverse_proj_x:divisor:1 | PROVEN | Integer projection factor65536/(scale+256). | 1/256 multiplier |  |
| basic.json / local:inverse_proj_x:scale:1 | PROVEN | Signed projection depth added to256 in the scale denominator. | projection depth units |  |
| basic.json / local:inverse_proj_x:a2:1 | PROVEN | Projected horizontal coordinate. | pixels |  |
| basic.json / function:inverse_proj_y | PROVEN | Map a vertical coordinate back through the quantised projection factor. | see parameters/result contract | 1:0d690 |
| basic.json / local:inverse_proj_y:divisor:1 | PROVEN | Integer projection factor65536/(scale+256). | 1/256 multiplier |  |
| basic.json / local:inverse_proj_y:scale:1 | PROVEN | Signed projection depth added to256 in the scale denominator. | projection depth units |  |
| basic.json / local:inverse_proj_y:a2:1 | PROVEN | Projected vertical coordinate. | pixels |  |
| basic.json / function:vblToEOA | PROVEN | Compute a positive, signed-16-truncated duration from the state animation period, frame count, clock phase and multiplier. | see parameters/result contract | 1:0d6c8 |
| basic.json / local:vblToEOA:eta:1 | PROVEN | State descriptor for the object current main/substate. | pointer |  |
| basic.json / local:vblToEOA:anim_speed:1 | PROVEN | Animation timing low nibble selecting a global phase counter. | clock index/nominal ticks |  |
| basic.json / local:vblToEOA:duration:1 | PROVEN | Signed16 product of timing nibble and state-selected frame count. | ticks |  |
| basic.json / local:vblToEOA:result:1 | PROVEN | Signed16 candidate duration after multiplier and phase adjustment. | ticks |  |
| basic.json / local:vblToEOA:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / local:vblToEOA:a2:1 | PROVEN | Multiplier applied to nominal full-animation duration. | dimensionless integer |  |
| basic.json / function:GET_ANIM_POS | PROVEN | Write the current animation-frame rectangle in world coordinates. | see parameters/result contract | 1:0d738 |
| basic.json / local:GET_ANIM_POS:frame:1 | PROVEN | Pointer to current animation frame bounding rectangle. | pointer |  |
| basic.json / local:GET_ANIM_POS:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / local:GET_ANIM_POS:x:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| basic.json / local:GET_ANIM_POS:y:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| basic.json / local:GET_ANIM_POS:w:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| basic.json / local:GET_ANIM_POS:h:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| basic.json / function:add_actobj | PROVEN | Append an object id to the active-object id array and write a following -1 sentinel. | see parameters/result contract | 1:0d7d8 |
| basic.json / local:add_actobj:index:1 | PROVEN | Loop or append/removal slot in the object/list sequence used by this helper. | object/list index |  |
| basic.json / local:add_actobj:obj_index:1 | PROVEN | Object id supplied to the object-list operation. | object id |  |
| basic.json / function:set_sub_etat | PROVEN | Set the object substate and request animation reset if the selected animation changes. | see parameters/result contract | 1:0d808 |
| basic.json / local:set_sub_etat:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / local:set_sub_etat:sub_etat:1 | PROVEN | New byte substate/main-state selector supplied to the setter. | state index |  |
| basic.json / function:set_main_etat | PROVEN | Set the object main state and request animation reset if the selected animation changes. | see parameters/result contract | 1:0d810 |
| basic.json / local:set_main_etat:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / local:set_main_etat:etat:1 | PROVEN | New byte substate/main-state selector supplied to the setter. | state index |  |
| basic.json / function:set_main_and_sub_etat | PROVEN | Set both state indices and request animation reset if the selected animation changes. | see parameters/result contract | 1:0d818 |
| basic.json / local:set_main_and_sub_etat:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / local:set_main_and_sub_etat:etat:1 | PROVEN | New byte substate/main-state selector supplied to the setter. | state index |  |
| basic.json / local:set_main_and_sub_etat:sub_etat:1 | PROVEN | New byte substate/main-state selector supplied to the setter. | state index |  |
| basic.json / function:get_center_x | PROVEN | Choose a type-dependent horizontal offset for the turn-marker block probe. | see parameters/result contract | 1:0d824 |
| basic.json / local:get_center_x:res:1 | PROVEN | Selected type-specific turn-probe offset returned by this helper. | pixels |  |
| basic.json / local:get_center_x:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / function:get_center_y | PROVEN | Choose a type-dependent vertical offset for the turn-marker block probe. | see parameters/result contract | 1:0d954 |
| basic.json / local:get_center_y:res:1 | PROVEN | Selected type-specific turn-probe offset returned by this helper. | pixels |  |
| basic.json / local:get_center_y:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / function:on_block_chdir | PROVEN | Test bit 5 of the block flags at the object position plus supplied probe offsets. | see parameters/result contract | 1:0dac8 |
| basic.json / local:on_block_chdir:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / local:on_block_chdir:center_x:1 | PROVEN | Object-relative coordinate offset of the block probe. | pixels |  |
| basic.json / local:on_block_chdir:center_y:1 | PROVEN | Object-relative coordinate offset of the block probe. | pixels |  |
| basic.json / function:CALC_FOLLOW_SPRITE_SPEED | PROVEN | Store current-minus-next selected animation-layer offsets into follow_x and follow_y. | see parameters/result contract | 1:0db04 |
| basic.json / local:CALC_FOLLOW_SPRITE_SPEED:unk_1:1 | PROVEN | Twice object x anchor offset minus current followed-sprite outer width. | pixels |  |
| basic.json / local:CALC_FOLLOW_SPRITE_SPEED:x_2:1 | PROVEN | Next followed-layer x offset after optional object reflection. | pixels |  |
| basic.json / local:CALC_FOLLOW_SPRITE_SPEED:x_1:1 | PROVEN | Current followed-layer x offset after optional object reflection. | pixels |  |
| basic.json / local:CALC_FOLLOW_SPRITE_SPEED:width_1:1 | PROVEN | Current followed sprite outer width used as the reflection offset. | pixels |  |
| basic.json / local:CALC_FOLLOW_SPRITE_SPEED:foll_spr:1 | PROVEN | Follow-sprite selector used as layer index or type-specific subtype in this helper. | index/subtype |  |
| basic.json / local:CALC_FOLLOW_SPRITE_SPEED:layer_1:1 | PROVEN | Current/next frame base in the frame-major layer table. | pointer |  |
| basic.json / local:CALC_FOLLOW_SPRITE_SPEED:layer_2:1 | PROVEN | Current/next frame base in the frame-major layer table. | pointer |  |
| basic.json / local:CALC_FOLLOW_SPRITE_SPEED:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / local:CALC_FOLLOW_SPRITE_SPEED:anim_1:1 | PROVEN | Current/next animation descriptor supplied for layer displacement calculation. | pointer |  |
| basic.json / local:CALC_FOLLOW_SPRITE_SPEED:anim_2:1 | PROVEN | Current/next animation descriptor supplied for layer displacement calculation. | pointer |  |
| basic.json / local:CALC_FOLLOW_SPRITE_SPEED:anim_frame_2:1 | PROVEN | Frame selector in the next supplied animation. | frame index |  |
| basic.json / function:GET_SPRITE_POS | PROVEN | Write the inner sprite rectangle for a layer of the current object animation when its sprite index and reference id are nonzero. | see parameters/result contract | 1:0dbb4 |
| basic.json / local:GET_SPRITE_POS:anim:1 | PROVEN | Current object animation descriptor. | pointer |  |
| basic.json / local:GET_SPRITE_POS:layer:1 | PROVEN | Selected layer in the current animation frame. | pointer |  |
| basic.json / local:GET_SPRITE_POS:sprite_ind:1 | PROVEN | Sprite-table index loaded from the selected layer. | sprite index |  |
| basic.json / local:GET_SPRITE_POS:sprite:1 | PROVEN | Descriptor for the layer-selected sprite. | pointer |  |
| basic.json / local:GET_SPRITE_POS:res:1 | PROVEN | Whether nonzero sprite index and reference id permit bounds output. | boolean |  |
| basic.json / local:GET_SPRITE_POS:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / local:GET_SPRITE_POS:index:1 | PROVEN | Layer index within the animation frame. | layer index |  |
| basic.json / local:GET_SPRITE_POS:x:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| basic.json / local:GET_SPRITE_POS:y:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| basic.json / local:GET_SPRITE_POS:w:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| basic.json / local:GET_SPRITE_POS:h:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| basic.json / function:GET_RAY_ZDC | PROVEN | Write a state-dependent Rayman collision rectangle and optionally project it around the object anchor. | see parameters/result contract | 1:0dcfc |
| basic.json / local:GET_RAY_ZDC:frame:1 | PROVEN | Current frame index reduced by32 once for the state7 rectangle test. | frame index |  |
| basic.json / local:GET_RAY_ZDC:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / local:GET_RAY_ZDC:x:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| basic.json / local:GET_RAY_ZDC:y:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| basic.json / local:GET_RAY_ZDC:w:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| basic.json / local:GET_RAY_ZDC:h:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| basic.json / function:GET_BB1_ZDCs | PROVEN | Derive two BB1 collision rectangles from selected sprite bounds and current animation height. | see parameters/result contract | 1:0dec0 |
| basic.json / local:GET_BB1_ZDCs:spr_ind:1 | PROVEN | Animation layer index selected for BB1 collision bounds. | layer index |  |
| basic.json / local:GET_BB1_ZDCs:h_loc:1 | PROVEN | Full current animation bounding height. | pixels |  |
| basic.json / local:GET_BB1_ZDCs:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / local:GET_BB1_ZDCs:x_1:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| basic.json / local:GET_BB1_ZDCs:y_1:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| basic.json / local:GET_BB1_ZDCs:w_1:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| basic.json / local:GET_BB1_ZDCs:h_1:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| basic.json / local:GET_BB1_ZDCs:x_2:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| basic.json / local:GET_BB1_ZDCs:y_2:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| basic.json / local:GET_BB1_ZDCs:w_2:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| basic.json / local:GET_BB1_ZDCs:h_2:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| basic.json / function:calc_obj_dir | PROVEN | Set object flip_x from the sign of Rayman anchor x minus object anchor x. | see parameters/result contract | 1:0dfb8 |
| basic.json / local:calc_obj_dir:dx:1 | PROVEN | Signed coordinate difference on the corresponding axis. | pixels |  |
| basic.json / local:calc_obj_dir:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / function:OBJ_IN_ZONE | PROVEN | Test the object detection-zone result with type-specific acceptance of values above one. | see parameters/result contract | 1:0dff4 |
| basic.json / local:OBJ_IN_ZONE:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / function:calc_obj_pos | PROVEN | Convert object world coordinates to stored screen coordinates using the camera offsets. | see parameters/result contract | 1:0e030 |
| basic.json / local:calc_obj_pos:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / function:makeUturn | PROVEN | Apply type- and state-specific facing/script changes when an object turns. | see parameters/result contract | 1:0e05c |
| basic.json / local:makeUturn:label:1 | PROVEN | Script jump label selected by turning logic, or255 for no label. | script label |  |
| basic.json / local:makeUturn:foll_spr:1 | UNKNOWN | Declared byte temporary with no uses in this reconstruction. | unknown |  |
| basic.json / local:makeUturn:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / function:BTYP | PROVEN | Return the collision-type byte of an in-bounds tile, or zero outside the map. | see parameters/result contract | 1:0e588 |
| basic.json / local:BTYP:x:1 | PROVEN | Promoted signed tile coordinate for the bounds check. | tiles |  |
| basic.json / local:BTYP:y:1 | PROVEN | Promoted signed tile coordinate for the bounds check. | tiles |  |
| basic.json / local:BTYP:tile_x:1 | PROVEN | Tile coordinate of the corresponding centre/side sample. | tiles |  |
| basic.json / local:BTYP:tile_y:1 | PROVEN | Tile coordinate of the corresponding centre/side sample. | tiles |  |
| basic.json / function:calc_btyp_square | PROVEN | Sample centre, left, right, above and below block types around an object anchor. | see parameters/result contract | 1:0e608 |
| basic.json / local:calc_btyp_square:x:1 | PROVEN | Signed16 object anchor coordinate used by collision sampling. | pixels |  |
| basic.json / local:calc_btyp_square:y:1 | PROVEN | Signed16 object anchor coordinate used by collision sampling. | pixels |  |
| basic.json / local:calc_btyp_square:tile_x_right:1 | PROVEN | Tile coordinate of the corresponding centre/side sample. | tiles |  |
| basic.json / local:calc_btyp_square:tile_x:1 | PROVEN | Tile coordinate of the corresponding centre/side sample. | tiles |  |
| basic.json / local:calc_btyp_square:tile_x_left:1 | PROVEN | Tile coordinate of the corresponding centre/side sample. | tiles |  |
| basic.json / local:calc_btyp_square:tile_y:1 | PROVEN | Tile coordinate of the corresponding centre/side sample. | tiles |  |
| basic.json / local:calc_btyp_square:x_offset:1 | PROVEN | Side sampling distance16,8 or4 according to object type and tiny mode. | pixels |  |
| basic.json / local:calc_btyp_square:center_btyp:1 | PROVEN | Centre collision type after optional airborne bloc_floor adjustment. | block type id |  |
| basic.json / local:calc_btyp_square:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / function:DO_OBJ_REBOND_EN_X | PROVEN | Negate horizontal speed when either side block has flag bit 4. | see parameters/result contract | 1:0e76c |
| basic.json / local:DO_OBJ_REBOND_EN_X:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / function:calc_btyp | PROVEN | Select the effective supporting block type and perform type-specific side-edge turning and Rayman edge bookkeeping. | see parameters/result contract | 1:0e790 |
| basic.json / local:calc_btyp:btyp:1 | PROVEN | Effective candidate collision type being selected. | block type id |  |
| basic.json / local:calc_btyp:btypes_1_2_solid:1 | PROVEN | Side-solid bit pair0..3, then chosen side with evolving0/1 encoding. | bitset then side selector |  |
| basic.json / local:calc_btyp:x:1 | PROVEN | Signed16 object anchor coordinate used by collision sampling. | pixels |  |
| basic.json / local:calc_btyp:y:1 | PROVEN | Signed16 object anchor coordinate used by collision sampling. | pixels |  |
| basic.json / local:calc_btyp:foll_spr:1 | PROVEN | Follow-sprite selector used as layer index or type-specific subtype in this helper. | index/subtype |  |
| basic.json / local:calc_btyp:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / function:init_obj_in_the_air | PROVEN | Clear the two byte gravity accumulators. | see parameters/result contract | 1:0eaa8 |
| basic.json / local:init_obj_in_the_air:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / function:make_my_fruit_go_down | PROVEN | Accelerate horizontal speed below magnitude32 and select the type-dependent falling state. | see parameters/result contract | 1:0eab4 |
| basic.json / local:make_my_fruit_go_down:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / local:make_my_fruit_go_down:x_accel:1 | PROVEN | Horizontal speed increment applied in the falling-state magnitude guard. | speed units/tick |  |
| basic.json / function:switchOff | PROVEN | On the timed animation-end event, clear object alive/active flags and remove its always-object entry with exceptions. | see parameters/result contract | 1:0eb18 |
| basic.json / local:switchOff:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / function:obj_hurt | PROVEN | Subtract current fist damage from target hit points with saturation at zero. | see parameters/result contract | 1:0eb80 |
| basic.json / local:obj_hurt:target:1 | PROVEN | Object whose hit points are reduced. | pointer |  |
| basic.json / function:Projectil_to_RM | PROVEN | Compute facing-constrained projectile speeds towards the stored follow_x/y target, then advance that stored target. | see parameters/result contract | 1:0eba0 |
| basic.json / local:Projectil_to_RM:diff_x:1 | PROVEN | Signed16 horizontal difference from object anchor to stored target plus Rayman x anchor offset. | pixels |  |
| basic.json / local:Projectil_to_RM:diff_y:1 | PROVEN | Signed16 vertical difference from object anchor to stored target plus Rayman y anchor offset. | pixels |  |
| basic.json / local:Projectil_to_RM:diff_x_abs:1 | PROVEN | Signed16 stored absolute horizontal difference. | pixels |  |
| basic.json / local:Projectil_to_RM:diff_y_abs:1 | PROVEN | Signed16 stored absolute vertical difference. | pixels |  |
| basic.json / local:Projectil_to_RM:unk_x:1 | PROVEN | Horizontal quotient/sign used to advance the stored projectile target by twice this value. | dimensionless integer |  |
| basic.json / local:Projectil_to_RM:unk_y:1 | PROVEN | Vertical quotient/sign used to advance the stored projectile target by twice this value. | dimensionless integer |  |
| basic.json / local:Projectil_to_RM:spd_x:1 | PROVEN | Wide temporary applying the special signed16-pixel horizontal speed adjustment. | output speed units |  |
| basic.json / local:Projectil_to_RM:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / local:Projectil_to_RM:out_spd_x:1 | PROVEN | Output pointer to computed projectile speed on the corresponding axis. | object speed units |  |
| basic.json / local:Projectil_to_RM:out_spd_y:1 | PROVEN | Output pointer to computed projectile speed on the corresponding axis. | object speed units |  |
| basic.json / local:Projectil_to_RM:new_spd_x:1 | PROVEN | Supplied horizontal speed magnitude before direction/proportional adjustment. | object speed units |  |
| basic.json / local:Projectil_to_RM:new_spd_y:1 | PROVEN | Supplied vertical speed magnitude before direction/proportional adjustment. | object speed units |  |
| basic.json / function:del_actobj | PROVEN | Clear object activity, find its id in actobj, shift entries using the original j-1 destination, and decrement the count. | see parameters/result contract | 1:0ee18 |
| basic.json / local:del_actobj:j:1 | PROVEN | Loop or append/removal slot in the object/list sequence used by this helper. | object/list index |  |
| basic.json / local:del_actobj:i:1 | PROVEN | Loop or append/removal slot in the object/list sequence used by this helper. | object/list index |  |
| basic.json / local:del_actobj:active:1 | PROVEN | Object selected from the current active-list id slot. | pointer |  |
| basic.json / local:del_actobj:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / function:calc_let_Width | PROVEN | Return the outer glyph width from the selected font or numeric sprite bank. | see parameters/result contract | 1:0eeb8 |
| basic.json / local:calc_let_Width:sprite:1 | PROVEN | Font glyph sprite descriptor chosen by bank and glyph number. | pointer |  |
| basic.json / local:calc_let_Width:font_size:1 | PROVEN | Font selector0/1/2 controlling bank and text metrics. | font selector |  |
| basic.json / local:calc_let_Width:num_let:1 | PROVEN | Decoded/adjusted glyph selector, with width determined by its declaration. | glyph index |  |
| basic.json / function:calc_let_Width2 | PROVEN | Return glyph inner width plus its low-nibble x inset. | see parameters/result contract | 1:0ef1c |
| basic.json / local:calc_let_Width2:sprite:1 | PROVEN | Font glyph sprite descriptor chosen by bank and glyph number. | pointer |  |
| basic.json / local:calc_let_Width2:font_size:1 | PROVEN | Font selector0/1/2 controlling bank and text metrics. | font selector |  |
| basic.json / local:calc_let_Width2:num_let:1 | PROVEN | Decoded/adjusted glyph selector, with width determined by its declaration. | glyph index |  |
| basic.json / function:INIT_TEXT_TO_DISPLAY | PROVEN | Clear text and two drawing-control bytes for display slots1 through10. | see parameters/result contract | 1:0ef80 |
| basic.json / local:INIT_TEXT_TO_DISPLAY:i:1 | PROVEN | Loop or append/removal slot in the object/list sequence used by this helper. | object/list index |  |
| basic.json / function:deter_num_let | PROVEN | Decode one text byte or a bracket-prefixed three-character numeric sprite escape into a glyph number. | see parameters/result contract | 1:0efb4 |
| basic.json / local:deter_num_let:result:1 | PROVEN | Decoded glyph number, zero for unrecognised bytes. | glyph index |  |
| basic.json / local:deter_num_let:next_text:1 | PROVEN | Three numeric escape bytes plus a zero terminator. | encoded bytes |  |
| basic.json / local:deter_num_let:c:1 | PROVEN | Current game-encoded text byte. | encoded byte |  |
| basic.json / local:deter_num_let:next_chars:1 | PROVEN | Pointer to bytes following the escape introducer. | pointer |  |
| basic.json / function:calc_num_let_spr | PROVEN | Add41 modulo256 to the pointed byte glyph number when font_size equals1. | see parameters/result contract | 1:0f194 |
| basic.json / local:calc_num_let_spr:font_size:1 | PROVEN | Font selector0/1/2 controlling bank and text metrics. | font selector |  |
| basic.json / local:calc_num_let_spr:letter_num:1 | PROVEN | Decoded/adjusted glyph selector, with width determined by its declaration. | glyph index |  |
| basic.json / function:calc_largmax_text | PROVEN | Accumulate glyph advances from the character after char_index until slash or NUL. | see parameters/result contract | 1:0f1b0 |
| basic.json / local:calc_largmax_text:width:1 | PROVEN | Accumulated line glyph advance. | pixels |  |
| basic.json / local:calc_largmax_text:i:1 | PROVEN | Text byte scan position. | byte index |  |
| basic.json / local:calc_largmax_text:num_let:1 | PROVEN | Decoded/adjusted glyph selector, with width determined by its declaration. | glyph index |  |
| basic.json / local:calc_largmax_text:text:1 | PROVEN | Encoded text bytes consumed or modified by this helper. | encoded bytes |  |
| basic.json / local:calc_largmax_text:char_index:1 | PROVEN | Starting text index; scan begins after this position. | byte index |  |
| basic.json / local:calc_largmax_text:space_width:1 | PROVEN | Advance for a literal space byte. | pixels |  |
| basic.json / local:calc_largmax_text:char_spacing:1 | PROVEN | Amount subtracted from each nonzero glyph advance. | pixels |  |
| basic.json / local:calc_largmax_text:font_size:1 | PROVEN | Font selector0/1/2 controlling bank and text metrics. | font selector |  |
| basic.json / function:INIT_TXT_BOX | PROVEN | Compute display-box rectangle from text glyph advances and the original slash-delimited line accounting. | see parameters/result contract | 1:0f21c |
| basic.json / local:INIT_TXT_BOX:i:1 | PROVEN | Text byte scan position. | byte index |  |
| basic.json / local:INIT_TXT_BOX:text:1 | PROVEN | Encoded text bytes consumed or modified by this helper. | encoded bytes |  |
| basic.json / local:INIT_TXT_BOX:max_width:1 | PROVEN | Largest width recorded at a slash, or full width on a slash-free line. | pixels |  |
| basic.json / local:INIT_TXT_BOX:num_let:1 | PROVEN | Decoded/adjusted glyph selector, with width determined by its declaration. | glyph index |  |
| basic.json / local:INIT_TXT_BOX:num_lines:1 | PROVEN | Slash-derived line count with original post-loop adjustment. | lines |  |
| basic.json / local:INIT_TXT_BOX:max_chars:1 | PROVEN | Maximum nonzero-glyph/space count recorded at slash boundaries; not used in final rectangle. | characters |  |
| basic.json / local:INIT_TXT_BOX:total_chars:1 | PROVEN | Current line glyph/space count; not used in final rectangle. | characters |  |
| basic.json / local:INIT_TXT_BOX:is_slash:1 | PROVEN | Whether any slash delimiter was observed. | boolean |  |
| basic.json / local:INIT_TXT_BOX:char_spacing:1 | PROVEN | Amount subtracted from each nonzero glyph advance. | pixels |  |
| basic.json / local:INIT_TXT_BOX:space_width:1 | PROVEN | Advance for a literal space byte. | pixels |  |
| basic.json / local:INIT_TXT_BOX:line_height:1 | PROVEN | Font-dependent line pitch36,23 or15. | pixels |  |
| basic.json / local:INIT_TXT_BOX:total_width:1 | PROVEN | Current slash-delimited line advance. | pixels |  |
| basic.json / local:INIT_TXT_BOX:font_size:1 | PROVEN | Font selector0/1/2 controlling bank and text metrics. | font selector |  |
| basic.json / local:INIT_TXT_BOX:box:1 | PROVEN | Display descriptor whose text and font determine computed rectangle fields. | pointer |  |
| basic.json / function:Deter_Option_Caract | PROVEN | Replace the selected text position with a..d for key0..3, optionally searching for the last dollar sign. | see parameters/result contract | 1:0f4a0 |
| basic.json / local:Deter_Option_Caract:i:1 | PROVEN | Text byte scan position. | byte index |  |
| basic.json / local:Deter_Option_Caract:text:1 | PROVEN | Encoded text bytes consumed or modified by this helper. | encoded bytes |  |
| basic.json / local:Deter_Option_Caract:key:1 | PROVEN | Selector0..3 mapping to characters a..d. | key selector |  |
| basic.json / local:Deter_Option_Caract:offset:1 | PROVEN | Replacement index or255 sentinel requesting a dollar-sign search. | byte index or sentinel |  |
| basic.json / function:SwapAB | PROVEN | Exchange the pointed signed-16 values. | see parameters/result contract | 1:0f50c |
| basic.json / local:SwapAB:temp:1 | PROVEN | Original value at the second pointed location. | signed16 value |  |
| basic.json / local:SwapAB:a:1 | PROVEN | Pointer to a signed16 value exchanged by SwapAB. | pointer |  |
| basic.json / local:SwapAB:b:1 | PROVEN | Pointer to a signed16 value exchanged by SwapAB. | pointer |  |
| basic.json / function:Bresenham | PROVEN | Visit spaced integer line points through a signed-16 callback using an endpoint fraction divided by128. | see parameters/result contract | 1:0f520 |
| basic.json / local:Bresenham:dx:1 | PROVEN | Signed coordinate difference on the corresponding axis. | pixels |  |
| basic.json / local:Bresenham:dy:1 | PROVEN | Signed coordinate difference on the corresponding axis. | pixels |  |
| basic.json / local:Bresenham:reversed:1 | PROVEN | Whether endpoint normalisation reversed traversal orientation. | boolean |  |
| basic.json / local:Bresenham:counter:1 | PROVEN | Major-axis selector initially, then callback cadence counter. | mode then line steps |  |
| basic.json / local:Bresenham:gap:1 | PROVEN | Local copy of callback gap threshold. | line steps |  |
| basic.json / local:Bresenham:x_straight:1 | PROVEN | Shallow-line error increment for a straight step. | scaled pixel error |  |
| basic.json / local:Bresenham:x_diagonal:1 | PROVEN | Shallow-line error increment for a diagonal step. | scaled pixel error |  |
| basic.json / local:Bresenham:y_straight:1 | PROVEN | Steep-line error increment for a straight step. | scaled pixel error |  |
| basic.json / local:Bresenham:y_diagonal:1 | PROVEN | Steep-line error increment for a diagonal step. | scaled pixel error |  |
| basic.json / local:Bresenham:step_x:1 | PROVEN | Signed minor-axis traversal direction selected from -1,0,1. | pixels/step |  |
| basic.json / local:Bresenham:step_y:1 | PROVEN | Signed minor-axis traversal direction selected from -1,0,1. | pixels/step |  |
| basic.json / local:Bresenham:error:1 | PROVEN | Bresenham signed error accumulator. | scaled pixel error |  |
| basic.json / local:Bresenham:x:1 | PROVEN | Current traversal coordinate on the corresponding axis. | pixels |  |
| basic.json / local:Bresenham:y:1 | PROVEN | Current traversal coordinate on the corresponding axis. | pixels |  |
| basic.json / local:Bresenham:end:1 | PROVEN | Fractional traversal terminal major-axis coordinate. | pixels |  |
| basic.json / local:Bresenham:func:1 | PROVEN | Callback receiving signed16 line point coordinates. | function pointer |  |
| basic.json / local:Bresenham:origin_x:1 | PROVEN | Line endpoint coordinate, potentially exchanged during direction normalisation. | pixels |  |
| basic.json / local:Bresenham:origin_y:1 | PROVEN | Line endpoint coordinate, potentially exchanged during direction normalisation. | pixels |  |
| basic.json / local:Bresenham:dest_x:1 | PROVEN | Line endpoint coordinate, potentially exchanged during direction normalisation. | pixels |  |
| basic.json / local:Bresenham:dest_y:1 | PROVEN | Line endpoint coordinate, potentially exchanged during direction normalisation. | pixels |  |
| basic.json / local:Bresenham:param_6:1 | PROVEN | Gap threshold used to decide callback cadence. | line steps |  |
| basic.json / local:Bresenham:percent:1 | PROVEN | Fraction of line extent with denominator128. | 1/128 full extent |  |
| basic.json / function:init_finBossLevel | PROVEN | Clear the twelve named progression bits in finBosslevel. | see parameters/result contract | 1:0f880 |
| basic.json / function:Change_Wait_Anim | PROVEN | Swap the state0/0 and state0/56 animation ids, copy the latter into state3/20, and toggle first_boss_meet. | see parameters/result contract | 1:0f8a4 |
| basic.json / local:Change_Wait_Anim:ray_eta:1 | PROVEN | Rayman main/substate table pointer used by the waiting-animation swap. | pointer |  |
| basic.json / local:Change_Wait_Anim:prev_00:1 | PROVEN | Previous animation id of state0/0 before the waiting-animation swap. | animation index |  |
| basic.json / function:add_alwobj | PROVEN | Append an object id to the level_obj array and increment its signed-16 count. | see parameters/result contract | 1:0f8e8 |
| basic.json / local:add_alwobj:index:1 | PROVEN | Loop or append/removal slot in the object/list sequence used by this helper. | object/list index |  |
| basic.json / local:add_alwobj:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / function:del_alwobj | PROVEN | Remove the first matching id from level_obj by replacing it with the last counted id. | see parameters/result contract | 1:0f918 |
| basic.json / local:del_alwobj:i:1 | PROVEN | Loop or append/removal slot in the object/list sequence used by this helper. | object/list index |  |
| basic.json / local:del_alwobj:obj_index:1 | PROVEN | Object id supplied to the object-list operation. | object id |  |
| basic.json / function:FatalError | PROVEN | Stop CD playback, enter text mode, print diagnostics, optionally wait for input, then exit with -1. | see parameters/result contract | 1:0f988 |
| basic.json / local:FatalError:message:1 | PROVEN | Error message string passed to FatalError diagnostic output. | text pointer |  |
| basic.json / function:sprite_of_ref | PROVEN | Return the first sprite-table index whose reference id matches, or zero if absent. | see parameters/result contract | 1:0fa00 |
| basic.json / local:sprite_of_ref:i:1 | PROVEN | Sprite-table scan index. | sprite index |  |
| basic.json / local:sprite_of_ref:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| basic.json / local:sprite_of_ref:ref:1 | PROVEN | Reference id to find in the sprite table. | sprite reference id |  |
| basic.json / global:saveobj | PROVEN | Packed snapshot of object alive/active flags. | two-bit entries | 2:3ce08 |
| basic.json / global:PROJ_CENTER_X | PROVEN | Global horizontal projection centre. | pixels | 2:3f838 |
| basic.json / global:PROJ_CENTER_Y | PROVEN | Global vertical projection centre. | pixels | 2:3f836 |
| basic.json / global:zoom_mode | PROVEN | Optional projection switch read by get_proj_dist. | zero/nonzero | 2:3fa63 |
| basic.json / global:alpha | PROVEN | Sprite font bank selected by font_size2. | font_t | 2:3e6e0 |
| basic.json / global:alpha2 | PROVEN | Sprite font bank selected by font_size0/1. | font_t | 2:3edc8 |
| basic.json / global:alpha_numbers | PROVEN | Sprite bank selected by glyph numbers>=1000. | font_t | 2:3e644 |
| basic.json / global:text_to_display | PROVEN | Array of eleven game text display descriptors. | display_item_t[11] | 2:3c4a0 |
| basic.json / global:errno | PROVEN | Watcom runtime error value obtained through __get_errno_ptr. | runtime error code | 2:574fc |
| basic.json / global:input_mode | PROVEN | Input-mode byte whose value1 suppresses FatalError key waiting. | mode | 2:3fa51 |
| collision1.json / function:get_nb_zdc | PROVEN | Extract the collision-zone count from the packed object reference. | see parameter/result and side-effect contract | 1:1b2d0 |
| collision1.json / local:get_nb_zdc:obj:1 | PROVEN | Object record processed by this function: Extract the collision-zone count from the packed object reference. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:get_zdc_index | PROVEN | Extract the collision-zone table start index from the packed object reference. | see parameter/result and side-effect contract | 1:1b2e0 |
| collision1.json / local:get_zdc_index:obj:1 | PROVEN | Object record processed by this function: Extract the collision-zone table start index from the packed object reference. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:get_zdc | PROVEN | Locate one zone relative to the packed object start index. | see parameter/result and side-effect contract | 1:1b2e8 |
| collision1.json / local:get_zdc:obj:1 | PROVEN | Object record processed by this function: Locate one zone relative to the packed object start index. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:get_zdc:a2:1 | PROVEN | Signed16 zone offset added to the packed start index. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:get_ZDCPTR | PROVEN | Return the current unsigned16 ZDCPTR slot. | see parameter/result and side-effect contract | 1:1b300 |
| collision1.json / function:in_coll_sprite_list | PROVEN | Test membership in the object's eligible sprite-collision layer list. | see parameter/result and side-effect contract | 1:1b308 |
| collision1.json / local:in_coll_sprite_list:unk_1:1 | PROVEN | Byte layer-id list terminated by0xff. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:in_coll_sprite_list:i:1 | PROVEN | Observed local dataflow: i = 0;; res = a2 == unk_1[i++];; while ((unk_1[i] != 0xFF) && (res != true)); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:in_coll_sprite_list:res:1 | PROVEN | Observed local dataflow: res = a2 == unk_1[i++];; while ((unk_1[i] != 0xFF) && (res != true));; return res; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:in_coll_sprite_list:obj:1 | PROVEN | Object record processed by this function: Test membership in the object's eligible sprite-collision layer list. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:in_coll_sprite_list:a2:1 | PROVEN | Requested collision layer checked against the selected list. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:box_inter_v_line | PROVEN | Test strict box-edge intersection with a vertical line segment. | see parameter/result and side-effect contract | 1:1b3b8 |
| collision1.json / local:box_inter_v_line:res:1 | PROVEN | Observed local dataflow: res = 1;; res = 1;; return res; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:box_inter_v_line:a1:1 | PROVEN | Box's left x coordinate. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:box_inter_v_line:a2:1 | PROVEN | Box's right x coordinate, already an endpoint rather than width. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:box_inter_v_line:a3:1 | PROVEN | Box's top y coordinate. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:box_inter_v_line:a4:1 | PROVEN | Box's bottom y coordinate. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:box_inter_v_line:a5:1 | PROVEN | Vertical segment x coordinate, strictly inside box x endpoints. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:box_inter_v_line:a6:1 | PROVEN | Lower numeric y endpoint of the tested open segment. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:box_inter_v_line:a7:1 | PROVEN | Upper numeric y endpoint of the tested open segment. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:box_inter_h_line | PROVEN | Test strict box-edge intersection with a horizontal line segment. | see parameter/result and side-effect contract | 1:1b3f4 |
| collision1.json / local:box_inter_h_line:res:1 | PROVEN | Observed local dataflow: res = 1;; res = 1;; return res; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:box_inter_h_line:a1:1 | PROVEN | Box's left x coordinate. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:box_inter_h_line:a2:1 | PROVEN | Box's right x coordinate. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:box_inter_h_line:a3:1 | PROVEN | Box's top y coordinate. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:box_inter_h_line:a4:1 | PROVEN | Box's bottom y coordinate. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:box_inter_h_line:a5:1 | PROVEN | Horizontal segment y coordinate, strictly inside box y endpoints. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:box_inter_h_line:a6:1 | PROVEN | Lower numeric x endpoint of the tested open segment. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:box_inter_h_line:a7:1 | PROVEN | Upper numeric x endpoint of the tested open segment. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:inter_box | PROVEN | Test closed rectangle overlap with signed16 intermediate narrowing. | see parameter/result and side-effect contract | 1:1b430 |
| collision1.json / local:inter_box:comp_x:1 | PROVEN | Observed local dataflow: comp_x = x_1 - w_2;; if ((((comp_x <= x_2) && (comp_y <= y_2)) && ((comp_x + sum_w) >= x_2)) && ((comp_y + sum_h) >= y_2)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:inter_box:comp_y:1 | PROVEN | Observed local dataflow: comp_y = y_1 - h_2;; if ((((comp_x <= x_2) && (comp_y <= y_2)) && ((comp_x + sum_w) >= x_2)) && ((comp_y + sum_h) >= y_2)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:inter_box:sum_w:1 | PROVEN | Observed local dataflow: sum_w = w_1 + w_2;; if ((((comp_x <= x_2) && (comp_y <= y_2)) && ((comp_x + sum_w) >= x_2)) && ((comp_y + sum_h) >= y_2)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:inter_box:sum_h:1 | PROVEN | Observed local dataflow: sum_h = perm_value_2 + h_2;; if ((((comp_x <= x_2) && (comp_y <= y_2)) && ((comp_x + sum_w) >= x_2)) && ((comp_y + sum_h) >= y_2)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:inter_box:perm_value_2:1 | PROVEN | Observed local dataflow: perm_value_2 = h_1;; sum_h = perm_value_2 + h_2; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:inter_box:x_1:1 | PROVEN | Observed parameter dataflow: comp_x = x_1 - w_2; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:inter_box:y_1:1 | PROVEN | Observed parameter dataflow: comp_y = y_1 - h_2; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:inter_box:w_1:1 | PROVEN | Observed parameter dataflow: sum_w = w_1 + w_2; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:inter_box:h_1:1 | PROVEN | Observed parameter dataflow: perm_value_2 = h_1; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:inter_box:x_2:1 | PROVEN | Observed parameter dataflow: if ((((comp_x <= x_2) && (comp_y <= y_2)) && ((comp_x + sum_w) >= x_2)) && ((comp_y + sum_h) >= y_2)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:inter_box:y_2:1 | PROVEN | Observed parameter dataflow: if ((((comp_x <= x_2) && (comp_y <= y_2)) && ((comp_x + sum_w) >= x_2)) && ((comp_y + sum_h) >= y_2)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:inter_box:w_2:1 | PROVEN | Observed parameter dataflow: comp_x = x_1 - w_2;; sum_w = w_1 + w_2; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:inter_box:h_2:1 | PROVEN | Observed parameter dataflow: comp_y = y_1 - h_2;; sum_h = perm_value_2 + h_2; | declared C storage; see exact computation and function contract |  |
| collision1.json / function:GET_OBJ_ZDC | PROVEN | Compute the type/state-specific object collision rectangle. | see parameter/result and side-effect contract | 1:1b48c |
| collision1.json / local:GET_OBJ_ZDC:anim_x:1 | PROVEN | Observed local dataflow: GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);; *out_x = anim_x + 13;; GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_OBJ_ZDC:anim_y:1 | PROVEN | Observed local dataflow: GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);; *out_y = anim_y + 13;; GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_OBJ_ZDC:anim_w:1 | PROVEN | Observed local dataflow: GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);; GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);; *out_w = anim_w - 20; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_OBJ_ZDC:anim_h:1 | PROVEN | Observed local dataflow: GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);; GET_ANIM_POS(obj, &anim_x, &anim_y, &anim_w, &anim_h);; *out_h = anim_h - 20; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_OBJ_ZDC:inset:1 | PROVEN | Observed local dataflow: inset = 24 - (12 * (get_eta(obj)->flags_bits.bit7 + (2 * get_eta(obj)->flags_bits.bit6)));; *out_x += inset;; *out_y += inset; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_OBJ_ZDC:old_flip_x:1 | PROVEN | Observed local dataflow: old_flip_x = obj->flags.flip_x;; obj->flags.flip_x = old_flip_x; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_OBJ_ZDC:bc_x_pos:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_OBJ_ZDC:new_x:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_OBJ_ZDC:unk_3:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_OBJ_ZDC:new_y:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_OBJ_ZDC:obj:1 | PROVEN | Object record processed by this function: Compute the type/state-specific object collision rectangle. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_OBJ_ZDC:out_x:1 | PROVEN | Observed parameter dataflow: GET_SPRITE_POS(obj, 1, out_x, out_y, out_w, out_h);; *out_x += 4;; *out_x += ((*out_w) >> 1) - 3; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_OBJ_ZDC:out_y:1 | PROVEN | Observed parameter dataflow: GET_SPRITE_POS(obj, 1, out_x, out_y, out_w, out_h);; *out_y += 8;; *out_y += 16; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_OBJ_ZDC:out_w:1 | PROVEN | Observed parameter dataflow: GET_SPRITE_POS(obj, 1, out_x, out_y, out_w, out_h);; if ((*out_w) > (*out_h)); *out_w -= 8; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_OBJ_ZDC:out_h:1 | PROVEN | Observed parameter dataflow: GET_SPRITE_POS(obj, 1, out_x, out_y, out_w, out_h);; if ((*out_w) > (*out_h)); *out_h -= 12; | declared C storage; see exact computation and function contract |  |
| collision1.json / function:GET_SPRITE_ZDC | PROVEN | Adjust a sprite-layer rectangle for collision according to object type and state. | see parameter/result and side-effect contract | 1:1c028 |
| collision1.json / local:GET_SPRITE_ZDC:unk_y:1 | PROVEN | Signed16 collision y correction added to the retrieved sprite y. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_SPRITE_ZDC:unk_x:1 | PROVEN | Signed16 collision x correction added to the retrieved sprite x. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_SPRITE_ZDC:unk_h:1 | PROVEN | Signed16 span correction added to the retrieved sprite height. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_SPRITE_ZDC:unk_w:1 | PROVEN | Signed16 span correction added to the retrieved sprite width. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_SPRITE_ZDC:spr_x:1 | PROVEN | Observed local dataflow: succ = GET_SPRITE_POS(obj, index, &spr_x, &spr_y, &spr_w, &spr_h);; spr_x = obj->x + 112;; *out_x = spr_x + unk_x; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_SPRITE_ZDC:type:1 | PROVEN | Observed local dataflow: type = obj->type;; switch (type) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_SPRITE_ZDC:spr_y:1 | PROVEN | Observed local dataflow: succ = GET_SPRITE_POS(obj, index, &spr_x, &spr_y, &spr_w, &spr_h);; spr_y = obj->y + 184;; *out_y = spr_y + unk_y; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_SPRITE_ZDC:spr_w:1 | PROVEN | Observed local dataflow: succ = GET_SPRITE_POS(obj, index, &spr_x, &spr_y, &spr_w, &spr_h);; spr_w = 34;; spr_w = 26; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_SPRITE_ZDC:spr_h:1 | PROVEN | Observed local dataflow: succ = GET_SPRITE_POS(obj, index, &spr_x, &spr_y, &spr_w, &spr_h);; spr_h = 32;; spr_h = 24; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_SPRITE_ZDC:succ:1 | PROVEN | Observed local dataflow: succ = GET_SPRITE_POS(obj, index, &spr_x, &spr_y, &spr_w, &spr_h);; if (succ); succ = false; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_SPRITE_ZDC:temp_h:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_SPRITE_ZDC:obj:1 | PROVEN | Object record processed by this function: Adjust a sprite-layer rectangle for collision according to object type and state. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_SPRITE_ZDC:index:1 | PROVEN | Observed parameter dataflow: succ = GET_SPRITE_POS(obj, index, &spr_x, &spr_y, &spr_w, &spr_h);; switch (index); switch (index) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_SPRITE_ZDC:out_x:1 | PROVEN | Observed parameter dataflow: *out_x = spr_x + unk_x; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_SPRITE_ZDC:out_y:1 | PROVEN | Observed parameter dataflow: *out_y = spr_y + unk_y; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_SPRITE_ZDC:out_w:1 | PROVEN | Observed parameter dataflow: *out_w = spr_w + unk_w; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:GET_SPRITE_ZDC:out_h:1 | PROVEN | Observed parameter dataflow: *out_h = spr_h + unk_h; | declared C storage; see exact computation and function contract |  |
| collision1.json / function:BOX_HIT_SPECIAL_ZDC | PROVEN | Test hardcoded bag and BB1-family collision shapes. | see parameter/result and side-effect contract | 1:1c33c |
| collision1.json / local:BOX_HIT_SPECIAL_ZDC:frame:1 | PROVEN | Observed local dataflow: frame = obj->anim_frame;; d = bagDy[frame];; if (inter_box(in_x, in_y, in_w, in_h, (obj->x + obj->offset_bx) - (bagW[frame] >> 1), ((obj->y + obj->offset_by) + d) - bagH[frame], bagW[frame], bagH[frame])) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_HIT_SPECIAL_ZDC:d:1 | PROVEN | Per-frame bagDy value; -1 suppresses the frame's collision rectangle. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_HIT_SPECIAL_ZDC:bb1_x_2:1 | PROVEN | Observed local dataflow: GET_BB1_ZDCs(obj, &bb1_x_1, &bb1_y_1, &bb1_w_1, &bb1_h_1, &bb1_x_2, &bb1_y_2, &bb1_w_2, &bb1_h_2);; if (inter_box(in_x, in_y, in_w, in_h, bb1_x_2, bb1_y_2, bb1_w_2, bb1_h_2)); GET_BB1_ZDCs(obj, &bb1_x_1, &bb1_y_1, &bb1_w_1, &bb1_h_1, &bb1_x_2, &bb1_y_2, &bb1_w_2, &bb1_h_2); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_HIT_SPECIAL_ZDC:bb1_y_2:1 | PROVEN | Observed local dataflow: GET_BB1_ZDCs(obj, &bb1_x_1, &bb1_y_1, &bb1_w_1, &bb1_h_1, &bb1_x_2, &bb1_y_2, &bb1_w_2, &bb1_h_2);; if (inter_box(in_x, in_y, in_w, in_h, bb1_x_2, bb1_y_2, bb1_w_2, bb1_h_2)); GET_BB1_ZDCs(obj, &bb1_x_1, &bb1_y_1, &bb1_w_1, &bb1_h_1, &bb1_x_2, &bb1_y_2, &bb1_w_2, &bb1_h_2); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_HIT_SPECIAL_ZDC:bb1_w_2:1 | PROVEN | Observed local dataflow: GET_BB1_ZDCs(obj, &bb1_x_1, &bb1_y_1, &bb1_w_1, &bb1_h_1, &bb1_x_2, &bb1_y_2, &bb1_w_2, &bb1_h_2);; if (inter_box(in_x, in_y, in_w, in_h, bb1_x_2, bb1_y_2, bb1_w_2, bb1_h_2)); GET_BB1_ZDCs(obj, &bb1_x_1, &bb1_y_1, &bb1_w_1, &bb1_h_1, &bb1_x_2, &bb1_y_2, &bb1_w_2, &bb1_h_2); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_HIT_SPECIAL_ZDC:bb1_h_2:1 | PROVEN | Observed local dataflow: GET_BB1_ZDCs(obj, &bb1_x_1, &bb1_y_1, &bb1_w_1, &bb1_h_1, &bb1_x_2, &bb1_y_2, &bb1_w_2, &bb1_h_2);; if (inter_box(in_x, in_y, in_w, in_h, bb1_x_2, bb1_y_2, bb1_w_2, bb1_h_2)); GET_BB1_ZDCs(obj, &bb1_x_1, &bb1_y_1, &bb1_w_1, &bb1_h_1, &bb1_x_2, &bb1_y_2, &bb1_w_2, &bb1_h_2); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_HIT_SPECIAL_ZDC:bb1_x_1:1 | PROVEN | Observed local dataflow: GET_BB1_ZDCs(obj, &bb1_x_1, &bb1_y_1, &bb1_w_1, &bb1_h_1, &bb1_x_2, &bb1_y_2, &bb1_w_2, &bb1_h_2);; if (inter_box(in_x, in_y, in_w, in_h, bb1_x_1, bb1_y_1, bb1_w_1, bb1_h_1)); GET_BB1_ZDCs(obj, &bb1_x_1, &bb1_y_1, &bb1_w_1, &bb1_h_1, &bb1_x_2, &bb1_y_2, &bb1_w_2, &bb1_h_2); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_HIT_SPECIAL_ZDC:bb1_y_1:1 | PROVEN | Observed local dataflow: GET_BB1_ZDCs(obj, &bb1_x_1, &bb1_y_1, &bb1_w_1, &bb1_h_1, &bb1_x_2, &bb1_y_2, &bb1_w_2, &bb1_h_2);; if (inter_box(in_x, in_y, in_w, in_h, bb1_x_1, bb1_y_1, bb1_w_1, bb1_h_1)); GET_BB1_ZDCs(obj, &bb1_x_1, &bb1_y_1, &bb1_w_1, &bb1_h_1, &bb1_x_2, &bb1_y_2, &bb1_w_2, &bb1_h_2); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_HIT_SPECIAL_ZDC:bb1_h_1:1 | PROVEN | Observed local dataflow: GET_BB1_ZDCs(obj, &bb1_x_1, &bb1_y_1, &bb1_w_1, &bb1_h_1, &bb1_x_2, &bb1_y_2, &bb1_w_2, &bb1_h_2);; if (inter_box(in_x, in_y, in_w, in_h, bb1_x_1, bb1_y_1, bb1_w_1, bb1_h_1)); GET_BB1_ZDCs(obj, &bb1_x_1, &bb1_y_1, &bb1_w_1, &bb1_h_1, &bb1_x_2, &bb1_y_2, &bb1_w_2, &bb1_h_2); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_HIT_SPECIAL_ZDC:bb1_w_1:1 | PROVEN | Observed local dataflow: GET_BB1_ZDCs(obj, &bb1_x_1, &bb1_y_1, &bb1_w_1, &bb1_h_1, &bb1_x_2, &bb1_y_2, &bb1_w_2, &bb1_h_2);; if (inter_box(in_x, in_y, in_w, in_h, bb1_x_1, bb1_y_1, bb1_w_1, bb1_h_1)); GET_BB1_ZDCs(obj, &bb1_x_1, &bb1_y_1, &bb1_w_1, &bb1_h_1, &bb1_x_2, &bb1_y_2, &bb1_w_2, &bb1_h_2); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_HIT_SPECIAL_ZDC:res:1 | PROVEN | Observed local dataflow: res = 1;; res = 1;; res = 1; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_HIT_SPECIAL_ZDC:in_x:1 | PROVEN | Observed parameter dataflow: if (inter_box(in_x, in_y, in_w, in_h, (obj->x + obj->offset_bx) - (bagW[frame] >> 1), ((obj->y + obj->offset_by) + d) - bagH[frame], bagW[frame], bagH[frame])); if (inter_box(in_x, in_y, in_w, in_h, bb1_x_1, bb1_y_1, bb1_w_1, bb1_h_1)); if (inter_box(in_x, in_y, in_w, in_h, bb1_x_2, bb1_y_2, bb1_w_2, bb1_h_2)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_HIT_SPECIAL_ZDC:in_y:1 | PROVEN | Observed parameter dataflow: if (inter_box(in_x, in_y, in_w, in_h, (obj->x + obj->offset_bx) - (bagW[frame] >> 1), ((obj->y + obj->offset_by) + d) - bagH[frame], bagW[frame], bagH[frame])); if (inter_box(in_x, in_y, in_w, in_h, bb1_x_1, bb1_y_1, bb1_w_1, bb1_h_1)); if (inter_box(in_x, in_y, in_w, in_h, bb1_x_2, bb1_y_2, bb1_w_2, bb1_h_2)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_HIT_SPECIAL_ZDC:in_w:1 | PROVEN | Observed parameter dataflow: if (inter_box(in_x, in_y, in_w, in_h, (obj->x + obj->offset_bx) - (bagW[frame] >> 1), ((obj->y + obj->offset_by) + d) - bagH[frame], bagW[frame], bagH[frame])); if (inter_box(in_x, in_y, in_w, in_h, bb1_x_1, bb1_y_1, bb1_w_1, bb1_h_1)); if (inter_box(in_x, in_y, in_w, in_h, bb1_x_2, bb1_y_2, bb1_w_2, bb1_h_2)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_HIT_SPECIAL_ZDC:in_h:1 | PROVEN | Observed parameter dataflow: if (inter_box(in_x, in_y, in_w, in_h, (obj->x + obj->offset_bx) - (bagW[frame] >> 1), ((obj->y + obj->offset_by) + d) - bagH[frame], bagW[frame], bagH[frame])); if (in_h == ray_zdc_y); if (inter_box(in_x, in_y, in_w, in_h, bb1_x_1, bb1_y_1, bb1_w_1, bb1_h_1)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_HIT_SPECIAL_ZDC:obj:1 | PROVEN | Object record processed by this function: Test hardcoded bag and BB1-family collision shapes. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:BOX_IN_COLL_ZONES | STRONG | Test a box against packed zones or sentinel-selected fallback shapes. | see parameter/result and side-effect contract | 1:1c670 |
| collision1.json / local:BOX_IN_COLL_ZONES:res:1 | STRONG | Observed local dataflow: res = -1;; res = cur_zdc->sprite;; res = inter_box(x, y, w, h, unk_x, unk_y, unk_w, unk_h); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_IN_COLL_ZONES:unk_x:1 | STRONG | Current collision rectangle world x, reused across zone iterations. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_IN_COLL_ZONES:unk_y:1 | STRONG | Current collision rectangle world y, reused across zone iterations. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_IN_COLL_ZONES:unk_w:1 | STRONG | Current collision rectangle width span, reused across zone iterations. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_IN_COLL_ZONES:unk_h:1 | STRONG | Current collision rectangle height span, reused across zone iterations. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_IN_COLL_ZONES:nb_zdc:1 | STRONG | Observed local dataflow: nb_zdc = get_nb_zdc(obj);; nb_zdc--;; for (i = 0; i < nb_zdc; i++) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_IN_COLL_ZONES:i:1 | STRONG | Observed local dataflow: for (i = 0; i < nb_zdc; i++); cur_zdc = get_zdc(obj, i); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_IN_COLL_ZONES:sprite_index:1 | STRONG | Observed local dataflow: if (cur_layer[cur_zdc->sprite].sprite_index != 0); for (sprite_index = 0; sprite_index < layers_count; sprite_index++); if ((in_coll_sprite_list(obj, sprite_index) && GET_SPRITE_ZDC(obj, sprite_index, &unk_x, &unk_y, &unk_w, &unk_h)) && inter_box(x, y, w, h, unk_x, unk_y, unk_w, unk_h)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_IN_COLL_ZONES:cur_zdc:1 | STRONG | Observed local dataflow: cur_zdc = get_zdc(obj, i);; if ((!(cur_zdc->flags & 4)) \|\| (type == TYPE_POING)); if (cur_zdc->flags & 2) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_IN_COLL_ZONES:cur_anim:1 | STRONG | Observed local dataflow: cur_anim = &obj->animations[obj->anim_index];; cur_layer = &cur_anim->layers[obj->anim_frame * (cur_anim->layers_per_frame & 0x3FFF)]; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_IN_COLL_ZONES:cur_layer:1 | STRONG | Observed local dataflow: anim_layer_t *cur_layer;; cur_layer = &cur_anim->layers[obj->anim_frame * (cur_anim->layers_per_frame & 0x3FFF)];; if (cur_layer[cur_zdc->sprite].sprite_index != 0) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_IN_COLL_ZONES:layers_count:1 | STRONG | Observed local dataflow: layers_count = obj->animations[obj->anim_index].layers_per_frame & 0x3FFF;; for (sprite_index = 0; sprite_index < layers_count; sprite_index++) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_IN_COLL_ZONES:type:1 | STRONG | Observed parameter dataflow: if ((num_world == 1) && ((((obj->type == TYPE_MOSKITO) \|\| (obj->type == TYPE_MOSKITO2)) && (!(obj->eta[obj->main_etat][obj->sub_etat].flags & 0x40))) \|\| ((obj->type == TYPE_BADGUY3) && (obj->eta[obj->main_etat][obj->sub_etat].flags & 0x40)))); if ((!(cur_zdc->flags & 4)) \|\| (type == TYPE_POING)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_IN_COLL_ZONES:x:1 | STRONG | Observed parameter dataflow: unk_x = obj->x + cur_zdc->x_pos;; unk_x = ((obj->x + obj->offset_bx) + (obj->x + obj->offset_bx)) - (unk_x + unk_w);; if (inter_box(x, y, w, h, unk_x, unk_y, unk_w, unk_h)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_IN_COLL_ZONES:y:1 | STRONG | Observed parameter dataflow: unk_y = obj->y + cur_zdc->y_pos;; if (inter_box(x, y, w, h, unk_x, unk_y, unk_w, unk_h)); res = inter_box(x, y, w, h, unk_x, unk_y, unk_w, unk_h); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_IN_COLL_ZONES:w:1 | STRONG | Observed parameter dataflow: if (inter_box(x, y, w, h, unk_x, unk_y, unk_w, unk_h)); res = inter_box(x, y, w, h, unk_x, unk_y, unk_w, unk_h);; if ((in_coll_sprite_list(obj, sprite_index) && GET_SPRITE_ZDC(obj, sprite_index, &unk_x, &unk_y, &unk_w, &unk_h)) && inter_box(x, y, w, h, unk_x, unk_y, unk_w, unk_h)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_IN_COLL_ZONES:h:1 | STRONG | Observed parameter dataflow: if (inter_box(x, y, w, h, unk_x, unk_y, unk_w, unk_h)); res = inter_box(x, y, w, h, unk_x, unk_y, unk_w, unk_h);; if ((in_coll_sprite_list(obj, sprite_index) && GET_SPRITE_ZDC(obj, sprite_index, &unk_x, &unk_y, &unk_w, &unk_h)) && inter_box(x, y, w, h, unk_x, unk_y, unk_w, unk_h)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:BOX_IN_COLL_ZONES:obj:1 | STRONG | Object record processed by this function: Test a box against packed zones or sentinel-selected fallback shapes. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:COLL_BOX_SPRITE | PROVEN | Test a box against the single layer named by hit_sprite. | see parameter/result and side-effect contract | 1:1ca84 |
| collision1.json / local:COLL_BOX_SPRITE:spr_x:1 | PROVEN | Observed local dataflow: succ = GET_SPRITE_POS(obj, obj->hit_sprite, &spr_x, &spr_y, &spr_w, &spr_h);; succ = inter_box(x, y, w, h, spr_x, spr_y, spr_w, spr_h); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_BOX_SPRITE:spr_y:1 | PROVEN | Observed local dataflow: succ = GET_SPRITE_POS(obj, obj->hit_sprite, &spr_x, &spr_y, &spr_w, &spr_h);; succ = inter_box(x, y, w, h, spr_x, spr_y, spr_w, spr_h); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_BOX_SPRITE:spr_w:1 | PROVEN | Observed local dataflow: succ = GET_SPRITE_POS(obj, obj->hit_sprite, &spr_x, &spr_y, &spr_w, &spr_h);; succ = inter_box(x, y, w, h, spr_x, spr_y, spr_w, spr_h); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_BOX_SPRITE:spr_h:1 | PROVEN | Observed local dataflow: succ = GET_SPRITE_POS(obj, obj->hit_sprite, &spr_x, &spr_y, &spr_w, &spr_h);; succ = inter_box(x, y, w, h, spr_x, spr_y, spr_w, spr_h); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_BOX_SPRITE:succ:1 | PROVEN | Observed local dataflow: succ = GET_SPRITE_POS(obj, obj->hit_sprite, &spr_x, &spr_y, &spr_w, &spr_h);; if (succ); succ = inter_box(x, y, w, h, spr_x, spr_y, spr_w, spr_h); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_BOX_SPRITE:x:1 | PROVEN | Observed parameter dataflow: succ = inter_box(x, y, w, h, spr_x, spr_y, spr_w, spr_h); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_BOX_SPRITE:y:1 | PROVEN | Observed parameter dataflow: succ = inter_box(x, y, w, h, spr_x, spr_y, spr_w, spr_h); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_BOX_SPRITE:w:1 | PROVEN | Observed parameter dataflow: succ = inter_box(x, y, w, h, spr_x, spr_y, spr_w, spr_h); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_BOX_SPRITE:h:1 | PROVEN | Observed parameter dataflow: succ = inter_box(x, y, w, h, spr_x, spr_y, spr_w, spr_h); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_BOX_SPRITE:obj:1 | PROVEN | Object record processed by this function: Test a box against the single layer named by hit_sprite. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:CHECK_BOX_COLLISION | PROVEN | Dispatch box collision by the object's collision-sprite selector. | see parameter/result and side-effect contract | 1:1cb1c |
| collision1.json / local:CHECK_BOX_COLLISION:hit_sprite:1 | PROVEN | Observed local dataflow: switch (hit_sprite) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:CHECK_BOX_COLLISION:type:1 | PROVEN | Observed parameter dataflow: return BOX_IN_COLL_ZONES(type, x, y, w, h, obj); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:CHECK_BOX_COLLISION:x:1 | PROVEN | Observed parameter dataflow: return BOX_HIT_SPECIAL_ZDC(x, y, w, h, obj);; return BOX_IN_COLL_ZONES(type, x, y, w, h, obj);; return COLL_BOX_SPRITE(x, y, w, h, obj); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:CHECK_BOX_COLLISION:y:1 | PROVEN | Observed parameter dataflow: return BOX_HIT_SPECIAL_ZDC(x, y, w, h, obj);; return BOX_IN_COLL_ZONES(type, x, y, w, h, obj);; return COLL_BOX_SPRITE(x, y, w, h, obj); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:CHECK_BOX_COLLISION:w:1 | PROVEN | Observed parameter dataflow: return BOX_HIT_SPECIAL_ZDC(x, y, w, h, obj);; return BOX_IN_COLL_ZONES(type, x, y, w, h, obj);; return COLL_BOX_SPRITE(x, y, w, h, obj); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:CHECK_BOX_COLLISION:h:1 | PROVEN | Observed parameter dataflow: return BOX_HIT_SPECIAL_ZDC(x, y, w, h, obj);; return BOX_IN_COLL_ZONES(type, x, y, w, h, obj);; return COLL_BOX_SPRITE(x, y, w, h, obj); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:CHECK_BOX_COLLISION:obj:1 | PROVEN | Object record processed by this function: Dispatch box collision by the object's collision-sprite selector. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:possible_sprite | PROVEN | Return one element of the type-specific support-layer list. | see parameter/result and side-effect contract | 1:1cb9c |
| collision1.json / local:possible_sprite:spr:1 | PROVEN | Observed local dataflow: spr[0] = 0;; spr[1] = 1;; spr[2] = 2; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:possible_sprite:obj:1 | PROVEN | Object record processed by this function: Return one element of the type-specific support-layer list. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:possible_sprite:index:1 | PROVEN | Observed parameter dataflow: return spr[index]; | declared C storage; see exact computation and function contract |  |
| collision1.json / function:setToleranceDist | STRONG | Encode vertical distance and horizontal support tolerance. | see parameter/result and side-effect contract | 1:1ce34 |
| collision1.json / local:setToleranceDist:ray_x:1 | STRONG | Observed local dataflow: if (inrange(ray_x, x - unk_2, unk_1 + unk_2)); if (inrange(ray_x, x, unk_1)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:setToleranceDist:ray_y:1 | STRONG | Observed local dataflow: return ((ray_y - y) < 0) ? (-(ray_y - y)) : (ray_y - y);; return (-(((ray_y - y) < 0) ? (-(ray_y - y)) : (ray_y - y))) - 1; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:setToleranceDist:unk_1:1 | PROVEN | Inclusive last support pixel x+w-1. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:setToleranceDist:unk_2:1 | PROVEN | Additional horizontal tolerance8 normal,4 tiny. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:setToleranceDist:x:1 | STRONG | Observed parameter dataflow: if (inrange(ray_x, x - unk_2, unk_1 + unk_2)); if (inrange(ray_x, x, unk_1)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:setToleranceDist:w:1 | STRONG | Observed parameter dataflow: s16 unk_1 = (x + w) - 1; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:setToleranceDist:y:1 | STRONG | Observed parameter dataflow: return ((ray_y - y) < 0) ? (-(ray_y - y)) : (ray_y - y);; return (-(((ray_y - y) < 0) ? (-(ray_y - y)) : (ray_y - y))) - 1; | declared C storage; see exact computation and function contract |  |
| collision1.json / function:SET_RAY_DIST_SLOPEY_PLAT | STRONG | Compute support distance for a slope platform using signed hit_points as slope. | see parameter/result and side-effect contract | 1:1cf14 |
| collision1.json / local:SET_RAY_DIST_SLOPEY_PLAT:limit:1 | STRONG | Observed local dataflow: limit = 8;; limit = 4;; if (inrange(ray_x, spr_x - limit, slope_x2 + limit)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_SLOPEY_PLAT:angle:1 | STRONG | Observed local dataflow: angle = (s8) obj->hit_points;; if (angle < 0); angle = -angle; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_SLOPEY_PLAT:spr_x:1 | STRONG | Observed local dataflow: GET_SPRITE_POS(obj, obj->follow_sprite, &spr_x, &spr_y, &spr_w, &spr_h);; spr_x += (spr_w - projected_w) >> 1;; slope_x2 = (spr_x + spr_w) - 1; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_SLOPEY_PLAT:spr_y:1 | STRONG | Observed local dataflow: GET_SPRITE_POS(obj, obj->follow_sprite, &spr_x, &spr_y, &spr_w, &spr_h);; spr_y += obj->offset_hy;; diff_y = spr_y - ray_y; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_SLOPEY_PLAT:spr_w:1 | STRONG | Observed local dataflow: GET_SPRITE_POS(obj, obj->follow_sprite, &spr_x, &spr_y, &spr_w, &spr_h);; projected_w = (((s32) cosinus(pente[angle])) * spr_w) >> 9;; spr_x += (spr_w - projected_w) >> 1; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_SLOPEY_PLAT:spr_h:1 | STRONG | Observed local dataflow: GET_SPRITE_POS(obj, obj->follow_sprite, &spr_x, &spr_y, &spr_w, &spr_h);; spr_h = diff_x * ((s8) obj->hit_points);; spr_h = ashr16(spr_h, 4); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_SLOPEY_PLAT:ray_x:1 | STRONG | Observed local dataflow: ray_x = ray.x + ray.offset_bx;; if (inrange(ray_x, spr_x - limit, slope_x2 + limit)); diff_x = ray_x - (spr_x + (spr_w >> 1)); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_SLOPEY_PLAT:ray_y:1 | STRONG | Observed local dataflow: ray_y = ray.y + ray.offset_by;; diff_y = spr_y - ray_y; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_SLOPEY_PLAT:projected_w:1 | STRONG | Observed local dataflow: projected_w = (((s32) cosinus(pente[angle])) * spr_w) >> 9;; spr_x += (spr_w - projected_w) >> 1;; spr_w = projected_w; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_SLOPEY_PLAT:slope_x2:1 | STRONG | Observed local dataflow: slope_x2 = (spr_x + spr_w) - 1;; if (inrange(ray_x, spr_x - limit, slope_x2 + limit)); if (inrange(ray_x, spr_x, slope_x2)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_SLOPEY_PLAT:diff_y:1 | STRONG | Observed local dataflow: diff_y = spr_y - ray_y;; if (((((s32) diff_y) < 0) ? (-((s32) diff_y)) : ((s32) diff_y)) < spr_w); obj->ray_dist = ((diff_y - spr_h) < 0) ? (-(diff_y - spr_h)) : (diff_y - spr_h); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_SLOPEY_PLAT:diff_x:1 | STRONG | Observed local dataflow: diff_x = ray_x - (spr_x + (spr_w >> 1));; spr_h = diff_x * ((s8) obj->hit_points); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_SLOPEY_PLAT:obj:1 | STRONG | Object record processed by this function: Compute support distance for a slope platform using signed hit_points as slope. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:SET_RAY_DIST_MULTISPR_CANTCHANGE | PROVEN | Select a candidate support layer and store its Rayman distance. | see parameter/result and side-effect contract | 1:1d0d4 |
| collision1.json / local:SET_RAY_DIST_MULTISPR_CANTCHANGE:unk_1:1 | PROVEN | Additional support x tolerance8 normal,4 tiny. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_MULTISPR_CANTCHANGE:ray_x:1 | PROVEN | Observed local dataflow: ray_x = ray.x + ray.offset_bx;; ray_x = ray.x + ray.offset_bx;; if ((ray_x <= ((spr_x + spr_w) + unk_1)) && (ray_x >= (spr_x - unk_1))) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_MULTISPR_CANTCHANGE:new_dist:1 | PROVEN | Observed local dataflow: new_dist = 10000;; new_dist = setToleranceDist(spr_x, spr_w, spr_y);; if (new_dist == 10000) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_MULTISPR_CANTCHANGE:i:1 | PROVEN | Observed local dataflow: i = 0;; sprite = possible_sprite(obj, i++);; sprite = possible_sprite(obj, i++); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_MULTISPR_CANTCHANGE:spr_x:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(obj, obj->follow_sprite, &spr_x, &spr_y, &spr_w, &spr_h);; spr_x += 5;; diff_x = (ray.x + ray.offset_bx) - (spr_x + (spr_w >> 1)); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_MULTISPR_CANTCHANGE:spr_y:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(obj, obj->follow_sprite, &spr_x, &spr_y, &spr_w, &spr_h);; spr_y += obj->offset_hy;; new_dist = setToleranceDist(spr_x, spr_w, spr_y); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_MULTISPR_CANTCHANGE:spr_w:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(obj, obj->follow_sprite, &spr_x, &spr_y, &spr_w, &spr_h);; spr_w -= 10;; diff_x = (ray.x + ray.offset_bx) - (spr_x + (spr_w >> 1)); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_MULTISPR_CANTCHANGE:ray_y:1 | PROVEN | Observed local dataflow: ray_y = ray.y + ray.offset_by;; new_dist = ray_y - spr_y; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_MULTISPR_CANTCHANGE:spr_h:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(obj, obj->follow_sprite, &spr_x, &spr_y, &spr_w, &spr_h);; GET_SPRITE_POS(obj, sprite, &spr_x, &spr_y, &spr_w, &spr_h); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_MULTISPR_CANTCHANGE:sprite:1 | PROVEN | Observed local dataflow: sprite = possible_sprite(obj, i++);; while ((abs32_inline(new_dist) > 7) && (sprite != 0xFF)); GET_SPRITE_POS(obj, sprite, &spr_x, &spr_y, &spr_w, &spr_h); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_MULTISPR_CANTCHANGE:diff_x:1 | PROVEN | Observed local dataflow: diff_x = (ray.x + ray.offset_bx) - (spr_x + (spr_w >> 1));; if (diff_x > 0); if (diff_x < 0) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_MULTISPR_CANTCHANGE:obj:1 | PROVEN | Object record processed by this function: Select a candidate support layer and store its Rayman distance. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:SET_RAY_DIST_PI | PROVEN | Compute support distance from fixed layer2 and its narrowed55-pixel span. | see parameter/result and side-effect contract | 1:1d334 |
| collision1.json / local:SET_RAY_DIST_PI:x:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(obj, 2, &x, &y, &w, &h);; x += 4;; new_dist = setToleranceDist(x, w, y); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_PI:y:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(obj, 2, &x, &y, &w, &h);; y += obj->offset_hy;; new_dist = setToleranceDist(x, w, y); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_PI:w:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(obj, 2, &x, &y, &w, &h);; w = 55;; new_dist = setToleranceDist(x, w, y); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_PI:h:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(obj, 2, &x, &y, &w, &h); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_PI:new_dist:1 | PROVEN | Observed local dataflow: new_dist = setToleranceDist(x, w, y);; if (new_dist != 10000); obj->ray_dist = new_dist; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_PI:prev_flip_x:1 | PROVEN | Observed local dataflow: obj->flags.flip_x = prev_flip_x; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_PI:obj:1 | PROVEN | Object record processed by this function: Compute support distance from fixed layer2 and its narrowed55-pixel span. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:SET_RAY_DIST_BAG | PROVEN | Compute a bag's support shape and launch Rayman at the terminal supporting frame. | see parameter/result and side-effect contract | 1:1d3ec |
| collision1.json / local:SET_RAY_DIST_BAG:x:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(obj, obj->follow_sprite, &x, &y, &w, &h);; x += ((s16) (w - new_w)) >> 1;; new_dist = setToleranceDist(x, w, y); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_BAG:h:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(obj, obj->follow_sprite, &x, &y, &w, &h); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_BAG:w:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(obj, obj->follow_sprite, &x, &y, &w, &h);; x += ((s16) (w - new_w)) >> 1;; w = new_w; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_BAG:y:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(obj, obj->follow_sprite, &x, &y, &w, &h);; y += obj->offset_hy;; new_dist = setToleranceDist(x, w, y); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_BAG:new_w:1 | PROVEN | Observed local dataflow: new_w = 45;; x += ((s16) (w - new_w)) >> 1;; w = new_w; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_BAG:new_dist:1 | PROVEN | Observed local dataflow: new_dist = setToleranceDist(x, w, y);; new_dist = 10000;; new_dist = 10000; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST_BAG:obj:1 | PROVEN | Object record processed by this function: Compute a bag's support shape and launch Rayman at the terminal supporting frame. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:SET_RAY_DIST | PROVEN | Select the object's support-distance calculator. | see parameter/result and side-effect contract | 1:1d4d0 |
| collision1.json / local:SET_RAY_DIST:x:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(obj, obj->follow_sprite, &x, &y, &w, &h);; x -= 2;; obj->ray_dist = setToleranceDist(x, w, y); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST:y:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(obj, obj->follow_sprite, &x, &y, &w, &h);; y += obj->offset_hy;; obj->ray_dist = setToleranceDist(x, w, y); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST:w:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(obj, obj->follow_sprite, &x, &y, &w, &h);; w += 4;; obj->ray_dist = setToleranceDist(x, w, y); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST:h:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(obj, obj->follow_sprite, &x, &y, &w, &h); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_RAY_DIST:obj:1 | PROVEN | Object record processed by this function: Select the object's support-distance calculator. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:do_boum | PROVEN | Mark fist impact and stop its outbound movement. | see parameter/result and side-effect contract | 1:1d5cc |
| collision1.json / function:DO_POING_COLLISION | PROVEN | Test the fist against tiles and active object collision shapes. | see parameter/result and side-effect contract | 1:1d5fc |
| collision1.json / local:DO_POING_COLLISION:x:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(poing_obj, 0, &x, &y, &w, &h);; btyp = BTYP((s16) ((poing_obj->x + poing_obj->offset_bx) >> 4), (s16) ((poing_obj->y + ((poing_obj->offset_by + poing_obj->offset_hy) >> 1)) >> 4));; btyp = BTYP((s16) ((poing_obj->x + poing_obj->offset_bx) >> 4), (s16) ((poing_obj->y + ((poing_obj->offset_by + poing_obj->offset_hy) >> 1)) >> 4)); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_POING_COLLISION:y:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(poing_obj, 0, &x, &y, &w, &h);; btyp = BTYP((s16) ((poing_obj->x + poing_obj->offset_bx) >> 4), (s16) ((poing_obj->y + ((poing_obj->offset_by + poing_obj->offset_hy) >> 1)) >> 4));; btyp = BTYP((s16) ((poing_obj->x + poing_obj->offset_bx) >> 4), (s16) ((poing_obj->y + ((poing_obj->offset_by + poing_obj->offset_hy) >> 1)) >> 4)); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_POING_COLLISION:w:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(poing_obj, 0, &x, &y, &w, &h);; sprite = CHECK_BOX_COLLISION(TYPE_POING, x, y, w, h, obj); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_POING_COLLISION:h:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(poing_obj, 0, &x, &y, &w, &h);; sprite = CHECK_BOX_COLLISION(TYPE_POING, x, y, w, h, obj); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_POING_COLLISION:btyp:1 | PROVEN | Observed local dataflow: btyp = 0;; btyp = BTYP((s16) ((poing_obj->x + poing_obj->offset_bx) >> 4), (s16) ((poing_obj->y + ((poing_obj->offset_by + poing_obj->offset_hy) >> 1)) >> 4));; btyp = BTYP((s16) ((poing_obj->x + poing_obj->offset_bx) >> 4), (s16) ((poing_obj->y + ((poing_obj->offset_by + poing_obj->offset_hy) >> 1)) >> 4)); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_POING_COLLISION:i:1 | PROVEN | Observed local dataflow: i = 0;; obj = level.objects + actobj.objects[i];; while (i < actobj.num_active_objects) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_POING_COLLISION:obj:1 | PROVEN | Observed local dataflow: obj = level.objects + actobj.objects[i];; if (((obj->display_prio != 0) && (obj->hit_points != 0)) && (get_eta(obj)->flags & 8)); sprite = CHECK_BOX_COLLISION(TYPE_POING, x, y, w, h, obj); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_POING_COLLISION:sprite:1 | PROVEN | Observed local dataflow: sprite = CHECK_BOX_COLLISION(TYPE_POING, x, y, w, h, obj);; if (sprite != (-1)); ObjectsFonctions[obj->type].poing_collision(obj, sprite); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_POING_COLLISION:old_hp:1 | PROVEN | Observed local dataflow: old_hp = obj->hit_points;; if ((obj->hit_points == old_hp) && (flags[obj->type].raw & flags3_1_poing_collision_snd)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_POING_COLLISION:unk_1:1 | PROVEN | Hit object's eta animation-speed high nibble, used to retain mode10/11 phase2. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:DoPoingCollisionDefault | PROVEN | Empty default per-type fist collision callback. | see parameter/result and side-effect contract | 1:1d858 |
| collision1.json / local:DoPoingCollisionDefault:obj:1 | UNKNOWN | Unused parameter declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DoPoingCollisionDefault:sprite:1 | UNKNOWN | Unused parameter declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:COLL_BOX_ALL_SPRITES | PROVEN | Return the first overlapping animation layer index. | see parameter/result and side-effect contract | 1:1d85c |
| collision1.json / local:COLL_BOX_ALL_SPRITES:i:1 | PROVEN | Observed local dataflow: for (i = 0; i < (obj->animations[obj->anim_index].layers_per_frame & 0x3fff); i++); GET_SPRITE_POS(obj, i, &spr_x, &spr_y, &spr_w, &spr_h);; res = i; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_BOX_ALL_SPRITES:spr_x:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(obj, i, &spr_x, &spr_y, &spr_w, &spr_h);; if (inter_box(in_x, in_y, in_w, in_h, spr_x, spr_y, spr_w, spr_h)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_BOX_ALL_SPRITES:spr_y:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(obj, i, &spr_x, &spr_y, &spr_w, &spr_h);; if (inter_box(in_x, in_y, in_w, in_h, spr_x, spr_y, spr_w, spr_h)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_BOX_ALL_SPRITES:spr_w:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(obj, i, &spr_x, &spr_y, &spr_w, &spr_h);; if (inter_box(in_x, in_y, in_w, in_h, spr_x, spr_y, spr_w, spr_h)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_BOX_ALL_SPRITES:spr_h:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(obj, i, &spr_x, &spr_y, &spr_w, &spr_h);; if (inter_box(in_x, in_y, in_w, in_h, spr_x, spr_y, spr_w, spr_h)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_BOX_ALL_SPRITES:res:1 | PROVEN | Observed local dataflow: res = i;; return res; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_BOX_ALL_SPRITES:in_x:1 | PROVEN | Observed parameter dataflow: if (inter_box(in_x, in_y, in_w, in_h, spr_x, spr_y, spr_w, spr_h)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_BOX_ALL_SPRITES:in_y:1 | PROVEN | Observed parameter dataflow: if (inter_box(in_x, in_y, in_w, in_h, spr_x, spr_y, spr_w, spr_h)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_BOX_ALL_SPRITES:in_w:1 | PROVEN | Observed parameter dataflow: if (inter_box(in_x, in_y, in_w, in_h, spr_x, spr_y, spr_w, spr_h)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_BOX_ALL_SPRITES:in_h:1 | PROVEN | Observed parameter dataflow: if (inter_box(in_x, in_y, in_w, in_h, spr_x, spr_y, spr_w, spr_h)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_BOX_ALL_SPRITES:obj:1 | PROVEN | Object record processed by this function: Return the first overlapping animation layer index. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:COLL_RAY_PIC | STRONG | Test the sampled Rayman tiles for hurt tile type8. | see parameter/result and side-effect contract | 1:1d914 |
| collision1.json / local:COLL_RAY_PIC:index:1 | STRONG | Observed local dataflow: index = ray.ray_dist;; res = mp.map[index].tile_type == BTYP_HURT;; index -= mp.width; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_RAY_PIC:left:1 | STRONG | Observed local dataflow: left = index - 1;; res = (mp.map[left].tile_type == BTYP_HURT) \|\| (mp.map[right].tile_type == BTYP_HURT);; left = index - mp.width; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_RAY_PIC:right:1 | STRONG | Observed local dataflow: right = index + 1;; res = (mp.map[left].tile_type == BTYP_HURT) \|\| (mp.map[right].tile_type == BTYP_HURT); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:COLL_RAY_PIC:res:1 | STRONG | Observed local dataflow: res = mp.map[index].tile_type == BTYP_HURT;; if (!res); res = mp.map[index].tile_type == BTYP_HURT; | declared C storage; see exact computation and function contract |  |
| collision1.json / function:COLL_RAY_BLK_MORTEL | PROVEN | Enter lethal-spike or cliff states from tile contact. | see parameter/result and side-effect contract | 1:1da98 |
| collision1.json / function:RAY_KO | PROVEN | Enter stationary knockout state3:0. | see parameter/result and side-effect contract | 1:1dbac |
| collision1.json / function:RAY_HIT | STRONG | Apply optional HP damage and establish hurt knockback. | see parameter/result and side-effect contract | 1:1dbdc |
| collision1.json / local:RAY_HIT:tile_x:1 | STRONG | Observed local dataflow: tile_x = ray.x + ray.offset_bx;; while (BTYP((s16) (tile_x >> 4), (s16) ((ray.y + ray.offset_by) >> 4)) == BTYP_WATER) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:RAY_HIT:eject_sens:1 | STRONG | Observed local dataflow: eject_sens = saxo2_get_eject_sens();; eject_sens = sko_get_eject_sens();; eject_sens = 1; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:RAY_HIT:bump_speed:1 | STRONG | Observed local dataflow: bump_speed = 5;; bump_speed = 4;; bump_speed = 2; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:RAY_HIT:xspeed_delta:1 | STRONG | Observed local dataflow: eject_sens = sgn(xspeed_delta); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:RAY_HIT:hurt:1 | STRONG | Observed parameter dataflow: if (hurt) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:RAY_HIT:obj:1 | STRONG | Object record processed by this function: Apply optional HP damage and establish hurt knockback. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:standard_frontZone | STRONG | Extend an animation rectangle into the object's facing detection region. | see parameter/result and side-effect contract | 1:1df34 |
| collision1.json / local:standard_frontZone:obj:1 | STRONG | Object record processed by this function: Extend an animation rectangle into the object's facing detection region. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:standard_frontZone:x:1 | STRONG | Observed parameter dataflow: *x -= obj->detect_zone;; *x += (*w) >> 1; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:standard_frontZone:w:1 | STRONG | Observed parameter dataflow: *x += (*w) >> 1;; *w = ((*w) >> 1) + obj->detect_zone; | declared C storage; see exact computation and function contract |  |
| collision1.json / function:SET_DETECT_ZONE_FLAG | STRONG | Update the object's consecutive-overlap detection phase. | see parameter/result and side-effect contract | 1:1df88 |
| collision1.json / local:SET_DETECT_ZONE_FLAG:obj_w:1 | STRONG | Observed local dataflow: GET_ANIM_POS(obj, &obj_x, &obj_y, &obj_w, &obj_h);; obj_w = obj->detect_zone;; standard_frontZone(obj, &obj_x, &obj_w); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_DETECT_ZONE_FLAG:obj_y:1 | STRONG | Observed local dataflow: GET_ANIM_POS(obj, &obj_x, &obj_y, &obj_w, &obj_h);; obj_y -= 16;; obj_y += obj_h; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_DETECT_ZONE_FLAG:obj_h:1 | STRONG | Observed local dataflow: GET_ANIM_POS(obj, &obj_x, &obj_y, &obj_w, &obj_h);; obj_x += (obj_h >> 1) - (obj->detect_zone >> 1);; obj_h += 64; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_DETECT_ZONE_FLAG:obj_x:1 | STRONG | Observed local dataflow: GET_ANIM_POS(obj, &obj_x, &obj_y, &obj_w, &obj_h);; obj_x += (obj_h >> 1) - (obj->detect_zone >> 1);; standard_frontZone(obj, &obj_x, &obj_w); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_DETECT_ZONE_FLAG:det_zn_flg:1 | STRONG | Previous byte detection phase before overlap advances it. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SET_DETECT_ZONE_FLAG:obj:1 | STRONG | Object record processed by this function: Update the object's consecutive-overlap detection phase. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:goToRay | PROVEN | Apply type-specific facing and script entry when a linked object is released. | see parameter/result and side-effect contract | 1:1e738 |
| collision1.json / local:goToRay:follow_sprite:1 | PROVEN | Observed local dataflow: follow_sprite = obj->follow_sprite;; if ((follow_sprite < 2) \|\| ((follow_sprite > 3) && ((follow_sprite < 6) \|\| (follow_sprite > 7)))) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:goToRay:obj:1 | PROVEN | Object record processed by this function: Apply type-specific facing and script entry when a linked object is released. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:unleashMonsterHost | PROVEN | Activate objects detached from a generating-door link cycle. | see parameter/result and side-effect contract | 1:1e998 |
| collision1.json / local:unleashMonsterHost:linked_obj:1 | PROVEN | Observed local dataflow: linked_obj = &level.objects[linked_id];; if (!((flags[linked_obj->type].raw & flags1_2_is_collectible) && bonus_taken(linked_obj->id))); obj_init(linked_obj); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:unleashMonsterHost:prev_id:1 | PROVEN | Observed local dataflow: prev_id = linked_id;; while (prev_id != linked_id); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:unleashMonsterHost:linked_id:1 | PROVEN | Observed local dataflow: linked_id = first_id;; linked_obj = &level.objects[linked_id];; prev_id = linked_id; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:unleashMonsterHost:first_id:1 | PROVEN | Observed local dataflow: linked_id = first_id;; if (first_id != obj->id) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:unleashMonsterHost:obj:1 | PROVEN | Object record processed by this function: Activate objects detached from a generating-door link cycle. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:SHOW_COLLISIONS_ZONES | PROVEN | Traverse enabled collision candidates and call box checks while discarding the result. | see parameter/result and side-effect contract | 1:1ea2c |
| collision1.json / local:SHOW_COLLISIONS_ZONES:i:1 | PROVEN | Observed local dataflow: i = 0;; obj = &level.objects[actobj.objects[i]];; while (i < actobj.num_active_objects) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:SHOW_COLLISIONS_ZONES:obj:1 | PROVEN | Observed local dataflow: obj = &level.objects[actobj.objects[i]];; if ((!(flags[obj->type].raw & flags0_4_no_collision)) && (get_eta(obj)->flags & 0x20)); CHECK_BOX_COLLISION(TYPE_23_RAYMAN, ray_zdc_x, ray_zdc_h, ray_zdc_w, ray_zdc_y, obj); | declared C storage; see exact computation and function contract |  |
| collision1.json / function:DO_COLLISIONS | PROVEN | Dispatch Rayman contact callbacks and age the invulnerability counter. | see parameter/result and side-effect contract | 1:1eae8 |
| collision1.json / local:DO_COLLISIONS:i:1 | PROVEN | Observed local dataflow: i = 0;; obj = level.objects + actobj.objects[i];; while (i < actobj.num_active_objects) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_COLLISIONS:obj:1 | PROVEN | Observed local dataflow: obj = level.objects + actobj.objects[i];; if (get_eta(obj)->flags & 0x20); if (!(flags[obj->type].raw & flags2_0x10_do_not_check_ray_collision)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_COLLISIONS:collision:1 | PROVEN | Observed local dataflow: collision = -1;; collision = -1;; collision = CHECK_BOX_COLLISION(TYPE_23_RAYMAN, ray_zdc_x, ray_zdc_h, ray_zdc_w, ray_zdc_y, obj); | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_COLLISIONS:collision:2 | PROVEN | Observed local dataflow: collision = -1;; collision = -1;; collision = CHECK_BOX_COLLISION(TYPE_23_RAYMAN, ray_zdc_x, ray_zdc_h, ray_zdc_w, ray_zdc_y, obj); | declared C storage; see exact computation and function contract |  |
| collision1.json / function:DoRaymanCollisionDefault | PROVEN | Apply special contact knockback while preserving ongoing invulnerability. | see parameter/result and side-effect contract | 1:1ed74 |
| collision1.json / local:DoRaymanCollisionDefault:ray_iframes:1 | PROVEN | Observed local dataflow: if (ray_iframes >= 60); ray.iframes_timer = ray_iframes;; if (ray_iframes == (-1)) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DoRaymanCollisionDefault:obj:1 | PROVEN | Object record processed by this function: Apply special contact knockback while preserving ongoing invulnerability. | declared C storage; see exact computation and function contract |  |
| collision1.json / function:DO_OBJ_COLLISIONS | STRONG | Apply selected falling-object contacts to active badguys and the Joe buoy. | see parameter/result and side-effect contract | 1:1ee20 |
| collision1.json / local:DO_OBJ_COLLISIONS:in_x:1 | STRONG | Observed local dataflow: GET_ANIM_POS(obj, &in_x, &in_y, &in_w, &in_h);; in_x += 10;; in_x = offs + in_x; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_OBJ_COLLISIONS:in_y:1 | STRONG | Observed local dataflow: GET_ANIM_POS(obj, &in_x, &in_y, &in_w, &in_h);; in_y += 20;; in_y = offs + in_y; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_OBJ_COLLISIONS:in_w:1 | STRONG | Observed local dataflow: GET_ANIM_POS(obj, &in_x, &in_y, &in_w, &in_h);; in_w += -20;; in_w -= offs * 2; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_OBJ_COLLISIONS:in_h:1 | STRONG | Observed local dataflow: GET_ANIM_POS(obj, &in_x, &in_y, &in_w, &in_h);; in_h += -25;; in_h -= offs * 2; | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_OBJ_COLLISIONS:done:1 | STRONG | Observed local dataflow: done = false;; done = true;; if (done) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_OBJ_COLLISIONS:i:1 | STRONG | Observed local dataflow: i = 0;; cur_obj = &level.objects[actobj.objects[i]];; if (i < actobj.num_active_objects) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_OBJ_COLLISIONS:cur_obj:1 | STRONG | Observed local dataflow: cur_obj = &level.objects[actobj.objects[i]];; if (obj != cur_obj); switch (cur_obj->type) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_OBJ_COLLISIONS:cur_x:1 | STRONG | Observed local dataflow: if (((cur_obj->main_etat != 2) && (((cur_obj->main_etat != 0) \|\| (cur_obj->sub_etat != 3)) != 0)) && (((!(cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat].flags & 0x40)) && GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &cur_x, &cur_y, &cur_w, &cur_h)) && inter_box(cur_x, cur_y, cur_w, cur_h, in_x, in_y, in_w, in_h))); if (((GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &cur_x, &cur_y, &cur_w, &cur_h) && (obj->type == TYPE_POI2)) && inter_box(cur_x, cur_y, cur_w, cur_h, in_x, in_y, in_w, in_h)) && (!((cur_obj->main_etat == 2) && (cur_obj->sub_etat == 6)))) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_OBJ_COLLISIONS:cur_y:1 | STRONG | Observed local dataflow: if (((cur_obj->main_etat != 2) && (((cur_obj->main_etat != 0) \|\| (cur_obj->sub_etat != 3)) != 0)) && (((!(cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat].flags & 0x40)) && GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &cur_x, &cur_y, &cur_w, &cur_h)) && inter_box(cur_x, cur_y, cur_w, cur_h, in_x, in_y, in_w, in_h))); if (((GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &cur_x, &cur_y, &cur_w, &cur_h) && (obj->type == TYPE_POI2)) && inter_box(cur_x, cur_y, cur_w, cur_h, in_x, in_y, in_w, in_h)) && (!((cur_obj->main_etat == 2) && (cur_obj->sub_etat == 6)))) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_OBJ_COLLISIONS:cur_w:1 | STRONG | Observed local dataflow: if (((cur_obj->main_etat != 2) && (((cur_obj->main_etat != 0) \|\| (cur_obj->sub_etat != 3)) != 0)) && (((!(cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat].flags & 0x40)) && GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &cur_x, &cur_y, &cur_w, &cur_h)) && inter_box(cur_x, cur_y, cur_w, cur_h, in_x, in_y, in_w, in_h))); if (((GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &cur_x, &cur_y, &cur_w, &cur_h) && (obj->type == TYPE_POI2)) && inter_box(cur_x, cur_y, cur_w, cur_h, in_x, in_y, in_w, in_h)) && (!((cur_obj->main_etat == 2) && (cur_obj->sub_etat == 6)))) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_OBJ_COLLISIONS:cur_h:1 | STRONG | Observed local dataflow: if (((cur_obj->main_etat != 2) && (((cur_obj->main_etat != 0) \|\| (cur_obj->sub_etat != 3)) != 0)) && (((!(cur_obj->eta[cur_obj->main_etat][cur_obj->sub_etat].flags & 0x40)) && GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &cur_x, &cur_y, &cur_w, &cur_h)) && inter_box(cur_x, cur_y, cur_w, cur_h, in_x, in_y, in_w, in_h))); if (((GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &cur_x, &cur_y, &cur_w, &cur_h) && (obj->type == TYPE_POI2)) && inter_box(cur_x, cur_y, cur_w, cur_h, in_x, in_y, in_w, in_h)) && (!((cur_obj->main_etat == 2) && (cur_obj->sub_etat == 6)))) | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_OBJ_COLLISIONS:obj:1 | STRONG | Object record processed by this function: Apply selected falling-object contacts to active badguys and the Joe buoy. | declared C storage; see exact computation and function contract |  |
| collision1.json / local:DO_OBJ_COLLISIONS:offs:1 | STRONG | Symmetric pixel inset for ordinary source rectangles; falling fruit types use fixed insets instead. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:test_allowed | PROVEN | Report entry into a different sampled tile and cache its linear index. | see parameter/result and side-effect contract | 1:1f1e0 |
| collision2.json / local:test_allowed:res:1 | PROVEN | Observed local dataflow: res = true;; return res; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:test_allowed:map_index:1 | PROVEN | Observed local dataflow: if (map_index != ((s16) obj->test_block_index)); obj->test_block_index = map_index; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:test_allowed:obj:1 | PROVEN | Object record processed by this function: Report entry into a different sampled tile and cache its linear index. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:test_allowed:center_x:1 | PROVEN | Observed parameter dataflow: s16 map_index = ((center_x + obj->x) >> 4) + (((center_y + obj->y) >> 4) * mp.width); | declared C storage; see exact computation and function contract |  |
| collision2.json / local:test_allowed:center_y:1 | PROVEN | Observed parameter dataflow: s16 map_index = ((center_x + obj->x) >> 4) + (((center_y + obj->y) >> 4) * mp.width); | declared C storage; see exact computation and function contract |  |
| collision2.json / function:obj_jump | PROVEN | Start the two supported enemy-specific jumps. | see parameter/result and side-effect contract | 1:1f224 |
| collision2.json / local:obj_jump:spd_y:1 | PROVEN | Observed local dataflow: spd_y = -3;; spd_y = -3;; obj->speed_y = spd_y; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:obj_jump:gravity_value_2:1 | PROVEN | Observed local dataflow: gravity_value_2 = 5;; obj->gravity_value_2 = gravity_value_2; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:obj_jump:spd_x:1 | PROVEN | Observed local dataflow: obj->speed_x = spd_x; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:obj_jump:label:1 | PROVEN | Observed local dataflow: label = 15;; label = 1;; if (label != 255) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:obj_jump:obj:1 | PROVEN | Object record processed by this function: Start the two supported enemy-specific jumps. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DO_MOVING_PLATFORM_COMMAND | PROVEN | Process direction-change markers and explicit moving-platform speed commands. | see parameter/result and side-effect contract | 1:1f2fc |
| collision2.json / local:DO_MOVING_PLATFORM_COMMAND:y_cen:1 | PROVEN | Observed local dataflow: y_cen = get_center_y(obj);; chdir = on_block_chdir(obj, x_cen, y_cen);; if (chdir && ((s16) test_allowed(obj, x_cen, y_cen))) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_MOVING_PLATFORM_COMMAND:x_cen:1 | PROVEN | Observed local dataflow: x_cen = get_center_x(obj);; chdir = on_block_chdir(obj, x_cen, y_cen);; if (chdir && ((s16) test_allowed(obj, x_cen, y_cen))) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_MOVING_PLATFORM_COMMAND:chdir:1 | PROVEN | Observed local dataflow: chdir = on_block_chdir(obj, x_cen, y_cen);; if (chdir && ((s16) test_allowed(obj, x_cen, y_cen))) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_MOVING_PLATFORM_COMMAND:obj:1 | PROVEN | Object record processed by this function: Process direction-change markers and explicit moving-platform speed commands. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DoPTGRAPPINPoingCollision | PROVEN | Convert a fist hit on a grapple object into swinging attachment. | see parameter/result and side-effect contract | 1:1f378 |
| collision2.json / local:DoPTGRAPPINPoingCollision:angle:1 | PROVEN | Observed local dataflow: angle = ANGLE_RAYMAN(obj);; obj->follow_x = angle;; if ((angle > 256) \|\| ((angle == 256) && (!ray.flags.flip_x))) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DoPTGRAPPINPoingCollision:obj:1 | PROVEN | Object record processed by this function: Convert a fist hit on a grapple object into swinging attachment. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DoPTGRAPPINPoingCollision:sprite:1 | UNKNOWN | Unused parameter declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DO_ONE_CMD_WAIT | PROVEN | Apply the wait command's state-specific stopping behaviour. | see parameter/result and side-effect contract | 1:1f44c |
| collision2.json / local:DO_ONE_CMD_WAIT:obj:1 | PROVEN | Object record processed by this function: Apply the wait command's state-specific stopping behaviour. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DO_ONE_CMD_LR_ATTENTE | PROVEN | Handle an idle object's left/right command by movement, jumping or attacks. | see parameter/result and side-effect contract | 1:1f488 |
| collision2.json / local:DO_ONE_CMD_LR_ATTENTE:sub_etat:1 | PROVEN | Observed local dataflow: if (!((((((((((((sub_etat == 1) \|\| (sub_etat == 2)) \|\| (sub_etat == 11)) \|\| (sub_etat == 22)) \|\| (sub_etat == 3)) \|\| (sub_etat == 9)) \|\| (sub_etat == 4)) \|\| (sub_etat == 5)) \|\| (sub_etat == 6)) \|\| (sub_etat == 15)) \|\| (sub_etat == 16)) \|\| (sub_etat == 17))); if ((sub_etat == 3) \|\| (sub_etat == 6)); if (((sub_etat == 11) \|\| (sub_etat == 2)) \|\| (sub_etat == 22)) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_ONE_CMD_LR_ATTENTE:type:1 | PROVEN | Observed local dataflow: if (!((type == TYPE_CHASSEUR1) \|\| (type == TYPE_CHASSEUR2))); switch (type) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_ONE_CMD_LR_ATTENTE:obj:1 | PROVEN | Object record processed by this function: Handle an idle object's left/right command by movement, jumping or attacks. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DO_ONE_CMD_UPDOWN | PROVEN | Apply up/down commands to fish and two platform types. | see parameter/result and side-effect contract | 1:1f594 |
| collision2.json / local:DO_ONE_CMD_UPDOWN:obj:1 | PROVEN | Object record processed by this function: Apply up/down commands to fish and two platform types. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:special_pour_liv | PROVEN | Disable command reading for badguy type0 in state1:11. | see parameter/result and side-effect contract | 1:1f63c |
| collision2.json / local:special_pour_liv:event:1 | PROVEN | Object record processed by this function: Disable command reading for badguy type0 in state1:11. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DO_ONE_CMD | PROVEN | Dispatch the object's current movement command. | see parameter/result and side-effect contract | 1:1f658 |
| collision2.json / local:DO_ONE_CMD:etat:1 | PROVEN | Main state snapshot taken before special_pour_liv and command dispatch. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_ONE_CMD:obj:1 | PROVEN | Object record processed by this function: Dispatch the object's current movement command. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DO_ROLL_EYES | PROVEN | Aim eye animation at Rayman and advance the killing-eye firing cycle. | see parameter/result and side-effect contract | 1:1f714 |
| collision2.json / local:DO_ROLL_EYES:diff_x:1 | PROVEN | Observed local dataflow: diff_x = ray.x - obj->x;; unk_x_1 = diff_x + 0x20; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_ROLL_EYES:diff_y:1 | PROVEN | Observed local dataflow: diff_y = ray.y - obj->y;; unk_y_1 = diff_y + 0x19; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_ROLL_EYES:screen_x:1 | PROVEN | Observed local dataflow: screen_x = obj->screen_x;; if ((((screen_x > (-52)) && (screen_y > (-36))) && (screen_x < 0x131)) && (screen_y < 195)) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_ROLL_EYES:screen_y:1 | PROVEN | Observed local dataflow: screen_y = obj->screen_y;; if ((((screen_x > (-52)) && (screen_y > (-36))) && (screen_x < 0x131)) && (screen_y < 195)) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_ROLL_EYES:unk_x_1:1 | PROVEN | Adjusted Rayman-minus-object x displacement, made nonnegative then optionally halved. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_ROLL_EYES:unk_y_1:1 | PROVEN | Adjusted Rayman-minus-object y displacement, made nonnegative then optionally halved. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_ROLL_EYES:fire_par_3:1 | PROVEN | Quadrant-mapped firing/animation direction; -1 for coincident adjusted position. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_ROLL_EYES:y_gt_0:1 | PROVEN | Observed local dataflow: y_gt_0 = unk_y_1 > 0;; if (y_gt_0 == 0); if (y_gt_0) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_ROLL_EYES:x_gt_0:1 | PROVEN | Observed local dataflow: x_gt_0 = unk_x_1 > 0;; if (x_gt_0 == 0); if (x_gt_0) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_ROLL_EYES:obj:1 | PROVEN | Object record processed by this function: Aim eye animation at Rayman and advance the killing-eye firing cycle. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DoKillingEyesPoingCollision | PROVEN | Decrement killing-eye HP and choose hurt or death state. | see parameter/result and side-effect contract | 1:1f978 |
| collision2.json / local:DoKillingEyesPoingCollision:obj:1 | PROVEN | Object record processed by this function: Decrement killing-eye HP and choose hurt or death state. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DoKillingEyesPoingCollision:sprite:1 | UNKNOWN | Unused parameter declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:YaUnBloc | STRONG | Sample hard tiles around a tall object's side or vertical edges. | see parameter/result and side-effect contract | 1:1f9b8 |
| collision2.json / local:YaUnBloc:i:1 | STRONG | Observed local dataflow: i = 0;; while (i < 10); i++; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:YaUnBloc:unk_y:1 | STRONG | Vertical pixel offset6+16*i for ten side samples. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:YaUnBloc:res:1 | STRONG | Observed local dataflow: res = true;; res = true;; res = true; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:YaUnBloc:obj:1 | STRONG | Object record processed by this function: Sample hard tiles around a tall object's side or vertical edges. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:YaUnBloc:dir:1 | STRONG | Observed parameter dataflow: switch (dir) | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DO_BAG1_COMMAND | PROVEN | Run bag commands and emit a timed landing effect at frame14. | see parameter/result and side-effect contract | 1:1fbf4 |
| collision2.json / local:DO_BAG1_COMMAND:obj:1 | PROVEN | Object record processed by this function: Run bag commands and emit a timed landing effect at frame14. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DoRaymanBag1Collision | PROVEN | Squash a grounded Rayman when scrolling is unlocked. | see parameter/result and side-effect contract | 1:1fc64 |
| collision2.json / local:DoRaymanBag1Collision:obj:1 | UNKNOWN | Unused parameter declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:MarCoince | PROVEN | Check three adjacent tiles at the maracas object's movement-leading boundary. | see parameter/result and side-effect contract | 1:1fcb0 |
| collision2.json / local:MarCoince:map_ind:1 | PROVEN | Observed local dataflow: map_ind = (x >> 4) + (mp.width * (y >> 4));; if (((u8 *) block_flags)[mp.map[map_ind].tile_type] & 2); if (((u8 *) block_flags)[mp.map[++map_ind].tile_type] & 2) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:MarCoince:pos_to_check:1 | PROVEN | Signed x or y remainder modulo16 selecting the leading boundary sample. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:MarCoince:res:1 | PROVEN | Observed local dataflow: res = true;; res = true;; res = true; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:MarCoince:y:1 | PROVEN | Observed local dataflow: y = obj->y + obj->offset_hy;; pos_to_check = y % 16;; map_ind = (x >> 4) + (mp.width * (y >> 4)); | declared C storage; see exact computation and function contract |  |
| collision2.json / local:MarCoince:x:1 | PROVEN | Observed local dataflow: x = (obj->x + obj->offset_bx) - 25;; map_ind = (x >> 4) + (mp.width * (y >> 4));; map_ind = (x >> 4) + (mp.width * ((y + (16 * 3)) >> 4)); | declared C storage; see exact computation and function contract |  |
| collision2.json / local:MarCoince:obj:1 | PROVEN | Object record processed by this function: Check three adjacent tiles at the maracas object's movement-leading boundary. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:MarCoince:dir:1 | PROVEN | Observed parameter dataflow: switch (dir) | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DO_MOVE_MARACAS_COMMAND | PROVEN | Move the maracas support from rider position and marker commands. | see parameter/result and side-effect contract | 1:1ffdc |
| collision2.json / local:DO_MOVE_MARACAS_COMMAND:cen_x:1 | PROVEN | Observed local dataflow: cen_x = get_center_x(obj);; if (((s16) on_block_chdir(obj, cen_x, cen_y)) && ((s16) test_allowed(obj, cen_x, cen_y))) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_MOVE_MARACAS_COMMAND:cen_y:1 | PROVEN | Observed local dataflow: cen_y = get_center_y(obj);; if (((s16) on_block_chdir(obj, cen_x, cen_y)) && ((s16) test_allowed(obj, cen_x, cen_y))) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_MOVE_MARACAS_COMMAND:speed_x:1 | PROVEN | Observed local dataflow: speed_x = 0;; speed_x = 1;; speed_x += 1; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_MOVE_MARACAS_COMMAND:diff_x:1 | PROVEN | Observed local dataflow: diff_x = (ray.offset_bx + ray.x) - (obj->offset_bx + obj->x);; if (diff_x > 0); if (diff_x > 10) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_MOVE_MARACAS_COMMAND:diff_x_inv:1 | PROVEN | Observed local dataflow: diff_x_inv = -diff_x;; if (diff_x_inv > 10); if (diff_x_inv > 15) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_MOVE_MARACAS_COMMAND:diff_x_less:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_MOVE_MARACAS_COMMAND:obj:1 | PROVEN | Object record processed by this function: Move the maracas support from rider position and marker commands. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DO_FLASH_COMMAND | PROVEN | Integrate flash motion using signed1/128-pixel residuals. | see parameter/result and side-effect contract | 1:201a4 |
| collision2.json / local:DO_FLASH_COMMAND:obj:1 | PROVEN | Object record processed by this function: Integrate flash motion using signed1/128-pixel residuals. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DO_WLKNOT_COMMAND | PROVEN | Move a walking object and turn at a newly encountered direction-change tile. | see parameter/result and side-effect contract | 1:20260 |
| collision2.json / local:DO_WLKNOT_COMMAND:bx:1 | PROVEN | Observed local dataflow: bx = obj->offset_bx;; if (((s16) test_allowed(obj, bx, by)) && ((s16) on_block_chdir(obj, bx, by))) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_WLKNOT_COMMAND:by:1 | PROVEN | Observed local dataflow: by = obj->offset_by - 8;; if (((s16) test_allowed(obj, bx, by)) && ((s16) on_block_chdir(obj, bx, by))) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_WLKNOT_COMMAND:obj:1 | PROVEN | Object record processed by this function: Move a walking object and turn at a newly encountered direction-change tile. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:ACTIVE_L_EAU | PROVEN | Initialize the linked visible water-strip objects. | see parameter/result and side-effect contract | 1:20318 |
| collision2.json / local:ACTIVE_L_EAU:i:1 | PROVEN | Observed local dataflow: i = 0;; while (i < nb_objs); i++; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:ACTIVE_L_EAU:cur_obj:1 | PROVEN | Observed local dataflow: cur_obj = level.objects;; if ((cur_obj->type == TYPE_157_EAU) && (!cur_obj->is_active)); cur_obj->flags.flip_x = 0; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:ACTIVE_L_EAU:nb_objs:1 | PROVEN | Observed local dataflow: nb_objs = level.nb_objects;; while (i < nb_objs) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:ACTIVE_L_EAU:eau_obj:1 | PROVEN | Object record processed by this function: Initialize the linked visible water-strip objects. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DO_EAU_QUI_MONTE | STRONG | Update world-specific rising water position and horizontal strip recycling. | see parameter/result and side-effect contract | 1:20490 |
| collision2.json / local:DO_EAU_QUI_MONTE:stone_id:1 | STRONG | Observed local dataflow: stone_id = pierreAcorde_obj_id;; if (stone_id == (-1)); if ((level.objects[stone_id].hit_points > 0) && (obj->y < (ymapmax + (SCREEN_HEIGHT - 80)))) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_EAU_QUI_MONTE:countdown:1 | STRONG | Observed local dataflow: if (countdown > 0); obj->param = countdown - 1; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_EAU_QUI_MONTE:ray_bottom:1 | STRONG | Observed local dataflow: obj->init_y = ray_bottom - 30; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_EAU_QUI_MONTE:obj:1 | STRONG | Object record processed by this function: Update world-specific rising water position and horizontal strip recycling. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:allocateOtherPosts | PROVEN | Activate linked spike-post copies with phased animations and positions. | see parameter/result and side-effect contract | 1:2076c |
| collision2.json / local:allocateOtherPosts:cur_obj:1 | PROVEN | Observed local dataflow: cur_obj = &level.objects[cur_id];; cur_obj->flags.alive = 1;; cur_obj->is_active = 1; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:allocateOtherPosts:cur_id:1 | PROVEN | Observed local dataflow: while (cur_id != her_bh_obj->id); cur_obj = &level.objects[cur_id];; cur_id = link_init[cur_id]; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:allocateOtherPosts:her_bh_frames:1 | PROVEN | Observed local dataflow: cur_obj->anim_frame = (her_bh_obj->anim_frame + i) % her_bh_frames; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:allocateOtherPosts:i:1 | PROVEN | Observed local dataflow: i++;; cur_obj->anim_frame = (her_bh_obj->anim_frame + i) % her_bh_frames;; cur_obj->init_x = her_bh_obj->init_x + ((i * 32) * (her_bh_obj->hit_points - 2)); | declared C storage; see exact computation and function contract |  |
| collision2.json / local:allocateOtherPosts:her_bh_obj:1 | PROVEN | Object record processed by this function: Activate linked spike-post copies with phased animations and positions. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:doHerseCommand | PROVEN | Initialize linked spike posts once, then apply sinusoidal vertical motion. | see parameter/result and side-effect contract | 1:2088c |
| collision2.json / local:doHerseCommand:obj:1 | PROVEN | Object record processed by this function: Initialize linked spike posts once, then apply sinusoidal vertical motion. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:doBlackRaymanCommand | PROVEN | Replay the stored Rayman/fist history into Black Rayman objects. | see parameter/result and side-effect contract | 1:20908 |
| collision2.json / local:doBlackRaymanCommand:scale:1 | PROVEN | Observed local dataflow: scale = br_stack->scale;; br_obj->scale = scale; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:doBlackRaymanCommand:fist_obj:1 | PROVEN | Observed local dataflow: fist_obj = &level.objects[black_fist_obj_id];; fist_obj->is_active = 0;; fist_obj->flags.alive = 0; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:doBlackRaymanCommand:br_stack:1 | PROVEN | Observed local dataflow: ray_stack_t *br_stack;; br_stack = &rayStack[black_pos_in_stack];; br_obj->x = br_stack->x_pos; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:doBlackRaymanCommand:br_obj:1 | PROVEN | Object record processed by this function: Replay the stored Rayman/fist history into Black Rayman objects. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:doBlKRayRaymanCollision | PROVEN | Set Rayman HP to zero on Black Rayman contact after history is full. | see parameter/result and side-effect contract | 1:20be4 |
| collision2.json / local:doBlKRayRaymanCollision:obj:1 | UNKNOWN | Unused parameter declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DO_POELLE_COMMAND | PROVEN | Attach the pan object to Rayman or expose it as a support when unmounted. | see parameter/result and side-effect contract | 1:20bf8 |
| collision2.json / local:DO_POELLE_COMMAND:abs_ray_spd_x:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_POELLE_COMMAND:po_x:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(po_obj, 0, &po_x, &po_y, &po_w, &po_h); | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_POELLE_COMMAND:po_h:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(po_obj, 0, &po_x, &po_y, &po_w, &po_h); | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_POELLE_COMMAND:po_y:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(po_obj, 0, &po_x, &po_y, &po_w, &po_h);; po_y = (po_y + po_obj->offset_hy) - (po_obj->y + po_obj->offset_by);; po_obj->y = ((ray.offset_by + ray.y) - po_obj->offset_by) - po_y; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_POELLE_COMMAND:po_w:1 | PROVEN | Observed local dataflow: GET_SPRITE_POS(po_obj, 0, &po_x, &po_y, &po_w, &po_h); | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_POELLE_COMMAND:sub_etat:1 | PROVEN | Observed local dataflow: if ((((ray.main_etat == 3) && (ray.sub_etat == 23)) \|\| ((ray.main_etat == 3) && (ray.sub_etat == 22))) \|\| (!RAY_DEAD())); if ((ray.main_etat == 3) && (ray.sub_etat == 22)); if ((((ray.main_etat == 2) && (ray.sub_etat == 1)) \|\| ((ray.main_etat == 2) && (ray.sub_etat == 2))) \|\| ((ray.main_etat == 2) && (ray.sub_etat == 6))) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_POELLE_COMMAND:po_obj:1 | PROVEN | Object record processed by this function: Attach the pan object to Rayman or expose it as a support when unmounted. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DO_CORDE_COMMAND | STRONG | Test super-helicopter contact against the rope segments and damage them. | see parameter/result and side-effect contract | 1:20ea8 |
| collision2.json / local:DO_CORDE_COMMAND:pa_id:1 | STRONG | Observed local dataflow: pa_id = pierreAcorde_obj_id;; if (pa_id != (-1)); if (obj->x < level.objects[pa_id].x) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_CORDE_COMMAND:bVar2:1 | STRONG | Reused signed16 scratch: first copies ray.x, then becomes the substate-supported rectangle flag. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_CORDE_COMMAND:obj_x_pos:1 | STRONG | Observed local dataflow: obj_x_pos = obj->x;; inter = inter_box(ray_x_pos + 57, ray_y_pos + 26, 46, 4, obj_x_pos + 126, obj_y_pos + obj_y_offs, 4, obj_h);; DO_FUMEE_CORDE(obj_x_pos + 128, ray_y_pos + 30); | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_CORDE_COMMAND:obj_y_pos:1 | STRONG | Observed local dataflow: obj_y_pos = obj->y;; inter = inter_box(ray_x_pos + 57, ray_y_pos + 26, 46, 4, obj_x_pos + 126, obj_y_pos + obj_y_offs, 4, obj_h); | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_CORDE_COMMAND:ray_x_pos:1 | STRONG | Observed local dataflow: ray_x_pos = bVar2;; inter = inter_box(ray_x_pos + 57, ray_y_pos + 26, 46, 4, obj_x_pos + 126, obj_y_pos + obj_y_offs, 4, obj_h); | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_CORDE_COMMAND:sVar4:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_CORDE_COMMAND:ray_y_pos:1 | STRONG | Observed local dataflow: ray_y_pos = ray.y;; inter = inter_box(ray_x_pos + 57, ray_y_pos + 26, 46, 4, obj_x_pos + 126, obj_y_pos + obj_y_offs, 4, obj_h);; DO_FUMEE_CORDE(obj_x_pos + 128, ray_y_pos + 30); | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_CORDE_COMMAND:obj_y_offs:1 | STRONG | Observed local dataflow: obj_y_offs = 48;; obj_y_offs = 48;; obj_y_offs = 56; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_CORDE_COMMAND:obj_h:1 | STRONG | Observed local dataflow: obj_h = 24;; obj_h = 32;; obj_h = 40; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_CORDE_COMMAND:inter:1 | STRONG | Observed local dataflow: inter = bVar2;; if (inter); inter = inter_box(ray_x_pos + 57, ray_y_pos + 26, 46, 4, obj_x_pos + 126, obj_y_pos + obj_y_offs, 4, obj_h); | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_CORDE_COMMAND:hp:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_CORDE_COMMAND:obj:1 | STRONG | Object record processed by this function: Test super-helicopter contact against the rope segments and damage them. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DO_PAC_COMMAND | PROVEN | Update the stone mechanism's downward motion and rope-contact marker. | see parameter/result and side-effect contract | 1:21060 |
| collision2.json / local:DO_PAC_COMMAND:obj:1 | PROVEN | Object record processed by this function: Update the stone mechanism's downward motion and rope-contact marker. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DO_CFUMEE_COMMAND | PROVEN | Follow Rayman's vertical position and retire rope smoke after its terminal frame. | see parameter/result and side-effect contract | 1:210f8 |
| collision2.json / local:DO_CFUMEE_COMMAND:obj:1 | PROVEN | Object record processed by this function: Follow Rayman's vertical position and retire rope smoke after its terminal frame. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DO_NOVA2_COMMAND | PROVEN | Delay a nova effect, then retire it at animation end. | see parameter/result and side-effect contract | 1:21190 |
| collision2.json / local:DO_NOVA2_COMMAND:obj:1 | PROVEN | Object record processed by this function: Delay a nova effect, then retire it at animation end. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:doShipCommand | PROVEN | Lock the camera to the ship scene and release linked enemies in sequence. | see parameter/result and side-effect contract | 1:21208 |
| collision2.json / local:doShipCommand:temp_s0_2:1 | PROVEN | Linked object of the selected release type. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:doShipCommand:temp_a0_1:1 | PROVEN | Ship param interpreted as prior released-object id; -1 means none. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:doShipCommand:var_s2:1 | PROVEN | Reused byte: initially prior object inactive/no id, then requests immediate phase-loop retry. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:doShipCommand:temp_v1:1 | PROVEN | Ship command snapshot selecting WAIT,SPEED or NOP behaviour. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:doShipCommand:var_s3:1 | PROVEN | Object type selected by the ship's link phase. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:doShipCommand:test_3:1 | PROVEN | Id traversing link_init until the selected type or the ship type is reached. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:doShipCommand:index:1 | PROVEN | Observed local dataflow: index = test_3;; links += index; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:doShipCommand:links:1 | PROVEN | Observed local dataflow: links = link_init;; links += index;; test_3 = *links; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:doShipCommand:obj:1 | PROVEN | Object record processed by this function: Lock the camera to the ship scene and release linked enemies in sequence. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DO_DROP_COMMAND | PROVEN | Select droplet fall animation and retire its terminal splash. | see parameter/result and side-effect contract | 1:21400 |
| collision2.json / local:DO_DROP_COMMAND:obj:1 | PROVEN | Object record processed by this function: Select droplet fall animation and retire its terminal splash. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:move_fruit_in_water | PROVEN | Let a supported floating fruit follow Rayman's horizontal direction until tile contact. | see parameter/result and side-effect contract | 1:214a4 |
| collision2.json / local:move_fruit_in_water:btyp:1 | PROVEN | Observed local dataflow: btyp = BTYP((x + ((obj->speed_x < 0) ? (0) : (w))) >> 4, (y + 16) >> 4);; if (((u8 *) block_flags)[btyp] & 2) | declared C storage; see exact computation and function contract |  |
| collision2.json / local:move_fruit_in_water:x:1 | PROVEN | Observed local dataflow: GET_ANIM_POS(obj, &x, &y, &w, &h);; x2 = x;; btyp = BTYP((x + ((obj->speed_x < 0) ? (0) : (w))) >> 4, (y + 16) >> 4); | declared C storage; see exact computation and function contract |  |
| collision2.json / local:move_fruit_in_water:y:1 | PROVEN | Observed local dataflow: GET_ANIM_POS(obj, &x, &y, &w, &h);; btyp = BTYP((x + ((obj->speed_x < 0) ? (0) : (w))) >> 4, (y + 16) >> 4); | declared C storage; see exact computation and function contract |  |
| collision2.json / local:move_fruit_in_water:w:1 | PROVEN | Observed local dataflow: GET_ANIM_POS(obj, &x, &y, &w, &h);; x2 += w;; btyp = BTYP((x + ((obj->speed_x < 0) ? (0) : (w))) >> 4, (y + 16) >> 4); | declared C storage; see exact computation and function contract |  |
| collision2.json / local:move_fruit_in_water:h:1 | PROVEN | Observed local dataflow: GET_ANIM_POS(obj, &x, &y, &w, &h); | declared C storage; see exact computation and function contract |  |
| collision2.json / local:move_fruit_in_water:x2:1 | PROVEN | Observed local dataflow: x2 = x;; x2 += w; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:move_fruit_in_water:obj_flip_x:1 | PROVEN | Observed local dataflow: obj->flags.flip_x = obj_flip_x; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:move_fruit_in_water:obj:1 | PROVEN | Object record processed by this function: Let a supported floating fruit follow Rayman's horizontal direction until tile contact. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DO_FALLING_OBJ_CMD | PROVEN | Convert falling fruits to water support and process object contacts. | see parameter/result and side-effect contract | 1:215f0 |
| collision2.json / local:DO_FALLING_OBJ_CMD:obj:1 | PROVEN | Object record processed by this function: Convert falling fruits to water support and process object contacts. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DoFallingObjPoingCollision | PROVEN | Apply fist-dependent horizontal impulse to a falling object. | see parameter/result and side-effect contract | 1:2167c |
| collision2.json / local:DoFallingObjPoingCollision:fall_x_accel:1 | PROVEN | Observed local dataflow: fall_x_accel = poing.damage;; fall_x_accel = poing.damage - 2;; fall_x_accel = (fall_x_accel * 5) + 10; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DoFallingObjPoingCollision:obj:1 | PROVEN | Object record processed by this function: Apply fist-dependent horizontal impulse to a falling object. | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DoFallingObjPoingCollision:sprite:1 | UNKNOWN | Unused parameter declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DO_BLKTOON_EYES_CMD | PROVEN | Preserve a blacktoon's horizontal motion across eta-induced direction reversal. | see parameter/result and side-effect contract | 1:2171c |
| collision2.json / local:DO_BLKTOON_EYES_CMD:prev_spd_x:1 | PROVEN | Observed local dataflow: if ((prev_spd_x * obj->speed_x) < 0); obj->speed_x = prev_spd_x; | declared C storage; see exact computation and function contract |  |
| collision2.json / local:DO_BLKTOON_EYES_CMD:obj:1 | PROVEN | Object record processed by this function: Preserve a blacktoon's horizontal motion across eta-induced direction reversal. | declared C storage; see exact computation and function contract |  |
| collision2.json / function:DO_RAY_POS_CMD | PROVEN | Activate and position Rayman after a byte countdown. | see parameter/result and side-effect contract | 1:21768 |
| collision2.json / local:DO_RAY_POS_CMD:obj:1 | PROVEN | Object record processed by this function: Activate and position Rayman after a byte countdown. | declared C storage; see exact computation and function contract |  |
| globals.json / global:horloge | PROVEN | Shared byte phase-counter bank: clock index i has period i engine calls, with indices0/1 always zero after update. | phase calls | 2:3f9f5 |
| globals.json / global:level | PROVEN | Loaded packed object-array pointer and signed16 object count. | object array/count | 2:3cdf0 |
| globals.json / global:flags | PROVEN | Per-object-type packed32-bit behaviour/movement flags. | bitset by type | 2:257c |
| globals.json / global:actobj | PROVEN | Counted active-object id array with a following-1 sentinel. | object ids/count | 2:3ef5c |
| globals.json / global:num_world | PROVEN | Current one-based world/level selector used by engine exceptions, time tables and spawn rules. | world/level index | 2:3f88c |
| globals.json / global:block_flags | PROVEN | Per-tile-type byte classification flags used by collision and slope/water movement rules. | bitset by tile type | 2:253c |
| globals.json / global:ray | PROVEN | Live Rayman object record, updated separately from the counted active-object loop. | object record | 2:3e650 |
| globals.json / global:xmap | PROVEN | Signed16 world x of the camera/map origin. | pixels | 2:3f89e |
| globals.json / global:ymap | PROVEN | Signed16 world y of the camera/map origin. | pixels | 2:3f8a6 |
| globals.json / global:mp | PROVEN | Current map dimensions, tile count and packed six-byte tile array. | tiles / pointer | 2:3e638 |
| globals.json / global:RayEvts | PROVEN | Packed Rayman ability/status bits, including two-bit force_run and reverse fields. | bitfield states | 2:3f81a |
| globals.json / global:ray_last_ground_btyp | STRONG | Rayman ground/support classification used by airborne inertia and direction transitions. | classification code | 2:3fa72 |
| globals.json / global:ray_mode | PROVEN | Player update mode dispatcher:1 ordinary,2 mounted,3/4 death,5 breakout; nonpositive values use direct placement in DO_MOTEUR2. | mode code | 2:3f868 |
| globals.json / global:poing | PROVEN | Fist control record, including the separate16-scaled vertical accumulator. | packed control record | 2:3e770 |
| globals.json / global:finBosslevel | PROVEN | Packed persistent boss/help-event completion bits. | bitfield state | 2:3f81e |
| globals.json / global:first_boss_meet | STRONG | Byte toggled by waiting-animation swapping and tested with boss hit_points/scroll lock during activation. | boolean marker | 2:3fa71 |
| globals.json / global:level_obj | PROVEN | Activation-candidate object-id list, initially containing non-always types and later live spawned always objects. | object ids/count | 2:3cdf8 |
| globals.json / global:hand_btyp | PROVEN | Tile type at the central Rayman hand sample. | tile-type id | 2:3fa7c |
| globals.json / global:hand_btypd | PROVEN | Tile type at the hand sample eight pixels to the right. | tile-type id | 2:3fa80 |
| globals.json / global:hand_btypg | PROVEN | Tile type at the hand sample eight pixels to the left. | tile-type id | 2:3fa7f |
| globals.json / global:ray_on_poelle | PROVEN | Whether Rayman is attached to the pan support with its restricted capability set. | boolean | 2:3fa54 |
| globals.json / global:saveRMjumpX | PROVEN | Low16 bits of Rayman world x captured on a successful jump. | pixel coordinate modulo65536 | 2:3f89c |
| globals.json / global:saveRMjumpY | PROVEN | Low16 bits of Rayman world y captured on a successful jump. | pixel coordinate modulo65536 | 2:3f89a |
| globals.json / global:options_jeu | STRONG | Input callback table and packed game-option storage. | record | 2:32f78 |
| globals.json / global:ray_between_clic | STRONG | Super-helicopter retrigger window countdown. | airborne update calls | 2:3f848 |
| globals.json / global:ray_clic | UNKNOWN | Signed16 slot cleared when super-helicopter activation starts. | unknown | 2:3f840 |
| globals.json / global:ray_inertie | STRONG | Byte count of super-helicopter retriggers used for the paired impulse rule. | retrigger count | 2:3fa5e |
| globals.json / global:joy_done | PROVEN | Per-Rayman-update count/latch of handled directional inputs. | byte count | 2:2994 |
| globals.json / global:animation_attente | PROVEN | Index of the next Rayman idle animation in a five-step cycle. | cycle index | 2:3f76c |
| globals.json / global:id_obj_grapped | PROVEN | Level-object id of the grapple used by Rayman swinging. | object id | 2:3f842 |
| globals.json / global:ray_zdc_h | PROVEN | Cached Rayman collision-box world y coordinate, despite the h suffix. | pixels | 2:3f862 |
| globals.json / global:ray_zdc_w | PROVEN | Cached Rayman collision-box width span. | pixels | 2:3f864 |
| globals.json / global:ray_zdc_x | PROVEN | Cached Rayman collision-box world x coordinate. | pixels | 2:3f860 |
| globals.json / global:rayStack | PROVEN | 101-entry ring of Rayman and optional fist display snapshots for Black Rayman replay. | 18-byte records | 2:3f026 |
| globals.json / global:zdc_tab | PROVEN | Table of packed8-byte object collision-zone records. | zdc_t records | 2:2dae |
| globals.json / global:ZDCPTR | UNKNOWN | Unsigned16 slot returned by get_ZDCPTR. | unknown | 2:3f81c |
| globals.json / global:bagDy | PROVEN | Per-animation-frame vertical offset for the special bag collision rectangle. | pixels / sentinel | 2:4b9c |
| globals.json / global:bagH | PROVEN | Per-frame height of the special bag collision rectangle. | pixels | 2:4bec |
| globals.json / global:bagW | PROVEN | Per-frame width of the special bag collision rectangle. | pixels | 2:4bc4 |
| globals.json / global:pente | STRONG | Angle lookup used to project slope-platform width from absolute signed slope. | 1/512-turn angle | 2:4a7c |
| globals.json / global:poing_obj_id | PROVEN | Level-object id of the live fist object. | object id | 2:3f88a |
| globals.json / global:ray_speed_inv | UNKNOWN | Signed16 slot cleared when Rayman enters hit knockback. | unknown | 2:3f83c |
| globals.json / global:N_anim | STRONG | Integer slope-ratio lookup for eye aiming direction. | direction offsets | 2:4a9c |
| globals.json / global:black_fist_obj_id | PROVEN | Level-object id of Black Rayman replay fist. | object id | 2:3f8be |
| globals.json / global:black_pos_in_stack | PROVEN | Next history entry read by Black Rayman replay. | ring index | 2:3fa8a |
| globals.json / global:BIG_MAP | STRONG | Runtime ModeNormal map descriptor passed to construct_MAP. | map descriptor | 2:321d8 |
| globals.json / global:BiosCheckSum | UNKNOWN | Uninterpreted world-header byte under the existing BiosCheckSum name. | raw byte | 2:3fa2b |
| globals.json / global:DrawBufferNormal | STRONG | ModeNormal screen drawing buffer reused for direct background decoding. | byte-buffer pointer | 2:3d138 |
| globals.json / global:EffetBufferNormal | STRONG | ModeNormal effect buffer reused for compressed background input or duplicate background storage. | byte-buffer pointer | 2:3d130 |
| globals.json / global:FondAutorise | PROVEN | Background-loading mode selecting disabled, allocated background, or screen/effect buffers. | mode byte | 2:3fa1a |
| globals.json / global:HeaderFilevignet | PROVEN | Executable-resident descriptors selecting encoded PCX blocks in VIGNET.DAT. | 12-byte archive descriptor array | 2:215c |
| globals.json / global:MAP_BLOCKS | PROVEN | Decoded ModeNormal level tile-pixel byte buffer. | byte-buffer pointer | 2:3d830 |
| globals.json / global:Mus_obj_id | PROVEN | Level-object index selected for type55 during INIT_OBJECTS. | object index | 2:3f896 |
| globals.json / global:P486 | PROVEN | Byte mode switch that selects the cropped background decode path when equal1. | mode byte | 2:3fa20 |
| globals.json / global:PLAN0 | PROVEN | Primary background pointer, republished to PLAN0BIT after Normal decoding. | byte-buffer pointer | 2:3d800 |
| globals.json / global:PLAN0BIT | PROVEN | Duplicate/planar background destination selected by allocate_PLAN0. | byte-buffer pointer | 2:3d824 |
| globals.json / global:PLAN1_BUFFER | PROVEN | ModeX runtime map-render buffer allocated during world load. | byte-buffer pointer | 2:3d838 |
| globals.json / global:PLAN2 | PROVEN | Allocated presentation PCX pixels returned by LoadPcxInVignet. | indexed-pixel pointer | 2:3d804 |
| globals.json / global:PLAN2BIT | PROVEN | Published presentation pixel pointer, aliasing PLAN2 after image load. | byte-buffer pointer | 2:3d828 |
| globals.json / global:Plan0NumPcx | PROVEN | World-local signed8 map from background slot to VIGNET resource index. | resource index array | 2:3ef48 |
| globals.json / global:RaymanExeCheckSum1 | UNKNOWN | Uninterpreted raw32 ALLFIX field read before each fixed atlas. | raw32 value | 2:3d070 |
| globals.json / global:RaymanExeCheckSum2 | UNKNOWN | Uninterpreted raw32 ALLFIX field read after each fixed atlas and trailing8-bit checksum. | raw32 value | 2:3d068 |
| globals.json / global:RaymanExeCheckSum3 | UNKNOWN | Uninterpreted raw32 ALLFIX field read after all fixed descriptors. | raw32 value | 2:3d06c |
| globals.json / global:RaymanExeSize | UNKNOWN | Uninterpreted raw32 ALLFIX field under the existing executable-size name. | raw32 value | 2:3d05c |
| globals.json / global:ScrollDiffOn | PROVEN | Differential scrolling switch used to choose background slot and mask construction. | boolean-like byte | 2:3fa1f |
| globals.json / global:ScrollDiffSprites | PROVEN | World descriptor for differential-scroll foreground sprites, orNULL. | obj_t pointer | 2:3e634 |
| globals.json / global:TailleMainMemFix | PROVEN | Requested fixed-resource arena capacity. | bytes | 2:3d898 |
| globals.json / global:TailleMainMemLevel | PROVEN | Requested level arena capacity. | bytes | 2:3d844 |
| globals.json / global:TailleMainMemSprite | PROVEN | Requested sprite-pixel arena capacity. | bytes | 2:3d848 |
| globals.json / global:TailleMainMemWorld | PROVEN | Requested world arena capacity. | bytes | 2:3d89c |
| globals.json / global:ToutSpriteAutorise | PROVEN | Override to load world descriptor payloads even when not marked always-used. | boolean-like byte | 2:3fa18 |
| globals.json / global:VideoBiosCheckSum | UNKNOWN | Second uninterpreted world-header byte under the existing VideoBiosCheckSum name. | raw byte | 2:3fa28 |
| globals.json / global:alternateBossSpeedFactor | PROVEN | Additive adjustment to the boss state-speed multiplier. | 1/16384 multiplier | 2:3e5b4 |
| globals.json / global:bigray | PROVEN | 132-byte template copied from the appended BRAY descriptor. | object template | 2:3e784 |
| globals.json / global:black_ray_obj_id | PROVEN | Level-object index selected for type204 during INIT_OBJECTS. | object index | 2:3f8bc |
| globals.json / global:block_add | PROVEN | Tile lookup table whose interpretation differs between video modes. | offset/pointer table | 2:34268 |
| globals.json / global:blocks_code | PROVEN | Decoded ModeX tile callback code buffer. | byte-buffer pointer | 2:3d7c4 |
| globals.json / global:bossEncounter | STRONG | Boss encounter/phase selector consumed by action scripts. | selector | 2:3fa6e |
| globals.json / global:bossSpeedFactor | PROVEN | Base boss state-speed multiplier. | 1/16384 multiplier | 2:3e5b0 |
| globals.json / global:bossXToReach | PROVEN | Horizontal target used by boss approach-speed computation. | world pixels | 2:3f850 |
| globals.json / global:bossYToReach | PROVEN | Vertical target used by boss approach-speed computation. | world pixels | 2:3f84a |
| globals.json / global:cb_ball_obj_id | PROVEN | Level-object index selected for type257 during INIT_OBJECTS. | object index | 2:3f814 |
| globals.json / global:clockobj | PROVEN | Fixed-resource template selected by the sixth ALLFIX tail index. | object template | 2:3ed44 |
| globals.json / global:corde_dark_obj_id | PROVEN | Level-object index selected for type246 during INIT_OBJECTS. | object index | 2:3f8ce |
| globals.json / global:currentBossAction | STRONG | Current action selector in boss scripts. | selector | 2:3fa6f |
| globals.json / global:currentBossActionIsOver | STRONG | Boss action-completion marker. | boolean-like byte | 2:3fa6c |
| globals.json / global:current_rvb | PROVEN | Current768-byte256-color palette used for display/palette effects. | RGB6 component bytes | 2:37988 |
| globals.json / global:cymbal_obj_id | PROVEN | Indices of type81 objects encountered during level initialization. | object-index array | 2:3e5cc |
| globals.json / global:div_obj | PROVEN | Fixed-resource template selected by the seventh ALLFIX tail index. | object template | 2:3e6ec |
| globals.json / global:gros_patai_block | PROVEN | ModeX tile pixel-record allocation. | 256-byte record pointer | 2:3d7d4 |
| globals.json / global:gros_patai_src | PROVEN | ModeX tile pixel-source lookup table. | 32-bit pointer table | 2:32fa8 |
| globals.json / global:language_infos | PROVEN | Executable-resident descriptors selecting the five encoded RAY.LNG language blocks. | 12-byte archive descriptor array | 2:24e0 |
| globals.json / global:language_txt | PROVEN | Pointers into the decoded mutable language block, separated in place. | char-pointer table | 2:32b48 |
| globals.json / global:laserSourceSprNumInAnim | STRONG | Space-Mama laser-source animation-layer selection, initialized to byte255. | animation layer or255 | 2:3fa6d |
| globals.json / global:last_plan1_palette | PROVEN | LEV palette index used to select current_rvb from rvb. | palette index | 2:3fa83 |
| globals.json / global:level_alw | PROVEN | Index list for objects whose type flag has flags0 bit0 set. | object-index list | 2:3cde8 |
| globals.json / global:loaded_eta | PROVEN | Runtime bank of main-state-to-substate-table pointers. | eta-bank pointer array | 2:3ce48 |
| globals.json / global:main_mem_fix | PROVEN | Arena descriptor for fixed-resource metadata, state banks and deformation scratch. | allocator arena | 2:3d874 |
| globals.json / global:main_mem_sprite | PROVEN | Arena descriptor used for fixed sprite-atlas pixels and caller-supplied world sprite resources. | allocator arena | 2:3d860 |
| globals.json / global:main_mem_world | PROVEN | Arena descriptor initialized for world resource metadata and backgrounds. | allocator arena | 2:3d880 |
| globals.json / global:mama_pirate_obj_id | PROVEN | Level-object index selected for type187 during INIT_OBJECTS. | object index | 2:3f80c |
| globals.json / global:mapobj | PROVEN | World-map object instances whose resources are bound from one fixed descriptor. | object array | 2:3d8cc |
| globals.json / global:moskitomama_droite_obj_id | PROVEN | Level-object index selected for type231 during INIT_OBJECTS. | object index | 2:3f8d8 |
| globals.json / global:moskitomama_gauche_obj_id | PROVEN | Level-object index selected for type232 during INIT_OBJECTS. | object index | 2:3f8c8 |
| globals.json / global:moskitosaxo_obj_id | PROVEN | Level-object index selected for type218 during INIT_OBJECTS. | object index | 2:3f8d6 |
| globals.json / global:mst_scroll_obj_id | PROVEN | Selected type147 object with hit_points0. | object index | 2:3f878 |
| globals.json / global:nb_blocks_plein | PROVEN | ModeNormal count of full tile-pixel records read from LEV. | tile count | 2:3d054 |
| globals.json / global:nb_cymbal_in_map | PROVEN | Number of type81 entries appended to cymbal_obj_id by initialization. | count | 2:3fa81 |
| globals.json / global:nb_des | PROVEN | Live descriptor count/end index, including fixed resources after world append. | descriptor count | 2:251c |
| globals.json / global:nb_fix_des | PROVEN | Fixed descriptor count/end index preserved across world loads. | descriptor count | 2:251e |
| globals.json / global:nb_fix_eta | PROVEN | Fixed eta-bank prefix count. | bank count | 2:3fa9a |
| globals.json / global:nb_loaded_eta | PROVEN | Loaded eta-bank count with loader-specific update rules. | bank count | 2:3fa9b |
| globals.json / global:nb_total_blocks | PROVEN | ModeNormal total tile-block count read from LEV. | tile count | 2:3d058 |
| globals.json / global:nb_wiz | PROVEN | Bonus-stage Ting object count accumulated during initialization. | Ting count | 2:3fa56 |
| globals.json / global:nb_wiz_save | PROVEN | Saved status-bar Ting count at bonus-stage initialization. | Ting count | 2:3d8ac |
| globals.json / global:new_sprite_deform | PROVEN | Runtime sprite-deformation scratch allocation. | byte-buffer pointer | 2:3d7bc |
| globals.json / global:no_fnd | PROVEN | Signed8 selected world background slot. | world background slot | 2:3fa22 |
| globals.json / global:path_has_changed | PROVEN | Path-prefix switch for selected data loaders. | boolean-like unsigned16 | 2:3f8f2 |
| globals.json / global:plan0_height | PROVEN | Published background height after decode/conversion. | pixels | 2:3f7e4 |
| globals.json / global:plan0_width | PROVEN | Published background width in the active representation. | pixels/storage columns | 2:3f7de |
| globals.json / global:plan0bit_length | PROVEN | Published total duplicated planar background byte span. | bytes | 2:3d808 |
| globals.json / global:plan0bit_nb_bytes | PROVEN | Published planar background row-stride parameter. | bytes | 2:3d7f4 |
| globals.json / global:plan2_height | PROVEN | Published presentation image height in signed16 storage. | pixels | 2:3f7e2 |
| globals.json / global:plan2_width | PROVEN | Published presentation image width. | pixels | 2:3d7ec |
| globals.json / global:plan2bit_length | PROVEN | Published presentation image pixel-area byte count. | bytes | 2:3d7f8 |
| globals.json / global:plan2bit_nb_bytes | PROVEN | Presentation display row-stride parameter set towidth/4. | bytes | 2:3d7fc |
| globals.json / global:prise_branchee | STRONG | Plug/power state used by the Joe sequence. | state | 2:3f796 |
| globals.json / global:raylittle | PROVEN | Small-Rayman object whose sprite resources are selected by the fourth ALLFIX tail index. | object template | 2:3e808 |
| globals.json / global:rideau_obj_id | PROVEN | Level-object index selected for type249 with initial substate2. | object index | 2:3f8cc |
| globals.json / global:rvb | PROVEN | Three raw level palettes loaded from the LEV header. | RGB component bytes | 2:37f88 |
| globals.json / global:rvb_pres | PROVEN | Palette copy preserved when a presentation image is loaded. | RGB6 component bytes | 2:37088 |
| globals.json / global:saveBossEncounter | STRONG | Saved boss encounter selector with255 initialization sentinel. | selector | 2:3fa69 |
| globals.json / global:sbar_obj_id | PROVEN | Level-object index selected for type11 during INIT_OBJECTS. | object index | 2:3f87e |
| globals.json / global:sko_phase | STRONG | Scorpion controller phase selector. | selector | 2:3f816 |
| globals.json / global:stosko_obj_id | PROVEN | Level-object index selected for type223 during INIT_OBJECTS. | object index | 2:3f8d0 |
| globals.json / global:t_world_info | STRONG | World-map stage records consulted for the current stage cage count. | stage record array | 2:4d5c |
| globals.json / global:type_zdc | PROVEN | Per-object-type packed collision-zone reference defaults. | packed collision-zone reference | 2:33ee |
| globals.json / global:wldobj | PROVEN | Resource-holder records sharing obj_t layout, separate from the live level.objects allocation. | resource descriptor array | 2:50d18 |
| globals.json / global:world_index | PROVEN | Current world-map stage-record index. | stage index | 2:3fa60 |
| load.json / function:load_world | STRONG | Load world resource descriptors, state banks and background selections after the fixed resources. | see parameter/result and side-effect contract | 1:34de0 |
| load.json / local:load_world:i:1 | STRONG | Observed local computation: i = 0;; Plan0NumPcx[i++] = -1;; while (i < 10); | declared C storage; exact operations retained |  |
| load.json / local:load_world:layer_table_size:1 | STRONG | Unsigned16 byte length of the next animation layer payload (including any unindexed bytes). | bytes |  |
| load.json / local:load_world:img_buffer_checksum:1 | STRONG | Modulo256 residual: start at stored encoded-byte sum, subtract encoded bytes before XOR; zero means valid. | mod256 byte sum |  |
| load.json / local:load_world:full_filename:1 | STRONG | Observed local computation: full_filename = (char *) temp_malloc(100);; strcpy(full_filename, "..\\");; full_filename[0] = '\0'; | declared C storage; exact operations retained |  |
| load.json / local:load_world:fp:1 | STRONG | Observed local computation: fp = fopen(full_filename, "rb");; if (fp == 0); fread(&background_width, 2, 1, fp); | declared C storage; exact operations retained |  |
| load.json / local:load_world:tick:1 | STRONG | Observed local computation: tick = perm_value_3;; while (tick == (*((volatile u8 *) 0x46c))); tick = *((volatile u8 *) 0x46c); | declared C storage; exact operations retained |  |
| load.json / local:load_world:perm_value_3:1 | STRONG | Compiler-steering temporary preserving the exact shown value/address dataflow; no separate gameplay entity. | declared C storage; exact operations retained |  |
| load.json / local:load_world:perm_address_2:1 | STRONG | Compiler-steering temporary preserving the exact shown value/address dataflow; no separate gameplay entity. | declared C storage; exact operations retained |  |
| load.json / local:load_world:error_buffer:1 | STRONG | Observed local computation: error_buffer = (char *) temp_malloc(200);; sprintf(error_buffer, "Can not open file %s (load_world).\n", full_filename);; FatalError(error_buffer); | declared C storage; exact operations retained |  |
| load.json / local:load_world:background_width:1 | STRONG | Observed local computation: fread(&background_width, 2, 1, fp);; PLAN0 = allocate_PLAN0(mem_world, background_width * 2, background_height);; PLAN0 = allocate_PLAN0(mem_world, background_width, background_height); | declared C storage; exact operations retained |  |
| load.json / local:load_world:des_index:1 | STRONG | Observed local computation: for (des_index = nb_fix_des; des_index < (nb_des + nb_fix_des); ++des_index); fread(&wldobj[des_index].x, 4, 1, fp);; wldobj[des_index].img_buffer = (u8 *) block_malloc(mem_sprite, wldobj[des_index].x); | declared C storage; exact operations retained |  |
| load.json / local:load_world:plan0_num_pcx_count:1 | STRONG | Observed local computation: fread(&plan0_num_pcx_count, 1, 1, fp);; for (i = 0; i < plan0_num_pcx_count; ++i) | declared C storage; exact operations retained |  |
| load.json / local:load_world:background_height:1 | STRONG | Observed local computation: fread(&background_height, 2, 1, fp);; PLAN0 = allocate_PLAN0(mem_world, background_width * 2, background_height);; PLAN0 = allocate_PLAN0(mem_world, background_width, background_height); | declared C storage; exact operations retained |  |
| load.json / local:load_world:pcx_number:1 | STRONG | Observed local computation: fread(&pcx_number, 1, 1, fp);; pcx_number ^= 0x15;; Plan0NumPcx[i] = pcx_number; | declared C storage; exact operations retained |  |
| load.json / local:load_world:sprite_is_always_used:1 | STRONG | Observed local computation: fread(&sprite_is_always_used, 1, 1, fp);; if (sprite_is_always_used \|\| ToutSpriteAutorise) | declared C storage; exact operations retained |  |
| load.json / local:load_world:image_offset:1 | STRONG | Observed local computation: image_offset = ftell(fp);; fseek(fp, image_offset, 0); | declared C storage; exact operations retained |  |
| load.json / local:load_world:retry_index:1 | STRONG | Observed local computation: for (retry_index = 0; retry_index < ((u32) wldobj[des_index].x); retry_index++); fread(wldobj[des_index].img_buffer + retry_index, 1, 1, fp); | declared C storage; exact operations retained |  |
| load.json / local:load_world:error_buffer:2 | STRONG | Observed local computation: error_buffer = (char *) temp_malloc(200);; sprintf(error_buffer, "Can not open file %s (load_world).\n", full_filename);; FatalError(error_buffer); | declared C storage; exact operations retained |  |
| load.json / local:load_world:layers_per_frame:1 | STRONG | Observed local computation: fread(&wldobj[des_index].animations[i].layers_per_frame, 2, 1, fp);; fread(&layers_per_frame, 2, 1, fp); | declared C storage; exact operations retained |  |
| load.json / local:load_world:frame_count:1 | STRONG | Observed local computation: fread(&frame_count, 2, 1, fp);; fseek(fp, frame_count * (sizeof(anim_frame_t)), 1); | declared C storage; exact operations retained |  |
| load.json / local:load_world:frames_ptr:1 | STRONG | Discarded producer pointer used only as sentinel: -1 skips frame rectangles; any other value requires frame_count*4 bytes. | serialized32 presence value |  |
| load.json / local:load_world:num_eta_groups:1 | STRONG | Observed local computation: fread(&num_eta_groups, 1, 1, fp);; loaded_eta[i] = (eta_t **) block_malloc(mem_world, num_eta_groups * (sizeof(eta_t *)));; for (j = 0; j < num_eta_groups; ++j) | declared C storage; exact operations retained |  |
| load.json / local:load_world:j:1 | STRONG | Observed local computation: for (j = 0; j < num_eta_groups; ++j); loaded_eta[i][j] = (eta_t *) block_malloc(mem_world, num_eta_entries * (sizeof(eta_t)));; fread(loaded_eta[i][j], sizeof(eta_t), num_eta_entries, fp); | declared C storage; exact operations retained |  |
| load.json / local:load_world:num_eta_entries:1 | STRONG | Observed local computation: fread(&num_eta_entries, 1, 1, fp);; loaded_eta[i][j] = (eta_t *) block_malloc(mem_world, num_eta_entries * (sizeof(eta_t)));; fread(loaded_eta[i][j], sizeof(eta_t), num_eta_entries, fp); | declared C storage; exact operations retained |  |
| load.json / local:load_world:mem_world:1 | STRONG | Observed parameter computation: PLAN1_BUFFER = block_malloc(mem_world, 71680);; PLAN0 = allocate_PLAN0(mem_world, background_width * 2, background_height);; PLAN0 = allocate_PLAN0(mem_world, background_width, background_height); | declared C storage; exact operations retained |  |
| load.json / local:load_world:mem_sprite:1 | STRONG | Observed parameter computation: wldobj[des_index].img_buffer = (u8 *) block_malloc(mem_sprite, wldobj[des_index].x); | declared C storage; exact operations retained |  |
| load.json / local:load_world:filename:1 | STRONG | Observed parameter computation: strcat(full_filename, filename); | declared C storage; exact operations retained |  |
| load.json / function:load_level | STRONG | Load map palettes, video-mode tile resources and serialized level instances; rebind indices to runtime pointers. | see parameter/result and side-effect contract | 1:35688 |
| load.json / local:load_level:world_names:1 | STRONG | Observed local computation: strcat(full_filename, world_names[num_world - 1]); | declared C storage; exact operations retained |  |
| load.json / local:load_level:full_filename:1 | STRONG | Observed local computation: full_filename = (char *) temp_malloc(100);; strcpy(full_filename, "..\\");; full_filename[0] = '\0'; | declared C storage; exact operations retained |  |
| load.json / local:load_level:error_buffer:1 | STRONG | Observed local computation: error_buffer = (char *) temp_malloc(200);; sprintf(error_buffer, "Can not open file %s (load_level).\n", full_filename);; FatalError(error_buffer); | declared C storage; exact operations retained |  |
| load.json / local:load_level:fp:1 | STRONG | Observed local computation: fp = fopen(full_filename, "rb");; if (fp == NULL); fread(&event_offset, 4, 1, fp); | declared C storage; exact operations retained |  |
| load.json / local:load_level:event_offset:1 | STRONG | Observed local computation: fread(&event_offset, 4, 1, fp);; fseek(fp, event_offset, 0); | declared C storage; exact operations retained |  |
| load.json / local:load_level:texture_data_size:1 | STRONG | Observed local computation: fread(&texture_data_size, 4, 1, fp);; MAP_BLOCKS = (u8 *) block_malloc(mem_level, texture_data_size);; fread(MAP_BLOCKS, texture_data_size, 1, fp); | declared C storage; exact operations retained |  |
| load.json / local:load_level:x_texture_count:1 | STRONG | Observed local computation: fread(&x_texture_count, 4, 1, fp);; if (x_texture_count != 0); gros_patai_block = (x_texture_local_t *) block_malloc(mem_level, x_texture_count * (sizeof(x_texture_local_t))); | declared C storage; exact operations retained |  |
| load.json / local:load_level:x_code_size:1 | STRONG | Observed local computation: fread(&x_code_size, 4, 1, fp);; if (x_code_size != 0); blocks_code = (u8 *) block_malloc(mem_level, x_code_size); | declared C storage; exact operations retained |  |
| load.json / local:load_level:texture_offset:1 | STRONG | Observed local computation: fread(&texture_offset, 4, 1, fp);; fseek(fp, texture_offset, 0); | declared C storage; exact operations retained |  |
| load.json / local:load_level:image_offset:1 | STRONG | Observed local computation: image_offset = ftell(fp);; fseek(fp, image_offset, 0);; image_offset = ftell(fp); | declared C storage; exact operations retained |  |
| load.json / local:load_level:i:1 | STRONG | Observed local computation: for (i = 0; i < 1200; i++); gros_patai_src[i] = ((u32) gros_patai_block) + gros_patai_src[i];; for (i = 0; 1200 > i; i++) | declared C storage; exact operations retained |  |
| load.json / local:load_level:retry_index:1 | STRONG | Observed local computation: for (retry_index = 0; retry_index < (x_texture_count * (sizeof(x_texture_local_t))); retry_index++); checksum -= ((u8 *) gros_patai_block)[retry_index];; ((u8 *) gros_patai_block)[retry_index] ^= 0x7d; | declared C storage; exact operations retained |  |
| load.json / local:load_level:background_static:1 | STRONG | u8 slot copied to signed8 no_fnd in ModeX or when differential scrolling is off; LOAD_FND indexes Plan0NumPcx[no_fnd]. | world background slot |  |
| load.json / local:load_level:checksum:1 | STRONG | Modulo256 residual: start at stored encoded-byte sum, subtract encoded bytes before XOR; zero means valid. | mod256 byte sum |  |
| load.json / local:load_level:background_scroll:1 | STRONG | u8 alternative slot copied to no_fnd for ModeNormal with ScrollDiffOn. | world background slot |  |
| load.json / local:load_level:cmd_count:1 | STRONG | Observed local computation: fread(&cmd_count, 2, 1, fp);; if (cmd_count != 0); level.objects[i].cmds = (u8 *) block_malloc(mem_level, cmd_count); | declared C storage; exact operations retained |  |
| load.json / local:load_level:label_count:1 | STRONG | Observed local computation: fread(&label_count, 2, 1, fp);; if (label_count != 0); level.objects[i].cmd_labels = (s16 *) block_malloc(mem_level, label_count * 2); | declared C storage; exact operations retained |  |
| load.json / local:load_level:mem_level:1 | STRONG | Observed parameter computation: mp.map = (map_tile_t *) block_malloc(mem_level, mp.length * (sizeof(map_tile_t)));; MaskScrollDiffSprites(mem_level);; gros_patai_block = (x_texture_local_t *) block_malloc(mem_level, x_texture_count * (sizeof(x_texture_local_t))); | declared C storage; exact operations retained |  |
| load.json / local:load_level:world_id:1 | UNKNOWN | No value-consuming use in DOS load_level; caller passes a sprite-arena pointer in EDX. Existing s32 declaration is retained; no purpose name is assigned. | unused |  |
| load.json / local:load_level:filename:1 | STRONG | Observed parameter computation: strcat(full_filename, filename); | declared C storage; exact operations retained |  |
| load.json / function:load_big_ray | STRONG | Append the big-Rayman sprite/animation descriptor and replace the loaded state-bank prefix. | see parameter/result and side-effect contract | 1:36078 |
| load.json / local:load_big_ray:fp:1 | STRONG | Observed local computation: fp = fopen(filename, "rb");; if (fp == 0); fread(&wldobj[index].x, 4, 1, fp); | declared C storage; exact operations retained |  |
| load.json / local:load_big_ray:filename:1 | STRONG | Observed local computation: filename = (char *) temp_malloc(100);; filename[0] = '\0';; strcpy(filename, "PCMAP"); | declared C storage; exact operations retained |  |
| load.json / local:load_big_ray:error_buffer:1 | STRONG | Observed local computation: error_buffer = (char *) temp_malloc(200);; sprintf(error_buffer, "Can not open file %s (load_big_ray).\n", filename);; FatalError(error_buffer); | declared C storage; exact operations retained |  |
| load.json / local:load_big_ray:index:1 | STRONG | wldobj descriptor index; ALLFIX tail reads reuse it to choose template resources. | descriptor index |  |
| load.json / local:load_big_ray:i:1 | STRONG | Observed local computation: for (i = 0; (i < wldobj[index].x) != 0; i++); wldobj[index].img_buffer[i] ^= 0x8f;; for (i = 0; i < nb_loaded_eta; i++) | declared C storage; exact operations retained |  |
| load.json / local:load_big_ray:j:1 | STRONG | Observed local computation: for (j = 0; j < groups; j++); loaded_eta[i][j] = (eta_t *) block_malloc(buffer, entries * (sizeof(eta_t)));; fread(loaded_eta[i][j], sizeof(eta_t), entries, fp); | declared C storage; exact operations retained |  |
| load.json / local:load_big_ray:anim_index:1 | STRONG | Observed local computation: for (anim_index = 0; anim_index < wldobj[index].anim_count; anim_index++); fread(&wldobj[index].animations[anim_index].layers_per_frame, 2, 1, fp);; fread(&wldobj[index].animations[anim_index].frames_count, 2, 1, fp); | declared C storage; exact operations retained |  |
| load.json / local:load_big_ray:groups:1 | STRONG | u8 number of main-state groups in one loaded eta bank. | count |  |
| load.json / local:load_big_ray:entries:1 | STRONG | u8 number of8-byte substate records in one main-state group. | count |  |
| load.json / local:load_big_ray:layer_table_size:1 | STRONG | Unsigned16 byte length of the next animation layer payload (including any unindexed bytes). | bytes |  |
| load.json / local:load_big_ray:perm_value_6:1 | STRONG | Compiler-steering temporary preserving the exact shown value/address dataflow; no separate gameplay entity. | declared C storage; exact operations retained |  |
| load.json / local:load_big_ray:buffer:1 | STRONG | Observed parameter computation: wldobj[index].img_buffer = (u8 *) block_malloc(buffer, wldobj[index].x);; wldobj[index].sprites = (sprite_t *) block_malloc(buffer, wldobj[index].nb_sprites * (sizeof(sprite_t)));; wldobj[index].animations = (anim_t *) block_malloc(buffer, wldobj[index].anim_count * (sizeof(anim_t))); | declared C storage; exact operations retained |  |
| load.json / function:LOAD_ALL_FIX | STRONG | Create the four arenas and load fixed resources, state banks and template bindings. | see parameter/result and side-effect contract | 1:364bc |
| load.json / local:LOAD_ALL_FIX:filename:1 | STRONG | Observed local computation: filename = (char *) temp_malloc(100);; strcpy(filename, "..\\");; filename[0] = '\0'; | declared C storage; exact operations retained |  |
| load.json / local:LOAD_ALL_FIX:error_buffer:1 | STRONG | Observed local computation: error_buffer = (char *) temp_malloc(200);; sprintf(error_buffer, "Can not open file %s (load_all_fix).\n", filename);; FatalError(error_buffer); | declared C storage; exact operations retained |  |
| load.json / local:LOAD_ALL_FIX:fp:1 | STRONG | Observed local computation: fp = fopen(filename, "rb");; if (fp == NULL); fread(&nb_loaded_eta, 1, 1, fp); | declared C storage; exact operations retained |  |
| load.json / local:LOAD_ALL_FIX:index:1 | STRONG | wldobj descriptor index; ALLFIX tail reads reuse it to choose template resources. | descriptor index |  |
| load.json / local:LOAD_ALL_FIX:i:1 | STRONG | Observed local computation: for (i = 0; i < num_eta_groups; i++); loaded_eta[index][i] = (eta_t *) block_malloc(&main_mem_fix, num_eta_entries * (sizeof(eta_t)));; fread(loaded_eta[index][i], sizeof(eta_t), num_eta_entries, fp); | declared C storage; exact operations retained |  |
| load.json / local:LOAD_ALL_FIX:image_offset:1 | STRONG | Observed local computation: image_offset = ftell(fp);; fseek(fp, image_offset, 0); | declared C storage; exact operations retained |  |
| load.json / local:LOAD_ALL_FIX:retry_index:1 | STRONG | Observed local computation: for (retry_index = 0; retry_index < ((u32) wldobj[index].x); retry_index++); fread(wldobj[index].img_buffer + retry_index, 1, 1, fp); | declared C storage; exact operations retained |  |
| load.json / local:LOAD_ALL_FIX:num_eta_groups:1 | STRONG | Observed local computation: fread(&num_eta_groups, 1, 1, fp);; loaded_eta[index] = (eta_t **) block_malloc(&main_mem_fix, num_eta_groups * (sizeof(eta_t *)));; for (i = 0; i < num_eta_groups; i++) | declared C storage; exact operations retained |  |
| load.json / local:LOAD_ALL_FIX:num_eta_entries:1 | STRONG | Observed local computation: fread(&num_eta_entries, 1, 1, fp);; loaded_eta[index][i] = (eta_t *) block_malloc(&main_mem_fix, num_eta_entries * (sizeof(eta_t)));; fread(loaded_eta[index][i], sizeof(eta_t), num_eta_entries, fp); | declared C storage; exact operations retained |  |
| load.json / local:LOAD_ALL_FIX:checksum_byte:1 | STRONG | Modulo256 residual: start at stored encoded-byte sum, subtract encoded bytes before XOR; zero means valid. | mod256 byte sum |  |
| load.json / local:LOAD_ALL_FIX:layer_table_size:1 | STRONG | Unsigned16 byte length of the next animation layer payload (including any unindexed bytes). | bytes |  |
| load.json / local:LOAD_ALL_FIX:atlas:1 | STRONG | Observed local computation: mapobj[i].img_buffer = atlas; | declared C storage; exact operations retained |  |
| load.json / function:LoadPcxInVignet | STRONG | Read and XOR-decode a VIGNET PCX entry, then allocate and expand its indexed pixels. | see parameter/result and side-effect contract | 1:36d68 |
| load.json / local:LoadPcxInVignet:pcx_buf:1 | STRONG | Observed local computation: pcx_buf = (u8 *) temp_malloc(pcx_data_size);; fread(pcx_buf, 1, pcx_data_size, fp);; end = pcx_buf + pcx_data_size; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxInVignet:path:1 | STRONG | Observed local computation: path = (char *) temp_malloc(100);; strcpy(path, "..\\");; path[0] = '\0'; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxInVignet:header:1 | STRONG | Observed local computation: pcx_header_local_t header;; fread(&header, 128, 1, fp);; checksum -= ((u8 *) (&header))[i]; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxInVignet:fp:1 | STRONG | Observed local computation: fp = fopen(path, "rb");; for (; (fp == 0) && (i < 10);); fp = fopen(path, "rb"); | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxInVignet:y:1 | STRONG | Observed local computation: for (y = 0; y < (*height); y++); pixels[(y * (*width)) + (x++)] = value;; pixels[(x++) + (y * (*width))] = value; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxInVignet:source_pos:1 | STRONG | Observed local computation: source_pos = 0;; perm_value_11 = pcx_buf[source_pos++];; perm_value_26 = pcx_buf[source_pos++]; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxInVignet:i:1 | STRONG | Observed local computation: i = 0;; for (; (fp == 0) && (i < 10);); i++; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxInVignet:retry_checksum:1 | STRONG | Modulo256 residual: start at stored encoded-byte sum, subtract encoded bytes before XOR; zero means valid. | mod256 byte sum |  |
| load.json / local:LoadPcxInVignet:pos:1 | STRONG | Observed local computation: for (pos = pcx_buf; pos < end; pos++); checksum -= *pos;; *pos ^= HeaderFilevignet[resource_id].xor_byte; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxInVignet:pcx_data_size:1 | STRONG | Encoded entry size minus128-byte PCX header; includes RLE data and terminal palette bytes. | bytes |  |
| load.json / local:LoadPcxInVignet:line_bytes:1 | STRONG | Decoded PCX bytes_per_line used as the expanded-byte target for each scanline. | decoded scanline bytes |  |
| load.json / local:LoadPcxInVignet:checksum:1 | STRONG | Modulo256 residual: start at stored encoded-byte sum, subtract encoded bytes before XOR; zero means valid. | mod256 byte sum |  |
| load.json / local:LoadPcxInVignet:end:1 | STRONG | Observed local computation: end = pcx_buf + pcx_data_size;; for (pos = pcx_buf; pos < end; pos++); end = pos + 16384; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxInVignet:x:1 | STRONG | Observed local computation: x = 0;; pixels[(y * (*width)) + (x++)] = value;; pixels[(x++) + (y * (*width))] = value; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxInVignet:error_buffer:1 | STRONG | Observed local computation: error_buffer = (char *) temp_malloc(200);; sprintf(error_buffer, "%s : Can not open (pcx).\n", path);; FatalError(error_buffer); | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxInVignet:run_length:1 | STRONG | Observed local computation: run_length = value & 0x3f;; while ((--run_length) != (-1)) | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxInVignet:value:1 | STRONG | Observed local computation: value = perm_value_11;; if (0xc0 == (value & 0xc0)); run_length = value & 0x3f; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxInVignet:pixels:1 | STRONG | Observed local computation: pixels = (u8 *) block_malloc(buffer, (*width) * (*height));; pixels = (u8 *) block_malloc(buffer, (*width) * (*height));; if ((pixels != 0) && (pcx_buf != 0)) | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxInVignet:perm_value_11:1 | STRONG | Compiler-steering temporary preserving the exact shown value/address dataflow; no separate gameplay entity. | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxInVignet:perm_value_26:1 | STRONG | Compiler-steering temporary preserving the exact shown value/address dataflow; no separate gameplay entity. | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxInVignet:buffer:1 | STRONG | Observed parameter computation: pixels = (u8 *) block_malloc(buffer, (*width) * (*height));; pixels = (u8 *) block_malloc(buffer, (*width) * (*height)); | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxInVignet:resource_id:1 | STRONG | Observed parameter computation: checksum = HeaderFilevignet[resource_id].checksum_byte;; pcx_data_size = HeaderFilevignet[resource_id].size - 128;; fseek(fp, HeaderFilevignet[resource_id].offset, 0); | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxInVignet:width:1 | STRONG | Observed parameter computation: *width = (header.max_x - header.min_x) + 1;; pixels = (u8 *) block_malloc(buffer, (*width) * (*height));; *width = (header.max_x - header.min_x) + 1; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxInVignet:height:1 | STRONG | Observed parameter computation: *height = (header.max_y - header.min_y) + 1;; pixels = (u8 *) block_malloc(buffer, (*width) * (*height));; *height = (header.max_y - header.min_y) + 1; | declared C storage; exact operations retained |  |
| load.json / function:LoadPcxPaletteInVignet | PROVEN | Load the final768 bytes of a VIGNET entry as a VGA palette. | see parameter/result and side-effect contract | 1:37280 |
| load.json / local:LoadPcxPaletteInVignet:fp:1 | PROVEN | Observed local computation: fp = fopen(path, "rb");; for (; (fp == NULL) && (tries < 10);); fp = fopen(path, "rb"); | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxPaletteInVignet:path:1 | PROVEN | Observed local computation: path = (char *) temp_malloc(100);; strcpy(path, "..\\");; path[0] = '\0'; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxPaletteInVignet:error_buffer:1 | PROVEN | Observed local computation: error_buffer = (char *) temp_malloc(200);; sprintf(error_buffer, "%s : Can not open (palette).\n", path);; FatalError(error_buffer); | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxPaletteInVignet:colors:1 | PROVEN | Observed local computation: colors = (u8 *) palette;; colors[i] ^= HeaderFilevignet[resource_id].xor_byte;; colors[i] /= 4; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxPaletteInVignet:i:1 | PROVEN | Observed local computation: for (i = 0; i < 768; ++i); colors[i] ^= HeaderFilevignet[resource_id].xor_byte;; colors[i] /= 4; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxPaletteInVignet:tries:1 | PROVEN | Observed local computation: tries = 0;; for (; (fp == NULL) && (tries < 10);); ++tries; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxPaletteInVignet:path_has_changed:1 | PROVEN | Function-scope extern declaration of global path_has_changed; it introduces no additional local storage. | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxPaletteInVignet:seconds:1 | UNKNOWN | Parameter name in the function-scope extern sleep prototype; no local object or value-consuming role in this function. | prototype parameter |  |
| load.json / local:LoadPcxPaletteInVignet:resource_id:1 | PROVEN | Observed parameter computation: fseek(fp, (HeaderFilevignet[resource_id].offset + HeaderFilevignet[resource_id].size) - 0x300, 0);; colors[i] ^= HeaderFilevignet[resource_id].xor_byte; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxPaletteInVignet:palette:1 | PROVEN | Observed parameter computation: sprintf(error_buffer, "%s : Can not open (palette).\n", path);; fread(palette, 0x300, 1, fp);; colors = (u8 *) palette; | declared C storage; exact operations retained |  |
| load.json / function:LoadPlan0InVignet | STRONG | Decode a VIGNET background into PLAN0 or the existing screen/effect buffers. | see parameter/result and side-effect contract | 1:373d0 |
| load.json / local:LoadPlan0InVignet:header_index:1 | STRONG | Observed local computation: for (header_index = 0; header_index < 128; header_index++); checksum -= ((u8 *) (&header))[header_index];; ((u8 *) (&header))[header_index] ^= HeaderFilevignet[resource_id].xor_byte; | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:destination:1 | STRONG | Observed local computation: destination = DrawBufferNormal;; destination = PLAN0;; destination = DrawBufferNormal; | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:run:1 | STRONG | Observed local computation: run = perm_value_28;; while ((--run) != (-1)); run = value & 0x3f; | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:end:1 | STRONG | Observed local computation: end = pos + 16384;; while (pos != end); | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:line_bytes:1 | STRONG | Decoded PCX bytes_per_line used as the expanded-byte target for each scanline. | decoded scanline bytes |  |
| load.json / local:LoadPlan0InVignet:x:1 | STRONG | Observed local computation: x = 0;; if (((x < width) && (output_y >= 0)) && (output_y < 200)); destination[(width * output_y) + x] = value; | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:truncated:1 | STRONG | Observed local computation: truncated = 0;; truncated = 1;; if ((truncated == 0) && (checksum != 0)) | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:pos:1 | STRONG | Observed local computation: for (i = 0, pos = compressed; i < data_size; i++, pos++); checksum -= *pos;; *pos ^= HeaderFilevignet[resource_id].xor_byte; | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:checksum:1 | STRONG | Modulo256 residual: start at stored encoded-byte sum, subtract encoded bytes before XOR; zero means valid. | mod256 byte sum |  |
| load.json / local:LoadPlan0InVignet:compressed:1 | STRONG | Observed local computation: compressed = EffetBufferNormal;; compressed = (u8 *) temp_malloc(data_size);; fread(compressed, 1, data_size, fp); | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:tries:1 | STRONG | Observed local computation: tries = 0;; for (; (fp == 0) && (tries < 10);); tries++; | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:header:1 | STRONG | Observed local computation: pcx_header_local_t header;; fread(&header, 128, 1, fp);; checksum -= ((u8 *) (&header))[header_index]; | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:width:1 | STRONG | Observed local computation: width = (header.max_x - header.min_x) + 1;; width = (header.max_x - header.min_x) + 1;; if (width > 320) | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:error_buffer:1 | STRONG | Observed local computation: error_buffer = (char *) temp_malloc(200);; sprintf(error_buffer, "%s : Can not open (background).\n", path);; FatalError(error_buffer); | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:data_size:1 | STRONG | Encoded entry size minus128-byte PCX header; includes RLE data and terminal palette bytes. | bytes |  |
| load.json / local:LoadPlan0InVignet:i:1 | STRONG | Observed local computation: for (i = 0, pos = compressed; i < data_size; i++, pos++); for (i = 0, pos = compressed; i < data_size; i++, pos++); for (i = 0, pos = compressed; i < data_size; i++, pos++) | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:source_pos:1 | STRONG | Observed local computation: source_pos = 0;; value = compressed[source_pos++];; value = compressed[source_pos++]; | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:value:1 | STRONG | Observed local computation: value = compressed[source_pos++];; if ((value & 0xc0) == 0xc0); *perm_address_25 = value & 0x3f; | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:fp:1 | STRONG | Observed local computation: fp = fopen(path, "rb");; for (; (fp == 0) && (tries < 10);); fp = fopen(path, "rb"); | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:path:1 | STRONG | Observed local computation: path = (char *) temp_malloc(100);; strcpy(path, "..\\");; path[0] = '\0'; | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:height:1 | STRONG | Observed local computation: height = (header.max_y - header.min_y) + 1;; height = (header.max_y - header.min_y) + 1;; if (height > 200) | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:y:1 | STRONG | Observed local computation: for (y = 0; y < (height + crop); y++); output_y = y - crop;; for (y = 0; y < height; y++) | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:crop:1 | STRONG | Observed local computation: crop = 0;; crop = height - 200;; crop = 0; | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:output_y:1 | STRONG | Observed local computation: output_y = y - crop;; if (((x < width) && (output_y >= 0)) && (output_y < 200)); destination[(width * output_y) + x] = value; | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:perm_value_4:1 | STRONG | Compiler-steering temporary preserving the exact shown value/address dataflow; no separate gameplay entity. | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:perm_value_28:1 | STRONG | Compiler-steering temporary preserving the exact shown value/address dataflow; no separate gameplay entity. | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:perm_address_25:1 | STRONG | Compiler-steering temporary preserving the exact shown value/address dataflow; no separate gameplay entity. | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:perm_value_37:1 | STRONG | Compiler-steering temporary preserving the exact shown value/address dataflow; no separate gameplay entity. | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:perm_value_39:1 | STRONG | Compiler-steering temporary preserving the exact shown value/address dataflow; no separate gameplay entity. | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan0InVignet:resource_id:1 | STRONG | Observed parameter computation: perm_value_4 = HeaderFilevignet[resource_id].checksum_byte;; data_size = HeaderFilevignet[resource_id].size - 128;; fseek(fp, HeaderFilevignet[resource_id].offset, 0); | declared C storage; exact operations retained |  |
| load.json / function:allocate_PLAN0 | PROVEN | Obtain primary and duplicate/planar background buffers. | see parameter/result and side-effect contract | 1:37bb0 |
| load.json / local:allocate_PLAN0:buffer1:1 | PROVEN | Observed local computation: buffer1 = DrawBufferNormal;; buffer1 = (u8 *) block_malloc(mem_world, width * height);; return buffer1; | declared C storage; exact operations retained |  |
| load.json / local:allocate_PLAN0:buffer2:1 | PROVEN | Observed local computation: buffer2 = EffetBufferNormal;; buffer2 = (u8 *) block_malloc(mem_world, ((2 * width) * height) + (8 * width));; PLAN0BIT = buffer2; | declared C storage; exact operations retained |  |
| load.json / local:allocate_PLAN0:mem_world:1 | PROVEN | Observed parameter computation: buffer1 = (u8 *) block_malloc(mem_world, width * height);; buffer2 = (u8 *) block_malloc(mem_world, ((2 * width) * height) + (8 * width)); | declared C storage; exact operations retained |  |
| load.json / local:allocate_PLAN0:width:1 | PROVEN | Observed parameter computation: buffer1 = (u8 *) block_malloc(mem_world, width * height);; buffer2 = (u8 *) block_malloc(mem_world, ((2 * width) * height) + (8 * width)); | declared C storage; exact operations retained |  |
| load.json / local:allocate_PLAN0:height:1 | PROVEN | Observed parameter computation: buffer1 = (u8 *) block_malloc(mem_world, width * height);; buffer2 = (u8 *) block_malloc(mem_world, ((2 * width) * height) + (8 * width)); | declared C storage; exact operations retained |  |
| load.json / function:PLAN0FND_to_bits_planes | STRONG | Rearrange indexed background pixels into duplicated four-plane storage with top-row extension. | see parameter/result and side-effect contract | 1:37c10 |
| load.json / local:PLAN0FND_to_bits_planes:x:1 | STRONG | Observed local computation: for (x = 0; x < plan0bit_nb_bytes; ++x); offset = ((x & 3) * (plan0bit_length >> 1)) + (x >> 2);; pixel = source[(y * width) + x]; | declared C storage; exact operations retained |  |
| load.json / local:PLAN0FND_to_bits_planes:y:1 | STRONG | Observed local computation: for (y = 0; y < height; ++y); offset += y * (plan0bit_nb_bytes >> 1);; pixel = source[(y * width) + x]; | declared C storage; exact operations retained |  |
| load.json / local:PLAN0FND_to_bits_planes:source:1 | STRONG | Observed local computation: source = buffer;; pixel = source[(y * width) + x];; buffer[offset + (plan0bit_nb_bytes >> 2)] = source[(y * width) + x]; | declared C storage; exact operations retained |  |
| load.json / local:PLAN0FND_to_bits_planes:offset:1 | STRONG | Observed local computation: offset = ((x & 3) * (plan0bit_length >> 1)) + (x >> 2);; offset += y * (plan0bit_nb_bytes >> 1);; buffer[offset] = pixel; | declared C storage; exact operations retained |  |
| load.json / local:PLAN0FND_to_bits_planes:pixel:1 | STRONG | Observed local computation: pixel = source[(y * width) + x];; buffer[offset] = pixel;; pixel = source[(y * width) + x]; | declared C storage; exact operations retained |  |
| load.json / local:PLAN0FND_to_bits_planes:perm_value_2:1 | STRONG | Compiler-steering temporary preserving the exact shown value/address dataflow; no separate gameplay entity. | declared C storage; exact operations retained |  |
| load.json / local:PLAN0FND_to_bits_planes:pixel:2 | STRONG | Observed local computation: pixel = source[(y * width) + x];; buffer[offset] = pixel;; pixel = source[(y * width) + x]; | declared C storage; exact operations retained |  |
| load.json / local:PLAN0FND_to_bits_planes:buffer:1 | STRONG | Observed parameter computation: source = buffer;; buffer = PLAN0BIT;; if (buffer == 0) | declared C storage; exact operations retained |  |
| load.json / local:PLAN0FND_to_bits_planes:width:1 | STRONG | Observed parameter computation: plan0_width = width;; pixel = source[(y * width) + x];; buffer[offset + (plan0bit_nb_bytes >> 2)] = source[(y * width) + x]; | declared C storage; exact operations retained |  |
| load.json / local:PLAN0FND_to_bits_planes:height:1 | STRONG | Observed parameter computation: plan0bit_length = plan0bit_nb_bytes * height;; plan0_height = height;; for (y = 0; y < height; ++y) | declared C storage; exact operations retained |  |
| load.json / function:LoadPlan2InVignet | PROVEN | Load a presentation image and palette, and publish its display geometry. | see parameter/result and side-effect contract | 1:37d3c |
| load.json / local:LoadPlan2InVignet:saved_resource_id:1 | PROVEN | Observed local computation: PLAN2 = LoadPcxInVignet(buffer, saved_resource_id, &width, &height);; LoadPcxPaletteInVignet(saved_resource_id, &current_rvb); | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan2InVignet:width:1 | PROVEN | Observed local computation: PLAN2 = LoadPcxInVignet(buffer, saved_resource_id, &width, &height);; plan2_width = width;; plan2bit_nb_bytes = width; | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan2InVignet:height:1 | PROVEN | Observed local computation: PLAN2 = LoadPcxInVignet(buffer, saved_resource_id, &width, &height);; plan2bit_length = plan2bit_nb_bytes * height;; plan2_height = height; | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan2InVignet:buffer:1 | PROVEN | Observed parameter computation: PLAN2 = LoadPcxInVignet(buffer, saved_resource_id, &width, &height); | declared C storage; exact operations retained |  |
| load.json / local:LoadPlan2InVignet:resource_id:1 | PROVEN | Observed parameter computation: s32 saved_resource_id = resource_id; | declared C storage; exact operations retained |  |
| load.json / function:GetStringTxt | PROVEN | Scan one comma-delimited language entry into scratch storage and return the following cursor. | see parameter/result and side-effect contract | 1:37dd8 |
| load.json / local:GetStringTxt:txt:1 | PROVEN | Observed parameter computation: while (((*txt) != ',') && ((*txt) != '*')); if ((*txt) >= ' '); *(out_buf++) = *(txt++); | declared C storage; exact operations retained |  |
| load.json / local:GetStringTxt:out_buf:1 | PROVEN | Observed parameter computation: *(out_buf++) = *(txt++);; *out_buf = '\0'; | declared C storage; exact operations retained |  |
| load.json / function:LoadLanguageTxt | STRONG | Decode one RAY.LNG block and build pointers into its mutable allocation. | see parameter/result and side-effect contract | 1:37e10 |
| load.json / local:LoadLanguageTxt:language_data:1 | STRONG | Observed local computation: language_data = (char *) block_malloc(mem, size);; if ((language_data == NULL) \|\| (temp_buffer == NULL)); end = ((u8 *) language_data) + size; | declared C storage; exact operations retained |  |
| load.json / local:LoadLanguageTxt:str_index:1 | STRONG | Observed local computation: str_index = 0;; language_txt[str_index++] = txt; | declared C storage; exact operations retained |  |
| load.json / local:LoadLanguageTxt:checksum:1 | STRONG | Modulo256 residual: start at stored encoded-byte sum, subtract encoded bytes before XOR; zero means valid. | mod256 byte sum |  |
| load.json / local:LoadLanguageTxt:parse_buffer:1 | STRONG | Points to400-byte temp_buffer; filtered scratch output is discarded after delimiter scanning. | scratch byte pointer |  |
| load.json / local:LoadLanguageTxt:temp_buffer:1 | STRONG | Observed local computation: temp_buffer = (char *) temp_malloc(400);; parse_buffer = temp_buffer;; if ((language_data == NULL) \|\| (temp_buffer == NULL)) | declared C storage; exact operations retained |  |
| load.json / local:LoadLanguageTxt:retry_checksum:1 | STRONG | Modulo256 residual: start at stored encoded-byte sum, subtract encoded bytes before XOR; zero means valid. | mod256 byte sum |  |
| load.json / local:LoadLanguageTxt:error_buffer:1 | STRONG | Observed local computation: error_buffer = (char *) temp_malloc(200);; sprintf(error_buffer, "%s : Can not open.\n", path);; FatalError(error_buffer); | declared C storage; exact operations retained |  |
| load.json / local:LoadLanguageTxt:pos:1 | STRONG | Observed local computation: for (pos = (u8 *) language_data; pos < end; pos++); checksum -= *pos;; *pos ^= language_infos[language_index].xor_byte; | declared C storage; exact operations retained |  |
| load.json / local:LoadLanguageTxt:txt:1 | STRONG | Start of the string assigned to language_txt; first block byte, then GetStringTxt return+2. | allocation byte pointer |  |
| load.json / local:LoadLanguageTxt:i:1 | STRONG | Observed local computation: for (i = 0, pos = (u8 *) language_data; i < size; i++, pos++); for (i = 0; i < size; i++); retry_checksum -= language_data[i]; | declared C storage; exact operations retained |  |
| load.json / local:LoadLanguageTxt:path:1 | STRONG | Observed local computation: path = (char *) temp_malloc(100);; strcpy(path, "RAY.LNG");; lng_file = fopen(path, "rb"); | declared C storage; exact operations retained |  |
| load.json / local:LoadLanguageTxt:next_txt:1 | STRONG | GetStringTxt scan cursor; each comma advances it just one byte, independently of the string-start+2 skip. | allocation byte pointer |  |
| load.json / local:LoadLanguageTxt:end:1 | STRONG | Observed local computation: end = ((u8 *) language_data) + size;; for (pos = (u8 *) language_data; pos < end; pos++) | declared C storage; exact operations retained |  |
| load.json / local:LoadLanguageTxt:lng_file:1 | STRONG | Observed local computation: lng_file = fopen(path, "rb");; if (lng_file == NULL); fseek(lng_file, language_infos[language_index].offset, 0); | declared C storage; exact operations retained |  |
| load.json / local:LoadLanguageTxt:size:1 | STRONG | Observed local computation: size = language_infos[language_index].size;; language_data = (char *) block_malloc(mem, size);; end = ((u8 *) language_data) + size; | declared C storage; exact operations retained |  |
| load.json / local:LoadLanguageTxt:mem:1 | STRONG | Observed parameter computation: language_data = (char *) block_malloc(mem, size); | declared C storage; exact operations retained |  |
| load.json / local:LoadLanguageTxt:language_index:1 | STRONG | Observed parameter computation: checksum = language_infos[language_index].checksum_byte;; size = language_infos[language_index].size;; fseek(lng_file, language_infos[language_index].offset, 0); | declared C storage; exact operations retained |  |
| load.json / function:LoadPcxOptions | STRONG | Decode a VIGNET PCX entry into a caller-provided pixel buffer. | see parameter/result and side-effect contract | 1:38038 |
| load.json / local:LoadPcxOptions:y:1 | STRONG | Observed local computation: for (y = 0; y < (*height); y++); buffer[(y * (*width)) + (x++)] = value;; buffer[(x++) + (y * (*width))] = value; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:checksum:1 | STRONG | Modulo256 residual: start at stored encoded-byte sum, subtract encoded bytes before XOR; zero means valid. | mod256 byte sum |  |
| load.json / local:LoadPcxOptions:run_length:1 | STRONG | Observed local computation: run_length = 0x3f & value;; while ((--run_length) != (-1)) | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:pcx_buf:1 | STRONG | Observed local computation: pcx_buf = (u8 *) temp_malloc(pcx_data_size);; fread(pcx_buf, 1, pcx_data_size, fp);; end = pcx_buf + pcx_data_size; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:source_pos:1 | STRONG | Observed local computation: source_pos = 0;; perm_value_15 = pcx_buf[source_pos++];; perm_value_29 = pcx_buf[source_pos++]; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:path:1 | STRONG | Observed local computation: path = (char *) temp_malloc(100);; strcpy(path, "..\\");; path[0] = '\0'; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:value:1 | STRONG | Observed local computation: value = perm_value_15;; if ((value & 0xc0) == 0xc0); run_length = 0x3f & value; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:pos:1 | STRONG | Observed local computation: for (pos = pcx_buf; pos < end; pos++); checksum -= *pos;; *pos ^= HeaderFilevignet[resource_id].xor_byte; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:x:1 | STRONG | Observed local computation: x = 0;; buffer[(y * (*width)) + (x++)] = value;; buffer[(x++) + (y * (*width))] = value; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:end:1 | STRONG | Observed local computation: end = pcx_buf + pcx_data_size;; for (pos = pcx_buf; pos < end; pos++); end = pos + 16384; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:fp:1 | STRONG | Observed local computation: fp = fopen(path, "rb");; if (fp == 0); fseek(fp, HeaderFilevignet[resource_id].offset, 0); | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:error_buffer:1 | STRONG | Observed local computation: error_buffer = (char *) temp_malloc(200);; sprintf(error_buffer, "%s : Can not open.\n", path);; FatalError(error_buffer); | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:header:1 | STRONG | Observed local computation: pcx_header_local_t header;; fread(&header, 128, 1, fp);; checksum -= ((u8 *) (&header))[i]; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:line_bytes:1 | STRONG | Decoded PCX bytes_per_line used as the expanded-byte target for each scanline. | decoded scanline bytes |  |
| load.json / local:LoadPcxOptions:pcx_data_size:1 | STRONG | Encoded entry size minus128-byte PCX header; includes RLE data and terminal palette bytes. | bytes |  |
| load.json / local:LoadPcxOptions:i:1 | STRONG | Observed local computation: for (i = 0; i < 128; i++); checksum -= ((u8 *) (&header))[i];; ((u8 *) (&header))[i] ^= HeaderFilevignet[resource_id].xor_byte; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:retry_checksum:1 | STRONG | Modulo256 residual: start at stored encoded-byte sum, subtract encoded bytes before XOR; zero means valid. | mod256 byte sum |  |
| load.json / local:LoadPcxOptions:perm_value_0:1 | STRONG | Compiler-steering temporary preserving the exact shown value/address dataflow; no separate gameplay entity. | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:perm_value_10:1 | STRONG | Compiler-steering temporary preserving the exact shown value/address dataflow; no separate gameplay entity. | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:perm_value_20:1 | STRONG | Compiler-steering temporary preserving the exact shown value/address dataflow; no separate gameplay entity. | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:perm_address_5:1 | STRONG | Compiler-steering temporary preserving the exact shown value/address dataflow; no separate gameplay entity. | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:perm_value_15:1 | STRONG | Compiler-steering temporary preserving the exact shown value/address dataflow; no separate gameplay entity. | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:perm_value_29:1 | STRONG | Compiler-steering temporary preserving the exact shown value/address dataflow; no separate gameplay entity. | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:buffer:1 | STRONG | Observed parameter computation: if (!((buffer != 0) && (pcx_buf != 0))); buffer[(y * (*width)) + (x++)] = value;; buffer[(x++) + (y * (*width))] = value; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:resource_id:1 | STRONG | Observed parameter computation: perm_value_0 = HeaderFilevignet[resource_id].checksum_byte;; pcx_data_size = HeaderFilevignet[resource_id].size - 128;; fseek(fp, HeaderFilevignet[resource_id].offset, 0); | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:width:1 | STRONG | Observed parameter computation: *width = (header.max_x - header.min_x) + 1;; *width = (header.max_x - header.min_x) + 1;; buffer[(y * (*width)) + (x++)] = value; | declared C storage; exact operations retained |  |
| load.json / local:LoadPcxOptions:height:1 | STRONG | Observed parameter computation: *height = (header.max_y - header.min_y) + 1;; *height = (header.max_y - header.min_y) + 1;; for (y = 0; y < (*height); y++) | declared C storage; exact operations retained |  |
| moteur.json / function:INIT_HORLOGES | PROVEN | Clear the 25-byte global phase-counter bank. | see parameter/result contract | 1:45480 |
| moteur.json / local:INIT_HORLOGES:i:1 | PROVEN | Clock-bank byte index 0..24. | clock index |  |
| moteur.json / function:horloges | PROVEN | Advance each clock once with period equal to its index, and add the byte argument to map_time. | see parameter/result contract | 1:45498 |
| moteur.json / local:horloges:i:1 | PROVEN | Clock-bank byte index and reset threshold; index zero always clears. | clock index |  |
| moteur.json / local:horloges:ticks:1 | PROVEN | Byte increment applied to map_time only, independently of the single clock-bank advance. | map ticks |  |
| moteur.json / function:init_allowed_time | PROVEN | Fill six 32-entry world rows of allowed_time with bonus-stage time limits and the -2 sentinel. | see parameter/result contract | 1:454ec |
| moteur.json / local:init_allowed_time:world:1 | PROVEN | World row index 1..6. | world index |  |
| moteur.json / local:init_allowed_time:index:1 | PROVEN | First slot of the current 32-entry world row. | table index |  |
| moteur.json / local:init_allowed_time:p:1 | PROVEN | Write cursor through the current world time-limit row. | pointer |  |
| moteur.json / function:fades | PROVEN | Start map-entry fade at map_time zero, start the death fade in a bounded dead_time interval, and advance an unfinished fade. | see parameter/result contract | 1:45ac8 |
| moteur.json / function:frapsol | PROVEN | Return the indexed expsin shake sample divided by 128 using the signed16 shift helper. | see parameter/result contract | 1:45b24 |
| moteur.json / local:frapsol:param_1:1 | PROVEN | Index into the expsin shake-sample table. | sample index |  |
| moteur.json / function:DO_SCREEN_TREMBLE | PROVEN | Add one scaled expsin sample to vertical camera position while advancing the shake index. | see parameter/result contract | 1:45b48 |
| moteur.json / local:DO_SCREEN_TREMBLE:unk_1:1 | PROVEN | Scaled signed vertical shake sample, or 255 before terminator handling. | pixels |  |
| moteur.json / function:DO_SCREEN_TREMBLE3 | PROVEN | Advance the third shake index and add its expsin2 sample to ymap until sample 255. | see parameter/result contract | 1:45b90 |
| moteur.json / local:DO_SCREEN_TREMBLE3:unk_1:1 | PROVEN | Signed expsin2 vertical shake sample or positive255 terminator. | pixels |  |
| moteur.json / function:DO_SCREEN_TREMBLE2 | PROVEN | Advance the second shake index and add its expsin2 sample to xmap until sample 255. | see parameter/result contract | 1:45bcc |
| moteur.json / local:DO_SCREEN_TREMBLE2:unk_1:1 | PROVEN | Signed expsin2 horizontal shake sample or positive255 terminator. | pixels |  |
| moteur.json / function:DO_SCROLL | PROVEN | Apply camera shake and requested pixel displacement, clamp xmap/ymap to scroll bounds, and recalculate Rayman screen position. | see parameter/result contract | 1:45c08 |
| moteur.json / local:DO_SCROLL:h_speed:1 | PROVEN | In/out horizontal camera displacement, corrected when a limit clamps. | pixels/call |  |
| moteur.json / local:DO_SCROLL:v_speed:1 | PROVEN | In/out vertical camera displacement; input255 means zero before processing. | pixels/call |  |
| moteur.json / function:allocateLandingSmoke | PROVEN | Activate at most two inactive type-208 objects at the input object foot anchor, using states 0/0 and 0/1. | see parameter/result contract | 1:45d78 |
| moteur.json / local:allocateLandingSmoke:i:1 | PROVEN | Side iteration value0 or2 selecting smoke substate and priority. | side selector |  |
| moteur.json / local:allocateLandingSmoke:j:1 | PROVEN | Level-object scan index for a reusable smoke object. | object index |  |
| moteur.json / local:allocateLandingSmoke:cur_obj:1 | PROVEN | Current candidate level object, selected only when type208 and inactive. | pointer |  |
| moteur.json / local:allocateLandingSmoke:nb_objs:1 | PROVEN | Snapshot of level.nb_objects used by the search. | objects |  |
| moteur.json / local:allocateLandingSmoke:pos:1 | PROVEN | Signed32 placement accumulator, first horizontal then vertical. | pixels |  |
| moteur.json / local:allocateLandingSmoke:offset_y:1 | PROVEN | Widened source object foot y offset loaded before its y coordinate. | pixels |  |
| moteur.json / local:allocateLandingSmoke:in_obj:1 | PROVEN | Object whose foot anchor positions both landing smoke objects. | pointer |  |
| moteur.json / function:instantSpeed | PROVEN | Convert a signed16 speed in sixteenths to this map_time phase integer displacement. | see parameter/result contract | 1:45e60 |
| moteur.json / local:instantSpeed:speed_wide:1 | PROVEN | Signed32 promotion of the signed16 speed, used by absolute value/sign. | sixteenths of pixel/call |  |
| moteur.json / local:instantSpeed:spd_shr:1 | PROVEN | Base signed16 helper quotient widened to32, then accumulated return displacement. | pixels/call |  |
| moteur.json / local:instantSpeed:spd_abs_max:1 | PROVEN | Absolute speed remainder masked to four bits. | sixteenths of pixel/call |  |
| moteur.json / local:instantSpeed:product:1 | PROVEN | Signed32 product of map_time and the nonnegative remainder, with DOS wraparound. | phase product |  |
| moteur.json / local:instantSpeed:remainder:1 | PROVEN | Current phase product minus one remainder, representing the preceding phase product. | phase product |  |
| moteur.json / local:instantSpeed:shifted_product:1 | PROVEN | Current signed32 product arithmetic-shifted right4. | pixels |  |
| moteur.json / local:instantSpeed:shifted_remainder:1 | PROVEN | Preceding signed32 product arithmetic-shifted right4. | pixels |  |
| moteur.json / local:instantSpeed:speed:1 | PROVEN | Signed16 requested speed in sixteenths converted for this global map_time phase. | sixteenths of pixel/call |  |
| moteur.json / function:SET_X_SPEED | PROVEN | Select state left/right speed on an animation-header-selected clock phase, otherwise store zero horizontal speed. | see parameter/result contract | 1:45ee4 |
| moteur.json / local:SET_X_SPEED:xspeed:1 | PROVEN | Selected signed state speed times the selected clock period, zero off-phase, optionally halved. | object horizontal speed units |  |
| moteur.json / local:SET_X_SPEED:horloge_index:1 | PROVEN | Period1..4 encoded by the animation header top two bits plus one. | clock index / calls |  |
| moteur.json / local:SET_X_SPEED:eta:1 | PROVEN | Current main/substate descriptor supplying left/right signed8 speeds. | pointer |  |
| moteur.json / local:SET_X_SPEED:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:make_active2 | PROVEN | Set active, the activation mark, and activation state zero unconditionally, optionally emitting a nova. | see parameter/result contract | 1:45f9c |
| moteur.json / local:make_active2:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / local:make_active2:do_nova:1 | PROVEN | Nonzero requests DO_NOVA after activation. | boolean |  |
| moteur.json / function:make_active | PROVEN | Activate a living object and mark it processed, optionally emitting a nova. | see parameter/result contract | 1:45fbc |
| moteur.json / local:make_active:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / local:make_active:do_nova:1 | PROVEN | Nonzero requests DO_NOVA after activation. | boolean |  |
| moteur.json / function:in_action_zone | PROVEN | Classify a type-dependent activation region using screen coordinates, level bounds and optional previous-active expansion. | see parameter/result contract | 1:45fe4 |
| moteur.json / local:in_action_zone:unk_1:1 | PROVEN | Acceptance marker for special-type or ordinary activation-region tests. | boolean |  |
| moteur.json / local:in_action_zone:zdiff_x:1 | PROVEN | Per-type horizontal region margin, enlarged by60 for previously active objects. | pixels |  |
| moteur.json / local:in_action_zone:zdiff_y:1 | PROVEN | Per-type vertical region margin, enlarged by60 for previously active objects. | pixels |  |
| moteur.json / local:in_action_zone:offset_bx:1 | PROVEN | Horizontal object anchor offset, overridden to128 for two laser types in the special branch. | pixels |  |
| moteur.json / local:in_action_zone:in_x:1 | PROVEN | Caller-supplied signed16 screen-relative x used by activation bounds. | pixels |  |
| moteur.json / local:in_action_zone:in_y:1 | PROVEN | Caller-supplied signed16 screen-relative y used by activation bounds. | pixels |  |
| moteur.json / local:in_action_zone:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / local:in_action_zone:param_4:1 | PROVEN | Previous-active marker enabling60-pixel hysteresis margins. | boolean |  |
| moteur.json / function:kill_obj | PROVEN | Move an object to (-32000,-32000), set activation state 1 and clear alive. | see parameter/result contract | 1:46310 |
| moteur.json / local:kill_obj:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:SET_ACTIVE_FLAG | PROVEN | Update activation, reactivation delay and linked-group reinitialization from action-zone membership and prior state. | see parameter/result contract | 1:46338 |
| moteur.json / local:SET_ACTIVE_FLAG:was_active:1 | PROVEN | Previous is_active value narrowed to a byte before deactivation/re-evaluation. | boolean storage |  |
| moteur.json / local:SET_ACTIVE_FLAG:id:1 | PROVEN | Circular link traversal cursor starting at the input object id. | object id |  |
| moteur.json / local:SET_ACTIVE_FLAG:cur_obj:1 | PROVEN | Current circular-link member being activated or reinitialised. | pointer |  |
| moteur.json / local:SET_ACTIVE_FLAG:active_flag:1 | PROVEN | Signed16 snapshot of the signed32 activation state used by outside-zone transitions. | activation state code |  |
| moteur.json / local:SET_ACTIVE_FLAG:dont_reinit_linked_objects:1 | PROVEN | Whether a traversed group member is not yet in state4, preventing whole-group reinitialisation. | boolean |  |
| moteur.json / local:SET_ACTIVE_FLAG:id:2 | PROVEN | Circular link traversal cursor starting at the input object id. | object id |  |
| moteur.json / local:SET_ACTIVE_FLAG:cur_obj:2 | PROVEN | Current circular-link member being activated or reinitialised. | pointer |  |
| moteur.json / local:SET_ACTIVE_FLAG:id:3 | PROVEN | Circular link traversal cursor starting at the input object id. | object id |  |
| moteur.json / local:SET_ACTIVE_FLAG:cur_obj:3 | PROVEN | Current circular-link member being activated or reinitialised. | pointer |  |
| moteur.json / local:SET_ACTIVE_FLAG:screen_x:1 | PROVEN | Screen-relative x passed to in_action_zone. | pixels |  |
| moteur.json / local:SET_ACTIVE_FLAG:screen_y:1 | PROVEN | Screen-relative y passed to in_action_zone. | pixels |  |
| moteur.json / local:SET_ACTIVE_FLAG:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:DO_PESANTEUR | PROVEN | Apply the current state high-nibble vertical-acceleration rule and return its rebound-compensation marker. | see parameter/result contract | 1:465d0 |
| moteur.json / local:DO_PESANTEUR:y_accel:1 | PROVEN | Generic signed vertical increment before optional multiplication by16. | pixels/call per update |  |
| moteur.json / local:DO_PESANTEUR:res:1 | PROVEN | Returned0/1 marker used by rebound helpers to add one after reflection; not all nonzero accelerations set it. | boolean marker |  |
| moteur.json / local:DO_PESANTEUR:anim_speed_div:1 | PROVEN | High nibble of current eta.anim_speed choosing the generic gravity rule. | selector0..15 |  |
| moteur.json / local:DO_PESANTEUR:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:freezeAnim | PROVEN | Place the animation frame at a capped supplied offset, with a distinct reverse-playback formula. | see parameter/result contract | 1:46734 |
| moteur.json / local:freezeAnim:count:1 | PROVEN | Animation last frame index, narrowed to signed16. | frame index |  |
| moteur.json / local:freezeAnim:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / local:freezeAnim:param_2:1 | PROVEN | Byte offset capped against the animation last frame before direction-dependent placement. | frames |  |
| moteur.json / function:DO_ANIM | PROVEN | Advance the selected clock-gated animation, apply reset requests, transition states at frame overflow, and update follow/changed bookkeeping. | see parameter/result contract | 1:467ac |
| moteur.json / local:DO_ANIM:new_frame:1 | PROVEN | Initial frame of the transitioned animation, zero or frames_count-1 by direction. | frame index |  |
| moteur.json / local:DO_ANIM:prev_anim_index:1 | PROVEN | Saved pre-update animation index. | animation index |  |
| moteur.json / local:DO_ANIM:prev_anim_frame:1 | PROVEN | Saved pre-update frame for follow-delta/changed calculations. | frame index |  |
| moteur.json / local:DO_ANIM:eta:1 | PROVEN | Current state descriptor, refreshed after the end transition. | pointer |  |
| moteur.json / local:DO_ANIM:prev_anim:1 | PROVEN | Saved pre-update animation descriptor. | pointer |  |
| moteur.json / local:DO_ANIM:anim_speed:1 | PROVEN | Low timing nibble selecting the animation clock; zero freezes progression. | clock index / nominal calls |  |
| moteur.json / local:DO_ANIM:anim:1 | PROVEN | Animation selected after reset/state transition. | pointer |  |
| moteur.json / local:DO_ANIM:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:prof_in_bloc | PROVEN | Return the object foot coordinate modulo the 16-pixel tile height. | see parameter/result contract | 1:46a38 |
| moteur.json / local:prof_in_bloc:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:do_boing | PROVEN | Settle a slow vertical bounce or apply tile-dependent rebound impulses on the period-2 clock. | see parameter/result contract | 1:46a4c |
| moteur.json / local:do_boing:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / local:do_boing:main_etat:1 | PROVEN | Main/substate selector for the settled bounce state. | state index |  |
| moteur.json / local:do_boing:sub_etat:1 | PROVEN | Main/substate selector for the settled bounce state. | state index |  |
| moteur.json / function:underSlope | PROVEN | Test bit 0 of the foot tile flags together with bit 6 of the upper tile flags. | see parameter/result contract | 1:46bc4 |
| moteur.json / local:underSlope:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:DO_STONEBOMB_REBOND | PROVEN | Apply type-47/type-69 tile-dependent horizontal/vertical rebound speeds and reset gravity counters to 0 and 2. | see parameter/result contract | 1:46bec |
| moteur.json / local:DO_STONEBOMB_REBOND:new_spd_x:1 | PROVEN | Type-dependent base horizontal rebound magnitude32 or48 for supported types. | sixteenths of pixel/call |  |
| moteur.json / local:DO_STONEBOMB_REBOND:new_spd_y:1 | PROVEN | Type-dependent base vertical rebound speed -4 or-6 for supported types. | pixels/call |  |
| moteur.json / local:DO_STONEBOMB_REBOND:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:DO_THROWN_BOMB_REBOND | PROVEN | Handle thrown-bomb impact, state-specific explosion/type delegation, slope impulses, rebound and position correction. | see parameter/result contract | 1:46cd8 |
| moteur.json / local:DO_THROWN_BOMB_REBOND:mul_45:1 | PROVEN | Slope impulse multiplier5 for 45-degree tiles. | dimensionless |  |
| moteur.json / local:DO_THROWN_BOMB_REBOND:btyp:1 | PROVEN | Selected foot/upper tile class for impact. | tile type |  |
| moteur.json / local:DO_THROWN_BOMB_REBOND:old_type:1 | PROVEN | Saved object type restored after temporary type47 delegation. | object type id |  |
| moteur.json / local:DO_THROWN_BOMB_REBOND:under:1 | PROVEN | Result of the two-tile underSlope flag conjunction. | boolean |  |
| moteur.json / local:DO_THROWN_BOMB_REBOND:mul_30:1 | PROVEN | Slope impulse multiplier3 for 30-degree tiles. | dimensionless |  |
| moteur.json / local:DO_THROWN_BOMB_REBOND:anim_speed:1 | PROVEN | Current state high nibble used to preserve gravity counter2 in modes10/11. | gravity selector |  |
| moteur.json / local:DO_THROWN_BOMB_REBOND:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / local:DO_THROWN_BOMB_REBOND:pesanteur:1 | PROVEN | Gravity-return marker enabling the extra +1 after vertical reflection. | boolean marker |  |
| moteur.json / local:DO_THROWN_BOMB_REBOND:a3:1 | PROVEN | Vertical reflection offset when positive; nonpositive selects fixed upward impulse. | vertical speed units |  |
| moteur.json / function:DO_FRUIT_REBOND | PROVEN | Apply fruit/seed horizontal damping, tile impulses, vertical rebound and floor correction. | see parameter/result contract | 1:46f00 |
| moteur.json / local:DO_FRUIT_REBOND:gravity:1 | PROVEN | Snapshot of the gravity-return rebound-compensation marker. | boolean marker |  |
| moteur.json / local:DO_FRUIT_REBOND:bounce:1 | PROVEN | Snapshot of the supplied vertical reflection offset; nonpositive selects fixed/type-dependent bounce. | vertical speed units |  |
| moteur.json / local:DO_FRUIT_REBOND:under:1 | PROVEN | Result of the two-tile underSlope flag conjunction. | boolean |  |
| moteur.json / local:DO_FRUIT_REBOND:btyp:1 | PROVEN | Tile class selected for rebound impulse. | tile type |  |
| moteur.json / local:DO_FRUIT_REBOND:anim_speed:1 | PROVEN | State high nibble used to retain counter2 for modes10/11. | gravity selector |  |
| moteur.json / local:DO_FRUIT_REBOND:accel_x:1 | PROVEN | Horizontal damping increment1 for seeds,5 otherwise. | horizontal speed units/update |  |
| moteur.json / local:DO_FRUIT_REBOND:speed_x_bnd:1 | PROVEN | Inclusive rest threshold1 for seeds,4 otherwise. | horizontal speed units |  |
| moteur.json / local:DO_FRUIT_REBOND:mul_45:1 | PROVEN | 45-degree impulse coefficient1 for seeds,5 otherwise. | dimensionless |  |
| moteur.json / local:DO_FRUIT_REBOND:mul_30:1 | PROVEN | 30-degree impulse numerator1 for seeds,3 otherwise. | dimensionless |  |
| moteur.json / local:DO_FRUIT_REBOND:div_30:1 | PROVEN | 30-degree impulse divisor2 for seeds,1 otherwise. | dimensionless |  |
| moteur.json / local:DO_FRUIT_REBOND:obj_type:1 | PROVEN | Object type snapshot selecting the fixed-bounce platform cases. | object type id |  |
| moteur.json / local:DO_FRUIT_REBOND:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / local:DO_FRUIT_REBOND:pesanteur:1 | PROVEN | Gravity-return marker used for rebound compensation. | boolean marker |  |
| moteur.json / local:DO_FRUIT_REBOND:a3:1 | PROVEN | Reflection offset, or nonpositive fixed-bounce selection. | vertical speed units |  |
| moteur.json / function:Drop_Atter | PROVEN | Put a landed drop in state 2/3 and stop both speeds. | see parameter/result contract | 1:4720c |
| moteur.json / local:Drop_Atter:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:BadGuyAtter | PROVEN | Snap a landed bad guy, choose its hurt/ordinary walking state, face Rayman and select the direction script label. | see parameter/result contract | 1:47230 |
| moteur.json / local:BadGuyAtter:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:MiteAtter | PROVEN | For a descending mite, snap to floor, select script label2 and clear follow_id when it exceeds500. | see parameter/result contract | 1:472c4 |
| moteur.json / local:MiteAtter:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:LidolPinkAtter | PROVEN | Bounce LidolPink in the gerbe/substate1 case, otherwise stop and snap applicable nonzero substates. | see parameter/result contract | 1:472f8 |
| moteur.json / local:LidolPinkAtter:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:stoneDogAtter | PROVEN | For stone-dog substate2, select script label4 and snap to the floor. | see parameter/result contract | 1:47344 |
| moteur.json / local:stoneDogAtter:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:stoneDogBounces | PROVEN | Probe tiles two pixels either side of the foot and alter stone-dog bounce script/direction on a slope edge. | see parameter/result contract | 1:4736c |
| moteur.json / local:stoneDogBounces:x:1 | PROVEN | Object horizontal foot anchor. | pixels |  |
| moteur.json / local:stoneDogBounces:y:1 | PROVEN | Object vertical foot anchor. | pixels |  |
| moteur.json / local:stoneDogBounces:block_l:1 | PROVEN | Tile type at x-2 at the foot row. | tile type |  |
| moteur.json / local:stoneDogBounces:block_r:1 | PROVEN | Tile type at x+2 at the foot row. | tile type |  |
| moteur.json / local:stoneDogBounces:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:Spider_Atter | PROVEN | Set the spider walking state 1/0 and choose script label1 or3 from facing. | see parameter/result contract | 1:474b8 |
| moteur.json / local:Spider_Atter:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:NormalAtter | PROVEN | Restore the saved initial main/substate and snap the object to the floor. | see parameter/result contract | 1:474f4 |
| moteur.json / local:NormalAtter:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:OBJ_IN_THE_AIR | PROVEN | Update gravity counters, apply acceleration, handle upward tile blocking, dispatch type-specific landing, and cap vertical speed. | see parameter/result contract | 1:47530 |
| moteur.json / local:OBJ_IN_THE_AIR:pesanteur:1 | PROVEN | DO_PESANTEUR return marker forwarded to selected rebound handlers. | boolean marker |  |
| moteur.json / local:OBJ_IN_THE_AIR:spr_w:1 | PROVEN | Follow-sprite rectangle width output; not otherwise consumed here. | pixels |  |
| moteur.json / local:OBJ_IN_THE_AIR:spr_x:1 | PROVEN | Top collision sample x, overwritten with the object foot x anchor after sprite query. | pixels |  |
| moteur.json / local:OBJ_IN_THE_AIR:spr_y:1 | PROVEN | Top collision sample y, derived from followed sprite or object origin plus offset_hy. | pixels |  |
| moteur.json / local:OBJ_IN_THE_AIR:spr_h:1 | PROVEN | Follow-sprite rectangle height output; not otherwise consumed here. | pixels |  |
| moteur.json / local:OBJ_IN_THE_AIR:anim_speed_div:1 | PROVEN | Current state high nibble selecting gravity-counter update policy. | gravity selector |  |
| moteur.json / local:OBJ_IN_THE_AIR:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:test_fall_in_water | PROVEN | Apply type/flag-dependent water-entry splash, velocity stop and offscreen sinking effects. | see parameter/result contract | 1:47fec |
| moteur.json / local:test_fall_in_water:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:MOVE_OBJECT | PROVEN | Integrate object speeds, optionally decay horizontal magnitude and turn at a wall, set movement bookkeeping and process water. | see parameter/result contract | 1:48084 |
| moteur.json / local:MOVE_OBJECT:type:1 | PROVEN | Widened object type used for per-type motion flag indexing. | object type id |  |
| moteur.json / local:MOVE_OBJECT:need_set_flag_0x100:1 | PROVEN | Movement-bookkeeping bit producer, true when ordinary computed displacement is nonzero. Candidate fist arm writes0; original B20 producer is uninitialised. | boolean storage |  |
| moteur.json / local:MOVE_OBJECT:dx:1 | PROVEN | Per-call integer horizontal movement after optional sixteenth conversion and wall reversal. | pixels/call |  |
| moteur.json / local:MOVE_OBJECT:dy:1 | PROVEN | Per-call integer vertical movement after optional sixteenth conversion. | pixels/call |  |
| moteur.json / local:MOVE_OBJECT:x1:1 | PROVEN | Pre-move horizontal foot anchor used for wall probing. | pixels |  |
| moteur.json / local:MOVE_OBJECT:x2:1 | PROVEN | Predicted horizontal foot anchor after dx. | pixels |  |
| moteur.json / local:MOVE_OBJECT:y:1 | PROVEN | Probe row eight pixels above the foot. | pixels |  |
| moteur.json / local:MOVE_OBJECT:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:DO_RAY_IN_ZONE | PROVEN | Call the object-type Rayman-in-zone callback when OBJ_IN_ZONE accepts the object. | see parameter/result contract | 1:48270 |
| moteur.json / local:DO_RAY_IN_ZONE:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:DoRaymanInZDDDefault | PROVEN | Default Rayman-in-detection-zone callback with no effects. | see parameter/result contract | 1:4829c |
| moteur.json / local:DoRaymanInZDDDefault:obj:1 | PROVEN | Unused callback object argument; no operation is performed. | pointer |  |
| moteur.json / function:DO_ONE_OBJECT | PROVEN | Run one object type update in tile/script/command/gravity/follow/platform/detection order. | see parameter/result contract | 1:482a0 |
| moteur.json / local:DO_ONE_OBJECT:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:build_active_table | PROVEN | Evaluate candidate level objects with linked activation marks and build the active-id list in reverse level_obj order. | see parameter/result contract | 1:48388 |
| moteur.json / local:build_active_table:min_x:1 | PROVEN | Recentered coarse x reference xmap+160 after initialization; despite its name this is used inside an absolute-distance test. | pixels |  |
| moteur.json / local:build_active_table:min_y:1 | PROVEN | Recentered coarse y reference ymap+100. | pixels |  |
| moteur.json / local:build_active_table:max_x:1 | PROVEN | Temporary xmap+1320 used to derive the coarse horizontal half extent. | pixels |  |
| moteur.json / local:build_active_table:max_y:1 | PROVEN | Temporary ymap+1200 used to derive the coarse vertical half extent. | pixels |  |
| moteur.json / local:build_active_table:half_x:1 | PROVEN | Coarse absolute-distance bound660. | pixels |  |
| moteur.json / local:build_active_table:half_y:1 | PROVEN | Coarse absolute-distance bound600. | pixels |  |
| moteur.json / local:build_active_table:i:1 | PROVEN | Candidate-list slot, forward for mark clearing and reverse for evaluation/output. | list index |  |
| moteur.json / local:build_active_table:added:1 | PROVEN | Number of active ids written so far; compared >100 after write. | objects |  |
| moteur.json / local:build_active_table:out:1 | PROVEN | Next active-list output slot; final slot receives -1. | pointer |  |
| moteur.json / local:build_active_table:out_start:1 | PROVEN | Saved first output slot for count calculation. | pointer |  |
| moteur.json / local:build_active_table:obj:1 | PROVEN | Level object selected by level_obj.obj_ids at the current slot. | pointer |  |
| moteur.json / function:Add_One_RAY_lives | PROVEN | Increment lives up to99 and restore Rayman health to four only when max_hitp is four, otherwise two. | see parameter/result contract | 1:48644 |
| moteur.json / function:DO_CLING_ANIMS | PROVEN | Advance the delayed extra-life and power pickup animations, retire them at EOA and award the life when allowed. | see parameter/result contract | 1:48680 |
| moteur.json / local:DO_CLING_ANIMS:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:DO_OBJECTS_ANIMS | PROVEN | Animate active-list entries except the recorded black Rayman/fist types204/205. | see parameter/result contract | 1:48754 |
| moteur.json / local:DO_OBJECTS_ANIMS:i:1 | PROVEN | Loop/list index in the exact traversal recorded by the function body. | index |  |
| moteur.json / local:DO_OBJECTS_ANIMS:cur_obj:1 | PROVEN | Current counted active-list object, or Rayman followed object after the initial loop. | pointer |  |
| moteur.json / function:DO_OBJECTS | PROVEN | Run cling/toon allocation and active-object dispatch with switch-off and hit-point-zero handling. | see parameter/result contract | 1:487c8 |
| moteur.json / local:DO_OBJECTS:i:1 | PROVEN | Loop/list index in the exact traversal recorded by the function body. | index |  |
| moteur.json / local:DO_OBJECTS:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:MOVE_OBJECTS | PROVEN | Apply MOVE_OBJECT to each counted active-list entry. | see parameter/result contract | 1:48908 |
| moteur.json / local:MOVE_OBJECTS:i:1 | PROVEN | Loop/list index in the exact traversal recorded by the function body. | index |  |
| moteur.json / local:MOVE_OBJECTS:cur_obj:1 | PROVEN | Current counted active-list object, or Rayman followed object after the initial loop. | pointer |  |
| moteur.json / function:RECALE_ALL_OBJECTS | PROVEN | Recalculate active-object screen positions and attach Rayman vertically to the active followed sprite. | see parameter/result contract | 1:48964 |
| moteur.json / local:RECALE_ALL_OBJECTS:x:1 | PROVEN | Followed-sprite left x output used by type254 tilt. | pixels |  |
| moteur.json / local:RECALE_ALL_OBJECTS:y:1 | PROVEN | Followed-sprite top y output used for Rayman foot placement. | pixels |  |
| moteur.json / local:RECALE_ALL_OBJECTS:w:1 | PROVEN | Followed-sprite width output used for its horizontal midpoint. | pixels |  |
| moteur.json / local:RECALE_ALL_OBJECTS:h:1 | PROVEN | Followed-sprite height output, not otherwise read here. | pixels |  |
| moteur.json / local:RECALE_ALL_OBJECTS:i:1 | PROVEN | Loop/list index in the exact traversal recorded by the function body. | index |  |
| moteur.json / local:RECALE_ALL_OBJECTS:cur_obj:1 | PROVEN | Current counted active-list object, or Rayman followed object after the initial loop. | pointer |  |
| moteur.json / local:RECALE_ALL_OBJECTS:dx:1 | PROVEN | Rayman foot x minus followed sprite midpoint, narrowed through signed16 before the tilt shift. | pixels |  |
| moteur.json / function:RayCoince | PROVEN | Probe a three-tile strip at the selected side of mounted Rayman when near the corresponding tile-grid boundary. | see parameter/result contract | 1:48aac |
| moteur.json / local:RayCoince:x_pos:1 | PROVEN | Mounted Rayman side-dependent x probe origin. | pixels |  |
| moteur.json / local:RayCoince:y_pos:1 | PROVEN | Mounted Rayman y+32 probe origin. | pixels |  |
| moteur.json / local:RayCoince:map_ind:1 | PROVEN | Row-major tile index, advanced horizontally or by mp.width for the three probes. | tile index |  |
| moteur.json / local:RayCoince:res:1 | PROVEN | Whether any selected probe has block_flags bit0. | boolean |  |
| moteur.json / local:RayCoince:dir:1 | PROVEN | Side selector0=left,1=right,2=up,3=down; other values return false. | direction selector |  |
| moteur.json / function:move_up_ray | PROVEN | Apply Rayman upward movement, stop at the scroll-start top bound and recalculate screen position. | see parameter/result contract | 1:48de8 |
| moteur.json / local:move_up_ray:unk_y_1:1 | PROVEN | Extra top-clamp margin40 for tiny/state-flag0x40, otherwise25. | pixels |  |
| moteur.json / function:move_down_ray | PROVEN | Apply downward movement and set vertical camera speed for the airborne screen-y threshold. | see parameter/result contract | 1:48ec8 |
| moteur.json / function:recale_ray_pos | PROVEN | Compute camera follow speeds from Rayman screen position, facing, inertia, motion and followed-object displacement. | see parameter/result contract | 1:48f70 |
| moteur.json / local:recale_ray_pos:unk_y_1:1 | PROVEN | Target Rayman screen y derived from half collision height, foot offset and114. | pixels |  |
| moteur.json / local:recale_ray_pos:unk_y_2:1 | PROVEN | Rayman screen y minus the target y. | pixels |  |
| moteur.json / local:recale_ray_pos:v_scr_temp:1 | PROVEN | Widened quarter vertical-error speed then clamped working value. | pixels/call |  |
| moteur.json / local:recale_ray_pos:spd_y_abs_1:1 | PROVEN | Absolute Rayman y speed, raised to at least3 for camera limit. | pixels/call |  |
| moteur.json / local:recale_ray_pos:spd_y_abs_2:1 | PROVEN | Signed16 snapshot of the vertical camera magnitude limit. | pixels/call |  |
| moteur.json / local:recale_ray_pos:unk_x_1:1 | PROVEN | Screen x target used while moving/facing right. | pixels |  |
| moteur.json / local:recale_ray_pos:unk_x_2:1 | PROVEN | Screen x target used while moving/facing left. | pixels |  |
| moteur.json / local:recale_ray_pos:unk_x_3:1 | PROVEN | Quarter horizontal target error which dhspeed approaches by one per call. | quarter-error units |  |
| moteur.json / local:recale_ray_pos:unk_x_3:2 | PROVEN | Quarter horizontal target error which dhspeed approaches by one per call. | quarter-error units |  |
| moteur.json / local:recale_ray_pos:unk_x_3:3 | PROVEN | Quarter horizontal target error which dhspeed approaches by one per call. | quarter-error units |  |
| moteur.json / local:recale_ray_pos:unk_x_3:4 | PROVEN | Quarter horizontal target error which dhspeed approaches by one per call. | quarter-error units |  |
| moteur.json / local:recale_ray_pos:other_obj:1 | PROVEN | Object currently followed by Rayman, contributing y speed and follow_y. | pointer |  |
| moteur.json / function:RAY_TO_THE_RIGHT | PROVEN | Apply Rayman rightward movement and clamp to RIGHT_MAP_BORDER, with mounted collision handling. | see parameter/result contract | 1:49418 |
| moteur.json / local:RAY_TO_THE_RIGHT:border:1 | PROVEN | Snapshot of the corresponding camera-relative Rayman x clamp. | pixels |  |
| moteur.json / function:RAY_TO_THE_LEFT | PROVEN | Apply Rayman leftward movement and ordinary-follow left-bound clamping, with mounted collision handling. | see parameter/result contract | 1:4949c |
| moteur.json / local:RAY_TO_THE_LEFT:border:1 | PROVEN | Snapshot of the corresponding camera-relative Rayman x clamp. | pixels |  |
| moteur.json / function:DO_FIXE | PROVEN | Derive HUD health slots and decimal lives/tings digits from live counters. | see parameter/result contract | 1:49518 |
| moteur.json / function:deactivate_ship_links | PROVEN | Clear alive on all other members of the ship circular link when the ship group marker is set. | see parameter/result contract | 1:495e8 |
| moteur.json / local:deactivate_ship_links:bateau_obj:1 | PROVEN | Ship object selected by bateau_obj_id. | pointer |  |
| moteur.json / local:deactivate_ship_links:linked:1 | PROVEN | Circular successor id excluding the ship itself. | object id |  |
| moteur.json / function:linkListHoldsAGendoor | PROVEN | Search other members of the object circular link for type164. | see parameter/result contract | 1:4965c |
| moteur.json / local:linkListHoldsAGendoor:res:1 | PROVEN | Whether another circular-list member has type164. | boolean |  |
| moteur.json / local:linkListHoldsAGendoor:linked:1 | PROVEN | Circular successor cursor, terminated by the input object id. | object id |  |
| moteur.json / local:linkListHoldsAGendoor:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:correct_gendoor_link | PROVEN | Detach generating doors from their circular groups, retain their next id in obj.link and optionally unlink dead-door groups. | see parameter/result contract | 1:496c0 |
| moteur.json / local:correct_gendoor_link:i:1 | PROVEN | Loop/list index in the exact traversal recorded by the function body. | index |  |
| moteur.json / local:correct_gendoor_link:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / local:correct_gendoor_link:count:1 | PROVEN | Snapshot of level.nb_objects. | objects |  |
| moteur.json / local:correct_gendoor_link:link:1 | PROVEN | Successor cursor while detaching the generating door. | object id |  |
| moteur.json / local:correct_gendoor_link:last_link:1 | PROVEN | Previous traversal id and predecessor when the cycle returns to the door. | object id |  |
| moteur.json / local:correct_gendoor_link:init_link:1 | PROVEN | Saved door obj.link cursor retained as the dead-group removal anchor. | object id |  |
| moteur.json / local:correct_gendoor_link:linked_obj_id:1 | PROVEN | Next member selected for suppression from the circular group. | object id |  |
| moteur.json / local:correct_gendoor_link:a1:1 | PROVEN | When nonzero, suppress all members of a dead generating door group after detachment. | boolean |  |
| moteur.json / function:suppressFromLinkList | PROVEN | Splice one object out of a circular successor list and self-link the removed object. | see parameter/result contract | 1:497fc |
| moteur.json / local:suppressFromLinkList:id:1 | PROVEN | Traversal cursor ending at the predecessor, initially the removed object id. | object id |  |
| moteur.json / local:suppressFromLinkList:linked_id:1 | PROVEN | Next successor id used to find the removed object predecessor. | object id |  |
| moteur.json / local:suppressFromLinkList:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:correct_link | PROVEN | Remove forbidden or door-required-without-door link members and set remaining group markers. | see parameter/result contract | 1:49888 |
| moteur.json / local:correct_link:i:1 | PROVEN | Loop/list index in the exact traversal recorded by the function body. | index |  |
| moteur.json / local:correct_link:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / function:INIT_RAY_BEGIN | PROVEN | Clear the listed Rayman ability/status bits and set initial HUD health/tings and continuation state. | see parameter/result contract | 1:49900 |
| moteur.json / function:INIT_RAY | PROVEN | Reinitialize the live Rayman object, abilities and camera, optionally locating a type99 start marker. | see parameter/result contract | 1:49948 |
| moteur.json / local:INIT_RAY:i:1 | PROVEN | Loop/list index in the exact traversal recorded by the function body. | index |  |
| moteur.json / local:INIT_RAY:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / local:INIT_RAY:i:2 | PROVEN | Loop/list index in the exact traversal recorded by the function body. | index |  |
| moteur.json / local:INIT_RAY:found:1 | PROVEN | Whether the whole-level search found a type99 start marker. | boolean |  |
| moteur.json / local:INIT_RAY:i:3 | PROVEN | Loop/list index in the exact traversal recorded by the function body. | index |  |
| moteur.json / local:INIT_RAY:new_lvl:1 | PROVEN | Nonzero clears the temporary super-helico ability during Rayman initialization. | boolean |  |
| moteur.json / function:is_icy_pente | PROVEN | Return whether the tile flags have both bit6 and bit3 set. | see parameter/result contract | 1:49cf4 |
| moteur.json / local:is_icy_pente:result:1 | PROVEN | Conjunction of block flag bit6 and bit3. | boolean |  |
| moteur.json / local:is_icy_pente:btyp:1 | PROVEN | Tile-type index into block_flags. | tile type |  |
| moteur.json / function:STOPPE_RAY_EN_XY | PROVEN | Look ahead by twice horizontal speed through up to four vertical tile samples and cancel blocked Rayman horizontal movement. | see parameter/result contract | 1:49d18 |
| moteur.json / local:STOPPE_RAY_EN_XY:x:1 | PROVEN | Look-ahead foot x after adding twice current horizontal speed. | pixels |  |
| moteur.json / local:STOPPE_RAY_EN_XY:y:1 | PROVEN | Foot y optionally raised8 for a slope tile. | pixels |  |
| moteur.json / local:STOPPE_RAY_EN_XY:btyp_1:1 | PROVEN | Look-ahead tile type8 pixels above sampled foot y. | tile type |  |
| moteur.json / local:STOPPE_RAY_EN_XY:btyp_2:1 | PROVEN | Look-ahead tile type24 pixels above sampled foot y. | tile type |  |
| moteur.json / local:STOPPE_RAY_EN_XY:btyp_3:1 | PROVEN | Look-ahead tile type40 pixels above sampled foot y. | tile type |  |
| moteur.json / local:STOPPE_RAY_EN_XY:btyp_4:1 | PROVEN | Look-ahead tile type56 pixels above sampled foot y. | tile type |  |
| moteur.json / local:STOPPE_RAY_EN_XY:stop:1 | PROVEN | Whether the applicable height samples require stopping x motion. | boolean |  |
| moteur.json / function:DO_PLACE_RAY | PROVEN | Move Rayman from direct directional input using RAY_MODE_SPEED and feed those speeds to the camera. | see parameter/result contract | 1:49e9c |
| moteur.json / function:DO_AUTO_SCROLL | PROVEN | Interpret type64 scroll-marker thresholds and hit_points modes to select forced camera movement. | see parameter/result contract | 1:49fb8 |
| moteur.json / local:DO_AUTO_SCROLL:i:1 | PROVEN | Loop/list index in the exact traversal recorded by the function body. | index |  |
| moteur.json / local:DO_AUTO_SCROLL:scr_obj:1 | PROVEN | Type64 scroll marker selected from scroll_obj_id. | pointer |  |
| moteur.json / local:DO_AUTO_SCROLL:scr_x:1 | PROVEN | Snapshot of the encoded horizontal scroll sum after zero/nonpositive adjustment. | scroll control code |  |
| moteur.json / local:DO_AUTO_SCROLL:ray_y:1 | PROVEN | Rayman world y narrowed to signed16 before threshold comparisons. | pixels |  |
| moteur.json / local:DO_AUTO_SCROLL:ray_x:1 | PROVEN | Rayman world x narrowed to signed16 before threshold comparisons. | pixels |  |
| moteur.json / function:INIT_MOTEUR | PROVEN | Reset level engine state, camera limits, weather/particle state, Rayman and objects, and restore the appropriate checkpoint branch. | see parameter/result contract | 1:4a1e0 |
| moteur.json / local:INIT_MOTEUR:i:1 | PROVEN | Loop/list index in the exact traversal recorded by the function body. | index |  |
| moteur.json / local:INIT_MOTEUR:j:1 | PROVEN | Loop/list index in the exact traversal recorded by the function body. | index |  |
| moteur.json / local:INIT_MOTEUR:cur_data:1 | PROVEN | Word-clear cursor over the 640-byte particle item storage. | pointer |  |
| moteur.json / local:INIT_MOTEUR:count:1 | PROVEN | Snapshot of level.nb_objects for mark clearing. | objects |  |
| moteur.json / local:INIT_MOTEUR:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / local:INIT_MOTEUR:saved_tings:1 | PROVEN | HUD tings count preserved across restoreGameState. | tings |  |
| moteur.json / local:INIT_MOTEUR:new_lvl:1 | PROVEN | New-level flag forwarded to INIT_RAY and INIT_OBJECTS. | boolean |  |
| moteur.json / function:INIT_MOTEUR_BEGIN | PROVEN | Initialize session-level limits, counters, Rayman defaults, demo selection and initial ordinary-follow camera modes. | see parameter/result contract | 1:4a5b4 |
| moteur.json / function:INIT_MOTEUR_WORLD | PROVEN | Clear new_world and set new_level for world entry. | see parameter/result contract | 1:4a6d8 |
| moteur.json / function:INIT_MOTEUR_LEVEL | PROVEN | Prepare displayed bounds, checkpoint/weather setup, engine initialization and the initial active table for a level. | see parameter/result contract | 1:4a6f4 |
| moteur.json / local:INIT_MOTEUR_LEVEL:need_save_game_state:1 | PROVEN | Whether this nonbonus/departlevel/noncontinue entry requires initializing and recording save1. | boolean |  |
| moteur.json / local:INIT_MOTEUR_LEVEL:a1:1 | PROVEN | Engine new-level argument forwarded with byte narrowing to INIT_MOTEUR. | new-level flag |  |
| moteur.json / function:restore_gendoor_link | PROVEN | Reconnect living generating doors or restore saved successor cycles for dead generating doors. | see parameter/result contract | 1:4a7dc |
| moteur.json / local:restore_gendoor_link:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / local:restore_gendoor_link:i:1 | PROVEN | Loop/list index in the exact traversal recorded by the function body. | index |  |
| moteur.json / local:restore_gendoor_link:count:1 | PROVEN | Snapshot of level.nb_objects. | objects |  |
| moteur.json / local:restore_gendoor_link:link:1 | PROVEN | Live/saved successor traversal cursor. | object id |  |
| moteur.json / local:restore_gendoor_link:prev:1 | PROVEN | Last visited live-group member used to insert the door before its saved successor. | object id |  |
| moteur.json / local:restore_gendoor_link:next:1 | PROVEN | Saved successor narrowed from u16 save1.link_init into u8 before writing live links. | object id, byte |  |
| moteur.json / function:DONE_MOTEUR_LEVEL | PROVEN | Restore generating-door links, finish save state when applicable, restore standard Rayman at transitions and align the level memory arena. | see parameter/result contract | 1:4a8e4 |
| moteur.json / function:INIT_MOTEUR_DEAD | PROVEN | At dead_time zero rebuild the engine and corrected door links, then save the secondary checkpoint when eligible. | see parameter/result contract | 1:4a9d8 |
| moteur.json / function:INIT_RAY_ON_MS | PROVEN | On NewMs, swap in the mounted Rayman object while preserving health-related fields and enter main state6/mode2. | see parameter/result contract | 1:4aa20 |
| moteur.json / function:DO_MOTEUR | PROVEN | Run demo abort input, cheats, one clock/map tick, camera/previous-state snapshots, wind selection and ordinary object updates. | see parameter/result contract | 1:4aae8 |
| moteur.json / local:DO_MOTEUR:has_joystick_input:1 | PROVEN | Raw byte read from joystick port0x201, held in signed32 for button-bit tests. | port bits |  |
| moteur.json / function:DO_MOTEUR2 | PROVEN | Run animation/player/movement/camera/active-list/HUD/weather/particle and tiny-mode projection work for the second engine phase. | see parameter/result contract | 1:4ac70 |
| moteur.json / local:DO_MOTEUR2:v20:1 | PROVEN | Active followed object whose display priority is saved and capped at3. | pointer |  |
| moteur.json / function:RAY_REVERSE_COMMANDS | PROVEN | Toggle reverse-control state and visibility/priorities of its two optional star objects. | see parameter/result contract | 1:4b0a4 |
| moteur.json / function:RAY_DEMIRAY | PROVEN | Toggle tiny Rayman by swapping object snapshots/templates and copying the live movement/state/health subset. | see parameter/result contract | 1:4b154 |
| moteur.json / function:Ray_RayEcrase | PROVEN | Toggle squashed Rayman when tiny and squashed agree, swapping templates and saving/restoring ability bits. | see parameter/result contract | 1:4b378 |
| moteur.json / function:DO_MOTEUR_GELE | PROVEN | Update the allowed frozen-engine subset and rebuild the active table even in freeze mode3. | see parameter/result contract | 1:4b668 |
| moteur.json / local:DO_MOTEUR_GELE:i:1 | PROVEN | Loop/list index in the exact traversal recorded by the function body. | index |  |
| moteur.json / local:DO_MOTEUR_GELE:obj:1 | PROVEN | Object record operated on or traversed by this helper. | pointer |  |
| moteur.json / global:Bloc_lim_H1 | STRONG | Vertical viewport/block bounds copied to display bounds at level initialization. | pixels | 2:3d0a4 |
| moteur.json / global:Bloc_lim_H1_Aff | STRONG | Displayed viewport/block bounds snapshot initialized from the corresponding live bound. | pixels | 2:3d088 |
| moteur.json / global:Bloc_lim_H2 | STRONG | Vertical viewport/block bounds copied to display bounds at level initialization. | pixels | 2:3d0a0 |
| moteur.json / global:Bloc_lim_H2_Aff | STRONG | Displayed viewport/block bounds snapshot initialized from the corresponding live bound. | pixels | 2:3d084 |
| moteur.json / global:Bloc_lim_W1 | STRONG | Horizontal viewport/block limits used to derive per-frame Rayman camera-relative boundaries. | pixels | 2:3d09c |
| moteur.json / global:Bloc_lim_W1_Aff | STRONG | Displayed viewport/block bounds snapshot initialized from the corresponding live bound. | pixels | 2:3d064 |
| moteur.json / global:Bloc_lim_W2 | STRONG | Horizontal viewport/block limits used to derive per-frame Rayman camera-relative boundaries. | pixels | 2:3d098 |
| moteur.json / global:Bloc_lim_W2_Aff | STRONG | Displayed viewport/block bounds snapshot initialized from the corresponding live bound. | pixels | 2:3d060 |
| moteur.json / global:DemoRecordMap | PROVEN | Parallel recorded-demo level/world selectors indexed by NumDemo. | world/level ids | 2:5e1c |
| moteur.json / global:DemoRecordWorld | PROVEN | Parallel recorded-demo level/world selectors indexed by NumDemo. | world/level ids | 2:5e10 |
| moteur.json / global:First_Hist | UNKNOWN | Session initialization writes1; menu code tests for2 and clears0 in a later branch. Intended role and a live writer of2 remain unresolved. | unknown state | 2:3fa41 |
| moteur.json / global:First_Menu | UNKNOWN | Session initialization writes1; no typed source reader or direct DOS fixup consumer was found. Intended role remains unresolved. | unknown | 2:3fa42 |
| moteur.json / global:GameModeVideo | STRONG | Video-mode selectors read by camera-limit initialization and per-frame player bounds. | mode code | 2:5e2d |
| moteur.json / global:IsBossThere | PROVEN | Latched boss-presence marker set when an accepted action-zone object has the boss type flag. | boolean marker | 2:3fa8e |
| moteur.json / global:LEFT_MAP_BORDER | PROVEN | Camera-relative horizontal player-origin limits, respectively right and left. | pixels | 2:3f756 |
| moteur.json / global:MapAvecPluieOuNeige | PROVEN | Whether weather particles/snow-sequence work should run for the current map. | boolean marker | 2:3f802 |
| moteur.json / global:ModeDemo | PROVEN | Demo-mode state used for magicseed retention and abort/transition handling. | state code | 2:3fa36 |
| moteur.json / global:ModeVideoActuel | STRONG | Video-mode selectors read by camera-limit initialization and per-frame player bounds. | mode code | 2:5e2c |
| moteur.json / global:NbTramesEcrase | PROVEN | Squashed Rayman shrink-delay countdown. | engine calls | 2:3fa97 |
| moteur.json / global:NewMs | STRONG | Pending switch to mounted Rayman mode. | boolean request | 2:3fa86 |
| moteur.json / global:NumDemo | PROVEN | Current recorded-demo selection index. | demo index | 2:3f76a |
| moteur.json / global:NumScrollObj | PROVEN | Count of level type64 auto-scroll marker ids. | objects | 2:3f87a |
| moteur.json / global:ObjectsFonctions | PROVEN | Object-type callback dispatch table with command/fist collision/Rayman collision/in-zone/turn slots. | function pointers by object type | 2:35fc |
| moteur.json / global:ProchainEclair | STRONG | Weather lightning scheduling control forced to1 during eligible frozen weather update. | weather sequence state | 2:3f742 |
| moteur.json / global:RAY_MODE_SPEED | PROVEN | Direct-placement cheat motion magnitude. | pixels/call | 2:3fa73 |
| moteur.json / global:RIGHT_MAP_BORDER | PROVEN | Camera-relative horizontal player-origin limits, respectively right and left. | pixels | 2:3f75a |
| moteur.json / global:RaymanDansUneMapDuJeu | STRONG | Marker that live Rayman is in a playable game map, enabling collision-tile initialization. | boolean | 2:3fa98 |
| moteur.json / global:RunTimeDemo | STRONG | Demo runtime allowance initialized to1800. | demo update calls | 2:3f768 |
| moteur.json / global:SauveRayEvts | PROVEN | Saved complete Rayman ability-bit record for squash restoration. | bitfield snapshot | 2:3f818 |
| moteur.json / global:SizeScreen | STRONG | Screen-size setting used with video mode to disable horizontal scrolling on narrow maps. | setting code | 2:5e6a |
| moteur.json / global:TextDark2_Affiche | STRONG | Final-boss text-display state cleared at engine initialization. | display state | 2:3fa89 |
| moteur.json / global:Vignet_To_Display | STRONG | Pending vignette/display selector cleared at engine initialization. | display selector | 2:3fa44 |
| moteur.json / global:You_Win | STRONG | Victory/credits progression flag cleared for a new session. | boolean | 2:3fa46 |
| moteur.json / global:allowed_time | PROVEN | Six32-entry world rows of timed-bonus limits; -2 denotes no timed bonus stage. | nominal seconds or -2 | 2:3b9c0 |
| moteur.json / global:bateau_obj_id | PROVEN | Ship object id or-1 when absent. | object id / -1 | 2:3f808 |
| moteur.json / global:bonus_map | PROVEN | Whether the current level has a timed-bonus limit. | boolean | 2:3f806 |
| moteur.json / global:boss_mort | STRONG | Distinct boss-death, boss-finish and game-finish controls. | sequence flags | 2:3fa78 |
| moteur.json / global:button_released | PROVEN | Rayman jump/helicopter press-release phase latch. | phase code | 2:3fa7d |
| moteur.json / global:compteur_attente | STRONG | Idle/wait animation counter reset during Rayman initialization. | update calls | 2:3f774 |
| moteur.json / global:current_pal_id | STRONG | Current palette-selection state used by palette actualization. | palette selector | 2:3fa82 |
| moteur.json / global:dans_la_map_monde | PROVEN | Worldmap-context flag influencing horizontal camera-follow smoothing. | boolean | 2:3fa53 |
| moteur.json / global:dead_time | PROVEN | Context-dependent byte death/drowning countdown shared with the fade/death lifecycle. | handler calls / byte countdown | 2:3fa7e |
| moteur.json / global:decalage_en_cours | PROVEN | Rayman horizontal momentum accumulator, converted through two four-bit stages to pixel motion. | 1/256 pixel before phase quantization | 2:3f866 |
| moteur.json / global:departlevel | PROVEN | Entry/return branch marker used by checkpoint, bonus and level-start initialization. | entry state | 2:3f8a0 |
| moteur.json / global:dhspeed | PROVEN | Signed16 horizontal camera-follow smoothing accumulator in quarter-error units. | camera error accumulator | 2:2520 |
| moteur.json / global:display_mode | STRONG | Display-mode control whose value adjusts the inclusive death-fade upper threshold. | mode code | 2:3f7e8 |
| moteur.json / global:dontdoit | STRONG | Worldmap end-sequence one-shot latch. | boolean marker | 2:3fa43 |
| moteur.json / global:dvspeed | UNKNOWN | Signed16 vertical camera state slot cleared at engine initialization and on vertical camera clamp. | unknown | 2:2522 |
| moteur.json / global:eau_obj_id | PROVEN | Water object id or-1 when absent. | object id / -1 | 2:3f882 |
| moteur.json / global:expsin | PROVEN | Signed16 sampled vertical-shake table, divided by128 by frapsol. | 1/128 pixel samples | 2:4c14 |
| moteur.json / global:expsin2 | PROVEN | Signed16 unscaled screen-shake sample table shared by horizontal and vertical shake indices. | pixels or255 terminator | 2:4c92 |
| moteur.json / global:fade | STRONG | Fade controller byte carrying phase and a bit0x40 completion/inactivity condition. | fade state | 2:3fa87 |
| moteur.json / global:fee_obj_id | STRONG | Fairy object id or-1 when absent. | object id / -1 | 2:3f888 |
| moteur.json / global:fin_boss | STRONG | Distinct boss-death, boss-finish and game-finish controls. | sequence flags | 2:3fa76 |
| moteur.json / global:fin_continue | STRONG | Continuation/entry flag gating engine reset, checkpoint writes and start-marker player hiding. | boolean marker | 2:3fa5c |
| moteur.json / global:fin_dark | STRONG | Final-boss sequence state cleared during Rayman initialization. | sequence state | 2:3fa3c |
| moteur.json / global:fin_de_rayman | PROVEN | Main game/application exit flag controlling the outer PcMain loop. | boolean | 2:3fa74 |
| moteur.json / global:fin_du_jeu | STRONG | Distinct boss-death, boss-finish and game-finish controls. | sequence flags | 2:3fa7a |
| moteur.json / global:gele | PROVEN | Engine freeze-mode control; value2 advances a restricted timed subset and3 skips that subset. | mode code | 2:3fa62 |
| moteur.json / global:gerbe | STRONG | Global event marker that changes cage/LidolPink bounce and landing handling. | boolean marker | 2:3fa70 |
| moteur.json / global:h_scroll_speed | PROVEN | Requested horizontal camera displacement for the current engine phase. | pixels/call | 2:3f8b2 |
| moteur.json / global:helico_time | PROVEN | Helicopter duration/impulse countdown advanced by the airborne handler. | airborne update calls | 2:3f898 |
| moteur.json / global:id_Cling_1up | PROVEN | Current extra-life/power pickup animation object ids, respectively, or-1 when absent. | object id / -1 | 2:3f82c |
| moteur.json / global:id_Cling_Pow | PROVEN | Current extra-life/power pickup animation object ids, respectively, or-1 when absent. | object id / -1 | 2:3f846 |
| moteur.json / global:in_air_because_hit | PROVEN | Marker that airborne gravity must not be suppressed by holding jump after a hit. | boolean | 2:3fa8c |
| moteur.json / global:in_pause | STRONG | Pause control cleared during engine initialization. | boolean | 2:3fa52 |
| moteur.json / global:joe_exp_probleme | STRONG | Joe interaction progression latch initialized from helped_joe_2. | boolean marker | 2:3f786 |
| moteur.json / global:jump_time | PROVEN | Number of airborne handler calls since the last jump/fall/hit reset. | airborne update calls | 2:3f8a4 |
| moteur.json / global:left_time | PROVEN | Timed-bonus remaining tick counter, with-2 denoting no timed limit. | nominal60-Hz ticks / -2 | 2:3d8c4 |
| moteur.json / global:lidol_source_obj | PROVEN | Object source for a deferred allocate_toons call. | pointer | 2:3d8c8 |
| moteur.json / global:lidol_to_allocate | PROVEN | Pending deferred toon-allocation request flag. | boolean marker | 2:3fa66 |
| moteur.json / global:life_becoz_wiz | STRONG | Marker that the current tings progression awarded an extra life. | boolean | 2:3fa99 |
| moteur.json / global:link_init | PROVEN | Live circular-successor array indexed by object id; a singleton points to itself. | object ids | 2:3ee58 |
| moteur.json / global:loop_time | STRONG | Bonus/postbonus sequence counter with both negative preparation and positive display phases. | sequence calls | 2:3f810 |
| moteur.json / global:main_mem_level | PROVEN | Level allocation arena with base pointer, bump offset and total capacity. | arena record | 2:3d84c |
| moteur.json / global:map_time | PROVEN | Signed32 global map-time counter, advanced by the byte horloges argument and also used as the common fractional movement phase. | nominal map ticks | 2:3d8c0 |
| moteur.json / global:nb_continue | STRONG | Remaining continues counter. | continues | 2:3fa3b |
| moteur.json / global:nb_wiz_collected | PROVEN | Bonus tings count and pending transfer count consumed into the HUD after leaving the bonus map. | tings | 2:3fa55 |
| moteur.json / global:new_level | PROVEN | Pending level/world transition flags. | boolean flags | 2:3f880 |
| moteur.json / global:new_world | PROVEN | Pending level/world transition flags. | boolean flags | 2:3f892 |
| moteur.json / global:no_ray_landing_smoke | STRONG | Flag suppressing the player landing-smoke path. | boolean | 2:3fa79 |
| moteur.json / global:num_level | PROVEN | Current one-based world/level selector used by engine exceptions, time tables and spawn rules. | world/level index | 2:3f87c |
| moteur.json / global:num_level_choice | STRONG | Selected world/level identifiers used by deferred entry/ability decisions. | world/level index | 2:3f82e |
| moteur.json / global:num_world_choice | STRONG | Selected world/level identifiers used by deferred entry/ability decisions. | world/level index | 2:3f83a |
| moteur.json / global:numero_palette_special | STRONG | Special weather palette selection state cleared during frozen weather update. | palette state | 2:3f740 |
| moteur.json / global:oldPrio | PROVEN | Saved original display priority of Rayman followed object. | display priority | 2:54da6 |
| moteur.json / global:ot | PROVEN | Current object-type dispatch cursor shared by engine helper calls. | object type id | 2:54da4 |
| moteur.json / global:pal_id_at_wizard | STRONG | Saved palette selector for the wizard/bonus return branch. | palette selector | 2:3fa84 |
| moteur.json / global:pierreAcorde_obj_id | STRONG | Object id of the rope/stone counterpart used by action-zone special cases. | object id | 2:3f88e |
| moteur.json / global:pix_gerbe | PROVEN | Eight particle batches with word-clear aliases and per-batch signed16 active markers. | particle records | 2:39a80 |
| moteur.json / global:pixels_enfonce | STRONG | Skops floor/camera sinking progress in pixels, reset at engine initialization and at scripted thresholds. | pixels | 2:3eef4 |
| moteur.json / global:poing_obj | PROVEN | Pointer to the live fist object. | pointer | 2:3e6dc |
| moteur.json / global:pos_stack | PROVEN | Ten-entry shift history of Rayman recent horizontal positions. | pixels, narrowed s16 | 2:3e5b8 |
| moteur.json / global:rayOnMsWasThere | UNKNOWN | Marker tested by the absent-mounted-template fallback in INIT_RAY_ON_MS. | unknown | 2:3fa49 |
| moteur.json / global:ray_Suphelico_bis | STRONG | Special super-helico state marker exempting one airborne substate from the ordinary airborne camera rule. | boolean marker | 2:3fa75 |
| moteur.json / global:ray_X_main_pos_init | STRONG | Saved Rayman world origin used on the wizard/bonus return branch. | pixels, s16 | 2:3f8aa |
| moteur.json / global:ray_Y_main_pos_init | STRONG | Saved Rayman world origin used on the wizard/bonus return branch. | pixels, s16 | 2:3f8a2 |
| moteur.json / global:ray_in_fee_zone | STRONG | Rayman fairy-zone interaction marker. | boolean | 2:3fa77 |
| moteur.json / global:ray_old_main_etat | PROVEN | Rayman state snapshots captured before the current DO_OBJECTS/DO_ANIM phase. | state index | 2:3f8c4 |
| moteur.json / global:ray_old_sub_etat | PROVEN | Rayman state snapshots captured before the current DO_OBJECTS/DO_ANIM phase. | state index | 2:3f8d4 |
| moteur.json / global:ray_pos_in_stack | PROVEN | Next Rayman replay-history ring write index. | ring index | 2:3fa8b |
| moteur.json / global:ray_se_noie | STRONG | Rayman drowning-state marker. | boolean marker | 2:3fa7b |
| moteur.json / global:ray_stack_is_full | PROVEN | Latch indicating the101-entry Rayman replay ring has wrapped. | boolean | 2:3fa88 |
| moteur.json / global:ray_wind_force | STRONG | Horizontal wind contribution selected for Rayman for the current engine phase. | player horizontal force units | 2:3f870 |
| moteur.json / global:ray_zdc_y | PROVEN | Cached Rayman collision-box height span, despite the y suffix. | pixels | 2:3f876 |
| moteur.json / global:rayman_obj_id | STRONG | Mounted/alternate Rayman object template id or-1 when unavailable. | object id / -1 | 2:3f894 |
| moteur.json / global:raytmp | PROVEN | Temporary full Rayman object snapshot used during object-template swaps. | object record | 2:3ee5c |
| moteur.json / global:record | PROVEN | Global demo-input stream descriptor and recording/playback controls. | record descriptor | 2:3af50 |
| moteur.json / global:reduced_rayman_id | STRONG | Level object-template id for tiny/squashed Rayman swaps. | object id | 2:3f886 |
| moteur.json / global:remoteRayXToReach | STRONG | Remote horizontal target used by player scripted movement, with-32000 for no target. | pixels / -32000 | 2:3f86a |
| moteur.json / global:rms | PROVEN | Saved alternate/full-size or mounted Rayman object snapshot used by template swaps. | object record | 2:3edd4 |
| moteur.json / global:rubis_list_calculated | STRONG | Whether Skops ruby-object setup/list preparation has run. | boolean | 2:3f8ec |
| moteur.json / global:save1 | PROVEN | Primary checkpoint image used during normal entry and generating-door link restoration. | checkpoint record | 2:3e88c |
| moteur.json / global:save2 | STRONG | Secondary checkpoint snapshot recorded by the death-reinitialization path. | checkpoint record | 2:3eae8 |
| moteur.json / global:save_save | STRONG | Saved primary-checkpoint valid marker for the wizard return path. | boolean marker | 2:3fa85 |
| moteur.json / global:screen_trembling | PROVEN | Index into the scaled vertical-shake table; zero disables its update. | sample index | 2:3f852 |
| moteur.json / global:screen_trembling2 | PROVEN | Index into expsin2 for horizontal camera shake. | sample index | 2:3f84e |
| moteur.json / global:screen_trembling3 | PROVEN | Index into expsin2 for the additional vertical camera shake. | sample index | 2:3f84c |
| moteur.json / global:scrollLocked | STRONG | Camera/boss-region lock marker read by activation and background/camera code. | boolean | 2:3fa68 |
| moteur.json / global:scroll_end_x | PROVEN | Current lower/upper horizontal camera-origin bound, respectively. | pixels | 2:3f872 |
| moteur.json / global:scroll_end_y | PROVEN | Current lower/upper vertical camera-origin bound, respectively. | pixels | 2:3f85e |
| moteur.json / global:scroll_obj_id | PROVEN | Level-object ids of auto-scroll markers, in level initialization order. | object ids | 2:3e5d0 |
| moteur.json / global:scroll_start_x | PROVEN | Current lower/upper horizontal camera-origin bound, respectively. | pixels | 2:3f86e |
| moteur.json / global:scroll_start_y | PROVEN | Current lower/upper vertical camera-origin bound, respectively. | pixels | 2:3f854 |
| moteur.json / global:scroll_x | PROVEN | Horizontal auto-scroll control: -1 ordinary follow, positive forced right amount, values<-1 encode forced left amount+1. | control code / pixels | 2:3f8d2 |
| moteur.json / global:scroll_y | PROVEN | Vertical scroll control: -1 ordinary follow,1 forced upward,2 alternating upward, transient-2 normalized back to-1. | control code | 2:3f8ba |
| moteur.json / global:sko_enfonce_enable | STRONG | Skops floor-sinking phase selector0/1/2. | phase code | 2:3fa8f |
| moteur.json / global:sko_last_action | STRONG | Skops previous action choice used to alternate attack selections. | action code | 2:3f8dc |
| moteur.json / global:sko_rayon_on | STRONG | Skops beam-related counter/control using0, timed countdowns and255 special state. | updates / control state | 2:3f8de |
| moteur.json / global:special_ray_mov_win_x_left | PROVEN | Additional left/right offsets in the horizontal camera-follow target window. | pixels | 2:3f8a8 |
| moteur.json / global:special_ray_mov_win_x_right | PROVEN | Additional left/right offsets in the horizontal camera-follow target window. | pixels | 2:3f8ae |
| moteur.json / global:star_ray_der | STRONG | Optional reverse-control star-effect objects behind/in front of Rayman, respectively. | nullable object pointers | 2:3e6d8 |
| moteur.json / global:star_ray_dev | STRONG | Optional reverse-control star-effect objects behind/in front of Rayman, respectively. | nullable object pointers | 2:3e6d4 |
| moteur.json / global:status_bar | PROVEN | HUD live counters and derived digit/health display selectors. | HUD record | 2:3ef52 |
| moteur.json / global:type_fndscrX | STRONG | Background-mode marker forcing horizontal camera bounds to the scroll-start position when nonzero. | mode marker | 2:3fa24 |
| moteur.json / global:v_scroll_speed | PROVEN | Requested vertical camera displacement or special positive255 control state. | pixels/call or255 | 2:3f8b4 |
| moteur.json / global:weather_wind | STRONG | Weather-provided horizontal wind state. | wind units | 2:3f874 |
| moteur.json / global:xmap_old | PROVEN | Prior camera coordinate snapshot for weather displacement. | pixels | 2:3f8b0 |
| moteur.json / global:xmapmax | PROVEN | Maximum horizontal camera origin used by world-size and movement-bound tests. | pixels | 2:3f890 |
| moteur.json / global:xmapsave | STRONG | Saved camera origin restored on wizard/bonus return. | pixels | 2:3f8b6 |
| moteur.json / global:ymap_old | PROVEN | Prior camera coordinate snapshot for weather displacement. | pixels | 2:3f8ac |
| moteur.json / global:ymapmax | PROVEN | Maximum vertical camera origin used for world-size/initial placement. | pixels | 2:3f884 |
| moteur.json / global:ymapsave | STRONG | Saved camera origin restored on wizard/bonus return. | pixels | 2:3f8b8 |
| moteur.json / global:zonediffx | PROVEN | Per-type horizontal/vertical action-zone expansion margins. | pixels | 2:2996 |
| moteur.json / global:zonediffy | PROVEN | Per-type horizontal/vertical action-zone expansion margins. | pixels | 2:2ba2 |
| objinit.json / function:Prio | PROVEN | Choose the object draw-order bucket from its type and a few level/object conditions. | see parameter/result and side-effect contract | 1:4e160 |
| objinit.json / local:Prio:obj:1 | PROVEN | Observed parameter computation: switch (obj->type); if (obj->hit_points == 0) | declared C storage; exact operations retained |  |
| objinit.json / function:first_obj_init | PROVEN | Capture per-instance reset state once and establish type-specific detection defaults. | see parameter/result and side-effect contract | 1:4e640 |
| objinit.json / local:first_obj_init:obj:1 | PROVEN | Observed parameter computation: if (((flags[obj->type].raw & 1) != 0) \|\| (((obj->type == TYPE_0_BADGUY1) \|\| (obj->type == TYPE_9_BADGUY2)) && (obj->x < 0))); obj->x = -32000;; obj->y = -32000; | declared C storage; exact operations retained |  |
| objinit.json / function:obj_init | PROVEN | Restore an instance from its captured reset state, reset common runtime members, then apply type-specific defaults. | see parameter/result and side-effect contract | 1:4e9bc |
| objinit.json / local:obj_init:unk_1:1 | UNKNOWN | Unused local; no value-consuming purpose is established. | declared C storage; exact operations retained |  |
| objinit.json / local:obj_init:frame_count:1 | PROVEN | Observed local computation: obj->anim_frame = myRand(frame_count - 1); | declared C storage; exact operations retained |  |
| objinit.json / local:obj_init:unk_2:1 | PROVEN | Signed16 vblToEOA(obj,1)-2 assigned to param and iframes_timer for non-jungle type26. | engine calls |  |
| objinit.json / local:obj_init:obj:1 | PROVEN | Observed parameter computation: obj->nb_cmd = 0;; obj->is_active = 0;; obj->display_prio = Prio(obj); | declared C storage; exact operations retained |  |
| objinit.json / function:init_struct_level | PROVEN | Build always-object and ordinary object-index lists after initialization. | see parameter/result and side-effect contract | 1:4f1e8 |
| objinit.json / local:init_struct_level:i:1 | PROVEN | Observed local computation: for (i = 0; i < level.nb_objects; ++i); level_alw.obj_ids[i] = 0;; level_obj.obj_ids[i] = 0; | declared C storage; exact operations retained |  |
| objinit.json / local:init_struct_level:obj:1 | PROVEN | Observed local computation: obj = &level.objects[i];; if ((bonus_map \|\| ((flags[obj->type].flags1 & 2) == 0)) \|\| (!bonus_taken(obj->id))); if (flags[obj->type].flags0 & 1) | declared C storage; exact operations retained |  |
| objinit.json / local:init_struct_level:nb_objs:1 | PROVEN | Signed16 level-object count cached for the iteration. | object count |  |
| objinit.json / function:INIT_OBJECTS | STRONG | Initialize all level instances and discover special-object IDs and lists. | see parameter/result and side-effect contract | 1:4f2d4 |
| objinit.json / local:INIT_OBJECTS:found_bbf2_droite:1 | STRONG | Observed local computation: found_bbf2_droite = -1;; Mus_obj_id = found_bbf2_droite;; rayman_obj_id = found_bbf2_droite; | declared C storage; exact operations retained |  |
| objinit.json / local:INIT_OBJECTS:found_bbf2_gauche:1 | STRONG | Observed local computation: found_bbf2_gauche = -1;; found_bbf2_gauche = obj->id;; if ((found_bbf2_droite != (-1)) && (found_bbf2_gauche != (-1))) | declared C storage; exact operations retained |  |
| objinit.json / local:INIT_OBJECTS:i:1 | STRONG | Observed local computation: i = 0;; obj = &level.objects[i];; while (i < nb_objs) | declared C storage; exact operations retained |  |
| objinit.json / local:INIT_OBJECTS:obj:1 | STRONG | Observed local computation: obj = &level.objects[i];; obj->id = i;; first_obj_init(obj); | declared C storage; exact operations retained |  |
| objinit.json / local:INIT_OBJECTS:nb_objs:1 | STRONG | Signed16 level-object count cached for the iteration. | object count |  |
| objinit.json / local:INIT_OBJECTS:prev_obj:1 | STRONG | Observed local computation: if ((prev_obj->type == TYPE_10_FISH) && (prev_obj->x == obj->x)) | declared C storage; exact operations retained |  |
| objinit.json / local:INIT_OBJECTS:a1:1 | STRONG | Nonzero captures initial position/state/hit points with first_obj_init; zero reuses the earlier snapshots. | boolean |  |
| objinit.json / function:REINIT_OBJECT | PROVEN | Reset an existing instance for reactivation without recreating its resources. | see parameter/result and side-effect contract | 1:50288 |
| objinit.json / local:REINIT_OBJECT:obj:1 | PROVEN | Observed parameter computation: obj_init(obj);; obj->flags.alive = 1;; obj->active_flag = 2; | declared C storage; exact operations retained |  |
| objinit.json / function:special_flags_init | PROVEN | Apply world/level exceptions to type-level activation/collision tables. | see parameter/result and side-effect contract | 1:502c4 |
| objinit.json / local:special_flags_init:i:1 | PROVEN | Observed local computation: for (i = 0; i < 262; ++i); if (i == TYPE_0_BADGUY1); zonediffx[i] = -120; | declared C storage; exact operations retained |  |
| ray.json / function:allocateRayLandingSmoke | PROVEN | Allocate the landing explosion effect unless the suppression byte is set. | see parameter/result and side-effect contract | 1:5bfd0 |
| ray.json / local:allocateRayLandingSmoke:obj:1 | PROVEN | Observed local dataflow: obj = allocateExplosion(&ray);; if (obj); set_main_and_sub_etat(obj, 0, 1); | declared C storage; see exact computation and function contract |  |
| ray.json / function:recale_ray_on_liane | PROVEN | Align Rayman's horizontal anchor to the centre of the detected vine tile. | see parameter/result and side-effect contract | 1:5c004 |
| ray.json / local:recale_ray_on_liane:x:1 | PROVEN | Left edge of the selected16-pixel vine tile. | declared C storage; see exact computation and function contract |  |
| ray.json / local:recale_ray_on_liane:unk_1:1 | PROVEN | Observed local dataflow: unk_1 = 8; | declared C storage; see exact computation and function contract |  |
| ray.json / function:calc_bhand_typ | PROVEN | Sample the centre and adjacent hand tiles for vine attachment. | see parameter/result and side-effect contract | 1:5c088 |
| ray.json / local:calc_bhand_typ:x:1 | PROVEN | Observed local dataflow: x = ray.offset_bx + ray.x;; hand_btyp = BTYP(x >> 4, y >> 4);; hand_btypg = BTYP((x - 8) >> 4, y >> 4); | declared C storage; see exact computation and function contract |  |
| ray.json / local:calc_bhand_typ:unk_1:1 | PROVEN | Byte-narrowed hand sample offset before adding object y. | declared C storage; see exact computation and function contract |  |
| ray.json / local:calc_bhand_typ:y:1 | PROVEN | Observed local dataflow: y = unk_1 + ray.y;; hand_btyp = BTYP(x >> 4, y >> 4);; hand_btypg = BTYP((x - 8) >> 4, y >> 4); | declared C storage; see exact computation and function contract |  |
| ray.json / function:IS_RAY_ON_LIANE | PROVEN | Attach airborne Rayman to a vine after the jump delay, or count down the reattachment delay. | see parameter/result and side-effect contract | 1:5c138 |
| ray.json / local:IS_RAY_ON_LIANE:speed_y:1 | PROVEN | Observed local dataflow: speed_y = ray.speed_y;; ray.speed_y = 0;; ray.speed_y = speed_y; | declared C storage; see exact computation and function contract |  |
| ray.json / function:rayMayLandOnAnObject | STRONG | Find a nearby active support surface and transfer Rayman onto it. | see parameter/result and side-effect contract | 1:5c224 |
| ray.json / local:rayMayLandOnAnObject:var_a1_1:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| ray.json / local:rayMayLandOnAnObject:cur_type:1 | STRONG | Observed local dataflow: cur_type = cur_obj->type;; if (((cur_type == TYPE_PI) \|\| (cur_type == TYPE_HERSE_HAUT)) \|\| (cur_type == TYPE_HERSE_BAS_NEXT)) | declared C storage; see exact computation and function contract |  |
| ray.json / local:rayMayLandOnAnObject:unk_y:1 | STRONG | Absolute relative vertical motion plus2, clamped to the size-dependent minimum. | declared C storage; see exact computation and function contract |  |
| ray.json / local:rayMayLandOnAnObject:cur_obj:1 | STRONG | Observed local dataflow: cur_obj = &level.objects[actobj.objects[0]];; if (cur_obj->flags.follow_enabled && (cur_obj->id != obj_id)); if (flags[cur_obj->type].raw & flags1_0x20_move_y) | declared C storage; see exact computation and function contract |  |
| ray.json / local:rayMayLandOnAnObject:i:1 | STRONG | Observed local dataflow: i = 0;; while (i < actobj.num_active_objects); ray.follow_id = actobj.objects[i]; | declared C storage; see exact computation and function contract |  |
| ray.json / local:rayMayLandOnAnObject:unk_1:1 | STRONG | Minimum landing tolerance:8 tiny,4 otherwise. | declared C storage; see exact computation and function contract |  |
| ray.json / local:rayMayLandOnAnObject:foll_w:1 | STRONG | Observed local dataflow: GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &foll_x, &foll_y, &foll_w, &foll_h); | declared C storage; see exact computation and function contract |  |
| ray.json / local:rayMayLandOnAnObject:foll_h:1 | STRONG | Observed local dataflow: GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &foll_x, &foll_y, &foll_w, &foll_h); | declared C storage; see exact computation and function contract |  |
| ray.json / local:rayMayLandOnAnObject:foll_y:1 | STRONG | Observed local dataflow: GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &foll_x, &foll_y, &foll_w, &foll_h);; ray.speed_y = ((cur_obj->offset_hy + foll_y) - ray.offset_by) - ray.y; | declared C storage; see exact computation and function contract |  |
| ray.json / local:rayMayLandOnAnObject:foll_x:1 | STRONG | Observed local dataflow: GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &foll_x, &foll_y, &foll_w, &foll_h); | declared C storage; see exact computation and function contract |  |
| ray.json / local:rayMayLandOnAnObject:distance:1 | STRONG | Observed local dataflow: ray.speed_y -= ashr16((s16) (((s8) cur_obj->hit_points) * distance), 4); | declared C storage; see exact computation and function contract |  |
| ray.json / local:rayMayLandOnAnObject:param_1:1 | STRONG | Caller-owned byte cleared only when a new support is acquired. | declared C storage; see exact computation and function contract |  |
| ray.json / local:rayMayLandOnAnObject:obj_id:1 | STRONG | Support id excluded from the scan and restored if no replacement is found; -1 means none. | declared C storage; see exact computation and function contract |  |
| ray.json / function:set_air_speed | PROVEN | Rewrite one airborne state's left/right speed limits using the current horizontal motion. | see parameter/result and side-effect contract | 1:5ca70 |
| ray.json / local:set_air_speed:eta:1 | PROVEN | Observed local dataflow: eta = &ray.eta[main_etat][sub_etat];; left = &eta->speed_x_left;; eta->speed_x_right = max32(param_4, unk_1); | declared C storage; see exact computation and function contract |  |
| ray.json / local:set_air_speed:left:1 | PROVEN | Observed local dataflow: left = &eta->speed_x_left;; *left = -16;; *left = min32(-param_4, unk_2); | declared C storage; see exact computation and function contract |  |
| ray.json / local:set_air_speed:unk_1:1 | PROVEN | Signed-byte magnitude assigned after abs(input)>>4, then clamped to112. | declared C storage; see exact computation and function contract |  |
| ray.json / local:set_air_speed:unk_2:1 | PROVEN | Negated signed-byte speed magnitude for the left-facing limit. | declared C storage; see exact computation and function contract |  |
| ray.json / local:set_air_speed:main_etat:1 | PROVEN | Observed parameter dataflow: eta = &ray.eta[main_etat][sub_etat]; | declared C storage; see exact computation and function contract |  |
| ray.json / local:set_air_speed:sub_etat:1 | PROVEN | Observed parameter dataflow: eta = &ray.eta[main_etat][sub_etat]; | declared C storage; see exact computation and function contract |  |
| ray.json / local:set_air_speed:param_3:1 | PROVEN | Input momentum whose magnitude>>4 supplies an eta speed limit. | declared C storage; see exact computation and function contract |  |
| ray.json / local:set_air_speed:param_4:1 | PROVEN | Minimum unsigned-byte magnitude for the selected directional eta speed. | declared C storage; see exact computation and function contract |  |
| ray.json / function:Reset_air_speed | PROVEN | Rewrite the normal or rolling set of airborne state speed limits. | see parameter/result and side-effect contract | 1:5caf8 |
| ray.json / local:Reset_air_speed:is_rolling_speed:1 | PROVEN | Observed parameter dataflow: if (is_rolling_speed) | declared C storage; see exact computation and function contract |  |
| ray.json / function:determineRayAirInertia | STRONG | Select whether the next airborne motion retains horizontal inertia. | see parameter/result and side-effect contract | 1:5cce0 |
| ray.json / local:determineRayAirInertia:wind:1 | STRONG | Observed local dataflow: if (wind != 0); ray.nb_cmd = wind; | declared C storage; see exact computation and function contract |  |
| ray.json / function:ray_jump | STRONG | Initiate a permitted jump and detach Rayman from support. | see parameter/result and side-effect contract | 1:5cd40 |
| ray.json / local:ray_jump:speed_y:1 | STRONG | Observed local dataflow: ray.speed_y -= 2;; ray.speed_y = -3;; ray.speed_y = -5; | declared C storage; see exact computation and function contract |  |
| ray.json / local:ray_jump:follow_y:1 | STRONG | Observed local dataflow: follow_y = level.objects[ray.follow_id].follow_y;; speed_y = min32(speed_y, follow_y); | declared C storage; see exact computation and function contract |  |
| ray.json / local:ray_jump:unk_2:1 | STRONG | Momentum magnitude threshold256 tiny,512 otherwise. | declared C storage; see exact computation and function contract |  |
| ray.json / local:ray_jump:is_rolling_speed:1 | STRONG | Observed local dataflow: is_rolling_speed = unk_2 < Abs(decalage_en_cours);; Reset_air_speed(is_rolling_speed);; if (is_rolling_speed) | declared C storage; see exact computation and function contract |  |
| ray.json / local:ray_jump:btype:1 | STRONG | Observed local dataflow: if (btype < 12); if (btype < 4); if (btype < 2) | declared C storage; see exact computation and function contract |  |
| ray.json / function:ray_inertia_speed | STRONG | Integrate and decay Rayman's horizontal momentum, then derive pixel motion. | see parameter/result and side-effect contract | 1:5d020 |
| ray.json / local:ray_inertia_speed:unk_1:1 | STRONG | Coefficient>>3 with minimum1 when coefficient!=0; increased by2 in airborne sub15. | declared C storage; see exact computation and function contract |  |
| ray.json / local:ray_inertia_speed:unk_2:1 | STRONG | Prior input times coefficient, arithmetic-shifted8 and narrowed16. | declared C storage; see exact computation and function contract |  |
| ray.json / local:ray_inertia_speed:unk_3:1 | STRONG | Surface contribution shifted left2 and narrowed16. | declared C storage; see exact computation and function contract |  |
| ray.json / local:ray_inertia_speed:unk_4:1 | STRONG | Signed16 sum of weighted prior, surface and wind contributions. | declared C storage; see exact computation and function contract |  |
| ray.json / local:ray_inertia_speed:unk_5:1 | STRONG | World-specific saturation bias6,3 or initial0. | declared C storage; see exact computation and function contract |  |
| ray.json / local:ray_inertia_speed:unk_6:1 | STRONG | Signed16 direction-specific saturation target, bounded against current momentum. | declared C storage; see exact computation and function contract |  |
| ray.json / local:ray_inertia_speed:a1:1 | STRONG | Byte coefficient weighting the prior speed contribution and post-update decay. | declared C storage; see exact computation and function contract |  |
| ray.json / local:ray_inertia_speed:a2:1 | STRONG | Signed surface contribution shifted left2 in the momentum increment. | declared C storage; see exact computation and function contract |  |
| ray.json / local:ray_inertia_speed:prev_speed_x:1 | STRONG | Observed parameter dataflow: decalage_en_cours = prev_speed_x;; unk_2 = ashr32(prev_speed_x * a1, 8);; unk_6 = (((unk_2 > 0) ? (prev_speed_x) : (0)) + ((unk_3 > 0) ? (ashl16(a2 + unk_5, 8) / unk_1) : (0))) + ashl16(a4, 8); | declared C storage; see exact computation and function contract |  |
| ray.json / local:ray_inertia_speed:a4:1 | STRONG | Signed wind contribution shifted left3 in the increment and left8 in its saturation target. | declared C storage; see exact computation and function contract |  |
| ray.json / function:RAY_SWIP | STRONG | Select horizontal inertia parameters from Rayman's state, support, surface and wind. | see parameter/result and side-effect contract | 1:5d2e8 |
| ray.json / local:RAY_SWIP:temp_a2:1 | STRONG | Copy of eta-selected speed shifted left4, passed to ray_inertia_speed. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_SWIP:temp_v0:1 | STRONG | Eta-selected signed16 horizontal speed shifted left4. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_SWIP:var_v0_2:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_SWIP:var_v0_3:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_SWIP:var_a1:1 | STRONG | -1 if nonzero selected magnitude is below257, otherwise0; later byte conversion is preserved. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_SWIP:var_v1:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_SWIP:temp_v1_2:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_SWIP:x_accel:1 | STRONG | Observed local dataflow: x_accel = 0;; x_accel = 0;; x_accel = -ashr16((s8) follow_obj->hit_points, 1); | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_SWIP:var_s4:1 | STRONG | Ground coefficient32 in world3,16 otherwise. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_SWIP:var_s0:1 | STRONG | Byte coefficient selected from support/surface/air rules and halved on pan. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_SWIP:follow_obj:1 | STRONG | Observed local dataflow: follow_obj = &level.objects[ray.follow_id];; if (!follow_obj->flags.follow_enabled); ray_dist = follow_obj->ray_dist; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_SWIP:ray_dist:1 | STRONG | Observed local dataflow: ray_dist = follow_obj->ray_dist;; if (((Abs(ray_dist) > 8) \|\| (follow_obj->ray_dist < 0)) \|\| (ray.follow_id == (-1))); if (Abs(follow_obj->ray_dist) > 8) | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_SWIP:sp10:1 | STRONG | Output byte supplied to rayMayLandOnAnObject; result is not consumed in this caller. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_SWIP:old_ray_main_etat:1 | STRONG | Observed local dataflow: old_ray_main_etat = ray.main_etat;; if ((old_ray_main_etat == 0) && (ray.sub_etat == 40)); if ((old_ray_main_etat == 1) && ((ray.sub_etat == 9) \|\| (ray.sub_etat == 11))) | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_SWIP:follow_obj:2 | STRONG | Observed local dataflow: follow_obj = &level.objects[ray.follow_id];; if (!follow_obj->flags.follow_enabled); ray_dist = follow_obj->ray_dist; | declared C storage; see exact computation and function contract |  |
| ray.json / function:RAY_STOP | PROVEN | Switch locomotion or vine movement to its stationary counterpart. | see parameter/result and side-effect contract | 1:5d724 |
| ray.json / function:RAY_HELICO | STRONG | Handle airborne helicopter activation, stopping and super-helicopter retriggers. | see parameter/result and side-effect contract | 1:5d7d4 |
| ray.json / local:RAY_HELICO:sel_eta:1 | STRONG | Observed local dataflow: sel_eta = &eta[ray.main_etat][ray.sub_etat];; sel_eta->anim_speed_bits.high = 0x3;; sel_eta = &eta[ray.main_etat][ray.sub_etat]; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_HELICO:eta:1 | STRONG | Observed local dataflow: if (((ray.eta[ray.main_etat][ray.sub_etat].flags & 4) && (button_released == 1)) && options_jeu.test_fire1()); sel_eta = &eta[ray.main_etat][ray.sub_etat];; eta[2][15].anim_speed_bits.high = 2; | declared C storage; see exact computation and function contract |  |
| ray.json / function:Make_Ray_Hang | STRONG | Snap Rayman to a tile ledge and enter hanging state5:0. | see parameter/result and side-effect contract | 1:5db1c |
| ray.json / local:Make_Ray_Hang:unk_4:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| ray.json / local:Make_Ray_Hang:unk_1:1 | STRONG | Projected32-pixel hanging offset, plus37 in tiny mode. | declared C storage; see exact computation and function contract |  |
| ray.json / local:Make_Ray_Hang:unk_2:1 | STRONG | Horizontal inset3 tiny,7 ordinary. | declared C storage; see exact computation and function contract |  |
| ray.json / local:Make_Ray_Hang:unk_3:1 | STRONG | Inset or16-inset selected by facing. | declared C storage; see exact computation and function contract |  |
| ray.json / local:Make_Ray_Hang:a1:1 | STRONG | World-pixel hand x whose16-pixel tile boundary anchors hanging x. | declared C storage; see exact computation and function contract |  |
| ray.json / local:Make_Ray_Hang:a2:1 | STRONG | World-pixel hand y whose16-pixel tile boundary anchors hanging y. | declared C storage; see exact computation and function contract |  |
| ray.json / function:AIR | PROVEN | Test absence of the tile support flag(mask2) at a linear map-tile index. | see parameter/result and side-effect contract | 1:5dc24 |
| ray.json / local:AIR:btyp:1 | PROVEN | Observed local dataflow: return !block_flags[btyp].bits.solid; | declared C storage; see exact computation and function contract |  |
| ray.json / local:AIR:a1:1 | PROVEN | Linear index into mp.map. | declared C storage; see exact computation and function contract |  |
| ray.json / function:MUR | PROVEN | Test the tile support flag(mask2) at a linear map-tile index. | see parameter/result and side-effect contract | 1:5dc4c |
| ray.json / local:MUR:btyp:1 | PROVEN | Observed local dataflow: return block_flags[btyp].bits.solid; | declared C storage; see exact computation and function contract |  |
| ray.json / local:MUR:a1:1 | PROVEN | Linear index into mp.map. | declared C storage; see exact computation and function contract |  |
| ray.json / function:CAN_RAY_HANG_BLOC | PROVEN | Check the ledge's solid and empty tile pattern before snapping to it. | see parameter/result and side-effect contract | 1:5dc78 |
| ray.json / local:CAN_RAY_HANG_BLOC:ray_x_1:1 | PROVEN | Observed local dataflow: ray_x_1 = ray.x + ray.offset_bx;; ray_x_2 = ray_x_1 >> 4;; set_proj_center(ray_x_1, ray.y + ray.offset_by); | declared C storage; see exact computation and function contract |  |
| ray.json / local:CAN_RAY_HANG_BLOC:ray_x_2:1 | PROVEN | Observed local dataflow: ray_x_2 = ray_x_1 >> 4;; if (inrange(ray_x_2, 1, mp.width - 2)); unk_1 = ray_x_2 + (ray_y_2 * mp.width); | declared C storage; see exact computation and function contract |  |
| ray.json / local:CAN_RAY_HANG_BLOC:ray_y_1:1 | PROVEN | Observed local dataflow: ray_y_1 = (ray.y + ray.offset_hy) + 32;; ray_y_1 = get_proj_y(ray.scale, ray_y_1);; ray_y_2 = ray_y_1 >> 4; | declared C storage; see exact computation and function contract |  |
| ray.json / local:CAN_RAY_HANG_BLOC:dir_x:1 | PROVEN | Observed local dataflow: dir_x = (ray.flags.flip_x) ? (1) : (-1);; unk_2 = (unk_1 - mp.width) + dir_x;; unk_6 = unk_1 + dir_x; | declared C storage; see exact computation and function contract |  |
| ray.json / local:CAN_RAY_HANG_BLOC:unk_2:1 | PROVEN | Tile one row above and one column toward the wall. | declared C storage; see exact computation and function contract |  |
| ray.json / local:CAN_RAY_HANG_BLOC:unk_1:1 | PROVEN | Linear map index at the projected hand position. | declared C storage; see exact computation and function contract |  |
| ray.json / local:CAN_RAY_HANG_BLOC:ray_y_2:1 | PROVEN | Observed local dataflow: ray_y_2 = ray_y_1 >> 4;; unk_1 = ray_x_2 + (ray_y_2 * mp.width); | declared C storage; see exact computation and function contract |  |
| ray.json / local:CAN_RAY_HANG_BLOC:unk_3:1 | PROVEN | Tile one row below the hand index. | declared C storage; see exact computation and function contract |  |
| ray.json / local:CAN_RAY_HANG_BLOC:unk_4:1 | PROVEN | Tile two rows below the hand index. | declared C storage; see exact computation and function contract |  |
| ray.json / local:CAN_RAY_HANG_BLOC:unk_5:1 | PROVEN | Third row below hand normally, second row when scaled. | declared C storage; see exact computation and function contract |  |
| ray.json / local:CAN_RAY_HANG_BLOC:unk_6:1 | PROVEN | Tile horizontally adjacent toward the wall at hand height. | declared C storage; see exact computation and function contract |  |
| ray.json / local:CAN_RAY_HANG_BLOC:unk_7:1 | PROVEN | Wall-adjacent tile two rows below hand. | declared C storage; see exact computation and function contract |  |
| ray.json / local:CAN_RAY_HANG_BLOC:unk_8:1 | PROVEN | Wall-adjacent tile one row below hand. | declared C storage; see exact computation and function contract |  |
| ray.json / function:RAY_TOMBE | PROVEN | Enter an airborne departure state without applying a fresh jump impulse. | see parameter/result and side-effect contract | 1:5df28 |
| ray.json / function:RAY_RESPOND_TO_DOWN | PROVEN | Apply the down input according to Rayman's main state. | see parameter/result and side-effect contract | 1:5e030 |
| ray.json / local:RAY_RESPOND_TO_DOWN:sel_eta_1:1 | PROVEN | Observed local dataflow: sel_eta_1 = &ray.eta[ray.main_etat][ray.sub_etat];; sel_eta_1->flags_bits.anim_reverse = !sel_eta_1->flags_bits.anim_reverse;; sel_eta_1->flags_bits.anim_reverse = !sel_eta_1->flags_bits.anim_reverse; | declared C storage; see exact computation and function contract |  |
| ray.json / function:RAY_RESPOND_TO_UP | STRONG | Apply the up input according to Rayman's main state. | see parameter/result and side-effect contract | 1:5e370 |
| ray.json / function:RAY_RESPOND_TO_DIR | STRONG | Apply horizontal input, including facing, crouch/crawl transitions and mount motion. | see parameter/result and side-effect contract | 1:5e548 |
| ray.json / local:RAY_RESPOND_TO_DIR:sel_anim:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_RESPOND_TO_DIR:unk_1:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_RESPOND_TO_DIR:to_speed:1 | STRONG | Observed local dataflow: to_speed = -1;; to_speed = flip_x;; flip_x = to_speed; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_RESPOND_TO_DIR:new_main_etat:1 | STRONG | Observed local dataflow: new_main_etat = 0;; new_main_etat = 0;; new_main_etat = 3; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_RESPOND_TO_DIR:new_sub_etat:1 | STRONG | Observed local dataflow: new_sub_etat = 47;; new_sub_etat = 48;; new_sub_etat = 6; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_RESPOND_TO_DIR:v5:1 | STRONG | Materialized condition that exits one of the crawl state families. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_RESPOND_TO_DIR:v5:2 | STRONG | Materialized condition that exits one of the crawl state families. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_RESPOND_TO_DIR:flip_x:1 | STRONG | Observed parameter dataflow: if (ray.flags.flip_x != ((u8) flip_x)); ray.flags.flip_x = flip_x;; if (ray.flags.flip_x == ((u8) flip_x)) | declared C storage; see exact computation and function contract |  |
| ray.json / function:RAY_RESPOND_TO_NOTHING | PROVEN | Apply no-direction state transitions, idle animation cycling and motion decay. | see parameter/result and side-effect contract | 1:5eb40 |
| ray.json / local:RAY_RESPOND_TO_NOTHING:unk_1:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_RESPOND_TO_NOTHING:sel_eta_1:1 | PROVEN | Observed local dataflow: sel_eta_1 = &ray.eta[ray.main_etat][ray.sub_etat];; sel_eta_1->flags_bits.anim_reverse = !sel_eta_1->flags_bits.anim_reverse;; sel_eta_1->flags_bits.anim_reverse = !sel_eta_1->flags_bits.anim_reverse; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_RESPOND_TO_NOTHING:sel_eta_2:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_RESPOND_TO_NOTHING:sel_anim:1 | PROVEN | Observed local dataflow: sel_anim = &ray.animations[ray.anim_index];; ray.anim_frame = sel_anim->frames_count - 1; | declared C storage; see exact computation and function contract |  |
| ray.json / function:RAY_RESPOND_TO_BUTTON4 | PROVEN | Empty action callback in the original Rayman unit. | see parameter/result and side-effect contract | 1:5f0ac |
| ray.json / function:RAY_RESPOND_TO_BUTTON3 | STRONG | Handle action-button run, seed and ground-action selection. | see parameter/result and side-effect contract | 1:5f0b0 |
| ray.json / function:RAY_RESPOND_TO_FIRE0 | PROVEN | Start fist preparation when the fist capability and availability permit it. | see parameter/result and side-effect contract | 1:5f1c8 |
| ray.json / function:RAY_RESPOND_TO_FIRE1 | PROVEN | Route the jump button to ray_jump for eligible main states. | see parameter/result and side-effect contract | 1:5f208 |
| ray.json / function:RAY_BALANCE_ANIM | PROVEN | Map a swing angle and facing to an animation frame. | see parameter/result and side-effect contract | 1:5f24c |
| ray.json / local:RAY_BALANCE_ANIM:base:1 | PROVEN | Observed local dataflow: base = 32;; base = 95;; return abs32_inline(base - grp_angle); | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_BALANCE_ANIM:grp_angle:1 | PROVEN | Observed parameter dataflow: if (grp_angle == 0); grp_angle++;; grp_angle = (grp_angle - 1) >> 3; | declared C storage; see exact computation and function contract |  |
| ray.json / function:SET_RAY_BALANCE | PROVEN | Enter grapple swinging state7:0 and reset movement bookkeeping. | see parameter/result and side-effect contract | 1:5f29c |
| ray.json / function:RAY_GOING_BALANCE | PROVEN | Move the grapple entry toward its swinging radius and angle. | see parameter/result and side-effect contract | 1:5f328 |
| ray.json / local:RAY_GOING_BALANCE:cosine_angle:1 | PROVEN | Observed local dataflow: abs_sinus_cosinus(angle + 128, &sine_angle, &cosine_angle);; if ((sine_angle > 2) && (cosine_angle > 2)); unk_3 = ashl32(diff_x, 9) / cosine_angle; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_GOING_BALANCE:sine_angle:1 | PROVEN | Observed local dataflow: abs_sinus_cosinus(angle + 128, &sine_angle, &cosine_angle);; if ((sine_angle > 2) && (cosine_angle > 2)); unk_4 = ashl32(diff_y, 9) / sine_angle; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_GOING_BALANCE:unk_3:1 | PROVEN | Absolute x displacement shifted9 and divided by the selected trig component. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_GOING_BALANCE:unk_4:1 | PROVEN | Absolute y displacement shifted9 and divided by the selected trig component. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_GOING_BALANCE:angle:1 | PROVEN | Observed local dataflow: angle = ANGLE_RAYMAN(obj);; obj->follow_x = angle;; if ((angle > 256) \|\| ((angle == 256) && (!ray.flags.flip_x))) | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_GOING_BALANCE:follow_y:1 | PROVEN | Observed local dataflow: follow_y = (unk_3 + unk_4) >> 1;; follow_y = diff_x;; follow_y = diff_y; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_GOING_BALANCE:diff_x:1 | PROVEN | Observed local dataflow: if (diff_x < 0); diff_x = -diff_x;; unk_3 = ashl32(diff_x, 9) / cosine_angle; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_GOING_BALANCE:diff_y:1 | PROVEN | Observed local dataflow: if (diff_y < 0); diff_y = -diff_y;; unk_4 = ashl32(diff_y, 9) / sine_angle; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_GOING_BALANCE:dist:1 | PROVEN | Observed local dataflow: if (dist > 4512U); if (dist <= 4512U) | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_GOING_BALANCE:obj:1 | PROVEN | Object record processed by this function: Move the grapple entry toward its swinging radius and angle. | declared C storage; see exact computation and function contract |  |
| ray.json / function:RAY_BALANCE | STRONG | Advance grapple swinging and derive Rayman's movement from the grapple anchor. | see parameter/result and side-effect contract | 1:5f500 |
| ray.json / local:RAY_BALANCE:cosine_grp_angle:1 | STRONG | Observed local dataflow: sinus_cosinus(grp_angle + 128, &cosine_grp_angle, &sine_grp_angle);; unk_spd_x = (((obj_grp->x + obj_grp->offset_bx) + ((obj_grp->follow_y * cosine_grp_angle) >> 9)) - ray.offset_bx) - ray.x; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_BALANCE:sine_grp_angle:1 | STRONG | Observed local dataflow: sinus_cosinus(grp_angle + 128, &cosine_grp_angle, &sine_grp_angle);; unk_spd_y = (((obj_grp->y + obj_grp->offset_by) - ((obj_grp->follow_y * sine_grp_angle) >> 9)) - ray.offset_by) - ray.y; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_BALANCE:unk_1:1 | STRONG | Lower swing angle bound128 in1/512-turn units. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_BALANCE:unk_2:1 | STRONG | Upper swing angle bound384 in1/512-turn units. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_BALANCE:unk_3:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_BALANCE:unk_4:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_BALANCE:unk_spd_x:1 | STRONG | Grapple-derived desired Rayman x minus current x before adding grapple motion. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_BALANCE:unk_spd_y:1 | STRONG | Grapple-derived desired Rayman y minus current y before adding grapple motion. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_BALANCE:grp_angle:1 | STRONG | Observed local dataflow: grp_angle = obj_grp->follow_x;; sinus_cosinus(grp_angle + 128, &cosine_grp_angle, &sine_grp_angle);; if (grp_angle >= unk_2) | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_BALANCE:ray_eta:1 | STRONG | Observed local dataflow: ray.anim_index = ray_eta[ray.main_etat][ray.sub_etat].anim_index; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_BALANCE:obj_grp:1 | STRONG | Observed local dataflow: grp_angle = obj_grp->follow_x;; RAY_GOING_BALANCE(obj_grp);; if (obj_grp->timer == 0) | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_BALANCE:v4:1 | STRONG | Signed angle increment with magnitude(abs_cosinus(angle)>>7)+1. | declared C storage; see exact computation and function contract |  |
| ray.json / function:RAY_FIN_BALANCE | PROVEN | Release the grapple into airborne state2:2. | see parameter/result and side-effect contract | 1:5f7d0 |
| ray.json / local:RAY_FIN_BALANCE:flip:1 | PROVEN | Observed local dataflow: ray.flags.flip_x = flip; | declared C storage; see exact computation and function contract |  |
| ray.json / function:RayTestBlocSH | STRONG | Stop super-helicopter motion against sampled hard tiles. | see parameter/result and side-effect contract | 1:5f870 |
| ray.json / local:RayTestBlocSH:x_1:1 | STRONG | Observed local dataflow: x_1 = ray.x + ray.offset_bx;; var_a1 = x_1;; temp_v1 = x_1 & 15; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayTestBlocSH:y_1:1 | STRONG | Observed local dataflow: y_1 = ray.y + ray.offset_hy;; if ((decalage_en_cours != 0) && ((MURDUR(temp_v1_2, y_1 + 16) \|\| MURDUR(temp_v1_2, y_1 + 32)) \|\| MURDUR(temp_v1_2, y_1 + 48))); if ((ray.speed_y < 0) && MURDUR(x_1, y_1 + 16)) | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayTestBlocSH:temp_v1_2:1 | STRONG | Signed16 world x used for three hard-tile samples. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayTestBlocSH:var_a1:1 | STRONG | Anchor x optionally advanced one tile according to its low nibble and movement sign. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayTestBlocSH:var_v0:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayTestBlocSH:temp_v1:1 | STRONG | Low4 bits of the world-pixel x anchor. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayTestBlocSH:var_a0:1 | STRONG | Signed16-pixel horizontal sample step. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayTestBlocSH:var_v0_2:1 | UNKNOWN | Unused local declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| ray.json / function:remoteControlRay | PROVEN | Move the scripted horizontal target state until within one pixel. | see parameter/result and side-effect contract | 1:5f95c |
| ray.json / local:remoteControlRay:diff:1 | PROVEN | Observed local dataflow: if (diff > 1); if (diff < (-1)) | declared C storage; see exact computation and function contract |  |
| ray.json / function:STOPPE_RAY_CONTRE_PAROIS | PROVEN | Stop upward airborne movement at a blocking tile. | see parameter/result and side-effect contract | 1:5f9e0 |
| ray.json / local:STOPPE_RAY_CONTRE_PAROIS:block:1 | PROVEN | Observed parameter dataflow: if (((((ray_mode != MODE_3_MORT_DE_RAYMAN) && (block_flags[block].raw & 0x10)) && (ray.sub_etat != 7)) && (ray.sub_etat != 9)) && (ray.speed_y <= 0)) | declared C storage; see exact computation and function contract |  |
| ray.json / function:RAY_IN_THE_AIR | PROVEN | Update airborne timers, jump gravity, helicopter, landing and attachment checks. | see parameter/result and side-effect contract | 1:5fa44 |
| ray.json / local:RAY_IN_THE_AIR:may_land_obj:1 | PROVEN | Observed local dataflow: may_land_obj = 1;; rayMayLandOnAnObject(&may_land_obj, -1);; rayMayLandOnAnObject(&may_land_obj, -1); | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_IN_THE_AIR:anim_speed:1 | PROVEN | Current eta animation-speed high nibble, which selects gravity behaviour. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_IN_THE_AIR:eta:1 | PROVEN | Observed local dataflow: anim_speed = ray.eta[ray.main_etat][ray.sub_etat].anim_speed_bits.high;; run_speed = ashr16(ray.eta[1][3].speed_x_right, 4); | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_IN_THE_AIR:block:1 | PROVEN | Observed local dataflow: block = calc_typ_trav(&ray, 2);; STOPPE_RAY_CONTRE_PAROIS(block); | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_IN_THE_AIR:ray_eta:1 | PROVEN | Observed local dataflow: ray_eta->anim_speed_bits.high = 1; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_IN_THE_AIR:i:1 | PROVEN | Observed local dataflow: for (i = 1; i < 10; i++); if (pos_stack[i - 1] != pos_stack[i]); if (i != 10) | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_IN_THE_AIR:new_timer:1 | PROVEN | Minimum of existing iframes and90 when a hurt state lands on tiles. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_IN_THE_AIR:run_speed:1 | PROVEN | Observed local dataflow: run_speed = ashr16(ray.eta[1][3].speed_x_right, 4);; if (abs32_inline(ray.speed_x) >= run_speed) | declared C storage; see exact computation and function contract |  |
| ray.json / function:terminateFistWhenRayDies | PROVEN | Spawn a fist disappearance effect and disable an active fist. | see parameter/result and side-effect contract | 1:6008c |
| ray.json / function:snifRayIsDead | PROVEN | Consume a life and clear death-related rewards/capabilities. | see parameter/result and side-effect contract | 1:600a4 |
| ray.json / local:snifRayIsDead:obj:1 | PROVEN | Object record processed by this function: Consume a life and clear death-related rewards/capabilities. | declared C storage; see exact computation and function contract |  |
| ray.json / function:rayfallsinwater | PROVEN | Begin drowning state3:22 and its one-count death delay. | see parameter/result and side-effect contract | 1:6010c |
| ray.json / function:RAY_DEAD | PROVEN | Check lethal bounds/water and retire or report the live Rayman object. | see parameter/result and side-effect contract | 1:601a8 |
| ray.json / local:RAY_DEAD:alive:1 | PROVEN | Observed local dataflow: if (((!((ray.main_etat == 2) && (ray.sub_etat == 9))) && (!((ray.main_etat == 3) && (ray.sub_etat == 22)))) && ray.flags.alive); ray.flags.alive = 0;; alive = ((ray.is_active && ray.flags.alive) && (ray_mode != MODE_3_MORT_DE_RAYMAN)) && (ray_mode != MODE_4_MORT_DE_RAYMAN_ON_MS); | declared C storage; see exact computation and function contract |  |
| ray.json / function:RAY_HURT | PROVEN | Subtract a hit point and select death mode on unsigned underflow. | see parameter/result and side-effect contract | 1:60408 |
| ray.json / function:RepousseRay | PROVEN | Push Rayman below the active stone mechanism and update its airborne state. | see parameter/result and side-effect contract | 1:604b0 |
| ray.json / local:RepousseRay:pa_obj:1 | PROVEN | Observed local dataflow: if (pa_obj->is_active); if ((ray.x > (pa_obj->x - 82)) && (ray.x < (pa_obj->x + 150))); if (ray.y < (pa_obj->y + 96)) | declared C storage; see exact computation and function contract |  |
| ray.json / function:RayEstIlBloque | STRONG | Detect horizontal blocking by active objects marked as Rayman barriers. | see parameter/result and side-effect contract | 1:60598 |
| ray.json / local:RayEstIlBloque:unk_x:1 | STRONG | Cached Rayman collision-box x plus its width only for positive motion. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayEstIlBloque:unk_h:1 | STRONG | Cached Rayman box height arithmetic-shifted2. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayEstIlBloque:i:1 | STRONG | Observed local dataflow: i = 0;; cur_obj = &level.objects[actobj.objects[i]];; while (i < actobj.num_active_objects) | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayEstIlBloque:ani_x:1 | STRONG | Observed local dataflow: GET_ANIM_POS(cur_obj, &ani_x, &ani_y, &ani_w, &ani_h);; ani_x += ani_w >> 3;; ani_x += 6; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayEstIlBloque:ani_y:1 | STRONG | Observed local dataflow: GET_ANIM_POS(cur_obj, &ani_x, &ani_y, &ani_w, &ani_h);; ani_y = (spr_y + cur_obj->offset_hy) + (ray.follow_id == cur_obj->id);; ani_y = cur_obj->y + cur_obj->offset_hy; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayEstIlBloque:ani_w:1 | STRONG | Observed local dataflow: GET_ANIM_POS(cur_obj, &ani_x, &ani_y, &ani_w, &ani_h);; ani_x += ani_w >> 3;; ani_w -= ani_w >> 2; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayEstIlBloque:ani_h:1 | STRONG | Observed local dataflow: GET_ANIM_POS(cur_obj, &ani_x, &ani_y, &ani_w, &ani_h);; ani_h = (cur_obj->y + cur_obj->offset_by) - ani_y;; ani_yh = ani_y + ani_h; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayEstIlBloque:spr_x:1 | STRONG | Observed local dataflow: GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &spr_x, &spr_y, &spr_w, &spr_h); | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayEstIlBloque:spr_y:1 | STRONG | Observed local dataflow: GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &spr_x, &spr_y, &spr_w, &spr_h);; ani_y = (spr_y + cur_obj->offset_hy) + (ray.follow_id == cur_obj->id); | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayEstIlBloque:spr_w:1 | STRONG | Observed local dataflow: GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &spr_x, &spr_y, &spr_w, &spr_h); | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayEstIlBloque:spr_h:1 | STRONG | Observed local dataflow: GET_SPRITE_POS(cur_obj, cur_obj->follow_sprite, &spr_x, &spr_y, &spr_w, &spr_h); | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayEstIlBloque:ani_xw:1 | STRONG | Observed local dataflow: ani_xw = ani_x + ani_w;; if (((ray.speed_x <= 0) \|\| (!inrange(unk_x, ani_x - ray.speed_x, ani_x + ray.speed_x))) && (!inrange(unk_x, ani_xw + ray.speed_x, ani_xw - ray.speed_x))) | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayEstIlBloque:ani_yh:1 | STRONG | Observed local dataflow: ani_yh = ani_y + ani_h;; if (((((!inrange(unk_y, ani_y, ani_yh)) && ((unk_y += unk_h, !inrange(unk_y, ani_y, ani_yh)))) && ((unk_y += unk_h, !inrange(unk_y, ani_y, ani_yh)))) && ((unk_y += unk_h, !inrange(unk_y, ani_y, ani_yh)))) && ((unk_y += unk_h, !inrange(unk_y, ani_y, ani_yh)))) | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayEstIlBloque:unk_y:1 | STRONG | World y sample starting at cached box top and repeatedly advanced by height/4. | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayEstIlBloque:res:1 | STRONG | Observed local dataflow: res = 0;; res = 1;; return res; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RayEstIlBloque:cur_obj:1 | STRONG | Observed local dataflow: cur_obj = &level.objects[actobj.objects[i]];; if (flags[cur_obj->type].raw & flags2_4_blocks_ray); GET_ANIM_POS(cur_obj, &ani_x, &ani_y, &ani_w, &ani_h); | declared C storage; see exact computation and function contract |  |
| ray.json / function:stackRay | PROVEN | Append Rayman and optional fist display state to the101-entry replay ring. | see parameter/result and side-effect contract | 1:6089c |
| ray.json / local:stackRay:anim_index:1 | PROVEN | Observed local dataflow: cur->anim_index = ray.anim_index;; anim_index = poing_obj->anim_index;; cur->poing_anim_index = anim_index; | declared C storage; see exact computation and function contract |  |
| ray.json / local:stackRay:active:1 | PROVEN | Observed local dataflow: active = poing.is_active;; cur->poing_is_active = active;; if (active) | declared C storage; see exact computation and function contract |  |
| ray.json / local:stackRay:cur:1 | PROVEN | Observed local dataflow: ray_stack_t *cur;; cur = &rayStack[ray_pos_in_stack];; cur->x_pos = ray.x; | declared C storage; see exact computation and function contract |  |
| ray.json / function:RAY_SURF | STRONG | Select fast/slow ground slide animation transitions from horizontal speed. | see parameter/result and side-effect contract | 1:60964 |
| ray.json / local:RAY_SURF:obj:1 | STRONG | Observed local dataflow: if ((obj->type == TYPE_160_TIBETAIN_2) \|\| (obj->type == TYPE_159_TIBETAIN_6)) | declared C storage; see exact computation and function contract |  |
| ray.json / function:DO_SURF_CHANGE_HAIRS | PROVEN | Choose the ground-slide hair animation according to motion and facing. | see parameter/result and side-effect contract | 1:60a8c |
| ray.json / local:DO_SURF_CHANGE_HAIRS:x_sign:1 | PROVEN | Observed local dataflow: if (sgn(ray.speed_x) == x_sign) | declared C storage; see exact computation and function contract |  |
| ray.json / function:DO_PIEDS_RAYMAN | PROVEN | Empty original Rayman feet callback. | see parameter/result and side-effect contract | 1:60ae4 |
| ray.json / local:DO_PIEDS_RAYMAN:obj:1 | UNKNOWN | Unused parameter declaration; no value-consuming role is established. | declared C storage; see exact computation and function contract |  |
| ray.json / function:DO_MORT_DE_RAY | PROVEN | Advance the airborne hurt/death sequence and finish life consumption. | see parameter/result and side-effect contract | 1:60ae8 |
| ray.json / function:TEST_FIN_FOLLOW | PROVEN | Detach a supported Rayman when tile contact blocks the carried motion. | see parameter/result and side-effect contract | 1:60c20 |
| ray.json / local:TEST_FIN_FOLLOW:unk_1:1 | PROVEN | calc_typ_trav selector0 in eta bit6 mode,2 otherwise. | declared C storage; see exact computation and function contract |  |
| ray.json / local:TEST_FIN_FOLLOW:btyp:1 | PROVEN | Observed local dataflow: btyp = (u8) BTYP((ray.x + ray.offset_bx) >> 4, (ray.y + ray.offset_by) >> 4);; btyp = (u8) calc_typ_trav(&ray, unk_1);; if (block_flags[btyp].raw & 2) | declared C storage; see exact computation and function contract |  |
| ray.json / function:RAY_FOLLOW | PROVEN | Add the supporting object's movement to Rayman's own pixel movement. | see parameter/result and side-effect contract | 1:60d58 |
| ray.json / local:RAY_FOLLOW:other_spd_x:1 | PROVEN | Observed local dataflow: other_spd_x = instantSpeed(other_obj->speed_x);; other_spd_x = (u16) other_obj->speed_x;; ray.speed_x = other_spd_x + ray.speed_x; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_FOLLOW:other_spd_y:1 | PROVEN | Observed local dataflow: other_spd_y = instantSpeed(other_obj->speed_y);; other_spd_y = (u16) other_obj->speed_y;; ray.speed_y += other_spd_y; | declared C storage; see exact computation and function contract |  |
| ray.json / local:RAY_FOLLOW:other_obj:1 | PROVEN | Observed local dataflow: other_obj = &level.objects[ray.follow_id];; if (flags[other_obj->type].raw & flags1_0x10_move_x); other_spd_x = instantSpeed(other_obj->speed_x); | declared C storage; see exact computation and function contract |  |
| ray.json / function:RAY_RESPOND_TO_ALL_DIRS | PROVEN | Order direction responses with pan, reversed-input and forced-run rules. | see parameter/result and side-effect contract | 1:60e14 |
| ray.json / function:DO_RAY_ON_MS | PROVEN | Update mounted Rayman movement, fist inputs and collision dispatch. | see parameter/result and side-effect contract | 1:60ff8 |
| ray.json / local:DO_RAY_ON_MS:ray_x:1 | PROVEN | Observed local dataflow: ray_x = ray.x + ray.offset_bx;; ray.ray_dist = ((ray_y >> 4) * mp.width) + (ray_x >> 4); | declared C storage; see exact computation and function contract |  |
| ray.json / local:DO_RAY_ON_MS:ray_y:1 | PROVEN | Observed local dataflow: ray_y = ray.y + ray.offset_by;; ray.ray_dist = ((ray_y >> 4) * mp.width) + (ray_x >> 4); | declared C storage; see exact computation and function contract |  |
| ray.json / function:DO_RAYMAN | STRONG | Order ordinary Rayman input, movement, support, animation and collision work. | see parameter/result and side-effect contract | 1:6133c |
| ray.json / local:DO_RAYMAN:ray_x:1 | STRONG | Observed local dataflow: ray_x = ray.x + ray.offset_bx;; ray.ray_dist = ((ray_y >> 4) * mp.width) + (ray_x >> 4);; GET_SPRITE_POS(&ray, 5, &ray_x, &ray_y, &ray_w, &ray_h); | declared C storage; see exact computation and function contract |  |
| ray.json / local:DO_RAYMAN:ray_y:1 | STRONG | Observed local dataflow: ray_y = ray.y + ray.offset_by;; ray.ray_dist = ((ray_y >> 4) * mp.width) + (ray_x >> 4);; GET_SPRITE_POS(&ray, 5, &ray_x, &ray_y, &ray_w, &ray_h); | declared C storage; see exact computation and function contract |  |
| ray.json / local:DO_RAYMAN:ray_x:2 | STRONG | Observed local dataflow: ray_x = ray.x + ray.offset_bx;; ray.ray_dist = ((ray_y >> 4) * mp.width) + (ray_x >> 4);; GET_SPRITE_POS(&ray, 5, &ray_x, &ray_y, &ray_w, &ray_h); | declared C storage; see exact computation and function contract |  |
| ray.json / local:DO_RAYMAN:ray_y:2 | STRONG | Observed local dataflow: ray_y = ray.y + ray.offset_by;; ray.ray_dist = ((ray_y >> 4) * mp.width) + (ray_x >> 4);; GET_SPRITE_POS(&ray, 5, &ray_x, &ray_y, &ray_w, &ray_h); | declared C storage; see exact computation and function contract |  |
| ray.json / local:DO_RAYMAN:ray_h:1 | STRONG | Observed local dataflow: GET_SPRITE_POS(&ray, 5, &ray_x, &ray_y, &ray_w, &ray_h); | declared C storage; see exact computation and function contract |  |
| ray.json / local:DO_RAYMAN:ray_w:1 | STRONG | Observed local dataflow: GET_SPRITE_POS(&ray, 5, &ray_x, &ray_y, &ray_w, &ray_h);; star_ray_der->x = (star_ray_dev->x = (ray_x + (ray_w >> 1)) - star_ray_dev->offset_bx); | declared C storage; see exact computation and function contract |  |
| types.json / type:obj_t | PROVEN | Packed DOS game object record. | record |  |
| types.json / type:eta_t | PROVEN | One8-byte main/substate entry. | record |  |
| types.json / type:sprite_t | PROVEN | Twelve-byte sprite image/bounds descriptor. | record |  |
| types.json / type:anim_t | PROVEN | Frame/layer animation descriptor. | record |  |
| types.json / type:anim_frame_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| types.json / type:anim_layer_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| types.json / type:obj_flags_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| types.json / type:cmd_context_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| types.json / type:level_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| types.json / type:level_obj_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| types.json / type:active_objects_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| types.json / type:obj_type_flags_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| types.json / type:map_data_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| types.json / type:map_tile_t | PROVEN | Six-byte map cell record with separate visual and collision selectors. | record |  |
| types.json / type:block_flags_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| types.json / type:rayevts_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| types.json / type:finbosslevel_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| types.json / type:poing_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| types.json / type:font_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| types.json / type:display_item_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| types.json / field:obj_t.sprites | PROVEN | Runtime sprite-table pointer; serialized instance slot is a signed16 descriptor index carried in32bits. | sprite-table pointer | +0x0, 4 B |
| types.json / field:obj_t.animations | PROVEN | Runtime animation-table pointer; serialized instance slot is a signed16 descriptor index carried in32bits. | animation-table pointer | +0x4, 4 B |
| types.json / field:obj_t.img_buffer | PROVEN | Runtime atlas pointer; serialized instance slot is a signed16 descriptor index carried in32bits. | byte-buffer pointer | +0x8, 4 B |
| types.json / field:obj_t.eta | PROVEN | Runtime main-state pointer table; serialized instance slot is a signed16 bank index carried in32bits. | eta-bank pointer | +0xc, 4 B |
| types.json / field:obj_t.cmds | PROVEN | Runtime object command byte stream, replacing an ignored serialized producer pointer. | command-byte pointer | +0x10, 4 B |
| types.json / field:obj_t.cmd_labels | PROVEN | Runtime signed16 command-label offset table, replacing an ignored serialized producer pointer. | signed16 offset pointer | +0x14, 4 B |
| types.json / field:obj_t.cmd_contexts | STRONG | Object script stream/context member cmd_contexts. | script dependent | +0x18, 4 B |
| types.json / field:obj_t.field_1C | PROVEN | Overloaded32-bit activation/restore scratch marker. | marker | +0x1c, 4 B |
| types.json / field:obj_t.link_has_gendoor | STRONG | Marker enabling linked-group operations, also selecting the reinitialization delay. | boolean-like32-bit marker | +0x20, 4 B |
| types.json / field:obj_t.is_active | PROVEN | Object participation flag saved as the high bit of each two-bit snapshot entry. | boolean | +0x24, 4 B |
| types.json / field:obj_t.x | PROVEN | Signed32 live-instance world-pixel x; resource-holder wldobj records reuse the same member as atlas byte length. | world pixels / atlas bytes | +0x28, 4 B |
| types.json / field:obj_t.y | PROVEN | Signed32 world pixel y origin in live object movement. | pixels in live objects | +0x2c, 4 B |
| types.json / field:obj_t.active_flag | PROVEN | Activation lifecycle code used by zone transitions: 0 activated/default,1 dead/outside,2 reinitialised awaiting departure,4 waiting for the linked group to leave. | state code | +0x30, 4 B |
| types.json / field:obj_t.id | PROVEN | Object identifier stored in the active and always-object id lists. | object index | +0x34, 2 B |
| types.json / field:obj_t.screen_x | PROVEN | Stored screen-space origin coordinate screen_x after camera subtraction. | pixels | +0x36, 2 B |
| types.json / field:obj_t.screen_y | PROVEN | Stored screen-space origin coordinate screen_y after camera subtraction. | pixels | +0x38, 2 B |
| types.json / field:obj_t.sub_speed | UNKNOWN | Type-dependent signed16 scratch used for a water-motion residual and another type's horizontal distance. | type-dependent pixels / sixteenths | +0x3a, 2 B |
| types.json / field:obj_t.init_x | PROVEN | Signed16 reset x captured from the low16bits of live x by first_obj_init. | world pixels | +0x3c, 2 B |
| types.json / field:obj_t.init_y | PROVEN | Signed16 reset y captured from the low16bits of live y by first_obj_init. | world pixels | +0x3e, 2 B |
| types.json / field:obj_t.speed_x | PROVEN | Signed16 horizontal speed, interpreted per type axis flag as integer pixels/call or sixteenths of a pixel/call. | pixels/call or 1/16 pixels/call | +0x40, 2 B |
| types.json / field:obj_t.speed_y | PROVEN | Signed16 vertical speed, interpreted per type axis flag as integer pixels/call or sixteenths of a pixel/call. | pixels/call or 1/16 pixels/call | +0x42, 2 B |
| types.json / field:obj_t.nb_sprites | PROVEN | Signed16 sprite count in obj_t, including resource holders. | sprite count | +0x44, 2 B |
| types.json / field:obj_t.cmd_offset | STRONG | Object script stream/context member cmd_offset. | script dependent | +0x46, 2 B |
| types.json / field:obj_t.nb_cmd | STRONG | Type-dependent script count or Rayman airborne-inertia selector. | count / selector | +0x48, 2 B |
| types.json / field:obj_t.follow_id | STRONG | Multipurpose signed16 slot: support/object ID in some actors, timing/command parameter in others. | type dependent | +0x4a, 2 B |
| types.json / field:obj_t.follow_y | STRONG | Type-dependent vertical follower displacement or grapple radius. | pixels / type-dependent | +0x4c, 2 B |
| types.json / field:obj_t.follow_x | STRONG | Type-dependent horizontal follower displacement or grapple angle. | pixels / 1/512-turn angle | +0x4e, 2 B |
| types.json / field:obj_t.param | PROVEN | Type-specific signed scratch parameter, including Rayman edge classification in calc_btyp. | context dependent | +0x50, 2 B |
| types.json / field:obj_t.link | UNKNOWN | Multipurpose signed16 scratch; initialization includes paired-object links and unrelated timers/parameters. | type dependent | +0x52, 2 B |
| types.json / field:obj_t.ray_dist | STRONG | Type-dependent Rayman tile index or support-distance code with several geometric conventions. | linear tile index / pixels / code | +0x54, 2 B |
| types.json / field:obj_t.iframes_timer | STRONG | Multipurpose signed16 timer/state slot, including invulnerability, platform delays and command parameters. | type dependent | +0x56, 2 B |
| types.json / field:obj_t.test_block_index | PROVEN | Last row-major map tile index used by the changed-block test. | tile index | +0x58, 2 B |
| types.json / field:obj_t.scale | PROVEN | Projection-depth parameter whose denominator is signed_scale+256. | projection depth units | +0x5a, 2 B |
| types.json / field:obj_t.zdc | PROVEN | Packed reference to an object collision-zone run. | packed count/index | +0x5c, 2 B |
| types.json / field:obj_t.active_timer | PROVEN | Object reactivation-delay counter. | engine calls | +0x5e, 2 B |
| types.json / field:obj_t.type | PROVEN | Unsigned object type discriminator indexing flags and selecting behaviour. | object type id | +0x60, 2 B |
| types.json / field:obj_t.btypes | PROVEN | Five cached collision block types in centre,left,right,above,below order. | block type id | +0x62, 5 B |
| types.json / field:obj_t.unused_padding | UNKNOWN | Neutral obj_t.unused_padding storage/view; its intended meaning is not established by this pilot. | unknown | +0x67, 1 B |
| types.json / field:obj_t.offset_bx | PROVEN | Horizontal object anchor offset used for collision probes and reflection centre. | pixels | +0x68, 1 B |
| types.json / field:obj_t.offset_by | PROVEN | Vertical object anchor offset used for foot/block probes and projection centre. | pixels | +0x69, 1 B |
| types.json / field:obj_t.anim_index | PROVEN | Current animation selector or current frame selector (anim_index). | index | +0x6a, 1 B |
| types.json / field:obj_t.anim_frame | PROVEN | Current animation selector or current frame selector (anim_frame). | index | +0x6b, 1 B |
| types.json / field:obj_t.sub_etat | PROVEN | Object-type- and main-state-specific substate-table index. | state index | +0x6c, 1 B |
| types.json / field:obj_t.main_etat | PROVEN | Object-type-specific main state-table index; Rayman's controller dispatches eight observed state families. | state index | +0x6d, 1 B |
| types.json / field:obj_t.init_sub_etat | PROVEN | Substate reset byte captured by first_obj_init and modified by some type-specific defaults. | substate index | +0x6e, 1 B |
| types.json / field:obj_t.init_main_etat | PROVEN | Main-state reset byte captured by first_obj_init. | main-state index | +0x6f, 1 B |
| types.json / field:obj_t.cmd | STRONG | Current script command discriminator. | opcode | +0x70, 1 B |
| types.json / field:obj_t.gravity_value_1 | STRONG | First type-dependent gravity/control phase byte. | phase/control byte | +0x71, 1 B |
| types.json / field:obj_t.gravity_value_2 | STRONG | Second type-dependent gravity/control phase byte. | phase/control byte | +0x72, 1 B |
| types.json / field:obj_t.change_anim_mode | PROVEN | Animation transition mode set to1 by the state setters. | mode | +0x73, 1 B |
| types.json / field:obj_t.offset_hy | STRONG | Unsigned vertical upper-body/collision anchor offset. | pixels | +0x74, 1 B |
| types.json / field:obj_t.follow_sprite | PROVEN | Selected animation layer for following, also overloaded as Blacktoon behaviour selector. | layer index or subtype | +0x75, 1 B |
| types.json / field:obj_t.hit_points | PROVEN | Unsigned hit-point byte in obj_hurt, with type-specific reuse including signed deformation angle input. | hit points or type-specific state | +0x76, 1 B |
| types.json / field:obj_t.init_hit_points | PROVEN | Reset byte captured from hit_points by first_obj_init. | type-dependent initial parameter | +0x77, 1 B |
| types.json / field:obj_t.init_flag | UNKNOWN | Serialized byte with initializer writes of7; no intended common meaning or typed reader is established. | unknown | +0x78, 1 B |
| types.json / field:obj_t.hit_sprite | PROVEN | Collision layer selector with three special dispatch codes. | layer id / sentinel | +0x79, 1 B |
| types.json / field:obj_t.detect_zone | PROVEN | Type-dependent unsigned8 detection-range parameter with initialization defaults. | pixels/type parameter | +0x7a, 1 B |
| types.json / field:obj_t.detect_zone_flag | PROVEN | Detection-zone classification tested by OBJ_IN_ZONE. | classification | +0x7b, 1 B |
| types.json / field:obj_t.cmd_context_depth | STRONG | Object script stream/context member cmd_context_depth. | script dependent | +0x7c, 1 B |
| types.json / field:obj_t.config | PROVEN | Type-specific byte configuration tested for Blacktoon edge-turning. | context dependent | +0x7d, 1 B |
| types.json / field:obj_t.display_prio | STRONG | Object drawing-order bucket selector. | priority bucket | +0x7e, 1 B |
| types.json / field:obj_t.timer | UNKNOWN | Multipurpose byte countdown/state scratch with common reset0 and actor-specific defaults. | type dependent | +0x7f, 1 B |
| types.json / field:obj_t.anim_count | PROVEN | Unsigned8 animation count loaded into resource-holder records. | animation count | +0x80, 1 B |
| types.json / field:obj_t.flags | PROVEN | Packed two-byte per-object flag record. | bitset | +0x81, 2 B |
| types.json / field:obj_t.field_83 | UNKNOWN | Neutral obj_t.field_83 storage/view; its intended meaning is not established by this pilot. | unknown | +0x83, 1 B |
| types.json / field:eta_t.speed_x_right | STRONG | Signed state-selected horizontal speed for the speed_x_right facing. | object speed units | +0x0, 1 B |
| types.json / field:eta_t.speed_x_left | STRONG | Signed state-selected horizontal speed for the speed_x_left facing. | object speed units | +0x1, 1 B |
| types.json / field:eta_t.anim_index | PROVEN | Animation id selected by this main/substate entry. | animation index | +0x2, 1 B |
| types.json / field:eta_t.next_main_etat | PROVEN | State indices selected when an animation crosses its frame bounds. | state index | +0x3, 1 B |
| types.json / field:eta_t.next_sub_etat | PROVEN | State indices selected when an animation crosses its frame bounds. | state index | +0x4, 1 B |
| types.json / field:eta_t.anim_speed | PROVEN | Packed state timing/control byte: low nibble selects the animation clock, high nibble selects the airborne gravity/control mode. | two selectors | +0x5, 1 B |
| types.json / field:eta_t.anim_speed_bits | PROVEN | Overlapping nibble/bit alias container for the corresponding eta byte. | bitset | +0x5, 1 B |
| types.json / field:eta_t.anim_speed_bits.low | PROVEN | Low nibble alias of the animation timing byte. | clock index | +0x5, 1 B |
| types.json / field:eta_t.anim_speed_bits.high | PROVEN | High-nibble gravity/control selector consumed by DO_PESANTEUR and specialized state logic. | selector0..15 | +0x5, 1 B |
| types.json / field:eta_t.sound_index | STRONG | State-selected sound event index. | sound id | +0x6, 1 B |
| types.json / field:eta_t.flags | PROVEN | State behaviour bitset, including reverse playback0x10 and Rayman rectangle selection0x40. | bitset | +0x7, 1 B |
| types.json / field:eta_t.flags_bits | PROVEN | Overlapping nibble/bit alias container for the corresponding eta byte. | bitset | +0x7, 1 B |
| types.json / field:eta_t.flags_bits.bit0 | STRONG | State permission bit tested by Rayman jump initiation and the stonewoman jump handler. | permission bit | +0x7, 1 B |
| types.json / field:eta_t.flags_bits.bit1 | STRONG | State permission bit gating fist preparation in RAY_PREPARE_FIST. | permission bit | +0x7, 1 B |
| types.json / field:eta_t.flags_bits.bit2 | STRONG | State permission bit gating helicopter activation. | permission bit | +0x7, 1 B |
| types.json / field:eta_t.flags_bits.bit3 | STRONG | State participation bit for damage/contact eligibility, with actor-dependent readers. | state flag | +0x7, 1 B |
| types.json / field:eta_t.flags_bits.bit4 | PROVEN | Reverse animation playback flag0x10, selecting decrement and last-frame initialization. | boolean | +0x7, 1 B |
| types.json / field:eta_t.flags_bits.bit5 | STRONG | State gate for Rayman contact dispatch and collision-zone traversal. | participation bit | +0x7, 1 B |
| types.json / field:eta_t.flags_bits.bit6 | PROVEN | Context-dependent state bit selecting Rayman's low collision shape and participating in generating-door inset encoding. | state flag | +0x7, 1 B |
| types.json / field:eta_t.flags_bits.bit7 | STRONG | Low-weight bit of generating-door collision inset selection, paired with bit6. | packed selector bit | +0x7, 1 B |
| types.json / field:sprite_t.offset_in_atlas | PROVEN | Unsigned32 byte offset of sprite pixels relative to the atlas base. | atlas bytes | +0x0, 4 B |
| types.json / field:sprite_t.id | PROVEN | Sprite reference id, with0 treated as an absent sprite in GET_SPRITE_POS. | reference id | +0x4, 1 B |
| types.json / field:sprite_t.outer_width | STRONG | Outer sprite rectangle dimension outer_width. | pixels | +0x5, 1 B |
| types.json / field:sprite_t.outer_height | STRONG | Outer sprite rectangle dimension outer_height. | pixels | +0x6, 1 B |
| types.json / field:sprite_t.inner_width | PROVEN | Returned inner sprite rectangle dimension inner_width. | pixels | +0x7, 1 B |
| types.json / field:sprite_t.inner_height | PROVEN | Returned inner sprite rectangle dimension inner_height. | pixels | +0x8, 1 B |
| types.json / field:sprite_t.sprite_pos | PROVEN | Overlapping raw/nibble views of the sprite inset byte. | bitset | +0x9, 1 B |
| types.json / field:sprite_t.sprite_pos.raw | PROVEN | Overlapping raw/nibble views of the sprite inset byte. | bitset | +0x9, 1 B |
| types.json / field:sprite_t.sprite_pos.bits | PROVEN | Overlapping raw/nibble views of the sprite inset byte. | bitset | +0x9, 1 B |
| types.json / field:sprite_t.sprite_pos.bits.x | PROVEN | Packed nonnegative sprite inset component sprite_pos.bits.x. | pixels | +0x9, 1 B |
| types.json / field:sprite_t.sprite_pos.bits.y | PROVEN | Packed nonnegative sprite inset component sprite_pos.bits.y. | pixels | +0x9, 1 B |
| types.json / field:sprite_t.color | PROVEN | Raw/bitfield view of the sprite color control byte. | bitset | +0xa, 1 B |
| types.json / field:sprite_t.color_bits | PROVEN | Raw/bitfield view of the sprite color_bits control byte. | bitset | +0xa, 1 B |
| types.json / field:sprite_t.color_bits.unused | UNKNOWN | Neutral sprite_t.color_bits.unused storage/view; its intended meaning is not established by this pilot. | unknown | +0xa, 1 B |
| types.json / field:sprite_t.color_bits.palette_index | STRONG | High-nibble palette selection. | palette index | +0xa, 1 B |
| types.json / field:sprite_t.flags | PROVEN | Raw/bitfield view of the sprite flags control byte. | bitset | +0xb, 1 B |
| types.json / field:sprite_t.flags_bits | PROVEN | Raw/bitfield view of the sprite flags_bits control byte. | bitset | +0xb, 1 B |
| types.json / field:sprite_t.flags_bits.unused0 | UNKNOWN | Neutral sprite_t.flags_bits.unused0 storage/view; its intended meaning is not established by this pilot. | unknown | +0xb, 1 B |
| types.json / field:sprite_t.flags_bits.flip_x | PROVEN | Sprite horizontal flip bit XORed with object flip to select the bounds reflection. | boolean | +0xb, 1 B |
| types.json / field:sprite_t.flags_bits.unused3 | UNKNOWN | Neutral sprite_t.flags_bits.unused3 storage/view; its intended meaning is not established by this pilot. | unknown | +0xb, 1 B |
| types.json / field:anim_t.layers | PROVEN | Runtime pointer to the allocated animation layer payload. | byte/4-byte-layer pointer | +0x0, 4 B |
| types.json / field:anim_t.frames | PROVEN | Runtime frame-rectangle pointer, with serialized32-bit-1 sentinel indicating no rectangle payload. | frame-rectangle pointer or-1 | +0x4, 4 B |
| types.json / field:anim_t.layers_per_frame | PROVEN | Packed animation header: low14 bits are layer count per frame, top2 bits encode the horizontal-speed clock period minus one. | layer count / period selector | +0x8, 2 B |
| types.json / field:anim_t.frames_count | PROVEN | Number of frames in this animation. | frames | +0xa, 2 B |
| types.json / field:anim_frame_t.x | PROVEN | Unsigned offset of the animation bounding rectangle (x). | pixels | +0x0, 1 B |
| types.json / field:anim_frame_t.y | PROVEN | Unsigned offset of the animation bounding rectangle (y). | pixels | +0x1, 1 B |
| types.json / field:anim_frame_t.width | PROVEN | Animation bounding rectangle dimension width. | pixels | +0x2, 1 B |
| types.json / field:anim_frame_t.height | PROVEN | Animation bounding rectangle dimension height. | pixels | +0x3, 1 B |
| types.json / field:anim_layer_t.mirrored | STRONG | Layer horizontal mirror selector used by the renderer. | boolean-like byte | +0x0, 1 B |
| types.json / field:anim_layer_t.x | PROVEN | Unsigned object-relative layer offset component x. | pixels | +0x1, 1 B |
| types.json / field:anim_layer_t.y | PROVEN | Unsigned object-relative layer offset component y. | pixels | +0x2, 1 B |
| types.json / field:anim_layer_t.sprite_index | PROVEN | Index into the owning object sprite table for this layer. | sprite index | +0x3, 1 B |
| types.json / field:obj_flags_t.hurtbyfist | STRONG | Fist-hit interaction flag. | boolean | +0x0, 1 B |
| types.json / field:obj_flags_t.command_test | STRONG | Stored result of the current script test. | boolean | +0x0, 1 B |
| types.json / field:obj_flags_t.alive | PROVEN | Low saved object-state bit, cleared at timed animation completion. | boolean | +0x0, 1 B |
| types.json / field:obj_flags_t.flip_x | PROVEN | Horizontal object reflection/facing flag. | boolean | +0x0, 1 B |
| types.json / field:obj_flags_t.read_commands | PROVEN | Per-object command mode used to select turn labels. | boolean | +0x0, 1 B |
| types.json / field:obj_flags_t.follow_enabled | STRONG | Enable support/follow-distance processing for the object. | boolean | +0x0, 1 B |
| types.json / field:obj_flags_t.flag_0x40 | STRONG | Water-entry/splash suppression flag in the generic movement path; also cleared by many spawned-object initializers. | boolean marker | +0x0, 1 B |
| types.json / field:obj_flags_t.anim_changed | STRONG | Flag used by animation update when the selected animation changes. | boolean | +0x0, 1 B |
| types.json / field:obj_flags_t.flag_0x100 | PROVEN | Ninth object flag written as movement-occurrence bookkeeping, with one original uninitialised producer. | boolean bit | +0x1, 1 B |
| types.json / field:cmd_context_t.cmd_offset | STRONG | Saved script command-context component cmd_offset. | script offset or repeat count | +0x0, 2 B |
| types.json / field:cmd_context_t.count | STRONG | Saved script command-context component count. | script offset or repeat count | +0x2, 2 B |
| types.json / field:level_t.objects | PROVEN | Base pointer of the packed level object array. | pointer | +0x0, 4 B |
| types.json / field:level_t.nb_objects | PROVEN | Count bounding the level object array. | objects | +0x4, 2 B |
| types.json / field:level_obj_t.obj_ids | PROVEN | Object-id sequence pointer shared by the level_obj activation-candidate descriptor and the separate level_alw always-type pool descriptor. | object ids | +0x0, 4 B |
| types.json / field:level_obj_t.nb_objects | PROVEN | Signed16 counted length of the id sequence in either level_obj or level_alw. | objects | +0x4, 2 B |
| types.json / field:active_objects_t.objects | PROVEN | Counted active-object id array with an additional -1 terminator written at the next slot. | object ids | +0x0, 200 B |
| types.json / field:active_objects_t.num_active_objects | PROVEN | Active-object list member num_active_objects. | object ids or count | +0xc8, 2 B |
| types.json / field:obj_type_flags_t.raw | PROVEN | Overlapping raw/byte/nibble view raw of per-type behaviour flags. | bitset | +0x0, 4 B |
| types.json / field:obj_type_flags_t.flags0 | PROVEN | Overlapping raw/byte/nibble view flags0 of per-type behaviour flags. | bitset | +0x0, 1 B |
| types.json / field:obj_type_flags_t.flags1 | PROVEN | Overlapping raw/byte/nibble view flags1 of per-type behaviour flags. | bitset | +0x1, 1 B |
| types.json / field:obj_type_flags_t.flags1_lo | PROVEN | Overlapping raw/byte/nibble view flags1_lo of per-type behaviour flags. | bitset | +0x1, 1 B |
| types.json / field:obj_type_flags_t.move_x | PROVEN | Select fractional speed conversion for the corresponding object movement axis. | boolean | +0x1, 1 B |
| types.json / field:obj_type_flags_t.move_y | PROVEN | Select fractional speed conversion for the corresponding object movement axis. | boolean | +0x1, 1 B |
| types.json / field:obj_type_flags_t.flags1_hi | PROVEN | Overlapping raw/byte/nibble view flags1_hi of per-type behaviour flags. | bitset | +0x1, 1 B |
| types.json / field:obj_type_flags_t.flags2 | PROVEN | Overlapping raw/byte/nibble view flags2 of per-type behaviour flags. | bitset | +0x2, 1 B |
| types.json / field:obj_type_flags_t.flags3 | PROVEN | Overlapping raw/byte/nibble view flags3 of per-type behaviour flags. | bitset | +0x3, 1 B |
| types.json / field:map_data_t.width | PROVEN | Map dimension width in16-pixel tiles. | tiles | +0x0, 2 B |
| types.json / field:map_data_t.height | PROVEN | Map dimension height in16-pixel tiles. | tiles | +0x2, 2 B |
| types.json / field:map_data_t.length | STRONG | Number of tile records used by full-map processing. | tiles | +0x4, 4 B |
| types.json / field:map_data_t.map | PROVEN | Pointer to row-major six-byte tile records. | pointer | +0x8, 4 B |
| types.json / field:map_tile_t.texture_id | STRONG | Index of a visual16x16 tile texture. | texture index | +0x0, 2 B |
| types.json / field:map_tile_t.tile_type | PROVEN | Collision/behaviour type byte returned by BTYP. | block type id | +0x2, 1 B |
| types.json / field:map_tile_t.field_3 | UNKNOWN | Raw serialized map byte+3 with no intended meaning assigned. | unknown | +0x3, 1 B |
| types.json / field:map_tile_t.transparency | PROVEN | Computed texture coverage classification:0 opaque,1 empty,2 mixed. | classification | +0x4, 1 B |
| types.json / field:map_tile_t.field_5 | UNKNOWN | Raw serialized map byte+5 with no intended meaning assigned. | unknown | +0x5, 1 B |
| types.json / field:block_flags_t.raw | PROVEN | Byte/bitfield views of block-type behaviour flags. | bitset | +0x0, 1 B |
| types.json / field:block_flags_t.bits | PROVEN | Byte/bitfield views of block-type behaviour flags. | bitset | +0x0, 1 B |
| types.json / field:block_flags_t.bits.unused0 | UNKNOWN | Observed raw bit0 used by underSlope, stone-dog side probes and mounted Rayman side-strip probes; its universal collision-class meaning remains unresolved. | tile-class bit | +0x0, 1 B |
| types.json / field:block_flags_t.bits.solid | PROVEN | Bit1 tested as the supporting/solid classification in block selection. | boolean | +0x0, 1 B |
| types.json / field:block_flags_t.bits.unused2 | UNKNOWN | Three-bit view of raw bits2..4; moteur tests bit3 together with bit6 for icy slopes, and bit4 for ceiling/wall blocking. Bit2 meaning is unresolved. | packed tile-class bits | +0x0, 1 B |
| types.json / field:block_flags_t.bits.chdir | PROVEN | Bit5 of the probed block flags, returned by the turn-marker test. | boolean | +0x0, 1 B |
| types.json / field:block_flags_t.bits.unused1 | STRONG | Two-bit slope classification/orientation view used during Rayman tile landing. | packed slope bits | +0x0, 1 B |
| types.json / field:rayevts_t.poing | STRONG | Enable the fist-throwing ability. | boolean | +0x0, 1 B |
| types.json / field:rayevts_t.hang | STRONG | Enable the ledge-hanging ability. | boolean | +0x0, 1 B |
| types.json / field:rayevts_t.helico | STRONG | Enable normal/enhanced helicopter response (helico). | boolean | +0x0, 1 B |
| types.json / field:rayevts_t.super_helico | STRONG | Enable normal/enhanced helicopter response (super_helico). | boolean | +0x0, 1 B |
| types.json / field:rayevts_t.handstand_dash | UNKNOWN | Neutral rayevts_t.handstand_dash storage/view; its intended meaning is not established by this pilot. | unknown | +0x0, 1 B |
| types.json / field:rayevts_t.handstand | UNKNOWN | Neutral rayevts_t.handstand storage/view; its intended meaning is not established by this pilot. | unknown | +0x0, 1 B |
| types.json / field:rayevts_t.magicseed | STRONG | Enable the level-specific magic-seed ability. | boolean | +0x0, 1 B |
| types.json / field:rayevts_t.grap | STRONG | Enable grabbing/swinging through the grappling interaction. | boolean | +0x0, 1 B |
| types.json / field:rayevts_t.run | STRONG | Enable Rayman running behaviour. | boolean | +0x1, 1 B |
| types.json / field:rayevts_t.tiny | STRONG | Rayman small-size mode/power bit. | boolean | +0x1, 1 B |
| types.json / field:rayevts_t.firefly | STRONG | Enable the firefly/light companion mode. | boolean | +0x1, 1 B |
| types.json / field:rayevts_t.force_run | PROVEN | Two-bit forced-running lifecycle state. | state code | +0x1, 1 B |
| types.json / field:rayevts_t.reverse | PROVEN | Two-bit reversed-controls lifecycle state whose nonzero values swap horizontal input directions. | state code | +0x1, 1 B |
| types.json / field:rayevts_t.squashed | STRONG | Reduced/squashed Rayman mode coupled to tiny and object/ability-state swapping. | boolean | +0x1, 1 B |
| types.json / field:finbosslevel_t.bzzit | STRONG | Progression flag for completion of the bzzit encounter. | boolean | +0x0, 1 B |
| types.json / field:finbosslevel_t.moskito | STRONG | Progression flag for completion of the moskito encounter. | boolean | +0x0, 1 B |
| types.json / field:finbosslevel_t.mr_sax | STRONG | Progression flag for completion of the mr_sax encounter. | boolean | +0x0, 1 B |
| types.json / field:finbosslevel_t.mr_stone | STRONG | Progression flag for completion of the mr_stone encounter. | boolean | +0x0, 1 B |
| types.json / field:finbosslevel_t.viking_mama | STRONG | Progression flag for completion of the viking_mama encounter. | boolean | +0x0, 1 B |
| types.json / field:finbosslevel_t.space_mama | STRONG | Progression flag for completion of the space_mama encounter. | boolean | +0x0, 1 B |
| types.json / field:finbosslevel_t.mr_skops | STRONG | Progression flag for completion of the mr_skops encounter. | boolean | +0x0, 1 B |
| types.json / field:finbosslevel_t.mr_dark | STRONG | Progression flag for completion of the mr_dark encounter. | boolean | +0x0, 1 B |
| types.json / field:finbosslevel_t.crazy_drummer | UNKNOWN | Neutral finbosslevel_t.crazy_drummer storage/view; its intended meaning is not established by this pilot. | unknown | +0x1, 1 B |
| types.json / field:finbosslevel_t.helped_joe_1 | STRONG | Progression flag for the corresponding help event (helped_joe_1). | boolean | +0x1, 1 B |
| types.json / field:finbosslevel_t.helped_joe_2 | STRONG | Progression flag for the corresponding help event (helped_joe_2). | boolean | +0x1, 1 B |
| types.json / field:finbosslevel_t.helped_musician | STRONG | Progression flag for the corresponding help event (helped_musician). | boolean | +0x1, 1 B |
| types.json / field:poing_t.y_16 | PROVEN | Fist signed32 vertical position accumulator scaled by16, separately maintained from integer obj.y. | sixteenths of pixel | +0x0, 4 B |
| types.json / field:poing_t.field_4 | UNKNOWN | Neutral poing_t.field_4 storage/view; its intended meaning is not established by this pilot. | unknown | +0x4, 2 B |
| types.json / field:poing_t.speed_x | PROVEN | Stored positive horizontal fist speed used to launch and constrain return speed. | pixels/tick | +0x6, 2 B |
| types.json / field:poing_t.charge | PROVEN | Fist charge counter before throwing, then scaled outbound budget decremented during flight. | charge ticks then scaled budget | +0x8, 2 B |
| types.json / field:poing_t.field_A | PROVEN | Temporary throw-range denominator then decrement applied to the scaled outbound charge budget. | context-dependent budget units | +0xa, 2 B |
| types.json / field:poing_t.sub_etat | STRONG | Saved/selected fist upgrade substate copied into fist objects and save data. | substate selector | +0xc, 1 B |
| types.json / field:poing_t.is_returning | PROVEN | Select the homing-return phase of fist flight. | boolean-like byte | +0xd, 1 B |
| types.json / field:poing_t.is_active | PROVEN | Enable fist update/collision processing. | boolean-like byte | +0xe, 1 B |
| types.json / field:poing_t.damage | PROVEN | Current fist damage applied by obj_hurt. | hit points | +0xf, 1 B |
| types.json / field:poing_t.is_charging | STRONG | Fist charge-preparation flag. | boolean-like byte | +0x10, 1 B |
| types.json / field:poing_t.is_boum | STRONG | Fist impact/explosion interaction state. | boolean-like byte | +0x11, 1 B |
| types.json / field:poing_t.field_12 | UNKNOWN | Neutral poing_t.field_12 storage/view; its intended meaning is not established by this pilot. | unknown | +0x12, 1 B |
| types.json / field:poing_t.field_13 | UNKNOWN | Neutral poing_t.field_13 storage/view; its intended meaning is not established by this pilot. | unknown | +0x13, 1 B |
| types.json / field:font_t.sprites | PROVEN | Base sprite table for the corresponding font bank. | pointer | +0x0, 4 B |
| types.json / field:font_t.img_buffer | STRONG | Font image buffer or sprite count (img_buffer) copied from loaded object descriptors. | pointer or count | +0x4, 4 B |
| types.json / field:font_t.nb_sprites | STRONG | Font image buffer or sprite count (nb_sprites) copied from loaded object descriptors. | pointer or count | +0x8, 2 B |
| types.json / field:display_item_t.text | PROVEN | Fixed200-byte text buffer interpreted by the game glyph decoder. | encoded bytes | +0x0, 200 B |
| types.json / field:display_item_t.centered_x_pos | PROVEN | Computed text-box rectangle component centered_x_pos. | screen pixels | +0xc8, 2 B |
| types.json / field:display_item_t.centered_y_pos | PROVEN | Computed text-box rectangle component centered_y_pos. | screen pixels | +0xca, 2 B |
| types.json / field:display_item_t.width | PROVEN | Computed text-box rectangle component width. | screen pixels | +0xcc, 2 B |
| types.json / field:display_item_t.height | PROVEN | Computed text-box rectangle component height. | screen pixels | +0xce, 2 B |
| types.json / field:display_item_t.xpos | PROVEN | Text anchor coordinate xpos. | screen pixels | +0xd0, 2 B |
| types.json / field:display_item_t.ypos | PROVEN | Text anchor coordinate ypos. | screen pixels | +0xd2, 2 B |
| types.json / field:display_item_t.font_size | PROVEN | Font selector controlling bank and line metrics. | font selector | +0xd4, 1 B |
| types.json / field:display_item_t.field_D5 | PROVEN | Byte selecting the palette255 box branch instead of the ordinary palette254/no-box branch. | boolean-like selector | +0xd5, 1 B |
| types.json / field:display_item_t.is_fond | PROVEN | Box drawing/position mode passed to DISPLAY_BLACKBOX and used to offset text by +/-1. | mode | +0xd6, 1 B |
| types.json / field:display_item_t.color | STRONG | Text palette/color selector passed to display_text. | palette selector | +0xd7, 1 B |
| types.json / type:status_bar_t | PROVEN | Packed HUD counters and derived digit/health selectors. | record |  |
| types.json / type:save_state_t | PROVEN | Packed604-byte checkpoint image consumed by saveGameState/restoreGameState; fields include circular links and Rayman state. | record |  |
| types.json / type:record_t | PROVEN | Packed24-byte demo-input stream decoder/recording descriptor. | record |  |
| types.json / type:mem_t | PROVEN | Packed12-byte block-allocation arena descriptor. | record |  |
| types.json / type:pix_gerbe_item_t | PROVEN | Packed10-byte particle item storage; only its aggregate clear is used in moteur. | record |  |
| types.json / type:pix_gerbe_t | PROVEN | Packed642-byte particle batch with64 ten-byte items/320-word alias and a signed16 active indicator. | record |  |
| types.json / type:obj_procs_t | PROVEN | Packed20-byte per-object-type callback dispatch record with five32-bit function pointers. | record |  |
| types.json / type:flocon_t | PROVEN | Packed8-byte local snow-record declaration; no typed field access in this unit. | record |  |
| types.json / field:status_bar_t.lives | PROVEN | Signed life counter with -1 display sentinel and upper gameplay cap99. | lives | +0x0, 2 B |
| types.json / field:status_bar_t.lives_digits | PROVEN | Tens then units decimal display digits of lives. | digits | +0x2, 2 B |
| types.json / field:status_bar_t.hp_sprites | PROVEN | Two HUD health-sprite selectors, slot1 for the first two health points and slot0 for extra health. | sprite selector | +0x4, 2 B |
| types.json / field:status_bar_t.num_wiz | PROVEN | Collected tings count used by the HUD and bonus/checkpoint rules. | tings | +0x6, 1 B |
| types.json / field:status_bar_t.wiz_digits | PROVEN | Tens then units decimal display digits for the tings count. | digits | +0x7, 2 B |
| types.json / field:status_bar_t.max_hitp | PROVEN | HUD maximum health class, normally2 or4. | health points | +0x9, 1 B |
| types.json / field:save_state_t.triggered_objects | PROVEN | Alive-bit snapshot for object types141,164,179,242 indexed by object id bit. | object bitset | +0x0, 32 B |
| types.json / field:save_state_t.nb_floc | PROVEN | Saved eight weather-layer flake counts. | flakes per layer | +0x20, 16 B |
| types.json / field:save_state_t.vent_x | PROVEN | Saved VENT_X weather/wind value. | weather units | +0x30, 2 B |
| types.json / field:save_state_t.vent_y | PROVEN | Saved VENT_Y weather/wind value. | weather units | +0x32, 2 B |
| types.json / field:save_state_t.x_map | PROVEN | Saved horizontal camera origin. | pixels | +0x34, 2 B |
| types.json / field:save_state_t.y_map | PROVEN | Saved vertical camera origin. | pixels | +0x36, 2 B |
| types.json / field:save_state_t.ray_x_pos | PROVEN | Saved Rayman world x narrowed to signed16. | pixels | +0x38, 2 B |
| types.json / field:save_state_t.ray_y_pos | PROVEN | Saved Rayman world y narrowed to signed16. | pixels | +0x3a, 2 B |
| types.json / field:save_state_t.ray_screen_x | PROVEN | Saved Rayman screen x. | pixels | +0x3c, 2 B |
| types.json / field:save_state_t.ray_screen_y | PROVEN | Saved Rayman screen y. | pixels | +0x3e, 2 B |
| types.json / field:save_state_t.ray_flip_x | PROVEN | Saved Rayman facing flag widened to u16. | boolean | +0x40, 2 B |
| types.json / field:save_state_t.save_obj_x_pos | PROVEN | Saved checkpoint-object world x narrowed to signed16. | pixels | +0x42, 2 B |
| types.json / field:save_state_t.save_obj_y_pos | PROVEN | Saved checkpoint-object world y narrowed to signed16. | pixels | +0x44, 2 B |
| types.json / field:save_state_t.save_obj_id | PROVEN | Checkpoint-object id, with-1 when saveGameState receives no object. | object id / -1 | +0x46, 2 B |
| types.json / field:save_state_t.link_init | PROVEN | Snapshot of live circular successor ids, stored as u16. | object ids | +0x48, 512 B |
| types.json / field:save_state_t.save_obj_detect_zone_flag | PROVEN | Saved checkpoint-object detection-zone classification. | classification | +0x248, 1 B |
| types.json / field:save_state_t.save_obj_flag_1 | PROVEN | Saved checkpoint-object hurtbyfist bit, despite the neutral numbered spelling. | boolean | +0x249, 1 B |
| types.json / field:save_state_t.ray_coll_btype | PROVEN | Saved five Rayman tile-collision sample types. | tile types | +0x24a, 5 B |
| types.json / field:save_state_t.ray_anim_index | PROVEN | Saved Rayman animation selector. | animation index | +0x24f, 1 B |
| types.json / field:save_state_t.ray_anim_frame | PROVEN | Saved Rayman animation frame selector. | frame index | +0x250, 1 B |
| types.json / field:save_state_t.ray_main_etat | PROVEN | Saved Rayman main state. | state index | +0x251, 1 B |
| types.json / field:save_state_t.ray_sub_etat | PROVEN | Saved Rayman substate. | state index | +0x252, 1 B |
| types.json / field:save_state_t.poing_sub_etat | PROVEN | Saved fist power/substate selection. | fist state | +0x253, 1 B |
| types.json / field:save_state_t.rayevts_reverse | PROVEN | Saved two-bit RayEvts.reverse value widened to a byte. | control state | +0x254, 1 B |
| types.json / field:save_state_t.rayevts_super_helico | PROVEN | Saved RayEvts.super_helico bit. | boolean | +0x255, 1 B |
| types.json / field:save_state_t.rayevts_poing | PROVEN | Saved RayEvts.poing ability bit. | boolean | +0x256, 1 B |
| types.json / field:save_state_t.current_pal_id | PROVEN | Saved palette selector. | palette selector | +0x257, 1 B |
| types.json / field:save_state_t.dead_time | PROVEN | Saved entry/death sequence counter. | sequence calls | +0x258, 1 B |
| types.json / field:save_state_t.is_just_saved | PROVEN | Checkpoint-valid marker set after a save image is filled. | boolean | +0x259, 1 B |
| types.json / field:save_state_t.status_bar_tings | PROVEN | Saved HUD tings count. | tings | +0x25a, 1 B |
| types.json / field:save_state_t.field_25b | UNKNOWN | Trailing checkpoint byte with no typed reader/writer found. | unknown | +0x25b, 1 B |
| types.json / field:record_t.current_offset | STRONG | Current byte cursor into the recorded demo stream. | bytes | +0x0, 4 B |
| types.json / field:record_t.repeat_length | STRONG | Length of the current repeated demo-input run. | input updates | +0x4, 4 B |
| types.json / field:record_t.repeat_index | STRONG | Progress within the current repeated demo-input run. | input updates | +0x8, 4 B |
| types.json / field:record_t.length | STRONG | Recorded demo stream length used by input playback. | bytes | +0xc, 4 B |
| types.json / field:record_t.data | STRONG | Demo stream byte buffer pointer. | pointer | +0x10, 4 B |
| types.json / field:record_t.is_recording | PROVEN | Nonzero while demo input recording is enabled. | boolean | +0x14, 1 B |
| types.json / field:record_t.is_playing | PROVEN | Nonzero while recorded demo input is being played. | boolean | +0x15, 1 B |
| types.json / field:record_t.is_finished | PROVEN | Request to stop demo playback at the current repeat-run boundary. | boolean | +0x16, 1 B |
| types.json / field:record_t.unknown_17 | UNKNOWN | Declared record_t.unknown_17 storage; moteur only clears its enclosing storage or never accesses it. | unknown | +0x17, 1 B |
| types.json / field:mem_t.align | PROVEN | Used-byte bump offset relative to arena data, despite the alignment-oriented field name. | bytes | +0x0, 4 B |
| types.json / field:mem_t.data | PROVEN | Arena base allocation pointer, with the live bump offset stored separately in align. | pointer | +0x4, 4 B |
| types.json / field:mem_t.capacity | PROVEN | Total arena byte capacity against which the bump offset is checked. | bytes | +0x8, 4 B |
| types.json / field:pix_gerbe_item_t.x_pos | UNKNOWN | Declared pix_gerbe_item_t.x_pos storage; moteur only clears its enclosing storage or never accesses it. | unknown | +0x0, 2 B |
| types.json / field:pix_gerbe_item_t.y_pos | UNKNOWN | Declared pix_gerbe_item_t.y_pos storage; moteur only clears its enclosing storage or never accesses it. | unknown | +0x2, 2 B |
| types.json / field:pix_gerbe_item_t.speed_x | UNKNOWN | Declared pix_gerbe_item_t.speed_x storage; moteur only clears its enclosing storage or never accesses it. | unknown | +0x4, 2 B |
| types.json / field:pix_gerbe_item_t.speed_y | UNKNOWN | Declared pix_gerbe_item_t.speed_y storage; moteur only clears its enclosing storage or never accesses it. | unknown | +0x6, 2 B |
| types.json / field:pix_gerbe_item_t.y_accel | UNKNOWN | Declared pix_gerbe_item_t.y_accel storage; moteur only clears its enclosing storage or never accesses it. | unknown | +0x8, 1 B |
| types.json / field:pix_gerbe_item_t.unk_1 | UNKNOWN | Declared pix_gerbe_item_t.unk_1 storage; moteur only clears its enclosing storage or never accesses it. | unknown | +0x9, 1 B |
| types.json / field:pix_gerbe_t.items | PROVEN | 64-item particle storage view. | aggregate storage | +0x0, 640 B |
| types.json / field:pix_gerbe_t.words | PROVEN | 320-word alias of the same640-byte particle item storage. | aggregate storage | +0x0, 640 B |
| types.json / field:pix_gerbe_t.is_active | PROVEN | Signed16 active marker for a particle batch. | active marker | +0x280, 2 B |
| types.json / field:obj_procs_t.command | PROVEN | Object command/update callback. | callback pointer | +0x0, 4 B |
| types.json / field:obj_procs_t.poing_collision | PROVEN | Object fist-collision callback receiving a signed16 sprite selector. | callback pointer | +0x4, 4 B |
| types.json / field:obj_procs_t.rayman_collision | PROVEN | Object Rayman-collision callback. | callback pointer | +0x8, 4 B |
| types.json / field:obj_procs_t.rayman_in_zone | PROVEN | Object Rayman-in-detection-zone callback. | callback pointer | +0xc, 4 B |
| types.json / field:obj_procs_t.u_turn | STRONG | Fifth callback slot, initialized with turn-handler function pointers. | callback pointer | +0x10, 4 B |
| types.json / field:flocon_t.field_0 | UNKNOWN | Declared flocon_t.field_0 storage; moteur only clears its enclosing storage or never accesses it. | unknown | +0x0, 2 B |
| types.json / field:flocon_t.field_2 | UNKNOWN | Declared flocon_t.field_2 storage; moteur only clears its enclosing storage or never accesses it. | unknown | +0x2, 2 B |
| types.json / field:flocon_t.field_4 | UNKNOWN | Declared flocon_t.field_4 storage; moteur only clears its enclosing storage or never accesses it. | unknown | +0x4, 2 B |
| types.json / field:flocon_t.field_6 | UNKNOWN | Declared flocon_t.field_6 storage; moteur only clears its enclosing storage or never accesses it. | unknown | +0x6, 1 B |
| types.json / field:flocon_t.field_7 | UNKNOWN | Declared flocon_t.field_7 storage; moteur only clears its enclosing storage or never accesses it. | unknown | +0x7, 1 B |
| types.json / type:zdc_t | PROVEN | Packed DOS collision-zone record. | record |  |
| types.json / type:options_jeu_t | PROVEN | DOS input callback and option record. | record |  |
| types.json / type:ray_stack_t | PROVEN | Packed Rayman/fist display-history record, declared identically in ray.c and collision2_local.h. | record |  |
| types.json / field:zdc_t.x_pos | PROVEN | Collision-zone horizontal offset in the selected coordinate frame. | pixels | +0x0, 2 B |
| types.json / field:zdc_t.y_pos | PROVEN | Collision-zone vertical offset in the selected coordinate frame. | pixels | +0x2, 2 B |
| types.json / field:zdc_t.width | PROVEN | Unsigned horizontal span of this collision rectangle. | pixels | +0x4, 1 B |
| types.json / field:zdc_t.height | PROVEN | Unsigned vertical span of this collision rectangle. | pixels | +0x5, 1 B |
| types.json / field:zdc_t.flags | PROVEN | Collision-zone mode flags. | bit mask | +0x6, 1 B |
| types.json / field:zdc_t.sprite | PROVEN | Animation-layer anchor index and collision-result code. | layer index / result code | +0x7, 1 B |
| types.json / field:options_jeu_t.test_fire1 | STRONG | Current jump input-test callback. | s16(void) function pointer | +0x0, 4 B |
| types.json / field:options_jeu_t.test_fire0 | STRONG | Current fist input-test callback. | s16(void) function pointer | +0x4, 4 B |
| types.json / field:options_jeu_t.test_button4 | STRONG | Current empty fourth-action input-test callback. | s16(void) function pointer | +0x8, 4 B |
| types.json / field:options_jeu_t.test_button3 | STRONG | Current action/run/seed input-test callback. | s16(void) function pointer | +0xc, 4 B |
| types.json / field:options_jeu_t.jump | PROVEN | Physical/logical button selector for test_fire1. | button selector | +0x10, 2 B |
| types.json / field:options_jeu_t.fist | PROVEN | Physical/logical button selector for test_fire0. | button selector | +0x12, 2 B |
| types.json / field:options_jeu_t.field_14 | PROVEN | Physical/logical button selector for test_button4. | button selector | +0x14, 2 B |
| types.json / field:options_jeu_t.action | PROVEN | Physical/logical button selector for test_button3. | button selector | +0x16, 2 B |
| types.json / field:options_jeu_t.music_enabled | UNKNOWN | Unsigned16 option slot outside the Rayman/collision field-access scope. | unknown | +0x18, 2 B |
| types.json / field:options_jeu_t.sound_volume | UNKNOWN | Unsigned16 option slot outside the Rayman/collision field-access scope. | unknown | +0x1a, 2 B |
| types.json / field:options_jeu_t.is_stereo | UNKNOWN | Unsigned16 option slot outside the Rayman/collision field-access scope. | unknown | +0x1c, 2 B |
| types.json / field:options_jeu_t.unknown_1e | UNKNOWN | Unsigned16 option slot outside the Rayman/collision field-access scope. | unknown | +0x1e, 2 B |
| types.json / field:ray_stack_t.x_pos | PROVEN | Snapshot of Rayman x_pos. | pixels (signed16) | +0x0, 2 B |
| types.json / field:ray_stack_t.y_pos | PROVEN | Snapshot of Rayman y_pos. | pixels (signed16) | +0x2, 2 B |
| types.json / field:ray_stack_t.poing_x_pos | PROVEN | Snapshot of fist x_pos. | pixels (signed16) | +0x4, 2 B |
| types.json / field:ray_stack_t.poing_y_pos | PROVEN | Snapshot of fist y_pos. | pixels (signed16) | +0x6, 2 B |
| types.json / field:ray_stack_t.main_etat | PROVEN | Snapshot of Rayman main_etat. | state/frame/index byte | +0x8, 1 B |
| types.json / field:ray_stack_t.sub_etat | PROVEN | Snapshot of Rayman sub_etat. | state/frame/index byte | +0x9, 1 B |
| types.json / field:ray_stack_t.anim_index | PROVEN | Snapshot of Rayman anim_index. | state/frame/index byte | +0xa, 1 B |
| types.json / field:ray_stack_t.anim_frame | PROVEN | Snapshot of Rayman anim_frame. | state/frame/index byte | +0xb, 1 B |
| types.json / field:ray_stack_t.flip_x | PROVEN | Snapshot of Rayman flip_x. | state/frame/index byte | +0xc, 1 B |
| types.json / field:ray_stack_t.poing_anim_index | PROVEN | Snapshot of fist anim_index. | state/frame/index byte | +0xd, 1 B |
| types.json / field:ray_stack_t.poing_anim_frame | PROVEN | Snapshot of fist anim_frame. | state/frame/index byte | +0xe, 1 B |
| types.json / field:ray_stack_t.poing_flip_x | PROVEN | Snapshot of fist flip_x. | state/frame/index byte | +0xf, 1 B |
| types.json / field:ray_stack_t.poing_is_active | PROVEN | Validity byte for the optional fist snapshot. | boolean | +0x10, 1 B |
| types.json / field:ray_stack_t.scale | PROVEN | Low8 bits of Rayman scale at the history sample. | scale modulo256 | +0x11, 1 B |
| types.json / type:world_info_t | PROVEN | Packed DOS world-map stage record; pass4 directly uses its cage-count byte. | record |  |
| types.json / type:rgb_t | PROVEN | Three-byte RGB component record. | record |  |
| types.json / type:rgb_palette_t | PROVEN | 256-entry DOS palette. | record |  |
| types.json / type:archive_header_local_t | PROVEN | Local view of executable-resident archive descriptors; no such header is read from the archive file. | record |  |
| types.json / type:pcx_header_local_t | PROVEN | 128-byte decoded PCX header view, with only coordinate bounds and scanline byte count consumed. | record |  |
| types.json / type:x_texture_local_t | PROVEN | 256-byte ModeX pixel record. | record |  |
| types.json / type:level_local_t | PROVEN | load.c local compatibility declaration of the six-byte runtime level descriptor. | record |  |
| types.json / type:level_obj_local_t | PROVEN | load.c local compatibility declaration of a six-byte runtime object-index list. | record |  |
| types.json / type:display_map_local_t | PROVEN | Existing load.c display-map record view, reached only as a pointer in this unit. | record |  |
| types.json / type:big_map_local_t | PROVEN | Existing30-byte prefix view passed by address to construct_MAP; canonical big_map_t has additional trailing storage. | record |  |
| types.json / field:world_info_t.xpos | UNKNOWN | Declared world_info_t.xpos storage; no intended meaning is assigned from its spelling alone. | unknown | +0x0, 2 B |
| types.json / field:world_info_t.ypos | UNKNOWN | Declared world_info_t.ypos storage; no intended meaning is assigned from its spelling alone. | unknown | +0x2, 2 B |
| types.json / field:world_info_t.index_up | UNKNOWN | Declared world_info_t.index_up storage; no intended meaning is assigned from its spelling alone. | unknown | +0x4, 1 B |
| types.json / field:world_info_t.index_down | UNKNOWN | Declared world_info_t.index_down storage; no intended meaning is assigned from its spelling alone. | unknown | +0x5, 1 B |
| types.json / field:world_info_t.index_left | UNKNOWN | Declared world_info_t.index_left storage; no intended meaning is assigned from its spelling alone. | unknown | +0x6, 1 B |
| types.json / field:world_info_t.index_right | UNKNOWN | Declared world_info_t.index_right storage; no intended meaning is assigned from its spelling alone. | unknown | +0x7, 1 B |
| types.json / field:world_info_t.state | UNKNOWN | Declared world_info_t.state storage; no intended meaning is assigned from its spelling alone. | unknown | +0x8, 1 B |
| types.json / field:world_info_t.is_unlocked | UNKNOWN | Declared world_info_t.is_unlocked storage; no intended meaning is assigned from its spelling alone. | unknown | +0x8, 1 B |
| types.json / field:world_info_t.state_bit1 | UNKNOWN | Declared world_info_t.state_bit1 storage; no intended meaning is assigned from its spelling alone. | unknown | +0x8, 1 B |
| types.json / field:world_info_t.is_unlocking | UNKNOWN | Declared world_info_t.is_unlocking storage; no intended meaning is assigned from its spelling alone. | unknown | +0x8, 1 B |
| types.json / field:world_info_t.state_rest | UNKNOWN | Declared world_info_t.state_rest storage; no intended meaning is assigned from its spelling alone. | unknown | +0x8, 1 B |
| types.json / field:world_info_t.nb_cages | PROVEN | Collected-cage count of one world-map stage record. | cage count | +0x9, 1 B |
| types.json / field:world_info_t.world | UNKNOWN | Declared world_info_t.world storage; no intended meaning is assigned from its spelling alone. | unknown | +0xa, 1 B |
| types.json / field:world_info_t.level | UNKNOWN | Declared world_info_t.level storage; no intended meaning is assigned from its spelling alone. | unknown | +0xb, 1 B |
| types.json / field:world_info_t.color | UNKNOWN | Declared world_info_t.color storage; no intended meaning is assigned from its spelling alone. | unknown | +0xc, 1 B |
| types.json / field:world_info_t.field_D | UNKNOWN | Declared world_info_t.field_D storage; no intended meaning is assigned from its spelling alone. | unknown | +0xd, 1 B |
| types.json / field:world_info_t.field_E | UNKNOWN | Declared world_info_t.field_E storage; no intended meaning is assigned from its spelling alone. | unknown | +0xe, 1 B |
| types.json / field:world_info_t.field_F | UNKNOWN | Declared world_info_t.field_F storage; no intended meaning is assigned from its spelling alone. | unknown | +0xf, 1 B |
| types.json / field:world_info_t.text | UNKNOWN | Declared world_info_t.text storage; no intended meaning is assigned from its spelling alone. | unknown | +0x10, 4 B |
| types.json / field:rgb_t.r | STRONG | First component in each three-byte RGB palette entry. | component byte | +0x0, 1 B |
| types.json / field:rgb_t.g | STRONG | Second component in each three-byte RGB palette entry. | component byte | +0x1, 1 B |
| types.json / field:rgb_t.b | STRONG | Third component in each three-byte RGB palette entry. | component byte | +0x2, 1 B |
| types.json / field:rgb_palette_t.colors | PROVEN | 256 three-byte palette entries. | RGB records | +0x0, 768 B |
| types.json / field:archive_header_local_t.offset | PROVEN | Absolute file byte offset of one encoded archive payload. | file bytes | +0x0, 4 B |
| types.json / field:archive_header_local_t.size | PROVEN | Encoded payload byte length including PCX header/body/palette where applicable. | bytes | +0x4, 4 B |
| types.json / field:archive_header_local_t.xor_byte | PROVEN | Per-entry XOR decode byte. | key byte | +0x8, 1 B |
| types.json / field:archive_header_local_t.checksum_byte | PROVEN | Modulo256 sum of the encoded payload bytes. | mod256 byte sum | +0x9, 1 B |
| types.json / field:archive_header_local_t.reserved | UNKNOWN | Two table bytes at+10 not read as archive fields by the scoped loaders. | unknown | +0xa, 2 B |
| types.json / field:pcx_header_local_t.header_byte | UNKNOWN | Declared pcx_header_local_t.header_byte storage; no intended meaning is assigned from its spelling alone. | unknown | +0x0, 1 B |
| types.json / field:pcx_header_local_t.software_version | UNKNOWN | Declared pcx_header_local_t.software_version storage; no intended meaning is assigned from its spelling alone. | unknown | +0x1, 1 B |
| types.json / field:pcx_header_local_t.encoding_method | UNKNOWN | Declared pcx_header_local_t.encoding_method storage; no intended meaning is assigned from its spelling alone. | unknown | +0x2, 1 B |
| types.json / field:pcx_header_local_t.bits_per_bitplane | UNKNOWN | Declared pcx_header_local_t.bits_per_bitplane storage; no intended meaning is assigned from its spelling alone. | unknown | +0x3, 1 B |
| types.json / field:pcx_header_local_t.min_x | PROVEN | Decoded16-bit horizontal lower bound used in width computation. | pixels | +0x4, 2 B |
| types.json / field:pcx_header_local_t.min_y | PROVEN | Decoded16-bit vertical lower bound used in height computation. | pixels | +0x6, 2 B |
| types.json / field:pcx_header_local_t.max_x | PROVEN | Decoded16-bit horizontal upper bound used in width computation. | pixels | +0x8, 2 B |
| types.json / field:pcx_header_local_t.max_y | PROVEN | Decoded16-bit vertical upper bound used in height computation. | pixels | +0xa, 2 B |
| types.json / field:pcx_header_local_t.hor_dpi | UNKNOWN | Declared pcx_header_local_t.hor_dpi storage; no intended meaning is assigned from its spelling alone. | unknown | +0xc, 2 B |
| types.json / field:pcx_header_local_t.ver_dpi | UNKNOWN | Declared pcx_header_local_t.ver_dpi storage; no intended meaning is assigned from its spelling alone. | unknown | +0xe, 2 B |
| types.json / field:pcx_header_local_t.ega_palette | UNKNOWN | Declared pcx_header_local_t.ega_palette storage; no intended meaning is assigned from its spelling alone. | unknown | +0x10, 48 B |
| types.json / field:pcx_header_local_t.reserved1 | UNKNOWN | Declared pcx_header_local_t.reserved1 storage; no intended meaning is assigned from its spelling alone. | unknown | +0x40, 1 B |
| types.json / field:pcx_header_local_t.n_color_planes | UNKNOWN | Declared pcx_header_local_t.n_color_planes storage; no intended meaning is assigned from its spelling alone. | unknown | +0x41, 1 B |
| types.json / field:pcx_header_local_t.bytes_per_line | PROVEN | Number of expanded bytes required by each RLE scanline loop. | decoded bytes per scanline | +0x42, 2 B |
| types.json / field:pcx_header_local_t.palette_mode | UNKNOWN | Declared pcx_header_local_t.palette_mode storage; no intended meaning is assigned from its spelling alone. | unknown | +0x44, 2 B |
| types.json / field:pcx_header_local_t.source_hor_res | UNKNOWN | Declared pcx_header_local_t.source_hor_res storage; no intended meaning is assigned from its spelling alone. | unknown | +0x46, 2 B |
| types.json / field:pcx_header_local_t.source_ver_res | UNKNOWN | Declared pcx_header_local_t.source_ver_res storage; no intended meaning is assigned from its spelling alone. | unknown | +0x48, 2 B |
| types.json / field:pcx_header_local_t.reserved2 | UNKNOWN | Declared pcx_header_local_t.reserved2 storage; no intended meaning is assigned from its spelling alone. | unknown | +0x4a, 54 B |
| types.json / field:x_texture_local_t.pixels | PROVEN | One256-byte ModeX tile-pixel record. | indexed pixels | +0x0, 256 B |
| types.json / field:level_local_t.objects | PROVEN | Runtime pointer to132-byte live level-instance records. | obj_t pointer | +0x0, 4 B |
| types.json / field:level_local_t.nb_objects | PROVEN | Signed16 live level-instance count. | object count | +0x4, 2 B |
| types.json / field:level_obj_local_t.obj_ids | PROVEN | Runtime pointer to a signed16 index list. | object-index pointer | +0x0, 4 B |
| types.json / field:level_obj_local_t.nb_objects | PROVEN | Signed16 number of populated object-index list entries. | object count | +0x4, 2 B |
| types.json / field:display_map_local_t.x | UNKNOWN | Declared display_map_local_t.x storage; no intended meaning is assigned from its spelling alone. | unknown | +0x0, 2 B |
| types.json / field:display_map_local_t.y | UNKNOWN | Declared display_map_local_t.y storage; no intended meaning is assigned from its spelling alone. | unknown | +0x2, 2 B |
| types.json / field:display_map_local_t.width_in_tiles | UNKNOWN | Declared display_map_local_t.width_in_tiles storage; no intended meaning is assigned from its spelling alone. | unknown | +0x4, 1 B |
| types.json / field:display_map_local_t.height_in_tiles | UNKNOWN | Declared display_map_local_t.height_in_tiles storage; no intended meaning is assigned from its spelling alone. | unknown | +0x5, 1 B |
| types.json / field:display_map_local_t.tile_index | UNKNOWN | Declared display_map_local_t.tile_index storage; no intended meaning is assigned from its spelling alone. | unknown | +0x6, 2 B |
| types.json / field:big_map_local_t.map | UNKNOWN | Declared big_map_local_t.map storage; no intended meaning is assigned from its spelling alone. | unknown | +0x0, 4 B |
| types.json / field:big_map_local_t.tile_texture_offsets | UNKNOWN | Declared big_map_local_t.tile_texture_offsets storage; no intended meaning is assigned from its spelling alone. | unknown | +0x4, 4 B |
| types.json / field:big_map_local_t.display_map | UNKNOWN | Declared big_map_local_t.display_map storage; no intended meaning is assigned from its spelling alone. | unknown | +0x8, 4 B |
| types.json / field:big_map_local_t.map_blocks | UNKNOWN | Declared big_map_local_t.map_blocks storage; no intended meaning is assigned from its spelling alone. | unknown | +0xc, 4 B |
| types.json / field:big_map_local_t.field_10 | UNKNOWN | Declared big_map_local_t.field_10 storage; no intended meaning is assigned from its spelling alone. | unknown | +0x10, 4 B |
| types.json / field:big_map_local_t.field_14 | UNKNOWN | Declared big_map_local_t.field_14 storage; no intended meaning is assigned from its spelling alone. | unknown | +0x14, 2 B |
| types.json / field:big_map_local_t.width | UNKNOWN | Declared big_map_local_t.width storage; no intended meaning is assigned from its spelling alone. | unknown | +0x16, 2 B |
| types.json / field:big_map_local_t.height | UNKNOWN | Declared big_map_local_t.height storage; no intended meaning is assigned from its spelling alone. | unknown | +0x18, 2 B |
| types.json / field:big_map_local_t.field_1A | UNKNOWN | Declared big_map_local_t.field_1A storage; no intended meaning is assigned from its spelling alone. | unknown | +0x1a, 2 B |
| types.json / field:big_map_local_t.field_1C | UNKNOWN | Declared big_map_local_t.field_1C storage; no intended meaning is assigned from its spelling alone. | unknown | +0x1c, 2 B |
