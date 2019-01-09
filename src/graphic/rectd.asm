;//
;// rectd.asm
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

#include "ega16.inc"

        .data
        extrn GraphicSegment : DWORD

        .code
        extrn _DrawLineH_  : proc
        extrn _DrawLineV_  : proc
        extrn _DrawLineHC_ : proc
        extrn _DrawLineVC_ : proc
        public _DrawRect_
        public _DrawRectC_

;zeichnet ein Rechteck (mit Clipping)
;> EAX = X1, EDX = Y1
;  EBX = X2, ECX = Y2
;< -
_DrawRectC_ proc
        cmp eax,ebx
        je NoRectangle
        jl L11
        xchg eax,ebx
L11:    cmp edx,ecx
        je NoRectangle
        jl L12
        xchg edx,ecx
L12:    push esi
        push edi
        push ebp
        mov esi,eax
        mov edi,edx
        mov ebp,ebx
        call _DrawLineHC_
        mov eax,ebp
        dec eax
        mov edx,edi
        mov ebx,ecx
        call _DrawLineVC_
        mov eax,esi
        mov edx,ecx
        dec edx
        mov ebx,ebp
        call _DrawLineHC_
        mov eax,esi
        mov edx,edi
        mov ebx,ecx
        call _DrawLineVC_
        pop ebp
        pop edi
        pop esi
NoRectangle:
        ret
_DrawRectC_ endp

;zeichnet ein Rechteck
;> EAX = X1, EDX = Y1
;  EBX = X2, ECX = Y2
;< -
_DrawRect_ proc
        cmp eax,ebx
        je NoRectangle
        jl L21
        xchg eax,ebx
L21:    cmp edx,ecx
        je NoRectangle
        jl L22
        xchg edx,ecx
L22:    push esi
        push edi
        push ebp
        mov esi,eax
        mov edi,edx
        mov ebp,ebx
        call _DrawLineH_
        mov eax,ebp
        dec eax
        mov edx,edi
        mov ebx,ecx
        call _DrawLineV_
        mov eax,esi
        mov edx,ecx
        dec edx
        mov ebx,ebp
        call _DrawLineH_
        mov eax,esi
        mov edx,edi
        mov ebx,ecx
        call _DrawLineV_
        pop ebp
        pop edi
        pop esi
        ret
_DrawRect_ endp

        end

