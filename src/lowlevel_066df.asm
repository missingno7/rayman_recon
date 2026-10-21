; DOS v1.21 low-level assembly slice 1:066df..1:06f7e.
; Typed state and all explicit instructions are kept at their oracle offsets.
.386p
_TEXT segment para public use32 'CODE'

extrn clip_sprite_on_screen_:near
extrn clip_sprite_on_screen_flipped_:near
extrn saved_sprite_color_:dword
extrn saved_sprite_width_:dword
extrn row_offsets_80_01e20_:dword
extrn lowlevel_timer_02148_:dword
extrn lowlevel_timer_02150_:dword
extrn lowlevel_timer_02154_:dword
extrn lowlevel_timer_02158_:dword
extrn XMIN_:dword
extrn XMAX_:dword
extrn YMIN_:dword
extrn YMAX_:dword
extrn mode_x_sprite_state_:dword

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
    mov dword ptr [saved_sprite_color_], edx
    call clip_sprite_on_screen_
    jb L_06875
    push ebp
    mov edx, dword ptr [saved_sprite_width_]
    mov dword ptr [mode_x_sprite_state_], edx
    xor edx, edx
    add edi, dword ptr [ebx*4 + row_offsets_80_01e20_]
    mov dl, ch
    mov ebx, dword ptr [edx*4 + row_offsets_80_01e20_]
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
    mov eax, dword ptr [saved_sprite_color_]
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
    mov eax, dword ptr [saved_sprite_color_]
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
    mov eax, dword ptr [saved_sprite_color_]
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
    mov eax, dword ptr [saved_sprite_color_]
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
    mov dword ptr [saved_sprite_color_], edx
    call clip_sprite_on_screen_flipped_
    jb L_06a17
    push ebp
    mov edx, dword ptr [saved_sprite_width_]
    mov dword ptr [mode_x_sprite_state_], edx
    xor edx, edx
    add edi, dword ptr [ebx*4 + row_offsets_80_01e20_]
    mov dl, ch
    mov ebx, dword ptr [edx*4 + row_offsets_80_01e20_]
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
    mov eax, dword ptr [saved_sprite_color_]
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
    mov eax, dword ptr [saved_sprite_color_]
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
    mov eax, dword ptr [saved_sprite_color_]
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
    mov eax, dword ptr [saved_sprite_color_]
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
    mov dword ptr [saved_sprite_width_], edx
    jmp L_06a30
; DOS 1:06a24
DrawSpriteNormal_:
    call clip_sprite_on_screen_
    jb L_06a9c
    cmp cl, 0
    jbe L_06a9c
L_06a30:
    mov edx, dword ptr [ebx*4 + row_offsets_80_01e20_]
    lea edx, [edx*4]
    add edi, edx
    xor edx, edx
    mov dl, ch
    mov ebx, dword ptr [edx*4 + row_offsets_80_01e20_]
    lea ebx, [ebx*4]
    add ebx, edi
    add edi, eax
    and ecx, 0xff
    push ebp
    mov edx, dword ptr [saved_sprite_width_]
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
    mov edx, dword ptr [ebx*4 + row_offsets_80_01e20_]
    lea edx, [edx*4]
    add edi, edx
    xor edx, edx
    mov dl, ch
    mov ebx, dword ptr [edx*4 + row_offsets_80_01e20_]
    lea ebx, [ebx*4]
    add ebx, edi
    add edi, eax
    and ecx, 0xff
    push ebp
    mov edx, dword ptr [saved_sprite_width_]
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
    mov dword ptr [saved_sprite_width_], edx
    jmp L_06b14
; DOS 1:06b08
DrawSpriteFlipNormal_:
    call clip_sprite_on_screen_flipped_
    jb L_06b87
    cmp cl, 0
    jbe L_06b87
L_06b14:
    mov edx, dword ptr [ebx*4 + row_offsets_80_01e20_]
    lea edx, [edx*4]
    add edi, edx
    xor edx, edx
    mov dl, ch
    mov ebx, dword ptr [edx*4 + row_offsets_80_01e20_]
    lea ebx, [ebx*4]
    add ebx, edi
    add edi, eax
    and ecx, 0xff
    push ebp
    mov edx, dword ptr [saved_sprite_width_]
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
    mov edx, dword ptr [ebx*4 + row_offsets_80_01e20_]
    lea edx, [edx*4]
    add edi, edx
    xor edx, edx
    mov dl, ch
    mov ebx, dword ptr [edx*4 + row_offsets_80_01e20_]
    lea ebx, [ebx*4]
    add ebx, edi
    add edi, eax
    and ecx, 0xff
    push ebp
    mov edx, dword ptr [saved_sprite_width_]
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
    mov dword ptr [saved_sprite_color_], edx
    call clip_sprite_on_screen_
    jb L_06c88
    cmp cl, 0
    jbe near ptr L_06c88
    mov edx, dword ptr [ebx*4 + row_offsets_80_01e20_]
    lea edx, [edx*4]
    add edi, edx
    xor edx, edx
    mov dl, ch
    mov ebx, dword ptr [edx*4 + row_offsets_80_01e20_]
    lea ebx, [ebx*4]
    add ebx, edi
    add edi, eax
    and ecx, 0xff
    push ebp
    mov edx, dword ptr [saved_sprite_width_]
    mov ah, byte ptr [saved_sprite_color_]
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
    mov ah, byte ptr [saved_sprite_color_]
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
    mov dword ptr [saved_sprite_color_], edx
    call clip_sprite_on_screen_flipped_
    jb L_06d2e
    cmp cl, 0
    jbe L_06d2e
    mov edx, dword ptr [ebx*4 + row_offsets_80_01e20_]
    lea edx, [edx*4]
    add edi, edx
    xor edx, edx
    mov dl, ch
    mov ebx, dword ptr [edx*4 + row_offsets_80_01e20_]
    lea ebx, [ebx*4]
    add ebx, edi
    add edi, eax
    and ecx, 0xff
    push ebp
    mov edx, dword ptr [saved_sprite_width_]
    mov ah, byte ptr [saved_sprite_color_]
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
    mov ah, byte ptr [saved_sprite_color_]
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
    mov edx, dword ptr [ebx*4 + row_offsets_80_01e20_]
    lea edx, [edx*4]
    add edi, edx
    xor edx, edx
    mov dl, ch
    mov ebx, dword ptr [edx*4 + row_offsets_80_01e20_]
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
    mov dword ptr [lowlevel_timer_02158_], 0
    mov dword ptr [lowlevel_timer_02154_], 0
    mov dword ptr [lowlevel_timer_02150_], 0
    push eax
    in al, 0x61
    or al, 3
    out 0x61, al
    mov dword ptr [lowlevel_timer_02148_], 0xffff
    mov al, 0xb4
    out 0x43, al
    mov eax, dword ptr [lowlevel_timer_02148_]
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
    mov ebx, dword ptr [lowlevel_timer_02148_]
    sub ebx, eax
    mov eax, ebx
    cmp eax, dword ptr [lowlevel_timer_02150_]
    jg L_06f49
    jne L_06f3d
    mov edx, dword ptr [lowlevel_timer_02158_]
    inc edx
    cmp edx, 0x14
    jle L_06f3d
    mov dword ptr [lowlevel_timer_02148_], 0
    xor edx, edx
L_06f3d:
    mov ebx, dword ptr [lowlevel_timer_02148_]
    add dword ptr [lowlevel_timer_02154_], ebx
L_06f49:
    mov dword ptr [lowlevel_timer_02150_], eax
    add eax, dword ptr [lowlevel_timer_02154_]
    mov dword ptr [lowlevel_timer_02158_], edx
    pop edx
    pop ebx
    ret
; DOS 1:06f5d
wait_until_:
    mov ecx, 1
    cmp dword ptr [lowlevel_timer_02148_], 0
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
