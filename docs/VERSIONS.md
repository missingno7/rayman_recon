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
measurement that the v1.12 game code is reproducible by the 10.6 compiler too (10.5 gives identical code).

## PS1
`SLUS-000.05` (US, SHA-1 `4ac1789e…`) is the decomp's exact target. PS1 is GCC/MIPS; shared names come from the
Android symbols used by both Rayverse and the PS1 decomp. `tools/xref.py` joins by name (816 of 1,462 Rayverse
functions have a PS1 address); a name join is a candidate link, not evidence of identical logic.

## Android Rayman Classic symbols (external/rayman-from-scratch, `tools/android.py`)
`objdump -TC libSparkApp.so` of two x86 Android releases: 7,564 named functions and ~2,600 named data objects with
sizes and sections (`.data` initialised, `.bss` uninitialised, `.rodata`). The Android port is built from the DOS
source (names) but with a different compiler and replaced platform layers. Measured uses:
- Names: 1,336 of 1,462 Rayverse-annotated functions occur verbatim (Rayverse names are these symbols); the rest
  are 51 `sub_XXXXX` placeholders and Rayverse-invented or DOS-only names. Android names are the preferred source
  for unnamed DOS functions and globals.
- Data sizes = true C types: every data binding with a simple type is checked against the Android size by
  `tools/promote.py` and `tools/verify.py`. Corrections so far: costab s16[129] (not 257), language_txt char*[300]
  (not 304); confirmations: main_mem_tmp 12 bytes (DOS mem_t, not Rayverse's port struct), status_bar 10, ray 0x84,
  rvb_plan3 0x300.
- Android widened a few globals: world_index, first_credit, last_credit, ray_inertie are 2 bytes on Android but
  lie inside an unbroken run of 1-byte BSS globals on DOS (2:3fa0e..2:3fa9c), which the proven size-rank _BSS order
  only allows for 1-byte objects. Such bindings carry an explicit `dos_size_evidence` that overrides the Android size.
- Android struct sizes use natural alignment (e.g. level 8 bytes vs DOS packed 6); only simple types are compared.
- File grouping: Android keeps a source file's functions together in .text (e.g. all audiocd functions, the bit
  utilities, rand, bat each form one block), so it is evidence for TU membership; the order inside a file differs
  from DOS (not usable for function order).
- NOT usable: Android `.bss` address order does not reflect the DOS declaration order (pairwise agreement with the
  DOS BSS order at chance level: 8,446/15,942) [negative result, `tools/probes/bss_android_order.py`].
- The DOS CD-audio driver layer is absent on Android (our INVENTED AudioTrackCommand & co. have no Android name).

### Android APK (assets/Android, `android_apk` in config/oracles.json)
Rayman Anniversary 1.0.1 APK (a later Android build than the text dumps; addresses differ between them). Its
`lib/x86/libSparkApp.so` has dynamic symbols only (no `.symtab`, no DWARF), but two things are usable:
- Initialised data contents: `python tools/androiddata.py` finds named `.data/.rodata` objects in DOS obj2 by
  content. 109 unique hits (>= 4 bytes); 61 unique hits >= 16 bytes with game names became data bindings
  (name ANDROID) after the overlap/size gates; they reproduce the three earlier independent bindings exactly
  (costab 2:6fe0, RandArray 2:7344, N_anim 2:4a9c). Watcom keeps `_DATA` in definition order, so these addresses
  also give the original definition order of initialised globals.
- x86 code per exported function: `python tools/android.py --dis NAME` (semantic reference for Rayverse stubs and
  unnamed functions; e.g. DoEffectBloodOut starts with MakeMyRand(1); start_cd_* call playSound on Android).
