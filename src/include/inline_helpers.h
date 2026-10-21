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

/* ORIGINAL_INLINE, INVENTED name (Android absent).
 * DOS 1:346f7 (effect_helper_346b4), 1:2881c (Do_Effect_Pause_unknown),
 * 1:32a54 (plot2linejumelle). Complete UNGUARDED sequence below.
 * EAX finishes as byte count; ESI/EDI advance by byte count; ECX becomes 0.
 * EDX/EBX survive 1:6e39e and 1:6e3e9; EBP survives 1:34838.
 * PROVISIONAL original advertised contract: physical clobbers are exact,
 * but full-function proofs remain NEAR in effect34580/effect1/jumelle.
 * Adding EBX gives effect_helper_346b4 STRICT, contradicting live EBX in
 * the band-copy callers. Original source shape / per-unit ABI remains OPEN.
 * Do not integrate as a universally matching contract yet.
 * The guarded 19-byte variant (JE before REP MOVSD) is a separate variant.
 * EDI destination, ESI source, ECX byte count; void; DF cleared.
 * Destroys EAX,ECX,ESI,EDI. Bytes (entire helper):
 * fc 89 c8 83 e1 03 74 02 f3 a4 89 c1 c1 e9 02 f3 a5.
 * Proofs: effect_helper_346b4/effect_helper_34760 (NEAR),
 * Do_Effect_Pause_unknown (NEAR), plot2linejumelle (NEAR). */
void copy(void *destination, const void *source, u32 count);
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
  parm [edi] [esi] [ecx] modify exact [eax ecx esi edi]
#endif
