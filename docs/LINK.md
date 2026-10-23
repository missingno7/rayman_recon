# Link and pack stages

How to run the natural link (tools adopted 2026-10-22 from build/workers/t_link):
`python tools/verify.py` (fresh report), `python tools/manifest.py` (writes build/link/manifest.json; rerun after any
config/source/oracle/library change), `python tools/link.py all` (or compile|link|report|pack). Outputs only under
build/link/. The natural link exits nonzero while `_language_txt` is unresolved; the UNDEFSOK diagnostic image is
measurement output only (packed with PMWLITE /C4 and round-tripped) and must never be run. src/hmi is excluded; the
pinned vendor library sosdw1cr.lib is linked. tests/test_link.py covers manifest schema, asset/unit symbol overlap and
response-file generation.

Evidence: build/workers/lk_pmw122 (first step) and build/workers/lk_repack (oracle repack, WLINK probes), 2026-10-22.
Tools: PMODE/W 1.22 (config/toolchain.json `pmodew122`, stub authenticated against RAYMAN.EXE), Watcom 10.6 WLINK.
DOS utilities run only under MS-DOS Player.

## Packer stage - PROVEN
- An LE carrying exactly the decoded oracle objects (two objects, flags 0x2045/0x2043, virtual sizes 0x7d4c2 and
  0x59960, CS:EIP 1:7013c, SS:ESP 2:59960) and the complete ordered fixup list, packed by PMWLITE 1.22 **/C4** with the
  shipped PMODEW.EXE stub, reproduces all **428,743 bytes** of assets/GOG/RAYMAN.EXE (SHA-256 599b1df2...): MZ/stub
  prefix, PMW1 header, both object streams and all 15 relocation blocks (13 + 2, 16,380-byte split). /C0-/C3 decode to
  the same content but differ in encoding (first difference: data-stream offset at file 0x2754).
- PMWLITE is deterministic (120 runs on a probe; names/dirs/timestamps do not leak); levels round-trip through
  tools/pmw1.py; the 64-byte MZ header + 9,955-byte stub body + 13 zero bytes arise from the shipped utilities.
- Relocations: 22,358 records (20,461 obj1 / 1,897 obj2), all OFF32, including ten duplicate page crossings; ordered
  tuple hashes in build/workers/lk_repack/NOTES.md. Order matters; never sort.

## Linker facts (WLINK 10.6, small programs) - PROVEN unless marked
- `system pmodew` (shipped PMODEW.LNK) emits LE; PMWLITE converts to PMW1. Module order = contribution order inside the
  merged objects (`_TEXT`, `_DATA`, CONST, `_BSS`); runtime archive members follow explicit modules; BEGTEXT first.
- C `_TEXT` contributions paragraph-aligned; CONST/CONST2/_DATA/_BSS dword-aligned.
- Default DGROUP order: _NULL, _AFTERNULL, CONST, CONST2, _DATA, TIB.., XIB.., YIB.., DATA, _BSS, STACK. **PROVEN
  (lk_stage2, 17 links):** the FIRST declaration of a DATA-class segment fixes the _DATA/CONST/CONST2 order; the DGROUP
  member list does not override it. A leading assembly module declaring `_DATA` (then CONST, CONST2, and DGROUP incl.
  DATA32) gives Rayman's order with no ORDER directive; declaring that first `_DATA` PARA-aligned puts it at 2:00010
  (matches the first assembly-data anchor). DATA32 joins object 2 only via an explicit DGROUP. STRONG: the original's
  leading hand-written module(s) introduced _DATA first; exact module structure OPEN.
- Contribution manifest (build/workers/lk_stage2/manifest.json/.md, generator contribution_manifest.py): 76 game-code
  envelopes, a leading assembly cluster, 177 runtime member candidates (CLIB3R, MATH387R, and MATH3R - missing from
  config/oracles.json's runtime list), 361 data/BSS slices; zero cycles between code order and located _DATA/CONST/
  CONST2/DATA32/BSS orders. Central data TU ownership/number OPEN (initialised 2:0215c..2:2d998, BSS 2:36d88..3fa9d).
- Relocation order: 512-byte append buffers PROVEN (6,144 probe positions; 511 fails); on the oracle, sorted-source
  traversal predicts all 20,461 obj1 records and obj2 pages 0..48; page 49 follows runtime members' OMF FIXUPP order
  (667 positions checked), consistent with the manifest.
- Startup markers (cstrt386 at 1:7013c, CLIB3R): `_edata` = 2:32190, `_end` = 2:5795c; STACK = round_up(_end,16) =
  2:57960..2:59960, i.e. **stack=8192**. Initialised bytes end at 2:3218a.
- LE fixup output order: ascending page; within a page, runs in reverse order, each run in OMF traversal order; an
  append-buffer model (capacity ~512 bytes, STRONG) predicted 2,006/2,006 positions in probes. Record width 16/32 by
  target offset affects run splitting.

## Current state and next steps (living; updated 2026-10-22 after stage 5)
The natural link (build/workers/lk_stage5/link.py) links all game, low-level asm, data and BSS units with the pinned
Watcom CLIB3R/MATH3R/MATH387R and the ORIGINAL HMI library sosdw1cr.lib as libraries (src/hmi is a separate
reconstruction, not linked). Only `_language_txt` is unresolved; an UNDEFSOK diagnostic image links and packs with /C4
(measured only, never executed). All 13 HMI members land in oracle order; their data shifts only with earlier drift.
Applied in stage 5: four initialised-data partitions at true extents; entry spelled `main` (pulls argcv386/initargv/
histsplt as in the original); zero-length XI/YI declarations in the leading asm module fix DATA32 class order.
First raw differences: _TEXT 1:052e6 (an asm reference into moved BSS), _DATA 2:035fc (code pointer into NEAR-drifted
code), CONST 2:2fea8, CONST2 2:31b60, DATA32 2:31ec5. Relocations: 20,287/20,461 (obj1), 1,897/1,897 (obj2); the 939
leading-assembly records are in exact order. Runtime extraction still starts set386 instead of strupr (pcmain's memset
dependency: the original inlines REP STOSD - a NEAR blocker).
Next frontiers (earliest first): (1) contents/ownership of the zero cells 2:06540..44 and 06548..4c (OPEN after dt_cells);
(2) first BSS gap CLOSED (DPMI record, dt_cells); language_txt capacity (237 used, reserved size unknown), original defining TUs of the BSS (central BSS
needs ~5 more declaration groups than known publics - docs/DATA.md); (3) pcmain fill mechanism (runtime order);
(4) NEAR allocation/size drift. Never fill holes, prescribe addresses, pad, or force member order.

## First natural link (2026-10-22, build/workers/lk_stage3: link.py, natural.lnk, gaps.json, contribution_diff.md)
Pipeline works end to end and is reproducible (compile all 446 units + asset step + declaration-only layout.asm ->
WLINK 10.6 `system pmodew` stack=8192 CASEEXACT, manifest-ordered modules, CLIB3R/MATH3R/MATH387R as libraries -> LE ->
per-contribution diff -> PMWLITE /C4); a fresh rebuild reproduces every artifact byte for byte. Asset step (decision
2026-10-22, reversible): Def_Bande/PalPlasma/PalFire bytes are extracted at build time from the user's own
assets/GOG/RAYMAN.EXE into build/ - never tracked; any other missing byte is reported, never filled.
The full link does not resolve yet (135 names): 97 dt_str_* string literals with no defining contribution (data_51d4
credits table 82, data_62d8 DOS error table 15, data_4d5c 1), 18 calls into the unassigned sound-driver code
1:71afa..1:755ce, 10 names needing the open BSS holes, 10 assembly labels not exported across slices.
First differences, PROVEN: _TEXT 1:03125 because the 8 assembly reconstruction slices are separate modules that WLINK
paragraph-aligns (not NEAR drift); _DATA 2:0195a because TableImageX is a dword-aligned C unit inside the assembly data;
lowlevel_data.asm's ungrouped _DATA2/_DATA3 form a third LE object; sound's 128-byte data prefix belongs to object 1
(1:61b80) but WLINK places it in object 2. Later code drift follows NEAR size differences at TU intervals.
Byte-equal, correctly placed contributions keep their ordered relocations (436/436 in the leading assembly).
(Historical note: the "unassigned" code after the game range was later identified as the HMI SOS library, 1:71afa..797f5.)

## Natural link, stage 4 (2026-10-22, build/workers/lk_stage4)
- Applied: sound's snd_sqrt_table is `const` (C const data goes to _TEXT; 128 bytes back in object 1 at 1:61b80);
  init_data_state and record are real BSS aggregates (5 interior field bindings removed). Unresolved non-HMI names:
  only `_language_txt` and `_menu_to_display` remain (their extents overlap other accepted globals - OPEN).
- Runtime member order: natural extraction starts set386 (aliases and pcmain's early memset_ reference - the PC
  main-loop memset is a known NEAR blocker, the original uses inline REP STOSD); an explicit runtime member FILE list
  reproduces 160/160 oracle runtime CODE members in order (diagnostic, not historical evidence). LIBFILE pulls 667
  members; reversed library order selects the wrong acos member.
- BSS: WLINK concatenates same-segment contributions in defining-module order; the current module order has 0
  inversions, but 103 fragments start at non-dword oracle addresses while their OMF segments are dword-aligned - so the
  original defining TUs (central data TU structure) must be reconstructed; no fragment order can place them.
- HMI link format (RESOLVED): the TASM 2.0 /op reconstruction objects (FLAT group, LOC=5 fixups) do not link to OFF32
  with WLINK 10.6. The original vendor library sosdw1cr.lib (asm members TASM 3.1, standard LOC=9) was found and is
  linked directly; src/hmi remains a byte-level source reconstruction only.

## Stage 6 re-measure (2026-10-22, build/workers/lk_stage6)
Only `_language_txt` unresolved. Recipe fix: PalFire is now defined by data_5fa0, so the asset step must not also
generate it (duplicate removed; map anchor errors 1 -> 0). With that, the first BSS difference moves from bss_32190 to
bss_32f58 (the 1,040-byte language_txt region) - the next open data question; the movie zero cells 2:06540..4c remain.
Relocations: obj1 first divergence index 526 (1:05ee9), obj2 index 0 (2:03fa4). NEAR drift (e.g. affiche..araignee
-96 B, pcmain InitData +0x51) and runtime extraction (set386 first; pcmain memset) are unchanged.

## Movie translation unit (2026-10-23, ev_regional)
The former units movieprefix (1:2a1c0..) and movie (1:2b288..) are one object: game `_TEXT` contributions are
paragraph-aligned (PROVEN, docs/COMPILER.md object layout) and 1:2b288 is not, in GOG and FR v1.21 alike (FR keeps
the code continuous across the cut at delta +0x10). Merged into src/movie.c: 32/45 STRICT before and after, movie
CONST VERIFIED, verifier 0 regressions; the natural link now places the whole object at one delta (-352) instead of
-352/-456. The other game units whose first named function is unaligned (affiche, basic, display, hifi, menu,
effect50350) start at aligned object boundaries per tools/tumap.py (leading bytes before the first public).

## First complete natural link (2026-10-23)
`_language_txt` is now defined (src/bss/bss_language_txt.c: `char *language_txt[237]`, the PROVEN minimum extent;
original capacity 237..260 OPEN, the 116-byte remainder of the +0x410 envelope stays an unattributed hole). With it
WLINK resolves every symbol: `python -B tools/link.py all` gives natural rc 0, gaps 0, an image (build/link/natural.exe)
and a /C4-packed image. Not byte-identical (NEAR drift, layout) and never executed by the tools. Remaining differences
are now matching/layout differences, not missing definitions.

## First runtime investigation (2026-10-23)
Baseline: main 2fb042c (sources identical to 46fb266: 1,544 STRICT, 0 regressions). `python -B tools/link.py all`
(manifest regenerated): natural rc 0, 0 unresolved; `pack_natural` packs natural.exe (never the UNDEFSOK diagnostic
image) with PMWLITE 1.22 /C4, decode round trip exact (sha256 dd8eb1eb..., not the original). Run: DOSBox-X 2022.09
(pinned), isolated copy of assets/GOG data with only RAYMAN.EXE replaced, read-only CD image assets/DOS_CD/RAYMAN.cue,
time-limited, AUTOTYPE input, window captures (scratch/runtime_run.py).
- Without a CD driver the rebuilt game prints the original "CD ROM's Initialization / Please install MSCDEX"
  message (strings at 2:30914/2:30b51 in GOG v1.21) and exits - original behaviour, environment condition.
- FIRST BLOCKER (reconstruction defect, PROVEN): with MSCDEX present the game stops in text mode after two INT 2Fh
  AX=1510h requests that DOSBox-X rejects ("MSCDEX: invalid call to interrupt handler" = no request header).
  sub_564f8 takes a dos_regs_t* (ax +0, bx +2, cx +4, ... es +18, ss +20); DOS passes 2:4e8a2 at every call.
  Our bindings split that one object into four phantom globals (cd_packet_command_word 4e8a2, argument_low 4e8a4,
  argument_high 4e8a6, segment 4e8b4); they only line up at the oracle addresses. The natural link placed them at
  4e8ec/4e8ee/4e8f0/4e8f4, so ES:BX (es read from +18) was lost. Same class as H2 (phantom globals).
- Experiment branch exp/cd-regs (one `dos_regs_t cd_request_regs` at 2:4e8a2, 22 bytes VERIFIED; worktree build,
  same pipeline): the rebuilt game passes CD init, plays CD audio, shows the title, the main menu, the save-slot
  screen ("Choose a game", name entry), loads the first level (The Dream Forest) and Rayman moves/scrolls under
  keyboard input. Runtime observation only - not a STRICT or EQUIVALENT claim.
- Not yet on main: the struct changes cdaudio sub_1a280's allocation (STRICT -> NEAR, 68 bytes; DOS keeps track in
  BL and builds AX = 0x1500|command); natural spellings tried (4) did not recover it. Open task: recover sub_1a280 with
  the correct struct, then integrate the struct (blocker B18).
- Static audit for further split objects (same day): (1) every address-of-global cast in src (`(s32)&x`, `(u8*)&x`,
  ...) was checked against the receiver's extent - only cd_packet_command_word (B18) is undersized
  (cd_disc_info 0x51f = sizeof, options_t 10 shorts, palettes 768). (2) every off32 reference of the build was compared
  with its symbol's extent in the natural map: 52 out-of-extent offsets, all negative Watcom idioms (dword/word loads
  ending at the object, shifted array bases, original asm base arithmetic) or src/hmi reconstruction units that the
  natural link does not use; no game reference runs forward into the next object. B18 is the only known split object
  that breaks the natural build.

