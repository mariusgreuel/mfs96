;//
;// rectf.asm
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

#include "ega16.inc"

        .data
        extrn GraphicSegment : DWORD
        extrn ClipRegion : DWORD

        .code
        public _FillRect_
        public _FillRectC_

;zeichnet gefülltes Rechteck (mit Clipping)
;> EAX = X1, EDX = Y1
;  EBX = X2, ECX = Y2
;< -
_FillRectC_ proc
        cmp eax,ebx
        je NoRectangle
        jl L11
        xchg eax,ebx
L11:    cmp edx,ecx
        je NoRectangle
        jl L12
        xchg edx,ecx
L12:    cmp ebx,[ClipRegion.Left]
        jl NoRectangle
        cmp ecx,[ClipRegion.Top]
        jl NoRectangle
        cmp eax,[ClipRegion.Right]
        jge NoRectangle
        cmp edx,[ClipRegion.Bottom]
        jge NoRectangle
        cmp eax,[ClipRegion.Left]
        jge L13
        mov eax,[ClipRegion.Left]
L13:    cmp edx,[ClipRegion.Top]
        jge L14
        mov edx,[ClipRegion.Top]
L14:    cmp ebx,[ClipRegion.Right]
        jl L15
        mov ebx,[ClipRegion.Right]
L15:    cmp ecx,[ClipRegion.Bottom]
        jl L22
        mov ecx,[ClipRegion.Bottom]
        jmp L22
NoRectangle:
        ret
_FillRectC_ endp

;zeichnet gefülltes Rechteck
;> EAX = X1, EDX = Y1
;  EBX = X2, ECX = Y2
;< -
_FillRect_ proc
        cmp eax,ebx
        je NoRectangle
        jl L21
        xchg eax,ebx
L21:    cmp edx,ecx
        je NoRectangle
        jl L22
        xchg edx,ecx
L22:    sub ecx,edx
        push esi
        push edi
        push ebp
        mov ebp,ecx             ;Länge Y in EBP
        imul esi,edx,BYTES_PER_LINE
        add esi,[GraphicSegment]
        mov cl,bl
        and cl,7
        mov dx,0ff00h
        ror edx,cl              ;rechte Maske in DL
        mov cl,al
        and cl,7
        mov dh,0ffh
        shr dh,cl               ;linke Maske in DH
        shr eax,3
        shr ebx,3
        sub ebx,eax             ;Länge in EBX
        add esi,eax             ;Offset in ESI
        push edx
        mov dx,GDC
        mov al,8
        out dx,al               ;BitMask Reg.
        inc dl
        pop eax                 ;Masken in AX
        or ebx,ebx
        jz L26
        dec ebx
        jz L28
        xchg al,ah
        even
L23:    mov edi,esi
        mov ecx,ebx             ;Länge in ECX
        push eax
        out dx,al
        inc byte ptr [edi]
        mov al,0ffh
        inc edi
        shr ecx,1
        out dx,al
        rep stosw
        mov al,ah
        jnc L24
        stosb
L24:    out dx,al
        inc byte ptr [edi]
        add esi,BYTES_PER_LINE
        pop eax
        dec ebp
        jnz L23
L25:    pop ebp
        pop edi
        pop esi
        ret
L26:    and al,ah
        out dx,al
L27:    inc byte ptr [esi]
        add esi,BYTES_PER_LINE
        dec ebp
        jnz L27
        jmp L25
L28:    xchg al,ah
        out dx,al
        inc byte ptr [esi]
        xchg al,ah
        out dx,al
        inc byte ptr [esi+1]
        add esi,BYTES_PER_LINE
        dec ebp
        jnz L28
        jmp L25
_FillRect_ endp

        end

