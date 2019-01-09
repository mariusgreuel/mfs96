;//
;// disppoly.asm
;//
;// Copyright Marius Greuel 1996. All rights reserved
;//

include object.inc

        .data?
        public PointBuffer

PointBuffer     dd 32 dup(?)            ;32 Punkte maximal

        .code
        public DisplayPolygon_

        extrn StorePoints    : proc
        extrn _SetColor_     : proc
        extrn _DrawPolygon_  : proc
        extrn _FillPolygon_  : proc
        extrn _FillPolygonT_ : proc

;Polygon zeichen
;> ebx = &Polygon
;< -
DisplayPolygon_ proc
        or ebx,ebx
        jz _ret
        push ebx                        ;Rückseitentest
        mov edi,PG_Point.[ebx+8]        ;Punkt 3
        mov esi,PG_Point.[ebx+4]        ;Punkt 2
        mov ebx,PG_Point.[ebx]          ;Punkt 1
        mov eax,PP.LX.[esi]
        sub eax,PP.LX.[ebx]
        mov edx,PP.LY.[edi]
        sub edx,PP.LY.[ebx]
        imul edx                ;(p2.x-p1.x)*(p3.y-p1.y)
        mov ecx,eax
        mov ebp,edx
        mov eax,PP.LY.[esi]
        sub eax,PP.LY.[ebx]
        mov edx,PP.LX.[edi]
        sub edx,PP.LX.[ebx]
        imul edx                ;(p2.y-p1.y)*(p3.x-p1.x)
        pop ebx                 ;Polygon zeigt Vorderseite ?
        sub ecx,eax
        sbb ebp,edx
        js IsBackFace
        push ebx
        mov ebx,PG_Behind.[ebx]
        call DisplayPolygon_    ;alle Polygone hinter diesem zeichnen
        pop ebx
        cmp PG_Color.FGF.[ebx],COLOR_NULL
        jnz L2
        cmp PG_Color.FGD.[ebx],COLOR_NULL
        jnz L2
L1:     mov ebx,PG_Before.[ebx]
        jmp DisplayPolygon_     ;und alle Polygone vor diesem zeichnen
_ret:   ret
L2:     call StorePoints        ;Punkte speichern
        cmp ecx,3               ;Polygon muß >=3 Punkte haben
        jb L1
        mov al,PG_Color.FGF.[ebx]
        cmp al,COLOR_NULL
        jz L3
        cmp al,COLOR_SHADOW
        jz L4
        call _SetColor_
        mov eax,ecx
        mov edx,offset PointBuffer
        call _FillPolygon_
L3:     mov al,PG_Color.FGD.[ebx]
        cmp al,COLOR_NULL
        jz L1
        call _SetColor_
        mov eax,ecx
        mov edx,offset PointBuffer
        call _DrawPolygon_
        jmp L1
L4:     call _SetColor_
        mov eax,ecx
        mov edx,offset PointBuffer
        call _FillPolygonT_
        jmp L3
IsBackFace:
        push ebx
        mov ebx,PG_Before.[ebx]
        call DisplayPolygon_    ;alle Polygone vor diesem zeichnen
        pop ebx
        cmp PG_Color.BGF.[ebx],COLOR_NULL
        jnz L6
        cmp PG_Color.BGD.[ebx],COLOR_NULL
        jnz L6
L5:     mov ebx,PG_Behind.[ebx]
        jmp DisplayPolygon_     ;und alle Polygone hinter diesem zeichnen
L6:     call StorePoints        ;Punkte speichern
        cmp ecx,3               ;Polygon muß >=3 Punkte haben
        jb L5
        mov al,PG_Color.BGF.[ebx]
        cmp al,COLOR_NULL
        jz L7
        cmp al,COLOR_SHADOW
        jz L8
        call _SetColor_
        mov eax,ecx
        mov edx,offset PointBuffer
        call _FillPolygon_
L7:     mov al,PG_Color.BGD.[ebx]
        cmp al,COLOR_NULL
        jz L5
        call _SetColor_
        mov eax,ecx
        mov edx,offset PointBuffer
        call _DrawPolygon_
        jmp L5
L8:     call _SetColor_
        mov eax,ecx
        mov edx,offset PointBuffer
        call _FillPolygonT_
        jmp L7
DisplayPolygon_ endp

        end

