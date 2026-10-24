# Portable specifications: original DOS assembly

**109 public entries + 9 private helpers; 118 EQUIVALENT_TESTED, 0 current mismatches.** 30, 208 executions; 1237/1239 intraprocedural basic blocks covered (116 entries at 100%).

This is a portable behavior specification and a finite differential execution certificate, not a universal formal proof or the reconstruction verifier's STRICT status. Original assembly remains unchanged. All outputs are confined to this worker directory. The source inventory originally listed 8 private helpers; a ninth unreferenced callable starts at 1:0309d after RET 1:0309c. No binding was added.

Reproduce from the repository root with installed Python, Capstone and Unicorn 2.1.4:

```text
python -B build/workers/asmspec/catalogue.py
python -B build/workers/asmspec/prove.py --count 256
python -B build/workers/asmspec/controls.py
python -B build/workers/asmspec/report.py
python -B build/workers/asmspec/validate.py
```

The main seed interval is 0x0A550000..0x0A5500FF; each result records its actual interval and exact script hashes. Longer focused reruns, when present, replace that entry's interval. Per-routine JSON keeps every detected historical mismatch, including harness failures; current mismatches must be empty. No per-seed trace is stored.

## Proof boundary and conventions

PROVEN labels direct DOS operations, record widths, and algebraic facts. STRONG labels caller-derived intent/domain. EQUIVALENT_TESTED labels finite independent-model execution. HYPOTHESIS is not used to prescribe behavior. OPEN questions are listed below. Function name provenance follows config/symbols.json; invented helper/field names are explicitly identified. Android symbols were checked read-only through android.load(), because its CLI writes outside this worker.

All integers in records are little endian. “width” is bytes/pixels unless otherwise stated. The portable image is 8-bit palette indices; no DAC expansion is implied. Test pointers are synthetic flat addresses: code 0x1000000, data 0x2000000, RAM 0x4000000, stack 0x8000000, VGA 0xA0000. BSS is zero-extended; PMW 1 fixups are relocated in emulator memory only. tools/image.py hash-checks decoded objects against the pinned GOG oracle.

Models compute drawing, clipping, rounding and device effects independently from x86 execution. Separate push/pop bookkeeping models the original ABI, including dead stack contents. Comparisons cover all 8 GPRs, the exact written byte-address set and every final byte value (stack and writable code included), CF for clip helpers, selected preserved flags/ES, and exact I/O sequences. They do not compare intermediate write-event order. ModeX writes are also projected onto four logical planes; latch-copy tests begin with independently randomized planes. BIOS and CPU service implementations are excluded and replaced by explicit input/output stubs.

Block coverage uses the DOS CFG, not a model CFG; direct calls fall through and private callees have their own certificates. Computed raster dispatches enumerate all bounded targets. Callback bodies reuse [the existing tile specification](../tilecode/SPEC.md) and its 71, 400-run corpus proof; wrapper tests execute 16 existing representative bodies without redoing that analysis.

## Findings and remaining questions

- PROVEN: Normal beveled borders write an extra shadow row/column; ModeX borders use inclusive coordinate deltas.
- PROVEN: DrawFondBoxNormal uses XOR bank-crossing arithmetic, not ordinary palette-bank saturation.
- PROVEN: Differential sprite clipping rounds to dwords, may touch a rounded fringe, and has asymmetric simultaneous-edge clipping; the exact behavior is retained.
- PROVEN: draw_sprite_deform prefills 0xA1 but its ascending path tests 0x81. The unvisited store at 1:049ed remains a coverage/reachability question.
- PROVEN: asm_02850 mutates saved EBX internally but 14*1280 equals the 0x4600 circular plane size, so valid calls return the original EBX. Its pragma is correct for that domain.
- PROVEN: asm_02850's seam-reversal block 1:02920 cannot occur in a valid forward 20-tile row: floor((column+4*i)/80) crosses the seam at most once. See controls.json arithmetic check.
- OPEN: Actual PMODE/W BIOS INT 15h pointer translation/completion; the isolated wrapper and exact abstract request are tested.
- STRONG: Source caller structs explain raster, deformation and row-run fields. Names of reconstructed helper fields remain INVENTED where no original symbol exists.

## Entry index

| Entry | Address | C sites* | Cases | Blocks | Status |
|---|---|---:|---:|---:|---|
| [effect_reset_geometry](#effect_reset_geometry) | 1:00010 | 4 | 256 | 5/5 | EQUIVALENT_TESTED |
| [draw_effect_segment](#draw_effect_segment) | 1:00045 | 5 | 256 | 184/184 | EQUIVALENT_TESTED |
| [asm_02640](#asm_02640) | 1:02640 | 0 | 256 | 1/1 | EQUIVALENT_TESTED |
| [sub_1268A](#sub_1268a) | 1:0268a | 4 | 256 | 9/9 | EQUIVALENT_TESTED |
| [copy_dwords](#copy_dwords) | 1:02737 | 1 | 256 | 1/1 | EQUIVALENT_TESTED |
| [bios_wait_timer](#bios_wait_timer) | 1:0273a | 1 | 256 | 1/1 | EQUIVALENT_TESTED |
| [sub_2748](#sub_2748) | 1:02748 | 5 | 256 | 1/1 | EQUIVALENT_TESTED |
| [asm_0275c](#asm_0275c) | 1:0275c | 0 | 256 | 3/3 | EQUIVALENT_TESTED |
| [display_emptypicture](#display_emptypicture) | 1:02783 | 3 | 256 | 1/1 | EQUIVALENT_TESTED |
| [display_anypicture](#display_anypicture) | 1:027a2 | 1 | 256 | 7/7 | EQUIVALENT_TESTED |
| [asm_0283c](#asm_0283c) | 1:0283c | 1 | 256 | 1/1 | EQUIVALENT_TESTED |
| [asm_02850](#asm_02850) | 1:02850 | 1 | 256 | 26/27 | EQUIVALENT_TESTED |
| [asm_029d3](#asm_029d3) | 1:029d3 | 1 | 256 | 12/12 | EQUIVALENT_TESTED |
| [asm_02a73](#asm_02a73) | 1:02a73 | 1 | 256 | 12/12 | EQUIVALENT_TESTED |
| [asm_02b30](#asm_02b30) | 1:02b30 | 0 | 256 | 3/3 | EQUIVALENT_TESTED |
| [mode_x_effect_2b3d](#mode_x_effect_2b3d) | 1:02b3d | 0 | 256 | 6/6 | EQUIVALENT_TESTED |
| [fplot_mode_x_2b86](#fplot_mode_x_2b86) | 1:02b86 | 1 | 256 | 1/1 | EQUIVALENT_TESTED |
| [mode_x_effect_2bb7](#mode_x_effect_2bb7) | 1:02bb7 | 0 | 256 | 6/6 | EQUIVALENT_TESTED |
| [mode_x_effect_2c00](#mode_x_effect_2c00) | 1:02c00 | 0 | 256 | 6/6 | EQUIVALENT_TESTED |
| [mode_x_effect_2c59](#mode_x_effect_2c59) | 1:02c59 | 0 | 256 | 6/6 | EQUIVALENT_TESTED |
| [mode_x_effect_2cbf](#mode_x_effect_2cbf) | 1:02cbf | 0 | 256 | 6/6 | EQUIVALENT_TESTED |
| [mode_x_effect_2d08](#mode_x_effect_2d08) | 1:02d08 | 0 | 256 | 6/6 | EQUIVALENT_TESTED |
| [mode_x_effect_2da9](#mode_x_effect_2da9) | 1:02da9 | 0 | 256 | 6/6 | EQUIVALENT_TESTED |
| [mode_x_effect_2dfb](#mode_x_effect_2dfb) | 1:02dfb | 1 | 256 | 6/6 | EQUIVALENT_TESTED |
| [mode_x_effect_2e9c](#mode_x_effect_2e9c) | 1:02e9c | 0 | 256 | 6/6 | EQUIVALENT_TESTED |
| [mode_x_effect_2f0c](#mode_x_effect_2f0c) | 1:02f0c | 1 | 256 | 6/6 | EQUIVALENT_TESTED |
| [mode_x_effect_300f](#mode_x_effect_300f) | 1:0300f | 1 | 256 | 6/6 | EQUIVALENT_TESTED |
| [asm_0309d](#asm_0309d) | 1:0309d | 0 | 256 | 1/1 | EQUIVALENT_TESTED |
| [draw_blackbox](#draw_blackbox) | 1:03125 | 1 | 256 | 20/20 | EQUIVALENT_TESTED |
| [draw_borderbox](#draw_borderbox) | 1:031ed | 2 | 256 | 7/7 | EQUIVALENT_TESTED |
| [asm_03257](#asm_03257) | 1:03257 | 1 | 256 | 17/17 | EQUIVALENT_TESTED |
| [asm_037e4](#asm_037e4) | 1:037e4 | 1 | 256 | 11/11 | EQUIVALENT_TESTED |
| [asm_03912](#asm_03912) | 1:03912 | 1 | 256 | 17/17 | EQUIVALENT_TESTED |
| [asm_03ea1](#asm_03ea1) | 1:03ea1 | 1 | 256 | 17/17 | EQUIVALENT_TESTED |
| [asm_04430](#asm_04430) | 1:04430 | 1 | 256 | 11/11 | EQUIVALENT_TESTED |
| [asm_04493](#asm_04493) | 1:04493 | 1 | 256 | 12/12 | EQUIVALENT_TESTED |
| [asm_04504](#asm_04504) | 1:04504 | 1 | 256 | 3/3 | EQUIVALENT_TESTED |
| [asm_0456e](#asm_0456e) | 1:0456e | 1 | 256 | 3/3 | EQUIVALENT_TESTED |
| [select_display_buffer](#select_display_buffer) | 1:045f1 | 3 | 256 | 1/1 | EQUIVALENT_TESTED |
| [InitFirstModeVideo](#initfirstmodevideo) | 1:045fc | 2 | 256 | 1/1 | EQUIVALENT_TESTED |
| [InitModeNormal](#initmodenormal) | 1:0465f | 1 | 256 | 1/1 | EQUIVALENT_TESTED |
| [InitModeX](#initmodex) | 1:046b8 | 1 | 256 | 1/1 | EQUIVALENT_TESTED |
| [block_clear](#block_clear) | 1:0471d | 1 | 256 | 1/1 | EQUIVALENT_TESTED |
| [block_copy](#block_copy) | 1:04765 | 3 | 256 | 1/1 | EQUIVALENT_TESTED |
| [textmode](#textmode) | 1:047e7 | 1 | 256 | 1/1 | EQUIVALENT_TESTED |
| [draw_sprite_deform](#draw_sprite_deform) | 1:04828 | 1 | 256 | 29/30 | EQUIVALENT_TESTED |
| [local_04a83](#local_04a83) | 1:04a83 | 0 | 256 | 5/5 | EQUIVALENT_TESTED |
| [DisplayBufferModeNormal](#displaybuffermodenormal) | 1:04ab7 | 2 | 256 | 1/1 | EQUIVALENT_TESTED |
| [DrawWldPointPlan2Normal](#drawwldpointplan2normal) | 1:04ac1 | 1 | 256 | 1/1 | EQUIVALENT_TESTED |
| [DrawBlackBoxNormal](#drawblackboxnormal) | 1:04aef | 3 | 256 | 5/5 | EQUIVALENT_TESTED |
| [DrawFondBoxNormal](#drawfondboxnormal) | 1:04b2c | 1 | 256 | 15/15 | EQUIVALENT_TESTED |
| [DrawBorderBoxNormal](#drawborderboxnormal) | 1:04bbb | 6 | 256 | 5/5 | EQUIVALENT_TESTED |
| [DisplayAnyPictureNormal](#displayanypicturenormal) | 1:04c19 | 3 | 256 | 7/7 | EQUIVALENT_TESTED |
| [ClearDrawAndDisplayBufferNormal](#cleardrawanddisplaybuffernormal) | 1:04c6b | 1 | 256 | 1/1 | EQUIVALENT_TESTED |
| [N_CLRSCR](#n_clrscr) | 1:04c8e | 4 | 256 | 1/1 | EQUIVALENT_TESTED |
| [Swap_To_Screen](#swap_to_screen) | 1:04c9c | 2 | 256 | 3/3 | EQUIVALENT_TESTED |
| [clear_borders_Normal](#clear_borders_normal) | 1:04cc7 | 1 | 256 | 3/3 | EQUIVALENT_TESTED |
| [Copy_Plan0Diff_To_Buf](#copy_plan0diff_to_buf) | 1:04cfa | 3 | 256 | 9/9 | EQUIVALENT_TESTED |
| [asm_04d55](#asm_04d55) | 1:04d55 | 0 | 256 | 9/9 | EQUIVALENT_TESTED |
| [Display_Bloc](#display_bloc) | 1:04e64 | 1 | 256 | 20/20 | EQUIVALENT_TESTED |
| [Display_Bloc_Plein](#display_bloc_plein) | 1:05041 | 1 | 256 | 1/1 | EQUIVALENT_TESTED |
| [loc_052DA](#loc_052da) | 1:052da | 0 | 256 | 19/19 | EQUIVALENT_TESTED |
| [Display_Bloc_Clippe](#display_bloc_clippe) | 1:053c6 | 4 | 256 | 29/29 | EQUIVALENT_TESTED |
| [Display_Bloc_Plein_Clippe](#display_bloc_plein_clippe) | 1:0555e | 4 | 256 | 13/13 | EQUIVALENT_TESTED |
| [fplot_Normal](#fplot_normal) | 1:05609 | 1 | 256 | 1/1 | EQUIVALENT_TESTED |
| [flocon_clipping](#flocon_clipping) | 1:05628 | 1 | 256 | 1/1 | EQUIVALENT_TESTED |
| [draw_flocon5_Normal](#draw_flocon5_normal) | 1:05640 | 0 | 256 | 6/6 | EQUIVALENT_TESTED |
| [draw_flocon1_Normal](#draw_flocon1_normal) | 1:05670 | 0 | 256 | 6/6 | EQUIVALENT_TESTED |
| [draw_flocon2_Normal](#draw_flocon2_normal) | 1:056a0 | 0 | 256 | 6/6 | EQUIVALENT_TESTED |
| [draw_flocon3_Normal](#draw_flocon3_normal) | 1:056dc | 0 | 256 | 6/6 | EQUIVALENT_TESTED |
| [draw_flocon4_Normal](#draw_flocon4_normal) | 1:05725 | 0 | 256 | 6/6 | EQUIVALENT_TESTED |
| [draw_normal_5755](#draw_normal_5755) | 1:05755 | 0 | 256 | 6/6 | EQUIVALENT_TESTED |
| [draw_pluie5_Normal](#draw_pluie5_normal) | 1:057a6 | 0 | 256 | 6/6 | EQUIVALENT_TESTED |
| [draw_flocon7_Normal](#draw_flocon7_normal) | 1:057df | 1 | 256 | 6/6 | EQUIVALENT_TESTED |
| [draw_pluie6_Normal](#draw_pluie6_normal) | 1:05830 | 0 | 256 | 6/6 | EQUIVALENT_TESTED |
| [draw_flocon6_Normal](#draw_flocon6_normal) | 1:0587d | 1 | 256 | 6/6 | EQUIVALENT_TESTED |
| [draw_pluie7_Normal](#draw_pluie7_normal) | 1:058f4 | 1 | 256 | 6/6 | EQUIVALENT_TESTED |
| [get_joy_input2_dos1](#get_joy_input2_dos1) | 1:0595d | 3 | 256 | 12/12 | EQUIVALENT_TESTED |
| [get_joy_input2_dos2](#get_joy_input2_dos2) | 1:05a57 | 3 | 256 | 13/13 | EQUIVALENT_TESTED |
| [get_joy_input2_windows1](#get_joy_input2_windows1) | 1:05b55 | 3 | 256 | 11/11 | EQUIVALENT_TESTED |
| [get_joy_input2_windows2](#get_joy_input2_windows2) | 1:05c13 | 3 | 256 | 11/11 | EQUIVALENT_TESTED |
| [dos_game_io_port_something](#dos_game_io_port_something) | 1:05cdf | 2 | 256 | 7/7 | EQUIVALENT_TESTED |
| [dos_game_io_port_something2](#dos_game_io_port_something2) | 1:05d14 | 1 | 256 | 7/7 | EQUIVALENT_TESTED |
| [get_joy_input1_dos1](#get_joy_input1_dos1) | 1:05d4a | 3 | 256 | 21/21 | EQUIVALENT_TESTED |
| [get_joy_input1_dos2](#get_joy_input1_dos2) | 1:05f34 | 3 | 256 | 21/21 | EQUIVALENT_TESTED |
| [DetectCpu](#detectcpu) | 1:0611e | 1 | 256 | 6/6 | EQUIVALENT_TESTED |
| [default_sprite_clipping](#default_sprite_clipping) | 1:06194 | 3 | 256 | 1/1 | EQUIVALENT_TESTED |
| [get_sprite_clipping](#get_sprite_clipping) | 1:061bd | 5 | 256 | 1/1 | EQUIVALENT_TESTED |
| [get_sprite_clipping_xmin](#get_sprite_clipping_xmin) | 1:061e2 | 0 | 256 | 1/1 | EQUIVALENT_TESTED |
| [get_sprite_clipping_xmax](#get_sprite_clipping_xmax) | 1:061e8 | 0 | 256 | 1/1 | EQUIVALENT_TESTED |
| [get_sprite_clipping_ymin](#get_sprite_clipping_ymin) | 1:061ee | 0 | 256 | 1/1 | EQUIVALENT_TESTED |
| [get_sprite_clipping_ymax](#get_sprite_clipping_ymax) | 1:061f4 | 0 | 256 | 1/1 | EQUIVALENT_TESTED |
| [sprite_clipping](#sprite_clipping) | 1:061fa | 22 | 256 | 17/17 | EQUIVALENT_TESTED |
| [clip_sprite_on_screen](#clip_sprite_on_screen) | 1:0626d | 0 | 256 | 14/14 | EQUIVALENT_TESTED |
| [clip_sprite_on_screen_flipped](#clip_sprite_on_screen_flipped) | 1:06323 | 0 | 256 | 14/14 | EQUIVALENT_TESTED |
| [DrawSpriteFlipNoClipX](#drawspriteflipnoclipx) | 1:063e6 | 0 | 256 | 39/39 | EQUIVALENT_TESTED |
| [DrawSpriteX](#drawspritex) | 1:063f2 | 3 | 256 | 40/40 | EQUIVALENT_TESTED |
| [DrawSpriteNoClipX](#drawspritenoclipx) | 1:06559 | 0 | 256 | 39/39 | EQUIVALENT_TESTED |
| [DrawSpriteFlipX](#drawspriteflipx) | 1:06565 | 0 | 256 | 40/40 | EQUIVALENT_TESTED |
| [DrawSpriteColorX](#drawspritecolorx) | 1:066df | 8 | 256 | 40/40 | EQUIVALENT_TESTED |
| [DrawSpriteColorFlipX](#drawspritecolorflipx) | 1:06876 | 0 | 256 | 40/40 | EQUIVALENT_TESTED |
| [DrawSpriteNormalNoClip](#drawspritenormalnoclip) | 1:06a18 | 0 | 256 | 10/10 | EQUIVALENT_TESTED |
| [DrawSpriteNormal](#drawspritenormal) | 1:06a24 | 3 | 256 | 12/12 | EQUIVALENT_TESTED |
| [DrawSpriteNormal256](#drawspritenormal256) | 1:06a9d | 3 | 256 | 10/10 | EQUIVALENT_TESTED |
| [DrawSpriteFlipNormalNoClip](#drawspriteflipnormalnoclip) | 1:06afc | 0 | 256 | 10/10 | EQUIVALENT_TESTED |
| [DrawSpriteFlipNormal](#drawspriteflipnormal) | 1:06b08 | 0 | 256 | 12/12 | EQUIVALENT_TESTED |
| [DrawSpriteFlipNormal256](#drawspriteflipnormal256) | 1:06b88 | 0 | 256 | 10/10 | EQUIVALENT_TESTED |
| [DrawSpriteColorNormal](#drawspritecolornormal) | 1:06bea | 8 | 256 | 12/12 | EQUIVALENT_TESTED |
| [DrawSpriteColorFlipNormal](#drawspritecolorflipnormal) | 1:06c89 | 0 | 256 | 12/12 | EQUIVALENT_TESTED |
| [asm_06d33](#asm_06d33) | 1:06d33 | 0 | 256 | 18/18 | EQUIVALENT_TESTED |
| [DrawSpriteDiffNormal](#drawspritediffnormal) | 1:06e0d | 8 | 256 | 10/10 | EQUIVALENT_TESTED |
| [synchro](#synchro) | 1:06e8a | 38 | 256 | 3/3 | EQUIVALENT_TESTED |
| [endsynchro](#endsynchro) | 1:06e9a | 36 | 256 | 3/3 | EQUIVALENT_TESTED |
| [set_speaker_on](#set_speaker_on) | 1:06eaa | 1 | 256 | 1/1 | EQUIVALENT_TESTED |
| [set_speaker_off](#set_speaker_off) | 1:06eea | 1 | 256 | 1/1 | EQUIVALENT_TESTED |
| [clock_ticks](#clock_ticks) | 1:06efd | 20 | 256 | 6/6 | EQUIVALENT_TESTED |
| [wait_until](#wait_until) | 1:06f5d | 7 | 256 | 4/4 | EQUIVALENT_TESTED |
| [speaker_enable](#speaker_enable) | 1:06f78 | 6 | 256 | 1/1 | EQUIVALENT_TESTED |

*Syntactic direct calls plus resolved dispatch-variable calls; address references are retained separately in JSON. This is an auditable lower bound, not runtime frequency or a complete points-to proof.

## effect_reset_geometry

`1:00010`; public; name **INVENTED**. Source: [src/lowlevel_00010.asm](../../../src/lowlevel_00010.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** No arguments. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:109:effect_reset_geometry](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux effect_reset_geometry modify exact [] /* src/effect6ec80.c:49 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: raster_rows; 80 writable code immediates.

**Layouts.**

raster_row (INVENTED) bytes; stride 32.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 4 | no | empty left extent biased by 0x8000 |
| 0x4 | 4 | no | empty right extent biased by 0x8000 |

**Computation.** Initialize all 200 raster rows: u32 +0=65535, +4=0 at stride 32; leave +8..31 unchanged. Restore 80 unrolled span-dispatch immediates at code offsets 0x815+96*i to signed values -320+4*i. Preserve GPRs.

**Valid test domain.** Raster rows allocated for 200 records; code writable.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 5/5 (100.0%). Current mismatches: 0. [Per-routine certificate](results/effect_reset_geometry.json).

**Caller evidence.** src/effect 2.c:120, src/effect 50350.c:28, src/effect 6ec80.c:70, src/effecthelp.c:86. Full lists and address registrations are in JSON.

## draw_effect_segment

`1:00045`; public; name **INVENTED**. Source: [src/lowlevel_00010.asm](../../../src/lowlevel_00010.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX destination, ESI effect_item_t+0x3c. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [raster.py:36:draw](raster.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux draw_effect_segment parm [eax] [esi] modify exact [] /* src/effect2.c:56 */
#pragma aux draw_effect_segment parm [eax] [esi] modify exact [] /* src/effect50350.c:14 */
#pragma aux draw_effect_segment parm [eax] [esi] modify exact [] /* src/effect6ec80.c:51 */
#pragma aux draw_effect_segment parm [eax] [esi] modify exact [] /* src/effecthelp.c:64 */
```

**Globals.** Reads: raster_rows; 80 reset span-store immediates. Writes: all raster_* globals; raster_rows bounds/UV; 24 edge interpolation immediate sites.

**Layouts.**

effect_item_t subrecord at+0x3c; src/effect 2.c (source names INVENTED) — size 44.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 4 | no | screen: pointer to 256-byte-pitch texture plane |
| 0x4 | 32 | yes | vertices[8]:four (x, y) signed 32 projected pairs |
| 0x24 | 8 | no | rectangle[8]:four (u, v) byte pairs; u0=u1, u2=u3, v0=v3, v1=v2 |

raster row (INVENTED field names) — size 32.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 4 | no | left x +32000; 65535 is empty; edge/consume writes low 16 only, upper 16 initialized zero |
| 0x4 | 4 | no | right x +32000; 0 is empty; edge/consume writes low 16 only, upper 16 initialized zero |
| 0x8 | 4 | no | left texture u; edge writes low 8 only, upper 24 initialized zero |
| 0xc | 4 | no | right texture u; edge writes low 8 only, upper 24 initialized zero |
| 0x10 | 4 | no | left texture v; edge writes low 8 only, upper 24 initialized zero |
| 0x14 | 4 | no | right texture v; edge writes low 8 only, upper 24 initialized zero |

**Computation.** Rasterize a textured projected quadrilateral. Edge order(0, 3), (3, 2), (2, 1), (1, 0); include both endpoint scanlines. Each edge starts at its lower-y endpoint and uses count=abs(dy)+1. X slope Q16=trunc((dx+sign_nonzero(dx))*65536/count), with +1 for dx>=0 and-1 otherwise. Varying texture slope Q16=trunc(deltaTex*65536/count); other coordinate is constant. Clip y to 0..199; retain strict min/max biased 16-bit x per row (ties retain previous UV). Consume and reset row bounds; clip x to 0..319. Horizontal slopes use endpoint differences PLUS 1, divided by original inclusive span width. Sample texture at u+256*v without transparency. The two coupled modular accumulators and the original fast-forward for left clipping are defined in raster.py; all carries, rounding, writes to raster scratch and self-modified immediates are part of this contract. Preserve all GPRs.

**Valid test domain.** Convex projected four-corner effect geometry in x/y range roughly-500..800; rectangle UV corresponds to an allocated 256-byte-pitch half-screen texture (u0..159, v0..199). effect_reset_geometry must have initialized row bounds and 80 span-store displacements. Source callers:effect 2.c and other effect units pass effect_item_t+0x3c. Nondegenerate positive scanline spans; no divide overflow.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 184/184 (100.0%). Current mismatches: 0. [Per-routine certificate](results/draw_effect_segment.json).

**Caller evidence.** src/effect 2.c:311, src/effect 50350.c:262, src/effect 50350.c:264, src/effect 6ec80.c:241, src/effecthelp.c:395. Full lists and address registrations are in JSON.

## asm_02640

`1:02640`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EBP raster row byte offset, ECX positive span width. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [raster.py:26:setup](raster.py). Preserved throughout the tested domain: EBP, ECX, EDI. ESP returns as specified above.

**Globals.** Reads: raster_rows. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

raster row (INVENTED field names) — size 32.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 4 | no | left x +32000; 65535 is empty; edge/consume writes low 16 only, upper 16 initialized zero |
| 0x4 | 4 | no | right x +32000; 0 is empty; edge/consume writes low 16 only, upper 16 initialized zero |
| 0x8 | 4 | no | left texture u; edge writes low 8 only, upper 24 initialized zero |
| 0xc | 4 | no | right texture u; edge writes low 8 only, upper 24 initialized zero |
| 0x10 | 4 | no | left texture v; edge writes low 8 only, upper 24 initialized zero |
| 0x14 | 4 | no | right texture v; edge writes low 8 only, upper 24 initialized zero |

**Computation.** Read left/right u/v dwords at raster_rows+EBP offsets 8, 12, 16, 20 and positive ECX span width. qU=trunc((uRight-uLeft+1)*65536/width), qV likewise. Return EAX=(frac 16(qV)<<16)|int 16(qU), EDX=(frac 16(qU)<<16)|(int 8(qV)&255), EBX=(vLeft<<8)|uLeft, ESI=0, ECX=width. Arithmetic is signed 64 division toward zero. EBP/EDI preserved.

**Valid test domain.** Row offset multiple 32 within 200 rows; byte-valued UV dwords; positive span width<=640 and representable quotients.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/asm_02640.json).

## sub_1268A

`1:0268a`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** ESI source, EBX source row pitch, EDX source plane pitch. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [hardware.py:29:upload](hardware.py). Preserved throughout the tested domain: EAX, EBP, EBX, EDX. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux sub_1268A parm [esi] [ebx] [edx] /* src/menu.c:294 */
#pragma aux sub_1268A parm [esi] [ebx] [edx] modify exact [ebx ecx esi edi] /* src/pcmain.c:427 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Upload four 80x200 planes to aperture 0xAB000; source row pitch EBX, source plane pitch EDX. OUT 0x3C4, word((mask<<8)|2) for masks 1, 2, 4, 8 at 1:026a0/026ca/026f4/0271d. Clear DF. This uses 16-bit PUSHA/POPA: only low 16 of GPRs restored; high halves of ECX, ESI, EDI reflect final transfer.

**Valid test domain.** Source row pitch>=80; four mapped 200-row planes; source plane pitch>=rowpitch*200.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:026a0 out dx, ax`
- `1:026ca out dx, ax`
- `1:026f4 out dx, ax`
- `1:0271d out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 9/9 (100.0%). Current mismatches: 0. [Per-routine certificate](results/sub_1268A.json).

**Caller evidence.** src/menu.c:770, src/pcmain.c:714, src/pcmain.c:731, src/pcmain.c:1108. Full lists and address registrations are in JSON.

## copy_dwords

`1:02737`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** ESI source, EDI destination, ECX dword count; incoming DF. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:126:copy_dwords](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, EDX. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux copy_dwords_t parm [esi] [edi] [ecx] modify exact [ecx esi edi] /* src/frame.c:68 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Sequentially copy ECX four-byte elements; DF selects +/-4 pointer movement. Consume ECX to zero, advance ESI/EDI; all other GPRs preserved.

**Valid test domain.** Count 0..256 in proof; pointers cover entire traversed intervals, DF=0 or 1.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/copy_dwords.json).

**Caller evidence.** src/frame.c:311. Full lists and address registrations are in JSON.

## bios_wait_timer

`1:0273a`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EBX flag pointer, CX:DX interval; ES saved/zeroed/restored. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [hardware.py:114:bios_wait](hardware.py). Preserved throughout the tested domain: EAX. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux bios_wait_timer_t parm [ecx] [edx] [ebx] /* src/frame.c:65 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Save EAX/ES, set ES=0, AX low 16=0x8300 and issue BIOS INT 0x15 at 1:02743, with EBX flag pointer and CX:DX delay. Restore EAX/ES; other register outputs depend on BIOS/extender response and are modeled as abstract inputs. The protected-mode ES=0, EBX pointer translation remains OPEN; no assertion about PMODE/W BIOS completion.

**Valid test domain.** C caller uses ECX=7, EDX=0xA120 (500000us); EBX mapped flag pointer. BIOS stub only; not proof of actual BIOS/extender semantics.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:0273b push es`
- `1:0273f mov es, eax`
- `1:02743 int 0x15`
- `1:02745 pop es`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/bios_wait_timer.json).

**Caller evidence.** src/frame.c:309. Full lists and address registrations are in JSON.

## sub_2748

`1:02748`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** ESI writable u32 BIOS tick output. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:101:sub_2748](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux sub_2748 parm [esi] /* src/effect1.c:82 */
#pragma aux sub_2748 parm [esi] /* src/sound.c:71 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** BIOS INT 0x1A AH=0 returns tick halves CX:DX; write ((CX&65535)<<16)|(DX&65535) through ESI. Preserve all GPRs. INT site 1:0274b. BIOS behavior is an abstract input.

**Valid test domain.** Mapped u32 output pointer; BIOS response supplies CX and DX.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:0274b int 0x1a`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/sub_2748.json).

**Caller evidence.** src/effect 1.c:664, src/effect 1.c:684, src/sound.c:866, src/sound.c:871, src/sound.c:885. Full lists and address registrations are in JSON.

## asm_0275c

`1:0275c`; private; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT_CONTRIBUTION (imported acceptance status, no fresh compile).

**Contract.** ESI source, EDI aperture, ECX plane pitch; incoming DF. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [private.py:41:upload](private.py). Preserved throughout the tested domain: EBP, EBX, ECX, EDI, EDX. ESP returns as specified above.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Private unreferenced planar upload. Copy four 16000-byte source planes separated by ECX to one aperture destination using masks 1, 2, 4, 8 (word OUT 0x3C4 at 1:02769); 16000 bytes are copied as 4000 dwords, with incoming DF. End ESI=original+3*plane_pitch, EAX AH=16; preserve other GPRs.

**Valid test domain.** Mapped 4-plane source and VGA destination; DF=0 or 1, with both traversed intervals mapped.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:02769 out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 3/3 (100.0%). Current mismatches: 0. [Per-routine certificate](results/asm_0275c.json).

## display_emptypicture

`1:02783`; public; name **RAYVERSE**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EDI aperture (ECX in some C declarations is ignored). Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:145:display_emptypicture](models.py). Preserved throughout the tested domain: EBP, EBX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux display_emptypicture parm [edi] [ecx] modify exact [eax ecx edx edi] /* src/affiche.c:334 */
#pragma aux display_emptypicture parm [edi] [ecx] /* src/frame.c:183 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** OUT 0x3C4, word(0x0F02) at 1:0278d selects all planes, then zero 16000 aperture bytes (64000 planar pixels). End EAX=ECX=0, EDX=0x3C4, EDI+=16000.

**Valid test domain.** DF=0; mapped planar aperture; flat bytes are aperture addresses.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:0278d out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/display_emptypicture.json).

**Caller evidence.** src/affiche.c:1194, src/frame.c:960, src/frame.c:961. Full lists and address registrations are in JSON.

## display_anypicture

`1:027a2`; public; name **RAYVERSE**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** ESI source, EDI aperture, EAX height, DL source row pitch, DH source plane phase, ECX plane pitch, EBX dword width. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [hardware.py:99:picture](hardware.py). Preserved throughout the tested domain: EBP, EBX. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux display_anypicture_t parm [esi] [edi] [eax] [edx] [ecx] [ebx] modify exact [eax ecx edx esi edi] /* src/frame.c:52 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: code 1:02799 source pitch; code 1:0279d plane pitch; code 1:027a1 source phase.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Copy planar rectangle: EAX height, EBX width in dwords, DL source row pitch, DH source-plane mask(1, 2, 4, 8), ECX plane pitch, ESI source, EDI aperture destination. Four output planes start at 1<<(EDI&3). Each plane copies width*4 bytes per row at source pitchDL, destination pitch 80; source traverses plane blocks, wrapping after mask 8 by subtracting 4*plane_pitch and advancing 1 source byte. Emit five word OUT 0x3C4 map-mask writes (initial plus one after each plane). Mutable source-pitch/source-plane state at code 1:02799..027a1. End EDI=original+1, ESI=original-plane_pitch+1, EDX=0x3C4, ECX=0x1000|(original EDI&3); EBP/EBX preserved.

**Valid test domain.** DF=0; height>=1; width dwords>=0; 4*width<=DL; DH in 1, 2, 4, 8; four source planes mapped with phase-dependent negative/positive margins.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:027dc out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 7/7 (100.0%). Current mismatches: 0. [Per-routine certificate](results/display_anypicture.json).

**Caller evidence.** src/frame.c:1052. Full lists and address registrations are in JSON.

## asm_0283c

`1:0283c`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** AL map mask. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:136:asm_0283c](models.py). Preserved throughout the tested domain: EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux asm_0283c parm [eax] modify exact [eax] /* src/frame.c:554 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Select write planes: OUT 0x3C4, word((AL<<8)|2) at 1:0284b. End EAX=((input AL)<<8)|2; preserve EDX and other GPRs.

**Valid test domain.** Any byte plane mask.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:0284b out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/asm_0283c.json).

**Caller evidence.** src/frame.c:621. Full lists and address registrations are in JSON.

## asm_02850

`1:02850`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX map_tile_t*, EDX output run lists, EBX source/cache pointer, ECX source stride or cache base, ESI destination, EDI map row skip bytes. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [builders.py:54:fn](builders.py). Preserved throughout the tested domain: EBP, EBX, ECX, EDI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux asm_02850 parm [eax] [edx] [ebx] [ecx] [esi] [edi] modify exact [eax edx esi] /* src/frame.c:744 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

map_tile_t; src/include/map.h (RAYVERSE structure, unknown fields retained) — size 6.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 2 | no | texture_id |
| 0x2 | 1 | no | tile_type |
| 0x3 | 1 | no | field_3 |
| 0x4 | 1 | no | transparency:0=opaque map cache, nonzero=background visible |
| 0x5 | 1 | no | field_5 |

row run list (INVENTED fields; src/frame.c declares u32[14][32]) bytes; stride 128.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 4 | no | number of 12-byte run entries, 0..10 |
| 0x4 | 4 | no | run[0].source: byte address in selected background/cache plane |
| 0x8 | 4 | no | run[0].destination: VGA aperture byte address or offset added to EDI |
| 0xc | 4 | no | run[0].tile_count: horizontal 16-pixel tiles (4 aperture bytes each) |
| 0x10 | 0 | no | remaining entries repeat at 12-byte stride |

**Computation.** Build 14 rows of 20 map tiles, emitting runs of byte+4==0. Source cache starts in EBX, wraps at ECX+0x4600 by subtracting 0x4600 after each 1280-byte tile row. Split runs at 80-byte cache-row seams; on wrapped horizontal segment subtract 80 from source. Destination starts ESI and advances 4 per tile, 1280 per row. Each 128-byte output row starts with run count; 12-byte entries are source address, destination address, tile count. EAX advances through 6-byte map records; EDI is additional map row skip in bytes and is preserved. EDX ends on last output row; ESI ends 14*1280 beyond start. EBX is advanced through one full 0x4600-byte circular plane and returns to its original value; ECX and EBP are preserved.

**Valid test domain.** Mapped map with at least 14 rows (15 for 02a73), width>=20; row skip=(map_width-20)*6; output 14x128 bytes; category 0/nonzero; at most 10 runs per row; cache-row positions valid for 8-bit quotient division.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 26/27 (96.3%). Current mismatches: 0. [Per-routine certificate](results/asm_02850.json).

Uncovered: 1:02920. See scope/reachability notes; no comparison was weakened to increase coverage.

**Caller evidence.** src/frame.c:775. Full lists and address registrations are in JSON.

## asm_029d3

`1:029d3`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX map_tile_t*, EDX output run lists, EBX source/cache pointer, ECX source stride or cache base, ESI destination, EDI map row skip bytes. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [builders.py:54:fn](builders.py). Preserved throughout the tested domain: EBP, EDI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux asm_029d3 parm [eax] [edx] [ebx] [ecx] [esi] [edi] modify exact [eax ebx ecx edx esi] /* src/frame.c:747 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

map_tile_t; src/include/map.h (RAYVERSE structure, unknown fields retained) — size 6.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 2 | no | texture_id |
| 0x2 | 1 | no | tile_type |
| 0x3 | 1 | no | field_3 |
| 0x4 | 1 | no | transparency:0=opaque map cache, nonzero=background visible |
| 0x5 | 1 | no | field_5 |

row run list (INVENTED fields; src/frame.c declares u32[14][32]) bytes; stride 128.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 4 | no | number of 12-byte run entries, 0..10 |
| 0x4 | 4 | no | run[0].source: byte address in selected background/cache plane |
| 0x8 | 4 | no | run[0].destination: VGA aperture byte address or offset added to EDI |
| 0xc | 4 | no | run[0].tile_count: horizontal 16-pixel tiles (4 aperture bytes each) |
| 0x10 | 0 | no | remaining entries repeat at 12-byte stride |

**Computation.** Build 14 rows of 20 map tiles, emitting runs of byte+4!=0. Source starts EBX and advances ECX per tile row; destination ESI advances 4 per tile, 1280 per row. Each 128-byte output row starts with run count; 12-byte entries are source address, destination address, tile count. EAX advances through 6-byte map records; EDI is additional map row skip in bytes and is preserved. EDX ends on last output row; ESI ends 14*1280 beyond start. Saved EBX is advanced and returned modified; EBP preserved.

**Valid test domain.** Mapped map with at least 14 rows (15 for 02a73), width>=20; row skip=(map_width-20)*6; output 14x128 bytes; category 0/nonzero; at most 10 runs per row; cache-row positions valid for 8-bit quotient division.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 12/12 (100.0%). Current mismatches: 0. [Per-routine certificate](results/asm_029d3.json).

**Caller evidence.** src/frame.c:772. Full lists and address registrations are in JSON.

## asm_02a73

`1:02a73`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX map_tile_t*, EDX output run lists, EBX source/cache pointer, ECX source stride or cache base, ESI destination, EDI map row skip bytes. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [builders.py:54:fn](builders.py). Preserved throughout the tested domain: EBP, ECX, EDI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux asm_02a73 parm [eax] [edx] [ebx] [ecx] [esi] [edi] modify exact [eax ebx ecx edx esi] /* src/frame.c:750 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

map_tile_t; src/include/map.h (RAYVERSE structure, unknown fields retained) — size 6.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 2 | no | texture_id |
| 0x2 | 1 | no | tile_type |
| 0x3 | 1 | no | field_3 |
| 0x4 | 1 | no | transparency:0=opaque map cache, nonzero=background visible |
| 0x5 | 1 | no | field_5 |

row run list (INVENTED fields; src/frame.c declares u32[14][32]) bytes; stride 128.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 4 | no | number of 12-byte run entries, 0..10 |
| 0x4 | 4 | no | run[0].source: byte address in selected background/cache plane |
| 0x8 | 4 | no | run[0].destination: VGA aperture byte address or offset added to EDI |
| 0xc | 4 | no | run[0].tile_count: horizontal 16-pixel tiles (4 aperture bytes each) |
| 0x10 | 0 | no | remaining entries repeat at 12-byte stride |

**Computation.** Skip initial map row, then build 14 rows of 19 inner tiles starting at column 1. Emit runs of byte+4!=0. Source saved EBX and destination ESI advance 4 per tile including skipped first column, 1280 per row. This variant ignores ECX as source stride when advancing the saved source slot. Each 128-byte output row starts with run count; 12-byte entries are source address, destination address, tile count. EAX advances through 6-byte map records; EDI is additional map row skip in bytes and is preserved. EDX ends on last output row; ESI ends 14*1280 beyond start. Saved EBX is advanced and returned modified; EBP preserved.

**Valid test domain.** Mapped map with at least 14 rows (15 for 02a73), width>=20; row skip=(map_width-20)*6; output 14x128 bytes; category 0/nonzero; at most 10 runs per row; cache-row positions valid for 8-bit quotient division.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 12/12 (100.0%). Current mismatches: 0. [Per-routine certificate](results/asm_02a73.json).

**Caller evidence.** src/frame.c:768. Full lists and address registrations are in JSON.

## asm_02b30

`1:02b30`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** AH plane mask, AL=2, DX=0x3C4, EDI aperture pointer. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [hardware.py:23:advance](hardware.py). Preserved throughout the tested domain: EBP, EBX, ECX, EDX, ESI. ESP returns as specified above.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Advance current plane mask AH by doubling; if it becomes 16, reset to 1 and increment EDI aperture address. OUT DX, AX (1:02b3a). AL is sequencer index 2. Preserve all other GPRs.

**Valid test domain.** AH in 1, 2, 4, 8; DX=0x3C4; AL=2.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:02b3a out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 3/3 (100.0%). Current mismatches: 0. [Per-routine certificate](results/asm_02b30.json).

## mode_x_effect_2b3d

`1:02b3d`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:61:fn](weather.py). Preserved throughout the tested domain: EBP, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Mode-X weather stamp columns ['5'] (top to bottom per column; dot untouched, digits palette indices). Anchor admitted iff 0<=x<=320 and 0<=y<=200 inclusive. Aperture address=EDI+floor(x/4)+80*y, initial plane=1<<(x&3); advance planes per column, wrapping 8->1 and advancing aperture byte. OUT 0x3C4, word((mask<<8)|2) initially and through helper 1:02b30. No extent clipping. End EBX=64*y, EAX=final mask<<8|2; preserve ECX, EDX, ESI, EDI, EBP.

**Valid test domain.** Mapped VGA footprint; anchor can lie outside screen and is then rejected. Plane writes and exact I/O are compared.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:02b6a out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/mode_x_effect_2b3d.json).

## fplot_mode_x_2b86

`1:02b86`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, ESI color, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:64:<lambda>](weather.py). Preserved throughout the tested domain: EBP, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux fplot_func_t parm [edi] [eax] [ebx] [esi] modify exact [eax ebx] /* src/action_local.h:18 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Store low byte of ESI at VGA plane x&3, address EDI+floor(x/4)+80*y. OUT 0x3C4, word((1<<(x&3))<<8|2) at 1:02b9a. No clipping. End EAX=mask<<8|2, EBX=64*y; preserve other GPRs.

**Valid test domain.** Mapped pixel; x0..319, y0..199.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:02b9a out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/fplot_mode_x_2b86.json).

**Caller evidence.** src/affiche.c:961. Full lists and address registrations are in JSON.

## mode_x_effect_2bb7

`1:02bb7`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:61:fn](weather.py). Preserved throughout the tested domain: EBP, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Mode-X weather stamp columns ['7'] (top to bottom per column; dot untouched, digits palette indices). Anchor admitted iff 0<=x<=320 and 0<=y<=200 inclusive. Aperture address=EDI+floor(x/4)+80*y, initial plane=1<<(x&3); advance planes per column, wrapping 8->1 and advancing aperture byte. OUT 0x3C4, word((mask<<8)|2) initially and through helper 1:02b30. No extent clipping. End EBX=64*y, EAX=final mask<<8|2; preserve ECX, EDX, ESI, EDI, EBP.

**Valid test domain.** Mapped VGA footprint; anchor can lie outside screen and is then rejected. Plane writes and exact I/O are compared.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:02be4 out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/mode_x_effect_2bb7.json).

## mode_x_effect_2c00

`1:02c00`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:61:fn](weather.py). Preserved throughout the tested domain: EBP, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Mode-X weather stamp columns ['87', '76'] (top to bottom per column; dot untouched, digits palette indices). Anchor admitted iff 0<=x<=320 and 0<=y<=200 inclusive. Aperture address=EDI+floor(x/4)+80*y, initial plane=1<<(x&3); advance planes per column, wrapping 8->1 and advancing aperture byte. OUT 0x3C4, word((mask<<8)|2) initially and through helper 1:02b30. No extent clipping. End EBX=64*y, EAX=final mask<<8|2; preserve ECX, EDX, ESI, EDI, EBP.

**Valid test domain.** Mapped VGA footprint; anchor can lie outside screen and is then rejected. Plane writes and exact I/O are compared.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:02c2d out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/mode_x_effect_2c00.json).

## mode_x_effect_2c59

`1:02c59`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:61:fn](weather.py). Preserved throughout the tested domain: EBP, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Mode-X weather stamp columns ['.6.', '686', '.6.'] (top to bottom per column; dot untouched, digits palette indices). Anchor admitted iff 0<=x<=320 and 0<=y<=200 inclusive. Aperture address=EDI+floor(x/4)+80*y, initial plane=1<<(x&3); advance planes per column, wrapping 8->1 and advancing aperture byte. OUT 0x3C4, word((mask<<8)|2) initially and through helper 1:02b30. No extent clipping. End EBX=64*y, EAX=final mask<<8|2; preserve ECX, EDX, ESI, EDI, EBP.

**Valid test domain.** Mapped VGA footprint; anchor can lie outside screen and is then rejected. Plane writes and exact I/O are compared.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:02c86 out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/mode_x_effect_2c59.json).

## mode_x_effect_2cbf

`1:02cbf`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:61:fn](weather.py). Preserved throughout the tested domain: EBP, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Mode-X weather stamp columns ['3'] (top to bottom per column; dot untouched, digits palette indices). Anchor admitted iff 0<=x<=320 and 0<=y<=200 inclusive. Aperture address=EDI+floor(x/4)+80*y, initial plane=1<<(x&3); advance planes per column, wrapping 8->1 and advancing aperture byte. OUT 0x3C4, word((mask<<8)|2) initially and through helper 1:02b30. No extent clipping. End EBX=64*y, EAX=final mask<<8|2; preserve ECX, EDX, ESI, EDI, EBP.

**Valid test domain.** Mapped VGA footprint; anchor can lie outside screen and is then rejected. Plane writes and exact I/O are compared.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:02cec out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/mode_x_effect_2cbf.json).

## mode_x_effect_2d08

`1:02d08`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:61:fn](weather.py). Preserved throughout the tested domain: EBP, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Mode-X weather stamp columns ['.55.', '5775', '5775', '.55.'] (top to bottom per column; dot untouched, digits palette indices). Anchor admitted iff 0<=x<=320 and 0<=y<=200 inclusive. Aperture address=EDI+floor(x/4)+80*y, initial plane=1<<(x&3); advance planes per column, wrapping 8->1 and advancing aperture byte. OUT 0x3C4, word((mask<<8)|2) initially and through helper 1:02b30. No extent clipping. End EBX=64*y, EAX=final mask<<8|2; preserve ECX, EDX, ESI, EDI, EBP.

**Valid test domain.** Mapped VGA footprint; anchor can lie outside screen and is then rejected. Plane writes and exact I/O are compared.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:02d41 out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/mode_x_effect_2d08.json).

## mode_x_effect_2da9

`1:02da9`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:61:fn](weather.py). Preserved throughout the tested domain: EBP, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Mode-X weather stamp columns ['.5', '4.'] (top to bottom per column; dot untouched, digits palette indices). Anchor admitted iff 0<=x<=320 and 0<=y<=200 inclusive. Aperture address=EDI+floor(x/4)+80*y, initial plane=1<<(x&3); advance planes per column, wrapping 8->1 and advancing aperture byte. OUT 0x3C4, word((mask<<8)|2) initially and through helper 1:02b30. No extent clipping. End EBX=64*y, EAX=final mask<<8|2; preserve ECX, EDX, ESI, EDI, EBP.

**Valid test domain.** Mapped VGA footprint; anchor can lie outside screen and is then rejected. Plane writes and exact I/O are compared.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:02dd6 out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/mode_x_effect_2da9.json).

## mode_x_effect_2dfb

`1:02dfb`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:61:fn](weather.py). Preserved throughout the tested domain: EBP, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Mode-X weather stamp columns ['.66.', '6886', '6886', '.66.'] (top to bottom per column; dot untouched, digits palette indices). Anchor admitted iff 0<=x<=320 and 0<=y<=200 inclusive. Aperture address=EDI+floor(x/4)+80*y, initial plane=1<<(x&3); advance planes per column, wrapping 8->1 and advancing aperture byte. OUT 0x3C4, word((mask<<8)|2) initially and through helper 1:02b30. No extent clipping. End EBX=64*y, EAX=final mask<<8|2; preserve ECX, EDX, ESI, EDI, EBP.

**Valid test domain.** Mapped VGA footprint; anchor can lie outside screen and is then rejected. Plane writes and exact I/O are compared.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:02e34 out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/mode_x_effect_2dfb.json).

**Caller evidence.** src/affiche.c:924. Full lists and address registrations are in JSON.

## mode_x_effect_2e9c

`1:02e9c`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:61:fn](weather.py). Preserved throughout the tested domain: EBP, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Mode-X weather stamp columns ['.36', '253', '42.'] (top to bottom per column; dot untouched, digits palette indices). Anchor admitted iff 0<=x<=320 and 0<=y<=200 inclusive. Aperture address=EDI+floor(x/4)+80*y, initial plane=1<<(x&3); advance planes per column, wrapping 8->1 and advancing aperture byte. OUT 0x3C4, word((mask<<8)|2) initially and through helper 1:02b30. No extent clipping. End EBX=64*y, EAX=final mask<<8|2; preserve ECX, EDX, ESI, EDI, EBP.

**Valid test domain.** Mapped VGA footprint; anchor can lie outside screen and is then rejected. Plane writes and exact I/O are compared.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:02ec9 out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/mode_x_effect_2e9c.json).

## mode_x_effect_2f0c

`1:02f0c`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:61:fn](weather.py). Preserved throughout the tested domain: EBP, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Mode-X weather stamp columns ['..66..', '.7887.', '688886', '688886', '.7887.', '..66..'] (top to bottom per column; dot untouched, digits palette indices). Anchor admitted iff 0<=x<=320 and 0<=y<=200 inclusive. Aperture address=EDI+floor(x/4)+80*y, initial plane=1<<(x&3); advance planes per column, wrapping 8->1 and advancing aperture byte. OUT 0x3C4, word((mask<<8)|2) initially and through helper 1:02b30. No extent clipping. End EBX=64*y, EAX=final mask<<8|2; preserve ECX, EDX, ESI, EDI, EBP.

**Valid test domain.** Mapped VGA footprint; anchor can lie outside screen and is then rejected. Plane writes and exact I/O are compared.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:02f49 out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/mode_x_effect_2f0c.json).

**Caller evidence.** src/affiche.c:918. Full lists and address registrations are in JSON.

## mode_x_effect_300f

`1:0300f`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:61:fn](weather.py). Preserved throughout the tested domain: EBP, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Mode-X weather stamp columns ['..46', '.354', '243.', '32..'] (top to bottom per column; dot untouched, digits palette indices). Anchor admitted iff 0<=x<=320 and 0<=y<=200 inclusive. Aperture address=EDI+floor(x/4)+80*y, initial plane=1<<(x&3); advance planes per column, wrapping 8->1 and advancing aperture byte. OUT 0x3C4, word((mask<<8)|2) initially and through helper 1:02b30. No extent clipping. End EBX=64*y, EAX=final mask<<8|2; preserve ECX, EDX, ESI, EDI, EBP.

**Valid test domain.** Mapped VGA footprint; anchor can lie outside screen and is then rejected. Plane writes and exact I/O are compared.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:03040 out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/mode_x_effect_300f.json).

**Caller evidence.** src/affiche.c:931. Full lists and address registrations are in JSON.

## asm_0309d

`1:0309d`; private; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT_CONTRIBUTION (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI aperture. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [private.py:57:stamp](private.py). Preserved throughout the tested domain: EBP, ECX, EDI, EDX, ESI. ESP returns as specified above.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Newly inventoried private unreferenced world-map-like stamp (purpose INVENTED). No clipping. Four successive plane columns contain [none, 12, 0], [12, 14, 10, 0], [0, 10, 9, 0], [none, 0, 0]; explicit zeros ARE written. Initial word OUT 0x3C4 selects 1<<(x&3), then advance through helper 1:02b30. End EAX=last mask<<8|2, EBX=64*y; preserve ECX/EDX/ESI/EDI/EBP.

**Valid test domain.** x0..319, y0..199 with mapped stamp footprint. Not included in prior 109+8 inventory.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:030b1 out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/asm_0309d.json).

## draw_blackbox

`1:03125`; public; name **RAYVERSE**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EDI aperture, ESI x, EDX y, EBX height, ECX width. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [hardware.py:47:black](hardware.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux draw_blackbox parm [edi] [esi] [edx] [ebx] [ecx] modify exact [] /* src/affiche.c:264 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Fill a black rectangle in ModeX: x=ESI, y=EDX, height=EBX, width=ECX. Ignore nonpositive dimensions. Select sequencer index 2 by byte OUT 0x3C4=2; write left partial plane mask, then whole aperture bytes with mask 15, then right partial mask. Aperture pitch 80. Masks are left[15, 14, 12, 8] and right[15, 1, 3, 7]; overlapping edge byte combines masks. Preserve GPRs.

**Valid test domain.** DF=0; x/y>=0; positive dimensions fit VGA aperture; nonpositive dimensions also tested.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:03149 out dx, al`
- `1:03176 out dx, al`
- `1:031a1 out dx, al`
- `1:031df out dx, al`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 20/20 (100.0%). Current mismatches: 0. [Per-routine certificate](results/draw_blackbox.json).

**Caller evidence.** src/affiche.c:981. Full lists and address registrations are in JSON.

## draw_borderbox

`1:031ed`; public; name **RAYVERSE**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EDI aperture, EAX x, EBX y, ECX height delta, EDX width delta. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [hardware.py:73:border](hardware.py). Preserved throughout the tested domain: EBP, EDI, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux draw_borderbox parm [edi] [eax] [ebx] [ecx] [edx] modify exact [eax ebx ecx edx] /* src/affiche.c:266 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Draw a white(255) ModeX outline from(x=EAX, y=EBX) to(x+EDX, y+ECX), both endpoints included: width/height arguments are coordinate deltas. Initial word OUT 0x3C4 selects plane at x; advance planes with helper 1:02b30 after each column. Preserve EDI/EBP/ESI; end EBX=80*height, ECX=0, EDX low 16=0x3C4, EAX=last plane mask<<8|2.

**Valid test domain.** width delta>=2, height delta>=0; mapped VGA footprint including final row/column.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:03202 out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 7/7 (100.0%). Current mismatches: 0. [Per-routine certificate](results/draw_borderbox.json).

**Caller evidence.** src/affiche.c:989, src/affiche.c:994. Full lists and address registrations are in JSON.

## asm_03257

`1:03257`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** AL plane mask, EDX source pitch, EBX plane offset, ECX context pointer, ESI run lists, EDI destination offset. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [records.py:63:fn](records.py). Preserved throughout the tested domain: EBP, EBX, EDI, EDX. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux asm_03257 parm [eax] [edx] [ebx] [esi] [edi] [ecx] modify exact [eax ecx esi] /* src/frame.c:80 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

row run list (INVENTED fields; src/frame.c declares u32[14][32]) bytes; stride 128.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 4 | no | number of 12-byte run entries, 0..10 |
| 0x4 | 4 | no | run[0].source: byte address in selected background/cache plane |
| 0x8 | 4 | no | run[0].destination: VGA aperture byte address or offset added to EDI |
| 0xc | 4 | no | run[0].tile_count: horizontal 16-pixel tiles (4 aperture bytes each) |
| 0x10 | 0 | no | remaining entries repeat at 12-byte stride |

frame_context_t first 3 fields; purpose names INVENTED, src/frame.c.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 4 | no | plane_size (saved then discarded by these renderers) |
| 0x4 | 4 | no | source_end threshold; also returned in ESI |
| 0x8 | 4 | no | run_step: source wrap subtraction (caller 03257 uses plan 0bit_length-1) |

**Computation.** Render 14128-byte row lists. Each run copies tile_count*4 bytes on 16 scanlines at source pitchEDX and destination pitch 80. Source=run.source+EBX; when source>=context.source_end subtract context.run_step once. Destination=run.destination+EDI. AL selects VGA plane through word OUT 0x3C4 (index 2). Alignment fragments copy first/last bytes or words, then middle dwords; visible result is the same rectangle. ESI returns context.source_end; ECX returns row count; EAX is last copy scratch (fully specified by model); preserve EBX/EDX/EDI/EBP.

**Valid test domain.** DF=0; list count 0..10; positive tile_count<=20; mapped source/destination rectangles, wrap bound valid; EDI offset 0 or 1 at C callers. No globals read.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:0326c out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 17/17 (100.0%). Current mismatches: 0. [Per-routine certificate](results/asm_03257.json).

**Caller evidence.** src/frame.c:522. Full lists and address registrations are in JSON.

## asm_037e4

`1:037e4`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** ESI run lists, ECX VGA destination page. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [records.py:66:latchcopy](records.py). Preserved throughout the tested domain: EBP. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux asm_037e4 parm [esi] [ecx] modify exact [eax ebx ecx edx esi edi] /* src/frame.c:82 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

row run list (INVENTED fields; src/frame.c declares u32[14][32]) bytes; stride 128.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 4 | no | number of 12-byte run entries, 0..10 |
| 0x4 | 4 | no | run[0].source: byte address in selected background/cache plane |
| 0x8 | 4 | no | run[0].destination: VGA aperture byte address or offset added to EDI |
| 0xc | 4 | no | run[0].tile_count: horizontal 16-pixel tiles (4 aperture bytes each) |
| 0x10 | 0 | no | remaining entries repeat at 12-byte stride |

**Computation.** VGA latch-copy path. Set graphics register 5 write-mode bits to 1 via OUT 0x3CE=5, IN/OUT 0x3CF at 1:037ef..037f7; select all planes with OUT 0x3C4, word 0x0F02. Copy 4 aperture bytes per row for 184 rows from 0xAB502 to ECX+0x502 at pitch 80. Then process 12 run-list rows: each nonzero tile count copies 4*count+1 aperture bytes on 16 rows, source/destination pitch 80, source/dest absolute addresses from run entries. A read latches all 4 source planes and each subsequent byte write copies all 4 latches (CPU byte itself is ignored by VGA write-mode 1). Finally read graphics register 5 and clear low 2 bits via 1:03906..0390c. CLI around run copies, final STI. EBP preserved; other GPR scratch outputs defined in model.

**Valid test domain.** DF=0; valid VGA source/destination rectangles, run count 0..10, tile count 0..19; 4 independent mapped VGA planes. Hardware latch behavior is an abstract device stub with exact port-sequence and logical-plane-byte comparison.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:037ef out dx, al`
- `1:037f2 in al, dx`
- `1:037f7 out dx, al`
- `1:03802 out dx, ax`
- `1:03829 cli `
- `1:03906 out dx, al`
- `1:03909 in al, dx`
- `1:0390c out dx, al`
- `1:03910 sti `

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 11/11 (100.0%). Current mismatches: 0. [Per-routine certificate](results/asm_037e4.json).

**Caller evidence.** src/frame.c:518. Full lists and address registrations are in JSON.

## asm_03912

`1:03912`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** AL plane mask, EDX source pitch, EBX plane offset, ECX context pointer, ESI run lists, EDI destination offset. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [records.py:63:fn](records.py). Preserved throughout the tested domain: EBP, EBX, EDI, EDX. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux asm_03912 parm [eax] [edx] [ebx] [ecx] [esi] [edi] modify exact [eax ecx esi] /* src/frame.c:549 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

row run list (INVENTED fields; src/frame.c declares u32[14][32]) bytes; stride 128.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 4 | no | number of 12-byte run entries, 0..10 |
| 0x4 | 4 | no | run[0].source: byte address in selected background/cache plane |
| 0x8 | 4 | no | run[0].destination: VGA aperture byte address or offset added to EDI |
| 0xc | 4 | no | run[0].tile_count: horizontal 16-pixel tiles (4 aperture bytes each) |
| 0x10 | 0 | no | remaining entries repeat at 12-byte stride |

frame_context_t first 3 fields; purpose names INVENTED, src/frame.c.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 4 | no | plane_size (saved then discarded by these renderers) |
| 0x4 | 4 | no | source_end threshold; also returned in ESI |
| 0x8 | 4 | no | run_step: source wrap subtraction (caller 03257 uses plan 0bit_length-1) |

**Computation.** Render 14128-byte row lists. Each run copies tile_count*4 bytes on 16 scanlines at source pitchEDX and destination pitch 80. Source=run.source+EBX; when source>=context.source_end subtract context.run_step once. Destination=run.destination+EDI. AL selects VGA plane through word OUT 0x3C4 (index 2). Alignment fragments copy first/last bytes or words, then middle dwords; visible result is the same rectangle. ESI returns context.source_end; ECX returns row count; EAX is last copy scratch (fully specified by model); preserve EBX/EDX/EDI/EBP.

**Valid test domain.** DF=0; list count 0..10; positive tile_count<=20; mapped source/destination rectangles, wrap bound valid; EDI offset 0 or 1 at C callers. No globals read.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:03927 out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 17/17 (100.0%). Current mismatches: 0. [Per-routine certificate](results/asm_03912.json).

**Caller evidence.** src/frame.c:594. Full lists and address registrations are in JSON.

## asm_03ea1

`1:03ea1`; public; name **INVENTED**. Source: [src/lowlevel_02640.asm](../../../src/lowlevel_02640.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** AL plane mask, EDX source pitch, EBX plane offset, ECX context pointer, ESI run lists, EDI destination offset. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [records.py:63:fn](records.py). Preserved throughout the tested domain: EBP, EBX, EDI, EDX. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux asm_03ea1 parm [eax] [edx] [ebx] [ecx] [esi] [edi] modify exact [eax ecx esi] /* src/frame.c:552 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

row run list (INVENTED fields; src/frame.c declares u32[14][32]) bytes; stride 128.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 4 | no | number of 12-byte run entries, 0..10 |
| 0x4 | 4 | no | run[0].source: byte address in selected background/cache plane |
| 0x8 | 4 | no | run[0].destination: VGA aperture byte address or offset added to EDI |
| 0xc | 4 | no | run[0].tile_count: horizontal 16-pixel tiles (4 aperture bytes each) |
| 0x10 | 0 | no | remaining entries repeat at 12-byte stride |

frame_context_t first 3 fields; purpose names INVENTED, src/frame.c.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 4 | no | plane_size (saved then discarded by these renderers) |
| 0x4 | 4 | no | source_end threshold; also returned in ESI |
| 0x8 | 4 | no | run_step: source wrap subtraction (caller 03257 uses plan 0bit_length-1) |

**Computation.** Render 12128-byte row lists. Each run copies tile_count*4 bytes on 16 scanlines at source pitchEDX and destination pitch 80. Source=run.source+EBX; when source>=context.source_end subtract context.run_step once. Destination=run.destination+EDI. AL selects VGA plane through word OUT 0x3C4 (index 2). Alignment fragments copy first/last bytes or words, then middle dwords; visible result is the same rectangle. ESI returns context.source_end; ECX returns row count; EAX is last copy scratch (fully specified by model); preserve EBX/EDX/EDI/EBP.

**Valid test domain.** DF=0; list count 0..10; positive tile_count<=20; mapped source/destination rectangles, wrap bound valid; EDI offset 0 or 1 at C callers. No globals read.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:03eb6 out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 17/17 (100.0%). Current mismatches: 0. [Per-routine certificate](results/asm_03ea1.json).

**Caller evidence.** src/frame.c:590. Full lists and address registrations are in JSON.

## asm_04430

`1:04430`; public; name **INVENTED**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** AH phase 0..3, EDX map_tile_t*, EBX frame_context_t*, ECX callback table, EDI aperture, ESI background compatibility argument unused. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [callbacks.py:58:fn](callbacks.py). Preserved throughout the tested domain: EAX, EBP, EBX. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux asm_04430 parm [eax] [edx] [ebx] [ecx] [esi] [edi] modify exact [edx esi edi] /* src/frame.c:560 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

map_tile_t; src/include/map.h (RAYVERSE structure, unknown fields retained) — size 6.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 2 | no | texture_id |
| 0x2 | 1 | no | tile_type |
| 0x3 | 1 | no | field_3 |
| 0x4 | 1 | no | transparency:0=opaque map cache, nonzero=background visible |
| 0x5 | 1 | no | field_5 |

frame_context_t; src/frame.c (purpose names INVENTED).

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x4 | 4 | no | source_end reused as map byte row stride |
| 0x8 | 4 | no | run_step reused as destination row adjustment (normally 1200) |
| 0x14 | 4 | no | last_tile pointer stopping threshold |

**Computation.** REUSED: ../tilecode/SPEC.md sections on ModeX callback callers. Traverse 20 map records per row and up to 14 rows. Only transparency byte+4==2 with nonnull callback table entry draws a sparse constant 16x16 tile selected by AH phase 0..3. EDI advances 4 per tile and context.run_step between rows; ESI advances context.source_end (used here as map byte stride) between rows and stops before context.last_tile. Callback pixel semantics/clobbers are imported from tilecode/catalogue.py and its existing 71400-run certificate, not re-specified. Preserve EAX/EBX/ECX/EBP; ESI/EDI/EDX outputs specified by wrapper model.

**Valid test domain.** Mapped 6-byte map records, 1200-entry callback table, frame_context_t, phaseAH 0..3, valid sparse tile bodies from shipped GOG levels. Wrapper execution uses 16 representative bodies from one level; full body corpus proof is inherited.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 11/11 (100.0%). Current mismatches: 0. [Per-routine certificate](results/asm_04430.json).

**Caller evidence.** src/frame.c:634. Full lists and address registrations are in JSON.

## asm_04493

`1:04493`; public; name **INVENTED**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** AH phase 0..3, EDX map_tile_t*, EBX frame_context_t*, ECX callback table, EDI aperture, ESI background compatibility argument unused. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [callbacks.py:58:fn](callbacks.py). Preserved throughout the tested domain: EAX, EBP, EBX. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux asm_04493 parm [eax] [edx] [ebx] [ecx] [esi] [edi] modify exact [edx esi edi] /* src/frame.c:563 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

map_tile_t; src/include/map.h (RAYVERSE structure, unknown fields retained) — size 6.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 2 | no | texture_id |
| 0x2 | 1 | no | tile_type |
| 0x3 | 1 | no | field_3 |
| 0x4 | 1 | no | transparency:0=opaque map cache, nonzero=background visible |
| 0x5 | 1 | no | field_5 |

frame_context_t; src/frame.c (purpose names INVENTED).

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x4 | 4 | no | source_end reused as map byte row stride |
| 0x8 | 4 | no | run_step reused as destination row adjustment (normally 1200) |
| 0x14 | 4 | no | last_tile pointer stopping threshold |

**Computation.** REUSED: ../tilecode/SPEC.md sections on ModeX callback callers. Traverse 20 map records per row and up to 12 inner rows after skipping first map/destination row. Only transparency byte+4==2 with nonnull callback table entry draws a sparse constant 16x16 tile selected by AH phase 0..3. EDI advances 4 per tile and context.run_step between rows; ESI advances context.source_end (used here as map byte stride) between rows and stops before context.last_tile. Callback pixel semantics/clobbers are imported from tilecode/catalogue.py and its existing 71400-run certificate, not re-specified. Preserve EAX/EBX/ECX/EBP; ESI/EDI/EDX outputs specified by wrapper model.

**Valid test domain.** Mapped 6-byte map records, 1200-entry callback table, frame_context_t, phaseAH 0..3, valid sparse tile bodies from shipped GOG levels. Wrapper execution uses 16 representative bodies from one level; full body corpus proof is inherited.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 12/12 (100.0%). Current mismatches: 0. [Per-routine certificate](results/asm_04493.json).

**Caller evidence.** src/frame.c:630. Full lists and address registrations are in JSON.

## asm_04504

`1:04504`; public; name **INVENTED**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EDI page aperture. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [hardware.py:89:fn](hardware.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux asm_04504 parm [edi] modify exact [] /* src/pcmain.c:423 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Select all planes with OUT 0x3C4, word 0x0F02 at entry, then clear 4 aperture bytes at EDI-2+80*y for y0..203 (side fringes of page). Preserve all GPRs.

**Valid test domain.** Mapped VGA page and its 2-byte left fringe; abstract planar effects compared.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:04512 out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 3/3 (100.0%). Current mismatches: 0. [Per-routine certificate](results/asm_04504.json).

**Caller evidence.** src/pcmain.c:708. Full lists and address registrations are in JSON.

## asm_0456e

`1:0456e`; public; name **INVENTED**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EDI page aperture. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [hardware.py:89:fn](hardware.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux asm_0456e parm [edi] modify exact [] /* src/pcmain.c:425 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Select all planes with OUT 0x3C4, word 0x0F02 at entry, then clear 4 aperture bytes at EDI-2+80*y for y0..203 (side fringes of page). Also clear 1280 bytes at EDI and 640 at EDI+0x3C00; explicitly clear DF. Preserve all GPRs.

**Valid test domain.** Mapped VGA page and its 2-byte left fringe; abstract planar effects compared.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:0457d out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 3/3 (100.0%). Current mismatches: 0. [Per-routine certificate](results/asm_0456e.json).

**Caller evidence.** src/pcmain.c:706. Full lists and address registrations are in JSON.

## select_display_buffer

`1:045f1`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX display page address (AH programs CRTC start high byte). Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:141:select_display_buffer](models.py). Preserved throughout the tested domain: EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Set CRTC high start address register: OUT 0x3D4, word((input AH<<8)|0x0C) at 1:045f8; low start register is untouched. EAX low byte becomes 0x0C, other GPRs preserved.

**Valid test domain.** Page address in EAX (normally 0xA0500/0xA6500).

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:045f8 out dx, ax`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/select_display_buffer.json).

**Caller evidence.** src/frame.c:274, src/frame.c:295, src/frame.c:1343. Full lists and address registrations are in JSON.

## InitFirstModeVideo

`1:045fc`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [hardware.py:5:first](hardware.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: saved_vga_registers_0196a[6].

**Layouts.**

saved_vga_registers_0196a (INVENTED).

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 1 | no | 0x3ce index 0x5 |
| 0x1 | 1 | no | 0x3ce index 0x6 |
| 0x2 | 1 | no | 0x3c4 index 0x4 |
| 0x3 | 1 | no | 0x3c4 index 0x2 |
| 0x4 | 1 | no | 0x3d4 index 0x14 |
| 0x5 | 1 | no | 0x3d4 index 0x17 |

**Computation.** Request BIOS INT 10h AX=0x13 (1:045fc..04603), then save 6 VGA register bytes. In order, OUT index / IN data: (0x3CE, 5), (0x3CE, 6), (0x3C4, 4), (0x3C4, 2), (0x3D4, 0x14), (0x3D4, 0x17). Data port=index port+1. Preserve GPRs.

**Valid test domain.** Abstract BIOS and six VGA reads; service side effects excluded.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:04601 int 0x10`
- `1:04609 out dx, al`
- `1:0460c in al, dx`
- `1:04618 out dx, al`
- `1:0461b in al, dx`
- `1:04627 out dx, al`
- `1:0462a in al, dx`
- `1:04636 out dx, al`
- `1:04639 in al, dx`
- `1:04645 out dx, al`
- `1:04648 in al, dx`
- `1:04654 out dx, al`
- `1:04657 in al, dx`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/InitFirstModeVideo.json).

**Caller evidence.** src/frame.c:1299, src/frame.c:1336. Full lists and address registrations are in JSON.

## InitModeNormal

`1:0465f`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [hardware.py:18:normal](hardware.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: saved_vga_registers_0196a[6]. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Restore six saved VGA registers in the index/data order listed for InitFirstModeVideo (1:0465f..046b7). Preserve GPRs.

**Valid test domain.** Valid saved VGA configuration.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:04666 out dx, al`
- `1:0466e out dx, al`
- `1:04673 out dx, al`
- `1:0467b out dx, al`
- `1:04682 out dx, al`
- `1:0468a out dx, al`
- `1:04691 out dx, al`
- `1:04699 out dx, al`
- `1:046a0 out dx, al`
- `1:046a8 out dx, al`
- `1:046ad out dx, al`
- `1:046b5 out dx, al`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/InitModeNormal.json).

**Caller evidence.** src/frame.c:1340. Full lists and address registrations are in JSON.

## InitModeX

`1:046b8`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [hardware.py:20:modex](hardware.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: saved_vga_registers_0196a[6]. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Program six VGA registers from saved configuration in same order as InitFirstModeVideo. Transform values respectively by &0xEF, &0xFD, (&0xF7)|4, identity, &0xBF, |0x40 (1:046b8..0471c). This unchains the four VGA planes. Preserve GPRs.

**Valid test domain.** Valid saved VGA configuration.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:046bf out dx, al`
- `1:046c9 out dx, al`
- `1:046ce out dx, al`
- `1:046d8 out dx, al`
- `1:046df out dx, al`
- `1:046eb out dx, al`
- `1:046f2 out dx, al`
- `1:046fa out dx, al`
- `1:04701 out dx, al`
- `1:0470b out dx, al`
- `1:04710 out dx, al`
- `1:0471a out dx, al`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/InitModeX.json).

**Caller evidence.** src/frame.c:1303. Full lists and address registrations are in JSON.

## block_clear

`1:0471d`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:149:block_clear](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux block_clear_t parm [edi] modify exact [] /* src/frame.c:48 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Clear 16 rows of four bytes at destination pitch 80. Preserve GPRs. One plane of a 16x16 planar tile.

**Valid test domain.** Mapped destination span of 1204 bytes; no VGA I/O.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/block_clear.json).

**Caller evidence.** src/frame.c:391. Full lists and address registrations are in JSON.

## block_copy

`1:04765`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** ESI source tile plane, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:155:block_copy](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux block_copy_t parm [esi] [edi] modify exact [] /* src/frame.c:47 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

one 16x16 tile plane (INVENTED) bytes; stride 4.

16 rows of four u8 palette indices

**Computation.** Copy 64 bytes of one tile plane: four bytes per row to 16 destination rows at pitch 80. Preserve GPRs.

**Valid test domain.** Disjoint mapped source 64 bytes and destination 1204 bytes.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/block_copy.json).

**Caller evidence.** src/frame.c:389, src/frame.c:417, src/frame.c:446. Full lists and address registrations are in JSON.

## textmode

`1:047e7`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:205:textmode](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** BIOS INT 10h AX=3 requests text mode; all GPRs preserved by PUSHAD/POPAD. INT site 1:047ed. BIOS implementation excluded.

**Valid test domain.** No arguments; service modeled as abstract interrupt.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:047ec int 0x10`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/textmode.json).

**Caller evidence.** src/frame.c:1330. Full lists and address registrations are in JSON.

## draw_sprite_deform

`1:04828`; public; name **RAYVERSE/ANDROID**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** ESI atlas source, EDI output sprite, ECX deform_blit_t*. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [deform.py:6:deform](deform.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux draw_sprite_deform parm [esi] [edi] [ecx] modify exact [] /* src/affiche.c:306 */
#pragma aux draw_sprite_deform parm [esi] [edi] [ecx] /* src/araignee.c:290 */
#pragma aux draw_sprite_deform parm [esi] [edi] [ecx] /* src/pcmain.c:297 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: code 1:047f0..04827 deformation scratch.

**Layouts.**

deform_blit_t; src/action_local.h (field names INVENTED/source reconstruction) — size 52.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 4 | yes | x_step_1 |
| 0x4 | 4 | yes | row_step_1 |
| 0x8 | 4 | yes | dx_1 |
| 0xc | 4 | yes | dy_1 |
| 0x10 | 4 | yes | x_step_2 |
| 0x14 | 4 | yes | row_step_2 |
| 0x18 | 4 | yes | dx_2 |
| 0x1c | 4 | yes | dy_2 |
| 0x20 | 4 | yes | dest_width |
| 0x24 | 4 | yes | dest_height |
| 0x28 | 4 | yes | dest_offset |
| 0x2c | 4 | yes | source_width |
| 0x30 | 4 | yes | source_height |

**Computation.** Generate a deformed row-major sprite from the 13-dword deform_blit_t record built by AFFICHE_SPRITE_DEFORME. Prefill 4*(floor(dest_width/4)+1)*dest_height consecutive bytes with 0xA1. Walk the two edge vectors with the exact integer error/step rules in deform.py; source sampling is nearest-left Q16, source pitch=source_width, destination pitch=dest_width encoded in signed row_step fields. Outer source increment=(source_height<<16)/(dx_2+dy_2); inner increment=(source_width<<16)/max(dx_1, dy_1), unsigned division. Repeated outer correction reduces inner increment by 64 Q16 units for that row. First source pointer is source-1 and each sample reads its+1 byte (the actual x86 reads 4 bytes). When dx_1>dy_1, copy only over 0xA1; when dx_1<=dy_1, copy only over 0x81 despite the 0xA1 prefill. This asymmetric original sentinel behavior is retained. Preserve all GPRs; writes 14 code-owned scratch dwords.

**Valid test domain.** Descriptor generated from a rotated/scaled rectangular sprite by src/affiche.c: nonnegative dx values, dy values forced>=1, positive destination dimensions and source byte dimensions; mapped destination footprint/source including 3-byte read tail. No float sampling.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 29/30 (96.67%). Current mismatches: 0. [Per-routine certificate](results/draw_sprite_deform.json).

Uncovered: 1:049ed. See scope/reachability notes; no comparison was weakened to increase coverage.

**Caller evidence.** src/affiche.c:1483. Full lists and address registrations are in JSON.

## local_04a83

`1:04a83`; private; name **INVENTED**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT_CONTRIBUTION (imported acceptance status, no fresh compile).

**Contract.** ESI packed source, EDI RAM planes. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [private.py:49:transpose](private.py). Preserved throughout the tested domain: EBP, EDI. ESP returns as specified above.

**Globals.** Reads: TableImageX[4]. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Convert packed 320x200 source into four contiguous RAM planes, using destination TableImageX[x&3]+floor(x/4)+80*y. Source advances 64000 bytes; EDI and EBP preserved. End ECX=320, EDX=16000, EAX=TableImageX[3]+15999, EBX=last source pixel.

**Valid test domain.** Disjoint mapped 64000-byte source/destination; TableImageX[4] contains plane offsets 0, 16000, 32000, 48000.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 5/5 (100.0%). Current mismatches: 0. [Per-routine certificate](results/local_04a83.json).

## DisplayBufferModeNormal

`1:04ab7`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** ESI source, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:122:DisplayBufferModeNormal](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDX. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux DisplayBufferModeNormal parm [esi] [edi] /* src/frame.c:182 */
#pragma aux DisplayBufferModeNormal parm [esi] [edi] /* src/ray_pc.c:28 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Forward copy 16000 dwords (64000 bytes), preserving ECX and advancing ESI/EDI by 64000.

**Valid test domain.** DF=0; two disjoint mapped 64000-byte buffers.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DisplayBufferModeNormal.json).

**Caller evidence.** src/frame.c:281, src/ray_pc.c:59. Full lists and address registrations are in JSON.

## DrawWldPointPlan2Normal

`1:04ac1`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI 384-byte-pitch destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:168:DrawWldPointPlan2Normal](models.py). Preserved throughout the tested domain: EAX, EBP, ECX, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux DrawWldPointPlan2Normal parm [edi] [eax] [ebx] modify exact [ebx edi] /* src/affiche.c:269 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** At dst+x+384*y write u16(+1)=0x0C00, u32(+384)=0x0C0E0A00, u32(+768)=0x000A0900, u16(+1153)=0. End EBX=384*y and EDI=start; preserve EAX and other GPRs.

**Valid test domain.** Mapped 384-byte-pitch surface; room for pattern 4 rows.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DrawWldPointPlan2Normal.json).

**Caller evidence.** src/affiche.c:680. Full lists and address registrations are in JSON.

## DrawBlackBoxNormal

`1:04aef`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, ECX height, EDX width, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:161:DrawBlackBoxNormal](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux DrawBlackBoxNormal parm [edi] [eax] [ebx] [ecx] [edx] modify exact [] /* src/affiche.c:260 */
#pragma aux DrawBlackBoxNormal parm [edi] [eax] [ebx] [ecx] [edx] modify [] /* src/box.c:53 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Fill width EDX by height ECX with zero at dst+EAX+320*EBX. Preserve GPRs. No clipping.

**Valid test domain.** DF=0; positive width/height; rectangle wholly mapped.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 5/5 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DrawBlackBoxNormal.json).

**Caller evidence.** src/affiche.c:985, src/box.c:352, src/box.c:401. Full lists and address registrations are in JSON.

## DrawFondBoxNormal

`1:04b2c`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX type, ESI x, EBX y, ECX height, EDX width, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:220:DrawFondBoxNormal](models.py). Preserved throughout the tested domain: EBP, EDX. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux DrawFondBoxNormal parm [edi] [esi] [ebx] [ecx] [edx] [eax] modify exact [eax ebx ecx esi edi] /* src/affiche.c:261 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** For type EAX!=2 traverse forward: read pixel at destination+321 and subtract 5 modulo 256. If source and result differ in high 3 bits, replace output by (source XOR result)&0xE0. For type 2 traverse backward from rectangle bottom-right, read destination-321 and add 5 modulo 256; on high-bit crossing replace by ((source XOR result)&0xE0)|31. Thus this is not ordinary saturation within the original 32-color bank. Destination pitch 320.

**Valid test domain.** Positive width/height; x=ESI, y=EBX; one-pixel diagonal source margin mapped. Type 2 reads upper-left, others lower-right.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 15/15 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DrawFondBoxNormal.json).

**Caller evidence.** src/affiche.c:983. Full lists and address registrations are in JSON.

## DrawBorderBoxNormal

`1:04bbb`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** AL/AH edge/shadow colors, ESI x, EBX y, ECX height, EDX width, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:209:DrawBorderBoxNormal](models.py). Preserved throughout the tested domain: EBP, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux DrawBorderBoxNormal parm [edi] [esi] [ebx] [ecx] [edx] [eax] modify exact [eax ebx ecx edx edi] /* src/affiche.c:262 */
#pragma aux DrawBorderBoxNormal parm [edi] [esi] [ebx] [ecx] [edx] [eax] modify [eax ebx ecx edx] /* src/box.c:55 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Draw beveled box with two colors AL/AH at dst+ESI+320*EBX. AL fills top row width and left column height. AH fills bottom row columns 1..width-1, an additional full row at y+height, and right columns width-1 (rows 1..height-1) and width (rows 0..height-1). This original shadow extends one row and column beyond requested dimensions. End EBX=height, ECX=0, EDX=width-1, EDI=start+height*320+width-1, EAX low 16 repeats original AH.

**Valid test domain.** DF=0; height>=2, width>=1; mapped rectangle including one-pixel shadow.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 5/5 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DrawBorderBoxNormal.json).

**Caller evidence.** src/affiche.c:991, src/affiche.c:996, src/box.c:342, src/box.c:353, src/box.c:386, src/box.c:402. Full lists and address registrations are in JSON.

## DisplayAnyPictureNormal

`1:04c19`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** ESI source, EDI destination, EAX source x, EBX source y, ECX dest x, EDX dest y; stack+4 stride, +8 width, +12 height (s32), RET 12. Entry[ESP+4/+8/+12] s32 source pitch, width, height; RET 12 advances ESP 16. No caller stack arguments remain.

**Register outputs.** Exact transformations: [models.py:174:DisplayAnyPictureNormal](models.py). Preserved throughout the tested domain: EBP. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux DisplayAnyPictureNormal parm [esi] [edi] [eax] [ebx] [ecx] [edx] modify [eax ebx ecx edx esi edi] /* src/include/lowlevel.h:16 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

callee-popped arguments (INVENTED).

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x4 | 4 | yes | source stride |
| 0x8 | 4 | yes | width |
| 0xc | 4 | yes | height |

**Computation.** Copy width by height raw bytes from source+(source_y*stride)+source_x to destination+dest_y*320+dest_x. Source pitch=stride, destination pitch 320. Callee pops 12 argument bytes. End EAX=ECX=0, EBX=320-width, EDX=stride-width; pointers advance by height*pitch.

**Valid test domain.** DF=0; positive dimensions, mapped disjoint buffers; stack+4/+8/+12 contain s32 stride, width, height.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 7/7 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DisplayAnyPictureNormal.json).

**Caller evidence.** src/cheat.c:477, src/cheat.c:705, src/jumelle.c:693. Full lists and address registrations are in JSON.

## ClearDrawAndDisplayBufferNormal

`1:04c6b`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EDI first buffer, ESI second buffer. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:130:ClearDrawAndDisplayBufferNormal](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux ClearDrawAndDisplayBufferNormal parm [edi] [esi] /* src/frame.c:184 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Zero 64000 bytes at original EDI, then at original ESI. End EDI=ESI+64000, preserving all other GPRs.

**Valid test domain.** DF=0; two disjoint mapped 64000-byte buffers.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/ClearDrawAndDisplayBufferNormal.json).

**Caller evidence.** src/frame.c:963. Full lists and address registrations are in JSON.

## N_CLRSCR

`1:04c8e`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EDI buffer. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:117:N_CLRSCR](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux N_CLRSCR parm [edi] modify [edi] /* src/include/lowlevel.h:20 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Zero exactly 64000 bytes at EDI (16000 dword stores). Advance EDI by 64000; preserve other GPRs.

**Valid test domain.** DF=0; mapped 64000-byte destination.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/N_CLRSCR.json).

**Caller evidence.** src/map_ngawe.c:162, src/menu.c:562, src/menu.c:768, src/menu.c:3339. Full lists and address registrations are in JSON.

## Swap_To_Screen

`1:04c9c`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX height, ECX byte width, ESI source, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:182:Swap_To_Screen](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux Swap_To_Screen parm [esi] [edi] [ecx] [eax] modify exact [] /* src/pcmain.c:382 */
```

**Globals.** Reads: code 1:04c86 source pitch; code 1:04c8a destination pitch. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Copy floor(ECX/4)*4 bytes on each of EAX rows, using source/destination pitches stored at code 1:04c86/04c8a (initially 320). Preserve all GPRs.

**Valid test domain.** DF=0; positive height; width 0..320; disjoint mapped source/destination.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 3/3 (100.0%). Current mismatches: 0. [Per-routine certificate](results/Swap_To_Screen.json).

**Caller evidence.** src/pcmain.c:510, src/pcmain.c:516. Full lists and address registrations are in JSON.

## clear_borders_Normal

`1:04cc7`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EDI first border, EDX byte separation, ECX row count. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:189:clear_borders_Normal](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux clear_borders_Normal parm [edi] [ecx] [edx] modify exact [] /* src/pcmain.c:384 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** For floor(ECX/2)*2 rows, zero four bytes at destination+row*320 and destination+row*320+EDX. Preserve GPRs; odd last row is not processed.

**Valid test domain.** ECX>=2; mapped border columns; no clipping.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 3/3 (100.0%). Current mismatches: 0. [Per-routine certificate](results/clear_borders_Normal.json).

**Caller evidence.** src/pcmain.c:558. Full lists and address registrations are in JSON.

## Copy_Plan0Diff_To_Buf

`1:04cfa`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** ESI source, EDI destination, ECX width, EBX height, EDX source pitch. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [copies.py:3:plan](copies.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** For positive ECX width/EBX height, copy ceil(width/8)*8 bytes per row from ESI with source pitchEDX to EDI with destination pitch 320. Skip nonpositive dimensions. Preserve GPRs.

**Valid test domain.** Mapped disjoint source/destination including 7 padding bytes after logical width.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 9/9 (100.0%). Current mismatches: 0. [Per-routine certificate](results/Copy_Plan0Diff_To_Buf.json).

**Caller evidence.** src/skops.c:276, src/skops.c:335, src/skops.c:352. Full lists and address registrations are in JSON.

## asm_04d55

`1:04d55`; public; name **INVENTED**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** ESI source, EDI destination, ECX width, EBX height, EDX source pitch. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [copies.py:14:blocks](copies.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Copy positive rectangle in 8x8 groups, width rounded up to 8 bytes, source pitchEDX, destination pitch 320. Ignore nonpositive width/height; positive height MUST be a multiple 8. Preserve GPRs.

**Valid test domain.** Height positive multiple 8, or nonpositive; source/destination mapped including rounded width.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 9/9 (100.0%). Current mismatches: 0. [Per-routine certificate](results/asm_04d55.json).

## Display_Bloc

`1:04e64`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** ESI normal tile, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [tiles.py:9:masked](tiles.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux Display_Bloc parm [esi] [edi] modify exact [] /* src/include/lowlevel.h:37 */
```

**Globals.** Reads: code 1:04c86 destination pitch. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

normal tile (INVENTED descriptive names; map caller uses MAP_BLOCKS) — size 516.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 256 | no | 16x16 u8 pixels, pitch 16 |
| 0x100 | 256 | no | per-pixel AND masks, pitch 16 |
| 0x200 | 4 | no | 2 bits per row, row 0 in bits 31..30; bit 30 skips, otherwise bit 31 copies opaque, otherwise AND/OR |

**Computation.** Render masked 16x16 tile using two-bit row flags. For each row, low flag bit skips; otherwise high flag bit copies 16 opaque bytes; otherwise destination=(destination&mask)|pixel. Source pixels at+0, AND masks+256, flags+512. Destination pitch read from code 1:04c86. Preserve GPRs.

**Valid test domain.** Mapped 516-byte source and 16-row destination; disjoint.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 20/20 (100.0%). Current mismatches: 0. [Per-routine certificate](results/Display_Bloc.json).

**Caller evidence.** src/map_ngawe.c:122. Full lists and address registrations are in JSON.

## Display_Bloc_Plein

`1:05041`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** ESI normal tile, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [tiles.py:17:opaque](tiles.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux Display_Bloc_Plein parm [esi] [edi] modify exact [] /* src/include/lowlevel.h:39 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

normal tile (INVENTED descriptive names; map caller uses MAP_BLOCKS) — size 516.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 256 | no | 16x16 u8 pixels, pitch 16 |
| 0x100 | 256 | no | per-pixel AND masks, pitch 16 |
| 0x200 | 4 | no | 2 bits per row, row 0 in bits 31..30; bit 30 skips, otherwise bit 31 copies opaque, otherwise AND/OR |

**Computation.** Copy opaque 16x16 tile from source pitch 16 to destination pitch 320. Preserve GPRs. Unlike masked variant this entry uses literal 320 rather than mutable stride.

**Valid test domain.** Mapped 256-byte source and 16-row destination; disjoint.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/Display_Bloc_Plein.json).

**Caller evidence.** src/map_ngawe.c:120. Full lists and address registrations are in JSON.

## loc_052DA

`1:052da`; private; name **INVENTED**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT_CONTRIBUTION (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, ECX=0x1010, ESI pixels, EDI destination; stack+4 row flags. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked. Caller-owned stack+4 flags/mask is updated as described.

**Register outputs.** Exact transformations: [private.py:13:tileclip](private.py). Preserved throughout the tested domain: EBP. CF is a specified accept/reject output. ESP returns as specified above.

**Globals.** Reads: Bloc_lim_W1/W2/H1/H2; code 1:04c86 destination pitch. Writes: code 1:052d6 original width; caller stack+4 row flags.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Private normal-tile clipping helper. CL must be 16, CH height 16. Save original width at code 1:052d6. Left trim rounds down to 4, advances ESI/EDI; right kept width rounds up to 4; output CL counts dwords. Top trim advances ESI by original width*rows and EDI by mutable destination pitch*rows; shift caller row flags at stack+4 left by 2 per skipped row. Update x/y/dimensions; CF=1 rejects.

**Valid test domain.** Called with ECX=0x1010, valid Bloc_lim ordered viewport; mapped source/destination; writable flags dword at caller stack+4.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 19/19 (100.0%). Current mismatches: 0. [Per-routine certificate](results/loc_052DA.json).

## Display_Bloc_Clippe

`1:053c6`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** ESI normal tile, EDI destination, EAX x, EBX y. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [tiles.py:54:fn](tiles.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux Display_Bloc_Clippe parm [esi] [edi] [eax] [ebx] modify exact [] /* src/include/lowlevel.h:41 */
```

**Globals.** Reads: Bloc_lim_W1; Bloc_lim_W2; Bloc_lim_H1; Bloc_lim_H2; code 1:04c86. Writes: code 1:052d6 original width.

**Layouts.**

normal tile (INVENTED descriptive names; map caller uses MAP_BLOCKS) — size 516.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 256 | no | 16x16 u8 pixels, pitch 16 |
| 0x100 | 256 | no | per-pixel AND masks, pitch 16 |
| 0x200 | 4 | no | 2 bits per row, row 0 in bits 31..30; bit 30 skips, otherwise bit 31 copies opaque, otherwise AND/OR |

**Computation.** Render masked 16x16 tile at provided destination pointer, clipped using EAX/EBX position and Bloc_lim_* bounds. Horizontal crop is dword aligned: left trim rounds down to 4 bytes, right kept width rounds up to 4. Top crop advances source by 16*rows and destination by pitch*rows; row flags shift by 2 per skipped row. Destination pitch from code 1:04c86. Preserve all GPRs.

**Valid test domain.** Ordered clip with dword-aligned game tile boundaries; mapped tile and destination including rounded fringe.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 29/29 (100.0%). Current mismatches: 0. [Per-routine certificate](results/Display_Bloc_Clippe.json).

**Caller evidence.** src/map_ngawe.c:97, src/map_ngawe.c:113, src/map_ngawe.c:131, src/map_ngawe.c:145. Full lists and address registrations are in JSON.

## Display_Bloc_Plein_Clippe

`1:0555e`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** ESI normal tile, EDI destination, EAX x, EBX y. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [tiles.py:54:fn](tiles.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux Display_Bloc_Plein_Clippe parm [esi] [edi] [eax] [ebx] modify exact [] /* src/include/lowlevel.h:43 */
```

**Globals.** Reads: Bloc_lim_W1; Bloc_lim_W2; Bloc_lim_H1; Bloc_lim_H2; code 1:04c86. Writes: code 1:052d6 original width.

**Layouts.**

normal tile (INVENTED descriptive names; map caller uses MAP_BLOCKS) — size 516.

| Offset | Width | Signed | Meaning |
|---:|---:|---|---|
| 0x0 | 256 | no | 16x16 u8 pixels, pitch 16 |
| 0x100 | 256 | no | per-pixel AND masks, pitch 16 |
| 0x200 | 4 | no | 2 bits per row, row 0 in bits 31..30; bit 30 skips, otherwise bit 31 copies opaque, otherwise AND/OR |

**Computation.** Render opaque 16x16 tile at provided destination pointer, clipped using EAX/EBX position and Bloc_lim_* bounds. Horizontal crop is dword aligned: left trim rounds down to 4 bytes, right kept width rounds up to 4. Top crop advances source by 16*rows and destination by pitch*rows; row flags shift by 2 per skipped row. Destination pitch from code 1:04c86. Preserve all GPRs.

**Valid test domain.** Ordered clip with dword-aligned game tile boundaries; mapped tile and destination including rounded fringe.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 13/13 (100.0%). Current mismatches: 0. [Per-routine certificate](results/Display_Bloc_Plein_Clippe.json).

**Caller evidence.** src/map_ngawe.c:95, src/map_ngawe.c:111, src/map_ngawe.c:129, src/map_ngawe.c:143. Full lists and address registrations are in JSON.

## fplot_Normal

`1:05609`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, ESI color, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:201:fplot_Normal](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux fplot_func_t parm [edi] [eax] [ebx] [esi] modify exact [eax ebx] /* src/action_local.h:18 */
```

**Globals.** Reads: row_offsets_320_01b00. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Write low byte of ESI at dst+EAX+row_offsets_320[EBX]. Preserve all GPRs. No clipping.

**Valid test domain.** Mapped pixel; x0..319, y0..199.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/fplot_Normal.json).

**Caller evidence.** src/affiche.c:961. Full lists and address registrations are in JSON.

## flocon_clipping

`1:05628`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX top, EBX bottom, ECX left, EDX right. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:196:flocon_clipping](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux flocon_clipping parm [eax] [ebx] [ecx] [edx] modify exact [] /* src/pcmain.c:386 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: code 1:05618..05627.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Store top=EAX, bottom=EBX, left=ECX, right=EDX as signed 32 weather clipping bounds. Preserve all GPRs.

**Valid test domain.** No sorting/clamping; valid drawing windows generated by callers.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/flocon_clipping.json).

**Caller evidence.** src/pcmain.c:541. Full lists and address registrations are in JSON.

## draw_flocon5_Normal

`1:05640`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:38:fn](weather.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: clip_top/bottom/left/right at code 1:05618..05627; row_offsets_320_01b00. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Weather stamp rows ['5'] (dot leaves pixel untouched, digits are palette indices), at EDI+EAX+320*EBX. Test anchor against inclusive weather bounds left<=x<=right, top<=y<=bottom; the stamp extent is not separately clipped. Preserve GPRs.

**Valid test domain.** C weather anchor coordinates and configured clip box; mapped pattern footprint including edge fringe.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/draw_flocon5_Normal.json).

## draw_flocon1_Normal

`1:05670`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:38:fn](weather.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: clip_top/bottom/left/right at code 1:05618..05627; row_offsets_320_01b00. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Weather stamp rows ['7'] (dot leaves pixel untouched, digits are palette indices), at EDI+EAX+320*EBX. Test anchor against inclusive weather bounds left<=x<=right, top<=y<=bottom; the stamp extent is not separately clipped. Preserve GPRs.

**Valid test domain.** C weather anchor coordinates and configured clip box; mapped pattern footprint including edge fringe.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/draw_flocon1_Normal.json).

## draw_flocon2_Normal

`1:056a0`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:38:fn](weather.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: clip_top/bottom/left/right at code 1:05618..05627; row_offsets_320_01b00. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Weather stamp rows ['78', '67'] (dot leaves pixel untouched, digits are palette indices), at EDI+EAX+320*EBX. Test anchor against inclusive weather bounds left<=x<=right, top<=y<=bottom; the stamp extent is not separately clipped. Preserve GPRs.

**Valid test domain.** C weather anchor coordinates and configured clip box; mapped pattern footprint including edge fringe.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/draw_flocon2_Normal.json).

## draw_flocon3_Normal

`1:056dc`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:38:fn](weather.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: clip_top/bottom/left/right at code 1:05618..05627; row_offsets_320_01b00. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Weather stamp rows ['.6.', '866', '.6.'] (dot leaves pixel untouched, digits are palette indices), at EDI+EAX+320*EBX. Test anchor against inclusive weather bounds left<=x<=right, top<=y<=bottom; the stamp extent is not separately clipped. Preserve GPRs.

**Valid test domain.** C weather anchor coordinates and configured clip box; mapped pattern footprint including edge fringe.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/draw_flocon3_Normal.json).

## draw_flocon4_Normal

`1:05725`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:38:fn](weather.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: clip_top/bottom/left/right at code 1:05618..05627; row_offsets_320_01b00. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Weather stamp rows ['3'] (dot leaves pixel untouched, digits are palette indices), at EDI+EAX+320*EBX. Test anchor against inclusive weather bounds left<=x<=right, top<=y<=bottom; the stamp extent is not separately clipped. Preserve GPRs.

**Valid test domain.** C weather anchor coordinates and configured clip box; mapped pattern footprint including edge fringe.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/draw_flocon4_Normal.json).

## draw_normal_5755

`1:05755`; public; name **INVENTED**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:38:fn](weather.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: clip_top/bottom/left/right at code 1:05618..05627; row_offsets_320_01b00. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Weather stamp rows ['.55.', '5775', '5775', '.55.'] (dot leaves pixel untouched, digits are palette indices), at EDI+EAX+320*EBX. Test anchor against inclusive weather bounds left<=x<=right, top<=y<=bottom; the stamp extent is not separately clipped. Preserve GPRs.

**Valid test domain.** C weather anchor coordinates and configured clip box; mapped pattern footprint including edge fringe.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/draw_normal_5755.json).

## draw_pluie5_Normal

`1:057a6`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:38:fn](weather.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: clip_top/bottom/left/right at code 1:05618..05627; row_offsets_320_01b00. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Weather stamp rows ['.4', '5.'] (dot leaves pixel untouched, digits are palette indices), at EDI+EAX+320*EBX. Test anchor against inclusive weather bounds left<=x<=right, top<=y<=bottom; the stamp extent is not separately clipped. Preserve GPRs.

**Valid test domain.** C weather anchor coordinates and configured clip box; mapped pattern footprint including edge fringe.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/draw_pluie5_Normal.json).

## draw_flocon7_Normal

`1:057df`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:38:fn](weather.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: clip_top/bottom/left/right at code 1:05618..05627; row_offsets_320_01b00. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Weather stamp rows ['.66.', '6886', '6886', '.66.'] (dot leaves pixel untouched, digits are palette indices), at EDI+EAX+320*EBX. Test anchor against inclusive weather bounds left<=x<=right, top<=y<=bottom; the stamp extent is not separately clipped. Preserve GPRs.

**Valid test domain.** C weather anchor coordinates and configured clip box; mapped pattern footprint including edge fringe.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/draw_flocon7_Normal.json).

**Caller evidence.** src/affiche.c:924. Full lists and address registrations are in JSON.

## draw_pluie6_Normal

`1:05830`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:38:fn](weather.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: clip_top/bottom/left/right at code 1:05618..05627; row_offsets_320_01b00. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Weather stamp rows ['.42', '532', '36.'] (dot leaves pixel untouched, digits are palette indices), at EDI+EAX+320*EBX. Test anchor against inclusive weather bounds left<=x<=right, top<=y<=bottom; the stamp extent is not separately clipped. Preserve GPRs.

**Valid test domain.** C weather anchor coordinates and configured clip box; mapped pattern footprint including edge fringe.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/draw_pluie6_Normal.json).

## draw_flocon6_Normal

`1:0587d`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:38:fn](weather.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: clip_top/bottom/left/right at code 1:05618..05627; row_offsets_320_01b00. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Weather stamp rows ['..66..', '.7887.', '688886', '688886', '.7887.', '..66..'] (dot leaves pixel untouched, digits are palette indices), at EDI+EAX+320*EBX. Test anchor against inclusive weather bounds left<=x<=right, top<=y<=bottom; the stamp extent is not separately clipped. Preserve GPRs.

**Valid test domain.** C weather anchor coordinates and configured clip box; mapped pattern footprint including edge fringe.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/draw_flocon6_Normal.json).

**Caller evidence.** src/affiche.c:918. Full lists and address registrations are in JSON.

## draw_pluie7_Normal

`1:058f4`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [weather.py:38:fn](weather.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: clip_top/bottom/left/right at code 1:05618..05627; row_offsets_320_01b00. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Weather stamp rows ['..32', '.324', '543.', '46..'] (dot leaves pixel untouched, digits are palette indices), at EDI+EAX+320*EBX. Test anchor against inclusive weather bounds left<=x<=right, top<=y<=bottom; the stamp extent is not separately clipped. Preserve GPRs.

**Valid test domain.** C weather anchor coordinates and configured clip box; mapped pattern footprint including edge fringe.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/draw_pluie7_Normal.json).

**Caller evidence.** src/affiche.c:931. Full lists and address registrations are in JSON.

## get_joy_input2_dos1

`1:0595d`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [joystick.py:61:fn](joystick.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: During_The_Menu; xpadcentre; ypadcentre; timer state. Writes: joyx 0; joyy 0; joy_buttonA1/A2/B1/B2; code 1:05959 timer start; timer state.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Read active-low buttons at port 0x201 bits 4..7, mapping toA 1, A2, B1, B2. If During_The_Menu==1 leave axes unchanged. Otherwise zero axes and time bit-clear events using clock_ticks relative to initial timestamp. Record elapsed low 16 ticks only when its bit and previous axis value are both zero. Selected axis bits are 0/1. Initial selected bits nonzero or 2000-poll timeout falls back to calibrated x/y centers. OUT 0x201=255 triggers sampling. Preserve all GPRs; finish IF=1, DF=0.

**Valid test domain.** Mapped game joystick globals; scripted port response with monotonic discharges or timeout; live PIT response abstracted.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:0595e cli `
- `1:05966 in al, dx`
- `1:059ea out dx, al`
- `1:059eb in al, dx`
- `1:05a2e sti `

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 12/12 (100.0%). Current mismatches: 0. [Per-routine certificate](results/get_joy_input2_dos1.json).

**Caller evidence.** src/input.c:386, src/input.c:391, src/input.c:406. Full lists and address registrations are in JSON.

## get_joy_input2_dos2

`1:05a57`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [joystick.py:61:fn](joystick.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: During_The_Menu; xpadcentre; ypadcentre; timer state. Writes: joyx 0; joyy 0; joy_buttonA1/A2/B1/B2; code 1:05959 timer start; timer state.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Read active-low buttons at port 0x201 bits 4..7, mapping toB 1, B2, A1, A2. If During_The_Menu==1 leave axes unchanged. Otherwise zero axes and time bit-clear events using clock_ticks relative to initial timestamp. Record elapsed low 16 ticks only when its bit and previous axis value are both zero. Selected axis bits are 2/3. Initial selected bits nonzero or 2000-poll timeout falls back to calibrated x/y centers. OUT 0x201=255 triggers sampling. Preserve all GPRs; finish IF=1, DF=0.

**Valid test domain.** Mapped game joystick globals; scripted port response with monotonic discharges or timeout; live PIT response abstracted.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:05a2e sti `
- `1:05a58 cli `
- `1:05a60 in al, dx`
- `1:05ae8 out dx, al`
- `1:05ae9 in al, dx`
- `1:05b2c sti `

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 13/13 (100.0%). Current mismatches: 0. [Per-routine certificate](results/get_joy_input2_dos2.json).

**Caller evidence.** src/input.c:386, src/input.c:391, src/input.c:406. Full lists and address registrations are in JSON.

## get_joy_input2_windows1

`1:05b55`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [joystick.py:61:fn](joystick.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: During_The_Menu; xpadcentre; ypadcentre. Writes: joyx 0; joyy 0; joy_buttonA1/A2/B1/B2.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Read active-low buttons at port 0x201 bits 4..7, mapping toA 1, A2, B1, B2. Zero both axes first. Count poll iterations Only increment each axis while its own bit is set. Selected axis bits are 0/1. Initial selected bits nonzero or 2000-poll timeout falls back to calibrated x/y centers. OUT 0x201=255 triggers sampling. Preserve all GPRs; finish IF=1, DF=0.

**Valid test domain.** Mapped game joystick globals; scripted port response with monotonic discharges or timeout; live PIT response abstracted.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:05b56 cli `
- `1:05b70 in al, dx`
- `1:05bcc out dx, al`
- `1:05bcd in al, dx`
- `1:05c10 sti `

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 11/11 (100.0%). Current mismatches: 0. [Per-routine certificate](results/get_joy_input2_windows1.json).

**Caller evidence.** src/input.c:386, src/input.c:391, src/input.c:406. Full lists and address registrations are in JSON.

## get_joy_input2_windows2

`1:05c13`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [joystick.py:61:fn](joystick.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: During_The_Menu; xpadcentre; ypadcentre. Writes: joyx 0; joyy 0; joy_buttonA1/A2/B1/B2.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Read active-low buttons at port 0x201 bits 4..7, mapping toB 1, B2, A1, A2. Zero both axes first. Count poll iterations Axis update test is (axis_bit+current_u16_axis)!=0, so Windows pair 2 continues incrementing a nonzero counter even after its bit clears. Selected axis bits are 2/3. Initial selected bits nonzero or 2000-poll timeout falls back to calibrated x/y centers. OUT 0x201=255 triggers sampling. Preserve all GPRs; finish IF=1, DF=0.

**Valid test domain.** Mapped game joystick globals; scripted port response with monotonic discharges or timeout; live PIT response abstracted.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:05c14 cli `
- `1:05c2e in al, dx`
- `1:05c8a out dx, al`
- `1:05c8b in al, dx`
- `1:05cdc sti `

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 11/11 (100.0%). Current mismatches: 0. [Per-routine certificate](results/get_joy_input2_windows2.json).

**Caller evidence.** src/input.c:386, src/input.c:391, src/input.c:406. Full lists and address registrations are in JSON.

## dos_game_io_port_something

`1:05cdf`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [joystick.py:23:fn](joystick.py). Preserved throughout the tested domain: EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Trigger game-port discharge with OUT 0x201=255; poll IN 0x201 until axis mask 3 clears. Return EAX=1 only if clearance takes 2..2000 polls, otherwise 0. Preserve other GPRs; finish IF=1, DF=0.

**Valid test domain.** Finite scripted RC response, including immediate-clear and 2000-poll timeout.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:05ce0 cli `
- `1:05cf0 out dx, al`
- `1:05cf1 in al, dx`
- `1:05d12 sti `

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 7/7 (100.0%). Current mismatches: 0. [Per-routine certificate](results/dos_game_io_port_something.json).

**Caller evidence.** src/input.c:681, src/input.c:691. Full lists and address registrations are in JSON.

## dos_game_io_port_something2

`1:05d14`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [joystick.py:23:fn](joystick.py). Preserved throughout the tested domain: EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Trigger game-port discharge with OUT 0x201=255; discard one initial read; poll IN 0x201 until axis mask 12 clears. Return EAX=1 only if clearance takes 2..2000 polls, otherwise 0. Preserve other GPRs; finish IF=1, DF=0.

**Valid test domain.** Finite scripted RC response, including immediate-clear and 2000-poll timeout.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:05d15 cli `
- `1:05d25 out dx, al`
- `1:05d26 in al, dx`
- `1:05d27 in al, dx`
- `1:05d48 sti `

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 7/7 (100.0%). Current mismatches: 0. [Per-routine certificate](results/dos_game_io_port_something2.json).

**Caller evidence.** src/input.c:682. Full lists and address registrations are in JSON.

## get_joy_input1_dos1

`1:05d4a`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX height, ECX byte width, ESI source, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [joystick.py:107:fn](joystick.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: code 1:05951 source pitch; code 1:05955 destination pitch; xpadcentre; ypadcentre; timer state. Writes: joy axes/buttons; code 1:05959 timer start; timer state.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Copy EAX rows of floor(ECX/4)*4 bytes from ESI to EDI using code-owned source/destination pitches at 1:05951/05955 (initially 320). Zero axes and read buttons in A1, A2, B1, B2 order in BOTH variants. Sample port 0x201 axis bits 0/1 before each copied row until both bits clear, then finish copy without further polling. If rows end first, poll up to 2000 further reads; timeout or invalid initial selected bits uses calibrated centers. Timing uses clock_ticks relative to initial latch; record each axis when bit and previous result are zero. Trigger with OUT 0x201=255. Preserve GPRs, including input EAX height; final IF=1, DF=0. Exact interleaving and timer state are part of contract.

**Valid test domain.** Positive frame height and mapped source/destination; width 0..320; scripted monotonic device response and PIT counter.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:05d4b cli `
- `1:05d6c in al, dx`
- `1:05ddf out dx, al`
- `1:05df4 in al, dx`
- `1:05e4b in al, dx`
- `1:05ed9 sti `

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 21/21 (100.0%). Current mismatches: 0. [Per-routine certificate](results/get_joy_input1_dos1.json).

**Caller evidence.** src/frame.c:279, src/pcmain.c:507, src/pcmain.c:514. Full lists and address registrations are in JSON.

## get_joy_input1_dos2

`1:05f34`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX height, ECX byte width, ESI source, EDI destination. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [joystick.py:107:fn](joystick.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: code 1:05951 source pitch; code 1:05955 destination pitch; xpadcentre; ypadcentre; timer state. Writes: joy axes/buttons; code 1:05959 timer start; timer state.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Copy EAX rows of floor(ECX/4)*4 bytes from ESI to EDI using code-owned source/destination pitches at 1:05951/05955 (initially 320). Zero axes and read buttons in A1, A2, B1, B2 order in BOTH variants. Sample port 0x201 axis bits 2/3 before each copied row until both bits clear, then finish copy without further polling. If rows end first, poll up to 2000 further reads; timeout or invalid initial selected bits uses calibrated centers. Timing uses clock_ticks relative to initial latch; record each axis when bit and previous result are zero. Trigger with OUT 0x201=255. Preserve GPRs, including input EAX height; final IF=1, DF=0. Exact interleaving and timer state are part of contract.

**Valid test domain.** Positive frame height and mapped source/destination; width 0..320; scripted monotonic device response and PIT counter.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:05f35 cli `
- `1:05f56 in al, dx`
- `1:05fc9 out dx, al`
- `1:05fde in al, dx`
- `1:06035 in al, dx`
- `1:060c3 sti `

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 21/21 (100.0%). Current mismatches: 0. [Per-routine certificate](results/get_joy_input1_dos2.json).

**Caller evidence.** src/frame.c:279, src/pcmain.c:507, src/pcmain.c:514. Full lists and address registrations are in JSON.

## DetectCpu

`1:0611e`; public; name **RAYVERSE/ANDROID**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [cpu.py:3:detect](cpu.py). Preserved throughout the tested domain: EBP, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux DetectCpu parm [] value [eax] modify exact [eax ebx ecx] /* src/pcmain.c:429 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Probe CPU features by attempting EFLAGS.AC then EFLAGS.ID changes. If AC cannot be set return 3; if AC succeeds but ID cannot toggle return 4; otherwise execute CPUID leaf 1 and return base family bits 8..11 of EAX (no extended-family combination). CPUID at 1:06167 is abstracted; supported AC/ID writes at 1:06134/0614d are explicit processor-capability stubs. Preserve EDX/ESI/EDI/EBP, restore low 16 EBX only; high 16 EBX is zero on legacy paths or CPUID EBX high 16 on modern path. ECX becomes CPUID ECX only on modern path. Restore EFLAGS and SP alignment.

**Valid test domain.** 32-bit flat caller stack, initial EFLAGS=2 in certificate; three CPU capability profiles (386/486/CPUID) and randomized CPUID leaf 1 outputs. Original function temporarily aligns low 16 SP to 4.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:06120 pushf `
- `1:0612a pushfd `
- `1:0612b pushfd `
- `1:06134 popfd `
- `1:06135 pushfd `
- `1:06137 popfd `
- `1:06143 pushfd `
- `1:0614d popfd `
- `1:0614e pushfd `
- `1:06151 popfd `
- `1:06167 cpuid `
- `1:0617f popf `

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DetectCpu.json).

**Caller evidence.** src/pcmain.c:1013. Full lists and address registrations are in JSON.

## default_sprite_clipping

`1:06194`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:51:default_sprite_clipping](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: XMIN; XMAX; YMIN; YMAX.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Store half-open sprite clip box [8, 312) x [0, 200). Preserve GPRs.

**Valid test domain.** No arguments.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/default_sprite_clipping.json).

**Caller evidence.** src/menu.c:565, src/pcmain.c:1067, src/pcmain.c:1103. Full lists and address registrations are in JSON.

## get_sprite_clipping

`1:061bd`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX/EBX/ECX/EDX writable s32 pointers to xmin/xmax/ymin/ymax. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:56:get_sprite_clipping](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux get_sprite_clipping parm [eax] [ebx] [ecx] [edx] modify exact [] /* src/affiche.c:341 */
#pragma aux get_sprite_clipping parm [eax] [ebx] [ecx] [edx] modify exact [] /* src/include/lowlevel.h:24 */
```

**Globals.** Reads: XMIN; XMAX; YMIN; YMAX. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Copy current xmin, xmax, ymin, ymax to the four s32 pointers EAX, EBX, ECX, EDX in that order. Preserve GPRs. Aliasing obeys the sequential source reads.

**Valid test domain.** Four mapped writable four-byte pointers.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/get_sprite_clipping.json).

**Caller evidence.** src/affiche.c:524, src/affiche.c:535, src/affiche.c:777, src/display.c:798, src/effect 1.c:467. Full lists and address registrations are in JSON.

## get_sprite_clipping_xmin

`1:061e2`; private; name **INVENTED**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT_CONTRIBUTION (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [private.py:7:fn](private.py). Preserved throughout the tested domain: EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

**Globals.** Reads: XMIN. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Return current xmin signed 32 clip bound in EAX; preserve other GPRs. Private unreferenced semantic entry.

**Valid test domain.** No arguments.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/get_sprite_clipping_xmin.json).

## get_sprite_clipping_xmax

`1:061e8`; private; name **INVENTED**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT_CONTRIBUTION (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [private.py:7:fn](private.py). Preserved throughout the tested domain: EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

**Globals.** Reads: XMAX. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Return current xmax signed 32 clip bound in EAX; preserve other GPRs. Private unreferenced semantic entry.

**Valid test domain.** No arguments.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/get_sprite_clipping_xmax.json).

## get_sprite_clipping_ymin

`1:061ee`; private; name **INVENTED**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT_CONTRIBUTION (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [private.py:7:fn](private.py). Preserved throughout the tested domain: EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

**Globals.** Reads: YMIN. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Return current ymin signed 32 clip bound in EAX; preserve other GPRs. Private unreferenced semantic entry.

**Valid test domain.** No arguments.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/get_sprite_clipping_ymin.json).

## get_sprite_clipping_ymax

`1:061f4`; private; name **INVENTED**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT_CONTRIBUTION (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [private.py:7:fn](private.py). Preserved throughout the tested domain: EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

**Globals.** Reads: YMAX. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Return current ymax signed 32 clip bound in EAX; preserve other GPRs. Private unreferenced semantic entry.

**Valid test domain.** No arguments.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/get_sprite_clipping_ymax.json).

## sprite_clipping

`1:061fa`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX xmin, EBX xmax, ECX ymin, EDX ymax. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:45:sprite_clipping](models.py). Preserved throughout the tested domain: EBP, EDI, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux sprite_clipping parm [eax] [ebx] [ecx] [edx] modify exact [eax ebx ecx edx] /* src/affiche.c:350 */
#pragma aux sprite_clipping parm [eax] [ebx] [ecx] [edx] modify exact [eax ebx ecx edx] /* src/pcmain.c:336 */
#pragma aux sprite_clipping parm [eax] [ebx] [ecx] [edx] modify exact [eax ebx ecx edx] /* src/include/lowlevel.h:10 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: XMIN; XMAX; YMIN; YMAX.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Clamp each signed input independently: xmin/xmax to [0, 320], ymin/ymax to [0, 200]. Store four signed 32-bit bounds. Does not sort or repair inverted bounds. Returns clamped values in the input registers.

**Valid test domain.** All signed 32-bit inputs.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 17/17 (100.0%). Current mismatches: 0. [Per-routine certificate](results/sprite_clipping.json).

**Caller evidence.** src/affiche.c:525, src/affiche.c:527, src/affiche.c:536, src/affiche.c:538, src/affiche.c:778, src/affiche.c:780, src/display.c:587, src/display.c:799. Full lists and address registrations are in JSON.

## clip_sprite_on_screen

`1:0626d`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, CL byte width, CH byte height, ESI source (EDI unchanged). Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [sprites.py:56:clip_normal](sprites.py). Preserved throughout the tested domain: EBP, EDI. CF is a specified accept/reject output. ESP returns as specified above.

**Globals.** Reads: XMIN; XMAX; YMIN; YMAX. Writes: saved_sprite_width.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Intersect packed CL width/CH height sprite with half-open clip bounds. Save original width, crop source left/top, update x/y and dimensions; CF=1 rejects. Exact output registers and failed-clip partial mutations are modeled.

**Valid test domain.** 1..255 width/height, ordered screen clip; mapped row-major source.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 14/14 (100.0%). Current mismatches: 0. [Per-routine certificate](results/clip_sprite_on_screen.json).

## clip_sprite_on_screen_flipped

`1:06323`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, CL byte width, CH byte height, ESI source (EDI unchanged). Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [sprites.py:58:clip_flip](sprites.py). Preserved throughout the tested domain: EBP, EDI. CF is a specified accept/reject output. ESP returns as specified above.

**Globals.** Reads: XMIN; XMAX; YMIN; YMAX. Writes: saved_sprite_width.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Clip horizontally mirrored sprite. Left cropping trims width without advancing source; right cropping advances source by original_width-new_width. Top crop advances source by original_width*rows. CF=1 rejects.

**Valid test domain.** Same as clip_sprite_on_screen.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 14/14 (100.0%). Current mismatches: 0. [Per-routine certificate](results/clip_sprite_on_screen_flipped.json).

## DrawSpriteFlipNoClipX

`1:063e6`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, CL byte width, CH byte height, EDI destination, ESI source; EDX compatibility argument ignored. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [sprites.py:174:fn](sprites.py). Preserved throughout the tested domain: EBP. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux draw_func_t parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi] /* src/action_local.h:15 */
```

**Globals.** Reads: row_offsets_80_01e20; sprite clip box. Writes: saved_sprite_width; code 1:063de/063e2.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Draw packed sprite through four VGA planes. Pixel values<0xA0 draw; all bytes>=0xA0 are individual transparent pixels (no run skipping). No clipping. For phase p=0..3, copy ceil((width-p)/4) samples with source column step+4, original source row pitch and 80-byte aperture pitch. Select initial plane 1<<(x&3) using word OUT 0x3C4, then remaining planes via byte OUT 0x3C5; wrap increments aperture byte. Preserve EBP. Original no-clip entry names are counterintuitive: FlipNoClipX is forward, NoClipX mirrors. Exact clobbers, scratch values and plane bytes are modeled.

**Valid test domain.** Positive byte width/height; mapped VGA footprint and packed sprite; ordered clip window.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:06453 out dx, ax`
- `1:064a2 out dx, al`
- `1:064ed out dx, al`
- `1:06534 out dx, al`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 39/39 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DrawSpriteFlipNoClipX.json).

## DrawSpriteX

`1:063f2`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, CL byte width, CH byte height, EDI destination, ESI source; EDX compatibility argument ignored. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [sprites.py:174:fn](sprites.py). Preserved throughout the tested domain: EBP. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux draw_func_t parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi] /* src/action_local.h:15 */
```

**Globals.** Reads: row_offsets_80_01e20; sprite clip box. Writes: saved_sprite_width; code 1:063de/063e2.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Draw packed sprite through four VGA planes. Pixel values<0xA0 draw; all bytes>=0xA0 are individual transparent pixels (no run skipping). Clip with shared sprite helper. For phase p=0..3, copy ceil((width-p)/4) samples with source column step+4, original source row pitch and 80-byte aperture pitch. Select initial plane 1<<(x&3) using word OUT 0x3C4, then remaining planes via byte OUT 0x3C5; wrap increments aperture byte. Preserve EBP. Original no-clip entry names are counterintuitive: FlipNoClipX is forward, NoClipX mirrors. Exact clobbers, scratch values and plane bytes are modeled.

**Valid test domain.** Positive byte width/height; mapped VGA footprint and packed sprite; ordered clip window.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:06453 out dx, ax`
- `1:064a2 out dx, al`
- `1:064ed out dx, al`
- `1:06534 out dx, al`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 40/40 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DrawSpriteX.json).

**Caller evidence.** src/affiche.c:743, src/affiche.c:1223, src/affiche.c:1485. Full lists and address registrations are in JSON.

## DrawSpriteNoClipX

`1:06559`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, CL byte width, CH byte height, EDI destination, ESI source; EDX compatibility argument ignored. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [sprites.py:174:fn](sprites.py). Preserved throughout the tested domain: EBP. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux draw_func_t parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi] /* src/action_local.h:15 */
```

**Globals.** Reads: row_offsets_80_01e20; sprite clip box. Writes: saved_sprite_width; code 1:063de/063e2.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Draw mirrored packed sprite through four VGA planes. Pixel values<0xA0 draw; all bytes>=0xA0 are individual transparent pixels (no run skipping). No clipping. For phase p=0..3, copy ceil((width-p)/4) samples with source column step-4, original source row pitch and 80-byte aperture pitch. Select initial plane 1<<(x&3) using word OUT 0x3C4, then remaining planes via byte OUT 0x3C5; wrap increments aperture byte. Preserve EBP. Original no-clip entry names are counterintuitive: FlipNoClipX is forward, NoClipX mirrors. Exact clobbers, scratch values and plane bytes are modeled.

**Valid test domain.** Positive byte width/height; mapped VGA footprint and packed sprite; ordered clip window.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:065cb out dx, ax`
- `1:0661c out dx, al`
- `1:06669 out dx, al`
- `1:066b2 out dx, al`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 39/39 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DrawSpriteNoClipX.json).

## DrawSpriteFlipX

`1:06565`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, CL byte width, CH byte height, EDI destination, ESI source; EDX compatibility argument ignored. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [sprites.py:174:fn](sprites.py). Preserved throughout the tested domain: EBP. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux draw_func_t parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi] /* src/action_local.h:15 */
```

**Globals.** Reads: row_offsets_80_01e20; sprite clip box. Writes: saved_sprite_width; code 1:063de/063e2.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Draw mirrored packed sprite through four VGA planes. Pixel values<0xA0 draw; all bytes>=0xA0 are individual transparent pixels (no run skipping). Clip with shared sprite helper. For phase p=0..3, copy ceil((width-p)/4) samples with source column step-4, original source row pitch and 80-byte aperture pitch. Select initial plane 1<<(x&3) using word OUT 0x3C4, then remaining planes via byte OUT 0x3C5; wrap increments aperture byte. Preserve EBP. Original no-clip entry names are counterintuitive: FlipNoClipX is forward, NoClipX mirrors. Exact clobbers, scratch values and plane bytes are modeled.

**Valid test domain.** Positive byte width/height; mapped VGA footprint and packed sprite; ordered clip window.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:065cb out dx, ax`
- `1:0661c out dx, al`
- `1:06669 out dx, al`
- `1:066b2 out dx, al`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 40/40 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DrawSpriteFlipX.json).

## DrawSpriteColorX

`1:066df`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, CL byte width, CH byte height, EDI destination, ESI source; EDX color bank. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [sprites.py:174:fn](sprites.py). Preserved throughout the tested domain: EBP. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux draw_func_t parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi] /* src/action_local.h:15 */
#pragma aux island_draw_t parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi] /* src/include/type_island_abi.h:6 */
```

**Globals.** Reads: row_offsets_80_01e20; sprite clip box. Writes: saved_sprite_width; mode_x_sprite_state; saved_sprite_color.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Draw packed sprite through four VGA planes. Pixel values<0xA0 draw; all bytes>=0xA0 are individual transparent pixels (no run skipping). Visible pixel ORs low byte of EDX*8. Clip with shared sprite helper. For phase p=0..3, copy ceil((width-p)/4) samples with source column step+4, original source row pitch and 80-byte aperture pitch. Select initial plane 1<<(x&3) using word OUT 0x3C4, then remaining planes via byte OUT 0x3C5; wrap increments aperture byte. Preserve EBP. Original no-clip entry names are counterintuitive: FlipNoClipX is forward, NoClipX mirrors. Exact clobbers, scratch values and plane bytes are modeled.

**Valid test domain.** Positive byte width/height; mapped VGA footprint and packed sprite; ordered clip window.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:0674e out dx, ax`
- `1:067a6 out dx, al`
- `1:067fa out dx, al`
- `1:0684a out dx, al`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 40/40 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DrawSpriteColorX.json).

**Caller evidence.** src/affiche.c:1032, src/affiche.c:1038, src/affiche.c:1042, src/affiche.c:1141, src/affiche.c:1146, src/affiche.c:1150, src/display.c:617, src/display.c:634. Full lists and address registrations are in JSON.

## DrawSpriteColorFlipX

`1:06876`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, CL byte width, CH byte height, EDI destination, ESI source; EDX color bank. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [sprites.py:174:fn](sprites.py). Preserved throughout the tested domain: EBP. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux draw_func_t parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi] /* src/action_local.h:15 */
```

**Globals.** Reads: row_offsets_80_01e20; sprite clip box. Writes: saved_sprite_width; mode_x_sprite_state; saved_sprite_color.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Draw mirrored packed sprite through four VGA planes. Pixel values<0xA0 draw; all bytes>=0xA0 are individual transparent pixels (no run skipping). Visible pixel ORs low byte of EDX*8. Clip with shared sprite helper. For phase p=0..3, copy ceil((width-p)/4) samples with source column step-4, original source row pitch and 80-byte aperture pitch. Select initial plane 1<<(x&3) using word OUT 0x3C4, then remaining planes via byte OUT 0x3C5; wrap increments aperture byte. Preserve EBP. Original no-clip entry names are counterintuitive: FlipNoClipX is forward, NoClipX mirrors. Exact clobbers, scratch values and plane bytes are modeled.

**Valid test domain.** Positive byte width/height; mapped VGA footprint and packed sprite; ordered clip window.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:068ea out dx, ax`
- `1:06944 out dx, al`
- `1:0699a out dx, al`
- `1:069ec out dx, al`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 40/40 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DrawSpriteColorFlipX.json).

## DrawSpriteNormalNoClip

`1:06a18`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, CL byte width, CH byte height, EDI destination, ESI source; EDX compatibility argument ignored. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [sprites.py:104:fn](sprites.py). Preserved throughout the tested domain: EBP. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux draw_func_t parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi] /* src/action_local.h:15 */
```

**Globals.** Reads: row_offsets_80_01e20; sprite clip box. Writes: saved_sprite_width.

**Layouts.**

sprite rows (INVENTED).

width*height u8 pixels, row pitch original width; transparency as described

**Computation.** Draw packed sprite with source pitch=original CL and destination pitch 320. Values<0xA0 draw; a marker>=0xA0 advances the source/destination column by marker-0xA0 (0xA0 is invalid). No clipping. Preserve EBP; other output registers are specified by the executable ABI adapter.

**Valid test domain.** Positive packed byte width/height; ordered nonempty clip; valid run-marker rows; mapped disjoint 320-byte-pitch destination and source.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 10/10 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DrawSpriteNormalNoClip.json).

## DrawSpriteNormal

`1:06a24`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, CL byte width, CH byte height, EDI destination, ESI source; EDX compatibility argument ignored. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [sprites.py:104:fn](sprites.py). Preserved throughout the tested domain: EBP. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux draw_func_t parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi] /* src/action_local.h:15 */
```

**Globals.** Reads: row_offsets_80_01e20; sprite clip box. Writes: saved_sprite_width.

**Layouts.**

sprite rows (INVENTED).

width*height u8 pixels, row pitch original width; transparency as described

**Computation.** Draw packed sprite with source pitch=original CL and destination pitch 320. Values<0xA0 draw; a marker>=0xA0 advances the source/destination column by marker-0xA0 (0xA0 is invalid). Use clip helper. Preserve EBP; other output registers are specified by the executable ABI adapter.

**Valid test domain.** Positive packed byte width/height; ordered nonempty clip; valid run-marker rows; mapped disjoint 320-byte-pitch destination and source.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 12/12 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DrawSpriteNormal.json).

**Caller evidence.** src/affiche.c:743, src/affiche.c:1223, src/affiche.c:1485. Full lists and address registrations are in JSON.

## DrawSpriteNormal256

`1:06a9d`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, CL byte width, CH byte height, EDI destination, ESI source; EDX compatibility argument ignored. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [sprites.py:104:fn](sprites.py). Preserved throughout the tested domain: EBP. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: row_offsets_80_01e20; sprite clip box. Writes: saved_sprite_width.

**Layouts.**

sprite rows (INVENTED).

width*height u8 pixels, row pitch original width; transparency as described

**Computation.** Draw packed sprite with source pitch=original CL and destination pitch 320. Zero is transparent; every other byte is copied. Use clip helper. Preserve EBP; other output registers are specified by the executable ABI adapter.

**Valid test domain.** Positive packed byte width/height; ordered nonempty clip; valid run-marker rows; mapped disjoint 320-byte-pitch destination and source.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 10/10 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DrawSpriteNormal256.json).

**Caller evidence.** src/affiche.c:743, src/affiche.c:1223, src/affiche.c:1485. Full lists and address registrations are in JSON.

## DrawSpriteFlipNormalNoClip

`1:06afc`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, CL byte width, CH byte height, EDI destination, ESI source; EDX compatibility argument ignored. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [sprites.py:104:fn](sprites.py). Preserved throughout the tested domain: EBP. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux draw_func_t parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi] /* src/action_local.h:15 */
```

**Globals.** Reads: row_offsets_80_01e20; sprite clip box. Writes: saved_sprite_width.

**Layouts.**

sprite rows (INVENTED).

width*height u8 pixels, row pitch original width; transparency as described

**Computation.** Draw packed sprite with source pitch=original CL and destination pitch 320. Values<0xA0 draw; a marker>=0xA0 advances the source/destination column by marker-0xA0 (0xA0 is invalid). Mirror destination columns. No clipping. Preserve EBP; other output registers are specified by the executable ABI adapter.

**Valid test domain.** Positive packed byte width/height; ordered nonempty clip; valid run-marker rows; mapped disjoint 320-byte-pitch destination and source.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 10/10 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DrawSpriteFlipNormalNoClip.json).

## DrawSpriteFlipNormal

`1:06b08`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, CL byte width, CH byte height, EDI destination, ESI source; EDX compatibility argument ignored. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [sprites.py:104:fn](sprites.py). Preserved throughout the tested domain: EBP. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux draw_func_t parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi] /* src/action_local.h:15 */
```

**Globals.** Reads: row_offsets_80_01e20; sprite clip box. Writes: saved_sprite_width.

**Layouts.**

sprite rows (INVENTED).

width*height u8 pixels, row pitch original width; transparency as described

**Computation.** Draw packed sprite with source pitch=original CL and destination pitch 320. Values<0xA0 draw; a marker>=0xA0 advances the source/destination column by marker-0xA0 (0xA0 is invalid). Mirror destination columns. Use mirrored clip helper. Preserve EBP; other output registers are specified by the executable ABI adapter.

**Valid test domain.** Positive packed byte width/height; ordered nonempty clip; valid run-marker rows; mapped disjoint 320-byte-pitch destination and source.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 12/12 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DrawSpriteFlipNormal.json).

## DrawSpriteFlipNormal256

`1:06b88`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, CL byte width, CH byte height, EDI destination, ESI source; EDX compatibility argument ignored. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [sprites.py:104:fn](sprites.py). Preserved throughout the tested domain: EBP. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: row_offsets_80_01e20; sprite clip box. Writes: saved_sprite_width.

**Layouts.**

sprite rows (INVENTED).

width*height u8 pixels, row pitch original width; transparency as described

**Computation.** Draw packed sprite with source pitch=original CL and destination pitch 320. Zero is transparent; every other byte is copied. Mirror destination columns. Use mirrored clip helper. Preserve EBP; other output registers are specified by the executable ABI adapter.

**Valid test domain.** Positive packed byte width/height; ordered nonempty clip; valid run-marker rows; mapped disjoint 320-byte-pitch destination and source.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 10/10 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DrawSpriteFlipNormal256.json).

## DrawSpriteColorNormal

`1:06bea`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, CL byte width, CH byte height, EDI destination, ESI source; EDX color bank. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [sprites.py:104:fn](sprites.py). Preserved throughout the tested domain: EBP. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux draw_func_t parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi] /* src/action_local.h:15 */
#pragma aux island_draw_t parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi] /* src/include/type_island_abi.h:6 */
```

**Globals.** Reads: row_offsets_80_01e20; sprite clip box. Writes: saved_sprite_width; saved_sprite_color.

**Layouts.**

sprite rows (INVENTED).

width*height u8 pixels, row pitch original width; transparency as described

**Computation.** Draw packed sprite with source pitch=original CL and destination pitch 320. Values<0xA0 draw; a marker>=0xA0 advances the source/destination column by marker-0xA0 (0xA0 is invalid). Visible pixels OR ((EDX*8)&255). Use clip helper. Preserve EBP; other output registers are specified by the executable ABI adapter.

**Valid test domain.** Positive packed byte width/height; ordered nonempty clip; valid run-marker rows; mapped disjoint 320-byte-pitch destination and source.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 12/12 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DrawSpriteColorNormal.json).

**Caller evidence.** src/affiche.c:1032, src/affiche.c:1038, src/affiche.c:1042, src/affiche.c:1141, src/affiche.c:1146, src/affiche.c:1150, src/display.c:617, src/display.c:634. Full lists and address registrations are in JSON.

## DrawSpriteColorFlipNormal

`1:06c89`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, CL byte width, CH byte height, EDI destination, ESI source; EDX color bank. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [sprites.py:104:fn](sprites.py). Preserved throughout the tested domain: EBP. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux draw_func_t parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi] /* src/action_local.h:15 */
```

**Globals.** Reads: row_offsets_80_01e20; sprite clip box. Writes: saved_sprite_width; saved_sprite_color.

**Layouts.**

sprite rows (INVENTED).

width*height u8 pixels, row pitch original width; transparency as described

**Computation.** Draw packed sprite with source pitch=original CL and destination pitch 320. Values<0xA0 draw; a marker>=0xA0 advances the source/destination column by marker-0xA0 (0xA0 is invalid). Mirror destination columns. Visible pixels OR ((EDX*8)&255). Use mirrored clip helper. Preserve EBP; other output registers are specified by the executable ABI adapter.

**Valid test domain.** Positive packed byte width/height; ordered nonempty clip; valid run-marker rows; mapped disjoint 320-byte-pitch destination and source.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 12/12 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DrawSpriteColorFlipNormal.json).

## asm_06d33

`1:06d33`; private; name **INVENTED**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT_CONTRIBUTION (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, CL byte width, CH byte height, ESI image; stack+4 mask pointer. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked. Caller-owned stack+4 flags/mask is updated as described.

**Register outputs.** Exact transformations: [private.py:10:diffclip](private.py). Preserved throughout the tested domain: EBP, EDI. CF is a specified accept/reject output. ESP returns as specified above.

**Globals.** Reads: sprite clip box. Writes: code 1:06d2f original width; caller stack+4 mask pointer.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Private differential-sprite clip helper. Save original CL width at code 1:06d2f. Left trim rounds down to 4 bytes and advances ESI and mask pointer at caller stack+4; left path skips subsequent right clipping. Convert width to floor(width/4), or ceil(right-cropped width/4). Top trim advances image/mask by original_width*rows. Top-trim path leaves EDX from 16-bit MUL, so bottom check uses y rather than y+remaining_height (original asymmetry). CF=1 rejects. All register outputs and live stack pointer update are defined by sprites.clip(diff=True).

**Valid test domain.** Packed positive byte dimensions; original-width source/mask mapped with dword padding; ordered clip box; caller stack+4 holds mask pointer.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 18/18 (100.0%). Current mismatches: 0. [Per-routine certificate](results/asm_06d33.json).

## DrawSpriteDiffNormal

`1:06e0d`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EAX x, EBX y, CL byte width, CH byte height, EDI destination, ESI source; EDX parallel mask pointer. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [sprites.py:107:diff](sprites.py). Preserved throughout the tested domain: EBP. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux DrawSpriteDiffNormal parm [eax] [ebx] [ecx] [edx] [edi] [esi] modify exact [eax ebx ecx edx esi edi] /* src/skops.c:80 */
```

**Globals.** Reads: sprite clip box; row_offsets_80_01e20. Writes: code 1:06d2f saved original width.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Clip image/mask together. Width becomes dword count: floor(width/4) when no right crop, ceil(cropped_width/4) at right edge; left crop advances in multiples of 4 and may leave x up to 3 pixels left of XMIN. At each nonzero image dword write (destination & mask)|image; zero image dwords leave destination untouched. Original source pitch in bytes; destination pitch 320. Preserve EBP; exact clobbers in model.

**Valid test domain.** Positive byte width/height; image/mask mapped with at least 3 right padding bytes per row; clip window ordered; source width normally multiple 4.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 10/10 (100.0%). Current mismatches: 0. [Per-routine certificate](results/DrawSpriteDiffNormal.json).

**Caller evidence.** src/skops.c:290, src/skops.c:297, src/skops.c:366, src/skops.c:373, src/skops.c:407, src/skops.c:414, src/skops.c:429, src/skops.c:435. Full lists and address registrations are in JSON.

## synchro

`1:06e8a`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:33:synchro](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux synchro parm [] modify exact [] /* src/movie.c:978 */
#pragma aux synchro parm [] modify exact [] /* src/pcmain.c:392 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Read VGA status port 0x3DA until bit 3 becomes one (already in retrace suffices). Preserve all GPRs. IN site 1:06e90.

**Valid test domain.** Finite status stream ending in a byte with bit 3 set.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:06e90 in al, dx`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 3/3 (100.0%). Current mismatches: 0. [Per-routine certificate](results/synchro.json).

**Caller evidence.** src/blood.c:72, src/cheat.c:289, src/cheat.c:294, src/cheat.c:318, src/cheat.c:322, src/cheat.c:419, src/cheat.c:423, src/cheat.c:608. Full lists and address registrations are in JSON.

## endsynchro

`1:06e9a`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:39:endsynchro](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux endsynchro parm [] modify exact [] /* src/movie.c:976 */
#pragma aux endsynchro parm [] modify exact [] /* src/pcmain.c:394 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Read VGA status port 0x3DA until bit 3 becomes zero. Preserve all GPRs. IN site 1:06ea0.

**Valid test domain.** Finite status stream ending in a byte with bit 3 clear.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:06ea0 in al, dx`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 3/3 (100.0%). Current mismatches: 0. [Per-routine certificate](results/endsynchro.json).

**Caller evidence.** src/cheat.c:286, src/cheat.c:292, src/cheat.c:315, src/cheat.c:417, src/cheat.c:422, src/cheat.c:609, src/cheat.c:612, src/cheat.c:780. Full lists and address registrations are in JSON.

## set_speaker_on

`1:06eaa`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:62:set_speaker_on](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: lowlevel_timer_02148; lowlevel_timer_02150; lowlevel_timer_02154; lowlevel_timer_02158.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Reset last, epoch, repetition counters to zero; read port 0x61, set bits 0 and 1 and write it; set period=65535; OUT 0x43=0xB4, then OUT 0x42=0xFF twice. GPRs preserved. Sites 1:06eaa..06ee9.

**Valid test domain.** Arbitrary input byte from port 0x61.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:06ec9 in al, 0x61`
- `1:06ecd out 0x61, al`
- `1:06edb out 0x43, al`
- `1:06ee2 out 0x42, al`
- `1:06ee6 out 0x42, al`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/set_speaker_on.json).

**Caller evidence.** src/cheat.c:363. Full lists and address registrations are in JSON.

## set_speaker_off

`1:06eea`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:69:set_speaker_off](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

No entry-specific current pragma found; reference ABI evidence is retained by address in asm_spec.json.

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: no named global beyond output buffers/ABI stack.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Read port 0x61, clear bits 0 and 1 and write it; OUT 0x43=0xB4, OUT 0x42=0 twice (PIT divisor 65536). Software timer globals are unchanged. Preserve GPRs. Sites 1:06eea..06efc.

**Valid test domain.** Arbitrary input byte from port 0x61.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:06eeb in al, 0x61`
- `1:06eef out 0x61, al`
- `1:06ef3 out 0x43, al`
- `1:06ef7 out 0x42, al`
- `1:06ef9 out 0x42, al`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/set_speaker_off.json).

**Caller evidence.** src/cheat.c:941. Full lists and address registrations are in JSON.

## clock_ticks

`1:06efd`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:77:clock_ticks](models.py). Preserved throughout the tested domain: EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux clock_ticks parm [] value [eax] modify exact [eax] /* src/pcmain.c:388 */
```

**Globals.** Reads: lowlevel_timer_02148; lowlevel_timer_02150; lowlevel_timer_02154; lowlevel_timer_02158. Writes: lowlevel_timer_02148; lowlevel_timer_02150; lowlevel_timer_02154; lowlevel_timer_02158.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Latch PIT 2 by OUT 0x43=0x80 and read low then high counter bytes at 0x42 (1:06f04..06f0c). Let delta=s32(period-counter), repeat=0. If delta<=s32(last), add period to epoch; when equal increment previous repetition count and if it exceeds 20 first set period=0 and repetition=0. Store last=delta, epoch, repeat and return u32(delta+epoch) in EAX; preserve other GPRs.

**Valid test domain.** 16-bit PIT counter; period in {0, 65535}; game-maintained signed last, epoch, repeat.

**Hardware boundary.** Exact expected sequences are generated by the model; abstract service responses are supplied by the fixture. DOS sites:

- `1:06f06 out 0x43, al`
- `1:06f09 in al, 0x42`
- `1:06f0d in al, 0x42`

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 6/6 (100.0%). Current mismatches: 0. [Per-routine certificate](results/clock_ticks.json).

**Caller evidence.** src/cheat.c:248, src/cheat.c:287, src/cheat.c:290, src/cheat.c:293, src/cheat.c:296, src/cheat.c:316, src/cheat.c:319, src/cheat.c:323. Full lists and address registrations are in JSON.

## wait_until

`1:06f5d`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** EBX signed target tick. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:91:wait_until](models.py). Preserved throughout the tested domain: EBP, EBX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux wait_until parm [ebx] value [ecx] modify exact [eax ecx] /* src/pcmain.c:390 */
#pragma aux wait_until parm [ebx] value [ecx] modify exact [eax ecx] /* src/include/lowlevel.h:28 */
```

**Globals.** Reads: lowlevel_timer_02148. Writes: timer state via clock_ticks.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** If period is zero set ECX=1 without polling. Otherwise poll clock_ticks until signed tick value > signed EBX target. ECX is polls-1; EAX is last tick; other GPRs preserved.

**Valid test domain.** Reachable finite timer sequence, no signed epoch overflow during wait.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 4/4 (100.0%). Current mismatches: 0. [Per-routine certificate](results/wait_until.json).

**Caller evidence.** src/cheat.c:288, src/cheat.c:293, src/cheat.c:317, src/cheat.c:321, src/display.c:680, src/pcmain.c:595, src/pcmain.c:750. Full lists and address registrations are in JSON.

## speaker_enable

`1:06f78`; public; name **RAYVERSE**. Source: [src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm). Reconstructed bytes: STRICT (imported acceptance status, no fresh compile).

**Contract.** No explicit arguments; globals/device inputs as below. Entry[ESP] return address; ordinary RET advances ESP 4; all temporary stack written bytes checked.

**Register outputs.** Exact transformations: [models.py:73:speaker_enable](models.py). Preserved throughout the tested domain: EAX, EBP, EBX, ECX, EDI, EDX, ESI. ESP returns as specified above.

C-side declarations (ABI-only pragmas):

```c
#pragma aux speaker_enable parm [] modify exact [] /* src/pcmain.c:396 */
#pragma aux speaker_enable parm [] modify exact [] /* src/include/lowlevel.h:32 */
```

**Globals.** Reads: none beyond explicit buffers/descriptor and abstract device inputs. Writes: lowlevel_timer_02148; lowlevel_timer_02150; lowlevel_timer_02154; lowlevel_timer_02158.

**Layouts.**

Raw byte buffers or scalar pointers only; pitches, extents and any packing are specified below.

**Computation.** Call set_speaker_on, including resetting its software clock state. Preserve GPRs.

**Valid test domain.** Same as set_speaker_on.

**Proof.** EQUIVALENT_TESTED; 256 cases; seeds `0xa550000..0xa5500ff`; blocks 1/1 (100.0%). Current mismatches: 0. [Per-routine certificate](results/speaker_enable.json).

**Caller evidence.** src/cheat.c:311, src/display.c:904, src/frame.c:973, src/pcmain.c:493, src/pcmain.c:652, src/pcmain.c:1075. Full lists and address registrations are in JSON.

