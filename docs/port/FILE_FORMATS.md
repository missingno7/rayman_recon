# Loader file formats — DOS GOG v1.21

These are the fields and operations actually used by `src/load.c`, not proposed native C structures. All multibyte file integers are little-endian. Signedness below describes DOS use; an untyped file word has no intrinsic signedness. Pointer-shaped file words are **32-bit file values**, never host pointers. Variable sections have no implicit C alignment padding.

Primary evidence: [load.dos.txt](evidence/load.dos.txt), [file_formats.json](evidence/file_formats.json), [archive_formats.json](evidence/archive_formats.json), and the independently pinned layout assertions in the proposed semantic checker. Numeric labels in the grammars are descriptive specification names, not claims about historical source spelling. All 82 LEV, six WLD, ALLFIX, BRAY, VIGNET and LNG files were read from `assets/GOG` without modification. The 92-file parse reaches every EOF and validates 488 encoded-payload checksums. Corpus membership alone does not prove a resource is reachable.

## Common records and encoding

| Record | Bytes | File representation |
|---|---:|---|
| Sprite | 12 | `u32 atlas_offset; u8 id,outer_width,outer_height,inner_width,inner_height,inset,color,flags` |
| Animation layer | 4 | `u8 mirrored,x,y,sprite_index` |
| Frame rectangle | 4 | `u8 x,y,width,height` |
| Eta/substate | 8 | `s8 speed_x_right,speed_x_left; u8 anim_index,next_main,next_sub,anim_speed,sound_index,flags` |
| Map tile | 6 | `u16 texture_id; u8 tile_type,field_3,transparency,field_5` |
| RGB entry | 3 | Three component bytes, no alpha |
| Palette | 768 | 256 RGB entries |
| Level instance | 132 | Packed DOS `obj_t`; full offset/width table below |

The sprite inset at +9 has low-nibble x and high-nibble y. The palette selection is the high nibble at +10; sprite horizontal flip is bit 2 at +11. The loader reads sprite records unchanged: `offset_in_atlas` stays an offset, and consumers add `img_buffer` at use time. `SpriteFixeOffset` does **not** relocate sprite records; it records an arena offset.

Eta `anim_speed` low nibble selects the animation clock; the high nibble is a state-dependent gravity/control selector. `flags` bit0x10 controls reverse animation. These are the earlier basic/moteur/ray/collision contracts, retained here. The map renderer's transparency byte is +4, distinct from collision `tile_type` +2. Bytes +3/+5 remain UNKNOWN.

For checked encoded payloads, the DOS operation is:

```text
residual = stored_checksum                  // u8
for each encoded byte b:
    residual = (residual - b) mod 256
    decoded_byte = b XOR key
success iff residual == 0
```

Thus the checksum is `sum(encoded_payload) & 255`, not the sum of decoded bytes. The trailing checksum itself is not XOR-decoded. Atlas/tile failure handling seeks back and retries by sequential one-byte reads, then calls `FatalError` if the second residual is nonzero. PCX retry differs; see its section.

## Animation and eta grammars

An animation in a resource stream starts with **10 bytes**, unlike the 12-byte runtime `anim_t`:

| Relative offset | Bytes | Meaning/use |
|---|---:|---|
| +0 | 2 | `u16 layers_per_frame`: retain all bits; low 14 are indexed layer count, top 2 encode the speed-clock period minus one |
| +2 | 2 | `u16 frames_count` |
| +4 | 4 | Raw producer frame pointer/presence value; signed 32-bit `-1` means no following frame-rectangle array |
| +8 | 2 | `u16 layer_payload_bytes` |
| +10 | `layer_payload_bytes` | Layer payload, allocated and read in full |
| following | `4*frames_count` or 0 | Frame rectangles, present when the raw +4 word is not `-1` |

No serialized `layers` pointer is read. The runtime layer pointer is the new allocation. Any non--1 frame word is discarded and replaced by a new frame-array pointer; `-1` remains the sentinel. All 1, 629 shipped headers take the present-array branch, so the absent-array branch is established by DOS instructions, not by corpus occurrence.

Every shipped layer payload has **four bytes more** than `4*(layers_per_frame & 0x3fff)*frames_count`. Those four bytes remain uninterpreted storage. They are part of the read/allocation and must not be counted as another indexed layer. No alignment or padding explanation is asserted.

An eta-bank set is:

```text
u8 bank_count
repeat bank_count:
    u8 main_state_count
    repeat main_state_count:
        u8 substate_count
        eta[substate_count]                 // 8 bytes each
```

The runtime representation is nested allocations of 32-bit pointers and eta records. No pointers occur in this file grammar. Counts are independent bytes; they are not a rectangular table. Different actors may interpret the same `(main,sub)` numbers differently. Rayman's named states remain in the earlier `ray.json` / `RAY_STATES.md`.

## WLD — `load_world`, 1:34de0

Path: optional `..\` then `PCMAP\filename`; `DEPART_WORLD` supplies the filename. Header:

| File offset | Bytes | DOS use |
|---|---:|---|
| 0 | 2 | Unsigned background width |
| 2 | 2 | Unsigned background height |
| 4 | 1 | Number B of world background selections |
| 5 | 1 | Raw byte stored in `BiosCheckSum`; intended use UNKNOWN |
| 6 | 1 | Raw byte stored in `VideoBiosCheckSum`; intended use UNKNOWN |
| 7 | B | Each byte XOR0x15, stored in signed 8-bit `Plan0NumPcx` |
| 7+B | 2 | Signed 16 world-only descriptor count D |

The descriptor stream has D records, assigned to `wldobj[nb_fix_des .. nb_fix_des+D-1]`:

```text
u8 always_used                             // loading gate, not the object-type always-list bit
s32 atlas_byte_count                       // stored in wldobj[index].x
u8 encoded_atlas[atlas_byte_count]          // XOR8f
u8 encoded_atlas_checksum
u16 sprite_count_word                      // stored in signed16 nb_sprites
sprite[sprite_count_word]
u8 animation_count
animation[animation_count]                 // grammar above
```

WLD uses the signed `nb_sprites` member in allocation/read/skip arithmetic. In the shipped data these counts are positive; the parser records the raw word without changing DOS signedness. The atlas count is a byte length, despite the resource holder using `obj_t.x` to store it.

Payloads load when `always_used != 0 || ToutSpriteAutorise != 0`. Otherwise DOS seeks past the atlas, reads the checksum without validating it, reads the counts and seeks past sprite/layer/frame data. It sets atlas/sprite pointers to NULL; nonzero animation counts also get NULL animations. A **zero animation count aliases slot 0's animations and count in both branches**. This is different from ALLFIX. The optional flag does not remove descriptor indices from the stream.

An eta-bank set follows all descriptors. World banks append at `nb_fix_eta`. The loader adds the fixed counts to the just-read world counts; `nb_des` is signed 16-bit and `nb_loaded_eta` unsigned 8-bit. WLD descriptors themselves do not contain an eta-bank index. Level instances select eta banks independently.

Before the read, all ten `Plan0NumPcx` slots become -1. `MODE_NORMAL` allocates background buffers using `2*width,height`; ModeX allocates `width,height` only when backgrounds are enabled. ModeX also allocates a 71, 680-byte `PLAN1_BUFFER`; other modes set it NULL. The first decoded background resource is loaded unless `FondAutorise == 2`.

Corpus: six WLD files, descriptor counts 24/26/23/26/32/24; eta counts 23/26/21/25/31/24. Width is 384 throughout; height is 288 except RAY6.WLD's 289. Background-slot counts are 5..8, within the declared ten slots. The two raw header bytes are loaded but no intended checksum algorithm/reader is established. Nearby widened-load references are retained as negatives, not promoted to consumers.

## ALLFIX.DAT — `LOAD_ALL_FIX`, 1:364bc

Path: optional `..\` then `PCMAP\ALLFIX.DAT`. The loader first initializes sprite, level, world and fixed arenas from their existing unsigned 32-bit capacity globals.

```text
eta_bank_set                               // copied count to nb_fix_eta
s16 descriptor_count N                     // copied to nb_fix_des; includes reserved slot0
repeat index = 1 .. N-1:
    s32 eta_bank_index                     // full32 -1 => NULL; otherwise loaded_eta[index]
    u32 raw_RaymanExeSize
    u32 raw_RaymanExeCheckSum1
    s32 atlas_byte_count
    encoded_atlas[atlas_byte_count]         // XOR8f
    u8 encoded_atlas_checksum
    u32 raw_RaymanExeCheckSum2
    u16 sprite_count_word
    sprite[sprite_count_word]              // this loader explicitly casts nb_sprites to u16
    u8 animation_count
    animation[animation_count]
u32 raw_RaymanExeCheckSum3
s32 template_descriptor_indices[7]
```

The four named raw 32 fields are distinct from the 8-bit atlas checksum. Their intended use and algorithms remain UNKNOWN: the whole-binary scan found the loader destinations, not demonstrated readers. The repeated shipped values are 427907, 3178074698, 1866446967, with tail value 952344370. No interpretation as the current EXE's physical size or computed checksum is asserted.

Fixed eta/animation/sprite metadata uses the fixed arena; atlas bytes use the sprite arena. With zero animation count, `animations = NULL`; no slot 0 alias. The shipped file has four eta banks, descriptor count 8 and seven actual descriptors. Their eta indices are `[0,1,0,2,-1,-1,3]` for slots 1..7.

Tail order and shipped indices:

| Consumer | Index | Members copied |
|---|---:|---|
| ray | 1 | Whole 132-byte resource-holder record |
| alpha | 5 | sprites, img_buffer, nb_sprites |
| alpha 2 | 6 | Same font resource subset |
| raylittle | 3 | sprites, img_buffer, nb_sprites |
| mapobj[0..23] | 4 | sprites, animations, nb_sprites, anim_count, img_buffer, eta |
| clockobj | 7 | Whole 132-byte record |
| div_obj | 2 | Whole 132-byte record |

`mapobj` is declared with 25 entries, but this loop initializes resource members only for 0..23. No larger capacity or purpose for entry 24 is inferred. After the file, the loader allocates 0x4b0/1200 bytes for `new_sprite_deform` and records the sprite arena offset. None of these template bindings performs `first_obj_init`/`obj_init`.

## BRAY.DAT — `load_big_ray`, 1:36078

Path is exactly `PCMAP\bray.dat`; this function does not apply `path_has_changed`.

```text
s32 atlas_byte_count
encoded_atlas[atlas_byte_count]             // XOR8f; NO checksum byte
u16 sprite_count_word                      // stored/used as signed16 nb_sprites
sprite[sprite_count_word]
u8 animation_count
animation[animation_count]
eta_bank_set
```

There is no WLD loading-gate byte, ALLFIX eta index, raw EXE fields, or atlas checksum. All allocations use the supplied arena. The descriptor is appended at the old `nb_des`; eta banks replace `loaded_eta` from bank 0, rather than append at `nb_fix_eta`. The descriptor eta is forced to bank 0, its whole 132 bytes are copied to `bigray`, then `nb_des` increments. `nb_fix_eta` is not updated.

Shipped BRAY: atlas 81, 816 bytes, 76 sprites, 7 animations, one eta bank with two main-state groups of 3 and 4 substates. No atlas integrity check is performed by this loader.

## LEV — `load_level`, 1:35688

Path: optional `..\`, then `PCMAP\{jungle,music,mountain,image,cave,cake}\filename`, selected by `num_world-1`. The caller obtains the level filename with `file_level`. The parameter currently spelled `world_id` is unused; it does not select the directory. `DEPART_LEVEL` supplies a sprite-arena pointer in its second argument register; no declaration/ABI change is proposed.

| File offset | Bytes | Meaning/use |
|---|---:|---|
| 0 | 4 | Signed 32 absolute event/object-section offset E |
| 4 | 4 | Signed 32 absolute ModeNormal texture-section offset Q |
| 8 | 2 | Signed 16 map width W |
| 10 | 2 | Signed 16 map height H |
| 12 | 2304 | Three raw palettes; no XOR/divide 4 |
| 2316 | 1 | Palette index; copied selection `rvb[index]` to current_rvb; current_pal_id set 0 |
| 2317 | 6*W*H | Raw row-major map records |
| T=2317+6*W*H | 1 | Static **world background slot**, not a VIGNET resource ID |
| T+1 | 1 | Alternate differential-scroll world background slot |
| T+2 | 4 | Full signed 32-bit ScrollDiffSprites descriptor index; -1 means NULL |

A non--1 scrolling-sprite word selects `&wldobj[index]`; only Normal+ScrollDiffOn builds masks. Static slot is used by ModeX or when differential scrolling is off; alternate slot by Normal with it on. Assignment narrows to signed 8-bit `no_fnd`; later `LOAD_FND` maps it through `Plan0NumPcx` to a VIGNET ID.

### ModeX section at T+6

```text
u32 x_texture_count A
u32 encoded_callback_bytes K
encoded_pixel_records[256*A]               // XOR7d
u8 encoded_pixel_checksum
u32 raw_pixel_offsets[1200]
encoded_callbacks[K]                       // XORf3
u8 encoded_callback_checksum
s32 raw_callback_offsets[1200]
```

Zero lengths still have a checksum byte. The rebinding rules differ:

```text
gros_patai_src[i] = pixel_allocation_base + raw_pixel_offsets[i]    // unconditionally
block_add[i] = 0                              if raw_callback_offsets[i] == -1
             = callback_base + raw_offset     otherwise
```

There is no loader fixup stream or patching of callback instructions. Some source-table entries are outside the pixel allocation for partial/unused tiles; absence must not be inferred from that table. Nonempty callback sections can coexist with zero pixel records. After these reads DOS seeks to E.

The already-established native tile interpretation is in the canonical [TILE_CODE.md](../../../docs/port/TILE_CODE.md), sections 2/7 (repository-relative `docs/port/TILE_CODE.md`). It proves that the asset callbacks are sparse constant painters and correspond to the separate Normal pixel data; pass 4 reuses that work, without executing or reconstructing the callbacks again.

### ModeNormal section at Q

```text
u32 raw_tile_offsets[1200]                  // read into the same s32 block_add storage
u32 total_block_count
u32 full_block_count
s32 encoded_texture_bytes M
encoded_texture[M]                         // XORff
u8 encoded_texture_checksum
```

DOS passes decoded texture bytes to `construct_MAP`; the offset table stays offset-based here. In all 82 shipped levels, this section follows the ModeX tables exactly and ends at E. Normal loading arrives at the event section sequentially; it does not seek to E afterward. Tile bitmap details are reused from `TILE_CODE.md`; no new interpretation of compressed tile forms is invented here.

### Event/object section at E

```text
s16 instance_count C
s16 link_successor[C]                      // allocated with2*C+1 bytes, reads only2*C
obj_t serialized_instances[C]              // exactly132*C bytes
repeat each instance in array order:
    u16 command_byte_count
    u16 label_count
    u8 command_bytes[command_byte_count]
    s16 label_offsets[label_count]         //2 bytes each
```

All instances precede all script/count payloads. Zero command/label counts set their corresponding pointers NULL. Nonzero counts allocate fresh arrays, overwriting the serialized producer-pointer words at +0x10/+0x14. Label words are not pointer relocations. The loader does not validate file-read counts, descriptor/table bounds or the instance count against a host capacity; this document establishes the shipped format, not a malformed-input guarantee.

The first four instance fields are **independent** indices encoded in pointer-shaped words:

| Instance offset | Serialized use | Runtime replacement |
|---|---|---|
| +0x00 | Low signed 16-bit of raw 32 descriptor index | wldobj[index].sprites |
| +0x04 | Low signed 16-bit of raw 32 descriptor index | wldobj[index].animations |
| +0x08 | Low signed 16-bit of raw 32 descriptor index | wldobj[index].img_buffer |
| +0x0c | Low signed 16-bit of raw 32 eta-bank index | loaded_eta[index] |
| +0x10 | Ignored producer command pointer | New command allocation or NULL |
| +0x14 | Ignored producer label pointer | New label allocation or NULL |

There is no instance `eta == -1` special case. The signed 16-bit narrowing is proven by MOVSX/SAR 16 at 1:35f25..35f81. This differs from the full 32 ScrollDiffSprites and ALLFIX eta-index rules. Resource pointer rebinding does not refresh the instance's serialized `nb_sprites` or `anim_count`.

The remaining DOS instance layout is:

| Offset | Representation/member |
|---|---|
| 0x18 | One declared 4-byte command-context view: s16 cmd_offset, count |
| 0x1c | u32 field_1C activation/restore scratch |
| 0x20 | u32 linked-group marker under existing link_has_gendoor name |
| 0x24 | s32 is_active |
| 0x28, 0x2c, 0x30 | s32 x, y, active_flag |
| 0x34..0x56 | Consecutive s16: id, screen_x, screen_y, sub_speed, init_x, init_y, speed_x, speed_y, nb_sprites, cmd_offset, nb_cmd, follow_id, follow_y, follow_x, param, link, ray_dist, iframes_timer |
| 0x58..0x60 | Consecutive u16: test_block_index, scale, zdc, active_timer, type |
| 0x62..0x66 | Five u8 block samples: centre, left, right, above, below |
| 0x67 | Unknown byte under existing unused_padding name |
| 0x68..0x7b | u8: offset_bx, offset_by, anim_index, anim_frame, sub_etat, main_etat, init_sub_etat, init_main_etat, cmd, gravity_value_1, gravity_value_2, change_anim_mode, offset_hy, follow_sprite, hit_points, init_hit_points, init_flag, hit_sprite, detect_zone, detect_zone_flag |
| 0x7c | s8 cmd_context_depth |
| 0x7d..0x80 | u8 config, display_prio, timer, anim_count |
| 0x81..0x82 | Packed flags: bits 0..7 hurtbyfist, command_test, alive, flip_x, read_commands, follow_enabled, flag_0x40, anim_changed; next byte bit 0 flag_0x100 |
| 0x83 | UNKNOWN byte field_83 |

Flags/views have exact masks in `types.json`. Other bits of byte0x82 are not named by this C view; do not synthesize fields. The command-context declaration is recorded as it exists; this pass does not infer a different capacity or change overlaps/TU layout. Every instance byte is physically serialized, including runtime scratch. Physical serialization does not establish that its producer value is used: see [object_lifecycle.json](evidence/object_lifecycle.json) and `LOAD_REPORT.md` for reset/rebinding.

Corpus:11, 837 instances across 82 levels, all event sections reach EOF after the final script. Original `link_init` values are 0..253 in this corpus. These are sample domains, not runtime bounds or declaration capacities.

## VIGNET.DAT — PCX readers

VIGNET is concatenated encoded payloads. Its table is **in the executable**, at 2:0215c, 75 entries of 12 bytes. The pre-existing load.c declaration says 71, matching Android's table count rather than DOS. The earlier data audit and all 75 independent file checks establish the DOS count; this proposal preserves the source declaration and records the distinction. No archive table/count is read from VIGNET itself.

An executable table entry is `{s32 offset; s32 size; u8 xor_byte; u8 checksum_byte; u8 reserved[2];}`. Offset/size delimit a payload; the XOR key applies throughout that payload, and checksum covers its encoded bytes. The reserved bytes have no scoped archive reader/meaning. Entries exactly partition the shipped file starting at offset 0.

Each decoded payload has a 128-byte PCX header, RLE body, byte0x0c and 768 palette bytes. The loader consumes these header fields:

| Header offset | Bytes | Use |
|---|---:|---|
| +4,+6 | 2 each | Unsigned min_x, min_y |
| +8,+10 | 2 each | Unsigned max_x, max_y |
| +66 | 2 | Unsigned decoded bytes per scanline |

Width=`max_x-min_x+1`, height likewise, narrowed to16bits. Other header bytes are checksum/XOR processed but not interpreted or validated by these routines: +0..3, +12..65 and +68..127. Their inherited PCX-oriented C names do not prove an additional runtime use.

RLE: a byte with top bits 11 has run length `byte & 0x3f`; consume the next value byte and emit that many copies. Other bytes emit one value. The loop ends each row when emitted x reaches `bytes_per_line`, and ends the image after decoded height rows. It does not use a general PCX plane/bit-depth decoder. All 75 shipped headers have bits-per-plane 8, one plane and bytes_per_line==width; their row decodes land exactly at the final marker/palette. These observations explain why a width-pitched output allocation works for this corpus, not for arbitrary PCX inputs.

`LoadPcxInVignet` allocates width*height pixels; `LoadPcxOptions` uses its caller's buffer. Both read/XOR/check the whole payload, including the palette, but their pixel loop consumes only image RLE. `LoadPcxPaletteInVignet` instead seeks directly to offset+size-768, reads only 768 bytes, XORs and divides each unsigned component by 4. It does not check either checksum or marker.

`LoadPlan0InVignet` has additional modes:

- FondAutorise 0 returns immediately. FondAutorise 2 uses EffetBufferNormal for compressed input and DrawBufferNormal for output; body size is capped at 128000, and checksum-failure handling is skipped when truncated.
- FondAutorise 2 or P486==1 crops to at most320x200. For tall non-truncated images it decodes/discards the upper rows and retains the bottom 200; truncated input sets crop 0.
- Otherwise ModeNormal duplicates every scanline horizontally, while ModeX writes one copy and calls the plane converter. The Normal path ultimately publishes PLAN0=PLAN0BIT and plan0_width=2*decoded_width; this pass records the existing pointer publication without changing buffers or drawing code.

`allocate_PLAN0(W,H)` allocates W*H and 2*W*H+8*W bytes, or uses existing draw/effect buffers when FondAutorise 2. The ModeX converter uses `L=W*H`, `offset=(x&3)*(L>>1)+(x>>2)+y*(W>>1)`, writes a second copy at offset+W/4, and extends the first 16 rows past 2*L. Final stride parameter is W>>1 and span 2*W*H. `LoadPlan2InVignet` publishes a different presentation contract: PLAN2BIT aliases unconverted PLAN2, stride metadata W/4, length W*H.

### Confirmed corruption-path quirks

The three image readers' checksum-failure header rereads perform 128 **one-byte** reads at destination addresses spaced 128 bytes apart, until header_base+0x4000. This is in the original DOS instructions:1:36fd1..36fee, 1:37663..3767b, 1:38248..38265. It is not a normal sequential retry and overwrites beyond the 128-byte header.

LoadPcxInVignet and Options reset their retry residual from the table. LoadPlan 0 does not: it continues subtracting into its first-pass nonzero residual (stack byte+0xc4, 1:37630..377f9). Retry paths also allocate replacement input/output buffers without first freeing the earlier allocations. These are static DOS facts. The valid shipped corpus takes none of these failure branches; no successful recovery behavior or safe malformed-file behavior is inferred. Pass 4 preserves the source and compiled output, rather than repairing these paths.

## RAY.LNG — `LoadLanguageTxt`, 1:37e10

Path is `RAY.LNG`, without the parent-prefix switch. Its five descriptors are executable data at 2:024e0, with the same 12-byte offset/size/key/checksum layout. They exactly partition the shipped file. There is no descriptor table, language count, string count or pointer table in the file itself.

The selected block is read into arena storage, checksum-subtracted over encoded bytes, then XOR-decoded in place. On a checksum failure this loader resets the residual and rereads bytes sequentially. Text remains **raw 8-bit glyph bytes**; the loader performs no Unicode/codepage conversion, quoting or escape processing.

`GetStringTxt` scans until comma (0x2c) or asterisk (0x2a). It copies bytes>=space to a 400-byte scratch buffer, ignoring control bytes. At comma it terminates that scratch output and returns comma+1; at asterisk it returns NULL without appending a scratch terminator. The caller discards the copied text and uses only the cursor:

```text
string_start = block_start
scan_cursor = block_start
for each non-NULL GetStringTxt result:
    language_txt[u16_index++] = string_start
    result[-1] = 0                         // comma -> NUL in original block
    string_start = result + 2
    scan_cursor = result                  // scan and stored-string starts differ
```

Thus commas delimit strings and `*` ends the scan; the next stored start blindly skips two bytes after each comma. Most observed separators are CRLF; one language 0 separator is two tabs. There is no whitespace normalization of the backing strings. All five blocks yield 237 pointers, with maximum scratch demands 60/68/75/39/33 bytes including NUL. The existing pointer-table minimum 237 is sufficient for this observed use; its unproven upper capacity is deliberately not investigated or changed.
