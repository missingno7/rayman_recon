# Versions and cross-version facts

## DOS builds available

| | v1.12 (`dos_v112_us`) | v1.21 (`gog_v121_us`) |
|---|---|---|
| banner | `RAYMAN (US) v1.12` ... `(UNPROTECTED)` | `RAYMAN (US) v1.21` |
| PMODE/W stub | 1.21, 0x2650 bytes | 1.22, 0x2730 bytes |
| obj1 (code) | 0x7c12e bytes, 20,229 fixups | 0x7d4c2 bytes, 20,461 fixups |
| obj2 (data) | 0x31e26 init / 0x58bf0 virtual, 1,896 fixups | 0x3218a init / 0x59960 virtual, 1,897 fixups |
| entry (`_cstart_`) | 1:6f6a8 | 1:7013c |
| Watcom runtime (libscan) | **10.5** CLIB3R: 112 members exact; 10 members match only 10.5 | **10.6** CLIB3R: 116 members exact; 11 members match only 10.6 |

[PROVEN] The runtime libraries differ by release (`tools/libscan.py --compare`), so the two builds were linked with
different Watcom releases (10.5 for v1.12, 10.6 for v1.21). Which compiler built the game objects is a separate
question (docs/COMPILER.md).

## Rayverse and rayman_disasm coordinates
- Rayverse `//XXXXX` annotations are v1.21 addresses = obj1 offset + 0x10000 [PROVEN on several functions].
  They do NOT apply to v1.12 (e.g. myRand: v1.21 1:5bf60, v1.12 1:5b540).
- rayman_disasm `gog/` = this v1.21 file (same SHA-256); its labels use obj1 base 0x10000 and obj2 base 0x90000
  (`tools/image.py` "ida" convention). Its README notes that two third-party PMW1 unpackers disagree in 3 places;
  our decoder is independent of both (docs/EXECUTABLE.md).
- rayman_disasm `demos/` contains LE images of five demos (not used yet).

## Function-level variants found
| function | v1.12 | v1.21 | PS1 | category |
|---|---|---|---|---|
| myRand | identical code (addresses differ) | STRICT (src/rand.c) | different: accumulates `PS1_RandSum` | CONFIRMED_SAME_LOGIC (v1.12/v1.21), CONFIRMED_VARIANT (PS1) |
| MakeMyRand | `RandArray[i] = rand();` (STRICT against v1.12 in scratch) | `rand() & 0xEFFF` | not compared | CONFIRMED_VARIANT (v1.12 vs v1.21) |

The v1.12 rand object was verified STRICT with the same profile (`scratch/v112/`, overlay bindings) - one
measurement that the v1.12 game code is reproducible by the 10.6 compiler too (10.5 compiler not yet runnable).

## PS1
`SLUS-000.05` (US, SHA-1 `4ac1789e…`) is the decomp's exact target. PS1 is GCC/MIPS; shared names come from the
Android symbols used by both Rayverse and the PS1 decomp. `tools/xref.py` joins by name (816 of 1,462 Rayverse
functions have a PS1 address); a name join is a candidate link, not evidence of identical logic.
