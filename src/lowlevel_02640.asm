; Rayman DOS v1.21 object 1 slice 1:02640..1:03125 (half-open).
; Source is address-anchored to build/workers/w5_asmscope/cfg.dis.
.386p
EXTRN _raster_rows:BYTE   ; 2:00014, 200 records of 32 bytes (lowlevel_data)
_TEXT segment para public use32 'CODE'
assume cs:_TEXT
public asm_02640_
public sub_1268A_
public copy_dwords_
public bios_wait_timer_
public sub_2748_
public display_emptypicture_
public display_anypicture_
public asm_0283c_
public asm_02850_
public asm_029d3_
public asm_02a73_
public mode_x_effect_2b3d_
public fplot_mode_x_2b86_
public mode_x_effect_2bb7_
public mode_x_effect_2c00_
public mode_x_effect_2c59_
public mode_x_effect_2cbf_
public mode_x_effect_2d08_
public mode_x_effect_2da9_
public mode_x_effect_2dfb_
public mode_x_effect_2e9c_
public mode_x_effect_2f0c_
public mode_x_effect_300f_
assume ds:_TEXT

asm_02640_:
    mov edx, dword ptr [ebp + _raster_rows + 14h]                ; 1:02640
    mov esi, dword ptr [ebp + _raster_rows + 10h]                ; 1:02646
    shl ecx, 0x10                                              ; 1:0264c
    sub edx, esi                                               ; 1:0264f
    xor eax, eax                                               ; 1:02651
    inc edx                                                    ; 1:02653
    shl esi, 8                                                 ; 1:02654
    idiv ecx                                                   ; 1:02657
    mov ebx, eax                                               ; 1:02659
    mov edx, dword ptr [ebp + _raster_rows + 0ch]                ; 1:0265b
    mov eax, dword ptr [ebp + _raster_rows + 08h]                ; 1:02661
    sub edx, eax                                               ; 1:02667
    add esi, eax                                               ; 1:02669
    inc edx                                                    ; 1:0266b
    xor eax, eax                                               ; 1:0266c
    idiv ecx                                                   ; 1:0266e
    mov dx, ax                                                 ; 1:02670
    shl edx, 0x10                                              ; 1:02673
    mov ax, bx                                                 ; 1:02676
    rol eax, 0x10                                              ; 1:02679
    shr ebx, 0x10                                              ; 1:0267c
    mov dl, bl                                                 ; 1:0267f
    mov bx, si                                                 ; 1:02681
    xor esi, esi                                               ; 1:02684
    shr ecx, 0x10                                              ; 1:02686
    ret                                                        ; 1:02689
sub_1268A_:
    pusha                                                      ; 1:0268a
    cld                                                        ; 1:0268c
    sub ebx, 0x50                                              ; 1:0268d
    push esi                                                   ; 1:02690
    mov edi, 0xab000                                           ; 1:02691
    mov ah, 1                                                  ; 1:02696
    push edx                                                   ; 1:02698
    push eax                                                   ; 1:02699
    mov dx, 0x3c4                                              ; 1:0269a
    mov al, 2                                                  ; 1:0269e
    out dx, ax                                                 ; 1:026a0
    pop eax                                                    ; 1:026a2
    pop edx                                                    ; 1:026a3
    mov ecx, 0xc8                                              ; 1:026a4
loc_26a9:
    push ecx                                                   ; 1:026a9
    mov ecx, 0x50                                              ; 1:026aa
    rep movsb                                                  ; 1:026af
    add esi, ebx                                               ; 1:026b1
    pop ecx                                                    ; 1:026b3
    dec ecx                                                    ; 1:026b4
    jne loc_26a9                                               ; 1:026b5
    pop esi                                                    ; 1:026b7
    add esi, edx                                               ; 1:026b8
    push esi                                                   ; 1:026ba
    mov edi, 0xab000                                           ; 1:026bb
    mov ah, 2                                                  ; 1:026c0
    push edx                                                   ; 1:026c2
    push eax                                                   ; 1:026c3
    mov dx, 0x3c4                                              ; 1:026c4
    mov al, 2                                                  ; 1:026c8
    out dx, ax                                                 ; 1:026ca
    pop eax                                                    ; 1:026cc
    pop edx                                                    ; 1:026cd
    mov ecx, 0xc8                                              ; 1:026ce
loc_26d3:
    push ecx                                                   ; 1:026d3
    mov ecx, 0x50                                              ; 1:026d4
    rep movsb                                                  ; 1:026d9
    add esi, ebx                                               ; 1:026db
    pop ecx                                                    ; 1:026dd
    dec ecx                                                    ; 1:026de
    jne loc_26d3                                               ; 1:026df
    pop esi                                                    ; 1:026e1
    add esi, edx                                               ; 1:026e2
    push esi                                                   ; 1:026e4
    mov edi, 0xab000                                           ; 1:026e5
    mov ah, 4                                                  ; 1:026ea
    push edx                                                   ; 1:026ec
    push eax                                                   ; 1:026ed
    mov dx, 0x3c4                                              ; 1:026ee
    mov al, 2                                                  ; 1:026f2
    out dx, ax                                                 ; 1:026f4
    pop eax                                                    ; 1:026f6
    pop edx                                                    ; 1:026f7
    mov ecx, 0xc8                                              ; 1:026f8
loc_26fd:
    push ecx                                                   ; 1:026fd
    mov ecx, 0x50                                              ; 1:026fe
    rep movsb                                                  ; 1:02703
    add esi, ebx                                               ; 1:02705
    pop ecx                                                    ; 1:02707
    dec ecx                                                    ; 1:02708
    jne loc_26fd                                               ; 1:02709
    pop esi                                                    ; 1:0270b
    add esi, edx                                               ; 1:0270c
    mov edi, 0xab000                                           ; 1:0270e
    mov ah, 8                                                  ; 1:02713
    push edx                                                   ; 1:02715
    push eax                                                   ; 1:02716
    mov dx, 0x3c4                                              ; 1:02717
    mov al, 2                                                  ; 1:0271b
    out dx, ax                                                 ; 1:0271d
    pop eax                                                    ; 1:0271f
    pop edx                                                    ; 1:02720
    mov ecx, 0xc8                                              ; 1:02721
loc_2726:
    push ecx                                                   ; 1:02726
    mov ecx, 0x50                                              ; 1:02727
    rep movsb                                                  ; 1:0272c
    add esi, ebx                                               ; 1:0272e
    pop ecx                                                    ; 1:02730
    dec ecx                                                    ; 1:02731
    jne loc_2726                                               ; 1:02732
    popa                                                       ; 1:02734
    ret                                                        ; 1:02736
copy_dwords_:
    rep movsd                                                  ; 1:02737
    ret                                                        ; 1:02739
bios_wait_timer_:
    push eax                                                   ; 1:0273a
    push es                                                    ; 1:0273b
    xor ax, ax                                                 ; 1:0273c
    mov es, ax                                                 ; 1:0273f
    mov ah, 0x83                                               ; 1:02741
    int 0x15                                                   ; 1:02743
    pop es                                                     ; 1:02745
    pop eax                                                    ; 1:02746
    ret                                                        ; 1:02747
sub_2748_:
    pushad                                                     ; 1:02748
    mov ah, 0                                                  ; 1:02749
    int 0x1a                                                   ; 1:0274b
    shl ecx, 0x10                                              ; 1:0274d
    and edx, 0xffff                                            ; 1:02750
    add edx, ecx                                               ; 1:02756
    mov dword ptr [esi], edx                                   ; 1:02758
    popad                                                      ; 1:0275a
    ret                                                        ; 1:0275b
asm_0275c:
    mov ah, 1                                                  ; 1:0275c
    sub esi, ecx                                               ; 1:0275e
loc_2760:
    push edi                                                   ; 1:02760
    push edx                                                   ; 1:02761
    push eax                                                   ; 1:02762
    mov dx, 0x3c4                                              ; 1:02763
    mov al, 2                                                  ; 1:02767
    out dx, ax                                                 ; 1:02769
    pop eax                                                    ; 1:0276b
    pop edx                                                    ; 1:0276c
    add esi, ecx                                               ; 1:0276d
    push esi                                                   ; 1:0276f
    push ecx                                                   ; 1:02770
    mov ecx, 0xfa0                                             ; 1:02771
    rep movsd                                                  ; 1:02776
    pop ecx                                                    ; 1:02778
    pop esi                                                    ; 1:02779
    pop edi                                                    ; 1:0277a
    shl ah, 1                                                  ; 1:0277b
    cmp ah, 0x10                                               ; 1:0277d
    jne loc_2760                                               ; 1:02780
    ret                                                        ; 1:02782
display_emptypicture_:
    mov edx, 0x3c4                                             ; 1:02783
    mov eax, 0xf02                                             ; 1:02788
    out dx, ax                                                 ; 1:0278d
    mov ecx, 0xfa0                                             ; 1:0278f
    xor eax, eax                                               ; 1:02794
    rep stosd                                                  ; 1:02796
    ret                                                        ; 1:02798

; Mutable state: stores at 1:027a3, 1:027b3, 1:027bf, 1:0282c;
; reads at 1:027d4/27e0/2811..2823 and 1:02804. Nine bytes, per cfg_code_writes.json.
picture_source_pitch:
    dd 0
picture_source_row:
    dd 0
picture_plane:
    db 0

display_anypicture_:
    push ebp                                                   ; 1:027a2
    mov dword ptr [picture_source_row], ecx                    ; 1:027a3
    mov ebp, eax                                               ; 1:027a9
    mov ecx, edx                                               ; 1:027ab
    and ecx, 0xff                                              ; 1:027ad
    mov dword ptr [picture_source_pitch], ecx                  ; 1:027b3
    and edx, 0xff00                                            ; 1:027b9
    mov byte ptr [picture_plane], dh                           ; 1:027bf
    mov ah, 1                                                  ; 1:027c5
    mov ecx, edi                                               ; 1:027c7
    and ecx, 3                                                 ; 1:027c9
    shl ah, cl                                                 ; 1:027cc
    mov dx, 0x3c4                                              ; 1:027ce
    mov ch, 1                                                  ; 1:027d2
    sub esi, dword ptr [picture_source_row]                    ; 1:027d4
    mov al, 2                                                  ; 1:027da
    out dx, ax                                                 ; 1:027dc
loc_27de:
    push ecx                                                   ; 1:027de
    push edi                                                   ; 1:027df
    add esi, dword ptr [picture_source_row]                    ; 1:027e0
    push esi                                                   ; 1:027e6
    push ebp                                                   ; 1:027e7
loc_27e8:
    push edi                                                   ; 1:027e8
    push esi                                                   ; 1:027e9
    mov ecx, ebx                                               ; 1:027ea
    rep movsd                                                  ; 1:027ec
    pop esi                                                    ; 1:027ee
    pop edi                                                    ; 1:027ef
    add edi, 0x50                                              ; 1:027f0
    add esi, dword ptr [picture_source_pitch]                  ; 1:027f3
    dec ebp                                                    ; 1:027f9
    jne loc_27e8                                               ; 1:027fa
    pop ebp                                                    ; 1:027fc
    pop esi                                                    ; 1:027fd
    pop edi                                                    ; 1:027fe
    call loc_2b30                                              ; 1:027ff
    mov ch, byte ptr [picture_plane]                           ; 1:02804
    shl ch, 1                                                  ; 1:0280a
    cmp ch, 0x10                                               ; 1:0280c
    jne loc_282c                                               ; 1:0280f
    sub esi, dword ptr [picture_source_row]                    ; 1:02811
    sub esi, dword ptr [picture_source_row]                    ; 1:02817
    sub esi, dword ptr [picture_source_row]                    ; 1:0281d
    sub esi, dword ptr [picture_source_row]                    ; 1:02823
    inc esi                                                    ; 1:02829
    mov ch, 1                                                  ; 1:0282a
loc_282c:
    mov byte ptr [picture_plane], ch                           ; 1:0282c
    pop ecx                                                    ; 1:02832
    shl ch, 1                                                  ; 1:02833
    cmp ch, 0x10                                               ; 1:02835
    jne loc_27de                                               ; 1:02838
    pop ebp                                                    ; 1:0283a
    ret                                                        ; 1:0283b
asm_0283c_:
    push dx                                                    ; 1:0283c
    mov dx, 0x3c4                                              ; 1:0283e
    and eax, 0xffff                                            ; 1:02842
    mov ah, al                                                 ; 1:02847
    mov al, 2                                                  ; 1:02849
    out dx, ax                                                 ; 1:0284b
    pop dx                                                     ; 1:0284d
    ret                                                        ; 1:0284f
asm_02850_:
    push ebp                                                   ; 1:02850
    push ebp                                                   ; 1:02851
    mov dword ptr [esp], 0                                     ; 1:02852
    push ecx                                                   ; 1:02859
    push ebx                                                   ; 1:0285a
    mov ebx, ecx                                               ; 1:0285b
    add ebx, 0x4600                                            ; 1:0285d
    sub edx, 0x80                                              ; 1:02863
    mov cl, 0                                                  ; 1:02869
    jmp loc_288b                                               ; 1:0286b
loc_286d:
    add dword ptr [esp], 0x500                                 ; 1:0286d
    cmp dword ptr [esp], ebx                                   ; 1:02874
    jb loc_2880                                                ; 1:02877
    sub dword ptr [esp], 0x4600                                ; 1:02879
loc_2880:
    inc cl                                                     ; 1:02880
    cmp cl, 0xe                                                ; 1:02882
    jge loc_29ce                                               ; 1:02885
loc_288b:
    push ecx                                                   ; 1:0288b
    push ebx                                                   ; 1:0288c
    add edx, 0x80                                              ; 1:0288d
    mov bx, 1                                                  ; 1:02893
    mov ebp, 0xfffffff4                                        ; 1:02897
    mov dword ptr [edx], 0                                     ; 1:0289c
    xor ecx, ecx                                               ; 1:028a2
    push edi                                                   ; 1:028a4
    jmp loc_28a9                                               ; 1:028a5
loc_28a7:
    inc cl                                                     ; 1:028a7
loc_28a9:
    cmp cl, 0x14                                               ; 1:028a9
    jge loc_29be                                               ; 1:028ac
    mov di, word ptr [eax + 4]                                 ; 1:028b2
    and di, 0xff                                               ; 1:028b6
    cmp di, 0                                                  ; 1:028bb
    jne loc_29b0                                               ; 1:028bf
    cmp bx, 0                                                  ; 1:028c5
    jne loc_2933                                               ; 1:028c9
    push edi                                                   ; 1:028cb
    push ecx                                                   ; 1:028cc
    mov edi, dword ptr [esp + 0x14]                            ; 1:028cd
    shl ecx, 2                                                 ; 1:028d1
    add edi, ecx                                               ; 1:028d4
    mov ecx, dword ptr [esp + 0x14]                            ; 1:028d6
    push edx                                                   ; 1:028da
    mov edx, dword ptr [esp + 0x1c]                            ; 1:028db
    push eax                                                   ; 1:028df
    push ebx                                                   ; 1:028e0
    push edx                                                   ; 1:028e1
    mov eax, edi                                               ; 1:028e2
    sub eax, edx                                               ; 1:028e4
    push edx                                                   ; 1:028e6
    mov dl, 0x50                                               ; 1:028e7
    div dl                                                     ; 1:028e9
    pop edx                                                    ; 1:028eb
    mov bl, al                                                 ; 1:028ec
    mov eax, ecx                                               ; 1:028ee
    sub eax, edx                                               ; 1:028f0
    push edx                                                   ; 1:028f2
    mov dl, 0x50                                               ; 1:028f3
    div dl                                                     ; 1:028f5
    pop edx                                                    ; 1:028f7
    cmp al, bl                                                 ; 1:028f8
    je loc_28ff                                                ; 1:028fa
    stc                                                        ; 1:028fc
    jmp loc_2900                                               ; 1:028fd
loc_28ff:
    clc                                                        ; 1:028ff
loc_2900:
    pop edx                                                    ; 1:02900
    pop ebx                                                    ; 1:02901
    pop eax                                                    ; 1:02902
    pop edx                                                    ; 1:02903
    pop ecx                                                    ; 1:02904
    pop edi                                                    ; 1:02905
    jae loc_2919                                               ; 1:02906
    cmp dword ptr [esp + 0x14], 0x50                           ; 1:02908
    je loc_292a                                                ; 1:0290d
    mov dword ptr [esp + 0x14], 0x50                           ; 1:0290f
    jmp loc_2980                                               ; 1:02917
loc_2919:
    cmp dword ptr [esp + 0x14], 0                              ; 1:02919
    je loc_292a                                                ; 1:0291e
    mov dword ptr [esp + 0x14], 0                              ; 1:02920
    jmp loc_2980                                               ; 1:02928
loc_292a:
    inc dword ptr [edx + ebp + 0xc]                            ; 1:0292a
    jmp loc_29b0                                               ; 1:0292e
loc_2933:
    mov dword ptr [esp + 0x14], 0                              ; 1:02933
    push edi                                                   ; 1:0293b
    push ecx                                                   ; 1:0293c
    mov edi, dword ptr [esp + 0x14]                            ; 1:0293d
    shl ecx, 2                                                 ; 1:02941
    add edi, ecx                                               ; 1:02944
    mov ecx, dword ptr [esp + 0x14]                            ; 1:02946
    push edx                                                   ; 1:0294a
    mov edx, dword ptr [esp + 0x1c]                            ; 1:0294b
    push eax                                                   ; 1:0294f
    push ebx                                                   ; 1:02950
    push edx                                                   ; 1:02951
    mov eax, edi                                               ; 1:02952
    sub eax, edx                                               ; 1:02954
    push edx                                                   ; 1:02956
    mov dl, 0x50                                               ; 1:02957
    div dl                                                     ; 1:02959
    pop edx                                                    ; 1:0295b
    mov bl, al                                                 ; 1:0295c
    mov eax, ecx                                               ; 1:0295e
    sub eax, edx                                               ; 1:02960
    push edx                                                   ; 1:02962
    mov dl, 0x50                                               ; 1:02963
    div dl                                                     ; 1:02965
    pop edx                                                    ; 1:02967
    cmp al, bl                                                 ; 1:02968
    je loc_296f                                                ; 1:0296a
    stc                                                        ; 1:0296c
    jmp loc_2970                                               ; 1:0296d
loc_296f:
    clc                                                        ; 1:0296f
loc_2970:
    pop edx                                                    ; 1:02970
    pop ebx                                                    ; 1:02971
    pop eax                                                    ; 1:02972
    pop edx                                                    ; 1:02973
    pop ecx                                                    ; 1:02974
    pop edi                                                    ; 1:02975
    jae loc_2980                                               ; 1:02976
    mov dword ptr [esp + 0x14], 0x50                           ; 1:02978
loc_2980:
    push edi                                                   ; 1:02980
    push ecx                                                   ; 1:02981
    inc dword ptr [edx]                                        ; 1:02982
    add ebp, 0xc                                               ; 1:02984
    mov edi, dword ptr [esp + 0x14]                            ; 1:02987
    shl ecx, 2                                                 ; 1:0298b
    add edi, ecx                                               ; 1:0298e
    sub edi, dword ptr [esp + 0x1c]                            ; 1:02990
    mov dword ptr [edx + ebp + 4], edi                         ; 1:02994
    mov dword ptr [edx + ebp + 8], esi                         ; 1:02998
    mov dword ptr [edx + ebp + 0xc], 0                         ; 1:0299c
    pop ecx                                                    ; 1:029a4
    pop edi                                                    ; 1:029a5
    inc dword ptr [edx + ebp + 0xc]                            ; 1:029a6
    jmp loc_29b0                                               ; 1:029aa
    ; Four explicit DOS NOPs at 1:029ac..1:029b0.
    nop
    nop
    nop
    nop
loc_29b0:
    mov bx, di                                                 ; 1:029b0
    add eax, 6                                                 ; 1:029b3
    add esi, 4                                                 ; 1:029b6
    jmp loc_28a7                                               ; 1:029b9
loc_29be:
    pop edi                                                    ; 1:029be
    add eax, edi                                               ; 1:029bf
    add esi, 0x4b0                                             ; 1:029c1
    pop ebx                                                    ; 1:029c7
    pop ecx                                                    ; 1:029c8
    jmp loc_286d                                               ; 1:029c9
loc_29ce:
    pop ebx                                                    ; 1:029ce
    pop ecx                                                    ; 1:029cf
    pop ebp                                                    ; 1:029d0
    pop ebp                                                    ; 1:029d1
    ret                                                        ; 1:029d2
asm_029d3_:
    push ebp                                                   ; 1:029d3
    push ebx                                                   ; 1:029d4
    mov ebx, ecx                                               ; 1:029d5
    sub edx, 0x80                                              ; 1:029d7
    mov cl, 0                                                  ; 1:029dd
    jmp loc_29ef                                               ; 1:029df
loc_29e1:
    add dword ptr [esp], ebx                                   ; 1:029e1
    inc cl                                                     ; 1:029e4
    cmp cl, 0xe                                                ; 1:029e6
    jge loc_2a70                                               ; 1:029e9
loc_29ef:
    push ecx                                                   ; 1:029ef
    push ebx                                                   ; 1:029f0
    add edx, 0x80                                              ; 1:029f1
    mov bx, 0                                                  ; 1:029f7
    mov ebp, 0xfffffff4                                        ; 1:029fb
    mov dword ptr [edx], 0                                     ; 1:02a00
    xor ecx, ecx                                               ; 1:02a06
    push edi                                                   ; 1:02a08
    jmp loc_2a0d                                               ; 1:02a09
loc_2a0b:
    inc cl                                                     ; 1:02a0b
loc_2a0d:
    cmp cl, 0x14                                               ; 1:02a0d
    jge loc_2a60                                               ; 1:02a10
    mov di, word ptr [eax + 4]                                 ; 1:02a12
    and di, 0xff                                               ; 1:02a16
    cmp di, 0                                                  ; 1:02a1b
    je loc_2a55                                                ; 1:02a1f
    cmp bx, 0                                                  ; 1:02a21
    je loc_2a2d                                                ; 1:02a25
loc_2a27:
    inc dword ptr [edx + ebp + 0xc]                            ; 1:02a27
    jmp loc_2a55                                               ; 1:02a2b
loc_2a2d:
    push edi                                                   ; 1:02a2d
    push ecx                                                   ; 1:02a2e
    inc dword ptr [edx]                                        ; 1:02a2f
    add ebp, 0xc                                               ; 1:02a31
    mov edi, dword ptr [esp + 0x14]                            ; 1:02a34
    shl ecx, 2                                                 ; 1:02a38
    add edi, ecx                                               ; 1:02a3b
    mov dword ptr [edx + ebp + 4], edi                         ; 1:02a3d
    mov dword ptr [edx + ebp + 8], esi                         ; 1:02a41
    mov dword ptr [edx + ebp + 0xc], 0                         ; 1:02a45
    pop ecx                                                    ; 1:02a4d
    pop edi                                                    ; 1:02a4e
    jmp loc_2a27                                               ; 1:02a4f
    ; Four explicit DOS NOPs at 1:02a51..1:02a55.
    nop
    nop
    nop
    nop
loc_2a55:
    mov bx, di                                                 ; 1:02a55
    add eax, 6                                                 ; 1:02a58
    add esi, 4                                                 ; 1:02a5b
    jmp loc_2a0b                                               ; 1:02a5e
loc_2a60:
    pop edi                                                    ; 1:02a60
    add eax, edi                                               ; 1:02a61
    add esi, 0x4b0                                             ; 1:02a63
    pop ebx                                                    ; 1:02a69
    pop ecx                                                    ; 1:02a6a
    jmp loc_29e1                                               ; 1:02a6b
loc_2a70:
    pop ebx                                                    ; 1:02a70
    pop ebp                                                    ; 1:02a71
    ret                                                        ; 1:02a72
asm_02a73_:
    push ebp                                                   ; 1:02a73
    push ebx                                                   ; 1:02a74
    mov ebx, ecx                                               ; 1:02a75
    sub edx, 0x80                                              ; 1:02a77
    add eax, 0x78                                              ; 1:02a7d
    add eax, edi                                               ; 1:02a80
    mov cl, 0xe                                                ; 1:02a82
    jmp loc_2a8e                                               ; 1:02a84
loc_2a86:
    dec cl                                                     ; 1:02a86
    je loc_2b2d                                                ; 1:02a88
loc_2a8e:
    push ecx                                                   ; 1:02a8e
    push ebx                                                   ; 1:02a8f
    add edx, 0x80                                              ; 1:02a90
    mov bx, 0                                                  ; 1:02a96
    mov ebp, 0xfffffff4                                        ; 1:02a9a
    mov dword ptr [edx], 0                                     ; 1:02a9f
    xor ecx, ecx                                               ; 1:02aa5
    push edi                                                   ; 1:02aa7
    add eax, 6                                                 ; 1:02aa8
    add esi, 4                                                 ; 1:02aab
    add dword ptr [esp + 0xc], 4                               ; 1:02aae
    jmp loc_2ab7                                               ; 1:02ab3
loc_2ab5:
    inc cl                                                     ; 1:02ab5
loc_2ab7:
    cmp cl, 0x13                                               ; 1:02ab7
    jge loc_2b15                                               ; 1:02aba
    mov di, word ptr [eax + 4]                                 ; 1:02abc
    and di, 0xff                                               ; 1:02ac0
    je loc_2ad0                                                ; 1:02ac5
    or bx, bx                                                  ; 1:02ac7
    je loc_2ae0                                                ; 1:02aca
    inc dword ptr [edx + ebp + 0xc]                            ; 1:02acc
loc_2ad0:
    mov bx, di                                                 ; 1:02ad0
    add eax, 6                                                 ; 1:02ad3
    add esi, 4                                                 ; 1:02ad6
    add dword ptr [esp + 0xc], 4                               ; 1:02ad9
    jmp loc_2ab5                                               ; 1:02ade
loc_2ae0:
    push edi                                                   ; 1:02ae0
    push ecx                                                   ; 1:02ae1
    inc dword ptr [edx]                                        ; 1:02ae2
    add ebp, 0xc                                               ; 1:02ae4
    mov edi, dword ptr [esp + 0x14]                            ; 1:02ae7
    add dword ptr [esp + 0x14], 4                              ; 1:02aeb
    mov dword ptr [edx + ebp + 4], edi                         ; 1:02af0
    mov dword ptr [edx + ebp + 8], esi                         ; 1:02af4
    mov dword ptr [edx + ebp + 0xc], 0                         ; 1:02af8
    pop ecx                                                    ; 1:02b00
    pop edi                                                    ; 1:02b01
    inc dword ptr [edx + ebp + 0xc]                            ; 1:02b02
    mov bx, di                                                 ; 1:02b06
    add eax, 6                                                 ; 1:02b09
    add esi, 4                                                 ; 1:02b0c
    jmp loc_2ab5                                               ; 1:02b0f
    ; Four explicit DOS NOPs at 1:02b11..1:02b15.
    nop
    nop
    nop
    nop
loc_2b15:
    pop edi                                                    ; 1:02b15
    add eax, edi                                               ; 1:02b16
    add esi, 0x4b0                                             ; 1:02b18
    add dword ptr [esp + 8], 0x4b0                             ; 1:02b1e
    pop ebx                                                    ; 1:02b26
    pop ecx                                                    ; 1:02b27
    jmp loc_2a86                                               ; 1:02b28
loc_2b2d:
    pop ebx                                                    ; 1:02b2d
    pop ebp                                                    ; 1:02b2e
    ret                                                        ; 1:02b2f
loc_2b30:
    shl ah, 1                                                  ; 1:02b30
    cmp ah, 0x10                                               ; 1:02b32
    jne loc_2b3a                                               ; 1:02b35
    inc edi                                                    ; 1:02b37
    mov ah, 1                                                  ; 1:02b38
loc_2b3a:
    out dx, ax                                                 ; 1:02b3a
    ret                                                        ; 1:02b3c
mode_x_effect_2b3d_:
    push edi                                                   ; 1:02b3d
    cmp eax, 0                                                 ; 1:02b3e
    jl loc_2b84                                                ; 1:02b41
    cmp eax, 0x140                                             ; 1:02b43
    ja loc_2b84                                                ; 1:02b48
    cmp ebx, 0                                                 ; 1:02b4a
    jl loc_2b84                                                ; 1:02b4d
    cmp ebx, 0xc8                                              ; 1:02b4f
    ja loc_2b84                                                ; 1:02b55
    push ecx                                                   ; 1:02b57
    push edx                                                   ; 1:02b58
    mov edi, eax                                               ; 1:02b59
    and eax, 3                                                 ; 1:02b5b
    mov ah, 1                                                  ; 1:02b5e
    mov cl, al                                                 ; 1:02b60
    shl ah, cl                                                 ; 1:02b62
    mov dx, 0x3c4                                              ; 1:02b64
    mov al, 2                                                  ; 1:02b68
    out dx, ax                                                 ; 1:02b6a
    sar edi, 2                                                 ; 1:02b6c
    mov ecx, ebx                                               ; 1:02b6f
    shl ecx, 4                                                 ; 1:02b71
    shl ebx, 6                                                 ; 1:02b74
    add ecx, ebx                                               ; 1:02b77
    add edi, ecx                                               ; 1:02b79
    add edi, dword ptr [esp + 8]                               ; 1:02b7b
    mov byte ptr [edi], 5                                      ; 1:02b7f
    pop edx                                                    ; 1:02b82
    pop ecx                                                    ; 1:02b83
loc_2b84:
    pop edi                                                    ; 1:02b84
    ret                                                        ; 1:02b85
fplot_mode_x_2b86_:
    push edi                                                   ; 1:02b86
    push ecx                                                   ; 1:02b87
    push edx                                                   ; 1:02b88
    mov edi, eax                                               ; 1:02b89
    and eax, 3                                                 ; 1:02b8b
    mov ah, 1                                                  ; 1:02b8e
    mov cl, al                                                 ; 1:02b90
    shl ah, cl                                                 ; 1:02b92
    mov dx, 0x3c4                                              ; 1:02b94
    mov al, 2                                                  ; 1:02b98
    out dx, ax                                                 ; 1:02b9a
    sar edi, 2                                                 ; 1:02b9c
    mov ecx, ebx                                               ; 1:02b9f
    shl ecx, 4                                                 ; 1:02ba1
    shl ebx, 6                                                 ; 1:02ba4
    add ecx, ebx                                               ; 1:02ba7
    add edi, ecx                                               ; 1:02ba9
    add edi, dword ptr [esp + 8]                               ; 1:02bab
    mov edx, esi                                               ; 1:02baf
    mov byte ptr [edi], dl                                     ; 1:02bb1
    pop edx                                                    ; 1:02bb3
    pop ecx                                                    ; 1:02bb4
    pop edi                                                    ; 1:02bb5
    ret                                                        ; 1:02bb6
mode_x_effect_2bb7_:
    push edi                                                   ; 1:02bb7
    cmp eax, 0                                                 ; 1:02bb8
    jl loc_2bfe                                                ; 1:02bbb
    cmp eax, 0x140                                             ; 1:02bbd
    ja loc_2bfe                                                ; 1:02bc2
    cmp ebx, 0                                                 ; 1:02bc4
    jl loc_2bfe                                                ; 1:02bc7
    cmp ebx, 0xc8                                              ; 1:02bc9
    ja loc_2bfe                                                ; 1:02bcf
    push ecx                                                   ; 1:02bd1
    push edx                                                   ; 1:02bd2
    mov edi, eax                                               ; 1:02bd3
    and eax, 3                                                 ; 1:02bd5
    mov ah, 1                                                  ; 1:02bd8
    mov cl, al                                                 ; 1:02bda
    shl ah, cl                                                 ; 1:02bdc
    mov dx, 0x3c4                                              ; 1:02bde
    mov al, 2                                                  ; 1:02be2
    out dx, ax                                                 ; 1:02be4
    sar edi, 2                                                 ; 1:02be6
    mov ecx, ebx                                               ; 1:02be9
    shl ecx, 4                                                 ; 1:02beb
    shl ebx, 6                                                 ; 1:02bee
    add ecx, ebx                                               ; 1:02bf1
    add edi, ecx                                               ; 1:02bf3
    add edi, dword ptr [esp + 8]                               ; 1:02bf5
    mov byte ptr [edi], 7                                      ; 1:02bf9
    pop edx                                                    ; 1:02bfc
    pop ecx                                                    ; 1:02bfd
loc_2bfe:
    pop edi                                                    ; 1:02bfe
    ret                                                        ; 1:02bff
mode_x_effect_2c00_:
    push edi                                                   ; 1:02c00
    cmp eax, 0                                                 ; 1:02c01
    jl loc_2c57                                                ; 1:02c04
    cmp eax, 0x140                                             ; 1:02c06
    ja loc_2c57                                                ; 1:02c0b
    cmp ebx, 0                                                 ; 1:02c0d
    jl loc_2c57                                                ; 1:02c10
    cmp ebx, 0xc8                                              ; 1:02c12
    ja loc_2c57                                                ; 1:02c18
    push ecx                                                   ; 1:02c1a
    push edx                                                   ; 1:02c1b
    mov edi, eax                                               ; 1:02c1c
    and eax, 3                                                 ; 1:02c1e
    mov ah, 1                                                  ; 1:02c21
    mov cl, al                                                 ; 1:02c23
    shl ah, cl                                                 ; 1:02c25
    mov dx, 0x3c4                                              ; 1:02c27
    mov al, 2                                                  ; 1:02c2b
    out dx, ax                                                 ; 1:02c2d
    sar edi, 2                                                 ; 1:02c2f
    mov ecx, ebx                                               ; 1:02c32
    shl ecx, 4                                                 ; 1:02c34
    shl ebx, 6                                                 ; 1:02c37
    add ecx, ebx                                               ; 1:02c3a
    add edi, ecx                                               ; 1:02c3c
    add edi, dword ptr [esp + 8]                               ; 1:02c3e
    mov byte ptr [edi], 8                                      ; 1:02c42
    mov byte ptr [edi + 0x50], 7                               ; 1:02c45
    call loc_2b30                                              ; 1:02c49
    mov byte ptr [edi], 7                                      ; 1:02c4e
    mov byte ptr [edi + 0x50], 6                               ; 1:02c51
    pop edx                                                    ; 1:02c55
    pop ecx                                                    ; 1:02c56
loc_2c57:
    pop edi                                                    ; 1:02c57
    ret                                                        ; 1:02c58
mode_x_effect_2c59_:
    push edi                                                   ; 1:02c59
    cmp eax, 0                                                 ; 1:02c5a
    jl loc_2cbd                                                ; 1:02c5d
    cmp eax, 0x140                                             ; 1:02c5f
    ja loc_2cbd                                                ; 1:02c64
    cmp ebx, 0                                                 ; 1:02c66
    jl loc_2cbd                                                ; 1:02c69
    cmp ebx, 0xc8                                              ; 1:02c6b
    ja loc_2cbd                                                ; 1:02c71
    push ecx                                                   ; 1:02c73
    push edx                                                   ; 1:02c74
    mov edi, eax                                               ; 1:02c75
    and eax, 3                                                 ; 1:02c77
    mov ah, 1                                                  ; 1:02c7a
    mov cl, al                                                 ; 1:02c7c
    shl ah, cl                                                 ; 1:02c7e
    mov dx, 0x3c4                                              ; 1:02c80
    mov al, 2                                                  ; 1:02c84
    out dx, ax                                                 ; 1:02c86
    sar edi, 2                                                 ; 1:02c88
    mov ecx, ebx                                               ; 1:02c8b
    shl ecx, 4                                                 ; 1:02c8d
    shl ebx, 6                                                 ; 1:02c90
    add ecx, ebx                                               ; 1:02c93
    add edi, ecx                                               ; 1:02c95
    add edi, dword ptr [esp + 8]                               ; 1:02c97
    mov byte ptr [edi + 0x50], 6                               ; 1:02c9b
    call loc_2b30                                              ; 1:02c9f
    mov byte ptr [edi], 6                                      ; 1:02ca4
    mov byte ptr [edi + 0x50], 8                               ; 1:02ca7
    mov byte ptr [edi + 0xa0], 6                               ; 1:02cab
    call loc_2b30                                              ; 1:02cb2
    mov byte ptr [edi + 0x50], 6                               ; 1:02cb7
    pop edx                                                    ; 1:02cbb
    pop ecx                                                    ; 1:02cbc
loc_2cbd:
    pop edi                                                    ; 1:02cbd
    ret                                                        ; 1:02cbe
mode_x_effect_2cbf_:
    push edi                                                   ; 1:02cbf
    cmp eax, 0                                                 ; 1:02cc0
    jl loc_2d06                                                ; 1:02cc3
    cmp eax, 0x140                                             ; 1:02cc5
    ja loc_2d06                                                ; 1:02cca
    cmp ebx, 0                                                 ; 1:02ccc
    jl loc_2d06                                                ; 1:02ccf
    cmp ebx, 0xc8                                              ; 1:02cd1
    ja loc_2d06                                                ; 1:02cd7
    push ecx                                                   ; 1:02cd9
    push edx                                                   ; 1:02cda
    mov edi, eax                                               ; 1:02cdb
    and eax, 3                                                 ; 1:02cdd
    mov ah, 1                                                  ; 1:02ce0
    mov cl, al                                                 ; 1:02ce2
    shl ah, cl                                                 ; 1:02ce4
    mov dx, 0x3c4                                              ; 1:02ce6
    mov al, 2                                                  ; 1:02cea
    out dx, ax                                                 ; 1:02cec
    sar edi, 2                                                 ; 1:02cee
    mov ecx, ebx                                               ; 1:02cf1
    shl ecx, 4                                                 ; 1:02cf3
    shl ebx, 6                                                 ; 1:02cf6
    add ecx, ebx                                               ; 1:02cf9
    add edi, ecx                                               ; 1:02cfb
    add edi, dword ptr [esp + 8]                               ; 1:02cfd
    mov byte ptr [edi], 3                                      ; 1:02d01
    pop edx                                                    ; 1:02d04
    pop ecx                                                    ; 1:02d05
loc_2d06:
    pop edi                                                    ; 1:02d06
    ret                                                        ; 1:02d07
mode_x_effect_2d08_:
    push edi                                                   ; 1:02d08
    cmp eax, 0                                                 ; 1:02d09
    jl loc_2da7                                                ; 1:02d0c
    cmp eax, 0x140                                             ; 1:02d12
    ja loc_2da7                                                ; 1:02d17
    cmp ebx, 0                                                 ; 1:02d1d
    jl loc_2da7                                                ; 1:02d20
    cmp ebx, 0xc8                                              ; 1:02d26
    ja loc_2da7                                                ; 1:02d2c
    push ecx                                                   ; 1:02d2e
    push edx                                                   ; 1:02d2f
    mov edi, eax                                               ; 1:02d30
    and eax, 3                                                 ; 1:02d32
    mov ah, 1                                                  ; 1:02d35
    mov cl, al                                                 ; 1:02d37
    shl ah, cl                                                 ; 1:02d39
    mov dx, 0x3c4                                              ; 1:02d3b
    mov al, 2                                                  ; 1:02d3f
    out dx, ax                                                 ; 1:02d41
    sar edi, 2                                                 ; 1:02d43
    mov ecx, ebx                                               ; 1:02d46
    shl ecx, 4                                                 ; 1:02d48
    shl ebx, 6                                                 ; 1:02d4b
    add ecx, ebx                                               ; 1:02d4e
    add edi, ecx                                               ; 1:02d50
    add edi, dword ptr [esp + 8]                               ; 1:02d52
    mov byte ptr [edi + 0x50], 5                               ; 1:02d56
    mov byte ptr [edi + 0xa0], 5                               ; 1:02d5a
    call loc_2b30                                              ; 1:02d61
    mov byte ptr [edi], 5                                      ; 1:02d66
    mov byte ptr [edi + 0x50], 7                               ; 1:02d69
    mov byte ptr [edi + 0xa0], 7                               ; 1:02d6d
    mov byte ptr [edi + 0xf0], 5                               ; 1:02d74
    call loc_2b30                                              ; 1:02d7b
    mov byte ptr [edi], 5                                      ; 1:02d80
    mov byte ptr [edi + 0x50], 7                               ; 1:02d83
    mov byte ptr [edi + 0xa0], 7                               ; 1:02d87
    mov byte ptr [edi + 0xf0], 5                               ; 1:02d8e
    call loc_2b30                                              ; 1:02d95
    mov byte ptr [edi + 0x50], 5                               ; 1:02d9a
    mov byte ptr [edi + 0xa0], 5                               ; 1:02d9e
    pop edx                                                    ; 1:02da5
    pop ecx                                                    ; 1:02da6
loc_2da7:
    pop edi                                                    ; 1:02da7
    ret                                                        ; 1:02da8
mode_x_effect_2da9_:
    push edi                                                   ; 1:02da9
    cmp eax, 0                                                 ; 1:02daa
    jl loc_2df9                                                ; 1:02dad
    cmp eax, 0x140                                             ; 1:02daf
    ja loc_2df9                                                ; 1:02db4
    cmp ebx, 0                                                 ; 1:02db6
    jl loc_2df9                                                ; 1:02db9
    cmp ebx, 0xc8                                              ; 1:02dbb
    ja loc_2df9                                                ; 1:02dc1
    push ecx                                                   ; 1:02dc3
    push edx                                                   ; 1:02dc4
    mov edi, eax                                               ; 1:02dc5
    and eax, 3                                                 ; 1:02dc7
    mov ah, 1                                                  ; 1:02dca
    mov cl, al                                                 ; 1:02dcc
    shl ah, cl                                                 ; 1:02dce
    mov dx, 0x3c4                                              ; 1:02dd0
    mov al, 2                                                  ; 1:02dd4
    out dx, ax                                                 ; 1:02dd6
    sar edi, 2                                                 ; 1:02dd8
    mov ecx, ebx                                               ; 1:02ddb
    shl ecx, 4                                                 ; 1:02ddd
    shl ebx, 6                                                 ; 1:02de0
    add ecx, ebx                                               ; 1:02de3
    add edi, ecx                                               ; 1:02de5
    add edi, dword ptr [esp + 8]                               ; 1:02de7
    mov byte ptr [edi + 0x50], 5                               ; 1:02deb
    call loc_2b30                                              ; 1:02def
    mov byte ptr [edi], 4                                      ; 1:02df4
    pop edx                                                    ; 1:02df7
    pop ecx                                                    ; 1:02df8
loc_2df9:
    pop edi                                                    ; 1:02df9
    ret                                                        ; 1:02dfa
mode_x_effect_2dfb_:
    push edi                                                   ; 1:02dfb
    cmp eax, 0                                                 ; 1:02dfc
    jl loc_2e9a                                                ; 1:02dff
    cmp eax, 0x140                                             ; 1:02e05
    ja loc_2e9a                                                ; 1:02e0a
    cmp ebx, 0                                                 ; 1:02e10
    jl loc_2e9a                                                ; 1:02e13
    cmp ebx, 0xc8                                              ; 1:02e19
    ja loc_2e9a                                                ; 1:02e1f
    push ecx                                                   ; 1:02e21
    push edx                                                   ; 1:02e22
    mov edi, eax                                               ; 1:02e23
    and eax, 3                                                 ; 1:02e25
    mov ah, 1                                                  ; 1:02e28
    mov cl, al                                                 ; 1:02e2a
    shl ah, cl                                                 ; 1:02e2c
    mov dx, 0x3c4                                              ; 1:02e2e
    mov al, 2                                                  ; 1:02e32
    out dx, ax                                                 ; 1:02e34
    sar edi, 2                                                 ; 1:02e36
    mov ecx, ebx                                               ; 1:02e39
    shl ecx, 4                                                 ; 1:02e3b
    shl ebx, 6                                                 ; 1:02e3e
    add ecx, ebx                                               ; 1:02e41
    add edi, ecx                                               ; 1:02e43
    add edi, dword ptr [esp + 8]                               ; 1:02e45
    mov byte ptr [edi + 0x50], 6                               ; 1:02e49
    mov byte ptr [edi + 0xa0], 6                               ; 1:02e4d
    call loc_2b30                                              ; 1:02e54
    mov byte ptr [edi], 6                                      ; 1:02e59
    mov byte ptr [edi + 0x50], 8                               ; 1:02e5c
    mov byte ptr [edi + 0xa0], 8                               ; 1:02e60
    mov byte ptr [edi + 0xf0], 6                               ; 1:02e67
    call loc_2b30                                              ; 1:02e6e
    mov byte ptr [edi], 6                                      ; 1:02e73
    mov byte ptr [edi + 0x50], 8                               ; 1:02e76
    mov byte ptr [edi + 0xa0], 8                               ; 1:02e7a
    mov byte ptr [edi + 0xf0], 6                               ; 1:02e81
    call loc_2b30                                              ; 1:02e88
    mov byte ptr [edi + 0x50], 6                               ; 1:02e8d
    mov byte ptr [edi + 0xa0], 6                               ; 1:02e91
    pop edx                                                    ; 1:02e98
    pop ecx                                                    ; 1:02e99
loc_2e9a:
    pop edi                                                    ; 1:02e9a
    ret                                                        ; 1:02e9b
mode_x_effect_2e9c_:
    push edi                                                   ; 1:02e9c
    cmp eax, 0                                                 ; 1:02e9d
    jl loc_2f0a                                                ; 1:02ea0
    cmp eax, 0x140                                             ; 1:02ea2
    ja loc_2f0a                                                ; 1:02ea7
    cmp ebx, 0                                                 ; 1:02ea9
    jl loc_2f0a                                                ; 1:02eac
    cmp ebx, 0xc8                                              ; 1:02eae
    ja loc_2f0a                                                ; 1:02eb4
    push ecx                                                   ; 1:02eb6
    push edx                                                   ; 1:02eb7
    mov edi, eax                                               ; 1:02eb8
    and eax, 3                                                 ; 1:02eba
    mov ah, 1                                                  ; 1:02ebd
    mov cl, al                                                 ; 1:02ebf
    shl ah, cl                                                 ; 1:02ec1
    mov dx, 0x3c4                                              ; 1:02ec3
    mov al, 2                                                  ; 1:02ec7
    out dx, ax                                                 ; 1:02ec9
    sar edi, 2                                                 ; 1:02ecb
    mov ecx, ebx                                               ; 1:02ece
    shl ecx, 4                                                 ; 1:02ed0
    shl ebx, 6                                                 ; 1:02ed3
    add ecx, ebx                                               ; 1:02ed6
    add edi, ecx                                               ; 1:02ed8
    add edi, dword ptr [esp + 8]                               ; 1:02eda
    mov byte ptr [edi + 0x50], 3                               ; 1:02ede
    mov byte ptr [edi + 0xa0], 6                               ; 1:02ee2
    call loc_2b30                                              ; 1:02ee9
    mov byte ptr [edi], 2                                      ; 1:02eee
    mov byte ptr [edi + 0x50], 5                               ; 1:02ef1
    mov byte ptr [edi + 0xa0], 3                               ; 1:02ef5
    call loc_2b30                                              ; 1:02efc
    mov byte ptr [edi], 4                                      ; 1:02f01
    mov byte ptr [edi + 0x50], 2                               ; 1:02f04
    pop edx                                                    ; 1:02f08
    pop ecx                                                    ; 1:02f09
loc_2f0a:
    pop edi                                                    ; 1:02f0a
    ret                                                        ; 1:02f0b
mode_x_effect_2f0c_:
    push edi                                                   ; 1:02f0c
    cmp eax, 0                                                 ; 1:02f0d
    jl loc_300d                                                ; 1:02f10
    cmp eax, 0x140                                             ; 1:02f16
    ja loc_300d                                                ; 1:02f1b
    cmp ebx, 0                                                 ; 1:02f21
    jl loc_300d                                                ; 1:02f24
    cmp ebx, 0xc8                                              ; 1:02f2a
    ja loc_300d                                                ; 1:02f30
    push ecx                                                   ; 1:02f36
    push edx                                                   ; 1:02f37
    mov edi, eax                                               ; 1:02f38
    and eax, 3                                                 ; 1:02f3a
    mov ah, 1                                                  ; 1:02f3d
    mov cl, al                                                 ; 1:02f3f
    shl ah, cl                                                 ; 1:02f41
    mov dx, 0x3c4                                              ; 1:02f43
    mov al, 2                                                  ; 1:02f47
    out dx, ax                                                 ; 1:02f49
    sar edi, 2                                                 ; 1:02f4b
    mov ecx, ebx                                               ; 1:02f4e
    shl ecx, 4                                                 ; 1:02f50
    shl ebx, 6                                                 ; 1:02f53
    add ecx, ebx                                               ; 1:02f56
    add edi, ecx                                               ; 1:02f58
    add edi, dword ptr [esp + 8]                               ; 1:02f5a
    mov byte ptr [edi + 0xa0], 6                               ; 1:02f5e
    mov byte ptr [edi + 0xf0], 6                               ; 1:02f65
    call loc_2b30                                              ; 1:02f6c
    mov byte ptr [edi + 0x50], 7                               ; 1:02f71
    mov byte ptr [edi + 0xa0], 8                               ; 1:02f75
    mov byte ptr [edi + 0xf0], 8                               ; 1:02f7c
    mov byte ptr [edi + 0x140], 7                              ; 1:02f83
    call loc_2b30                                              ; 1:02f8a
    mov byte ptr [edi], 6                                      ; 1:02f8f
    mov byte ptr [edi + 0x50], 8                               ; 1:02f92
    mov byte ptr [edi + 0xa0], 8                               ; 1:02f96
    mov byte ptr [edi + 0xf0], 8                               ; 1:02f9d
    mov byte ptr [edi + 0x140], 8                              ; 1:02fa4
    mov byte ptr [edi + 0x190], 6                              ; 1:02fab
    call loc_2b30                                              ; 1:02fb2
    mov byte ptr [edi], 6                                      ; 1:02fb7
    mov byte ptr [edi + 0x50], 8                               ; 1:02fba
    mov byte ptr [edi + 0xa0], 8                               ; 1:02fbe
    mov byte ptr [edi + 0xf0], 8                               ; 1:02fc5
    mov byte ptr [edi + 0x140], 8                              ; 1:02fcc
    mov byte ptr [edi + 0x190], 6                              ; 1:02fd3
    call loc_2b30                                              ; 1:02fda
    mov byte ptr [edi + 0x50], 7                               ; 1:02fdf
    mov byte ptr [edi + 0xa0], 8                               ; 1:02fe3
    mov byte ptr [edi + 0xf0], 8                               ; 1:02fea
    mov byte ptr [edi + 0x140], 7                              ; 1:02ff1
    call loc_2b30                                              ; 1:02ff8
    mov byte ptr [edi + 0xa0], 6                               ; 1:02ffd
    mov byte ptr [edi + 0xf0], 6                               ; 1:03004
    pop edx                                                    ; 1:0300b
    pop ecx                                                    ; 1:0300c
loc_300d:
    pop edi                                                    ; 1:0300d
    ret                                                        ; 1:0300e
mode_x_effect_300f_:
    push edi                                                   ; 1:0300f
    cmp eax, 0                                                 ; 1:03010
    jl loc_309b                                                ; 1:03013
    cmp eax, 0x140                                             ; 1:03019
    ja loc_309b                                                ; 1:0301e
    cmp ebx, 0                                                 ; 1:03020
    jl loc_309b                                                ; 1:03023
    cmp ebx, 0xc8                                              ; 1:03025
    ja loc_309b                                                ; 1:0302b
    push ecx                                                   ; 1:0302d
    push edx                                                   ; 1:0302e
    mov edi, eax                                               ; 1:0302f
    and eax, 3                                                 ; 1:03031
    mov ah, 1                                                  ; 1:03034
    mov cl, al                                                 ; 1:03036
    shl ah, cl                                                 ; 1:03038
    mov dx, 0x3c4                                              ; 1:0303a
    mov al, 2                                                  ; 1:0303e
    out dx, ax                                                 ; 1:03040
    sar edi, 2                                                 ; 1:03042
    mov ecx, ebx                                               ; 1:03045
    shl ecx, 4                                                 ; 1:03047
    shl ebx, 6                                                 ; 1:0304a
    add ecx, ebx                                               ; 1:0304d
    add edi, ecx                                               ; 1:0304f
    add edi, dword ptr [esp + 8]                               ; 1:03051
    mov byte ptr [edi + 0xa0], 4                               ; 1:03055
    mov byte ptr [edi + 0xf0], 6                               ; 1:0305c
    call loc_2b30                                              ; 1:03063
    mov byte ptr [edi + 0x50], 3                               ; 1:03068
    mov byte ptr [edi + 0xa0], 5                               ; 1:0306c
    mov byte ptr [edi + 0xf0], 4                               ; 1:03073
    call loc_2b30                                              ; 1:0307a
    mov byte ptr [edi], 2                                      ; 1:0307f
    mov byte ptr [edi + 0x50], 4                               ; 1:03082
    mov byte ptr [edi + 0xa0], 3                               ; 1:03086
    call loc_2b30                                              ; 1:0308d
    mov byte ptr [edi], 3                                      ; 1:03092
    mov byte ptr [edi + 0x50], 2                               ; 1:03095
    pop edx                                                    ; 1:03099
    pop ecx                                                    ; 1:0309a
loc_309b:
    pop edi                                                    ; 1:0309b
    ret                                                        ; 1:0309c
asm_0309d:
    push edi                                                   ; 1:0309d
    push ecx                                                   ; 1:0309e
    push edx                                                   ; 1:0309f
    mov edi, eax                                               ; 1:030a0
    and eax, 3                                                 ; 1:030a2
    mov ah, 1                                                  ; 1:030a5
    mov cl, al                                                 ; 1:030a7
    shl ah, cl                                                 ; 1:030a9
    mov dx, 0x3c4                                              ; 1:030ab
    mov al, 2                                                  ; 1:030af
    out dx, ax                                                 ; 1:030b1
    sar edi, 2                                                 ; 1:030b3
    mov ecx, ebx                                               ; 1:030b6
    shl ecx, 4                                                 ; 1:030b8
    shl ebx, 6                                                 ; 1:030bb
    add ecx, ebx                                               ; 1:030be
    add edi, ecx                                               ; 1:030c0
    add edi, dword ptr [esp + 8]                               ; 1:030c2
    mov byte ptr [edi + 0x50], 0xc                             ; 1:030c6
    mov byte ptr [edi + 0xa0], 0                               ; 1:030ca
    call loc_2b30                                              ; 1:030d1
    mov byte ptr [edi], 0xc                                    ; 1:030d6
    mov byte ptr [edi + 0x50], 0xe                             ; 1:030d9
    mov byte ptr [edi + 0xa0], 0xa                             ; 1:030dd
    mov byte ptr [edi + 0xf0], 0                               ; 1:030e4
    call loc_2b30                                              ; 1:030eb
    mov byte ptr [edi], 0                                      ; 1:030f0
    mov byte ptr [edi + 0x50], 0xa                             ; 1:030f3
    mov byte ptr [edi + 0xa0], 9                               ; 1:030f7
    mov byte ptr [edi + 0xf0], 0                               ; 1:030fe
    call loc_2b30                                              ; 1:03105
    mov byte ptr [edi + 0x50], 0                               ; 1:0310a
    mov byte ptr [edi + 0xa0], 0                               ; 1:0310e
    pop edx                                                    ; 1:03115
    pop ecx                                                    ; 1:03116
    pop edi                                                    ; 1:03117
    ret                                                        ; 1:03118

; OPEN classification at 1:03119..0311d: two unreachable zero pairs follow the 1:03118 RET.
; cfg.dis decodes each pair as ADD byte ptr [EAX], AL; retain those readable spellings.
    add byte ptr [eax], al
    add byte ptr [eax], al
; Proven 4-byte lookup masks: read bytewise at 1:03159 and 1:0316d (cfg.dis).
picture_mask_by_plane:
    db 0fh, 0eh, 0ch, 08h
picture_mask_by_pixel:
    db 0fh, 01h, 03h, 07h
_TEXT ends
end
