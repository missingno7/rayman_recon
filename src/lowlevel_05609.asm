; DOS v1.21 object 1 slice 1:05609..1:0611e (end exclusive).
; Readable reconstruction from address-anchored CFG; labels preserve original offsets.
.386p
EXTRN clock_ticks_:NEAR
EXTRN _row_offsets_320_01b00:DWORD
EXTRN _During_The_Menu:WORD
EXTRN _joyy0:WORD
EXTRN _joyx0:WORD
EXTRN _ypadcentre:WORD
EXTRN _xpadcentre:WORD
EXTRN _joy_buttonB1:WORD
EXTRN _joy_buttonB2:WORD
EXTRN _joy_buttonA1:WORD
EXTRN _joy_buttonA2:WORD
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
    add eax, dword ptr [ebx*4 + _row_offsets_320_01b00] ; 1:0560a
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
    add eax, dword ptr [ebx*4 + _row_offsets_320_01b00] ; 1:05662
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
    add eax, dword ptr [ebx*4 + _row_offsets_320_01b00] ; 1:05692
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
    add eax, dword ptr [ebx*4 + _row_offsets_320_01b00] ; 1:056c2
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
    add eax, dword ptr [ebx*4 + _row_offsets_320_01b00] ; 1:056fe
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
    add eax, dword ptr [ebx*4 + _row_offsets_320_01b00] ; 1:05747
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
    add eax, dword ptr [ebx*4 + _row_offsets_320_01b00] ; 1:05777
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
    add eax, dword ptr [ebx*4 + _row_offsets_320_01b00] ; 1:057c8
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
    add eax, dword ptr [ebx*4 + _row_offsets_320_01b00] ; 1:05801
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
    add eax, dword ptr [ebx*4 + _row_offsets_320_01b00] ; 1:05852
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
    add eax, dword ptr [ebx*4 + _row_offsets_320_01b00] ; 1:0589f
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
    add eax, dword ptr [ebx*4 + _row_offsets_320_01b00] ; 1:05916
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
end
