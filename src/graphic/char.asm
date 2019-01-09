;//
;// char.asm
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

#include "ega16.inc"

        .data
        extrn GraphicSegment : DWORD
        extrn ClipRegion : DWORD
FontPointer     dd 0ffa6eh

        .code
        public _DrawChar_
        public _DrawCharC_

_ret:   ret

;gibt Zeichen aus (mit Clipping)
;> EAX = Zeichen
;  EDX = X, EBX = Y
;< -
_DrawCharC_ proc
        cmp edx,[ClipRegion.Left]
        jl _ret
        cmp ebx,[ClipRegion.Top]
        jl _ret
        add edx,8
        cmp edx,[ClipRegion.Right]
        jge _ret
        add ebx,8
        cmp ebx,[ClipRegion.Bottom]
        jge _ret
        sub edx,8
        sub ebx,8
;;      jmp _DrawChar_
_DrawCharC_ endp

;gibt Zeichen aus
;> EAX = Zeichen
;  EDX = X, EBX = Y
;< -
_DrawChar_ proc
        push ecx
        push esi
        push edi
        mov esi,[FontPointer]
        movzx eax,al
        shl eax,3
        add esi,eax
        mov cl,dl
        and cl,7
        imul edi,ebx,BYTES_PER_LINE
        shr edx,3
        add edi,edx
        add edi,[GraphicSegment]
        mov ch,FONT_Y_RES
        mov dx,GDC
L11:    mov ah,[esi]
        inc esi
        mov al,0
        ror ax,cl
        mov bl,al
        mov al,8
        out dx,ax
        or [edi],ah
        mov ah,bl
        out dx,ax
        or [edi+1],ah
        add edi,BYTES_PER_LINE
        dec ch
        jnz L11
Finish: pop edi
        pop esi
        pop ecx
        ret
_DrawChar_ endp

        end


