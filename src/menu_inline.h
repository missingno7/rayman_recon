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
/* INVENTED purpose names. ORIGINAL_INLINE instructions verified in DOS v1.21.
   Arithmetic, comparisons and register setup remain ordinary C. */
u32 menu_read_dr0(void);
#pragma aux menu_read_dr0 = "mov eax,dr0" value [eax] modify exact [eax]
/* ORIGINAL_INLINE 1:3a45a (0f21c0). */
u32 menu_read_dr1(void);
#pragma aux menu_read_dr1 = "mov eax,dr1" value [eax] modify exact [eax]
/* ORIGINAL_INLINE 1:3a462 (0f21c8). */
u32 menu_read_dr2(void);
#pragma aux menu_read_dr2 = "mov eax,dr2" value [eax] modify exact [eax]
/* ORIGINAL_INLINE 1:3a46a (0f21d0). */
u32 menu_read_dr3(void);
#pragma aux menu_read_dr3 = "mov eax,dr3" value [eax] modify exact [eax]
/* ORIGINAL_INLINE 1:3a472 (0f21d8). */
void menu_dos_interrupt(u32 service);
#pragma aux menu_dos_interrupt = "int 21h" parm [eax] modify exact [eax]
/* ORIGINAL_INLINE 1:3a481 (cd21); EAX=4c00 is set by C at the call. */
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
