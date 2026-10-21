; DOS v1.21 object 1, 1:04430..1:04ab7. Readable reconstruction of the
; address-anchored handwritten assembly scope. The runtime callback pointers
; loaded through EBP are deliberately not declared as static relocations.
.386p
_TEXT segment para public use32 'CODE'
assume ds:_TEXT

extrn _TableImageX:dword
extrn _saved_vga_registers_0196a:byte

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
    mov byte ptr [_saved_vga_registers_0196a], al
    mov dx, 03ceh
    mov al, 6
    out dx, al
    inc dx
    in al, dx
    mov byte ptr [_saved_vga_registers_0196a+1], al
    mov dx, 03c4h
    mov al, 4
    out dx, al
    inc dx
    in al, dx
    mov byte ptr [_saved_vga_registers_0196a+2], al
    mov dx, 03c4h
    mov al, 2
    out dx, al
    inc dx
    in al, dx
    mov byte ptr [_saved_vga_registers_0196a+3], al
    mov dx, 03d4h
    mov al, 14h
    out dx, al
    inc dx
    in al, dx
    mov byte ptr [_saved_vga_registers_0196a+4], al
    mov dx, 03d4h
    mov al, 17h
    out dx, al
    inc dx
    in al, dx
    mov byte ptr [_saved_vga_registers_0196a+5], al
    popad
    ret

InitModeNormal_:
    pushad
    mov dx, 03ceh
    mov al, 5
    out dx, al
    inc dx
    mov al, byte ptr [_saved_vga_registers_0196a]
    out dx, al
    dec dx
    mov al, 6
    out dx, al
    inc dx
    mov al, byte ptr [_saved_vga_registers_0196a+1]
    out dx, al
    mov dx, 03c4h
    mov al, 4
    out dx, al
    inc dx
    mov al, byte ptr [_saved_vga_registers_0196a+2]
    out dx, al
    mov dx, 03c4h
    mov al, 2
    out dx, al
    inc dx
    mov al, byte ptr [_saved_vga_registers_0196a+3]
    out dx, al
    mov dx, 03d4h
    mov al, 14h
    out dx, al
    inc dx
    mov al, byte ptr [_saved_vga_registers_0196a+4]
    out dx, al
    dec dx
    mov al, 17h
    out dx, al
    inc dx
    mov al, byte ptr [_saved_vga_registers_0196a+5]
    out dx, al
    popad
    ret

InitModeX_:
    pushad
    mov dx, 03ceh
    mov al, 5
    out dx, al
    inc dx
    mov al, byte ptr [_saved_vga_registers_0196a]
    and al, 0efh
    out dx, al
    dec dx
    mov al, 6
    out dx, al
    inc dx
    mov al, byte ptr [_saved_vga_registers_0196a+1]
    and al, 0fdh
    out dx, al
    mov dx, 03c4h
    mov al, 4
    out dx, al
    inc dx
    mov al, byte ptr [_saved_vga_registers_0196a+2]
    and al, 0f7h
    or al, 4
    out dx, al
    mov dx, 03c4h
    mov al, 2
    out dx, al
    inc dx
    mov al, byte ptr [_saved_vga_registers_0196a+3]
    out dx, al
    mov dx, 03d4h
    mov al, 14h
    out dx, al
    inc dx
    mov al, byte ptr [_saved_vga_registers_0196a+4]
    and al, 0bfh
    out dx, al
    dec dx
    mov al, 17h
    out dx, al
    inc dx
    mov al, byte ptr [_saved_vga_registers_0196a+5]
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
end
