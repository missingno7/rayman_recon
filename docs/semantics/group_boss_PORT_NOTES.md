# Boss/enemy behavior contracts

GOG v1.21. Numeric object types and state pairs are actor-specific. Functions graded STRONG retain their existing NEAR status. No new behavior is inferred from a name or from presence in the asset corpus. Exact predicate/effect records are in `evidence/actor_effects.json`; switch fallthrough remains defined by the checked algorithms in the registries.

## bbmont

DOS callback-table type IDs: 46, 96, 97, 115, 121, 198, 199, 200, 201.

Type 97 param selects launch/bounce behavior; param 3 has a 38-tick lifetime. Pattern 1 wall speeds reverse through magnitudes 12,13,14,15 before destruction. PosPierre=9999 is a sentinel. Attack rows have seven entries with 255 termination. Preserve the six consecutive debris and paired spark assumptions; completion writes finBosslevel.mr_stone.

## pmama

DOS callback-table type IDs: 187, 190.

Five type-190 knives use 20-byte table records and obj.param as a table index (255 invalid). active means target-arrival synchronization, not obj.is_active. Speeds use sixteenths; exact shifts/narrowing matter. Four groups each contain two attacks; group wrap reduces pma_tempo/cou_tempo by ten to minima 100/50. Initial HP=6; delays=150/70. Knife target x/y are camera-relative in the motion readers.

## skops

DOS callback-table type IDs: 150.

Level 10 uses phases 0..3 and collapse platforms; level 11 uses fist/Rayman-directed rays and the stored claw. Skops HP is reduced by returned-ray collision, not by the ordinary phase-specific fist callback. Ray timers 60/120, coordinate sentinel -32000, and clock-gated steering/trail emission are distinct. The first six functions also own scrolling background rendering; keep horizontal/vertical scroll mode branches and heat-wrap arithmetic.

## saxo

DOS callback-table type IDs: 71, 74, 90, 91, 92, 93, 180, 181, 182, 183.

Types 90/91/92/93/102/182 have separate note, fragment, splitting, reflected-note and shaking behaviors. A type-93 collision with the cached 20x140 boss box requests damage. Attack table entries are 4 bytes, launch entries 10 bytes; next-note values 7..9 can select boss actions rather than note objects. Main defeat uses Phase=100 and writes finBosslevel.mr_sax. Shared Phase scratch is not a universal boss-state enum.

## hybrid

DOS callback-table type IDs: 32, 33, 108, 209, 210, 218, 223, 231, 232, 233.

Dark hybrids use links for object IDs and several paired state/HP contracts. STOSKO sprite 6 requires an outgoing fist and eta vulnerability; MOSAMS accepts sprites 3/6. Force fist damage to one at these sites. Dark phase 2 coordinates flame scripts, spell lifetimes, dialogue and Toon fist recovery. Flame contact preserves short existing iframes or resets them to ten; it is not a generic enemy hit rule.

## stoneman

DOS callback-table type IDs: 35, 36, 40, 43, 47, 56, 69, 79, 109, 111, 122, 172.

Keep type-specific pooled allocation, exact animation-frame fire gates and link latches. Stone Woman pursuit uses config, timer and distance thresholds; Stone Dog movement differs by type. DO_PI_EXPLOSION finds a consecutive type-110 run; chip placement instead scans by type/substate. Shared field names do not impose generic gravity/timer/link meaning on every actor.

## dark

DOS callback-table type IDs: 212, 215, 236, 247.

Spell substates 0/1/2/3 force run, reverse commands, toggle tiny and disable fist respectively, once per nonzero HP latch. Cancellation is conditional on effects being present. A full spell pool scans for anim_frame strictly above the initial maximum zero; the selected object index defaults to zero if none wins. dark_phase 1/2/3/5 are transitions with rope/camera/fist setup, not Rayman animation states. Completion restores normal size and writes finBosslevel.mr_dark. Window mode is a repeat count with a 100-tick period.

## meredenis

DOS callback-table type IDs: 98, 192, 193, 213, 244.

Space Mama uses shared encounter/action cursors, separately saved interruption cursors, weapon/machine object IDs and type 98/213 collision-zone switching. Bomb IDs are an 8x8 table with -1 empty; mass explosion sets timer=70, not an immediate explosion call. Encounter 1 advances after >=2 phase hits and stepsForward>2; encounter 2 after >=3. Circle target advances by four and ends at -32000. Preserve action-byte controls and sentinel 254/255 bomb sequence cases.

## moskito

DOS callback-table type IDs: 50, 53, 147, 227.

For initial HP=12, remaining HP >9/>7/>4/>0 selects encounter 3/5/6/7; zero selects 9. Encounter 8 saves/resumes interruption state. Safe timer gates one-point damage and is set to 255. Target sentinel is -32000; speed factor has a 0x4000 base. Low-HP fist evasion is suppressed in demo/recording paths. Do not replace the actor-specific double scale decrement with a generic timer service.

## mite

DOS callback-table type IDs: 100, 217.

follow_id>=500 selects attack/evasion state, with 500/501 transition values; it is not an object ID here. follow_x stores the attack origin and config the travel bound (initially 140). Horizontal speed clamps to [-4,4]. Eta animation-reverse bits can be mutated at runtime. Unused compiler-steering locals retain their names and no gameplay meaning.

## clown

DOS callback-table type IDs: 60, 61, 116, 117, 118.

TNT variants differ in tracking, launch velocity and attack frames. link is an attack latch/retreat selector, timer can reset it after >80. Water-drop pool type 62 and TNT pool type 119 have distinct launch rules. Big clown ground shake affects Rayman support states; preserve that indirect gameplay effect.

## araignee

DOS callback-table type IDs: 65, 195, 196.

Ceiling and ground Spider handlers use different state/HP/attack rules. Ceiling dart homing uses Manhattan normalization with bounded velocities and frame thresholds; the global position_ray selects Rayman target height. Ground dart type 66 launches at +/-3. Keep command-stack label ordering for hit reactions; do not flatten it into a generic state reset.

## bat

DOS callback-table type IDs: 120.

Single-left and single-right fire entries both read sprite 14 in DOS; combined fire reads 15 then 14. The fire direction uses N_anim[abs(dx)*45/abs(dy)], quadrant bases 0/9/18/27 and -1 for coincident horizontal aim. Preserve the literal collision-global usage despite historically confusing names. Fist collision callback is empty; do not introduce damage.

## blacktoon

DOS callback-table type IDs: 123.

config bits independently select ground/sinusoidal/fixed-vector/horizontal behaviors. follow_sprite variants control detection reactions, while follow_id/iframes_timer can hold velocity operands. timer=255 is the waiting sentinel, then 60 is a countdown. Zero HP spawns pooled type-130 eyes; no heap allocation is implied.

## poisson

DOS callback-table type IDs: 10, 104, 132.

Type-10 recycling is grouped by init_x. A living peer with timer<100 blocks another spawn; timer 100 or leaving the screen can trigger reuse. Blue-fish attack begins at 2:16 with (+/-3,-5); terminal animation 2:18 changes to sinking speed 5 before deactivation. Up to two link_init successors can be disabled on fist contact.

## fee

DOS callback-table type IDs: 153.

Dialogue exists at world/level 1:3,1:8,1:17,2:11,3:11 and grants fist,hang,grapple,helicopter,run respectively. Localization substitutions read configured fist/jump/action inputs. Dialogue index 255 represents no current line; old/current line bytes and signed display globals must retain narrowing. Approach preserves Rayman x parity. Delays 100/110/50/120 and sweep +4 pixels are frame-based.

## trumpet

DOS callback-table type IDs: 70.

Wind rectangle is expanded from the animation box and contributes +/-7 to ray_wind_force. Substates 2(frame>=10)/4 versus 6(frame>=10)/7 choose opposite modes. Every wind call reloads timer=100; detection attacks require timer zero. decalage_en_cours receives a shift by eight except while Rayman main state is 5.

## ufo

DOS callback-table type IDs: 162, 163, 166.

On a newly entered in-bounds tile_type 1, match the first active indicator in the same map cell. Subtypes 0/1/2/3 select right/down/left/up; 4 maps vertical to right and horizontal to up, 5 vertical to right and horizontal to down, 6 vertical to left and horizontal to up, 7 vertical to left and horizontal to down. Subtype 8 reverses. 9/10 adjust an already horizontal nonzero speed;11/12 adjust an already vertical nonzero speed, with adjusted magnitudes limited to2..4. Lever cycles linked indicator 0..3 only in state 5:53. Start changes0:13 to0:14 at(2,0).

## cymbal

DOS callback-table type IDs: 81, 168, 169.

Cymbal link values 0..5 are local coordination phases. Paired halves resolve x before y with 4/1 pixel convergence speeds. Closure hurt/capture window is frames 20..22. Centered contact within 25 pixels can deactivate Rayman and attach follow_id; a later opening releases him at state 0:8. Preserve the existing asymmetric speed comparisons and collision-speed recomputation.

## breakout

DOS callback-table type IDs: 256, 257.

Ball_x/y are signed fixed-point accumulators with eight fractional bits. Angle period 72, quadrant 18, speed level 0..7. Ball object substates 0 init, 1 wait/launch, 2 play, 3 life loss, 4 completion delay; kind 6 is a temporary bonus brick. Twenty brick slots come from animation 2. but0pressed launches; leftjoy/rightjoy steer Rayman; TOUCHE(1) requests exit. Enter requests world 1 level 22, saves choice globals; exit restores choices and clears active with num_world=7. Do not reinterpret CBdata as floating-point physics.

## Persistent state and input

The completion paths write the existing `finBosslevel` fields; this pass does not establish a new save-file format. Fairy input substitutions use the existing configured fist/jump/action options; breakout input uses existing logical button/left/right queries, with the literal TOUCHE(1) exit path. Preserve sentinels, packed widths and update order instead of treating the names of shared scratch fields as actor-independent meanings.
