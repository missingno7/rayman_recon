; Investigation-only OMF feasibility probe. This 32-byte slice is NOT claimed
; to be an original object boundary. DOS 1:06e8a..1:06eaa; Android/Rayverse names.
.386p
_TEXT segment para public use32 'CODE'
assume cs:_TEXT
public synchro_, endsynchro_
synchro_:
push edx
push eax
mov dx,03dah
poll_set:
in al,dx
test ax,8
jz poll_set
pop eax
pop edx
ret
endsynchro_:
push edx
push eax
mov dx,03dah
poll_clear:
in al,dx
test ax,8
jnz poll_clear
pop eax
pop edx
ret
_TEXT ends
end
