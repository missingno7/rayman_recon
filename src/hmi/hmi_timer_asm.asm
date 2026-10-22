; ORIGINAL_ASM: DOS v1.21 lock-delimited timer_asm group.
; Names INVENTED unless already canonical. Original assembler/version OPEN.
; Pinned WASM reproducer; whole CODE contribution begins at a public.
; NOP islands are observed original instructions, not invented padding.
.386p
.model flat
_TEXT segment para public use32 'CODE'
assume ds:FLAT

; Packed storage-field displacements; one state symbol at its true start.
TIMER_CALLBACK equ 0ch
TIMER_CALLBACK_SELECTOR equ 010h
TIMER_CALLBACK_DS equ 014h
TIMER_PRIVATE_STACK equ 016h
TIMER_PRIVATE_SS equ 01ah
TIMER_INTERRUPTED_STACK equ 01eh
TIMER_INTERRUPTED_SS equ 022h
TIMER_SAVED_ESP equ 01026h
TIMER_SAVED_EAX equ 0102ah
TIMER_IN_IRQ equ 0102eh
TIMER_ENABLED equ 01030h
TIMER_USE_DPMI_VECTOR equ 01034h
TIMER_PREVIOUS_SELECTOR equ 04h

public hmi_timer_asm_begin_
public hmi_timer_irq0_
public hmi_timer_call_callback_
public hmi_timer_set_divisor_
public hmi_timer_install_irq0_
public hmi_timer_mask_irq0_
public hmi_timer_unmask_irq0_
public hmi_timer_restore_irq0_
public hmi_timer_chain_irq0_
public hmi_timer_asm_end_
extrn __GETDS:near
; Default keeps the previously STRICT external-reference proof.
; Define HMI_DEFINE_TIMER_DATA only for the owned-data audit.
; The current matcher cannot certify code references into a data
; segment containing a relocation; see NOTES.md.
ifndef HMI_DEFINE_TIMER_DATA
extrn _hmi_timer_asm_state:byte
endif

; 1:784ac: timer_asm_begin
hmi_timer_asm_begin_:
    ret ; 1:784ac

; 1:784ad: timer_irq0
hmi_timer_irq0_:
    push ds ; 1:784ad
    call __GETDS ; 1:784ae
    cmp word ptr [_hmi_timer_asm_state + TIMER_IN_IRQ], 1 ; 1:784b3
    je short timer_asm_at_78508 ; 1:784bb
    nop ; 1:784bd
    nop ; 1:784be
    nop ; 1:784bf
    nop ; 1:784c0
    mov word ptr [_hmi_timer_asm_state + TIMER_IN_IRQ], 1 ; 1:784c1
    mov dword ptr [_hmi_timer_asm_state + TIMER_INTERRUPTED_STACK], esp ; 1:784ca
    mov word ptr [_hmi_timer_asm_state + TIMER_INTERRUPTED_SS], ss ; 1:784d0
    lss esp, fword ptr [_hmi_timer_asm_state + TIMER_PRIVATE_STACK] ; 1:784d7
    pushad ; 1:784de
    push es ; 1:784df
    push fs ; 1:784e0
    push gs ; 1:784e2
    push ds ; 1:784e4
    pop es ; 1:784e5
    mov dword ptr [_hmi_timer_asm_state + TIMER_SAVED_ESP], esp ; 1:784e6
    call fword ptr [_hmi_timer_asm_state + TIMER_CALLBACK] ; 1:784ec
    pop gs ; 1:784f2
    pop fs ; 1:784f4
    pop es ; 1:784f6
    popad ; 1:784f7
    lss esp, fword ptr [_hmi_timer_asm_state + TIMER_INTERRUPTED_STACK] ; 1:784f8
    mov word ptr [_hmi_timer_asm_state + TIMER_IN_IRQ], 0 ; 1:784ff

; 1:78508: internal control-flow target
timer_asm_at_78508:
    pop ds ; 1:78508
    push ax ; 1:78509
    mov ax, 020h ; 1:7850b
    out 020h, al ; 1:7850f
    pop ax ; 1:78511
    iretd ; 1:78513

; 1:78514: timer_call_callback
hmi_timer_call_callback_:
    pushad ; 1:78514
    push ds ; 1:78515
    push es ; 1:78516
    push fs ; 1:78517
    push gs ; 1:78519
    call fword ptr [_hmi_timer_asm_state + TIMER_CALLBACK] ; 1:7851b
    pop gs ; 1:78521
    pop fs ; 1:78523
    pop es ; 1:78525
    pop ds ; 1:78526
    popad ; 1:78527
    ret ; 1:78528

; 1:78529: timer_set_divisor
hmi_timer_set_divisor_:
    push ebp ; 1:78529
    mov ebp, esp ; 1:7852a
    push esi ; 1:7852c
    push edi ; 1:7852d
    push ebx ; 1:7852e
    push ecx ; 1:7852f
    push fs ; 1:78530
    push gs ; 1:78532
    push es ; 1:78534
    cmp dword ptr [_hmi_timer_asm_state + TIMER_ENABLED], 0 ; 1:78535
    je short timer_asm_at_78564 ; 1:7853c
    nop ; 1:7853e
    nop ; 1:7853f
    nop ; 1:78540
    nop ; 1:78541
    mov dx, 021h ; 1:78542
    in al, dx ; 1:78546
    or al, 1 ; 1:78547
    out dx, al ; 1:78549
    mov dx, 043h ; 1:7854a
    mov al, 036h ; 1:7854e
    out dx, al ; 1:78550
    mov dx, 040h ; 1:78551
    mov eax, dword ptr [ebp + 8] ; 1:78555
    out dx, al ; 1:78558
    mov al, ah ; 1:78559
    out dx, al ; 1:7855b
    mov dx, 021h ; 1:7855c
    in al, dx ; 1:78560
    and al, 0feh ; 1:78561
    out dx, al ; 1:78563

; 1:78564: internal control-flow target
timer_asm_at_78564:
    pop es ; 1:78564
    pop gs ; 1:78565
    pop fs ; 1:78567
    pop ecx ; 1:78569
    pop ebx ; 1:7856a
    pop edi ; 1:7856b
    pop esi ; 1:7856c
    pop ebp ; 1:7856d
    ret ; 1:7856e

; 1:7856f: timer_install_irq0
hmi_timer_install_irq0_:
    push ebp ; 1:7856f
    mov ebp, esp ; 1:78570
    push esi ; 1:78572
    push edi ; 1:78573
    push ebx ; 1:78574
    push ecx ; 1:78575
    push fs ; 1:78576
    push gs ; 1:78578
    push es ; 1:7857a
    les edi, fword ptr [ebp + 0ch] ; 1:7857b
    mov dword ptr [_hmi_timer_asm_state + TIMER_CALLBACK], edi ; 1:7857e
    mov word ptr [_hmi_timer_asm_state + TIMER_CALLBACK_SELECTOR], es ; 1:78584
    mov word ptr [_hmi_timer_asm_state + TIMER_CALLBACK_DS], ds ; 1:7858b
    mov word ptr [_hmi_timer_asm_state + TIMER_PRIVATE_SS], ds ; 1:78592
    cmp dword ptr [_hmi_timer_asm_state + TIMER_ENABLED], 0 ; 1:78599
    je near ptr timer_asm_at_78626 ; 1:785a0
    mov dx, 021h ; 1:785a6
    in al, dx ; 1:785aa
    or al, 1 ; 1:785ab
    out dx, al ; 1:785ad
    cmp dword ptr [_hmi_timer_asm_state + TIMER_USE_DPMI_VECTOR], 0 ; 1:785ae
    je short timer_asm_at_785e5 ; 1:785b5
    nop ; 1:785b7
    nop ; 1:785b8
    nop ; 1:785b9
    nop ; 1:785ba
    mov ax, 0204h ; 1:785bb
    mov bl, 8 ; 1:785bf
    int 031h ; 1:785c1
    mov dword ptr [_hmi_timer_asm_state], edx ; 1:785c3
    mov word ptr [_hmi_timer_asm_state + TIMER_PREVIOUS_SELECTOR], cx ; 1:785c9
    mov ax, 0205h ; 1:785d0
    mov bl, 8 ; 1:785d4
    mov edx, offset hmi_timer_irq0_ ; 1:785d6
    mov cx, cs ; 1:785db
    int 031h ; 1:785de
    jmp short timer_asm_at_7860c ; 1:785e0
    nop ; 1:785e2
    nop ; 1:785e3
    nop ; 1:785e4

; 1:785e5: internal control-flow target
timer_asm_at_785e5:
    mov ax, 03508h ; 1:785e5
    int 021h ; 1:785e9
    mov dword ptr [_hmi_timer_asm_state], ebx ; 1:785eb
    mov word ptr [_hmi_timer_asm_state + TIMER_PREVIOUS_SELECTOR], es ; 1:785f1
    push ds ; 1:785f8
    xor ax, ax ; 1:785f9
    mov es, ax ; 1:785fc
    mov ax, 02508h ; 1:785fe
    mov edx, offset hmi_timer_irq0_ ; 1:78602
    push cs ; 1:78607
    pop ds ; 1:78608
    int 021h ; 1:78609
    pop ds ; 1:7860b

; 1:7860c: internal control-flow target
timer_asm_at_7860c:
    mov dx, 043h ; 1:7860c
    mov al, 036h ; 1:78610
    out dx, al ; 1:78612
    mov dx, 040h ; 1:78613
    mov eax, dword ptr [ebp + 8] ; 1:78617
    out dx, al ; 1:7861a
    mov al, ah ; 1:7861b
    out dx, al ; 1:7861d
    mov dx, 021h ; 1:7861e
    in al, dx ; 1:78622
    and al, 0feh ; 1:78623
    out dx, al ; 1:78625

; 1:78626: internal control-flow target
timer_asm_at_78626:
    pop es ; 1:78626
    pop gs ; 1:78627
    pop fs ; 1:78629
    pop ecx ; 1:7862b
    pop ebx ; 1:7862c
    pop edi ; 1:7862d
    pop esi ; 1:7862e
    pop ebp ; 1:7862f
    ret ; 1:78630

; 1:78631: timer_mask_irq0
hmi_timer_mask_irq0_:
    push ebp ; 1:78631
    mov ebp, esp ; 1:78632
    push esi ; 1:78634
    push edi ; 1:78635
    push ebx ; 1:78636
    push ecx ; 1:78637
    push fs ; 1:78638
    push gs ; 1:7863a
    push es ; 1:7863c
    cmp dword ptr [_hmi_timer_asm_state + TIMER_ENABLED], 0 ; 1:7863d
    je short timer_asm_at_78652 ; 1:78644
    nop ; 1:78646
    nop ; 1:78647
    nop ; 1:78648
    nop ; 1:78649
    mov dx, 021h ; 1:7864a
    in al, dx ; 1:7864e
    or al, 1 ; 1:7864f
    out dx, al ; 1:78651

; 1:78652: internal control-flow target
timer_asm_at_78652:
    pop es ; 1:78652
    pop gs ; 1:78653
    pop fs ; 1:78655
    pop ecx ; 1:78657
    pop ebx ; 1:78658
    pop edi ; 1:78659
    pop esi ; 1:7865a
    pop ebp ; 1:7865b
    ret ; 1:7865c

; 1:7865d: timer_unmask_irq0
hmi_timer_unmask_irq0_:
    push ebp ; 1:7865d
    mov ebp, esp ; 1:7865e
    push esi ; 1:78660
    push edi ; 1:78661
    push ebx ; 1:78662
    push ecx ; 1:78663
    push fs ; 1:78664
    push gs ; 1:78666
    push es ; 1:78668
    cmp dword ptr [_hmi_timer_asm_state + TIMER_ENABLED], 0 ; 1:78669
    je short timer_asm_at_7867e ; 1:78670
    nop ; 1:78672
    nop ; 1:78673
    nop ; 1:78674
    nop ; 1:78675
    mov dx, 021h ; 1:78676
    in al, dx ; 1:7867a
    and al, 0feh ; 1:7867b
    out dx, al ; 1:7867d

; 1:7867e: internal control-flow target
timer_asm_at_7867e:
    pop es ; 1:7867e
    pop gs ; 1:7867f
    pop fs ; 1:78681
    pop ecx ; 1:78683
    pop ebx ; 1:78684
    pop edi ; 1:78685
    pop esi ; 1:78686
    pop ebp ; 1:78687
    ret ; 1:78688

; 1:78689: timer_restore_irq0
hmi_timer_restore_irq0_:
    push ebp ; 1:78689
    mov ebp, esp ; 1:7868a
    push esi ; 1:7868c
    push edi ; 1:7868d
    push ebx ; 1:7868e
    push ecx ; 1:7868f
    push fs ; 1:78690
    push gs ; 1:78692
    push es ; 1:78694
    cmp dword ptr [_hmi_timer_asm_state + TIMER_ENABLED], 0 ; 1:78695
    je short timer_asm_at_786fa ; 1:7869c
    nop ; 1:7869e
    nop ; 1:7869f
    nop ; 1:786a0
    nop ; 1:786a1
    mov dx, 021h ; 1:786a2
    in al, dx ; 1:786a6
    or al, 1 ; 1:786a7
    out dx, al ; 1:786a9
    cmp dword ptr [_hmi_timer_asm_state + TIMER_USE_DPMI_VECTOR], 0 ; 1:786aa
    je short timer_asm_at_786d1 ; 1:786b1
    nop ; 1:786b3
    nop ; 1:786b4
    nop ; 1:786b5
    nop ; 1:786b6
    mov ax, 0205h ; 1:786b7
    mov bl, 8 ; 1:786bb
    mov cx, word ptr [_hmi_timer_asm_state + TIMER_PREVIOUS_SELECTOR] ; 1:786bd
    mov edx, dword ptr [_hmi_timer_asm_state] ; 1:786c4
    int 031h ; 1:786ca
    jmp short timer_asm_at_786e8 ; 1:786cc
    nop ; 1:786ce
    nop ; 1:786cf
    nop ; 1:786d0

; 1:786d1: internal control-flow target
timer_asm_at_786d1:
    push ds ; 1:786d1
    mov ax, 02508h ; 1:786d2
    mov cx, word ptr [_hmi_timer_asm_state + TIMER_PREVIOUS_SELECTOR] ; 1:786d6
    mov edx, dword ptr [_hmi_timer_asm_state] ; 1:786dd
    mov ds, cx ; 1:786e3
    int 021h ; 1:786e5
    pop ds ; 1:786e7

; 1:786e8: internal control-flow target
timer_asm_at_786e8:
    mov dx, 040h ; 1:786e8
    mov ax, 0 ; 1:786ec
    out dx, al ; 1:786f0
    out dx, al ; 1:786f1
    mov dx, 021h ; 1:786f2
    in al, dx ; 1:786f6
    and al, 0feh ; 1:786f7
    out dx, al ; 1:786f9

; 1:786fa: internal control-flow target
timer_asm_at_786fa:
    pop es ; 1:786fa
    pop gs ; 1:786fb
    pop fs ; 1:786fd
    pop ecx ; 1:786ff
    pop ebx ; 1:78700
    pop edi ; 1:78701
    pop esi ; 1:78702
    pop ebp ; 1:78703
    ret ; 1:78704

; 1:78705: timer_chain_irq0
hmi_timer_chain_irq0_:
    cmp dword ptr [_hmi_timer_asm_state + TIMER_ENABLED], 0 ; 1:78705
    je short timer_asm_at_7875d ; 1:7870c
    nop ; 1:7870e
    nop ; 1:7870f
    nop ; 1:78710
    nop ; 1:78711
    mov word ptr [_hmi_timer_asm_state + TIMER_IN_IRQ], 0 ; 1:78712
    mov esp, dword ptr [_hmi_timer_asm_state + TIMER_SAVED_ESP] ; 1:7871b
    pop gs ; 1:78721
    pop fs ; 1:78723
    pop es ; 1:78725
    popad ; 1:78726
    lss esp, fword ptr [_hmi_timer_asm_state + TIMER_INTERRUPTED_STACK] ; 1:78727
    mov dword ptr [_hmi_timer_asm_state + TIMER_SAVED_EAX], eax ; 1:7872e
    pop eax ; 1:78733
    mov word ptr [_hmi_timer_asm_state + TIMER_SAVED_ESP], ax ; 1:78734
    push eax ; 1:7873a
    push eax ; 1:7873b
    mov eax, dword ptr [_hmi_timer_asm_state] ; 1:7873c
    mov dword ptr [esp], eax ; 1:78741
    mov eax, dword ptr [_hmi_timer_asm_state + TIMER_PREVIOUS_SELECTOR] ; 1:78744
    mov dword ptr [esp + 4], eax ; 1:78749
    mov eax, dword ptr [_hmi_timer_asm_state + TIMER_SAVED_EAX] ; 1:7874d
    push eax ; 1:78752
    mov ax, word ptr [_hmi_timer_asm_state + TIMER_SAVED_ESP] ; 1:78753
    mov ds, ax ; 1:78759
    pop eax ; 1:7875b
    retf ; 1:7875c

; 1:7875d: internal control-flow target
timer_asm_at_7875d:
    ret ; 1:7875d

; 1:7875e: timer_asm_end
hmi_timer_asm_end_:
    ret ; 1:7875e

_TEXT ends
ifdef HMI_DEFINE_TIMER_DATA

; Object-2 lock interval 2:2ec2c..2:2fc68, plus its four-byte end marker.
; Storage field names are INVENTED. See hmi_local.h for the packed C view.
_DATA segment para public use32 'DATA'
public _hmi_timer_asm_data_begin, _hmi_timer_asm_state, _hmi_timer_asm_data_end
_hmi_timer_asm_data_begin dd 0
_hmi_timer_asm_state label byte
    dd 0                         ; previous IRQ0 offset, +00
    dw 0                         ; previous IRQ0 selector, +04
    db 6 dup (0)                 ; unidentified storage, +06
    dd 0                         ; callback offset, +0c
    dw 0                         ; callback selector, +10
    dw 0                         ; unidentified word, +12
    dw 0                         ; callback DS, +14
    dd offset _hmi_timer_asm_state + 01026h ; private stack top, +16
    dw 0                         ; private stack SS, +1a
    dw 0                         ; unidentified word, +1c
    dd 0                         ; interrupted ESP, +1e
    dw 0                         ; interrupted SS, +22
    dw 0                         ; unidentified word, +24
    db 4096 dup (0)              ; private IRQ stack, +26
    dd 0                         ; saved ESP / reused DS word, +1026
    dd 0                         ; saved EAX, +102a
    dw 0                         ; IRQ reentrancy flag, +102e
    dd 0                         ; timer enabled, +1030
    dd 0                         ; use DPMI vector API, +1034
_hmi_timer_asm_data_end dd 0
_DATA ends
endif

end
