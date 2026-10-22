; Reconstruction module; historical ownership remains OPEN (see NOTES.md).

; ---- Accepted source: lowlevel_04430.asm ----
; DOS v1.21 object 1, 1:04430..1:04ab7. Readable reconstruction of the
; address-anchored handwritten assembly scope. The runtime callback pointers
; loaded through EBP are deliberately not declared as static relocations.
.386p
.model flat
_TEXT segment para public use32 'CODE'
assume ds:FLAT

extrn _TableImageX:dword
extrn saved_vga_registers_0196a:byte

public XMIN_, XMAX_, YMIN_, YMAX_
public clip_sprite_on_screen_, clip_sprite_on_screen_flipped_, mode_x_sprite_state_
public asm_04430_
public asm_04493_
public asm_04504_
public asm_0456e_
public select_display_buffer_
public InitFirstModeVideo_
public InitModeNormal_
public InitModeX_
public block_clear_
public block_copy_
public textmode_
public draw_sprite_deform_

asm_04430_:
    push ebp
    push ecx
    push ecx
    mov ebp, ecx
    mov esi, edx
    mov ecx, 0f0h
    mov dword ptr [esp+4], 0eh
    jmp cb_a_row_check
cb_a_row_step:
    dec dword ptr [esp+4]
    je cb_a_return
    add esi, dword ptr [ebx+4]
    cmp esi, dword ptr [ebx+14h]
    jae cb_a_return
    add edi, dword ptr [ebx+8]
cb_a_row_check:
    push ebx
    push esi
    xor ebx, ebx
    mov dword ptr [esp+8], 14h
    jmp cb_a_record_check
cb_a_record_step:
    add esi, 6
    add edi, 4
    dec dword ptr [esp+8]
    je cb_a_records_done
cb_a_record_check:
    mov bl, byte ptr [esi+4]
    cmp bl, 2
    jne cb_a_record_step
    mov bx, word ptr [esi]
    mov edx, dword ptr [ebp+ebx*4]
    cmp edx, 0
    je cb_a_record_step
    push edi
    call edx
    pop edi
    jmp cb_a_record_step
cb_a_records_done:
    pop esi
    pop ebx
    jmp cb_a_row_step
cb_a_return:
    pop ecx
    pop ecx
    pop ebp
    ret

asm_04493_:
    push ebp
    push ecx
    push ecx
    mov ebp, ecx
    mov esi, edx
    mov ecx, 0f0h
    add edi, dword ptr [ebx+8]
    add edi, 50h
    add esi, dword ptr [ebx+4]
    cmp esi, dword ptr [ebx+14h]
    jae cb_b_return
    mov dword ptr [esp+4], 0ch
    jmp cb_b_row_check
cb_b_row_step:
    dec dword ptr [esp+4]
    je cb_b_return
    add esi, dword ptr [ebx+4]
    cmp esi, dword ptr [ebx+14h]
    jae cb_b_return
    add edi, dword ptr [ebx+8]
cb_b_row_check:
    push ebx
    push esi
    xor ebx, ebx
    mov dword ptr [esp+8], 14h
    jmp cb_b_record_check
cb_b_record_step:
    add esi, 6
    add edi, 4
    dec dword ptr [esp+8]
    je cb_b_records_done
cb_b_record_check:
    mov bl, byte ptr [esi+4]
    cmp bl, 2
    jne cb_b_record_step
    mov bx, word ptr [esi]
    mov edx, dword ptr [ebp+ebx*4]
    cmp edx, 0
    je cb_b_record_step
    push edi
    call edx
    pop edi
    jmp cb_b_record_step
cb_b_records_done:
    pop esi
    pop ebx
    jmp cb_b_row_step
cb_b_return:
    pop ecx
    pop ecx
    pop ebp
    ret

asm_04504_:
    pushad
    mov eax, 0f02h
    mov edx, 03c4h
    sub edi, 2
    out dx, ax
    mov ebx, 140h
    xor eax, eax
    mov esi, edi
    mov ecx, 11h
    add esi, 50h
mode_clear_loop:
    mov dword ptr [edi], eax
    mov dword ptr [esi], eax
    mov dword ptr [edi+0a0h], eax
    mov dword ptr [esi+0a0h], eax
    mov dword ptr [edi+ebx], eax
    mov dword ptr [esi+ebx], eax
    mov dword ptr [edi+ebx+0a0h], eax
    mov dword ptr [esi+ebx+0a0h], eax
    mov dword ptr [edi+ebx*2], eax
    mov dword ptr [esi+ebx*2], eax
    mov dword ptr [edi+ebx*2+0a0h], eax
    mov dword ptr [esi+ebx*2+0a0h], eax
    add edi, 3c0h
    add esi, 3c0h
    dec ecx
    jne mode_clear_loop
    popad
    ret

asm_0456e_:
    pushad
    push edi
    mov eax, 0f02h
    mov edx, 03c4h
    sub edi, 2
    out dx, ax
    mov ebx, 140h
    xor eax, eax
    mov esi, edi
    mov ecx, 11h
    add esi, 50h
mode_clear_bars_loop:
    mov dword ptr [edi], eax
    mov dword ptr [esi], eax
    mov dword ptr [edi+0a0h], eax
    mov dword ptr [esi+0a0h], eax
    mov dword ptr [edi+ebx], eax
    mov dword ptr [esi+ebx], eax
    mov dword ptr [edi+ebx+0a0h], eax
    mov dword ptr [esi+ebx+0a0h], eax
    mov dword ptr [edi+ebx*2], eax
    mov dword ptr [esi+ebx*2], eax
    mov dword ptr [edi+ebx*2+0a0h], eax
    mov dword ptr [esi+ebx*2+0a0h], eax
    add edi, 3c0h
    add esi, 3c0h
    dec ecx
    jne mode_clear_bars_loop
    pop edi
    cld
    xor eax, eax
    mov ecx, 140h
    rep stosd
    add edi, 3700h
    mov ecx, 0a0h
    rep stosd
    popad
    ret

select_display_buffer_:
    push edx
    mov dx, 03d4h
    mov al, 0ch
    out dx, ax
    pop edx
    ret

InitFirstModeVideo_:
    pushad
    mov ax, 13h
    int 10h
    mov dx, 03ceh
    mov al, 5
    out dx, al
    inc dx
    in al, dx
    mov byte ptr [saved_vga_registers_0196a], al
    mov dx, 03ceh
    mov al, 6
    out dx, al
    inc dx
    in al, dx
    mov byte ptr [saved_vga_registers_0196a+1], al
    mov dx, 03c4h
    mov al, 4
    out dx, al
    inc dx
    in al, dx
    mov byte ptr [saved_vga_registers_0196a+2], al
    mov dx, 03c4h
    mov al, 2
    out dx, al
    inc dx
    in al, dx
    mov byte ptr [saved_vga_registers_0196a+3], al
    mov dx, 03d4h
    mov al, 14h
    out dx, al
    inc dx
    in al, dx
    mov byte ptr [saved_vga_registers_0196a+4], al
    mov dx, 03d4h
    mov al, 17h
    out dx, al
    inc dx
    in al, dx
    mov byte ptr [saved_vga_registers_0196a+5], al
    popad
    ret

InitModeNormal_:
    pushad
    mov dx, 03ceh
    mov al, 5
    out dx, al
    inc dx
    mov al, byte ptr [saved_vga_registers_0196a]
    out dx, al
    dec dx
    mov al, 6
    out dx, al
    inc dx
    mov al, byte ptr [saved_vga_registers_0196a+1]
    out dx, al
    mov dx, 03c4h
    mov al, 4
    out dx, al
    inc dx
    mov al, byte ptr [saved_vga_registers_0196a+2]
    out dx, al
    mov dx, 03c4h
    mov al, 2
    out dx, al
    inc dx
    mov al, byte ptr [saved_vga_registers_0196a+3]
    out dx, al
    mov dx, 03d4h
    mov al, 14h
    out dx, al
    inc dx
    mov al, byte ptr [saved_vga_registers_0196a+4]
    out dx, al
    dec dx
    mov al, 17h
    out dx, al
    inc dx
    mov al, byte ptr [saved_vga_registers_0196a+5]
    out dx, al
    popad
    ret

InitModeX_:
    pushad
    mov dx, 03ceh
    mov al, 5
    out dx, al
    inc dx
    mov al, byte ptr [saved_vga_registers_0196a]
    and al, 0efh
    out dx, al
    dec dx
    mov al, 6
    out dx, al
    inc dx
    mov al, byte ptr [saved_vga_registers_0196a+1]
    and al, 0fdh
    out dx, al
    mov dx, 03c4h
    mov al, 4
    out dx, al
    inc dx
    mov al, byte ptr [saved_vga_registers_0196a+2]
    and al, 0f7h
    or al, 4
    out dx, al
    mov dx, 03c4h
    mov al, 2
    out dx, al
    inc dx
    mov al, byte ptr [saved_vga_registers_0196a+3]
    out dx, al
    mov dx, 03d4h
    mov al, 14h
    out dx, al
    inc dx
    mov al, byte ptr [saved_vga_registers_0196a+4]
    and al, 0bfh
    out dx, al
    dec dx
    mov al, 17h
    out dx, al
    inc dx
    mov al, byte ptr [saved_vga_registers_0196a+5]
    or al, 40h
    out dx, al
    popad
    ret

block_clear_:
    pushad
    mov eax, 0
    mov ebx, 50h
    mov ecx, 140h
block_clear_rows:
    mov dword ptr [edi], eax
    mov dword ptr [edi+ebx], eax
    mov dword ptr [edi+ebx*2], eax
    mov dword ptr [edi+ebx*2+50h], eax
    add edi, ecx
    mov dword ptr [edi], eax
    mov dword ptr [edi+ebx], eax
    mov dword ptr [edi+ebx*2], eax
    mov dword ptr [edi+ebx*2+50h], eax
    add edi, ecx
    mov dword ptr [edi], eax
    mov dword ptr [edi+ebx], eax
    mov dword ptr [edi+ebx*2], eax
    mov dword ptr [edi+ebx*2+50h], eax
    add edi, ecx
    mov dword ptr [edi], eax
    mov dword ptr [edi+ebx], eax
    mov dword ptr [edi+ebx*2], eax
    mov dword ptr [edi+ebx*2+50h], eax
    popad
    ret

block_copy_:
    pushad
    mov ebp, 50h
block_copy_row_1:
    mov eax, dword ptr [esi]
    mov ebx, dword ptr [esi+4]
    mov ecx, dword ptr [esi+8]
    mov edx, dword ptr [esi+0ch]
    add esi, 10h
    mov dword ptr [edi], eax
    mov dword ptr [edi+ebp], ebx
    mov dword ptr [edi+ebp*2], ecx
    mov dword ptr [edi+ebp*2+50h], edx
    add edi, 140h
    mov eax, dword ptr [esi]
    mov ebx, dword ptr [esi+4]
    mov ecx, dword ptr [esi+8]
    mov edx, dword ptr [esi+0ch]
    add esi, 10h
    mov dword ptr [edi], eax
    mov dword ptr [edi+ebp], ebx
    mov dword ptr [edi+ebp*2], ecx
    mov dword ptr [edi+ebp*2+50h], edx
    add edi, 140h
    mov eax, dword ptr [esi]
    mov ebx, dword ptr [esi+4]
    mov ecx, dword ptr [esi+8]
    mov edx, dword ptr [esi+0ch]
    add esi, 10h
    mov dword ptr [edi], eax
    mov dword ptr [edi+ebp], ebx
    mov dword ptr [edi+ebp*2], ecx
    mov dword ptr [edi+ebp*2+50h], edx
    add edi, 140h
    mov eax, dword ptr [esi]
    mov ebx, dword ptr [esi+4]
    mov ecx, dword ptr [esi+8]
    mov edx, dword ptr [esi+0ch]
    add esi, 10h
    mov dword ptr [edi], eax
    mov dword ptr [edi+ebp], ebx
    mov dword ptr [edi+ebp*2], ecx
    mov dword ptr [edi+ebp*2+50h], edx
    popad
    ret

textmode_:
    pushad
    mov ax, 3
    int 10h
    popad
    ret

; PROVEN embedded writable state 1:047f0..1:04828: fourteen zero-initialised
; dwords. The deformation routine writes these at 1:0484f..1:0489f and
; 1:048af..1:04a77, then reads/updates them throughout 1:048b0..1:04a77.
; Their exact zero contents are confirmed in the extracted DOS object.
state_047f0: dd 0
state_047f4: dd 0
state_047f8: dd 0
state_047fc: dd 0
state_04800: dd 0
state_04804: dd 0
state_04808: dd 0
state_0480c: dd 0
state_04810: dd 0
state_04814: dd 0
state_04818: dd 0
state_0481c: dd 0
state_04820: dd 0
state_04824: dd 0

draw_sprite_deform_:
    pushad
    dec esi
    mov dword ptr [state_04810], esi
    push ecx
    push edi
    mov ebx, dword ptr [ecx+20h]
    mov edx, dword ptr [ecx+24h]
    sar ebx, 2
    mov eax, 0a1a1a1a1h
    inc ebx
deform_fill_loop:
    mov ecx, ebx
    rep stosd
    dec edx
    jne deform_fill_loop
    pop edi
    pop ecx
    mov eax, dword ptr [ecx]
    mov ebx, dword ptr [ecx+4]
    mov dword ptr [state_047f0], eax
    mov dword ptr [state_047f4], ebx
    mov eax, dword ptr [ecx+8]
    mov ebx, dword ptr [ecx+0ch]
    mov dword ptr [state_047f8], eax
    mov dword ptr [state_047fc], ebx
    mov eax, dword ptr [ecx+10h]
    mov ebx, dword ptr [ecx+14h]
    mov dword ptr [state_04800], eax
    mov dword ptr [state_04804], ebx
    mov eax, dword ptr [ecx+18h]
    mov ebx, dword ptr [ecx+1ch]
    mov dword ptr [state_04808], eax
    mov dword ptr [state_0480c], ebx
    mov edx, ebx
    add edx, eax
    mov dword ptr [state_04824], edx
    mov ebx, dword ptr [ecx+2ch]
    mov eax, dword ptr [ecx+30h]
    mov dword ptr [state_04820], ebx
    sar edx, 1fh
    shl eax, 10h
    div dword ptr [state_04824]
    mov dword ptr [state_0481c], eax
    mov esi, dword ptr [state_04810]
    add edi, dword ptr [ecx+28h]
    mov eax, dword ptr [state_047f8]
    mov ebx, dword ptr [state_047fc]
    xor ecx, ecx
    xor ebp, ebp
    xor edx, edx
    cmp eax, ebx
    jbe deform_y_ascending
    mov ebx, dword ptr [state_0480c]
    push eax
    mov eax, dword ptr [state_04820]
    sar edx, 1fh
    shl eax, 10h
    div dword ptr [state_047f8]
    mov dword ptr [state_04818], eax
    mov dword ptr [state_04814], eax
    pop eax
    xor edx, edx
deform_y_descending_row:
    push ecx
    push edi
    push ebp
    push ebx
    push edx
    xor ecx, ecx
    xor ebp, ebp
    xor ebx, ebx
deform_y_descending_pixel:
    push ebx
    shr ebx, 10h
    mov edx, dword ptr [esi+ebx]
    inc ebp
    pop ebx
    cmp byte ptr [edi], 0a1h
    jne deform_y_descending_no_pixel
    mov byte ptr [edi], dh
deform_y_descending_no_pixel:
    add ebx, dword ptr [state_04818]
    add ecx, dword ptr [state_047fc]
    cmp ebp, eax
    jge deform_y_descending_pixels_done
    add edi, dword ptr [state_047f0]
    cmp ecx, dword ptr [state_047f8]
    jbe deform_y_descending_pixel
    add edi, dword ptr [state_047f4]
    sub ecx, dword ptr [state_047f8]
    jmp deform_y_descending_pixel
deform_y_descending_pixels_done:
    pop edx
    pop ebx
    pop ebp
    pop edi
    pop ecx
    inc ebp
    mov esi, dword ptr [state_04810]
    add ecx, dword ptr [state_04808]
    add edi, dword ptr [state_04804]
    add edx, dword ptr [state_0481c]
    push edx
    shr edx, 10h
deform_y_descending_advance:
    je deform_y_descending_advanced
    add esi, dword ptr [state_04820]
    dec edx
    jmp deform_y_descending_advance
deform_y_descending_advanced:
    mov edx, dword ptr [state_04814]
    mov dword ptr [state_04818], edx
    pop edx
    cmp ebp, ebx
    jge deform_done
    cmp ecx, 0
    jle deform_y_descending_row
    add edi, dword ptr [state_04800]
    dec ebp
    sub ecx, dword ptr [state_04824]
    sub edi, dword ptr [state_04804]
    sub dword ptr [state_04818], 40h
    jmp deform_y_descending_row

deform_y_ascending:
    mov eax, dword ptr [state_04820]
    sar edx, 1fh
    shl eax, 10h
    div dword ptr [state_047fc]
    mov dword ptr [state_04818], eax
    mov dword ptr [state_04814], eax
    xor edx, edx
    mov ebx, dword ptr [state_04808]
    mov eax, dword ptr [state_047fc]
deform_y_ascending_row:
    push ecx
    push edi
    push ebp
    push ebx
    push edx
    xor ecx, ecx
    xor ebp, ebp
    xor ebx, ebx
deform_y_ascending_pixel:
    push ebx
    shr ebx, 10h
    mov edx, dword ptr [esi+ebx]
    inc ebp
    pop ebx
    cmp byte ptr [edi], 81h
    jne deform_y_ascending_no_pixel
    mov byte ptr [edi], dh
deform_y_ascending_no_pixel:
    add ebx, dword ptr [state_04818]
    add ecx, dword ptr [state_047f8]
    cmp ebp, eax
    jge deform_y_ascending_pixels_done
    add edi, dword ptr [state_047f4]
    cmp ecx, 0
    jle deform_y_ascending_pixel
    add edi, dword ptr [state_047f0]
    sub ecx, dword ptr [state_047fc]
    jmp deform_y_ascending_pixel
deform_y_ascending_pixels_done:
    pop edx
    pop ebx
    pop ebp
    pop edi
    pop ecx
    inc ebp
    mov esi, dword ptr [state_04810]
    add ecx, dword ptr [state_0480c]
    add edi, dword ptr [state_04800]
    add edx, dword ptr [state_0481c]
    push edx
    shr edx, 10h
deform_y_ascending_advance:
    je deform_y_ascending_advanced
    add esi, dword ptr [state_04820]
    dec edx
    jmp deform_y_ascending_advance
deform_y_ascending_advanced:
    mov edx, dword ptr [state_04814]
    mov dword ptr [state_04818], edx
    pop edx
    cmp ebp, ebx
    jge deform_done
    cmp ecx, dword ptr [state_04808]
    jle deform_y_ascending_row
    add edi, dword ptr [state_04804]
    dec ebp
    sub ecx, dword ptr [state_04824]
    sub edi, dword ptr [state_04800]
    sub dword ptr [state_04818], 40h
    jmp deform_y_ascending_row
deform_done:
    popad
    ret

; Unreferenced local code entry at 1:04a83 (not made PUBLIC). Its TableImageX
; lookup is the four-dword table bound at its true start, 2:0195a.
local_04a83:
    xor edx, edx
local_04a85:
    xor ecx, ecx
local_04a87:
    mov eax, ecx
    mov ebx, ecx
    shr ebx, 2
    and eax, 3
    mov eax, dword ptr [_TableImageX+eax*4]
    add eax, ebx
    add eax, edx
    mov bl, byte ptr [esi]
    mov byte ptr [edi+eax], bl
    inc esi
    inc ecx
    cmp ecx, 140h
    jne local_04a87
    add edx, 50h
    cmp edx, 3e80h
    jne local_04a85
    ret

_TEXT ends

; ---- Accepted source: lowlevel_04ab7.asm ----
; Readable reconstruction of DOS hand-written low-level CODE 1:04ab7..1:05609.
; Oracle authority: GOG v1.21; address-anchored CFG: build/workers/w5_asmscope/cfg.dis.
.386p
_TEXT segment para public use32 'CODE'
public DisplayBufferModeNormal_, DrawWldPointPlan2Normal_, DrawBlackBoxNormal_, DrawFondBoxNormal_
public DrawBorderBoxNormal_, DisplayAnyPictureNormal_, ClearDrawAndDisplayBufferNormal_, N_CLRSCR_
public Swap_To_Screen_, clear_borders_Normal_, Copy_Plan0Diff_To_Buf_, asm_04d55_
public Display_Bloc_, Display_Bloc_Plein_, Display_Bloc_Clippe_, Display_Bloc_Plein_Clippe_
extrn _Bloc_lim_W2:dword
extrn _Bloc_lim_W1:dword
extrn _Bloc_lim_H2:dword
extrn _Bloc_lim_H1:dword
DisplayBufferModeNormal_:
    push ecx ; 1:04ab7
loc_04AB8:
    mov ecx, 0x3e80 ; 1:04ab8
loc_04ABD:
    rep movsd ; 1:04abd
loc_04ABF:
    pop ecx ; 1:04abf
loc_04AC0:
    ret ; 1:04ac0
DrawWldPointPlan2Normal_:
    imul ebx, ebx, 0x180 ; 1:04ac1
loc_04AC7:
    add edi, ebx ; 1:04ac7
loc_04AC9:
    add edi, eax ; 1:04ac9
loc_04ACB:
    mov word ptr [edi + 1], 0xc00 ; 1:04acb
loc_04AD1:
    mov dword ptr [edi + 0x180], 0xc0e0a00 ; 1:04ad1
loc_04ADB:
    mov dword ptr [edi + 0x300], 0xa0900 ; 1:04adb
loc_04AE5:
    mov word ptr [edi + 0x481], 0 ; 1:04ae5
loc_04AEE:
    ret ; 1:04aee
DrawBlackBoxNormal_:
    pushad ; 1:04aef
loc_04AF0:
    imul ebx, ebx, 0x140 ; 1:04af0
loc_04AF6:
    add edi, ebx ; 1:04af6
loc_04AF8:
    add edi, eax ; 1:04af8
loc_04AFA:
    mov eax, 0 ; 1:04afa
loc_04AFF:
    mov ebx, 0x140 ; 1:04aff
loc_04B04:
    sub ebx, edx ; 1:04b04
loc_04B06:
    push ecx ; 1:04b06
loc_04B07:
    mov ecx, edx ; 1:04b07
loc_04B09:
    test ecx, 1 ; 1:04b09
loc_04B0F:
    je loc_04B13 ; 1:04b0f
loc_04B11:
    stosb ; 1:04b11
loc_04B12:
    dec ecx ; 1:04b12
loc_04B13:
    test ecx, 2 ; 1:04b13
loc_04B19:
    je loc_04B20 ; 1:04b19
loc_04B1B:
    stosw ; 1:04b1b
loc_04B1D:
    sub ecx, 2 ; 1:04b1d
loc_04B20:
    sar ecx, 2 ; 1:04b20
loc_04B23:
    rep stosd ; 1:04b23
loc_04B25:
    add edi, ebx ; 1:04b25
loc_04B27:
    pop ecx ; 1:04b27
loc_04B28:
    loop loc_04B06 ; 1:04b28
loc_04B2A:
    popad ; 1:04b2a
loc_04B2B:
    ret ; 1:04b2b
DrawFondBoxNormal_:
    cmp eax, 2 ; 1:04b2c
loc_04B2F:
    je loc_04B71 ; 1:04b2f
loc_04B31:
    imul ebx, ebx, 0x140 ; 1:04b31
loc_04B37:
    add edi, ebx ; 1:04b37
loc_04B39:
    add edi, esi ; 1:04b39
loc_04B3B:
    mov esi, edi ; 1:04b3b
loc_04B3D:
    add esi, 0x141 ; 1:04b3d
loc_04B43:
    mov ebx, 0x140 ; 1:04b43
loc_04B48:
    sub ebx, edx ; 1:04b48
loc_04B4A:
    push ecx ; 1:04b4a
loc_04B4B:
    mov ecx, edx ; 1:04b4b
loc_04B4D:
    mov al, byte ptr [esi] ; 1:04b4d
loc_04B4F:
    mov ah, al ; 1:04b4f
loc_04B51:
    and ah, 0xe0 ; 1:04b51
loc_04B54:
    sub al, 5 ; 1:04b54
loc_04B56:
    xor ah, al ; 1:04b56
loc_04B58:
    and ah, 0xe0 ; 1:04b58
loc_04B5B:
    je loc_04B61 ; 1:04b5b
loc_04B5D:
    mov al, ah ; 1:04b5d
loc_04B5F:
    and al, 0xe0 ; 1:04b5f
loc_04B61:
    mov byte ptr [edi], al ; 1:04b61
loc_04B63:
    inc edi ; 1:04b63
loc_04B64:
    inc esi ; 1:04b64
loc_04B65:
    dec ecx ; 1:04b65
loc_04B66:
    jne loc_04B4D ; 1:04b66
loc_04B68:
    add edi, ebx ; 1:04b68
loc_04B6A:
    add esi, ebx ; 1:04b6a
loc_04B6C:
    pop ecx ; 1:04b6c
loc_04B6D:
    dec ecx ; 1:04b6d
loc_04B6E:
    jne loc_04B4A ; 1:04b6e
loc_04B70:
    ret ; 1:04b70
loc_04B71:
    add ebx, ecx ; 1:04b71
loc_04B73:
    imul ebx, ebx, 0x140 ; 1:04b73
loc_04B79:
    add edi, ebx ; 1:04b79
loc_04B7B:
    add edi, edx ; 1:04b7b
loc_04B7D:
    add edi, esi ; 1:04b7d
loc_04B7F:
    sub edi, 0x141 ; 1:04b7f
loc_04B85:
    mov esi, edi ; 1:04b85
loc_04B87:
    sub esi, 0x141 ; 1:04b87
loc_04B8D:
    mov ebx, 0x140 ; 1:04b8d
loc_04B92:
    sub ebx, edx ; 1:04b92
loc_04B94:
    push ecx ; 1:04b94
loc_04B95:
    mov ecx, edx ; 1:04b95
loc_04B97:
    mov al, byte ptr [esi] ; 1:04b97
loc_04B99:
    mov ah, al ; 1:04b99
loc_04B9B:
    and ah, 0xe0 ; 1:04b9b
loc_04B9E:
    add al, 5 ; 1:04b9e
loc_04BA0:
    xor ah, al ; 1:04ba0
loc_04BA2:
    and ah, 0xe0 ; 1:04ba2
loc_04BA5:
    je loc_04BAB ; 1:04ba5
loc_04BA7:
    mov al, ah ; 1:04ba7
loc_04BA9:
    or al, 0x1f ; 1:04ba9
loc_04BAB:
    mov byte ptr [edi], al ; 1:04bab
loc_04BAD:
    dec edi ; 1:04bad
loc_04BAE:
    dec esi ; 1:04bae
loc_04BAF:
    dec ecx ; 1:04baf
loc_04BB0:
    jne loc_04B97 ; 1:04bb0
loc_04BB2:
    sub edi, ebx ; 1:04bb2
loc_04BB4:
    sub esi, ebx ; 1:04bb4
loc_04BB6:
    pop ecx ; 1:04bb6
loc_04BB7:
    dec ecx ; 1:04bb7
loc_04BB8:
    jne loc_04B94 ; 1:04bb8
loc_04BBA:
    ret ; 1:04bba
DrawBorderBoxNormal_:
    imul ebx, ebx, 0x140 ; 1:04bbb
loc_04BC1:
    add edi, ebx ; 1:04bc1
loc_04BC3:
    add edi, esi ; 1:04bc3
loc_04BC5:
    dec edx ; 1:04bc5
loc_04BC6:
    mov ebx, ecx ; 1:04bc6
loc_04BC8:
    push edi ; 1:04bc8
loc_04BC9:
    mov byte ptr [edi], al ; 1:04bc9
loc_04BCB:
    add edi, 0x140 ; 1:04bcb
loc_04BD1:
    dec ecx ; 1:04bd1
loc_04BD2:
    jne loc_04BC9 ; 1:04bd2
loc_04BD4:
    mov ecx, edx ; 1:04bd4
loc_04BD6:
    sub edi, 0x140 ; 1:04bd6
loc_04BDC:
    inc edi ; 1:04bdc
loc_04BDD:
    xchg ah, al ; 1:04bdd
loc_04BDF:
    rep stosb ; 1:04bdf
loc_04BE1:
    add edi, 0x13f ; 1:04be1
loc_04BE7:
    sub edi, edx ; 1:04be7
loc_04BE9:
    mov ecx, edx ; 1:04be9
loc_04BEB:
    inc ecx ; 1:04beb
loc_04BEC:
    rep stosb ; 1:04bec
loc_04BEE:
    pop edi ; 1:04bee
loc_04BEF:
    xchg ah, al ; 1:04bef
loc_04BF1:
    mov ecx, edx ; 1:04bf1
loc_04BF3:
    inc ecx ; 1:04bf3
loc_04BF4:
    rep stosb ; 1:04bf4
loc_04BF6:
    mov ecx, ebx ; 1:04bf6
loc_04BF8:
    dec ecx ; 1:04bf8
loc_04BF9:
    mov al, ah ; 1:04bf9
loc_04BFB:
    add edi, 0x13f ; 1:04bfb
loc_04C01:
    mov byte ptr [edi], al ; 1:04c01
loc_04C03:
    mov byte ptr [edi - 0x13f], al ; 1:04c03
loc_04C09:
    add edi, 0x140 ; 1:04c09
loc_04C0F:
    dec ecx ; 1:04c0f
loc_04C10:
    jne loc_04C01 ; 1:04c10
loc_04C12:
    mov byte ptr [edi - 0x13f], al ; 1:04c12
loc_04C18:
    ret ; 1:04c18
DisplayAnyPictureNormal_:
    imul ebx, dword ptr [esp + 4] ; 1:04c19
loc_04C1E:
    add esi, ebx ; 1:04c1e
loc_04C20:
    add esi, eax ; 1:04c20
loc_04C22:
    imul edx, edx, 0x140 ; 1:04c22
loc_04C28:
    add edi, edx ; 1:04c28
loc_04C2A:
    add edi, ecx ; 1:04c2a
loc_04C2C:
    mov ebx, 0x140 ; 1:04c2c
loc_04C31:
    sub ebx, dword ptr [esp + 8] ; 1:04c31
loc_04C35:
    mov edx, dword ptr [esp + 4] ; 1:04c35
loc_04C39:
    sub edx, dword ptr [esp + 8] ; 1:04c39
loc_04C3D:
    mov eax, dword ptr [esp + 0xc] ; 1:04c3d
loc_04C41:
    mov ecx, dword ptr [esp + 8] ; 1:04c41
loc_04C45:
    test ecx, 1 ; 1:04c45
loc_04C4B:
    je loc_04C4F ; 1:04c4b
loc_04C4D:
    movsb ; 1:04c4d
loc_04C4E:
    dec ecx ; 1:04c4e
loc_04C4F:
    test ecx, 2 ; 1:04c4f
loc_04C55:
    je loc_04C5C ; 1:04c55
loc_04C57:
    movsw ; 1:04c57
loc_04C59:
    sub ecx, 2 ; 1:04c59
loc_04C5C:
    sar ecx, 2 ; 1:04c5c
loc_04C5F:
    rep movsd ; 1:04c5f
loc_04C61:
    add edi, ebx ; 1:04c61
loc_04C63:
    add esi, edx ; 1:04c63
loc_04C65:
    dec eax ; 1:04c65
loc_04C66:
    jne loc_04C41 ; 1:04c66
loc_04C68:
    ret 0xc ; 1:04c68
ClearDrawAndDisplayBufferNormal_:
    push ecx ; 1:04c6b
loc_04C6C:
    push eax ; 1:04c6c
loc_04C6D:
    mov eax, 0 ; 1:04c6d
loc_04C72:
    mov ecx, 0x3e80 ; 1:04c72
loc_04C77:
    rep stosd ; 1:04c77
loc_04C79:
    mov edi, esi ; 1:04c79
loc_04C7B:
    mov ecx, 0x3e80 ; 1:04c7b
loc_04C80:
    rep stosd ; 1:04c80
loc_04C82:
    pop eax ; 1:04c82
loc_04C83:
    pop ecx ; 1:04c83
loc_04C84:
    ret ; 1:04c84
loc_04C85:
    ret ; 1:04c85
; PROVEN embedded stride state 1:04c86..04c8e: two dwords 320.
; Reads: 1:04caf/04cb5, 04ec0, 04f14, 04f68, 04fbc, 0538d, 05429, 05499, 054f4, 05537, 055a2, 055c3, 055de, 055f3.
state_stride_04c86:
dd 320, 320
N_CLRSCR_:
    push ecx ; 1:04c8e
loc_04C8F:
    push eax ; 1:04c8f
loc_04C90:
    mov ecx, 0x3e80 ; 1:04c90
loc_04C95:
    xor eax, eax ; 1:04c95
loc_04C97:
    rep stosd ; 1:04c97
loc_04C99:
    pop eax ; 1:04c99
loc_04C9A:
    pop ecx ; 1:04c9a
loc_04C9B:
    ret ; 1:04c9b
Swap_To_Screen_:
    pushad ; 1:04c9c
loc_04C9D:
    sub esp, 4 ; 1:04c9d
loc_04CA0:
    shr ecx, 2 ; 1:04ca0
loc_04CA3:
    mov dword ptr [esp], ecx ; 1:04ca3
loc_04CA6:
    mov ebx, esi ; 1:04ca6
loc_04CA8:
    mov edx, edi ; 1:04ca8
loc_04CAA:
    mov ecx, dword ptr [esp] ; 1:04caa
loc_04CAD:
    rep movsd ; 1:04cad
loc_04CAF:
    add ebx, dword ptr [state_stride_04c86]   ; fix@+2->1:4c86 ; 1:04caf
loc_04CB5:
    add edx, dword ptr [state_stride_04c86+4]   ; fix@+2->1:4c8a ; 1:04cb5
loc_04CBB:
    mov esi, ebx ; 1:04cbb
loc_04CBD:
    mov edi, edx ; 1:04cbd
loc_04CBF:
    dec eax ; 1:04cbf
loc_04CC0:
    jne loc_04CAA ; 1:04cc0
loc_04CC2:
    add esp, 4 ; 1:04cc2
loc_04CC5:
    popad ; 1:04cc5
loc_04CC6:
    ret ; 1:04cc6
clear_borders_Normal_:
    pushad ; 1:04cc7
loc_04CC8:
    mov ebx, 0x140 ; 1:04cc8
loc_04CCD:
    lea eax, [edx + 0x140] ; 1:04ccd
loc_04CD3:
    shr ecx, 1 ; 1:04cd3
loc_04CD5:
    mov dword ptr [edi], 0 ; 1:04cd5
loc_04CDB:
    mov dword ptr [edi + edx], 0 ; 1:04cdb
loc_04CE2:
    mov esi, edi ; 1:04ce2
loc_04CE4:
    mov dword ptr [edi + ebx], 0 ; 1:04ce4
loc_04CEB:
    mov dword ptr [edi + eax], 0 ; 1:04ceb
loc_04CF2:
    dec ecx ; 1:04cf2
loc_04CF3:
    lea edi, [esi + ebx*2] ; 1:04cf3
loc_04CF6:
    jne loc_04CD5 ; 1:04cf6
loc_04CF8:
    popad ; 1:04cf8
loc_04CF9:
    ret ; 1:04cf9
Copy_Plan0Diff_To_Buf_:
    pushad ; 1:04cfa
loc_04CFB:
    cmp ecx, 0 ; 1:04cfb
loc_04CFE:
    jle loc_04D53 ; 1:04cfe
loc_04D00:
    cmp ebx, 0 ; 1:04d00
loc_04D03:
    jle loc_04D53 ; 1:04d03
loc_04D05:
    sub esp, 8 ; 1:04d05
loc_04D08:
    mov dword ptr [esp + 4], ebx ; 1:04d08
loc_04D0C:
    mov eax, edx ; 1:04d0c
loc_04D0E:
    mov dword ptr [esp], ecx ; 1:04d0e
loc_04D11:
    mov ebp, esi ; 1:04d11
loc_04D13:
    shr dword ptr [esp], 3 ; 1:04d13
loc_04D17:
    and ecx, 7 ; 1:04d17
loc_04D1A:
    je loc_04D1F ; 1:04d1a
loc_04D1C:
    inc dword ptr [esp] ; 1:04d1c
loc_04D1F:
    mov ebx, edi ; 1:04d1f
loc_04D21:
    mov ecx, dword ptr [esp] ; 1:04d21
loc_04D24:
    push ebx ; 1:04d24
loc_04D25:
    push edx ; 1:04d25
loc_04D26:
    mov edx, dword ptr [esi] ; 1:04d26
loc_04D28:
    mov ebx, dword ptr [esi + 4] ; 1:04d28
loc_04D2B:
    mov dword ptr [edi], edx ; 1:04d2b
loc_04D2D:
    mov dword ptr [edi + 4], ebx ; 1:04d2d
loc_04D30:
    add esi, 8 ; 1:04d30
loc_04D33:
    add edi, 8 ; 1:04d33
loc_04D36:
    sub ecx, 1 ; 1:04d36
loc_04D39:
    jne loc_04D26 ; 1:04d39
loc_04D3B:
    pop edx ; 1:04d3b
loc_04D3C:
    pop ebx ; 1:04d3c
loc_04D3D:
    lea esi, [ebp + edx] ; 1:04d3d
loc_04D41:
    lea edx, [edx + eax] ; 1:04d41
loc_04D44:
    lea edi, [ebx + 0x140] ; 1:04d44
loc_04D4A:
    dec dword ptr [esp + 4] ; 1:04d4a
loc_04D4E:
    jne loc_04D1F ; 1:04d4e
loc_04D50:
    add esp, 8 ; 1:04d50
loc_04D53:
    popad ; 1:04d53
loc_04D54:
    ret ; 1:04d54
asm_04d55_:
    pushad ; 1:04d55
loc_04D56:
    cmp ecx, 0 ; 1:04d56
loc_04D59:
    jle loc_04E62 ; 1:04d59
loc_04D5F:
    cmp ebx, 0 ; 1:04d5f
loc_04D62:
    jle loc_04E62 ; 1:04d62
loc_04D68:
    sub esp, 0x14 ; 1:04d68
loc_04D6B:
    mov dword ptr [esp + 4], ebx ; 1:04d6b
loc_04D6F:
    mov eax, edx ; 1:04d6f
loc_04D71:
    lea edx, [edx*8] ; 1:04d71
loc_04D78:
    mov dword ptr [esp], ecx ; 1:04d78
loc_04D7B:
    mov dword ptr [esp + 0x10], esi ; 1:04d7b
loc_04D7F:
    shr dword ptr [esp], 3 ; 1:04d7f
loc_04D83:
    and ecx, 7 ; 1:04d83
loc_04D86:
    je loc_04D8B ; 1:04d86
loc_04D88:
    inc dword ptr [esp] ; 1:04d88
loc_04D8B:
    mov ecx, dword ptr [esp] ; 1:04d8b
loc_04D8E:
    mov dword ptr [esp + 8], edi ; 1:04d8e
loc_04D92:
    mov dword ptr [esp + 0xc], edx ; 1:04d92
loc_04D96:
    mov edx, dword ptr [esi] ; 1:04d96
loc_04D98:
    mov ebx, dword ptr [esi + 4] ; 1:04d98
loc_04D9B:
    mov dword ptr [edi], edx ; 1:04d9b
loc_04D9D:
    mov dword ptr [edi + 4], ebx ; 1:04d9d
loc_04DA0:
    mov edx, dword ptr [esi + eax] ; 1:04da0
loc_04DA3:
    mov ebx, dword ptr [esi + eax + 4] ; 1:04da3
loc_04DA7:
    mov dword ptr [edi + 0x140], edx ; 1:04da7
loc_04DAD:
    mov dword ptr [edi + 0x144], ebx ; 1:04dad
loc_04DB3:
    mov edx, dword ptr [esi + eax*2] ; 1:04db3
loc_04DB6:
    mov ebx, dword ptr [esi + eax*2 + 4] ; 1:04db6
loc_04DBA:
    mov dword ptr [edi + 0x280], edx ; 1:04dba
loc_04DC0:
    mov dword ptr [edi + 0x284], ebx ; 1:04dc0
loc_04DC6:
    mov edx, dword ptr [esi + eax*4] ; 1:04dc6
loc_04DC9:
    mov ebx, dword ptr [esi + eax*4 + 4] ; 1:04dc9
loc_04DCD:
    mov dword ptr [edi + 0x500], edx ; 1:04dcd
loc_04DD3:
    mov dword ptr [edi + 0x504], ebx ; 1:04dd3
loc_04DD9:
    lea ebp, [eax + eax*4] ; 1:04dd9
loc_04DDC:
    mov edx, dword ptr [esi + ebp] ; 1:04ddc
loc_04DDF:
    mov ebx, dword ptr [esi + ebp + 4] ; 1:04ddf
loc_04DE3:
    mov dword ptr [edi + 0x640], edx ; 1:04de3
loc_04DE9:
    mov dword ptr [edi + 0x644], ebx ; 1:04de9
loc_04DEF:
    lea ebp, [eax + eax*2] ; 1:04def
loc_04DF2:
    mov edx, dword ptr [esi + ebp] ; 1:04df2
loc_04DF5:
    mov ebx, dword ptr [esi + ebp + 4] ; 1:04df5
loc_04DF9:
    mov dword ptr [edi + 0x3c0], edx ; 1:04df9
loc_04DFF:
    mov dword ptr [edi + 0x3c4], ebx ; 1:04dff
loc_04E05:
    mov edx, dword ptr [esi + ebp*2] ; 1:04e05
loc_04E08:
    mov ebx, dword ptr [esi + ebp*2 + 4] ; 1:04e08
loc_04E0C:
    mov dword ptr [edi + 0x780], edx ; 1:04e0c
loc_04E12:
    mov dword ptr [edi + 0x784], ebx ; 1:04e12
loc_04E18:
    lea ebp, [eax + ebp*2] ; 1:04e18
loc_04E1B:
    mov edx, dword ptr [esi + ebp] ; 1:04e1b
loc_04E1E:
    mov ebx, dword ptr [esi + ebp + 4] ; 1:04e1e
loc_04E22:
    mov dword ptr [edi + 0x8c0], edx ; 1:04e22
loc_04E28:
    mov dword ptr [edi + 0x8c4], ebx ; 1:04e28
loc_04E2E:
    add esi, 8 ; 1:04e2e
loc_04E31:
    add edi, 8 ; 1:04e31
loc_04E34:
    dec ecx ; 1:04e34
loc_04E35:
    jne loc_04D96 ; 1:04e35
loc_04E3B:
    mov ebp, dword ptr [esp + 0x10] ; 1:04e3b
loc_04E3F:
    mov edx, dword ptr [esp + 0xc] ; 1:04e3f
loc_04E43:
    mov ebx, dword ptr [esp + 8] ; 1:04e43
loc_04E47:
    lea esi, [ebp + edx] ; 1:04e47
loc_04E4B:
    lea edx, [edx + eax*8] ; 1:04e4b
loc_04E4E:
    lea edi, [ebx + 0xa00] ; 1:04e4e
loc_04E54:
    sub dword ptr [esp + 4], 8 ; 1:04e54
loc_04E59:
    jne loc_04D8B ; 1:04e59
loc_04E5F:
    add esp, 0x14 ; 1:04e5f
loc_04E62:
    popad ; 1:04e62
loc_04E63:
    ret ; 1:04e63
Display_Bloc_:
    pushad ; 1:04e64
loc_04E65:
    mov eax, dword ptr [esi + 0x200] ; 1:04e65
loc_04E6B:
    sub esp, 4 ; 1:04e6b
loc_04E6E:
    mov dword ptr [esp], 4 ; 1:04e6e
loc_04E75:
    test eax, 0x40000000 ; 1:04e75
loc_04E7A:
    jne loc_04EC0 ; 1:04e7a
loc_04E7C:
    test eax, 0x80000000 ; 1:04e7c
loc_04E81:
    jne loc_04FD3 ; 1:04e81
loc_04E87:
    mov ecx, dword ptr [esi + 0x100] ; 1:04e87
loc_04E8D:
    mov ebp, dword ptr [esi + 0x104] ; 1:04e8d
loc_04E93:
    mov ebx, dword ptr [esi] ; 1:04e93
loc_04E95:
    mov edx, dword ptr [esi + 4] ; 1:04e95
loc_04E98:
    and dword ptr [edi], ecx ; 1:04e98
loc_04E9A:
    and dword ptr [edi + 4], ebp ; 1:04e9a
loc_04E9D:
    or dword ptr [edi], ebx ; 1:04e9d
loc_04E9F:
    or dword ptr [edi + 4], edx ; 1:04e9f
loc_04EA2:
    mov ecx, dword ptr [esi + 0x108] ; 1:04ea2
loc_04EA8:
    mov ebp, dword ptr [esi + 0x10c] ; 1:04ea8
loc_04EAE:
    mov ebx, dword ptr [esi + 8] ; 1:04eae
loc_04EB1:
    mov edx, dword ptr [esi + 0xc] ; 1:04eb1
loc_04EB4:
    and dword ptr [edi + 8], ecx ; 1:04eb4
loc_04EB7:
    and dword ptr [edi + 0xc], ebp ; 1:04eb7
loc_04EBA:
    or dword ptr [edi + 8], ebx ; 1:04eba
loc_04EBD:
    or dword ptr [edi + 0xc], edx ; 1:04ebd
loc_04EC0:
    add edi, dword ptr [state_stride_04c86]   ; fix@+2->1:4c86 ; 1:04ec0
loc_04EC6:
    add esi, 0x10 ; 1:04ec6
loc_04EC9:
    test eax, 0x10000000 ; 1:04ec9
loc_04ECE:
    jne loc_04F14 ; 1:04ece
loc_04ED0:
    test eax, 0x20000000 ; 1:04ed0
loc_04ED5:
    jne loc_04FEE ; 1:04ed5
loc_04EDB:
    mov ecx, dword ptr [esi + 0x100] ; 1:04edb
loc_04EE1:
    mov ebp, dword ptr [esi + 0x104] ; 1:04ee1
loc_04EE7:
    mov ebx, dword ptr [esi] ; 1:04ee7
loc_04EE9:
    mov edx, dword ptr [esi + 4] ; 1:04ee9
loc_04EEC:
    and dword ptr [edi], ecx ; 1:04eec
loc_04EEE:
    and dword ptr [edi + 4], ebp ; 1:04eee
loc_04EF1:
    or dword ptr [edi], ebx ; 1:04ef1
loc_04EF3:
    or dword ptr [edi + 4], edx ; 1:04ef3
loc_04EF6:
    mov ecx, dword ptr [esi + 0x108] ; 1:04ef6
loc_04EFC:
    mov ebp, dword ptr [esi + 0x10c] ; 1:04efc
loc_04F02:
    mov ebx, dword ptr [esi + 8] ; 1:04f02
loc_04F05:
    mov edx, dword ptr [esi + 0xc] ; 1:04f05
loc_04F08:
    and dword ptr [edi + 8], ecx ; 1:04f08
loc_04F0B:
    and dword ptr [edi + 0xc], ebp ; 1:04f0b
loc_04F0E:
    or dword ptr [edi + 8], ebx ; 1:04f0e
loc_04F11:
    or dword ptr [edi + 0xc], edx ; 1:04f11
loc_04F14:
    add edi, dword ptr [state_stride_04c86]   ; fix@+2->1:4c86 ; 1:04f14
loc_04F1A:
    add esi, 0x10 ; 1:04f1a
loc_04F1D:
    test eax, 0x4000000 ; 1:04f1d
loc_04F22:
    jne loc_04F68 ; 1:04f22
loc_04F24:
    test eax, 0x8000000 ; 1:04f24
loc_04F29:
    jne loc_05009 ; 1:04f29
loc_04F2F:
    mov ecx, dword ptr [esi + 0x100] ; 1:04f2f
loc_04F35:
    mov ebp, dword ptr [esi + 0x104] ; 1:04f35
loc_04F3B:
    mov ebx, dword ptr [esi] ; 1:04f3b
loc_04F3D:
    mov edx, dword ptr [esi + 4] ; 1:04f3d
loc_04F40:
    and dword ptr [edi], ecx ; 1:04f40
loc_04F42:
    and dword ptr [edi + 4], ebp ; 1:04f42
loc_04F45:
    or dword ptr [edi], ebx ; 1:04f45
loc_04F47:
    or dword ptr [edi + 4], edx ; 1:04f47
loc_04F4A:
    mov ecx, dword ptr [esi + 0x108] ; 1:04f4a
loc_04F50:
    mov ebp, dword ptr [esi + 0x10c] ; 1:04f50
loc_04F56:
    mov ebx, dword ptr [esi + 8] ; 1:04f56
loc_04F59:
    mov edx, dword ptr [esi + 0xc] ; 1:04f59
loc_04F5C:
    and dword ptr [edi + 8], ecx ; 1:04f5c
loc_04F5F:
    and dword ptr [edi + 0xc], ebp ; 1:04f5f
loc_04F62:
    or dword ptr [edi + 8], ebx ; 1:04f62
loc_04F65:
    or dword ptr [edi + 0xc], edx ; 1:04f65
loc_04F68:
    add edi, dword ptr [state_stride_04c86]   ; fix@+2->1:4c86 ; 1:04f68
loc_04F6E:
    add esi, 0x10 ; 1:04f6e
loc_04F71:
    test eax, 0x1000000 ; 1:04f71
loc_04F76:
    jne loc_04FBC ; 1:04f76
loc_04F78:
    test eax, 0x2000000 ; 1:04f78
loc_04F7D:
    jne loc_05024 ; 1:04f7d
loc_04F83:
    mov ecx, dword ptr [esi + 0x100] ; 1:04f83
loc_04F89:
    mov ebp, dword ptr [esi + 0x104] ; 1:04f89
loc_04F8F:
    mov ebx, dword ptr [esi] ; 1:04f8f
loc_04F91:
    mov edx, dword ptr [esi + 4] ; 1:04f91
loc_04F94:
    and dword ptr [edi], ecx ; 1:04f94
loc_04F96:
    and dword ptr [edi + 4], ebp ; 1:04f96
loc_04F99:
    or dword ptr [edi], ebx ; 1:04f99
loc_04F9B:
    or dword ptr [edi + 4], edx ; 1:04f9b
loc_04F9E:
    mov ecx, dword ptr [esi + 0x108] ; 1:04f9e
loc_04FA4:
    mov ebp, dword ptr [esi + 0x10c] ; 1:04fa4
loc_04FAA:
    mov ebx, dword ptr [esi + 8] ; 1:04faa
loc_04FAD:
    mov edx, dword ptr [esi + 0xc] ; 1:04fad
loc_04FB0:
    and dword ptr [edi + 8], ecx ; 1:04fb0
loc_04FB3:
    and dword ptr [edi + 0xc], ebp ; 1:04fb3
loc_04FB6:
    or dword ptr [edi + 8], ebx ; 1:04fb6
loc_04FB9:
    or dword ptr [edi + 0xc], edx ; 1:04fb9
loc_04FBC:
    add edi, dword ptr [state_stride_04c86]   ; fix@+2->1:4c86 ; 1:04fbc
loc_04FC2:
    add esi, 0x10 ; 1:04fc2
loc_04FC5:
    shl eax, 8 ; 1:04fc5
loc_04FC8:
    dec dword ptr [esp] ; 1:04fc8
loc_04FCB:
    jne loc_04E75 ; 1:04fcb
loc_04FD1:
    jmp loc_0503C ; 1:04fd1
loc_04FD3:
    mov ebx, dword ptr [esi] ; 1:04fd3
loc_04FD5:
    mov ecx, dword ptr [esi + 4] ; 1:04fd5
loc_04FD8:
    mov edx, dword ptr [esi + 8] ; 1:04fd8
loc_04FDB:
    mov ebp, dword ptr [esi + 0xc] ; 1:04fdb
loc_04FDE:
    mov dword ptr [edi], ebx ; 1:04fde
loc_04FE0:
    mov dword ptr [edi + 4], ecx ; 1:04fe0
loc_04FE3:
    mov dword ptr [edi + 8], edx ; 1:04fe3
loc_04FE6:
    mov dword ptr [edi + 0xc], ebp ; 1:04fe6
loc_04FE9:
    jmp loc_04EC0 ; 1:04fe9
loc_04FEE:
    mov ebx, dword ptr [esi] ; 1:04fee
loc_04FF0:
    mov ecx, dword ptr [esi + 4] ; 1:04ff0
loc_04FF3:
    mov edx, dword ptr [esi + 8] ; 1:04ff3
loc_04FF6:
    mov ebp, dword ptr [esi + 0xc] ; 1:04ff6
loc_04FF9:
    mov dword ptr [edi], ebx ; 1:04ff9
loc_04FFB:
    mov dword ptr [edi + 4], ecx ; 1:04ffb
loc_04FFE:
    mov dword ptr [edi + 8], edx ; 1:04ffe
loc_05001:
    mov dword ptr [edi + 0xc], ebp ; 1:05001
loc_05004:
    jmp loc_04F14 ; 1:05004
loc_05009:
    mov ebx, dword ptr [esi] ; 1:05009
loc_0500B:
    mov ecx, dword ptr [esi + 4] ; 1:0500b
loc_0500E:
    mov edx, dword ptr [esi + 8] ; 1:0500e
loc_05011:
    mov ebp, dword ptr [esi + 0xc] ; 1:05011
loc_05014:
    mov dword ptr [edi], ebx ; 1:05014
loc_05016:
    mov dword ptr [edi + 4], ecx ; 1:05016
loc_05019:
    mov dword ptr [edi + 8], edx ; 1:05019
loc_0501C:
    mov dword ptr [edi + 0xc], ebp ; 1:0501c
loc_0501F:
    jmp loc_04F68 ; 1:0501f
loc_05024:
    mov ebx, dword ptr [esi] ; 1:05024
loc_05026:
    mov ecx, dword ptr [esi + 4] ; 1:05026
loc_05029:
    mov edx, dword ptr [esi + 8] ; 1:05029
loc_0502C:
    mov ebp, dword ptr [esi + 0xc] ; 1:0502c
loc_0502F:
    mov dword ptr [edi], ebx ; 1:0502f
loc_05031:
    mov dword ptr [edi + 4], ecx ; 1:05031
loc_05034:
    mov dword ptr [edi + 8], edx ; 1:05034
loc_05037:
    mov dword ptr [edi + 0xc], ebp ; 1:05037
loc_0503A:
    jmp loc_04FBC ; 1:0503a
loc_0503C:
    add esp, 4 ; 1:0503c
loc_0503F:
    popad ; 1:0503f
loc_05040:
    ret ; 1:05040
Display_Bloc_Plein_:
    pushad ; 1:05041
loc_05042:
    mov ebx, dword ptr [esi] ; 1:05042
loc_05044:
    mov ecx, dword ptr [esi + 4] ; 1:05044
loc_05047:
    mov edx, dword ptr [esi + 8] ; 1:05047
loc_0504A:
    mov ebp, dword ptr [esi + 0xc] ; 1:0504a
loc_0504D:
    mov dword ptr [edi], ebx ; 1:0504d
loc_0504F:
    mov dword ptr [edi + 4], ecx ; 1:0504f
loc_05052:
    mov dword ptr [edi + 8], edx ; 1:05052
loc_05055:
    mov dword ptr [edi + 0xc], ebp ; 1:05055
loc_05058:
    mov ebx, dword ptr [esi + 0x10] ; 1:05058
loc_0505B:
    mov ecx, dword ptr [esi + 0x14] ; 1:0505b
loc_0505E:
    mov edx, dword ptr [esi + 0x18] ; 1:0505e
loc_05061:
    mov ebp, dword ptr [esi + 0x1c] ; 1:05061
loc_05064:
    mov dword ptr [edi + 0x140], ebx ; 1:05064
loc_0506A:
    mov dword ptr [edi + 0x144], ecx ; 1:0506a
loc_05070:
    mov dword ptr [edi + 0x148], edx ; 1:05070
loc_05076:
    mov dword ptr [edi + 0x14c], ebp ; 1:05076
loc_0507C:
    mov ebx, dword ptr [esi + 0x20] ; 1:0507c
loc_0507F:
    mov ecx, dword ptr [esi + 0x24] ; 1:0507f
loc_05082:
    mov edx, dword ptr [esi + 0x28] ; 1:05082
loc_05085:
    mov ebp, dword ptr [esi + 0x2c] ; 1:05085
loc_05088:
    mov dword ptr [edi + 0x280], ebx ; 1:05088
loc_0508E:
    mov dword ptr [edi + 0x284], ecx ; 1:0508e
loc_05094:
    mov dword ptr [edi + 0x288], edx ; 1:05094
loc_0509A:
    mov dword ptr [edi + 0x28c], ebp ; 1:0509a
loc_050A0:
    mov ebx, dword ptr [esi + 0x30] ; 1:050a0
loc_050A3:
    mov ecx, dword ptr [esi + 0x34] ; 1:050a3
loc_050A6:
    mov edx, dword ptr [esi + 0x38] ; 1:050a6
loc_050A9:
    mov ebp, dword ptr [esi + 0x3c] ; 1:050a9
loc_050AC:
    mov dword ptr [edi + 0x3c0], ebx ; 1:050ac
loc_050B2:
    mov dword ptr [edi + 0x3c4], ecx ; 1:050b2
loc_050B8:
    mov dword ptr [edi + 0x3c8], edx ; 1:050b8
loc_050BE:
    mov dword ptr [edi + 0x3cc], ebp ; 1:050be
loc_050C4:
    mov ebx, dword ptr [esi + 0x40] ; 1:050c4
loc_050C7:
    mov ecx, dword ptr [esi + 0x44] ; 1:050c7
loc_050CA:
    mov edx, dword ptr [esi + 0x48] ; 1:050ca
loc_050CD:
    mov ebp, dword ptr [esi + 0x4c] ; 1:050cd
loc_050D0:
    mov dword ptr [edi + 0x500], ebx ; 1:050d0
loc_050D6:
    mov dword ptr [edi + 0x504], ecx ; 1:050d6
loc_050DC:
    mov dword ptr [edi + 0x508], edx ; 1:050dc
loc_050E2:
    mov dword ptr [edi + 0x50c], ebp ; 1:050e2
loc_050E8:
    mov ebx, dword ptr [esi + 0x50] ; 1:050e8
loc_050EB:
    mov ecx, dword ptr [esi + 0x54] ; 1:050eb
loc_050EE:
    mov edx, dword ptr [esi + 0x58] ; 1:050ee
loc_050F1:
    mov ebp, dword ptr [esi + 0x5c] ; 1:050f1
loc_050F4:
    mov dword ptr [edi + 0x640], ebx ; 1:050f4
loc_050FA:
    mov dword ptr [edi + 0x644], ecx ; 1:050fa
loc_05100:
    mov dword ptr [edi + 0x648], edx ; 1:05100
loc_05106:
    mov dword ptr [edi + 0x64c], ebp ; 1:05106
loc_0510C:
    mov ebx, dword ptr [esi + 0x60] ; 1:0510c
loc_0510F:
    mov ecx, dword ptr [esi + 0x64] ; 1:0510f
loc_05112:
    mov edx, dword ptr [esi + 0x68] ; 1:05112
loc_05115:
    mov ebp, dword ptr [esi + 0x6c] ; 1:05115
loc_05118:
    mov dword ptr [edi + 0x780], ebx ; 1:05118
loc_0511E:
    mov dword ptr [edi + 0x784], ecx ; 1:0511e
loc_05124:
    mov dword ptr [edi + 0x788], edx ; 1:05124
loc_0512A:
    mov dword ptr [edi + 0x78c], ebp ; 1:0512a
loc_05130:
    mov ebx, dword ptr [esi + 0x70] ; 1:05130
loc_05133:
    mov ecx, dword ptr [esi + 0x74] ; 1:05133
loc_05136:
    mov edx, dword ptr [esi + 0x78] ; 1:05136
loc_05139:
    mov ebp, dword ptr [esi + 0x7c] ; 1:05139
loc_0513C:
    mov dword ptr [edi + 0x8c0], ebx ; 1:0513c
loc_05142:
    mov dword ptr [edi + 0x8c4], ecx ; 1:05142
loc_05148:
    mov dword ptr [edi + 0x8c8], edx ; 1:05148
loc_0514E:
    mov dword ptr [edi + 0x8cc], ebp ; 1:0514e
loc_05154:
    mov ebx, dword ptr [esi + 0x80] ; 1:05154
loc_0515A:
    mov ecx, dword ptr [esi + 0x84] ; 1:0515a
loc_05160:
    mov edx, dword ptr [esi + 0x88] ; 1:05160
loc_05166:
    mov ebp, dword ptr [esi + 0x8c] ; 1:05166
loc_0516C:
    mov dword ptr [edi + 0xa00], ebx ; 1:0516c
loc_05172:
    mov dword ptr [edi + 0xa04], ecx ; 1:05172
loc_05178:
    mov dword ptr [edi + 0xa08], edx ; 1:05178
loc_0517E:
    mov dword ptr [edi + 0xa0c], ebp ; 1:0517e
loc_05184:
    mov ebx, dword ptr [esi + 0x90] ; 1:05184
loc_0518A:
    mov ecx, dword ptr [esi + 0x94] ; 1:0518a
loc_05190:
    mov edx, dword ptr [esi + 0x98] ; 1:05190
loc_05196:
    mov ebp, dword ptr [esi + 0x9c] ; 1:05196
loc_0519C:
    mov dword ptr [edi + 0xb40], ebx ; 1:0519c
loc_051A2:
    mov dword ptr [edi + 0xb44], ecx ; 1:051a2
loc_051A8:
    mov dword ptr [edi + 0xb48], edx ; 1:051a8
loc_051AE:
    mov dword ptr [edi + 0xb4c], ebp ; 1:051ae
loc_051B4:
    mov ebx, dword ptr [esi + 0xa0] ; 1:051b4
loc_051BA:
    mov ecx, dword ptr [esi + 0xa4] ; 1:051ba
loc_051C0:
    mov edx, dword ptr [esi + 0xa8] ; 1:051c0
loc_051C6:
    mov ebp, dword ptr [esi + 0xac] ; 1:051c6
loc_051CC:
    mov dword ptr [edi + 0xc80], ebx ; 1:051cc
loc_051D2:
    mov dword ptr [edi + 0xc84], ecx ; 1:051d2
loc_051D8:
    mov dword ptr [edi + 0xc88], edx ; 1:051d8
loc_051DE:
    mov dword ptr [edi + 0xc8c], ebp ; 1:051de
loc_051E4:
    mov ebx, dword ptr [esi + 0xb0] ; 1:051e4
loc_051EA:
    mov ecx, dword ptr [esi + 0xb4] ; 1:051ea
loc_051F0:
    mov edx, dword ptr [esi + 0xb8] ; 1:051f0
loc_051F6:
    mov ebp, dword ptr [esi + 0xbc] ; 1:051f6
loc_051FC:
    mov dword ptr [edi + 0xdc0], ebx ; 1:051fc
loc_05202:
    mov dword ptr [edi + 0xdc4], ecx ; 1:05202
loc_05208:
    mov dword ptr [edi + 0xdc8], edx ; 1:05208
loc_0520E:
    mov dword ptr [edi + 0xdcc], ebp ; 1:0520e
loc_05214:
    mov ebx, dword ptr [esi + 0xc0] ; 1:05214
loc_0521A:
    mov ecx, dword ptr [esi + 0xc4] ; 1:0521a
loc_05220:
    mov edx, dword ptr [esi + 0xc8] ; 1:05220
loc_05226:
    mov ebp, dword ptr [esi + 0xcc] ; 1:05226
loc_0522C:
    mov dword ptr [edi + 0xf00], ebx ; 1:0522c
loc_05232:
    mov dword ptr [edi + 0xf04], ecx ; 1:05232
loc_05238:
    mov dword ptr [edi + 0xf08], edx ; 1:05238
loc_0523E:
    mov dword ptr [edi + 0xf0c], ebp ; 1:0523e
loc_05244:
    mov ebx, dword ptr [esi + 0xd0] ; 1:05244
loc_0524A:
    mov ecx, dword ptr [esi + 0xd4] ; 1:0524a
loc_05250:
    mov edx, dword ptr [esi + 0xd8] ; 1:05250
loc_05256:
    mov ebp, dword ptr [esi + 0xdc] ; 1:05256
loc_0525C:
    mov dword ptr [edi + 0x1040], ebx ; 1:0525c
loc_05262:
    mov dword ptr [edi + 0x1044], ecx ; 1:05262
loc_05268:
    mov dword ptr [edi + 0x1048], edx ; 1:05268
loc_0526E:
    mov dword ptr [edi + 0x104c], ebp ; 1:0526e
loc_05274:
    mov ebx, dword ptr [esi + 0xe0] ; 1:05274
loc_0527A:
    mov ecx, dword ptr [esi + 0xe4] ; 1:0527a
loc_05280:
    mov edx, dword ptr [esi + 0xe8] ; 1:05280
loc_05286:
    mov ebp, dword ptr [esi + 0xec] ; 1:05286
loc_0528C:
    mov dword ptr [edi + 0x1180], ebx ; 1:0528c
loc_05292:
    mov dword ptr [edi + 0x1184], ecx ; 1:05292
loc_05298:
    mov dword ptr [edi + 0x1188], edx ; 1:05298
loc_0529E:
    mov dword ptr [edi + 0x118c], ebp ; 1:0529e
loc_052A4:
    mov ebx, dword ptr [esi + 0xf0] ; 1:052a4
loc_052AA:
    mov ecx, dword ptr [esi + 0xf4] ; 1:052aa
loc_052B0:
    mov edx, dword ptr [esi + 0xf8] ; 1:052b0
loc_052B6:
    mov ebp, dword ptr [esi + 0xfc] ; 1:052b6
loc_052BC:
    mov dword ptr [edi + 0x12c0], ebx ; 1:052bc
loc_052C2:
    mov dword ptr [edi + 0x12c4], ecx ; 1:052c2
loc_052C8:
    mov dword ptr [edi + 0x12c8], edx ; 1:052c8
loc_052CE:
    mov dword ptr [edi + 0x12cc], ebp ; 1:052ce
loc_052D4:
    popad ; 1:052d4
loc_052D5:
    ret ; 1:052d5
; PROVEN embedded writable dword 1:052d6..052da; stored at 1:052de and read at 1:05383.
state_052d6:
dd 0
loc_052DA:
    xor edx, edx ; 1:052da
loc_052DC:
    mov dl, cl ; 1:052dc
loc_052DE:
    mov dword ptr [state_052d6], edx   ; fix@+2->1:52d6 ; 1:052de
loc_052E4:
    cmp eax, dword ptr [_Bloc_lim_W1]   ; fix@+2->2:3d09c ; 1:052e4
loc_052EA:
    jl loc_05329 ; 1:052ea
loc_052EC:
    add edx, eax ; 1:052ec
loc_052EE:
    cmp edx, dword ptr [_Bloc_lim_W2]   ; fix@+2->2:3d098 ; 1:052ee
loc_052F4:
    ja loc_05312 ; 1:052f4
loc_052F6:
    mov cl, 4 ; 1:052f6
loc_052F8:
    xor edx, edx ; 1:052f8
loc_052FA:
    mov dl, ch ; 1:052fa
loc_052FC:
    cmp ebx, dword ptr [_Bloc_lim_H1]   ; fix@+2->2:3d0a4 ; 1:052fc
loc_05302:
    jl loc_05362 ; 1:05302
loc_05304:
    add edx, ebx ; 1:05304
loc_05306:
    cmp edx, dword ptr [_Bloc_lim_H2]   ; fix@+2->2:3d0a0 ; 1:05306
loc_0530C:
    ja loc_0534E ; 1:0530c
loc_0530E:
    clc ; 1:0530e
loc_0530F:
    ret ; 1:0530f
loc_05310:
    stc ; 1:05310
loc_05311:
    ret ; 1:05311
loc_05312:
    cmp eax, dword ptr [_Bloc_lim_W2]   ; fix@+2->2:3d098 ; 1:05312
loc_05318:
    jae loc_05310 ; 1:05318
loc_0531A:
    mov edx, dword ptr [_Bloc_lim_W2]   ; fix@+2->2:3d098 ; 1:0531a
loc_05320:
    sub edx, eax ; 1:05320
loc_05322:
    mov cl, dl ; 1:05322
loc_05324:
    jmp loc_053AF ; 1:05324
loc_05329:
    sub eax, dword ptr [_Bloc_lim_W1]   ; fix@+2->2:3d09c ; 1:05329
loc_0532F:
    neg eax ; 1:0532f
loc_05331:
    cmp edx, eax ; 1:05331
loc_05333:
    jle loc_05310 ; 1:05333
loc_05335:
    and eax, 0xfffc ; 1:05335
loc_0533A:
    sub edx, eax ; 1:0533a
loc_0533C:
    mov cl, dl ; 1:0533c
loc_0533E:
    lea esi, [esi + eax] ; 1:0533e
loc_05341:
    lea edi, [edi + eax] ; 1:05341
loc_05344:
    mov eax, dword ptr [_Bloc_lim_W1]   ; fix@+1->2:3d09c ; 1:05344
loc_05349:
    shr cl, 2 ; 1:05349
loc_0534C:
    jmp loc_052F8 ; 1:0534c
loc_0534E:
    cmp ebx, dword ptr [_Bloc_lim_H2]   ; fix@+2->2:3d0a0 ; 1:0534e
loc_05354:
    jae loc_05310 ; 1:05354
loc_05356:
    mov edx, dword ptr [_Bloc_lim_H2]   ; fix@+2->2:3d0a0 ; 1:05356
loc_0535C:
    sub edx, ebx ; 1:0535c
loc_0535E:
    mov ch, dl ; 1:0535e
loc_05360:
    jmp loc_0530E ; 1:05360
loc_05362:
    add edx, ebx ; 1:05362
loc_05364:
    cmp edx, dword ptr [_Bloc_lim_H1]   ; fix@+2->2:3d0a4 ; 1:05364
loc_0536A:
    jle loc_05310 ; 1:0536a
loc_0536C:
    xor edx, edx ; 1:0536c
loc_0536E:
    sub ebx, dword ptr [_Bloc_lim_H1]   ; fix@+2->2:3d0a4 ; 1:0536e
loc_05374:
    mov dl, ch ; 1:05374
loc_05376:
    neg ebx ; 1:05376
loc_05378:
    and ebx, 0xffff ; 1:05378
loc_0537E:
    sub edx, ebx ; 1:0537e
loc_05380:
    mov ch, dl ; 1:05380
loc_05382:
    push eax ; 1:05382
loc_05383:
    mov eax, dword ptr [state_052d6]   ; fix@+1->1:52d6 ; 1:05383
loc_05388:
    mul bx ; 1:05388
loc_0538B:
    add esi, eax ; 1:0538b
loc_0538D:
    mov eax, dword ptr [state_stride_04c86]   ; fix@+1->1:4c86 ; 1:0538d
loc_05392:
    mul bx ; 1:05392
loc_05395:
    add edi, eax ; 1:05395
loc_05397:
    shl dword ptr [esp + 8], 2 ; 1:05397
loc_0539C:
    dec ebx ; 1:0539c
loc_0539D:
    jne loc_05397 ; 1:0539d
loc_0539F:
    pop eax ; 1:0539f
loc_053A0:
    mov ebx, dword ptr [_Bloc_lim_H1]   ; fix@+2->2:3d0a4 ; 1:053a0
loc_053A6:
    xor edx, edx ; 1:053a6
loc_053A8:
    mov dl, ch ; 1:053a8
loc_053AA:
    jmp loc_05304 ; 1:053aa
loc_053AF:
    xor edx, edx ; 1:053af
loc_053B1:
    mov dl, cl ; 1:053b1
loc_053B3:
    shr cl, 2 ; 1:053b3
loc_053B6:
    and edx, 3 ; 1:053b6
loc_053B9:
    je loc_052F8 ; 1:053b9
loc_053BF:
    inc cl ; 1:053bf
loc_053C1:
    jmp loc_052F8 ; 1:053c1
Display_Bloc_Clippe_:
    pushad ; 1:053c6
loc_053C7:
    mov ecx, 0x1010 ; 1:053c7
loc_053CC:
    sub esp, 5 ; 1:053cc
loc_053CF:
    mov ebp, dword ptr [esi + 0x200] ; 1:053cf
loc_053D5:
    mov dword ptr [esp], ebp ; 1:053d5
loc_053D8:
    call loc_052DA ; 1:053d8
loc_053DD:
    jb loc_05559 ; 1:053dd
loc_053E3:
    mov eax, dword ptr [esp] ; 1:053e3
loc_053E6:
    mov byte ptr [esp + 4], ch ; 1:053e6
loc_053EA:
    cmp cl, 3 ; 1:053ea
loc_053ED:
    je loc_0547B ; 1:053ed
loc_053F3:
    cmp cl, 2 ; 1:053f3
loc_053F6:
    je loc_054DC ; 1:053f6
loc_053FC:
    cmp cl, 1 ; 1:053fc
loc_053FF:
    je loc_05525 ; 1:053ff
loc_05405:
    test eax, 0x40000000 ; 1:05405
loc_0540A:
    jne loc_05429 ; 1:0540a
loc_0540C:
    test eax, 0x80000000 ; 1:0540c
loc_05411:
    je loc_05440 ; 1:05411
loc_05413:
    mov ebx, dword ptr [esi] ; 1:05413
loc_05415:
    mov ecx, dword ptr [esi + 4] ; 1:05415
loc_05418:
    mov edx, dword ptr [esi + 8] ; 1:05418
loc_0541B:
    mov ebp, dword ptr [esi + 0xc] ; 1:0541b
loc_0541E:
    mov dword ptr [edi], ebx ; 1:0541e
loc_05420:
    mov dword ptr [edi + 4], ecx ; 1:05420
loc_05423:
    mov dword ptr [edi + 8], edx ; 1:05423
loc_05426:
    mov dword ptr [edi + 0xc], ebp ; 1:05426
loc_05429:
    add edi, dword ptr [state_stride_04c86]   ; fix@+2->1:4c86 ; 1:05429
loc_0542F:
    add esi, 0x10 ; 1:0542f
loc_05432:
    shl eax, 2 ; 1:05432
loc_05435:
    dec byte ptr [esp + 4] ; 1:05435
loc_05439:
    jne loc_05405 ; 1:05439
loc_0543B:
    jmp loc_05559 ; 1:0543b
loc_05440:
    mov ecx, dword ptr [esi + 0x100] ; 1:05440
loc_05446:
    mov ebp, dword ptr [esi + 0x104] ; 1:05446
loc_0544C:
    mov ebx, dword ptr [esi] ; 1:0544c
loc_0544E:
    mov edx, dword ptr [esi + 4] ; 1:0544e
loc_05451:
    and dword ptr [edi], ecx ; 1:05451
loc_05453:
    and dword ptr [edi + 4], ebp ; 1:05453
loc_05456:
    or dword ptr [edi], ebx ; 1:05456
loc_05458:
    or dword ptr [edi + 4], edx ; 1:05458
loc_0545B:
    mov ecx, dword ptr [esi + 0x108] ; 1:0545b
loc_05461:
    mov ebp, dword ptr [esi + 0x10c] ; 1:05461
loc_05467:
    mov ebx, dword ptr [esi + 8] ; 1:05467
loc_0546A:
    mov edx, dword ptr [esi + 0xc] ; 1:0546a
loc_0546D:
    and dword ptr [edi + 8], ecx ; 1:0546d
loc_05470:
    and dword ptr [edi + 0xc], ebp ; 1:05470
loc_05473:
    or dword ptr [edi + 8], ebx ; 1:05473
loc_05476:
    or dword ptr [edi + 0xc], edx ; 1:05476
loc_05479:
    jmp loc_05429 ; 1:05479
loc_0547B:
    test eax, 0x40000000 ; 1:0547b
loc_05480:
    jne loc_05499 ; 1:05480
loc_05482:
    test eax, 0x80000000 ; 1:05482
loc_05487:
    je loc_054B0 ; 1:05487
loc_05489:
    mov ebx, dword ptr [esi] ; 1:05489
loc_0548B:
    mov ecx, dword ptr [esi + 4] ; 1:0548b
loc_0548E:
    mov edx, dword ptr [esi + 8] ; 1:0548e
loc_05491:
    mov dword ptr [edi], ebx ; 1:05491
loc_05493:
    mov dword ptr [edi + 4], ecx ; 1:05493
loc_05496:
    mov dword ptr [edi + 8], edx ; 1:05496
loc_05499:
    add edi, dword ptr [state_stride_04c86]   ; fix@+2->1:4c86 ; 1:05499
loc_0549F:
    add esi, 0x10 ; 1:0549f
loc_054A2:
    shl eax, 2 ; 1:054a2
loc_054A5:
    dec byte ptr [esp + 4] ; 1:054a5
loc_054A9:
    jne loc_0547B ; 1:054a9
loc_054AB:
    jmp loc_05559 ; 1:054ab
loc_054B0:
    mov ecx, dword ptr [esi + 0x100] ; 1:054b0
loc_054B6:
    mov ebp, dword ptr [esi + 0x104] ; 1:054b6
loc_054BC:
    mov ebx, dword ptr [esi] ; 1:054bc
loc_054BE:
    mov edx, dword ptr [esi + 4] ; 1:054be
loc_054C1:
    and dword ptr [edi], ecx ; 1:054c1
loc_054C3:
    and dword ptr [edi + 4], ebp ; 1:054c3
loc_054C6:
    or dword ptr [edi], ebx ; 1:054c6
loc_054C8:
    or dword ptr [edi + 4], edx ; 1:054c8
loc_054CB:
    mov ecx, dword ptr [esi + 0x108] ; 1:054cb
loc_054D1:
    mov ebx, dword ptr [esi + 8] ; 1:054d1
loc_054D4:
    and dword ptr [edi + 8], ecx ; 1:054d4
loc_054D7:
    or dword ptr [edi + 8], ebx ; 1:054d7
loc_054DA:
    jmp loc_05499 ; 1:054da
loc_054DC:
    test eax, 0x40000000 ; 1:054dc
loc_054E1:
    jne loc_054F4 ; 1:054e1
loc_054E3:
    test eax, 0x80000000 ; 1:054e3
loc_054E8:
    je loc_05508 ; 1:054e8
loc_054EA:
    mov ebx, dword ptr [esi] ; 1:054ea
loc_054EC:
    mov ecx, dword ptr [esi + 4] ; 1:054ec
loc_054EF:
    mov dword ptr [edi], ebx ; 1:054ef
loc_054F1:
    mov dword ptr [edi + 4], ecx ; 1:054f1
loc_054F4:
    add edi, dword ptr [state_stride_04c86]   ; fix@+2->1:4c86 ; 1:054f4
loc_054FA:
    add esi, 0x10 ; 1:054fa
loc_054FD:
    shl eax, 2 ; 1:054fd
loc_05500:
    dec byte ptr [esp + 4] ; 1:05500
loc_05504:
    jne loc_054DC ; 1:05504
loc_05506:
    jmp loc_05559 ; 1:05506
loc_05508:
    mov ecx, dword ptr [esi + 0x100] ; 1:05508
loc_0550E:
    mov ebp, dword ptr [esi + 0x104] ; 1:0550e
loc_05514:
    mov ebx, dword ptr [esi] ; 1:05514
loc_05516:
    mov edx, dword ptr [esi + 4] ; 1:05516
loc_05519:
    and dword ptr [edi], ecx ; 1:05519
loc_0551B:
    and dword ptr [edi + 4], ebp ; 1:0551b
loc_0551E:
    or dword ptr [edi], ebx ; 1:0551e
loc_05520:
    or dword ptr [edi + 4], edx ; 1:05520
loc_05523:
    jmp loc_054F4 ; 1:05523
loc_05525:
    test eax, 0x40000000 ; 1:05525
loc_0552A:
    jne loc_05537 ; 1:0552a
loc_0552C:
    test eax, 0x80000000 ; 1:0552c
loc_05531:
    je loc_0554B ; 1:05531
loc_05533:
    mov ebx, dword ptr [esi] ; 1:05533
loc_05535:
    mov dword ptr [edi], ebx ; 1:05535
loc_05537:
    add edi, dword ptr [state_stride_04c86]   ; fix@+2->1:4c86 ; 1:05537
loc_0553D:
    add esi, 0x10 ; 1:0553d
loc_05540:
    shl eax, 2 ; 1:05540
loc_05543:
    dec byte ptr [esp + 4] ; 1:05543
loc_05547:
    jne loc_05525 ; 1:05547
loc_05549:
    jmp loc_05559 ; 1:05549
loc_0554B:
    mov ecx, dword ptr [esi + 0x100] ; 1:0554b
loc_05551:
    mov ebx, dword ptr [esi] ; 1:05551
loc_05553:
    and dword ptr [edi], ecx ; 1:05553
loc_05555:
    or dword ptr [edi], ebx ; 1:05555
loc_05557:
    jmp loc_05537 ; 1:05557
loc_05559:
    add esp, 5 ; 1:05559
loc_0555C:
    popad ; 1:0555c
loc_0555D:
    ret ; 1:0555d
Display_Bloc_Plein_Clippe_:
    pushad ; 1:0555e
loc_0555F:
    mov ecx, 0x1010 ; 1:0555f
loc_05564:
    sub esp, 5 ; 1:05564
loc_05567:
    mov dword ptr [esp], 0 ; 1:05567
loc_0556E:
    call loc_052DA ; 1:0556e
loc_05573:
    jb loc_05604 ; 1:05573
loc_05579:
    mov byte ptr [esp + 4], ch ; 1:05579
loc_0557D:
    cmp cl, 3 ; 1:0557d
loc_05580:
    je loc_055B3 ; 1:05580
loc_05582:
    cmp cl, 2 ; 1:05582
loc_05585:
    je loc_055D4 ; 1:05585
loc_05587:
    cmp cl, 1 ; 1:05587
loc_0558A:
    je loc_055EF ; 1:0558a
loc_0558C:
    mov ebx, dword ptr [esi] ; 1:0558c
loc_0558E:
    mov ecx, dword ptr [esi + 4] ; 1:0558e
loc_05591:
    mov edx, dword ptr [esi + 8] ; 1:05591
loc_05594:
    mov ebp, dword ptr [esi + 0xc] ; 1:05594
loc_05597:
    mov dword ptr [edi], ebx ; 1:05597
loc_05599:
    mov dword ptr [edi + 4], ecx ; 1:05599
loc_0559C:
    mov dword ptr [edi + 8], edx ; 1:0559c
loc_0559F:
    mov dword ptr [edi + 0xc], ebp ; 1:0559f
loc_055A2:
    add edi, dword ptr [state_stride_04c86]   ; fix@+2->1:4c86 ; 1:055a2
loc_055A8:
    add esi, 0x10 ; 1:055a8
loc_055AB:
    dec byte ptr [esp + 4] ; 1:055ab
loc_055AF:
    jne loc_0558C ; 1:055af
loc_055B1:
    jmp loc_05604 ; 1:055b1
loc_055B3:
    mov ebx, dword ptr [esi] ; 1:055b3
loc_055B5:
    mov ecx, dword ptr [esi + 4] ; 1:055b5
loc_055B8:
    mov edx, dword ptr [esi + 8] ; 1:055b8
loc_055BB:
    mov dword ptr [edi], ebx ; 1:055bb
loc_055BD:
    mov dword ptr [edi + 4], ecx ; 1:055bd
loc_055C0:
    mov dword ptr [edi + 8], edx ; 1:055c0
loc_055C3:
    add edi, dword ptr [state_stride_04c86]   ; fix@+2->1:4c86 ; 1:055c3
loc_055C9:
    add esi, 0x10 ; 1:055c9
loc_055CC:
    dec byte ptr [esp + 4] ; 1:055cc
loc_055D0:
    jne loc_055B3 ; 1:055d0
loc_055D2:
    jmp loc_05604 ; 1:055d2
loc_055D4:
    mov ebx, dword ptr [esi] ; 1:055d4
loc_055D6:
    mov ecx, dword ptr [esi + 4] ; 1:055d6
loc_055D9:
    mov dword ptr [edi], ebx ; 1:055d9
loc_055DB:
    mov dword ptr [edi + 4], ecx ; 1:055db
loc_055DE:
    add edi, dword ptr [state_stride_04c86]   ; fix@+2->1:4c86 ; 1:055de
loc_055E4:
    add esi, 0x10 ; 1:055e4
loc_055E7:
    dec byte ptr [esp + 4] ; 1:055e7
loc_055EB:
    jne loc_055D4 ; 1:055eb
loc_055ED:
    jmp loc_05604 ; 1:055ed
loc_055EF:
    mov ebx, dword ptr [esi] ; 1:055ef
loc_055F1:
    mov dword ptr [edi], ebx ; 1:055f1
loc_055F3:
    add edi, dword ptr [state_stride_04c86]   ; fix@+2->1:4c86 ; 1:055f3
loc_055F9:
    add esi, 0x10 ; 1:055f9
loc_055FC:
    dec byte ptr [esp + 4] ; 1:055fc
loc_05600:
    jne loc_055EF ; 1:05600
loc_05602:
    jmp loc_05604 ; 1:05602
loc_05604:
    add esp, 5 ; 1:05604
loc_05607:
    popad ; 1:05607
loc_05608:
    ret ; 1:05608
_TEXT ends

; ---- Accepted source: lowlevel_05609.asm ----
; DOS v1.21 object 1 slice 1:05609..1:0611e (end exclusive).
; Readable reconstruction from address-anchored CFG; labels preserve original offsets.
.386p
extrn row_offsets_320_01b00:DWORD
extrn _During_The_Menu:WORD
extrn _joyy0:WORD
extrn _joyx0:WORD
extrn _ypadcentre:WORD
extrn _xpadcentre:WORD
extrn _joy_buttonB1:WORD
extrn _joy_buttonB2:WORD
extrn _joy_buttonA1:WORD
extrn _joy_buttonA2:WORD
_TEXT segment para public use32 'CODE'
public fplot_Normal_
public flocon_clipping_
public draw_flocon5_Normal_
public draw_flocon1_Normal_
public draw_flocon2_Normal_
public draw_flocon3_Normal_
public draw_flocon4_Normal_
public draw_normal_5755_
public draw_pluie5_Normal_
public draw_flocon7_Normal_
public draw_pluie6_Normal_
public draw_flocon6_Normal_
public draw_pluie7_Normal_
public get_joy_input2_dos1_
public get_joy_input2_dos2_
public get_joy_input2_windows1_
public get_joy_input2_windows2_
public dos_game_io_port_something_
public dos_game_io_port_something2_
public get_joy_input1_dos1_
public get_joy_input1_dos2_

fplot_Normal_: ; DOS 1:05609
    pushad ; 1:05609
    add eax, dword ptr [ebx*4 + row_offsets_320_01b00] ; 1:0560a
    mov edx, esi ; 1:05611
    mov byte ptr [edi + eax], dl ; 1:05613
    popad ; 1:05616
    ret ; 1:05617
clip_top:
    dd 0 ; PROVEN state: written by flocon_clipping at 1:05628; read at 1:05652 and later.
clip_bottom:
    dd 0 ; PROVEN state: written by flocon_clipping at 1:0562d; read at 1:0565a and later.
clip_left:
    dd 0 ; PROVEN state: written by flocon_clipping at 1:05633; read at 1:05642 and later.
clip_right:
    dd 0 ; PROVEN state: written by flocon_clipping at 1:05639; read at 1:0564a and later.
flocon_clipping_: ; DOS 1:05628
    mov dword ptr [clip_top], eax ; 1:05628
    mov dword ptr [clip_bottom], ebx ; 1:0562d
    mov dword ptr [clip_left], ecx ; 1:05633
    mov dword ptr [clip_right], edx ; 1:05639
    ret ; 1:0563f
draw_flocon5_Normal_: ; DOS 1:05640
    push eax ; 1:05640
    push edi ; 1:05641
    cmp eax, dword ptr [clip_left] ; 1:05642
    jl L_0566D ; 1:05648
    cmp eax, dword ptr [clip_right] ; 1:0564a
    ja L_0566D ; 1:05650
    cmp ebx, dword ptr [clip_top] ; 1:05652
    jl L_0566D ; 1:05658
    cmp ebx, dword ptr [clip_bottom] ; 1:0565a
    ja L_0566D ; 1:05660
    add eax, dword ptr [ebx*4 + row_offsets_320_01b00] ; 1:05662
    mov byte ptr [edi + eax], 5 ; 1:05669
L_0566D:
    pop edi ; 1:0566d
    pop eax ; 1:0566e
    ret ; 1:0566f
draw_flocon1_Normal_: ; DOS 1:05670
    push eax ; 1:05670
    push edi ; 1:05671
    cmp eax, dword ptr [clip_left] ; 1:05672
    jl L_0569D ; 1:05678
    cmp eax, dword ptr [clip_right] ; 1:0567a
    ja L_0569D ; 1:05680
    cmp ebx, dword ptr [clip_top] ; 1:05682
    jl L_0569D ; 1:05688
    cmp ebx, dword ptr [clip_bottom] ; 1:0568a
    ja L_0569D ; 1:05690
    add eax, dword ptr [ebx*4 + row_offsets_320_01b00] ; 1:05692
    mov byte ptr [edi + eax], 7 ; 1:05699
L_0569D:
    pop edi ; 1:0569d
    pop eax ; 1:0569e
    ret ; 1:0569f
draw_flocon2_Normal_: ; DOS 1:056a0
    push edi ; 1:056a0
    push eax ; 1:056a1
    cmp eax, dword ptr [clip_left] ; 1:056a2
    jl L_056D9 ; 1:056a8
    cmp eax, dword ptr [clip_right] ; 1:056aa
    ja L_056D9 ; 1:056b0
    cmp ebx, dword ptr [clip_top] ; 1:056b2
    jl L_056D9 ; 1:056b8
    cmp ebx, dword ptr [clip_bottom] ; 1:056ba
    ja L_056D9 ; 1:056c0
    add eax, dword ptr [ebx*4 + row_offsets_320_01b00] ; 1:056c2
    add edi, eax ; 1:056c9
    mov word ptr [edi], 807h ; 1:056cb
    mov word ptr [edi + 140h], 706h ; 1:056d0
L_056D9:
    pop eax ; 1:056d9
    pop edi ; 1:056da
    ret ; 1:056db
draw_flocon3_Normal_: ; DOS 1:056dc
    push edi ; 1:056dc
    push eax ; 1:056dd
    cmp eax, dword ptr [clip_left] ; 1:056de
    jl L_05722 ; 1:056e4
    cmp eax, dword ptr [clip_right] ; 1:056e6
    ja L_05722 ; 1:056ec
    cmp ebx, dword ptr [clip_top] ; 1:056ee
    jl L_05722 ; 1:056f4
    cmp ebx, dword ptr [clip_bottom] ; 1:056f6
    ja L_05722 ; 1:056fc
    add eax, dword ptr [ebx*4 + row_offsets_320_01b00] ; 1:056fe
    add edi, eax ; 1:05705
    mov word ptr [edi + 140h], 608h ; 1:05707
    mov byte ptr [edi + 142h], 6 ; 1:05710
    mov byte ptr [edi + 1], 6 ; 1:05717
    mov byte ptr [edi + 281h], 6 ; 1:0571b
L_05722:
    pop eax ; 1:05722
    pop edi ; 1:05723
    ret ; 1:05724
draw_flocon4_Normal_: ; DOS 1:05725
    push edi ; 1:05725
    push eax ; 1:05726
    cmp eax, dword ptr [clip_left] ; 1:05727
    jl L_05752 ; 1:0572d
    cmp eax, dword ptr [clip_right] ; 1:0572f
    ja L_05752 ; 1:05735
    cmp ebx, dword ptr [clip_top] ; 1:05737
    jl L_05752 ; 1:0573d
    cmp ebx, dword ptr [clip_bottom] ; 1:0573f
    ja L_05752 ; 1:05745
    add eax, dword ptr [ebx*4 + row_offsets_320_01b00] ; 1:05747
    mov byte ptr [edi + eax], 3 ; 1:0574e
L_05752:
    pop eax ; 1:05752
    pop edi ; 1:05753
    ret ; 1:05754
draw_normal_5755_: ; DOS 1:05755
    push edi ; 1:05755
    push eax ; 1:05756
    cmp eax, dword ptr [clip_left] ; 1:05757
    jl L_057A3 ; 1:0575d
    cmp eax, dword ptr [clip_right] ; 1:0575f
    ja L_057A3 ; 1:05765
    cmp ebx, dword ptr [clip_top] ; 1:05767
    jl L_057A3 ; 1:0576d
    cmp ebx, dword ptr [clip_bottom] ; 1:0576f
    ja L_057A3 ; 1:05775
    add eax, dword ptr [ebx*4 + row_offsets_320_01b00] ; 1:05777
    add edi, eax ; 1:0577e
    mov word ptr [edi + 1], 505h ; 1:05780
    mov dword ptr [edi + 140h], 5070705h ; 1:05786
    mov dword ptr [edi + 280h], 5070705h ; 1:05790
    mov word ptr [edi + 3c1h], 505h ; 1:0579a
L_057A3:
    pop eax ; 1:057a3
    pop edi ; 1:057a4
    ret ; 1:057a5
draw_pluie5_Normal_: ; DOS 1:057a6
    push edi ; 1:057a6
    push eax ; 1:057a7
    cmp eax, dword ptr [clip_left] ; 1:057a8
    jl L_057DC ; 1:057ae
    cmp eax, dword ptr [clip_right] ; 1:057b0
    ja L_057DC ; 1:057b6
    cmp ebx, dword ptr [clip_top] ; 1:057b8
    jl L_057DC ; 1:057be
    cmp ebx, dword ptr [clip_bottom] ; 1:057c0
    ja L_057DC ; 1:057c6
    add eax, dword ptr [ebx*4 + row_offsets_320_01b00] ; 1:057c8
    add edi, eax ; 1:057cf
    mov byte ptr [edi + 140h], 5 ; 1:057d1
    mov byte ptr [edi + 1], 4 ; 1:057d8
L_057DC:
    pop eax ; 1:057dc
    pop edi ; 1:057dd
    ret ; 1:057de
draw_flocon7_Normal_: ; DOS 1:057df
    push edi ; 1:057df
    push eax ; 1:057e0
    cmp eax, dword ptr [clip_left] ; 1:057e1
    jl L_0582D ; 1:057e7
    cmp eax, dword ptr [clip_right] ; 1:057e9
    ja L_0582D ; 1:057ef
    cmp ebx, dword ptr [clip_top] ; 1:057f1
    jl L_0582D ; 1:057f7
    cmp ebx, dword ptr [clip_bottom] ; 1:057f9
    ja L_0582D ; 1:057ff
    add eax, dword ptr [ebx*4 + row_offsets_320_01b00] ; 1:05801
    add edi, eax ; 1:05808
    mov word ptr [edi + 1], 606h ; 1:0580a
    mov dword ptr [edi + 140h], 6080806h ; 1:05810
    mov dword ptr [edi + 280h], 6080806h ; 1:0581a
    mov word ptr [edi + 3c1h], 606h ; 1:05824
L_0582D:
    pop eax ; 1:0582d
    pop edi ; 1:0582e
    ret ; 1:0582f
draw_pluie6_Normal_: ; DOS 1:05830
    push edi ; 1:05830
    push eax ; 1:05831
    cmp eax, dword ptr [clip_left] ; 1:05832
    jl L_0587A ; 1:05838
    cmp eax, dword ptr [clip_right] ; 1:0583a
    ja L_0587A ; 1:05840
    cmp ebx, dword ptr [clip_top] ; 1:05842
    jl L_0587A ; 1:05848
    cmp ebx, dword ptr [clip_bottom] ; 1:0584a
    ja L_0587A ; 1:05850
    add eax, dword ptr [ebx*4 + row_offsets_320_01b00] ; 1:05852
    add edi, eax ; 1:05859
    mov word ptr [edi + 1], 204h ; 1:0585b
    mov word ptr [edi + 140h], 305h ; 1:05861
    mov byte ptr [edi + 142h], 2 ; 1:0586a
    mov word ptr [edi + 280h], 603h ; 1:05871
L_0587A:
    pop eax ; 1:0587a
    pop edi ; 1:0587b
    ret ; 1:0587c
draw_flocon6_Normal_: ; DOS 1:0587d
    push edi ; 1:0587d
    push eax ; 1:0587e
    cmp eax, dword ptr [clip_left] ; 1:0587f
    jl L_058F1 ; 1:05885
    cmp eax, dword ptr [clip_right] ; 1:05887
    ja L_058F1 ; 1:0588d
    cmp ebx, dword ptr [clip_top] ; 1:0588f
    jl L_058F1 ; 1:05895
    cmp ebx, dword ptr [clip_bottom] ; 1:05897
    ja L_058F1 ; 1:0589d
    add eax, dword ptr [ebx*4 + row_offsets_320_01b00] ; 1:0589f
    add edi, eax ; 1:058a6
    mov word ptr [edi + 2], 606h ; 1:058a8
    mov dword ptr [edi + 141h], 7080807h ; 1:058ae
    mov dword ptr [edi + 280h], 8080806h ; 1:058b8
    mov dword ptr [edi + 3c0h], 8080806h ; 1:058c2
    mov dword ptr [edi + 501h], 7080807h ; 1:058cc
    mov word ptr [edi + 642h], 606h ; 1:058d6
    mov word ptr [edi + 284h], 608h ; 1:058df
    mov word ptr [edi + 3c4h], 608h ; 1:058e8
L_058F1:
    pop eax ; 1:058f1
    pop edi ; 1:058f2
    ret ; 1:058f3
draw_pluie7_Normal_: ; DOS 1:058f4
    push edi ; 1:058f4
    push eax ; 1:058f5
    cmp eax, dword ptr [clip_left] ; 1:058f6
    jl L_0594E ; 1:058fc
    cmp eax, dword ptr [clip_right] ; 1:058fe
    ja L_0594E ; 1:05904
    cmp ebx, dword ptr [clip_top] ; 1:05906
    jl L_0594E ; 1:0590c
    cmp ebx, dword ptr [clip_bottom] ; 1:0590e
    ja L_0594E ; 1:05914
    add eax, dword ptr [ebx*4 + row_offsets_320_01b00] ; 1:05916
    add edi, eax ; 1:0591d
    mov word ptr [edi + 2], 203h ; 1:0591f
    mov byte ptr [edi + 141h], 3 ; 1:05925
    mov word ptr [edi + 142h], 402h ; 1:0592c
    mov word ptr [edi + 280h], 405h ; 1:05935
    mov byte ptr [edi + 282h], 3 ; 1:0593e
    mov word ptr [edi + 3c0h], 604h ; 1:05945
L_0594E:
    pop eax ; 1:0594e
    pop edi ; 1:0594f
    ret ; 1:05950
joy_source_stride:
    dd 320 ; PROVEN stride: dword reads at 1:05e2f/1:06019, table contents verified in data_tables.json.
joy_dest_stride:
    dd 320 ; PROVEN stride: dword reads at 1:05e35/1:0601f, table contents verified in data_tables.json.
joy_timer_start:
    dd 0 ; PROVEN state: dword write at 1:059e3; read at 1:05a36 and later.
get_joy_input2_dos1_: ; DOS 1:0595d
    pushad ; 1:0595d
    cli ; 1:0595e
    cld ; 1:0595f
    xor eax, eax ; 1:05960
    mov dx, 201h ; 1:05962
    in al, dx ; 1:05966
    mov ebx, eax ; 1:05967
    shr al, 5 ; 1:05969
    mov word ptr [_joy_buttonA1], 1 ; 1:0596c
    sbb word ptr [_joy_buttonA1], 0 ; 1:05975
    shr al, 1 ; 1:0597d
    mov word ptr [_joy_buttonA2], 1 ; 1:0597f
    sbb word ptr [_joy_buttonA2], 0 ; 1:05988
    shr al, 1 ; 1:05990
    mov word ptr [_joy_buttonB1], 1 ; 1:05992
    sbb word ptr [_joy_buttonB1], 0 ; 1:0599b
    shr al, 1 ; 1:059a3
    mov word ptr [_joy_buttonB2], 1 ; 1:059a5
    sbb word ptr [_joy_buttonB2], 0 ; 1:059ae
    mov ax, word ptr [_During_The_Menu] ; 1:059b6
    cmp ax, 1 ; 1:059bc
    je L_05A2E ; 1:059c0
    and ebx, 3 ; 1:059c2
    jne L_05A12 ; 1:059c5
    mov word ptr [_joyx0], 0 ; 1:059c7
    mov word ptr [_joyy0], 0 ; 1:059d0
    mov ebx, 7d0h ; 1:059d9
    call clock_ticks_ ; 1:059de
    mov dword ptr [joy_timer_start], eax ; 1:059e3
    mov al, 0ffh ; 1:059e8
    out dx, al ; 1:059ea
L_059EB:
    in al, dx ; 1:059eb
    mov ebp, eax ; 1:059ec
    and eax, 1 ; 1:059ee
    add ax, word ptr [_joyx0] ; 1:059f1
    je L_05A31 ; 1:059f8
L_059FA:
    mov eax, ebp ; 1:059fa
    and eax, 2 ; 1:059fc
    add ax, word ptr [_joyy0] ; 1:059ff
    je L_05A44 ; 1:05a06
L_05A08:
    dec ebx ; 1:05a08
    je L_05A12 ; 1:05a09
    and ebp, 3 ; 1:05a0b
    jne L_059EB ; 1:05a0e
    jmp L_05A2E ; 1:05a10
L_05A12:
    mov bx, word ptr [_xpadcentre] ; 1:05a12
    mov word ptr [_joyx0], bx ; 1:05a19
    mov bx, word ptr [_ypadcentre] ; 1:05a20
    mov word ptr [_joyy0], bx ; 1:05a27
L_05A2E:
    sti ; 1:05a2e
    popad ; 1:05a2f
    ret ; 1:05a30
L_05A31:
    call clock_ticks_ ; 1:05a31
    sub eax, dword ptr [joy_timer_start] ; 1:05a36
    mov word ptr [_joyx0], ax ; 1:05a3c
    jmp L_059FA ; 1:05a42
L_05A44:
    call clock_ticks_ ; 1:05a44
    sub eax, dword ptr [joy_timer_start] ; 1:05a49
    mov word ptr [_joyy0], ax ; 1:05a4f
    jmp L_05A08 ; 1:05a55
get_joy_input2_dos2_: ; DOS 1:05a57
    pushad ; 1:05a57
    cli ; 1:05a58
    cld ; 1:05a59
    xor eax, eax ; 1:05a5a
    mov dx, 201h ; 1:05a5c
    in al, dx ; 1:05a60
    mov ebx, eax ; 1:05a61
    shr al, 5 ; 1:05a63
    mov word ptr [_joy_buttonB1], 1 ; 1:05a66
    sbb word ptr [_joy_buttonB1], 0 ; 1:05a6f
    shr al, 1 ; 1:05a77
    mov word ptr [_joy_buttonB2], 1 ; 1:05a79
    sbb word ptr [_joy_buttonB2], 0 ; 1:05a82
    shr al, 1 ; 1:05a8a
    mov word ptr [_joy_buttonA1], 1 ; 1:05a8c
    sbb word ptr [_joy_buttonA1], 0 ; 1:05a95
    shr al, 1 ; 1:05a9d
    mov word ptr [_joy_buttonA2], 1 ; 1:05a9f
    sbb word ptr [_joy_buttonA2], 0 ; 1:05aa8
    mov ax, word ptr [_During_The_Menu] ; 1:05ab0
    cmp ax, 1 ; 1:05ab6
    je L_05A2E ; 1:05aba
    and ebx, 0ch ; 1:05ac0
    jne L_05B10 ; 1:05ac3
    mov word ptr [_joyx0], 0 ; 1:05ac5
    mov word ptr [_joyy0], 0 ; 1:05ace
    mov ebx, 7d0h ; 1:05ad7
    call clock_ticks_ ; 1:05adc
    mov dword ptr [joy_timer_start], eax ; 1:05ae1
    mov al, 0ffh ; 1:05ae6
    out dx, al ; 1:05ae8
L_05AE9:
    in al, dx ; 1:05ae9
    mov ebp, eax ; 1:05aea
    and eax, 4 ; 1:05aec
    add ax, word ptr [_joyx0] ; 1:05aef
    je L_05B2F ; 1:05af6
L_05AF8:
    mov eax, ebp ; 1:05af8
    and eax, 8 ; 1:05afa
    add ax, word ptr [_joyy0] ; 1:05afd
    je L_05B42 ; 1:05b04
L_05B06:
    dec ebx ; 1:05b06
    je L_05B10 ; 1:05b07
    and ebp, 0ch ; 1:05b09
    jne L_05AE9 ; 1:05b0c
    jmp L_05B2C ; 1:05b0e
L_05B10:
    mov bx, word ptr [_xpadcentre] ; 1:05b10
    mov word ptr [_joyx0], bx ; 1:05b17
    mov bx, word ptr [_ypadcentre] ; 1:05b1e
    mov word ptr [_joyy0], bx ; 1:05b25
L_05B2C:
    sti ; 1:05b2c
    popad ; 1:05b2d
    ret ; 1:05b2e
L_05B2F:
    call clock_ticks_ ; 1:05b2f
    sub eax, dword ptr [joy_timer_start] ; 1:05b34
    mov word ptr [_joyx0], ax ; 1:05b3a
    jmp L_05AF8 ; 1:05b40
L_05B42:
    call clock_ticks_ ; 1:05b42
    sub eax, dword ptr [joy_timer_start] ; 1:05b47
    mov word ptr [_joyy0], ax ; 1:05b4d
    jmp L_05B06 ; 1:05b53
get_joy_input2_windows1_: ; DOS 1:05b55
    pushad ; 1:05b55
    cli ; 1:05b56
    cld ; 1:05b57
    mov word ptr [_joyx0], 0 ; 1:05b58
    mov word ptr [_joyy0], 0 ; 1:05b61
    xor eax, eax ; 1:05b6a
    mov dx, 201h ; 1:05b6c
    in al, dx ; 1:05b70
    mov ebx, eax ; 1:05b71
    shr al, 5 ; 1:05b73
    mov word ptr [_joy_buttonA1], 1 ; 1:05b76
    sbb word ptr [_joy_buttonA1], 0 ; 1:05b7f
    shr al, 1 ; 1:05b87
    mov word ptr [_joy_buttonA2], 1 ; 1:05b89
    sbb word ptr [_joy_buttonA2], 0 ; 1:05b92
    shr al, 1 ; 1:05b9a
    mov word ptr [_joy_buttonB1], 1 ; 1:05b9c
    sbb word ptr [_joy_buttonB1], 0 ; 1:05ba5
    shr al, 1 ; 1:05bad
    mov word ptr [_joy_buttonB2], 1 ; 1:05baf
    sbb word ptr [_joy_buttonB2], 0 ; 1:05bb8
    and ebx, 3 ; 1:05bc0
    jne L_05BF4 ; 1:05bc3
    mov ebx, 7d0h ; 1:05bc5
    mov al, 0ffh ; 1:05bca
    out dx, al ; 1:05bcc
L_05BCD:
    in al, dx ; 1:05bcd
    mov ebp, eax ; 1:05bce
    and eax, 1 ; 1:05bd0
    je L_05BDC ; 1:05bd3
    inc word ptr [_joyx0] ; 1:05bd5
L_05BDC:
    mov eax, ebp ; 1:05bdc
    and eax, 2 ; 1:05bde
    je L_05BEA ; 1:05be1
    inc word ptr [_joyy0] ; 1:05be3
L_05BEA:
    dec ebx ; 1:05bea
    je L_05BF4 ; 1:05beb
    and ebp, 3 ; 1:05bed
    jne L_05BCD ; 1:05bf0
    jmp L_05C10 ; 1:05bf2
L_05BF4:
    mov bx, word ptr [_xpadcentre] ; 1:05bf4
    mov word ptr [_joyx0], bx ; 1:05bfb
    mov bx, word ptr [_ypadcentre] ; 1:05c02
    mov word ptr [_joyy0], bx ; 1:05c09
L_05C10:
    sti ; 1:05c10
    popad ; 1:05c11
    ret ; 1:05c12
get_joy_input2_windows2_: ; DOS 1:05c13
    pushad ; 1:05c13
    cli ; 1:05c14
    cld ; 1:05c15
    mov word ptr [_joyx0], 0 ; 1:05c16
    mov word ptr [_joyy0], 0 ; 1:05c1f
    xor eax, eax ; 1:05c28
    mov dx, 201h ; 1:05c2a
    in al, dx ; 1:05c2e
    mov ebx, eax ; 1:05c2f
    shr al, 5 ; 1:05c31
    mov word ptr [_joy_buttonB1], 1 ; 1:05c34
    sbb word ptr [_joy_buttonB1], 0 ; 1:05c3d
    shr al, 1 ; 1:05c45
    mov word ptr [_joy_buttonB2], 1 ; 1:05c47
    sbb word ptr [_joy_buttonB2], 0 ; 1:05c50
    shr al, 1 ; 1:05c58
    mov word ptr [_joy_buttonA1], 1 ; 1:05c5a
    sbb word ptr [_joy_buttonA1], 0 ; 1:05c63
    shr al, 1 ; 1:05c6b
    mov word ptr [_joy_buttonA2], 1 ; 1:05c6d
    sbb word ptr [_joy_buttonA2], 0 ; 1:05c76
    and ebx, 0ch ; 1:05c7e
    jne L_05CC0 ; 1:05c81
    mov ebx, 7d0h ; 1:05c83
    mov al, 0ffh ; 1:05c88
    out dx, al ; 1:05c8a
L_05C8B:
    in al, dx ; 1:05c8b
    mov ebp, eax ; 1:05c8c
    and eax, 4 ; 1:05c8e
    add ax, word ptr [_joyx0] ; 1:05c91
    je L_05CA1 ; 1:05c98
    inc word ptr [_joyx0] ; 1:05c9a
L_05CA1:
    mov eax, ebp ; 1:05ca1
    and eax, 8 ; 1:05ca3
    add ax, word ptr [_joyy0] ; 1:05ca6
    je L_05CB6 ; 1:05cad
    inc word ptr [_joyy0] ; 1:05caf
L_05CB6:
    dec ebx ; 1:05cb6
    je L_05CC0 ; 1:05cb7
    and ebp, 0ch ; 1:05cb9
    jne L_05C8B ; 1:05cbc
    jmp L_05CDC ; 1:05cbe
L_05CC0:
    mov bx, word ptr [_xpadcentre] ; 1:05cc0
    mov word ptr [_joyx0], bx ; 1:05cc7
    mov bx, word ptr [_ypadcentre] ; 1:05cce
    mov word ptr [_joyy0], bx ; 1:05cd5
L_05CDC:
    sti ; 1:05cdc
    popad ; 1:05cdd
    ret ; 1:05cde
dos_game_io_port_something_: ; DOS 1:05cdf
    pushad ; 1:05cdf
    cli ; 1:05ce0
    cld ; 1:05ce1
    mov ebx, 7d0h ; 1:05ce2
    mov dx, 201h ; 1:05ce7
    mov eax, 0ffh ; 1:05ceb
    out dx, al ; 1:05cf0
L_05CF1:
    in al, dx ; 1:05cf1
    mov ebp, eax ; 1:05cf2
    and ebp, 3 ; 1:05cf4
    je L_05D04 ; 1:05cf7
    dec ebx ; 1:05cf9
    jne L_05CF1 ; 1:05cfa
L_05CFC:
    popad ; 1:05cfc
    mov eax, 0 ; 1:05cfd
    jmp L_05D12 ; 1:05d02
L_05D04:
    cmp ebx, 7d0h ; 1:05d04
    je L_05CFC ; 1:05d0a
    popad ; 1:05d0c
    mov eax, 1 ; 1:05d0d
L_05D12:
    sti ; 1:05d12
    ret ; 1:05d13
dos_game_io_port_something2_: ; DOS 1:05d14
    pushad ; 1:05d14
    cli ; 1:05d15
    cld ; 1:05d16
    mov ebx, 7d0h ; 1:05d17
    mov dx, 201h ; 1:05d1c
    mov eax, 0ffh ; 1:05d20
    out dx, al ; 1:05d25
    in al, dx ; 1:05d26
L_05D27:
    in al, dx ; 1:05d27
    mov ebp, eax ; 1:05d28
    and ebp, 0ch ; 1:05d2a
    je L_05D3A ; 1:05d2d
    dec ebx ; 1:05d2f
    jne L_05D27 ; 1:05d30
L_05D32:
    popad ; 1:05d32
    mov eax, 0 ; 1:05d33
    jmp L_05D48 ; 1:05d38
L_05D3A:
    cmp ebx, 7d0h ; 1:05d3a
    je L_05D32 ; 1:05d40
    popad ; 1:05d42
    mov eax, 1 ; 1:05d43
L_05D48:
    sti ; 1:05d48
    ret ; 1:05d49
get_joy_input1_dos1_: ; DOS 1:05d4a
    pushad ; 1:05d4a
    cli ; 1:05d4b
    cld ; 1:05d4c
    sub esp, 0ch ; 1:05d4d
    mov dword ptr [esp + 4], eax ; 1:05d50
    mov word ptr [_joyx0], 0 ; 1:05d54
    mov word ptr [_joyy0], 0 ; 1:05d5d
    xor eax, eax ; 1:05d66
    mov dx, 201h ; 1:05d68
    in al, dx ; 1:05d6c
    mov ebp, eax ; 1:05d6d
    shr al, 5 ; 1:05d6f
    mov word ptr [_joy_buttonA1], 1 ; 1:05d72
    sbb word ptr [_joy_buttonA1], 0 ; 1:05d7b
    shr al, 1 ; 1:05d83
    mov word ptr [_joy_buttonA2], 1 ; 1:05d85
    sbb word ptr [_joy_buttonA2], 0 ; 1:05d8e
    shr al, 1 ; 1:05d96
    mov word ptr [_joy_buttonB1], 1 ; 1:05d98
    sbb word ptr [_joy_buttonB1], 0 ; 1:05da1
    shr al, 1 ; 1:05da9
    mov word ptr [_joy_buttonB2], 1 ; 1:05dab
    sbb word ptr [_joy_buttonB2], 0 ; 1:05db4
    mov eax, dword ptr [esp + 4] ; 1:05dbc
    and ebp, 3 ; 1:05dc0
    jne L_05E98 ; 1:05dc3
    shr ecx, 2 ; 1:05dc9
    mov dword ptr [esp], ecx ; 1:05dcc
    mov dword ptr [esp + 4], eax ; 1:05dcf
    call clock_ticks_ ; 1:05dd3
    mov dword ptr [joy_timer_start], eax ; 1:05dd8
    mov al, 0ffh ; 1:05ddd
    out dx, al ; 1:05ddf
    mov eax, dword ptr [esp + 4] ; 1:05de0
    mov ebx, esi ; 1:05de4
    mov edx, edi ; 1:05de6
L_05DE8:
    mov dword ptr [esp + 8], edx ; 1:05de8
    mov dword ptr [esp + 4], eax ; 1:05dec
    mov dx, 201h ; 1:05df0
    in al, dx ; 1:05df4
    mov ebp, eax ; 1:05df5
    and eax, 1 ; 1:05df7
    add ax, word ptr [_joyx0] ; 1:05dfa
    je L_05EDC ; 1:05e01
L_05E07:
    mov eax, ebp ; 1:05e07
    and eax, 2 ; 1:05e09
    add ax, word ptr [_joyy0] ; 1:05e0c
    je L_05EF2 ; 1:05e13
L_05E19:
    mov edx, dword ptr [esp + 8] ; 1:05e19
    mov eax, dword ptr [esp + 4] ; 1:05e1d
    and ebp, 3 ; 1:05e21
    je L_05EBE ; 1:05e24
    mov ecx, dword ptr [esp] ; 1:05e2a
    rep movsd ; 1:05e2d
    add ebx, dword ptr [joy_source_stride] ; 1:05e2f
    add edx, dword ptr [joy_dest_stride] ; 1:05e35
    mov esi, ebx ; 1:05e3b
    mov edi, edx ; 1:05e3d
    dec eax ; 1:05e3f
    jne L_05DE8 ; 1:05e40
    mov ebx, 7d0h ; 1:05e42
    mov dx, 201h ; 1:05e47
L_05E4B:
    in al, dx ; 1:05e4b
    mov ebp, eax ; 1:05e4c
    and eax, 1 ; 1:05e4e
    add ax, word ptr [_joyx0] ; 1:05e51
    je L_05F08 ; 1:05e58
L_05E5E:
    mov eax, ebp ; 1:05e5e
    and eax, 2 ; 1:05e60
    add ax, word ptr [_joyy0] ; 1:05e63
    je L_05F1E ; 1:05e6a
L_05E70:
    dec ebx ; 1:05e70
    je L_05E7A ; 1:05e71
    and ebp, 3 ; 1:05e73
    jne L_05E4B ; 1:05e76
    jmp L_05E96 ; 1:05e78
L_05E7A:
    mov bx, word ptr [_xpadcentre] ; 1:05e7a
    mov word ptr [_joyx0], bx ; 1:05e81
    mov bx, word ptr [_ypadcentre] ; 1:05e88
    mov word ptr [_joyy0], bx ; 1:05e8f
L_05E96:
    jmp L_05ED6 ; 1:05e96
L_05E98:
    mov bx, word ptr [_xpadcentre] ; 1:05e98
    mov word ptr [_joyx0], bx ; 1:05e9f
    mov bx, word ptr [_ypadcentre] ; 1:05ea6
    mov word ptr [_joyy0], bx ; 1:05ead
    shr ecx, 2 ; 1:05eb4
    mov dword ptr [esp], ecx ; 1:05eb7
    mov ebx, esi ; 1:05eba
    mov edx, edi ; 1:05ebc
L_05EBE:
    mov ecx, dword ptr [esp] ; 1:05ebe
    rep movsd ; 1:05ec1
    add ebx, dword ptr [joy_source_stride] ; 1:05ec3
    add edx, dword ptr [joy_dest_stride] ; 1:05ec9
    mov esi, ebx ; 1:05ecf
    mov edi, edx ; 1:05ed1
    dec eax ; 1:05ed3
    jne L_05EBE ; 1:05ed4
L_05ED6:
    add esp, 0ch ; 1:05ed6
    sti ; 1:05ed9
    popad ; 1:05eda
    ret ; 1:05edb
L_05EDC:
    call clock_ticks_ ; 1:05edc
    sub eax, dword ptr [joy_timer_start] ; 1:05ee1
    mov word ptr [_joyx0], ax ; 1:05ee7
    jmp L_05E07 ; 1:05eed
L_05EF2:
    call clock_ticks_ ; 1:05ef2
    sub eax, dword ptr [joy_timer_start] ; 1:05ef7
    mov word ptr [_joyy0], ax ; 1:05efd
    jmp L_05E19 ; 1:05f03
L_05F08:
    call clock_ticks_ ; 1:05f08
    sub eax, dword ptr [joy_timer_start] ; 1:05f0d
    mov word ptr [_joyx0], ax ; 1:05f13
    jmp L_05E5E ; 1:05f19
L_05F1E:
    call clock_ticks_ ; 1:05f1e
    sub eax, dword ptr [joy_timer_start] ; 1:05f23
    mov word ptr [_joyy0], ax ; 1:05f29
    jmp L_05E70 ; 1:05f2f
get_joy_input1_dos2_: ; DOS 1:05f34
    pushad ; 1:05f34
    cli ; 1:05f35
    cld ; 1:05f36
    sub esp, 0ch ; 1:05f37
    mov dword ptr [esp + 4], eax ; 1:05f3a
    mov word ptr [_joyx0], 0 ; 1:05f3e
    mov word ptr [_joyy0], 0 ; 1:05f47
    xor eax, eax ; 1:05f50
    mov dx, 201h ; 1:05f52
    in al, dx ; 1:05f56
    mov ebp, eax ; 1:05f57
    shr al, 5 ; 1:05f59
    mov word ptr [_joy_buttonA1], 1 ; 1:05f5c
    sbb word ptr [_joy_buttonA1], 0 ; 1:05f65
    shr al, 1 ; 1:05f6d
    mov word ptr [_joy_buttonA2], 1 ; 1:05f6f
    sbb word ptr [_joy_buttonA2], 0 ; 1:05f78
    shr al, 1 ; 1:05f80
    mov word ptr [_joy_buttonB1], 1 ; 1:05f82
    sbb word ptr [_joy_buttonB1], 0 ; 1:05f8b
    shr al, 1 ; 1:05f93
    mov word ptr [_joy_buttonB2], 1 ; 1:05f95
    sbb word ptr [_joy_buttonB2], 0 ; 1:05f9e
    mov eax, dword ptr [esp + 4] ; 1:05fa6
    and ebp, 0ch ; 1:05faa
    jne L_06082 ; 1:05fad
    shr ecx, 2 ; 1:05fb3
    mov dword ptr [esp], ecx ; 1:05fb6
    mov dword ptr [esp + 4], eax ; 1:05fb9
    call clock_ticks_ ; 1:05fbd
    mov dword ptr [joy_timer_start], eax ; 1:05fc2
    mov al, 0ffh ; 1:05fc7
    out dx, al ; 1:05fc9
    mov eax, dword ptr [esp + 4] ; 1:05fca
    mov ebx, esi ; 1:05fce
    mov edx, edi ; 1:05fd0
L_05FD2:
    mov dword ptr [esp + 8], edx ; 1:05fd2
    mov dword ptr [esp + 4], eax ; 1:05fd6
    mov dx, 201h ; 1:05fda
    in al, dx ; 1:05fde
    mov ebp, eax ; 1:05fdf
    and eax, 4 ; 1:05fe1
    add ax, word ptr [_joyx0] ; 1:05fe4
    je L_060C6 ; 1:05feb
L_05FF1:
    mov eax, ebp ; 1:05ff1
    and eax, 8 ; 1:05ff3
    add ax, word ptr [_joyy0] ; 1:05ff6
    je L_060DC ; 1:05ffd
L_06003:
    mov edx, dword ptr [esp + 8] ; 1:06003
    mov eax, dword ptr [esp + 4] ; 1:06007
    and ebp, 0ch ; 1:0600b
    je L_060A8 ; 1:0600e
    mov ecx, dword ptr [esp] ; 1:06014
    rep movsd ; 1:06017
    add ebx, dword ptr [joy_source_stride] ; 1:06019
    add edx, dword ptr [joy_dest_stride] ; 1:0601f
    mov esi, ebx ; 1:06025
    mov edi, edx ; 1:06027
    dec eax ; 1:06029
    jne L_05FD2 ; 1:0602a
    mov ebx, 7d0h ; 1:0602c
    mov dx, 201h ; 1:06031
L_06035:
    in al, dx ; 1:06035
    mov ebp, eax ; 1:06036
    and eax, 4 ; 1:06038
    add ax, word ptr [_joyx0] ; 1:0603b
    je L_060F2 ; 1:06042
L_06048:
    mov eax, ebp ; 1:06048
    and eax, 8 ; 1:0604a
    add ax, word ptr [_joyy0] ; 1:0604d
    je L_06108 ; 1:06054
L_0605A:
    dec ebx ; 1:0605a
    je L_06064 ; 1:0605b
    and ebp, 0ch ; 1:0605d
    jne L_06035 ; 1:06060
    jmp L_06080 ; 1:06062
L_06064:
    mov bx, word ptr [_xpadcentre] ; 1:06064
    mov word ptr [_joyx0], bx ; 1:0606b
    mov bx, word ptr [_ypadcentre] ; 1:06072
    mov word ptr [_joyy0], bx ; 1:06079
L_06080:
    jmp L_060C0 ; 1:06080
L_06082:
    mov bx, word ptr [_xpadcentre] ; 1:06082
    mov word ptr [_joyx0], bx ; 1:06089
    mov bx, word ptr [_ypadcentre] ; 1:06090
    mov word ptr [_joyy0], bx ; 1:06097
    shr ecx, 2 ; 1:0609e
    mov dword ptr [esp], ecx ; 1:060a1
    mov ebx, esi ; 1:060a4
    mov edx, edi ; 1:060a6
L_060A8:
    mov ecx, dword ptr [esp] ; 1:060a8
    rep movsd ; 1:060ab
    add ebx, dword ptr [joy_source_stride] ; 1:060ad
    add edx, dword ptr [joy_dest_stride] ; 1:060b3
    mov esi, ebx ; 1:060b9
    mov edi, edx ; 1:060bb
    dec eax ; 1:060bd
    jne L_060A8 ; 1:060be
L_060C0:
    add esp, 0ch ; 1:060c0
    sti ; 1:060c3
    popad ; 1:060c4
    ret ; 1:060c5
L_060C6:
    call clock_ticks_ ; 1:060c6
    sub eax, dword ptr [joy_timer_start] ; 1:060cb
    mov word ptr [_joyx0], ax ; 1:060d1
    jmp L_05FF1 ; 1:060d7
L_060DC:
    call clock_ticks_ ; 1:060dc
    sub eax, dword ptr [joy_timer_start] ; 1:060e1
    mov word ptr [_joyy0], ax ; 1:060e7
    jmp L_06003 ; 1:060ed
L_060F2:
    call clock_ticks_ ; 1:060f2
    sub eax, dword ptr [joy_timer_start] ; 1:060f7
    mov word ptr [_joyx0], ax ; 1:060fd
    jmp L_06048 ; 1:06103
L_06108:
    call clock_ticks_ ; 1:06108
    sub eax, dword ptr [joy_timer_start] ; 1:0610d
    mov word ptr [_joyy0], ax ; 1:06113
    jmp L_0605A ; 1:06119
_TEXT ends

; ---- Accepted source: lowlevel_0611e.asm ----
; DOS RAYMAN.EXE v1.21 low-level assembly, 1:0611e..1:066df (end exclusive).
; Readable WASM 10.6 source reconstructed from the address-anchored cfg.dis listing.
.586p
_TEXT segment para public use32 'CODE'
public DetectCpu_, default_sprite_clipping_, get_sprite_clipping_, sprite_clipping_, DrawSpriteFlipNoClipX_, DrawSpriteX_, DrawSpriteNoClipX_, DrawSpriteFlipX_
extrn _saved_sprite_width:dword, row_offsets_80_01e20:dword

; 1:0611e public; config binding RAYVERSE/ANDROID
DetectCpu_:
    push    bx ; 1:0611e

    pushf   ; 1:06120

    mov     ax, sp ; 1:06122

    and     sp, 0fffch ; 1:06125

    pushfd  ; 1:0612a

    pushfd  ; 1:0612b

    pop     ebx ; 1:0612c

    or      ebx, 040000h ; 1:0612d

    push    ebx ; 1:06133

    popfd   ; 1:06134

    pushfd  ; 1:06135

    pop     ebx ; 1:06136

    popfd   ; 1:06137

    mov     sp, ax ; 1:06138

    test    ebx, 040000h ; 1:0613b

    je      L_617A ; 1:06141

    pushfd  ; 1:06143

    pop     eax ; 1:06144

    mov     ebx, eax ; 1:06145

    xor     eax, 0200000h ; 1:06147

    push    eax ; 1:0614c

    popfd   ; 1:0614d

    pushfd  ; 1:0614e

    pop     eax ; 1:0614f

    push    ebx ; 1:06150

    popfd   ; 1:06151

    and     eax, 0200000h ; 1:06152

    and     ebx, 0200000h ; 1:06157

    cmp     eax, ebx ; 1:0615d

    je      L_6173 ; 1:0615f

    mov     eax, 1 ; 1:06161

    push    edx ; 1:06166

    cpuid   ; 1:06167

    pop     edx ; 1:06169

    shr     ax, 8 ; 1:0616a

    and     eax, 0fh ; 1:0616e

    jmp     L_617F ; 1:06171

L_6173:
    mov     eax, 4 ; 1:06173

    jmp     L_617F ; 1:06178

L_617A:
    mov     eax, 3 ; 1:0617a

L_617F:
    popf    ; 1:0617f

    pop     bx ; 1:06181

    ret     ; 1:06183

; PROVEN state 1:06184..06194; defaults at 06194 initialize these dwords.
; Reads: get_sprite_clipping at 061bf/061c7/061cf/061d7, getters 061e2..061f4,
; and clipping helpers from 06277 onward; writes: 06194..061bc and 06210/0622c/06249/06266.
XMIN_:
dd 8
XMAX_:
dd 312
YMIN_:
dd 0
YMAX_:
dd 200

; 1:06194 public; config binding RAYVERSE
default_sprite_clipping_:
    mov     dword ptr [XMIN_], 8 ; 1:06194

    mov     dword ptr [XMAX_], 0138h ; 1:0619e

    mov     dword ptr [YMIN_], 0 ; 1:061a8

    mov     dword ptr [YMAX_], 0c8h ; 1:061b2

    ret     ; 1:061bc

; 1:061bd public; config binding RAYVERSE
get_sprite_clipping_:
    push    ebp ; 1:061bd

    push    edi ; 1:061be

    mov     ebp, dword ptr [XMIN_] ; 1:061bf

    mov     dword ptr [eax], ebp ; 1:061c5

    mov     edi, dword ptr [XMAX_] ; 1:061c7

    mov     dword ptr [ebx], edi ; 1:061cd

    mov     ebp, dword ptr [YMIN_] ; 1:061cf

    mov     dword ptr [ecx], ebp ; 1:061d5

    mov     edi, dword ptr [YMAX_] ; 1:061d7

    mov     dword ptr [edx], edi ; 1:061dd

    pop     edi ; 1:061df

    pop     ebp ; 1:061e0

    ret     ; 1:061e1

; 1:061e2 local semantic entry (not externally referenced)
get_sprite_clipping_xmin:
    mov     eax, dword ptr [XMIN_] ; 1:061e2

    ret     ; 1:061e7

; 1:061e8 local semantic entry (not externally referenced)
get_sprite_clipping_xmax:
    mov     eax, dword ptr [XMAX_] ; 1:061e8

    ret     ; 1:061ed

; 1:061ee local semantic entry (not externally referenced)
get_sprite_clipping_ymin:
    mov     eax, dword ptr [YMIN_] ; 1:061ee

    ret     ; 1:061f3

; 1:061f4 local semantic entry (not externally referenced)
get_sprite_clipping_ymax:
    mov     eax, dword ptr [YMAX_] ; 1:061f4

    ret     ; 1:061f9

; 1:061fa public; config binding RAYVERSE
sprite_clipping_:
    cmp     eax, 0 ; 1:061fa

    jge     L_6204 ; 1:061fd

    mov     eax, 0 ; 1:061ff

L_6204:
    cmp     eax, 0140h ; 1:06204

    jle     L_6210 ; 1:06209

    mov     eax, 0140h ; 1:0620b

L_6210:
    mov     dword ptr [XMIN_], eax ; 1:06210

    cmp     ebx, 0 ; 1:06215

    jge     L_621F ; 1:06218

    mov     ebx, 0 ; 1:0621a

L_621F:
    cmp     ebx, 0140h ; 1:0621f

    jle     L_622C ; 1:06225

    mov     ebx, 0140h ; 1:06227

L_622C:
    mov     dword ptr [XMAX_], ebx ; 1:0622c

    cmp     ecx, 0 ; 1:06232

    jge     L_623C ; 1:06235

    mov     ecx, 0 ; 1:06237

L_623C:
    cmp     ecx, 0c8h ; 1:0623c

    jle     L_6249 ; 1:06242

    mov     ecx, 0c8h ; 1:06244

L_6249:
    mov     dword ptr [YMIN_], ecx ; 1:06249

    cmp     edx, 0 ; 1:0624f

    jge     L_6259 ; 1:06252

    mov     edx, 0 ; 1:06254

L_6259:
    cmp     edx, 0c8h ; 1:06259

    jle     L_6266 ; 1:0625f

    mov     edx, 0c8h ; 1:06261

L_6266:
    mov     dword ptr [YMAX_], edx ; 1:06266

    ret     ; 1:0626c

clip_sprite_on_screen_:
    xor     edx, edx ; 1:0626d

    mov     dl, cl ; 1:0626f

    mov     dword ptr [_saved_sprite_width], edx ; 1:06271

    cmp     eax, dword ptr [XMIN_] ; 1:06277

    jl      L_62B7 ; 1:0627d

L_627F:
    add     edx, eax ; 1:0627f

    cmp     edx, dword ptr [XMAX_] ; 1:06281

    ja      L_62A3 ; 1:06287

L_6289:
    xor     edx, edx ; 1:06289

    mov     dl, ch ; 1:0628b

    cmp     ebx, dword ptr [YMIN_] ; 1:0628d

    jl      L_62E9 ; 1:06293

L_6295:
    add     edx, ebx ; 1:06295

    cmp     edx, dword ptr [YMAX_] ; 1:06297

    ja      L_62D5 ; 1:0629d

L_629F:
    clc     ; 1:0629f

    ret     ; 1:062a0

L_62A1:
    stc     ; 1:062a1

    ret     ; 1:062a2

L_62A3:
    cmp     eax, dword ptr [XMAX_] ; 1:062a3

    jae     L_62A1 ; 1:062a9

    mov     edx, dword ptr [XMAX_] ; 1:062ab

    sub     edx, eax ; 1:062b1

    mov     cl, dl ; 1:062b3

    jmp     L_6289 ; 1:062b5

L_62B7:
    sub     eax, dword ptr [XMIN_] ; 1:062b7

    neg     eax ; 1:062bd

    cmp     edx, eax ; 1:062bf

    jle     L_62A1 ; 1:062c1

    and     eax, 0ffffh ; 1:062c3

    sub     edx, eax ; 1:062c8

    mov     cl, dl ; 1:062ca

    add     esi, eax ; 1:062cc

    mov     eax, dword ptr [XMIN_] ; 1:062ce

    jmp     L_627F ; 1:062d3

L_62D5:
    cmp     ebx, dword ptr [YMAX_] ; 1:062d5

    jae     L_62A1 ; 1:062db

    mov     edx, dword ptr [YMAX_] ; 1:062dd

    sub     edx, ebx ; 1:062e3

    mov     ch, dl ; 1:062e5

    jmp     L_629F ; 1:062e7

L_62E9:
    add     edx, ebx ; 1:062e9

    cmp     edx, dword ptr [YMIN_] ; 1:062eb

    jle     L_62A1 ; 1:062f1

    sub     ebx, dword ptr [YMIN_] ; 1:062f3

    movzx   edx, ch ; 1:062f9

    neg     ebx ; 1:062fc

    and     ebx, 0ffffh ; 1:062fe

    sub     edx, ebx ; 1:06304

    mov     ch, dl ; 1:06306

    push    eax ; 1:06308

    mov     eax, dword ptr [_saved_sprite_width] ; 1:06309

    mul     bx ; 1:0630e

    add     esi, eax ; 1:06311

    pop     eax ; 1:06313

    mov     ebx, dword ptr [YMIN_] ; 1:06314

    xor     edx, edx ; 1:0631a

    mov     dl, ch ; 1:0631c

    jmp     L_6295 ; 1:0631e

clip_sprite_on_screen_flipped_:
    xor     edx, edx ; 1:06323

    mov     dl, cl ; 1:06325

    mov     dword ptr [_saved_sprite_width], edx ; 1:06327

    cmp     eax, dword ptr [XMIN_] ; 1:0632d

    jl      L_6374 ; 1:06333

L_6335:
    add     edx, eax ; 1:06335

    cmp     edx, dword ptr [XMAX_] ; 1:06337

    ja      L_6358 ; 1:0633d

L_633F:
    movzx   edx, ch ; 1:0633f

    cmp     ebx, dword ptr [YMIN_] ; 1:06342

    jl      L_63A4 ; 1:06348

L_634A:
    add     edx, ebx ; 1:0634a

    cmp     edx, dword ptr [YMAX_] ; 1:0634c

    ja      L_6390 ; 1:06352

L_6354:
    clc     ; 1:06354

    ret     ; 1:06355

L_6356:
    stc     ; 1:06356

    ret     ; 1:06357

L_6358:
    cmp     eax, dword ptr [XMAX_] ; 1:06358

    jae     L_6356 ; 1:0635e

    mov     edx, dword ptr [XMAX_] ; 1:06360

    sub     edx, eax ; 1:06366

    mov     cl, dl ; 1:06368

    add     esi, dword ptr [_saved_sprite_width] ; 1:0636a

    sub     esi, edx ; 1:06370

    jmp     L_633F ; 1:06372

L_6374:
    sub     eax, dword ptr [XMIN_] ; 1:06374

    neg     eax ; 1:0637a

    cmp     edx, eax ; 1:0637c

    jle     L_6356 ; 1:0637e

    and     eax, 0ffffh ; 1:06380

    sub     edx, eax ; 1:06385

    mov     cl, dl ; 1:06387

    mov     eax, dword ptr [XMIN_] ; 1:06389

    jmp     L_6335 ; 1:0638e

L_6390:
    cmp     ebx, dword ptr [YMAX_] ; 1:06390

    jae     L_6356 ; 1:06396

    mov     edx, dword ptr [YMAX_] ; 1:06398

    sub     edx, ebx ; 1:0639e

    mov     ch, dl ; 1:063a0

    jmp     L_6354 ; 1:063a2

L_63A4:
    add     edx, ebx ; 1:063a4

    cmp     edx, dword ptr [YMIN_] ; 1:063a6

    jle     L_6356 ; 1:063ac

    sub     ebx, dword ptr [YMIN_] ; 1:063ae

    movzx   edx, ch ; 1:063b4

    neg     ebx ; 1:063b7

    and     ebx, 0ffffh ; 1:063b9

    sub     edx, ebx ; 1:063bf

    mov     ch, dl ; 1:063c1

    push    eax ; 1:063c3

    mov     eax, dword ptr [_saved_sprite_width] ; 1:063c4

    mul     bx ; 1:063c9

    add     esi, eax ; 1:063cc

    pop     eax ; 1:063ce

    mov     ebx, dword ptr [YMIN_] ; 1:063cf

    xor     edx, edx ; 1:063d5

    mov     dl, ch ; 1:063d7

    jmp     L_634A ; 1:063d9

; PROVEN state 1:063de..063e6, zero-initialized.
; 06404 and 06577 write the source stride; 0646c..0654d and 065e4..066cb read it.
; 06446/06493/064de/06525 and 065be..066a3 write the destination stride;
; 0646c..06547 and 065e4..066c5 read it.
sprite_source_stride_x:
dd 0
sprite_dest_stride_x:
dd 0

; 1:063e6 public; config binding RAYVERSE
DrawSpriteFlipNoClipX_:
    xor     edx, edx ; 1:063e6

    mov     dl, cl ; 1:063e8

    mov     dword ptr [_saved_sprite_width], edx ; 1:063ea

    jmp     L_63FD ; 1:063f0

; 1:063f2 public; config binding RAYVERSE
DrawSpriteX_:
    call    clip_sprite_on_screen_ ; 1:063f2

    jb      L_6558 ; 1:063f7

L_63FD:
    push    ebp ; 1:063fd

    mov     edx, dword ptr [_saved_sprite_width] ; 1:063fe

    mov     dword ptr [sprite_source_stride_x], edx ; 1:06404

    add     edi, dword ptr [ebx*4 + row_offsets_80_01e20] ; 1:0640a

    movzx   edx, ch ; 1:06411

    mov     ebx, dword ptr [edx*4 + row_offsets_80_01e20] ; 1:06414

    add     ebx, edi ; 1:0641b

    and     ecx, 0ffh ; 1:0641d

    push    ecx ; 1:06423

    mov     ecx, eax ; 1:06424

    shr     eax, 2 ; 1:06426

    add     edi, eax ; 1:06429

    and     ecx, 3 ; 1:0642b

    mov     ah, 1 ; 1:0642e

    shl     ah, cl ; 1:06430

    pop     ecx ; 1:06432

    push    ecx ; 1:06433

    add     ecx, 3 ; 1:06434

    shr     ecx, 2 ; 1:06437

    cmp     ecx, 0 ; 1:0643a

    jle     L_647E ; 1:0643d

    mov     ebp, 050h ; 1:0643f

    sub     ebp, ecx ; 1:06444

    mov     dword ptr [sprite_dest_stride_x], ebp ; 1:06446

    mov     edx, 03c4h ; 1:0644c

    mov     al, 2 ; 1:06451

    out     dx, ax ; 1:06453

    mov     al, ah ; 1:06455

    inc     edx ; 1:06457

    push    esi ; 1:06458

    push    edi ; 1:06459

L_645A:
    xor     ebp, ebp ; 1:0645a

L_645C:
    mov     ah, byte ptr [esi + ebp*4] ; 1:0645c

    cmp     ah, 0a0h ; 1:0645f

    jae     L_6466 ; 1:06462

    mov     byte ptr [edi], ah ; 1:06464

L_6466:
    inc     ebp ; 1:06466

    inc     edi ; 1:06467

    cmp     ebp, ecx ; 1:06468

    jb      L_645C ; 1:0646a

    add     edi, dword ptr [sprite_dest_stride_x] ; 1:0646c

    add     esi, dword ptr [sprite_source_stride_x] ; 1:06472

    cmp     edi, ebx ; 1:06478

    jb      L_645A ; 1:0647a

    pop     edi ; 1:0647c

    pop     esi ; 1:0647d

L_647E:
    pop     ecx ; 1:0647e

    inc     esi ; 1:0647f

    push    ecx ; 1:06480

    add     ecx, 2 ; 1:06481

    shr     ecx, 2 ; 1:06484

    cmp     ecx, 0 ; 1:06487

    jle     L_64C9 ; 1:0648a

    mov     ebp, 050h ; 1:0648c

    sub     ebp, ecx ; 1:06491

    mov     dword ptr [sprite_dest_stride_x], ebp ; 1:06493

    add     al, al ; 1:06499

    cmp     al, 010h ; 1:0649b

    jne     L_64A2 ; 1:0649d

    inc     edi ; 1:0649f

    mov     al, 1 ; 1:064a0

L_64A2:
    out     dx, al ; 1:064a2

    push    esi ; 1:064a3

    push    edi ; 1:064a4

L_64A5:
    xor     ebp, ebp ; 1:064a5

L_64A7:
    mov     ah, byte ptr [esi + ebp*4] ; 1:064a7

    cmp     ah, 0a0h ; 1:064aa

    jae     L_64B1 ; 1:064ad

    mov     byte ptr [edi], ah ; 1:064af

L_64B1:
    inc     ebp ; 1:064b1

    inc     edi ; 1:064b2

    cmp     ebp, ecx ; 1:064b3

    jb      L_64A7 ; 1:064b5

    add     edi, dword ptr [sprite_dest_stride_x] ; 1:064b7

    add     esi, dword ptr [sprite_source_stride_x] ; 1:064bd

    cmp     edi, ebx ; 1:064c3

    jb      L_64A5 ; 1:064c5

    pop     edi ; 1:064c7

    pop     esi ; 1:064c8

L_64C9:
    pop     ecx ; 1:064c9

    inc     esi ; 1:064ca

    push    ecx ; 1:064cb

    add     ecx, 1 ; 1:064cc

    shr     ecx, 2 ; 1:064cf

    cmp     ecx, 0 ; 1:064d2

    jle     L_6514 ; 1:064d5

    mov     ebp, 050h ; 1:064d7

    sub     ebp, ecx ; 1:064dc

    mov     dword ptr [sprite_dest_stride_x], ebp ; 1:064de

    add     al, al ; 1:064e4

    cmp     al, 010h ; 1:064e6

    jne     L_64ED ; 1:064e8

    inc     edi ; 1:064ea

    mov     al, 1 ; 1:064eb

L_64ED:
    out     dx, al ; 1:064ed

    push    esi ; 1:064ee

    push    edi ; 1:064ef

L_64F0:
    xor     ebp, ebp ; 1:064f0

L_64F2:
    mov     ah, byte ptr [esi + ebp*4] ; 1:064f2

    cmp     ah, 0a0h ; 1:064f5

    jae     L_64FC ; 1:064f8

    mov     byte ptr [edi], ah ; 1:064fa

L_64FC:
    inc     ebp ; 1:064fc

    inc     edi ; 1:064fd

    cmp     ebp, ecx ; 1:064fe

    jb      L_64F2 ; 1:06500

    add     edi, dword ptr [sprite_dest_stride_x] ; 1:06502

    add     esi, dword ptr [sprite_source_stride_x] ; 1:06508

    cmp     edi, ebx ; 1:0650e

    jb      L_64F0 ; 1:06510

    pop     edi ; 1:06512

    pop     esi ; 1:06513

L_6514:
    pop     ecx ; 1:06514

    inc     esi ; 1:06515

    shr     ecx, 2 ; 1:06516

    cmp     ecx, 0 ; 1:06519

    jle     L_6557 ; 1:0651c

    mov     ebp, 050h ; 1:0651e

    sub     ebp, ecx ; 1:06523

    mov     dword ptr [sprite_dest_stride_x], ebp ; 1:06525

    add     al, al ; 1:0652b

    cmp     al, 010h ; 1:0652d

    jne     L_6534 ; 1:0652f

    inc     edi ; 1:06531

    mov     al, 1 ; 1:06532

L_6534:
    out     dx, al ; 1:06534

L_6535:
    xor     ebp, ebp ; 1:06535

L_6537:
    mov     ah, byte ptr [esi + ebp*4] ; 1:06537

    cmp     ah, 0a0h ; 1:0653a

    jae     L_6541 ; 1:0653d

    mov     byte ptr [edi], ah ; 1:0653f

L_6541:
    inc     ebp ; 1:06541

    inc     edi ; 1:06542

    cmp     ebp, ecx ; 1:06543

    jb      L_6537 ; 1:06545

    add     edi, dword ptr [sprite_dest_stride_x] ; 1:06547

    add     esi, dword ptr [sprite_source_stride_x] ; 1:0654d

    cmp     edi, ebx ; 1:06553

    jb      L_6535 ; 1:06555

L_6557:
    pop     ebp ; 1:06557

L_6558:
    ret     ; 1:06558

; 1:06559 public; config binding RAYVERSE
DrawSpriteNoClipX_:
    xor     edx, edx ; 1:06559

    mov     dl, cl ; 1:0655b

    mov     dword ptr [_saved_sprite_width], edx ; 1:0655d

    jmp     L_6570 ; 1:06563

; 1:06565 public; config binding RAYVERSE
DrawSpriteFlipX_:
    call    clip_sprite_on_screen_flipped_ ; 1:06565

    jb      L_66D6 ; 1:0656a

L_6570:
    push    ebp ; 1:06570

    mov     edx, dword ptr [_saved_sprite_width] ; 1:06571

    mov     dword ptr [sprite_source_stride_x], edx ; 1:06577

    add     edi, dword ptr [ebx*4 + row_offsets_80_01e20] ; 1:0657d

    movzx   edx, ch ; 1:06584

    mov     ebx, dword ptr [edx*4 + row_offsets_80_01e20] ; 1:06587

    add     ebx, edi ; 1:0658e

    and     ecx, 0ffh ; 1:06590

    push    ecx ; 1:06596

    mov     ecx, eax ; 1:06597

    shr     eax, 2 ; 1:06599

    add     edi, eax ; 1:0659c

    and     ecx, 3 ; 1:0659e

    mov     ah, 1 ; 1:065a1

    shl     ah, cl ; 1:065a3

    pop     ecx ; 1:065a5

    add     esi, ecx ; 1:065a6

    dec     esi ; 1:065a8

    push    ecx ; 1:065a9

    add     ecx, 3 ; 1:065aa

    shr     ecx, 2 ; 1:065ad

    cmp     ecx, 0 ; 1:065b0

    jle     L_65F6 ; 1:065b3

    neg     ecx ; 1:065b5

    mov     ebp, 050h ; 1:065b7

    add     ebp, ecx ; 1:065bc

    mov     dword ptr [sprite_dest_stride_x], ebp ; 1:065be

    mov     edx, 03c4h ; 1:065c4

    mov     al, 2 ; 1:065c9

    out     dx, ax ; 1:065cb

    mov     al, ah ; 1:065cd

    inc     edx ; 1:065cf

    push    esi ; 1:065d0

    push    edi ; 1:065d1

L_65D2:
    xor     ebp, ebp ; 1:065d2

L_65D4:
    mov     ah, byte ptr [esi + ebp*4] ; 1:065d4

    cmp     ah, 0a0h ; 1:065d7

    jae     L_65DE ; 1:065da

    mov     byte ptr [edi], ah ; 1:065dc

L_65DE:
    dec     ebp ; 1:065de

    inc     edi ; 1:065df

    cmp     ebp, ecx ; 1:065e0

    jg      L_65D4 ; 1:065e2

    add     edi, dword ptr [sprite_dest_stride_x] ; 1:065e4

    add     esi, dword ptr [sprite_source_stride_x] ; 1:065ea

    cmp     edi, ebx ; 1:065f0

    jb      L_65D2 ; 1:065f2

    pop     edi ; 1:065f4

    pop     esi ; 1:065f5

L_65F6:
    pop     ecx ; 1:065f6

    dec     esi ; 1:065f7

    push    ecx ; 1:065f8

    add     ecx, 2 ; 1:065f9

    shr     ecx, 2 ; 1:065fc

    cmp     ecx, 0 ; 1:065ff

    jle     L_6643 ; 1:06602

    neg     ecx ; 1:06604

    mov     ebp, 050h ; 1:06606

    add     ebp, ecx ; 1:0660b

    mov     dword ptr [sprite_dest_stride_x], ebp ; 1:0660d

    add     al, al ; 1:06613

    cmp     al, 010h ; 1:06615

    jne     L_661C ; 1:06617

    inc     edi ; 1:06619

    mov     al, 1 ; 1:0661a

L_661C:
    out     dx, al ; 1:0661c

    push    esi ; 1:0661d

    push    edi ; 1:0661e

L_661F:
    xor     ebp, ebp ; 1:0661f

L_6621:
    mov     ah, byte ptr [esi + ebp*4] ; 1:06621

    cmp     ah, 0a0h ; 1:06624

    jae     L_662B ; 1:06627

    mov     byte ptr [edi], ah ; 1:06629

L_662B:
    dec     ebp ; 1:0662b

    inc     edi ; 1:0662c

    cmp     ebp, ecx ; 1:0662d

    jg      L_6621 ; 1:0662f

    add     edi, dword ptr [sprite_dest_stride_x] ; 1:06631

    add     esi, dword ptr [sprite_source_stride_x] ; 1:06637

    cmp     edi, ebx ; 1:0663d

    jb      L_661F ; 1:0663f

    pop     edi ; 1:06641

    pop     esi ; 1:06642

L_6643:
    pop     ecx ; 1:06643

    dec     esi ; 1:06644

    push    ecx ; 1:06645

    add     ecx, 1 ; 1:06646

    shr     ecx, 2 ; 1:06649

    cmp     ecx, 0 ; 1:0664c

    jle     L_6690 ; 1:0664f

    neg     ecx ; 1:06651

    mov     ebp, 050h ; 1:06653

    add     ebp, ecx ; 1:06658

    mov     dword ptr [sprite_dest_stride_x], ebp ; 1:0665a

    add     al, al ; 1:06660

    cmp     al, 010h ; 1:06662

    jne     L_6669 ; 1:06664

    inc     edi ; 1:06666

    mov     al, 1 ; 1:06667

L_6669:
    out     dx, al ; 1:06669

    push    esi ; 1:0666a

    push    edi ; 1:0666b

L_666C:
    xor     ebp, ebp ; 1:0666c

L_666E:
    mov     ah, byte ptr [esi + ebp*4] ; 1:0666e

    cmp     ah, 0a0h ; 1:06671

    jae     L_6678 ; 1:06674

    mov     byte ptr [edi], ah ; 1:06676

L_6678:
    dec     ebp ; 1:06678

    inc     edi ; 1:06679

    cmp     ebp, ecx ; 1:0667a

    jg      L_666E ; 1:0667c

    add     edi, dword ptr [sprite_dest_stride_x] ; 1:0667e

    add     esi, dword ptr [sprite_source_stride_x] ; 1:06684

    cmp     edi, ebx ; 1:0668a

    jb      L_666C ; 1:0668c

    pop     edi ; 1:0668e

    pop     esi ; 1:0668f

L_6690:
    pop     ecx ; 1:06690

    dec     esi ; 1:06691

    shr     ecx, 2 ; 1:06692

    cmp     ecx, 0 ; 1:06695

    jle     L_66D5 ; 1:06698

    neg     ecx ; 1:0669a

    mov     ebp, 050h ; 1:0669c

    add     ebp, ecx ; 1:066a1

    mov     dword ptr [sprite_dest_stride_x], ebp ; 1:066a3

    add     al, al ; 1:066a9

    cmp     al, 010h ; 1:066ab

    jne     L_66B2 ; 1:066ad

    inc     edi ; 1:066af

    mov     al, 1 ; 1:066b0

L_66B2:
    out     dx, al ; 1:066b2

L_66B3:
    xor     ebp, ebp ; 1:066b3

L_66B5:
    mov     ah, byte ptr [esi + ebp*4] ; 1:066b5

    cmp     ah, 0a0h ; 1:066b8

    jae     L_66BF ; 1:066bb

    mov     byte ptr [edi], ah ; 1:066bd

L_66BF:
    dec     ebp ; 1:066bf

    inc     edi ; 1:066c0

    cmp     ebp, ecx ; 1:066c1

    jg      L_66B5 ; 1:066c3

    add     edi, dword ptr [sprite_dest_stride_x] ; 1:066c5

    add     esi, dword ptr [sprite_source_stride_x] ; 1:066cb

    cmp     edi, ebx ; 1:066d1

    jb      L_66B3 ; 1:066d3

L_66D5:
    pop     ebp ; 1:066d5

L_66D6:
    ret     ; 1:066d6

; PROVEN state 1:066d7..066df, zero-initialized mutable copy state.
; Later writers: 066fe/06895 for source stride and 06741/06797/0683b for destination stride;
; readers: 06775/067c9/0681d/0686a and 0676f/067c3/06817/06864.
mode_x_sprite_state_:
dd 0
mode_x_sprite_destination_stride:
dd 0

_TEXT ends

; ---- Accepted source: lowlevel_066df.asm ----
; DOS v1.21 low-level assembly slice 1:066df..1:06f7e.
; Typed state and all explicit instructions are kept at their oracle offsets.
.386p
_TEXT segment para public use32 'CODE'

extrn _saved_sprite_color:dword
extrn _saved_sprite_width:dword
extrn row_offsets_80_01e20:dword
extrn lowlevel_timer_02148:dword
extrn lowlevel_timer_02150:dword
extrn lowlevel_timer_02154:dword
extrn lowlevel_timer_02158:dword

public DrawSpriteColorX_
public DrawSpriteColorFlipX_
public DrawSpriteNormalNoClip_
public DrawSpriteNormal_
public DrawSpriteNormal256_
public DrawSpriteFlipNormalNoClip_
public DrawSpriteFlipNormal_
public DrawSpriteFlipNormal256_
public DrawSpriteColorNormal_
public DrawSpriteColorFlipNormal_
public DrawSpriteDiffNormal_
public synchro_
public endsynchro_
public set_speaker_on_
public set_speaker_off_
public clock_ticks_
public wait_until_
public speaker_enable_

; DOS 1:066df
DrawSpriteColorX_:
    lea edx, [edx*8]
    mov dword ptr [_saved_sprite_color], edx
    call clip_sprite_on_screen_
    jb L_06875
    push ebp
    mov edx, dword ptr [_saved_sprite_width]
    mov dword ptr [mode_x_sprite_state_], edx
    xor edx, edx
    add edi, dword ptr [ebx*4 + row_offsets_80_01e20]
    mov dl, ch
    mov ebx, dword ptr [edx*4 + row_offsets_80_01e20]
    and ecx, 0xff
    add ebx, edi
    push ecx
    mov ecx, eax
    shr eax, 2
    add edi, eax
    and ecx, 3
    mov ah, 1
    shl ah, cl
    pop ecx
    push ecx
    add ecx, 3
    shr ecx, 2
    cmp ecx, 0
    jle L_06782
    mov ebp, 0x50
    sub ebp, ecx
    mov dword ptr [mode_x_sprite_state_ + 4], ebp
    mov edx, 0x3c4
    mov al, 2
    out dx, ax
    mov al, ah
    inc edx
    push esi
    push edi
    push eax
    mov eax, dword ptr [_saved_sprite_color]
L_0675b:
    xor ebp, ebp
L_0675d:
    mov ah, byte ptr [esi + ebp*4]
    cmp ah, 0xa0
    jae L_06769
    or ah, al
    mov byte ptr [edi], ah
L_06769:
    inc ebp
    inc edi
    cmp ebp, ecx
    jb L_0675d
    add edi, dword ptr [mode_x_sprite_state_ + 4]
    add esi, dword ptr [mode_x_sprite_state_]
    cmp edi, ebx
    jb L_0675b
    pop eax
    pop edi
    pop esi
L_06782:
    pop ecx
    inc esi
    push ecx
    add ecx, 2
    shr ecx, 2
    cmp ecx, 0
    jle L_067d6
    mov ebp, 0x50
    sub ebp, ecx
    mov dword ptr [mode_x_sprite_state_ + 4], ebp
    add al, al
    cmp al, 0x10
    jne L_067a6
    inc edi
    mov al, 1
L_067a6:
    out dx, al
    push esi
    push edi
    push eax
    mov eax, dword ptr [_saved_sprite_color]
L_067af:
    xor ebp, ebp
L_067b1:
    mov ah, byte ptr [esi + ebp*4]
    cmp ah, 0xa0
    jae L_067bd
    or ah, al
    mov byte ptr [edi], ah
L_067bd:
    inc ebp
    inc edi
    cmp ebp, ecx
    jb L_067b1
    add edi, dword ptr [mode_x_sprite_state_ + 4]
    add esi, dword ptr [mode_x_sprite_state_]
    cmp edi, ebx
    jb L_067af
    pop eax
    pop edi
    pop esi
L_067d6:
    pop ecx
    inc esi
    push ecx
    add ecx, 1
    shr ecx, 2
    cmp ecx, 0
    jle L_0682a
    mov ebp, 0x50
    sub ebp, ecx
    mov dword ptr [mode_x_sprite_state_ + 4], ebp
    add al, al
    cmp al, 0x10
    jne L_067fa
    inc edi
    mov al, 1
L_067fa:
    out dx, al
    push esi
    push edi
    push eax
    mov eax, dword ptr [_saved_sprite_color]
L_06803:
    xor ebp, ebp
L_06805:
    mov ah, byte ptr [esi + ebp*4]
    cmp ah, 0xa0
    jae L_06811
    or ah, al
    mov byte ptr [edi], ah
L_06811:
    inc ebp
    inc edi
    cmp ebp, ecx
    jb L_06805
    add edi, dword ptr [mode_x_sprite_state_ + 4]
    add esi, dword ptr [mode_x_sprite_state_]
    cmp edi, ebx
    jb L_06803
    pop eax
    pop edi
    pop esi
L_0682a:
    pop ecx
    inc esi
    shr ecx, 2
    cmp ecx, 0
    jle L_06874
    mov ebp, 0x50
    sub ebp, ecx
    mov dword ptr [mode_x_sprite_state_ + 4], ebp
    add al, al
    cmp al, 0x10
    jne L_0684a
    inc edi
    mov al, 1
L_0684a:
    out dx, al
L_0684b:
    xor ebp, ebp
    mov eax, dword ptr [_saved_sprite_color]
L_06852:
    mov ah, byte ptr [esi + ebp*4]
    cmp ah, 0xa0
    jae L_0685e
    or ah, al
    mov byte ptr [edi], ah
L_0685e:
    inc ebp
    inc edi
    cmp ebp, ecx
    jb L_06852
    add edi, dword ptr [mode_x_sprite_state_ + 4]
    add esi, dword ptr [mode_x_sprite_state_]
    cmp edi, ebx
    jb L_0684b
L_06874:
    pop ebp
L_06875:
    ret
; DOS 1:06876
DrawSpriteColorFlipX_:
    lea edx, [edx*8]
    mov dword ptr [_saved_sprite_color], edx
    call clip_sprite_on_screen_flipped_
    jb L_06a17
    push ebp
    mov edx, dword ptr [_saved_sprite_width]
    mov dword ptr [mode_x_sprite_state_], edx
    xor edx, edx
    add edi, dword ptr [ebx*4 + row_offsets_80_01e20]
    mov dl, ch
    mov ebx, dword ptr [edx*4 + row_offsets_80_01e20]
    and ecx, 0xff
    add ebx, edi
    push ecx
    mov ecx, eax
    shr eax, 2
    add edi, eax
    and ecx, 3
    mov ah, 1
    shl ah, cl
    pop ecx
    add esi, ecx
    dec esi
    push ecx
    add ecx, 3
    shr ecx, 2
    cmp ecx, 0
    jle L_0691e
    neg ecx
    mov ebp, 0x50
    add ebp, ecx
    mov dword ptr [mode_x_sprite_state_ + 4], ebp
    mov edx, 0x3c4
    mov al, 2
    out dx, ax
    mov al, ah
    inc edx
    push esi
    push edi
    push eax
    mov eax, dword ptr [_saved_sprite_color]
L_068f7:
    xor ebp, ebp
L_068f9:
    mov ah, byte ptr [esi + ebp*4]
    cmp ah, 0xa0
    jae L_06905
    or ah, al
    mov byte ptr [edi], ah
L_06905:
    dec ebp
    inc edi
    cmp ebp, ecx
    jg L_068f9
    add edi, dword ptr [mode_x_sprite_state_ + 4]
    add esi, dword ptr [mode_x_sprite_state_]
    cmp edi, ebx
    jb L_068f7
    pop eax
    pop edi
    pop esi
L_0691e:
    pop ecx
    dec esi
    push ecx
    add ecx, 2
    shr ecx, 2
    cmp ecx, 0
    jle L_06974
    neg ecx
    mov ebp, 0x50
    add ebp, ecx
    mov dword ptr [mode_x_sprite_state_ + 4], ebp
    add al, al
    cmp al, 0x10
    jne L_06944
    inc edi
    mov al, 1
L_06944:
    out dx, al
    push esi
    push edi
    push eax
    mov eax, dword ptr [_saved_sprite_color]
L_0694d:
    xor ebp, ebp
L_0694f:
    mov ah, byte ptr [esi + ebp*4]
    cmp ah, 0xa0
    jae L_0695b
    or ah, al
    mov byte ptr [edi], ah
L_0695b:
    dec ebp
    inc edi
    cmp ebp, ecx
    jg L_0694f
    add edi, dword ptr [mode_x_sprite_state_ + 4]
    add esi, dword ptr [mode_x_sprite_state_]
    cmp edi, ebx
    jb L_0694d
    pop eax
    pop edi
    pop esi
L_06974:
    pop ecx
    dec esi
    push ecx
    add ecx, 1
    shr ecx, 2
    cmp ecx, 0
    jle L_069ca
    neg ecx
    mov ebp, 0x50
    add ebp, ecx
    mov dword ptr [mode_x_sprite_state_ + 4], ebp
    add al, al
    cmp al, 0x10
    jne L_0699a
    inc edi
    mov al, 1
L_0699a:
    out dx, al
    push esi
    push edi
    push eax
    mov eax, dword ptr [_saved_sprite_color]
L_069a3:
    xor ebp, ebp
L_069a5:
    mov ah, byte ptr [esi + ebp*4]
    cmp ah, 0xa0
    jae L_069b1
    or ah, al
    mov byte ptr [edi], ah
L_069b1:
    dec ebp
    inc edi
    cmp ebp, ecx
    jg L_069a5
    add edi, dword ptr [mode_x_sprite_state_ + 4]
    add esi, dword ptr [mode_x_sprite_state_]
    cmp edi, ebx
    jb L_069a3
    pop eax
    pop edi
    pop esi
L_069ca:
    pop ecx
    dec esi
    shr ecx, 2
    cmp ecx, 0
    jle L_06a16
    neg ecx
    mov ebp, 0x50
    add ebp, ecx
    mov dword ptr [mode_x_sprite_state_ + 4], ebp
    add al, al
    cmp al, 0x10
    jne L_069ec
    inc edi
    mov al, 1
L_069ec:
    out dx, al
    mov eax, dword ptr [_saved_sprite_color]
L_069f2:
    xor ebp, ebp
L_069f4:
    mov ah, byte ptr [esi + ebp*4]
    cmp ah, 0xa0
    jae L_06a00
    or ah, al
    mov byte ptr [edi], ah
L_06a00:
    dec ebp
    inc edi
    cmp ebp, ecx
    jg L_069f4
    add edi, dword ptr [mode_x_sprite_state_ + 4]
    add esi, dword ptr [mode_x_sprite_state_]
    cmp edi, ebx
    jb L_069f2
L_06a16:
    pop ebp
L_06a17:
    ret
; DOS 1:06a18
DrawSpriteNormalNoClip_:
    xor edx, edx
    mov dl, cl
    mov dword ptr [_saved_sprite_width], edx
    jmp L_06a30
; DOS 1:06a24
DrawSpriteNormal_:
    call clip_sprite_on_screen_
    jb L_06a9c
    cmp cl, 0
    jbe L_06a9c
L_06a30:
    mov edx, dword ptr [ebx*4 + row_offsets_80_01e20]
    lea edx, [edx*4]
    add edi, edx
    xor edx, edx
    mov dl, ch
    mov ebx, dword ptr [edx*4 + row_offsets_80_01e20]
    lea ebx, [ebx*4]
    add ebx, edi
    add edi, eax
    and ecx, 0xff
    push ebp
    mov edx, dword ptr [_saved_sprite_width]
    xor eax, eax
L_06a65:
    xor ebp, ebp
L_06a67:
    mov al, byte ptr [esi + ebp]
    cmp al, 0xa0
    jae L_06a84
    mov byte ptr [edi + ebp], al
    inc ebp
    cmp ebp, ecx
    jb L_06a67
    add edi, 0x140
    add esi, edx
    cmp edi, ebx
    jb L_06a65
    pop ebp
    ret
L_06a84:
    sub eax, 0xa0
    add ebp, eax
    cmp ebp, ecx
    jl L_06a67
    add edi, 0x140
    add esi, edx
    cmp edi, ebx
    jb L_06a65
    pop ebp
L_06a9c:
    ret
; DOS 1:06a9d
DrawSpriteNormal256_:
    call clip_sprite_on_screen_
    jb L_06afa
    cmp cl, 0
    jbe L_06afa
    mov edx, dword ptr [ebx*4 + row_offsets_80_01e20]
    lea edx, [edx*4]
    add edi, edx
    xor edx, edx
    mov dl, ch
    mov ebx, dword ptr [edx*4 + row_offsets_80_01e20]
    lea ebx, [ebx*4]
    add ebx, edi
    add edi, eax
    and ecx, 0xff
    push ebp
    mov edx, dword ptr [_saved_sprite_width]
L_06adc:
    xor ebp, ebp
L_06ade:
    mov al, byte ptr [esi + ebp]
    cmp al, 0
    je L_06ae8
    mov byte ptr [edi + ebp], al
L_06ae8:
    inc ebp
    cmp ebp, ecx
    jb L_06ade
    add edi, 0x140
    add esi, edx
    cmp edi, ebx
    jb L_06adc
    pop ebp
L_06afa:
    ret
; DOS 06afb: Unreferenced DOS RET byte in the original span.
L_06afb:
    ret

; DOS 1:06afc
DrawSpriteFlipNormalNoClip_:
    xor edx, edx
    mov dl, cl
    mov dword ptr [_saved_sprite_width], edx
    jmp L_06b14
; DOS 1:06b08
DrawSpriteFlipNormal_:
    call clip_sprite_on_screen_flipped_
    jb L_06b87
    cmp cl, 0
    jbe L_06b87
L_06b14:
    mov edx, dword ptr [ebx*4 + row_offsets_80_01e20]
    lea edx, [edx*4]
    add edi, edx
    xor edx, edx
    mov dl, ch
    mov ebx, dword ptr [edx*4 + row_offsets_80_01e20]
    lea ebx, [ebx*4]
    add ebx, edi
    add edi, eax
    and ecx, 0xff
    push ebp
    mov edx, dword ptr [_saved_sprite_width]
    xor eax, eax
L_06b49:
    xor ebp, ebp
    push edi
L_06b4c:
    mov al, byte ptr [esi + ebp]
    cmp al, 0xa0
    jae L_06b6c
    mov byte ptr [edi + ecx - 1], al
    inc ebp
    dec edi
    cmp ebp, ecx
    jl L_06b4c
    pop edi
    add edi, 0x140
    add esi, edx
    cmp edi, ebx
    jb L_06b49
    pop ebp
    ret
L_06b6c:
    sub eax, 0xa0
    add ebp, eax
    sub edi, eax
    cmp ebp, ecx
    jl L_06b4c
    pop edi
    add edi, 0x140
    add esi, edx
    cmp edi, ebx
    jb L_06b49
    pop ebp
L_06b87:
    ret
; DOS 1:06b88
DrawSpriteFlipNormal256_:
    call clip_sprite_on_screen_flipped_
    jb L_06be9
    cmp cl, 0
    jbe L_06be9
    mov edx, dword ptr [ebx*4 + row_offsets_80_01e20]
    lea edx, [edx*4]
    add edi, edx
    xor edx, edx
    mov dl, ch
    mov ebx, dword ptr [edx*4 + row_offsets_80_01e20]
    lea ebx, [ebx*4]
    add ebx, edi
    add edi, eax
    and ecx, 0xff
    push ebp
    mov edx, dword ptr [_saved_sprite_width]
L_06bc7:
    xor ebp, ebp
    push edi
L_06bca:
    mov al, byte ptr [esi + ebp]
    cmp al, 0
    je L_06bd5
    mov byte ptr [edi + ecx - 1], al
L_06bd5:
    inc ebp
    dec edi
    cmp ebp, ecx
    jl L_06bca
    pop edi
    add edi, 0x140
    add esi, edx
    cmp edi, ebx
    jb L_06bc7
    pop ebp
L_06be9:
    ret
; DOS 1:06bea
DrawSpriteColorNormal_:
    lea edx, [edx*8]
    mov dword ptr [_saved_sprite_color], edx
    call clip_sprite_on_screen_
    jb L_06c88
    cmp cl, 0
    jbe near ptr L_06c88
    mov edx, dword ptr [ebx*4 + row_offsets_80_01e20]
    lea edx, [edx*4]
    add edi, edx
    xor edx, edx
    mov dl, ch
    mov ebx, dword ptr [edx*4 + row_offsets_80_01e20]
    lea ebx, [ebx*4]
    add ebx, edi
    add edi, eax
    and ecx, 0xff
    push ebp
    mov edx, dword ptr [_saved_sprite_width]
    mov ah, byte ptr [_saved_sprite_color]
L_06c44:
    xor ebp, ebp
L_06c46:
    mov al, byte ptr [esi + ebp]
    cmp al, 0xa0
    jae L_06c65
    or al, ah
    mov byte ptr [edi + ebp], al
    inc ebp
    cmp ebp, ecx
    jb L_06c46
    add edi, 0x140
    add esi, edx
    cmp edi, ebx
    jb L_06c44
    pop ebp
    ret
L_06c65:
    and eax, 0xff
    sub eax, 0xa0
    add ebp, eax
    mov ah, byte ptr [_saved_sprite_color]
    cmp ebp, ecx
    jl L_06c46
    add edi, 0x140
    add esi, edx
    cmp edi, ebx
    jb L_06c44
    pop ebp
L_06c88:
    ret
; DOS 1:06c89
DrawSpriteColorFlipNormal_:
    lea edx, [edx*8]
    mov dword ptr [_saved_sprite_color], edx
    call clip_sprite_on_screen_flipped_
    jb L_06d2e
    cmp cl, 0
    jbe L_06d2e
    mov edx, dword ptr [ebx*4 + row_offsets_80_01e20]
    lea edx, [edx*4]
    add edi, edx
    xor edx, edx
    mov dl, ch
    mov ebx, dword ptr [edx*4 + row_offsets_80_01e20]
    lea ebx, [ebx*4]
    add ebx, edi
    add edi, eax
    and ecx, 0xff
    push ebp
    mov edx, dword ptr [_saved_sprite_width]
    mov ah, byte ptr [_saved_sprite_color]
L_06ce3:
    xor ebp, ebp
    push edi
L_06ce6:
    mov al, byte ptr [esi + ebp]
    cmp al, 0xa0
    jae L_06d08
    or al, ah
    mov byte ptr [edi + ecx - 1], al
    inc ebp
    dec edi
    cmp ebp, ecx
    jl L_06ce6
    pop edi
    add edi, 0x140
    add esi, edx
    cmp edi, ebx
    jb L_06ce3
    pop ebp
    ret
L_06d08:
    and eax, 0xff
    sub eax, 0xa0
    add ebp, eax
    sub edi, eax
    mov ah, byte ptr [_saved_sprite_color]
    cmp ebp, ecx
    jl L_06ce6
    pop edi
    add edi, 0x140
    add esi, edx
    cmp edi, ebx
    jb L_06ce3
    pop ebp
L_06d2e:
    ret
; PROVEN Writable dword state: 06d37 writes it; 06ddb and 06e75/06e7b read it.
diff_saved_sprite_width:
    dd 0

; Internal clip/adjust helper; entry is not externally referenced.
asm_06d33:
    xor edx, edx
    mov dl, cl
    mov dword ptr [diff_saved_sprite_width], edx
    cmp eax, dword ptr [XMIN_]
    jl L_06d81
    add edx, eax
    cmp edx, dword ptr [XMAX_]
    ja L_06d6d
    shr cl, 2
L_06d52:
    xor edx, edx
    mov dl, ch
    cmp ebx, dword ptr [YMIN_]
    jl L_06dbb
L_06d5e:
    add edx, ebx
    cmp edx, dword ptr [YMAX_]
    ja L_06da7
L_06d68:
    clc
    ret
L_06d6a:
    pop eax
L_06d6b:
    stc
    ret
L_06d6d:
    cmp eax, dword ptr [XMAX_]
    jae L_06d6b
    mov edx, dword ptr [XMAX_]
    sub edx, eax
    mov cl, dl
    jmp L_06df5
L_06d81:
    push eax
    sub eax, dword ptr [XMIN_]
    neg eax
    cmp edx, eax
    jle L_06d6a
    and eax, 0xfffc
    sub edx, eax
    mov cl, dl
    mov edx, eax
    pop eax
    add eax, edx
    add dword ptr [esp + 4], edx
    add esi, edx
    shr cl, 2
    jmp L_06d52
L_06da7:
    cmp ebx, dword ptr [YMAX_]
    jae L_06d6b
    mov edx, dword ptr [YMAX_]
    sub edx, ebx
    mov ch, dl
    jmp L_06d68
L_06dbb:
    add edx, ebx
    cmp edx, dword ptr [YMIN_]
    jle L_06d6b
    sub ebx, dword ptr [YMIN_]
    movzx edx, ch
    neg ebx
    and ebx, 0xffff
    sub edx, ebx
    mov ch, dl
    push eax
    mov eax, dword ptr [diff_saved_sprite_width]
    mul bx
    add esi, eax
    add dword ptr [esp + 8], eax
    pop eax
    mov ebx, dword ptr [YMIN_]
    jmp L_06d5e
L_06df5:
    xor edx, edx
    mov dl, cl
    shr cl, 2
    and edx, 3
    je L_06d52
    inc cl
    jmp L_06d52
; DOS 06e0c: Unreferenced DOS RET byte in the original span.
L_06e0c:
    ret

; DOS 1:06e0d
DrawSpriteDiffNormal_:
    sub esp, 0xc
    mov dword ptr [esp], edx
    call asm_06d33
    jb L_06e86
    cmp cl, 0
    jbe L_06e86
    mov edx, dword ptr [ebx*4 + row_offsets_80_01e20]
    lea edx, [edx*4]
    add edi, edx
    xor edx, edx
    mov dl, ch
    mov ebx, dword ptr [edx*4 + row_offsets_80_01e20]
    lea ebx, [ebx*4]
    add ebx, edi
    mov dword ptr [esp + 4], ebx
    add edi, eax
    mov edx, dword ptr [esp]
    and ecx, 0xff
    push ebp
    xor ebx, ebx
L_06e55:
    xor ebp, ebp
L_06e57:
    mov eax, dword ptr [esi + ebp*4]
    cmp eax, 0
    je L_06e68
    mov ebx, dword ptr [edx + ebp*4]
    and dword ptr [edi + ebp*4], ebx
    or dword ptr [edi + ebp*4], eax
L_06e68:
    inc ebp
    cmp ebp, ecx
    jb L_06e57
    add edi, 0x140
    add esi, dword ptr [diff_saved_sprite_width]
    add edx, dword ptr [diff_saved_sprite_width]
    cmp edi, dword ptr [esp + 8]
    jb L_06e55
    pop ebp
L_06e86:
    add esp, 0xc
    ret
; DOS 1:06e8a
synchro_:
    push edx
    push eax
    mov dx, 0x3da
L_06e90:
    in al, dx
    test ax, 8
    je L_06e90
    pop eax
    pop edx
    ret
; DOS 1:06e9a
endsynchro_:
    push edx
    push eax
    mov dx, 0x3da
L_06ea0:
    in al, dx
    test ax, 8
    jne L_06ea0
    pop eax
    pop edx
    ret
; DOS 1:06eaa
set_speaker_on_:
    mov dword ptr [lowlevel_timer_02158], 0
    mov dword ptr [lowlevel_timer_02154], 0
    mov dword ptr [lowlevel_timer_02150], 0
    push eax
    in al, 0x61
    or al, 3
    out 0x61, al
    mov dword ptr [lowlevel_timer_02148], 0xffff
    mov al, 0xb4
    out 0x43, al
    mov eax, dword ptr [lowlevel_timer_02148]
    out 0x42, al
    mov al, ah
    out 0x42, al
    pop eax
    ret
; DOS 1:06eea
set_speaker_off_:
    push eax
    in al, 0x61
    and al, 0xfc
    out 0x61, al
    mov al, 0xb4
    out 0x43, al
    mov al, 0
    out 0x42, al
    out 0x42, al
    pop eax
    ret
; DOS 1:06efd
clock_ticks_:
    push ebx
    push edx
    xor edx, edx
    mov eax, 0x80
    out 0x43, al
    nop
    in al, 0x42
    mov ah, al
    in al, 0x42
    xchg ah, al
    mov ebx, dword ptr [lowlevel_timer_02148]
    sub ebx, eax
    mov eax, ebx
    cmp eax, dword ptr [lowlevel_timer_02150]
    jg L_06f49
    jne L_06f3d
    mov edx, dword ptr [lowlevel_timer_02158]
    inc edx
    cmp edx, 0x14
    jle L_06f3d
    mov dword ptr [lowlevel_timer_02148], 0
    xor edx, edx
L_06f3d:
    mov ebx, dword ptr [lowlevel_timer_02148]
    add dword ptr [lowlevel_timer_02154], ebx
L_06f49:
    mov dword ptr [lowlevel_timer_02150], eax
    add eax, dword ptr [lowlevel_timer_02154]
    mov dword ptr [lowlevel_timer_02158], edx
    pop edx
    pop ebx
    ret
; DOS 1:06f5d
wait_until_:
    mov ecx, 1
    cmp dword ptr [lowlevel_timer_02148], 0
    je L_06f77
    dec ecx
    dec ecx
L_06f6d:
    call clock_ticks_
    inc ecx
    cmp eax, ebx
    jle L_06f6d
L_06f77:
    ret
; DOS 1:06f78
speaker_enable_:
    call set_speaker_on_
    ret
_TEXT ends
end
