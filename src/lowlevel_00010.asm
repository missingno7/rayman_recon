; Reconstruction module; historical ownership remains OPEN (see NOTES.md).
.386p
.model flat
; Proven WLINK first-declaration precedence, docs/LINK.md / lk_stage2.
_DATA segment para public use32 'DATA'
_DATA ends
CONST segment dword public use32 'DATA'
CONST ends
CONST2 segment dword public use32 'DATA'
CONST2 ends
DATA32 segment dword public use32 'DATA'
DATA32 ends
DGROUP group _DATA, CONST, CONST2, DATA32

; ---- Accepted source: lowlevel_00010.asm ----
; DOS gog_v121_us 1:00010..1:02640, complete assembly slice.
; Canonical routine names INVENTED. Local address labels cite cfg.dis.
; No data directives or inferred padding: every line is an original instruction.
.386p
; 000c1 stores clipped minimum scanline; 00638 reads it to calculate y*320 and y*32.
extrn _raster_first_y:dword
; 00028 loads start; 0002f/00035 initialize first two dwords, 0003c advances 32 bytes, CL=200 at 0002d. 00663/00672 read bounds; 001dd/001fa compare signed-bias row fields.
extrn _raster_rows:byte
; 0004b..00050 stores ESI+4; 0022e/00387/004e0 reload descriptor for the next edges.
extrn _raster_descriptor:dword
; 00055..00057 copies descriptor first dword; 00758 and each pixel block add the pointer to the 16-bit texture index.
extrn _raster_texture:dword
; 0005e..00064 computes unsigned byte bound difference; 003c1 reads as edge slope dividend.
extrn _raster_du:dword
; 00069..0006b stores negated raster_du; 0010f reads as edge slope dividend.
extrn _raster_neg_du:dword
; 00072..00078 computes byte bound difference; 00519 reads as edge slope dividend.
extrn _raster_dv:dword
; 0007d..0007f stores negated raster_dv; 00268 reads as edge slope dividend.
extrn _raster_neg_dv:dword
; 00046 saves incoming EAX; 0064b adds it to first_y*320.
extrn _raster_destination:dword
; 00651 initializes destination scanline pointer; 00721 reads it; 02621 advances by 320.
extrn _raster_scanline:dword
; 00657 initializes first_y*32; 0065d reads index; 0262b advances by 32.
extrn _raster_row_offset:dword
; 000d3..000d6 stores clipped max_y-min_y+1; 02632 decrements and 02638 branches while nonzero.
extrn _raster_rows_left:dword
; raster_rows is a byte-addressed array of 200 records, stride 32, not a byte scalar.
; Explicit PTR widths below describe each field access. No shifted external aliases.
_TEXT segment para public use32 'CODE'
; Flat DS addresses both object-2 data and writable instruction operands.
assume cs:FLAT, ds:FLAT
public effect_reset_geometry_, draw_effect_segment_
effect_reset_geometry_:
    pushad ; 1:00010
l_00011:
    mov edi, offset l_007bb ; 1:00011
l_00016:
    add edi, 05ah ; 1:00016
l_00019:
    mov eax, 0fffffec0h ; 1:00019
l_0001e:
    mov dword ptr [edi], eax ; 1:0001e
l_00020:
    add edi, 060h ; 1:00020
l_00023:
    add eax, 4 ; 1:00023
l_00026:
    jne short l_0001e ; 1:00026
l_00028:
    mov edi, offset _raster_rows ; 1:00028
l_0002d:
    mov cl, 0c8h ; 1:0002d
l_0002f:
    mov dword ptr [edi], 0ffffh ; 1:0002f
l_00035:
    mov dword ptr [edi + 4], 0 ; 1:00035
l_0003c:
    add edi, 020h ; 1:0003c
l_0003f:
    dec cl ; 1:0003f
l_00041:
    jne short l_0002f ; 1:00041
l_00043:
    popad ; 1:00043
l_00044:
    ret ; 1:00044
draw_effect_segment_:
    pushad ; 1:00045
l_00046:
    mov dword ptr [_raster_destination], eax ; 1:00046
l_0004b:
    mov eax, esi ; 1:0004b
l_0004d:
    add eax, 4 ; 1:0004d
l_00050:
    mov dword ptr [_raster_descriptor], eax ; 1:00050
l_00055:
    mov eax, dword ptr [esi] ; 1:00055
l_00057:
    mov dword ptr [_raster_texture], eax ; 1:00057
l_0005c:
    xor eax, eax ; 1:0005c
l_0005e:
    mov al, byte ptr [esi + 02ah] ; 1:0005e
l_00061:
    sub al, byte ptr [esi + 024h] ; 1:00061
l_00064:
    mov dword ptr [_raster_du], eax ; 1:00064
l_00069:
    neg eax ; 1:00069
l_0006b:
    mov dword ptr [_raster_neg_du], eax ; 1:0006b
l_00070:
    xor eax, eax ; 1:00070
l_00072:
    mov al, byte ptr [esi + 027h] ; 1:00072
l_00075:
    sub al, byte ptr [esi + 025h] ; 1:00075
l_00078:
    mov dword ptr [_raster_dv], eax ; 1:00078
l_0007d:
    neg eax ; 1:0007d
l_0007f:
    mov dword ptr [_raster_neg_dv], eax ; 1:0007f
l_00084:
    mov ebx, esi ; 1:00084
l_00086:
    mov cl, 3 ; 1:00086
l_00088:
    add ebx, 4 ; 1:00088
l_0008b:
    add esi, 010h ; 1:0008b
l_0008e:
    mov eax, dword ptr [ebx + 4] ; 1:0008e
l_00091:
    mov edx, eax ; 1:00091
l_00093:
    mov edi, dword ptr [esi] ; 1:00093
l_00095:
    cmp edi, eax ; 1:00095
l_00097:
    jge short l_0009b ; 1:00097
l_00099:
    mov eax, edi ; 1:00099
l_0009b:
    cmp edi, edx ; 1:0009b
l_0009d:
    jle short l_000a1 ; 1:0009d
l_0009f:
    mov edx, edi ; 1:0009f
l_000a1:
    add esi, 8 ; 1:000a1
l_000a4:
    dec cl ; 1:000a4
l_000a6:
    jne short l_00093 ; 1:000a6
l_000a8:
    cmp eax, 0c8h ; 1:000a8
l_000ad:
    jge near ptr l_0263e ; 1:000ad
l_000b3:
    or edx, edx ; 1:000b3
l_000b5:
    js near ptr l_0263e ; 1:000b5
l_000bb:
    or eax, eax ; 1:000bb
l_000bd:
    jns short l_000c1 ; 1:000bd
l_000bf:
    xor eax, eax ; 1:000bf
l_000c1:
    mov dword ptr [_raster_first_y], eax ; 1:000c1
l_000c6:
    cmp edx, 0c8h ; 1:000c6
l_000cc:
    jb short l_000d3 ; 1:000cc
l_000ce:
    mov edx, 0c7h ; 1:000ce
l_000d3:
    sub edx, eax ; 1:000d3
l_000d5:
    inc edx ; 1:000d5
l_000d6:
    mov dword ptr [_raster_rows_left], edx ; 1:000d6
l_000dc:
; Accumulate and clip one descriptor edge into the scanline records.
    mov esi, dword ptr [ebx] ; 1:000dc
l_000de:
    mov ecx, dword ptr [ebx + 4] ; 1:000de
l_000e1:
    mov ebp, dword ptr [ebx + 018h] ; 1:000e1
l_000e4:
    mov edi, dword ptr [ebx + 01ch] ; 1:000e4
l_000e7:
    or ecx, ecx ; 1:000e7
l_000e9:
    jns short l_000f3 ; 1:000e9
l_000eb:
    or edi, edi ; 1:000eb
l_000ed:
    js near ptr l_0022e ; 1:000ed
l_000f3:
    cmp ecx, 0c8h ; 1:000f3
l_000f9:
    jl short l_00107 ; 1:000f9
l_000fb:
    cmp edi, 0c8h ; 1:000fb
l_00101:
    jge near ptr l_0022e ; 1:00101
l_00107:
    mov ah, byte ptr [ebx + 026h] ; 1:00107
l_0010a:
    sub esi, ebp ; 1:0010a
l_0010c:
    mov al, byte ptr [ebx + 027h] ; 1:0010c
l_0010f:
    mov edx, dword ptr [_raster_neg_du] ; 1:0010f
l_00115:
    sub ecx, edi ; 1:00115
l_00117:
    jns short l_00129 ; 1:00117
l_00119:
    add edi, ecx ; 1:00119
l_0011b:
    add ebp, esi ; 1:0011b
l_0011d:
    neg ecx ; 1:0011d
l_0011f:
    mov ah, byte ptr [ebx + 020h] ; 1:0011f
l_00122:
    neg esi ; 1:00122
l_00124:
    mov al, byte ptr [ebx + 021h] ; 1:00124
l_00127:
    neg edx ; 1:00127
l_00129:
    add edi, 026ach ; 1:00129
l_0012f:
    add ebp, 07d00h ; 1:0012f
l_00135:
    mov byte ptr [l_001f3 + 6], al ; 1:00135
l_0013a:
    mov byte ptr [l_00210 + 6], al ; 1:0013a
l_0013f:
    xor al, al ; 1:0013f
l_00141:
    mov ebx, eax ; 1:00141
l_00143:
    inc ecx ; 1:00143
l_00144:
    shl ecx, 010h ; 1:00144
l_00147:
    xor eax, eax ; 1:00147
l_00149:
    idiv ecx ; 1:00149
l_0014b:
    mov edx, esi ; 1:0014b
l_0014d:
    mov esi, eax ; 1:0014d
l_0014f:
    mov word ptr [l_0021f + 4], ax ; 1:0014f
l_00155:
    mov word ptr [l_00196 + 2], ax ; 1:00155
l_0015b:
    or edx, edx ; 1:0015b
l_0015d:
    jns short l_00162 ; 1:0015d
l_0015f:
    sub edx, 2 ; 1:0015f
l_00162:
    inc edx ; 1:00162
l_00163:
    xor eax, eax ; 1:00163
l_00165:
    idiv ecx ; 1:00165
l_00167:
    mov word ptr [l_00217 + 3], ax ; 1:00167
l_0016d:
    mov word ptr [l_001ae + 2], ax ; 1:0016d
l_00173:
    sar eax, 010h ; 1:00173
l_00176:
    mov si, ax ; 1:00176
l_00179:
    xor edx, edx ; 1:00179
l_0017b:
    shld eax, esi, 010h ; 1:0017b
l_0017f:
    shr ecx, 010h ; 1:0017f
l_00182:
    cmp edi, 026ach ; 1:00182
l_00188:
    jae short l_001c6 ; 1:00188
l_0018a:
    push eax ; 1:0018a
l_0018b:
    sub edi, 026ach ; 1:0018b
l_00191:
    neg edi ; 1:00191
l_00193:
    shl eax, 010h ; 1:00193
l_00196:
    mov ax, 0ffffh ; 1:00196
l_0019a:
    mul edi ; 1:0019a
l_0019c:
    mov dx, ax ; 1:0019c
l_0019f:
    shl edx, 010h ; 1:0019f
l_001a2:
    shr eax, 010h ; 1:001a2
l_001a5:
    add bh, al ; 1:001a5
l_001a7:
    push edx ; 1:001a7
l_001a8:
    mov ax, si ; 1:001a8
l_001ab:
    shl eax, 010h ; 1:001ab
l_001ae:
    mov ax, 0ffffh ; 1:001ae
l_001b2:
    mul edi ; 1:001b2
l_001b4:
    pop edx ; 1:001b4
l_001b5:
    mov dx, ax ; 1:001b5
l_001b8:
    shr eax, 010h ; 1:001b8
l_001bb:
    add bp, ax ; 1:001bb
l_001be:
    sub ecx, edi ; 1:001be
l_001c0:
    pop eax ; 1:001c0
l_001c1:
    mov edi, 026ach ; 1:001c1
l_001c6:
    push edi ; 1:001c6
l_001c7:
    add edi, ecx ; 1:001c7
l_001c9:
    cmp edi, 02774h ; 1:001c9
l_001cf:
    jb short l_001d9 ; 1:001cf
l_001d1:
    sub edi, 02774h ; 1:001d1
l_001d7:
    sub ecx, edi ; 1:001d7
l_001d9:
    pop edi ; 1:001d9
l_001da:
    shl edi, 5 ; 1:001da
l_001dd:
    cmp word ptr [edi + _raster_rows - 04d580h], bp ; 1:001dd
l_001e4:
    jbe short l_001fa ; 1:001e4
l_001e6:
    mov word ptr [edi + _raster_rows - 04d580h], bp ; 1:001e6
l_001ed:
    mov byte ptr [edi + _raster_rows - 04d578h], bh ; 1:001ed
l_001f3:
    mov byte ptr [edi + _raster_rows - 04d570h], 0 ; 1:001f3
l_001fa:
    cmp word ptr [edi + _raster_rows - 04d57ch], bp ; 1:001fa
l_00201:
    jae short l_00217 ; 1:00201
l_00203:
    mov word ptr [edi + _raster_rows - 04d57ch], bp ; 1:00203
l_0020a:
    mov byte ptr [edi + _raster_rows - 04d574h], bh ; 1:0020a
l_00210:
    mov byte ptr [edi + _raster_rows - 04d56ch], 0 ; 1:00210
l_00217:
    add dx, 0ffffh ; 1:00217
l_0021c:
    adc bp, si ; 1:0021c
l_0021f:
    add edx, 0ffff0000h ; 1:0021f
l_00225:
    adc bh, al ; 1:00225
l_00227:
    add edi, 020h ; 1:00227
l_0022a:
    dec cx ; 1:0022a
l_0022c:
    jne short l_001dd ; 1:0022c
l_0022e:
; Accumulate and clip one descriptor edge into the scanline records.
    mov ebx, dword ptr [_raster_descriptor] ; 1:0022e
l_00234:
    mov esi, dword ptr [ebx + 018h] ; 1:00234
l_00237:
    mov ecx, dword ptr [ebx + 01ch] ; 1:00237
l_0023a:
    mov ebp, dword ptr [ebx + 010h] ; 1:0023a
l_0023d:
    mov edi, dword ptr [ebx + 014h] ; 1:0023d
l_00240:
    or ecx, ecx ; 1:00240
l_00242:
    jns short l_0024c ; 1:00242
l_00244:
    or edi, edi ; 1:00244
l_00246:
    js near ptr l_00387 ; 1:00246
l_0024c:
    cmp ecx, 0c8h ; 1:0024c
l_00252:
    jl short l_00260 ; 1:00252
l_00254:
    cmp edi, 0c8h ; 1:00254
l_0025a:
    jge near ptr l_00387 ; 1:0025a
l_00260:
    mov al, byte ptr [ebx + 024h] ; 1:00260
l_00263:
    sub esi, ebp ; 1:00263
l_00265:
    mov ah, byte ptr [ebx + 025h] ; 1:00265
l_00268:
    mov edx, dword ptr [_raster_neg_dv] ; 1:00268
l_0026e:
    sub ecx, edi ; 1:0026e
l_00270:
    jns short l_00282 ; 1:00270
l_00272:
    add edi, ecx ; 1:00272
l_00274:
    add ebp, esi ; 1:00274
l_00276:
    neg ecx ; 1:00276
l_00278:
    mov al, byte ptr [ebx + 026h] ; 1:00278
l_0027b:
    neg esi ; 1:0027b
l_0027d:
    mov ah, byte ptr [ebx + 027h] ; 1:0027d
l_00280:
    neg edx ; 1:00280
l_00282:
    add edi, 026ach ; 1:00282
l_00288:
    add ebp, 07d00h ; 1:00288
l_0028e:
    mov byte ptr [l_00346 + 6], al ; 1:0028e
l_00293:
    mov byte ptr [l_00363 + 6], al ; 1:00293
l_00298:
    xor al, al ; 1:00298
l_0029a:
    mov ebx, eax ; 1:0029a
l_0029c:
    inc ecx ; 1:0029c
l_0029d:
    shl ecx, 010h ; 1:0029d
l_002a0:
    xor eax, eax ; 1:002a0
l_002a2:
    idiv ecx ; 1:002a2
l_002a4:
    mov edx, esi ; 1:002a4
l_002a6:
    mov esi, eax ; 1:002a6
l_002a8:
    mov word ptr [l_00378 + 4], ax ; 1:002a8
l_002ae:
    mov word ptr [l_002ef + 2], ax ; 1:002ae
l_002b4:
    or edx, edx ; 1:002b4
l_002b6:
    jns short l_002bb ; 1:002b6
l_002b8:
    sub edx, 2 ; 1:002b8
l_002bb:
    inc edx ; 1:002bb
l_002bc:
    xor eax, eax ; 1:002bc
l_002be:
    idiv ecx ; 1:002be
l_002c0:
    mov word ptr [l_00370 + 3], ax ; 1:002c0
l_002c6:
    mov word ptr [l_00307 + 2], ax ; 1:002c6
l_002cc:
    sar eax, 010h ; 1:002cc
l_002cf:
    mov si, ax ; 1:002cf
l_002d2:
    xor edx, edx ; 1:002d2
l_002d4:
    shld eax, esi, 010h ; 1:002d4
l_002d8:
    shr ecx, 010h ; 1:002d8
l_002db:
    cmp edi, 026ach ; 1:002db
l_002e1:
    jae short l_0031f ; 1:002e1
l_002e3:
    push eax ; 1:002e3
l_002e4:
    sub edi, 026ach ; 1:002e4
l_002ea:
    neg edi ; 1:002ea
l_002ec:
    shl eax, 010h ; 1:002ec
l_002ef:
    mov ax, 0ffffh ; 1:002ef
l_002f3:
    mul edi ; 1:002f3
l_002f5:
    mov dx, ax ; 1:002f5
l_002f8:
    shl edx, 010h ; 1:002f8
l_002fb:
    shr eax, 010h ; 1:002fb
l_002fe:
    add bh, al ; 1:002fe
l_00300:
    push edx ; 1:00300
l_00301:
    mov ax, si ; 1:00301
l_00304:
    shl eax, 010h ; 1:00304
l_00307:
    mov ax, 0ffffh ; 1:00307
l_0030b:
    mul edi ; 1:0030b
l_0030d:
    pop edx ; 1:0030d
l_0030e:
    mov dx, ax ; 1:0030e
l_00311:
    shr eax, 010h ; 1:00311
l_00314:
    add bp, ax ; 1:00314
l_00317:
    sub ecx, edi ; 1:00317
l_00319:
    mov edi, 026ach ; 1:00319
l_0031e:
    pop eax ; 1:0031e
l_0031f:
    push edi ; 1:0031f
l_00320:
    add edi, ecx ; 1:00320
l_00322:
    cmp edi, 02774h ; 1:00322
l_00328:
    jb short l_00332 ; 1:00328
l_0032a:
    sub edi, 02774h ; 1:0032a
l_00330:
    sub ecx, edi ; 1:00330
l_00332:
    pop edi ; 1:00332
l_00333:
    shl edi, 5 ; 1:00333
l_00336:
    cmp word ptr [edi + _raster_rows - 04d580h], bp ; 1:00336
l_0033d:
    jbe short l_00353 ; 1:0033d
l_0033f:
    mov word ptr [edi + _raster_rows - 04d580h], bp ; 1:0033f
l_00346:
    mov byte ptr [edi + _raster_rows - 04d578h], 0ffh ; 1:00346
l_0034d:
    mov byte ptr [edi + _raster_rows - 04d570h], bh ; 1:0034d
l_00353:
    cmp word ptr [edi + _raster_rows - 04d57ch], bp ; 1:00353
l_0035a:
    jae short l_00370 ; 1:0035a
l_0035c:
    mov word ptr [edi + _raster_rows - 04d57ch], bp ; 1:0035c
l_00363:
    mov byte ptr [edi + _raster_rows - 04d574h], 0ffh ; 1:00363
l_0036a:
    mov byte ptr [edi + _raster_rows - 04d56ch], bh ; 1:0036a
l_00370:
    add dx, 0ffffh ; 1:00370
l_00375:
    adc bp, si ; 1:00375
l_00378:
    add edx, 0ffff0000h ; 1:00378
l_0037e:
    adc bh, al ; 1:0037e
l_00380:
    add edi, 020h ; 1:00380
l_00383:
    dec cx ; 1:00383
l_00385:
    jne short l_00336 ; 1:00385
l_00387:
; Accumulate and clip one descriptor edge into the scanline records.
    mov ebx, dword ptr [_raster_descriptor] ; 1:00387
l_0038d:
    mov esi, dword ptr [ebx + 010h] ; 1:0038d
l_00390:
    mov ecx, dword ptr [ebx + 014h] ; 1:00390
l_00393:
    mov ebp, dword ptr [ebx + 8] ; 1:00393
l_00396:
    mov edi, dword ptr [ebx + 0ch] ; 1:00396
l_00399:
    or ecx, ecx ; 1:00399
l_0039b:
    jns short l_003a5 ; 1:0039b
l_0039d:
    or edi, edi ; 1:0039d
l_0039f:
    js near ptr l_004e0 ; 1:0039f
l_003a5:
    cmp ecx, 0c8h ; 1:003a5
l_003ab:
    jl short l_003b9 ; 1:003ab
l_003ad:
    cmp edi, 0c8h ; 1:003ad
l_003b3:
    jge near ptr l_004e0 ; 1:003b3
l_003b9:
    mov al, byte ptr [ebx + 023h] ; 1:003b9
l_003bc:
    sub esi, ebp ; 1:003bc
l_003be:
    mov ah, byte ptr [ebx + 022h] ; 1:003be
l_003c1:
    mov edx, dword ptr [_raster_du] ; 1:003c1
l_003c7:
    sub ecx, edi ; 1:003c7
l_003c9:
    jns short l_003db ; 1:003c9
l_003cb:
    add edi, ecx ; 1:003cb
l_003cd:
    add ebp, esi ; 1:003cd
l_003cf:
    neg ecx ; 1:003cf
l_003d1:
    mov al, byte ptr [ebx + 025h] ; 1:003d1
l_003d4:
    neg esi ; 1:003d4
l_003d6:
    mov ah, byte ptr [ebx + 024h] ; 1:003d6
l_003d9:
    neg edx ; 1:003d9
l_003db:
    add edi, 026ach ; 1:003db
l_003e1:
    add ebp, 07d00h ; 1:003e1
l_003e7:
    mov byte ptr [l_004a5 + 6], al ; 1:003e7
l_003ec:
    mov byte ptr [l_004c2 + 6], al ; 1:003ec
l_003f1:
    xor al, al ; 1:003f1
l_003f3:
    mov ebx, eax ; 1:003f3
l_003f5:
    inc ecx ; 1:003f5
l_003f6:
    shl ecx, 010h ; 1:003f6
l_003f9:
    xor eax, eax ; 1:003f9
l_003fb:
    idiv ecx ; 1:003fb
l_003fd:
    mov edx, esi ; 1:003fd
l_003ff:
    mov esi, eax ; 1:003ff
l_00401:
    mov word ptr [l_004d1 + 4], ax ; 1:00401
l_00407:
    mov word ptr [l_00448 + 2], ax ; 1:00407
l_0040d:
    or edx, edx ; 1:0040d
l_0040f:
    jns short l_00414 ; 1:0040f
l_00411:
    sub edx, 2 ; 1:00411
l_00414:
    inc edx ; 1:00414
l_00415:
    xor eax, eax ; 1:00415
l_00417:
    idiv ecx ; 1:00417
l_00419:
    mov word ptr [l_004c9 + 3], ax ; 1:00419
l_0041f:
    mov word ptr [l_00460 + 2], ax ; 1:0041f
l_00425:
    sar eax, 010h ; 1:00425
l_00428:
    mov si, ax ; 1:00428
l_0042b:
    xor edx, edx ; 1:0042b
l_0042d:
    shld eax, esi, 010h ; 1:0042d
l_00431:
    shr ecx, 010h ; 1:00431
l_00434:
    cmp edi, 026ach ; 1:00434
l_0043a:
    jae short l_00478 ; 1:0043a
l_0043c:
    push eax ; 1:0043c
l_0043d:
    sub edi, 026ach ; 1:0043d
l_00443:
    neg edi ; 1:00443
l_00445:
    shl eax, 010h ; 1:00445
l_00448:
    mov ax, 0ffffh ; 1:00448
l_0044c:
    mul edi ; 1:0044c
l_0044e:
    mov dx, ax ; 1:0044e
l_00451:
    shl edx, 010h ; 1:00451
l_00454:
    shr eax, 010h ; 1:00454
l_00457:
    add bh, al ; 1:00457
l_00459:
    push edx ; 1:00459
l_0045a:
    mov ax, si ; 1:0045a
l_0045d:
    shl eax, 010h ; 1:0045d
l_00460:
    mov ax, 0ffffh ; 1:00460
l_00464:
    mul edi ; 1:00464
l_00466:
    pop edx ; 1:00466
l_00467:
    mov dx, ax ; 1:00467
l_0046a:
    shr eax, 010h ; 1:0046a
l_0046d:
    add bp, ax ; 1:0046d
l_00470:
    sub ecx, edi ; 1:00470
l_00472:
    mov edi, 026ach ; 1:00472
l_00477:
    pop eax ; 1:00477
l_00478:
    push edi ; 1:00478
l_00479:
    add edi, ecx ; 1:00479
l_0047b:
    cmp edi, 02774h ; 1:0047b
l_00481:
    jb short l_0048b ; 1:00481
l_00483:
    sub edi, 02774h ; 1:00483
l_00489:
    sub ecx, edi ; 1:00489
l_0048b:
    pop edi ; 1:0048b
l_0048c:
    shl edi, 5 ; 1:0048c
l_0048f:
    cmp word ptr [edi + _raster_rows - 04d580h], bp ; 1:0048f
l_00496:
    jbe short l_004ac ; 1:00496
l_00498:
    mov word ptr [edi + _raster_rows - 04d580h], bp ; 1:00498
l_0049f:
    mov byte ptr [edi + _raster_rows - 04d578h], bh ; 1:0049f
l_004a5:
    mov byte ptr [edi + _raster_rows - 04d570h], 0ffh ; 1:004a5
l_004ac:
    cmp word ptr [edi + _raster_rows - 04d57ch], bp ; 1:004ac
l_004b3:
    jae short l_004c9 ; 1:004b3
l_004b5:
    mov word ptr [edi + _raster_rows - 04d57ch], bp ; 1:004b5
l_004bc:
    mov byte ptr [edi + _raster_rows - 04d574h], bh ; 1:004bc
l_004c2:
    mov byte ptr [edi + _raster_rows - 04d56ch], 0ffh ; 1:004c2
l_004c9:
    add dx, 0ffffh ; 1:004c9
l_004ce:
    adc bp, si ; 1:004ce
l_004d1:
    add edx, 0ffff0000h ; 1:004d1
l_004d7:
    adc bh, al ; 1:004d7
l_004d9:
    add edi, 020h ; 1:004d9
l_004dc:
    dec cx ; 1:004dc
l_004de:
    jne short l_0048f ; 1:004de
l_004e0:
; Accumulate and clip one descriptor edge into the scanline records.
    mov ebx, dword ptr [_raster_descriptor] ; 1:004e0
l_004e6:
    mov ebp, dword ptr [ebx] ; 1:004e6
l_004e8:
    mov edi, dword ptr [ebx + 4] ; 1:004e8
l_004eb:
    mov esi, dword ptr [ebx + 8] ; 1:004eb
l_004ee:
    mov ecx, dword ptr [ebx + 0ch] ; 1:004ee
l_004f1:
    or ecx, ecx ; 1:004f1
l_004f3:
    jns short l_004fd ; 1:004f3
l_004f5:
    or edi, edi ; 1:004f5
l_004f7:
    js near ptr l_00638 ; 1:004f7
l_004fd:
    cmp ecx, 0c8h ; 1:004fd
l_00503:
    jl short l_00511 ; 1:00503
l_00505:
    cmp edi, 0c8h ; 1:00505
l_0050b:
    jge near ptr l_00638 ; 1:0050b
l_00511:
    mov al, byte ptr [ebx + 020h] ; 1:00511
l_00514:
    sub esi, ebp ; 1:00514
l_00516:
    mov ah, byte ptr [ebx + 021h] ; 1:00516
l_00519:
    mov edx, dword ptr [_raster_dv] ; 1:00519
l_0051f:
    sub ecx, edi ; 1:0051f
l_00521:
    jns short l_00533 ; 1:00521
l_00523:
    add edi, ecx ; 1:00523
l_00525:
    add ebp, esi ; 1:00525
l_00527:
    neg ecx ; 1:00527
l_00529:
    mov al, byte ptr [ebx + 022h] ; 1:00529
l_0052c:
    neg esi ; 1:0052c
l_0052e:
    mov ah, byte ptr [ebx + 023h] ; 1:0052e
l_00531:
    neg edx ; 1:00531
l_00533:
    add edi, 026ach ; 1:00533
l_00539:
    add ebp, 07d00h ; 1:00539
l_0053f:
    mov byte ptr [l_005f7 + 6], al ; 1:0053f
l_00544:
    mov byte ptr [l_00614 + 6], al ; 1:00544
l_00549:
    xor al, al ; 1:00549
l_0054b:
    mov ebx, eax ; 1:0054b
l_0054d:
    inc ecx ; 1:0054d
l_0054e:
    shl ecx, 010h ; 1:0054e
l_00551:
    xor eax, eax ; 1:00551
l_00553:
    idiv ecx ; 1:00553
l_00555:
    mov edx, esi ; 1:00555
l_00557:
    mov esi, eax ; 1:00557
l_00559:
    mov word ptr [l_00629 + 4], ax ; 1:00559
l_0055f:
    mov word ptr [l_005a0 + 2], ax ; 1:0055f
l_00565:
    or edx, edx ; 1:00565
l_00567:
    jns short l_0056c ; 1:00567
l_00569:
    sub edx, 2 ; 1:00569
l_0056c:
    inc edx ; 1:0056c
l_0056d:
    xor eax, eax ; 1:0056d
l_0056f:
    idiv ecx ; 1:0056f
l_00571:
    mov word ptr [l_00621 + 3], ax ; 1:00571
l_00577:
    mov word ptr [l_005b8 + 2], ax ; 1:00577
l_0057d:
    sar eax, 010h ; 1:0057d
l_00580:
    mov si, ax ; 1:00580
l_00583:
    xor edx, edx ; 1:00583
l_00585:
    shld eax, esi, 010h ; 1:00585
l_00589:
    shr ecx, 010h ; 1:00589
l_0058c:
    cmp edi, 026ach ; 1:0058c
l_00592:
    jae short l_005d0 ; 1:00592
l_00594:
    push eax ; 1:00594
l_00595:
    sub edi, 026ach ; 1:00595
l_0059b:
    neg edi ; 1:0059b
l_0059d:
    shl eax, 010h ; 1:0059d
l_005a0:
    mov ax, 0ffffh ; 1:005a0
l_005a4:
    mul edi ; 1:005a4
l_005a6:
    mov dx, ax ; 1:005a6
l_005a9:
    shl edx, 010h ; 1:005a9
l_005ac:
    shr eax, 010h ; 1:005ac
l_005af:
    add bh, al ; 1:005af
l_005b1:
    push edx ; 1:005b1
l_005b2:
    mov ax, si ; 1:005b2
l_005b5:
    shl eax, 010h ; 1:005b5
l_005b8:
    mov ax, 0ffffh ; 1:005b8
l_005bc:
    mul edi ; 1:005bc
l_005be:
    pop edx ; 1:005be
l_005bf:
    mov dx, ax ; 1:005bf
l_005c2:
    shr eax, 010h ; 1:005c2
l_005c5:
    add bp, ax ; 1:005c5
l_005c8:
    sub ecx, edi ; 1:005c8
l_005ca:
    mov edi, 026ach ; 1:005ca
l_005cf:
    pop eax ; 1:005cf
l_005d0:
    push edi ; 1:005d0
l_005d1:
    add edi, ecx ; 1:005d1
l_005d3:
    cmp edi, 02774h ; 1:005d3
l_005d9:
    jb short l_005e3 ; 1:005d9
l_005db:
    sub edi, 02774h ; 1:005db
l_005e1:
    sub ecx, edi ; 1:005e1
l_005e3:
    pop edi ; 1:005e3
l_005e4:
    shl edi, 5 ; 1:005e4
l_005e7:
    cmp word ptr [edi + _raster_rows - 04d580h], bp ; 1:005e7
l_005ee:
    jbe short l_00604 ; 1:005ee
l_005f0:
    mov word ptr [edi + _raster_rows - 04d580h], bp ; 1:005f0
l_005f7:
    mov byte ptr [edi + _raster_rows - 04d578h], 0 ; 1:005f7
l_005fe:
    mov byte ptr [edi + _raster_rows - 04d570h], bh ; 1:005fe
l_00604:
    cmp word ptr [edi + _raster_rows - 04d57ch], bp ; 1:00604
l_0060b:
    jae short l_00621 ; 1:0060b
l_0060d:
    mov word ptr [edi + _raster_rows - 04d57ch], bp ; 1:0060d
l_00614:
    mov byte ptr [edi + _raster_rows - 04d574h], 0 ; 1:00614
l_0061b:
    mov byte ptr [edi + _raster_rows - 04d56ch], bh ; 1:0061b
l_00621:
    add dx, 0ffffh ; 1:00621
l_00626:
    adc bp, si ; 1:00626
l_00629:
    add edx, 0ffff0000h ; 1:00629
l_0062f:
    adc bh, al ; 1:0062f
l_00631:
    add edi, 020h ; 1:00631
l_00634:
    dec cx ; 1:00634
l_00636:
    jne short l_005e7 ; 1:00636
l_00638:
; Convert first_y to screen-byte and row-record offsets.
    mov ebp, dword ptr [_raster_first_y] ; 1:00638
l_0063e:
    mov esi, ebp ; 1:0063e
l_00640:
    shl esi, 2 ; 1:00640
l_00643:
    add esi, ebp ; 1:00643
l_00645:
    shl esi, 6 ; 1:00645
l_00648:
    shl ebp, 5 ; 1:00648
l_0064b:
    add esi, dword ptr [_raster_destination] ; 1:0064b
l_00651:
    mov dword ptr [_raster_scanline], esi ; 1:00651
l_00657:
    mov dword ptr [_raster_row_offset], ebp ; 1:00657
l_0065d:
; Consume one scanline, reset its bounds, clip x, and derive texture steps.
    mov ebp, dword ptr [_raster_row_offset] ; 1:0065d
l_00663:
    mov edi, dword ptr [ebp + _raster_rows] ; 1:00663
l_00669:
    mov word ptr [ebp + _raster_rows], 0ffffh ; 1:00669
l_00672:
    mov ecx, dword ptr [ebp + _raster_rows + 04h] ; 1:00672
l_00678:
    mov word ptr [ebp + _raster_rows + 04h], 0 ; 1:00678
l_00681:
    cmp edi, 07e40h ; 1:00681
l_00687:
    jae near ptr l_02621 ; 1:00687
l_0068d:
    cmp ecx, 07d00h ; 1:0068d
l_00693:
    jb near ptr l_02621 ; 1:00693
l_00699:
    sub ecx, edi ; 1:00699
l_0069b:
    inc ecx ; 1:0069b
l_0069c:
    mov edx, dword ptr [ebp + _raster_rows + 014h] ; 1:0069c
l_006a2:
    mov esi, dword ptr [ebp + _raster_rows + 010h] ; 1:006a2
l_006a8:
    shl ecx, 010h ; 1:006a8
l_006ab:
    sub edx, esi ; 1:006ab
l_006ad:
    xor eax, eax ; 1:006ad
l_006af:
    inc edx ; 1:006af
l_006b0:
    shl esi, 8 ; 1:006b0
l_006b3:
    idiv ecx ; 1:006b3
l_006b5:
    mov ebx, eax ; 1:006b5
l_006b7:
    mov edx, dword ptr [ebp + _raster_rows + 0ch] ; 1:006b7
l_006bd:
    mov eax, dword ptr [ebp + _raster_rows + 08h] ; 1:006bd
l_006c3:
    sub edx, eax ; 1:006c3
l_006c5:
    add esi, eax ; 1:006c5
l_006c7:
    inc edx ; 1:006c7
l_006c8:
    xor eax, eax ; 1:006c8
l_006ca:
    idiv ecx ; 1:006ca
l_006cc:
    mov dx, ax ; 1:006cc
l_006cf:
    shl edx, 010h ; 1:006cf
l_006d2:
    mov ax, bx ; 1:006d2
l_006d5:
    rol eax, 010h ; 1:006d5
l_006d8:
    shr ebx, 010h ; 1:006d8
l_006db:
    mov dl, bl ; 1:006db
l_006dd:
    mov bx, si ; 1:006dd
l_006e0:
    xor esi, esi ; 1:006e0
l_006e2:
    shr ecx, 010h ; 1:006e2
l_006e5:
    sub edi, 07d00h ; 1:006e5
l_006eb:
    jns short l_0070b ; 1:006eb
l_006ed:
    mov ebp, eax ; 1:006ed
l_006ef:
    mov esi, edx ; 1:006ef
l_006f1:
    neg edi ; 1:006f1
l_006f3:
    sub ecx, edi ; 1:006f3
l_006f5:
    mul edi ; 1:006f5
l_006f7:
    add ebx, eax ; 1:006f7
l_006f9:
    adc bh, dl ; 1:006f9
l_006fb:
    mov eax, esi ; 1:006fb
l_006fd:
    mul edi ; 1:006fd
l_006ff:
    add bl, dl ; 1:006ff
l_00701:
    add bh, al ; 1:00701
l_00703:
    mov edx, esi ; 1:00703
l_00705:
    mov esi, eax ; 1:00705
l_00707:
    mov eax, ebp ; 1:00707
l_00709:
    xor edi, edi ; 1:00709
l_0070b:
    mov ebp, edi ; 1:0070b
l_0070d:
    add edi, ecx ; 1:0070d
l_0070f:
    cmp edi, 0140h ; 1:0070f
l_00715:
    jb short l_0071f ; 1:00715
l_00717:
    sub edi, 0140h ; 1:00717
l_0071d:
    sub ecx, edi ; 1:0071d
l_0071f:
    mov edi, ebp ; 1:0071f
l_00721:
    add edi, dword ptr [_raster_scanline] ; 1:00721
l_00727:
    mov ebp, edi ; 1:00727
l_00729:
    and ebp, 3 ; 1:00729
l_0072c:
    sub cx, bp ; 1:0072c
l_0072f:
    jns short l_00738 ; 1:0072f
l_00731:
    add cx, bp ; 1:00731
l_00734:
    mov ebp, ecx ; 1:00734
l_00736:
    xor ecx, ecx ; 1:00736
l_00738:
; Dispatch up to three leading pixels via 24-byte sample fragments.
    add edi, ebp ; 1:00738
l_0073a:
    push ecx ; 1:0073a
l_0073b:
    mov cx, bp ; 1:0073b
l_0073e:
    shl cx, 5 ; 1:0073e
l_00742:
    shl bp, 3 ; 1:00742
l_00746:
    sub cx, bp ; 1:00746
l_00749:
    sub ecx, offset l_0079b ; 1:00749
l_0074f:
    neg ecx ; 1:0074f
l_00751:
    jmp ecx ; 1:00751
l_00753:
    xor ebp, ebp ; 1:00753
l_00755:
    mov bp, bx ; 1:00755
l_00758:
    add ebp, dword ptr [_raster_texture] ; 1:00758
l_0075e:
    nop ; 1:0075e
l_0075f:
    mov cl, byte ptr [ebp] ; 1:0075f
l_00762:
    add esi, edx ; 1:00762
l_00764:
    adc ebx, eax ; 1:00764
l_00766:
    adc bh, dl ; 1:00766
l_00768:
    mov byte ptr [edi - 3], cl ; 1:00768
l_0076b:
    xor ebp, ebp ; 1:0076b
l_0076d:
    mov bp, bx ; 1:0076d
l_00770:
    add ebp, dword ptr [_raster_texture] ; 1:00770
l_00776:
    nop ; 1:00776
l_00777:
    mov cl, byte ptr [ebp] ; 1:00777
l_0077a:
    add esi, edx ; 1:0077a
l_0077c:
    adc ebx, eax ; 1:0077c
l_0077e:
    adc bh, dl ; 1:0077e
l_00780:
    mov byte ptr [edi - 2], cl ; 1:00780
l_00783:
    xor ebp, ebp ; 1:00783
l_00785:
    mov bp, bx ; 1:00785
l_00788:
    add ebp, dword ptr [_raster_texture] ; 1:00788
l_0078e:
    nop ; 1:0078e
l_0078f:
    mov cl, byte ptr [ebp] ; 1:0078f
l_00792:
    add esi, edx ; 1:00792
l_00794:
    adc ebx, eax ; 1:00794
l_00796:
    adc bh, dl ; 1:00796
l_00798:
    mov byte ptr [edi - 1], cl ; 1:00798
l_0079b:
; Dispatch dword output via 80 fixed 96-byte raster blocks.
    pop ecx ; 1:0079b
l_0079c:
    push ecx ; 1:0079c
l_0079d:
    shr ecx, 2 ; 1:0079d
l_007a0:
    lea edi, [edi + ecx*4] ; 1:007a0
l_007a3:
    mov bp, cx ; 1:007a3
l_007a6:
    shl bp, 5 ; 1:007a6
l_007aa:
    shl cx, 7 ; 1:007aa
l_007ae:
    sub cx, bp ; 1:007ae
l_007b1:
    sub ecx, offset l_025bb ; 1:007b1
l_007b7:
    neg ecx ; 1:007b7
l_007b9:
    jmp ecx ; 1:007b9
l_007bb:
; 80 explicit blocks, each 96 bytes. No macro expansion assumed.
; Raster dword block 0: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:007bb
l_007bd:
    mov bp, bx ; 1:007bd
l_007c0:
    add ebp, dword ptr [_raster_texture] ; 1:007c0
l_007c6:
    nop ; 1:007c6
l_007c7:
    mov cl, byte ptr [ebp] ; 1:007c7
l_007ca:
    add esi, edx ; 1:007ca
l_007cc:
    adc ebx, eax ; 1:007cc
l_007ce:
    adc bh, dl ; 1:007ce
l_007d0:
    xor ebp, ebp ; 1:007d0
l_007d2:
    mov bp, bx ; 1:007d2
l_007d5:
    add ebp, dword ptr [_raster_texture] ; 1:007d5
l_007db:
    nop ; 1:007db
l_007dc:
    mov ch, byte ptr [ebp] ; 1:007dc
l_007df:
    shl ecx, 010h ; 1:007df
l_007e2:
    add esi, edx ; 1:007e2
l_007e4:
    adc ebx, eax ; 1:007e4
l_007e6:
    adc bh, dl ; 1:007e6
l_007e8:
    xor ebp, ebp ; 1:007e8
l_007ea:
    mov bp, bx ; 1:007ea
l_007ed:
    add ebp, dword ptr [_raster_texture] ; 1:007ed
l_007f3:
    nop ; 1:007f3
l_007f4:
    mov cl, byte ptr [ebp] ; 1:007f4
l_007f7:
    add esi, edx ; 1:007f7
l_007f9:
    adc ebx, eax ; 1:007f9
l_007fb:
    adc bh, dl ; 1:007fb
l_007fd:
    xor ebp, ebp ; 1:007fd
l_007ff:
    mov bp, bx ; 1:007ff
l_00802:
    add ebp, dword ptr [_raster_texture] ; 1:00802
l_00808:
    nop ; 1:00808
l_00809:
    mov ch, byte ptr [ebp] ; 1:00809
l_0080c:
    rol ecx, 010h ; 1:0080c
l_0080f:
    add esi, edx ; 1:0080f
l_00811:
    adc ebx, eax ; 1:00811
l_00813:
    mov dword ptr [edi + 07fffh], ecx ; 1:00813
l_00819:
    adc bh, dl ; 1:00819
l_0081b:
; Raster dword block 1: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0081b
l_0081d:
    mov bp, bx ; 1:0081d
l_00820:
    add ebp, dword ptr [_raster_texture] ; 1:00820
l_00826:
    nop ; 1:00826
l_00827:
    mov cl, byte ptr [ebp] ; 1:00827
l_0082a:
    add esi, edx ; 1:0082a
l_0082c:
    adc ebx, eax ; 1:0082c
l_0082e:
    adc bh, dl ; 1:0082e
l_00830:
    xor ebp, ebp ; 1:00830
l_00832:
    mov bp, bx ; 1:00832
l_00835:
    add ebp, dword ptr [_raster_texture] ; 1:00835
l_0083b:
    nop ; 1:0083b
l_0083c:
    mov ch, byte ptr [ebp] ; 1:0083c
l_0083f:
    shl ecx, 010h ; 1:0083f
l_00842:
    add esi, edx ; 1:00842
l_00844:
    adc ebx, eax ; 1:00844
l_00846:
    adc bh, dl ; 1:00846
l_00848:
    xor ebp, ebp ; 1:00848
l_0084a:
    mov bp, bx ; 1:0084a
l_0084d:
    add ebp, dword ptr [_raster_texture] ; 1:0084d
l_00853:
    nop ; 1:00853
l_00854:
    mov cl, byte ptr [ebp] ; 1:00854
l_00857:
    add esi, edx ; 1:00857
l_00859:
    adc ebx, eax ; 1:00859
l_0085b:
    adc bh, dl ; 1:0085b
l_0085d:
    xor ebp, ebp ; 1:0085d
l_0085f:
    mov bp, bx ; 1:0085f
l_00862:
    add ebp, dword ptr [_raster_texture] ; 1:00862
l_00868:
    nop ; 1:00868
l_00869:
    mov ch, byte ptr [ebp] ; 1:00869
l_0086c:
    rol ecx, 010h ; 1:0086c
l_0086f:
    add esi, edx ; 1:0086f
l_00871:
    adc ebx, eax ; 1:00871
l_00873:
    mov dword ptr [edi + 07fffh], ecx ; 1:00873
l_00879:
    adc bh, dl ; 1:00879
l_0087b:
; Raster dword block 2: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0087b
l_0087d:
    mov bp, bx ; 1:0087d
l_00880:
    add ebp, dword ptr [_raster_texture] ; 1:00880
l_00886:
    nop ; 1:00886
l_00887:
    mov cl, byte ptr [ebp] ; 1:00887
l_0088a:
    add esi, edx ; 1:0088a
l_0088c:
    adc ebx, eax ; 1:0088c
l_0088e:
    adc bh, dl ; 1:0088e
l_00890:
    xor ebp, ebp ; 1:00890
l_00892:
    mov bp, bx ; 1:00892
l_00895:
    add ebp, dword ptr [_raster_texture] ; 1:00895
l_0089b:
    nop ; 1:0089b
l_0089c:
    mov ch, byte ptr [ebp] ; 1:0089c
l_0089f:
    shl ecx, 010h ; 1:0089f
l_008a2:
    add esi, edx ; 1:008a2
l_008a4:
    adc ebx, eax ; 1:008a4
l_008a6:
    adc bh, dl ; 1:008a6
l_008a8:
    xor ebp, ebp ; 1:008a8
l_008aa:
    mov bp, bx ; 1:008aa
l_008ad:
    add ebp, dword ptr [_raster_texture] ; 1:008ad
l_008b3:
    nop ; 1:008b3
l_008b4:
    mov cl, byte ptr [ebp] ; 1:008b4
l_008b7:
    add esi, edx ; 1:008b7
l_008b9:
    adc ebx, eax ; 1:008b9
l_008bb:
    adc bh, dl ; 1:008bb
l_008bd:
    xor ebp, ebp ; 1:008bd
l_008bf:
    mov bp, bx ; 1:008bf
l_008c2:
    add ebp, dword ptr [_raster_texture] ; 1:008c2
l_008c8:
    nop ; 1:008c8
l_008c9:
    mov ch, byte ptr [ebp] ; 1:008c9
l_008cc:
    rol ecx, 010h ; 1:008cc
l_008cf:
    add esi, edx ; 1:008cf
l_008d1:
    adc ebx, eax ; 1:008d1
l_008d3:
    mov dword ptr [edi + 07fffh], ecx ; 1:008d3
l_008d9:
    adc bh, dl ; 1:008d9
l_008db:
; Raster dword block 3: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:008db
l_008dd:
    mov bp, bx ; 1:008dd
l_008e0:
    add ebp, dword ptr [_raster_texture] ; 1:008e0
l_008e6:
    nop ; 1:008e6
l_008e7:
    mov cl, byte ptr [ebp] ; 1:008e7
l_008ea:
    add esi, edx ; 1:008ea
l_008ec:
    adc ebx, eax ; 1:008ec
l_008ee:
    adc bh, dl ; 1:008ee
l_008f0:
    xor ebp, ebp ; 1:008f0
l_008f2:
    mov bp, bx ; 1:008f2
l_008f5:
    add ebp, dword ptr [_raster_texture] ; 1:008f5
l_008fb:
    nop ; 1:008fb
l_008fc:
    mov ch, byte ptr [ebp] ; 1:008fc
l_008ff:
    shl ecx, 010h ; 1:008ff
l_00902:
    add esi, edx ; 1:00902
l_00904:
    adc ebx, eax ; 1:00904
l_00906:
    adc bh, dl ; 1:00906
l_00908:
    xor ebp, ebp ; 1:00908
l_0090a:
    mov bp, bx ; 1:0090a
l_0090d:
    add ebp, dword ptr [_raster_texture] ; 1:0090d
l_00913:
    nop ; 1:00913
l_00914:
    mov cl, byte ptr [ebp] ; 1:00914
l_00917:
    add esi, edx ; 1:00917
l_00919:
    adc ebx, eax ; 1:00919
l_0091b:
    adc bh, dl ; 1:0091b
l_0091d:
    xor ebp, ebp ; 1:0091d
l_0091f:
    mov bp, bx ; 1:0091f
l_00922:
    add ebp, dword ptr [_raster_texture] ; 1:00922
l_00928:
    nop ; 1:00928
l_00929:
    mov ch, byte ptr [ebp] ; 1:00929
l_0092c:
    rol ecx, 010h ; 1:0092c
l_0092f:
    add esi, edx ; 1:0092f
l_00931:
    adc ebx, eax ; 1:00931
l_00933:
    mov dword ptr [edi + 07fffh], ecx ; 1:00933
l_00939:
    adc bh, dl ; 1:00939
l_0093b:
; Raster dword block 4: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0093b
l_0093d:
    mov bp, bx ; 1:0093d
l_00940:
    add ebp, dword ptr [_raster_texture] ; 1:00940
l_00946:
    nop ; 1:00946
l_00947:
    mov cl, byte ptr [ebp] ; 1:00947
l_0094a:
    add esi, edx ; 1:0094a
l_0094c:
    adc ebx, eax ; 1:0094c
l_0094e:
    adc bh, dl ; 1:0094e
l_00950:
    xor ebp, ebp ; 1:00950
l_00952:
    mov bp, bx ; 1:00952
l_00955:
    add ebp, dword ptr [_raster_texture] ; 1:00955
l_0095b:
    nop ; 1:0095b
l_0095c:
    mov ch, byte ptr [ebp] ; 1:0095c
l_0095f:
    shl ecx, 010h ; 1:0095f
l_00962:
    add esi, edx ; 1:00962
l_00964:
    adc ebx, eax ; 1:00964
l_00966:
    adc bh, dl ; 1:00966
l_00968:
    xor ebp, ebp ; 1:00968
l_0096a:
    mov bp, bx ; 1:0096a
l_0096d:
    add ebp, dword ptr [_raster_texture] ; 1:0096d
l_00973:
    nop ; 1:00973
l_00974:
    mov cl, byte ptr [ebp] ; 1:00974
l_00977:
    add esi, edx ; 1:00977
l_00979:
    adc ebx, eax ; 1:00979
l_0097b:
    adc bh, dl ; 1:0097b
l_0097d:
    xor ebp, ebp ; 1:0097d
l_0097f:
    mov bp, bx ; 1:0097f
l_00982:
    add ebp, dword ptr [_raster_texture] ; 1:00982
l_00988:
    nop ; 1:00988
l_00989:
    mov ch, byte ptr [ebp] ; 1:00989
l_0098c:
    rol ecx, 010h ; 1:0098c
l_0098f:
    add esi, edx ; 1:0098f
l_00991:
    adc ebx, eax ; 1:00991
l_00993:
    mov dword ptr [edi + 07fffh], ecx ; 1:00993
l_00999:
    adc bh, dl ; 1:00999
l_0099b:
; Raster dword block 5: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0099b
l_0099d:
    mov bp, bx ; 1:0099d
l_009a0:
    add ebp, dword ptr [_raster_texture] ; 1:009a0
l_009a6:
    nop ; 1:009a6
l_009a7:
    mov cl, byte ptr [ebp] ; 1:009a7
l_009aa:
    add esi, edx ; 1:009aa
l_009ac:
    adc ebx, eax ; 1:009ac
l_009ae:
    adc bh, dl ; 1:009ae
l_009b0:
    xor ebp, ebp ; 1:009b0
l_009b2:
    mov bp, bx ; 1:009b2
l_009b5:
    add ebp, dword ptr [_raster_texture] ; 1:009b5
l_009bb:
    nop ; 1:009bb
l_009bc:
    mov ch, byte ptr [ebp] ; 1:009bc
l_009bf:
    shl ecx, 010h ; 1:009bf
l_009c2:
    add esi, edx ; 1:009c2
l_009c4:
    adc ebx, eax ; 1:009c4
l_009c6:
    adc bh, dl ; 1:009c6
l_009c8:
    xor ebp, ebp ; 1:009c8
l_009ca:
    mov bp, bx ; 1:009ca
l_009cd:
    add ebp, dword ptr [_raster_texture] ; 1:009cd
l_009d3:
    nop ; 1:009d3
l_009d4:
    mov cl, byte ptr [ebp] ; 1:009d4
l_009d7:
    add esi, edx ; 1:009d7
l_009d9:
    adc ebx, eax ; 1:009d9
l_009db:
    adc bh, dl ; 1:009db
l_009dd:
    xor ebp, ebp ; 1:009dd
l_009df:
    mov bp, bx ; 1:009df
l_009e2:
    add ebp, dword ptr [_raster_texture] ; 1:009e2
l_009e8:
    nop ; 1:009e8
l_009e9:
    mov ch, byte ptr [ebp] ; 1:009e9
l_009ec:
    rol ecx, 010h ; 1:009ec
l_009ef:
    add esi, edx ; 1:009ef
l_009f1:
    adc ebx, eax ; 1:009f1
l_009f3:
    mov dword ptr [edi + 07fffh], ecx ; 1:009f3
l_009f9:
    adc bh, dl ; 1:009f9
l_009fb:
; Raster dword block 6: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:009fb
l_009fd:
    mov bp, bx ; 1:009fd
l_00a00:
    add ebp, dword ptr [_raster_texture] ; 1:00a00
l_00a06:
    nop ; 1:00a06
l_00a07:
    mov cl, byte ptr [ebp] ; 1:00a07
l_00a0a:
    add esi, edx ; 1:00a0a
l_00a0c:
    adc ebx, eax ; 1:00a0c
l_00a0e:
    adc bh, dl ; 1:00a0e
l_00a10:
    xor ebp, ebp ; 1:00a10
l_00a12:
    mov bp, bx ; 1:00a12
l_00a15:
    add ebp, dword ptr [_raster_texture] ; 1:00a15
l_00a1b:
    nop ; 1:00a1b
l_00a1c:
    mov ch, byte ptr [ebp] ; 1:00a1c
l_00a1f:
    shl ecx, 010h ; 1:00a1f
l_00a22:
    add esi, edx ; 1:00a22
l_00a24:
    adc ebx, eax ; 1:00a24
l_00a26:
    adc bh, dl ; 1:00a26
l_00a28:
    xor ebp, ebp ; 1:00a28
l_00a2a:
    mov bp, bx ; 1:00a2a
l_00a2d:
    add ebp, dword ptr [_raster_texture] ; 1:00a2d
l_00a33:
    nop ; 1:00a33
l_00a34:
    mov cl, byte ptr [ebp] ; 1:00a34
l_00a37:
    add esi, edx ; 1:00a37
l_00a39:
    adc ebx, eax ; 1:00a39
l_00a3b:
    adc bh, dl ; 1:00a3b
l_00a3d:
    xor ebp, ebp ; 1:00a3d
l_00a3f:
    mov bp, bx ; 1:00a3f
l_00a42:
    add ebp, dword ptr [_raster_texture] ; 1:00a42
l_00a48:
    nop ; 1:00a48
l_00a49:
    mov ch, byte ptr [ebp] ; 1:00a49
l_00a4c:
    rol ecx, 010h ; 1:00a4c
l_00a4f:
    add esi, edx ; 1:00a4f
l_00a51:
    adc ebx, eax ; 1:00a51
l_00a53:
    mov dword ptr [edi + 07fffh], ecx ; 1:00a53
l_00a59:
    adc bh, dl ; 1:00a59
l_00a5b:
; Raster dword block 7: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:00a5b
l_00a5d:
    mov bp, bx ; 1:00a5d
l_00a60:
    add ebp, dword ptr [_raster_texture] ; 1:00a60
l_00a66:
    nop ; 1:00a66
l_00a67:
    mov cl, byte ptr [ebp] ; 1:00a67
l_00a6a:
    add esi, edx ; 1:00a6a
l_00a6c:
    adc ebx, eax ; 1:00a6c
l_00a6e:
    adc bh, dl ; 1:00a6e
l_00a70:
    xor ebp, ebp ; 1:00a70
l_00a72:
    mov bp, bx ; 1:00a72
l_00a75:
    add ebp, dword ptr [_raster_texture] ; 1:00a75
l_00a7b:
    nop ; 1:00a7b
l_00a7c:
    mov ch, byte ptr [ebp] ; 1:00a7c
l_00a7f:
    shl ecx, 010h ; 1:00a7f
l_00a82:
    add esi, edx ; 1:00a82
l_00a84:
    adc ebx, eax ; 1:00a84
l_00a86:
    adc bh, dl ; 1:00a86
l_00a88:
    xor ebp, ebp ; 1:00a88
l_00a8a:
    mov bp, bx ; 1:00a8a
l_00a8d:
    add ebp, dword ptr [_raster_texture] ; 1:00a8d
l_00a93:
    nop ; 1:00a93
l_00a94:
    mov cl, byte ptr [ebp] ; 1:00a94
l_00a97:
    add esi, edx ; 1:00a97
l_00a99:
    adc ebx, eax ; 1:00a99
l_00a9b:
    adc bh, dl ; 1:00a9b
l_00a9d:
    xor ebp, ebp ; 1:00a9d
l_00a9f:
    mov bp, bx ; 1:00a9f
l_00aa2:
    add ebp, dword ptr [_raster_texture] ; 1:00aa2
l_00aa8:
    nop ; 1:00aa8
l_00aa9:
    mov ch, byte ptr [ebp] ; 1:00aa9
l_00aac:
    rol ecx, 010h ; 1:00aac
l_00aaf:
    add esi, edx ; 1:00aaf
l_00ab1:
    adc ebx, eax ; 1:00ab1
l_00ab3:
    mov dword ptr [edi + 07fffh], ecx ; 1:00ab3
l_00ab9:
    adc bh, dl ; 1:00ab9
l_00abb:
; Raster dword block 8: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:00abb
l_00abd:
    mov bp, bx ; 1:00abd
l_00ac0:
    add ebp, dword ptr [_raster_texture] ; 1:00ac0
l_00ac6:
    nop ; 1:00ac6
l_00ac7:
    mov cl, byte ptr [ebp] ; 1:00ac7
l_00aca:
    add esi, edx ; 1:00aca
l_00acc:
    adc ebx, eax ; 1:00acc
l_00ace:
    adc bh, dl ; 1:00ace
l_00ad0:
    xor ebp, ebp ; 1:00ad0
l_00ad2:
    mov bp, bx ; 1:00ad2
l_00ad5:
    add ebp, dword ptr [_raster_texture] ; 1:00ad5
l_00adb:
    nop ; 1:00adb
l_00adc:
    mov ch, byte ptr [ebp] ; 1:00adc
l_00adf:
    shl ecx, 010h ; 1:00adf
l_00ae2:
    add esi, edx ; 1:00ae2
l_00ae4:
    adc ebx, eax ; 1:00ae4
l_00ae6:
    adc bh, dl ; 1:00ae6
l_00ae8:
    xor ebp, ebp ; 1:00ae8
l_00aea:
    mov bp, bx ; 1:00aea
l_00aed:
    add ebp, dword ptr [_raster_texture] ; 1:00aed
l_00af3:
    nop ; 1:00af3
l_00af4:
    mov cl, byte ptr [ebp] ; 1:00af4
l_00af7:
    add esi, edx ; 1:00af7
l_00af9:
    adc ebx, eax ; 1:00af9
l_00afb:
    adc bh, dl ; 1:00afb
l_00afd:
    xor ebp, ebp ; 1:00afd
l_00aff:
    mov bp, bx ; 1:00aff
l_00b02:
    add ebp, dword ptr [_raster_texture] ; 1:00b02
l_00b08:
    nop ; 1:00b08
l_00b09:
    mov ch, byte ptr [ebp] ; 1:00b09
l_00b0c:
    rol ecx, 010h ; 1:00b0c
l_00b0f:
    add esi, edx ; 1:00b0f
l_00b11:
    adc ebx, eax ; 1:00b11
l_00b13:
    mov dword ptr [edi + 07fffh], ecx ; 1:00b13
l_00b19:
    adc bh, dl ; 1:00b19
l_00b1b:
; Raster dword block 9: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:00b1b
l_00b1d:
    mov bp, bx ; 1:00b1d
l_00b20:
    add ebp, dword ptr [_raster_texture] ; 1:00b20
l_00b26:
    nop ; 1:00b26
l_00b27:
    mov cl, byte ptr [ebp] ; 1:00b27
l_00b2a:
    add esi, edx ; 1:00b2a
l_00b2c:
    adc ebx, eax ; 1:00b2c
l_00b2e:
    adc bh, dl ; 1:00b2e
l_00b30:
    xor ebp, ebp ; 1:00b30
l_00b32:
    mov bp, bx ; 1:00b32
l_00b35:
    add ebp, dword ptr [_raster_texture] ; 1:00b35
l_00b3b:
    nop ; 1:00b3b
l_00b3c:
    mov ch, byte ptr [ebp] ; 1:00b3c
l_00b3f:
    shl ecx, 010h ; 1:00b3f
l_00b42:
    add esi, edx ; 1:00b42
l_00b44:
    adc ebx, eax ; 1:00b44
l_00b46:
    adc bh, dl ; 1:00b46
l_00b48:
    xor ebp, ebp ; 1:00b48
l_00b4a:
    mov bp, bx ; 1:00b4a
l_00b4d:
    add ebp, dword ptr [_raster_texture] ; 1:00b4d
l_00b53:
    nop ; 1:00b53
l_00b54:
    mov cl, byte ptr [ebp] ; 1:00b54
l_00b57:
    add esi, edx ; 1:00b57
l_00b59:
    adc ebx, eax ; 1:00b59
l_00b5b:
    adc bh, dl ; 1:00b5b
l_00b5d:
    xor ebp, ebp ; 1:00b5d
l_00b5f:
    mov bp, bx ; 1:00b5f
l_00b62:
    add ebp, dword ptr [_raster_texture] ; 1:00b62
l_00b68:
    nop ; 1:00b68
l_00b69:
    mov ch, byte ptr [ebp] ; 1:00b69
l_00b6c:
    rol ecx, 010h ; 1:00b6c
l_00b6f:
    add esi, edx ; 1:00b6f
l_00b71:
    adc ebx, eax ; 1:00b71
l_00b73:
    mov dword ptr [edi + 07fffh], ecx ; 1:00b73
l_00b79:
    adc bh, dl ; 1:00b79
l_00b7b:
; Raster dword block 10: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:00b7b
l_00b7d:
    mov bp, bx ; 1:00b7d
l_00b80:
    add ebp, dword ptr [_raster_texture] ; 1:00b80
l_00b86:
    nop ; 1:00b86
l_00b87:
    mov cl, byte ptr [ebp] ; 1:00b87
l_00b8a:
    add esi, edx ; 1:00b8a
l_00b8c:
    adc ebx, eax ; 1:00b8c
l_00b8e:
    adc bh, dl ; 1:00b8e
l_00b90:
    xor ebp, ebp ; 1:00b90
l_00b92:
    mov bp, bx ; 1:00b92
l_00b95:
    add ebp, dword ptr [_raster_texture] ; 1:00b95
l_00b9b:
    nop ; 1:00b9b
l_00b9c:
    mov ch, byte ptr [ebp] ; 1:00b9c
l_00b9f:
    shl ecx, 010h ; 1:00b9f
l_00ba2:
    add esi, edx ; 1:00ba2
l_00ba4:
    adc ebx, eax ; 1:00ba4
l_00ba6:
    adc bh, dl ; 1:00ba6
l_00ba8:
    xor ebp, ebp ; 1:00ba8
l_00baa:
    mov bp, bx ; 1:00baa
l_00bad:
    add ebp, dword ptr [_raster_texture] ; 1:00bad
l_00bb3:
    nop ; 1:00bb3
l_00bb4:
    mov cl, byte ptr [ebp] ; 1:00bb4
l_00bb7:
    add esi, edx ; 1:00bb7
l_00bb9:
    adc ebx, eax ; 1:00bb9
l_00bbb:
    adc bh, dl ; 1:00bbb
l_00bbd:
    xor ebp, ebp ; 1:00bbd
l_00bbf:
    mov bp, bx ; 1:00bbf
l_00bc2:
    add ebp, dword ptr [_raster_texture] ; 1:00bc2
l_00bc8:
    nop ; 1:00bc8
l_00bc9:
    mov ch, byte ptr [ebp] ; 1:00bc9
l_00bcc:
    rol ecx, 010h ; 1:00bcc
l_00bcf:
    add esi, edx ; 1:00bcf
l_00bd1:
    adc ebx, eax ; 1:00bd1
l_00bd3:
    mov dword ptr [edi + 07fffh], ecx ; 1:00bd3
l_00bd9:
    adc bh, dl ; 1:00bd9
l_00bdb:
; Raster dword block 11: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:00bdb
l_00bdd:
    mov bp, bx ; 1:00bdd
l_00be0:
    add ebp, dword ptr [_raster_texture] ; 1:00be0
l_00be6:
    nop ; 1:00be6
l_00be7:
    mov cl, byte ptr [ebp] ; 1:00be7
l_00bea:
    add esi, edx ; 1:00bea
l_00bec:
    adc ebx, eax ; 1:00bec
l_00bee:
    adc bh, dl ; 1:00bee
l_00bf0:
    xor ebp, ebp ; 1:00bf0
l_00bf2:
    mov bp, bx ; 1:00bf2
l_00bf5:
    add ebp, dword ptr [_raster_texture] ; 1:00bf5
l_00bfb:
    nop ; 1:00bfb
l_00bfc:
    mov ch, byte ptr [ebp] ; 1:00bfc
l_00bff:
    shl ecx, 010h ; 1:00bff
l_00c02:
    add esi, edx ; 1:00c02
l_00c04:
    adc ebx, eax ; 1:00c04
l_00c06:
    adc bh, dl ; 1:00c06
l_00c08:
    xor ebp, ebp ; 1:00c08
l_00c0a:
    mov bp, bx ; 1:00c0a
l_00c0d:
    add ebp, dword ptr [_raster_texture] ; 1:00c0d
l_00c13:
    nop ; 1:00c13
l_00c14:
    mov cl, byte ptr [ebp] ; 1:00c14
l_00c17:
    add esi, edx ; 1:00c17
l_00c19:
    adc ebx, eax ; 1:00c19
l_00c1b:
    adc bh, dl ; 1:00c1b
l_00c1d:
    xor ebp, ebp ; 1:00c1d
l_00c1f:
    mov bp, bx ; 1:00c1f
l_00c22:
    add ebp, dword ptr [_raster_texture] ; 1:00c22
l_00c28:
    nop ; 1:00c28
l_00c29:
    mov ch, byte ptr [ebp] ; 1:00c29
l_00c2c:
    rol ecx, 010h ; 1:00c2c
l_00c2f:
    add esi, edx ; 1:00c2f
l_00c31:
    adc ebx, eax ; 1:00c31
l_00c33:
    mov dword ptr [edi + 07fffh], ecx ; 1:00c33
l_00c39:
    adc bh, dl ; 1:00c39
l_00c3b:
; Raster dword block 12: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:00c3b
l_00c3d:
    mov bp, bx ; 1:00c3d
l_00c40:
    add ebp, dword ptr [_raster_texture] ; 1:00c40
l_00c46:
    nop ; 1:00c46
l_00c47:
    mov cl, byte ptr [ebp] ; 1:00c47
l_00c4a:
    add esi, edx ; 1:00c4a
l_00c4c:
    adc ebx, eax ; 1:00c4c
l_00c4e:
    adc bh, dl ; 1:00c4e
l_00c50:
    xor ebp, ebp ; 1:00c50
l_00c52:
    mov bp, bx ; 1:00c52
l_00c55:
    add ebp, dword ptr [_raster_texture] ; 1:00c55
l_00c5b:
    nop ; 1:00c5b
l_00c5c:
    mov ch, byte ptr [ebp] ; 1:00c5c
l_00c5f:
    shl ecx, 010h ; 1:00c5f
l_00c62:
    add esi, edx ; 1:00c62
l_00c64:
    adc ebx, eax ; 1:00c64
l_00c66:
    adc bh, dl ; 1:00c66
l_00c68:
    xor ebp, ebp ; 1:00c68
l_00c6a:
    mov bp, bx ; 1:00c6a
l_00c6d:
    add ebp, dword ptr [_raster_texture] ; 1:00c6d
l_00c73:
    nop ; 1:00c73
l_00c74:
    mov cl, byte ptr [ebp] ; 1:00c74
l_00c77:
    add esi, edx ; 1:00c77
l_00c79:
    adc ebx, eax ; 1:00c79
l_00c7b:
    adc bh, dl ; 1:00c7b
l_00c7d:
    xor ebp, ebp ; 1:00c7d
l_00c7f:
    mov bp, bx ; 1:00c7f
l_00c82:
    add ebp, dword ptr [_raster_texture] ; 1:00c82
l_00c88:
    nop ; 1:00c88
l_00c89:
    mov ch, byte ptr [ebp] ; 1:00c89
l_00c8c:
    rol ecx, 010h ; 1:00c8c
l_00c8f:
    add esi, edx ; 1:00c8f
l_00c91:
    adc ebx, eax ; 1:00c91
l_00c93:
    mov dword ptr [edi + 07fffh], ecx ; 1:00c93
l_00c99:
    adc bh, dl ; 1:00c99
l_00c9b:
; Raster dword block 13: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:00c9b
l_00c9d:
    mov bp, bx ; 1:00c9d
l_00ca0:
    add ebp, dword ptr [_raster_texture] ; 1:00ca0
l_00ca6:
    nop ; 1:00ca6
l_00ca7:
    mov cl, byte ptr [ebp] ; 1:00ca7
l_00caa:
    add esi, edx ; 1:00caa
l_00cac:
    adc ebx, eax ; 1:00cac
l_00cae:
    adc bh, dl ; 1:00cae
l_00cb0:
    xor ebp, ebp ; 1:00cb0
l_00cb2:
    mov bp, bx ; 1:00cb2
l_00cb5:
    add ebp, dword ptr [_raster_texture] ; 1:00cb5
l_00cbb:
    nop ; 1:00cbb
l_00cbc:
    mov ch, byte ptr [ebp] ; 1:00cbc
l_00cbf:
    shl ecx, 010h ; 1:00cbf
l_00cc2:
    add esi, edx ; 1:00cc2
l_00cc4:
    adc ebx, eax ; 1:00cc4
l_00cc6:
    adc bh, dl ; 1:00cc6
l_00cc8:
    xor ebp, ebp ; 1:00cc8
l_00cca:
    mov bp, bx ; 1:00cca
l_00ccd:
    add ebp, dword ptr [_raster_texture] ; 1:00ccd
l_00cd3:
    nop ; 1:00cd3
l_00cd4:
    mov cl, byte ptr [ebp] ; 1:00cd4
l_00cd7:
    add esi, edx ; 1:00cd7
l_00cd9:
    adc ebx, eax ; 1:00cd9
l_00cdb:
    adc bh, dl ; 1:00cdb
l_00cdd:
    xor ebp, ebp ; 1:00cdd
l_00cdf:
    mov bp, bx ; 1:00cdf
l_00ce2:
    add ebp, dword ptr [_raster_texture] ; 1:00ce2
l_00ce8:
    nop ; 1:00ce8
l_00ce9:
    mov ch, byte ptr [ebp] ; 1:00ce9
l_00cec:
    rol ecx, 010h ; 1:00cec
l_00cef:
    add esi, edx ; 1:00cef
l_00cf1:
    adc ebx, eax ; 1:00cf1
l_00cf3:
    mov dword ptr [edi + 07fffh], ecx ; 1:00cf3
l_00cf9:
    adc bh, dl ; 1:00cf9
l_00cfb:
; Raster dword block 14: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:00cfb
l_00cfd:
    mov bp, bx ; 1:00cfd
l_00d00:
    add ebp, dword ptr [_raster_texture] ; 1:00d00
l_00d06:
    nop ; 1:00d06
l_00d07:
    mov cl, byte ptr [ebp] ; 1:00d07
l_00d0a:
    add esi, edx ; 1:00d0a
l_00d0c:
    adc ebx, eax ; 1:00d0c
l_00d0e:
    adc bh, dl ; 1:00d0e
l_00d10:
    xor ebp, ebp ; 1:00d10
l_00d12:
    mov bp, bx ; 1:00d12
l_00d15:
    add ebp, dword ptr [_raster_texture] ; 1:00d15
l_00d1b:
    nop ; 1:00d1b
l_00d1c:
    mov ch, byte ptr [ebp] ; 1:00d1c
l_00d1f:
    shl ecx, 010h ; 1:00d1f
l_00d22:
    add esi, edx ; 1:00d22
l_00d24:
    adc ebx, eax ; 1:00d24
l_00d26:
    adc bh, dl ; 1:00d26
l_00d28:
    xor ebp, ebp ; 1:00d28
l_00d2a:
    mov bp, bx ; 1:00d2a
l_00d2d:
    add ebp, dword ptr [_raster_texture] ; 1:00d2d
l_00d33:
    nop ; 1:00d33
l_00d34:
    mov cl, byte ptr [ebp] ; 1:00d34
l_00d37:
    add esi, edx ; 1:00d37
l_00d39:
    adc ebx, eax ; 1:00d39
l_00d3b:
    adc bh, dl ; 1:00d3b
l_00d3d:
    xor ebp, ebp ; 1:00d3d
l_00d3f:
    mov bp, bx ; 1:00d3f
l_00d42:
    add ebp, dword ptr [_raster_texture] ; 1:00d42
l_00d48:
    nop ; 1:00d48
l_00d49:
    mov ch, byte ptr [ebp] ; 1:00d49
l_00d4c:
    rol ecx, 010h ; 1:00d4c
l_00d4f:
    add esi, edx ; 1:00d4f
l_00d51:
    adc ebx, eax ; 1:00d51
l_00d53:
    mov dword ptr [edi + 07fffh], ecx ; 1:00d53
l_00d59:
    adc bh, dl ; 1:00d59
l_00d5b:
; Raster dword block 15: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:00d5b
l_00d5d:
    mov bp, bx ; 1:00d5d
l_00d60:
    add ebp, dword ptr [_raster_texture] ; 1:00d60
l_00d66:
    nop ; 1:00d66
l_00d67:
    mov cl, byte ptr [ebp] ; 1:00d67
l_00d6a:
    add esi, edx ; 1:00d6a
l_00d6c:
    adc ebx, eax ; 1:00d6c
l_00d6e:
    adc bh, dl ; 1:00d6e
l_00d70:
    xor ebp, ebp ; 1:00d70
l_00d72:
    mov bp, bx ; 1:00d72
l_00d75:
    add ebp, dword ptr [_raster_texture] ; 1:00d75
l_00d7b:
    nop ; 1:00d7b
l_00d7c:
    mov ch, byte ptr [ebp] ; 1:00d7c
l_00d7f:
    shl ecx, 010h ; 1:00d7f
l_00d82:
    add esi, edx ; 1:00d82
l_00d84:
    adc ebx, eax ; 1:00d84
l_00d86:
    adc bh, dl ; 1:00d86
l_00d88:
    xor ebp, ebp ; 1:00d88
l_00d8a:
    mov bp, bx ; 1:00d8a
l_00d8d:
    add ebp, dword ptr [_raster_texture] ; 1:00d8d
l_00d93:
    nop ; 1:00d93
l_00d94:
    mov cl, byte ptr [ebp] ; 1:00d94
l_00d97:
    add esi, edx ; 1:00d97
l_00d99:
    adc ebx, eax ; 1:00d99
l_00d9b:
    adc bh, dl ; 1:00d9b
l_00d9d:
    xor ebp, ebp ; 1:00d9d
l_00d9f:
    mov bp, bx ; 1:00d9f
l_00da2:
    add ebp, dword ptr [_raster_texture] ; 1:00da2
l_00da8:
    nop ; 1:00da8
l_00da9:
    mov ch, byte ptr [ebp] ; 1:00da9
l_00dac:
    rol ecx, 010h ; 1:00dac
l_00daf:
    add esi, edx ; 1:00daf
l_00db1:
    adc ebx, eax ; 1:00db1
l_00db3:
    mov dword ptr [edi + 07fffh], ecx ; 1:00db3
l_00db9:
    adc bh, dl ; 1:00db9
l_00dbb:
; Raster dword block 16: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:00dbb
l_00dbd:
    mov bp, bx ; 1:00dbd
l_00dc0:
    add ebp, dword ptr [_raster_texture] ; 1:00dc0
l_00dc6:
    nop ; 1:00dc6
l_00dc7:
    mov cl, byte ptr [ebp] ; 1:00dc7
l_00dca:
    add esi, edx ; 1:00dca
l_00dcc:
    adc ebx, eax ; 1:00dcc
l_00dce:
    adc bh, dl ; 1:00dce
l_00dd0:
    xor ebp, ebp ; 1:00dd0
l_00dd2:
    mov bp, bx ; 1:00dd2
l_00dd5:
    add ebp, dword ptr [_raster_texture] ; 1:00dd5
l_00ddb:
    nop ; 1:00ddb
l_00ddc:
    mov ch, byte ptr [ebp] ; 1:00ddc
l_00ddf:
    shl ecx, 010h ; 1:00ddf
l_00de2:
    add esi, edx ; 1:00de2
l_00de4:
    adc ebx, eax ; 1:00de4
l_00de6:
    adc bh, dl ; 1:00de6
l_00de8:
    xor ebp, ebp ; 1:00de8
l_00dea:
    mov bp, bx ; 1:00dea
l_00ded:
    add ebp, dword ptr [_raster_texture] ; 1:00ded
l_00df3:
    nop ; 1:00df3
l_00df4:
    mov cl, byte ptr [ebp] ; 1:00df4
l_00df7:
    add esi, edx ; 1:00df7
l_00df9:
    adc ebx, eax ; 1:00df9
l_00dfb:
    adc bh, dl ; 1:00dfb
l_00dfd:
    xor ebp, ebp ; 1:00dfd
l_00dff:
    mov bp, bx ; 1:00dff
l_00e02:
    add ebp, dword ptr [_raster_texture] ; 1:00e02
l_00e08:
    nop ; 1:00e08
l_00e09:
    mov ch, byte ptr [ebp] ; 1:00e09
l_00e0c:
    rol ecx, 010h ; 1:00e0c
l_00e0f:
    add esi, edx ; 1:00e0f
l_00e11:
    adc ebx, eax ; 1:00e11
l_00e13:
    mov dword ptr [edi + 07fffh], ecx ; 1:00e13
l_00e19:
    adc bh, dl ; 1:00e19
l_00e1b:
; Raster dword block 17: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:00e1b
l_00e1d:
    mov bp, bx ; 1:00e1d
l_00e20:
    add ebp, dword ptr [_raster_texture] ; 1:00e20
l_00e26:
    nop ; 1:00e26
l_00e27:
    mov cl, byte ptr [ebp] ; 1:00e27
l_00e2a:
    add esi, edx ; 1:00e2a
l_00e2c:
    adc ebx, eax ; 1:00e2c
l_00e2e:
    adc bh, dl ; 1:00e2e
l_00e30:
    xor ebp, ebp ; 1:00e30
l_00e32:
    mov bp, bx ; 1:00e32
l_00e35:
    add ebp, dword ptr [_raster_texture] ; 1:00e35
l_00e3b:
    nop ; 1:00e3b
l_00e3c:
    mov ch, byte ptr [ebp] ; 1:00e3c
l_00e3f:
    shl ecx, 010h ; 1:00e3f
l_00e42:
    add esi, edx ; 1:00e42
l_00e44:
    adc ebx, eax ; 1:00e44
l_00e46:
    adc bh, dl ; 1:00e46
l_00e48:
    xor ebp, ebp ; 1:00e48
l_00e4a:
    mov bp, bx ; 1:00e4a
l_00e4d:
    add ebp, dword ptr [_raster_texture] ; 1:00e4d
l_00e53:
    nop ; 1:00e53
l_00e54:
    mov cl, byte ptr [ebp] ; 1:00e54
l_00e57:
    add esi, edx ; 1:00e57
l_00e59:
    adc ebx, eax ; 1:00e59
l_00e5b:
    adc bh, dl ; 1:00e5b
l_00e5d:
    xor ebp, ebp ; 1:00e5d
l_00e5f:
    mov bp, bx ; 1:00e5f
l_00e62:
    add ebp, dword ptr [_raster_texture] ; 1:00e62
l_00e68:
    nop ; 1:00e68
l_00e69:
    mov ch, byte ptr [ebp] ; 1:00e69
l_00e6c:
    rol ecx, 010h ; 1:00e6c
l_00e6f:
    add esi, edx ; 1:00e6f
l_00e71:
    adc ebx, eax ; 1:00e71
l_00e73:
    mov dword ptr [edi + 07fffh], ecx ; 1:00e73
l_00e79:
    adc bh, dl ; 1:00e79
l_00e7b:
; Raster dword block 18: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:00e7b
l_00e7d:
    mov bp, bx ; 1:00e7d
l_00e80:
    add ebp, dword ptr [_raster_texture] ; 1:00e80
l_00e86:
    nop ; 1:00e86
l_00e87:
    mov cl, byte ptr [ebp] ; 1:00e87
l_00e8a:
    add esi, edx ; 1:00e8a
l_00e8c:
    adc ebx, eax ; 1:00e8c
l_00e8e:
    adc bh, dl ; 1:00e8e
l_00e90:
    xor ebp, ebp ; 1:00e90
l_00e92:
    mov bp, bx ; 1:00e92
l_00e95:
    add ebp, dword ptr [_raster_texture] ; 1:00e95
l_00e9b:
    nop ; 1:00e9b
l_00e9c:
    mov ch, byte ptr [ebp] ; 1:00e9c
l_00e9f:
    shl ecx, 010h ; 1:00e9f
l_00ea2:
    add esi, edx ; 1:00ea2
l_00ea4:
    adc ebx, eax ; 1:00ea4
l_00ea6:
    adc bh, dl ; 1:00ea6
l_00ea8:
    xor ebp, ebp ; 1:00ea8
l_00eaa:
    mov bp, bx ; 1:00eaa
l_00ead:
    add ebp, dword ptr [_raster_texture] ; 1:00ead
l_00eb3:
    nop ; 1:00eb3
l_00eb4:
    mov cl, byte ptr [ebp] ; 1:00eb4
l_00eb7:
    add esi, edx ; 1:00eb7
l_00eb9:
    adc ebx, eax ; 1:00eb9
l_00ebb:
    adc bh, dl ; 1:00ebb
l_00ebd:
    xor ebp, ebp ; 1:00ebd
l_00ebf:
    mov bp, bx ; 1:00ebf
l_00ec2:
    add ebp, dword ptr [_raster_texture] ; 1:00ec2
l_00ec8:
    nop ; 1:00ec8
l_00ec9:
    mov ch, byte ptr [ebp] ; 1:00ec9
l_00ecc:
    rol ecx, 010h ; 1:00ecc
l_00ecf:
    add esi, edx ; 1:00ecf
l_00ed1:
    adc ebx, eax ; 1:00ed1
l_00ed3:
    mov dword ptr [edi + 07fffh], ecx ; 1:00ed3
l_00ed9:
    adc bh, dl ; 1:00ed9
l_00edb:
; Raster dword block 19: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:00edb
l_00edd:
    mov bp, bx ; 1:00edd
l_00ee0:
    add ebp, dword ptr [_raster_texture] ; 1:00ee0
l_00ee6:
    nop ; 1:00ee6
l_00ee7:
    mov cl, byte ptr [ebp] ; 1:00ee7
l_00eea:
    add esi, edx ; 1:00eea
l_00eec:
    adc ebx, eax ; 1:00eec
l_00eee:
    adc bh, dl ; 1:00eee
l_00ef0:
    xor ebp, ebp ; 1:00ef0
l_00ef2:
    mov bp, bx ; 1:00ef2
l_00ef5:
    add ebp, dword ptr [_raster_texture] ; 1:00ef5
l_00efb:
    nop ; 1:00efb
l_00efc:
    mov ch, byte ptr [ebp] ; 1:00efc
l_00eff:
    shl ecx, 010h ; 1:00eff
l_00f02:
    add esi, edx ; 1:00f02
l_00f04:
    adc ebx, eax ; 1:00f04
l_00f06:
    adc bh, dl ; 1:00f06
l_00f08:
    xor ebp, ebp ; 1:00f08
l_00f0a:
    mov bp, bx ; 1:00f0a
l_00f0d:
    add ebp, dword ptr [_raster_texture] ; 1:00f0d
l_00f13:
    nop ; 1:00f13
l_00f14:
    mov cl, byte ptr [ebp] ; 1:00f14
l_00f17:
    add esi, edx ; 1:00f17
l_00f19:
    adc ebx, eax ; 1:00f19
l_00f1b:
    adc bh, dl ; 1:00f1b
l_00f1d:
    xor ebp, ebp ; 1:00f1d
l_00f1f:
    mov bp, bx ; 1:00f1f
l_00f22:
    add ebp, dword ptr [_raster_texture] ; 1:00f22
l_00f28:
    nop ; 1:00f28
l_00f29:
    mov ch, byte ptr [ebp] ; 1:00f29
l_00f2c:
    rol ecx, 010h ; 1:00f2c
l_00f2f:
    add esi, edx ; 1:00f2f
l_00f31:
    adc ebx, eax ; 1:00f31
l_00f33:
    mov dword ptr [edi + 07fffh], ecx ; 1:00f33
l_00f39:
    adc bh, dl ; 1:00f39
l_00f3b:
; Raster dword block 20: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:00f3b
l_00f3d:
    mov bp, bx ; 1:00f3d
l_00f40:
    add ebp, dword ptr [_raster_texture] ; 1:00f40
l_00f46:
    nop ; 1:00f46
l_00f47:
    mov cl, byte ptr [ebp] ; 1:00f47
l_00f4a:
    add esi, edx ; 1:00f4a
l_00f4c:
    adc ebx, eax ; 1:00f4c
l_00f4e:
    adc bh, dl ; 1:00f4e
l_00f50:
    xor ebp, ebp ; 1:00f50
l_00f52:
    mov bp, bx ; 1:00f52
l_00f55:
    add ebp, dword ptr [_raster_texture] ; 1:00f55
l_00f5b:
    nop ; 1:00f5b
l_00f5c:
    mov ch, byte ptr [ebp] ; 1:00f5c
l_00f5f:
    shl ecx, 010h ; 1:00f5f
l_00f62:
    add esi, edx ; 1:00f62
l_00f64:
    adc ebx, eax ; 1:00f64
l_00f66:
    adc bh, dl ; 1:00f66
l_00f68:
    xor ebp, ebp ; 1:00f68
l_00f6a:
    mov bp, bx ; 1:00f6a
l_00f6d:
    add ebp, dword ptr [_raster_texture] ; 1:00f6d
l_00f73:
    nop ; 1:00f73
l_00f74:
    mov cl, byte ptr [ebp] ; 1:00f74
l_00f77:
    add esi, edx ; 1:00f77
l_00f79:
    adc ebx, eax ; 1:00f79
l_00f7b:
    adc bh, dl ; 1:00f7b
l_00f7d:
    xor ebp, ebp ; 1:00f7d
l_00f7f:
    mov bp, bx ; 1:00f7f
l_00f82:
    add ebp, dword ptr [_raster_texture] ; 1:00f82
l_00f88:
    nop ; 1:00f88
l_00f89:
    mov ch, byte ptr [ebp] ; 1:00f89
l_00f8c:
    rol ecx, 010h ; 1:00f8c
l_00f8f:
    add esi, edx ; 1:00f8f
l_00f91:
    adc ebx, eax ; 1:00f91
l_00f93:
    mov dword ptr [edi + 07fffh], ecx ; 1:00f93
l_00f99:
    adc bh, dl ; 1:00f99
l_00f9b:
; Raster dword block 21: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:00f9b
l_00f9d:
    mov bp, bx ; 1:00f9d
l_00fa0:
    add ebp, dword ptr [_raster_texture] ; 1:00fa0
l_00fa6:
    nop ; 1:00fa6
l_00fa7:
    mov cl, byte ptr [ebp] ; 1:00fa7
l_00faa:
    add esi, edx ; 1:00faa
l_00fac:
    adc ebx, eax ; 1:00fac
l_00fae:
    adc bh, dl ; 1:00fae
l_00fb0:
    xor ebp, ebp ; 1:00fb0
l_00fb2:
    mov bp, bx ; 1:00fb2
l_00fb5:
    add ebp, dword ptr [_raster_texture] ; 1:00fb5
l_00fbb:
    nop ; 1:00fbb
l_00fbc:
    mov ch, byte ptr [ebp] ; 1:00fbc
l_00fbf:
    shl ecx, 010h ; 1:00fbf
l_00fc2:
    add esi, edx ; 1:00fc2
l_00fc4:
    adc ebx, eax ; 1:00fc4
l_00fc6:
    adc bh, dl ; 1:00fc6
l_00fc8:
    xor ebp, ebp ; 1:00fc8
l_00fca:
    mov bp, bx ; 1:00fca
l_00fcd:
    add ebp, dword ptr [_raster_texture] ; 1:00fcd
l_00fd3:
    nop ; 1:00fd3
l_00fd4:
    mov cl, byte ptr [ebp] ; 1:00fd4
l_00fd7:
    add esi, edx ; 1:00fd7
l_00fd9:
    adc ebx, eax ; 1:00fd9
l_00fdb:
    adc bh, dl ; 1:00fdb
l_00fdd:
    xor ebp, ebp ; 1:00fdd
l_00fdf:
    mov bp, bx ; 1:00fdf
l_00fe2:
    add ebp, dword ptr [_raster_texture] ; 1:00fe2
l_00fe8:
    nop ; 1:00fe8
l_00fe9:
    mov ch, byte ptr [ebp] ; 1:00fe9
l_00fec:
    rol ecx, 010h ; 1:00fec
l_00fef:
    add esi, edx ; 1:00fef
l_00ff1:
    adc ebx, eax ; 1:00ff1
l_00ff3:
    mov dword ptr [edi + 07fffh], ecx ; 1:00ff3
l_00ff9:
    adc bh, dl ; 1:00ff9
l_00ffb:
; Raster dword block 22: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:00ffb
l_00ffd:
    mov bp, bx ; 1:00ffd
l_01000:
    add ebp, dword ptr [_raster_texture] ; 1:01000
l_01006:
    nop ; 1:01006
l_01007:
    mov cl, byte ptr [ebp] ; 1:01007
l_0100a:
    add esi, edx ; 1:0100a
l_0100c:
    adc ebx, eax ; 1:0100c
l_0100e:
    adc bh, dl ; 1:0100e
l_01010:
    xor ebp, ebp ; 1:01010
l_01012:
    mov bp, bx ; 1:01012
l_01015:
    add ebp, dword ptr [_raster_texture] ; 1:01015
l_0101b:
    nop ; 1:0101b
l_0101c:
    mov ch, byte ptr [ebp] ; 1:0101c
l_0101f:
    shl ecx, 010h ; 1:0101f
l_01022:
    add esi, edx ; 1:01022
l_01024:
    adc ebx, eax ; 1:01024
l_01026:
    adc bh, dl ; 1:01026
l_01028:
    xor ebp, ebp ; 1:01028
l_0102a:
    mov bp, bx ; 1:0102a
l_0102d:
    add ebp, dword ptr [_raster_texture] ; 1:0102d
l_01033:
    nop ; 1:01033
l_01034:
    mov cl, byte ptr [ebp] ; 1:01034
l_01037:
    add esi, edx ; 1:01037
l_01039:
    adc ebx, eax ; 1:01039
l_0103b:
    adc bh, dl ; 1:0103b
l_0103d:
    xor ebp, ebp ; 1:0103d
l_0103f:
    mov bp, bx ; 1:0103f
l_01042:
    add ebp, dword ptr [_raster_texture] ; 1:01042
l_01048:
    nop ; 1:01048
l_01049:
    mov ch, byte ptr [ebp] ; 1:01049
l_0104c:
    rol ecx, 010h ; 1:0104c
l_0104f:
    add esi, edx ; 1:0104f
l_01051:
    adc ebx, eax ; 1:01051
l_01053:
    mov dword ptr [edi + 07fffh], ecx ; 1:01053
l_01059:
    adc bh, dl ; 1:01059
l_0105b:
; Raster dword block 23: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0105b
l_0105d:
    mov bp, bx ; 1:0105d
l_01060:
    add ebp, dword ptr [_raster_texture] ; 1:01060
l_01066:
    nop ; 1:01066
l_01067:
    mov cl, byte ptr [ebp] ; 1:01067
l_0106a:
    add esi, edx ; 1:0106a
l_0106c:
    adc ebx, eax ; 1:0106c
l_0106e:
    adc bh, dl ; 1:0106e
l_01070:
    xor ebp, ebp ; 1:01070
l_01072:
    mov bp, bx ; 1:01072
l_01075:
    add ebp, dword ptr [_raster_texture] ; 1:01075
l_0107b:
    nop ; 1:0107b
l_0107c:
    mov ch, byte ptr [ebp] ; 1:0107c
l_0107f:
    shl ecx, 010h ; 1:0107f
l_01082:
    add esi, edx ; 1:01082
l_01084:
    adc ebx, eax ; 1:01084
l_01086:
    adc bh, dl ; 1:01086
l_01088:
    xor ebp, ebp ; 1:01088
l_0108a:
    mov bp, bx ; 1:0108a
l_0108d:
    add ebp, dword ptr [_raster_texture] ; 1:0108d
l_01093:
    nop ; 1:01093
l_01094:
    mov cl, byte ptr [ebp] ; 1:01094
l_01097:
    add esi, edx ; 1:01097
l_01099:
    adc ebx, eax ; 1:01099
l_0109b:
    adc bh, dl ; 1:0109b
l_0109d:
    xor ebp, ebp ; 1:0109d
l_0109f:
    mov bp, bx ; 1:0109f
l_010a2:
    add ebp, dword ptr [_raster_texture] ; 1:010a2
l_010a8:
    nop ; 1:010a8
l_010a9:
    mov ch, byte ptr [ebp] ; 1:010a9
l_010ac:
    rol ecx, 010h ; 1:010ac
l_010af:
    add esi, edx ; 1:010af
l_010b1:
    adc ebx, eax ; 1:010b1
l_010b3:
    mov dword ptr [edi + 07fffh], ecx ; 1:010b3
l_010b9:
    adc bh, dl ; 1:010b9
l_010bb:
; Raster dword block 24: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:010bb
l_010bd:
    mov bp, bx ; 1:010bd
l_010c0:
    add ebp, dword ptr [_raster_texture] ; 1:010c0
l_010c6:
    nop ; 1:010c6
l_010c7:
    mov cl, byte ptr [ebp] ; 1:010c7
l_010ca:
    add esi, edx ; 1:010ca
l_010cc:
    adc ebx, eax ; 1:010cc
l_010ce:
    adc bh, dl ; 1:010ce
l_010d0:
    xor ebp, ebp ; 1:010d0
l_010d2:
    mov bp, bx ; 1:010d2
l_010d5:
    add ebp, dword ptr [_raster_texture] ; 1:010d5
l_010db:
    nop ; 1:010db
l_010dc:
    mov ch, byte ptr [ebp] ; 1:010dc
l_010df:
    shl ecx, 010h ; 1:010df
l_010e2:
    add esi, edx ; 1:010e2
l_010e4:
    adc ebx, eax ; 1:010e4
l_010e6:
    adc bh, dl ; 1:010e6
l_010e8:
    xor ebp, ebp ; 1:010e8
l_010ea:
    mov bp, bx ; 1:010ea
l_010ed:
    add ebp, dword ptr [_raster_texture] ; 1:010ed
l_010f3:
    nop ; 1:010f3
l_010f4:
    mov cl, byte ptr [ebp] ; 1:010f4
l_010f7:
    add esi, edx ; 1:010f7
l_010f9:
    adc ebx, eax ; 1:010f9
l_010fb:
    adc bh, dl ; 1:010fb
l_010fd:
    xor ebp, ebp ; 1:010fd
l_010ff:
    mov bp, bx ; 1:010ff
l_01102:
    add ebp, dword ptr [_raster_texture] ; 1:01102
l_01108:
    nop ; 1:01108
l_01109:
    mov ch, byte ptr [ebp] ; 1:01109
l_0110c:
    rol ecx, 010h ; 1:0110c
l_0110f:
    add esi, edx ; 1:0110f
l_01111:
    adc ebx, eax ; 1:01111
l_01113:
    mov dword ptr [edi + 07fffh], ecx ; 1:01113
l_01119:
    adc bh, dl ; 1:01119
l_0111b:
; Raster dword block 25: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0111b
l_0111d:
    mov bp, bx ; 1:0111d
l_01120:
    add ebp, dword ptr [_raster_texture] ; 1:01120
l_01126:
    nop ; 1:01126
l_01127:
    mov cl, byte ptr [ebp] ; 1:01127
l_0112a:
    add esi, edx ; 1:0112a
l_0112c:
    adc ebx, eax ; 1:0112c
l_0112e:
    adc bh, dl ; 1:0112e
l_01130:
    xor ebp, ebp ; 1:01130
l_01132:
    mov bp, bx ; 1:01132
l_01135:
    add ebp, dword ptr [_raster_texture] ; 1:01135
l_0113b:
    nop ; 1:0113b
l_0113c:
    mov ch, byte ptr [ebp] ; 1:0113c
l_0113f:
    shl ecx, 010h ; 1:0113f
l_01142:
    add esi, edx ; 1:01142
l_01144:
    adc ebx, eax ; 1:01144
l_01146:
    adc bh, dl ; 1:01146
l_01148:
    xor ebp, ebp ; 1:01148
l_0114a:
    mov bp, bx ; 1:0114a
l_0114d:
    add ebp, dword ptr [_raster_texture] ; 1:0114d
l_01153:
    nop ; 1:01153
l_01154:
    mov cl, byte ptr [ebp] ; 1:01154
l_01157:
    add esi, edx ; 1:01157
l_01159:
    adc ebx, eax ; 1:01159
l_0115b:
    adc bh, dl ; 1:0115b
l_0115d:
    xor ebp, ebp ; 1:0115d
l_0115f:
    mov bp, bx ; 1:0115f
l_01162:
    add ebp, dword ptr [_raster_texture] ; 1:01162
l_01168:
    nop ; 1:01168
l_01169:
    mov ch, byte ptr [ebp] ; 1:01169
l_0116c:
    rol ecx, 010h ; 1:0116c
l_0116f:
    add esi, edx ; 1:0116f
l_01171:
    adc ebx, eax ; 1:01171
l_01173:
    mov dword ptr [edi + 07fffh], ecx ; 1:01173
l_01179:
    adc bh, dl ; 1:01179
l_0117b:
; Raster dword block 26: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0117b
l_0117d:
    mov bp, bx ; 1:0117d
l_01180:
    add ebp, dword ptr [_raster_texture] ; 1:01180
l_01186:
    nop ; 1:01186
l_01187:
    mov cl, byte ptr [ebp] ; 1:01187
l_0118a:
    add esi, edx ; 1:0118a
l_0118c:
    adc ebx, eax ; 1:0118c
l_0118e:
    adc bh, dl ; 1:0118e
l_01190:
    xor ebp, ebp ; 1:01190
l_01192:
    mov bp, bx ; 1:01192
l_01195:
    add ebp, dword ptr [_raster_texture] ; 1:01195
l_0119b:
    nop ; 1:0119b
l_0119c:
    mov ch, byte ptr [ebp] ; 1:0119c
l_0119f:
    shl ecx, 010h ; 1:0119f
l_011a2:
    add esi, edx ; 1:011a2
l_011a4:
    adc ebx, eax ; 1:011a4
l_011a6:
    adc bh, dl ; 1:011a6
l_011a8:
    xor ebp, ebp ; 1:011a8
l_011aa:
    mov bp, bx ; 1:011aa
l_011ad:
    add ebp, dword ptr [_raster_texture] ; 1:011ad
l_011b3:
    nop ; 1:011b3
l_011b4:
    mov cl, byte ptr [ebp] ; 1:011b4
l_011b7:
    add esi, edx ; 1:011b7
l_011b9:
    adc ebx, eax ; 1:011b9
l_011bb:
    adc bh, dl ; 1:011bb
l_011bd:
    xor ebp, ebp ; 1:011bd
l_011bf:
    mov bp, bx ; 1:011bf
l_011c2:
    add ebp, dword ptr [_raster_texture] ; 1:011c2
l_011c8:
    nop ; 1:011c8
l_011c9:
    mov ch, byte ptr [ebp] ; 1:011c9
l_011cc:
    rol ecx, 010h ; 1:011cc
l_011cf:
    add esi, edx ; 1:011cf
l_011d1:
    adc ebx, eax ; 1:011d1
l_011d3:
    mov dword ptr [edi + 07fffh], ecx ; 1:011d3
l_011d9:
    adc bh, dl ; 1:011d9
l_011db:
; Raster dword block 27: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:011db
l_011dd:
    mov bp, bx ; 1:011dd
l_011e0:
    add ebp, dword ptr [_raster_texture] ; 1:011e0
l_011e6:
    nop ; 1:011e6
l_011e7:
    mov cl, byte ptr [ebp] ; 1:011e7
l_011ea:
    add esi, edx ; 1:011ea
l_011ec:
    adc ebx, eax ; 1:011ec
l_011ee:
    adc bh, dl ; 1:011ee
l_011f0:
    xor ebp, ebp ; 1:011f0
l_011f2:
    mov bp, bx ; 1:011f2
l_011f5:
    add ebp, dword ptr [_raster_texture] ; 1:011f5
l_011fb:
    nop ; 1:011fb
l_011fc:
    mov ch, byte ptr [ebp] ; 1:011fc
l_011ff:
    shl ecx, 010h ; 1:011ff
l_01202:
    add esi, edx ; 1:01202
l_01204:
    adc ebx, eax ; 1:01204
l_01206:
    adc bh, dl ; 1:01206
l_01208:
    xor ebp, ebp ; 1:01208
l_0120a:
    mov bp, bx ; 1:0120a
l_0120d:
    add ebp, dword ptr [_raster_texture] ; 1:0120d
l_01213:
    nop ; 1:01213
l_01214:
    mov cl, byte ptr [ebp] ; 1:01214
l_01217:
    add esi, edx ; 1:01217
l_01219:
    adc ebx, eax ; 1:01219
l_0121b:
    adc bh, dl ; 1:0121b
l_0121d:
    xor ebp, ebp ; 1:0121d
l_0121f:
    mov bp, bx ; 1:0121f
l_01222:
    add ebp, dword ptr [_raster_texture] ; 1:01222
l_01228:
    nop ; 1:01228
l_01229:
    mov ch, byte ptr [ebp] ; 1:01229
l_0122c:
    rol ecx, 010h ; 1:0122c
l_0122f:
    add esi, edx ; 1:0122f
l_01231:
    adc ebx, eax ; 1:01231
l_01233:
    mov dword ptr [edi + 07fffh], ecx ; 1:01233
l_01239:
    adc bh, dl ; 1:01239
l_0123b:
; Raster dword block 28: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0123b
l_0123d:
    mov bp, bx ; 1:0123d
l_01240:
    add ebp, dword ptr [_raster_texture] ; 1:01240
l_01246:
    nop ; 1:01246
l_01247:
    mov cl, byte ptr [ebp] ; 1:01247
l_0124a:
    add esi, edx ; 1:0124a
l_0124c:
    adc ebx, eax ; 1:0124c
l_0124e:
    adc bh, dl ; 1:0124e
l_01250:
    xor ebp, ebp ; 1:01250
l_01252:
    mov bp, bx ; 1:01252
l_01255:
    add ebp, dword ptr [_raster_texture] ; 1:01255
l_0125b:
    nop ; 1:0125b
l_0125c:
    mov ch, byte ptr [ebp] ; 1:0125c
l_0125f:
    shl ecx, 010h ; 1:0125f
l_01262:
    add esi, edx ; 1:01262
l_01264:
    adc ebx, eax ; 1:01264
l_01266:
    adc bh, dl ; 1:01266
l_01268:
    xor ebp, ebp ; 1:01268
l_0126a:
    mov bp, bx ; 1:0126a
l_0126d:
    add ebp, dword ptr [_raster_texture] ; 1:0126d
l_01273:
    nop ; 1:01273
l_01274:
    mov cl, byte ptr [ebp] ; 1:01274
l_01277:
    add esi, edx ; 1:01277
l_01279:
    adc ebx, eax ; 1:01279
l_0127b:
    adc bh, dl ; 1:0127b
l_0127d:
    xor ebp, ebp ; 1:0127d
l_0127f:
    mov bp, bx ; 1:0127f
l_01282:
    add ebp, dword ptr [_raster_texture] ; 1:01282
l_01288:
    nop ; 1:01288
l_01289:
    mov ch, byte ptr [ebp] ; 1:01289
l_0128c:
    rol ecx, 010h ; 1:0128c
l_0128f:
    add esi, edx ; 1:0128f
l_01291:
    adc ebx, eax ; 1:01291
l_01293:
    mov dword ptr [edi + 07fffh], ecx ; 1:01293
l_01299:
    adc bh, dl ; 1:01299
l_0129b:
; Raster dword block 29: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0129b
l_0129d:
    mov bp, bx ; 1:0129d
l_012a0:
    add ebp, dword ptr [_raster_texture] ; 1:012a0
l_012a6:
    nop ; 1:012a6
l_012a7:
    mov cl, byte ptr [ebp] ; 1:012a7
l_012aa:
    add esi, edx ; 1:012aa
l_012ac:
    adc ebx, eax ; 1:012ac
l_012ae:
    adc bh, dl ; 1:012ae
l_012b0:
    xor ebp, ebp ; 1:012b0
l_012b2:
    mov bp, bx ; 1:012b2
l_012b5:
    add ebp, dword ptr [_raster_texture] ; 1:012b5
l_012bb:
    nop ; 1:012bb
l_012bc:
    mov ch, byte ptr [ebp] ; 1:012bc
l_012bf:
    shl ecx, 010h ; 1:012bf
l_012c2:
    add esi, edx ; 1:012c2
l_012c4:
    adc ebx, eax ; 1:012c4
l_012c6:
    adc bh, dl ; 1:012c6
l_012c8:
    xor ebp, ebp ; 1:012c8
l_012ca:
    mov bp, bx ; 1:012ca
l_012cd:
    add ebp, dword ptr [_raster_texture] ; 1:012cd
l_012d3:
    nop ; 1:012d3
l_012d4:
    mov cl, byte ptr [ebp] ; 1:012d4
l_012d7:
    add esi, edx ; 1:012d7
l_012d9:
    adc ebx, eax ; 1:012d9
l_012db:
    adc bh, dl ; 1:012db
l_012dd:
    xor ebp, ebp ; 1:012dd
l_012df:
    mov bp, bx ; 1:012df
l_012e2:
    add ebp, dword ptr [_raster_texture] ; 1:012e2
l_012e8:
    nop ; 1:012e8
l_012e9:
    mov ch, byte ptr [ebp] ; 1:012e9
l_012ec:
    rol ecx, 010h ; 1:012ec
l_012ef:
    add esi, edx ; 1:012ef
l_012f1:
    adc ebx, eax ; 1:012f1
l_012f3:
    mov dword ptr [edi + 07fffh], ecx ; 1:012f3
l_012f9:
    adc bh, dl ; 1:012f9
l_012fb:
; Raster dword block 30: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:012fb
l_012fd:
    mov bp, bx ; 1:012fd
l_01300:
    add ebp, dword ptr [_raster_texture] ; 1:01300
l_01306:
    nop ; 1:01306
l_01307:
    mov cl, byte ptr [ebp] ; 1:01307
l_0130a:
    add esi, edx ; 1:0130a
l_0130c:
    adc ebx, eax ; 1:0130c
l_0130e:
    adc bh, dl ; 1:0130e
l_01310:
    xor ebp, ebp ; 1:01310
l_01312:
    mov bp, bx ; 1:01312
l_01315:
    add ebp, dword ptr [_raster_texture] ; 1:01315
l_0131b:
    nop ; 1:0131b
l_0131c:
    mov ch, byte ptr [ebp] ; 1:0131c
l_0131f:
    shl ecx, 010h ; 1:0131f
l_01322:
    add esi, edx ; 1:01322
l_01324:
    adc ebx, eax ; 1:01324
l_01326:
    adc bh, dl ; 1:01326
l_01328:
    xor ebp, ebp ; 1:01328
l_0132a:
    mov bp, bx ; 1:0132a
l_0132d:
    add ebp, dword ptr [_raster_texture] ; 1:0132d
l_01333:
    nop ; 1:01333
l_01334:
    mov cl, byte ptr [ebp] ; 1:01334
l_01337:
    add esi, edx ; 1:01337
l_01339:
    adc ebx, eax ; 1:01339
l_0133b:
    adc bh, dl ; 1:0133b
l_0133d:
    xor ebp, ebp ; 1:0133d
l_0133f:
    mov bp, bx ; 1:0133f
l_01342:
    add ebp, dword ptr [_raster_texture] ; 1:01342
l_01348:
    nop ; 1:01348
l_01349:
    mov ch, byte ptr [ebp] ; 1:01349
l_0134c:
    rol ecx, 010h ; 1:0134c
l_0134f:
    add esi, edx ; 1:0134f
l_01351:
    adc ebx, eax ; 1:01351
l_01353:
    mov dword ptr [edi + 07fffh], ecx ; 1:01353
l_01359:
    adc bh, dl ; 1:01359
l_0135b:
; Raster dword block 31: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0135b
l_0135d:
    mov bp, bx ; 1:0135d
l_01360:
    add ebp, dword ptr [_raster_texture] ; 1:01360
l_01366:
    nop ; 1:01366
l_01367:
    mov cl, byte ptr [ebp] ; 1:01367
l_0136a:
    add esi, edx ; 1:0136a
l_0136c:
    adc ebx, eax ; 1:0136c
l_0136e:
    adc bh, dl ; 1:0136e
l_01370:
    xor ebp, ebp ; 1:01370
l_01372:
    mov bp, bx ; 1:01372
l_01375:
    add ebp, dword ptr [_raster_texture] ; 1:01375
l_0137b:
    nop ; 1:0137b
l_0137c:
    mov ch, byte ptr [ebp] ; 1:0137c
l_0137f:
    shl ecx, 010h ; 1:0137f
l_01382:
    add esi, edx ; 1:01382
l_01384:
    adc ebx, eax ; 1:01384
l_01386:
    adc bh, dl ; 1:01386
l_01388:
    xor ebp, ebp ; 1:01388
l_0138a:
    mov bp, bx ; 1:0138a
l_0138d:
    add ebp, dword ptr [_raster_texture] ; 1:0138d
l_01393:
    nop ; 1:01393
l_01394:
    mov cl, byte ptr [ebp] ; 1:01394
l_01397:
    add esi, edx ; 1:01397
l_01399:
    adc ebx, eax ; 1:01399
l_0139b:
    adc bh, dl ; 1:0139b
l_0139d:
    xor ebp, ebp ; 1:0139d
l_0139f:
    mov bp, bx ; 1:0139f
l_013a2:
    add ebp, dword ptr [_raster_texture] ; 1:013a2
l_013a8:
    nop ; 1:013a8
l_013a9:
    mov ch, byte ptr [ebp] ; 1:013a9
l_013ac:
    rol ecx, 010h ; 1:013ac
l_013af:
    add esi, edx ; 1:013af
l_013b1:
    adc ebx, eax ; 1:013b1
l_013b3:
    mov dword ptr [edi + 07fffh], ecx ; 1:013b3
l_013b9:
    adc bh, dl ; 1:013b9
l_013bb:
; Raster dword block 32: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:013bb
l_013bd:
    mov bp, bx ; 1:013bd
l_013c0:
    add ebp, dword ptr [_raster_texture] ; 1:013c0
l_013c6:
    nop ; 1:013c6
l_013c7:
    mov cl, byte ptr [ebp] ; 1:013c7
l_013ca:
    add esi, edx ; 1:013ca
l_013cc:
    adc ebx, eax ; 1:013cc
l_013ce:
    adc bh, dl ; 1:013ce
l_013d0:
    xor ebp, ebp ; 1:013d0
l_013d2:
    mov bp, bx ; 1:013d2
l_013d5:
    add ebp, dword ptr [_raster_texture] ; 1:013d5
l_013db:
    nop ; 1:013db
l_013dc:
    mov ch, byte ptr [ebp] ; 1:013dc
l_013df:
    shl ecx, 010h ; 1:013df
l_013e2:
    add esi, edx ; 1:013e2
l_013e4:
    adc ebx, eax ; 1:013e4
l_013e6:
    adc bh, dl ; 1:013e6
l_013e8:
    xor ebp, ebp ; 1:013e8
l_013ea:
    mov bp, bx ; 1:013ea
l_013ed:
    add ebp, dword ptr [_raster_texture] ; 1:013ed
l_013f3:
    nop ; 1:013f3
l_013f4:
    mov cl, byte ptr [ebp] ; 1:013f4
l_013f7:
    add esi, edx ; 1:013f7
l_013f9:
    adc ebx, eax ; 1:013f9
l_013fb:
    adc bh, dl ; 1:013fb
l_013fd:
    xor ebp, ebp ; 1:013fd
l_013ff:
    mov bp, bx ; 1:013ff
l_01402:
    add ebp, dword ptr [_raster_texture] ; 1:01402
l_01408:
    nop ; 1:01408
l_01409:
    mov ch, byte ptr [ebp] ; 1:01409
l_0140c:
    rol ecx, 010h ; 1:0140c
l_0140f:
    add esi, edx ; 1:0140f
l_01411:
    adc ebx, eax ; 1:01411
l_01413:
    mov dword ptr [edi + 07fffh], ecx ; 1:01413
l_01419:
    adc bh, dl ; 1:01419
l_0141b:
; Raster dword block 33: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0141b
l_0141d:
    mov bp, bx ; 1:0141d
l_01420:
    add ebp, dword ptr [_raster_texture] ; 1:01420
l_01426:
    nop ; 1:01426
l_01427:
    mov cl, byte ptr [ebp] ; 1:01427
l_0142a:
    add esi, edx ; 1:0142a
l_0142c:
    adc ebx, eax ; 1:0142c
l_0142e:
    adc bh, dl ; 1:0142e
l_01430:
    xor ebp, ebp ; 1:01430
l_01432:
    mov bp, bx ; 1:01432
l_01435:
    add ebp, dword ptr [_raster_texture] ; 1:01435
l_0143b:
    nop ; 1:0143b
l_0143c:
    mov ch, byte ptr [ebp] ; 1:0143c
l_0143f:
    shl ecx, 010h ; 1:0143f
l_01442:
    add esi, edx ; 1:01442
l_01444:
    adc ebx, eax ; 1:01444
l_01446:
    adc bh, dl ; 1:01446
l_01448:
    xor ebp, ebp ; 1:01448
l_0144a:
    mov bp, bx ; 1:0144a
l_0144d:
    add ebp, dword ptr [_raster_texture] ; 1:0144d
l_01453:
    nop ; 1:01453
l_01454:
    mov cl, byte ptr [ebp] ; 1:01454
l_01457:
    add esi, edx ; 1:01457
l_01459:
    adc ebx, eax ; 1:01459
l_0145b:
    adc bh, dl ; 1:0145b
l_0145d:
    xor ebp, ebp ; 1:0145d
l_0145f:
    mov bp, bx ; 1:0145f
l_01462:
    add ebp, dword ptr [_raster_texture] ; 1:01462
l_01468:
    nop ; 1:01468
l_01469:
    mov ch, byte ptr [ebp] ; 1:01469
l_0146c:
    rol ecx, 010h ; 1:0146c
l_0146f:
    add esi, edx ; 1:0146f
l_01471:
    adc ebx, eax ; 1:01471
l_01473:
    mov dword ptr [edi + 07fffh], ecx ; 1:01473
l_01479:
    adc bh, dl ; 1:01479
l_0147b:
; Raster dword block 34: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0147b
l_0147d:
    mov bp, bx ; 1:0147d
l_01480:
    add ebp, dword ptr [_raster_texture] ; 1:01480
l_01486:
    nop ; 1:01486
l_01487:
    mov cl, byte ptr [ebp] ; 1:01487
l_0148a:
    add esi, edx ; 1:0148a
l_0148c:
    adc ebx, eax ; 1:0148c
l_0148e:
    adc bh, dl ; 1:0148e
l_01490:
    xor ebp, ebp ; 1:01490
l_01492:
    mov bp, bx ; 1:01492
l_01495:
    add ebp, dword ptr [_raster_texture] ; 1:01495
l_0149b:
    nop ; 1:0149b
l_0149c:
    mov ch, byte ptr [ebp] ; 1:0149c
l_0149f:
    shl ecx, 010h ; 1:0149f
l_014a2:
    add esi, edx ; 1:014a2
l_014a4:
    adc ebx, eax ; 1:014a4
l_014a6:
    adc bh, dl ; 1:014a6
l_014a8:
    xor ebp, ebp ; 1:014a8
l_014aa:
    mov bp, bx ; 1:014aa
l_014ad:
    add ebp, dword ptr [_raster_texture] ; 1:014ad
l_014b3:
    nop ; 1:014b3
l_014b4:
    mov cl, byte ptr [ebp] ; 1:014b4
l_014b7:
    add esi, edx ; 1:014b7
l_014b9:
    adc ebx, eax ; 1:014b9
l_014bb:
    adc bh, dl ; 1:014bb
l_014bd:
    xor ebp, ebp ; 1:014bd
l_014bf:
    mov bp, bx ; 1:014bf
l_014c2:
    add ebp, dword ptr [_raster_texture] ; 1:014c2
l_014c8:
    nop ; 1:014c8
l_014c9:
    mov ch, byte ptr [ebp] ; 1:014c9
l_014cc:
    rol ecx, 010h ; 1:014cc
l_014cf:
    add esi, edx ; 1:014cf
l_014d1:
    adc ebx, eax ; 1:014d1
l_014d3:
    mov dword ptr [edi + 07fffh], ecx ; 1:014d3
l_014d9:
    adc bh, dl ; 1:014d9
l_014db:
; Raster dword block 35: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:014db
l_014dd:
    mov bp, bx ; 1:014dd
l_014e0:
    add ebp, dword ptr [_raster_texture] ; 1:014e0
l_014e6:
    nop ; 1:014e6
l_014e7:
    mov cl, byte ptr [ebp] ; 1:014e7
l_014ea:
    add esi, edx ; 1:014ea
l_014ec:
    adc ebx, eax ; 1:014ec
l_014ee:
    adc bh, dl ; 1:014ee
l_014f0:
    xor ebp, ebp ; 1:014f0
l_014f2:
    mov bp, bx ; 1:014f2
l_014f5:
    add ebp, dword ptr [_raster_texture] ; 1:014f5
l_014fb:
    nop ; 1:014fb
l_014fc:
    mov ch, byte ptr [ebp] ; 1:014fc
l_014ff:
    shl ecx, 010h ; 1:014ff
l_01502:
    add esi, edx ; 1:01502
l_01504:
    adc ebx, eax ; 1:01504
l_01506:
    adc bh, dl ; 1:01506
l_01508:
    xor ebp, ebp ; 1:01508
l_0150a:
    mov bp, bx ; 1:0150a
l_0150d:
    add ebp, dword ptr [_raster_texture] ; 1:0150d
l_01513:
    nop ; 1:01513
l_01514:
    mov cl, byte ptr [ebp] ; 1:01514
l_01517:
    add esi, edx ; 1:01517
l_01519:
    adc ebx, eax ; 1:01519
l_0151b:
    adc bh, dl ; 1:0151b
l_0151d:
    xor ebp, ebp ; 1:0151d
l_0151f:
    mov bp, bx ; 1:0151f
l_01522:
    add ebp, dword ptr [_raster_texture] ; 1:01522
l_01528:
    nop ; 1:01528
l_01529:
    mov ch, byte ptr [ebp] ; 1:01529
l_0152c:
    rol ecx, 010h ; 1:0152c
l_0152f:
    add esi, edx ; 1:0152f
l_01531:
    adc ebx, eax ; 1:01531
l_01533:
    mov dword ptr [edi + 07fffh], ecx ; 1:01533
l_01539:
    adc bh, dl ; 1:01539
l_0153b:
; Raster dword block 36: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0153b
l_0153d:
    mov bp, bx ; 1:0153d
l_01540:
    add ebp, dword ptr [_raster_texture] ; 1:01540
l_01546:
    nop ; 1:01546
l_01547:
    mov cl, byte ptr [ebp] ; 1:01547
l_0154a:
    add esi, edx ; 1:0154a
l_0154c:
    adc ebx, eax ; 1:0154c
l_0154e:
    adc bh, dl ; 1:0154e
l_01550:
    xor ebp, ebp ; 1:01550
l_01552:
    mov bp, bx ; 1:01552
l_01555:
    add ebp, dword ptr [_raster_texture] ; 1:01555
l_0155b:
    nop ; 1:0155b
l_0155c:
    mov ch, byte ptr [ebp] ; 1:0155c
l_0155f:
    shl ecx, 010h ; 1:0155f
l_01562:
    add esi, edx ; 1:01562
l_01564:
    adc ebx, eax ; 1:01564
l_01566:
    adc bh, dl ; 1:01566
l_01568:
    xor ebp, ebp ; 1:01568
l_0156a:
    mov bp, bx ; 1:0156a
l_0156d:
    add ebp, dword ptr [_raster_texture] ; 1:0156d
l_01573:
    nop ; 1:01573
l_01574:
    mov cl, byte ptr [ebp] ; 1:01574
l_01577:
    add esi, edx ; 1:01577
l_01579:
    adc ebx, eax ; 1:01579
l_0157b:
    adc bh, dl ; 1:0157b
l_0157d:
    xor ebp, ebp ; 1:0157d
l_0157f:
    mov bp, bx ; 1:0157f
l_01582:
    add ebp, dword ptr [_raster_texture] ; 1:01582
l_01588:
    nop ; 1:01588
l_01589:
    mov ch, byte ptr [ebp] ; 1:01589
l_0158c:
    rol ecx, 010h ; 1:0158c
l_0158f:
    add esi, edx ; 1:0158f
l_01591:
    adc ebx, eax ; 1:01591
l_01593:
    mov dword ptr [edi + 07fffh], ecx ; 1:01593
l_01599:
    adc bh, dl ; 1:01599
l_0159b:
; Raster dword block 37: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0159b
l_0159d:
    mov bp, bx ; 1:0159d
l_015a0:
    add ebp, dword ptr [_raster_texture] ; 1:015a0
l_015a6:
    nop ; 1:015a6
l_015a7:
    mov cl, byte ptr [ebp] ; 1:015a7
l_015aa:
    add esi, edx ; 1:015aa
l_015ac:
    adc ebx, eax ; 1:015ac
l_015ae:
    adc bh, dl ; 1:015ae
l_015b0:
    xor ebp, ebp ; 1:015b0
l_015b2:
    mov bp, bx ; 1:015b2
l_015b5:
    add ebp, dword ptr [_raster_texture] ; 1:015b5
l_015bb:
    nop ; 1:015bb
l_015bc:
    mov ch, byte ptr [ebp] ; 1:015bc
l_015bf:
    shl ecx, 010h ; 1:015bf
l_015c2:
    add esi, edx ; 1:015c2
l_015c4:
    adc ebx, eax ; 1:015c4
l_015c6:
    adc bh, dl ; 1:015c6
l_015c8:
    xor ebp, ebp ; 1:015c8
l_015ca:
    mov bp, bx ; 1:015ca
l_015cd:
    add ebp, dword ptr [_raster_texture] ; 1:015cd
l_015d3:
    nop ; 1:015d3
l_015d4:
    mov cl, byte ptr [ebp] ; 1:015d4
l_015d7:
    add esi, edx ; 1:015d7
l_015d9:
    adc ebx, eax ; 1:015d9
l_015db:
    adc bh, dl ; 1:015db
l_015dd:
    xor ebp, ebp ; 1:015dd
l_015df:
    mov bp, bx ; 1:015df
l_015e2:
    add ebp, dword ptr [_raster_texture] ; 1:015e2
l_015e8:
    nop ; 1:015e8
l_015e9:
    mov ch, byte ptr [ebp] ; 1:015e9
l_015ec:
    rol ecx, 010h ; 1:015ec
l_015ef:
    add esi, edx ; 1:015ef
l_015f1:
    adc ebx, eax ; 1:015f1
l_015f3:
    mov dword ptr [edi + 07fffh], ecx ; 1:015f3
l_015f9:
    adc bh, dl ; 1:015f9
l_015fb:
; Raster dword block 38: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:015fb
l_015fd:
    mov bp, bx ; 1:015fd
l_01600:
    add ebp, dword ptr [_raster_texture] ; 1:01600
l_01606:
    nop ; 1:01606
l_01607:
    mov cl, byte ptr [ebp] ; 1:01607
l_0160a:
    add esi, edx ; 1:0160a
l_0160c:
    adc ebx, eax ; 1:0160c
l_0160e:
    adc bh, dl ; 1:0160e
l_01610:
    xor ebp, ebp ; 1:01610
l_01612:
    mov bp, bx ; 1:01612
l_01615:
    add ebp, dword ptr [_raster_texture] ; 1:01615
l_0161b:
    nop ; 1:0161b
l_0161c:
    mov ch, byte ptr [ebp] ; 1:0161c
l_0161f:
    shl ecx, 010h ; 1:0161f
l_01622:
    add esi, edx ; 1:01622
l_01624:
    adc ebx, eax ; 1:01624
l_01626:
    adc bh, dl ; 1:01626
l_01628:
    xor ebp, ebp ; 1:01628
l_0162a:
    mov bp, bx ; 1:0162a
l_0162d:
    add ebp, dword ptr [_raster_texture] ; 1:0162d
l_01633:
    nop ; 1:01633
l_01634:
    mov cl, byte ptr [ebp] ; 1:01634
l_01637:
    add esi, edx ; 1:01637
l_01639:
    adc ebx, eax ; 1:01639
l_0163b:
    adc bh, dl ; 1:0163b
l_0163d:
    xor ebp, ebp ; 1:0163d
l_0163f:
    mov bp, bx ; 1:0163f
l_01642:
    add ebp, dword ptr [_raster_texture] ; 1:01642
l_01648:
    nop ; 1:01648
l_01649:
    mov ch, byte ptr [ebp] ; 1:01649
l_0164c:
    rol ecx, 010h ; 1:0164c
l_0164f:
    add esi, edx ; 1:0164f
l_01651:
    adc ebx, eax ; 1:01651
l_01653:
    mov dword ptr [edi + 07fffh], ecx ; 1:01653
l_01659:
    adc bh, dl ; 1:01659
l_0165b:
; Raster dword block 39: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0165b
l_0165d:
    mov bp, bx ; 1:0165d
l_01660:
    add ebp, dword ptr [_raster_texture] ; 1:01660
l_01666:
    nop ; 1:01666
l_01667:
    mov cl, byte ptr [ebp] ; 1:01667
l_0166a:
    add esi, edx ; 1:0166a
l_0166c:
    adc ebx, eax ; 1:0166c
l_0166e:
    adc bh, dl ; 1:0166e
l_01670:
    xor ebp, ebp ; 1:01670
l_01672:
    mov bp, bx ; 1:01672
l_01675:
    add ebp, dword ptr [_raster_texture] ; 1:01675
l_0167b:
    nop ; 1:0167b
l_0167c:
    mov ch, byte ptr [ebp] ; 1:0167c
l_0167f:
    shl ecx, 010h ; 1:0167f
l_01682:
    add esi, edx ; 1:01682
l_01684:
    adc ebx, eax ; 1:01684
l_01686:
    adc bh, dl ; 1:01686
l_01688:
    xor ebp, ebp ; 1:01688
l_0168a:
    mov bp, bx ; 1:0168a
l_0168d:
    add ebp, dword ptr [_raster_texture] ; 1:0168d
l_01693:
    nop ; 1:01693
l_01694:
    mov cl, byte ptr [ebp] ; 1:01694
l_01697:
    add esi, edx ; 1:01697
l_01699:
    adc ebx, eax ; 1:01699
l_0169b:
    adc bh, dl ; 1:0169b
l_0169d:
    xor ebp, ebp ; 1:0169d
l_0169f:
    mov bp, bx ; 1:0169f
l_016a2:
    add ebp, dword ptr [_raster_texture] ; 1:016a2
l_016a8:
    nop ; 1:016a8
l_016a9:
    mov ch, byte ptr [ebp] ; 1:016a9
l_016ac:
    rol ecx, 010h ; 1:016ac
l_016af:
    add esi, edx ; 1:016af
l_016b1:
    adc ebx, eax ; 1:016b1
l_016b3:
    mov dword ptr [edi + 07fffh], ecx ; 1:016b3
l_016b9:
    adc bh, dl ; 1:016b9
l_016bb:
; Raster dword block 40: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:016bb
l_016bd:
    mov bp, bx ; 1:016bd
l_016c0:
    add ebp, dword ptr [_raster_texture] ; 1:016c0
l_016c6:
    nop ; 1:016c6
l_016c7:
    mov cl, byte ptr [ebp] ; 1:016c7
l_016ca:
    add esi, edx ; 1:016ca
l_016cc:
    adc ebx, eax ; 1:016cc
l_016ce:
    adc bh, dl ; 1:016ce
l_016d0:
    xor ebp, ebp ; 1:016d0
l_016d2:
    mov bp, bx ; 1:016d2
l_016d5:
    add ebp, dword ptr [_raster_texture] ; 1:016d5
l_016db:
    nop ; 1:016db
l_016dc:
    mov ch, byte ptr [ebp] ; 1:016dc
l_016df:
    shl ecx, 010h ; 1:016df
l_016e2:
    add esi, edx ; 1:016e2
l_016e4:
    adc ebx, eax ; 1:016e4
l_016e6:
    adc bh, dl ; 1:016e6
l_016e8:
    xor ebp, ebp ; 1:016e8
l_016ea:
    mov bp, bx ; 1:016ea
l_016ed:
    add ebp, dword ptr [_raster_texture] ; 1:016ed
l_016f3:
    nop ; 1:016f3
l_016f4:
    mov cl, byte ptr [ebp] ; 1:016f4
l_016f7:
    add esi, edx ; 1:016f7
l_016f9:
    adc ebx, eax ; 1:016f9
l_016fb:
    adc bh, dl ; 1:016fb
l_016fd:
    xor ebp, ebp ; 1:016fd
l_016ff:
    mov bp, bx ; 1:016ff
l_01702:
    add ebp, dword ptr [_raster_texture] ; 1:01702
l_01708:
    nop ; 1:01708
l_01709:
    mov ch, byte ptr [ebp] ; 1:01709
l_0170c:
    rol ecx, 010h ; 1:0170c
l_0170f:
    add esi, edx ; 1:0170f
l_01711:
    adc ebx, eax ; 1:01711
l_01713:
    mov dword ptr [edi + 07fffh], ecx ; 1:01713
l_01719:
    adc bh, dl ; 1:01719
l_0171b:
; Raster dword block 41: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0171b
l_0171d:
    mov bp, bx ; 1:0171d
l_01720:
    add ebp, dword ptr [_raster_texture] ; 1:01720
l_01726:
    nop ; 1:01726
l_01727:
    mov cl, byte ptr [ebp] ; 1:01727
l_0172a:
    add esi, edx ; 1:0172a
l_0172c:
    adc ebx, eax ; 1:0172c
l_0172e:
    adc bh, dl ; 1:0172e
l_01730:
    xor ebp, ebp ; 1:01730
l_01732:
    mov bp, bx ; 1:01732
l_01735:
    add ebp, dword ptr [_raster_texture] ; 1:01735
l_0173b:
    nop ; 1:0173b
l_0173c:
    mov ch, byte ptr [ebp] ; 1:0173c
l_0173f:
    shl ecx, 010h ; 1:0173f
l_01742:
    add esi, edx ; 1:01742
l_01744:
    adc ebx, eax ; 1:01744
l_01746:
    adc bh, dl ; 1:01746
l_01748:
    xor ebp, ebp ; 1:01748
l_0174a:
    mov bp, bx ; 1:0174a
l_0174d:
    add ebp, dword ptr [_raster_texture] ; 1:0174d
l_01753:
    nop ; 1:01753
l_01754:
    mov cl, byte ptr [ebp] ; 1:01754
l_01757:
    add esi, edx ; 1:01757
l_01759:
    adc ebx, eax ; 1:01759
l_0175b:
    adc bh, dl ; 1:0175b
l_0175d:
    xor ebp, ebp ; 1:0175d
l_0175f:
    mov bp, bx ; 1:0175f
l_01762:
    add ebp, dword ptr [_raster_texture] ; 1:01762
l_01768:
    nop ; 1:01768
l_01769:
    mov ch, byte ptr [ebp] ; 1:01769
l_0176c:
    rol ecx, 010h ; 1:0176c
l_0176f:
    add esi, edx ; 1:0176f
l_01771:
    adc ebx, eax ; 1:01771
l_01773:
    mov dword ptr [edi + 07fffh], ecx ; 1:01773
l_01779:
    adc bh, dl ; 1:01779
l_0177b:
; Raster dword block 42: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0177b
l_0177d:
    mov bp, bx ; 1:0177d
l_01780:
    add ebp, dword ptr [_raster_texture] ; 1:01780
l_01786:
    nop ; 1:01786
l_01787:
    mov cl, byte ptr [ebp] ; 1:01787
l_0178a:
    add esi, edx ; 1:0178a
l_0178c:
    adc ebx, eax ; 1:0178c
l_0178e:
    adc bh, dl ; 1:0178e
l_01790:
    xor ebp, ebp ; 1:01790
l_01792:
    mov bp, bx ; 1:01792
l_01795:
    add ebp, dword ptr [_raster_texture] ; 1:01795
l_0179b:
    nop ; 1:0179b
l_0179c:
    mov ch, byte ptr [ebp] ; 1:0179c
l_0179f:
    shl ecx, 010h ; 1:0179f
l_017a2:
    add esi, edx ; 1:017a2
l_017a4:
    adc ebx, eax ; 1:017a4
l_017a6:
    adc bh, dl ; 1:017a6
l_017a8:
    xor ebp, ebp ; 1:017a8
l_017aa:
    mov bp, bx ; 1:017aa
l_017ad:
    add ebp, dword ptr [_raster_texture] ; 1:017ad
l_017b3:
    nop ; 1:017b3
l_017b4:
    mov cl, byte ptr [ebp] ; 1:017b4
l_017b7:
    add esi, edx ; 1:017b7
l_017b9:
    adc ebx, eax ; 1:017b9
l_017bb:
    adc bh, dl ; 1:017bb
l_017bd:
    xor ebp, ebp ; 1:017bd
l_017bf:
    mov bp, bx ; 1:017bf
l_017c2:
    add ebp, dword ptr [_raster_texture] ; 1:017c2
l_017c8:
    nop ; 1:017c8
l_017c9:
    mov ch, byte ptr [ebp] ; 1:017c9
l_017cc:
    rol ecx, 010h ; 1:017cc
l_017cf:
    add esi, edx ; 1:017cf
l_017d1:
    adc ebx, eax ; 1:017d1
l_017d3:
    mov dword ptr [edi + 07fffh], ecx ; 1:017d3
l_017d9:
    adc bh, dl ; 1:017d9
l_017db:
; Raster dword block 43: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:017db
l_017dd:
    mov bp, bx ; 1:017dd
l_017e0:
    add ebp, dword ptr [_raster_texture] ; 1:017e0
l_017e6:
    nop ; 1:017e6
l_017e7:
    mov cl, byte ptr [ebp] ; 1:017e7
l_017ea:
    add esi, edx ; 1:017ea
l_017ec:
    adc ebx, eax ; 1:017ec
l_017ee:
    adc bh, dl ; 1:017ee
l_017f0:
    xor ebp, ebp ; 1:017f0
l_017f2:
    mov bp, bx ; 1:017f2
l_017f5:
    add ebp, dword ptr [_raster_texture] ; 1:017f5
l_017fb:
    nop ; 1:017fb
l_017fc:
    mov ch, byte ptr [ebp] ; 1:017fc
l_017ff:
    shl ecx, 010h ; 1:017ff
l_01802:
    add esi, edx ; 1:01802
l_01804:
    adc ebx, eax ; 1:01804
l_01806:
    adc bh, dl ; 1:01806
l_01808:
    xor ebp, ebp ; 1:01808
l_0180a:
    mov bp, bx ; 1:0180a
l_0180d:
    add ebp, dword ptr [_raster_texture] ; 1:0180d
l_01813:
    nop ; 1:01813
l_01814:
    mov cl, byte ptr [ebp] ; 1:01814
l_01817:
    add esi, edx ; 1:01817
l_01819:
    adc ebx, eax ; 1:01819
l_0181b:
    adc bh, dl ; 1:0181b
l_0181d:
    xor ebp, ebp ; 1:0181d
l_0181f:
    mov bp, bx ; 1:0181f
l_01822:
    add ebp, dword ptr [_raster_texture] ; 1:01822
l_01828:
    nop ; 1:01828
l_01829:
    mov ch, byte ptr [ebp] ; 1:01829
l_0182c:
    rol ecx, 010h ; 1:0182c
l_0182f:
    add esi, edx ; 1:0182f
l_01831:
    adc ebx, eax ; 1:01831
l_01833:
    mov dword ptr [edi + 07fffh], ecx ; 1:01833
l_01839:
    adc bh, dl ; 1:01839
l_0183b:
; Raster dword block 44: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0183b
l_0183d:
    mov bp, bx ; 1:0183d
l_01840:
    add ebp, dword ptr [_raster_texture] ; 1:01840
l_01846:
    nop ; 1:01846
l_01847:
    mov cl, byte ptr [ebp] ; 1:01847
l_0184a:
    add esi, edx ; 1:0184a
l_0184c:
    adc ebx, eax ; 1:0184c
l_0184e:
    adc bh, dl ; 1:0184e
l_01850:
    xor ebp, ebp ; 1:01850
l_01852:
    mov bp, bx ; 1:01852
l_01855:
    add ebp, dword ptr [_raster_texture] ; 1:01855
l_0185b:
    nop ; 1:0185b
l_0185c:
    mov ch, byte ptr [ebp] ; 1:0185c
l_0185f:
    shl ecx, 010h ; 1:0185f
l_01862:
    add esi, edx ; 1:01862
l_01864:
    adc ebx, eax ; 1:01864
l_01866:
    adc bh, dl ; 1:01866
l_01868:
    xor ebp, ebp ; 1:01868
l_0186a:
    mov bp, bx ; 1:0186a
l_0186d:
    add ebp, dword ptr [_raster_texture] ; 1:0186d
l_01873:
    nop ; 1:01873
l_01874:
    mov cl, byte ptr [ebp] ; 1:01874
l_01877:
    add esi, edx ; 1:01877
l_01879:
    adc ebx, eax ; 1:01879
l_0187b:
    adc bh, dl ; 1:0187b
l_0187d:
    xor ebp, ebp ; 1:0187d
l_0187f:
    mov bp, bx ; 1:0187f
l_01882:
    add ebp, dword ptr [_raster_texture] ; 1:01882
l_01888:
    nop ; 1:01888
l_01889:
    mov ch, byte ptr [ebp] ; 1:01889
l_0188c:
    rol ecx, 010h ; 1:0188c
l_0188f:
    add esi, edx ; 1:0188f
l_01891:
    adc ebx, eax ; 1:01891
l_01893:
    mov dword ptr [edi + 07fffh], ecx ; 1:01893
l_01899:
    adc bh, dl ; 1:01899
l_0189b:
; Raster dword block 45: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0189b
l_0189d:
    mov bp, bx ; 1:0189d
l_018a0:
    add ebp, dword ptr [_raster_texture] ; 1:018a0
l_018a6:
    nop ; 1:018a6
l_018a7:
    mov cl, byte ptr [ebp] ; 1:018a7
l_018aa:
    add esi, edx ; 1:018aa
l_018ac:
    adc ebx, eax ; 1:018ac
l_018ae:
    adc bh, dl ; 1:018ae
l_018b0:
    xor ebp, ebp ; 1:018b0
l_018b2:
    mov bp, bx ; 1:018b2
l_018b5:
    add ebp, dword ptr [_raster_texture] ; 1:018b5
l_018bb:
    nop ; 1:018bb
l_018bc:
    mov ch, byte ptr [ebp] ; 1:018bc
l_018bf:
    shl ecx, 010h ; 1:018bf
l_018c2:
    add esi, edx ; 1:018c2
l_018c4:
    adc ebx, eax ; 1:018c4
l_018c6:
    adc bh, dl ; 1:018c6
l_018c8:
    xor ebp, ebp ; 1:018c8
l_018ca:
    mov bp, bx ; 1:018ca
l_018cd:
    add ebp, dword ptr [_raster_texture] ; 1:018cd
l_018d3:
    nop ; 1:018d3
l_018d4:
    mov cl, byte ptr [ebp] ; 1:018d4
l_018d7:
    add esi, edx ; 1:018d7
l_018d9:
    adc ebx, eax ; 1:018d9
l_018db:
    adc bh, dl ; 1:018db
l_018dd:
    xor ebp, ebp ; 1:018dd
l_018df:
    mov bp, bx ; 1:018df
l_018e2:
    add ebp, dword ptr [_raster_texture] ; 1:018e2
l_018e8:
    nop ; 1:018e8
l_018e9:
    mov ch, byte ptr [ebp] ; 1:018e9
l_018ec:
    rol ecx, 010h ; 1:018ec
l_018ef:
    add esi, edx ; 1:018ef
l_018f1:
    adc ebx, eax ; 1:018f1
l_018f3:
    mov dword ptr [edi + 07fffh], ecx ; 1:018f3
l_018f9:
    adc bh, dl ; 1:018f9
l_018fb:
; Raster dword block 46: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:018fb
l_018fd:
    mov bp, bx ; 1:018fd
l_01900:
    add ebp, dword ptr [_raster_texture] ; 1:01900
l_01906:
    nop ; 1:01906
l_01907:
    mov cl, byte ptr [ebp] ; 1:01907
l_0190a:
    add esi, edx ; 1:0190a
l_0190c:
    adc ebx, eax ; 1:0190c
l_0190e:
    adc bh, dl ; 1:0190e
l_01910:
    xor ebp, ebp ; 1:01910
l_01912:
    mov bp, bx ; 1:01912
l_01915:
    add ebp, dword ptr [_raster_texture] ; 1:01915
l_0191b:
    nop ; 1:0191b
l_0191c:
    mov ch, byte ptr [ebp] ; 1:0191c
l_0191f:
    shl ecx, 010h ; 1:0191f
l_01922:
    add esi, edx ; 1:01922
l_01924:
    adc ebx, eax ; 1:01924
l_01926:
    adc bh, dl ; 1:01926
l_01928:
    xor ebp, ebp ; 1:01928
l_0192a:
    mov bp, bx ; 1:0192a
l_0192d:
    add ebp, dword ptr [_raster_texture] ; 1:0192d
l_01933:
    nop ; 1:01933
l_01934:
    mov cl, byte ptr [ebp] ; 1:01934
l_01937:
    add esi, edx ; 1:01937
l_01939:
    adc ebx, eax ; 1:01939
l_0193b:
    adc bh, dl ; 1:0193b
l_0193d:
    xor ebp, ebp ; 1:0193d
l_0193f:
    mov bp, bx ; 1:0193f
l_01942:
    add ebp, dword ptr [_raster_texture] ; 1:01942
l_01948:
    nop ; 1:01948
l_01949:
    mov ch, byte ptr [ebp] ; 1:01949
l_0194c:
    rol ecx, 010h ; 1:0194c
l_0194f:
    add esi, edx ; 1:0194f
l_01951:
    adc ebx, eax ; 1:01951
l_01953:
    mov dword ptr [edi + 07fffh], ecx ; 1:01953
l_01959:
    adc bh, dl ; 1:01959
l_0195b:
; Raster dword block 47: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0195b
l_0195d:
    mov bp, bx ; 1:0195d
l_01960:
    add ebp, dword ptr [_raster_texture] ; 1:01960
l_01966:
    nop ; 1:01966
l_01967:
    mov cl, byte ptr [ebp] ; 1:01967
l_0196a:
    add esi, edx ; 1:0196a
l_0196c:
    adc ebx, eax ; 1:0196c
l_0196e:
    adc bh, dl ; 1:0196e
l_01970:
    xor ebp, ebp ; 1:01970
l_01972:
    mov bp, bx ; 1:01972
l_01975:
    add ebp, dword ptr [_raster_texture] ; 1:01975
l_0197b:
    nop ; 1:0197b
l_0197c:
    mov ch, byte ptr [ebp] ; 1:0197c
l_0197f:
    shl ecx, 010h ; 1:0197f
l_01982:
    add esi, edx ; 1:01982
l_01984:
    adc ebx, eax ; 1:01984
l_01986:
    adc bh, dl ; 1:01986
l_01988:
    xor ebp, ebp ; 1:01988
l_0198a:
    mov bp, bx ; 1:0198a
l_0198d:
    add ebp, dword ptr [_raster_texture] ; 1:0198d
l_01993:
    nop ; 1:01993
l_01994:
    mov cl, byte ptr [ebp] ; 1:01994
l_01997:
    add esi, edx ; 1:01997
l_01999:
    adc ebx, eax ; 1:01999
l_0199b:
    adc bh, dl ; 1:0199b
l_0199d:
    xor ebp, ebp ; 1:0199d
l_0199f:
    mov bp, bx ; 1:0199f
l_019a2:
    add ebp, dword ptr [_raster_texture] ; 1:019a2
l_019a8:
    nop ; 1:019a8
l_019a9:
    mov ch, byte ptr [ebp] ; 1:019a9
l_019ac:
    rol ecx, 010h ; 1:019ac
l_019af:
    add esi, edx ; 1:019af
l_019b1:
    adc ebx, eax ; 1:019b1
l_019b3:
    mov dword ptr [edi + 07fffh], ecx ; 1:019b3
l_019b9:
    adc bh, dl ; 1:019b9
l_019bb:
; Raster dword block 48: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:019bb
l_019bd:
    mov bp, bx ; 1:019bd
l_019c0:
    add ebp, dword ptr [_raster_texture] ; 1:019c0
l_019c6:
    nop ; 1:019c6
l_019c7:
    mov cl, byte ptr [ebp] ; 1:019c7
l_019ca:
    add esi, edx ; 1:019ca
l_019cc:
    adc ebx, eax ; 1:019cc
l_019ce:
    adc bh, dl ; 1:019ce
l_019d0:
    xor ebp, ebp ; 1:019d0
l_019d2:
    mov bp, bx ; 1:019d2
l_019d5:
    add ebp, dword ptr [_raster_texture] ; 1:019d5
l_019db:
    nop ; 1:019db
l_019dc:
    mov ch, byte ptr [ebp] ; 1:019dc
l_019df:
    shl ecx, 010h ; 1:019df
l_019e2:
    add esi, edx ; 1:019e2
l_019e4:
    adc ebx, eax ; 1:019e4
l_019e6:
    adc bh, dl ; 1:019e6
l_019e8:
    xor ebp, ebp ; 1:019e8
l_019ea:
    mov bp, bx ; 1:019ea
l_019ed:
    add ebp, dword ptr [_raster_texture] ; 1:019ed
l_019f3:
    nop ; 1:019f3
l_019f4:
    mov cl, byte ptr [ebp] ; 1:019f4
l_019f7:
    add esi, edx ; 1:019f7
l_019f9:
    adc ebx, eax ; 1:019f9
l_019fb:
    adc bh, dl ; 1:019fb
l_019fd:
    xor ebp, ebp ; 1:019fd
l_019ff:
    mov bp, bx ; 1:019ff
l_01a02:
    add ebp, dword ptr [_raster_texture] ; 1:01a02
l_01a08:
    nop ; 1:01a08
l_01a09:
    mov ch, byte ptr [ebp] ; 1:01a09
l_01a0c:
    rol ecx, 010h ; 1:01a0c
l_01a0f:
    add esi, edx ; 1:01a0f
l_01a11:
    adc ebx, eax ; 1:01a11
l_01a13:
    mov dword ptr [edi + 07fffh], ecx ; 1:01a13
l_01a19:
    adc bh, dl ; 1:01a19
l_01a1b:
; Raster dword block 49: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:01a1b
l_01a1d:
    mov bp, bx ; 1:01a1d
l_01a20:
    add ebp, dword ptr [_raster_texture] ; 1:01a20
l_01a26:
    nop ; 1:01a26
l_01a27:
    mov cl, byte ptr [ebp] ; 1:01a27
l_01a2a:
    add esi, edx ; 1:01a2a
l_01a2c:
    adc ebx, eax ; 1:01a2c
l_01a2e:
    adc bh, dl ; 1:01a2e
l_01a30:
    xor ebp, ebp ; 1:01a30
l_01a32:
    mov bp, bx ; 1:01a32
l_01a35:
    add ebp, dword ptr [_raster_texture] ; 1:01a35
l_01a3b:
    nop ; 1:01a3b
l_01a3c:
    mov ch, byte ptr [ebp] ; 1:01a3c
l_01a3f:
    shl ecx, 010h ; 1:01a3f
l_01a42:
    add esi, edx ; 1:01a42
l_01a44:
    adc ebx, eax ; 1:01a44
l_01a46:
    adc bh, dl ; 1:01a46
l_01a48:
    xor ebp, ebp ; 1:01a48
l_01a4a:
    mov bp, bx ; 1:01a4a
l_01a4d:
    add ebp, dword ptr [_raster_texture] ; 1:01a4d
l_01a53:
    nop ; 1:01a53
l_01a54:
    mov cl, byte ptr [ebp] ; 1:01a54
l_01a57:
    add esi, edx ; 1:01a57
l_01a59:
    adc ebx, eax ; 1:01a59
l_01a5b:
    adc bh, dl ; 1:01a5b
l_01a5d:
    xor ebp, ebp ; 1:01a5d
l_01a5f:
    mov bp, bx ; 1:01a5f
l_01a62:
    add ebp, dword ptr [_raster_texture] ; 1:01a62
l_01a68:
    nop ; 1:01a68
l_01a69:
    mov ch, byte ptr [ebp] ; 1:01a69
l_01a6c:
    rol ecx, 010h ; 1:01a6c
l_01a6f:
    add esi, edx ; 1:01a6f
l_01a71:
    adc ebx, eax ; 1:01a71
l_01a73:
    mov dword ptr [edi + 07fffh], ecx ; 1:01a73
l_01a79:
    adc bh, dl ; 1:01a79
l_01a7b:
; Raster dword block 50: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:01a7b
l_01a7d:
    mov bp, bx ; 1:01a7d
l_01a80:
    add ebp, dword ptr [_raster_texture] ; 1:01a80
l_01a86:
    nop ; 1:01a86
l_01a87:
    mov cl, byte ptr [ebp] ; 1:01a87
l_01a8a:
    add esi, edx ; 1:01a8a
l_01a8c:
    adc ebx, eax ; 1:01a8c
l_01a8e:
    adc bh, dl ; 1:01a8e
l_01a90:
    xor ebp, ebp ; 1:01a90
l_01a92:
    mov bp, bx ; 1:01a92
l_01a95:
    add ebp, dword ptr [_raster_texture] ; 1:01a95
l_01a9b:
    nop ; 1:01a9b
l_01a9c:
    mov ch, byte ptr [ebp] ; 1:01a9c
l_01a9f:
    shl ecx, 010h ; 1:01a9f
l_01aa2:
    add esi, edx ; 1:01aa2
l_01aa4:
    adc ebx, eax ; 1:01aa4
l_01aa6:
    adc bh, dl ; 1:01aa6
l_01aa8:
    xor ebp, ebp ; 1:01aa8
l_01aaa:
    mov bp, bx ; 1:01aaa
l_01aad:
    add ebp, dword ptr [_raster_texture] ; 1:01aad
l_01ab3:
    nop ; 1:01ab3
l_01ab4:
    mov cl, byte ptr [ebp] ; 1:01ab4
l_01ab7:
    add esi, edx ; 1:01ab7
l_01ab9:
    adc ebx, eax ; 1:01ab9
l_01abb:
    adc bh, dl ; 1:01abb
l_01abd:
    xor ebp, ebp ; 1:01abd
l_01abf:
    mov bp, bx ; 1:01abf
l_01ac2:
    add ebp, dword ptr [_raster_texture] ; 1:01ac2
l_01ac8:
    nop ; 1:01ac8
l_01ac9:
    mov ch, byte ptr [ebp] ; 1:01ac9
l_01acc:
    rol ecx, 010h ; 1:01acc
l_01acf:
    add esi, edx ; 1:01acf
l_01ad1:
    adc ebx, eax ; 1:01ad1
l_01ad3:
    mov dword ptr [edi + 07fffh], ecx ; 1:01ad3
l_01ad9:
    adc bh, dl ; 1:01ad9
l_01adb:
; Raster dword block 51: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:01adb
l_01add:
    mov bp, bx ; 1:01add
l_01ae0:
    add ebp, dword ptr [_raster_texture] ; 1:01ae0
l_01ae6:
    nop ; 1:01ae6
l_01ae7:
    mov cl, byte ptr [ebp] ; 1:01ae7
l_01aea:
    add esi, edx ; 1:01aea
l_01aec:
    adc ebx, eax ; 1:01aec
l_01aee:
    adc bh, dl ; 1:01aee
l_01af0:
    xor ebp, ebp ; 1:01af0
l_01af2:
    mov bp, bx ; 1:01af2
l_01af5:
    add ebp, dword ptr [_raster_texture] ; 1:01af5
l_01afb:
    nop ; 1:01afb
l_01afc:
    mov ch, byte ptr [ebp] ; 1:01afc
l_01aff:
    shl ecx, 010h ; 1:01aff
l_01b02:
    add esi, edx ; 1:01b02
l_01b04:
    adc ebx, eax ; 1:01b04
l_01b06:
    adc bh, dl ; 1:01b06
l_01b08:
    xor ebp, ebp ; 1:01b08
l_01b0a:
    mov bp, bx ; 1:01b0a
l_01b0d:
    add ebp, dword ptr [_raster_texture] ; 1:01b0d
l_01b13:
    nop ; 1:01b13
l_01b14:
    mov cl, byte ptr [ebp] ; 1:01b14
l_01b17:
    add esi, edx ; 1:01b17
l_01b19:
    adc ebx, eax ; 1:01b19
l_01b1b:
    adc bh, dl ; 1:01b1b
l_01b1d:
    xor ebp, ebp ; 1:01b1d
l_01b1f:
    mov bp, bx ; 1:01b1f
l_01b22:
    add ebp, dword ptr [_raster_texture] ; 1:01b22
l_01b28:
    nop ; 1:01b28
l_01b29:
    mov ch, byte ptr [ebp] ; 1:01b29
l_01b2c:
    rol ecx, 010h ; 1:01b2c
l_01b2f:
    add esi, edx ; 1:01b2f
l_01b31:
    adc ebx, eax ; 1:01b31
l_01b33:
    mov dword ptr [edi + 07fffh], ecx ; 1:01b33
l_01b39:
    adc bh, dl ; 1:01b39
l_01b3b:
; Raster dword block 52: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:01b3b
l_01b3d:
    mov bp, bx ; 1:01b3d
l_01b40:
    add ebp, dword ptr [_raster_texture] ; 1:01b40
l_01b46:
    nop ; 1:01b46
l_01b47:
    mov cl, byte ptr [ebp] ; 1:01b47
l_01b4a:
    add esi, edx ; 1:01b4a
l_01b4c:
    adc ebx, eax ; 1:01b4c
l_01b4e:
    adc bh, dl ; 1:01b4e
l_01b50:
    xor ebp, ebp ; 1:01b50
l_01b52:
    mov bp, bx ; 1:01b52
l_01b55:
    add ebp, dword ptr [_raster_texture] ; 1:01b55
l_01b5b:
    nop ; 1:01b5b
l_01b5c:
    mov ch, byte ptr [ebp] ; 1:01b5c
l_01b5f:
    shl ecx, 010h ; 1:01b5f
l_01b62:
    add esi, edx ; 1:01b62
l_01b64:
    adc ebx, eax ; 1:01b64
l_01b66:
    adc bh, dl ; 1:01b66
l_01b68:
    xor ebp, ebp ; 1:01b68
l_01b6a:
    mov bp, bx ; 1:01b6a
l_01b6d:
    add ebp, dword ptr [_raster_texture] ; 1:01b6d
l_01b73:
    nop ; 1:01b73
l_01b74:
    mov cl, byte ptr [ebp] ; 1:01b74
l_01b77:
    add esi, edx ; 1:01b77
l_01b79:
    adc ebx, eax ; 1:01b79
l_01b7b:
    adc bh, dl ; 1:01b7b
l_01b7d:
    xor ebp, ebp ; 1:01b7d
l_01b7f:
    mov bp, bx ; 1:01b7f
l_01b82:
    add ebp, dword ptr [_raster_texture] ; 1:01b82
l_01b88:
    nop ; 1:01b88
l_01b89:
    mov ch, byte ptr [ebp] ; 1:01b89
l_01b8c:
    rol ecx, 010h ; 1:01b8c
l_01b8f:
    add esi, edx ; 1:01b8f
l_01b91:
    adc ebx, eax ; 1:01b91
l_01b93:
    mov dword ptr [edi + 07fffh], ecx ; 1:01b93
l_01b99:
    adc bh, dl ; 1:01b99
l_01b9b:
; Raster dword block 53: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:01b9b
l_01b9d:
    mov bp, bx ; 1:01b9d
l_01ba0:
    add ebp, dword ptr [_raster_texture] ; 1:01ba0
l_01ba6:
    nop ; 1:01ba6
l_01ba7:
    mov cl, byte ptr [ebp] ; 1:01ba7
l_01baa:
    add esi, edx ; 1:01baa
l_01bac:
    adc ebx, eax ; 1:01bac
l_01bae:
    adc bh, dl ; 1:01bae
l_01bb0:
    xor ebp, ebp ; 1:01bb0
l_01bb2:
    mov bp, bx ; 1:01bb2
l_01bb5:
    add ebp, dword ptr [_raster_texture] ; 1:01bb5
l_01bbb:
    nop ; 1:01bbb
l_01bbc:
    mov ch, byte ptr [ebp] ; 1:01bbc
l_01bbf:
    shl ecx, 010h ; 1:01bbf
l_01bc2:
    add esi, edx ; 1:01bc2
l_01bc4:
    adc ebx, eax ; 1:01bc4
l_01bc6:
    adc bh, dl ; 1:01bc6
l_01bc8:
    xor ebp, ebp ; 1:01bc8
l_01bca:
    mov bp, bx ; 1:01bca
l_01bcd:
    add ebp, dword ptr [_raster_texture] ; 1:01bcd
l_01bd3:
    nop ; 1:01bd3
l_01bd4:
    mov cl, byte ptr [ebp] ; 1:01bd4
l_01bd7:
    add esi, edx ; 1:01bd7
l_01bd9:
    adc ebx, eax ; 1:01bd9
l_01bdb:
    adc bh, dl ; 1:01bdb
l_01bdd:
    xor ebp, ebp ; 1:01bdd
l_01bdf:
    mov bp, bx ; 1:01bdf
l_01be2:
    add ebp, dword ptr [_raster_texture] ; 1:01be2
l_01be8:
    nop ; 1:01be8
l_01be9:
    mov ch, byte ptr [ebp] ; 1:01be9
l_01bec:
    rol ecx, 010h ; 1:01bec
l_01bef:
    add esi, edx ; 1:01bef
l_01bf1:
    adc ebx, eax ; 1:01bf1
l_01bf3:
    mov dword ptr [edi + 07fffh], ecx ; 1:01bf3
l_01bf9:
    adc bh, dl ; 1:01bf9
l_01bfb:
; Raster dword block 54: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:01bfb
l_01bfd:
    mov bp, bx ; 1:01bfd
l_01c00:
    add ebp, dword ptr [_raster_texture] ; 1:01c00
l_01c06:
    nop ; 1:01c06
l_01c07:
    mov cl, byte ptr [ebp] ; 1:01c07
l_01c0a:
    add esi, edx ; 1:01c0a
l_01c0c:
    adc ebx, eax ; 1:01c0c
l_01c0e:
    adc bh, dl ; 1:01c0e
l_01c10:
    xor ebp, ebp ; 1:01c10
l_01c12:
    mov bp, bx ; 1:01c12
l_01c15:
    add ebp, dword ptr [_raster_texture] ; 1:01c15
l_01c1b:
    nop ; 1:01c1b
l_01c1c:
    mov ch, byte ptr [ebp] ; 1:01c1c
l_01c1f:
    shl ecx, 010h ; 1:01c1f
l_01c22:
    add esi, edx ; 1:01c22
l_01c24:
    adc ebx, eax ; 1:01c24
l_01c26:
    adc bh, dl ; 1:01c26
l_01c28:
    xor ebp, ebp ; 1:01c28
l_01c2a:
    mov bp, bx ; 1:01c2a
l_01c2d:
    add ebp, dword ptr [_raster_texture] ; 1:01c2d
l_01c33:
    nop ; 1:01c33
l_01c34:
    mov cl, byte ptr [ebp] ; 1:01c34
l_01c37:
    add esi, edx ; 1:01c37
l_01c39:
    adc ebx, eax ; 1:01c39
l_01c3b:
    adc bh, dl ; 1:01c3b
l_01c3d:
    xor ebp, ebp ; 1:01c3d
l_01c3f:
    mov bp, bx ; 1:01c3f
l_01c42:
    add ebp, dword ptr [_raster_texture] ; 1:01c42
l_01c48:
    nop ; 1:01c48
l_01c49:
    mov ch, byte ptr [ebp] ; 1:01c49
l_01c4c:
    rol ecx, 010h ; 1:01c4c
l_01c4f:
    add esi, edx ; 1:01c4f
l_01c51:
    adc ebx, eax ; 1:01c51
l_01c53:
    mov dword ptr [edi + 07fffh], ecx ; 1:01c53
l_01c59:
    adc bh, dl ; 1:01c59
l_01c5b:
; Raster dword block 55: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:01c5b
l_01c5d:
    mov bp, bx ; 1:01c5d
l_01c60:
    add ebp, dword ptr [_raster_texture] ; 1:01c60
l_01c66:
    nop ; 1:01c66
l_01c67:
    mov cl, byte ptr [ebp] ; 1:01c67
l_01c6a:
    add esi, edx ; 1:01c6a
l_01c6c:
    adc ebx, eax ; 1:01c6c
l_01c6e:
    adc bh, dl ; 1:01c6e
l_01c70:
    xor ebp, ebp ; 1:01c70
l_01c72:
    mov bp, bx ; 1:01c72
l_01c75:
    add ebp, dword ptr [_raster_texture] ; 1:01c75
l_01c7b:
    nop ; 1:01c7b
l_01c7c:
    mov ch, byte ptr [ebp] ; 1:01c7c
l_01c7f:
    shl ecx, 010h ; 1:01c7f
l_01c82:
    add esi, edx ; 1:01c82
l_01c84:
    adc ebx, eax ; 1:01c84
l_01c86:
    adc bh, dl ; 1:01c86
l_01c88:
    xor ebp, ebp ; 1:01c88
l_01c8a:
    mov bp, bx ; 1:01c8a
l_01c8d:
    add ebp, dword ptr [_raster_texture] ; 1:01c8d
l_01c93:
    nop ; 1:01c93
l_01c94:
    mov cl, byte ptr [ebp] ; 1:01c94
l_01c97:
    add esi, edx ; 1:01c97
l_01c99:
    adc ebx, eax ; 1:01c99
l_01c9b:
    adc bh, dl ; 1:01c9b
l_01c9d:
    xor ebp, ebp ; 1:01c9d
l_01c9f:
    mov bp, bx ; 1:01c9f
l_01ca2:
    add ebp, dword ptr [_raster_texture] ; 1:01ca2
l_01ca8:
    nop ; 1:01ca8
l_01ca9:
    mov ch, byte ptr [ebp] ; 1:01ca9
l_01cac:
    rol ecx, 010h ; 1:01cac
l_01caf:
    add esi, edx ; 1:01caf
l_01cb1:
    adc ebx, eax ; 1:01cb1
l_01cb3:
    mov dword ptr [edi + 07fffh], ecx ; 1:01cb3
l_01cb9:
    adc bh, dl ; 1:01cb9
l_01cbb:
; Raster dword block 56: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:01cbb
l_01cbd:
    mov bp, bx ; 1:01cbd
l_01cc0:
    add ebp, dword ptr [_raster_texture] ; 1:01cc0
l_01cc6:
    nop ; 1:01cc6
l_01cc7:
    mov cl, byte ptr [ebp] ; 1:01cc7
l_01cca:
    add esi, edx ; 1:01cca
l_01ccc:
    adc ebx, eax ; 1:01ccc
l_01cce:
    adc bh, dl ; 1:01cce
l_01cd0:
    xor ebp, ebp ; 1:01cd0
l_01cd2:
    mov bp, bx ; 1:01cd2
l_01cd5:
    add ebp, dword ptr [_raster_texture] ; 1:01cd5
l_01cdb:
    nop ; 1:01cdb
l_01cdc:
    mov ch, byte ptr [ebp] ; 1:01cdc
l_01cdf:
    shl ecx, 010h ; 1:01cdf
l_01ce2:
    add esi, edx ; 1:01ce2
l_01ce4:
    adc ebx, eax ; 1:01ce4
l_01ce6:
    adc bh, dl ; 1:01ce6
l_01ce8:
    xor ebp, ebp ; 1:01ce8
l_01cea:
    mov bp, bx ; 1:01cea
l_01ced:
    add ebp, dword ptr [_raster_texture] ; 1:01ced
l_01cf3:
    nop ; 1:01cf3
l_01cf4:
    mov cl, byte ptr [ebp] ; 1:01cf4
l_01cf7:
    add esi, edx ; 1:01cf7
l_01cf9:
    adc ebx, eax ; 1:01cf9
l_01cfb:
    adc bh, dl ; 1:01cfb
l_01cfd:
    xor ebp, ebp ; 1:01cfd
l_01cff:
    mov bp, bx ; 1:01cff
l_01d02:
    add ebp, dword ptr [_raster_texture] ; 1:01d02
l_01d08:
    nop ; 1:01d08
l_01d09:
    mov ch, byte ptr [ebp] ; 1:01d09
l_01d0c:
    rol ecx, 010h ; 1:01d0c
l_01d0f:
    add esi, edx ; 1:01d0f
l_01d11:
    adc ebx, eax ; 1:01d11
l_01d13:
    mov dword ptr [edi + 07fffh], ecx ; 1:01d13
l_01d19:
    adc bh, dl ; 1:01d19
l_01d1b:
; Raster dword block 57: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:01d1b
l_01d1d:
    mov bp, bx ; 1:01d1d
l_01d20:
    add ebp, dword ptr [_raster_texture] ; 1:01d20
l_01d26:
    nop ; 1:01d26
l_01d27:
    mov cl, byte ptr [ebp] ; 1:01d27
l_01d2a:
    add esi, edx ; 1:01d2a
l_01d2c:
    adc ebx, eax ; 1:01d2c
l_01d2e:
    adc bh, dl ; 1:01d2e
l_01d30:
    xor ebp, ebp ; 1:01d30
l_01d32:
    mov bp, bx ; 1:01d32
l_01d35:
    add ebp, dword ptr [_raster_texture] ; 1:01d35
l_01d3b:
    nop ; 1:01d3b
l_01d3c:
    mov ch, byte ptr [ebp] ; 1:01d3c
l_01d3f:
    shl ecx, 010h ; 1:01d3f
l_01d42:
    add esi, edx ; 1:01d42
l_01d44:
    adc ebx, eax ; 1:01d44
l_01d46:
    adc bh, dl ; 1:01d46
l_01d48:
    xor ebp, ebp ; 1:01d48
l_01d4a:
    mov bp, bx ; 1:01d4a
l_01d4d:
    add ebp, dword ptr [_raster_texture] ; 1:01d4d
l_01d53:
    nop ; 1:01d53
l_01d54:
    mov cl, byte ptr [ebp] ; 1:01d54
l_01d57:
    add esi, edx ; 1:01d57
l_01d59:
    adc ebx, eax ; 1:01d59
l_01d5b:
    adc bh, dl ; 1:01d5b
l_01d5d:
    xor ebp, ebp ; 1:01d5d
l_01d5f:
    mov bp, bx ; 1:01d5f
l_01d62:
    add ebp, dword ptr [_raster_texture] ; 1:01d62
l_01d68:
    nop ; 1:01d68
l_01d69:
    mov ch, byte ptr [ebp] ; 1:01d69
l_01d6c:
    rol ecx, 010h ; 1:01d6c
l_01d6f:
    add esi, edx ; 1:01d6f
l_01d71:
    adc ebx, eax ; 1:01d71
l_01d73:
    mov dword ptr [edi + 07fffh], ecx ; 1:01d73
l_01d79:
    adc bh, dl ; 1:01d79
l_01d7b:
; Raster dword block 58: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:01d7b
l_01d7d:
    mov bp, bx ; 1:01d7d
l_01d80:
    add ebp, dword ptr [_raster_texture] ; 1:01d80
l_01d86:
    nop ; 1:01d86
l_01d87:
    mov cl, byte ptr [ebp] ; 1:01d87
l_01d8a:
    add esi, edx ; 1:01d8a
l_01d8c:
    adc ebx, eax ; 1:01d8c
l_01d8e:
    adc bh, dl ; 1:01d8e
l_01d90:
    xor ebp, ebp ; 1:01d90
l_01d92:
    mov bp, bx ; 1:01d92
l_01d95:
    add ebp, dword ptr [_raster_texture] ; 1:01d95
l_01d9b:
    nop ; 1:01d9b
l_01d9c:
    mov ch, byte ptr [ebp] ; 1:01d9c
l_01d9f:
    shl ecx, 010h ; 1:01d9f
l_01da2:
    add esi, edx ; 1:01da2
l_01da4:
    adc ebx, eax ; 1:01da4
l_01da6:
    adc bh, dl ; 1:01da6
l_01da8:
    xor ebp, ebp ; 1:01da8
l_01daa:
    mov bp, bx ; 1:01daa
l_01dad:
    add ebp, dword ptr [_raster_texture] ; 1:01dad
l_01db3:
    nop ; 1:01db3
l_01db4:
    mov cl, byte ptr [ebp] ; 1:01db4
l_01db7:
    add esi, edx ; 1:01db7
l_01db9:
    adc ebx, eax ; 1:01db9
l_01dbb:
    adc bh, dl ; 1:01dbb
l_01dbd:
    xor ebp, ebp ; 1:01dbd
l_01dbf:
    mov bp, bx ; 1:01dbf
l_01dc2:
    add ebp, dword ptr [_raster_texture] ; 1:01dc2
l_01dc8:
    nop ; 1:01dc8
l_01dc9:
    mov ch, byte ptr [ebp] ; 1:01dc9
l_01dcc:
    rol ecx, 010h ; 1:01dcc
l_01dcf:
    add esi, edx ; 1:01dcf
l_01dd1:
    adc ebx, eax ; 1:01dd1
l_01dd3:
    mov dword ptr [edi + 07fffh], ecx ; 1:01dd3
l_01dd9:
    adc bh, dl ; 1:01dd9
l_01ddb:
; Raster dword block 59: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:01ddb
l_01ddd:
    mov bp, bx ; 1:01ddd
l_01de0:
    add ebp, dword ptr [_raster_texture] ; 1:01de0
l_01de6:
    nop ; 1:01de6
l_01de7:
    mov cl, byte ptr [ebp] ; 1:01de7
l_01dea:
    add esi, edx ; 1:01dea
l_01dec:
    adc ebx, eax ; 1:01dec
l_01dee:
    adc bh, dl ; 1:01dee
l_01df0:
    xor ebp, ebp ; 1:01df0
l_01df2:
    mov bp, bx ; 1:01df2
l_01df5:
    add ebp, dword ptr [_raster_texture] ; 1:01df5
l_01dfb:
    nop ; 1:01dfb
l_01dfc:
    mov ch, byte ptr [ebp] ; 1:01dfc
l_01dff:
    shl ecx, 010h ; 1:01dff
l_01e02:
    add esi, edx ; 1:01e02
l_01e04:
    adc ebx, eax ; 1:01e04
l_01e06:
    adc bh, dl ; 1:01e06
l_01e08:
    xor ebp, ebp ; 1:01e08
l_01e0a:
    mov bp, bx ; 1:01e0a
l_01e0d:
    add ebp, dword ptr [_raster_texture] ; 1:01e0d
l_01e13:
    nop ; 1:01e13
l_01e14:
    mov cl, byte ptr [ebp] ; 1:01e14
l_01e17:
    add esi, edx ; 1:01e17
l_01e19:
    adc ebx, eax ; 1:01e19
l_01e1b:
    adc bh, dl ; 1:01e1b
l_01e1d:
    xor ebp, ebp ; 1:01e1d
l_01e1f:
    mov bp, bx ; 1:01e1f
l_01e22:
    add ebp, dword ptr [_raster_texture] ; 1:01e22
l_01e28:
    nop ; 1:01e28
l_01e29:
    mov ch, byte ptr [ebp] ; 1:01e29
l_01e2c:
    rol ecx, 010h ; 1:01e2c
l_01e2f:
    add esi, edx ; 1:01e2f
l_01e31:
    adc ebx, eax ; 1:01e31
l_01e33:
    mov dword ptr [edi + 07fffh], ecx ; 1:01e33
l_01e39:
    adc bh, dl ; 1:01e39
l_01e3b:
; Raster dword block 60: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:01e3b
l_01e3d:
    mov bp, bx ; 1:01e3d
l_01e40:
    add ebp, dword ptr [_raster_texture] ; 1:01e40
l_01e46:
    nop ; 1:01e46
l_01e47:
    mov cl, byte ptr [ebp] ; 1:01e47
l_01e4a:
    add esi, edx ; 1:01e4a
l_01e4c:
    adc ebx, eax ; 1:01e4c
l_01e4e:
    adc bh, dl ; 1:01e4e
l_01e50:
    xor ebp, ebp ; 1:01e50
l_01e52:
    mov bp, bx ; 1:01e52
l_01e55:
    add ebp, dword ptr [_raster_texture] ; 1:01e55
l_01e5b:
    nop ; 1:01e5b
l_01e5c:
    mov ch, byte ptr [ebp] ; 1:01e5c
l_01e5f:
    shl ecx, 010h ; 1:01e5f
l_01e62:
    add esi, edx ; 1:01e62
l_01e64:
    adc ebx, eax ; 1:01e64
l_01e66:
    adc bh, dl ; 1:01e66
l_01e68:
    xor ebp, ebp ; 1:01e68
l_01e6a:
    mov bp, bx ; 1:01e6a
l_01e6d:
    add ebp, dword ptr [_raster_texture] ; 1:01e6d
l_01e73:
    nop ; 1:01e73
l_01e74:
    mov cl, byte ptr [ebp] ; 1:01e74
l_01e77:
    add esi, edx ; 1:01e77
l_01e79:
    adc ebx, eax ; 1:01e79
l_01e7b:
    adc bh, dl ; 1:01e7b
l_01e7d:
    xor ebp, ebp ; 1:01e7d
l_01e7f:
    mov bp, bx ; 1:01e7f
l_01e82:
    add ebp, dword ptr [_raster_texture] ; 1:01e82
l_01e88:
    nop ; 1:01e88
l_01e89:
    mov ch, byte ptr [ebp] ; 1:01e89
l_01e8c:
    rol ecx, 010h ; 1:01e8c
l_01e8f:
    add esi, edx ; 1:01e8f
l_01e91:
    adc ebx, eax ; 1:01e91
l_01e93:
    mov dword ptr [edi + 07fffh], ecx ; 1:01e93
l_01e99:
    adc bh, dl ; 1:01e99
l_01e9b:
; Raster dword block 61: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:01e9b
l_01e9d:
    mov bp, bx ; 1:01e9d
l_01ea0:
    add ebp, dword ptr [_raster_texture] ; 1:01ea0
l_01ea6:
    nop ; 1:01ea6
l_01ea7:
    mov cl, byte ptr [ebp] ; 1:01ea7
l_01eaa:
    add esi, edx ; 1:01eaa
l_01eac:
    adc ebx, eax ; 1:01eac
l_01eae:
    adc bh, dl ; 1:01eae
l_01eb0:
    xor ebp, ebp ; 1:01eb0
l_01eb2:
    mov bp, bx ; 1:01eb2
l_01eb5:
    add ebp, dword ptr [_raster_texture] ; 1:01eb5
l_01ebb:
    nop ; 1:01ebb
l_01ebc:
    mov ch, byte ptr [ebp] ; 1:01ebc
l_01ebf:
    shl ecx, 010h ; 1:01ebf
l_01ec2:
    add esi, edx ; 1:01ec2
l_01ec4:
    adc ebx, eax ; 1:01ec4
l_01ec6:
    adc bh, dl ; 1:01ec6
l_01ec8:
    xor ebp, ebp ; 1:01ec8
l_01eca:
    mov bp, bx ; 1:01eca
l_01ecd:
    add ebp, dword ptr [_raster_texture] ; 1:01ecd
l_01ed3:
    nop ; 1:01ed3
l_01ed4:
    mov cl, byte ptr [ebp] ; 1:01ed4
l_01ed7:
    add esi, edx ; 1:01ed7
l_01ed9:
    adc ebx, eax ; 1:01ed9
l_01edb:
    adc bh, dl ; 1:01edb
l_01edd:
    xor ebp, ebp ; 1:01edd
l_01edf:
    mov bp, bx ; 1:01edf
l_01ee2:
    add ebp, dword ptr [_raster_texture] ; 1:01ee2
l_01ee8:
    nop ; 1:01ee8
l_01ee9:
    mov ch, byte ptr [ebp] ; 1:01ee9
l_01eec:
    rol ecx, 010h ; 1:01eec
l_01eef:
    add esi, edx ; 1:01eef
l_01ef1:
    adc ebx, eax ; 1:01ef1
l_01ef3:
    mov dword ptr [edi + 07fffh], ecx ; 1:01ef3
l_01ef9:
    adc bh, dl ; 1:01ef9
l_01efb:
; Raster dword block 62: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:01efb
l_01efd:
    mov bp, bx ; 1:01efd
l_01f00:
    add ebp, dword ptr [_raster_texture] ; 1:01f00
l_01f06:
    nop ; 1:01f06
l_01f07:
    mov cl, byte ptr [ebp] ; 1:01f07
l_01f0a:
    add esi, edx ; 1:01f0a
l_01f0c:
    adc ebx, eax ; 1:01f0c
l_01f0e:
    adc bh, dl ; 1:01f0e
l_01f10:
    xor ebp, ebp ; 1:01f10
l_01f12:
    mov bp, bx ; 1:01f12
l_01f15:
    add ebp, dword ptr [_raster_texture] ; 1:01f15
l_01f1b:
    nop ; 1:01f1b
l_01f1c:
    mov ch, byte ptr [ebp] ; 1:01f1c
l_01f1f:
    shl ecx, 010h ; 1:01f1f
l_01f22:
    add esi, edx ; 1:01f22
l_01f24:
    adc ebx, eax ; 1:01f24
l_01f26:
    adc bh, dl ; 1:01f26
l_01f28:
    xor ebp, ebp ; 1:01f28
l_01f2a:
    mov bp, bx ; 1:01f2a
l_01f2d:
    add ebp, dword ptr [_raster_texture] ; 1:01f2d
l_01f33:
    nop ; 1:01f33
l_01f34:
    mov cl, byte ptr [ebp] ; 1:01f34
l_01f37:
    add esi, edx ; 1:01f37
l_01f39:
    adc ebx, eax ; 1:01f39
l_01f3b:
    adc bh, dl ; 1:01f3b
l_01f3d:
    xor ebp, ebp ; 1:01f3d
l_01f3f:
    mov bp, bx ; 1:01f3f
l_01f42:
    add ebp, dword ptr [_raster_texture] ; 1:01f42
l_01f48:
    nop ; 1:01f48
l_01f49:
    mov ch, byte ptr [ebp] ; 1:01f49
l_01f4c:
    rol ecx, 010h ; 1:01f4c
l_01f4f:
    add esi, edx ; 1:01f4f
l_01f51:
    adc ebx, eax ; 1:01f51
l_01f53:
    mov dword ptr [edi + 07fffh], ecx ; 1:01f53
l_01f59:
    adc bh, dl ; 1:01f59
l_01f5b:
; Raster dword block 63: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:01f5b
l_01f5d:
    mov bp, bx ; 1:01f5d
l_01f60:
    add ebp, dword ptr [_raster_texture] ; 1:01f60
l_01f66:
    nop ; 1:01f66
l_01f67:
    mov cl, byte ptr [ebp] ; 1:01f67
l_01f6a:
    add esi, edx ; 1:01f6a
l_01f6c:
    adc ebx, eax ; 1:01f6c
l_01f6e:
    adc bh, dl ; 1:01f6e
l_01f70:
    xor ebp, ebp ; 1:01f70
l_01f72:
    mov bp, bx ; 1:01f72
l_01f75:
    add ebp, dword ptr [_raster_texture] ; 1:01f75
l_01f7b:
    nop ; 1:01f7b
l_01f7c:
    mov ch, byte ptr [ebp] ; 1:01f7c
l_01f7f:
    shl ecx, 010h ; 1:01f7f
l_01f82:
    add esi, edx ; 1:01f82
l_01f84:
    adc ebx, eax ; 1:01f84
l_01f86:
    adc bh, dl ; 1:01f86
l_01f88:
    xor ebp, ebp ; 1:01f88
l_01f8a:
    mov bp, bx ; 1:01f8a
l_01f8d:
    add ebp, dword ptr [_raster_texture] ; 1:01f8d
l_01f93:
    nop ; 1:01f93
l_01f94:
    mov cl, byte ptr [ebp] ; 1:01f94
l_01f97:
    add esi, edx ; 1:01f97
l_01f99:
    adc ebx, eax ; 1:01f99
l_01f9b:
    adc bh, dl ; 1:01f9b
l_01f9d:
    xor ebp, ebp ; 1:01f9d
l_01f9f:
    mov bp, bx ; 1:01f9f
l_01fa2:
    add ebp, dword ptr [_raster_texture] ; 1:01fa2
l_01fa8:
    nop ; 1:01fa8
l_01fa9:
    mov ch, byte ptr [ebp] ; 1:01fa9
l_01fac:
    rol ecx, 010h ; 1:01fac
l_01faf:
    add esi, edx ; 1:01faf
l_01fb1:
    adc ebx, eax ; 1:01fb1
l_01fb3:
    mov dword ptr [edi + 07fffh], ecx ; 1:01fb3
l_01fb9:
    adc bh, dl ; 1:01fb9
l_01fbb:
; Raster dword block 64: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:01fbb
l_01fbd:
    mov bp, bx ; 1:01fbd
l_01fc0:
    add ebp, dword ptr [_raster_texture] ; 1:01fc0
l_01fc6:
    nop ; 1:01fc6
l_01fc7:
    mov cl, byte ptr [ebp] ; 1:01fc7
l_01fca:
    add esi, edx ; 1:01fca
l_01fcc:
    adc ebx, eax ; 1:01fcc
l_01fce:
    adc bh, dl ; 1:01fce
l_01fd0:
    xor ebp, ebp ; 1:01fd0
l_01fd2:
    mov bp, bx ; 1:01fd2
l_01fd5:
    add ebp, dword ptr [_raster_texture] ; 1:01fd5
l_01fdb:
    nop ; 1:01fdb
l_01fdc:
    mov ch, byte ptr [ebp] ; 1:01fdc
l_01fdf:
    shl ecx, 010h ; 1:01fdf
l_01fe2:
    add esi, edx ; 1:01fe2
l_01fe4:
    adc ebx, eax ; 1:01fe4
l_01fe6:
    adc bh, dl ; 1:01fe6
l_01fe8:
    xor ebp, ebp ; 1:01fe8
l_01fea:
    mov bp, bx ; 1:01fea
l_01fed:
    add ebp, dword ptr [_raster_texture] ; 1:01fed
l_01ff3:
    nop ; 1:01ff3
l_01ff4:
    mov cl, byte ptr [ebp] ; 1:01ff4
l_01ff7:
    add esi, edx ; 1:01ff7
l_01ff9:
    adc ebx, eax ; 1:01ff9
l_01ffb:
    adc bh, dl ; 1:01ffb
l_01ffd:
    xor ebp, ebp ; 1:01ffd
l_01fff:
    mov bp, bx ; 1:01fff
l_02002:
    add ebp, dword ptr [_raster_texture] ; 1:02002
l_02008:
    nop ; 1:02008
l_02009:
    mov ch, byte ptr [ebp] ; 1:02009
l_0200c:
    rol ecx, 010h ; 1:0200c
l_0200f:
    add esi, edx ; 1:0200f
l_02011:
    adc ebx, eax ; 1:02011
l_02013:
    mov dword ptr [edi + 07fffh], ecx ; 1:02013
l_02019:
    adc bh, dl ; 1:02019
l_0201b:
; Raster dword block 65: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0201b
l_0201d:
    mov bp, bx ; 1:0201d
l_02020:
    add ebp, dword ptr [_raster_texture] ; 1:02020
l_02026:
    nop ; 1:02026
l_02027:
    mov cl, byte ptr [ebp] ; 1:02027
l_0202a:
    add esi, edx ; 1:0202a
l_0202c:
    adc ebx, eax ; 1:0202c
l_0202e:
    adc bh, dl ; 1:0202e
l_02030:
    xor ebp, ebp ; 1:02030
l_02032:
    mov bp, bx ; 1:02032
l_02035:
    add ebp, dword ptr [_raster_texture] ; 1:02035
l_0203b:
    nop ; 1:0203b
l_0203c:
    mov ch, byte ptr [ebp] ; 1:0203c
l_0203f:
    shl ecx, 010h ; 1:0203f
l_02042:
    add esi, edx ; 1:02042
l_02044:
    adc ebx, eax ; 1:02044
l_02046:
    adc bh, dl ; 1:02046
l_02048:
    xor ebp, ebp ; 1:02048
l_0204a:
    mov bp, bx ; 1:0204a
l_0204d:
    add ebp, dword ptr [_raster_texture] ; 1:0204d
l_02053:
    nop ; 1:02053
l_02054:
    mov cl, byte ptr [ebp] ; 1:02054
l_02057:
    add esi, edx ; 1:02057
l_02059:
    adc ebx, eax ; 1:02059
l_0205b:
    adc bh, dl ; 1:0205b
l_0205d:
    xor ebp, ebp ; 1:0205d
l_0205f:
    mov bp, bx ; 1:0205f
l_02062:
    add ebp, dword ptr [_raster_texture] ; 1:02062
l_02068:
    nop ; 1:02068
l_02069:
    mov ch, byte ptr [ebp] ; 1:02069
l_0206c:
    rol ecx, 010h ; 1:0206c
l_0206f:
    add esi, edx ; 1:0206f
l_02071:
    adc ebx, eax ; 1:02071
l_02073:
    mov dword ptr [edi + 07fffh], ecx ; 1:02073
l_02079:
    adc bh, dl ; 1:02079
l_0207b:
; Raster dword block 66: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0207b
l_0207d:
    mov bp, bx ; 1:0207d
l_02080:
    add ebp, dword ptr [_raster_texture] ; 1:02080
l_02086:
    nop ; 1:02086
l_02087:
    mov cl, byte ptr [ebp] ; 1:02087
l_0208a:
    add esi, edx ; 1:0208a
l_0208c:
    adc ebx, eax ; 1:0208c
l_0208e:
    adc bh, dl ; 1:0208e
l_02090:
    xor ebp, ebp ; 1:02090
l_02092:
    mov bp, bx ; 1:02092
l_02095:
    add ebp, dword ptr [_raster_texture] ; 1:02095
l_0209b:
    nop ; 1:0209b
l_0209c:
    mov ch, byte ptr [ebp] ; 1:0209c
l_0209f:
    shl ecx, 010h ; 1:0209f
l_020a2:
    add esi, edx ; 1:020a2
l_020a4:
    adc ebx, eax ; 1:020a4
l_020a6:
    adc bh, dl ; 1:020a6
l_020a8:
    xor ebp, ebp ; 1:020a8
l_020aa:
    mov bp, bx ; 1:020aa
l_020ad:
    add ebp, dword ptr [_raster_texture] ; 1:020ad
l_020b3:
    nop ; 1:020b3
l_020b4:
    mov cl, byte ptr [ebp] ; 1:020b4
l_020b7:
    add esi, edx ; 1:020b7
l_020b9:
    adc ebx, eax ; 1:020b9
l_020bb:
    adc bh, dl ; 1:020bb
l_020bd:
    xor ebp, ebp ; 1:020bd
l_020bf:
    mov bp, bx ; 1:020bf
l_020c2:
    add ebp, dword ptr [_raster_texture] ; 1:020c2
l_020c8:
    nop ; 1:020c8
l_020c9:
    mov ch, byte ptr [ebp] ; 1:020c9
l_020cc:
    rol ecx, 010h ; 1:020cc
l_020cf:
    add esi, edx ; 1:020cf
l_020d1:
    adc ebx, eax ; 1:020d1
l_020d3:
    mov dword ptr [edi + 07fffh], ecx ; 1:020d3
l_020d9:
    adc bh, dl ; 1:020d9
l_020db:
; Raster dword block 67: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:020db
l_020dd:
    mov bp, bx ; 1:020dd
l_020e0:
    add ebp, dword ptr [_raster_texture] ; 1:020e0
l_020e6:
    nop ; 1:020e6
l_020e7:
    mov cl, byte ptr [ebp] ; 1:020e7
l_020ea:
    add esi, edx ; 1:020ea
l_020ec:
    adc ebx, eax ; 1:020ec
l_020ee:
    adc bh, dl ; 1:020ee
l_020f0:
    xor ebp, ebp ; 1:020f0
l_020f2:
    mov bp, bx ; 1:020f2
l_020f5:
    add ebp, dword ptr [_raster_texture] ; 1:020f5
l_020fb:
    nop ; 1:020fb
l_020fc:
    mov ch, byte ptr [ebp] ; 1:020fc
l_020ff:
    shl ecx, 010h ; 1:020ff
l_02102:
    add esi, edx ; 1:02102
l_02104:
    adc ebx, eax ; 1:02104
l_02106:
    adc bh, dl ; 1:02106
l_02108:
    xor ebp, ebp ; 1:02108
l_0210a:
    mov bp, bx ; 1:0210a
l_0210d:
    add ebp, dword ptr [_raster_texture] ; 1:0210d
l_02113:
    nop ; 1:02113
l_02114:
    mov cl, byte ptr [ebp] ; 1:02114
l_02117:
    add esi, edx ; 1:02117
l_02119:
    adc ebx, eax ; 1:02119
l_0211b:
    adc bh, dl ; 1:0211b
l_0211d:
    xor ebp, ebp ; 1:0211d
l_0211f:
    mov bp, bx ; 1:0211f
l_02122:
    add ebp, dword ptr [_raster_texture] ; 1:02122
l_02128:
    nop ; 1:02128
l_02129:
    mov ch, byte ptr [ebp] ; 1:02129
l_0212c:
    rol ecx, 010h ; 1:0212c
l_0212f:
    add esi, edx ; 1:0212f
l_02131:
    adc ebx, eax ; 1:02131
l_02133:
    mov dword ptr [edi + 07fffh], ecx ; 1:02133
l_02139:
    adc bh, dl ; 1:02139
l_0213b:
; Raster dword block 68: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0213b
l_0213d:
    mov bp, bx ; 1:0213d
l_02140:
    add ebp, dword ptr [_raster_texture] ; 1:02140
l_02146:
    nop ; 1:02146
l_02147:
    mov cl, byte ptr [ebp] ; 1:02147
l_0214a:
    add esi, edx ; 1:0214a
l_0214c:
    adc ebx, eax ; 1:0214c
l_0214e:
    adc bh, dl ; 1:0214e
l_02150:
    xor ebp, ebp ; 1:02150
l_02152:
    mov bp, bx ; 1:02152
l_02155:
    add ebp, dword ptr [_raster_texture] ; 1:02155
l_0215b:
    nop ; 1:0215b
l_0215c:
    mov ch, byte ptr [ebp] ; 1:0215c
l_0215f:
    shl ecx, 010h ; 1:0215f
l_02162:
    add esi, edx ; 1:02162
l_02164:
    adc ebx, eax ; 1:02164
l_02166:
    adc bh, dl ; 1:02166
l_02168:
    xor ebp, ebp ; 1:02168
l_0216a:
    mov bp, bx ; 1:0216a
l_0216d:
    add ebp, dword ptr [_raster_texture] ; 1:0216d
l_02173:
    nop ; 1:02173
l_02174:
    mov cl, byte ptr [ebp] ; 1:02174
l_02177:
    add esi, edx ; 1:02177
l_02179:
    adc ebx, eax ; 1:02179
l_0217b:
    adc bh, dl ; 1:0217b
l_0217d:
    xor ebp, ebp ; 1:0217d
l_0217f:
    mov bp, bx ; 1:0217f
l_02182:
    add ebp, dword ptr [_raster_texture] ; 1:02182
l_02188:
    nop ; 1:02188
l_02189:
    mov ch, byte ptr [ebp] ; 1:02189
l_0218c:
    rol ecx, 010h ; 1:0218c
l_0218f:
    add esi, edx ; 1:0218f
l_02191:
    adc ebx, eax ; 1:02191
l_02193:
    mov dword ptr [edi + 07fffh], ecx ; 1:02193
l_02199:
    adc bh, dl ; 1:02199
l_0219b:
; Raster dword block 69: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0219b
l_0219d:
    mov bp, bx ; 1:0219d
l_021a0:
    add ebp, dword ptr [_raster_texture] ; 1:021a0
l_021a6:
    nop ; 1:021a6
l_021a7:
    mov cl, byte ptr [ebp] ; 1:021a7
l_021aa:
    add esi, edx ; 1:021aa
l_021ac:
    adc ebx, eax ; 1:021ac
l_021ae:
    adc bh, dl ; 1:021ae
l_021b0:
    xor ebp, ebp ; 1:021b0
l_021b2:
    mov bp, bx ; 1:021b2
l_021b5:
    add ebp, dword ptr [_raster_texture] ; 1:021b5
l_021bb:
    nop ; 1:021bb
l_021bc:
    mov ch, byte ptr [ebp] ; 1:021bc
l_021bf:
    shl ecx, 010h ; 1:021bf
l_021c2:
    add esi, edx ; 1:021c2
l_021c4:
    adc ebx, eax ; 1:021c4
l_021c6:
    adc bh, dl ; 1:021c6
l_021c8:
    xor ebp, ebp ; 1:021c8
l_021ca:
    mov bp, bx ; 1:021ca
l_021cd:
    add ebp, dword ptr [_raster_texture] ; 1:021cd
l_021d3:
    nop ; 1:021d3
l_021d4:
    mov cl, byte ptr [ebp] ; 1:021d4
l_021d7:
    add esi, edx ; 1:021d7
l_021d9:
    adc ebx, eax ; 1:021d9
l_021db:
    adc bh, dl ; 1:021db
l_021dd:
    xor ebp, ebp ; 1:021dd
l_021df:
    mov bp, bx ; 1:021df
l_021e2:
    add ebp, dword ptr [_raster_texture] ; 1:021e2
l_021e8:
    nop ; 1:021e8
l_021e9:
    mov ch, byte ptr [ebp] ; 1:021e9
l_021ec:
    rol ecx, 010h ; 1:021ec
l_021ef:
    add esi, edx ; 1:021ef
l_021f1:
    adc ebx, eax ; 1:021f1
l_021f3:
    mov dword ptr [edi + 07fffh], ecx ; 1:021f3
l_021f9:
    adc bh, dl ; 1:021f9
l_021fb:
; Raster dword block 70: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:021fb
l_021fd:
    mov bp, bx ; 1:021fd
l_02200:
    add ebp, dword ptr [_raster_texture] ; 1:02200
l_02206:
    nop ; 1:02206
l_02207:
    mov cl, byte ptr [ebp] ; 1:02207
l_0220a:
    add esi, edx ; 1:0220a
l_0220c:
    adc ebx, eax ; 1:0220c
l_0220e:
    adc bh, dl ; 1:0220e
l_02210:
    xor ebp, ebp ; 1:02210
l_02212:
    mov bp, bx ; 1:02212
l_02215:
    add ebp, dword ptr [_raster_texture] ; 1:02215
l_0221b:
    nop ; 1:0221b
l_0221c:
    mov ch, byte ptr [ebp] ; 1:0221c
l_0221f:
    shl ecx, 010h ; 1:0221f
l_02222:
    add esi, edx ; 1:02222
l_02224:
    adc ebx, eax ; 1:02224
l_02226:
    adc bh, dl ; 1:02226
l_02228:
    xor ebp, ebp ; 1:02228
l_0222a:
    mov bp, bx ; 1:0222a
l_0222d:
    add ebp, dword ptr [_raster_texture] ; 1:0222d
l_02233:
    nop ; 1:02233
l_02234:
    mov cl, byte ptr [ebp] ; 1:02234
l_02237:
    add esi, edx ; 1:02237
l_02239:
    adc ebx, eax ; 1:02239
l_0223b:
    adc bh, dl ; 1:0223b
l_0223d:
    xor ebp, ebp ; 1:0223d
l_0223f:
    mov bp, bx ; 1:0223f
l_02242:
    add ebp, dword ptr [_raster_texture] ; 1:02242
l_02248:
    nop ; 1:02248
l_02249:
    mov ch, byte ptr [ebp] ; 1:02249
l_0224c:
    rol ecx, 010h ; 1:0224c
l_0224f:
    add esi, edx ; 1:0224f
l_02251:
    adc ebx, eax ; 1:02251
l_02253:
    mov dword ptr [edi + 07fffh], ecx ; 1:02253
l_02259:
    adc bh, dl ; 1:02259
l_0225b:
; Raster dword block 71: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0225b
l_0225d:
    mov bp, bx ; 1:0225d
l_02260:
    add ebp, dword ptr [_raster_texture] ; 1:02260
l_02266:
    nop ; 1:02266
l_02267:
    mov cl, byte ptr [ebp] ; 1:02267
l_0226a:
    add esi, edx ; 1:0226a
l_0226c:
    adc ebx, eax ; 1:0226c
l_0226e:
    adc bh, dl ; 1:0226e
l_02270:
    xor ebp, ebp ; 1:02270
l_02272:
    mov bp, bx ; 1:02272
l_02275:
    add ebp, dword ptr [_raster_texture] ; 1:02275
l_0227b:
    nop ; 1:0227b
l_0227c:
    mov ch, byte ptr [ebp] ; 1:0227c
l_0227f:
    shl ecx, 010h ; 1:0227f
l_02282:
    add esi, edx ; 1:02282
l_02284:
    adc ebx, eax ; 1:02284
l_02286:
    adc bh, dl ; 1:02286
l_02288:
    xor ebp, ebp ; 1:02288
l_0228a:
    mov bp, bx ; 1:0228a
l_0228d:
    add ebp, dword ptr [_raster_texture] ; 1:0228d
l_02293:
    nop ; 1:02293
l_02294:
    mov cl, byte ptr [ebp] ; 1:02294
l_02297:
    add esi, edx ; 1:02297
l_02299:
    adc ebx, eax ; 1:02299
l_0229b:
    adc bh, dl ; 1:0229b
l_0229d:
    xor ebp, ebp ; 1:0229d
l_0229f:
    mov bp, bx ; 1:0229f
l_022a2:
    add ebp, dword ptr [_raster_texture] ; 1:022a2
l_022a8:
    nop ; 1:022a8
l_022a9:
    mov ch, byte ptr [ebp] ; 1:022a9
l_022ac:
    rol ecx, 010h ; 1:022ac
l_022af:
    add esi, edx ; 1:022af
l_022b1:
    adc ebx, eax ; 1:022b1
l_022b3:
    mov dword ptr [edi + 07fffh], ecx ; 1:022b3
l_022b9:
    adc bh, dl ; 1:022b9
l_022bb:
; Raster dword block 72: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:022bb
l_022bd:
    mov bp, bx ; 1:022bd
l_022c0:
    add ebp, dword ptr [_raster_texture] ; 1:022c0
l_022c6:
    nop ; 1:022c6
l_022c7:
    mov cl, byte ptr [ebp] ; 1:022c7
l_022ca:
    add esi, edx ; 1:022ca
l_022cc:
    adc ebx, eax ; 1:022cc
l_022ce:
    adc bh, dl ; 1:022ce
l_022d0:
    xor ebp, ebp ; 1:022d0
l_022d2:
    mov bp, bx ; 1:022d2
l_022d5:
    add ebp, dword ptr [_raster_texture] ; 1:022d5
l_022db:
    nop ; 1:022db
l_022dc:
    mov ch, byte ptr [ebp] ; 1:022dc
l_022df:
    shl ecx, 010h ; 1:022df
l_022e2:
    add esi, edx ; 1:022e2
l_022e4:
    adc ebx, eax ; 1:022e4
l_022e6:
    adc bh, dl ; 1:022e6
l_022e8:
    xor ebp, ebp ; 1:022e8
l_022ea:
    mov bp, bx ; 1:022ea
l_022ed:
    add ebp, dword ptr [_raster_texture] ; 1:022ed
l_022f3:
    nop ; 1:022f3
l_022f4:
    mov cl, byte ptr [ebp] ; 1:022f4
l_022f7:
    add esi, edx ; 1:022f7
l_022f9:
    adc ebx, eax ; 1:022f9
l_022fb:
    adc bh, dl ; 1:022fb
l_022fd:
    xor ebp, ebp ; 1:022fd
l_022ff:
    mov bp, bx ; 1:022ff
l_02302:
    add ebp, dword ptr [_raster_texture] ; 1:02302
l_02308:
    nop ; 1:02308
l_02309:
    mov ch, byte ptr [ebp] ; 1:02309
l_0230c:
    rol ecx, 010h ; 1:0230c
l_0230f:
    add esi, edx ; 1:0230f
l_02311:
    adc ebx, eax ; 1:02311
l_02313:
    mov dword ptr [edi + 07fffh], ecx ; 1:02313
l_02319:
    adc bh, dl ; 1:02319
l_0231b:
; Raster dword block 73: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0231b
l_0231d:
    mov bp, bx ; 1:0231d
l_02320:
    add ebp, dword ptr [_raster_texture] ; 1:02320
l_02326:
    nop ; 1:02326
l_02327:
    mov cl, byte ptr [ebp] ; 1:02327
l_0232a:
    add esi, edx ; 1:0232a
l_0232c:
    adc ebx, eax ; 1:0232c
l_0232e:
    adc bh, dl ; 1:0232e
l_02330:
    xor ebp, ebp ; 1:02330
l_02332:
    mov bp, bx ; 1:02332
l_02335:
    add ebp, dword ptr [_raster_texture] ; 1:02335
l_0233b:
    nop ; 1:0233b
l_0233c:
    mov ch, byte ptr [ebp] ; 1:0233c
l_0233f:
    shl ecx, 010h ; 1:0233f
l_02342:
    add esi, edx ; 1:02342
l_02344:
    adc ebx, eax ; 1:02344
l_02346:
    adc bh, dl ; 1:02346
l_02348:
    xor ebp, ebp ; 1:02348
l_0234a:
    mov bp, bx ; 1:0234a
l_0234d:
    add ebp, dword ptr [_raster_texture] ; 1:0234d
l_02353:
    nop ; 1:02353
l_02354:
    mov cl, byte ptr [ebp] ; 1:02354
l_02357:
    add esi, edx ; 1:02357
l_02359:
    adc ebx, eax ; 1:02359
l_0235b:
    adc bh, dl ; 1:0235b
l_0235d:
    xor ebp, ebp ; 1:0235d
l_0235f:
    mov bp, bx ; 1:0235f
l_02362:
    add ebp, dword ptr [_raster_texture] ; 1:02362
l_02368:
    nop ; 1:02368
l_02369:
    mov ch, byte ptr [ebp] ; 1:02369
l_0236c:
    rol ecx, 010h ; 1:0236c
l_0236f:
    add esi, edx ; 1:0236f
l_02371:
    adc ebx, eax ; 1:02371
l_02373:
    mov dword ptr [edi + 07fffh], ecx ; 1:02373
l_02379:
    adc bh, dl ; 1:02379
l_0237b:
; Raster dword block 74: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0237b
l_0237d:
    mov bp, bx ; 1:0237d
l_02380:
    add ebp, dword ptr [_raster_texture] ; 1:02380
l_02386:
    nop ; 1:02386
l_02387:
    mov cl, byte ptr [ebp] ; 1:02387
l_0238a:
    add esi, edx ; 1:0238a
l_0238c:
    adc ebx, eax ; 1:0238c
l_0238e:
    adc bh, dl ; 1:0238e
l_02390:
    xor ebp, ebp ; 1:02390
l_02392:
    mov bp, bx ; 1:02392
l_02395:
    add ebp, dword ptr [_raster_texture] ; 1:02395
l_0239b:
    nop ; 1:0239b
l_0239c:
    mov ch, byte ptr [ebp] ; 1:0239c
l_0239f:
    shl ecx, 010h ; 1:0239f
l_023a2:
    add esi, edx ; 1:023a2
l_023a4:
    adc ebx, eax ; 1:023a4
l_023a6:
    adc bh, dl ; 1:023a6
l_023a8:
    xor ebp, ebp ; 1:023a8
l_023aa:
    mov bp, bx ; 1:023aa
l_023ad:
    add ebp, dword ptr [_raster_texture] ; 1:023ad
l_023b3:
    nop ; 1:023b3
l_023b4:
    mov cl, byte ptr [ebp] ; 1:023b4
l_023b7:
    add esi, edx ; 1:023b7
l_023b9:
    adc ebx, eax ; 1:023b9
l_023bb:
    adc bh, dl ; 1:023bb
l_023bd:
    xor ebp, ebp ; 1:023bd
l_023bf:
    mov bp, bx ; 1:023bf
l_023c2:
    add ebp, dword ptr [_raster_texture] ; 1:023c2
l_023c8:
    nop ; 1:023c8
l_023c9:
    mov ch, byte ptr [ebp] ; 1:023c9
l_023cc:
    rol ecx, 010h ; 1:023cc
l_023cf:
    add esi, edx ; 1:023cf
l_023d1:
    adc ebx, eax ; 1:023d1
l_023d3:
    mov dword ptr [edi + 07fffh], ecx ; 1:023d3
l_023d9:
    adc bh, dl ; 1:023d9
l_023db:
; Raster dword block 75: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:023db
l_023dd:
    mov bp, bx ; 1:023dd
l_023e0:
    add ebp, dword ptr [_raster_texture] ; 1:023e0
l_023e6:
    nop ; 1:023e6
l_023e7:
    mov cl, byte ptr [ebp] ; 1:023e7
l_023ea:
    add esi, edx ; 1:023ea
l_023ec:
    adc ebx, eax ; 1:023ec
l_023ee:
    adc bh, dl ; 1:023ee
l_023f0:
    xor ebp, ebp ; 1:023f0
l_023f2:
    mov bp, bx ; 1:023f2
l_023f5:
    add ebp, dword ptr [_raster_texture] ; 1:023f5
l_023fb:
    nop ; 1:023fb
l_023fc:
    mov ch, byte ptr [ebp] ; 1:023fc
l_023ff:
    shl ecx, 010h ; 1:023ff
l_02402:
    add esi, edx ; 1:02402
l_02404:
    adc ebx, eax ; 1:02404
l_02406:
    adc bh, dl ; 1:02406
l_02408:
    xor ebp, ebp ; 1:02408
l_0240a:
    mov bp, bx ; 1:0240a
l_0240d:
    add ebp, dword ptr [_raster_texture] ; 1:0240d
l_02413:
    nop ; 1:02413
l_02414:
    mov cl, byte ptr [ebp] ; 1:02414
l_02417:
    add esi, edx ; 1:02417
l_02419:
    adc ebx, eax ; 1:02419
l_0241b:
    adc bh, dl ; 1:0241b
l_0241d:
    xor ebp, ebp ; 1:0241d
l_0241f:
    mov bp, bx ; 1:0241f
l_02422:
    add ebp, dword ptr [_raster_texture] ; 1:02422
l_02428:
    nop ; 1:02428
l_02429:
    mov ch, byte ptr [ebp] ; 1:02429
l_0242c:
    rol ecx, 010h ; 1:0242c
l_0242f:
    add esi, edx ; 1:0242f
l_02431:
    adc ebx, eax ; 1:02431
l_02433:
    mov dword ptr [edi + 07fffh], ecx ; 1:02433
l_02439:
    adc bh, dl ; 1:02439
l_0243b:
; Raster dword block 76: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0243b
l_0243d:
    mov bp, bx ; 1:0243d
l_02440:
    add ebp, dword ptr [_raster_texture] ; 1:02440
l_02446:
    nop ; 1:02446
l_02447:
    mov cl, byte ptr [ebp] ; 1:02447
l_0244a:
    add esi, edx ; 1:0244a
l_0244c:
    adc ebx, eax ; 1:0244c
l_0244e:
    adc bh, dl ; 1:0244e
l_02450:
    xor ebp, ebp ; 1:02450
l_02452:
    mov bp, bx ; 1:02452
l_02455:
    add ebp, dword ptr [_raster_texture] ; 1:02455
l_0245b:
    nop ; 1:0245b
l_0245c:
    mov ch, byte ptr [ebp] ; 1:0245c
l_0245f:
    shl ecx, 010h ; 1:0245f
l_02462:
    add esi, edx ; 1:02462
l_02464:
    adc ebx, eax ; 1:02464
l_02466:
    adc bh, dl ; 1:02466
l_02468:
    xor ebp, ebp ; 1:02468
l_0246a:
    mov bp, bx ; 1:0246a
l_0246d:
    add ebp, dword ptr [_raster_texture] ; 1:0246d
l_02473:
    nop ; 1:02473
l_02474:
    mov cl, byte ptr [ebp] ; 1:02474
l_02477:
    add esi, edx ; 1:02477
l_02479:
    adc ebx, eax ; 1:02479
l_0247b:
    adc bh, dl ; 1:0247b
l_0247d:
    xor ebp, ebp ; 1:0247d
l_0247f:
    mov bp, bx ; 1:0247f
l_02482:
    add ebp, dword ptr [_raster_texture] ; 1:02482
l_02488:
    nop ; 1:02488
l_02489:
    mov ch, byte ptr [ebp] ; 1:02489
l_0248c:
    rol ecx, 010h ; 1:0248c
l_0248f:
    add esi, edx ; 1:0248f
l_02491:
    adc ebx, eax ; 1:02491
l_02493:
    mov dword ptr [edi + 07fffh], ecx ; 1:02493
l_02499:
    adc bh, dl ; 1:02499
l_0249b:
; Raster dword block 77: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0249b
l_0249d:
    mov bp, bx ; 1:0249d
l_024a0:
    add ebp, dword ptr [_raster_texture] ; 1:024a0
l_024a6:
    nop ; 1:024a6
l_024a7:
    mov cl, byte ptr [ebp] ; 1:024a7
l_024aa:
    add esi, edx ; 1:024aa
l_024ac:
    adc ebx, eax ; 1:024ac
l_024ae:
    adc bh, dl ; 1:024ae
l_024b0:
    xor ebp, ebp ; 1:024b0
l_024b2:
    mov bp, bx ; 1:024b2
l_024b5:
    add ebp, dword ptr [_raster_texture] ; 1:024b5
l_024bb:
    nop ; 1:024bb
l_024bc:
    mov ch, byte ptr [ebp] ; 1:024bc
l_024bf:
    shl ecx, 010h ; 1:024bf
l_024c2:
    add esi, edx ; 1:024c2
l_024c4:
    adc ebx, eax ; 1:024c4
l_024c6:
    adc bh, dl ; 1:024c6
l_024c8:
    xor ebp, ebp ; 1:024c8
l_024ca:
    mov bp, bx ; 1:024ca
l_024cd:
    add ebp, dword ptr [_raster_texture] ; 1:024cd
l_024d3:
    nop ; 1:024d3
l_024d4:
    mov cl, byte ptr [ebp] ; 1:024d4
l_024d7:
    add esi, edx ; 1:024d7
l_024d9:
    adc ebx, eax ; 1:024d9
l_024db:
    adc bh, dl ; 1:024db
l_024dd:
    xor ebp, ebp ; 1:024dd
l_024df:
    mov bp, bx ; 1:024df
l_024e2:
    add ebp, dword ptr [_raster_texture] ; 1:024e2
l_024e8:
    nop ; 1:024e8
l_024e9:
    mov ch, byte ptr [ebp] ; 1:024e9
l_024ec:
    rol ecx, 010h ; 1:024ec
l_024ef:
    add esi, edx ; 1:024ef
l_024f1:
    adc ebx, eax ; 1:024f1
l_024f3:
    mov dword ptr [edi + 07fffh], ecx ; 1:024f3
l_024f9:
    adc bh, dl ; 1:024f9
l_024fb:
; Raster dword block 78: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:024fb
l_024fd:
    mov bp, bx ; 1:024fd
l_02500:
    add ebp, dword ptr [_raster_texture] ; 1:02500
l_02506:
    nop ; 1:02506
l_02507:
    mov cl, byte ptr [ebp] ; 1:02507
l_0250a:
    add esi, edx ; 1:0250a
l_0250c:
    adc ebx, eax ; 1:0250c
l_0250e:
    adc bh, dl ; 1:0250e
l_02510:
    xor ebp, ebp ; 1:02510
l_02512:
    mov bp, bx ; 1:02512
l_02515:
    add ebp, dword ptr [_raster_texture] ; 1:02515
l_0251b:
    nop ; 1:0251b
l_0251c:
    mov ch, byte ptr [ebp] ; 1:0251c
l_0251f:
    shl ecx, 010h ; 1:0251f
l_02522:
    add esi, edx ; 1:02522
l_02524:
    adc ebx, eax ; 1:02524
l_02526:
    adc bh, dl ; 1:02526
l_02528:
    xor ebp, ebp ; 1:02528
l_0252a:
    mov bp, bx ; 1:0252a
l_0252d:
    add ebp, dword ptr [_raster_texture] ; 1:0252d
l_02533:
    nop ; 1:02533
l_02534:
    mov cl, byte ptr [ebp] ; 1:02534
l_02537:
    add esi, edx ; 1:02537
l_02539:
    adc ebx, eax ; 1:02539
l_0253b:
    adc bh, dl ; 1:0253b
l_0253d:
    xor ebp, ebp ; 1:0253d
l_0253f:
    mov bp, bx ; 1:0253f
l_02542:
    add ebp, dword ptr [_raster_texture] ; 1:02542
l_02548:
    nop ; 1:02548
l_02549:
    mov ch, byte ptr [ebp] ; 1:02549
l_0254c:
    rol ecx, 010h ; 1:0254c
l_0254f:
    add esi, edx ; 1:0254f
l_02551:
    adc ebx, eax ; 1:02551
l_02553:
    mov dword ptr [edi + 07fffh], ecx ; 1:02553
l_02559:
    adc bh, dl ; 1:02559
l_0255b:
; Raster dword block 79: four texture samples; +90 displacement patched by 00010.
    xor ebp, ebp ; 1:0255b
l_0255d:
    mov bp, bx ; 1:0255d
l_02560:
    add ebp, dword ptr [_raster_texture] ; 1:02560
l_02566:
    nop ; 1:02566
l_02567:
    mov cl, byte ptr [ebp] ; 1:02567
l_0256a:
    add esi, edx ; 1:0256a
l_0256c:
    adc ebx, eax ; 1:0256c
l_0256e:
    adc bh, dl ; 1:0256e
l_02570:
    xor ebp, ebp ; 1:02570
l_02572:
    mov bp, bx ; 1:02572
l_02575:
    add ebp, dword ptr [_raster_texture] ; 1:02575
l_0257b:
    nop ; 1:0257b
l_0257c:
    mov ch, byte ptr [ebp] ; 1:0257c
l_0257f:
    shl ecx, 010h ; 1:0257f
l_02582:
    add esi, edx ; 1:02582
l_02584:
    adc ebx, eax ; 1:02584
l_02586:
    adc bh, dl ; 1:02586
l_02588:
    xor ebp, ebp ; 1:02588
l_0258a:
    mov bp, bx ; 1:0258a
l_0258d:
    add ebp, dword ptr [_raster_texture] ; 1:0258d
l_02593:
    nop ; 1:02593
l_02594:
    mov cl, byte ptr [ebp] ; 1:02594
l_02597:
    add esi, edx ; 1:02597
l_02599:
    adc ebx, eax ; 1:02599
l_0259b:
    adc bh, dl ; 1:0259b
l_0259d:
    xor ebp, ebp ; 1:0259d
l_0259f:
    mov bp, bx ; 1:0259f
l_025a2:
    add ebp, dword ptr [_raster_texture] ; 1:025a2
l_025a8:
    nop ; 1:025a8
l_025a9:
    mov ch, byte ptr [ebp] ; 1:025a9
l_025ac:
    rol ecx, 010h ; 1:025ac
l_025af:
    add esi, edx ; 1:025af
l_025b1:
    adc ebx, eax ; 1:025b1
l_025b3:
    mov dword ptr [edi + 07fffh], ecx ; 1:025b3
l_025b9:
    adc bh, dl ; 1:025b9
l_025bb:
; Dispatch up to three trailing pixels via 24-byte sample fragments.
    pop ecx ; 1:025bb
l_025bc:
    and ecx, 3 ; 1:025bc
l_025bf:
    add edi, ecx ; 1:025bf
l_025c1:
    mov bp, cx ; 1:025c1
l_025c4:
    shl cx, 5 ; 1:025c4
l_025c8:
    shl bp, 3 ; 1:025c8
l_025cc:
    sub cx, bp ; 1:025cc
l_025cf:
    sub ecx, offset l_02621 ; 1:025cf
l_025d5:
    neg ecx ; 1:025d5
l_025d7:
    jmp ecx ; 1:025d7
l_025d9:
    xor ebp, ebp ; 1:025d9
l_025db:
    mov bp, bx ; 1:025db
l_025de:
    add ebp, dword ptr [_raster_texture] ; 1:025de
l_025e4:
    nop ; 1:025e4
l_025e5:
    mov cl, byte ptr [ebp] ; 1:025e5
l_025e8:
    add esi, edx ; 1:025e8
l_025ea:
    adc ebx, eax ; 1:025ea
l_025ec:
    adc bh, dl ; 1:025ec
l_025ee:
    mov byte ptr [edi - 3], cl ; 1:025ee
l_025f1:
    xor ebp, ebp ; 1:025f1
l_025f3:
    mov bp, bx ; 1:025f3
l_025f6:
    add ebp, dword ptr [_raster_texture] ; 1:025f6
l_025fc:
    nop ; 1:025fc
l_025fd:
    mov cl, byte ptr [ebp] ; 1:025fd
l_02600:
    add esi, edx ; 1:02600
l_02602:
    adc ebx, eax ; 1:02602
l_02604:
    adc bh, dl ; 1:02604
l_02606:
    mov byte ptr [edi - 2], cl ; 1:02606
l_02609:
    xor ebp, ebp ; 1:02609
l_0260b:
    mov bp, bx ; 1:0260b
l_0260e:
    add ebp, dword ptr [_raster_texture] ; 1:0260e
l_02614:
    nop ; 1:02614
l_02615:
    mov cl, byte ptr [ebp] ; 1:02615
l_02618:
    add esi, edx ; 1:02618
l_0261a:
    adc ebx, eax ; 1:0261a
l_0261c:
    adc bh, dl ; 1:0261c
l_0261e:
    mov byte ptr [edi - 1], cl ; 1:0261e
l_02621:
; Advance to the next screen row and repeat until rows_left is zero.
    add dword ptr [_raster_scanline], 0140h ; 1:02621
l_0262b:
    add dword ptr [_raster_row_offset], 020h ; 1:0262b
l_02632:
    dec dword ptr [_raster_rows_left] ; 1:02632
l_02638:
    jne near ptr l_0065d ; 1:02638
l_0263e:
    popad ; 1:0263e
l_0263f:
    ret ; 1:0263f
_TEXT ends
end
