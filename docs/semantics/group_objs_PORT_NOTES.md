# Object behaviours and map: port notes

DOS coordinates are object-relative. State numbers below are actor-specific. See the per-function registries for exact predicates, operands, evidence and current STRICT/NEAR status.

## objupdate1

Treat the object script as bytecode, not animation data. Signed byte conversion is explicit for GO_X/GO_Y and GO_SPEED velocity operands; duration/test operands have their separate widths. GET_OBJ_CMD tests the old nb_cmd value before decrement, and zero handler result yields to the object updater. Preserve label scanning, temporary read-enable restoration, parity-only SETTEST, signed depth -1 and predecremented loop counts. The 34 three-pointer dispatch records are decoded in evidence/command_dispatch.json. Context capacity is not inferred.

## objupdate2

Keep wizard entry at ten Tings, clock-2 deductions and the saved palette/return flags. Fist upgrades preserve the two 1/3/5 and 8/10/12 families and update save1. Pickups, generators, checkpoint photographer, Joe, musician and exit signs each have actor-specific state rules in their function records. Preserve clock/frame spawn latches, generator fan speeds, the photographer timer wrap avoidance, slope-platform signed-byte tilt, and pending HUD reward settlement. Joe uses exact ticks100/101. Do not reinterpret overloaded hit_points/link/follow_id as universally health/id data.

## objupdate3

Pan-pirate type variants select signed horizontal speeds and two substate families separated by15. Pan spawn requires substate13/28, frame6 and NONZERO horloge[2], reuses the first inactive type186, sets alive/active and disables its script. Fist damage accepts only the explicitly listed substates; health-zero selects13/28. Rising death states14/29 force (0,-8).

## objupdate4

Preserve delayed rider-contact counters reloaded from follow_id, automatic jump offsets (-3 or knife -4), and jump_time12. Crumble link countdown, param lifetime, follow-enable flag and world1 blink/deactivation are distinct. Spring follow_id is an amplitude that is consumed/reset; first-bounce suppression differs from manual button release. Small and large springs have different velocity boosts. On/off and boss platforms explicitly detach Rayman; no single main/substate label applies across these platform types.

## special

Progression is a world/level/trigger decision table, including bonus return routes and final-node threshold102 cages; fix_numlevel always requests new_level. Allocation searches reuse inactive typed level slots and preserve first-match behavior and each caller's failure policy. Planting chooses the greater age of two type39 objects with first-on-tie behavior and a final-frame one-shot guard. Shipped data has exactly two such objects, both Jungle9. Particle positions/speeds are Q6, camera compensation is scroll*64, and initialized count is64-8*group_index. GetY samples five 101-pixel water sections with animation phase offsets; it is not a generic terrain height query.

## action

Target sentinel -32000 disables reaching speed if either coordinate is unset; testActionEnd uses both-unset plus nb_cmd0 as a separate completion path. Keep Q4 target gaps, Q14 factor division, max-axis normalization, signed word narrowing, blend denominator255 and selected-clock gating. Floor scan advances by16 through the ymapmax+200 guard. Normal boss camera x limits include bias8; other modes clamp to0..xmapmax.

## blocs

Collision tile size is16. Exact type15 wall probes differ from the general block solid bit. Preserve all slope formulas, signed shifts, temporary x shifts/restoration, tiny/crouch probe-height logic and floor-distance inequalities. Spring type9 bounces only for Rayman or jump-enabled types and nonnegative vertical speed. Support loss selects actor-specific states and gravity resets; Rayman vine/pan cases are separate. Keep the existing first-callback signed16 return declaration provisional; no ABI change is proposed.

## map

Normal textures pack full tiles in288-byte records and mixed tiles in544-byte records, including transparency masks and two-bit row flags. Atlas tile0 is special, and counting stops at the first all-zero tile. Preserve the source's exact boundary comparisons when classifying texture records. Visible rectangle coordinates include partial tiles; tile_index narrows to16bits and tile counts to8bits. Draw alignment displacement is4-(x&3), giving1..4. Camera/scroll clamping includes bias8, small-map behavior and two locked-scroll level exceptions. Odd-width scrolling-sprite allocation behavior is an observed DOS quirk, not a new valid-input guarantee.

## map_ngawe

DRAW_MAP uses a320-byte pitch,16-pixel cells, clipped perimeter and un-clipped interior; it draws only when both visible counts exceed1. Mode setup saves/biases/restores draw pointers and selects the existing background-copy ABI. Pirate rings store their owner id in param, consume owner config on successful allocation and restore ammunition only on completed return; link is a travel budget, hurtbyfist is the return-stage latch and command_test is consumed by scripts. Owner inactivity, fist guard states, frame26 firing latch and ship attachment/death-script interaction stay separate.

## poing

Charge begins at5 (air throw32), grows to63, then becomes a signed16 charge<<8 outbound budget. Upgrades select base speeds5/8/11, gold adds damage2, movement adjustment caps stored launch speed at16. Keep the separate Q4 y accumulator, +8 screen x bias, paired outbound/return substates, deceleration thresholds11/8, zero-speed reversal, crossing test and no-overshoot return steering. The collision globals ray_zdc_h/ray_zdc_y have existing reversed y-origin/height roles; use actual expressions. Carried-object release chooses initial position versus Rayman placement from force/alive state.

## bonus

Bonus time uses60 ticks/second, a120-tick opening allowance and -2 untimed sentinel. Timeout tests exact zero. Perfect completion is a192-bit world*32+level map, separate from current remaining Tings. Result screen awards the first-completion life at tick15, animates it after20, permits buttons only after50 and ends at EXACTLY500. The negative transition timer reaches -1 before setup; restore prior video mode, palette and saved Ting count. Post-bonus Ting transfer occurs one per clock4 zero phase.

## neige

Eight weather groups have capacities20,30,50,60,70,80,96,106 (512 total) and scales -64..160 by32. add_256_flocs deliberately sets251 visible particles. Particle coordinates are integer pre-projection pixels, separate from Q6 pixel sparks. Wind division by4 uses explicit clock dithering; vertical +4 is temporary, then VENT values are restored. Wrap through inverse-projected bounds and keep projection center(160,170). Sequence opcode/duration pairs advance at exact equality; preserve list header/cursor wrap and trigger substate mapping. No enum meaning is assigned to unused flocon tail bytes.

## luciole

Light follows a four-update target history, with signed unit acceleration and ordinary speed clamp +/-8. It tracks the active fist or Rayman sprite2; the source uses sprite origin plus (size>1), not half-size. Death switches to angular drift and shrinking radius. Actual screen x is y_luc=x_main_luc-xmap+8, screen y is x_luc=y_main_luc-ymap. Keep radius growth/oscillation near54, clear fringe max movement+6, render radius-4, clipping save/install/restore and320-byte pitch. Shared coordinate names are deferred for coordinated renaming.

## cage

Broken cages increment the total, persist their broken initial state, set lidol_to_allocate=5 and allocate a medallion. The value5 is a nonzero request latch: DO_OBJECTS at1:487eb..48805 passes7 to allocate_toons and clears the latch. The medallion selects32+old cage count and saturates the stored count at6, freezes camera/engine and advances its own clocks/sounds until38. Toons scan fourteen slots from first type3 and use the seven exact launch-speed pairs; a requested count is not a pool-capacity proof. Cage-grid activation and fragment motion are separate. Preserve walking/falling/water/ceiling toon reactions and the timed cage-grid spawn.
