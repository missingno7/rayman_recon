# basic.c semantic registry

Meaning confidence is separate from compiler layout and STRICT status. UNKNOWN retains neutral names. Raw source/displacement/external token hits are candidates, not type proofs.

| Entity | Level | Meaning | Units | DOS layout/address |
|---|---|---|---|---|
| function:EOA | PROVEN | Report the terminal animation frame only on a zero phase of its selected animation clock. | see parameters/result contract | 1:0d074 |
| local:EOA:on_last_frame:1 | PROVEN | Whether the current frame is the direction-dependent terminal frame. | boolean |  |
| local:EOA:eta:1 | PROVEN | Current main/substate descriptor. | pointer |  |
| local:EOA:anim:1 | PROVEN | Current object animation descriptor. | pointer |  |
| local:EOA:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| function:save_objects_flags | PROVEN | Pack alive and is_active into two bits per level object. | see parameters/result contract | 1:0d0ec |
| local:save_objects_flags:i:1 | PROVEN | Loop or append/removal slot in the object/list sequence used by this helper. | object/list index |  |
| local:save_objects_flags:nb_objects:1 | PROVEN | Local snapshot of the current level object count. | objects |  |
| local:save_objects_flags:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| function:restore_objects_flags | PROVEN | Restore packed alive and active bits for every level object except type 58. | see parameters/result contract | 1:0d15c |
| local:restore_objects_flags:high_bit:1 | PROVEN | Decoded active flag bit. | boolean |  |
| local:restore_objects_flags:low_bit:1 | PROVEN | Decoded alive flag bit. | boolean |  |
| local:restore_objects_flags:i:1 | PROVEN | Loop or append/removal slot in the object/list sequence used by this helper. | object/list index |  |
| local:restore_objects_flags:nb_objects:1 | PROVEN | Local snapshot of the current level object count. | objects |  |
| local:restore_objects_flags:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| local:restore_objects_flags:word_index:1 | PROVEN | Snapshot word index i divided by16. | word index |  |
| local:restore_objects_flags:bit_index:1 | PROVEN | Object slot within a two-bit snapshot word. | two-bit slot |  |
| function:snapToSprite | PROVEN | Compute follower speeds towards a selected sprite of the other object at its predicted next animation frame. | see parameters/result contract | 1:0d204 |
| local:snapToSprite:obj_2_frames_count:1 | PROVEN | Predicted animation frame count narrowed to a byte. | frames |  |
| local:snapToSprite:obj_2_eta_2:1 | PROVEN | Predicted transition state descriptor. | pointer |  |
| local:snapToSprite:prev_frame:1 | PROVEN | Saved actual target animation field restored after the predictive bounds query. | animation/frame index |  |
| local:snapToSprite:prev_index:1 | PROVEN | Saved actual target animation field restored after the predictive bounds query. | animation/frame index |  |
| local:snapToSprite:obj_2_x:1 | PROVEN | Predicted followed-sprite x coordinate. | pixels |  |
| local:snapToSprite:obj_2_y:1 | PROVEN | Predicted followed-sprite y coordinate. | pixels |  |
| local:snapToSprite:obj_2_w:1 | PROVEN | Predicted followed-sprite width output. | pixels |  |
| local:snapToSprite:obj_2_h:1 | PROVEN | Predicted followed-sprite height output. | pixels |  |
| local:snapToSprite:new_spd_x:1 | PROVEN | Signed16 residual x distance then converted follower x speed. | pixels then object speed units |  |
| local:snapToSprite:new_spd_y:1 | PROVEN | Signed16 residual y distance then converted follower y speed. | pixels then object speed units |  |
| local:snapToSprite:obj_2_eta_1:1 | PROVEN | Current state descriptor of the followed object. | pointer |  |
| local:snapToSprite:obj_2_anim_index:1 | PROVEN | Predicted followed-object animation selector. | animation index |  |
| local:snapToSprite:obj_2_anim_frame:1 | PROVEN | Predicted followed-object frame selector. | frame index |  |
| local:snapToSprite:perm_value_7:1 | PROVEN | First frame of the predicted next animation, last frame when playing backwards. | frame index |  |
| local:snapToSprite:first_flags:1 | PROVEN | Byte1 type flags snapshot for the follower; interpretation depends on the enclosing axis block. | bitset |  |
| local:snapToSprite:second_flags:1 | PROVEN | Byte1 type flags snapshot for the followed object; interpretation depends on the enclosing axis block. | bitset |  |
| local:snapToSprite:first_flags:2 | PROVEN | Byte1 type flags snapshot for the follower; interpretation depends on the enclosing axis block. | bitset |  |
| local:snapToSprite:second_flags:2 | PROVEN | Byte1 type flags snapshot for the followed object; interpretation depends on the enclosing axis block. | bitset |  |
| local:snapToSprite:obj_1:1 | PROVEN | Follower object whose position and speeds are updated. | pointer |  |
| local:snapToSprite:obj_2:1 | PROVEN | Followed object whose predicted sprite position is sampled. | pointer |  |
| local:snapToSprite:obj_2_spr:1 | PROVEN | Animation layer index to sample on the followed object. | layer index |  |
| local:snapToSprite:x_offs:1 | PROVEN | Horizontal follower placement offset from the sampled sprite. | pixels |  |
| local:snapToSprite:y_offs:1 | PROVEN | Unsigned16 vertical follower placement offset from the sampled sprite. | pixels |  |
| function:findfirstObject | PROVEN | Return the first object of the requested type, or the pointer just past the level object array. | see parameters/result contract | 1:0d4c4 |
| local:findfirstObject:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| local:findfirstObject:i:1 | PROVEN | Loop or append/removal slot in the object/list sequence used by this helper. | object/list index |  |
| local:findfirstObject:nb_objs:1 | PROVEN | Local snapshot of the current level object count. | objects |  |
| local:findfirstObject:obj_type:1 | PROVEN | Object type discriminator sought by the linear search. | object type id |  |
| function:findfirstInactiveObject | PROVEN | Return the first inactive object of the requested type, or the pointer just past the level object array. | see parameters/result contract | 1:0d4f4 |
| local:findfirstInactiveObject:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| local:findfirstInactiveObject:i:1 | PROVEN | Loop or append/removal slot in the object/list sequence used by this helper. | object/list index |  |
| local:findfirstInactiveObject:nb_objs:1 | PROVEN | Local snapshot of the current level object count. | objects |  |
| local:findfirstInactiveObject:obj_type:1 | PROVEN | Object type discriminator sought by the linear search. | object type id |  |
| function:sinYspeed | PROVEN | Advance a 12-bit phase and return the sine target displacement minus current displacement from init_y. | see parameters/result contract | 1:0d528 |
| local:sinYspeed:unk_1:1 | PROVEN | Wrapped updated sine phase. | 1/4096 cycle |  |
| local:sinYspeed:diff_y:1 | PROVEN | Signed16 current y minus initial y. | pixels |  |
| local:sinYspeed:unk_2:1 | PROVEN | Target vertical displacement from the sine product shifted by9. | pixels |  |
| local:sinYspeed:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| local:sinYspeed:a2:1 | PROVEN | Advance added to the stored phase before wrapping to12 bits. | 1/4096 cycle |  |
| local:sinYspeed:a3:1 | PROVEN | Signed sine amplitude multiplier. | pixels |  |
| local:sinYspeed:a4:1 | PROVEN | Pointer to the mutable phase accumulator. | 1/4096 cycle |  |
| function:set_proj_center | PROVEN | Store the signed-16 projection centre coordinates. | see parameters/result contract | 1:0d560 |
| local:set_proj_center:x:1 | PROVEN | Coordinate value consumed by this helper computation. | pixels |  |
| local:set_proj_center:y:1 | PROVEN | Coordinate value consumed by this helper computation. | pixels |  |
| function:get_proj_dist | PROVEN | Return a dimension unchanged with zoom disabled, otherwise apply the integer projection factor. | see parameters/result contract | 1:0d570 |
| local:get_proj_dist:temp:1 | PROVEN | Projected dimension product before division by256. | 1/256 pixel |  |
| local:get_proj_dist:scale:1 | PROVEN | Signed projection depth added to256 in the scale denominator. | projection depth units |  |
| local:get_proj_dist:outer_dim:1 | PROVEN | Signed dimension to optionally project. | pixels |  |
| function:get_proj_dist2 | PROVEN | Scale a signed dimension by the integer projection factor without consulting zoom_mode. | see parameters/result contract | 1:0d5b0 |
| local:get_proj_dist2:scale:1 | PROVEN | Signed projection depth added to256 in the scale denominator. | projection depth units |  |
| local:get_proj_dist2:a2:1 | PROVEN | Signed dimension to project using arithmetic-shift rounding. | pixels |  |
| function:get_proj_x | PROVEN | Project a signed-16 horizontal coordinate about PROJ_CENTER_X. | see parameters/result contract | 1:0d5d8 |
| local:get_proj_x:temp:1 | PROVEN | Projection factor times x displacement from the centre. | 1/256 pixel |  |
| local:get_proj_x:scale:1 | PROVEN | Signed projection depth added to256 in the scale denominator. | projection depth units |  |
| local:get_proj_x:a2:1 | PROVEN | Horizontal coordinate to project. | pixels |  |
| function:get_proj_y | PROVEN | Project a signed-16 vertical coordinate about PROJ_CENTER_Y. | see parameters/result contract | 1:0d614 |
| local:get_proj_y:temp:1 | PROVEN | Projection factor times y displacement from the centre. | 1/256 pixel |  |
| local:get_proj_y:scale:1 | PROVEN | Signed projection depth added to256 in the scale denominator. | projection depth units |  |
| local:get_proj_y:a2:1 | PROVEN | Vertical coordinate to project. | pixels |  |
| function:set_zoom_mode | PROVEN | Store the byte controlling the optional projection branch of get_proj_dist. | see parameters/result contract | 1:0d650 |
| local:set_zoom_mode:mode:1 | PROVEN | Byte stored into zoom_mode. | zero/nonzero mode |  |
| function:inverse_proj_x | PROVEN | Map a horizontal coordinate back through the quantised projection factor. | see parameters/result contract | 1:0d658 |
| local:inverse_proj_x:divisor:1 | PROVEN | Integer projection factor65536/(scale+256). | 1/256 multiplier |  |
| local:inverse_proj_x:scale:1 | PROVEN | Signed projection depth added to256 in the scale denominator. | projection depth units |  |
| local:inverse_proj_x:a2:1 | PROVEN | Projected horizontal coordinate. | pixels |  |
| function:inverse_proj_y | PROVEN | Map a vertical coordinate back through the quantised projection factor. | see parameters/result contract | 1:0d690 |
| local:inverse_proj_y:divisor:1 | PROVEN | Integer projection factor65536/(scale+256). | 1/256 multiplier |  |
| local:inverse_proj_y:scale:1 | PROVEN | Signed projection depth added to256 in the scale denominator. | projection depth units |  |
| local:inverse_proj_y:a2:1 | PROVEN | Projected vertical coordinate. | pixels |  |
| function:vblToEOA | PROVEN | Compute a positive, signed-16-truncated duration from the state animation period, frame count, clock phase and multiplier. | see parameters/result contract | 1:0d6c8 |
| local:vblToEOA:eta:1 | PROVEN | State descriptor for the object current main/substate. | pointer |  |
| local:vblToEOA:anim_speed:1 | PROVEN | Animation timing low nibble selecting a global phase counter. | clock index/nominal ticks |  |
| local:vblToEOA:duration:1 | PROVEN | Signed16 product of timing nibble and state-selected frame count. | ticks |  |
| local:vblToEOA:result:1 | PROVEN | Signed16 candidate duration after multiplier and phase adjustment. | ticks |  |
| local:vblToEOA:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| local:vblToEOA:a2:1 | PROVEN | Multiplier applied to nominal full-animation duration. | dimensionless integer |  |
| function:GET_ANIM_POS | PROVEN | Write the current animation-frame rectangle in world coordinates. | see parameters/result contract | 1:0d738 |
| local:GET_ANIM_POS:frame:1 | PROVEN | Pointer to current animation frame bounding rectangle. | pointer |  |
| local:GET_ANIM_POS:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| local:GET_ANIM_POS:x:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| local:GET_ANIM_POS:y:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| local:GET_ANIM_POS:w:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| local:GET_ANIM_POS:h:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| function:add_actobj | PROVEN | Append an object id to the active-object id array and write a following -1 sentinel. | see parameters/result contract | 1:0d7d8 |
| local:add_actobj:index:1 | PROVEN | Loop or append/removal slot in the object/list sequence used by this helper. | object/list index |  |
| local:add_actobj:obj_index:1 | PROVEN | Object id supplied to the object-list operation. | object id |  |
| function:set_sub_etat | PROVEN | Set the object substate and request animation reset if the selected animation changes. | see parameters/result contract | 1:0d808 |
| local:set_sub_etat:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| local:set_sub_etat:sub_etat:1 | PROVEN | New byte substate/main-state selector supplied to the setter. | state index |  |
| function:set_main_etat | PROVEN | Set the object main state and request animation reset if the selected animation changes. | see parameters/result contract | 1:0d810 |
| local:set_main_etat:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| local:set_main_etat:etat:1 | PROVEN | New byte substate/main-state selector supplied to the setter. | state index |  |
| function:set_main_and_sub_etat | PROVEN | Set both state indices and request animation reset if the selected animation changes. | see parameters/result contract | 1:0d818 |
| local:set_main_and_sub_etat:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| local:set_main_and_sub_etat:etat:1 | PROVEN | New byte substate/main-state selector supplied to the setter. | state index |  |
| local:set_main_and_sub_etat:sub_etat:1 | PROVEN | New byte substate/main-state selector supplied to the setter. | state index |  |
| function:get_center_x | PROVEN | Choose a type-dependent horizontal offset for the turn-marker block probe. | see parameters/result contract | 1:0d824 |
| local:get_center_x:res:1 | PROVEN | Selected type-specific turn-probe offset returned by this helper. | pixels |  |
| local:get_center_x:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| function:get_center_y | PROVEN | Choose a type-dependent vertical offset for the turn-marker block probe. | see parameters/result contract | 1:0d954 |
| local:get_center_y:res:1 | PROVEN | Selected type-specific turn-probe offset returned by this helper. | pixels |  |
| local:get_center_y:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| function:on_block_chdir | PROVEN | Test bit 5 of the block flags at the object position plus supplied probe offsets. | see parameters/result contract | 1:0dac8 |
| local:on_block_chdir:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| local:on_block_chdir:center_x:1 | PROVEN | Object-relative coordinate offset of the block probe. | pixels |  |
| local:on_block_chdir:center_y:1 | PROVEN | Object-relative coordinate offset of the block probe. | pixels |  |
| function:CALC_FOLLOW_SPRITE_SPEED | PROVEN | Store current-minus-next selected animation-layer offsets into follow_x and follow_y. | see parameters/result contract | 1:0db04 |
| local:CALC_FOLLOW_SPRITE_SPEED:unk_1:1 | PROVEN | Twice object x anchor offset minus current followed-sprite outer width. | pixels |  |
| local:CALC_FOLLOW_SPRITE_SPEED:x_2:1 | PROVEN | Next followed-layer x offset after optional object reflection. | pixels |  |
| local:CALC_FOLLOW_SPRITE_SPEED:x_1:1 | PROVEN | Current followed-layer x offset after optional object reflection. | pixels |  |
| local:CALC_FOLLOW_SPRITE_SPEED:width_1:1 | PROVEN | Current followed sprite outer width used as the reflection offset. | pixels |  |
| local:CALC_FOLLOW_SPRITE_SPEED:foll_spr:1 | PROVEN | Follow-sprite selector used as layer index or type-specific subtype in this helper. | index/subtype |  |
| local:CALC_FOLLOW_SPRITE_SPEED:layer_1:1 | PROVEN | Current/next frame base in the frame-major layer table. | pointer |  |
| local:CALC_FOLLOW_SPRITE_SPEED:layer_2:1 | PROVEN | Current/next frame base in the frame-major layer table. | pointer |  |
| local:CALC_FOLLOW_SPRITE_SPEED:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| local:CALC_FOLLOW_SPRITE_SPEED:anim_1:1 | PROVEN | Current/next animation descriptor supplied for layer displacement calculation. | pointer |  |
| local:CALC_FOLLOW_SPRITE_SPEED:anim_2:1 | PROVEN | Current/next animation descriptor supplied for layer displacement calculation. | pointer |  |
| local:CALC_FOLLOW_SPRITE_SPEED:anim_frame_2:1 | PROVEN | Frame selector in the next supplied animation. | frame index |  |
| function:GET_SPRITE_POS | PROVEN | Write the inner sprite rectangle for a layer of the current object animation when its sprite index and reference id are nonzero. | see parameters/result contract | 1:0dbb4 |
| local:GET_SPRITE_POS:anim:1 | PROVEN | Current object animation descriptor. | pointer |  |
| local:GET_SPRITE_POS:layer:1 | PROVEN | Selected layer in the current animation frame. | pointer |  |
| local:GET_SPRITE_POS:sprite_ind:1 | PROVEN | Sprite-table index loaded from the selected layer. | sprite index |  |
| local:GET_SPRITE_POS:sprite:1 | PROVEN | Descriptor for the layer-selected sprite. | pointer |  |
| local:GET_SPRITE_POS:res:1 | PROVEN | Whether nonzero sprite index and reference id permit bounds output. | boolean |  |
| local:GET_SPRITE_POS:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| local:GET_SPRITE_POS:index:1 | PROVEN | Layer index within the animation frame. | layer index |  |
| local:GET_SPRITE_POS:x:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| local:GET_SPRITE_POS:y:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| local:GET_SPRITE_POS:w:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| local:GET_SPRITE_POS:h:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| function:GET_RAY_ZDC | PROVEN | Write a state-dependent Rayman collision rectangle and optionally project it around the object anchor. | see parameters/result contract | 1:0dcfc |
| local:GET_RAY_ZDC:frame:1 | PROVEN | Current frame index reduced by32 once for the state7 rectangle test. | frame index |  |
| local:GET_RAY_ZDC:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| local:GET_RAY_ZDC:x:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| local:GET_RAY_ZDC:y:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| local:GET_RAY_ZDC:w:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| local:GET_RAY_ZDC:h:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| function:GET_BB1_ZDCs | PROVEN | Derive two BB1 collision rectangles from selected sprite bounds and current animation height. | see parameters/result contract | 1:0dec0 |
| local:GET_BB1_ZDCs:spr_ind:1 | PROVEN | Animation layer index selected for BB1 collision bounds. | layer index |  |
| local:GET_BB1_ZDCs:h_loc:1 | PROVEN | Full current animation bounding height. | pixels |  |
| local:GET_BB1_ZDCs:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| local:GET_BB1_ZDCs:x_1:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| local:GET_BB1_ZDCs:y_1:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| local:GET_BB1_ZDCs:w_1:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| local:GET_BB1_ZDCs:h_1:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| local:GET_BB1_ZDCs:x_2:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| local:GET_BB1_ZDCs:y_2:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| local:GET_BB1_ZDCs:w_2:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| local:GET_BB1_ZDCs:h_2:1 | PROVEN | Output pointer for the corresponding rectangle coordinate/dimension. | pixels |  |
| function:calc_obj_dir | PROVEN | Set object flip_x from the sign of Rayman anchor x minus object anchor x. | see parameters/result contract | 1:0dfb8 |
| local:calc_obj_dir:dx:1 | PROVEN | Signed coordinate difference on the corresponding axis. | pixels |  |
| local:calc_obj_dir:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| function:OBJ_IN_ZONE | PROVEN | Test the object detection-zone result with type-specific acceptance of values above one. | see parameters/result contract | 1:0dff4 |
| local:OBJ_IN_ZONE:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| function:calc_obj_pos | PROVEN | Convert object world coordinates to stored screen coordinates using the camera offsets. | see parameters/result contract | 1:0e030 |
| local:calc_obj_pos:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| function:makeUturn | PROVEN | Apply type- and state-specific facing/script changes when an object turns. | see parameters/result contract | 1:0e05c |
| local:makeUturn:label:1 | PROVEN | Script jump label selected by turning logic, or255 for no label. | script label |  |
| local:makeUturn:foll_spr:1 | UNKNOWN | Declared byte temporary with no uses in this reconstruction. | unknown |  |
| local:makeUturn:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| function:BTYP | PROVEN | Return the collision-type byte of an in-bounds tile, or zero outside the map. | see parameters/result contract | 1:0e588 |
| local:BTYP:x:1 | PROVEN | Promoted signed tile coordinate for the bounds check. | tiles |  |
| local:BTYP:y:1 | PROVEN | Promoted signed tile coordinate for the bounds check. | tiles |  |
| local:BTYP:tile_x:1 | PROVEN | Tile coordinate of the corresponding centre/side sample. | tiles |  |
| local:BTYP:tile_y:1 | PROVEN | Tile coordinate of the corresponding centre/side sample. | tiles |  |
| function:calc_btyp_square | PROVEN | Sample centre, left, right, above and below block types around an object anchor. | see parameters/result contract | 1:0e608 |
| local:calc_btyp_square:x:1 | PROVEN | Signed16 object anchor coordinate used by collision sampling. | pixels |  |
| local:calc_btyp_square:y:1 | PROVEN | Signed16 object anchor coordinate used by collision sampling. | pixels |  |
| local:calc_btyp_square:tile_x_right:1 | PROVEN | Tile coordinate of the corresponding centre/side sample. | tiles |  |
| local:calc_btyp_square:tile_x:1 | PROVEN | Tile coordinate of the corresponding centre/side sample. | tiles |  |
| local:calc_btyp_square:tile_x_left:1 | PROVEN | Tile coordinate of the corresponding centre/side sample. | tiles |  |
| local:calc_btyp_square:tile_y:1 | PROVEN | Tile coordinate of the corresponding centre/side sample. | tiles |  |
| local:calc_btyp_square:x_offset:1 | PROVEN | Side sampling distance16,8 or4 according to object type and tiny mode. | pixels |  |
| local:calc_btyp_square:center_btyp:1 | PROVEN | Centre collision type after optional airborne bloc_floor adjustment. | block type id |  |
| local:calc_btyp_square:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| function:DO_OBJ_REBOND_EN_X | PROVEN | Negate horizontal speed when either side block has flag bit 4. | see parameters/result contract | 1:0e76c |
| local:DO_OBJ_REBOND_EN_X:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| function:calc_btyp | PROVEN | Select the effective supporting block type and perform type-specific side-edge turning and Rayman edge bookkeeping. | see parameters/result contract | 1:0e790 |
| local:calc_btyp:btyp:1 | PROVEN | Effective candidate collision type being selected. | block type id |  |
| local:calc_btyp:btypes_1_2_solid:1 | PROVEN | Side-solid bit pair0..3, then chosen side with evolving0/1 encoding. | bitset then side selector |  |
| local:calc_btyp:x:1 | PROVEN | Signed16 object anchor coordinate used by collision sampling. | pixels |  |
| local:calc_btyp:y:1 | PROVEN | Signed16 object anchor coordinate used by collision sampling. | pixels |  |
| local:calc_btyp:foll_spr:1 | PROVEN | Follow-sprite selector used as layer index or type-specific subtype in this helper. | index/subtype |  |
| local:calc_btyp:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| function:init_obj_in_the_air | PROVEN | Clear the two byte gravity accumulators. | see parameters/result contract | 1:0eaa8 |
| local:init_obj_in_the_air:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| function:make_my_fruit_go_down | PROVEN | Accelerate horizontal speed below magnitude32 and select the type-dependent falling state. | see parameters/result contract | 1:0eab4 |
| local:make_my_fruit_go_down:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| local:make_my_fruit_go_down:x_accel:1 | PROVEN | Horizontal speed increment applied in the falling-state magnitude guard. | speed units/tick |  |
| function:switchOff | PROVEN | On the timed animation-end event, clear object alive/active flags and remove its always-object entry with exceptions. | see parameters/result contract | 1:0eb18 |
| local:switchOff:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| function:obj_hurt | PROVEN | Subtract current fist damage from target hit points with saturation at zero. | see parameters/result contract | 1:0eb80 |
| local:obj_hurt:target:1 | PROVEN | Object whose hit points are reduced. | pointer |  |
| function:Projectil_to_RM | PROVEN | Compute facing-constrained projectile speeds towards the stored follow_x/y target, then advance that stored target. | see parameters/result contract | 1:0eba0 |
| local:Projectil_to_RM:diff_x:1 | PROVEN | Signed16 horizontal difference from object anchor to stored target plus Rayman x anchor offset. | pixels |  |
| local:Projectil_to_RM:diff_y:1 | PROVEN | Signed16 vertical difference from object anchor to stored target plus Rayman y anchor offset. | pixels |  |
| local:Projectil_to_RM:diff_x_abs:1 | PROVEN | Signed16 stored absolute horizontal difference. | pixels |  |
| local:Projectil_to_RM:diff_y_abs:1 | PROVEN | Signed16 stored absolute vertical difference. | pixels |  |
| local:Projectil_to_RM:unk_x:1 | PROVEN | Horizontal quotient/sign used to advance the stored projectile target by twice this value. | dimensionless integer |  |
| local:Projectil_to_RM:unk_y:1 | PROVEN | Vertical quotient/sign used to advance the stored projectile target by twice this value. | dimensionless integer |  |
| local:Projectil_to_RM:spd_x:1 | PROVEN | Wide temporary applying the special signed16-pixel horizontal speed adjustment. | output speed units |  |
| local:Projectil_to_RM:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| local:Projectil_to_RM:out_spd_x:1 | PROVEN | Output pointer to computed projectile speed on the corresponding axis. | object speed units |  |
| local:Projectil_to_RM:out_spd_y:1 | PROVEN | Output pointer to computed projectile speed on the corresponding axis. | object speed units |  |
| local:Projectil_to_RM:new_spd_x:1 | PROVEN | Supplied horizontal speed magnitude before direction/proportional adjustment. | object speed units |  |
| local:Projectil_to_RM:new_spd_y:1 | PROVEN | Supplied vertical speed magnitude before direction/proportional adjustment. | object speed units |  |
| function:del_actobj | PROVEN | Clear object activity, find its id in actobj, shift entries using the original j-1 destination, and decrement the count. | see parameters/result contract | 1:0ee18 |
| local:del_actobj:j:1 | PROVEN | Loop or append/removal slot in the object/list sequence used by this helper. | object/list index |  |
| local:del_actobj:i:1 | PROVEN | Loop or append/removal slot in the object/list sequence used by this helper. | object/list index |  |
| local:del_actobj:active:1 | PROVEN | Object selected from the current active-list id slot. | pointer |  |
| local:del_actobj:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| function:calc_let_Width | PROVEN | Return the outer glyph width from the selected font or numeric sprite bank. | see parameters/result contract | 1:0eeb8 |
| local:calc_let_Width:sprite:1 | PROVEN | Font glyph sprite descriptor chosen by bank and glyph number. | pointer |  |
| local:calc_let_Width:font_size:1 | PROVEN | Font selector0/1/2 controlling bank and text metrics. | font selector |  |
| local:calc_let_Width:num_let:1 | PROVEN | Decoded/adjusted glyph selector, with width determined by its declaration. | glyph index |  |
| function:calc_let_Width2 | PROVEN | Return glyph inner width plus its low-nibble x inset. | see parameters/result contract | 1:0ef1c |
| local:calc_let_Width2:sprite:1 | PROVEN | Font glyph sprite descriptor chosen by bank and glyph number. | pointer |  |
| local:calc_let_Width2:font_size:1 | PROVEN | Font selector0/1/2 controlling bank and text metrics. | font selector |  |
| local:calc_let_Width2:num_let:1 | PROVEN | Decoded/adjusted glyph selector, with width determined by its declaration. | glyph index |  |
| function:INIT_TEXT_TO_DISPLAY | PROVEN | Clear text and two drawing-control bytes for display slots1 through10. | see parameters/result contract | 1:0ef80 |
| local:INIT_TEXT_TO_DISPLAY:i:1 | PROVEN | Loop or append/removal slot in the object/list sequence used by this helper. | object/list index |  |
| function:deter_num_let | PROVEN | Decode one text byte or a bracket-prefixed three-character numeric sprite escape into a glyph number. | see parameters/result contract | 1:0efb4 |
| local:deter_num_let:result:1 | PROVEN | Decoded glyph number, zero for unrecognised bytes. | glyph index |  |
| local:deter_num_let:next_text:1 | PROVEN | Three numeric escape bytes plus a zero terminator. | encoded bytes |  |
| local:deter_num_let:c:1 | PROVEN | Current game-encoded text byte. | encoded byte |  |
| local:deter_num_let:next_chars:1 | PROVEN | Pointer to bytes following the escape introducer. | pointer |  |
| function:calc_num_let_spr | PROVEN | Add41 modulo256 to the pointed byte glyph number when font_size equals1. | see parameters/result contract | 1:0f194 |
| local:calc_num_let_spr:font_size:1 | PROVEN | Font selector0/1/2 controlling bank and text metrics. | font selector |  |
| local:calc_num_let_spr:letter_num:1 | PROVEN | Decoded/adjusted glyph selector, with width determined by its declaration. | glyph index |  |
| function:calc_largmax_text | PROVEN | Accumulate glyph advances from the character after char_index until slash or NUL. | see parameters/result contract | 1:0f1b0 |
| local:calc_largmax_text:width:1 | PROVEN | Accumulated line glyph advance. | pixels |  |
| local:calc_largmax_text:i:1 | PROVEN | Text byte scan position. | byte index |  |
| local:calc_largmax_text:num_let:1 | PROVEN | Decoded/adjusted glyph selector, with width determined by its declaration. | glyph index |  |
| local:calc_largmax_text:text:1 | PROVEN | Encoded text bytes consumed or modified by this helper. | encoded bytes |  |
| local:calc_largmax_text:char_index:1 | PROVEN | Starting text index; scan begins after this position. | byte index |  |
| local:calc_largmax_text:space_width:1 | PROVEN | Advance for a literal space byte. | pixels |  |
| local:calc_largmax_text:char_spacing:1 | PROVEN | Amount subtracted from each nonzero glyph advance. | pixels |  |
| local:calc_largmax_text:font_size:1 | PROVEN | Font selector0/1/2 controlling bank and text metrics. | font selector |  |
| function:INIT_TXT_BOX | PROVEN | Compute display-box rectangle from text glyph advances and the original slash-delimited line accounting. | see parameters/result contract | 1:0f21c |
| local:INIT_TXT_BOX:i:1 | PROVEN | Text byte scan position. | byte index |  |
| local:INIT_TXT_BOX:text:1 | PROVEN | Encoded text bytes consumed or modified by this helper. | encoded bytes |  |
| local:INIT_TXT_BOX:max_width:1 | PROVEN | Largest width recorded at a slash, or full width on a slash-free line. | pixels |  |
| local:INIT_TXT_BOX:num_let:1 | PROVEN | Decoded/adjusted glyph selector, with width determined by its declaration. | glyph index |  |
| local:INIT_TXT_BOX:num_lines:1 | PROVEN | Slash-derived line count with original post-loop adjustment. | lines |  |
| local:INIT_TXT_BOX:max_chars:1 | PROVEN | Maximum nonzero-glyph/space count recorded at slash boundaries; not used in final rectangle. | characters |  |
| local:INIT_TXT_BOX:total_chars:1 | PROVEN | Current line glyph/space count; not used in final rectangle. | characters |  |
| local:INIT_TXT_BOX:is_slash:1 | PROVEN | Whether any slash delimiter was observed. | boolean |  |
| local:INIT_TXT_BOX:char_spacing:1 | PROVEN | Amount subtracted from each nonzero glyph advance. | pixels |  |
| local:INIT_TXT_BOX:space_width:1 | PROVEN | Advance for a literal space byte. | pixels |  |
| local:INIT_TXT_BOX:line_height:1 | PROVEN | Font-dependent line pitch36,23 or15. | pixels |  |
| local:INIT_TXT_BOX:total_width:1 | PROVEN | Current slash-delimited line advance. | pixels |  |
| local:INIT_TXT_BOX:font_size:1 | PROVEN | Font selector0/1/2 controlling bank and text metrics. | font selector |  |
| local:INIT_TXT_BOX:box:1 | PROVEN | Display descriptor whose text and font determine computed rectangle fields. | pointer |  |
| function:Deter_Option_Caract | PROVEN | Replace the selected text position with a..d for key0..3, optionally searching for the last dollar sign. | see parameters/result contract | 1:0f4a0 |
| local:Deter_Option_Caract:i:1 | PROVEN | Text byte scan position. | byte index |  |
| local:Deter_Option_Caract:text:1 | PROVEN | Encoded text bytes consumed or modified by this helper. | encoded bytes |  |
| local:Deter_Option_Caract:key:1 | PROVEN | Selector0..3 mapping to characters a..d. | key selector |  |
| local:Deter_Option_Caract:offset:1 | PROVEN | Replacement index or255 sentinel requesting a dollar-sign search. | byte index or sentinel |  |
| function:SwapAB | PROVEN | Exchange the pointed signed-16 values. | see parameters/result contract | 1:0f50c |
| local:SwapAB:temp:1 | PROVEN | Original value at the second pointed location. | signed16 value |  |
| local:SwapAB:a:1 | PROVEN | Pointer to a signed16 value exchanged by SwapAB. | pointer |  |
| local:SwapAB:b:1 | PROVEN | Pointer to a signed16 value exchanged by SwapAB. | pointer |  |
| function:Bresenham | PROVEN | Visit spaced integer line points through a signed-16 callback using an endpoint fraction divided by128. | see parameters/result contract | 1:0f520 |
| local:Bresenham:dx:1 | PROVEN | Signed coordinate difference on the corresponding axis. | pixels |  |
| local:Bresenham:dy:1 | PROVEN | Signed coordinate difference on the corresponding axis. | pixels |  |
| local:Bresenham:reversed:1 | PROVEN | Whether endpoint normalisation reversed traversal orientation. | boolean |  |
| local:Bresenham:counter:1 | PROVEN | Major-axis selector initially, then callback cadence counter. | mode then line steps |  |
| local:Bresenham:gap:1 | PROVEN | Local copy of callback gap threshold. | line steps |  |
| local:Bresenham:x_straight:1 | PROVEN | Shallow-line error increment for a straight step. | scaled pixel error |  |
| local:Bresenham:x_diagonal:1 | PROVEN | Shallow-line error increment for a diagonal step. | scaled pixel error |  |
| local:Bresenham:y_straight:1 | PROVEN | Steep-line error increment for a straight step. | scaled pixel error |  |
| local:Bresenham:y_diagonal:1 | PROVEN | Steep-line error increment for a diagonal step. | scaled pixel error |  |
| local:Bresenham:step_x:1 | PROVEN | Signed minor-axis traversal direction selected from -1,0,1. | pixels/step |  |
| local:Bresenham:step_y:1 | PROVEN | Signed minor-axis traversal direction selected from -1,0,1. | pixels/step |  |
| local:Bresenham:error:1 | PROVEN | Bresenham signed error accumulator. | scaled pixel error |  |
| local:Bresenham:x:1 | PROVEN | Current traversal coordinate on the corresponding axis. | pixels |  |
| local:Bresenham:y:1 | PROVEN | Current traversal coordinate on the corresponding axis. | pixels |  |
| local:Bresenham:end:1 | PROVEN | Fractional traversal terminal major-axis coordinate. | pixels |  |
| local:Bresenham:func:1 | PROVEN | Callback receiving signed16 line point coordinates. | function pointer |  |
| local:Bresenham:origin_x:1 | PROVEN | Line endpoint coordinate, potentially exchanged during direction normalisation. | pixels |  |
| local:Bresenham:origin_y:1 | PROVEN | Line endpoint coordinate, potentially exchanged during direction normalisation. | pixels |  |
| local:Bresenham:dest_x:1 | PROVEN | Line endpoint coordinate, potentially exchanged during direction normalisation. | pixels |  |
| local:Bresenham:dest_y:1 | PROVEN | Line endpoint coordinate, potentially exchanged during direction normalisation. | pixels |  |
| local:Bresenham:param_6:1 | PROVEN | Gap threshold used to decide callback cadence. | line steps |  |
| local:Bresenham:percent:1 | PROVEN | Fraction of line extent with denominator128. | 1/128 full extent |  |
| function:init_finBossLevel | PROVEN | Clear the twelve named progression bits in finBosslevel. | see parameters/result contract | 1:0f880 |
| function:Change_Wait_Anim | PROVEN | Swap the state0/0 and state0/56 animation ids, copy the latter into state3/20, and toggle first_boss_meet. | see parameters/result contract | 1:0f8a4 |
| local:Change_Wait_Anim:ray_eta:1 | PROVEN | Rayman main/substate table pointer used by the waiting-animation swap. | pointer |  |
| local:Change_Wait_Anim:prev_00:1 | PROVEN | Previous animation id of state0/0 before the waiting-animation swap. | animation index |  |
| function:add_alwobj | PROVEN | Append an object id to the level_obj array and increment its signed-16 count. | see parameters/result contract | 1:0f8e8 |
| local:add_alwobj:index:1 | PROVEN | Loop or append/removal slot in the object/list sequence used by this helper. | object/list index |  |
| local:add_alwobj:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| function:del_alwobj | PROVEN | Remove the first matching id from level_obj by replacing it with the last counted id. | see parameters/result contract | 1:0f918 |
| local:del_alwobj:i:1 | PROVEN | Loop or append/removal slot in the object/list sequence used by this helper. | object/list index |  |
| local:del_alwobj:obj_index:1 | PROVEN | Object id supplied to the object-list operation. | object id |  |
| function:FatalError | PROVEN | Stop CD playback, enter text mode, print diagnostics, optionally wait for input, then exit with -1. | see parameters/result contract | 1:0f988 |
| local:FatalError:message:1 | PROVEN | Error message string passed to FatalError diagnostic output. | text pointer |  |
| function:sprite_of_ref | PROVEN | Return the first sprite-table index whose reference id matches, or zero if absent. | see parameters/result contract | 1:0fa00 |
| local:sprite_of_ref:i:1 | PROVEN | Sprite-table scan index. | sprite index |  |
| local:sprite_of_ref:obj:1 | PROVEN | Object record operated on by this helper. | pointer |  |
| local:sprite_of_ref:ref:1 | PROVEN | Reference id to find in the sprite table. | sprite reference id |  |
| global:horloge | PROVEN | Global phase-counter bank indexed by animation period. | ticks | 2:3f9f5 |
| global:level | PROVEN | Current level packed object-array descriptor. | objects | 2:3cdf0 |
| global:saveobj | PROVEN | Packed snapshot of object alive/active flags. | two-bit entries | 2:3ce08 |
| global:flags | PROVEN | Per-object-type behaviour flag table. | bitset | 2:257c |
| global:PROJ_CENTER_X | PROVEN | Global horizontal projection centre. | pixels | 2:3f838 |
| global:PROJ_CENTER_Y | PROVEN | Global vertical projection centre. | pixels | 2:3f836 |
| global:zoom_mode | PROVEN | Optional projection switch read by get_proj_dist. | zero/nonzero | 2:3fa63 |
| global:actobj | PROVEN | Current active-object id array and count. | object ids | 2:3ef5c |
| global:num_world | STRONG | Current world discriminator used by type-specific turn-probe offsets. | world id | 2:3f88c |
| global:block_flags | PROVEN | Flags indexed by collision tile type. | bitset | 2:253c |
| global:ray | PROVEN | Live Rayman object record. | obj_t | 2:3e650 |
| global:xmap | PROVEN | Horizontal camera/map origin subtracted from world coordinates. | pixels | 2:3f89e |
| global:ymap | PROVEN | Vertical camera/map origin subtracted from world coordinates. | pixels | 2:3f8a6 |
| global:mp | PROVEN | Current map dimensions and tile-record descriptor. | tiles | 2:3e638 |
| global:RayEvts | STRONG | Packed Rayman ability/mode state. | bitset | 2:3f81a |
| global:ray_last_ground_btyp | PROVEN | Sticky classification of the last recognised supporting surface as ordinary versus slippery. | boolean | 2:3fa72 |
| global:ray_mode | STRONG | Rayman/game mode discriminator. | mode | 2:3f868 |
| global:poing | STRONG | Rayman fist state including damage and fixed-point vertical accumulator. | mixed | 2:3e770 |
| global:alpha | PROVEN | Sprite font bank selected by font_size2. | font_t | 2:3e6e0 |
| global:alpha2 | PROVEN | Sprite font bank selected by font_size0/1. | font_t | 2:3edc8 |
| global:alpha_numbers | PROVEN | Sprite bank selected by glyph numbers>=1000. | font_t | 2:3e644 |
| global:text_to_display | PROVEN | Array of eleven game text display descriptors. | display_item_t[11] | 2:3c4a0 |
| global:finBosslevel | STRONG | Packed boss/progression flag record. | bitset | 2:3f81e |
| global:first_boss_meet | PROVEN | Byte toggled while swapping Rayman waiting-animation entries. | 0/1 in intended domain | 2:3fa71 |
| global:level_obj | PROVEN | Always-object id list and count used by activation processing. | object ids | 2:3cdf8 |
| global:errno | PROVEN | Watcom runtime error value obtained through __get_errno_ptr. | runtime error code | 2:574fc |
| global:input_mode | PROVEN | Input-mode byte whose value1 suppresses FatalError key waiting. | mode | 2:3fa51 |
| type:obj_t | PROVEN | Packed DOS game object record. | record |  |
| type:eta_t | PROVEN | One8-byte main/substate entry. | record |  |
| type:sprite_t | PROVEN | Twelve-byte sprite image/bounds descriptor. | record |  |
| type:anim_t | PROVEN | Frame/layer animation descriptor. | record |  |
| type:anim_frame_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| type:anim_layer_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| type:obj_flags_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| type:cmd_context_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| type:level_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| type:level_obj_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| type:active_objects_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| type:obj_type_flags_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| type:map_data_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| type:map_tile_t | PROVEN | Six-byte map cell record with separate visual and collision selectors. | record |  |
| type:block_flags_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| type:rayevts_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| type:finbosslevel_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| type:poing_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| type:font_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| type:display_item_t | PROVEN | Packed DOS record; member meanings and remaining unknowns are recorded separately. | record |  |
| field:obj_t.sprites | PROVEN | Pointer to the object-specific sprites table. | pointer | +0x0, 4 B |
| field:obj_t.animations | PROVEN | Pointer to the object-specific animations table. | pointer | +0x4, 4 B |
| field:obj_t.img_buffer | STRONG | Object sprite pixel-atlas buffer. | pointer | +0x8, 4 B |
| field:obj_t.eta | PROVEN | Pointer to the object-specific eta table. | pointer | +0xc, 4 B |
| field:obj_t.cmds | STRONG | Object script stream/context member cmds. | script dependent | +0x10, 4 B |
| field:obj_t.cmd_labels | STRONG | Object script stream/context member cmd_labels. | script dependent | +0x14, 4 B |
| field:obj_t.cmd_contexts | STRONG | Object script stream/context member cmd_contexts. | script dependent | +0x18, 4 B |
| field:obj_t.field_1C | UNKNOWN | Neutral obj_t.field_1C storage/view; its intended meaning is not established by this pilot. | unknown | +0x1c, 4 B |
| field:obj_t.link_has_gendoor | UNKNOWN | Neutral obj_t.link_has_gendoor storage/view; its intended meaning is not established by this pilot. | unknown | +0x20, 4 B |
| field:obj_t.is_active | PROVEN | Object participation flag saved as the high bit of each two-bit snapshot entry. | boolean | +0x24, 4 B |
| field:obj_t.x | PROVEN | World-space live-object origin coordinate on the x axis, with loader-descriptor reuse. | pixels (live objects) | +0x28, 4 B |
| field:obj_t.y | PROVEN | World-space live-object origin coordinate on the y axis, with loader-descriptor reuse. | pixels (live objects) | +0x2c, 4 B |
| field:obj_t.active_flag | UNKNOWN | Neutral obj_t.active_flag storage/view; its intended meaning is not established by this pilot. | unknown | +0x30, 4 B |
| field:obj_t.id | PROVEN | Object identifier stored in the active and always-object id lists. | object index | +0x34, 2 B |
| field:obj_t.screen_x | PROVEN | Stored screen-space origin coordinate screen_x after camera subtraction. | pixels | +0x36, 2 B |
| field:obj_t.screen_y | PROVEN | Stored screen-space origin coordinate screen_y after camera subtraction. | pixels | +0x38, 2 B |
| field:obj_t.sub_speed | UNKNOWN | Neutral obj_t.sub_speed storage/view; its intended meaning is not established by this pilot. | unknown | +0x3a, 2 B |
| field:obj_t.init_x | STRONG | Initial position component init_x used by reset and relative-displacement logic. | pixels | +0x3c, 2 B |
| field:obj_t.init_y | STRONG | Initial position component init_y used by reset and relative-displacement logic. | pixels | +0x3e, 2 B |
| field:obj_t.speed_x | PROVEN | Signed per-axis movement quantity whose scale is selected by object-type flags. | pixels/tick or 1/16 pixel/tick | +0x40, 2 B |
| field:obj_t.speed_y | PROVEN | Signed per-axis movement quantity whose scale is selected by object-type flags. | pixels/tick or 1/16 pixel/tick | +0x42, 2 B |
| field:obj_t.nb_sprites | PROVEN | Count bounding the object sprite-table search. | sprites | +0x44, 2 B |
| field:obj_t.cmd_offset | STRONG | Object script stream/context member cmd_offset. | script dependent | +0x46, 2 B |
| field:obj_t.nb_cmd | STRONG | Object script stream/context member nb_cmd. | script dependent | +0x48, 2 B |
| field:obj_t.follow_id | STRONG | Identifier of an object being followed, with type-specific reuse. | object id or scratch | +0x4a, 2 B |
| field:obj_t.follow_y | PROVEN | Context-dependent scratch value: animation-layer displacement, projectile target coordinate, or other type-specific state. | context dependent | +0x4c, 2 B |
| field:obj_t.follow_x | PROVEN | Context-dependent scratch value: animation-layer displacement, projectile target coordinate, or other type-specific state. | context dependent | +0x4e, 2 B |
| field:obj_t.param | PROVEN | Type-specific signed scratch parameter, including Rayman edge classification in calc_btyp. | context dependent | +0x50, 2 B |
| field:obj_t.link | UNKNOWN | Neutral obj_t.link storage/view; its intended meaning is not established by this pilot. | unknown | +0x52, 2 B |
| field:obj_t.ray_dist | PROVEN | Context-dependent Rayman tile index or signed object-to-Rayman support distance. | tile index or pixels | +0x54, 2 B |
| field:obj_t.iframes_timer | STRONG | Signed invulnerability/response timer. | ticks | +0x56, 2 B |
| field:obj_t.test_block_index | PROVEN | Last row-major map tile index used by the changed-block test. | tile index | +0x58, 2 B |
| field:obj_t.scale | PROVEN | Projection-depth parameter whose denominator is signed_scale+256. | projection depth units | +0x5a, 2 B |
| field:obj_t.zdc | UNKNOWN | Neutral obj_t.zdc storage/view; its intended meaning is not established by this pilot. | unknown | +0x5c, 2 B |
| field:obj_t.active_timer | UNKNOWN | Neutral obj_t.active_timer storage/view; its intended meaning is not established by this pilot. | unknown | +0x5e, 2 B |
| field:obj_t.type | PROVEN | Unsigned object type discriminator indexing flags and selecting behaviour. | object type id | +0x60, 2 B |
| field:obj_t.btypes | PROVEN | Five cached collision block types in centre,left,right,above,below order. | block type id | +0x62, 5 B |
| field:obj_t.unused_padding | UNKNOWN | Neutral obj_t.unused_padding storage/view; its intended meaning is not established by this pilot. | unknown | +0x67, 1 B |
| field:obj_t.offset_bx | PROVEN | Horizontal object anchor offset used for collision probes and reflection centre. | pixels | +0x68, 1 B |
| field:obj_t.offset_by | PROVEN | Vertical object anchor offset used for foot/block probes and projection centre. | pixels | +0x69, 1 B |
| field:obj_t.anim_index | PROVEN | Current animation selector or current frame selector (anim_index). | index | +0x6a, 1 B |
| field:obj_t.anim_frame | PROVEN | Current animation selector or current frame selector (anim_frame). | index | +0x6b, 1 B |
| field:obj_t.sub_etat | PROVEN | Byte index into the main/substate animation-state table (sub_etat). | state index | +0x6c, 1 B |
| field:obj_t.main_etat | PROVEN | Byte index into the main/substate animation-state table (main_etat). | state index | +0x6d, 1 B |
| field:obj_t.init_sub_etat | STRONG | Reset value for init_sub_etat. | state index or hit points | +0x6e, 1 B |
| field:obj_t.init_main_etat | STRONG | Reset value for init_main_etat. | state index or hit points | +0x6f, 1 B |
| field:obj_t.cmd | STRONG | Current script command discriminator. | opcode | +0x70, 1 B |
| field:obj_t.gravity_value_1 | STRONG | Byte gravity integration state component gravity_value_1. | context dependent | +0x71, 1 B |
| field:obj_t.gravity_value_2 | STRONG | Byte gravity integration state component gravity_value_2. | context dependent | +0x72, 1 B |
| field:obj_t.change_anim_mode | PROVEN | Animation transition mode set to1 by the state setters. | mode | +0x73, 1 B |
| field:obj_t.offset_hy | STRONG | Unsigned vertical upper-body/collision anchor offset. | pixels | +0x74, 1 B |
| field:obj_t.follow_sprite | PROVEN | Selected animation layer for following, also overloaded as Blacktoon behaviour selector. | layer index or subtype | +0x75, 1 B |
| field:obj_t.hit_points | PROVEN | Unsigned hit-point byte in obj_hurt, with type-specific reuse including signed deformation angle input. | hit points or type-specific state | +0x76, 1 B |
| field:obj_t.init_hit_points | STRONG | Reset value for init_hit_points. | state index or hit points | +0x77, 1 B |
| field:obj_t.init_flag | UNKNOWN | Neutral obj_t.init_flag storage/view; its intended meaning is not established by this pilot. | unknown | +0x78, 1 B |
| field:obj_t.hit_sprite | UNKNOWN | Neutral obj_t.hit_sprite storage/view; its intended meaning is not established by this pilot. | unknown | +0x79, 1 B |
| field:obj_t.detect_zone | STRONG | Type-dependent size/range parameter used by the detection-zone computation. | pixels or type-specific range | +0x7a, 1 B |
| field:obj_t.detect_zone_flag | PROVEN | Detection-zone classification tested by OBJ_IN_ZONE. | classification | +0x7b, 1 B |
| field:obj_t.cmd_context_depth | STRONG | Object script stream/context member cmd_context_depth. | script dependent | +0x7c, 1 B |
| field:obj_t.config | PROVEN | Type-specific byte configuration tested for Blacktoon edge-turning. | context dependent | +0x7d, 1 B |
| field:obj_t.display_prio | STRONG | Object drawing-order bucket selector. | priority bucket | +0x7e, 1 B |
| field:obj_t.timer | UNKNOWN | Neutral obj_t.timer storage/view; its intended meaning is not established by this pilot. | unknown | +0x7f, 1 B |
| field:obj_t.anim_count | STRONG | Number of animations in the object descriptor. | animations | +0x80, 1 B |
| field:obj_t.flags | PROVEN | Packed two-byte per-object flag record. | bitset | +0x81, 2 B |
| field:obj_t.field_83 | UNKNOWN | Neutral obj_t.field_83 storage/view; its intended meaning is not established by this pilot. | unknown | +0x83, 1 B |
| field:eta_t.speed_x_right | STRONG | Signed state-selected horizontal speed for the speed_x_right facing. | object speed units | +0x0, 1 B |
| field:eta_t.speed_x_left | STRONG | Signed state-selected horizontal speed for the speed_x_left facing. | object speed units | +0x1, 1 B |
| field:eta_t.anim_index | PROVEN | Animation id selected by this main/substate entry. | animation index | +0x2, 1 B |
| field:eta_t.next_main_etat | PROVEN | State indices selected when an animation crosses its frame bounds. | state index | +0x3, 1 B |
| field:eta_t.next_sub_etat | PROVEN | State indices selected when an animation crosses its frame bounds. | state index | +0x4, 1 B |
| field:eta_t.anim_speed | PROVEN | Packed animation timing/control byte whose low nibble selects the clock. | clock index/control bits | +0x5, 1 B |
| field:eta_t.anim_speed_bits | PROVEN | Overlapping nibble/bit alias container for the corresponding eta byte. | bitset | +0x5, 1 B |
| field:eta_t.anim_speed_bits.low | PROVEN | Low nibble alias of the animation timing byte. | clock index | +0x5, 1 B |
| field:eta_t.anim_speed_bits.high | PROVEN | High-nibble state control selector used by gravity/helicopter processing. | control code | +0x5, 1 B |
| field:eta_t.sound_index | STRONG | State-selected sound event index. | sound id | +0x6, 1 B |
| field:eta_t.flags | PROVEN | State behaviour bitset, including reverse playback0x10 and Rayman rectangle selection0x40. | bitset | +0x7, 1 B |
| field:eta_t.flags_bits | PROVEN | Overlapping nibble/bit alias container for the corresponding eta byte. | bitset | +0x7, 1 B |
| field:eta_t.flags_bits.bit0 | UNKNOWN | Neutral eta_t.flags_bits.bit0 storage/view; its intended meaning is not established by this pilot. | unknown | +0x7, 1 B |
| field:eta_t.flags_bits.bit1 | UNKNOWN | Neutral eta_t.flags_bits.bit1 storage/view; its intended meaning is not established by this pilot. | unknown | +0x7, 1 B |
| field:eta_t.flags_bits.bit2 | UNKNOWN | Neutral eta_t.flags_bits.bit2 storage/view; its intended meaning is not established by this pilot. | unknown | +0x7, 1 B |
| field:eta_t.flags_bits.bit3 | UNKNOWN | Neutral eta_t.flags_bits.bit3 storage/view; its intended meaning is not established by this pilot. | unknown | +0x7, 1 B |
| field:eta_t.flags_bits.bit4 | PROVEN | Alias for reverse animation playback flag0x10. | boolean | +0x7, 1 B |
| field:eta_t.flags_bits.bit5 | UNKNOWN | Neutral eta_t.flags_bits.bit5 storage/view; its intended meaning is not established by this pilot. | unknown | +0x7, 1 B |
| field:eta_t.flags_bits.bit6 | PROVEN | Alias for flag0x40 selecting the small14-pixel Rayman collision rectangle in GET_RAY_ZDC. | boolean | +0x7, 1 B |
| field:eta_t.flags_bits.bit7 | UNKNOWN | Neutral eta_t.flags_bits.bit7 storage/view; its intended meaning is not established by this pilot. | unknown | +0x7, 1 B |
| field:sprite_t.offset_in_atlas | STRONG | Pixel offset of this sprite in its object atlas buffer. | bytes | +0x0, 4 B |
| field:sprite_t.id | PROVEN | Sprite reference id, with0 treated as an absent sprite in GET_SPRITE_POS. | reference id | +0x4, 1 B |
| field:sprite_t.outer_width | STRONG | Outer sprite rectangle dimension outer_width. | pixels | +0x5, 1 B |
| field:sprite_t.outer_height | STRONG | Outer sprite rectangle dimension outer_height. | pixels | +0x6, 1 B |
| field:sprite_t.inner_width | PROVEN | Returned inner sprite rectangle dimension inner_width. | pixels | +0x7, 1 B |
| field:sprite_t.inner_height | PROVEN | Returned inner sprite rectangle dimension inner_height. | pixels | +0x8, 1 B |
| field:sprite_t.sprite_pos | PROVEN | Overlapping raw/nibble views of the sprite inset byte. | bitset | +0x9, 1 B |
| field:sprite_t.sprite_pos.raw | PROVEN | Overlapping raw/nibble views of the sprite inset byte. | bitset | +0x9, 1 B |
| field:sprite_t.sprite_pos.bits | PROVEN | Overlapping raw/nibble views of the sprite inset byte. | bitset | +0x9, 1 B |
| field:sprite_t.sprite_pos.bits.x | PROVEN | Packed nonnegative sprite inset component sprite_pos.bits.x. | pixels | +0x9, 1 B |
| field:sprite_t.sprite_pos.bits.y | PROVEN | Packed nonnegative sprite inset component sprite_pos.bits.y. | pixels | +0x9, 1 B |
| field:sprite_t.color | PROVEN | Raw/bitfield view of the sprite color control byte. | bitset | +0xa, 1 B |
| field:sprite_t.color_bits | PROVEN | Raw/bitfield view of the sprite color_bits control byte. | bitset | +0xa, 1 B |
| field:sprite_t.color_bits.unused | UNKNOWN | Neutral sprite_t.color_bits.unused storage/view; its intended meaning is not established by this pilot. | unknown | +0xa, 1 B |
| field:sprite_t.color_bits.palette_index | STRONG | High-nibble palette selection. | palette index | +0xa, 1 B |
| field:sprite_t.flags | PROVEN | Raw/bitfield view of the sprite flags control byte. | bitset | +0xb, 1 B |
| field:sprite_t.flags_bits | PROVEN | Raw/bitfield view of the sprite flags_bits control byte. | bitset | +0xb, 1 B |
| field:sprite_t.flags_bits.unused0 | UNKNOWN | Neutral sprite_t.flags_bits.unused0 storage/view; its intended meaning is not established by this pilot. | unknown | +0xb, 1 B |
| field:sprite_t.flags_bits.flip_x | PROVEN | Sprite horizontal flip bit XORed with object flip to select the bounds reflection. | boolean | +0xb, 1 B |
| field:sprite_t.flags_bits.unused3 | UNKNOWN | Neutral sprite_t.flags_bits.unused3 storage/view; its intended meaning is not established by this pilot. | unknown | +0xb, 1 B |
| field:anim_t.layers | PROVEN | Pointer to frame-major animation layers. | pointer | +0x0, 4 B |
| field:anim_t.frames | PROVEN | Pointer to one bounding rectangle per animation frame. | pointer | +0x4, 4 B |
| field:anim_t.layers_per_frame | PROVEN | Packed layer-count and horizontal-speed clock selector. | count and bitset | +0x8, 2 B |
| field:anim_t.frames_count | PROVEN | Number of frames in this animation. | frames | +0xa, 2 B |
| field:anim_frame_t.x | PROVEN | Unsigned offset of the animation bounding rectangle (x). | pixels | +0x0, 1 B |
| field:anim_frame_t.y | PROVEN | Unsigned offset of the animation bounding rectangle (y). | pixels | +0x1, 1 B |
| field:anim_frame_t.width | PROVEN | Animation bounding rectangle dimension width. | pixels | +0x2, 1 B |
| field:anim_frame_t.height | PROVEN | Animation bounding rectangle dimension height. | pixels | +0x3, 1 B |
| field:anim_layer_t.mirrored | STRONG | Layer horizontal mirror selector used by the renderer. | boolean-like byte | +0x0, 1 B |
| field:anim_layer_t.x | PROVEN | Unsigned object-relative layer offset component x. | pixels | +0x1, 1 B |
| field:anim_layer_t.y | PROVEN | Unsigned object-relative layer offset component y. | pixels | +0x2, 1 B |
| field:anim_layer_t.sprite_index | PROVEN | Index into the owning object sprite table for this layer. | sprite index | +0x3, 1 B |
| field:obj_flags_t.hurtbyfist | STRONG | Fist-hit interaction flag. | boolean | +0x0, 1 B |
| field:obj_flags_t.command_test | STRONG | Stored result of the current script test. | boolean | +0x0, 1 B |
| field:obj_flags_t.alive | PROVEN | Low saved object-state bit, cleared at timed animation completion. | boolean | +0x0, 1 B |
| field:obj_flags_t.flip_x | PROVEN | Horizontal object reflection/facing flag. | boolean | +0x0, 1 B |
| field:obj_flags_t.read_commands | PROVEN | Per-object command mode used to select turn labels. | boolean | +0x0, 1 B |
| field:obj_flags_t.follow_enabled | STRONG | Enable support/follow-distance processing for the object. | boolean | +0x0, 1 B |
| field:obj_flags_t.flag_0x40 | UNKNOWN | Neutral obj_flags_t.flag_0x40 storage/view; its intended meaning is not established by this pilot. | unknown | +0x0, 1 B |
| field:obj_flags_t.anim_changed | STRONG | Flag used by animation update when the selected animation changes. | boolean | +0x0, 1 B |
| field:obj_flags_t.flag_0x100 | PROVEN | Ninth object flag written as movement-occurrence bookkeeping, with one original uninitialised producer. | boolean bit | +0x1, 1 B |
| field:cmd_context_t.cmd_offset | STRONG | Saved script command-context component cmd_offset. | script offset or repeat count | +0x0, 2 B |
| field:cmd_context_t.count | STRONG | Saved script command-context component count. | script offset or repeat count | +0x2, 2 B |
| field:level_t.objects | PROVEN | Base pointer of the packed level object array. | pointer | +0x0, 4 B |
| field:level_t.nb_objects | PROVEN | Count bounding the level object array. | objects | +0x4, 2 B |
| field:level_obj_t.obj_ids | PROVEN | Always-object list member obj_ids. | object ids or count | +0x0, 4 B |
| field:level_obj_t.nb_objects | PROVEN | Always-object list member nb_objects. | object ids or count | +0x4, 2 B |
| field:active_objects_t.objects | PROVEN | Active-object list member objects. | object ids or count | +0x0, 200 B |
| field:active_objects_t.num_active_objects | PROVEN | Active-object list member num_active_objects. | object ids or count | +0xc8, 2 B |
| field:obj_type_flags_t.raw | PROVEN | Overlapping raw/byte/nibble view raw of per-type behaviour flags. | bitset | +0x0, 4 B |
| field:obj_type_flags_t.flags0 | PROVEN | Overlapping raw/byte/nibble view flags0 of per-type behaviour flags. | bitset | +0x0, 1 B |
| field:obj_type_flags_t.flags1 | PROVEN | Overlapping raw/byte/nibble view flags1 of per-type behaviour flags. | bitset | +0x1, 1 B |
| field:obj_type_flags_t.flags1_lo | PROVEN | Overlapping raw/byte/nibble view flags1_lo of per-type behaviour flags. | bitset | +0x1, 1 B |
| field:obj_type_flags_t.move_x | PROVEN | Select fractional speed conversion for the corresponding object movement axis. | boolean | +0x1, 1 B |
| field:obj_type_flags_t.move_y | PROVEN | Select fractional speed conversion for the corresponding object movement axis. | boolean | +0x1, 1 B |
| field:obj_type_flags_t.flags1_hi | PROVEN | Overlapping raw/byte/nibble view flags1_hi of per-type behaviour flags. | bitset | +0x1, 1 B |
| field:obj_type_flags_t.flags2 | PROVEN | Overlapping raw/byte/nibble view flags2 of per-type behaviour flags. | bitset | +0x2, 1 B |
| field:obj_type_flags_t.flags3 | PROVEN | Overlapping raw/byte/nibble view flags3 of per-type behaviour flags. | bitset | +0x3, 1 B |
| field:map_data_t.width | PROVEN | Map dimension width in16-pixel tiles. | tiles | +0x0, 2 B |
| field:map_data_t.height | PROVEN | Map dimension height in16-pixel tiles. | tiles | +0x2, 2 B |
| field:map_data_t.length | STRONG | Number of tile records used by full-map processing. | tiles | +0x4, 4 B |
| field:map_data_t.map | PROVEN | Pointer to row-major six-byte tile records. | pointer | +0x8, 4 B |
| field:map_tile_t.texture_id | STRONG | Index of a visual16x16 tile texture. | texture index | +0x0, 2 B |
| field:map_tile_t.tile_type | PROVEN | Collision/behaviour type byte returned by BTYP. | block type id | +0x2, 1 B |
| field:map_tile_t.field_3 | UNKNOWN | Neutral map_tile_t.field_3 storage/view; its intended meaning is not established by this pilot. | unknown | +0x3, 1 B |
| field:map_tile_t.transparency | PROVEN | Computed texture coverage classification:0 opaque,1 empty,2 mixed. | classification | +0x4, 1 B |
| field:map_tile_t.field_5 | UNKNOWN | Neutral map_tile_t.field_5 storage/view; its intended meaning is not established by this pilot. | unknown | +0x5, 1 B |
| field:block_flags_t.raw | PROVEN | Byte/bitfield views of block-type behaviour flags. | bitset | +0x0, 1 B |
| field:block_flags_t.bits | PROVEN | Byte/bitfield views of block-type behaviour flags. | bitset | +0x0, 1 B |
| field:block_flags_t.bits.unused0 | UNKNOWN | Neutral block_flags_t.bits.unused0 storage/view; its intended meaning is not established by this pilot. | unknown | +0x0, 1 B |
| field:block_flags_t.bits.solid | PROVEN | Bit1 tested as the supporting/solid classification in block selection. | boolean | +0x0, 1 B |
| field:block_flags_t.bits.unused2 | UNKNOWN | Neutral block_flags_t.bits.unused2 storage/view; its intended meaning is not established by this pilot. | unknown | +0x0, 1 B |
| field:block_flags_t.bits.chdir | PROVEN | Bit5 of the probed block flags, returned by the turn-marker test. | boolean | +0x0, 1 B |
| field:block_flags_t.bits.unused1 | UNKNOWN | Neutral block_flags_t.bits.unused1 storage/view; its intended meaning is not established by this pilot. | unknown | +0x0, 1 B |
| field:rayevts_t.poing | STRONG | Enable the fist-throwing ability. | boolean | +0x0, 1 B |
| field:rayevts_t.hang | STRONG | Enable the ledge-hanging ability. | boolean | +0x0, 1 B |
| field:rayevts_t.helico | STRONG | Enable normal/enhanced helicopter response (helico). | boolean | +0x0, 1 B |
| field:rayevts_t.super_helico | STRONG | Enable normal/enhanced helicopter response (super_helico). | boolean | +0x0, 1 B |
| field:rayevts_t.handstand_dash | UNKNOWN | Neutral rayevts_t.handstand_dash storage/view; its intended meaning is not established by this pilot. | unknown | +0x0, 1 B |
| field:rayevts_t.handstand | UNKNOWN | Neutral rayevts_t.handstand storage/view; its intended meaning is not established by this pilot. | unknown | +0x0, 1 B |
| field:rayevts_t.magicseed | STRONG | Enable the level-specific magic-seed ability. | boolean | +0x0, 1 B |
| field:rayevts_t.grap | STRONG | Enable grabbing/swinging through the grappling interaction. | boolean | +0x0, 1 B |
| field:rayevts_t.run | STRONG | Enable Rayman running behaviour. | boolean | +0x1, 1 B |
| field:rayevts_t.tiny | STRONG | Rayman small-size mode/power bit. | boolean | +0x1, 1 B |
| field:rayevts_t.firefly | STRONG | Enable the firefly/light companion mode. | boolean | +0x1, 1 B |
| field:rayevts_t.force_run | PROVEN | Two-bit forced-running lifecycle state. | state code | +0x1, 1 B |
| field:rayevts_t.reverse | PROVEN | Two-bit reversed-controls lifecycle state whose nonzero values swap horizontal input directions. | state code | +0x1, 1 B |
| field:rayevts_t.squashed | STRONG | Reduced/squashed Rayman mode coupled to tiny and object/ability-state swapping. | boolean | +0x1, 1 B |
| field:finbosslevel_t.bzzit | STRONG | Progression flag for completion of the bzzit encounter. | boolean | +0x0, 1 B |
| field:finbosslevel_t.moskito | STRONG | Progression flag for completion of the moskito encounter. | boolean | +0x0, 1 B |
| field:finbosslevel_t.mr_sax | STRONG | Progression flag for completion of the mr_sax encounter. | boolean | +0x0, 1 B |
| field:finbosslevel_t.mr_stone | STRONG | Progression flag for completion of the mr_stone encounter. | boolean | +0x0, 1 B |
| field:finbosslevel_t.viking_mama | STRONG | Progression flag for completion of the viking_mama encounter. | boolean | +0x0, 1 B |
| field:finbosslevel_t.space_mama | STRONG | Progression flag for completion of the space_mama encounter. | boolean | +0x0, 1 B |
| field:finbosslevel_t.mr_skops | STRONG | Progression flag for completion of the mr_skops encounter. | boolean | +0x0, 1 B |
| field:finbosslevel_t.mr_dark | STRONG | Progression flag for completion of the mr_dark encounter. | boolean | +0x0, 1 B |
| field:finbosslevel_t.crazy_drummer | UNKNOWN | Neutral finbosslevel_t.crazy_drummer storage/view; its intended meaning is not established by this pilot. | unknown | +0x1, 1 B |
| field:finbosslevel_t.helped_joe_1 | STRONG | Progression flag for the corresponding help event (helped_joe_1). | boolean | +0x1, 1 B |
| field:finbosslevel_t.helped_joe_2 | STRONG | Progression flag for the corresponding help event (helped_joe_2). | boolean | +0x1, 1 B |
| field:finbosslevel_t.helped_musician | STRONG | Progression flag for the corresponding help event (helped_musician). | boolean | +0x1, 1 B |
| field:poing_t.y_16 | PROVEN | Fist vertical position accumulator scaled by16. | 1/16 pixel | +0x0, 4 B |
| field:poing_t.field_4 | UNKNOWN | Neutral poing_t.field_4 storage/view; its intended meaning is not established by this pilot. | unknown | +0x4, 2 B |
| field:poing_t.speed_x | PROVEN | Stored positive horizontal fist speed used to launch and constrain return speed. | pixels/tick | +0x6, 2 B |
| field:poing_t.charge | PROVEN | Fist charge counter before throwing, then scaled outbound budget decremented during flight. | charge ticks then scaled budget | +0x8, 2 B |
| field:poing_t.field_A | PROVEN | Temporary throw-range denominator then decrement applied to the scaled outbound charge budget. | context-dependent budget units | +0xa, 2 B |
| field:poing_t.sub_etat | STRONG | Saved/selected fist upgrade substate copied into fist objects and save data. | substate selector | +0xc, 1 B |
| field:poing_t.is_returning | PROVEN | Select the homing-return phase of fist flight. | boolean-like byte | +0xd, 1 B |
| field:poing_t.is_active | PROVEN | Enable fist update/collision processing. | boolean-like byte | +0xe, 1 B |
| field:poing_t.damage | PROVEN | Current fist damage applied by obj_hurt. | hit points | +0xf, 1 B |
| field:poing_t.is_charging | STRONG | Fist charge-preparation flag. | boolean-like byte | +0x10, 1 B |
| field:poing_t.is_boum | STRONG | Fist impact/explosion interaction state. | boolean-like byte | +0x11, 1 B |
| field:poing_t.field_12 | UNKNOWN | Neutral poing_t.field_12 storage/view; its intended meaning is not established by this pilot. | unknown | +0x12, 1 B |
| field:poing_t.field_13 | UNKNOWN | Neutral poing_t.field_13 storage/view; its intended meaning is not established by this pilot. | unknown | +0x13, 1 B |
| field:font_t.sprites | PROVEN | Base sprite table for the corresponding font bank. | pointer | +0x0, 4 B |
| field:font_t.img_buffer | STRONG | Font image buffer or sprite count (img_buffer) copied from loaded object descriptors. | pointer or count | +0x4, 4 B |
| field:font_t.nb_sprites | STRONG | Font image buffer or sprite count (nb_sprites) copied from loaded object descriptors. | pointer or count | +0x8, 2 B |
| field:display_item_t.text | PROVEN | Fixed200-byte text buffer interpreted by the game glyph decoder. | encoded bytes | +0x0, 200 B |
| field:display_item_t.centered_x_pos | PROVEN | Computed text-box rectangle component centered_x_pos. | screen pixels | +0xc8, 2 B |
| field:display_item_t.centered_y_pos | PROVEN | Computed text-box rectangle component centered_y_pos. | screen pixels | +0xca, 2 B |
| field:display_item_t.width | PROVEN | Computed text-box rectangle component width. | screen pixels | +0xcc, 2 B |
| field:display_item_t.height | PROVEN | Computed text-box rectangle component height. | screen pixels | +0xce, 2 B |
| field:display_item_t.xpos | PROVEN | Text anchor coordinate xpos. | screen pixels | +0xd0, 2 B |
| field:display_item_t.ypos | PROVEN | Text anchor coordinate ypos. | screen pixels | +0xd2, 2 B |
| field:display_item_t.font_size | PROVEN | Font selector controlling bank and line metrics. | font selector | +0xd4, 1 B |
| field:display_item_t.field_D5 | PROVEN | Byte selecting the palette255 box branch instead of the ordinary palette254/no-box branch. | boolean-like selector | +0xd5, 1 B |
| field:display_item_t.is_fond | PROVEN | Box drawing/position mode passed to DISPLAY_BLACKBOX and used to offset text by +/-1. | mode | +0xd6, 1 B |
| field:display_item_t.color | STRONG | Text palette/color selector passed to display_text. | palette selector | +0xd7, 1 B |
