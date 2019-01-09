;//
;// page.asm
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

#include "ega16.inc"

        .data
        public State
State   db 0

        extrn GraphicSegment : DWORD

        .code
        public _SetActivePage_
        public _GetActivePage_
        public _SetVisiblePage_
        public _GetVisiblePage_

;holt die Videoseite, auf die geschrieben wird
;> -
;< AL = Seite ( 0 / 1 )
_GetActivePage_ proc
        test [State],ND_PAGE_ACTIVE
        setne al
        ret
_GetActivePage_ endp

;setzt die Videoseite, auf die geschrieben wird
;> AL = Seite ( 0 / 1 )
;< AL = Seite ( 0 / 1 ) die aktiv war
_SetActivePage_ proc
        mov ah,[State]
        and [GraphicSegment],not 08000h
        and [State],not ND_PAGE_ACTIVE
        or al,al
        jz SetFirstPage
        or [GraphicSegment],08000h
        or [State],ND_PAGE_ACTIVE
SetFirstPage:
        test ah,ND_PAGE_ACTIVE
        setne al
        ret
_SetActivePage_ endp

;holt die Videoseite, die sichtbar ist
;> -
;< AL = Seite ( 0 / 1 )
_GetVisiblePage_ proc
        test [State],ND_PAGE_VISIBLE
        setne al
        ret
_GetVisiblePage_ endp

;setzt die Videoseite, die sichtbar ist
;> AL = Seite ( 0 / 1 )
;< -
_SetVisiblePage_ proc
        push ebx
        push edx
        mov bl,al
        mov bh,[State]
        mov ax,0ch
        and [State],not ND_PAGE_VISIBLE
        or bl,bl
        jz ShowFirstPage
        or ah,80h
        or [State],ND_PAGE_VISIBLE
ShowFirstPage:
        mov dx,CRTC
        out dx,ax
        test bh,ND_PAGE_VISIBLE
        setne al
        pop edx
        pop ebx
        ret
_SetVisiblePage_ endp

        end


