;//
;// linev.asm
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

#include "ega16.inc"

        .data
        extrn GraphicSegment : DWORD
        extrn ClipRegion : DWORD

        .code
        public _DrawLineV_
        public _DrawLineVC_

;zeichnet vertikale Linie
;> EAX = X1, EDX = Y1
;  EBX = Y2
;< -
_DrawLineVC_ proc
        cmp edx,ebx
        je NoLine
        jl L11
        xchg edx,ebx
L11:    cmp eax,[ClipRegion.Left]
        jl NoLine
        cmp ebx,[ClipRegion.Top]
        jl NoLine
        cmp eax,[ClipRegion.Right]
        jge NoLine
        cmp edx,[ClipRegion.Bottom]
        jge NoLine
        cmp edx,[ClipRegion.Top]
        jge L12
        mov edx,[ClipRegion.Top]
L12:    cmp ebx,[ClipRegion.Bottom]
        jl L21
        mov ebx,[ClipRegion.Bottom]
        jmp L21
_DrawLineVC_ endp


;zeichnet vertikale Linie
;> EAX = X1, EDX = Y1
;  EBX = Y2
;< -
_DrawLineV_ proc
        cmp edx,ebx
        je NoLine
        jl L21
        xchg edx,ebx
L21:    push ecx
        sub ebx,edx             ;EBX = Länge
        imul edx,BYTES_PER_LINE
        mov cl,al
        shr eax,3
        add edx,eax             ;EDX=Offset
        add edx,[GraphicSegment]
        mov ah,080h
        and cl,7
        ror ah,cl               ;AL=Maske
        mov al,8
        xchg ecx,edx
        mov dx,GDC
        out dx,ax
        xchg ecx,edx
L22:    inc byte ptr [edx]
        add edx,BYTES_PER_LINE
        dec ebx
        jnz L22
        pop ecx
NoLine: ret
_DrawLineV_ endp

        end

