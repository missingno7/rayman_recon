# Platform boundary

What the native port provides in place of the DOS machine, per entry. "Original contract" means the routine's
behaviour is specified in docs/port/ASM_SPEC.md (proven against the DOS bytes) and implemented in portable C.

## Assembly layer (src/lowlevel_*.asm)

| Entries | Port | Proof |
|---|---|---|
| Normal-mode drawing: DrawSprite{,Flip,Color,ColorFlip}Normal{,NoClip,256}, DrawSpriteDiffNormal, clip helpers, sprite_clipping / get / default, Display_Bloc{,_Plein}{,_Clippe}, Swap_To_Screen, N_CLRSCR, ClearDrawAndDisplayBufferNormal, DisplayBufferModeNormal, clear_borders_Normal, Copy_Plan0Diff_To_Buf, asm_04d55, DisplayAnyPictureNormal, Draw{Black,Fond,Border}BoxNormal, DrawWldPointPlan2Normal, fplot_Normal, flocon_clipping, draw_flocon1..7 / pluie5..7 / normal_5755, copy_dwords, draw_sprite_deform, local_04a83, effect_reset_geometry, draw_effect_segment | `video/lowlevel_normal.c` (original contracts) | `tests/asm_diff`: 52 entries, 14,594 cases vs the original bytes in Unicorn, 0 mismatches, 565/565 blocks; 3/3 negative controls |
| island_copy_t view of Copy_Plan0Diff_To_Buf | `video/lowlevel_adapters.c` | argument reorder only |
| synchro, endsynchro, set_speaker_on/off, speaker_enable, clock_ticks, wait_until | `compat/src/pc_hardware.c` (original algorithms on the modelled PIT/VGA) | `tests/unit_tests.c` timing cases |
| InitFirstModeVideo, InitModeNormal, textmode, select_display_buffer | `video/vga.c` (BIOS mode, CRTC start = window start) | - |
| get_joy_input1/2_*, dos_game_io_port_something(2) | `compat/src/gameport.c`: original no-joystick outcome | - |
| DetectCpu | Pentium (5) | - |
| Mode X: DrawSprite*X, mode_x_effect_*, fplot_mode_x_2b86, asm_02850.., asm_029d3, asm_02a73, asm_03257.., asm_04430/04493/04504/0456e, display_(any/empty)picture, draw_blackbox/borderbox, block_clear/copy, sub_1268A, InitModeX | not provided: stops the program (`compat/src/unimplemented.c`) | - |
| bios_wait_timer, sub_2748 | not provided (no caller on supported paths) | - |

## Inline sequences (ORIGINAL_INLINE)

`compat/src/inline_helpers.c`: ashr16, ashl16, ashl32, ashr32_inline, abs32_inline, inrange, sgn, min32, max32,
fill, fill_cld, copy, copy_guarded (forward, overlap-sensitive), palette_output, write_movie_palette, the frame/menu
DAC streams (through the VGA DAC model), interrupt flag and PIC mask, CPU-flag and debug-register access (no
debugger: reads 0, writes ignored), INT3 loader traps (return), exit_if_cracked (original: text mode + exit),
exit_if_not_cd_drive (succeeds), menu_patch_once (no-op), menu_check_debug_breakpoints (no breakpoint).
`tests/unit_tests.c` checks the arithmetic helpers against their instruction sequences.

## DOS, BIOS, DPMI, hardware

| Service | Port |
|---|---|
| Files (fopen/open/read/write/lseek/filelength/unlink) | DOS paths resolved case-insensitively; reads from the save directory, then the player's data directory (never written); writes to the save directory; Watcom O_* flag values decoded |
| INT 10h | mode set / get (VGA model) |
| INT 21h | 30h version (6.22), 4Ch exit |
| INT 2Fh (via DPMI 0300h, `sub_564f8`) | 1600h/4680h: no Windows; 1500h/150Bh/150Ch/1510h and optional 1502h-1504h: virtual MSCDEX 2.21 drive (`platform/cdrom.c`, device packets IOCTL in 04/06/0A/0B/0C/0F, out 00/01/03/05, PLAY 84, STOP 85, RESUME 88, READ LONG 80); CD-DA output `platform/cd_audio.c` (SDL3 stream, 44.1 kHz s16 stereo from the BIN) |
| INT 33h | 0000h: no mouse |
| INT 1Ah | 00h: BIOS ticks from the clock |
| DPMI 0100h/0101h | no conventional blocks (their only users are replaced services) |
| BIOS tick byte at 0x46C | `port_bios_tick_byte()` (load.c derivative) |
| Ports | 0x20/0x21 PIC, 0x42/0x43/0x61 PIT ch. 2 and speaker gate, 0x60 keyboard latch, 0x201 game port (nothing connected), 0x3C0..0x3DF VGA |
| Interrupt vectors | table in `pc_hardware.c`; INT 9 handler run per scan code; other vectors stored |
| HMI SOS (sos*) | `compat/src/hmi_absent.c`: system/timer init succeed, driver init fails -> no-sound path |
| Watcom C library | rand/srand (Watcom LCG, DOS 1:71396), itoa/ltoa, strupr, delay (`wait_milliseconds`), memset helper (`clear_memory`), acos (`effect_math_curve`, Watcom constant), sleep |
| Data not reconstructed as source | PalPlasma, Def_Bande: generated at build time from the player's RAYMAN.EXE (`tools/gen_assets.py`, user decision 2026-10-22) |

Any port, interrupt or service not listed stops the program with `PORT: unimplemented DOS/hardware service
reached: ...`.
