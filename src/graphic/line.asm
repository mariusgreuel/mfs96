;//
;// line.asm
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

#include "ega16.inc"

        .data
        extrn GraphicSegment : DWORD

        .code
        extrn _DrawLineH_ : proc
        extrn _DrawLineV_ : proc

        public _DrawLine_

LineIsVertical:
        mov ebx,ecx
        jmp _DrawLineV_
LineIsHorizontal:
        jmp _DrawLineH_

;zeichnet Linie
;> EAX = X1, EDX = Y1
;  EBX = X2, ECX = Y2
;< -
_DrawLine_ proc
        cmp eax,ebx
        jz LineIsVertical
        jl L1
        xchg eax,ebx
        xchg edx,ecx
L1:     sub ecx,edx
        jz LineIsHorizontal
        push esi
        push edi
        push ebp
        sub ebx,eax
        mov esi,ebx
        mov edi,ecx
        imul ebx,edx,BYTES_PER_LINE
        mov cl,al
        shr eax,3
        add ebx,eax
        add ebx,[GraphicSegment] ;EBX = Offset
        mov al,8
        mov dx,GDC
        out dx,al
        inc dl                  ;DX = GDC BitMask Daten
        mov eax,8080h
        and cl,7
        ror eax,cl              ;AX = Maske
        or edi,edi
        js L30
L10:    cmp esi,edi
        jb L20
        mov ecx,esi
        mov ebp,ecx
        shr ebp,1
        neg edi
L11:    or al,ah
L12:    add ebp,edi
        jns L13
        add ebp,esi
        out dx,al
        inc byte ptr [ebx]
        ror ah,1
        adc ebx,BYTES_PER_LINE
        mov al,ah
        dec ecx
        jnz L12
        pop ebp
        pop edi
        pop esi
        ret
        even
L13:    ror ah,1
        jnb L14
        out dx,al
        inc byte ptr [ebx]
        inc ebx
        mov al,ah
        dec ecx
        jnz L12
        pop ebp
        pop edi
        pop esi
        ret
        even
L14:    dec ecx
        jnz L11
        out dx,al
        inc byte ptr [ebx]
        pop ebp
        pop edi
        pop esi
        ret
        even
L20:    mov ecx,edi
        mov ebp,ecx
        inc ecx
        shr ebp,1
        neg esi
        jmp L22
        even
L21:    add ebp,esi
        jns L23
        add ebp,edi
        ror al,1
        adc ebx,BYTES_PER_LINE
L22:    out dx,al
        inc byte ptr [ebx]
        dec ecx
        jnz L21
        pop ebp
        pop edi
        pop esi
        ret
        even
L23:    add ebx,BYTES_PER_LINE
        inc byte ptr [ebx]
        dec ecx
        jnz L21
        pop ebp
        pop edi
        pop esi
        ret
        even
L30:    neg edi
        cmp esi,edi
        jb L40
        mov ecx,esi
        mov ebp,ecx
        inc ecx
        shr ebp,1
        neg edi
L31:    or al,ah
L32:    add ebp,edi
        jns L33
        add ebp,esi
        out dx,al
        inc byte ptr [ebx]
        ror ah,1
        adc ebx,-BYTES_PER_LINE
        mov al,ah
        dec ecx
        jnz L32
        pop ebp
        pop edi
        pop esi
        ret
        even
L33:    ror ah,1
        jnb L34
        out dx,al
        inc byte ptr [ebx]
        inc ebx
        mov al,ah
        dec ecx
        jnz L32
        pop ebp
        pop edi
        pop esi
        ret
        even
L34:    dec ecx
        jnz L31
        out dx,al
        inc byte ptr [ebx]
        pop ebp
        pop edi
        pop esi
        ret
        even
L40:    mov ecx,edi
        mov ebp,ecx
        inc ecx
        shr ebp,1
        neg esi
        jmp L42
        even
L41:    add ebp,esi
        jns L43
        add ebp,edi
        ror al,1
        adc ebx,-BYTES_PER_LINE
L42:    out dx,al
        inc byte ptr [ebx]
        dec ecx
        jnz L41
        pop ebp
        pop edi
        pop esi
        ret
        even
L43:    add ebx,-BYTES_PER_LINE
        inc byte ptr [ebx]
        dec ecx
        jnz L41
        pop ebp
        pop edi
        pop esi
        ret
_DrawLine_ endp

        end

