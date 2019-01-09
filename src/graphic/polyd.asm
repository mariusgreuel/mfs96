;//
;// polyd.asm
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

#include "ega16.inc"

        .data
        extrn GraphicSegment : DWORD

        .code
        public _DrawPolygon_

        extrn _DrawLine_ : proc

;zeichnet ein Polygon
;> EAX = Anzahl Punkte
;  EDX = *Punkte
;< -
_DrawPolygon_ proc
        push ebx
        push ecx
        push esi
        push edi
        push ebp
        mov ebp,eax
        mov esi,edx
        dec eax
        shl eax,2
        add eax,esi
        movzx edx,[eax].Y
        movzx eax,[eax].X
L1:     push esi
        movzx ebx,[esi].X
        movzx ecx,[esi].Y
        mov esi,ebx
        mov edi,ecx
        call _DrawLine_
        mov eax,esi
        mov edx,edi
        pop esi
        add esi,4
        dec ebp
        jnz L1
        pop ebp
        pop edi
        pop esi
        pop ecx
        pop ebx
        ret
_DrawPolygon_ endp

        end

