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
| NEAR game functions, SHAPE / REFS_LAYOUT | ~200 (~166 KB) | same calls/globals/fields/constants (census) - strong, not certified. Close by differential execution (item 1a) | OPEN |
| 1a. differential execution | all NEAR game functions | run the DOS function and our compiled function on identical random machine states (Unicorn), compare every memory write, every outgoing call (target, arguments) and the return value; a counterexample is either a behaviour bug (fix) or an original quirk (document). Evidence level EQUIVALENT_TESTED, never STRICT | TO BUILD |
| code-range gaps between bound functions | 146 gaps, 3,071 B (38 > 16 B, 2,248 B) | classify each: jump table / literal / alignment padding / unbound code; bind or explain every byte | OPEN |
| original hand-written assembly | 106 entries, all STRICT as asm | behaviour exact; the port needs C equivalents - each checked against the asm with item 1a | LATER (port) |

## 2. Known behaviour questions
| item | state |
|---|---|
| B19 oldest_planted: fewer than 2 seed objects -> uninitialised ng_ids read | OPEN: check every level that enables the seed power (level data, as sem_a3 did for links) |
| B20 MOVE_OBJECT: fist path writes a bit computed from uninitialised EDI/[ESP] | OPEN: prove the bit has no reader (code + data) or find it |
| LoadTchatchPerdu -> PlayTchatch reference 5 bytes off although PlayTchatch is STRICT | OPEN |
| box CalcTab constant (B17) | value known exactly (1/9.5 as stored in DOS); behaviour known; only the C spelling is blocked |
| protection sequences (B16: INIT_OPTIONS_CHOICE, FIN_MAP_LOOP, LOAD_CONTINUE_SCREEN) | bytes exact; the port must document what each check does and what the game does on failure |

## 3. Data
| item | size | state |
|---|---|---|
| initialised _DATA with named, sized bindings | 175,759 of 186,776 B (94.1%) | 11,017 B unmapped: OPEN |
| BSS layout verified | 151,803 B; holes (B9: 2:3bd08..3c4a0, language table remainder, small holes ~3.5 KB) | OPEN |
| language_txt capacity | 237 proven, 237..260 possible | OPEN (no behavioural effect if <= 237 used) |
| _DATA 400-byte zero run 2:01970 (B10) | 400 B | OPEN |

## 4. Meaning (names, types) - needed for a maintainable port, not for behaviour
| item | size | state |
|---|---|---|
| function names INVENTED / placeholder-style | 241 INVENTED (198 sub_/effect_helper_-style) of 1,851 | OPEN |
| data names INVENTED / placeholder-style | 228 INVENTED (165 placeholder-style) of 1,358 | OPEN |
| unk_/field_/sub_/dword_ identifiers in src | 353 distinct | OPEN |

## 5. Platform boundary (what the SDL3 port replaces)
| item | state |
|---|---|
| inventory of DOS-specific functions (video/VGA ports, keyboard ISR, timer, DPMI/memory, CD/MSCDEX, HMI sound, file I/O paths) with each one's contract | OPEN |

## 6. Runtime
| item | state |
|---|---|
| rebuilt game reaches the first level and responds to input (DOSBox-X) | DONE (observation, not proof) |
| broader gameplay parity | deferred until items 1-2 are closed (user: no autonomous gameplay agents) |
