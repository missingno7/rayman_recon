#ifndef CHEAT_INLINE_H
#define CHEAT_INLINE_H
/* INVENTED purpose name (Android absent; prot_inline/NOTES.md).
 * ORIGINAL_INLINE: DOS v1.21 1:26d73..26d89 inclusive.
 * CMP EAX,0 is C-unselectable (cg_cmp); the near JE skips the two terminating
 * interrupts. Input EAX is the captured JeuCracker. EAX/flags are destroyed;
 * EDX survives to its original POP at 1:26d8a. No result on success.
 * The capture, optional RAY_DEMIRAY and argument MOV remain plain C. */
void exit_if_cracked(s32 cracked);
#pragma aux exit_if_cracked = \
    "cmp eax,0" \
    "je near ptr cracked_done" \
    "mov eax,3" \
    "int 10h" \
    "mov eax,4c00h" \
    "int 21h" \
    "cracked_done:" \
    parm [eax] modify exact [eax]
#endif
