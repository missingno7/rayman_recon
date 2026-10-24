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

/* INVENTED purpose name. ORIGINAL_INLINE: DOS v1.21 1:2662a..26641 (LOAD_CONTINUE_SCREEN).
 * INT 21h AH=36h (free disk space) on the CD drive: drm_byte_CFA27 is the CD drive letter set
 * by cdsave (AudioStatusByte()+'A'), DL = letter-'@' = drive number. AX=FFFFh (invalid drive) or
 * BX != 0 (free clusters: a writable disk, not the CD) terminates with INT 21h AX=4C00h.
 * CMP BX,0 is C-unselectable (cg_cmp; B16 gate). The AH=36h service MOV and DL setup remain C.
 * INT 21h AH=36h returns AX,BX,CX,DX; no C result. */
void exit_if_not_cd_drive(u32 service, u8 drive);
#pragma aux exit_if_not_cd_drive = \
    "int 21h" \
    "cmp ax,0ffffh" \
    "je short not_cd_drive" \
    "cmp bx,0" \
    "jne short not_cd_drive" \
    "jmp short cd_drive_ok" \
    "not_cd_drive:" \
    "mov eax,4c00h" \
    "int 21h" \
    "cd_drive_ok:" \
    parm [eax] [dl] modify exact [eax ebx ecx edx]
#endif
