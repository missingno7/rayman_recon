# Semantic merge review

All 20 cross-worker meaning/level conflicts are resolved by retaining every statement verbatim with its worker, source context and original evidence level. The entity level records the strongest supported facet; UNKNOWN context statements do not lower independently supported meanings. No shared symbols or source/data/TU ownership are moved.

Two workers supplied action_local.h; both copies are byte-identical to current src/action_local.h. No file is changed by two workers. All structural entity attributes agree across canonical and worker records.

Value-domain dictionaries/lists are unioned. When scalar facts differ, the usable original scalar is retained and every additional fact is recorded with its context under merge_metadata_facts; contextual_facts.json indexes these alternatives. They are observations in distinct contexts, not silently substituted runtime bounds.

## global:Nb_total_cages

Owner: `globals.json`; merged level: **PROVEN**.

- **sem_objs; cage.c / PROVEN**: Total rescued-cage counter incremented when a cage is broken; progression compares it with 102 to unlock the final map node.
- **sem_flow; worldmap.c / PROVEN**: Byte total accumulated from24 stage cage counts at INIT_CHEMIN;>=102 marks final node17 pending.

Resolution/evidence: cage.c break-cage increment and worldmap.c INIT_CHEMIN accumulation/102 gate describe different lifetimes of the same byte.

## global:bossScrollEndX

Owner: `globals.json`; merged level: **STRONG**.

- **sem_objs; action.c / STRONG**: Right bound of the boss camera range; initialized from scroll_end_x then clamped to mode-dependent map bounds.
- **sem_boss; meredenis.c, moskito.c, saxo.c / STRONG**: Right encounter camera boundary calculated by setBossScrollLimits and tested by attack gates.

Resolution/evidence: action.c setBossScrollLimits producer plus boss attack-boundary readers; both retain STRONG.

## global:bossScrollEndY

Owner: `globals.json`; merged level: **STRONG**.

- **sem_objs; action.c / STRONG**: Lower bound of the boss camera range initialized to ymapmax and clamped.
- **sem_boss; meredenis.c, moskito.c, saxo.c / STRONG**: Bottom encounter camera boundary used for attack gating and target placement.

Resolution/evidence: action.c limit/clamp producer plus boss target-placement readers; both retain STRONG.

## global:bossScrollStartX

Owner: `globals.json`; merged level: **STRONG**.

- **sem_objs; action.c / STRONG**: Left bound of the boss camera range; initialized from scroll_start_x then clamped to mode-dependent map bounds.
- **sem_boss; meredenis.c, moskito.c, saxo.c / STRONG**: Left encounter camera boundary calculated by setBossScrollLimits and tested by attack gates.

Resolution/evidence: action.c camera-limit producer plus boss attack-boundary readers; both retain STRONG.

## global:bossScrollStartY

Owner: `globals.json`; merged level: **STRONG**.

- **sem_objs; action.c / STRONG**: Upper bound of the boss camera range derived from floorLine minus viewport height minus 16 and then clamped.
- **sem_boss; meredenis.c, moskito.c, saxo.c / STRONG**: Top encounter camera boundary calculated from the floor and viewport.

Resolution/evidence: action.c floor/viewport calculation plus boss encounter readers; both retain STRONG.

## global:floorLine

Owner: `globals.json`; merged level: **STRONG**.

- **sem_objs; action.c / STRONG**: Floor coordinate found by downward tile scan from the current boss feet; used to place the boss camera upper boundary.
- **sem_boss; meredenis.c, moskito.c / STRONG**: Floor y from firstFloorBelow used by boss camera setup and Space Mama target placement.

Resolution/evidence: action.c firstFloorBelow tile scan and boss positioning consumers support STRONG floor-coordinate role.

## global:BufferNormalDeplt

Owner: `globals.json`; merged level: **PROVEN**.

- **sem_objs; map.c / PROVEN**: Normal draw-buffer alignment displacement computed as 4 minus the visible tile-origin x low two bits.
- **sem_boss; skops.c / STRONG**: Normal-mode destination alignment displacement derived from map x low bits.

Resolution/evidence: sem_objs map.c DOS-backed typed arithmetic gives the exact 4-minus-low-two-bits producer; boss alignment observation is weaker and complementary.

## global:DrawBufferNormalIni

Owner: `globals.json`; merged level: **PROVEN**.

- **sem_objs; map.c / PROVEN**: Saved unshifted Normal draw-buffer base used when recalculating alignment and leaving Normal map mode.
- **sem_flow; cheat.c / UNKNOWN**: Observed storage used by cheat. Read/write operations are retained; intended meaning beyond them is UNKNOWN.

Resolution/evidence: sem_objs map.c saved-base writes and alignment/exit reads support PROVEN; cheat-only UNKNOWN expresses a narrower lack of interpretation.

## global:Scroll_Masque

Owner: `globals.json`; merged level: **PROVEN**.

- **sem_objs; map.c / PROVEN**: Per-sprite transparency masks for differential-scroll sprites: zero source pixels map to 0xff, nonzero to zero.
- **sem_boss; skops.c / STRONG**: Background sprite mask pointers prepared by map loading and consumed by DrawSpriteDiffNormal.

Resolution/evidence: sem_objs map.c DOS-backed mask creation establishes zero/nonzero polarity; boss rendering consumer adds usage context.

## global:Copy_Plan0_To_Buf

Owner: `globals.json`; merged level: **PROVEN**.

- **sem_objs; map_ngawe.c / PROVEN**: Active Normal background-copy routine pointer; INIT_GAME_MODE_NORMAL selects Copy_Plan0Diff_To_Buf with the existing assembly register ABI.
- **sem_boss; skops.c / STRONG**: Selected assembly background-copy function pointer; its register ABI is declared separately.

Resolution/evidence: sem_objs map_ngawe.c INIT_GAME_MODE_NORMAL assignment identifies the selected routine; boss records retain its separate assembly ABI requirement.

## global:bonus_perfect

Owner: `globals.json`; merged level: **PROVEN**.

- **sem_objs; bonus.c / PROVEN**: 192-bit map of perfect-bonus completion, indexed by (world-1)*32+level-1 and cleared as 24 bytes.
- **sem_flow; save.c, worldmap.c / PROVEN**: 24 bytes serialized after save_zone;INIT_NEW_GAME clears0..22 only. Existing gameplay consumers establish bonus completion;no meaning assigned to unused slot23.

Resolution/evidence: bonus.c completion indexing and save.c serialized 24-byte extent/23-slot reset are complementary; capacity is not a runtime bound or a meaning for slot 23.

## global:VENT_X

Owner: `globals.json`; merged level: **STRONG**.

- **sem_objs; neige.c / STRONG**: Signed horizontal weather wind accumulator in quarter-pixel units; particles use phase-dithered division by four and non-Jungle player wind is derived by division by eight.
- **sem_flow; save.c / UNKNOWN**: Observed storage used by save. Read/write operations are retained; intended meaning beyond them is UNKNOWN.

Resolution/evidence: sem_objs neige.c typed arithmetic and DOS readers support STRONG quarter-pixel weather role; save-only UNKNOWN does not negate it.

## global:VENT_Y

Owner: `globals.json`; merged level: **STRONG**.

- **sem_objs; neige.c / STRONG**: Signed vertical weather wind accumulator in quarter-pixel units; do_flocons adds 4 temporarily for fall motion and restores the original stored value.
- **sem_flow; save.c / UNKNOWN**: Observed storage used by save. Read/write operations are retained; intended meaning beyond them is UNKNOWN.

Resolution/evidence: sem_objs neige.c temporary +4 particle update and restoring store support STRONG; save-only UNKNOWN does not negate it.

## field:save_state_t.poing_sub_etat

Owner: `types.json`; merged level: **PROVEN**.

- **sem_objs; moteur.c / PROVEN**: Saved fist power/substate selection.
- **sem_flow; moteur.c / PROVEN**: Checkpoint fist state;restoring save1 degrades5/12->3 and1/3/8/10->1 by mutating saved value before applying to fist.

Resolution/evidence: save.c restoreGameState typed/DOS contract establishes checkpoint degradation; object pass describes the saved selection.

## field:world_info_t.state

Owner: `types.json`; merged level: **PROVEN**.

- **sem_objs; special.c / UNKNOWN**: Declared world_info_t.state storage; no intended meaning is assigned from its spelling alone.
- **sem_flow; bit flags / PROVEN**: Access/animation byte:bit0 accessible,bit1 drawing traversal/neighbor mark,bit2 pending unlock;high5 retained. Runtime bit2 is serialized as save-byte bit1.

Resolution/evidence: sem_flow worldmap.c typed accesses and DOS contract review prove runtime masks 1/2/4 and save repacking; object-only UNKNOWN is retained as context.

## global:rotationtxt

Owner: `globals.json`; merged level: **PROVEN**.

- **sem_objs; display.c, luciole.c / PROVEN**: Shared signed16 control used as text-deformation rotation and, during firefly scenes, as light-growth/drift angle.
- **sem_flow; display.c, worldmap.c / PROVEN**: Text deformation rotation argument.

Resolution/evidence: sem_objs luciole.c cited DOS readers/writers prove a second scene role while menu deformation remains valid; no universal angle unit is inferred.

## global:coeffktxt

Owner: `globals.json`; merged level: **STRONG**.

- **sem_objs; display.c, luciole.c / STRONG**: Shared signed16 control used as a save-name deformation phase and, during firefly death motion, as drift amplitude.
- **sem_flow; display.c, worldmap.c / STRONG**: Shared integer phase used for save-name deformation.

Resolution/evidence: sem_objs luciole.c cited multiply/shift and capped increments support the drift-amplitude facet; menu phase facet remains STRONG.

## global:byte_CFA14

Owner: `globals.json`; merged level: **STRONG**.

- **sem_boss; bat.c / STRONG**: Result flag set by detect_dos_version when DOS major version is ten or twenty; gates bat and CD paths.
- **sem_flow; cdsave.c / UNKNOWN**: Observed storage used by cdsave. Read/write operations are retained; intended meaning beyond them is UNKNOWN.

Resolution/evidence: sem_boss detect_dos_version producer and bat/CD readers support STRONG; cdsave-only UNKNOWN does not supply a competing interpretation.

## global:dark_phase

Owner: `globals.json`; merged level: **STRONG**.

- **sem_boss; dark.c / STRONG**: Dark encounter phase selector changed by phase-entry routines and command progression.
- **sem_flow; worldmap.c / UNKNOWN**: Observed storage used by worldmap. Read/write operations are retained; intended meaning beyond them is UNKNOWN.

Resolution/evidence: sem_boss dark.c phase-entry writes and command progression support STRONG; worldmap-only UNKNOWN remains explicitly limited.

## global:is_fee

Owner: `globals.json`; merged level: **STRONG**.

- **sem_boss; fee.c / STRONG**: Whether the current world/level has the fairy dialogue prepared by init_fee.
- **sem_flow; menu.c, pcmain.c / UNKNOWN**: Observed storage used by menu, pcmain. Read/write operations are retained; intended meaning beyond them is UNKNOWN.

Resolution/evidence: sem_boss fee.c init_fee producer and fairy-dialogue consumers support STRONG; menu/pcmain-only UNKNOWN remains explicitly limited.

## Evidence-level refinements

The following changes to current levels have new worker-specific typed/DOS evidence, retained in merge_receipt.json. These are semantic statement levels, not a promotion of NEAR code to STRICT.

- `field:flocon_t.field_4`: UNKNOWN -> PROVEN. sem_objs: Perspective depth/scale parameter stored per weather particle and passed to projection by the drawing readers.
- `field:poing_t.field_4`: UNKNOWN -> PROVEN. sem_objs: Launch-time snapshot of charge shifted left eight, written alongside the outbound charge budget. A further live consumer is not established.
- `field:world_info_t.state`: UNKNOWN -> PROVEN. sem_flow: Access/animation byte:bit0 accessible,bit1 drawing traversal/neighbor mark,bit2 pending unlock;high5 retained. Runtime bit2 is serialized as save-byte bit1.
- `global:compteur`: STRONG -> PROVEN. sem_flow: Held-input repeat counter;release0,increment until max_compteur,then delai_repetition+1.
- `global:menuEtape`: STRONG -> PROVEN. sem_flow: Menu dispatcher stage:0 general,3 save selection,4 options,7 keys,8 pad,9 graphics,10 video. In-game options uses4/7/8/9 and0 exit.
- `field:record_t.current_offset`: STRONG -> PROVEN. sem_flow: Byte offset of current repeat-length byte after startup;first frame consumes data0 buttons and leaves offset1. Subsequent runs advance2.
- `field:record_t.repeat_length`: STRONG -> PROVEN. sem_flow: Second byte of current input pair;repeat_index equality ends run. Zero length is not automatically treated as immediate termination.
- `field:record_t.repeat_index`: STRONG -> PROVEN. sem_flow: Incrementing32-bit index into current repeated run;startup/new run resets0.
- `field:record_t.length`: STRONG -> PROVEN. sem_flow: REC file length narrowed through signed16 by read_record then stored in record;used to bound next pair.
- `field:record_t.data`: STRONG -> PROVEN. sem_flow: Allocated packed input pairs(button byte,repeat-length byte);no native pointer is serialized by read_record.
- `field:options_jeu_t.music_enabled`: UNKNOWN -> PROVEN. sem_flow: Persistent music enable value;menu toggles logical zero/nonzero and controls MusicCdActive only when OptionMusicCdActive permits. Historical default13 is retained rather than normalized.
- `field:options_jeu_t.sound_volume`: UNKNOWN -> PROVEN. sem_flow: Persistent sound-volume value passed to raj_env_sound;menu working copy adjusts0..max_sound. CFG stores16 bits;historical default18.
- `field:options_jeu_t.is_stereo`: UNKNOWN -> PROVEN. sem_flow: Persistent stereo setting passed to raj_env_stereo;CFG stores16 bits,default1.
- `field:world_info_t.xpos`: UNKNOWN -> PROVEN. sem_flow: World-map node center coordinate;used to place medallions and compute path interpolation.
- `field:world_info_t.ypos`: UNKNOWN -> PROVEN. sem_flow: World-map node center coordinate;used to place medallions and compute path interpolation.
- `field:world_info_t.index_up`: UNKNOWN -> PROVEN. sem_flow: Neighbor node index for the named direction;self index means no movement. Shipped table stays0..23.
- `field:world_info_t.index_down`: UNKNOWN -> PROVEN. sem_flow: Neighbor node index for the named direction;self index means no movement. Shipped table stays0..23.
- `field:world_info_t.index_left`: UNKNOWN -> PROVEN. sem_flow: Neighbor node index for the named direction;self index means no movement. Shipped table stays0..23.
- `field:world_info_t.index_right`: UNKNOWN -> PROVEN. sem_flow: Neighbor node index for the named direction;self index means no movement. Shipped table stays0..23.
- `field:world_info_t.is_unlocked`: UNKNOWN -> PROVEN. sem_flow: Bit0 allows navigation/entry and ordinary medallion/path drawing.
- `field:world_info_t.state_bit1`: UNKNOWN -> PROVEN. sem_flow: Bit1 scratch traversal/neighbor mark:DISPLAY_PTS_WAY clears all then marks processed accessible nodes;DO_MEDAILLONS also marks accessible nodes after unlock. Not a separate persistent completion flag.
- `field:world_info_t.is_unlocking`: UNKNOWN -> PROVEN. sem_flow: Bit2 pending unlock;at progress128 and marked neighbor,convert to accessible and clear bits1/2. Saved in packed save bit1.
- `field:world_info_t.world`: UNKNOWN -> PROVEN. sem_flow: Actual game world selected by node;shipped playable nodes1..6,save nodes18..23 carry7.
- `field:world_info_t.level`: UNKNOWN -> PROVEN. sem_flow: Actual starting level selected by node;save nodes carry20 and take separate save UI route.
- `field:world_info_t.color`: UNKNOWN -> STRONG. sem_flow: Signed stage-title text color copied to text display state;exact uses retained.
- `field:world_info_t.text`: UNKNOWN -> PROVEN. sem_flow: Localized node label pointer assigned from language_txt;save nodes replace it with150/151/152 status strings.

## Additional direct DOS review for level refinements

The two shared globals whose worker-specific evidence record was a reference scan received direct producer/consumer review during merge. Evidence is attached to the entities as merge_DOS_contract_review; the other 24 refinements already carry explicit typed/DOS statement evidence.

- `global:compteur`: TestCompteur is freshly STRICT. Word compteur at 2:3f78a is compared signed with max_compteur at 2:3f794; held input increments it, the saturated held path loads delai_repetition at 2:3f79a then increments, and released input stores zero. Reads of button_released at 2:3fa7d gate the paths. This directly supports the refined repeat-counter statement, rather than relying on reference-token hits.
- `global:menuEtape`: The DOS byte at 2:3fa37 indexes the 11-entry code jump table at 1:3996c. Entries 0/3/4/7/8/9/10 branch to 1:39a67/39a87/39ab0/39ae2/39ae9/39afd/39b12: general/save/options/keys/pad/graphics/video respectively; 1/2/5/6 share default 1:39b2a. The return path stores zero to the same byte at 1:39b3a. The separate in-game switch at 1:39cc4 uses 4/7/8/9 and 0 exit. DO_NEW_MENUS remains NEAR (3 byte differences); the directly reviewed table/loads/callback targets support only the stated dispatcher role.
