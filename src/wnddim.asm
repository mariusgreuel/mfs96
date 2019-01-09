;//
;// clipwnd.asm
;//
;// Copyright Marius Greuel 1996. All rights reserved
;//

include object.inc

        .data?
        public Window
        public Pan

Window          _LRECT<>
Pan             _LPOINT<>

        .code
        public _SetWindowDimension_

;Fensterdimensionen setzen
;> eax = &WindowRect
;< -
_SetWindowDimension_ proc
        push ebx
        push edx
        movzx ebx,[eax].SLeft
        movzx edx,[eax].SRight
        shl ebx,6
        shl edx,6
        dec edx
        mov [Window].LLeft,ebx
        mov [Window].LRight,edx   ;Fensterkoordinaten speichern
        sub edx,ebx
        shr edx,1
        add ebx,edx
        mov [Pan].LX,ebx         ;und Offset bis Mitte Fenster berechnen
        movzx ebx,[eax].STop
        movzx edx,[eax].SBottom
        shl ebx,6
        shl edx,6
        dec edx
        mov [Window].LTop,ebx
        mov [Window].LBottom,edx
        sub edx,ebx
        shr edx,1
        add ebx,edx
        mov [Pan].LY,ebx
        pop edx
        pop ebx
        ret
_SetWindowDimension_ endp

        end


