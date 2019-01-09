;//
;// mode.asm
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

#include "bios.inc"
#include "ega16.inc"

        .data
        extrn State : BYTE

        .code
        public _DetectGraph_
        public _SetTextMode_
        public _SetGraphicMode_

        extrn _SetActivePage_   : proc
        extrn _SetVisiblePage_  : proc
        extrn _SetColor_        : proc

;Test, ob Grafikkarte (EGA, 256KByte) vorhanden
;> -
;< AX == 0, wenn keine entsprechende Grafikkarte
_DetectGraph_ proc
        push ebx
        mov ah,12h
        mov bl,10h
        int VIDEO_SERVICE
        xor eax,eax
        cmp bl,10h      ;EGA da ?
        jz NoEga
        cmp bl,3        ;256 Kbyte ?
        setnc al
NoEGA:  pop ebx
        ret
_DetectGraph_ endp

;setzt Textmodus
;> -
;< -
_SetTextMode_ proc
        push eax
        test [State],TEXT_MODE_ON
        jnz TextModeAlreadyOn
        mov eax,TEXT_MODE
        int VIDEO_SERVICE
        and [State],not GRAPHIC_MODE_ON
        or [State],TEXT_MODE_ON
TextModeAlreadyOn:
        pop eax
        ret
_SetTextMode_ endp

;setzt Grafikmodus
;> -
;< -
_SetGraphicMode_ proc
        push eax
        test [State],GRAPHIC_MODE_ON
        jnz GraphicModeAlreadyOn
        mov ax,GRAPHIC_MODE
        int VIDEO_SERVICE
        mov al,0
        call _SetActivePage_
        mov al,0
        call _SetVisiblePage_
        mov al,0
        call _SetColor_
        and [State],not TEXT_MODE_ON
        or [State],GRAPHIC_MODE_ON
GraphicModeAlreadyOn:
        pop eax
        ret
_SetGraphicMode_ endp

        end

