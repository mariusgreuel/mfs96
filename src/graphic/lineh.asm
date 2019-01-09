;//
;// lineh.asm
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

#include "ega16.inc"

        .data
        extrn GraphicSegment : DWORD
        extrn ClipRegion : DWORD

        .code
        public _DrawLineH_
        public _DrawLineHC_

;zeichnet horizontale Linie (mit Clipping)
;> EAX = X1, EDX = Y1
;  EBX = X2
_DrawLineHC_ proc
        cmp eax,ebx
        je NoLine
        jl L11
        xchg eax,ebx
L11:    cmp ebx,[ClipRegion.Left]
        jl NoLine
        cmp edx,[ClipRegion.Top]
        jl NoLine
        cmp eax,[ClipRegion.Right]
        jge NoLine
        cmp edx,[ClipRegion.Bottom]
        jge NoLine
        cmp eax,[ClipRegion.Left]
        jge L12
        mov eax,[ClipRegion.Left]
L12:    cmp ebx,[ClipRegion.Right]
        jl L21
        mov ebx,[ClipRegion.Right]
        jmp L21
NoLine: ret
_DrawLineHC_ endp

;zeichnet horizontale Linie
;> EAX = X1, EDX = Y1
;  EBX = X2
_DrawLineH_ proc
        cmp eax,ebx
        je NoLine
        jl L21
        xchg eax,ebx
L21:    push ecx
        push edi
        imul edi,edx,BYTES_PER_LINE
        add edi,[GraphicSegment]
        mov cl,bl
        and cl,7
        mov dx,0ff00h
        ror edx,cl              ;Maske in DL
        mov cl,al
        and cl,7
        mov dh,0ffh
        shr dh,cl               ;Maske in DH
        shr eax,3
        shr ebx,3
        add edi,eax             ;Offset in EDI
        mov ecx,ebx
        sub ecx,eax             ;Länge in ECX
        mov eax,edx             ;Masken in AX
        mov dx,GDC
        jnz L22
        and ah,al
        mov al,8
        out dx,ax
        or [edi],ah
        jmp L24
L22:    mov bl,al
        mov al,8
        out dx,ax
        or [edi],ah
        inc edi
        dec ecx
        jz L23
        mov ax,0ff08h
        out dx,ax               ;BitMask Register
        mov al,ah
        shr ecx,1
        rep stosw
        jnc L23
        stosb
L23:    mov al,8
        mov ah,bl
        out dx,ax
        or [edi],ah
L24:    pop edi
        pop ecx
        ret
_DrawLineH_ endp

        end

