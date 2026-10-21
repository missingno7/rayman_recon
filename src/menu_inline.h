#ifndef SFIN_MENU_INLINE_H
#define SFIN_MENU_INLINE_H
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
