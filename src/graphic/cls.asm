;//
;// cls.asm
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

#include "ega16.inc"

        .data
extrn   GraphicSegment  : DWORD
extrn   Color           : BYTE

        .code
        public _ClearScreen_

;löscht den Bildschirmpuffer
;> -
;< -
_ClearScreen_ proc
        push eax
        push ecx
        push edx
        push edi
        mov dx,GDC
        mov al,0
        mov ah,[Color]
        out dx,al               ;Set/Reset Reg., Farbe setzen
        mov ax,0f01h
        out dx,ax               ;Enable S/R Reg., S/R freigeben
        mov ax,03
        out dx,ax               ;DataRotate Reg., 0 Bits rotieren
        mov ax,05
        out dx,ax               ;Mode Reg., Lesemodus 0, Schreibmodus 0
        mov ax,0ff08h
        out dx,ax               ;BitMask Reg. alle Bits an
        mov edi,[GraphicSegment]
        cld
        mov ecx,GRAPHIC_BUFFER_SIZE/4
        mov eax,-1
        rep stosd
        pop edi
        pop edx
        pop ecx
        pop eax
        ret
_ClearScreen_ endp

        end

