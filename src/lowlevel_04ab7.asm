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
end

