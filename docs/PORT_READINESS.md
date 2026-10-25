# Port readiness: register of everything not yet known (started 2026-10-24)

Goal (user, 2026-10-24): before the SDL3 port, the DOS game must be fully understood - no missing behaviour, no
unexplained bytes, no guessed data. STRICT matching stays the verification standard for the reconstruction; this
register tracks what is still UNKNOWN, independent of matching. Each item: measured size, how it is closed, state.
Numbers from main 0fb458b (tools/status.py, build/verify/report.json, build/near_census.json).

## 1. Code behaviour
| item | size | closing method | state |
|---|---|---|---|
| STRICT functions | 1,579 (289,141 B game code) | byte-identical to DOS: behaviour proven by identity | DONE |
| NEAR game functions, census-flagged (DIFF/CALL_ORDER) | 43 | DOS-disassembly triage (sem_triage, sem_a3): 42 EQUIVALENT, MOVE_OBJECT UNRESOLVED (B20) | DONE except B20 |
| NEAR game functions, SHAPE / REFS_LAYOUT | ~200 (~166 KB) | same calls/globals/fields/constants (census) plus differential execution (item 1a) and semir (docs/CAUSES.md H14) | DONE (EQUIVALENT_TESTED, see 1a) |
| 1a. differential execution | all 242 NEAR game functions | tools/difftest (outputs build/difftest): DOS function vs our compiled function from identical machine states in Unicorn; compares writes, calls with arguments at declared width, memory at each call, output parameters by identity, return value; evidence-backed input domains (tools/difftest/domains); file layer reads the REAL game data (assets/GOG levels, VIGNET.DAT, RAY.LNG, ...; movies from assets/DOS). Final sweep 2026-10-24: 231 EQUIVALENT_TESTED, 8 OUT_OF_DOMAIN_DIFFERENCE (equivalent inside the game's domain), 3 DIVERGENT, all triaged (MOVE_OBJECT = B20 original uninitialised read; RAY_BALANCE store order unobservable; allocatePirateGuetteurBomb harness artefact), 0 HARNESS_LIMIT. Three real behaviour bugs found and fixed on the way (c112f95). Controls: STRICT zero noise, all mutants caught (incl. file-layer offset/size/XOR mutants). Limits: loader coverage 54-90% (real files never take checksum-failure branches); PcMain/DoEffectBloodOut/movie helpers compared up to a fixed call boundary; selector model and BIOS tick are HYPOTHESIS-level environments | DONE |
| code-range gaps between bound functions | 146 gaps, 3,071 B | gap_code 2026-10-24: 1,633 B reachable tails of NEAR functions longer than our candidates, 658 B padding, 492 B private _TEXT initialisers, 288 B switch tables; **0 unexplained**. Found one real data bug: select_level_prg `num[6]` was 0x1100,0x1000 instead of 17,16 (fixed); the verifier now checks CODE prefixes | DONE |
| original hand-written assembly | 109 public entries + 9 private helpers, all STRICT as asm | asmspec 2026-10-24: portable specification of every routine (contract, record layouts, computation; hardware access as abstract effects with the exact port/value sequences) in docs/port/ASM_SPEC.md + asm_spec.json, each proved against an independent Python model in Unicorn (tools/portspec/asm: 30,208 executions, 1,237/1,239 blocks covered, 116 entries at 100%, 12/12 negative controls caught, 0 mismatches). OPEN: BIOS/PMODE/W real-mode pointer translation (bios_wait_timer), reachability of one deformation store | DONE (2 OPEN) |
| x86 code shipped inside level files | 82 levels, 10,125 callbacks | tilecode 2026-10-24 (docs/port/TILE_CODE.md, proofs tools/portspec/tiles): 2,380 distinct bodies / 2,070 normalised shapes, ONE semantic family - sparse constant tile painters (write fixed pixel values at fixed offsets of the destination); no callback reads a source buffer, global, timer, random state or modifies itself; zero unexplained bytes. 71,400 Unicorn executions against a Python model passed and all 10,125 tiles equal the levels' separate Mode-Normal tile bitmaps. Port: draw tiles from the bitmap data, never run the code. Evidence EQUIVALENT_TESTED (+ exhaustive static grammar check) | DONE |
| stubs in src | 1 found | LOAD_CONTINUE_SCREEN had "//stub: there is a DRM check here"; DOS check now implemented (B16). Other TODO markers are inherited Rayverse call-site notes in functions whose callees exist (STRICT or covered by item 1a) | DONE |

## 2. Known behaviour questions
| item | state |
|---|---|
| B19 oldest_planted: fewer than 2 seed objects -> uninitialised ng_ids read | RESOLVED (gap_code): only Jungle 9 enables the seed power and it has exactly two type-39 objects - the case cannot occur with the shipped levels |
| B20 MOVE_OBJECT: fist path writes a bit computed from uninitialised EDI/[ESP] | STRONG (gap_code): every reader of obj+0x82 masks the bit away; no gameplay or save-data consumer found - observably dead |
| LoadTchatchPerdu -> PlayTchatch reference 5 bytes off although PlayTchatch is STRICT | EXPLAINED (gap_code): the table carried by LoadTchatchPerdu is PlayTchatch's switch; its sound-5 entry targets a different basic block in our build |
| box CalcTab constant (B17) | value known exactly (1/9.5 as stored in DOS); behaviour known; only the C spelling is blocked |
| protection sequences (B16: INIT_OPTIONS_CHOICE, FIN_MAP_LOOP, LOAD_CONTINUE_SCREEN) | bytes exact; the port must document what each check does and what the game does on failure |

## 3. Data
| item | size | state |
|---|---|---|
| initialised _DATA with named, sized bindings | 184,623 of 186,776 B (98.8%) | gap_data 2026-10-24: 8,864 B were 31 known objects without size metadata (sizes now recorded with DOS evidence); 1,380 B proven credits_init prefix (capacity 115 used / 135 possible OPEN); 56 B alignment; 4 B runtime _NULL; **713 B OPEN** (2:01918.., 2:01970 400 B zero run, 2:05738 240 B, small ranges) - no direct relocation targets them |
| BSS layout verified | 151,803 B; game BSS holes 42 / 493 B (gap_data: the older ~3.5 KB figure was stale): 38 B alignment, **455 B OPEN**; runtime BSS fully attributed except 5 B alignment | MOSTLY DONE |
| language_txt capacity | 237 proven, 237..260 possible; 92-byte remainder | OPEN (gap_data: no evidence distinguishes; no behavioural effect - the game uses 237) |
| _DATA 400-byte zero run 2:01970 (B10) | 400 B | OPEN |

## 4. Meaning (names, types) - needed for a maintainable port, not for behaviour
| item | size | state |
|---|---|---|
| semantic registry (names and meanings) | docs/semantics: types.json, globals.json + per-unit basic, moteur, ray, collision1, collision2, objinit, load - 2,262 entities (2,262 = 282 functions, 1,267 locals, 294 globals, 41 types, 378 fields); Rayman states (RAY_STATES.md); on-disk formats of all 92 data files with 488 checksums, resource rebinding and per-type object defaults (docs/port/FILE_FORMATS.md, OBJECT_DEFAULTS.md). Enforced against code and compiled layout (tools/semantics.py, tests/test_semantics.py). Renames: 190 locals/parameters + 3 fields, compiled objects identical. Next: object-type behaviour units, display/affiche, menus | IN PROGRESS |
| function names INVENTED / placeholder-style | 241 INVENTED (198 sub_/effect_helper_-style) of 1,851 | OPEN |
| data names INVENTED / placeholder-style | 228 INVENTED (165 placeholder-style) of 1,358 | OPEN |
| unk_/field_/sub_/dword_ identifiers in src | 353 distinct | OPEN |

## 5. Platform boundary (what the SDL3 port replaces)
| item | state |
|---|---|
| inventory of DOS-specific functions (video/VGA ports, keyboard ISR, timer, DPMI/memory, CD/MSCDEX, HMI sound, file I/O paths) with each one's contract | DONE as inventory (platform worker 2026-10-24, build/workers/platform/PLATFORM.md + platform.json, not tracked: quotes disassembly): 655 contracts (562 STRICT / 93 NEAR), all 1,689 game callables classified, 109 asm entries, 346 platform globals, 18 HMI APIs via 11 game entry points. 26 OPEN items: asset-shipped tile code (above), asm record field meanings (draw_effect_segment, draw_sprite_deform, 03257/03912/03ea1), BIOS INT 15h wait under PMODE/W, debug-register startup watch, PCM signedness/loop convention, HMI capabilities buffer extent, CD auxiliary status bytes, check_cd_for_save probe path, word_E0CD0 meaning |
| open platform questions (9) | docs/port/PLATFORM_OPEN.md (platform_open 2026-10-24): RESOLVED - PCM formats/pitch/loop convention, HMI capabilities write = exactly 106 bytes, check_cd_for_save path (DOS truncation makes the probe writable), deformation store reachable in its caller domain; PORT_DECISION - omit the startup debug-register watch, the unused 500 ms INT 15h benchmark (no caller), the unused CD auxiliary accessors and orphan globals (word_E0CD0 etc., no live consumer); replace temp_malloc by a malloc-backed wrapper (the original 30-descriptor table has no bounds guard - overflow demonstrated on a valid API sequence, no shipped-game occurrence shown). Residual OPEN: historical PMODE/W INT 15h translation, shipped max-live temp allocations | DONE |

## 6. Runtime
| item | state |
|---|---|
| rebuilt game reaches the first level and responds to input (DOSBox-X) | DONE (observation, not proof) |
| broader gameplay parity | deferred until items 1-2 are closed (user: no autonomous gameplay agents) |
