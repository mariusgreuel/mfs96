;//
;// dispobj.asm
;//
;// Copyright Marius Greuel 1996. All rights reserved
;//

include object.inc

        .code
        public _DisplayObject_

        extrn TransformPoints_ : proc
        extrn DisplayPolygon_  : proc

;Objekt darstellen
;> eax = &Objekt
;  edx = &Matrix
;< -
_DisplayObject_ proc
        pushad
        push eax
        mov ebp,OBJ_Points.[eax]
        mov esi,OBJ_Point.[eax]
        mov edi,edx
        call TransformPoints_
        pop eax
        mov ebx,OBJ_BspRoot.[eax]
        call DisplayPolygon_
        popad
        ret
_DisplayObject_ endp

        end


