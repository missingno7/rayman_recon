<!-- platform_open worker 2026-10-24; evidence files (*.dis, *.json) referenced below stay in build/workers/platform_open (untracked). -->
# Open platform items: GOG v1.21

Read-only evidence work, 2026-10-24. **Four RESOLVED; five PORT_DECISION.** A PORT_DECISION names a DOS mechanism the native game does not need and explains why. Remaining historical questions are retained explicitly; they are not promoted to proven facts. Machine-readable companion: [open_items.json](open_items.json).

All writes are inside `build/workers/platform_open/`. No source/config/tools/tests/docs/assets/external edits, git operations, compiles, background processes or gameplay runs. Unicorn executes isolated original routines/blocks. `tools/image.py.Image` checks the decoded oracle hashes against config/oracles.json. `1:`/`2:` addresses are object offsets, not runtime linear addresses. Loaded HMI timer offsets are explicitly body-relative.

PROVEN means exact instructions/relocations, an explicit source/SDK definition, or bounded execution assertions with stated scope. STRONG marks interpretation or a scoped static negative. HYPOTHESIS marks historical intent. OPEN names missing evidence. Bounded execution is behavioral evidence, not a new STRICT matching result. Existing names retain config provenance (Rayverse/Android/PS1 or INVENTED); placeholder sub_/byte_/word_/unk_ names and explanatory test labels are INVENTED. No original name is invented here. The pinned HMI header is under C:/tools/hmi-sos3; none was found under external/.

| Item | Verdict | Core finding |
|---|---|---|
| 1 | PORT_DECISION | Tick/debug watch has no game result; omit native debug-vector mechanism. |
| 2 | PORT_DECISION | Dormant 500 ms VGA throughput benchmark; BIOS translation unnecessary for native gameplay. |
| 3 | RESOLVED | U8 mono effects, S16LE mono speech; 10000 Hz reference; loops use start plus length. |
| 4 | RESOLVED | Exactly 106 bytes written, not 108; application owns the output. |
| 5 | PORT_DECISION | Three orphan CD byte APIs; no live status meaning established. |
| 6 | RESOLVED | DOS truncates the suffix to eight characters; probe is separate from save path. |
| 7 | PORT_DECISION | Write-only/orphan fields; benchmark pointer has no initializer/live owner. |
| 8 | PORT_DECISION | Malloc wrapper appropriate; DOS descriptor availability is not guaranteed. |
| 9 | RESOLVED | Store 049ed is reachable with actual caller-generated geometry and scratch history. |

## 1. O-STARTUP-WATCH — PORT_DECISION

### Question

What does the DR0/DR7 and INT1/INT3 startup/loading sequence do on a normal machine and under a debugger? Does game state depend on it?

### Evidence

**PROVEN, DOS:** main 1:083bb..083ea saves vectors, installs the handler on vectors 0..5, clears TF/RF, zeros DR0..DR3, masks DR6, and disables breakpoint enables/control fields with `DR7 &= DE00h`. sub_5BB78 restores saved protected INT3; sub_5BB94 restores saved protected INT1. Both run immediately before the watches at 1:08442 and 1:34e0b. PMODE/W 1.22's pinned manual sections 3.5/3.9 say DOS AH=25h/35h map to protected-mode vector services: these are not ordinary writes to the real-mode IVT.

**PROVEN hardware decoding:** DR0=046Ch; DR7 OR 30003h sets L0/G0, R/W0=11 (data reads **and** writes), LEN0=00 (one byte). The game's own polling loads can cause #DB: this is not merely a watch for BIOS writes. The [Intel 80386 manual, 12.2](https://pdos.csail.mit.edu/6.828/2018/readings/i386/s12_02.htm) defines the fields; [12.3.1.2](https://pdos.lcs.mit.edu/6.828/2018/readings/i386/s12_03.htm) describes a data breakpoint as a trap after the access. DR access requires privilege zero. PMODE/W 1.22 documents ring-zero execution on clean/XMS/VCPI systems.

**PROVEN dataflow:** Each watch enables interrupts, waits for the low BIOS tick byte to change, snapshots it again, then waits for another change. It zeros DR0 and masks DR7 afterward. There is no elapsed-time subtraction, speed threshold, trap-count test, game-global store or success/failure result in these spans. No later game read of DR6 was found: its one source read is the earlier startup clear. The branch predicate is tick-byte equality. With conventional PIT0 BIOS timing, two successive observed changes normally occupy about one to two 54.9 ms tick periods; debugger stops can make this arbitrarily longer. A missing/unchanging tick hangs the loop.

**PROVEN:** interrupt_handler_5ba8c, 1:5ba8c..5baa5, saves registers/segments, obtains DS, executes CLI/STI/CLD, restores registers/segments and IRETD. It neither inspects a cause nor updates a game variable. It is reinstalled after startup and loading. While saved INT3 is restored, loader checkpoints issue explicit INT3s, including 1:34f5d, 35008, 35389 and 35604. The saved host handlers are exposed during these intervals.

**STRONG normal/debugger effect:** On a permissive DOS setup with returning default debug/breakpoint handlers, this is tick synchronization plus repeated harmless traps. A debugger can stop on watched accesses/loading INT3s; existing hardware breakpoints and stepping are disrupted by the register/flag clears and vector substitution. The precise response is the installed handler/debugger's policy. **HYPOTHESIS:** protection/anti-debug interference explains the pattern better than CPU-speed measurement; authorial intent is unproven. No timing-validation decision exists here.

### Verdict

The native port does not need this CPU/interrupt mechanism: watched values and trap results do not feed a game calculation or protection flag. This is a port decision, not a universal historical-host result. “No explicit game-state dependency” does not imply identical wall-clock behavior: PcMain calls MakeMyRand(1) afterward, which seeds from time(), and asynchronous audio/time can advance during waits.

### What the port must do

Omit DR operations, vector replacement/restoration and loading INT3s in the native boundary. Do not implement a native debugger check or replace frame pacing with this delay. Retain the actual game clock/RNG services separately. If preserving the startup/loading pause is desired, represent its tick phase/delay explicitly as presentation timing. Separate B16 protection predicates are not the no-result watch and must not be conflated with it.

**Remaining OPEN:** Original PMODE/W 1.22 interrupt traces with actual saved INT1/INT3 targets on clean DOS/VCPI and a named debugger; exact historical intent. Arbitrary debugger/DPMI behavior is not claimed.

Evidence: [startup.dis](startup.dis), [loading_watch.dis](loading_watch.dis), [interrupt_vectors.dis](interrupt_vectors.dis), [external_input_hashes.json](external_input_hashes.json).

## 2. bios_wait_timer / sub_3B5E8 — PORT_DECISION

### Question

What event and interval are waited for, what consumes the result, and what is the portable equivalent?

### Evidence

**PROVEN:** 1:0273a saves EAX/ES, sets ES=0, AX=8300h, executes INT15h with caller-supplied EBX/CX/DX, then restores ES/EAX. It does not check carry/status. sub_3B5E8 clears `*unk_ptr_3d840`, requests CX=7, DX=A120h, and copies 3FFFh dwords from linear 0 to A0000h repeatedly until that flag becomes nonzero. `(7 << 16) | A120h = 500000` microseconds. Each copy transfers 65,532 bytes, not 64,000. The return is signed copy count divided by eight.

**PROVEN service contract / STRONG interpretation:** AX=8300h arms an asynchronous BIOS interval, whose completion sets bit 7 of the caller-owned flag; it is not waiting for a CD, key or frame. The primary [SeaBIOS source](https://github.com/coreboot/seabios/blob/master/src/clock.c), handle_158300/set_usertimer/handle_70, independently exhibits CX:DX microseconds, real-mode ES:BX and OR80h completion with RTC granularity. This corroborates the BIOS contract, not Rayman's historical pointer translation. The caller is a half-second VGA-copy throughput benchmark with a scaled return, not a sleep function.

**PROVEN static negative:** An all-byte E8/E9 scan finds no call/jump to 1:2b5e8; no PMW1 code/data fixup takes its address. The source only defines it. unk_ptr_3d840 has exactly two relocation references, both loads in this function, and no initialization store/address escape. It starts as zero BSS. No game consumer of the return exists. The already analyzed asset tile callbacks are constant painters, not hidden benchmark callers.

**OPEN historical translation:** Protected-mode EBX is not automatically a BIOS ES:BX pointer. The pinned PMODE/W 1.22 general-reflection documentation says segment registers cannot simply be forwarded and lists no INT15h pointer extension. No original-host trace proves translation/completion or busy/unsupported handling. Arming failure would leave the caller spinning because it ignores carry. The evidence supports a dormant diagnostic, not an active game dependency.

### Verdict

Not needed by the port: there is no caller, initialized flag owner or result consumer in this v1.21 build. Event/arithmetic meaning is resolved. Exact historical PMODE/W translation remains OPEN but does not block the native game.

### What the port must do

Omit the unused diagnostic from gameplay. If retained as a diagnostic, use an owned completion object/deadline and an elapsed-time source such as [SDL_GetTicksNS](https://wiki.libsdl.org/SDL3/SDL_GetTicksNS), run a bounded RAM/backend-copy test for 500,000 microseconds, and return count/8. Do not read physical zero or write VGA; do not use its result for gameplay pacing. A DOS diagnostic would also need verified conventional-memory flag mapping and BIOS failure handling.

**Remaining OPEN:** Actual PMODE/W 1.22 INT15h translation/reflection and completion/error trace on a named BIOS; unnecessary for native gameplay.

Evidence: [bios.dis](bios.dis), [benchmark.dis](benchmark.dis), [no_caller_scan.json](no_caller_scan.json), [references.json](references.json), [source_refs.json](source_refs.json).

## 3. O-AUDIO-FORMAT — RESOLVED

### Question

What are the PCM signedness, width, rate, channel layout and loop endpoint convention?

### Evidence

**PROVEN inputs:** audio.py reads the image's tables at 2:06550/065a4/065f8 and decodes all seven SNDH8B banks, seven SNDD8B banks and nine SNDVIG chunks in memory. All 23 checksums pass. All 216 nonempty sample spans and 13 looping entries are within bank/sample bounds. No decoded PCM file is retained.

| Input | Source PCM | Playback reference |
|---|---|---|
| SNDD8B.DAT effects | Unsigned 8-bit mono; silence80h, recordings often alternate7Fh/80h | 10000 Hz at unity pitch; voice ratio below |
| SNDVIG.DAT speech | Signed 16-bit little-endian mono; silence0000h | Requested mixer rate10000 Hz, unity pitch |

**PROVEN measurements:** Effect-bank byte means are127.3847..127.6467, with 7Fh/80h leading modes. The wrong signed 16-bit interpretation yields RMS about27,591..28,629. Speech signed little-endian 16-bit means are near zero (-3.3197..5.8474), with quiet prefixes/suffixes of 0000h,0001h andFFFFh. Every speech length is even. Statistics alone would be STRONG; corresponding requests and mixer selection remove format ambiguity.

**PROVEN SDK/requests:** The pinned sos.h defines720h=PITCH_SHIFT|PANNING|VOLUME|TRANSLATE8TO16. Looping KeyOn uses4760h, adding LOOPING|STAGE_LOOP. Speech uses180h=VOLUME|TRANSLATE16TO8. Request channel+10h is2, `_CENTER_CHANNEL`; `_INTERLEAVED`=3 and STEREOTOMONO=10h are not requested. These are mono source voices panned into a possibly stereo output, not interleaved stereo samples. ACTIVE=8000h is not an input signedness flag; capability SIGNED_DATA concerns output hardware. Actual timer 1000h fragments select the high byte of 16-bit words and use XOR80h for signed/unsigned 8-bit conversion.

**PROVEN rate, correcting the field name:** 1:2e7a8..2e7bd writes2710h=10000 to sound_config+10h. SDK `wSampleRate` is not a timeout; HMI InitDriver forwards it to sosDRVSetRate. KeyOn request+38h is SDK `dwSamplePitchAdd`, a Q16 source step, **not Hz**: `pitchTab[note] << 8`, ratio `pitchTab[note]/256`. Index 53 contains256 and gives65536=1.0. Speech omits PITCH_SHIFT and uses unity stepping. Raw banks have no per-sample Hz metadata. 10000 Hz is the requested reference; physical driver rate quantization can alter the realized clock.

**PROVEN loops:** Bank header+8 is loop byte start L; +0Ch is byte **length** N. Linked HMI initializes loop pointer=data+L, initial bytes-left=L and reload length=N. audio_loop_proof.py executes original StartSample and the shipped timer 1000h infinite staged-loop reload. Bank1/program50 (also bank5/program50): size 6928, L=2621, N=3199; reload returns to+2621 with 3199 bytes and count still-1. Portable loop: `[2621,5820)`, not `[2621,3199]`. Entries with N=size, size-1 and size-2 support no invented+1.

### Verdict

Resolved for every shipped chunk/entry. Logical format/endpoints are separate from hardware output conversion and mixer processing granularity. This is not a bit-identical native mixer/resampler certificate.

### What the port must do

Decode effects as U8 mono and speech as S16LE mono. Sizes/loops are bytes (effects one byte/frame; speech two). Use a 10000 Hz reference and the exact Q16 KeyOn ratio; preserve volume/pan, stop and voice-done behavior. Loop from L through L+N exclusive, -1 infinite. Convert at the native audio boundary. Do not infer11025/22050 Hz from names, interpret720h as a PCM format number, treat N as an absolute endpoint or add a sample.

**Stronger scope not claimed:** Selected hardware's exact clock quantization or bit-identical HMI resampling/mixing.

Evidence: [audio_data.json](audio_data.json), [audio_loop_proof.json](audio_loop_proof.json), [hifi_init.dis](hifi_init.dis), [hifi_KeyOn.dis](hifi_KeyOn.dis), [hmi_start_sample.dis](hmi_start_sample.dis), [hmi_mixer_excerpt.dis](hmi_mixer_excerpt.dis), [external_input_hashes.json](external_input_hashes.json).

## 4. O-CAPS-BUFFER — RESOLVED

### Question

Exactly how many bytes does HMI write over current_device_name, and who owns them?

### Evidence

**PROVEN:** sub_3E710 passes 2:50b0a to sosDIGIDetectFindFirst as a capabilities **output**, not an input name. Successful detection calls sosDetDRVGetCapsInfo. Its wrapper 1:78aee..78b3b calls driver operation 2 for EDX, loads the caller's output pointer, then at 1:78b10 sets ECX=6Ah. REP MOVSB at 1:78b16 copies **106 bytes**. Four 16-bit selector fields at+44h/+4Ch/+54h/+5Ch are replaced afterward, inside that span. There is no 108-byte write here.

| Relative bytes | DOS addresses | Meaning/extent |
|---|---|---|
| 0..31 | 2:50b0a..50b29 | SDK szDeviceName[32] |
| 32..99 | 2:50b2a..50b6d | Version/format/rates/flags, padded far-pointer slots and fBackground |
| 100..103 | 2:50b6e..50b71 | Complete wDeviceID, viewed as current_device_id |
| 104..105 | 2:50b72..50b73 | Low 16 bits of wTimerID |
| 106..107 | 2:50b74..50b75 | **Not written** by this wrapper; initially zero BSS |

Exact write interval: `[2:50b0a,2:50b74)`. The SDK C type's four-byte WORD scalars and padded six-byte far pointers give sizeof 108. That is not the copy count. The name binding's100 bytes contain more than a string: its actual string prefix is32 bytes. The next named driver handle begins at 2:50b78.

**PROVEN executions:** small_proofs.py executes the actual copy/selector-fix tail with patterned data and guards: exactly offsets 0..105 written, all four selectors correct, guards+106..109 remain CC. hmi_caps_records.py executes pure operation 2 dispatch for all 66 relevant HMIDET records (extender bit 8000 set), stopping before RETF: 66/66 return an in-buffer pointer with no IN/OUT. IDs at+100 agree with their headers; timer IDs appear at+104. These are queries, not hardware-detection emulations.

**PROVEN ownership flow:** Application storage owns the output. HMI `_lpDETDeviceCaps` borrows it for GetSettings, which uses copied driver resource-list views. DetectUnInit frees the loaded detection buffer: those far-pointer views cannot become persistent native pointers. SetDeviceID later writes+100; GetDeviceName returns the base. **STRONG:** the intended logical owner is an audio-device capabilities/selection record; current name/id source fragments are overlapping views, not a sound100-character-string abstraction. No canonical ownership edit was made.

### Verdict

Resolved: 106-byte copy plus internal selector replacements. SDK allocation size 108; the final two bytes are not populated by this wrapper.

### What the port must do

Use an owned native device descriptor with separate string, ID and typed capabilities; translate values while the query's storage is valid. A DOS compatibility record should allocate/initialize the full108-byte SDK extent and model the 106-byte write precisely. Do not write through char[100], rely on neighboring globals or retain freed far pointers.

Evidence: [hmi_caps.dis](hmi_caps.dis), [caps_proof.json](caps_proof.json), [hmi_caps_records.json](hmi_caps_records.json), [references.json](references.json), [sound_detect.dis](sound_detect.dis).

## 5. CD auxiliary status bytes — PORT_DECISION

### Question

What do sub_193b8, sub_193c0 and sub_193c8 return, and who reads them?

### Evidence

**PROVEN:** 1:193b8 loads byte_DEF00 (2:4ef00); 1:193c0 loads byte_DEEFC (2:4eefc); 1:193c8 loads byte_DEEFD (2:4eefd). Matching setters are1:193d8, 193e0, 193e8. They are plain byte getters/setters over zero-initialized BSS.

**PROVEN static negative:** No E8/E9 target anywhere in decoded code names these six entries; no PMW1 fixup takes their addresses. No source caller exists. Each byte's only exact target relocations are its getter load and setter store. Neighboring-address references were audited: byte_DEEFB is the selected drive index; cd_request_busy and cd_status_playing/ready/error are distinct state. No audited dword access spans the auxiliary bytes. byte_DEF00 is not cd_status_playing (2:4eeff).

**OPEN historical meaning:** No observable assignment/live consumer distinguishes “paused”, “ready”, “enabled”, etc. Giving one of those names would be invention. These are not the live MSCDEX status-query outputs.

### Verdict

Not needed by the port: accessor entries and their storage references are orphaned in this build. Byte-access contracts are resolved; unused historical meanings remain OPEN. This is not an arbitrary-code formal points-to proof.

### What the port must do

Omit these orphan flags/accessors from the native CD backend. An optional API compatibility surface can keep three independent zero-initialized bytes with trivial getters/setters. Preserve the actual selected-drive/playback/request state separately; never substitute these getters for a live status call.

**Remaining OPEN:** Another release, original symbol/debug information or a live writer/reader establishing their historical names. No required v1.21 behavior depends on the answer.

Evidence: [cd_flags.dis](cd_flags.dis), [no_caller_scan.json](no_caller_scan.json), [references.json](references.json), [source_refs.json](source_refs.json).

## 6. check_cd_for_save — RESOLVED

### Question

What path is built, what happens on DOS, and how should native save locations work?

### Evidence

**PROVEN construction:** Literal 2:312d4 is `%c:\????????.???`. Positions: drive0, colon1, slash2, question marks3..10, dot11, question marks12..14, NUL15. Stores1:5bc97 replace2..9; 1:5bcaf replaces11..13. Both slash and dot disappear; question marks remain at 10 and14. Eleven myRand(26) calls yield 'a'..'{' because the maximum is inclusive. Example: `D:abcdefgh?ijk?`, a 13-character component after a drive-relative prefix, not `D:\abcdefgh.ijk`.

save_probe_proof.py executes the original construction/control instructions with external services explicitly stubbed, captures fopen arguments, and never opens a file. RNG results0..10 capture exactly `D:abcdefgh?ijk?`, mode `wb`, eleven draws.

**PROVEN MS-DOS 4.0 source behavior, corroborated by local DOSBox-X SFN code:** [FCB.ASM](https://github.com/microsoft/MS-DOS/blob/main/v4.0/src/DOS/FCB.ASM) NameTrans/NORMSCAN sets CX=8. MUSTGETWORD's JCXZ precedes wildcard handling: suffix characters are consumed but neither stored nor marked ambiguous after eight positions. [MACRO2.ASM](https://github.com/microsoft/MS-DOS/blob/main/v4.0/src/DOS/MACRO2.ASM) CopyComponent applies translation; [FILE.ASM](https://github.com/microsoft/MS-DOS/blob/main/v4.0/src/DOS/FILE.ASM) AccessFile checks the translated ambiguity before creation. The example becomes `ABCDEFGH`, no extension, in drive D's **current directory**. '{' is an ordinary valid character. Neither surviving '?' forces rejection. A model control with '?' inside the first eight characters stays ambiguous. Local DOSBox-X DOS_MakeName also truncates a dotless SFN component at eight.

**PROVEN result use:** If CD detection/status permits probing, it stops CD playback and tries fopen(...,"wb"). Success closes/unlinks the path and sets JeuCracker=1. Failure leaves JeuCracker unchanged. Missing MSCDEX/CD status sets it to 1 directly. Thus a writable non-CD drive can trigger the cracked/limited state, while an ordinary read-only CD fails creation. It probes the source/media drive, not the save directory. DOS truncation could truncate/remove an existing matching eight-character file; no experiment touched a drive.

**PROVEN separate save path:** init_arg defaults demo_video_mem to `.\` and supports SAVE=pathname; src/save.c creates `RAYMAN<n>.SAV` there. check_cd_for_save never uses demo_video_mem.

### Verdict

Resolved for the evidenced DOS short-name semantics: a valid extensionless, drive-relative eight-character probe can result. Inferring inevitable failure from the literal question marks is false. No blanket claim covers every DOS clone/redirector/long-filename API.

### What the port must do

Use a per-user writable directory, such as [SDL_GetPrefPath](https://wiki.libsdl.org/SDL3/SDL_GetPrefPath), with an explicit override where appropriate. Keep asset/media location separate; retain save filenames/serialization. Use exclusive temporary creation in the save directory and safe destination replacement. Do not port the malformed drive probe or use save writability to set JeuCracker; keep genuine-media/protection policy explicit. For legacy RNG-sequence parity, account for eleven myRand(26) draws at each executed normal successful-media probe, or document an intentional RNG change. Likewise account separately for stop_cd.

**Remaining stronger-scope evidence:** No live trace on the exact historical DOS/redirector. The evidenced MS-DOS 4.0 parser and DOSBox-X SFN implementation are distinguished from other hosts.

Evidence: [cd_probe.dis](cd_probe.dis), [save_probe_proof.json](save_probe_proof.json), [save_probe_proof.py](save_probe_proof.py), [xref.json](xref.json).

## 7. word_E0CD0 / auxiliary bytes / unk_ptr_3d840 — PORT_DECISION

### Question

Who reads these globals, and what owns the benchmark flag pointer?

### Evidence

**PROVEN word_E0CD0:** sub_4212C at 1:3212c writes AX to 2:50cd0 and returns. FIRST_INIT calls it with 3 at 1:26261. Its only exact data relocation is that store; cheat.c's declaration is unused. There is no getter or pointer escape. Nearby leftjoy at 1:31d46 loads a **16-bit** joy_rec_left at 2:50cce, not a dword spanning word_E0CD0. sub_42408 writes neighboring record flags independently. No live joystick-count/debounce meaning is established.

**PROVEN byte_DEEFC/DEEFD/DEF00:** Exactly the orphan CD bytes from item5, not additional BIOS/playback flags.

**PROVEN unk_ptr_3d840:** The BSS dword2:3d840 is loaded only at 1:2b5fb and2b624, then dereferenced in the unused benchmark. No relocation reference initializes it or passes its address as an output; source has no assignment. There is no active allocation/flag owner. Initial zero fits a dormant routine; it does not prove that the IVT was intentionally used as a completion flag.

**STRONG scope:** Exact/neighboring fixup targets, all-byte direct-call candidates and source references agree. Negatives apply to GOG v1.21 and the inspected callbacks, not injected code or another release.

### Verdict

Not needed by the port: word_E0CD0 has no semantic reader, CD bytes have only unused accessors, and the pointer belongs only to an unused/uninitialized benchmark. Useful behavior here is absence of a live game effect, not an invented domain name.

### What the port must do

Omit these orphan fields from native gameplay/platform state. Source-compatible symbols, if retained, keep their widths/zero initialization and sub_4212C's low 16 store. Do not merge them with live neighboring joystick/CD fields. A revived benchmark needs an explicitly owned completion object, not a guessed pointer address.

**Remaining OPEN:** Original names/purposes and any removed historical benchmark initialization/caller require another release or original symbols.

Evidence: [sub_4212C.dis](sub_4212C.dis), [leftjoy.dis](leftjoy.dis), [sub_42408.dis](sub_42408.dis), [references.json](references.json), [no_caller_scan.json](no_caller_scan.json), [source_refs.json](source_refs.json).

## 8. temp_malloc — PORT_DECISION

### Question

What are allocator invariants/request sizes; is a malloc replacement behaviorally safe?

### Evidence

**PROVEN algorithm:** Thirty 12-byte descriptors at 2:39688 contain capacity+0, pointer+4, allocated byte+8. Initialization creates one free pool span and 29 zero-capacity sentinels at pool-end. Requests use `n += n & 3` (75 becomes78), **not** round-up-to-four. Best fit selects the smallest adequate free span, later entry on ties, via signed inclusive comparisons; valid shipped sizes/capacities are below 2^31. Allocation takes the beginning, transfers its suffix into an adjacent free entry or pool-end sentinel, and keeps live pointers stable. temp_free marks the matching live span free; pack_temp_zone merges adjacent free spans without moving live data. No adequate span means ordinary malloc fallback, not live compaction. Heap-backed pointers use ordinary free. Shutdown sub_6F740 releases the whole pool and clears descriptors.

**PROVEN required invariants:** Positive free/live spans partition the pool without overlap; zero-capacity descriptors are unused pool-end sentinels; the counter tracks allocated pool capacity; adjacency/split searches must find an index below 30. Searches have no bounds guard. Free bytes do not imply a spare descriptor. With m live spans, a well-formed coalesced partition has at most m+1 positive free spans; m<=14 leaves at least one sentinel before an allocation. No whole-game bound of 14 was established. Exact-fit allocation still performs the descriptor search.

**PROVEN counterexample:** small_proofs.py initializes the documented table with a 1024-byte pool, then executes the original routine thirty times requesting4 bytes. Calls 1..29 stay inside the table. Call 30, with 908 free bytes before it, reads descriptor30's allocated byte at table+368 (1:6f97d), writes its pointer at+364 (1:6f9d2), and capacity at+360 (1:6f9f2). These exceed the 360-byte table. This is a concrete missing guard on a valid API sequence, **not a claimed gameplay occurrence**. Initialization does not guarantee perpetual descriptor availability.

**PROVEN source census:** temp_calls.json records all 42 assignment call sites; the decoded binary independently contains exactly 42 E8 references to temp_malloc (temp_binary_call_sites.json). Constants:75 (world-stage name),100 (paths/CD/error buffers),200 (errors),400 (language parse scratch),1200 (map transparency),3996 and4000 (timing arrays). Variable requests are VIGNET record size minus128 or read_record's sign-extended low 16 file length. All75 VIGNET requests are recorded:9049..133555 bytes before adjustment. Signed file-length truncation is visible at 1:32304; no shipped .REC size is asserted for that interactive filename. InitMemoryVariable chooses pools95232 (17400h) or139264 (22000h). Requests exceeding the smaller pool legitimately use heap fallback.

**STRONG abstraction evidence:** Outside sysutils, no source operation consumes descriptors/pool statistics as gameplay state. Returned storage holds strings, decoded data, map bytes, records or timing arrays; frees use returned pointers. No caller requires adjacency or a particular address range. Diagnostic peak accounting does not precisely total all outstanding fallback blocks. Record loading and checksum-retry paths prevent an unconditional allocation/leak bound from this census alone.

### Verdict

A malloc-backed native wrapper is appropriate for defined positive-size allocation/lifetime behavior; the descriptor policy and its out-of-bounds behavior are unnecessary. A one-line substitution without extent/lifetime/failure qualifications is not a whole-program equivalence proof. The generic descriptor premise is disproved; shipped maximum-live behavior remains OPEN.

### What the port must do

Allocate at least the original adjusted extent, or safely more; provide stable nonoverlapping storage; retain temp_free and ownership/shutdown handling for outstanding temp allocations; preserve fatal allocation-failure handling. Keep ordinary heap/fixed-pool ownership distinct. Avoid address-based classification and the 30-descriptor limit. Reject impossible sizes/overflow in native input handling instead of reproducing corruption. No defined-equivalence promise covers zero/negative/overflow sizes, double free or allocation after shutdown. Placement/fragmentation/diagnostic numbers may differ under the native policy; do not introduce gameplay consumers for them.

**Remaining OPEN:** A complete shipped-game maximum-live/lifetime trace, including record input and checksum-failure retries. Historical invalid-call/corruption behavior has no general malloc equivalence.

Evidence: [temp_allocator.dis](temp_allocator.dis), [allocator_proof.json](allocator_proof.json), [temp_calls.json](temp_calls.json), [temp_binary_call_sites.json](temp_binary_call_sites.json), [vignette_temp_sizes.json](vignette_temp_sizes.json), [read_record.dis](read_record.dis), [source_refs.json](source_refs.json).

## 9. Deformation-store reachability — RESOLVED

### Question

Can the ascending store at 1:049ed execute within the caller descriptor domain?

### Evidence

**PROVEN premise:** Prefill writes `P=4*(floor(width/4)+1)*height` consecutive bytes ofA1h. Descending copies overA1h; ascending compares81h at 1:049e8 and stores DH at 1:049ed. A fresh rectangle'sA1h fill does not prove that store dead: generated visits can be beyond the new prefill yet inside the shared 1200-byte allocation.

**PROVEN two-call witness:** deformation_proof.py compares the pinned oracle with the existing independent Python model while carrying complete scratch history between calls. It also separately executes the original C descriptor builder AFFICHE_SPRITE_DEFORME (1:0b944) with these source dimensions/corners: actual 13-dword records equal those below.

1. Source 40x4, pixels 81h; corners `(0,0),(40,0),(0,4),(40,4)`. Record `[1,40,40,1,1,40,0,4,40,4,0,40,4]`. Prefill 176 bytes; descending paints offsets 0..159 with 81h.
2. Source 4x32, pixels 17h; corners `(0,4),(0,0),(32,4),(32,0)` (quarter turn). Record `[1,-32,0,4,1,32,32,1,32,4,128,4,32]`. Prefill 144 bytes. First pixels of ascending columns visit 128..159;144..159 retain81h from call 1. The actual049ed store executes and changes these sixteen bytes to 17h. Accesses remain inside the 1200-byte owning scratch allocation; the source atlas read tail is mapped as required by the assembly contract.

Ascending reaches 108 distinct instructions; seeded descending reaches 109. Both have zero register, written-address-set or memory mismatches against the independent model. A negative control uses the same ascending record on fresh zeroed backing: 049ed is not visited. No source patch, opcode change, weakened comparison or enlarged nominal allocation was used. The writes exceed this call's logical width*height and prefill, not their owning buffer. The initial81h comes from a valid prior call, not injection into a freshly filled region.

**OPEN stronger scope:** No shipped level/actor/text sequence with these exact sprites/history was identified. Actual gameplay occurrence and visible consequences are separate questions. The descriptors are accepted by the actual C caller and are ordinary rectangle-transform geometry; assembly-domain reachability does not depend on finding that scene.

### Verdict

Resolved: 049ed is reachable for valid caller-generated descriptors and feasible history. It cannot be dropped as unreachable based on the prior 256 fresh randomized cases. This verdict is explicitly a routine/caller-domain reachability result, not a shipped-scene trace.

### What the port must do

Preserve the asymmetric A1h/81h tests and shared scratch history, including writes beyond the new rectangle but within the owning allocation, in a faithful model. Do not clear all 1200 bytes per call, replace 81h with A1h or drop049ed without a separate deliberate behavior decision/proof. The witness establishes execution/memory effects; it does not assert an immediate visible pixel difference in that call.

**Remaining stronger-scope evidence:** Actual sprite/angle/history trace from shipped gameplay and downstream visibility. Not needed to preserve the established full routine contract.

Evidence: [deform.dis](deform.dis), [AFFICHE_SPRITE_DEFORME.dis](AFFICHE_SPRITE_DEFORME.dis), [deformation_proof.py](deformation_proof.py), [deformation_proof.json](deformation_proof.json).

## Reproduction and limits

From repository root, use `python -B` to prevent bytecode writes outside this worker directory:

```text
python -B build/workers/platform_open/audio.py
python -B build/workers/platform_open/hmi_parse.py
python -B build/workers/platform_open/hmi_caps_records.py
python -B build/workers/platform_open/small_proofs.py
python -B build/workers/platform_open/audio_loop_proof.py
python -B build/workers/platform_open/save_probe_proof.py
python -B build/workers/platform_open/deformation_proof.py
python -B build/workers/platform_open/make_json.py
python -B build/workers/platform_open/validate.py
```

collect.py, extra_evidence.py and source_audit.py produce static/source evidence. xref.build() was used read-only: the CLI writes outside this directory, so it was not invoked. rg is unavailable; searches used PowerShell/Python. No compile/verifier gate is relevant to this read-only task; none was run.

Negative results retained: no benchmark caller/pointer initializer; no CD auxiliary caller; no word_E0CD0 semantic reader; no external/ SDK header; the wildcards-necessarily-fail hypothesis is refuted by DOS parser ordering; the generic spare-descriptor guarantee is refuted by original execution; fresh-zero ascending deformation does not reach 049ed. The first caps harness counted CPU GDT accessed-bit maintenance as output writes; it was corrected to count and guard the complete destination neighborhood. Destination bytes and the 106-byte expectation were unchanged. PMODE/W 1.33 source was inspected only as a lead, not substituted for missing 1.22 behavior. A Python network fetch was unavailable; supplementary primary specifications were read through the web tool and linked beside claims.

Input hashes: [input_hashes.json](input_hashes.json) (source/config snapshot), [external_input_hashes.json](external_input_hashes.json) (SDK/manual/assets). [validation.json](validation.json) records final drift/proof/JSON checks and size. These are evidence and port-contract decisions; no port or canonical-documentation update is implemented.
