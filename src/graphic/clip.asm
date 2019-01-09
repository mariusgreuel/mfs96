;//
;// clip.asm
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

#include "ega16.inc"

        .data
        extrn   GraphicSegment  : DWORD
        public ClipRegion
ClipRegion      LRECT < 0, 0, X_RES, Y_RES >

        .code
        public _SetClipRegion_

;setzt die Clipregion
;> EAX = X1, EDX = Y1
;  EBX = X2, ECX = Y2
_SetClipRegion_ proc
        cmp eax,ebx
        jl L11
        xchg eax,ebx
L11:    cmp edx,ecx
        jl L12
        xchg edx,ecx
L12:    cmp eax,0
        jnl L13
        xor eax,eax
L13:    cmp edx,X_RES
        jng L14
        mov edx,X_RES
L14:    cmp ebx,0
        jnl L15
        xor ebx,ebx
L15:    cmp ecx,Y_RES
        jng L16
        mov ecx,Y_RES
L16:    mov [ClipRegion.Left],eax
        mov [ClipRegion.Top],edx
        mov [ClipRegion.Right],ebx
        mov [ClipRegion.Bottom],ecx
        ret
_SetClipRegion_ endp

        end

