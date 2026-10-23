/* Shared ORIGINAL_INLINE helpers (AGENTS.md exception, user decision 2026-10-21). Watcom 10.6 game profile.
 * Derived and proven by build/workers/w6_helpers (REPORT.md); sites: scratch/helpers_sites.py -v.
 * See REPORT.md: copy's original advertised clobbers remain OPEN.
 * All string forms reproduce DOS bytes: asmcheck.txt.
 * "modify exact" is the compiler contract, distinct from physical writes. */
#ifndef RAYMAN_INLINE_HELPERS_H
#define RAYMAN_INLINE_HELPERS_H
#include "rtypes.h"

/* ORIGINAL_INLINE, RAYVERSE/Android name. DOS 1:0fab0 (DO_BAT_FLASH),
 * 1:45b30 (frapsol), 1:0aa3c (display_text_sin).
 * Physically only AX/flags change; advertised CX clobber is necessary.
 * Preserve original 16-bit negation overflow at value=-32768.
 * AX value, CL shift, AX result; compiler contract destroys AX,CX.
 * Bytes: 66 85 c0 78 05 66 d3 f8 eb 09 66 f7 d8 66 d3 f8 66 f7 d8.
 * Proofs: DO_BAT_FLASH (bat), frapsol (moteur). */
s16 ashr16(s16 value, u8 shift);
#pragma aux ashr16 = \
  "test ax,ax" \
  "js short neg16" \
  "sar ax,cl" \
  "jmp short end16" \
  "neg16:" \
  "neg ax" \
  "sar ax,cl" \
  "neg ax" \
  "end16:" \
  parm [ax] [cl] value [ax] modify exact [ax cx]

/* ORIGINAL_INLINE, INVENTED name (Android absent).
 * DOS 1:45af0 (fades), 1:40bf0 (mereDenisCanAttak), 1:6e7a4.
 * Physically only EAX/flags change. Advertised EBX/ECX clobbers inferred
 * from repeated bounds reloads: EBX 1:6e7c5, ECX 1:607eb/1:6080e.
 * EAX value, EBX lower, ECX upper; signed inclusive range, EAX 0/1.
 * Bytes: 39 d8 7c 0b 39 c8 7f 07 b8 01 00 00 00 eb 02 31 c0.
 * STRICT proofs: fades (moteur), mereDenisCanAttak (meredenis).
 * Those proofs alone do not discriminate narrower modify lists; see REPORT.md. */
s32 inrange(s32 value, s32 lower, s32 upper);
#pragma aux inrange = \
  "cmp eax,ebx" \
  "jl short out_range" \
  "cmp eax,ecx" \
  "jg short out_range" \
  "mov eax,1" \
  "jmp short end_range" \
  "out_range:" \
  "xor eax,eax" \
  "end_range:" \
  parm [eax] [ebx] [ecx] value [eax] modify exact [eax ebx ecx]

/* ORIGINAL_INLINE, RAYVERSE/Android name.
 * DOS 1:577b5 (DoPoissonBleuRaymanZDD), 1:60ac1 (DO_SURF_CHANGE_HAIRS),
 * 1:4bc1c (do_flocons). ECX object pointer survives the first site;
 * EDX sign survives the second site.
 * EAX signed input/result (-1,0,1); only EAX is destroyed.
 * Bytes: 83 f8 00 74 09 b8 01 00 00 00 7f 02 f7 d8.
 * STRICT proofs: DoPoissonBleuRaymanZDD (poisson), DO_SURF_CHANGE_HAIRS (ray). */
s32 sgn(s32 value);
#pragma aux sgn = \
  "cmp eax,0" \
  "je short end_sgn" \
  "mov eax,1" \
  "jg short end_sgn" \
  "neg eax" \
  "end_sgn:" \
  parm [eax] value [eax] modify exact [eax]

/* ORIGINAL_INLINE, INVENTED name (Android absent).
 * DOS 1:27b9e (sub_37B90), 1:26a2d (DEPART_LEVEL),
 * 1:3fd58 (confirmation_msg), 1:25056 (WORLD_CHOICE).
 * No CLD: incoming DF must be zero. Count/color parameters did not reproduce.
 * Other STOSD count variants are not covered by this fixed-size helper.
 * EDI destination; fills 64000 bytes with zero; destroys EAX,ECX,EDI.
 * Bytes: b8 00 00 00 00 b9 80 3e 00 00 f3 ab.
 * STRICT proofs: sub_37B90/sub_37CDC/sub_37D90/sub_37EE0 (effect1),
 * DEPART_LEVEL (cheat), confirmation_msg (menu). */
void fill(void *destination);
#pragma aux fill = \
  "mov eax,0" \
  "mov ecx,16000" \
  "rep stosd" \
  parm [edi] modify exact [eax ecx edi]

/* ORIGINAL_INLINE, INVENTED name (Android lookup absent).
 * Complete 17-byte body, including CLD. Oracle sites: 1:6e39e and 1:6e3e9
 * (effect_copy_band/effect_copy_pause_band), 1:635b5 (effect_helper_634f0).
 * Source ESI, destination EDI, byte count ECX. Source-FIRST C interface:
 * all six formal-argument orders were tested on both band functions; only
 * [esi] [edi] [ecx] closes both complete functions (contracts.json).
 * Physical/validated compiler clobbers: EAX,ECX,ESI,EDI and flags; clears DF.
 * EAX=count; ECX=0; ESI/EDI advance count bytes; memory is written.
 * EBX and EDX remain live at BOTH band sites (subsequent ADDs without reload),
 * EBP remains live across 1:635b5 (subsequent state member accesses).
 * Full-function STRICT: effect_copy_band (76), effect_copy_pause_band (73),
 * effect_helper_634f0 (292). Fresh audit: 0 previous STRICT losses.
 * No result is consumed: void and the historical return declaration are not
 * uniquely determined by these callers. See REPORT.md for remaining callers.
 * Do NOT add EBX for effect_helper_346b4 alone: both band proofs regress.
 * Bytes: fc 89 c8 83 e1 03 74 02 f3 a4 89 c1 c1 e9 02 f3 a5. */
void copy(const void *source, void *destination, u32 count);
#pragma aux copy = \
  "cld" \
  "mov eax,ecx" \
  "and ecx,3" \
  "je short copy_dwords" \
  "rep movsb" \
  "copy_dwords:" \
  "mov ecx,eax" \
  "shr ecx,2" \
  "rep movsd" \
  parm [esi] [edi] [ecx] modify exact [eax ecx esi edi]
/* ORIGINAL_INLINE: DOS 1:54ffb (convertspeed), 1:6e63e (trompetteWind),
 * 1:0c881/0c894 (DO_DARD_PLAFOND_ALWAYS), 1:238b2/238c5 (DO_DARK_SORT_COMMAND).
 * Entire body: 98 d3 e0. AX signed input, CL count, EAX signed result.
 * Physical writes: EAX and flags. CL is advertised destroyed: repeated
 * loads at 1:0c88e and 1:238bf, despite no physical count-register change.
 * Validated compiler contract: modify exact [eax cx]. CL-only and full ECX
 * lists also pass the two proofs; the historical advertised width remains
 * OPEN. Do not claim these two closures uniquely recover that width.
 * STRICT: convertspeed (pmama), trompetteWind (trumpet), zero old STRICT losses.
 * Added 2026-10-21 (user approval); derivation build/workers/w6_shl/REPORT.md.
 * This narrow helper does not cover the bare-SHL ashl32 variant.
 */
s32 ashl16(s16 value, u8 shift);
#pragma aux ashl16 = \
  "cwde" \
  "shl eax,cl" \
  parm [ax] [cl] value [eax] modify exact [eax cx]
/* ORIGINAL_INLINE (port string output): DOS 1:2ae90..2aeab (movieprefix) and 1:2b2ed..2b308 (movie).
 * Saves all registers, selects VGA DAC entry 1 at port 0x3c8, sends 0x2fd palette bytes from ESI+3 to 0x3c9.
 * Contract `modify []` (standard implicit clobbers): proven at both sites - movie needs it (the caller reloads AH and
 * ESI each fade step, 1:2b290/1:2b2e5) and movieprefix matches identically with it (26/41 unchanged). */
void write_movie_palette(u8 *palette);
#pragma aux write_movie_palette = \
  "pushad" \
  "add esi,3" \
  "mov edx,0x3c8" \
  "mov eax,1" \
  "out dx,al" \
  "mov ecx,0x2fd" \
  "mov edx,0x3c9" \
  "rep outsb" \
  "popad" \
  parm [esi] modify []
/* ORIGINAL_INLINE (added 2026-10-21, user approval), INVENTED palette_output (Android lookup absent).
 * Entire identical body: InitPlasma 1:14049..14060,
 * InitFire 1:14472..14489, SYNCHRO_LOOP_PALETTE 1:3ff0f..3ff26,
 * start_fade_in 1:2c587..2c59e. ESI palette base, EAX first,
 * ECX inclusive last. Proofs STRICT: InitPlasma (box), SYNCHRO_LOOP_PALETTE (menu), actualize_palette (frame); build/workers/w8_mine/REPORT.md for proofs and contract limits. */
void palette_output(s32 first, s32 last, const void *palette);
#pragma aux palette_output = \
 "sub ecx,eax" \
 "inc ecx" \
 "imul ecx,ecx,3" \
 "mov dx,03c8h" \
 "out dx,al" \
 "imul eax,eax,3" \
 "add esi,eax" \
 "cli" \
 "mov dx,03c9h" \
 "rep outsb" \
 "sti" \
 parm [eax] [ecx] [esi] modify exact [eax ecx edx esi]
/* ORIGINAL_INLINE (added 2026-10-21, user approval), INVENTED min32/max32 (Android names absent).
 * min: 1:3310d (jumelle), 1:34609 (effect_prepare_rows).
 * max: 1:3311a (jumelle), 1:345d6/345e8/345fe (effect_prepare_rows).
 * EAX first and return; EBX second; signed 32-bit comparison.
 * There is NO instruction-aligned 32-bit instance in a verified STRICT extent;
 * previous menu hit is inside 66 CMP AX,BX; special hit is outside extent.
 * Proofs STRICT: ChangeDeltaSizeJumelle, ChangeDeltaPosX/YJumelleWithoutLimit (jumelle), effect_prepare_rows (effect34580); advertised EBX clobber OPEN ([eax] also passes); build/workers/w8_mine/REPORT.md for contracts, C negatives and full-function proofs. */
s32 min32(s32 a,s32 b);
#pragma aux min32 = \
 "cmp eax,ebx" \
 "jle short min_end" \
 "mov eax,ebx" \
 "min_end:" \
 parm [eax] [ebx] value [eax] modify exact [eax ebx]
s32 max32(s32 a,s32 b);
#pragma aux max32 = \
 "cmp eax,ebx" \
 "jge short max_end" \
 "mov eax,ebx" \
 "max_end:" \
 parm [eax] [ebx] value [eax] modify exact [eax ebx]
/* ORIGINAL_INLINE (gate met 2026-10-22, build/workers/w10_proofs): constant left shift of a 32-bit value through CL
 * (`mov cl,N` is caller code; plain C folds `x << N` to `shl eax,N`). Bytes: d3 e0 (shl eax,cl). Sites: 4 in 2 functions.
 * EAX value, CL count -> EAX; modify exact [eax cx] (CL reload evidence as for ashl16).
 * STRICT proofs: set_ball_pos (breakout), RAY_GOING_BALANCE (ray); 0 STRICT losses in six audited units. */
s32 ashl32(s32 value, u8 shift);
#pragma aux ashl32 =   "shl eax,cl"   parm [eax] [cl] value [eax] modify exact [eax cx]
/* ORIGINAL_INLINE, ashr32 is RAYVERSE/Android; ashr32_inline is an INVENTED
 * conflict-avoiding C spelling (tools/android.py ashr32_inline: absent).
 * DOS uses a zero test, unlike Android's negative test. For nonzero inputs
 * this is an arithmetic shift (rounding down); zero takes the negate path.
 * Exact recurrence in two currently non-STRICT C functions:
 * instantSpeed 1:45e9f..45ead and 1:45eb5..45ec3;
 * ray_inertia_speed 1:5d04d..5d05b. Additional sites 1:0f639,0f6b0,
 * 0f7b2,0f828,16e1b,16e34. EAX signed value/result, CL count.
 * EDX survives instantSpeed (used by sgn at 1:45ec5) and ray_inertia_speed
 * (a2 used at 1:5d08b); EBX/ESI/EDI survive the instantSpeed sites.
 * Count reloads 1:45eb1 and 1:5d061 establish advertised CL destruction.
 * As for ashr16/ashl16, the advertised upper count-register width is OPEN;
 * [eax cx] is the existing shared-helper convention, subject to matching.
 * All sequences and surrounding register use are in helper_evidence.txt.
 * Register uniformity: the exact 7-instruction sequence occurs 4 times (1:0f639, 0f6b0, 0f7b2, 0f828), always EAX/CL.
 * User decision 2026-10-23: accepted.
 */
s32 ashr32_inline(s32 value, u8 shift);
#pragma aux ashr32_inline = \
 "test eax,eax" \
 "jz short ashr32_zero" \
 "sar eax,cl" \
 "jmp short ashr32_end" \
 "ashr32_zero:" \
 "neg eax" \
 "sar eax,cl" \
 "neg eax" \
 "ashr32_end:" \
 parm [eax] [cl] value [eax] modify exact [eax cx]

/* ORIGINAL_INLINE, INVENTED name (Android abs32_inline: absent).
 * Two-site gate: instantSpeed 1:45e85..45e8b and RAY_BALANCE_ANIM
 * 1:5f291..5f297 have the exact TEST EAX / JGE +2 / NEG EAX sequence.
 * Neither current function is STRICT through plain C (97 and 1 bytes,
 * respectively before this worker); plain-C experiments and traces are
 * retained under build/workers/co_ray/out, manual, and perm.
 * EAX signed input/result, physically only EAX/flags change. EDX survives
 * instantSpeed through its later sgn (1:45ec5); EBX integral speed survives
 * through 1:45ed8. RAY_BALANCE_ANIM returns the EAX result at 1:5f298
 * without any intervening transformation. Exact contract: EAX only;
 * no code/register-allocation pragma is added to an individual unit.
 * Inputs at both cited sites lie within -32768..32768, excluding INT_MIN.
 * Register uniformity (supervisor scan 2026-10-23): all 190 TEST/JGE/NEG absolute sequences in DOS game code use
 * EAX (none uses another register; 27 lie in functions already STRICT) - a C macro would vary registers with
 * allocation, so the original was a fixed-EAX inline routine. User decision 2026-10-23: accepted (supersedes the
 * 2026-10-22 rejection of an abs helper).
 */
s32 abs32_inline(s32 value);
#pragma aux abs32_inline = \
 "test eax,eax" \
 "jge short abs32_nonnegative" \
 "neg eax" \
 "abs32_nonnegative:" \
 parm [eax] value [eax] modify exact [eax]

/* Additional verified DOS fixed-EAX absolute sites (co_ray):
 * RAY_BALANCE_ANIM: 1:5f291
 * determineRayAirInertia: 1:5cd1c
 * set_air_speed: 1:5ca93
 * instantSpeed: 1:45e85
 * retour_couteau: 1:5539d, 1:553af, 1:553dd, 1:553ea
 * RAY_THROW_FIST: 1:569d3
 * alter_fist_speed: 1:56cf7, 1:56d0c
 */
/* Additional retained original Abs sites: rayMayLandOnAnObject 1:5c2cd,1:5c310,1:5c8ed */
/* Additional retained original Abs sites: RAY_RESPOND_TO_NOTHING 1:5ed49 */
/* Additional retained original Abs sites: RAY_SURF 1:609de,1:60a3d */
/* Additional retained original Abs sites: DO_AUTO_SCROLL 1:4a0ec */
#endif
