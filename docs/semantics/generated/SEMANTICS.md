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
| globals.json / global:ray_last_ground_btyp | PROVEN | Sticky ordinary/slippery ground classification, not the literal last tile-type number. | classification0/1 | 2:3fa72 |
| globals.json / global:ray_mode | PROVEN | Player update mode dispatcher:1 ordinary,2 mounted,3/4 death,5 breakout; nonpositive values use direct placement in DO_MOTEUR2. | mode code | 2:3f868 |
| globals.json / global:poing | PROVEN | Fist control record, including the separate16-scaled vertical accumulator. | packed control record | 2:3e770 |
| globals.json / global:finBosslevel | PROVEN | Packed persistent boss/help-event completion bits. | bitfield state | 2:3f81e |
| globals.json / global:first_boss_meet | STRONG | Byte toggled by waiting-animation swapping and tested with boss hit_points/scroll lock during activation. | boolean marker | 2:3fa71 |
| globals.json / global:level_obj | PROVEN | Activation-candidate object-id list, initially containing non-always types and later live spawned always objects. | object ids/count | 2:3cdf8 |
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
| moteur.json / global:button_released | STRONG | Player jump/input-release latch set before the forced hybrid landing jump. | boolean | 2:3fa7d |
| moteur.json / global:compteur_attente | STRONG | Idle/wait animation counter reset during Rayman initialization. | update calls | 2:3f774 |
| moteur.json / global:current_pal_id | STRONG | Current palette-selection state used by palette actualization. | palette selector | 2:3fa82 |
| moteur.json / global:dans_la_map_monde | PROVEN | Worldmap-context flag influencing horizontal camera-follow smoothing. | boolean | 2:3fa53 |
| moteur.json / global:dead_time | PROVEN | Byte death/entry sequence countdown used for fades, reinitialization and delayed player death. | engine calls / sequence state | 2:3fa7e |
| moteur.json / global:decalage_en_cours | STRONG | Rayman horizontal inertia state also used to choose the camera-follow direction. | player inertia units | 2:3f866 |
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
| moteur.json / global:helico_time | STRONG | Signed16 helicopter-duration state with-1 reset sentinel. | player update calls / -1 | 2:3f898 |
| moteur.json / global:id_Cling_1up | PROVEN | Current extra-life/power pickup animation object ids, respectively, or-1 when absent. | object id / -1 | 2:3f82c |
| moteur.json / global:id_Cling_Pow | PROVEN | Current extra-life/power pickup animation object ids, respectively, or-1 when absent. | object id / -1 | 2:3f846 |
| moteur.json / global:in_air_because_hit | STRONG | Marker that Rayman airborne motion originated from a hit. | boolean | 2:3fa8c |
| moteur.json / global:in_pause | STRONG | Pause control cleared during engine initialization. | boolean | 2:3fa52 |
| moteur.json / global:joe_exp_probleme | STRONG | Joe interaction progression latch initialized from helped_joe_2. | boolean marker | 2:3f786 |
| moteur.json / global:jump_time | STRONG | Signed16 player jump-duration counter reset on level initialization. | player update calls | 2:3f8a4 |
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
| moteur.json / global:ray_pos_in_stack | PROVEN | Current write slot in rayStack, the full Rayman/fist snapshot ring replayed by black Rayman. | ring index | 2:3fa8b |
| moteur.json / global:ray_se_noie | STRONG | Rayman drowning-state marker. | boolean marker | 2:3fa7b |
| moteur.json / global:ray_stack_is_full | PROVEN | Whether the Rayman/fist snapshot ring rayStack has wrapped and can be replayed by black Rayman. | boolean | 2:3fa88 |
| moteur.json / global:ray_wind_force | STRONG | Horizontal wind contribution selected for Rayman for the current engine phase. | player horizontal force units | 2:3f870 |
| moteur.json / global:ray_zdc_y | STRONG | Rayman collision-height value used in the vertical camera target calculation. | pixels | 2:3f876 |
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
| types.json / field:obj_t.sprites | PROVEN | Pointer to the object-specific sprites table. | pointer | +0x0, 4 B |
| types.json / field:obj_t.animations | PROVEN | Pointer to the object-specific animations table. | pointer | +0x4, 4 B |
| types.json / field:obj_t.img_buffer | STRONG | Object sprite pixel-atlas buffer. | pointer | +0x8, 4 B |
| types.json / field:obj_t.eta | PROVEN | Pointer to the object-specific eta table. | pointer | +0xc, 4 B |
| types.json / field:obj_t.cmds | STRONG | Object script stream/context member cmds. | script dependent | +0x10, 4 B |
| types.json / field:obj_t.cmd_labels | STRONG | Object script stream/context member cmd_labels. | script dependent | +0x14, 4 B |
| types.json / field:obj_t.cmd_contexts | STRONG | Object script stream/context member cmd_contexts. | script dependent | +0x18, 4 B |
| types.json / field:obj_t.field_1C | PROVEN | Overloaded 32-bit scratch mark: set by make_active/make_active2 and tested to suppress repeat linked activation; also set during save restoration for gate types179/242 and tested by INIT_OBJECTS. | boolean mark | +0x1c, 4 B |
| types.json / field:obj_t.link_has_gendoor | STRONG | Marker enabling linked-group operations; normally set for eligible non-self successor groups, not a proof that a generating door exists. | boolean marker | +0x20, 4 B |
| types.json / field:obj_t.is_active | PROVEN | Object participation flag saved as the high bit of each two-bit snapshot entry. | boolean | +0x24, 4 B |
| types.json / field:obj_t.x | PROVEN | Signed32 world pixel x origin in live object movement; loader also reuses object records as descriptors. | pixels in live objects | +0x28, 4 B |
| types.json / field:obj_t.y | PROVEN | Signed32 world pixel y origin in live object movement. | pixels in live objects | +0x2c, 4 B |
| types.json / field:obj_t.active_flag | PROVEN | Activation lifecycle code used by zone transitions: 0 activated/default,1 dead/outside,2 reinitialised awaiting departure,4 waiting for the linked group to leave. | state code | +0x30, 4 B |
| types.json / field:obj_t.id | PROVEN | Object identifier stored in the active and always-object id lists. | object index | +0x34, 2 B |
| types.json / field:obj_t.screen_x | PROVEN | Stored screen-space origin coordinate screen_x after camera subtraction. | pixels | +0x36, 2 B |
| types.json / field:obj_t.screen_y | PROVEN | Stored screen-space origin coordinate screen_y after camera subtraction. | pixels | +0x38, 2 B |
| types.json / field:obj_t.sub_speed | UNKNOWN | Neutral obj_t.sub_speed storage/view; its intended meaning is not established by this pilot. | unknown | +0x3a, 2 B |
| types.json / field:obj_t.init_x | STRONG | Initial position component init_x used by reset and relative-displacement logic. | pixels | +0x3c, 2 B |
| types.json / field:obj_t.init_y | STRONG | Initial position component init_y used by reset and relative-displacement logic. | pixels | +0x3e, 2 B |
| types.json / field:obj_t.speed_x | PROVEN | Signed16 horizontal speed, interpreted per type axis flag as integer pixels/call or sixteenths of a pixel/call. | pixels/call or 1/16 pixels/call | +0x40, 2 B |
| types.json / field:obj_t.speed_y | PROVEN | Signed16 vertical speed, interpreted per type axis flag as integer pixels/call or sixteenths of a pixel/call. | pixels/call or 1/16 pixels/call | +0x42, 2 B |
| types.json / field:obj_t.nb_sprites | PROVEN | Count bounding the object sprite-table search. | sprites | +0x44, 2 B |
| types.json / field:obj_t.cmd_offset | STRONG | Object script stream/context member cmd_offset. | script dependent | +0x46, 2 B |
| types.json / field:obj_t.nb_cmd | STRONG | Object script stream/context member nb_cmd. | script dependent | +0x48, 2 B |
| types.json / field:obj_t.follow_id | STRONG | Identifier of an object being followed, with type-specific reuse. | object id or scratch | +0x4a, 2 B |
| types.json / field:obj_t.follow_y | PROVEN | Context-dependent scratch value: animation-layer displacement, projectile target coordinate, or other type-specific state. | context dependent | +0x4c, 2 B |
| types.json / field:obj_t.follow_x | PROVEN | Context-dependent scratch value: animation-layer displacement, projectile target coordinate, or other type-specific state. | context dependent | +0x4e, 2 B |
| types.json / field:obj_t.param | PROVEN | Type-specific signed scratch parameter, including Rayman edge classification in calc_btyp. | context dependent | +0x50, 2 B |
| types.json / field:obj_t.link | UNKNOWN | Type-dependent signed16 storage: generating doors save their detached successor id here; live Rayman initialization uses-1, and template swaps copy the field. A universal meaning across object types remains unresolved. | type-dependent state / object id | +0x52, 2 B |
| types.json / field:obj_t.ray_dist | PROVEN | Context-dependent Rayman tile index or signed object-to-Rayman support distance. | tile index or pixels | +0x54, 2 B |
| types.json / field:obj_t.iframes_timer | STRONG | Signed invulnerability/response timer. | ticks | +0x56, 2 B |
| types.json / field:obj_t.test_block_index | PROVEN | Last row-major map tile index used by the changed-block test. | tile index | +0x58, 2 B |
| types.json / field:obj_t.scale | PROVEN | Projection-depth parameter whose denominator is signed_scale+256. | projection depth units | +0x5a, 2 B |
| types.json / field:obj_t.zdc | UNKNOWN | Neutral obj_t.zdc storage/view; its intended meaning is not established by this pilot. | unknown | +0x5c, 2 B |
| types.json / field:obj_t.active_timer | PROVEN | Delay inhibiting reactivation, counted down while the living object is in its action zone. | activation calls | +0x5e, 2 B |
| types.json / field:obj_t.type | PROVEN | Unsigned object type discriminator indexing flags and selecting behaviour. | object type id | +0x60, 2 B |
| types.json / field:obj_t.btypes | PROVEN | Five cached collision block types in centre,left,right,above,below order. | block type id | +0x62, 5 B |
| types.json / field:obj_t.unused_padding | UNKNOWN | Neutral obj_t.unused_padding storage/view; its intended meaning is not established by this pilot. | unknown | +0x67, 1 B |
| types.json / field:obj_t.offset_bx | PROVEN | Horizontal object anchor offset used for collision probes and reflection centre. | pixels | +0x68, 1 B |
| types.json / field:obj_t.offset_by | PROVEN | Vertical object anchor offset used for foot/block probes and projection centre. | pixels | +0x69, 1 B |
| types.json / field:obj_t.anim_index | PROVEN | Current animation selector or current frame selector (anim_index). | index | +0x6a, 1 B |
| types.json / field:obj_t.anim_frame | PROVEN | Current animation selector or current frame selector (anim_frame). | index | +0x6b, 1 B |
| types.json / field:obj_t.sub_etat | PROVEN | Byte index into the main/substate animation-state table (sub_etat). | state index | +0x6c, 1 B |
| types.json / field:obj_t.main_etat | PROVEN | Byte index into the main/substate animation-state table (main_etat). | state index | +0x6d, 1 B |
| types.json / field:obj_t.init_sub_etat | STRONG | Reset value for init_sub_etat. | state index or hit points | +0x6e, 1 B |
| types.json / field:obj_t.init_main_etat | STRONG | Reset value for init_main_etat. | state index or hit points | +0x6f, 1 B |
| types.json / field:obj_t.cmd | STRONG | Current script command discriminator. | opcode | +0x70, 1 B |
| types.json / field:obj_t.gravity_value_1 | PROVEN | First gravity counter: modulo3 in ordinary airborne updates, or the byte phase counter compared against gravity_value_2 by gravity modes10/11. | update phase | +0x71, 1 B |
| types.json / field:obj_t.gravity_value_2 | PROVEN | Second gravity counter: modulo4 in ordinary airborne updates, or the comparison threshold retained for gravity modes10/11. | phase / interval threshold | +0x72, 1 B |
| types.json / field:obj_t.change_anim_mode | PROVEN | Animation transition mode set to1 by the state setters. | mode | +0x73, 1 B |
| types.json / field:obj_t.offset_hy | STRONG | Unsigned vertical upper-body/collision anchor offset. | pixels | +0x74, 1 B |
| types.json / field:obj_t.follow_sprite | PROVEN | Selected animation layer for following, also overloaded as Blacktoon behaviour selector. | layer index or subtype | +0x75, 1 B |
| types.json / field:obj_t.hit_points | PROVEN | Unsigned hit-point byte in obj_hurt, with type-specific reuse including signed deformation angle input. | hit points or type-specific state | +0x76, 1 B |
| types.json / field:obj_t.init_hit_points | STRONG | Reset value for init_hit_points. | state index or hit points | +0x77, 1 B |
| types.json / field:obj_t.init_flag | UNKNOWN | Neutral obj_t.init_flag storage/view; its intended meaning is not established by this pilot. | unknown | +0x78, 1 B |
| types.json / field:obj_t.hit_sprite | UNKNOWN | Neutral obj_t.hit_sprite storage/view; its intended meaning is not established by this pilot. | unknown | +0x79, 1 B |
| types.json / field:obj_t.detect_zone | STRONG | Type-dependent size/range parameter used by the detection-zone computation. | pixels or type-specific range | +0x7a, 1 B |
| types.json / field:obj_t.detect_zone_flag | PROVEN | Detection-zone classification tested by OBJ_IN_ZONE. | classification | +0x7b, 1 B |
| types.json / field:obj_t.cmd_context_depth | STRONG | Object script stream/context member cmd_context_depth. | script dependent | +0x7c, 1 B |
| types.json / field:obj_t.config | PROVEN | Type-specific byte configuration tested for Blacktoon edge-turning. | context dependent | +0x7d, 1 B |
| types.json / field:obj_t.display_prio | STRONG | Object drawing-order bucket selector. | priority bucket | +0x7e, 1 B |
| types.json / field:obj_t.timer | UNKNOWN | Type-dependent byte storage; the cling pickup animation path decrements it as a delay while holding frame zero. Its other object-specific roles remain unresolved. | type-dependent updates/state | +0x7f, 1 B |
| types.json / field:obj_t.anim_count | STRONG | Number of animations in the object descriptor. | animations | +0x80, 1 B |
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
| types.json / field:eta_t.flags_bits.bit0 | UNKNOWN | Neutral eta_t.flags_bits.bit0 storage/view; its intended meaning is not established by this pilot. | unknown | +0x7, 1 B |
| types.json / field:eta_t.flags_bits.bit1 | UNKNOWN | Neutral eta_t.flags_bits.bit1 storage/view; its intended meaning is not established by this pilot. | unknown | +0x7, 1 B |
| types.json / field:eta_t.flags_bits.bit2 | UNKNOWN | Neutral eta_t.flags_bits.bit2 storage/view; its intended meaning is not established by this pilot. | unknown | +0x7, 1 B |
| types.json / field:eta_t.flags_bits.bit3 | UNKNOWN | Observed state flag0x08: DO_OBJECTS increments a nonboss object hit_points from0 to1 after its update when this bit is set. A broader intended state name is unresolved. | state flag | +0x7, 1 B |
| types.json / field:eta_t.flags_bits.bit4 | PROVEN | Reverse animation playback flag0x10, selecting decrement and last-frame initialization. | boolean | +0x7, 1 B |
| types.json / field:eta_t.flags_bits.bit5 | UNKNOWN | Neutral eta_t.flags_bits.bit5 storage/view; its intended meaning is not established by this pilot. | unknown | +0x7, 1 B |
| types.json / field:eta_t.flags_bits.bit6 | PROVEN | Alias for flag0x40 selecting the small14-pixel Rayman collision rectangle in GET_RAY_ZDC. | boolean | +0x7, 1 B |
| types.json / field:eta_t.flags_bits.bit7 | UNKNOWN | Neutral eta_t.flags_bits.bit7 storage/view; its intended meaning is not established by this pilot. | unknown | +0x7, 1 B |
| types.json / field:sprite_t.offset_in_atlas | STRONG | Pixel offset of this sprite in its object atlas buffer. | bytes | +0x0, 4 B |
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
| types.json / field:anim_t.layers | PROVEN | Pointer to frame-major animation layers. | pointer | +0x0, 4 B |
| types.json / field:anim_t.frames | PROVEN | Pointer to one bounding rectangle per animation frame. | pointer | +0x4, 4 B |
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
| types.json / field:map_tile_t.field_3 | UNKNOWN | Neutral map_tile_t.field_3 storage/view; its intended meaning is not established by this pilot. | unknown | +0x3, 1 B |
| types.json / field:map_tile_t.transparency | PROVEN | Computed texture coverage classification:0 opaque,1 empty,2 mixed. | classification | +0x4, 1 B |
| types.json / field:map_tile_t.field_5 | UNKNOWN | Neutral map_tile_t.field_5 storage/view; its intended meaning is not established by this pilot. | unknown | +0x5, 1 B |
| types.json / field:block_flags_t.raw | PROVEN | Byte/bitfield views of block-type behaviour flags. | bitset | +0x0, 1 B |
| types.json / field:block_flags_t.bits | PROVEN | Byte/bitfield views of block-type behaviour flags. | bitset | +0x0, 1 B |
| types.json / field:block_flags_t.bits.unused0 | UNKNOWN | Observed raw bit0 used by underSlope, stone-dog side probes and mounted Rayman side-strip probes; its universal collision-class meaning remains unresolved. | tile-class bit | +0x0, 1 B |
| types.json / field:block_flags_t.bits.solid | PROVEN | Bit1 tested as the supporting/solid classification in block selection. | boolean | +0x0, 1 B |
| types.json / field:block_flags_t.bits.unused2 | UNKNOWN | Three-bit view of raw bits2..4; moteur tests bit3 together with bit6 for icy slopes, and bit4 for ceiling/wall blocking. Bit2 meaning is unresolved. | packed tile-class bits | +0x0, 1 B |
| types.json / field:block_flags_t.bits.chdir | PROVEN | Bit5 of the probed block flags, returned by the turn-marker test. | boolean | +0x0, 1 B |
| types.json / field:block_flags_t.bits.unused1 | UNKNOWN | Two-bit view of raw bits6..7; moteur consumes bit6 in slope predicates while the role of bit7 remains unresolved. | packed tile-class bits | +0x0, 1 B |
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
