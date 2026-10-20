# Status (bootstrap, 2026-10-20)

Live numbers: `python tools/verify.py && python tools/status.py`. This file records what is established and what is
not; it is not the counter of record (config/units.json + the verifier are).

## Verified facts
- **Oracles** (config/oracles.json, docs/ORIGINALS.md): primary GOG v1.21 `599b1df2…09eb6`; secondary CD/installed
  v1.12 `913a8d17…6f6d6` (installed tree == CD `RAYMAN/` subtree, 100/100 files); PS1 `SLUS-000.05` SHA-1
  `4ac1789e…` == rayman-ps1-decomp target.
- **Container** (docs/EXECUTABLE.md): PMODE/W stub + PMW1 (v1.22 / v1.21), 2 objects, LZ-compressed, 32-bit offset
  fixups only. Decoder ported from the official PMODE/W 1.33 source; 17 streams per build decode to exact sizes;
  all 22,358 / 22,125 fixup sites hold exactly the independently compressed relocation targets.
- **Runtime**: Watcom CLIB3R/MATH387R, 10.6 for v1.21, 10.5 for v1.12 (exact library members with fixup-site
  agreement; release-specific members discriminate). Entry = `_cstart_`.
- **Game compiler profile**: `wcc386 -oaxt -s -5r` (10.6; 10.5 identical so far). `-5`, `-oa`+`-ot`, `-s`, register
  convention proven by STRICT matches; `-oe` excluded [STRONG]; `-on` open (docs/COMPILER.md).
- **Object structure**: paragraph-aligned `_TEXT` per object, nop padding inside, 00 between objects; 83
  decode-validated object boundaries; whole objects reproduced contiguously: rand (1:5bf60), mathutil (1:4def0).
- **Cross-reference**: 1,462 Rayverse-annotated v1.21 functions (address convention verified), 816 linked by name to
  PS1 addresses/files (`tools/xref.py`); curated categories in config/xref_evidence.json.

## Matching results (GOG v1.21, `python tools/verify.py`)
| unit | source | STRICT | other |
|---|---|---|---|
| rand | src/rand.c | 2/2 (whole object) | - |
| mathutil | src/mathutil.c | 12/12 (whole object) | - |
| audiocd | src/audiocd.c | 8/11 | start_cd_gameover NEAR (unexplained `mov eax,eax` before ret); DoCdCredits, DoCdRap NEAR only through the resulting 4-byte shift |
| bat | src/bat.c | 6/9 | DO_BAT_FLASH, bat_dir, DO_BAT_COMMAND NEAR |
| bonus | src/bonus.c | 6/8 | calc_left_time (register choice at +2), do_perfect_bonus (4 bytes short) NEAR |
| **total** | | **34 functions, 1,900 code bytes STRICT** | 8 NEAR_MATCH |

Additionally verified outside src/: the v1.12 rand object (without the v1.21 `& 0xEFFF` mask) is STRICT against the
v1.12 oracle (scratch/v112/) - one cross-version confirmation. Verifier self-tests: 7/7 pass (6 negative controls).

STRICT here means function-level: bytes at the bound address, identical fixup-site sets, every reference target
verified. It says nothing yet about data/BSS layout, object order, or the linked executable.

## Tooling that runs
inventory, extract (PMW1), libscan (identification / --compare / --symbols), xref, tumap, disasm, wcc (pinned
compiler runner), match (strict verifier), verify (clean rebuild + zero-regression gate), promote (integration),
status, wpack (Watcom installer unpacker). See README.md for the commands.

## Unresolved
- `start_cd_gameover` extra `mov eax,eax` (OPEN; 9 source forms and -oe/-on tested).
- bat: DO_BAT_COMMAND frame (`push ebp` vs `sub esp,4`), bat_dir jump-table target offset, DO_BAT_FLASH.
- bonus: calc_left_time (`push esi` vs `push edi`), do_perfect_bonus (`mov ebx,0x3c` vs a map_time load at +0x22).
  The first worker result for this object was rejected (a short global mis-declared as a shifted int to force the
  `-5` load idiom); the accepted version declares the true `s16` globals.
- `-on`, `-oi`, `-ol` undetermined; struct packing (`-zp`) only implicitly consistent so far.
- No data (`_DATA`/`CONST`/`_BSS`) verification yet; no WLINK experiments; no PMWLITE recompression round trip.
- Runtime linear addresses under PMODE/W unknown (no dynamic analysis done).
- v1.21 game data files differ from v1.12 (not analysed). Other CD executables (CONFIG.EXE etc.) not analysed.

## Not accomplished in the bootstrap
Complete TU reconstruction beyond the two whole objects; data/BSS layout; hand-written assembly classification
(e.g. the 1:00010 sprite/polygon routines that use self-modifying code); link/relink; any SDL3 work (by design).
