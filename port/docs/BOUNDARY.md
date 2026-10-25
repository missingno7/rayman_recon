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
| HMI SOS (sos*) | `compat/src/hmi.c`: native 32-voice clock-driven HMI API; shared SDL audio device (`platform/audio.c`) with CD-DA |
| Watcom C library | rand/srand (Watcom LCG, DOS 1:71396), itoa/ltoa, strupr, delay (`wait_milliseconds`), memset helper (`clear_memory`), acos (`effect_math_curve`, Watcom constant), sleep |
| Data not reconstructed as source | PalPlasma, Def_Bande: generated at build time from the player's RAYMAN.EXE (`tools/gen_assets.py`, user decision 2026-10-22) |

Any port, interrupt or service not listed stops the program with `PORT: unimplemented DOS/hardware service
reached: ...`.

## Effects and speech (HMI boundary)

`compat/src/hmi.c` replaces every SOS entry used by hifi/menu/movie/sound. This is one native driver
(handle 0), 32 voices. `src/hmi/hmi_samples.c`, `hmi_active.c`, `hmi_status.c` and `hmi_voice_params.c`
are the API reference: first clear ACTIVE (8000h) slot, -1 on exhaustion; start ORs A000h; stop returns
10 for voice >=32, otherwise 0, clearing ACTIVE/sample ID except when 1000h prevents it. Done returns
10 for an invalid voice, 1 for inactive, 0 for active. SamplesPlaying counts ACTIVE. Volume/pan setters
return 0 if inactive, otherwise the previous **signed 16-bit** stored value, storing the new low 16 bits.
The game ignores setter results; presentation interprets pan as unsigned. Invalid setter indices are safely
rejected (the original library indexed unchecked storage; no valid-call behavior depends on that).

System/driver initialization succeeds without an output device. The driver supplies an opaque callback token;
the DOS callback was the loaded driver's own DMA/mixer service, not a game function. Timer registration keeps
the original first-free slot convention (16 entries, slot 15 occupied by DOS timer chaining when requested,
11 when full); remove frees its slot. The port never calls that token: clock-position updates replace the
service routine, with no DMA/IRQ or host audio callback deciding game state. Movie suspend/resume retains
its original stop/wait/remove/register calls. Detection returns unavailable from DetectInit, FindFirst and
GetSettings, with UnInit succeeding. `sub_3E710` has no source callers; LOAD_CONFIG uses `sub_3E780`
(InitDriver directly), so detection is not reached on the supported entries. No DOS capabilities record
is written into the 100-byte name buffer (PLATFORM_OPEN item 4).

A voice borrows its sample storage until stop/done. Source position is floor(elapsed_ns * 10000 * Q16_step /
(1e9 * 65536)), using overflow-safe integer decomposition. Without PITCH_SHIFT the step is 65536.
Sizes and staged-loop L/N are bytes; U8 effects use one byte/frame, S16LE speech two. Staged loops play
[0,L), then reload [L,L+N), count -1 infinitely. A nonlooping voice loses ACTIVE when all frames have
been consumed. Updating or querying it never depends on queue consumption, wall-clock audio callbacks,
or the presence of a host device. A deterministic status poll charges 1 microsecond (the game-port read
model's per-read cost), then `port_pc_poll` pumps events, keyboard interrupts, CD and HMI. This also
terminates the game's unchanged FreeTchatchVignette busy wait.

Presentation uses a float stereo stream at the actual SDL device rate, nearest-source-frame sampling,
and volume 0..32767. Pan uses linear balance: centre 8000h has full gain on both channels; to either side,
the opposite channel falls linearly to zero at 0000h/FFFFh. Speech without PANNING is centred. The SFX sum
is clipped to [-1,1]; SDL mixes this stream with the existing 44.1 kHz CD stream on **one shared logical
device**. The original Sound Blaster driver's exact mixing, translation arithmetic and rate quantization
are not reproduced; these are presentation choices. No device/stream is opened in headless or deterministic
mode. The main thread fills bounded lookahead buffers; start/stop/gain/pan changes discard pending SFX
presentation so freed sample storage is never read by another thread. CD transport and voice completion
remain clock-driven.

The hifi derivative reads/checksums/XORs the same 128 DOS records, then decodes their LE fields and binds
native pointers. It retains all calculations and service-call order, changes the speech offset write to
flags |= 100h, and uses a native request pointer and full config view matching data_6664.c (its buffer
pointer moves timeout/callback on 64-bit hosts). No other pointer-record literals or wrapped u32 indices
occur in this unit. BSS bank headers already have matching native field layouts.

`RAYMAN_SOUND_TRACE=PATH` records Init, device format/queue diagnostics, StartSample, StopSample, Done and allocation exhaustion with clock,
voice, fixed/world/speech bank (0/1/2), program, bytes, step and flags. This is diagnostic only. End-to-end
tests require CarteSonAutorisee=1 and sample activity; RAYMAN_E2E_EVIDENCE retains traces/frame logs separately
from golden summaries. Unit tests compile the current reconstructed HMI API bodies with private names and
compare randomized operation sequences, alongside playback/PCM/interrupt tests and all nine actual PlayTchatch request paths.
