
data SEGMENT
Val1 db 11110000b
Val2 db 50
String db 'new'
Val3 dw 4567d
Val4 dw 0EFh
Val5 dd 67ff89h
Val6 dd 150
Aval1 dw 150
Aval2 db 11000000b
Tval = 45
data ENDS

code SEGMENT
begin:
mov esi, 0FFFFh
mov edi, 0FFh
cmpsb
Point:
mov bh,  11110000b
mov bl,  11110000b
inc bl
jbe  Point
dec ds: dword ptr [ebp + ecx + 6]
bsr ebx, [ebp + ecx + 6]
mov edx,  0EFh
and [edx + esi + 10], edx
mov edx, 0FFFFFFFFh
or dword ptr es: [edx + esi + 10], 0101b
jmp short Label1
Label2:
mov dl, 'a'
Label1:
mov ebx, 506
mov bl, 50
idiv bl
mov ecx, Tval
Tval = 67
mov ecx, Tval
jbe Point1
mov bh, 'c'
Point1:
jmp short Label2
code ENDS
end begin