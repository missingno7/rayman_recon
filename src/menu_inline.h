#ifndef SFIN_MENU_INLINE_H
#define SFIN_MENU_INLINE_H
/* INVENTED purpose name. ORIGINAL_INLINE, DOS 1:39eb5..39edf inclusive.
   Saves/restores ESP through EBX. Its internal CALL supplies ECX=39ec7;
   writers 39ed4/39edb erase that CALL (39ec2..39ec6), and 39eb9 erases
   the backward JMP (39ec7..39ec8), before a temporary CODE-as-stack pivot.
   No arguments/result. EBX/ECX clobbered; ESP restored; flags preserved.
   DOS keeps EAX (color initializer), EDX (zero), ESI/EDI (callbacks) live.
   Evidence: build/workers/x_menutramp/NOTES.md and tramp.asm. */
void menu_patch_once(void);
#pragma aux menu_patch_once = \
    "mov ebx,esp" \
    "jmp short once" \
    "erase_back:" \
    "mov word ptr [ecx],9090h" \
    "mov esp,ecx" \
    "nop" \
    "nop" \
    "once:" \
    "call patch_call" \
    "jmp short erase_back" \
    "jmp short restore" \
    "nop" \
    "nop" \
    "restore:" \
    "mov esp,ebx" \
    "jmp short done" \
    "patch_call:" \
    "mov ecx,dword ptr [esp]" \
    "mov dword ptr [ecx-5],90909090h" \
    "mov byte ptr [ecx-1],90h" \
    "ret" \
    "done:" \
    modify exact [ebx ecx]
/* ORIGINAL_INLINE, INVENTED purpose name (Android absent).
 * DOS v1.21 1:3a45a..3a482 inclusive; continuation 1:3a483.
 * Four DR reads and CMP EAX,0 predicates, explicit success jump, and DOS exit.
 * No inputs/results. EAX and flags destroyed; other GPRs preserved on success.
 * The WindowsLance guard at 1:3a451..3a459 stays C, as do all initializers.
 * Proof: cg_cmp/FINDINGS.md; exact sequence and live context in NOTES.md. */
void menu_check_debug_breakpoints(void);
#pragma aux menu_check_debug_breakpoints = \
    "mov eax,dr0" \
    "cmp eax,0" \
    "jne short breakpoint_found" \
    "mov eax,dr1" \
    "cmp eax,0" \
    "jne short breakpoint_found" \
    "mov eax,dr2" \
    "cmp eax,0" \
    "jne short breakpoint_found" \
    "mov eax,dr3" \
    "cmp eax,0" \
    "jne short breakpoint_found" \
    "jmp short breakpoint_done" \
    "breakpoint_found:" \
    "mov eax,4c00h" \
    "int 21h" \
    "breakpoint_done:" \
    modify exact [eax]
/* Existing instruction-only palette declarations retained verbatim. */
void menu_palette_index(u16 port, u8 index);
#pragma aux menu_palette_index = "out dx,al" parm [dx] [al] modify exact []
/* ORIGINAL_INLINE 1:3ff19 (ee). */
void menu_palette_stream(u16 port, const u8 *source, u32 count);
#pragma aux menu_palette_stream = "rep outsb" parm [dx] [esi] [ecx] modify exact [esi ecx]
/* ORIGINAL_INLINE 1:3ff24 (f36e). */
void menu_palette_disable(void);
#pragma aux menu_palette_disable = "cli" modify exact []
/* ORIGINAL_INLINE 1:3ff1f (fa). */
void menu_palette_enable(void);
#pragma aux menu_palette_enable = "sti" modify exact []
/* ORIGINAL_INLINE 1:3ff26 (fb). */
#endif
