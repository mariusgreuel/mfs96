;//
;// savescr.asm
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

#include "bios.inc"
#include "ega16.inc"

        .data
        extrn State : BYTE
TextSegment             dd TEXT_SEGMENT
TextBuffer              dd ?
OldVideoMode            db ?
OldCursorPos            dw ?

        .code
        public _SaveScreen_
        public _RestoreScreen_

        extrn malloc_           : proc
        extrn free_             : proc

;speichert alten Videomodus
;> -
;< -
_SaveScreen_ proc
        push eax
        push ebx
        push ecx
        push edx
        push esi
        push edi
        test [State],VIDEO_MODE_SAVED 
        jnz DontSaveScreen
        mov ah,INT10_GET_MODE
        int VIDEO_SERVICE
        mov [OldVideoMode],al
        or [State],VIDEO_MODE_SAVED
        cmp al,3
        ja DontSaveScreen
        mov eax,TEXT_BUFFER_SIZE+4
        call malloc_
        or eax,eax
        jz DontSaveScreen
        mov [TextBuffer],eax
        mov esi,[TextSegment]
        mov edi,[TextBuffer]
        mov ecx,TEXT_BUFFER_SIZE/4
        cld
        rep movsd
        mov ah,INT10_READ_CURSOR
        mov bh,0
        int VIDEO_SERVICE
        mov [OldCursorPos],dx
        or [State],SCREEN_SAVED
DontSaveScreen:
        pop edi
        pop esi
        pop edx
        pop ecx
        pop ebx
        pop eax
        ret
_SaveScreen_ endp

;restauriert alten Videomodus
;> -
;< -
_RestoreScreen_ proc
        push eax
        push ebx
        push ecx
        push edx
        push esi
        push edi
        test [State],VIDEO_MODE_SAVED
        jz DontRestoreScreen
        xor eax,eax
        mov al,[OldVideoMode]
        mov ah,INT10_SET_MODE
        int VIDEO_SERVICE
        and [State],not VIDEO_MODE_SAVED
        test [State],SCREEN_SAVED
        jz DontRestoreScreen
        mov esi,[TextBuffer]
        mov edi,[TextSegment]
        mov ecx,TEXT_BUFFER_SIZE/4
        cld
        rep movsd
        mov ah,INT10_SET_CURSOR_POS
        mov bh,0
        movzx edx,[OldCursorPos]
        int VIDEO_SERVICE
        mov eax,[TextBuffer]
        call free_
        and [State],not SCREEN_SAVED
DontRestoreScreen:
        pop edi
        pop esi
        pop edx
        pop ecx
        pop ebx
        pop eax
        ret
_RestoreScreen_ endp

        end


