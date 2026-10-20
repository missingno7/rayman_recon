/* Reference only (not compiled): register ABIs of 101 hand-written assembly routines in 1:00000..1:06f80,
   derived from each routine body and its DOS call sites by worker asm_abi (2026-10-20). Names asm_XXXXX and
   parameter names are INVENTED unless Rayverse/Android names exist. Move entries into src/include/lowlevel.h only
   when a unit that calls the routine is integrated and its prototype is checked against the call site. */
/* DOS v1.21 handwritten assembly ABIs. No code in pragmas. Evidence: NOTES.md + abi.json.
   No globals are declared or rebound. OPEN entries are documented, not enabled. */
#ifndef LOWLEVEL_H
#define LOWLEVEL_H
#include "rtypes.h"

/* 1:00010 PROVEN; name INVENTED. 00010 PUSHA; EDI/EAX/CL initialized locally; exit 00043 POPA / 00044 RET. No argument-dependent operation. */
void asm_00010(void);
#pragma aux asm_00010 parm [] modify exact []

/* 1:00045 PROVEN; name INVENTED. 00046 stores incoming EAX as destination; 00055 reads [ESI]; 0005e..00075 reads descriptor bounds. PUSHA at 00045; shared exit 0263e POPA / 0263f RET. Self-modified immediate jump dispatch is internal; no external return there. */
void asm_00045(u32 target_buffer, const void *quad);
#pragma aux asm_00045 parm [eax] [esi] modify exact []

/* 1:0268a PROVEN; name INVENTED. 0268d subtracts 80 from EBX, REP MOVSB reads ESI, 026b8 adds EDX for next plane. 0268a/02734 are PUSH/POPAW (66 prefixes), only low 16 bits restored; upper EBX/ECX/ESI/EDI can change. EAX high part unchanged, EDX full saved around each OUT. RET 02736. */
void asm_0268a(u8 *source, s32 row_stride, s32 plane_stride);
#pragma aux asm_0268a parm [esi] [ebx] [edx] modify exact [ebx ecx esi edi]

/* 1:02737 PROVEN; name INVENTED. 02737 REP MOVSD consumes ESI/EDI/ECX; plain RET 02739. */
void asm_02737(const void *source, void *dest, u32 dword_count);
#pragma aux asm_02737 parm [esi] [edi] [ecx] modify exact [ecx esi edi]

/* 1:0273a OPEN; name INVENTED. 0273a saves EAX/ES, zeros ES, sets AH=83, INT 15h, restores ES/EAX, RET 02747. DOS caller 2b60e sets EBX flag pointer, ECX=7, EDX=a120; incoming AL is not initialized near this call. BIOS/extender service contract and its preserved register set remain OPEN; conservative clobbers here are NOT an accepted ABI. */
/* OPEN: asm_0273a -- declaration intentionally withheld pending evidence. */

/* 1:02748 PROVEN; name INVENTED. 02748 PUSHA; INT 1ah AH=0; 0274d..02756 combine CX:DX; 02758 writes [ESI]; POPA 0275a / RET 0275b. No C return value. */
void asm_02748(u32 *ticks);
#pragma aux asm_02748 parm [esi] modify exact []

/* 1:02783 PROVEN; name RAYVERSE. 02783..02794 initialize EDX/EAX/ECX, REP STOSD uses EDI; RET 02798. Rayverse void(void) stub omits its input. */
void display_emptypicture(u8 *buffer);
#pragma aux display_emptypicture parm [edi] modify exact [eax ecx edx edi]

/* 1:027a2 PROVEN; name RAYVERSE. 027a3 saves ECX plane stride to code data, 027a9 copies EAX height to EBP; 027ab..027bf decode DL stride and DH plane mask. EBX feeds copy count at 027ea. EBP saved/restored 027a2/0283a; helper 02b30 changes AH/EDI; RET 0283b. Packed semantics names INVENTED. */
void display_anypicture(const u8 *source, u8 *dest, s32 height, u32 packed_stride, s32 plane_stride, s32 dword_width);
#pragma aux display_anypicture parm [esi] [edi] [eax] [edx] [ecx] [ebx] modify exact [eax ecx edx esi edi]

/* 1:0283c PROVEN; name INVENTED. 02842 AND EAX,ffff; 02847 copies AL to AH; AL=2 before OUT. Only DX saved/restored; RET 0284f. */
void asm_0283c(u8 mask);
#pragma aux asm_0283c parm [eax] modify exact [eax]

/* 1:02850 PROVEN; name INVENTED. 0285b ECX -> EBX; EDX output records, EAX map entries, ESI indexes and original EDI row skip. Original EBX is consumed from its saved slot at 028cd/028d6/028db. EBX/ECX/EBP restored by 029ce..029d1; EDI restored for every row; RET 029d2. Raw parameter names/types INVENTED: no verified semantic descriptor typedef. */
void asm_02850(u32 reg_eax, u32 reg_edx, u32 reg_ebx, u32 reg_ecx, u32 reg_esi, u32 reg_edi);
#pragma aux asm_02850 parm [eax] [edx] [ebx] [ecx] [esi] [edi] modify exact [eax edx esi]

/* 1:029d3 PROVEN; name INVENTED. 029d5 ECX -> EBX; EDX output records, EAX map entries, ESI indexes, EDI row skip; original EBX is read via its saved slot at 02a34. 029e1 modifies that saved EBX slot: final POP EBX does NOT preserve it. EDI/EBP preserved; RET 02a72. Raw parameter names/types INVENTED: no verified semantic descriptor typedef. */
void asm_029d3(u32 reg_eax, u32 reg_edx, u32 reg_ebx, u32 reg_ecx, u32 reg_esi, u32 reg_edi);
#pragma aux asm_029d3 parm [eax] [edx] [ebx] [ecx] [esi] [edi] modify exact [eax ebx ecx edx esi]

/* 1:02a73 PROVEN; name INVENTED. 02a75 ECX -> EBX; EAX/EDI determine map start; EDX output, ESI indexes. 02aae/02ad9 and 02b1e update saved slots including incoming EBX; final POP EBX is NOT preservation. EDI/EBP preserved; RET 02b2f. Raw parameter names/types INVENTED: no verified semantic descriptor typedef. */
void asm_02a73(u32 reg_eax, u32 reg_edx, u32 reg_ebx, u32 reg_ecx, u32 reg_esi, u32 reg_edi);
#pragma aux asm_02a73 parm [eax] [edx] [ebx] [ecx] [esi] [edi] modify exact [eax ebx ecx edx esi]

/* 1:02b3d PROVEN; name RAYVERSE. 02b3d saves EDI, tests EAX x / EBX y against screen bounds. Computes x/4+80*y from EAX/EBX and original EDI saved on stack; saves/restores ECX/EDX around planar I/O and plotting. All exits restore EDI; EAX/EBX remain changed. Function-pointer setup is in InitModeXWithFrequency 2d937..2d9ba; names STRONG semantic mapping to Android draw_flocon/draw_pluie family. */
void draw_flocon5(u8 *buffer, s32 x, s32 y);
#pragma aux draw_flocon5 parm [edi] [eax] [ebx] modify exact [eax ebx]

/* 1:02bb7 PROVEN; name RAYVERSE. 02bb7 saves EDI, tests EAX x / EBX y against screen bounds. Computes x/4+80*y from EAX/EBX and original EDI saved on stack; saves/restores ECX/EDX around planar I/O and plotting. All exits restore EDI; EAX/EBX remain changed. Function-pointer setup is in InitModeXWithFrequency 2d937..2d9ba; names STRONG semantic mapping to Android draw_flocon/draw_pluie family. */
void draw_flocon1(u8 *buffer, s32 x, s32 y);
#pragma aux draw_flocon1 parm [edi] [eax] [ebx] modify exact [eax ebx]

/* 1:02c00 PROVEN; name RAYVERSE. 02c00 saves EDI, tests EAX x / EBX y against screen bounds. Computes x/4+80*y from EAX/EBX and original EDI saved on stack; saves/restores ECX/EDX around planar I/O and plotting. All exits restore EDI; EAX/EBX remain changed. Function-pointer setup is in InitModeXWithFrequency 2d937..2d9ba; names STRONG semantic mapping to Android draw_flocon/draw_pluie family. */
void draw_flocon2(u8 *buffer, s32 x, s32 y);
#pragma aux draw_flocon2 parm [edi] [eax] [ebx] modify exact [eax ebx]

/* 1:02c59 PROVEN; name RAYVERSE. 02c59 saves EDI, tests EAX x / EBX y against screen bounds. Computes x/4+80*y from EAX/EBX and original EDI saved on stack; saves/restores ECX/EDX around planar I/O and plotting. All exits restore EDI; EAX/EBX remain changed. Function-pointer setup is in InitModeXWithFrequency 2d937..2d9ba; names STRONG semantic mapping to Android draw_flocon/draw_pluie family. */
void draw_flocon3(u8 *buffer, s32 x, s32 y);
#pragma aux draw_flocon3 parm [edi] [eax] [ebx] modify exact [eax ebx]

/* 1:02cbf PROVEN; name RAYVERSE. 02cbf saves EDI, tests EAX x / EBX y against screen bounds. Computes x/4+80*y from EAX/EBX and original EDI saved on stack; saves/restores ECX/EDX around planar I/O and plotting. All exits restore EDI; EAX/EBX remain changed. Function-pointer setup is in InitModeXWithFrequency 2d937..2d9ba; names STRONG semantic mapping to Android draw_flocon/draw_pluie family. */
void draw_pluie4(u8 *buffer, s32 x, s32 y);
#pragma aux draw_pluie4 parm [edi] [eax] [ebx] modify exact [eax ebx]

/* 1:02d08 PROVEN; name RAYVERSE. 02d08 saves EDI, tests EAX x / EBX y against screen bounds. Computes x/4+80*y from EAX/EBX and original EDI saved on stack; saves/restores ECX/EDX around planar I/O and plotting. All exits restore EDI; EAX/EBX remain changed. Function-pointer setup is in InitModeXWithFrequency 2d937..2d9ba; names STRONG semantic mapping to Android draw_flocon/draw_pluie family. */
void draw_flocon4(u8 *buffer, s32 x, s32 y);
#pragma aux draw_flocon4 parm [edi] [eax] [ebx] modify exact [eax ebx]

/* 1:02da9 PROVEN; name RAYVERSE. 02da9 saves EDI, tests EAX x / EBX y against screen bounds. Computes x/4+80*y from EAX/EBX and original EDI saved on stack; saves/restores ECX/EDX around planar I/O and plotting. All exits restore EDI; EAX/EBX remain changed. Function-pointer setup is in InitModeXWithFrequency 2d937..2d9ba; names STRONG semantic mapping to Android draw_flocon/draw_pluie family. */
void draw_pluie5(u8 *buffer, s32 x, s32 y);
#pragma aux draw_pluie5 parm [edi] [eax] [ebx] modify exact [eax ebx]

/* 1:02dfb PROVEN; name RAYVERSE. 02dfb saves EDI, tests EAX x / EBX y against screen bounds. Computes x/4+80*y from EAX/EBX and original EDI saved on stack; saves/restores ECX/EDX around planar I/O and plotting. All exits restore EDI; EAX/EBX remain changed. Function-pointer setup is in InitModeXWithFrequency 2d937..2d9ba; names STRONG semantic mapping to Android draw_flocon/draw_pluie family. */
void draw_flocon7(u8 *buffer, s32 x, s32 y);
#pragma aux draw_flocon7 parm [edi] [eax] [ebx] modify exact [eax ebx]

/* 1:02e9c PROVEN; name RAYVERSE. 02e9c saves EDI, tests EAX x / EBX y against screen bounds. Computes x/4+80*y from EAX/EBX and original EDI saved on stack; saves/restores ECX/EDX around planar I/O and plotting. All exits restore EDI; EAX/EBX remain changed. Function-pointer setup is in InitModeXWithFrequency 2d937..2d9ba; names STRONG semantic mapping to Android draw_flocon/draw_pluie family. */
void draw_pluie6(u8 *buffer, s32 x, s32 y);
#pragma aux draw_pluie6 parm [edi] [eax] [ebx] modify exact [eax ebx]

/* 1:02f0c PROVEN; name RAYVERSE. 02f0c saves EDI, tests EAX x / EBX y against screen bounds. Computes x/4+80*y from EAX/EBX and original EDI saved on stack; saves/restores ECX/EDX around planar I/O and plotting. All exits restore EDI; EAX/EBX remain changed. Function-pointer setup is in InitModeXWithFrequency 2d937..2d9ba; names STRONG semantic mapping to Android draw_flocon/draw_pluie family. */
void draw_flocon6(u8 *buffer, s32 x, s32 y);
#pragma aux draw_flocon6 parm [edi] [eax] [ebx] modify exact [eax ebx]

/* 1:0300f PROVEN; name RAYVERSE. 0300f saves EDI, tests EAX x / EBX y against screen bounds. Computes x/4+80*y from EAX/EBX and original EDI saved on stack; saves/restores ECX/EDX around planar I/O and plotting. All exits restore EDI; EAX/EBX remain changed. Function-pointer setup is in InitModeXWithFrequency 2d937..2d9ba; names STRONG semantic mapping to Android draw_flocon/draw_pluie family. */
void draw_pluie7(u8 *buffer, s32 x, s32 y);
#pragma aux draw_pluie7 parm [edi] [eax] [ebx] modify exact [eax ebx]

/* 1:02b86 PROVEN; name INVENTED. 02b86 saves EDI/ECX/EDX; 02b89..02ba9 forms planar x/4+80*y + saved EDI; 02baf color=ESI -> DL; restores saved registers; RET 02bb6. Name fplot_X INVENTED (Android fplot_Normal only). */
void fplot_X(u8 *buffer, s32 x, s32 y, s32 color);
#pragma aux fplot_X parm [edi] [eax] [ebx] [esi] modify exact [eax ebx]

/* 1:03125 PROVEN; name INVENTED. 03126/0312f test EBX height/ECX width; EDX*80 gives row, ESI gives x (0314b..03152), EDI base; PUSHA/POPA 03125/031eb, RET 031ec. DISPLAY_BLACKBOX at a2c5 confirms registers. */
void asm_03125(u8 *buffer, s32 x, s32 y, s32 height, s32 width);
#pragma aux asm_03125 parm [edi] [esi] [edx] [ebx] [ecx] modify exact []

/* 1:031ed PROVEN; name INVENTED. 031f1 EAX x; 03207..0320f EBX y*80. Original EDI base read from stack 03213; saved EDX width popped into EBP 03217, saved ECX height popped to ECX 03218 and consumed. Only EDI/EBP are restored at 03254/03255. Helper 02b30 changes AH and I/O DX remains 3c4. RET 03256; caller a35b confirms. */
void asm_031ed(u8 *buffer, s32 x, s32 y, s32 height, s32 width);
#pragma aux asm_031ed parm [edi] [eax] [ebx] [ecx] [edx] modify exact [eax ebx ecx edx]

/* 1:03257 PROVEN; name INVENTED. 03257 saves EBP and three words at [ECX+8,+4,+0]; copies ESI record list to EBP; AL -> AH plane mask. EDI destination, EBX source offset, EDX row stride consumed inside each record. Nested copies save/restore EBX/EDX/EDI, but EAX/ECX change; exit 037dd POP ESI twice returns descriptor word [incoming ECX+4] in ESI, discards last local, restores EBP, RET. Raw parameter names/types INVENTED. */
void asm_03257(u32 reg_eax, u32 reg_edx, u32 reg_ebx, u32 reg_ecx, u32 reg_esi, u32 reg_edi);
#pragma aux asm_03257 parm [eax] [edx] [ebx] [ecx] [esi] [edi] modify exact [eax ecx esi]

/* 1:03912 PROVEN; name INVENTED. 03912 saves EBP and three words at [ECX+8,+4,+0]; copies ESI record list to EBP; AL -> AH plane mask. EDI destination, EBX source offset, EDX row stride consumed inside each record. Nested copies save/restore EBX/EDX/EDI, but EAX/ECX change; exit 03e9a POP ESI twice returns descriptor word [incoming ECX+4] in ESI, discards last local, restores EBP, RET. Raw parameter names/types INVENTED. */
void asm_03912(u32 reg_eax, u32 reg_edx, u32 reg_ebx, u32 reg_ecx, u32 reg_esi, u32 reg_edi);
#pragma aux asm_03912 parm [eax] [edx] [ebx] [ecx] [esi] [edi] modify exact [eax ecx esi]

/* 1:03ea1 PROVEN; name INVENTED. 03ea1 saves EBP and three words at [ECX+8,+4,+0]; copies ESI record list to EBP; AL -> AH plane mask. EDI destination, EBX source offset, EDX row stride consumed inside each record. Nested copies save/restore EBX/EDX/EDI, but EAX/ECX change; exit 04429 POP ESI twice returns descriptor word [incoming ECX+4] in ESI, discards last local, restores EBP, RET. Raw parameter names/types INVENTED. */
void asm_03ea1(u32 reg_eax, u32 reg_edx, u32 reg_ebx, u32 reg_ecx, u32 reg_esi, u32 reg_edi);
#pragma aux asm_03ea1 parm [eax] [edx] [ebx] [ecx] [esi] [edi] modify exact [eax ecx esi]

/* 1:037e4 PROVEN; name INVENTED. 037e5 saves record list in EBP; 03804 destination=ECX; record copy loops write EAX/EBX/ECX/EDX/ESI/EDI. Only EBP is preserved globally, 0390f POP EBP; RET 03911. */
void asm_037e4(const void *records, u8 *dest);
#pragma aux asm_037e4 parm [esi] [ecx] modify exact [eax ebx ecx edx esi edi]

/* 1:04430 OPEN; name INVENTED. 04430 saves EBP and ECX twice; ECX -> EBP callback table, EDX -> ESI tile records; [EBX+4,+8,+14] context, EDI dest. At 04486 CALL EDX invokes a dynamically selected callback. Callback bodies/clobbers need separate generated-code audit; local EBX/ECX/EBP saves alone do not establish full ABI. No declaration enabled. */
/* OPEN: asm_04430 -- declaration intentionally withheld pending evidence. */

/* 1:04493 OPEN; name INVENTED. 04493 saves EBP and ECX twice; ECX -> EBP callback table, EDX -> ESI tile records; [EBX+4,+8,+14] context, EDI dest. At 044f7 CALL EDX invokes a dynamically selected callback. Callback bodies/clobbers need separate generated-code audit; local EBX/ECX/EBP saves alone do not establish full ABI. No declaration enabled. */
/* OPEN: asm_04493 -- declaration intentionally withheld pending evidence. */

/* 1:04504 PROVEN; name INVENTED. 04504 PUSHA; EDI screen-base input adjusted by -2, all other working regs initialized; shared POPA immediately before final RET. 0456e additionally clears top/bottom bars with REP STOSD; full register preservation. */
void asm_04504(u8 *buffer);
#pragma aux asm_04504 parm [edi] modify exact []

/* 1:0456e PROVEN; name INVENTED. 0456e PUSHA; EDI screen-base input adjusted by -2, all other working regs initialized; shared POPA immediately before final RET. 0456e additionally clears top/bottom bars with REP STOSD; full register preservation. */
void asm_0456e(u8 *buffer);
#pragma aux asm_0456e parm [edi] modify exact []

/* 1:045f1 PROVEN; name RAYVERSE. 045f1 saves EDX; 045f6 sets AL=0c; OUT DX,AX therefore consumes AH from supplied EAX address. Restores EDX, RET 045fb. Callers load screen address in EAX (2b515); xref get_sprite_clipping_ymax here is WRONG; real getter is 061f4. */
void select_display_buffer(u8 *buffer);
#pragma aux select_display_buffer parm [eax] modify exact [eax]

/* 1:045fc PROVEN; name RAYVERSE. 045fc PUSHA; AX=13h / INT 10h then saves VGA registers to globals; POPA before RET. No argument reads apart from register-save traffic. */
void InitFirstModeVideo(void);
#pragma aux InitFirstModeVideo parm [] modify exact []

/* 1:0465f PROVEN; name RAYVERSE. 0465f PUSHA; restores VGA registers from globals; POPA before RET. No argument reads apart from register-save traffic. */
void InitModeNormal(void);
#pragma aux InitModeNormal parm [] modify exact []

/* 1:046b8 PROVEN; name RAYVERSE. 046b8 PUSHA; programs VGA mode-X register bits; POPA before RET. No argument reads apart from register-save traffic. */
void InitModeX(void);
#pragma aux InitModeX parm [] modify exact []

/* 1:047e7 PROVEN; name RAYVERSE. 047e7 PUSHA; AX=3 / INT 10h; POPA before RET. No argument reads apart from register-save traffic. */
void textmode(void);
#pragma aux textmode parm [] modify exact []

/* 1:0471d PROVEN; name RAYVERSE. 0471d PUSHA, EAX=0/EBX=80/ECX=320, writes [EDI+row/plane offsets]; POPA 04763 / RET 04764. Rayverse source argument unused and omitted. */
void block_clear(u8 *dest);
#pragma aux block_clear parm [edi] modify exact []

/* 1:04765 PROVEN; name RAYVERSE. 04765 PUSHA; 0476b..04773 read tile [ESI], 04779..04781 write EDI planes; POPA 047e5 / RET 047e6. */
void block_copy(const u8 *source, u8 *dest);
#pragma aux block_copy parm [esi] [edi] modify exact []

/* 1:04828 PROVEN; name RAYVERSE. 04829 decrements ESI source, 04832/04835 read [ECX+20/+24], EDI supplies target. Descriptor read at 0484a..048ba; all other regs initialized. PUSHA 04828, POPA 04a81 / RET 04a82. Trailing code at 04a83 is a separate internal entry, not fallthrough. Descriptor type/name INVENTED. */
void draw_sprite_deform(const u8 *source, u8 *dest, const void *descriptor);
#pragma aux draw_sprite_deform parm [esi] [edi] [ecx] modify exact []

/* 1:04ab7 PROVEN; name RAYVERSE. 04ab7 saves ECX; 16000 dword REP MOVSD; restores ECX, RET 04ac0. */
void DisplayBufferModeNormal(const u8 *source, u8 *dest);
#pragma aux DisplayBufferModeNormal parm [esi] [edi] modify exact [esi edi]

/* 1:04ac1 PROVEN; name RAYVERSE. 04ac1 EBX *=384; EDI +=EBX+EAX; fixed stores, RET 04aee. No EAX write. */
void DrawWldPointPlan2Normal(u8 *buffer, s32 x, s32 y);
#pragma aux DrawWldPointPlan2Normal parm [edi] [eax] [ebx] modify exact [ebx edi]

/* 1:04aef PROVEN; name RAYVERSE. 04aef PUSHA; y=EBX*320, x=EAX, width=EDX to ECX REP STOS; outer height=ECX saved at 04b06. POPA 04b2a/RET 04b2b. Rayverse register comments disagree; DOS callers a323/14bc3 prove this order. */
void DrawBlackBoxNormal(u8 *buffer, s32 x, s32 y, s32 height, s32 width);
#pragma aux DrawBlackBoxNormal parm [edi] [eax] [ebx] [ecx] [edx] modify exact []

/* 1:04b2c PROVEN; name RAYVERSE. 04b2c tests EAX type; EBX y*320; ESI x; EDI base; EDX width; ECX height. Two paths end at 04b70/04bba; neither restores EAX/EBX/ECX/ESI/EDI, EDX unchanged. */
void DrawFondBoxNormal(u8 *buffer, s32 x, s32 y, s32 height, s32 width, s32 fond_type);
#pragma aux DrawFondBoxNormal parm [edi] [esi] [ebx] [ecx] [edx] [eax] modify exact [eax ebx ecx esi edi]

/* 1:04bbb PROVEN; name RAYVERSE. 04bbb EBX y*320; 04bc3 ESI x; EDX-- width; ECX height. AL/AH colors used/swapped/AL=AH at 04bf9. Temporary EDI save is computed position, not original base. ESI unchanged; RET 04c18. */
void DrawBorderBoxNormal(u8 *buffer, s32 x, s32 y, s32 height, s32 width, u16 colors);
#pragma aux DrawBorderBoxNormal parm [edi] [esi] [ebx] [ecx] [edx] [eax] modify exact [eax ebx ecx edx edi]

/* 1:04c19 PROVEN; name RAYVERSE. 04c19 multiplies EBX source_y by [ESP+4] stride; source=ESI+EAX; target=EDI+320*EDX+ECX. [ESP+8] width, [ESP+c] height. No GPR saves. RET 12 at 04c68, stack args right-to-left (stride nearest return). */
void DisplayAnyPictureNormal(u8 *source_buffer, u8 *dest_buffer, s32 source_x, s32 source_y, s32 dest_x, s32 dest_y, s32 stride, s32 width, s32 height);
#pragma aux DisplayAnyPictureNormal parm [esi] [edi] [eax] [ebx] [ecx] [edx] modify exact [eax ebx ecx edx esi edi]

/* 1:04c6b PROVEN; name RAYVERSE. 04c6b saves ECX/EAX; clears EDI, EDI=ESI then clears; ECX/EAX restored, ESI unchanged; RET 04c84. */
void ClearDrawAndDisplayBufferNormal(u8 *draw_buf, u8 *display_buf);
#pragma aux ClearDrawAndDisplayBufferNormal parm [edi] [esi] modify exact [edi]

/* 1:04c8e PROVEN; name RAYVERSE. 04c8e saves ECX/EAX, 16000 dword REP STOSD; restores ECX/EAX; RET 04c9b. */
void N_CLRSCR(u8 *buffer);
#pragma aux N_CLRSCR parm [edi] modify exact [edi]

/* 1:04c9c PROVEN; name RAYVERSE. 04c9c PUSHA; ECX/4 width, EAX height, ESI/EDI pointers; row copy, adds global strides; POPA 04cc5 / RET 04cc6. */
void Swap_To_Screen(u8 *draw_buf, u8 *display_buf, s32 width, s32 height);
#pragma aux Swap_To_Screen parm [esi] [edi] [ecx] [eax] modify exact []

/* 1:04cc7 PROVEN; name RAYVERSE. 04cc7 PUSHA; EDX horizontal border distance, ECX/2 pairs of rows; EDI base. POPA 04cf8 / RET 04cf9. */
void clear_borders_Normal(u8 *buffer, s32 height, s32 width);
#pragma aux clear_borders_Normal parm [edi] [ecx] [edx] modify exact []

/* 1:04cfa PROVEN; name RAYVERSE. 04cfb/04d00 test ECX draw_width/EBX height; 04d0c EAX=EDX source row width; ESI/EDI pointers. Eight-byte chunk copy, no stack args. PUSHA/POPA 04cfa/04d53, RET 04d54. 04d55 is a separate internal entry, not part of this ABI. */
void Copy_Plan0Diff_To_Buf(u8 *source, u8 *dest, s32 width, s32 height, s32 draw_width);
#pragma aux Copy_Plan0Diff_To_Buf parm [esi] [edi] [edx] [ebx] [ecx] modify exact []

/* 1:04e64 PROVEN; name RAYVERSE. 04e64 PUSHA; only incoming ESI/EDI are used (16x16 tile source/target). All other regs overwritten before use; POPA/RET 0503f/05040 or 052d4/052d5. No coordinates or width/height arguments. */
void Display_Bloc(u8 *source, u8 *dest);
#pragma aux Display_Bloc parm [esi] [edi] modify exact []

/* 1:05041 PROVEN; name RAYVERSE. 05041 PUSHA; only incoming ESI/EDI are used (16x16 tile source/target). All other regs overwritten before use; POPA/RET 0503f/05040 or 052d4/052d5. No coordinates or width/height arguments. */
void Display_Bloc_Plein(u8 *source, u8 *dest);
#pragma aux Display_Bloc_Plein parm [esi] [edi] modify exact []

/* 1:053c6 PROVEN; name RAYVERSE. 053c6 PUSHA, ECX=1010 (fixed 16x16), calls Clip 052da which reads EAX x/EBX y and may adjust ESI/EDI. No extra stack arguments: 5 bytes reserved locally and removed before POPA/RET. Caller 4d311/4d2f3 loads EAX/EBX/ESI/EDI only. */
void Display_Bloc_Clippe(u8 *source, u8 *dest, s32 x, s32 y);
#pragma aux Display_Bloc_Clippe parm [esi] [edi] [eax] [ebx] modify exact []

/* 1:0555e PROVEN; name RAYVERSE. 0555e PUSHA, ECX=1010 (fixed 16x16), calls Clip 052da which reads EAX x/EBX y and may adjust ESI/EDI. No extra stack arguments: 5 bytes reserved locally and removed before POPA/RET. Caller 4d311/4d2f3 loads EAX/EBX/ESI/EDI only. */
void Display_Bloc_Plein_Clippe(u8 *source, u8 *dest, s32 x, s32 y);
#pragma aux Display_Bloc_Plein_Clippe parm [esi] [edi] [eax] [ebx] modify exact []

/* 1:05609 PROVEN; name RAYVERSE. 05609 PUSHA; EAX +=[EBX*4+row320]; EDX=ESI then DL stored to [EDI+EAX]; POPA/RET 05616/05617. Color only low byte consumed. */
void fplot_Normal(u8 *buffer, s32 x, s32 y, s32 color);
#pragma aux fplot_Normal parm [edi] [eax] [ebx] [esi] modify exact []

/* 1:05628 PROVEN; name RAYVERSE. 05628..05639 store EAX/EBX/ECX/EDX to four bounds; RET 0563f, no GPR changes. */
void flocon_clipping(s32 h1, s32 h2, s32 w1, s32 w2);
#pragma aux flocon_clipping parm [eax] [ebx] [ecx] [edx] modify exact []

/* 1:05640 PROVEN; name RAYVERSE. 05640 saves EAX/EDI; compares EAX against W1/W2 and EBX against H1/H2; address EDI+EAX+[EBX*4+row320]. Fixed-color stores; all exits restore EAX/EDI, EBX never changed. RET no immediate. Actual function-pointer calls share register ABI with the X-mode variants. */
void draw_flocon5_Normal(u8 *buffer, s32 x, s32 y);
#pragma aux draw_flocon5_Normal parm [edi] [eax] [ebx] modify exact []

/* 1:05670 PROVEN; name RAYVERSE. 05670 saves EAX/EDI; compares EAX against W1/W2 and EBX against H1/H2; address EDI+EAX+[EBX*4+row320]. Fixed-color stores; all exits restore EAX/EDI, EBX never changed. RET no immediate. Actual function-pointer calls share register ABI with the X-mode variants. */
void draw_flocon1_Normal(u8 *buffer, s32 x, s32 y);
#pragma aux draw_flocon1_Normal parm [edi] [eax] [ebx] modify exact []

/* 1:056a0 PROVEN; name RAYVERSE. 056a0 saves EAX/EDI; compares EAX against W1/W2 and EBX against H1/H2; address EDI+EAX+[EBX*4+row320]. Fixed-color stores; all exits restore EAX/EDI, EBX never changed. RET no immediate. Actual function-pointer calls share register ABI with the X-mode variants. */
void draw_flocon2_Normal(u8 *buffer, s32 x, s32 y);
#pragma aux draw_flocon2_Normal parm [edi] [eax] [ebx] modify exact []

/* 1:056dc PROVEN; name RAYVERSE. 056dc saves EAX/EDI; compares EAX against W1/W2 and EBX against H1/H2; address EDI+EAX+[EBX*4+row320]. Fixed-color stores; all exits restore EAX/EDI, EBX never changed. RET no immediate. Actual function-pointer calls share register ABI with the X-mode variants. */
void draw_flocon3_Normal(u8 *buffer, s32 x, s32 y);
#pragma aux draw_flocon3_Normal parm [edi] [eax] [ebx] modify exact []

/* 1:05725 PROVEN; name RAYVERSE. 05725 saves EAX/EDI; compares EAX against W1/W2 and EBX against H1/H2; address EDI+EAX+[EBX*4+row320]. Fixed-color stores; all exits restore EAX/EDI, EBX never changed. RET no immediate. Actual function-pointer calls share register ABI with the X-mode variants. */
void draw_pluie4_Normal(u8 *buffer, s32 x, s32 y);
#pragma aux draw_pluie4_Normal parm [edi] [eax] [ebx] modify exact []

/* 1:05755 PROVEN; name RAYVERSE. 05755 saves EAX/EDI; compares EAX against W1/W2 and EBX against H1/H2; address EDI+EAX+[EBX*4+row320]. Fixed-color stores; all exits restore EAX/EDI, EBX never changed. RET no immediate. Actual function-pointer calls share register ABI with the X-mode variants. */
void draw_flocon4_Normal(u8 *buffer, s32 x, s32 y);
#pragma aux draw_flocon4_Normal parm [edi] [eax] [ebx] modify exact []

/* 1:057a6 PROVEN; name RAYVERSE. 057a6 saves EAX/EDI; compares EAX against W1/W2 and EBX against H1/H2; address EDI+EAX+[EBX*4+row320]. Fixed-color stores; all exits restore EAX/EDI, EBX never changed. RET no immediate. Actual function-pointer calls share register ABI with the X-mode variants. */
void draw_pluie5_Normal(u8 *buffer, s32 x, s32 y);
#pragma aux draw_pluie5_Normal parm [edi] [eax] [ebx] modify exact []

/* 1:057df PROVEN; name RAYVERSE. 057df saves EAX/EDI; compares EAX against W1/W2 and EBX against H1/H2; address EDI+EAX+[EBX*4+row320]. Fixed-color stores; all exits restore EAX/EDI, EBX never changed. RET no immediate. Actual function-pointer calls share register ABI with the X-mode variants. */
void draw_flocon7_Normal(u8 *buffer, s32 x, s32 y);
#pragma aux draw_flocon7_Normal parm [edi] [eax] [ebx] modify exact []

/* 1:05830 PROVEN; name RAYVERSE. 05830 saves EAX/EDI; compares EAX against W1/W2 and EBX against H1/H2; address EDI+EAX+[EBX*4+row320]. Fixed-color stores; all exits restore EAX/EDI, EBX never changed. RET no immediate. Actual function-pointer calls share register ABI with the X-mode variants. */
void draw_pluie6_Normal(u8 *buffer, s32 x, s32 y);
#pragma aux draw_pluie6_Normal parm [edi] [eax] [ebx] modify exact []

/* 1:0587d PROVEN; name RAYVERSE. 0587d saves EAX/EDI; compares EAX against W1/W2 and EBX against H1/H2; address EDI+EAX+[EBX*4+row320]. Fixed-color stores; all exits restore EAX/EDI, EBX never changed. RET no immediate. Actual function-pointer calls share register ABI with the X-mode variants. */
void draw_flocon6_Normal(u8 *buffer, s32 x, s32 y);
#pragma aux draw_flocon6_Normal parm [edi] [eax] [ebx] modify exact []

/* 1:058f4 PROVEN; name RAYVERSE. 058f4 saves EAX/EDI; compares EAX against W1/W2 and EBX against H1/H2; address EDI+EAX+[EBX*4+row320]. Fixed-color stores; all exits restore EAX/EDI, EBX never changed. RET no immediate. Actual function-pointer calls share register ABI with the X-mode variants. */
void draw_pluie7_Normal(u8 *buffer, s32 x, s32 y);
#pragma aux draw_pluie7_Normal parm [edi] [eax] [ebx] modify exact []

/* 1:0595d PROVEN; name RAYVERSE. 0595d PUSHA, initializes working registers and reads port 201h; updates joystick globals, optionally calls clock_ticks (EAX only); common STI/POPA/RET restores all GPRs. No source/dest/width/height input: Rayverse stale prototypes differ from DOS. */
void get_joy_input2_dos1(void);
#pragma aux get_joy_input2_dos1 parm [] modify exact []

/* 1:05a57 PROVEN; name RAYVERSE. 05a57 PUSHA, initializes working registers and reads port 201h; updates joystick globals, optionally calls clock_ticks (EAX only); common STI/POPA/RET restores all GPRs. No source/dest/width/height input: Rayverse stale prototypes differ from DOS. */
void get_joy_input2_dos2(void);
#pragma aux get_joy_input2_dos2 parm [] modify exact []

/* 1:05b55 PROVEN; name RAYVERSE. 05b55 PUSHA, initializes working registers and reads port 201h; updates joystick globals, optionally calls clock_ticks (EAX only); common STI/POPA/RET restores all GPRs. No source/dest/width/height input: Rayverse stale prototypes differ from DOS. */
void get_joy_input2_windows1(void);
#pragma aux get_joy_input2_windows1 parm [] modify exact []

/* 1:05c13 PROVEN; name RAYVERSE. 05c13 PUSHA, initializes working registers and reads port 201h; updates joystick globals, optionally calls clock_ticks (EAX only); common STI/POPA/RET restores all GPRs. No source/dest/width/height input: Rayverse stale prototypes differ from DOS. */
void get_joy_input2_windows2(void);
#pragma aux get_joy_input2_windows2 parm [] modify exact []

/* 1:05cdf PROVEN; name RAYVERSE. 05cdf PUSHA; reads port 201h, branches to POPA then EAX=0/1; STI/RET. Only EAX changes, boolean full 32-bit result. */
s32 dos_game_io_port_something(void);
#pragma aux dos_game_io_port_something parm [] value [eax] modify exact [eax]

/* 1:05d14 PROVEN; name RAYVERSE. 05d14 PUSHA; reads port 201h, branches to POPA then EAX=0/1; STI/RET. Only EAX changes, boolean full 32-bit result. */
s32 dos_game_io_port_something2(void);
#pragma aux dos_game_io_port_something2 parm [] value [eax] modify exact [eax]

/* 1:05d4a PROVEN; name RAYVERSE. 05d4a PUSHA; saves EAX height on local stack before joystick polling; ECX width/4, ESI/EDI are copy pointers. Row strides from 1:05951/05955. Every path converges at 05ed6..05edb local cleanup/STI/POPA/RET. No defined C result; original EAX height is restored. */
void get_joy_input1_dos1(u8 *source_buf, u8 *dest_buf, s32 width, s32 height);
#pragma aux get_joy_input1_dos1 parm [esi] [edi] [ecx] [eax] modify exact []

/* 1:05f34 PROVEN; name RAYVERSE. 05f34 PUSHA; saves EAX height on local stack before joystick polling; ECX width/4, ESI/EDI are copy pointers. Row strides from 1:05951/05955. Every path converges at 060c0..060c5 local cleanup/STI/POPA/RET. No defined C result; original EAX height is restored. */
void get_joy_input1_dos2(u8 *source_buf, u8 *dest_buf, s32 width, s32 height);
#pragma aux get_joy_input1_dos2 parm [esi] [edi] [ecx] [eax] modify exact []

/* 1:0611e PROVEN; name RAYVERSE. 0611e PUSH BX (16 bits), flags save/alignment test, optional CPUID at 06167 writes EAX/EBX/ECX/EDX but EDX pushed/popped. 06181 POP BX only restores low EBX; ECX CPUID result and high EBX not restored. Family result in EAX, RET 06183. */
s32 DetectCpu(void);
#pragma aux DetectCpu parm [] value [eax] modify exact [eax ebx ecx]

/* 1:06194 PROVEN; name RAYVERSE. 06194..061b2 direct immediate stores only; RET 061bc; bounds=(8,312,0,200). */
void default_sprite_clipping(void);
#pragma aux default_sprite_clipping parm [] modify exact []

/* 1:061bd PROVEN; name RAYVERSE. 061bd/061be save EBP/EDI, writes [EAX]/[EBX]/[ECX]/[EDX] at 061c5/061cd/061d5/061dd; restores EDI/EBP, RET 061e1. No return value; following 061e2/061e8/061ee/061f4 are separate getters. */
void get_sprite_clipping(s32 *xmin, s32 *xmax, s32 *ymin, s32 *ymax);
#pragma aux get_sprite_clipping parm [eax] [ebx] [ecx] [edx] modify exact []

/* 1:061fa PROVEN; name RAYVERSE. 061fa..0626c clamp and store each input: EAX/EBX 0..320, ECX/EDX 0..200. No saves; all four may change. Callers load EAX/EBX/ECX/EDX in that order. */
void sprite_clipping(s32 xmin, s32 xmax, s32 ymin, s32 ymax);
#pragma aux sprite_clipping parm [eax] [ebx] [ecx] [edx] modify exact [eax ebx ecx edx]

/* 1:063e6 PROVEN; name RAYVERSE. 063e6 overwrites incoming EDX: color is an unused compatibility parameter in Rayverse shared sprite prototype; EAX x, EBX y, CL width/CH height, EDI target, ESI pixels. Clipping helper 0626d/06323 and common body modify six registers; EBP saved/restored around drawing. No stack arguments, RET 0. NoClip entry jumps into adjacent clipped body. size is u16 packed width|height<<8 (INVENTED representation of Rayverse vec2b_t; avoid duplicate struct typedef). */
void DrawSpriteFlipNoClipX(s32 x, s32 color, s32 y, u16 size, u8 *draw_buf, u8 *image_data);
#pragma aux DrawSpriteFlipNoClipX parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi]

/* 1:063f2 PROVEN; name RAYVERSE. 063f2 overwrites incoming EDX: color is an unused compatibility parameter in Rayverse shared sprite prototype; EAX x, EBX y, CL width/CH height, EDI target, ESI pixels. Clipping helper 0626d/06323 and common body modify six registers; EBP saved/restored around drawing. No stack arguments, RET 0. NoClip entry jumps into adjacent clipped body. size is u16 packed width|height<<8 (INVENTED representation of Rayverse vec2b_t; avoid duplicate struct typedef). */
void DrawSpriteX(s32 x, s32 color, s32 y, u16 size, u8 *draw_buf, u8 *image_data);
#pragma aux DrawSpriteX parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi]

/* 1:06559 PROVEN; name RAYVERSE. 06559 overwrites incoming EDX: color is an unused compatibility parameter in Rayverse shared sprite prototype; EAX x, EBX y, CL width/CH height, EDI target, ESI pixels. Clipping helper 0626d/06323 and common body modify six registers; EBP saved/restored around drawing. No stack arguments, RET 0. NoClip entry jumps into adjacent clipped body. size is u16 packed width|height<<8 (INVENTED representation of Rayverse vec2b_t; avoid duplicate struct typedef). */
void DrawSpriteNoClipX(s32 x, s32 color, s32 y, u16 size, u8 *draw_buf, u8 *image_data);
#pragma aux DrawSpriteNoClipX parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi]

/* 1:06565 PROVEN; name RAYVERSE. 06565 overwrites incoming EDX: color is an unused compatibility parameter in Rayverse shared sprite prototype; EAX x, EBX y, CL width/CH height, EDI target, ESI pixels. Clipping helper 0626d/06323 and common body modify six registers; EBP saved/restored around drawing. No stack arguments, RET 0. NoClip entry jumps into adjacent clipped body. size is u16 packed width|height<<8 (INVENTED representation of Rayverse vec2b_t; avoid duplicate struct typedef). */
void DrawSpriteFlipX(s32 x, s32 color, s32 y, u16 size, u8 *draw_buf, u8 *image_data);
#pragma aux DrawSpriteFlipX parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi]

/* 1:066df PROVEN; name RAYVERSE. 066df first saves/scales EDX color before clipping; EAX x, EBX y, CL width/CH height, EDI target, ESI pixels. Clipping helper 0626d/06323 and common body modify six registers; EBP saved/restored around drawing. No stack arguments, RET 0. NoClip entry jumps into adjacent clipped body. size is u16 packed width|height<<8 (INVENTED representation of Rayverse vec2b_t; avoid duplicate struct typedef). */
void DrawSpriteColorX(s32 x, s32 color, s32 y, u16 size, u8 *draw_buf, u8 *image_data);
#pragma aux DrawSpriteColorX parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi]

/* 1:06876 PROVEN; name RAYVERSE. 06876 first saves/scales EDX color before clipping; EAX x, EBX y, CL width/CH height, EDI target, ESI pixels. Clipping helper 0626d/06323 and common body modify six registers; EBP saved/restored around drawing. No stack arguments, RET 0. NoClip entry jumps into adjacent clipped body. size is u16 packed width|height<<8 (INVENTED representation of Rayverse vec2b_t; avoid duplicate struct typedef). */
void DrawSpriteColorFlipX(s32 x, s32 color, s32 y, u16 size, u8 *draw_buf, u8 *image_data);
#pragma aux DrawSpriteColorFlipX parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi]

/* 1:06a18 PROVEN; name RAYVERSE. 06a18 overwrites incoming EDX: color is an unused compatibility parameter in Rayverse shared sprite prototype; EAX x, EBX y, CL width/CH height, EDI target, ESI pixels. Clipping helper 0626d/06323 and common body modify six registers; EBP saved/restored around drawing. No stack arguments, RET 0. NoClip entry jumps into adjacent clipped body. size is u16 packed width|height<<8 (INVENTED representation of Rayverse vec2b_t; avoid duplicate struct typedef). */
void DrawSpriteNormalNoClip(s32 x, s32 color, s32 y, u16 size, u8 *draw_buf, u8 *image_data);
#pragma aux DrawSpriteNormalNoClip parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi]

/* 1:06a24 PROVEN; name RAYVERSE. 06a24 overwrites incoming EDX: color is an unused compatibility parameter in Rayverse shared sprite prototype; EAX x, EBX y, CL width/CH height, EDI target, ESI pixels. Clipping helper 0626d/06323 and common body modify six registers; EBP saved/restored around drawing. No stack arguments, RET 0. NoClip entry jumps into adjacent clipped body. size is u16 packed width|height<<8 (INVENTED representation of Rayverse vec2b_t; avoid duplicate struct typedef). */
void DrawSpriteNormal(s32 x, s32 color, s32 y, u16 size, u8 *draw_buf, u8 *image_data);
#pragma aux DrawSpriteNormal parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi]

/* 1:06a9d PROVEN; name RAYVERSE. 06a9d overwrites incoming EDX: color is an unused compatibility parameter in Rayverse shared sprite prototype; EAX x, EBX y, CL width/CH height, EDI target, ESI pixels. Clipping helper 0626d/06323 and common body modify six registers; EBP saved/restored around drawing. No stack arguments, RET 0. NoClip entry jumps into adjacent clipped body. size is u16 packed width|height<<8 (INVENTED representation of Rayverse vec2b_t; avoid duplicate struct typedef). */
void DrawSpriteNormal256(s32 x, s32 color, s32 y, u16 size, u8 *draw_buf, u8 *image_data);
#pragma aux DrawSpriteNormal256 parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi]

/* 1:06afc PROVEN; name RAYVERSE. 06afc overwrites incoming EDX: color is an unused compatibility parameter in Rayverse shared sprite prototype; EAX x, EBX y, CL width/CH height, EDI target, ESI pixels. Clipping helper 0626d/06323 and common body modify six registers; EBP saved/restored around drawing. No stack arguments, RET 0. NoClip entry jumps into adjacent clipped body. size is u16 packed width|height<<8 (INVENTED representation of Rayverse vec2b_t; avoid duplicate struct typedef). */
void DrawSpriteFlipNormalNoClip(s32 x, s32 color, s32 y, u16 size, u8 *draw_buf, u8 *image_data);
#pragma aux DrawSpriteFlipNormalNoClip parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi]

/* 1:06b08 PROVEN; name RAYVERSE. 06b08 overwrites incoming EDX: color is an unused compatibility parameter in Rayverse shared sprite prototype; EAX x, EBX y, CL width/CH height, EDI target, ESI pixels. Clipping helper 0626d/06323 and common body modify six registers; EBP saved/restored around drawing. No stack arguments, RET 0. NoClip entry jumps into adjacent clipped body. size is u16 packed width|height<<8 (INVENTED representation of Rayverse vec2b_t; avoid duplicate struct typedef). */
void DrawSpriteFlipNormal(s32 x, s32 color, s32 y, u16 size, u8 *draw_buf, u8 *image_data);
#pragma aux DrawSpriteFlipNormal parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi]

/* 1:06b88 PROVEN; name RAYVERSE. 06b88 overwrites incoming EDX: color is an unused compatibility parameter in Rayverse shared sprite prototype; EAX x, EBX y, CL width/CH height, EDI target, ESI pixels. Clipping helper 0626d/06323 and common body modify six registers; EBP saved/restored around drawing. No stack arguments, RET 0. NoClip entry jumps into adjacent clipped body. size is u16 packed width|height<<8 (INVENTED representation of Rayverse vec2b_t; avoid duplicate struct typedef). */
void DrawSpriteFlipNormal256(s32 x, s32 color, s32 y, u16 size, u8 *draw_buf, u8 *image_data);
#pragma aux DrawSpriteFlipNormal256 parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi]

/* 1:06bea PROVEN; name RAYVERSE. 06bea first saves/scales EDX color before clipping; EAX x, EBX y, CL width/CH height, EDI target, ESI pixels. Clipping helper 0626d/06323 and common body modify six registers; EBP saved/restored around drawing. No stack arguments, RET 0. NoClip entry jumps into adjacent clipped body. size is u16 packed width|height<<8 (INVENTED representation of Rayverse vec2b_t; avoid duplicate struct typedef). */
void DrawSpriteColorNormal(s32 x, s32 color, s32 y, u16 size, u8 *draw_buf, u8 *image_data);
#pragma aux DrawSpriteColorNormal parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi]

/* 1:06c89 PROVEN; name RAYVERSE. 06c89 first saves/scales EDX color before clipping; EAX x, EBX y, CL width/CH height, EDI target, ESI pixels. Clipping helper 0626d/06323 and common body modify six registers; EBP saved/restored around drawing. No stack arguments, RET 0. NoClip entry jumps into adjacent clipped body. size is u16 packed width|height<<8 (INVENTED representation of Rayverse vec2b_t; avoid duplicate struct typedef). */
void DrawSpriteColorFlipNormal(s32 x, s32 color, s32 y, u16 size, u8 *draw_buf, u8 *image_data);
#pragma aux DrawSpriteColorFlipNormal parm [eax] [edx] [ebx] [ecx] [edi] [esi] modify exact [eax ebx ecx edx esi edi]

/* 1:06e0d PROVEN; name RAYVERSE. 06e10 saves incoming EDX mask in local stack; helper 06d33 clips EAX/EBX/CL:CH and adjusts ESI pixels/EDI target plus saved mask. Draw loop 06e57 reads [ESI+EBP*4], 06e5f [EDX+EBP*4] mask, AND/OR [EDI+EBP*4]. EBP saved/restored. Local 12-byte frame removed at 06e86, plain RET (NOT RET 12). Rayverse draw/image register comments are swapped; DOS wins. */
void DrawSpriteDiffNormal(s32 x, s32 y, u16 size, u8 *mask, u8 *draw_buf, u8 *image_data);
#pragma aux DrawSpriteDiffNormal parm [eax] [ebx] [ecx] [edx] [edi] [esi] modify exact [eax ebx ecx edx esi edi]

/* 1:06e8a PROVEN; name RAYVERSE. 06e8a saves EDX/EAX, polls 3dah AL bit 3, restores EAX/EDX, plain RET. TEST AX,8 ignores incoming AH; no inputs or return value. */
void synchro(void);
#pragma aux synchro parm [] modify exact []

/* 1:06e9a PROVEN; name RAYVERSE. 06e9a saves EDX/EAX, polls 3dah AL bit 3, restores EAX/EDX, plain RET. TEST AX,8 ignores incoming AH; no inputs or return value. */
void endsynchro(void);
#pragma aux endsynchro parm [] modify exact []

/* 1:06eaa PROVEN; name RAYVERSE. 06eaa saves/restores EAX around speaker/PIT port I/O, no other GPR write; plain RET. No parameters or result. */
void set_speaker_on(void);
#pragma aux set_speaker_on parm [] modify exact []

/* 1:06eea PROVEN; name RAYVERSE. 06eea saves/restores EAX around speaker/PIT port I/O, no other GPR write; plain RET. No parameters or result. */
void set_speaker_off(void);
#pragma aux set_speaker_off parm [] modify exact []

/* 1:06efd PROVEN; name RAYVERSE. 06efd/06efe save EBX/EDX; PIT polling and elapsed-time calculation in EAX, stores timer globals, restores EDX/EBX, RET 06f5c. Full 32-bit EAX result consumed by callers. */
s32 clock_ticks(void);
#pragma aux clock_ticks parm [] value [eax] modify exact [eax]

/* 1:06f5d PROVEN; name RAYVERSE. 06f5d ECX=1; if timer active ECX -=2, calls clock_ticks (EAX), ECX++, compares EAX with EBX target, loops while <=. C callers 07653/07c6c TEST ECX immediately: return is ECX, not EAX. No wait ->1; active -> number of polls minus one. EBX preserved; RET 06f77. */
s32 wait_until(s32 target);
#pragma aux wait_until parm [ebx] value [ecx] modify exact [eax ecx]

/* 1:06f78 PROVEN; name RAYVERSE. 06f78 calls set_speaker_on, which preserves all GPRs; RET 06f7d. No C return value. */
void speaker_enable(void);
#pragma aux speaker_enable parm [] modify exact []

#endif
