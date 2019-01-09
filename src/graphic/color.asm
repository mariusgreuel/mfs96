;//
;// color.asm
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

#include "bios.inc"
#include "ega16.inc"

        .data?
        public Color

Color           db ?

        .code
        public _SetColor_
        public _SetColorReg_

;setzt Farbe
;> AL = Farbe
;< -
_SetColor_ proc
        push ebx
        push edx
        mov [Color],al
        mov bl,0                ;Bits normal bearbeiten
        cmp al,-2
        jnz NormalColor
        mov al,07h
        mov bl,8                ;Bits maskieren (AND Latch)
NormalColor:
        mov dx,GDC
        mov ah,al
        mov al,0                ;Set/Reset Reg. Farbe setzen
        out dx,ax
        mov ah,bl
        mov al,3                ;Data Rotate Reg.
        out dx,ax
        pop edx
        pop ebx
        ret
_SetColor_ endp

;setzt ein Farbregister neu
;> AL = Register (0-15)
;  EDX = 0, Red, Green, Blue 
;< -
_SetColorReg_ proc
        push ebx
        push ecx
        movzx ebx,al
        mov ah,INT10_SET_PALETTE_REGS
        mov al,7                        ;ReadPalRegister
        int VIDEO_SERVICE
        mov ah,INT10_SET_PALETTE_REGS
        mov al,10h                      ;SetColorRegister
        mov bl,bh
        mov cl,dl
        shr edx,8
        mov ch,dl
        int VIDEO_SERVICE
        pop ecx
        pop ebx
        ret
_SetColorReg_ endp

        end

