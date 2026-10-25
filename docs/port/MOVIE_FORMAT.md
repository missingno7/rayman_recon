# Movie parser and presentation contract

GOG v1.21 code accepts AF11h/AF12h. Compact files use `u32 size_marker=12; u16 magic; u16 frame_count,width,height`, then frames at byte 12. The loader still reads128 bytes initially, so depth/flags and other ordinary-header slots overlap frame bytes in this form; they are not compact-file fields. Compact FLC sets speed 30; compact FLI sets5 then multiplies1000/70. Standard FLI begins128; standard FLC uses+50h frame offset. Dimensions must be<=320x200. Header speed is not consumed by the presentation pacing code.

Frames:16-byte prefix `u32 total_size; u16 magic=F1FAh; s16 chunks; u8 unused[8]`. Chunks: `u32 total_size; u16 type`, payload at+6; size determines the next record. Type4 RGB8 palette (components>>2),11 RGB6 palette,7 word delta,12 byte-line delta,13 clear all 64000 bytes,15 BRUN,16 raw rows; others skipped. Palette packet run0 means256 entries. BRUN ignores stored packet count and expands to width. Byte delta negative runs repeat bytes; word delta signed8 runs scale by 2 and negative repeats words.

DOS word-delta odd-byte control uses the unusual call x=low control byte,y=rightmost x,color=low current y (1:2a907..2a91a). Preserve this observed contract separately from any conventional FLC specification. No such controls occur in the two secondary movie files. Source copies preserve signed8 narrowing for extreme byte runs; malformed-domain behavior is not newly validated.

Drawing centers at((320-width)/2,(200-height)/2) with 320-byte stride. All frames decode. Selected frames wait for unsigned now>deadline, synchronize, output dirty palette and copy the movie rectangle. Scale1200h=4608 gives deadline additions276(60 request) or576(125 request). BIOS/PIT counter is fraction+((CL<<24)+((DH<<8)+DL)<<8), CH omitted; it is not a millisecond clock. Fast path presents every frame; processor4 or Windows-selected slow path toggles presentation each decoded frame. Final fade has32 synchronized steps, subtracting2/clamping0 from every VGA6 component.

Intro accompaniment track 20; conclusion21. CD is started then paused before playback; frame 1 can resume. For intro, pause only within[start,end) and when packed current MSF reaches threshold. Pause advances row index before the resume threshold is read. At exact end frame the row can advance without pause; index 5 becomes signed-1.

| Row | Packed MSF threshold hex | Resume frame | Pause start | Pause end |
|---|---|---|---|---|
| 0 | 0xf00 | 1 | 215 | 223 |
| 1 | 0x1f00 | 223 | 412 | 468 |
| 2 | 0x10128 | 468 | 890 | 920 |
| 3 | 0x11400 | 920 | 1170 | 1195 |
| 4 | 0xffffff | 1195 | 1800 | 1805 |

| Secondary file | Header frames | Frames to EOF | Geometry | Chunk counts |
|---|---:|---:|---|---|
| assets/DOS/CONCLU.DAT | 242 | 243 | 240x150 | {'18': 1, '4': 243, '15': 1, '16': 201, '7': 40} |
| assets/DOS/INTRO.DAT | 1804 | 1805 | 240x150 | {'18': 1, '4': 1774, '15': 1, '7': 1408, '16': 365} |

Both secondary files have one additional EOF frame. One-shot playback consumes header frame_count frames; looping decodes the first frame once, then header frame_count frames starting at the second-frame offset. No historical label is inferred for the extra frame. assets/GOG contains no movie files here; this corpus is corroborative, not a GOG asset-domain proof.

Native replacements: filesystem path/descriptor wrapper, indexed surface, monotonic timing policy and track playback API. Keep input poll placement, return/error codes(-3 dimensions,-4 frame signature,-5 open,-6 DOS read,-11 format,-13 CPU selection), palette mutation and frame/CD phase behavior. _dos_read success does not check actual byte count; no new short-read rejection is attributed to DOS.
