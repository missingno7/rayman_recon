#ifndef PCMAIN_ORIGINAL_INLINE_H
#define PCMAIN_ORIGINAL_INLINE_H
/* ORIGINAL_INLINE: DOS main_Ray 1:083c0..083ea. Each sequence below contains
 * only CPU state access that plain C cannot express. Masks/zero values are C.
 * Names INVENTED. PUSHFD/POPFD transfer EFLAGS; DR accesses name their register.
 * No memory/register allocation patch instructions are present. */
/* ORIGINAL_INLINE 1:083c0 PUSHFD; POP EAX: EAX output, EFLAGS unchanged. */
u32 read_processor_flags(void);
#pragma aux read_processor_flags = "pushfd" "pop eax" value [eax] modify exact [eax]
/* ORIGINAL_INLINE 1:083c7 PUSH EAX; POPFD: EAX preserved, EFLAGS input. */
void write_processor_flags(u32);
#pragma aux write_processor_flags = "push eax" "popfd" parm [eax] modify exact []
/* ORIGINAL_INLINE 1:083cb MOV DR0,EAX: EAX preserved, writes DR0. */
void write_debug_address0(u32);
#pragma aux write_debug_address0 = "mov dr0,eax" parm [eax] modify exact []
/* ORIGINAL_INLINE 1:083ce MOV DR1,EAX: EAX preserved, writes DR1. */
void write_debug_address1(u32);
#pragma aux write_debug_address1 = "mov dr1,eax" parm [eax] modify exact []
/* ORIGINAL_INLINE 1:083d1 MOV DR2,EAX: EAX preserved, writes DR2. */
void write_debug_address2(u32);
#pragma aux write_debug_address2 = "mov dr2,eax" parm [eax] modify exact []
/* ORIGINAL_INLINE 1:083d4 MOV DR3,EAX: EAX preserved, writes DR3. */
void write_debug_address3(u32);
#pragma aux write_debug_address3 = "mov dr3,eax" parm [eax] modify exact []
/* ORIGINAL_INLINE 1:083d7 MOV EAX,DR6: EAX output. */
u32 read_debug_status(void);
#pragma aux read_debug_status = "mov eax,dr6" value [eax] modify exact [eax]
/* ORIGINAL_INLINE 1:083df MOV DR6,EAX: EAX preserved, writes DR6. */
void write_debug_status(u32);
#pragma aux write_debug_status = "mov dr6,eax" parm [eax] modify exact []
/* ORIGINAL_INLINE 1:083e2 MOV EAX,DR7: EAX output. */
u32 read_debug_control(void);
#pragma aux read_debug_control = "mov eax,dr7" value [eax] modify exact [eax]
/* ORIGINAL_INLINE 1:083ea MOV DR7,EAX: EAX preserved, writes DR7. */
void write_debug_control(u32);
#pragma aux write_debug_control = "mov dr7,eax" parm [eax] modify exact []
/* ORIGINAL_INLINE: PcMain 1:08455 and 1:08463, STI only.
 * Enables maskable interrupts; no general register changes. Name INVENTED. */
void enable_tick_interrupts(void);
#pragma aux enable_tick_interrupts = "sti" modify exact []
#endif
