; DOS low-level slice 1:03125..1:0442f, reconstructed from address-anchored CFG.
; All address comments identify original instruction starts.
; The public names follow config/symbols.json; unknown entries retain INVENTED asm names.
.386p
_TEXT segment para public use32 'CODE'
assume cs:_TEXT, ds:_TEXT
public draw_blackbox_, draw_borderbox_, asm_03257_, asm_037e4_, asm_03912_, asm_03ea1_
extrn asm_02b30_:near
extrn _mask_0311d:byte
extrn _mask_03121:byte

draw_blackbox_:
    pushad ; 1:03125
    cmp ebx, 0 ; 1:03126
    jle L_031eb ; 1:03129
    cmp ecx, 0 ; 1:0312f
    jle L_031eb ; 1:03132
    shl edx, 4 ; 1:03138
    add edi, edx ; 1:0313b
    shl edx, 2 ; 1:0313d
    add edi, edx ; 1:03140
    mov edx, 3c4h ; 1:03142
    mov al, 2 ; 1:03147
    out dx, al ; 1:03149
    inc edx ; 1:0314a
    add ecx, esi ; 1:0314b
    mov eax, esi ; 1:0314d
    shr esi, 2 ; 1:0314f
    and eax, 3 ; 1:03152
    je L_03186 ; 1:03155
    push edi ; 1:03157
    push ebx ; 1:03158
    mov al, byte ptr [eax + _mask_0311d] ; 1:03159
    mov ebx, ecx ; 1:0315f
    shr ebx, 2 ; 1:03161
    cmp ebx, esi ; 1:03164
    jne L_03175 ; 1:03166
    mov ebx, ecx ; 1:03168
    and ebx, 3 ; 1:0316a
    mov ah, byte ptr [ebx + _mask_03121] ; 1:0316d
    and al, ah ; 1:03173
L_03175:
    pop ebx ; 1:03175
    out dx, al ; 1:03176
    add edi, esi ; 1:03177
    mov eax, ebx ; 1:03179
L_0317b:
    mov byte ptr [edi], 0 ; 1:0317b
    add edi, 50h ; 1:0317e
    dec eax ; 1:03181
    jne L_0317b ; 1:03182
    pop edi ; 1:03184
    inc esi ; 1:03185
L_03186:
    mov eax, ecx ; 1:03186
    shr eax, 2 ; 1:03188
    sub eax, esi ; 1:0318b
    jl L_031eb ; 1:0318d
    je L_031cd ; 1:0318f
    push edi ; 1:03191
    push ecx ; 1:03192
    push ebx ; 1:03193
    add edi, esi ; 1:03194
    mov ecx, eax ; 1:03196
    mov esi, 50h ; 1:03198
    mov al, 0fh ; 1:0319d
    sub esi, ecx ; 1:0319f
    out dx, al ; 1:031a1
    xor eax, eax ; 1:031a2
    push edx ; 1:031a4
    mov edx, ecx ; 1:031a5
    shr ecx, 2 ; 1:031a7
    and edx, 3 ; 1:031aa
    jne L_031bc ; 1:031ad
    mov edx, ecx ; 1:031af
L_031b1:
    rep stosd ; 1:031b1
    mov ecx, edx ; 1:031b3
    add edi, esi ; 1:031b5
    dec ebx ; 1:031b7
    jne L_031b1 ; 1:031b8
    jmp L_031c9 ; 1:031ba
L_031bc:
    push ecx ; 1:031bc
    rep stosd ; 1:031bd
    mov ecx, edx ; 1:031bf
    rep stosb ; 1:031c1
    pop ecx ; 1:031c3
    add edi, esi ; 1:031c4
    dec ebx ; 1:031c6
    jne L_031bc ; 1:031c7
L_031c9:
    pop edx ; 1:031c9
    pop ebx ; 1:031ca
    pop ecx ; 1:031cb
    pop edi ; 1:031cc
L_031cd:
    mov eax, ecx ; 1:031cd
    shr ecx, 2 ; 1:031cf
    and eax, 3 ; 1:031d2
    je L_031eb ; 1:031d5
    mov al, byte ptr [eax + _mask_03121] ; 1:031d7
    add edi, ecx ; 1:031dd
    out dx, al ; 1:031df
    mov eax, ebx ; 1:031e0
L_031e2:
    mov byte ptr [edi], 0 ; 1:031e2
    add edi, 50h ; 1:031e5
    dec eax ; 1:031e8
    jne L_031e2 ; 1:031e9
L_031eb:
    popad ; 1:031eb
    ret ; 1:031ec
draw_borderbox_:
    push ebp ; 1:031ed
    push edi ; 1:031ee
    push ecx ; 1:031ef
    push edx ; 1:031f0
    mov edi, eax ; 1:031f1
    and eax, 3 ; 1:031f3
    mov ah, 1 ; 1:031f6
    mov cl, al ; 1:031f8
    shl ah, cl ; 1:031fa
    mov dx, 3c4h ; 1:031fc
    mov al, 2 ; 1:03200
    out dx, ax ; 1:03202
    sar edi, 2 ; 1:03204
    mov ecx, ebx ; 1:03207
    shl ecx, 4 ; 1:03209
    shl ebx, 6 ; 1:0320c
    add ecx, ebx ; 1:0320f
    add edi, ecx ; 1:03211
    add edi, dword ptr [esp + 8] ; 1:03213
    pop ebp ; 1:03217
    pop ecx ; 1:03218
    push edi ; 1:03219
    mov ebx, ecx ; 1:0321a
    inc ebx ; 1:0321c
L_0321d:
    mov byte ptr [edi], 0ffh ; 1:0321d
    add edi, 50h ; 1:03220
    dec ebx ; 1:03223
    jne L_0321d ; 1:03224
    pop edi ; 1:03226
    call asm_02b30_ ; 1:03227
    mov ebx, ecx ; 1:0322c
    dec ebp ; 1:0322e
    push ecx ; 1:0322f
    mov ebx, ecx ; 1:03230
    shl ecx, 6 ; 1:03232
    shl ebx, 4 ; 1:03235
    add ebx, ecx ; 1:03238
L_0323a:
    mov byte ptr [edi], 0ffh ; 1:0323a
    mov byte ptr [edi + ebx], 0ffh ; 1:0323d
    call asm_02b30_ ; 1:03241
    dec ebp ; 1:03246
    jne L_0323a ; 1:03247
    pop ecx ; 1:03249
    inc ecx ; 1:0324a
L_0324b:
    mov byte ptr [edi], 0ffh ; 1:0324b
    add edi, 50h ; 1:0324e
    dec ecx ; 1:03251
    jne L_0324b ; 1:03252
    pop edi ; 1:03254
    pop ebp ; 1:03255
    ret ; 1:03256
asm_03257_:
    push ebp ; 1:03257
    push dword ptr [ecx + 8] ; 1:03258
    push dword ptr [ecx + 4] ; 1:0325b
    push dword ptr [ecx] ; 1:0325e
    mov ebp, esi ; 1:03260
    mov ah, al ; 1:03262
    push edx ; 1:03264
    push eax ; 1:03265
    mov dx, 3c4h ; 1:03266
    mov al, 2 ; 1:0326a
    out dx, ax ; 1:0326c
    pop eax ; 1:0326e
    pop edx ; 1:0326f
    xor ecx, ecx ; 1:03270
    jmp L_03285 ; 1:03272
L_03274:
    add ebp, 80h ; 1:03274
    inc cl ; 1:0327a
    cmp cl, 0eh ; 1:0327c
    jge L_037dd ; 1:0327f
L_03285:
    push ecx ; 1:03285
    push ebp ; 1:03286
    mov ecx, dword ptr [ebp] ; 1:03287
    add ebp, 4 ; 1:0328a
L_0328d:
    cmp ecx, 0 ; 1:0328d
    je L_037d6 ; 1:03290
    push edi ; 1:03296
    push ecx ; 1:03297
    mov esi, dword ptr [ebp] ; 1:03298
    add esi, ebx ; 1:0329b
    cmp esi, dword ptr [esp + 14h] ; 1:0329d
    jb L_032a7 ; 1:032a1
    sub esi, dword ptr [esp + 18h] ; 1:032a3
L_032a7:
    add edi, dword ptr [ebp + 4] ; 1:032a7
    mov ecx, dword ptr [ebp + 8] ; 1:032aa
    push ebp ; 1:032ad
    push ebx ; 1:032ae
    push ecx ; 1:032af
    push edx ; 1:032b0
    test edi, 1 ; 1:032b1
    je L_0358b ; 1:032b7
    push ebp ; 1:032bd
    push ebx ; 1:032be
    push ecx ; 1:032bf
    push esi ; 1:032c0
    push edi ; 1:032c1
    mov ebp, edx ; 1:032c2
    add ebp, edx ; 1:032c4
    mov ecx, 50h ; 1:032c6
    mov ebx, 0a0h ; 1:032cb
    mov ah, byte ptr [esi] ; 1:032d0
    mov al, byte ptr [esi + edx] ; 1:032d2
    add esi, ebp ; 1:032d5
    mov byte ptr [edi], ah ; 1:032d7
    mov byte ptr [edi + ecx], al ; 1:032d9
    add edi, ebx ; 1:032dc
    mov ah, byte ptr [esi] ; 1:032de
    mov al, byte ptr [esi + edx] ; 1:032e0
    add esi, ebp ; 1:032e3
    mov byte ptr [edi], ah ; 1:032e5
    mov byte ptr [edi + ecx], al ; 1:032e7
    add edi, ebx ; 1:032ea
    mov ah, byte ptr [esi] ; 1:032ec
    mov al, byte ptr [esi + edx] ; 1:032ee
    add esi, ebp ; 1:032f1
    mov byte ptr [edi], ah ; 1:032f3
    mov byte ptr [edi + ecx], al ; 1:032f5
    add edi, ebx ; 1:032f8
    mov ah, byte ptr [esi] ; 1:032fa
    mov al, byte ptr [esi + edx] ; 1:032fc
    add esi, ebp ; 1:032ff
    mov byte ptr [edi], ah ; 1:03301
    mov byte ptr [edi + ecx], al ; 1:03303
    add edi, ebx ; 1:03306
    mov ah, byte ptr [esi] ; 1:03308
    mov al, byte ptr [esi + edx] ; 1:0330a
    add esi, ebp ; 1:0330d
    mov byte ptr [edi], ah ; 1:0330f
    mov byte ptr [edi + ecx], al ; 1:03311
    add edi, ebx ; 1:03314
    mov ah, byte ptr [esi] ; 1:03316
    mov al, byte ptr [esi + edx] ; 1:03318
    add esi, ebp ; 1:0331b
    mov byte ptr [edi], ah ; 1:0331d
    mov byte ptr [edi + ecx], al ; 1:0331f
    add edi, ebx ; 1:03322
    mov ah, byte ptr [esi] ; 1:03324
    mov al, byte ptr [esi + edx] ; 1:03326
    add esi, ebp ; 1:03329
    mov byte ptr [edi], ah ; 1:0332b
    mov byte ptr [edi + ecx], al ; 1:0332d
    add edi, ebx ; 1:03330
    mov ah, byte ptr [esi] ; 1:03332
    mov al, byte ptr [esi + edx] ; 1:03334
    add esi, ebp ; 1:03337
    mov byte ptr [edi], ah ; 1:03339
    mov byte ptr [edi + ecx], al ; 1:0333b
    pop edi ; 1:0333e
    pop esi ; 1:0333f
    pop ecx ; 1:03340
    pop ebx ; 1:03341
    pop ebp ; 1:03342
    push esi ; 1:03343
    push edi ; 1:03344
    push ecx ; 1:03345
    shl ecx, 2 ; 1:03346
    dec ecx ; 1:03349
    add esi, ecx ; 1:0334a
    add edi, ecx ; 1:0334c
    push ebp ; 1:0334e
    push ebx ; 1:0334f
    push ecx ; 1:03350
    push esi ; 1:03351
    push edi ; 1:03352
    mov ebp, edx ; 1:03353
    add ebp, edx ; 1:03355
    mov ecx, 50h ; 1:03357
    mov ebx, 0a0h ; 1:0335c
    mov ah, byte ptr [esi] ; 1:03361
    mov al, byte ptr [esi + edx] ; 1:03363
    add esi, ebp ; 1:03366
    mov byte ptr [edi], ah ; 1:03368
    mov byte ptr [edi + ecx], al ; 1:0336a
    add edi, ebx ; 1:0336d
    mov ah, byte ptr [esi] ; 1:0336f
    mov al, byte ptr [esi + edx] ; 1:03371
    add esi, ebp ; 1:03374
    mov byte ptr [edi], ah ; 1:03376
    mov byte ptr [edi + ecx], al ; 1:03378
    add edi, ebx ; 1:0337b
    mov ah, byte ptr [esi] ; 1:0337d
    mov al, byte ptr [esi + edx] ; 1:0337f
    add esi, ebp ; 1:03382
    mov byte ptr [edi], ah ; 1:03384
    mov byte ptr [edi + ecx], al ; 1:03386
    add edi, ebx ; 1:03389
    mov ah, byte ptr [esi] ; 1:0338b
    mov al, byte ptr [esi + edx] ; 1:0338d
    add esi, ebp ; 1:03390
    mov byte ptr [edi], ah ; 1:03392
    mov byte ptr [edi + ecx], al ; 1:03394
    add edi, ebx ; 1:03397
    mov ah, byte ptr [esi] ; 1:03399
    mov al, byte ptr [esi + edx] ; 1:0339b
    add esi, ebp ; 1:0339e
    mov byte ptr [edi], ah ; 1:033a0
    mov byte ptr [edi + ecx], al ; 1:033a2
    add edi, ebx ; 1:033a5
    mov ah, byte ptr [esi] ; 1:033a7
    mov al, byte ptr [esi + edx] ; 1:033a9
    add esi, ebp ; 1:033ac
    mov byte ptr [edi], ah ; 1:033ae
    mov byte ptr [edi + ecx], al ; 1:033b0
    add edi, ebx ; 1:033b3
    mov ah, byte ptr [esi] ; 1:033b5
    mov al, byte ptr [esi + edx] ; 1:033b7
    add esi, ebp ; 1:033ba
    mov byte ptr [edi], ah ; 1:033bc
    mov byte ptr [edi + ecx], al ; 1:033be
    add edi, ebx ; 1:033c1
    mov ah, byte ptr [esi] ; 1:033c3
    mov al, byte ptr [esi + edx] ; 1:033c5
    add esi, ebp ; 1:033c8
    mov byte ptr [edi], ah ; 1:033ca
    mov byte ptr [edi + ecx], al ; 1:033cc
    pop edi ; 1:033cf
    pop esi ; 1:033d0
    pop ecx ; 1:033d1
    pop ebx ; 1:033d2
    pop ebp ; 1:033d3
    pop ecx ; 1:033d4
    pop edi ; 1:033d5
    pop esi ; 1:033d6
    inc edi ; 1:033d7
    inc esi ; 1:033d8
    test edi, 2 ; 1:033d9
    je L_034b3 ; 1:033df
    push ebp ; 1:033e5
    push ebx ; 1:033e6
    push ecx ; 1:033e7
    push esi ; 1:033e8
    push edi ; 1:033e9
    mov ebp, edx ; 1:033ea
    add ebp, edx ; 1:033ec
    mov ecx, 50h ; 1:033ee
    mov ebx, 0a0h ; 1:033f3
    mov ax, word ptr [esi] ; 1:033f8
    mov bx, word ptr [esi + edx] ; 1:033fb
    add esi, ebp ; 1:033ff
    mov word ptr [edi], ax ; 1:03401
    mov word ptr [edi + ecx], bx ; 1:03404
    add edi, 0a0h ; 1:03408
    mov ax, word ptr [esi] ; 1:0340e
    mov bx, word ptr [esi + edx] ; 1:03411
    add esi, ebp ; 1:03415
    mov word ptr [edi], ax ; 1:03417
    mov word ptr [edi + ecx], bx ; 1:0341a
    add edi, 0a0h ; 1:0341e
    mov ax, word ptr [esi] ; 1:03424
    mov bx, word ptr [esi + edx] ; 1:03427
    add esi, ebp ; 1:0342b
    mov word ptr [edi], ax ; 1:0342d
    mov word ptr [edi + ecx], bx ; 1:03430
    add edi, 0a0h ; 1:03434
    mov ax, word ptr [esi] ; 1:0343a
    mov bx, word ptr [esi + edx] ; 1:0343d
    add esi, ebp ; 1:03441
    mov word ptr [edi], ax ; 1:03443
    mov word ptr [edi + ecx], bx ; 1:03446
    add edi, 0a0h ; 1:0344a
    mov ax, word ptr [esi] ; 1:03450
    mov bx, word ptr [esi + edx] ; 1:03453
    add esi, ebp ; 1:03457
    mov word ptr [edi], ax ; 1:03459
    mov word ptr [edi + ecx], bx ; 1:0345c
    add edi, 0a0h ; 1:03460
    mov ax, word ptr [esi] ; 1:03466
    mov bx, word ptr [esi + edx] ; 1:03469
    add esi, ebp ; 1:0346d
    mov word ptr [edi], ax ; 1:0346f
    mov word ptr [edi + ecx], bx ; 1:03472
    add edi, 0a0h ; 1:03476
    mov ax, word ptr [esi] ; 1:0347c
    mov bx, word ptr [esi + edx] ; 1:0347f
    add esi, ebp ; 1:03483
    mov word ptr [edi], ax ; 1:03485
    mov word ptr [edi + ecx], bx ; 1:03488
    add edi, 0a0h ; 1:0348c
    mov ax, word ptr [esi] ; 1:03492
    mov bx, word ptr [esi + edx] ; 1:03495
    add esi, ebp ; 1:03499
    mov word ptr [edi], ax ; 1:0349b
    mov word ptr [edi + ecx], bx ; 1:0349e
    pop edi ; 1:034a2
    pop esi ; 1:034a3
    pop ecx ; 1:034a4
    pop ebx ; 1:034a5
    pop ebp ; 1:034a6
    add esi, 2 ; 1:034a7
    add edi, 2 ; 1:034aa
    dec ecx ; 1:034ad
    jmp L_03732 ; 1:034ae
L_034b3:
    push esi ; 1:034b3
    push edi ; 1:034b4
    push ecx ; 1:034b5
    shl ecx, 2 ; 1:034b6
    sub ecx, 4 ; 1:034b9
    add esi, ecx ; 1:034bc
    add edi, ecx ; 1:034be
    push ebp ; 1:034c0
    push ebx ; 1:034c1
    push ecx ; 1:034c2
    push esi ; 1:034c3
    push edi ; 1:034c4
    mov ebp, edx ; 1:034c5
    add ebp, edx ; 1:034c7
    mov ecx, 50h ; 1:034c9
    mov ebx, 0a0h ; 1:034ce
    mov ax, word ptr [esi] ; 1:034d3
    mov bx, word ptr [esi + edx] ; 1:034d6
    add esi, ebp ; 1:034da
    mov word ptr [edi], ax ; 1:034dc
    mov word ptr [edi + ecx], bx ; 1:034df
    add edi, 0a0h ; 1:034e3
    mov ax, word ptr [esi] ; 1:034e9
    mov bx, word ptr [esi + edx] ; 1:034ec
    add esi, ebp ; 1:034f0
    mov word ptr [edi], ax ; 1:034f2
    mov word ptr [edi + ecx], bx ; 1:034f5
    add edi, 0a0h ; 1:034f9
    mov ax, word ptr [esi] ; 1:034ff
    mov bx, word ptr [esi + edx] ; 1:03502
    add esi, ebp ; 1:03506
    mov word ptr [edi], ax ; 1:03508
    mov word ptr [edi + ecx], bx ; 1:0350b
    add edi, 0a0h ; 1:0350f
    mov ax, word ptr [esi] ; 1:03515
    mov bx, word ptr [esi + edx] ; 1:03518
    add esi, ebp ; 1:0351c
    mov word ptr [edi], ax ; 1:0351e
    mov word ptr [edi + ecx], bx ; 1:03521
    add edi, 0a0h ; 1:03525
    mov ax, word ptr [esi] ; 1:0352b
    mov bx, word ptr [esi + edx] ; 1:0352e
    add esi, ebp ; 1:03532
    mov word ptr [edi], ax ; 1:03534
    mov word ptr [edi + ecx], bx ; 1:03537
    add edi, 0a0h ; 1:0353b
    mov ax, word ptr [esi] ; 1:03541
    mov bx, word ptr [esi + edx] ; 1:03544
    add esi, ebp ; 1:03548
    mov word ptr [edi], ax ; 1:0354a
    mov word ptr [edi + ecx], bx ; 1:0354d
    add edi, 0a0h ; 1:03551
    mov ax, word ptr [esi] ; 1:03557
    mov bx, word ptr [esi + edx] ; 1:0355a
    add esi, ebp ; 1:0355e
    mov word ptr [edi], ax ; 1:03560
    mov word ptr [edi + ecx], bx ; 1:03563
    add edi, 0a0h ; 1:03567
    mov ax, word ptr [esi] ; 1:0356d
    mov bx, word ptr [esi + edx] ; 1:03570
    add esi, ebp ; 1:03574
    mov word ptr [edi], ax ; 1:03576
    mov word ptr [edi + ecx], bx ; 1:03579
    pop edi ; 1:0357d
    pop esi ; 1:0357e
    pop ecx ; 1:0357f
    pop ebx ; 1:03580
    pop ebp ; 1:03581
    pop ecx ; 1:03582
    pop edi ; 1:03583
    pop esi ; 1:03584
    dec ecx ; 1:03585
    jmp L_03732 ; 1:03586
L_0358b:
    test edi, 2 ; 1:0358b
    je L_03732 ; 1:03591
    push ebp ; 1:03597
    push ebx ; 1:03598
    push ecx ; 1:03599
    push esi ; 1:0359a
    push edi ; 1:0359b
    mov ebp, edx ; 1:0359c
    add ebp, edx ; 1:0359e
    mov ecx, 50h ; 1:035a0
    mov ebx, 0a0h ; 1:035a5
    mov ax, word ptr [esi] ; 1:035aa
    mov bx, word ptr [esi + edx] ; 1:035ad
    add esi, ebp ; 1:035b1
    mov word ptr [edi], ax ; 1:035b3
    mov word ptr [edi + ecx], bx ; 1:035b6
    add edi, 0a0h ; 1:035ba
    mov ax, word ptr [esi] ; 1:035c0
    mov bx, word ptr [esi + edx] ; 1:035c3
    add esi, ebp ; 1:035c7
    mov word ptr [edi], ax ; 1:035c9
    mov word ptr [edi + ecx], bx ; 1:035cc
    add edi, 0a0h ; 1:035d0
    mov ax, word ptr [esi] ; 1:035d6
    mov bx, word ptr [esi + edx] ; 1:035d9
    add esi, ebp ; 1:035dd
    mov word ptr [edi], ax ; 1:035df
    mov word ptr [edi + ecx], bx ; 1:035e2
    add edi, 0a0h ; 1:035e6
    mov ax, word ptr [esi] ; 1:035ec
    mov bx, word ptr [esi + edx] ; 1:035ef
    add esi, ebp ; 1:035f3
    mov word ptr [edi], ax ; 1:035f5
    mov word ptr [edi + ecx], bx ; 1:035f8
    add edi, 0a0h ; 1:035fc
    mov ax, word ptr [esi] ; 1:03602
    mov bx, word ptr [esi + edx] ; 1:03605
    add esi, ebp ; 1:03609
    mov word ptr [edi], ax ; 1:0360b
    mov word ptr [edi + ecx], bx ; 1:0360e
    add edi, 0a0h ; 1:03612
    mov ax, word ptr [esi] ; 1:03618
    mov bx, word ptr [esi + edx] ; 1:0361b
    add esi, ebp ; 1:0361f
    mov word ptr [edi], ax ; 1:03621
    mov word ptr [edi + ecx], bx ; 1:03624
    add edi, 0a0h ; 1:03628
    mov ax, word ptr [esi] ; 1:0362e
    mov bx, word ptr [esi + edx] ; 1:03631
    add esi, ebp ; 1:03635
    mov word ptr [edi], ax ; 1:03637
    mov word ptr [edi + ecx], bx ; 1:0363a
    add edi, 0a0h ; 1:0363e
    mov ax, word ptr [esi] ; 1:03644
    mov bx, word ptr [esi + edx] ; 1:03647
    add esi, ebp ; 1:0364b
    mov word ptr [edi], ax ; 1:0364d
    mov word ptr [edi + ecx], bx ; 1:03650
    pop edi ; 1:03654
    pop esi ; 1:03655
    pop ecx ; 1:03656
    pop ebx ; 1:03657
    pop ebp ; 1:03658
    push esi ; 1:03659
    push edi ; 1:0365a
    push ecx ; 1:0365b
    shl ecx, 2 ; 1:0365c
    sub ecx, 2 ; 1:0365f
    add esi, ecx ; 1:03662
    add edi, ecx ; 1:03664
    push ebp ; 1:03666
    push ebx ; 1:03667
    push ecx ; 1:03668
    push esi ; 1:03669
    push edi ; 1:0366a
    mov ebp, edx ; 1:0366b
    add ebp, edx ; 1:0366d
    mov ecx, 50h ; 1:0366f
    mov ebx, 0a0h ; 1:03674
    mov ax, word ptr [esi] ; 1:03679
    mov bx, word ptr [esi + edx] ; 1:0367c
    add esi, ebp ; 1:03680
    mov word ptr [edi], ax ; 1:03682
    mov word ptr [edi + ecx], bx ; 1:03685
    add edi, 0a0h ; 1:03689
    mov ax, word ptr [esi] ; 1:0368f
    mov bx, word ptr [esi + edx] ; 1:03692
    add esi, ebp ; 1:03696
    mov word ptr [edi], ax ; 1:03698
    mov word ptr [edi + ecx], bx ; 1:0369b
    add edi, 0a0h ; 1:0369f
    mov ax, word ptr [esi] ; 1:036a5
    mov bx, word ptr [esi + edx] ; 1:036a8
    add esi, ebp ; 1:036ac
    mov word ptr [edi], ax ; 1:036ae
    mov word ptr [edi + ecx], bx ; 1:036b1
    add edi, 0a0h ; 1:036b5
    mov ax, word ptr [esi] ; 1:036bb
    mov bx, word ptr [esi + edx] ; 1:036be
    add esi, ebp ; 1:036c2
    mov word ptr [edi], ax ; 1:036c4
    mov word ptr [edi + ecx], bx ; 1:036c7
    add edi, 0a0h ; 1:036cb
    mov ax, word ptr [esi] ; 1:036d1
    mov bx, word ptr [esi + edx] ; 1:036d4
    add esi, ebp ; 1:036d8
    mov word ptr [edi], ax ; 1:036da
    mov word ptr [edi + ecx], bx ; 1:036dd
    add edi, 0a0h ; 1:036e1
    mov ax, word ptr [esi] ; 1:036e7
    mov bx, word ptr [esi + edx] ; 1:036ea
    add esi, ebp ; 1:036ee
    mov word ptr [edi], ax ; 1:036f0
    mov word ptr [edi + ecx], bx ; 1:036f3
    add edi, 0a0h ; 1:036f7
    mov ax, word ptr [esi] ; 1:036fd
    mov bx, word ptr [esi + edx] ; 1:03700
    add esi, ebp ; 1:03704
    mov word ptr [edi], ax ; 1:03706
    mov word ptr [edi + ecx], bx ; 1:03709
    add edi, 0a0h ; 1:0370d
    mov ax, word ptr [esi] ; 1:03713
    mov bx, word ptr [esi + edx] ; 1:03716
    add esi, ebp ; 1:0371a
    mov word ptr [edi], ax ; 1:0371c
    mov word ptr [edi + ecx], bx ; 1:0371f
    pop edi ; 1:03723
    pop esi ; 1:03724
    pop ecx ; 1:03725
    pop ebx ; 1:03726
    pop ebp ; 1:03727
    pop ecx ; 1:03728
    pop edi ; 1:03729
    pop esi ; 1:0372a
    dec ecx ; 1:0372b
    add esi, 2 ; 1:0372c
    add edi, 2 ; 1:0372f
L_03732:
    push esi ; 1:03732
    cmp ecx, 0 ; 1:03733
    je L_037c6 ; 1:03736
    mov ebx, ecx ; 1:0373c
    shl ebx, 2 ; 1:0373e
    mov eax, 50h ; 1:03741
    sub eax, ebx ; 1:03746
    sub edx, ebx ; 1:03748
    mov ebx, ecx ; 1:0374a
    rep movsd ; 1:0374c
    add esi, edx ; 1:0374e
    add edi, eax ; 1:03750
    mov ecx, ebx ; 1:03752
    rep movsd ; 1:03754
    add esi, edx ; 1:03756
    add edi, eax ; 1:03758
    mov ecx, ebx ; 1:0375a
    rep movsd ; 1:0375c
    add esi, edx ; 1:0375e
    add edi, eax ; 1:03760
    mov ecx, ebx ; 1:03762
    rep movsd ; 1:03764
    add esi, edx ; 1:03766
    add edi, eax ; 1:03768
    mov ecx, ebx ; 1:0376a
    rep movsd ; 1:0376c
    add esi, edx ; 1:0376e
    add edi, eax ; 1:03770
    mov ecx, ebx ; 1:03772
    rep movsd ; 1:03774
    add esi, edx ; 1:03776
    add edi, eax ; 1:03778
    mov ecx, ebx ; 1:0377a
    rep movsd ; 1:0377c
    add esi, edx ; 1:0377e
    add edi, eax ; 1:03780
    mov ecx, ebx ; 1:03782
    rep movsd ; 1:03784
    add esi, edx ; 1:03786
    add edi, eax ; 1:03788
    mov ecx, ebx ; 1:0378a
    rep movsd ; 1:0378c
    add esi, edx ; 1:0378e
    add edi, eax ; 1:03790
    mov ecx, ebx ; 1:03792
    rep movsd ; 1:03794
    add esi, edx ; 1:03796
    add edi, eax ; 1:03798
    mov ecx, ebx ; 1:0379a
    rep movsd ; 1:0379c
    add esi, edx ; 1:0379e
    add edi, eax ; 1:037a0
    mov ecx, ebx ; 1:037a2
    rep movsd ; 1:037a4
    add esi, edx ; 1:037a6
    add edi, eax ; 1:037a8
    mov ecx, ebx ; 1:037aa
    rep movsd ; 1:037ac
    add esi, edx ; 1:037ae
    add edi, eax ; 1:037b0
    mov ecx, ebx ; 1:037b2
    rep movsd ; 1:037b4
    add esi, edx ; 1:037b6
    add edi, eax ; 1:037b8
    mov ecx, ebx ; 1:037ba
    rep movsd ; 1:037bc
    add esi, edx ; 1:037be
    add edi, eax ; 1:037c0
    mov ecx, ebx ; 1:037c2
    rep movsd ; 1:037c4
L_037c6:
    pop esi ; 1:037c6
    pop edx ; 1:037c7
    pop ecx ; 1:037c8
    pop ebx ; 1:037c9
    pop ebp ; 1:037ca
    pop ecx ; 1:037cb
    pop edi ; 1:037cc
    dec ecx ; 1:037cd
    add ebp, 0ch ; 1:037ce
    jmp L_0328d ; 1:037d1
L_037d6:
    pop ebp ; 1:037d6
    pop ecx ; 1:037d7
    jmp L_03274 ; 1:037d8
L_037dd:
    pop esi ; 1:037dd
    pop esi ; 1:037de
    add esp, 4 ; 1:037df
    pop ebp ; 1:037e2
    ret ; 1:037e3
asm_037e4_:
    push ebp ; 1:037e4
    mov ebp, esi ; 1:037e5
    push edx ; 1:037e7
    push eax ; 1:037e8
    mov dx, 3ceh ; 1:037e9
    mov al, 5 ; 1:037ed
    out dx, al ; 1:037ef
    inc dx ; 1:037f0
    in al, dx ; 1:037f2
    and al, 0fch ; 1:037f3
    or al, 1 ; 1:037f5
    out dx, al ; 1:037f7
    pop eax ; 1:037f8
    pop edx ; 1:037f9
    mov dx, 3c4h ; 1:037fa
    mov al, 2 ; 1:037fe
    mov ah, 0fh ; 1:03800
    out dx, ax ; 1:03802
    mov edi, ecx ; 1:03804
    mov esi, 0ab000h ; 1:03806
    add edi, 502h ; 1:0380b
    add esi, 502h ; 1:03811
    mov ecx, 0b8h ; 1:03817
L_0381c:
    movsb ; 1:0381c
    movsb ; 1:0381d
    movsb ; 1:0381e
    movsb ; 1:0381f
    add edi, 4ch ; 1:03820
    add esi, 4ch ; 1:03823
    dec ecx ; 1:03826
    jne L_0381c ; 1:03827
    cli ; 1:03829
    xor ecx, ecx ; 1:0382a
    jmp L_0383f ; 1:0382c
L_0382e:
    add ebp, 80h ; 1:0382e
    inc cl ; 1:03834
    cmp cl, 0ch ; 1:03836
    jge L_038fe ; 1:03839
L_0383f:
    push ecx ; 1:0383f
    push ebp ; 1:03840
    mov ecx, dword ptr [ebp] ; 1:03841
    add ebp, 4 ; 1:03844
L_03847:
    cmp ecx, 0 ; 1:03847
    je L_038f7 ; 1:0384a
    push ecx ; 1:03850
    mov esi, dword ptr [ebp] ; 1:03851
    mov edi, dword ptr [ebp + 4] ; 1:03854
    mov ecx, dword ptr [ebp + 8] ; 1:03857
    or ecx, ecx ; 1:0385a
    je L_038ed ; 1:0385c
    shl ecx, 2 ; 1:03862
    add ecx, 1 ; 1:03865
    mov ebx, ecx ; 1:03868
    mov eax, 50h ; 1:0386a
    sub eax, ebx ; 1:0386f
    mov edx, eax ; 1:03871
    rep movsb ; 1:03873
    add esi, edx ; 1:03875
    add edi, eax ; 1:03877
    mov ecx, ebx ; 1:03879
    rep movsb ; 1:0387b
    add esi, edx ; 1:0387d
    add edi, eax ; 1:0387f
    mov ecx, ebx ; 1:03881
    rep movsb ; 1:03883
    add esi, edx ; 1:03885
    add edi, eax ; 1:03887
    mov ecx, ebx ; 1:03889
    rep movsb ; 1:0388b
    add esi, edx ; 1:0388d
    add edi, eax ; 1:0388f
    mov ecx, ebx ; 1:03891
    rep movsb ; 1:03893
    add esi, edx ; 1:03895
    add edi, eax ; 1:03897
    mov ecx, ebx ; 1:03899
    rep movsb ; 1:0389b
    add esi, edx ; 1:0389d
    add edi, eax ; 1:0389f
    mov ecx, ebx ; 1:038a1
    rep movsb ; 1:038a3
    add esi, edx ; 1:038a5
    add edi, eax ; 1:038a7
    mov ecx, ebx ; 1:038a9
    rep movsb ; 1:038ab
    add esi, edx ; 1:038ad
    add edi, eax ; 1:038af
    mov ecx, ebx ; 1:038b1
    rep movsb ; 1:038b3
    add esi, edx ; 1:038b5
    add edi, eax ; 1:038b7
    mov ecx, ebx ; 1:038b9
    rep movsb ; 1:038bb
    add esi, edx ; 1:038bd
    add edi, eax ; 1:038bf
    mov ecx, ebx ; 1:038c1
    rep movsb ; 1:038c3
    add esi, edx ; 1:038c5
    add edi, eax ; 1:038c7
    mov ecx, ebx ; 1:038c9
    rep movsb ; 1:038cb
    add esi, edx ; 1:038cd
    add edi, eax ; 1:038cf
    mov ecx, ebx ; 1:038d1
    rep movsb ; 1:038d3
    add esi, edx ; 1:038d5
    add edi, eax ; 1:038d7
    mov ecx, ebx ; 1:038d9
    rep movsb ; 1:038db
    add esi, edx ; 1:038dd
    add edi, eax ; 1:038df
    mov ecx, ebx ; 1:038e1
    rep movsb ; 1:038e3
    add esi, edx ; 1:038e5
    add edi, eax ; 1:038e7
    mov ecx, ebx ; 1:038e9
    rep movsb ; 1:038eb
L_038ed:
    pop ecx ; 1:038ed
    dec ecx ; 1:038ee
    add ebp, 0ch ; 1:038ef
    jmp L_03847 ; 1:038f2
L_038f7:
    pop ebp ; 1:038f7
    pop ecx ; 1:038f8
    jmp L_0382e ; 1:038f9
L_038fe:
    push edx ; 1:038fe
    push eax ; 1:038ff
    mov dx, 3ceh ; 1:03900
    mov al, 5 ; 1:03904
    out dx, al ; 1:03906
    inc dx ; 1:03907
    in al, dx ; 1:03909
    and al, 0fch ; 1:0390a
    out dx, al ; 1:0390c
    pop eax ; 1:0390d
    pop edx ; 1:0390e
    pop ebp ; 1:0390f
    sti ; 1:03910
    ret ; 1:03911
asm_03912_:
    push ebp ; 1:03912
    push dword ptr [ecx + 8] ; 1:03913
    push dword ptr [ecx + 4] ; 1:03916
    push dword ptr [ecx] ; 1:03919
    mov ebp, esi ; 1:0391b
    mov ah, al ; 1:0391d
    push edx ; 1:0391f
    push eax ; 1:03920
    mov dx, 3c4h ; 1:03921
    mov al, 2 ; 1:03925
    out dx, ax ; 1:03927
    pop eax ; 1:03929
    pop edx ; 1:0392a
    xor ecx, ecx ; 1:0392b
    mov cl, 0 ; 1:0392d
    jmp L_03942 ; 1:0392f
L_03931:
    add ebp, 80h ; 1:03931
    inc cl ; 1:03937
    cmp cl, 0eh ; 1:03939
    jge L_03e9a ; 1:0393c
L_03942:
    push ecx ; 1:03942
    push ebp ; 1:03943
    mov ecx, dword ptr [ebp] ; 1:03944
    add ebp, 4 ; 1:03947
L_0394a:
    cmp ecx, 0 ; 1:0394a
    je L_03e93 ; 1:0394d
    push edi ; 1:03953
    push ecx ; 1:03954
    mov esi, dword ptr [ebp] ; 1:03955
    add esi, ebx ; 1:03958
    cmp esi, dword ptr [esp + 14h] ; 1:0395a
    jb L_03964 ; 1:0395e
    sub esi, dword ptr [esp + 18h] ; 1:03960
L_03964:
    add edi, dword ptr [ebp + 4] ; 1:03964
    mov ecx, dword ptr [ebp + 8] ; 1:03967
    push ebp ; 1:0396a
    push ebx ; 1:0396b
    push ecx ; 1:0396c
    push edx ; 1:0396d
    test edi, 1 ; 1:0396e
    je L_03c48 ; 1:03974
    push ebp ; 1:0397a
    push ebx ; 1:0397b
    push ecx ; 1:0397c
    push esi ; 1:0397d
    push edi ; 1:0397e
    mov ebp, edx ; 1:0397f
    add ebp, edx ; 1:03981
    mov ecx, 50h ; 1:03983
    mov ebx, 0a0h ; 1:03988
    mov ah, byte ptr [esi] ; 1:0398d
    mov al, byte ptr [esi + edx] ; 1:0398f
    add esi, ebp ; 1:03992
    mov byte ptr [edi], ah ; 1:03994
    mov byte ptr [edi + ecx], al ; 1:03996
    add edi, ebx ; 1:03999
    mov ah, byte ptr [esi] ; 1:0399b
    mov al, byte ptr [esi + edx] ; 1:0399d
    add esi, ebp ; 1:039a0
    mov byte ptr [edi], ah ; 1:039a2
    mov byte ptr [edi + ecx], al ; 1:039a4
    add edi, ebx ; 1:039a7
    mov ah, byte ptr [esi] ; 1:039a9
    mov al, byte ptr [esi + edx] ; 1:039ab
    add esi, ebp ; 1:039ae
    mov byte ptr [edi], ah ; 1:039b0
    mov byte ptr [edi + ecx], al ; 1:039b2
    add edi, ebx ; 1:039b5
    mov ah, byte ptr [esi] ; 1:039b7
    mov al, byte ptr [esi + edx] ; 1:039b9
    add esi, ebp ; 1:039bc
    mov byte ptr [edi], ah ; 1:039be
    mov byte ptr [edi + ecx], al ; 1:039c0
    add edi, ebx ; 1:039c3
    mov ah, byte ptr [esi] ; 1:039c5
    mov al, byte ptr [esi + edx] ; 1:039c7
    add esi, ebp ; 1:039ca
    mov byte ptr [edi], ah ; 1:039cc
    mov byte ptr [edi + ecx], al ; 1:039ce
    add edi, ebx ; 1:039d1
    mov ah, byte ptr [esi] ; 1:039d3
    mov al, byte ptr [esi + edx] ; 1:039d5
    add esi, ebp ; 1:039d8
    mov byte ptr [edi], ah ; 1:039da
    mov byte ptr [edi + ecx], al ; 1:039dc
    add edi, ebx ; 1:039df
    mov ah, byte ptr [esi] ; 1:039e1
    mov al, byte ptr [esi + edx] ; 1:039e3
    add esi, ebp ; 1:039e6
    mov byte ptr [edi], ah ; 1:039e8
    mov byte ptr [edi + ecx], al ; 1:039ea
    add edi, ebx ; 1:039ed
    mov ah, byte ptr [esi] ; 1:039ef
    mov al, byte ptr [esi + edx] ; 1:039f1
    add esi, ebp ; 1:039f4
    mov byte ptr [edi], ah ; 1:039f6
    mov byte ptr [edi + ecx], al ; 1:039f8
    pop edi ; 1:039fb
    pop esi ; 1:039fc
    pop ecx ; 1:039fd
    pop ebx ; 1:039fe
    pop ebp ; 1:039ff
    push esi ; 1:03a00
    push edi ; 1:03a01
    push ecx ; 1:03a02
    shl ecx, 2 ; 1:03a03
    dec ecx ; 1:03a06
    add esi, ecx ; 1:03a07
    add edi, ecx ; 1:03a09
    push ebp ; 1:03a0b
    push ebx ; 1:03a0c
    push ecx ; 1:03a0d
    push esi ; 1:03a0e
    push edi ; 1:03a0f
    mov ebp, edx ; 1:03a10
    add ebp, edx ; 1:03a12
    mov ecx, 50h ; 1:03a14
    mov ebx, 0a0h ; 1:03a19
    mov ah, byte ptr [esi] ; 1:03a1e
    mov al, byte ptr [esi + edx] ; 1:03a20
    add esi, ebp ; 1:03a23
    mov byte ptr [edi], ah ; 1:03a25
    mov byte ptr [edi + ecx], al ; 1:03a27
    add edi, ebx ; 1:03a2a
    mov ah, byte ptr [esi] ; 1:03a2c
    mov al, byte ptr [esi + edx] ; 1:03a2e
    add esi, ebp ; 1:03a31
    mov byte ptr [edi], ah ; 1:03a33
    mov byte ptr [edi + ecx], al ; 1:03a35
    add edi, ebx ; 1:03a38
    mov ah, byte ptr [esi] ; 1:03a3a
    mov al, byte ptr [esi + edx] ; 1:03a3c
    add esi, ebp ; 1:03a3f
    mov byte ptr [edi], ah ; 1:03a41
    mov byte ptr [edi + ecx], al ; 1:03a43
    add edi, ebx ; 1:03a46
    mov ah, byte ptr [esi] ; 1:03a48
    mov al, byte ptr [esi + edx] ; 1:03a4a
    add esi, ebp ; 1:03a4d
    mov byte ptr [edi], ah ; 1:03a4f
    mov byte ptr [edi + ecx], al ; 1:03a51
    add edi, ebx ; 1:03a54
    mov ah, byte ptr [esi] ; 1:03a56
    mov al, byte ptr [esi + edx] ; 1:03a58
    add esi, ebp ; 1:03a5b
    mov byte ptr [edi], ah ; 1:03a5d
    mov byte ptr [edi + ecx], al ; 1:03a5f
    add edi, ebx ; 1:03a62
    mov ah, byte ptr [esi] ; 1:03a64
    mov al, byte ptr [esi + edx] ; 1:03a66
    add esi, ebp ; 1:03a69
    mov byte ptr [edi], ah ; 1:03a6b
    mov byte ptr [edi + ecx], al ; 1:03a6d
    add edi, ebx ; 1:03a70
    mov ah, byte ptr [esi] ; 1:03a72
    mov al, byte ptr [esi + edx] ; 1:03a74
    add esi, ebp ; 1:03a77
    mov byte ptr [edi], ah ; 1:03a79
    mov byte ptr [edi + ecx], al ; 1:03a7b
    add edi, ebx ; 1:03a7e
    mov ah, byte ptr [esi] ; 1:03a80
    mov al, byte ptr [esi + edx] ; 1:03a82
    add esi, ebp ; 1:03a85
    mov byte ptr [edi], ah ; 1:03a87
    mov byte ptr [edi + ecx], al ; 1:03a89
    pop edi ; 1:03a8c
    pop esi ; 1:03a8d
    pop ecx ; 1:03a8e
    pop ebx ; 1:03a8f
    pop ebp ; 1:03a90
    pop ecx ; 1:03a91
    pop edi ; 1:03a92
    pop esi ; 1:03a93
    inc edi ; 1:03a94
    inc esi ; 1:03a95
    test edi, 2 ; 1:03a96
    je L_03b70 ; 1:03a9c
    push ebp ; 1:03aa2
    push ebx ; 1:03aa3
    push ecx ; 1:03aa4
    push esi ; 1:03aa5
    push edi ; 1:03aa6
    mov ebp, edx ; 1:03aa7
    add ebp, edx ; 1:03aa9
    mov ecx, 50h ; 1:03aab
    mov ebx, 0a0h ; 1:03ab0
    mov ax, word ptr [esi] ; 1:03ab5
    mov bx, word ptr [esi + edx] ; 1:03ab8
    add esi, ebp ; 1:03abc
    mov word ptr [edi], ax ; 1:03abe
    mov word ptr [edi + ecx], bx ; 1:03ac1
    add edi, 0a0h ; 1:03ac5
    mov ax, word ptr [esi] ; 1:03acb
    mov bx, word ptr [esi + edx] ; 1:03ace
    add esi, ebp ; 1:03ad2
    mov word ptr [edi], ax ; 1:03ad4
    mov word ptr [edi + ecx], bx ; 1:03ad7
    add edi, 0a0h ; 1:03adb
    mov ax, word ptr [esi] ; 1:03ae1
    mov bx, word ptr [esi + edx] ; 1:03ae4
    add esi, ebp ; 1:03ae8
    mov word ptr [edi], ax ; 1:03aea
    mov word ptr [edi + ecx], bx ; 1:03aed
    add edi, 0a0h ; 1:03af1
    mov ax, word ptr [esi] ; 1:03af7
    mov bx, word ptr [esi + edx] ; 1:03afa
    add esi, ebp ; 1:03afe
    mov word ptr [edi], ax ; 1:03b00
    mov word ptr [edi + ecx], bx ; 1:03b03
    add edi, 0a0h ; 1:03b07
    mov ax, word ptr [esi] ; 1:03b0d
    mov bx, word ptr [esi + edx] ; 1:03b10
    add esi, ebp ; 1:03b14
    mov word ptr [edi], ax ; 1:03b16
    mov word ptr [edi + ecx], bx ; 1:03b19
    add edi, 0a0h ; 1:03b1d
    mov ax, word ptr [esi] ; 1:03b23
    mov bx, word ptr [esi + edx] ; 1:03b26
    add esi, ebp ; 1:03b2a
    mov word ptr [edi], ax ; 1:03b2c
    mov word ptr [edi + ecx], bx ; 1:03b2f
    add edi, 0a0h ; 1:03b33
    mov ax, word ptr [esi] ; 1:03b39
    mov bx, word ptr [esi + edx] ; 1:03b3c
    add esi, ebp ; 1:03b40
    mov word ptr [edi], ax ; 1:03b42
    mov word ptr [edi + ecx], bx ; 1:03b45
    add edi, 0a0h ; 1:03b49
    mov ax, word ptr [esi] ; 1:03b4f
    mov bx, word ptr [esi + edx] ; 1:03b52
    add esi, ebp ; 1:03b56
    mov word ptr [edi], ax ; 1:03b58
    mov word ptr [edi + ecx], bx ; 1:03b5b
    pop edi ; 1:03b5f
    pop esi ; 1:03b60
    pop ecx ; 1:03b61
    pop ebx ; 1:03b62
    pop ebp ; 1:03b63
    add esi, 2 ; 1:03b64
    add edi, 2 ; 1:03b67
    dec ecx ; 1:03b6a
    jmp L_03def ; 1:03b6b
L_03b70:
    push esi ; 1:03b70
    push edi ; 1:03b71
    push ecx ; 1:03b72
    shl ecx, 2 ; 1:03b73
    sub ecx, 4 ; 1:03b76
    add esi, ecx ; 1:03b79
    add edi, ecx ; 1:03b7b
    push ebp ; 1:03b7d
    push ebx ; 1:03b7e
    push ecx ; 1:03b7f
    push esi ; 1:03b80
    push edi ; 1:03b81
    mov ebp, edx ; 1:03b82
    add ebp, edx ; 1:03b84
    mov ecx, 50h ; 1:03b86
    mov ebx, 0a0h ; 1:03b8b
    mov ax, word ptr [esi] ; 1:03b90
    mov bx, word ptr [esi + edx] ; 1:03b93
    add esi, ebp ; 1:03b97
    mov word ptr [edi], ax ; 1:03b99
    mov word ptr [edi + ecx], bx ; 1:03b9c
    add edi, 0a0h ; 1:03ba0
    mov ax, word ptr [esi] ; 1:03ba6
    mov bx, word ptr [esi + edx] ; 1:03ba9
    add esi, ebp ; 1:03bad
    mov word ptr [edi], ax ; 1:03baf
    mov word ptr [edi + ecx], bx ; 1:03bb2
    add edi, 0a0h ; 1:03bb6
    mov ax, word ptr [esi] ; 1:03bbc
    mov bx, word ptr [esi + edx] ; 1:03bbf
    add esi, ebp ; 1:03bc3
    mov word ptr [edi], ax ; 1:03bc5
    mov word ptr [edi + ecx], bx ; 1:03bc8
    add edi, 0a0h ; 1:03bcc
    mov ax, word ptr [esi] ; 1:03bd2
    mov bx, word ptr [esi + edx] ; 1:03bd5
    add esi, ebp ; 1:03bd9
    mov word ptr [edi], ax ; 1:03bdb
    mov word ptr [edi + ecx], bx ; 1:03bde
    add edi, 0a0h ; 1:03be2
    mov ax, word ptr [esi] ; 1:03be8
    mov bx, word ptr [esi + edx] ; 1:03beb
    add esi, ebp ; 1:03bef
    mov word ptr [edi], ax ; 1:03bf1
    mov word ptr [edi + ecx], bx ; 1:03bf4
    add edi, 0a0h ; 1:03bf8
    mov ax, word ptr [esi] ; 1:03bfe
    mov bx, word ptr [esi + edx] ; 1:03c01
    add esi, ebp ; 1:03c05
    mov word ptr [edi], ax ; 1:03c07
    mov word ptr [edi + ecx], bx ; 1:03c0a
    add edi, 0a0h ; 1:03c0e
    mov ax, word ptr [esi] ; 1:03c14
    mov bx, word ptr [esi + edx] ; 1:03c17
    add esi, ebp ; 1:03c1b
    mov word ptr [edi], ax ; 1:03c1d
    mov word ptr [edi + ecx], bx ; 1:03c20
    add edi, 0a0h ; 1:03c24
    mov ax, word ptr [esi] ; 1:03c2a
    mov bx, word ptr [esi + edx] ; 1:03c2d
    add esi, ebp ; 1:03c31
    mov word ptr [edi], ax ; 1:03c33
    mov word ptr [edi + ecx], bx ; 1:03c36
    pop edi ; 1:03c3a
    pop esi ; 1:03c3b
    pop ecx ; 1:03c3c
    pop ebx ; 1:03c3d
    pop ebp ; 1:03c3e
    pop ecx ; 1:03c3f
    pop edi ; 1:03c40
    pop esi ; 1:03c41
    dec ecx ; 1:03c42
    jmp L_03def ; 1:03c43
L_03c48:
    test edi, 2 ; 1:03c48
    je L_03def ; 1:03c4e
    push ebp ; 1:03c54
    push ebx ; 1:03c55
    push ecx ; 1:03c56
    push esi ; 1:03c57
    push edi ; 1:03c58
    mov ebp, edx ; 1:03c59
    add ebp, edx ; 1:03c5b
    mov ecx, 50h ; 1:03c5d
    mov ebx, 0a0h ; 1:03c62
    mov ax, word ptr [esi] ; 1:03c67
    mov bx, word ptr [esi + edx] ; 1:03c6a
    add esi, ebp ; 1:03c6e
    mov word ptr [edi], ax ; 1:03c70
    mov word ptr [edi + ecx], bx ; 1:03c73
    add edi, 0a0h ; 1:03c77
    mov ax, word ptr [esi] ; 1:03c7d
    mov bx, word ptr [esi + edx] ; 1:03c80
    add esi, ebp ; 1:03c84
    mov word ptr [edi], ax ; 1:03c86
    mov word ptr [edi + ecx], bx ; 1:03c89
    add edi, 0a0h ; 1:03c8d
    mov ax, word ptr [esi] ; 1:03c93
    mov bx, word ptr [esi + edx] ; 1:03c96
    add esi, ebp ; 1:03c9a
    mov word ptr [edi], ax ; 1:03c9c
    mov word ptr [edi + ecx], bx ; 1:03c9f
    add edi, 0a0h ; 1:03ca3
    mov ax, word ptr [esi] ; 1:03ca9
    mov bx, word ptr [esi + edx] ; 1:03cac
    add esi, ebp ; 1:03cb0
    mov word ptr [edi], ax ; 1:03cb2
    mov word ptr [edi + ecx], bx ; 1:03cb5
    add edi, 0a0h ; 1:03cb9
    mov ax, word ptr [esi] ; 1:03cbf
    mov bx, word ptr [esi + edx] ; 1:03cc2
    add esi, ebp ; 1:03cc6
    mov word ptr [edi], ax ; 1:03cc8
    mov word ptr [edi + ecx], bx ; 1:03ccb
    add edi, 0a0h ; 1:03ccf
    mov ax, word ptr [esi] ; 1:03cd5
    mov bx, word ptr [esi + edx] ; 1:03cd8
    add esi, ebp ; 1:03cdc
    mov word ptr [edi], ax ; 1:03cde
    mov word ptr [edi + ecx], bx ; 1:03ce1
    add edi, 0a0h ; 1:03ce5
    mov ax, word ptr [esi] ; 1:03ceb
    mov bx, word ptr [esi + edx] ; 1:03cee
    add esi, ebp ; 1:03cf2
    mov word ptr [edi], ax ; 1:03cf4
    mov word ptr [edi + ecx], bx ; 1:03cf7
    add edi, 0a0h ; 1:03cfb
    mov ax, word ptr [esi] ; 1:03d01
    mov bx, word ptr [esi + edx] ; 1:03d04
    add esi, ebp ; 1:03d08
    mov word ptr [edi], ax ; 1:03d0a
    mov word ptr [edi + ecx], bx ; 1:03d0d
    pop edi ; 1:03d11
    pop esi ; 1:03d12
    pop ecx ; 1:03d13
    pop ebx ; 1:03d14
    pop ebp ; 1:03d15
    push esi ; 1:03d16
    push edi ; 1:03d17
    push ecx ; 1:03d18
    shl ecx, 2 ; 1:03d19
    sub ecx, 2 ; 1:03d1c
    add esi, ecx ; 1:03d1f
    add edi, ecx ; 1:03d21
    push ebp ; 1:03d23
    push ebx ; 1:03d24
    push ecx ; 1:03d25
    push esi ; 1:03d26
    push edi ; 1:03d27
    mov ebp, edx ; 1:03d28
    add ebp, edx ; 1:03d2a
    mov ecx, 50h ; 1:03d2c
    mov ebx, 0a0h ; 1:03d31
    mov ax, word ptr [esi] ; 1:03d36
    mov bx, word ptr [esi + edx] ; 1:03d39
    add esi, ebp ; 1:03d3d
    mov word ptr [edi], ax ; 1:03d3f
    mov word ptr [edi + ecx], bx ; 1:03d42
    add edi, 0a0h ; 1:03d46
    mov ax, word ptr [esi] ; 1:03d4c
    mov bx, word ptr [esi + edx] ; 1:03d4f
    add esi, ebp ; 1:03d53
    mov word ptr [edi], ax ; 1:03d55
    mov word ptr [edi + ecx], bx ; 1:03d58
    add edi, 0a0h ; 1:03d5c
    mov ax, word ptr [esi] ; 1:03d62
    mov bx, word ptr [esi + edx] ; 1:03d65
    add esi, ebp ; 1:03d69
    mov word ptr [edi], ax ; 1:03d6b
    mov word ptr [edi + ecx], bx ; 1:03d6e
    add edi, 0a0h ; 1:03d72
    mov ax, word ptr [esi] ; 1:03d78
    mov bx, word ptr [esi + edx] ; 1:03d7b
    add esi, ebp ; 1:03d7f
    mov word ptr [edi], ax ; 1:03d81
    mov word ptr [edi + ecx], bx ; 1:03d84
    add edi, 0a0h ; 1:03d88
    mov ax, word ptr [esi] ; 1:03d8e
    mov bx, word ptr [esi + edx] ; 1:03d91
    add esi, ebp ; 1:03d95
    mov word ptr [edi], ax ; 1:03d97
    mov word ptr [edi + ecx], bx ; 1:03d9a
    add edi, 0a0h ; 1:03d9e
    mov ax, word ptr [esi] ; 1:03da4
    mov bx, word ptr [esi + edx] ; 1:03da7
    add esi, ebp ; 1:03dab
    mov word ptr [edi], ax ; 1:03dad
    mov word ptr [edi + ecx], bx ; 1:03db0
    add edi, 0a0h ; 1:03db4
    mov ax, word ptr [esi] ; 1:03dba
    mov bx, word ptr [esi + edx] ; 1:03dbd
    add esi, ebp ; 1:03dc1
    mov word ptr [edi], ax ; 1:03dc3
    mov word ptr [edi + ecx], bx ; 1:03dc6
    add edi, 0a0h ; 1:03dca
    mov ax, word ptr [esi] ; 1:03dd0
    mov bx, word ptr [esi + edx] ; 1:03dd3
    add esi, ebp ; 1:03dd7
    mov word ptr [edi], ax ; 1:03dd9
    mov word ptr [edi + ecx], bx ; 1:03ddc
    pop edi ; 1:03de0
    pop esi ; 1:03de1
    pop ecx ; 1:03de2
    pop ebx ; 1:03de3
    pop ebp ; 1:03de4
    pop ecx ; 1:03de5
    pop edi ; 1:03de6
    pop esi ; 1:03de7
    dec ecx ; 1:03de8
    add esi, 2 ; 1:03de9
    add edi, 2 ; 1:03dec
L_03def:
    push esi ; 1:03def
    cmp ecx, 0 ; 1:03df0
    je L_03e83 ; 1:03df3
    mov ebx, ecx ; 1:03df9
    shl ebx, 2 ; 1:03dfb
    mov eax, 50h ; 1:03dfe
    sub eax, ebx ; 1:03e03
    sub edx, ebx ; 1:03e05
    mov ebx, ecx ; 1:03e07
    rep movsd ; 1:03e09
    add esi, edx ; 1:03e0b
    add edi, eax ; 1:03e0d
    mov ecx, ebx ; 1:03e0f
    rep movsd ; 1:03e11
    add esi, edx ; 1:03e13
    add edi, eax ; 1:03e15
    mov ecx, ebx ; 1:03e17
    rep movsd ; 1:03e19
    add esi, edx ; 1:03e1b
    add edi, eax ; 1:03e1d
    mov ecx, ebx ; 1:03e1f
    rep movsd ; 1:03e21
    add esi, edx ; 1:03e23
    add edi, eax ; 1:03e25
    mov ecx, ebx ; 1:03e27
    rep movsd ; 1:03e29
    add esi, edx ; 1:03e2b
    add edi, eax ; 1:03e2d
    mov ecx, ebx ; 1:03e2f
    rep movsd ; 1:03e31
    add esi, edx ; 1:03e33
    add edi, eax ; 1:03e35
    mov ecx, ebx ; 1:03e37
    rep movsd ; 1:03e39
    add esi, edx ; 1:03e3b
    add edi, eax ; 1:03e3d
    mov ecx, ebx ; 1:03e3f
    rep movsd ; 1:03e41
    add esi, edx ; 1:03e43
    add edi, eax ; 1:03e45
    mov ecx, ebx ; 1:03e47
    rep movsd ; 1:03e49
    add esi, edx ; 1:03e4b
    add edi, eax ; 1:03e4d
    mov ecx, ebx ; 1:03e4f
    rep movsd ; 1:03e51
    add esi, edx ; 1:03e53
    add edi, eax ; 1:03e55
    mov ecx, ebx ; 1:03e57
    rep movsd ; 1:03e59
    add esi, edx ; 1:03e5b
    add edi, eax ; 1:03e5d
    mov ecx, ebx ; 1:03e5f
    rep movsd ; 1:03e61
    add esi, edx ; 1:03e63
    add edi, eax ; 1:03e65
    mov ecx, ebx ; 1:03e67
    rep movsd ; 1:03e69
    add esi, edx ; 1:03e6b
    add edi, eax ; 1:03e6d
    mov ecx, ebx ; 1:03e6f
    rep movsd ; 1:03e71
    add esi, edx ; 1:03e73
    add edi, eax ; 1:03e75
    mov ecx, ebx ; 1:03e77
    rep movsd ; 1:03e79
    add esi, edx ; 1:03e7b
    add edi, eax ; 1:03e7d
    mov ecx, ebx ; 1:03e7f
    rep movsd ; 1:03e81
L_03e83:
    pop esi ; 1:03e83
    pop edx ; 1:03e84
    pop ecx ; 1:03e85
    pop ebx ; 1:03e86
    pop ebp ; 1:03e87
    pop ecx ; 1:03e88
    pop edi ; 1:03e89
    dec ecx ; 1:03e8a
    add ebp, 0ch ; 1:03e8b
    jmp L_0394a ; 1:03e8e
L_03e93:
    pop ebp ; 1:03e93
    pop ecx ; 1:03e94
    jmp L_03931 ; 1:03e95
L_03e9a:
    pop esi ; 1:03e9a
    pop esi ; 1:03e9b
    add esp, 4 ; 1:03e9c
    pop ebp ; 1:03e9f
    ret ; 1:03ea0
asm_03ea1_:
    push ebp ; 1:03ea1
    push dword ptr [ecx + 8] ; 1:03ea2
    push dword ptr [ecx + 4] ; 1:03ea5
    push dword ptr [ecx] ; 1:03ea8
    mov ebp, esi ; 1:03eaa
    mov ah, al ; 1:03eac
    push edx ; 1:03eae
    push eax ; 1:03eaf
    mov dx, 3c4h ; 1:03eb0
    mov al, 2 ; 1:03eb4
    out dx, ax ; 1:03eb6
    pop eax ; 1:03eb8
    pop edx ; 1:03eb9
    xor ecx, ecx ; 1:03eba
    mov cl, 0 ; 1:03ebc
    jmp L_03ed1 ; 1:03ebe
L_03ec0:
    add ebp, 80h ; 1:03ec0
    inc cl ; 1:03ec6
    cmp cl, 0ch ; 1:03ec8
    jge L_04429 ; 1:03ecb
L_03ed1:
    push ecx ; 1:03ed1
    push ebp ; 1:03ed2
    mov ecx, dword ptr [ebp] ; 1:03ed3
    add ebp, 4 ; 1:03ed6
L_03ed9:
    cmp ecx, 0 ; 1:03ed9
    je L_04422 ; 1:03edc
    push edi ; 1:03ee2
    push ecx ; 1:03ee3
    mov esi, dword ptr [ebp] ; 1:03ee4
    add esi, ebx ; 1:03ee7
    cmp esi, dword ptr [esp + 14h] ; 1:03ee9
    jb L_03ef3 ; 1:03eed
    sub esi, dword ptr [esp + 18h] ; 1:03eef
L_03ef3:
    add edi, dword ptr [ebp + 4] ; 1:03ef3
    mov ecx, dword ptr [ebp + 8] ; 1:03ef6
    push ebp ; 1:03ef9
    push ebx ; 1:03efa
    push ecx ; 1:03efb
    push edx ; 1:03efc
    test edi, 1 ; 1:03efd
    je L_041d7 ; 1:03f03
    push ebp ; 1:03f09
    push ebx ; 1:03f0a
    push ecx ; 1:03f0b
    push esi ; 1:03f0c
    push edi ; 1:03f0d
    mov ebp, edx ; 1:03f0e
    add ebp, edx ; 1:03f10
    mov ecx, 50h ; 1:03f12
    mov ebx, 0a0h ; 1:03f17
    mov ah, byte ptr [esi] ; 1:03f1c
    mov al, byte ptr [esi + edx] ; 1:03f1e
    add esi, ebp ; 1:03f21
    mov byte ptr [edi], ah ; 1:03f23
    mov byte ptr [edi + ecx], al ; 1:03f25
    add edi, ebx ; 1:03f28
    mov ah, byte ptr [esi] ; 1:03f2a
    mov al, byte ptr [esi + edx] ; 1:03f2c
    add esi, ebp ; 1:03f2f
    mov byte ptr [edi], ah ; 1:03f31
    mov byte ptr [edi + ecx], al ; 1:03f33
    add edi, ebx ; 1:03f36
    mov ah, byte ptr [esi] ; 1:03f38
    mov al, byte ptr [esi + edx] ; 1:03f3a
    add esi, ebp ; 1:03f3d
    mov byte ptr [edi], ah ; 1:03f3f
    mov byte ptr [edi + ecx], al ; 1:03f41
    add edi, ebx ; 1:03f44
    mov ah, byte ptr [esi] ; 1:03f46
    mov al, byte ptr [esi + edx] ; 1:03f48
    add esi, ebp ; 1:03f4b
    mov byte ptr [edi], ah ; 1:03f4d
    mov byte ptr [edi + ecx], al ; 1:03f4f
    add edi, ebx ; 1:03f52
    mov ah, byte ptr [esi] ; 1:03f54
    mov al, byte ptr [esi + edx] ; 1:03f56
    add esi, ebp ; 1:03f59
    mov byte ptr [edi], ah ; 1:03f5b
    mov byte ptr [edi + ecx], al ; 1:03f5d
    add edi, ebx ; 1:03f60
    mov ah, byte ptr [esi] ; 1:03f62
    mov al, byte ptr [esi + edx] ; 1:03f64
    add esi, ebp ; 1:03f67
    mov byte ptr [edi], ah ; 1:03f69
    mov byte ptr [edi + ecx], al ; 1:03f6b
    add edi, ebx ; 1:03f6e
    mov ah, byte ptr [esi] ; 1:03f70
    mov al, byte ptr [esi + edx] ; 1:03f72
    add esi, ebp ; 1:03f75
    mov byte ptr [edi], ah ; 1:03f77
    mov byte ptr [edi + ecx], al ; 1:03f79
    add edi, ebx ; 1:03f7c
    mov ah, byte ptr [esi] ; 1:03f7e
    mov al, byte ptr [esi + edx] ; 1:03f80
    add esi, ebp ; 1:03f83
    mov byte ptr [edi], ah ; 1:03f85
    mov byte ptr [edi + ecx], al ; 1:03f87
    pop edi ; 1:03f8a
    pop esi ; 1:03f8b
    pop ecx ; 1:03f8c
    pop ebx ; 1:03f8d
    pop ebp ; 1:03f8e
    push esi ; 1:03f8f
    push edi ; 1:03f90
    push ecx ; 1:03f91
    shl ecx, 2 ; 1:03f92
    dec ecx ; 1:03f95
    add esi, ecx ; 1:03f96
    add edi, ecx ; 1:03f98
    push ebp ; 1:03f9a
    push ebx ; 1:03f9b
    push ecx ; 1:03f9c
    push esi ; 1:03f9d
    push edi ; 1:03f9e
    mov ebp, edx ; 1:03f9f
    add ebp, edx ; 1:03fa1
    mov ecx, 50h ; 1:03fa3
    mov ebx, 0a0h ; 1:03fa8
    mov ah, byte ptr [esi] ; 1:03fad
    mov al, byte ptr [esi + edx] ; 1:03faf
    add esi, ebp ; 1:03fb2
    mov byte ptr [edi], ah ; 1:03fb4
    mov byte ptr [edi + ecx], al ; 1:03fb6
    add edi, ebx ; 1:03fb9
    mov ah, byte ptr [esi] ; 1:03fbb
    mov al, byte ptr [esi + edx] ; 1:03fbd
    add esi, ebp ; 1:03fc0
    mov byte ptr [edi], ah ; 1:03fc2
    mov byte ptr [edi + ecx], al ; 1:03fc4
    add edi, ebx ; 1:03fc7
    mov ah, byte ptr [esi] ; 1:03fc9
    mov al, byte ptr [esi + edx] ; 1:03fcb
    add esi, ebp ; 1:03fce
    mov byte ptr [edi], ah ; 1:03fd0
    mov byte ptr [edi + ecx], al ; 1:03fd2
    add edi, ebx ; 1:03fd5
    mov ah, byte ptr [esi] ; 1:03fd7
    mov al, byte ptr [esi + edx] ; 1:03fd9
    add esi, ebp ; 1:03fdc
    mov byte ptr [edi], ah ; 1:03fde
    mov byte ptr [edi + ecx], al ; 1:03fe0
    add edi, ebx ; 1:03fe3
    mov ah, byte ptr [esi] ; 1:03fe5
    mov al, byte ptr [esi + edx] ; 1:03fe7
    add esi, ebp ; 1:03fea
    mov byte ptr [edi], ah ; 1:03fec
    mov byte ptr [edi + ecx], al ; 1:03fee
    add edi, ebx ; 1:03ff1
    mov ah, byte ptr [esi] ; 1:03ff3
    mov al, byte ptr [esi + edx] ; 1:03ff5
    add esi, ebp ; 1:03ff8
    mov byte ptr [edi], ah ; 1:03ffa
    mov byte ptr [edi + ecx], al ; 1:03ffc
    add edi, ebx ; 1:03fff
    mov ah, byte ptr [esi] ; 1:04001
    mov al, byte ptr [esi + edx] ; 1:04003
    add esi, ebp ; 1:04006
    mov byte ptr [edi], ah ; 1:04008
    mov byte ptr [edi + ecx], al ; 1:0400a
    add edi, ebx ; 1:0400d
    mov ah, byte ptr [esi] ; 1:0400f
    mov al, byte ptr [esi + edx] ; 1:04011
    add esi, ebp ; 1:04014
    mov byte ptr [edi], ah ; 1:04016
    mov byte ptr [edi + ecx], al ; 1:04018
    pop edi ; 1:0401b
    pop esi ; 1:0401c
    pop ecx ; 1:0401d
    pop ebx ; 1:0401e
    pop ebp ; 1:0401f
    pop ecx ; 1:04020
    pop edi ; 1:04021
    pop esi ; 1:04022
    inc edi ; 1:04023
    inc esi ; 1:04024
    test edi, 2 ; 1:04025
    je L_040ff ; 1:0402b
    push ebp ; 1:04031
    push ebx ; 1:04032
    push ecx ; 1:04033
    push esi ; 1:04034
    push edi ; 1:04035
    mov ebp, edx ; 1:04036
    add ebp, edx ; 1:04038
    mov ecx, 50h ; 1:0403a
    mov ebx, 0a0h ; 1:0403f
    mov ax, word ptr [esi] ; 1:04044
    mov bx, word ptr [esi + edx] ; 1:04047
    add esi, ebp ; 1:0404b
    mov word ptr [edi], ax ; 1:0404d
    mov word ptr [edi + ecx], bx ; 1:04050
    add edi, 0a0h ; 1:04054
    mov ax, word ptr [esi] ; 1:0405a
    mov bx, word ptr [esi + edx] ; 1:0405d
    add esi, ebp ; 1:04061
    mov word ptr [edi], ax ; 1:04063
    mov word ptr [edi + ecx], bx ; 1:04066
    add edi, 0a0h ; 1:0406a
    mov ax, word ptr [esi] ; 1:04070
    mov bx, word ptr [esi + edx] ; 1:04073
    add esi, ebp ; 1:04077
    mov word ptr [edi], ax ; 1:04079
    mov word ptr [edi + ecx], bx ; 1:0407c
    add edi, 0a0h ; 1:04080
    mov ax, word ptr [esi] ; 1:04086
    mov bx, word ptr [esi + edx] ; 1:04089
    add esi, ebp ; 1:0408d
    mov word ptr [edi], ax ; 1:0408f
    mov word ptr [edi + ecx], bx ; 1:04092
    add edi, 0a0h ; 1:04096
    mov ax, word ptr [esi] ; 1:0409c
    mov bx, word ptr [esi + edx] ; 1:0409f
    add esi, ebp ; 1:040a3
    mov word ptr [edi], ax ; 1:040a5
    mov word ptr [edi + ecx], bx ; 1:040a8
    add edi, 0a0h ; 1:040ac
    mov ax, word ptr [esi] ; 1:040b2
    mov bx, word ptr [esi + edx] ; 1:040b5
    add esi, ebp ; 1:040b9
    mov word ptr [edi], ax ; 1:040bb
    mov word ptr [edi + ecx], bx ; 1:040be
    add edi, 0a0h ; 1:040c2
    mov ax, word ptr [esi] ; 1:040c8
    mov bx, word ptr [esi + edx] ; 1:040cb
    add esi, ebp ; 1:040cf
    mov word ptr [edi], ax ; 1:040d1
    mov word ptr [edi + ecx], bx ; 1:040d4
    add edi, 0a0h ; 1:040d8
    mov ax, word ptr [esi] ; 1:040de
    mov bx, word ptr [esi + edx] ; 1:040e1
    add esi, ebp ; 1:040e5
    mov word ptr [edi], ax ; 1:040e7
    mov word ptr [edi + ecx], bx ; 1:040ea
    pop edi ; 1:040ee
    pop esi ; 1:040ef
    pop ecx ; 1:040f0
    pop ebx ; 1:040f1
    pop ebp ; 1:040f2
    add esi, 2 ; 1:040f3
    add edi, 2 ; 1:040f6
    dec ecx ; 1:040f9
    jmp L_0437e ; 1:040fa
L_040ff:
    push esi ; 1:040ff
    push edi ; 1:04100
    push ecx ; 1:04101
    shl ecx, 2 ; 1:04102
    sub ecx, 4 ; 1:04105
    add esi, ecx ; 1:04108
    add edi, ecx ; 1:0410a
    push ebp ; 1:0410c
    push ebx ; 1:0410d
    push ecx ; 1:0410e
    push esi ; 1:0410f
    push edi ; 1:04110
    mov ebp, edx ; 1:04111
    add ebp, edx ; 1:04113
    mov ecx, 50h ; 1:04115
    mov ebx, 0a0h ; 1:0411a
    mov ax, word ptr [esi] ; 1:0411f
    mov bx, word ptr [esi + edx] ; 1:04122
    add esi, ebp ; 1:04126
    mov word ptr [edi], ax ; 1:04128
    mov word ptr [edi + ecx], bx ; 1:0412b
    add edi, 0a0h ; 1:0412f
    mov ax, word ptr [esi] ; 1:04135
    mov bx, word ptr [esi + edx] ; 1:04138
    add esi, ebp ; 1:0413c
    mov word ptr [edi], ax ; 1:0413e
    mov word ptr [edi + ecx], bx ; 1:04141
    add edi, 0a0h ; 1:04145
    mov ax, word ptr [esi] ; 1:0414b
    mov bx, word ptr [esi + edx] ; 1:0414e
    add esi, ebp ; 1:04152
    mov word ptr [edi], ax ; 1:04154
    mov word ptr [edi + ecx], bx ; 1:04157
    add edi, 0a0h ; 1:0415b
    mov ax, word ptr [esi] ; 1:04161
    mov bx, word ptr [esi + edx] ; 1:04164
    add esi, ebp ; 1:04168
    mov word ptr [edi], ax ; 1:0416a
    mov word ptr [edi + ecx], bx ; 1:0416d
    add edi, 0a0h ; 1:04171
    mov ax, word ptr [esi] ; 1:04177
    mov bx, word ptr [esi + edx] ; 1:0417a
    add esi, ebp ; 1:0417e
    mov word ptr [edi], ax ; 1:04180
    mov word ptr [edi + ecx], bx ; 1:04183
    add edi, 0a0h ; 1:04187
    mov ax, word ptr [esi] ; 1:0418d
    mov bx, word ptr [esi + edx] ; 1:04190
    add esi, ebp ; 1:04194
    mov word ptr [edi], ax ; 1:04196
    mov word ptr [edi + ecx], bx ; 1:04199
    add edi, 0a0h ; 1:0419d
    mov ax, word ptr [esi] ; 1:041a3
    mov bx, word ptr [esi + edx] ; 1:041a6
    add esi, ebp ; 1:041aa
    mov word ptr [edi], ax ; 1:041ac
    mov word ptr [edi + ecx], bx ; 1:041af
    add edi, 0a0h ; 1:041b3
    mov ax, word ptr [esi] ; 1:041b9
    mov bx, word ptr [esi + edx] ; 1:041bc
    add esi, ebp ; 1:041c0
    mov word ptr [edi], ax ; 1:041c2
    mov word ptr [edi + ecx], bx ; 1:041c5
    pop edi ; 1:041c9
    pop esi ; 1:041ca
    pop ecx ; 1:041cb
    pop ebx ; 1:041cc
    pop ebp ; 1:041cd
    pop ecx ; 1:041ce
    pop edi ; 1:041cf
    pop esi ; 1:041d0
    dec ecx ; 1:041d1
    jmp L_0437e ; 1:041d2
L_041d7:
    test edi, 2 ; 1:041d7
    je L_0437e ; 1:041dd
    push ebp ; 1:041e3
    push ebx ; 1:041e4
    push ecx ; 1:041e5
    push esi ; 1:041e6
    push edi ; 1:041e7
    mov ebp, edx ; 1:041e8
    add ebp, edx ; 1:041ea
    mov ecx, 50h ; 1:041ec
    mov ebx, 0a0h ; 1:041f1
    mov ax, word ptr [esi] ; 1:041f6
    mov bx, word ptr [esi + edx] ; 1:041f9
    add esi, ebp ; 1:041fd
    mov word ptr [edi], ax ; 1:041ff
    mov word ptr [edi + ecx], bx ; 1:04202
    add edi, 0a0h ; 1:04206
    mov ax, word ptr [esi] ; 1:0420c
    mov bx, word ptr [esi + edx] ; 1:0420f
    add esi, ebp ; 1:04213
    mov word ptr [edi], ax ; 1:04215
    mov word ptr [edi + ecx], bx ; 1:04218
    add edi, 0a0h ; 1:0421c
    mov ax, word ptr [esi] ; 1:04222
    mov bx, word ptr [esi + edx] ; 1:04225
    add esi, ebp ; 1:04229
    mov word ptr [edi], ax ; 1:0422b
    mov word ptr [edi + ecx], bx ; 1:0422e
    add edi, 0a0h ; 1:04232
    mov ax, word ptr [esi] ; 1:04238
    mov bx, word ptr [esi + edx] ; 1:0423b
    add esi, ebp ; 1:0423f
    mov word ptr [edi], ax ; 1:04241
    mov word ptr [edi + ecx], bx ; 1:04244
    add edi, 0a0h ; 1:04248
    mov ax, word ptr [esi] ; 1:0424e
    mov bx, word ptr [esi + edx] ; 1:04251
    add esi, ebp ; 1:04255
    mov word ptr [edi], ax ; 1:04257
    mov word ptr [edi + ecx], bx ; 1:0425a
    add edi, 0a0h ; 1:0425e
    mov ax, word ptr [esi] ; 1:04264
    mov bx, word ptr [esi + edx] ; 1:04267
    add esi, ebp ; 1:0426b
    mov word ptr [edi], ax ; 1:0426d
    mov word ptr [edi + ecx], bx ; 1:04270
    add edi, 0a0h ; 1:04274
    mov ax, word ptr [esi] ; 1:0427a
    mov bx, word ptr [esi + edx] ; 1:0427d
    add esi, ebp ; 1:04281
    mov word ptr [edi], ax ; 1:04283
    mov word ptr [edi + ecx], bx ; 1:04286
    add edi, 0a0h ; 1:0428a
    mov ax, word ptr [esi] ; 1:04290
    mov bx, word ptr [esi + edx] ; 1:04293
    add esi, ebp ; 1:04297
    mov word ptr [edi], ax ; 1:04299
    mov word ptr [edi + ecx], bx ; 1:0429c
    pop edi ; 1:042a0
    pop esi ; 1:042a1
    pop ecx ; 1:042a2
    pop ebx ; 1:042a3
    pop ebp ; 1:042a4
    push esi ; 1:042a5
    push edi ; 1:042a6
    push ecx ; 1:042a7
    shl ecx, 2 ; 1:042a8
    sub ecx, 2 ; 1:042ab
    add esi, ecx ; 1:042ae
    add edi, ecx ; 1:042b0
    push ebp ; 1:042b2
    push ebx ; 1:042b3
    push ecx ; 1:042b4
    push esi ; 1:042b5
    push edi ; 1:042b6
    mov ebp, edx ; 1:042b7
    add ebp, edx ; 1:042b9
    mov ecx, 50h ; 1:042bb
    mov ebx, 0a0h ; 1:042c0
    mov ax, word ptr [esi] ; 1:042c5
    mov bx, word ptr [esi + edx] ; 1:042c8
    add esi, ebp ; 1:042cc
    mov word ptr [edi], ax ; 1:042ce
    mov word ptr [edi + ecx], bx ; 1:042d1
    add edi, 0a0h ; 1:042d5
    mov ax, word ptr [esi] ; 1:042db
    mov bx, word ptr [esi + edx] ; 1:042de
    add esi, ebp ; 1:042e2
    mov word ptr [edi], ax ; 1:042e4
    mov word ptr [edi + ecx], bx ; 1:042e7
    add edi, 0a0h ; 1:042eb
    mov ax, word ptr [esi] ; 1:042f1
    mov bx, word ptr [esi + edx] ; 1:042f4
    add esi, ebp ; 1:042f8
    mov word ptr [edi], ax ; 1:042fa
    mov word ptr [edi + ecx], bx ; 1:042fd
    add edi, 0a0h ; 1:04301
    mov ax, word ptr [esi] ; 1:04307
    mov bx, word ptr [esi + edx] ; 1:0430a
    add esi, ebp ; 1:0430e
    mov word ptr [edi], ax ; 1:04310
    mov word ptr [edi + ecx], bx ; 1:04313
    add edi, 0a0h ; 1:04317
    mov ax, word ptr [esi] ; 1:0431d
    mov bx, word ptr [esi + edx] ; 1:04320
    add esi, ebp ; 1:04324
    mov word ptr [edi], ax ; 1:04326
    mov word ptr [edi + ecx], bx ; 1:04329
    add edi, 0a0h ; 1:0432d
    mov ax, word ptr [esi] ; 1:04333
    mov bx, word ptr [esi + edx] ; 1:04336
    add esi, ebp ; 1:0433a
    mov word ptr [edi], ax ; 1:0433c
    mov word ptr [edi + ecx], bx ; 1:0433f
    add edi, 0a0h ; 1:04343
    mov ax, word ptr [esi] ; 1:04349
    mov bx, word ptr [esi + edx] ; 1:0434c
    add esi, ebp ; 1:04350
    mov word ptr [edi], ax ; 1:04352
    mov word ptr [edi + ecx], bx ; 1:04355
    add edi, 0a0h ; 1:04359
    mov ax, word ptr [esi] ; 1:0435f
    mov bx, word ptr [esi + edx] ; 1:04362
    add esi, ebp ; 1:04366
    mov word ptr [edi], ax ; 1:04368
    mov word ptr [edi + ecx], bx ; 1:0436b
    pop edi ; 1:0436f
    pop esi ; 1:04370
    pop ecx ; 1:04371
    pop ebx ; 1:04372
    pop ebp ; 1:04373
    pop ecx ; 1:04374
    pop edi ; 1:04375
    pop esi ; 1:04376
    dec ecx ; 1:04377
    add esi, 2 ; 1:04378
    add edi, 2 ; 1:0437b
L_0437e:
    push esi ; 1:0437e
    cmp ecx, 0 ; 1:0437f
    je L_04412 ; 1:04382
    mov ebx, ecx ; 1:04388
    shl ebx, 2 ; 1:0438a
    mov eax, 50h ; 1:0438d
    sub eax, ebx ; 1:04392
    sub edx, ebx ; 1:04394
    mov ebx, ecx ; 1:04396
    rep movsd ; 1:04398
    add esi, edx ; 1:0439a
    add edi, eax ; 1:0439c
    mov ecx, ebx ; 1:0439e
    rep movsd ; 1:043a0
    add esi, edx ; 1:043a2
    add edi, eax ; 1:043a4
    mov ecx, ebx ; 1:043a6
    rep movsd ; 1:043a8
    add esi, edx ; 1:043aa
    add edi, eax ; 1:043ac
    mov ecx, ebx ; 1:043ae
    rep movsd ; 1:043b0
    add esi, edx ; 1:043b2
    add edi, eax ; 1:043b4
    mov ecx, ebx ; 1:043b6
    rep movsd ; 1:043b8
    add esi, edx ; 1:043ba
    add edi, eax ; 1:043bc
    mov ecx, ebx ; 1:043be
    rep movsd ; 1:043c0
    add esi, edx ; 1:043c2
    add edi, eax ; 1:043c4
    mov ecx, ebx ; 1:043c6
    rep movsd ; 1:043c8
    add esi, edx ; 1:043ca
    add edi, eax ; 1:043cc
    mov ecx, ebx ; 1:043ce
    rep movsd ; 1:043d0
    add esi, edx ; 1:043d2
    add edi, eax ; 1:043d4
    mov ecx, ebx ; 1:043d6
    rep movsd ; 1:043d8
    add esi, edx ; 1:043da
    add edi, eax ; 1:043dc
    mov ecx, ebx ; 1:043de
    rep movsd ; 1:043e0
    add esi, edx ; 1:043e2
    add edi, eax ; 1:043e4
    mov ecx, ebx ; 1:043e6
    rep movsd ; 1:043e8
    add esi, edx ; 1:043ea
    add edi, eax ; 1:043ec
    mov ecx, ebx ; 1:043ee
    rep movsd ; 1:043f0
    add esi, edx ; 1:043f2
    add edi, eax ; 1:043f4
    mov ecx, ebx ; 1:043f6
    rep movsd ; 1:043f8
    add esi, edx ; 1:043fa
    add edi, eax ; 1:043fc
    mov ecx, ebx ; 1:043fe
    rep movsd ; 1:04400
    add esi, edx ; 1:04402
    add edi, eax ; 1:04404
    mov ecx, ebx ; 1:04406
    rep movsd ; 1:04408
    add esi, edx ; 1:0440a
    add edi, eax ; 1:0440c
    mov ecx, ebx ; 1:0440e
    rep movsd ; 1:04410
L_04412:
    pop esi ; 1:04412
    pop edx ; 1:04413
    pop ecx ; 1:04414
    pop ebx ; 1:04415
    pop ebp ; 1:04416
    pop ecx ; 1:04417
    pop edi ; 1:04418
    dec ecx ; 1:04419
    add ebp, 0ch ; 1:0441a
    jmp L_03ed9 ; 1:0441d
L_04422:
    pop ebp ; 1:04422
    pop ecx ; 1:04423
    jmp L_03ec0 ; 1:04424
L_04429:
    pop esi ; 1:04429
    pop esi ; 1:0442a
    add esp, 4 ; 1:0442b
    pop ebp ; 1:0442e
    ret ; 1:0442f
_TEXT ends
end
