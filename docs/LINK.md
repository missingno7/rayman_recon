# Link and pack stages

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

## Next stage
Natural WLINK image of fresh sources + pinned runtime members, compared (objects, virtual metadata, ordered relocations)
before packing with /C4. Preconditions, in order: DGROUP precedence probe; ordered manifest of original contributions
(code TUs, central data/BSS TU(s), assembly modules, runtime members); central data/BSS ownership; relocation-order
model validation on mixed-width/cross-page inputs. No raw oracle payload, address patches or artificial padding.

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
Unassigned code envelopes after the game range: 1:71afa..755ce (15 KB, sound library), ~5 KB more (1:785dc..797f5).
