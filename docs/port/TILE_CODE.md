# GOG v1.21 LEV tile callbacks: complete specification

Scope: all 82 `assets/GOG/PCMAP/**/*.LEV` files present on 2026-10-24. Analysis is read-only;
all deliverables are under `build/workers/tilecode/`. File SHA-256s, exact section offsets,
routine locations and shape counts are recorded in [shapes.json](shapes.json).
Shape identifiers and descriptive family names below are INVENTED analysis labels, not recovered game symbols.

**Result: PROVEN structural coverage of 4,595,115 decoded code bytes, 10,125 callbacks
(2,380 distinct bodies), 2,070 normalized instruction shapes, one semantic family,
zero unexplained routines or bytes.** Nine levels contain no callback code. No callback
depends on a source/background buffer, global variable, timer, random state, previous call,
or self-modified instruction. All are sparse constant tile painters.

The dynamic certificate is **EQUIVALENT_TESTED**, not the reconstruction verifier's STRICT.
Static grammar checks and the independent bitmap comparison are exhaustive over the shipped corpus;
randomized execution is finite differential testing, not a general formal emulator proof.

## 1. Evidence and reproduction

Read alongside [src/load.c](../../../src/load.c), [src/frame.c](../../../src/frame.c),
[src/lowlevel_04430.asm](../../../src/lowlevel_04430.asm), [src/include/map.h](../../../src/include/map.h),
and [docs/PORT_READINESS.md](../../../docs/PORT_READINESS.md).
The source was checked against the hash-validated GOG DOS object with these read-only commands:

```text
python -B tools/disasm.py 1:04430 0x100
python -B tools/disasm.py 1:35688 0x780
python -B tools/disasm.py sub_3BEE0 0x350
python -B tools/disasm.py file_level
python -B tools/disasm.py 1:0283c 0x14
```

`sub_3BEE0` is a Rayverse/IDA-style name: its DOS object coordinate is **1:2bee0**,
not 1:3bee0. All numeric DOS addresses in this document use object coordinates.

From the repository root, using installed Python, Capstone 5.0.7 and Unicorn 2.1.4:

```text
python -B build/workers/tilecode/extract.py
python -B build/workers/tilecode/catalogue.py
python -B build/workers/tilecode/audit.py
python -B build/workers/tilecode/prove.py
python -B build/workers/tilecode/extract.py --sample CAKE/RAY1.LEV --tile 1
```

`extract.py` reads/checksums both Mode X sections, splits code by the callback table,
and disassembles every distinct body using Capstone. `catalogue.py` validates every instruction
and every branch target, creates the complete catalogue and two bounded sample listings.
`audit.py` independently compares with Mode Normal pixel data and accounts for all map records.
`prove.py` executes every distinct body against the straightforward pixel model.
No compilation, asset writes, package installation, Git operation or game launch is required.
Do not use `python -O`: these analysis scripts intentionally fail closed with assertions.

## 2. Exact on-disk layout (PROVEN)

Integers are little-endian. Offsets below are absolute byte offsets from the start of a `.LEV`.
The code section has no magic signature or embedded length/header; its position follows from the
map dimensions and preceding sections. Let `W`, `H` be the positive map dimensions.

| Offset | Bytes | Meaning |
|---|---:|---|
| 0 | 4 | `event_offset`: absolute offset of object/event data |
| 4 | 4 | `texture_offset`: absolute offset of Mode Normal tile data |
| 8 | 2 | map width W (`mp.width`, a signed 16-bit field in C; positive in corpus) |
| 10 | 2 | map height H |
| 12 | 2304 | three raw 768-byte RGB palettes, `rvb[3]` |
| 2316 | 1 | `last_plan1_palette` |
| 2317 (`0x90d`) | `6*W*H` | map records in row-major order |
| `T = 2317+6*W*H` | 1 | static-background selection |
| `T+1` | 1 | scrolling-background selection |
| `T+2` | 4 | `ScrollDiffSprites` object index; -1 means none |
| `X = T+6` | 4 | `x_texture_count` N |
| `X+4` | 4 | `x_code_size` K, encoded code byte count |
| `P = X+8` | `256*N` | Mode X pixel records, every byte XOR 7Dh |
| `P+256*N` | 1 | checksum of preceding **encoded** pixel bytes |
| `G = P+256*N+1` | 4800 | 1200 raw u32 offsets, `gros_patai_src` |
| `C = G+4800` | K | callback code, every byte XOR F3h |
| `C+K` | 1 | checksum of preceding **encoded** code bytes |
| `B = C+K+1` | 4800 | 1200 raw signed 32-bit offsets, `block_add` |
| `B+4800` | variable | Mode Normal data; equals header `texture_offset` in all 82 files |

A map record is `{u16 texture_id; u8 tile_type; u8 field_3; u8 transparency; u8 field_5;}`.
The renderer tests byte +4, not `tile_type` at +2.

The loader's checksum is initialized from the trailing checksum byte, then subtracts each
encoded payload byte modulo 256 while XOR-decoding that byte in place. Success means zero;
equivalently `sum(encoded_payload) & 255 == checksum_byte`. The checksum is not XOR-decoded.
On failure the loader seeks back, reads byte by byte and repeats; a second failure calls FatalError.
Both sections, including zero-length ones and their checksum bytes, pass in every shipped file.
The two pointer tables have no such checksum or XOR.

There is **no fixup-site/relocation stream inside the callback code**. There are two external
offset tables with different bases and sentinel rules:

```text
gros_patai_src[i] = pixel_buffer_base + raw_u32_source_offset[i]   (all 1200 entries)
block_add[i] = NULL                         if raw_s32_code_offset[i] == -1
             code_buffer_base + offset     otherwise
```

The callback code itself is never patched. JNE branches are relative within the body;
stores use EDI/EDX addressing and constants. Source-table values for unused/partial tiles
must not be interpreted as proof that an opaque pixel record exists. The loader adds the
pixel base unconditionally, even to entries outside the allocated pixel extent.
N may be zero while K is nonzero: MUSIC/RAY17.LEV and RAY18.LEV each have six callbacks.

DOS evidence: header/map/palette reads 1:357f2..35927; N/K reads and allocations
1:359ac..35a17; pixel XOR 1:35a4e..35a75; source-table read/rebase 1:35b26..35b5a;
code read/XOR/checksum 1:35b5c..35c5a; callback-table read/rebase 1:35c5c..35ca0.
After Mode X loading, 1:35ca2..35caa seeks to `event_offset`.
Mode Normal loading instead seeks directly to `texture_offset` at 1:35cbc..35cc4.

`file_level` (1:2631c) builds `RAY` + decimal level number + `.LEV`.
DEPART_LEVEL calls it at 1:26ab0, then calls load_level at 1:26ae2.
The load path is `PCMAP\\{jungle,music,mountain,image,cave,cake}\\filename`, selected by
`num_world-1`, optionally prefixed by `..\\`. The C parameter named `world_id` is not
the path selector: the caller supplies the sprite-memory descriptor in EDX and the
filename in EBX; this unused second argument does not affect the file layout.

### Splitting and coverage

Sort distinct non--1 callback offsets. A body's extent ends at the next offset, or K for the last.
In this corpus every non-null entry has its own offset within its level (no pointer aliases),
the first starts at zero, and every extent ends with the fifth RET required by the grammar below.
There are no gaps, padding, unreferenced prefixes, suffix data or internal entry points.
The SHA-256 of every complete body and its tile ID/code offset is in `levels[].routines`.
Identical bytes recur between levels, giving 2,380 distinct bodies from 10,125 level occurrences.
An absent callback is represented by -1, including all 1200 entries in code-empty levels.

Example: CAKE/RAY1.LEV has W=600,H=54,N=232,K=39698; X=196723,
P=196731,G=256124,C=260924,B=300623,texture_offset=305423,event_offset=423612.
Tile 1's callback is at code offset 0, file offset 260924, length 599.

## 3. Renderer and callback calling contract (PROVEN)

The C caller is `sub_3BEE0` at DOS 1:2bee0..2c190, source `src/frame.c`.
It selects a VGA map mask using asm_0283c (port 3C4h, sequencer index 2), then calls one
of the two renderers four times. Phase q=0,1,2,3 is passed as EAX=`q<<8`
(1:2c133..2c140 / 1:2c153..2c160). When the map mask wraps 8 -> 1, the destination
origin advances one byte (1:2c165..2c171). This supplies the four horizontal pixel lanes
even when scrolling rotates the first hardware plane. Source tile x is `q+4*c` for byte column c.

At entry to asm_04430 / asm_04493:

| Register | Meaning |
|---|---|
| EAX | phase in AH; other bits are irrelevant to the callback |
| EDX | first map record pointer |
| EBX | frame-context pointer |
| ECX | relocated `block_add[1200]` pointer |
| EDI | destination byte origin in the currently selected Mode X plane |
| ESI | caller passes background pointer; renderer immediately overwrites it with EDX |

The relevant context offsets are +4 = map-row byte stride (`6*W`), +8 = 1200,
+14h = `mp.map + (W*H-1)` (last record pointer, used as an exclusive row-start stop).
The renderer saves EBP and the original ECX; EBP becomes the callback table,
ESI becomes the current map record, ECX becomes F0h=240. Per row it saves EBX/ESI,
zeros EBX, and visits 20 records. It checks `[ESI+4]==2`, loads the u16 texture ID
into BX with EBX's upper 16 bits zero, loads EDX=`[EBP+4*EBX]`, and skips null pointers.
Otherwise it executes `PUSH EDI; CALL EDX; POP EDI` at 1:04485..04488 / 1:044f6..044f9.

The renderer advances each record by 6 and the destination by 4. After 20 columns,
it advances the map start by context[+4] and destination by context[+8]. Thus one
tile row advances destination by 80+1200=1280=16*80 bytes, not just 1200.
There is no per-tile clipping, width test, or per-record end test here.

asm_04430 draws up to 14 tile rows; its first row has no row-start bounds check.
asm_04493 first advances by one map row and `1200+80` destination bytes, tests the
row-start bound, then draws up to 12 rows. Both stop subsequent rows when the next
map-row start is >= context[+14h]. These are literal loop bounds, including the
last-record comparison; this analysis does not replace them with guessed clipping.

At **callback entry**, after the indirect CALL:

| Register / memory | Callback meaning / requirement |
|---|---|
| AH | selects plane 0..3; any other value draws nothing |
| EDI | byte address D of this tile in selected destination plane |
| ECX | 240, the three-scanline destination increment |
| EDX | address of this callback on entry; overwritten with 80 immediately |
| ESI | map record pointer, preserved; never read by any callback |
| EBX | tile ID with upper 16 bits zero, preserved; never read |
| EBP | table pointer, preserved; never read |
| [ESP] | ordinary 32-bit near return address, read only by RET |
| [ESP+4] | renderer's saved EDI, untouched |

Postcondition: EAX, EBX, ECX, ESI, EBP and segment registers are unchanged;
EDX=80; EDI=D+1200 for a valid phase (even if that phase has no pixels), otherwise EDI=D.
RET pops four bytes: ESP_after=ESP_entry+4 and EIP is the return address.
No other stack access/write, FPU/SIMD operation, port I/O or external call occurs.
OF/SF/ZF/AF/PF/CF are clobbered: valid phase leaves the flags of the fifth `ADD EDI,ECX`;
invalid phase leaves those of the 8-bit `CMP AH,3`. Other flags, including DF, are unchanged.

The caller requires phase, table, map pointer and stride to survive each callback.
It explicitly restores EDI and does not retain EDX. Overall renderers preserve
EAX/EBX/ECX/EBP and modify EDX/ESI/EDI; no arithmetic flag preservation is promised.
Memory is flat readable code/stack and writable destination. The callbacks do not
consult VGA registers, but the hardware destination plane/map mask is an explicit
part of their surrounding drawing context.

## 4. Complete shape catalogue and semantics (PROVEN)

There is one semantic family, `phase_selected_sparse_constant_16x16`.
The **2,070 syntactic shapes** retain mnemonic, register, operand width, address scale
and control-flow target structure while normalizing immediate values and memory displacements.
Thus differences in byte/word/dword packing, store order and skipped stores remain distinct
shapes; different color constants and address displacements are parameters.
Instruction encoding length (e.g. zero vs nonzero displacement encoding) is not a semantic shape dimension.

Every shape's exact normalized store schedule, parameter ranges, representative location,
distinct-body count and per-level counts are in `shapes.json.shapes[S...]`.
Every shape inherits the **same** description, register contract and proof below;
there is no unexplained residual category. `proof.json.shape_executions` gives a nonzero
execution count for each of all 2,070 IDs. Per-level `shape_counts` counts callback entries
(equal to distinct level-body counts here); reverse counts are also stored in each shape's `levels`.

### Exact grammar

```text
MOV EDX,80
for q in [0,1,2,3], sequentially in the instruction stream:
    CMP AH,q
    JNE next_phase
    group 0 stores
    ADD EDI,ECX
    group 1 stores
    ADD EDI,ECX
    group 2 stores
    ADD EDI,ECX
    group 3 stores
    ADD EDI,ECX
    group 4 stores
    ADD EDI,ECX
    group 5 stores
    RET
next_phase:
RET                         # only reached when AH is outside 0..3
```

Each store is `MOV byte/word/dword ptr [EDI + m*EDX + c], immediate`, m in {0,1,2},
c in {0,1,2,3}, c+store_width <= 4. A missing EDX index means m=0.
The parser proves that the derived row `3*group+m` is in 0..15 and each written
pixel has exactly one writer per phase. Consequently group 5 only writes row 15.
Groups may be empty, including all six groups of an otherwise empty phase.
There are exactly four CMPs, four forward JNEs, twenty ADDs and five RETs per body.
Every JNE lands at the next CMP or the final RET, never in a store or another body.

The compact catalogue encoding `phase_store_schedules[q][group]` is a space-separated
sequence such as `W0 B0 D1 D2`: B/W/D means 1/2/4-byte immediate store; the digit is m.
The six array elements are separated by the five ADDs above. This plus the fixed
wrapper is equivalent to the normalized instruction structure. Actual c, immediates,
encoded branch distances and instruction lengths are recovered from the input, not discarded
when producing tile pixels. `S` plus the first 16 hex digits of SHA-256 identifies the
full normalized instruction list, with branch target instruction indices retained.

### Platform-neutral parameters and model

Each callback supplies 256 constant logical pixels `P[y][x]` and a 256-bit write mask M.
Select q=AH. For y=0..15 and c=0..3, let x=q+4*c:

```text
if 0 <= q < 4 and M[y][x]:
    destination[D + 80*y + c] = P[y][x]
```

All other destination bytes retain their incoming value. There is **no source copy,
destination blending, color lookup, mask read, or runtime test for transparent color**.
The write mask is expressed by the absence of store instructions. Colors are literal
8-bit palette indices from the immediates; word/dword immediates expand little-endian.
This formula describes one selected plane; combining q=0..3 gives the full masked 16x16 tile.

Parameters varying between bodies are the mask, colors, store widths/packing/order and
displacements c. EDX's pitch 80, phase comparisons 0..3 and five EDI advances are fixed.
ECX is an input but the renderer always supplies 240: it is not a freely variable row pitch.
For an out-of-contract ECX value V, the literal address formula would instead be
`D + floor(y/3)*V + (y%3)*80 + c`; the native model/proof uses the actual renderer contract.

The corpus has 1..255 written pixels per whole callback, no callback stores color zero,
70 distinct bodies have an entirely empty phase and 281 have a fully occupied phase.
Routine sizes are 89..806 bytes. A full 256-pixel opaque tile uses another renderer path;
null callbacks include opaque tiles as well as empty/unused tile IDs.

### Why the description follows from the bytes

The fail-closed grammar checks every decoded instruction over each entire extent.
The only memory operands are immediate destination stores; the only explicit register writes
are EDX=80 and five EDI+=ECX on a selected path. CMP/JNE only inspect AH. Unconditional RET
ends each selected path. Expanding the stores yields nonoverlapping pixels within a 16x4 plane.
At ECX=240, each ADD advances exactly three rows, proving the address formula.
This establishes absence of hidden inputs/state, not merely lack of such effects in samples.

Independently, `audit.py` decodes the **separate Mode Normal tile data**:
at `texture_offset`, 1200 u32 tile offsets, then u32 total count, u32 full count,
u32 byte size, then XOR-FFh encoded pixel/mask data and its encoded-byte checksum.
For each callback tile ID, the first 256 bytes at its normal tile offset are its
row-major 16x16 pixels. Replacing absent callback pixels by zero yields **exact equality
for all 10,125 callbacks: 2,592,000 bytes compared**. All normal section checksums pass;
their ends equal `event_offset`. This independently verifies lane order, row order,
color values and zero-as-hole interpretation rather than relying solely on disassembly.

Negative result retained: `gros_patai_src` is not a source bitmap for these callbacks.
For callback tile IDs, 108 offsets are outside the pixel buffer; the other 10,017 are
in range but **all 10,017 differ** from the reconstructed tile. Reading that table for
partial tiles would silently draw unrelated opaque data. Use callback constants or the
independently verified Mode Normal tile data instead.

## 5. Unicorn differential certificate

See [prove.py](prove.py) and [proof.json](proof.json). Default seed 0x54494c45,
five trials/body, phases {0,1,2,3,4,255}. Every one of the 2,380 distinct bodies is
executed, covering all 10,125 byte-identical level occurrences and all 2,070 shapes.
This includes smallest/largest bodies, one-pixel/255-pixel tiles, empty/full phases,
all observed store sizes/offsets, unaligned origins and page-boundary crossings.

Each body gets random source and stack buffers. Each execution gets randomized EAX's
non-phase bits, EBX/EDX/EBP, ESI, arithmetic flags, DF and IF. ESI alternates between a
random source-buffer address and an arbitrary usually-unmapped value. Three destination
trials are random, one is all 00h, one all FFh. ECX is the caller's 240.
The Python model iterates logical pixels, without reproducing x86 store packing/order.

For every execution the harness checks:

* the **complete 16 KiB destination mapping**, including untouched guard bytes;
* a memory-write-hook dictionary of **every byte actually written**, including writes
  that happen to store the existing value; unexpected or duplicate writes fail;
* every data read: exactly the four-byte return-address read, no source/destination/global reads;
* preserved EAX/EBX/ECX/ESI/EBP, exact EDX/EDI/ESP/EIP results;
* arithmetic flags against independent addition/subtraction arithmetic and all other flag bits unchanged;
* complete source and stack buffers unchanged; code and source mappings disallow writes.

Out-of-range q=4 and q=255 test the no-draw path. The static four-equality dispatch proof
covers the other 250 out-of-range AH values as well. A wrong color and a wrong mask are
also deliberately supplied to the model; both must fail with a destination mismatch.
These negative controls alter only a temporary Python model, never oracle bytes.

The final execution counts and exact per-shape counts are stored in `proof.json`.
This does not emulate VGA hardware or a whole game frame: the compared memory is a
single explicit plane, and the surrounding plane selection is specified in section 3.

## 6. Native port requirements

For each of 1200 tile IDs per level retain callback presence separately from tile type.
For present callbacks extract a 16x16 row-major array of palette indices plus sixteen
uint16 row masks, with bit x indicating a write at column x. This completely replaces
the executable callback data. All colors written in this corpus are nonzero, so a
zero-filled 256-byte image alone also encodes the mask, but an explicit mask preserves
the precise specification without relying on that corpus property.

The supplied optional exporter does this directly from the decoded callbacks:

```text
python -B build/workers/tilecode/native_tiles.py --level JUNGLE/RAY1.LEV --out tiles.json
```

Output is confined to this worker directory. Omitting `--level` exports all levels.
The JSON includes source hashes, tile IDs, 256-byte `pixels_hex`, 32-byte `write_mask_hex`
(16 little-endian row masks) and table size 1200. Missing IDs mean absent callbacks,
not necessarily empty tiles. All 10,125 exported pixel/mask records were round-tripped
in memory against their decoded callback models (`delivery_check.json`); no bulk
exported pixels are retained for this report.

A native renderer can use arbitrary destination pitch and plain C:

```c
for (y = 0; y < 16; ++y)
    for (x = 0; x < 16; ++x)
        if (row_mask[y] & (1u << x))
            dest[y * pitch + x] = pixels[y * 16 + x];
```

Keep indexed colors until palette conversion; the callback has no embedded RGB palette.
Retain level palette/selection data, map texture IDs and transparency fields, and the
existing background-before-partial-tiles draw order. Apply the native viewport clipping
outside this tile operation. Ordinary opaque/empty tile handling is a separate path;
the callback table alone is insufficient to recover those tiles.

**Simpler equivalent asset path, proven for every callback:** decode the existing Mode Normal
block data/table and take the first 256 bytes for each callback tile ID, with nonzero
pixels as the write mask. The exhaustive independent comparison above proves that a
port need not execute or even translate the embedded code to obtain these tile images.
The code decoder remains useful as an auditable validation oracle for that conversion.

There are **zero stateful or exceptional callback tiles** to list. For an individual
callback the inputs are phase, destination origin, fixed caller stride and its immutable
extracted tile data. The visible result also retains the incoming destination at holes
and uses the externally selected palette/VGA plane. Those are explicit rendering inputs,
not undiscovered state. No callback-behavior question remains OPEN for this corpus.
No historical generator provenance is claimed (HYPOTHESIS, not needed for the specification).

## 7. Per-level coverage

All non-null entries below are the one described family. `shapes` is the number of
normalized syntactic shapes in that level. Detailed counts for **each individual shape**
are in `levels[].shape_counts` and `shapes[].levels` in shapes.json; every null slot and
every callback tile ID is accounted for. There are 98,400 table slots in total:
10,125 non-null and 88,275 null. All 900,072 map records satisfy the classification:
206,206 type-0, 584,288 type-1, 109,578 type-2. Every type-2 record references a callback;
every type-0/type-1 record references a null entry. This is exhaustive shipped-data evidence,
not an invented validation rule applied to the original executable.

<!-- LEVEL_COUNTS -->

| Level | Callbacks | Shapes | Null slots | Code bytes |
|---|---:|---:|---:|---:|
| CAKE/RAY1.LEV | 85 | 85 | 1115 | 39698 |
| CAKE/RAY2.LEV | 148 | 145 | 1052 | 67422 |
| CAKE/RAY3.LEV | 173 | 170 | 1027 | 78234 |
| CAKE/RAY4.LEV | 0 | 0 | 1200 | 0 |
| CAVE/RAY1.LEV | 85 | 79 | 1115 | 43554 |
| CAVE/RAY10.LEV | 44 | 40 | 1156 | 22391 |
| CAVE/RAY11.LEV | 0 | 0 | 1200 | 0 |
| CAVE/RAY12.LEV | 79 | 74 | 1121 | 41366 |
| CAVE/RAY2.LEV | 139 | 133 | 1061 | 68708 |
| CAVE/RAY3.LEV | 0 | 0 | 1200 | 0 |
| CAVE/RAY4.LEV | 80 | 75 | 1120 | 41612 |
| CAVE/RAY5.LEV | 182 | 174 | 1018 | 89136 |
| CAVE/RAY6.LEV | 111 | 104 | 1089 | 54994 |
| CAVE/RAY7.LEV | 166 | 158 | 1034 | 81766 |
| CAVE/RAY8.LEV | 139 | 134 | 1061 | 63504 |
| CAVE/RAY9.LEV | 81 | 74 | 1119 | 42373 |
| IMAGE/RAY1.LEV | 69 | 52 | 1131 | 33142 |
| IMAGE/RAY10.LEV | 74 | 65 | 1126 | 35954 |
| IMAGE/RAY11.LEV | 0 | 0 | 1200 | 0 |
| IMAGE/RAY12.LEV | 45 | 41 | 1155 | 20928 |
| IMAGE/RAY13.LEV | 29 | 22 | 1171 | 12844 |
| IMAGE/RAY2.LEV | 76 | 64 | 1124 | 35980 |
| IMAGE/RAY3.LEV | 162 | 137 | 1038 | 78213 |
| IMAGE/RAY4.LEV | 0 | 0 | 1200 | 0 |
| IMAGE/RAY5.LEV | 133 | 112 | 1067 | 62355 |
| IMAGE/RAY6.LEV | 88 | 76 | 1112 | 42946 |
| IMAGE/RAY7.LEV | 64 | 52 | 1136 | 30857 |
| IMAGE/RAY8.LEV | 104 | 85 | 1096 | 48559 |
| IMAGE/RAY9.LEV | 99 | 79 | 1101 | 47124 |
| JUNGLE/RAY1.LEV | 214 | 213 | 986 | 96971 |
| JUNGLE/RAY10.LEV | 168 | 168 | 1032 | 78424 |
| JUNGLE/RAY11.LEV | 197 | 197 | 1003 | 92892 |
| JUNGLE/RAY12.LEV | 309 | 308 | 891 | 144139 |
| JUNGLE/RAY13.LEV | 303 | 301 | 897 | 137048 |
| JUNGLE/RAY14.LEV | 206 | 206 | 994 | 93923 |
| JUNGLE/RAY15.LEV | 144 | 143 | 1056 | 61376 |
| JUNGLE/RAY16.LEV | 0 | 0 | 1200 | 0 |
| JUNGLE/RAY17.LEV | 133 | 133 | 1067 | 59427 |
| JUNGLE/RAY18.LEV | 181 | 181 | 1019 | 78851 |
| JUNGLE/RAY19.LEV | 173 | 173 | 1027 | 76105 |
| JUNGLE/RAY2.LEV | 290 | 288 | 910 | 135295 |
| JUNGLE/RAY20.LEV | 119 | 119 | 1081 | 51824 |
| JUNGLE/RAY21.LEV | 79 | 78 | 1121 | 35768 |
| JUNGLE/RAY22.LEV | 0 | 0 | 1200 | 0 |
| JUNGLE/RAY3.LEV | 104 | 104 | 1096 | 50584 |
| JUNGLE/RAY4.LEV | 195 | 195 | 1005 | 90471 |
| JUNGLE/RAY5.LEV | 328 | 326 | 872 | 148193 |
| JUNGLE/RAY6.LEV | 192 | 192 | 1008 | 84471 |
| JUNGLE/RAY7.LEV | 114 | 114 | 1086 | 53932 |
| JUNGLE/RAY8.LEV | 180 | 180 | 1020 | 79541 |
| JUNGLE/RAY9.LEV | 134 | 133 | 1066 | 61162 |
| MOUNTAIN/RAY1.LEV | 136 | 136 | 1064 | 64657 |
| MOUNTAIN/RAY10.LEV | 0 | 0 | 1200 | 0 |
| MOUNTAIN/RAY11.LEV | 53 | 53 | 1147 | 28977 |
| MOUNTAIN/RAY12.LEV | 28 | 28 | 1172 | 14808 |
| MOUNTAIN/RAY13.LEV | 32 | 32 | 1168 | 16513 |
| MOUNTAIN/RAY2.LEV | 116 | 115 | 1084 | 56559 |
| MOUNTAIN/RAY3.LEV | 91 | 90 | 1109 | 46309 |
| MOUNTAIN/RAY4.LEV | 189 | 187 | 1011 | 87634 |
| MOUNTAIN/RAY5.LEV | 122 | 121 | 1078 | 59038 |
| MOUNTAIN/RAY6.LEV | 131 | 129 | 1069 | 63479 |
| MOUNTAIN/RAY7.LEV | 86 | 86 | 1114 | 43114 |
| MOUNTAIN/RAY8.LEV | 190 | 188 | 1010 | 88647 |
| MOUNTAIN/RAY9.LEV | 111 | 110 | 1089 | 55593 |
| MUSIC/RAY1.LEV | 305 | 277 | 895 | 122967 |
| MUSIC/RAY10.LEV | 204 | 185 | 996 | 84854 |
| MUSIC/RAY11.LEV | 159 | 143 | 1041 | 65948 |
| MUSIC/RAY12.LEV | 87 | 78 | 1113 | 37333 |
| MUSIC/RAY13.LEV | 74 | 71 | 1126 | 29444 |
| MUSIC/RAY14.LEV | 259 | 226 | 941 | 104342 |
| MUSIC/RAY15.LEV | 99 | 85 | 1101 | 41075 |
| MUSIC/RAY16.LEV | 0 | 0 | 1200 | 0 |
| MUSIC/RAY17.LEV | 6 | 6 | 1194 | 2540 |
| MUSIC/RAY18.LEV | 6 | 6 | 1194 | 2540 |
| MUSIC/RAY2.LEV | 121 | 105 | 1079 | 50170 |
| MUSIC/RAY3.LEV | 199 | 180 | 1001 | 83980 |
| MUSIC/RAY4.LEV | 130 | 108 | 1070 | 52874 |
| MUSIC/RAY5.LEV | 214 | 191 | 986 | 88866 |
| MUSIC/RAY6.LEV | 150 | 129 | 1050 | 61582 |
| MUSIC/RAY7.LEV | 161 | 148 | 1039 | 66414 |
| MUSIC/RAY8.LEV | 193 | 176 | 1007 | 78622 |
| MUSIC/RAY9.LEV | 255 | 228 | 945 | 102179 |
| **Total** | **10125** | **2070 distinct across levels** | **88275** | **4595115** |

Final proof: **71,400 executions**, **1,835,470 byte writes** from **752,665 store instructions**; 47,600 valid-phase and 23,800 out-of-range-phase runs. Both negative controls rejected.
