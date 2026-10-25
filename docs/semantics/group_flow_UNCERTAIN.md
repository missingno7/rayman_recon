# Remaining uncertainty — game flow/UI

UNKNOWN records retain their current spellings. These are observed operations, not new named hypotheses. Earlier unrelated UNKNOWN entities remain in the canonical uncertainty lists.

Representation is measured independently of intended meaning. Empty hooks and unused parameters are documented without speculative purpose. No HYPOTHESIS is promoted into a name.

## cdsave

- `local:interrupt_handler_5ba70:registers:1` — Unused parameter;no value-consuming meaning established. Observed uses: 
- `local:interrupt_handler_5ba8c:registers:1` — Unused parameter;no value-consuming meaning established. Observed uses: 

## cheat

- `local:init_divers_level_PC:a1:1` — Observed parameter dataflow in init_divers_level_PC: *a1 = 255;. Intended role beyond these operations remains unassigned. Observed uses: *a1 = 255;
- `local:DEPART_LEVEL:i:3` — Unused local;no value-consuming meaning established. Observed uses: s32 i;
- `global:freeze` — Observed storage used by cheat. Read/write operations are retained; intended meaning beyond them is UNKNOWN. 

## input

- `global:word_E0CD0` — Signed16 storage written by sub_4212C;no live semantic reader established by prior whole-image audit. Neutral name retained. 

## jumelle

- `local:plot2linejumelle:v53:1` — Observed local dataflow in plot2linejumelle: s32 v53;; v53 = jumelle_y - 64;; s32 *zoom = JumelleZoomDef + (((448 * (v9 - v53)) / 2) + (v7 - v56));; s32 *zoom = JumelleZoomDef + (((448 * (v16 - v53)) / 2) + (v7 - v56));; s32 *zoom = JumelleZoomDef + (((448 * (v26 - v53)) / 2) + (v7 - v56));. Intended role beyond these operations remains unassigned. Observed uses: s32 v53;; v53 = jumelle_y - 64;; s32 *zoom = JumelleZoomDef + (((448 * (v9 - v53)) / 2) + (v7 - v56));
- `local:plot2linejumelle:v55:2` — Observed local dataflow in plot2linejumelle: s32 v55;; v55 = (SCREEN_WIDTH * JumelleYMin) + JumelleXMin;; u8 *v11 = ((effet_buf + (SCREEN_WIDTH * v9)) + v7) - v55;; u8 *v11 = ((effet_buf + (SCREEN_WIDTH * v16)) + v7) - v55;; s32 v28 = v27 - v55;. Intended role beyond these operations remains unassigned. Observed uses: s32 v55;; v55 = (SCREEN_WIDTH * JumelleYMin) + JumelleXMin;; u8 *v11 = ((effet_buf + (SCREEN_WIDTH * v9)) + v7) - v55;
- `local:plot2linejumelle:v56:3` — Observed local dataflow in plot2linejumelle: s32 v56;; v56 = jumelle_x - 112;; s32 *zoom = JumelleZoomDef + (((448 * (v9 - v53)) / 2) + (v7 - v56));; s32 *zoom = JumelleZoomDef + (((448 * (v16 - v53)) / 2) + (v7 - v56));; s32 *zoom = JumelleZoomDef + (((448 * (v26 - v53)) / 2) + (v7 - v56));. Intended role beyond these operations remains unassigned. Observed uses: s32 v56;; v56 = jumelle_x - 112;; s32 *zoom = JumelleZoomDef + (((448 * (v9 - v53)) / 2) + (v7 - v56));
- `local:plot2linejumelle:v7:4` — Observed local dataflow in plot2linejumelle: s32 v7;; v7 = (jumelle_x - plot_x) - circle2_offset;; if (v7 < 0); v58 += v7;; v7 = 0;. Intended role beyond these operations remains unassigned. Observed uses: s32 v7;; v7 = (jumelle_x - plot_x) - circle2_offset;; if (v7 < 0)
- `local:plot2linejumelle:v58:5` — Observed local dataflow in plot2linejumelle: s32 v58 = 2 * (plot_x + circle2_offset);; v58 += v7;; if ((v7 + v58) >= SCREEN_WIDTH); v58 = (SCREEN_WIDTH - v7) - 1;; if (v58 > 0). Intended role beyond these operations remains unassigned. Observed uses: s32 v58 = 2 * (plot_x + circle2_offset);; v58 += v7;; if ((v7 + v58) >= SCREEN_WIDTH)
- `local:plot2linejumelle:v9:6` — Observed local dataflow in plot2linejumelle: s32 v9 = jumelle_y - plot_y;; if ((v9 >= 0) && (v9 < SCREEN_HEIGHT)); u8 *v10 = (draw_buf + (SCREEN_WIDTH * v9)) + v7;; u8 *v11 = ((effet_buf + (SCREEN_WIDTH * v9)) + v7) - v55;; s32 *zoom = JumelleZoomDef + (((448 * (v9 - v53)) / 2) + (v7 - v56));. Intended role beyond these operations remains unassigned. Observed uses: s32 v9 = jumelle_y - plot_y;; if ((v9 >= 0) && (v9 < SCREEN_HEIGHT)); u8 *v10 = (draw_buf + (SCREEN_WIDTH * v9)) + v7;
- `local:plot2linejumelle:v16:7` — Observed local dataflow in plot2linejumelle: s32 v16;; v16 = jumelle_y + plot_y;; if ((v16 >= 0) && (v16 < SCREEN_HEIGHT)); u8 *v10 = (draw_buf + (SCREEN_WIDTH * v16)) + v7;; u8 *v11 = ((effet_buf + (SCREEN_WIDTH * v16)) + v7) - v55;. Intended role beyond these operations remains unassigned. Observed uses: s32 v16;; v16 = jumelle_y + plot_y;; if ((v16 >= 0) && (v16 < SCREEN_HEIGHT))
- `local:plot2linejumelle:v10:8` — Observed local dataflow in plot2linejumelle: u8 *v10 = (draw_buf + (SCREEN_WIDTH * v9)) + v7;; *v10 = v11[*zoom];; ++v10;; copy(v11, v10, v58);; u8 *v10 = (draw_buf + (SCREEN_WIDTH * v16)) + v7;. Intended role beyond these operations remains unassigned. Observed uses: u8 *v10 = (draw_buf + (SCREEN_WIDTH * v9)) + v7;; *v10 = v11[*zoom];; ++v10;
- `local:plot2linejumelle:v11:9` — Observed local dataflow in plot2linejumelle: u8 *v11 = ((effet_buf + (SCREEN_WIDTH * v9)) + v7) - v55;; *v10 = v11[*zoom];; ++v11;; copy(v11, v10, v58);; u8 *v11 = ((effet_buf + (SCREEN_WIDTH * v16)) + v7) - v55;. Intended role beyond these operations remains unassigned. Observed uses: u8 *v11 = ((effet_buf + (SCREEN_WIDTH * v9)) + v7) - v55;; *v10 = v11[*zoom];; ++v11;
- `local:plot2linejumelle:v10:12` — Observed local dataflow in plot2linejumelle: u8 *v10 = (draw_buf + (SCREEN_WIDTH * v9)) + v7;; *v10 = v11[*zoom];; ++v10;; copy(v11, v10, v58);; u8 *v10 = (draw_buf + (SCREEN_WIDTH * v16)) + v7;. Intended role beyond these operations remains unassigned. Observed uses: u8 *v10 = (draw_buf + (SCREEN_WIDTH * v9)) + v7;; *v10 = v11[*zoom];; ++v10;
- `local:plot2linejumelle:v11:13` — Observed local dataflow in plot2linejumelle: u8 *v11 = ((effet_buf + (SCREEN_WIDTH * v9)) + v7) - v55;; *v10 = v11[*zoom];; ++v11;; copy(v11, v10, v58);; u8 *v11 = ((effet_buf + (SCREEN_WIDTH * v16)) + v7) - v55;. Intended role beyond these operations remains unassigned. Observed uses: u8 *v11 = ((effet_buf + (SCREEN_WIDTH * v9)) + v7) - v55;; *v10 = v11[*zoom];; ++v11;
- `local:plot2linejumelle:v60:16` — Observed local dataflow in plot2linejumelle: s32 v60 = 2 * plot_x;; v60 += v7;; if ((v7 + v60) >= SCREEN_WIDTH); v60 = (SCREEN_WIDTH - v7) - 1;; if (v60 > 0). Intended role beyond these operations remains unassigned. Observed uses: s32 v60 = 2 * plot_x;; v60 += v7;; if ((v7 + v60) >= SCREEN_WIDTH)
- `local:plot2linejumelle:v25:17` — Observed local dataflow in plot2linejumelle: s32 v25;; v25 = (circle2_offset + jumelle_x) - plot_x;; v57 = v25;; if (v25 < 0); v59 += v25;. Intended role beyond these operations remains unassigned. Observed uses: s32 v25;; v25 = (circle2_offset + jumelle_x) - plot_x;; v57 = v25;
- `local:plot2linejumelle:v59:18` — Observed local dataflow in plot2linejumelle: s32 v59;; v59 = 2 * plot_x;; v59 += v25;; if ((v59 + v57) >= SCREEN_WIDTH); v59 = (SCREEN_WIDTH - v57) - 1;. Intended role beyond these operations remains unassigned. Observed uses: s32 v59;; v59 = 2 * plot_x;; v59 += v25;
- `local:plot2linejumelle:v57:19` — Observed local dataflow in plot2linejumelle: s32 v57;; v57 = v25;; v57 = 0;; if ((v59 + v57) >= SCREEN_WIDTH); v59 = (SCREEN_WIDTH - v57) - 1;. Intended role beyond these operations remains unassigned. Observed uses: s32 v57;; v57 = v25;; v57 = 0;
- `local:plot2linejumelle:v26:20` — Observed local dataflow in plot2linejumelle: s32 v26;; v26 = jumelle_y - plot_y;; v54 = v26;; if (((jumelle_y - plot_y) >= 0) && (v26 < SCREEN_HEIGHT)); s32 v27 = (SCREEN_WIDTH * v26) + v7;. Intended role beyond these operations remains unassigned. Observed uses: s32 v26;; v26 = jumelle_y - plot_y;; v54 = v26;
- `local:plot2linejumelle:v54:21` — Observed local dataflow in plot2linejumelle: s32 v54;; v54 = v26;; u8 *v35 = ((effet_buf + (SCREEN_WIDTH * v54)) + v57) - v55;; u8 *v36 = (draw_buf + (SCREEN_WIDTH * v54)) + v57;; s32 *zoom = JumelleZoomDef + (((448 * (v54 - v53)) / 2) + (v57 - v56));. Intended role beyond these operations remains unassigned. Observed uses: s32 v54;; v54 = v26;; u8 *v35 = ((effet_buf + (SCREEN_WIDTH * v54)) + v57) - v55;
- `local:plot2linejumelle:v8:22` — Observed local dataflow in plot2linejumelle: s32 v8;; v8 = plot_y + jumelle_y;; v52 = v8;; if ((v8 >= 0) && (v8 < SCREEN_HEIGHT)). Intended role beyond these operations remains unassigned. Observed uses: s32 v8;; v8 = plot_y + jumelle_y;; v52 = v8;
- `local:plot2linejumelle:v52:23` — Observed local dataflow in plot2linejumelle: s32 v52;; v52 = v8;; u8 *v41 = (draw_buf + (SCREEN_WIDTH * v52)) + v7;; u8 *v42 = ((effet_buf + (SCREEN_WIDTH * v52)) + v7) - v55;; s32 *zoom = JumelleZoomDef + (((448 * (v52 - v53)) / 2) + (v7 - v56));. Intended role beyond these operations remains unassigned. Observed uses: s32 v52;; v52 = v8;; u8 *v41 = (draw_buf + (SCREEN_WIDTH * v52)) + v7;
- `local:plot2linejumelle:v27:24` — Observed local dataflow in plot2linejumelle: s32 v27 = (SCREEN_WIDTH * v26) + v7;; s32 v28 = v27 - v55;; u8 *v29 = draw_buf + v27;. Intended role beyond these operations remains unassigned. Observed uses: s32 v27 = (SCREEN_WIDTH * v26) + v7;; s32 v28 = v27 - v55;; u8 *v29 = draw_buf + v27;
- `local:plot2linejumelle:v28:25` — Observed local dataflow in plot2linejumelle: s32 v28 = v27 - v55;; u8 *v30 = effet_buf + v28;. Intended role beyond these operations remains unassigned. Observed uses: s32 v28 = v27 - v55;; u8 *v30 = effet_buf + v28;
- `local:plot2linejumelle:v29:26` — Observed local dataflow in plot2linejumelle: u8 *v29 = draw_buf + v27;; *v29 = v30[*zoom];; ++v29;; copy(v30, v29, v60);. Intended role beyond these operations remains unassigned. Observed uses: u8 *v29 = draw_buf + v27;; *v29 = v30[*zoom];; ++v29;
- `local:plot2linejumelle:v30:27` — Observed local dataflow in plot2linejumelle: u8 *v30 = effet_buf + v28;; *v29 = v30[*zoom];; ++v30;; copy(v30, v29, v60);. Intended role beyond these operations remains unassigned. Observed uses: u8 *v30 = effet_buf + v28;; *v29 = v30[*zoom];; ++v30;
- `local:plot2linejumelle:v35:30` — Observed local dataflow in plot2linejumelle: u8 *v35 = ((effet_buf + (SCREEN_WIDTH * v54)) + v57) - v55;; *(v36++) = v35[*(zoom++)];; ++v35;; copy(v35, v36, v59);. Intended role beyond these operations remains unassigned. Observed uses: u8 *v35 = ((effet_buf + (SCREEN_WIDTH * v54)) + v57) - v55;; *(v36++) = v35[*(zoom++)];; ++v35;
- `local:plot2linejumelle:v36:31` — Observed local dataflow in plot2linejumelle: u8 *v36 = (draw_buf + (SCREEN_WIDTH * v54)) + v57;; *(v36++) = v35[*(zoom++)];; copy(v35, v36, v59);. Intended role beyond these operations remains unassigned. Observed uses: u8 *v36 = (draw_buf + (SCREEN_WIDTH * v54)) + v57;; *(v36++) = v35[*(zoom++)];; copy(v35, v36, v59);
- `local:plot2linejumelle:v41:34` — Observed local dataflow in plot2linejumelle: u8 *v41 = (draw_buf + (SCREEN_WIDTH * v52)) + v7;; *(v41++) = v42[*(zoom++)];; copy(v42, v41, v60);. Intended role beyond these operations remains unassigned. Observed uses: u8 *v41 = (draw_buf + (SCREEN_WIDTH * v52)) + v7;; *(v41++) = v42[*(zoom++)];; copy(v42, v41, v60);
- `local:plot2linejumelle:v42:35` — Observed local dataflow in plot2linejumelle: u8 *v42 = ((effet_buf + (SCREEN_WIDTH * v52)) + v7) - v55;; *(v41++) = v42[*(zoom++)];; ++v42;; copy(v42, v41, v60);. Intended role beyond these operations remains unassigned. Observed uses: u8 *v42 = ((effet_buf + (SCREEN_WIDTH * v52)) + v7) - v55;; *(v41++) = v42[*(zoom++)];; ++v42;
- `local:plot2linejumelle:v18:38` — Observed local dataflow in plot2linejumelle: u8 *v18 = (draw_buf + (SCREEN_WIDTH * v52)) + v57;; *v18 = v24[*zoom];; ++v18;; copy(v24, v18, v59);. Intended role beyond these operations remains unassigned. Observed uses: u8 *v18 = (draw_buf + (SCREEN_WIDTH * v52)) + v57;; *v18 = v24[*zoom];; ++v18;
- `local:plot2linejumelle:v24:39` — Observed local dataflow in plot2linejumelle: u8 *v24 = ((effet_buf + (SCREEN_WIDTH * v52)) + v57) - v55;; *v18 = v24[*zoom];; ++v24;; copy(v24, v18, v59);. Intended role beyond these operations remains unassigned. Observed uses: u8 *v24 = ((effet_buf + (SCREEN_WIDTH * v52)) + v57) - v55;; *v18 = v24[*zoom];; ++v24;
- `local:RecaleRayPosInJumelle:v13:1` — Observed local dataflow in RecaleRayPosInJumelle: s16 v13 = ((3 * HauteurJumelle) >> 2) - ray.offset_by;; s16 v14 = ray.screen_y - v13;; if (ray.screen_y < (v13 + 48)). Intended role beyond these operations remains unassigned. Observed uses: s16 v13 = ((3 * HauteurJumelle) >> 2) - ray.offset_by;; s16 v14 = ray.screen_y - v13;; if (ray.screen_y < (v13 + 48))
- `local:RecaleRayPosInJumelle:v14:2` — Observed local dataflow in RecaleRayPosInJumelle: s16 v14 = ray.screen_y - v13;; v_scroll_speed = ashr16(v14, 2);; if (v14 <= 0); if (v14 < 0). Intended role beyond these operations remains unassigned. Observed uses: s16 v14 = ray.screen_y - v13;; v_scroll_speed = ashr16(v14, 2);; if (v14 <= 0)
- `local:RecaleRayPosInJumelle:v16:3` — Observed local dataflow in RecaleRayPosInJumelle: s16 v16 = max32(3, Abs((s32) ray.speed_y));; v_scroll_speed = min32(v_scroll_speed, v16);; v_scroll_speed = max32(v_scroll_speed, -v16);. Intended role beyond these operations remains unassigned. Observed uses: s16 v16 = max32(3, Abs((s32) ray.speed_y));; v_scroll_speed = min32(v_scroll_speed, v16);; v_scroll_speed = max32(v_scroll_speed, -v16);

## mathutil

- `local:reflexion:unused:2` — Unused parameter;no value-consuming meaning established. Observed uses: 

## menu

- `local:INIT_AFFICHE_ECRAN_OPTIONS:v28:1` — Observed local dataflow in INIT_AFFICHE_ECRAN_OPTIONS: u8 v28[9] = {5, 5, 5, 5, 5, 5, 3, 0};; menu_to_display[i].color = v28[6];; menu_to_display[i].color = v28[i];. Intended role beyond these operations remains unassigned. Observed uses: u8 v28[9] = {5, 5, 5, 5, 5, 5, 3, 0};; menu_to_display[i].color = v28[6];; menu_to_display[i].color = v28[i];
- `local:INIT_AFFICHE_ECRAN_OPTIONS:v39:2` — Observed local dataflow in INIT_AFFICHE_ECRAN_OPTIONS: char v39[3] = "on";; text_width += calc_largmax_text(v39, -1, 8, 1, 2);. Intended role beyond these operations remains unassigned. Observed uses: char v39[3] = "on";; text_width += calc_largmax_text(v39, -1, 8, 1, 2);
- `local:INIT_AFFICHE_ECRAN_OPTIONS:v32:3` — Observed local dataflow in INIT_AFFICHE_ECRAN_OPTIONS: char v32[4] = "off";; text_width += calc_largmax_text(v32, -1, 8, 1, 2);. Intended role beyond these operations remains unassigned. Observed uses: char v32[4] = "off";; text_width += calc_largmax_text(v32, -1, 8, 1, 2);
- `local:AFFICHE_ECRAN_OPTIONS:v5:3` — Observed local dataflow in AFFICHE_ECRAN_OPTIONS: s16 v5;; v5 = calc_largmax_text(menu_to_display[option_index].text, -1, 8, 1, 2);; v6 = x + v5;; v5 = 5;; v5 = 3;. Intended role beyond these operations remains unassigned. Observed uses: s16 v5;; v5 = calc_largmax_text(menu_to_display[option_index].text, -1, 8, 1, 2);; v6 = x + v5;
- `local:AFFICHE_ECRAN_OPTIONS:v6:4` — Observed local dataflow in AFFICHE_ECRAN_OPTIONS: s16 v6;; v6 = x + v5;; draw_x = v6 + 16;; v18 = (v6 + calc_largmax_text(on_text, -1, 8, 1, 2)) + 32;; v8 = (v6 + calc_largmax_text(on_text, -1, 8, 1, 2)) + 32;. Intended role beyond these operations remains unassigned. Observed uses: s16 v6;; v6 = x + v5;; draw_x = v6 + 16;
- `local:AFFICHE_ECRAN_OPTIONS:v18:5` — Observed local dataflow in AFFICHE_ECRAN_OPTIONS: s16 v18;; v18 = (v6 + calc_largmax_text(on_text, -1, 8, 1, 2)) + 32;; v18 = draw_x;; display_text(nonselected_text, v18, y, 2, 5);. Intended role beyond these operations remains unassigned. Observed uses: s16 v18;; v18 = (v6 + calc_largmax_text(on_text, -1, 8, 1, 2)) + 32;; v18 = draw_x;
- `local:AFFICHE_ECRAN_OPTIONS:v8:6` — Observed local dataflow in AFFICHE_ECRAN_OPTIONS: s16 v8;; v8 = draw_x;; v8 = (v6 + calc_largmax_text(on_text, -1, 8, 1, 2)) + 32;; to_display->xpos = v8;; display_text(other_text, v8, y, 2, 1);. Intended role beyond these operations remains unassigned. Observed uses: s16 v8;; v8 = draw_x;; v8 = (v6 + calc_largmax_text(on_text, -1, 8, 1, 2)) + 32;
- `local:calibrate_joystick_prg:a1:2` — Unused parameter;no value-consuming meaning established. Observed uses: (void) a1;
- `local:center_joystick_prg:a1:2` — Unused parameter;no value-consuming meaning established. Observed uses: (void) a1;
- `local:menu_prg:a1:3` — Unused parameter;no value-consuming meaning established. Observed uses: (void) a1;
- `local:display_box_msg_prg:a1:1` — Unused parameter;no value-consuming meaning established. Observed uses: 
- `local:confirmation_msg:i:1` — Unused local;no value-consuming meaning established. Observed uses: s32 i;; s32 i;
- `local:confirmation_msg:i:2` — Unused local;no value-consuming meaning established. Observed uses: s32 i;; s32 i;
- `local:apparition_prg:a1:8` — Unused parameter;no value-consuming meaning established. Observed uses: 
- `local:fondu_prg:a1:7` — Unused parameter;no value-consuming meaning established. Observed uses: 
- `local:FonduPixel_prg:arg:7` — Unused parameter;no value-consuming meaning established. Observed uses: 
- `global:delai_stereo` — Observed storage used by menu. Read/write operations are retained; intended meaning beyond them is UNKNOWN. 
- `global:word_CF79E` — Observed storage used by menu. Read/write operations are retained; intended meaning beyond them is UNKNOWN. 

## shared delta

- `global:First_Hist` — Session initialization writes1; menu code tests for2 and clears0 in a later branch. Intended role and a live writer of2 remain unresolved. 
- `global:choix_menu` — Observed storage used by menu. Read/write operations are retained; intended meaning beyond them is UNKNOWN. 
- `global:is_fee` — Observed storage used by menu, pcmain. Read/write operations are retained; intended meaning beyond them is UNKNOWN. 
- `global:dark_phase` — Observed storage used by worldmap. Read/write operations are retained; intended meaning beyond them is UNKNOWN. 
- `global:positiony2` — Observed storage used by worldmap. Read/write operations are retained; intended meaning beyond them is UNKNOWN. 
- `global:DrawBufferNormalIni` — Observed storage used by cheat. Read/write operations are retained; intended meaning beyond them is UNKNOWN. 
- `global:VENT_X` — Observed storage used by save. Read/write operations are retained; intended meaning beyond them is UNKNOWN. 
- `global:VENT_Y` — Observed storage used by save. Read/write operations are retained; intended meaning beyond them is UNKNOWN. 
- `global:byte_CFA14` — Observed storage used by cdsave. Read/write operations are retained; intended meaning beyond them is UNKNOWN. 
- `global:dvspeed` — Signed16 vertical camera state slot cleared at engine initialization and on vertical camera clamp. 
- `global:yp0` — Observed storage used by pcmain. Read/write operations are retained; intended meaning beyond them is UNKNOWN. 
- `field:options_t.field_E` — Declared signed16 working-record storage;no value-consuming use established. 
- `field:options_t.field_10` — Declared signed16 working-record storage;no value-consuming use established. 
- `field:options_t.field_12` — Declared signed16 working-record storage;no value-consuming use established. 
- `field:world_info_save_t.unused` — High3 bits retained by store/retrieve field operations;no semantic consumer established. 
- `field:dpmi_memory_info_t.remaining_info` — Remaining11 DPMI output dwords;not consumed by this unit. Do not invent per-element names. 
- `field:obj_t.timer` — Multipurpose byte countdown/state scratch with common reset0 and actor-specific defaults. 
- `field:credit_t.cmd` — Credit control byte consumed by credits logic outside the display loop. 

## sysutils

- `local:init_memory:data:2` — Unused local;no value-consuming meaning established. Observed uses: u8 *data;; mem->data = (u8 *) malloc(capacity);; if (mem->data == 0)

## worldmap

- `local:DO_STAGE_NAMES:perm_value_18:18` — Observed local dataflow in DO_STAGE_NAMES: s32 perm_value_18;; perm_value_18 = PositionXStageNameWorld[i] - 2560;; speed = perm_value_18;. Intended role beyond these operations remains unassigned. Observed uses: s32 perm_value_18;; perm_value_18 = PositionXStageNameWorld[i] - 2560;; speed = perm_value_18;
- `local:DO_STAGE_NAMES:perm_value_22:19` — Observed local dataflow in DO_STAGE_NAMES: s32 perm_value_22;; perm_value_22 = myRand(64) + 32;; rand_speed = perm_value_22;. Intended role beyond these operations remains unassigned. Observed uses: s32 perm_value_22;; perm_value_22 = myRand(64) + 32;; rand_speed = perm_value_22;
- `local:DO_RAYMAN_IN_WLD_MAP:perm_value_0:7` — Observed local dataflow in DO_RAYMAN_IN_WLD_MAP: s16 perm_value_0;; perm_value_0 = t_world_info[num_world_choice].xpos - t_world_info[old_num_world].xpos;; diff_x = perm_value_0;. Intended role beyond these operations remains unassigned. Observed uses: s16 perm_value_0;; perm_value_0 = t_world_info[num_world_choice].xpos - t_world_info[old_num_world].xpos;; diff_x = perm_value_0;
- `local:DISPLAY_SYMBOLE:unused:5` — Unused parameter;no value-consuming meaning established. Observed uses: 
- `local:MAIN_CONTINUE_PRG:flag_set:1` — Unused local;no value-consuming meaning established. Observed uses: u8 flag_set;
- `local:DO_CREDITS:unk_1:4` — Observed local dataflow in DO_CREDITS: s16 unk_1;; unk_1 = 36;; unk_1 = 23;; unk_1 = 15;; unk_1 = 160;. Intended role beyond these operations remains unassigned. Observed uses: s16 unk_1;; unk_1 = 36;; unk_1 = 23;

## Boundaries and negative findings

- `word_E0CD0`: store wrapper and startup value 3, with no established live semantic reader. Keep neutral name.
- Reserved options words, world-info padding/high bits, checkpoint trailing byte and DPMI output tail have no invented intended names.
- World-map axis-aligned unassigned-local branches are outside the pinned 50-edge graph. This is not a contract for modified maps.
- A function-only typedef (`pc_swap_func_t`) has no object size; the pinned compiler rejects sizeof. Primitive aliases differ by TU: bool is one byte in menu/cheat and four in jumelle.
- Raw displacement/adjacent fixup candidates and identifier matches are retained as candidates, never promoted to typed consumers solely by matching an offset/name.
- Prior unresolved PMODE/W/platform boundaries and allocator max-live counts are retained in docs/port/PLATFORM_OPEN.md; no new host execution or gameplay observation was attempted.
