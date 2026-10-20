# RAYMAN.EXE container: PMODE/W + PMW1

Format authority: PMODE/W 1.33 source distribution (MIT, `C:\tools\pmodew-1.33\src`): `DOCS/PMW1FMT.TXT`,
`PMWLITE/PMW1.H`, `PMWLITE/PMWLITE.C` (LE -> PMW1 converter), `DECODE.ASM` (decompressor). Nothing from it is executed.

## Layout (both builds)
```
0x0000  MZ header + PMODE/W real/protected-mode extender stub (e_lfanew -> PMW1 header)
        v1.21: stub 0x2730 bytes, PMODE/W 1.22 ; v1.12: 0x2650 bytes, PMODE/W 1.21 (header version word)
PMW1    header (0x28): id, version, flags (bit0 = compressed: set), CS:EIP obj/off, SS:ESP obj/off,
        object table offset/count (2), relocation table offset, data offset
        object table (2 x 0x18): virtual size, stored size, LE object flags, reloc offset, reloc block count,
        uncompressed size
        relocation blocks: {u16 compressed size, u16 size} + compressed stream; decoded = 10-byte items
        {u8 type, i32 source offset, u8 target object, i32 target offset} (packed)
        object data: one compressed stream per object (obj1 code, obj2 initialised data)
```
| | v1.21 | v1.12 |
|---|---|---|
| obj1 flags | 0x2045 (readable, executable, preload, 32-bit) | same |
| obj2 flags | 0x2043 (readable, writable, preload, 32-bit) | same |
| obj1 stream | file 0x1bdba..0x63e89 -> 0x7d4c2 bytes | 0x1b908..0x62f7e -> 0x7c12e |
| obj2 stream | 0x63e89..0x68ac7 -> 0x3218a (BSS+stack to 0x59960) | 0x62f7e..0x67a65 -> 0x31e26 (to 0x58bf0) |
| relocations | 13 + 2 blocks; 20,461 + 1,897 items, all type 7 (32-bit offset) | 13 + 2; 20,229 + 1,896 |
| entry / stack | 1:7013c (`_cstart_`) / 2:59960 | 1:6f6a8 / 2:58bf0 |

PMWLITE drops the LE header/page table and keeps only object sizes/flags + this relocation list, so any "unpacked LE"
is a reconstruction. Our canonical coordinates are therefore PMW1 object:offset; file offsets exist only per
compressed stream (`build/oracles/<id>/pmw1.json`). Runtime linear addresses are not determined [OPEN].
No self-relative fixups exist: Watcom flat code calls with rel32 that needs no relocation.

## Decoder validation (tools/pmw1.py, tools/extract.py)
The decoder is a line-by-line Python port of `DECODE.ASM` (LZ77 + interleaved 32-bit bit buffer, end marker =
short code with zero distance). Proven consistency [PROVEN, every run]:
1. all 17 compressed streams (2 objects + 15 relocation blocks per build) end exactly at their declared compressed
   length and produce exactly the declared size;
2. at every one of the 22,358 (v1.21) / 22,125 (v1.12) fixup sites the decoded object contains exactly the
   target offset given by the separately compressed relocation stream (mod 2^32; 32 negative offsets into obj2
   occur in hand-written-looking code);
3. 11 duplicate records per build are identical and straddle 4 KiB page boundaries (LE page-split fixups that
   PMWLITE emitted once per page);
4. the decoded runtime matches Watcom CLIB members exactly with identical fixup sites (tools/libscan.py).
Not done: re-compression round trip with PMWLITE (would be the last step to a byte-identical EXE) [OPEN].

## Code object organisation (v1.21)
- 1:00000 `___begtext` (cstrt386), then game code and hand-written assembly up to ~1:6fbe6, then Watcom 10.6
  CLIB3R/MATH387R members to the end (117 members exact).
- Object boundaries: wcc386 `_TEXT` is paragraph aligned; WLINK pads between objects with 00, inside objects
  functions are padded with nop forms. `python tools/tumap.py --list` finds 83 candidate objects (a 00 run before a
  16-byte boundary counts only if a linear sweep from the nearest known function / library-member start ends with a
  ret/jmp exactly where the zeros begin; this removed 31 false positives where the zeros were immediate or
  displacement bytes). They line up with Rayverse's file grouping (e.g. `rand.c` = 1:5bf60..1:5bfd0,
  `mathutil.c` = 1:4def0..1:4e160 exactly). Objects ending on a 16-byte boundary are invisible to this method, so
  a candidate object may contain several original objects.
