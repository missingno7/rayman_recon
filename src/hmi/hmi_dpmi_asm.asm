; ORIGINAL_ASM: DOS v1.21 lock-delimited dpmi_asm group.
; Names INVENTED unless already canonical. Original assembler/version OPEN.
; Pinned WASM reproducer; whole CODE contribution begins at a public.
; NOP islands are observed original instructions, not invented padding.
.386p
.model flat
_TEXT segment para public use32 'CODE'
assume ds:FLAT

; Packed storage-field displacements; one state symbol at its true start.
BRIDGE_RECORD_TABLE equ 050h
BRIDGE_RECORD_SELECTOR equ 054h
BRIDGE_CALLBACK_TABLE equ 058h
BRIDGE_CALLBACK_SELECTOR equ 05ch
BRIDGE_MEMORY_INFO equ 060h
BRIDGE_REAL_REGS equ 09ch
BRIDGE_DETECT_AX equ 0ech
BRIDGE_DETECT_CL equ 0f0h
BRIDGE_DETECT_CH equ 0f4h
BRIDGE_DETECT_DX equ 0f8h

public hmi_dpmi_asm_begin_
public hmi_dpmi_lock_
public hmi_dpmi_unlock_
public hmi_dpmi_alloc_linear_selector_
public hmi_dpmi_free_linear_selector_
public hmi_dpmi_alias_code_selector_
public hmi_build_voice_slots_
public hmi_dpmi_alloc_dos_extra_
public hmi_dpmi_free_dos_wide_
public hmi_driver_op8_copy_
public hmi_driver_op8_copy_selectors_
public hmi_driver_op5_
public hmi_driver_op0_
public hmi_driver_op1_detect_results_
public hmi_driver_op2_copy_selectors_
public hmi_driver_op3_parameters_
public hmi_driver_op1_
public hmi_driver_op10_callbacks_
public hmi_driver_op0_parameters_
public hmi_driver_op4_parameters_
public hmi_driver_op2_parameter_
public hmi_driver_op3_bx8_
public hmi_windows_query_passthrough_
public hmi_windows_vds_lock_
public hmi_windows_vds_unlock_
public hmi_driver_op4_tables_
public hmi_driver_copy_record_
public hmi_driver_set_table_callback_
public hmi_driver_op11_op12_transfer_
public hmi_dpmi_largest_free_block_
public hmi_windows_query_
public hmi_dpmi_alloc_dos_
public hmi_dpmi_free_dos_
public hmi_vds_lock_real_interrupt_
public hmi_vds_unlock_real_interrupt_
public hmi_dpmi_map_physical_selector_
public hmi_dpmi_free_selector_
public hmi_get_ds_selector_
public hmi_get_cs_selector_
public hmi_dpmi_asm_end_

; 1:787b4: dpmi_asm_begin
hmi_dpmi_asm_begin_:
    ret ; 1:787b4

; 1:787b5: dpmi_lock
hmi_dpmi_lock_:
    push ebp ; 1:787b5
    mov ebp, esp ; 1:787b6
    push esi ; 1:787b8
    push edi ; 1:787b9
    push ebx ; 1:787ba
    push ecx ; 1:787bb
    push fs ; 1:787bc
    push gs ; 1:787be
    push es ; 1:787c0
    mov cx, word ptr [ebp + 8] ; 1:787c1
    mov bx, word ptr [ebp + 0ah] ; 1:787c5
    mov di, word ptr [ebp + 0ch] ; 1:787c9
    mov si, word ptr [ebp + 0eh] ; 1:787cd
    mov ax, 0600h ; 1:787d1
    int 031h ; 1:787d5
    mov eax, 0 ; 1:787d7
    jae short dpmi_asm_at_787e7 ; 1:787dc
    nop ; 1:787de
    nop ; 1:787df
    nop ; 1:787e0
    nop ; 1:787e1
    mov eax, 1 ; 1:787e2

; 1:787e7: internal control-flow target
dpmi_asm_at_787e7:
    pop es ; 1:787e7
    pop gs ; 1:787e8
    pop fs ; 1:787ea
    pop ecx ; 1:787ec
    pop ebx ; 1:787ed
    pop edi ; 1:787ee
    pop esi ; 1:787ef
    pop ebp ; 1:787f0
    ret ; 1:787f1

; 1:787f2: dpmi_unlock
hmi_dpmi_unlock_:
    push ebp ; 1:787f2
    mov ebp, esp ; 1:787f3
    push esi ; 1:787f5
    push edi ; 1:787f6
    push ebx ; 1:787f7
    push ecx ; 1:787f8
    push fs ; 1:787f9
    push gs ; 1:787fb
    push es ; 1:787fd
    mov cx, word ptr [ebp + 8] ; 1:787fe
    mov bx, word ptr [ebp + 0ah] ; 1:78802
    mov di, word ptr [ebp + 0ch] ; 1:78806
    mov si, word ptr [ebp + 0eh] ; 1:7880a
    mov ax, 0601h ; 1:7880e
    int 031h ; 1:78812
    mov eax, 0 ; 1:78814
    jae short dpmi_asm_at_78824 ; 1:78819
    nop ; 1:7881b
    nop ; 1:7881c
    nop ; 1:7881d
    nop ; 1:7881e
    mov eax, 1 ; 1:7881f

; 1:78824: internal control-flow target
dpmi_asm_at_78824:
    pop es ; 1:78824
    pop gs ; 1:78825
    pop fs ; 1:78827
    pop ecx ; 1:78829
    pop ebx ; 1:7882a
    pop edi ; 1:7882b
    pop esi ; 1:7882c
    pop ebp ; 1:7882d
    ret ; 1:7882e

; 1:7882f: dpmi_alloc_linear_selector
hmi_dpmi_alloc_linear_selector_:
    push ebp ; 1:7882f
    mov ebp, esp ; 1:78830
    push esi ; 1:78832
    push edi ; 1:78833
    push ebx ; 1:78834
    push ecx ; 1:78835
    push fs ; 1:78836
    push gs ; 1:78838
    push es ; 1:7883a
    mov ax, 0 ; 1:7883b
    mov cx, 1 ; 1:7883f
    int 031h ; 1:78843
    push eax ; 1:78845
    mov word ptr [ebp - 2], ax ; 1:78846
    push eax ; 1:7884a
    mov bx, ax ; 1:7884b
    mov ax, 8 ; 1:7884e
    xor cx, cx ; 1:78852
    mov edx, dword ptr [ebp + 8] ; 1:78855
    int 031h ; 1:78858
    pop eax ; 1:7885a
    mov ax, 0501h ; 1:7885b
    xor ebx, ebx ; 1:7885f
    mov ecx, dword ptr [ebp + 8] ; 1:78861
    add cx, 050h ; 1:78864
    int 031h ; 1:78868
    push ebx ; 1:7886a
    mov ebx, dword ptr [ebp + 0ch] ; 1:7886b
    mov word ptr [ebx], si ; 1:7886e
    mov word ptr [ebx + 2], di ; 1:78871
    pop ebx ; 1:78875
    pop eax ; 1:78876
    push eax ; 1:78877
    mov esi, dword ptr [ebp + 010h] ; 1:78878
    mov word ptr [esi], cx ; 1:7887b
    mov word ptr [esi + 2], bx ; 1:7887e
    mov dx, cx ; 1:78882
    mov cx, bx ; 1:78885
    mov bx, ax ; 1:78888
    mov ax, 7 ; 1:7888b
    int 031h ; 1:7888f
    movzx ecx, word ptr [ebp - 2] ; 1:78891
    lar ecx, ecx ; 1:78895
    shr ecx, 8 ; 1:78898
    or cx, 04000h ; 1:7889b
    mov bx, word ptr [ebp - 2] ; 1:788a0
    mov ax, 9 ; 1:788a4
    int 031h ; 1:788a8
    pop eax ; 1:788aa
    xor edx, edx ; 1:788ab
    mov dx, ax ; 1:788ad
    xor eax, eax ; 1:788b0
    pop es ; 1:788b2
    pop gs ; 1:788b3
    pop fs ; 1:788b5
    pop ecx ; 1:788b7
    pop ebx ; 1:788b8
    pop edi ; 1:788b9
    pop esi ; 1:788ba
    pop ebp ; 1:788bb
    ret ; 1:788bc

; 1:788bd: dpmi_free_linear_selector
hmi_dpmi_free_linear_selector_:
    push eax ; 1:788bd
    mov ax, ds ; 1:788be
    mov gs, ax ; 1:788c1
    pop eax ; 1:788c3
    push ebp ; 1:788c4
    mov ebp, esp ; 1:788c5
    push esi ; 1:788c7
    push edi ; 1:788c8
    push ebx ; 1:788c9
    push ecx ; 1:788ca
    push fs ; 1:788cb
    push gs ; 1:788cd
    push es ; 1:788cf
    mov si, word ptr [ebp + 010h] ; 1:788d0
    mov di, word ptr [ebp + 012h] ; 1:788d4
    mov ax, 0502h ; 1:788d8
    int 031h ; 1:788dc
    mov ax, 1 ; 1:788de
    mov bx, word ptr [ebp + 0ch] ; 1:788e2
    int 031h ; 1:788e6
    pop es ; 1:788e8
    pop gs ; 1:788e9
    pop fs ; 1:788eb
    pop ecx ; 1:788ed
    pop ebx ; 1:788ee
    pop edi ; 1:788ef
    pop esi ; 1:788f0
    pop ebp ; 1:788f1
    ret ; 1:788f2

; 1:788f3: dpmi_alias_code_selector
hmi_dpmi_alias_code_selector_:
    push ebp ; 1:788f3
    mov ebp, esp ; 1:788f4
    push esi ; 1:788f6
    push edi ; 1:788f7
    push ebx ; 1:788f8
    push ecx ; 1:788f9
    push fs ; 1:788fa
    push gs ; 1:788fc
    push es ; 1:788fe
    mov ax, 0 ; 1:788ff
    mov cx, 1 ; 1:78903
    int 031h ; 1:78907
    push eax ; 1:78909
    push eax ; 1:7890a
    push eax ; 1:7890b
    push eax ; 1:7890c
    mov bx, word ptr [ebp + 0ch] ; 1:7890d
    mov ax, 6 ; 1:78911
    int 031h ; 1:78915
    pop ebx ; 1:78917
    mov ax, 7 ; 1:78918
    int 031h ; 1:7891c
    xor ebx, ebx ; 1:7891e
    mov bx, word ptr [ebp + 0ch] ; 1:78920
    lsl edx, ebx ; 1:78924
    mov ax, 8 ; 1:78927
    pop ebx ; 1:7892b
    xor ecx, ecx ; 1:7892c
    int 031h ; 1:7892e
    mov ax, 9 ; 1:78930
    pop ebx ; 1:78934
    lar ecx, ebx ; 1:78935
    shr ecx, 8 ; 1:78938
    or cx, 04008h ; 1:7893b
    int 031h ; 1:78940
    pop edx ; 1:78942
    xor eax, eax ; 1:78943
    pop es ; 1:78945
    pop gs ; 1:78946
    pop fs ; 1:78948
    pop ecx ; 1:7894a
    pop ebx ; 1:7894b
    pop edi ; 1:7894c
    pop esi ; 1:7894d
    pop ebp ; 1:7894e
    ret ; 1:7894f

; 1:78950: build_voice_slots
hmi_build_voice_slots_:
    push ebp ; 1:78950
    mov ebp, esp ; 1:78951
    push esi ; 1:78953
    push edi ; 1:78954
    push ebx ; 1:78955
    push ecx ; 1:78956
    push fs ; 1:78957
    push gs ; 1:78959
    push es ; 1:7895b
    mov esi, dword ptr [ebp + 8] ; 1:7895c
    les edi, fword ptr [ebp + 0ch] ; 1:7895f
    mov ecx, 020h ; 1:78962

; 1:78967: internal control-flow target
dpmi_asm_at_78967:
    mov dword ptr [esi], edi ; 1:78967
    mov word ptr [esi + 4], es ; 1:78969
    add edi, 06ch ; 1:7896d
    add esi, 6 ; 1:78970
    loop dpmi_asm_at_78967 ; 1:78973
    pop es ; 1:78975
    pop gs ; 1:78976
    pop fs ; 1:78978
    pop ecx ; 1:7897a
    pop ebx ; 1:7897b
    pop edi ; 1:7897c
    pop esi ; 1:7897d
    pop ebp ; 1:7897e
    ret ; 1:7897f

; 1:78980: dpmi_alloc_dos_extra
hmi_dpmi_alloc_dos_extra_:
    push ebp ; 1:78980
    mov ebp, esp ; 1:78981
    push esi ; 1:78983
    push edi ; 1:78984
    push ebx ; 1:78985
    push ecx ; 1:78986
    push fs ; 1:78987
    push gs ; 1:78989
    push es ; 1:7898b
    xor eax, eax ; 1:7898c
    mov ax, 0100h ; 1:7898e
    mov ebx, dword ptr [ebp + 8] ; 1:78992
    add ebx, 0100h ; 1:78995
    int 031h ; 1:7899b
    mov esi, dword ptr [ebp + 0ch] ; 1:7899d
    shl eax, 4 ; 1:789a0
    mov dword ptr [esi], eax ; 1:789a3
    mov esi, dword ptr [ebp + 010h] ; 1:789a5
    mov word ptr [esi], dx ; 1:789a8
    pop es ; 1:789ab
    pop gs ; 1:789ac
    pop fs ; 1:789ae
    pop ecx ; 1:789b0
    pop ebx ; 1:789b1
    pop edi ; 1:789b2
    pop esi ; 1:789b3
    pop ebp ; 1:789b4
    ret ; 1:789b5

; 1:789b6: dpmi_free_dos_wide
hmi_dpmi_free_dos_wide_:
    mov ax, ds ; 1:789b6
    mov gs, ax ; 1:789b9
    mov es, ax ; 1:789bb
    mov fs, ax ; 1:789bd
    push ebp ; 1:789bf
    mov ebp, esp ; 1:789c0
    push esi ; 1:789c2
    push edi ; 1:789c3
    push ebx ; 1:789c4
    push ecx ; 1:789c5
    push fs ; 1:789c6
    push gs ; 1:789c8
    push es ; 1:789ca
    xor eax, eax ; 1:789cb
    mov ax, 0101h ; 1:789cd
    mov edx, dword ptr [ebp + 8] ; 1:789d1
    int 031h ; 1:789d4
    pop es ; 1:789d6
    pop gs ; 1:789d7
    pop fs ; 1:789d9
    pop ecx ; 1:789db
    pop ebx ; 1:789dc
    pop edi ; 1:789dd
    pop esi ; 1:789de
    pop ebp ; 1:789df
    ret ; 1:789e0

; 1:789e1: driver_op8_copy
hmi_driver_op8_copy_:
    push ebp ; 1:789e1
    mov ebp, esp ; 1:789e2
    push esi ; 1:789e4
    push edi ; 1:789e5
    push ebx ; 1:789e6
    push ecx ; 1:789e7
    push fs ; 1:789e8
    push gs ; 1:789ea
    push es ; 1:789ec
    mov eax, 8 ; 1:789ed
    lfs edi, fword ptr [ebp + 010h] ; 1:789f2
    call fword ptr [ebp + 8] ; 1:789f6
    push ds ; 1:789f9
    les edi, fword ptr [ebp + 018h] ; 1:789fa
    lds esi, fword ptr [ebp + 010h] ; 1:789fd
    mov esi, edx ; 1:78a00
    mov ecx, 06ah ; 1:78a02
    cld ; 1:78a07
    rep movsb ; 1:78a08
    pop ds ; 1:78a0a
    pop es ; 1:78a0b
    pop gs ; 1:78a0c
    pop fs ; 1:78a0e
    pop ecx ; 1:78a10
    pop ebx ; 1:78a11
    pop edi ; 1:78a12
    pop esi ; 1:78a13
    pop ebp ; 1:78a14
    ret ; 1:78a15

; 1:78a16: driver_op8_copy_selectors
hmi_driver_op8_copy_selectors_:
    push ebp ; 1:78a16
    mov ebp, esp ; 1:78a17
    push esi ; 1:78a19
    push edi ; 1:78a1a
    push ebx ; 1:78a1b
    push ecx ; 1:78a1c
    push fs ; 1:78a1d
    push gs ; 1:78a1f
    push es ; 1:78a21
    mov eax, 8 ; 1:78a22
    lfs edi, fword ptr [ebp + 010h] ; 1:78a27
    call fword ptr [ebp + 8] ; 1:78a2b
    push ds ; 1:78a2e
    push es ; 1:78a2f
    les edi, fword ptr [ebp + 018h] ; 1:78a30
    lds esi, fword ptr [ebp + 010h] ; 1:78a33
    mov esi, edx ; 1:78a36
    mov ecx, 06ah ; 1:78a38
    cld ; 1:78a3d
    rep movsb ; 1:78a3e
    les edi, fword ptr [ebp + 018h] ; 1:78a40
    mov word ptr es:[edi + 044h], ds ; 1:78a43
    mov word ptr es:[edi + 04ch], ds ; 1:78a48
    mov word ptr es:[edi + 054h], ds ; 1:78a4d
    mov word ptr es:[edi + 05ch], ds ; 1:78a52
    pop es ; 1:78a57
    pop ds ; 1:78a58
    pop es ; 1:78a59
    pop gs ; 1:78a5a
    pop fs ; 1:78a5c
    pop ecx ; 1:78a5e
    pop ebx ; 1:78a5f
    pop edi ; 1:78a60
    pop esi ; 1:78a61
    pop ebp ; 1:78a62
    ret ; 1:78a63

; 1:78a64: driver_op5
hmi_driver_op5_:
    push ebp ; 1:78a64
    mov ebp, esp ; 1:78a65
    push esi ; 1:78a67
    push edi ; 1:78a68
    push ebx ; 1:78a69
    push ecx ; 1:78a6a
    push fs ; 1:78a6b
    push gs ; 1:78a6d
    push es ; 1:78a6f
    mov eax, 5 ; 1:78a70
    lfs edi, fword ptr [ebp + 010h] ; 1:78a75
    call fword ptr [ebp + 8] ; 1:78a79
    pop es ; 1:78a7c
    pop gs ; 1:78a7d
    pop fs ; 1:78a7f
    pop ecx ; 1:78a81
    pop ebx ; 1:78a82
    pop edi ; 1:78a83
    pop esi ; 1:78a84
    pop ebp ; 1:78a85
    ret ; 1:78a86

; 1:78a87: driver_op0
hmi_driver_op0_:
    push ebp ; 1:78a87
    mov ebp, esp ; 1:78a88
    push esi ; 1:78a8a
    push edi ; 1:78a8b
    push ebx ; 1:78a8c
    push ecx ; 1:78a8d
    push fs ; 1:78a8e
    push gs ; 1:78a90
    push es ; 1:78a92
    mov eax, 0 ; 1:78a93
    lfs edi, fword ptr [ebp + 010h] ; 1:78a98
    call fword ptr [ebp + 8] ; 1:78a9c
    pop es ; 1:78a9f
    pop gs ; 1:78aa0
    pop fs ; 1:78aa2
    pop ecx ; 1:78aa4
    pop ebx ; 1:78aa5
    pop edi ; 1:78aa6
    pop esi ; 1:78aa7
    pop ebp ; 1:78aa8
    ret ; 1:78aa9

; 1:78aaa: driver_op1_detect_results
hmi_driver_op1_detect_results_:
    push ebp ; 1:78aaa
    mov ebp, esp ; 1:78aab
    push esi ; 1:78aad
    push edi ; 1:78aae
    push ebx ; 1:78aaf
    push ecx ; 1:78ab0
    push fs ; 1:78ab1
    push gs ; 1:78ab3
    push es ; 1:78ab5
    mov eax, 1 ; 1:78ab6
    lfs edi, fword ptr [ebp + 010h] ; 1:78abb
    call fword ptr [ebp + 8] ; 1:78abf
    mov word ptr [_hmi_dpmi_asm_state + BRIDGE_DETECT_AX], ax ; 1:78ac2
    mov al, ch ; 1:78ac8
    cbw ; 1:78aca
    mov word ptr [_hmi_dpmi_asm_state + BRIDGE_DETECT_CH], ax ; 1:78acc
    mov al, cl ; 1:78ad2
    cbw ; 1:78ad4
    mov word ptr [_hmi_dpmi_asm_state + BRIDGE_DETECT_CL], ax ; 1:78ad6
    mov word ptr [_hmi_dpmi_asm_state + BRIDGE_DETECT_DX], dx ; 1:78adc
    pop es ; 1:78ae3
    pop gs ; 1:78ae4
    pop fs ; 1:78ae6
    pop ecx ; 1:78ae8
    pop ebx ; 1:78ae9
    pop edi ; 1:78aea
    pop esi ; 1:78aeb
    pop ebp ; 1:78aec
    ret ; 1:78aed

; 1:78aee: driver_op2_copy_selectors
hmi_driver_op2_copy_selectors_:
    push ebp ; 1:78aee
    mov ebp, esp ; 1:78aef
    push esi ; 1:78af1
    push edi ; 1:78af2
    push ebx ; 1:78af3
    push ecx ; 1:78af4
    push fs ; 1:78af5
    push gs ; 1:78af7
    push es ; 1:78af9
    push es ; 1:78afa
    mov eax, 2 ; 1:78afb
    lfs edi, fword ptr [ebp + 010h] ; 1:78b00
    call fword ptr [ebp + 8] ; 1:78b04
    push ds ; 1:78b07
    les edi, fword ptr [ebp + 018h] ; 1:78b08
    lds esi, fword ptr [ebp + 010h] ; 1:78b0b
    mov esi, edx ; 1:78b0e
    mov ecx, 06ah ; 1:78b10
    cld ; 1:78b15
    rep movsb ; 1:78b16
    les edi, fword ptr [ebp + 018h] ; 1:78b18
    mov word ptr es:[edi + 044h], ds ; 1:78b1b
    mov word ptr es:[edi + 04ch], ds ; 1:78b20
    mov word ptr es:[edi + 054h], ds ; 1:78b25
    mov word ptr es:[edi + 05ch], ds ; 1:78b2a
    pop ds ; 1:78b2f
    pop es ; 1:78b30
    pop es ; 1:78b31
    pop gs ; 1:78b32
    pop fs ; 1:78b34
    pop ecx ; 1:78b36
    pop ebx ; 1:78b37
    pop edi ; 1:78b38
    pop esi ; 1:78b39
    pop ebp ; 1:78b3a
    ret ; 1:78b3b

; 1:78b3c: driver_op3_parameters
hmi_driver_op3_parameters_:
    push ebp ; 1:78b3c
    mov ebp, esp ; 1:78b3d
    push esi ; 1:78b3f
    push edi ; 1:78b40
    push ebx ; 1:78b41
    push ecx ; 1:78b42
    push fs ; 1:78b43
    push gs ; 1:78b45
    push es ; 1:78b47
    mov ebx, dword ptr [ebp + 010h] ; 1:78b48
    mov eax, dword ptr [ebp + 014h] ; 1:78b4b
    mov ch, al ; 1:78b4e
    mov eax, dword ptr [ebp + 018h] ; 1:78b50
    mov cl, al ; 1:78b53
    mov eax, 3 ; 1:78b55
    lfs edi, fword ptr [ebp + 01ch] ; 1:78b5a
    call fword ptr [ebp + 8] ; 1:78b5e
    pop es ; 1:78b61
    pop gs ; 1:78b62
    pop fs ; 1:78b64
    pop ecx ; 1:78b66
    pop ebx ; 1:78b67
    pop edi ; 1:78b68
    pop esi ; 1:78b69
    pop ebp ; 1:78b6a
    ret ; 1:78b6b

; 1:78b6c: driver_op1
hmi_driver_op1_:
    push ebp ; 1:78b6c
    mov ebp, esp ; 1:78b6d
    push esi ; 1:78b6f
    push edi ; 1:78b70
    push ebx ; 1:78b71
    push ecx ; 1:78b72
    push fs ; 1:78b73
    push gs ; 1:78b75
    push es ; 1:78b77
    mov eax, 1 ; 1:78b78
    lfs edi, fword ptr [ebp + 010h] ; 1:78b7d
    call fword ptr [ebp + 8] ; 1:78b81
    pop es ; 1:78b84
    pop gs ; 1:78b85
    pop fs ; 1:78b87
    pop ecx ; 1:78b89
    pop ebx ; 1:78b8a
    pop edi ; 1:78b8b
    pop esi ; 1:78b8c
    pop ebp ; 1:78b8d
    ret ; 1:78b8e

; 1:78b8f: driver_op10_callbacks
hmi_driver_op10_callbacks_:
    push ebp ; 1:78b8f
    mov ebp, esp ; 1:78b90
    push esi ; 1:78b92
    push edi ; 1:78b93
    push ebx ; 1:78b94
    push ecx ; 1:78b95
    push fs ; 1:78b96
    push gs ; 1:78b98
    push es ; 1:78b9a
    lfs edi, fword ptr [ebp + 010h] ; 1:78b9b
    mov eax, 0ah ; 1:78b9f
    mov ecx, dword ptr [ebp + 020h] ; 1:78ba4
    les edi, fword ptr [ebp + 018h] ; 1:78ba7
    mov edx, dword ptr [ebp + 024h] ; 1:78baa
    mov ebx, dword ptr [ebp + 028h] ; 1:78bad
    push ds ; 1:78bb0
    pop gs ; 1:78bb1
    call fword ptr [ebp + 8] ; 1:78bb3
    cli ; 1:78bb6
    mov ebx, dword ptr [ebp + 02ch] ; 1:78bb7
    mov dword ptr [ebx], edi ; 1:78bba
    mov dword ptr [ebx + 8], ecx ; 1:78bbc
    mov dword ptr [ebx + 010h], esi ; 1:78bbf
    mov ax, word ptr [ebp + 0ch] ; 1:78bc2
    mov word ptr [ebx + 4], ax ; 1:78bc6
    mov ax, word ptr [ebp + 014h] ; 1:78bca
    mov word ptr [ebx + 0ch], ax ; 1:78bce
    mov word ptr [ebx + 014h], ax ; 1:78bd2
    sti ; 1:78bd6
    pop es ; 1:78bd7
    pop gs ; 1:78bd8
    pop fs ; 1:78bda
    pop ecx ; 1:78bdc
    pop ebx ; 1:78bdd
    pop edi ; 1:78bde
    pop esi ; 1:78bdf
    pop ebp ; 1:78be0
    ret ; 1:78be1

; 1:78be2: driver_op0_parameters
hmi_driver_op0_parameters_:
    push ebp ; 1:78be2
    mov ebp, esp ; 1:78be3
    push esi ; 1:78be5
    push edi ; 1:78be6
    push ebx ; 1:78be7
    push ecx ; 1:78be8
    push fs ; 1:78be9
    push gs ; 1:78beb
    push es ; 1:78bed
    lfs edi, fword ptr [ebp + 010h] ; 1:78bee
    mov eax, 0 ; 1:78bf2
    mov bx, word ptr [ebp + 018h] ; 1:78bf7
    mov cx, word ptr [ebp + 01ch] ; 1:78bfb
    mov dx, word ptr [ebp + 020h] ; 1:78bff
    mov ch, dl ; 1:78c03
    mov si, word ptr [ebp + 02ch] ; 1:78c05
    call fword ptr [ebp + 8] ; 1:78c09
    pop es ; 1:78c0c
    pop gs ; 1:78c0d
    pop fs ; 1:78c0f
    pop ecx ; 1:78c11
    pop ebx ; 1:78c12
    pop edi ; 1:78c13
    pop esi ; 1:78c14
    pop ebp ; 1:78c15
    ret ; 1:78c16

; 1:78c17: driver_op4_parameters
hmi_driver_op4_parameters_:
    push ebp ; 1:78c17
    mov ebp, esp ; 1:78c18
    push esi ; 1:78c1a
    push edi ; 1:78c1b
    push ebx ; 1:78c1c
    push ecx ; 1:78c1d
    push fs ; 1:78c1e
    push gs ; 1:78c20
    push es ; 1:78c22
    lfs edi, fword ptr [ebp + 010h] ; 1:78c23
    mov eax, 4 ; 1:78c27
    mov ecx, dword ptr [ebp + 01ch] ; 1:78c2c
    mov edi, dword ptr [ebp + 018h] ; 1:78c2f
    call fword ptr [ebp + 8] ; 1:78c32
    pop es ; 1:78c35
    pop gs ; 1:78c36
    pop fs ; 1:78c38
    pop ecx ; 1:78c3a
    pop ebx ; 1:78c3b
    pop edi ; 1:78c3c
    pop esi ; 1:78c3d
    pop ebp ; 1:78c3e
    ret ; 1:78c3f

; 1:78c40: driver_op2_parameter
hmi_driver_op2_parameter_:
    push ebp ; 1:78c40
    mov ebp, esp ; 1:78c41
    push esi ; 1:78c43
    push edi ; 1:78c44
    push ebx ; 1:78c45
    push ecx ; 1:78c46
    push fs ; 1:78c47
    push gs ; 1:78c49
    push es ; 1:78c4b
    lfs edi, fword ptr [ebp + 010h] ; 1:78c4c
    mov eax, 2 ; 1:78c50
    mov ebx, dword ptr [ebp + 018h] ; 1:78c55
    call fword ptr [ebp + 8] ; 1:78c58
    pop es ; 1:78c5b
    pop gs ; 1:78c5c
    pop fs ; 1:78c5e
    pop ecx ; 1:78c60
    pop ebx ; 1:78c61
    pop edi ; 1:78c62
    pop esi ; 1:78c63
    pop ebp ; 1:78c64
    ret ; 1:78c65

; 1:78c66: driver_op3_bx8
hmi_driver_op3_bx8_:
    push ebp ; 1:78c66
    mov ebp, esp ; 1:78c67
    push esi ; 1:78c69
    push edi ; 1:78c6a
    push ebx ; 1:78c6b
    push ecx ; 1:78c6c
    push fs ; 1:78c6d
    push gs ; 1:78c6f
    push es ; 1:78c71
    lfs edi, fword ptr [ebp + 010h] ; 1:78c72
    mov eax, 3 ; 1:78c76
    mov bx, 8 ; 1:78c7b
    call fword ptr [ebp + 8] ; 1:78c7f
    pop es ; 1:78c82
    pop gs ; 1:78c83
    pop fs ; 1:78c85
    pop ecx ; 1:78c87
    pop ebx ; 1:78c88
    pop edi ; 1:78c89
    pop esi ; 1:78c8a
    pop ebp ; 1:78c8b
    ret ; 1:78c8c

; 1:78c8d: windows_query_passthrough
hmi_windows_query_passthrough_:
    push ebp ; 1:78c8d
    mov ebp, esp ; 1:78c8e
    push esi ; 1:78c90
    push edi ; 1:78c91
    push ebx ; 1:78c92
    push ecx ; 1:78c93
    push fs ; 1:78c94
    push gs ; 1:78c96
    push es ; 1:78c98
    mov ax, 01600h ; 1:78c99
    int 02fh ; 1:78c9d
    cmp ax, 0a03h ; 1:78c9f
    mov eax, dword ptr [ebp + 014h] ; 1:78ca3
    jne short dpmi_asm_at_78cac ; 1:78ca6
    nop ; 1:78ca8
    nop ; 1:78ca9
    nop ; 1:78caa
    nop ; 1:78cab

; 1:78cac: internal control-flow target
dpmi_asm_at_78cac:
    pop es ; 1:78cac
    pop gs ; 1:78cad
    pop fs ; 1:78caf
    pop ecx ; 1:78cb1
    pop ebx ; 1:78cb2
    pop edi ; 1:78cb3
    pop esi ; 1:78cb4
    pop ebp ; 1:78cb5
    ret ; 1:78cb6

; 1:78cb7: windows_vds_lock
hmi_windows_vds_lock_:
    push ebp ; 1:78cb7
    mov ebp, esp ; 1:78cb8
    push esi ; 1:78cba
    push edi ; 1:78cbb
    push ebx ; 1:78cbc
    push ecx ; 1:78cbd
    push fs ; 1:78cbe
    push gs ; 1:78cc0
    push es ; 1:78cc2
    mov ax, 01600h ; 1:78cc3
    int 02fh ; 1:78cc7
    cmp al, 3 ; 1:78cc9
    jne short dpmi_asm_at_78ce1 ; 1:78ccb
    nop ; 1:78ccd
    nop ; 1:78cce
    nop ; 1:78ccf
    nop ; 1:78cd0
    mov ah, 081h ; 1:78cd1
    mov al, 0bh ; 1:78cd3
    mov ebx, dword ptr [ebp + 8] ; 1:78cd5
    xor dx, dx ; 1:78cd8
    int 04bh ; 1:78cdb
    mov ax, 0 ; 1:78cdd

; 1:78ce1: internal control-flow target
dpmi_asm_at_78ce1:
    pop es ; 1:78ce1
    pop gs ; 1:78ce2
    pop fs ; 1:78ce4
    pop ecx ; 1:78ce6
    pop ebx ; 1:78ce7
    pop edi ; 1:78ce8
    pop esi ; 1:78ce9
    pop ebp ; 1:78cea
    ret ; 1:78ceb

; 1:78cec: windows_vds_unlock
hmi_windows_vds_unlock_:
    push ebp ; 1:78cec
    mov ebp, esp ; 1:78ced
    push esi ; 1:78cef
    push edi ; 1:78cf0
    push ebx ; 1:78cf1
    push ecx ; 1:78cf2
    push fs ; 1:78cf3
    push gs ; 1:78cf5
    push es ; 1:78cf7
    mov ax, 01600h ; 1:78cf8
    int 02fh ; 1:78cfc
    cmp al, 3 ; 1:78cfe
    jne short dpmi_asm_at_78d16 ; 1:78d00
    nop ; 1:78d02
    nop ; 1:78d03
    nop ; 1:78d04
    nop ; 1:78d05
    mov ah, 081h ; 1:78d06
    mov al, 0ch ; 1:78d08
    mov ebx, dword ptr [ebp + 8] ; 1:78d0a
    xor dx, dx ; 1:78d0d
    int 04bh ; 1:78d10
    mov ax, 0 ; 1:78d12

; 1:78d16: internal control-flow target
dpmi_asm_at_78d16:
    pop es ; 1:78d16
    pop gs ; 1:78d17
    pop fs ; 1:78d19
    pop ecx ; 1:78d1b
    pop ebx ; 1:78d1c
    pop edi ; 1:78d1d
    pop esi ; 1:78d1e
    pop ebp ; 1:78d1f
    ret ; 1:78d20

; 1:78d21: driver_op4_tables
hmi_driver_op4_tables_:
    push ebp ; 1:78d21
    mov ebp, esp ; 1:78d22
    push esi ; 1:78d24
    push edi ; 1:78d25
    push ebx ; 1:78d26
    push ecx ; 1:78d27
    push fs ; 1:78d28
    push gs ; 1:78d2a
    push es ; 1:78d2c
    lfs edi, fword ptr [ebp + 010h] ; 1:78d2d
    mov eax, 4 ; 1:78d31
    call fword ptr [ebp + 8] ; 1:78d36
    mov dword ptr [_hmi_dpmi_asm_state + BRIDGE_RECORD_TABLE], esi ; 1:78d39
    mov dword ptr [_hmi_dpmi_asm_state + BRIDGE_CALLBACK_TABLE], edi ; 1:78d3f
    mov ax, word ptr [ebp + 014h] ; 1:78d45
    mov word ptr [_hmi_dpmi_asm_state + BRIDGE_RECORD_SELECTOR], ax ; 1:78d49
    mov word ptr [_hmi_dpmi_asm_state + BRIDGE_CALLBACK_SELECTOR], ax ; 1:78d4f
    pop es ; 1:78d55
    pop gs ; 1:78d56
    pop fs ; 1:78d58
    pop ecx ; 1:78d5a
    pop ebx ; 1:78d5b
    pop edi ; 1:78d5c
    pop esi ; 1:78d5d
    pop ebp ; 1:78d5e
    ret ; 1:78d5f

; 1:78d60: driver_copy_record
hmi_driver_copy_record_:
    push ebp ; 1:78d60
    mov ebp, esp ; 1:78d61
    push esi ; 1:78d63
    push edi ; 1:78d64
    push ebx ; 1:78d65
    push ecx ; 1:78d66
    push fs ; 1:78d67
    push gs ; 1:78d69
    push es ; 1:78d6b
    push ds ; 1:78d6c
    push ds ; 1:78d6d
    pop es ; 1:78d6e
    mov edi, offset _hmi_dpmi_asm_state ; 1:78d6f
    mov ebx, dword ptr [ebp + 8] ; 1:78d74
    lds esi, fword ptr [_hmi_dpmi_asm_state + BRIDGE_RECORD_TABLE] ; 1:78d77
    mov esi, dword ptr [esi + ebx*4] ; 1:78d7d
    cmp esi, -1 ; 1:78d80
    jne short dpmi_asm_at_78d91 ; 1:78d83
    nop ; 1:78d85
    nop ; 1:78d86
    nop ; 1:78d87
    nop ; 1:78d88
    xor eax, eax ; 1:78d89
    pop ds ; 1:78d8b
    jmp short dpmi_asm_at_78d9f ; 1:78d8c
    nop ; 1:78d8e
    nop ; 1:78d8f
    nop ; 1:78d90

; 1:78d91: internal control-flow target
dpmi_asm_at_78d91:
    mov ecx, 050h ; 1:78d91
    cld ; 1:78d96
    repne movsb ; 1:78d97
    pop ds ; 1:78d99
    mov eax, offset _hmi_dpmi_asm_state ; 1:78d9a

; 1:78d9f: internal control-flow target
dpmi_asm_at_78d9f:
    pop es ; 1:78d9f
    pop gs ; 1:78da0
    pop fs ; 1:78da2
    pop ecx ; 1:78da4
    pop ebx ; 1:78da5
    pop edi ; 1:78da6
    pop esi ; 1:78da7
    pop ebp ; 1:78da8
    ret ; 1:78da9

; 1:78daa: driver_set_table_callback
hmi_driver_set_table_callback_:
    push ebp ; 1:78daa
    mov ebp, esp ; 1:78dab
    push esi ; 1:78dad
    push edi ; 1:78dae
    push ebx ; 1:78daf
    push ecx ; 1:78db0
    push fs ; 1:78db1
    push gs ; 1:78db3
    push es ; 1:78db5
    les esi, fword ptr [_hmi_dpmi_asm_state + BRIDGE_CALLBACK_TABLE] ; 1:78db6
    mov eax, dword ptr [ebp + 8] ; 1:78dbc
    mov ebx, eax ; 1:78dbf
    shl eax, 1 ; 1:78dc1
    shl ebx, 2 ; 1:78dc3
    add ebx, eax ; 1:78dc6
    mov eax, dword ptr [ebp + 0ch] ; 1:78dc8
    mov dword ptr es:[esi + ebx], eax ; 1:78dcb
    mov word ptr es:[esi + ebx + 4], ds ; 1:78dcf
    pop es ; 1:78dd5
    pop gs ; 1:78dd6
    pop fs ; 1:78dd8
    pop ecx ; 1:78dda
    pop ebx ; 1:78ddb
    pop edi ; 1:78ddc
    pop esi ; 1:78ddd
    pop ebp ; 1:78dde
    ret ; 1:78ddf

; 1:78de0: driver_op11_op12_transfer
hmi_driver_op11_op12_transfer_:
    push ebp ; 1:78de0
    mov ebp, esp ; 1:78de1
    push esi ; 1:78de3
    push edi ; 1:78de4
    push ebx ; 1:78de5
    push ecx ; 1:78de6
    push fs ; 1:78de7
    push gs ; 1:78de9
    push es ; 1:78deb
    push esi ; 1:78dec
    push es ; 1:78ded
    mov ax, 0bh ; 1:78dee
    lfs edi, fword ptr [ebp + 020h] ; 1:78df2
    call fword ptr [ebp + 018h] ; 1:78df6
    mov esi, eax ; 1:78df9
    mov es, dx ; 1:78dfb
    mov ax, 0ch ; 1:78dfd
    lfs edi, fword ptr [ebp + 010h] ; 1:78e01
    call fword ptr [ebp + 8] ; 1:78e05
    pop es ; 1:78e08
    pop esi ; 1:78e09
    pop es ; 1:78e0a
    pop gs ; 1:78e0b
    pop fs ; 1:78e0d
    pop ecx ; 1:78e0f
    pop ebx ; 1:78e10
    pop edi ; 1:78e11
    pop esi ; 1:78e12
    pop ebp ; 1:78e13
    ret ; 1:78e14

; 1:78e15: dpmi_largest_free_block
hmi_dpmi_largest_free_block_:
    push ebp ; 1:78e15
    mov ebp, esp ; 1:78e16
    push esi ; 1:78e18
    push edi ; 1:78e19
    push ebx ; 1:78e1a
    push ecx ; 1:78e1b
    push fs ; 1:78e1c
    push gs ; 1:78e1e
    push es ; 1:78e20
    mov ax, 0500h ; 1:78e21
    int 031h ; 1:78e25
    push ds ; 1:78e27
    pop es ; 1:78e28
    mov edi, offset _hmi_dpmi_asm_state + BRIDGE_MEMORY_INFO ; 1:78e29
    int 031h ; 1:78e2e
    mov eax, dword ptr [_hmi_dpmi_asm_state + BRIDGE_MEMORY_INFO] ; 1:78e30
    pop es ; 1:78e35
    pop gs ; 1:78e36
    pop fs ; 1:78e38
    pop ecx ; 1:78e3a
    pop ebx ; 1:78e3b
    pop edi ; 1:78e3c
    pop esi ; 1:78e3d
    pop ebp ; 1:78e3e
    ret ; 1:78e3f

; 1:78e40: windows_query
hmi_windows_query_:
    push ebp ; 1:78e40
    mov ebp, esp ; 1:78e41
    push esi ; 1:78e43
    push edi ; 1:78e44
    push ebx ; 1:78e45
    push ecx ; 1:78e46
    push fs ; 1:78e47
    push gs ; 1:78e49
    push es ; 1:78e4b
    mov ax, 01600h ; 1:78e4c
    int 02fh ; 1:78e50
    pop es ; 1:78e52
    pop gs ; 1:78e53
    pop fs ; 1:78e55
    pop ecx ; 1:78e57
    pop ebx ; 1:78e58
    pop edi ; 1:78e59
    pop esi ; 1:78e5a
    pop ebp ; 1:78e5b
    ret ; 1:78e5c

; 1:78e5d: dpmi_alloc_dos
hmi_dpmi_alloc_dos_:
    push ebp ; 1:78e5d
    mov ebp, esp ; 1:78e5e
    push esi ; 1:78e60
    push edi ; 1:78e61
    push ebx ; 1:78e62
    push ecx ; 1:78e63
    push fs ; 1:78e64
    push gs ; 1:78e66
    push es ; 1:78e68
    mov ax, 0100h ; 1:78e69
    mov ebx, dword ptr [ebp + 8] ; 1:78e6d
    int 031h ; 1:78e70
    jae short dpmi_asm_at_78e82 ; 1:78e72
    nop ; 1:78e74
    nop ; 1:78e75
    nop ; 1:78e76
    nop ; 1:78e77
    mov eax, 1 ; 1:78e78
    jmp short dpmi_asm_at_78e93 ; 1:78e7d
    nop ; 1:78e7f
    nop ; 1:78e80
    nop ; 1:78e81

; 1:78e82: internal control-flow target
dpmi_asm_at_78e82:
    mov esi, dword ptr [ebp + 0ch] ; 1:78e82
    mov word ptr [esi], dx ; 1:78e85
    mov esi, dword ptr [ebp + 010h] ; 1:78e88
    mov word ptr [esi], ax ; 1:78e8b
    mov eax, 0 ; 1:78e8e

; 1:78e93: internal control-flow target
dpmi_asm_at_78e93:
    pop es ; 1:78e93
    pop gs ; 1:78e94
    pop fs ; 1:78e96
    pop ecx ; 1:78e98
    pop ebx ; 1:78e99
    pop edi ; 1:78e9a
    pop esi ; 1:78e9b
    pop ebp ; 1:78e9c
    ret ; 1:78e9d

; 1:78e9e: dpmi_free_dos
hmi_dpmi_free_dos_:
    mov ax, ds ; 1:78e9e
    mov gs, ax ; 1:78ea1
    mov fs, ax ; 1:78ea3
    mov es, ax ; 1:78ea5
    push ebp ; 1:78ea7
    mov ebp, esp ; 1:78ea8
    push esi ; 1:78eaa
    push edi ; 1:78eab
    push ebx ; 1:78eac
    push ecx ; 1:78ead
    push fs ; 1:78eae
    push gs ; 1:78eb0
    push es ; 1:78eb2
    mov ax, 0101h ; 1:78eb3
    mov dx, word ptr [ebp + 8] ; 1:78eb7
    int 031h ; 1:78ebb
    mov ax, 0 ; 1:78ebd
    pop es ; 1:78ec1
    pop gs ; 1:78ec2
    pop fs ; 1:78ec4
    pop ecx ; 1:78ec6
    pop ebx ; 1:78ec7
    pop edi ; 1:78ec8
    pop esi ; 1:78ec9
    pop ebp ; 1:78eca
    ret ; 1:78ecb

; 1:78ecc: vds_lock_real_interrupt
hmi_vds_lock_real_interrupt_:
    push ebp ; 1:78ecc
    mov ebp, esp ; 1:78ecd
    push esi ; 1:78ecf
    push edi ; 1:78ed0
    push ebx ; 1:78ed1
    push ecx ; 1:78ed2
    push fs ; 1:78ed3
    push gs ; 1:78ed5
    push es ; 1:78ed7
    mov esi, offset _hmi_dpmi_asm_state + BRIDGE_REAL_REGS ; 1:78ed8
    mov dword ptr [esi + 01ch], 08107h ; 1:78edd
    mov dword ptr [esi + 014h], 0 ; 1:78ee4
    mov ax, word ptr [ebp + 8] ; 1:78eeb
    mov word ptr [esi + 022h], ax ; 1:78eef
    mov dword ptr [esi], 0 ; 1:78ef3
    mov ax, 0300h ; 1:78ef9
    mov bl, 04bh ; 1:78efd
    mov bh, 0 ; 1:78eff
    mov cx, 0 ; 1:78f01
    mov edi, offset _hmi_dpmi_asm_state + BRIDGE_REAL_REGS ; 1:78f05
    push ds ; 1:78f0a
    pop es ; 1:78f0b
    int 031h ; 1:78f0c
    mov esi, offset _hmi_dpmi_asm_state + BRIDGE_REAL_REGS ; 1:78f0e
    and word ptr [esi + 020h], 1 ; 1:78f13
    jne short dpmi_asm_at_78f28 ; 1:78f18
    nop ; 1:78f1a
    nop ; 1:78f1b
    nop ; 1:78f1c
    nop ; 1:78f1d
    mov eax, 0 ; 1:78f1e
    jmp short dpmi_asm_at_78f2d ; 1:78f23
    nop ; 1:78f25
    nop ; 1:78f26
    nop ; 1:78f27

; 1:78f28: internal control-flow target
dpmi_asm_at_78f28:
    mov eax, 1 ; 1:78f28

; 1:78f2d: internal control-flow target
dpmi_asm_at_78f2d:
    pop es ; 1:78f2d
    pop gs ; 1:78f2e
    pop fs ; 1:78f30
    pop ecx ; 1:78f32
    pop ebx ; 1:78f33
    pop edi ; 1:78f34
    pop esi ; 1:78f35
    pop ebp ; 1:78f36
    ret ; 1:78f37

; 1:78f38: vds_unlock_real_interrupt
hmi_vds_unlock_real_interrupt_:
    push ebp ; 1:78f38
    mov ebp, esp ; 1:78f39
    push esi ; 1:78f3b
    push edi ; 1:78f3c
    push ebx ; 1:78f3d
    push ecx ; 1:78f3e
    push fs ; 1:78f3f
    push gs ; 1:78f41
    push es ; 1:78f43
    mov esi, offset _hmi_dpmi_asm_state + BRIDGE_REAL_REGS ; 1:78f44
    mov dword ptr [esi + 01ch], 08108h ; 1:78f49
    mov dword ptr [esi + 014h], 0 ; 1:78f50
    mov ax, word ptr [ebp + 8] ; 1:78f57
    mov word ptr [esi + 022h], ax ; 1:78f5b
    mov dword ptr [esi], 0 ; 1:78f5f
    mov ax, 0300h ; 1:78f65
    mov bl, 04bh ; 1:78f69
    mov bh, 0 ; 1:78f6b
    mov cx, 0 ; 1:78f6d
    mov edi, offset _hmi_dpmi_asm_state + BRIDGE_REAL_REGS ; 1:78f71
    push ds ; 1:78f76
    pop es ; 1:78f77
    int 031h ; 1:78f78
    pop es ; 1:78f7a
    pop gs ; 1:78f7b
    pop fs ; 1:78f7d
    pop ecx ; 1:78f7f
    pop ebx ; 1:78f80
    pop edi ; 1:78f81
    pop esi ; 1:78f82
    pop ebp ; 1:78f83
    ret ; 1:78f84

; 1:78f85: dpmi_map_physical_selector
hmi_dpmi_map_physical_selector_:
    push ebp ; 1:78f85
    mov ebp, esp ; 1:78f86
    push esi ; 1:78f88
    push edi ; 1:78f89
    push ebx ; 1:78f8a
    push ecx ; 1:78f8b
    push fs ; 1:78f8c
    push gs ; 1:78f8e
    push es ; 1:78f90
    mov ax, 0800h ; 1:78f91
    mov bx, word ptr [ebp + 0ah] ; 1:78f95
    mov cx, word ptr [ebp + 8] ; 1:78f99
    mov si, 0 ; 1:78f9d
    mov di, 04000h ; 1:78fa1
    int 031h ; 1:78fa5
    mov word ptr [ebp - 8], cx ; 1:78fa7
    mov word ptr [ebp - 6], bx ; 1:78fab
    mov ax, 0 ; 1:78faf
    mov cx, 1 ; 1:78fb3
    int 031h ; 1:78fb7
    mov word ptr [ebp - 4], ax ; 1:78fb9
    mov ax, 7 ; 1:78fbd
    mov bx, word ptr [ebp - 4] ; 1:78fc1
    mov cx, word ptr [ebp - 6] ; 1:78fc5
    mov dx, word ptr [ebp - 8] ; 1:78fc9
    int 031h ; 1:78fcd
    mov ax, 8 ; 1:78fcf
    mov bx, word ptr [ebp - 4] ; 1:78fd3
    mov cx, 0ffffh ; 1:78fd7
    mov dx, 0ffffh ; 1:78fdb
    int 031h ; 1:78fdf
    mov ax, 9 ; 1:78fe1
    xor ecx, ecx ; 1:78fe5
    mov cx, ds ; 1:78fe7
    lar ecx, ecx ; 1:78fea
    shr ecx, 8 ; 1:78fed
    mov bx, word ptr [ebp - 4] ; 1:78ff0
    int 031h ; 1:78ff4
    mov ax, word ptr [ebp - 4] ; 1:78ff6
    pop es ; 1:78ffa
    pop gs ; 1:78ffb
    pop fs ; 1:78ffd
    pop ecx ; 1:78fff
    pop ebx ; 1:79000
    pop edi ; 1:79001
    pop esi ; 1:79002
    pop ebp ; 1:79003
    ret ; 1:79004

; 1:79005: dpmi_free_selector
hmi_dpmi_free_selector_:
    mov ax, ds ; 1:79005
    mov gs, ax ; 1:79008
    mov fs, ax ; 1:7900a
    mov es, ax ; 1:7900c
    push ebp ; 1:7900e
    mov ebp, esp ; 1:7900f
    push esi ; 1:79011
    push edi ; 1:79012
    push ebx ; 1:79013
    push ecx ; 1:79014
    push fs ; 1:79015
    push gs ; 1:79017
    push es ; 1:79019
    mov ax, 1 ; 1:7901a
    mov bx, word ptr [ebp + 8] ; 1:7901e
    int 031h ; 1:79022
    pop es ; 1:79024
    pop gs ; 1:79025
    pop fs ; 1:79027
    pop ecx ; 1:79029
    pop ebx ; 1:7902a
    pop edi ; 1:7902b
    pop esi ; 1:7902c
    pop ebp ; 1:7902d
    ret ; 1:7902e

; 1:7902f: get_ds_selector
hmi_get_ds_selector_:
    mov ax, ds ; 1:7902f
    ret ; 1:79032

; 1:79033: get_cs_selector
hmi_get_cs_selector_:
    mov ax, cs ; 1:79033
    ret ; 1:79036

; 1:79037: dpmi_asm_end
hmi_dpmi_asm_end_:
    ret ; 1:79037

_TEXT ends

; Object-2 lock interval 2:2fc6c..2:2fd6c, plus its four-byte end marker.
; Storage field names are INVENTED. Unknown spans are kept explicit.
_DATA segment para public use32 'DATA'
public _hmi_dpmi_asm_data_begin, _hmi_dpmi_asm_state, _hmi_dpmi_asm_data_end
_hmi_dpmi_asm_data_begin dd 0
_hmi_dpmi_asm_state label byte
    db 050h dup (0)              ; driver record copy buffer, +00
    dd 0                         ; record-table offset, +50
    dw 0, 0                      ; selector and reserved word, +54
    dd 0                         ; callback-table offset, +58
    dw 0, 0                      ; selector and reserved word, +5c
    dd 12 dup (0)                ; DPMI memory-information buffer, +60
    db 12 dup (0)                ; unidentified storage, +90
    dd 8 dup (0)                 ; real-mode GPR image, +9c
    dw 9 dup (0)                 ; flags/segments/IP/CS/SP/SS, +bc
    db 01eh dup (0)              ; unidentified storage, +ce
    dw 0, 0                      ; detected AX, reserved, +ec
    dw 0, 0                      ; sign-extended CL, reserved, +f0
    dw 0, 0                      ; sign-extended CH, reserved, +f4
    dw 0, 0                      ; detected DX, reserved, +f8
_hmi_dpmi_asm_data_end dd 0
_DATA ends

end
