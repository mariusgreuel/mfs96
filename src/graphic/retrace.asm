;//
;// retrace.asm
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

#include "bios.inc"
#include "ega16.inc"

        .data
        public GraphicSegment

GraphicSegment  dd GRAPHIC_SEGMENT

        .code
        public _WaitForRetrace_

;wartet auf Strahlenrücklauf
;> -
;< -
_WaitForRetrace_ proc
        push eax
        push edx
        mov edx,STATUS6845
        mov ah,8                ;Vertical Retrace
RetraceHigh:
        in al,dx
        test al,ah
        jnz RetraceHigh
RetraceLow:
        in al,dx
        test al,ah
        jz RetraceLow
        pop edx
        pop eax
        ret
_WaitForRetrace_ endp

        end
