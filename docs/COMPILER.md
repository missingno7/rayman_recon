# Compiler and toolchain evidence

Evidence levels: PROVEN (measured, reproducible command), STRONG (several consistent observations),
HYPOTHESIS, OPEN. Probe sources live in `scratch/probe/` (ignored); conclusions here.

## Established
- **Watcom C, 32-bit flat, register calling convention** [PROVEN]: args in EAX, EDX, EBX, ECX; callee saves used
  registers (`push ebx/ecx/edx`); symbols `name_` for functions, `_name` for data (OMF from wcc386 matches).
- **Runtime = Watcom CLIB3R + MATH387R** (register-convention libs) [PROVEN, tools/libscan.py]:
  v1.21 links **10.6** libraries (116 CLIB3R + 1 MATH387R members exact incl. fixup sites; 11 members exact only
  with 10.6, 0 only with 10.5); v1.12 links **10.5** libraries (112 exact; 10 only with 10.5, 0 only with 10.6).
  10.0/10.0a: 41-44 members only. Copyright string "1988-1995" fits 10.5 and 10.6.
- **Game profile `-oaxt -s -5r` with wcc386 10.6 reproduces the rand object exactly** [PROVEN]:
  `python tools/verify.py` -> myRand, MakeMyRand STRICT (111 bytes, whole object `_TEXT`). Discriminators:
  - `-5` (Pentium scheduling): short array element load as `mov edx,[i*2+arr-2]; sar edx,16` (`-4r` gives `movsx`).
  - `-oa` (relaxed aliasing): the global `RandomIndex` is kept in EBX and written back at exit *after* the return
    value is in EAX; without `-oa` every access goes to memory (41 byte diffs).
  - `-ot` required together with `-oa` (`-oxa` does not reproduce it, `-oaxt` does).
  - `-s` (no stack checks): no `__CHK` calls anywhere in game code.
- **Object layout** [PROVEN on rand and mathutil (whole objects contiguous); 83 decode-validated boundaries]: `_TEXT` paragraph aligned (OMF align 3);
  functions inside 4-aligned with nop padding (90, 8bc0, 8d4000); WLINK pads between objects with 00.
- **Data reference encoding**: wcc386 keeps the addend in the fixup field (`RandArray-2` -> field -2); PMW1 stores
  absolute object offsets, so `match.py` checks bound address + field.

- **No `-oe`** [STRONG]: with `-oe` (alone or inside `-oneatx`/`-otexan`) `DoCdRap` inlines `cd_playing` from the
  same object, while the original calls it; audiocd drops from 8 to 7 STRICT. rand and mathutil are unaffected.
- **Watcom 10.5 and 10.6 compilers give identical code** on all accepted units (rand 2/2, audiocd 8/11,
  mathutil 12/12 with both) [non-discriminating so far].

## Undetermined / next discriminators
- `-on` (fast FP), `-oi`, `-ol`/`-ol+`, `-om`: no accepted unit discriminates them yet. Need FP code (`-on`,
  `-fp3/-fp5`, `-fpi87`), loops (`-ol`), intrinsic candidates (`-oi`: memcpy/strcpy/abs inline forms).
- Compiler release for v1.21 game code: 10.0, 10.0a, 10.5 and 10.6 give identical code on everything tested
  (only the 10.6 libraries match the v1.21 runtime). The 10.5 compiler was recovered from the CD's packed
  `DISKIMGS` set (`C:\tools\watcom-10.5-pck`, docs/TOOLCHAIN.md).
- `-zp` structure packing, `-ei` enums, `-j` char signedness: first struct-heavy matches will tell.
- Linker: WLINK version/options, object order, `_DATA`/`_BSS` order not studied yet.
- v1.12 rand variant is STRICT with the 10.6 compiler against v1.12 (scratch), although its runtime is 10.5.

## Negative results (keep)
- `-ox`, `-oxa`, `-os`, `-ot`, `-oil`, `-or`, `-otr`, `-olr`, `-omr`, `-oir`, `-oxe`, `-oxn`, `-oxl+`, `-ox -oe=100`:
  myRand not STRICT. `-ob*` is not a 10.6 option.
- `myRand` with a local index (Rayverse shape) is never STRICT: 3-10 byte diffs (store/return order).
- `extern` vs defining the globals in the TU: no effect on these functions.
