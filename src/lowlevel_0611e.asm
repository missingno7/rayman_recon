; DOS RAYMAN.EXE v1.21 low-level assembly, 1:0611e..1:066df (end exclusive).
; Readable WASM 10.6 source reconstructed from the address-anchored cfg.dis listing.
.586p
_TEXT segment para public use32 'CODE'
public DetectCpu_, default_sprite_clipping_, get_sprite_clipping_, sprite_clipping_, DrawSpriteFlipNoClipX_, DrawSpriteX_, DrawSpriteNoClipX_, DrawSpriteFlipX_
extrn _saved_sprite_width:dword, _row_offsets_80_01e20:dword

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
clip_x_min:
dd 8
clip_x_max:
dd 312
clip_y_min:
dd 0
clip_y_max:
dd 200

; 1:06194 public; config binding RAYVERSE
default_sprite_clipping_:
    mov     dword ptr [clip_x_min], 8 ; 1:06194

    mov     dword ptr [clip_x_max], 0138h ; 1:0619e

    mov     dword ptr [clip_y_min], 0 ; 1:061a8

    mov     dword ptr [clip_y_max], 0c8h ; 1:061b2

    ret     ; 1:061bc

; 1:061bd public; config binding RAYVERSE
get_sprite_clipping_:
    push    ebp ; 1:061bd

    push    edi ; 1:061be

    mov     ebp, dword ptr [clip_x_min] ; 1:061bf

    mov     dword ptr [eax], ebp ; 1:061c5

    mov     edi, dword ptr [clip_x_max] ; 1:061c7

    mov     dword ptr [ebx], edi ; 1:061cd

    mov     ebp, dword ptr [clip_y_min] ; 1:061cf

    mov     dword ptr [ecx], ebp ; 1:061d5

    mov     edi, dword ptr [clip_y_max] ; 1:061d7

    mov     dword ptr [edx], edi ; 1:061dd

    pop     edi ; 1:061df

    pop     ebp ; 1:061e0

    ret     ; 1:061e1

; 1:061e2 local semantic entry (not externally referenced)
get_sprite_clipping_xmin:
    mov     eax, dword ptr [clip_x_min] ; 1:061e2

    ret     ; 1:061e7

; 1:061e8 local semantic entry (not externally referenced)
get_sprite_clipping_xmax:
    mov     eax, dword ptr [clip_x_max] ; 1:061e8

    ret     ; 1:061ed

; 1:061ee local semantic entry (not externally referenced)
get_sprite_clipping_ymin:
    mov     eax, dword ptr [clip_y_min] ; 1:061ee

    ret     ; 1:061f3

; 1:061f4 local semantic entry (not externally referenced)
get_sprite_clipping_ymax:
    mov     eax, dword ptr [clip_y_max] ; 1:061f4

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
    mov     dword ptr [clip_x_min], eax ; 1:06210

    cmp     ebx, 0 ; 1:06215

    jge     L_621F ; 1:06218

    mov     ebx, 0 ; 1:0621a

L_621F:
    cmp     ebx, 0140h ; 1:0621f

    jle     L_622C ; 1:06225

    mov     ebx, 0140h ; 1:06227

L_622C:
    mov     dword ptr [clip_x_max], ebx ; 1:0622c

    cmp     ecx, 0 ; 1:06232

    jge     L_623C ; 1:06235

    mov     ecx, 0 ; 1:06237

L_623C:
    cmp     ecx, 0c8h ; 1:0623c

    jle     L_6249 ; 1:06242

    mov     ecx, 0c8h ; 1:06244

L_6249:
    mov     dword ptr [clip_y_min], ecx ; 1:06249

    cmp     edx, 0 ; 1:0624f

    jge     L_6259 ; 1:06252

    mov     edx, 0 ; 1:06254

L_6259:
    cmp     edx, 0c8h ; 1:06259

    jle     L_6266 ; 1:0625f

    mov     edx, 0c8h ; 1:06261

L_6266:
    mov     dword ptr [clip_y_max], edx ; 1:06266

    ret     ; 1:0626c

L_626D:
    xor     edx, edx ; 1:0626d

    mov     dl, cl ; 1:0626f

    mov     dword ptr [_saved_sprite_width], edx ; 1:06271

    cmp     eax, dword ptr [clip_x_min] ; 1:06277

    jl      L_62B7 ; 1:0627d

L_627F:
    add     edx, eax ; 1:0627f

    cmp     edx, dword ptr [clip_x_max] ; 1:06281

    ja      L_62A3 ; 1:06287

L_6289:
    xor     edx, edx ; 1:06289

    mov     dl, ch ; 1:0628b

    cmp     ebx, dword ptr [clip_y_min] ; 1:0628d

    jl      L_62E9 ; 1:06293

L_6295:
    add     edx, ebx ; 1:06295

    cmp     edx, dword ptr [clip_y_max] ; 1:06297

    ja      L_62D5 ; 1:0629d

L_629F:
    clc     ; 1:0629f

    ret     ; 1:062a0

L_62A1:
    stc     ; 1:062a1

    ret     ; 1:062a2

L_62A3:
    cmp     eax, dword ptr [clip_x_max] ; 1:062a3

    jae     L_62A1 ; 1:062a9

    mov     edx, dword ptr [clip_x_max] ; 1:062ab

    sub     edx, eax ; 1:062b1

    mov     cl, dl ; 1:062b3

    jmp     L_6289 ; 1:062b5

L_62B7:
    sub     eax, dword ptr [clip_x_min] ; 1:062b7

    neg     eax ; 1:062bd

    cmp     edx, eax ; 1:062bf

    jle     L_62A1 ; 1:062c1

    and     eax, 0ffffh ; 1:062c3

    sub     edx, eax ; 1:062c8

    mov     cl, dl ; 1:062ca

    add     esi, eax ; 1:062cc

    mov     eax, dword ptr [clip_x_min] ; 1:062ce

    jmp     L_627F ; 1:062d3

L_62D5:
    cmp     ebx, dword ptr [clip_y_max] ; 1:062d5

    jae     L_62A1 ; 1:062db

    mov     edx, dword ptr [clip_y_max] ; 1:062dd

    sub     edx, ebx ; 1:062e3

    mov     ch, dl ; 1:062e5

    jmp     L_629F ; 1:062e7

L_62E9:
    add     edx, ebx ; 1:062e9

    cmp     edx, dword ptr [clip_y_min] ; 1:062eb

    jle     L_62A1 ; 1:062f1

    sub     ebx, dword ptr [clip_y_min] ; 1:062f3

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

    mov     ebx, dword ptr [clip_y_min] ; 1:06314

    xor     edx, edx ; 1:0631a

    mov     dl, ch ; 1:0631c

    jmp     L_6295 ; 1:0631e

L_6323:
    xor     edx, edx ; 1:06323

    mov     dl, cl ; 1:06325

    mov     dword ptr [_saved_sprite_width], edx ; 1:06327

    cmp     eax, dword ptr [clip_x_min] ; 1:0632d

    jl      L_6374 ; 1:06333

L_6335:
    add     edx, eax ; 1:06335

    cmp     edx, dword ptr [clip_x_max] ; 1:06337

    ja      L_6358 ; 1:0633d

L_633F:
    movzx   edx, ch ; 1:0633f

    cmp     ebx, dword ptr [clip_y_min] ; 1:06342

    jl      L_63A4 ; 1:06348

L_634A:
    add     edx, ebx ; 1:0634a

    cmp     edx, dword ptr [clip_y_max] ; 1:0634c

    ja      L_6390 ; 1:06352

L_6354:
    clc     ; 1:06354

    ret     ; 1:06355

L_6356:
    stc     ; 1:06356

    ret     ; 1:06357

L_6358:
    cmp     eax, dword ptr [clip_x_max] ; 1:06358

    jae     L_6356 ; 1:0635e

    mov     edx, dword ptr [clip_x_max] ; 1:06360

    sub     edx, eax ; 1:06366

    mov     cl, dl ; 1:06368

    add     esi, dword ptr [_saved_sprite_width] ; 1:0636a

    sub     esi, edx ; 1:06370

    jmp     L_633F ; 1:06372

L_6374:
    sub     eax, dword ptr [clip_x_min] ; 1:06374

    neg     eax ; 1:0637a

    cmp     edx, eax ; 1:0637c

    jle     L_6356 ; 1:0637e

    and     eax, 0ffffh ; 1:06380

    sub     edx, eax ; 1:06385

    mov     cl, dl ; 1:06387

    mov     eax, dword ptr [clip_x_min] ; 1:06389

    jmp     L_6335 ; 1:0638e

L_6390:
    cmp     ebx, dword ptr [clip_y_max] ; 1:06390

    jae     L_6356 ; 1:06396

    mov     edx, dword ptr [clip_y_max] ; 1:06398

    sub     edx, ebx ; 1:0639e

    mov     ch, dl ; 1:063a0

    jmp     L_6354 ; 1:063a2

L_63A4:
    add     edx, ebx ; 1:063a4

    cmp     edx, dword ptr [clip_y_min] ; 1:063a6

    jle     L_6356 ; 1:063ac

    sub     ebx, dword ptr [clip_y_min] ; 1:063ae

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

    mov     ebx, dword ptr [clip_y_min] ; 1:063cf

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
    call    L_626D ; 1:063f2

    jb      L_6558 ; 1:063f7

L_63FD:
    push    ebp ; 1:063fd

    mov     edx, dword ptr [_saved_sprite_width] ; 1:063fe

    mov     dword ptr [sprite_source_stride_x], edx ; 1:06404

    add     edi, dword ptr [ebx*4 + _row_offsets_80_01e20] ; 1:0640a

    movzx   edx, ch ; 1:06411

    mov     ebx, dword ptr [edx*4 + _row_offsets_80_01e20] ; 1:06414

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
    call    L_6323 ; 1:06565

    jb      L_66D6 ; 1:0656a

L_6570:
    push    ebp ; 1:06570

    mov     edx, dword ptr [_saved_sprite_width] ; 1:06571

    mov     dword ptr [sprite_source_stride_x], edx ; 1:06577

    add     edi, dword ptr [ebx*4 + _row_offsets_80_01e20] ; 1:0657d

    movzx   edx, ch ; 1:06584

    mov     ebx, dword ptr [edx*4 + _row_offsets_80_01e20] ; 1:06587

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
sprite_source_stride_flip_x:
dd 0
sprite_dest_stride_flip_x:
dd 0

_TEXT ends
end
