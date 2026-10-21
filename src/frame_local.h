#ifndef FRAME_INLINE_H
#define FRAME_INLINE_H
/* INVENTED purpose name. ORIGINAL_INLINE: DOS 1:2d768 and 1:2d82b
   contain F3 6E (REP OUTSB). All address/count arithmetic stays in C.
   ESI and ECX advance; EDX and the other registers are unchanged. */
void frame_palette_stream(u32 port, const u8 *source, u32 count);
#pragma aux frame_palette_stream = "rep outsb" parm [edx] [esi] [ecx] modify exact [esi ecx]
/* ORIGINAL_INLINE: DOS 1:2d75d and 1:2d820, EE (OUT DX,AL).
   EAX/EDX carry the full-width C arguments; the instruction preserves them. */
void frame_palette_index(u32 port, u32 color);
#pragma aux frame_palette_index = "out dx,al" parm [edx] [eax] modify exact []
/* INVENTED purpose names. ORIGINAL_INLINE: CLI/STI at DOS
   1:2c597/1:2c59e, 1:2c699/1:2c6a0 and 1:2c763/1:2c76a.
   They change IF only; no general register is modified. */
void frame_disable_irq(void);
#pragma aux frame_disable_irq = "cli" modify exact []
void frame_enable_irq(void);
#pragma aux frame_enable_irq = "sti" modify exact []
#endif
