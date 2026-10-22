.386p
_TEXT segment para public use32 'CODE'
_TEXT ends
_DATA segment para public use32 'DATA'
_DATA ends
FLAT group _TEXT, _DATA
assume cs:_TEXT, ds:FLAT, ss:FLAT
.386p
_TEXT segment para public use32 'CODE'
assume cs:_TEXT, ds:FLAT
public probe_
probe_:
 mov eax, offset target_
 mov eax, dword ptr [target_]
 ret
_TEXT ends
_DATA segment para public use32 'DATA'
 db 10000h dup (0)
public target_
target_ dd 12345678h
_DATA ends
end
