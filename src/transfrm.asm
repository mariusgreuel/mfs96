;//
;// transfrm.asm
;//
;// Copyright Marius Greuel 1996. All rights reserved
;//
;// Punkte mit Transformationsmatrix multiplizieren und projezieren
;//
; X- und Y-Koordinaten werden bei 'TransformPoints' mit 2^16 multipliziert.
; Dies beinhaltet die gewünschte Skalierung von ~1024 (=2^10), so daß noch
; 2^6 als Reserve für den Rückseitentest bleibt. Die verbleibenden 6 Bit
; werden bei 'StorePoint' berücksichtigt.

include object.inc

BACK_PLANE      = 0fffe0000h

        .data?
        extrn Pan : DWORD

        .code
        public TransformPoints_

;Punktliste transformieren und projezieren
;> ebp = Anzahl Punkte
;  esi = Zeiger auf Punktliste
;  edi = Zeiger auf Matrix
;< -
TransformPoints_ proc
        irp Col,<0,4,8>                 ;Alle drei Vektor-Komponenten * Spalte
          irp Row,<0,4,8>
            mov eax,OP.[esi+Row]
            imul dword ptr [edi+Row*3+Col]
            ife Row
              mov ebx,eax
              mov ecx,edx
            else
              add ebx,eax
              adc ecx,edx
            endif
          endm
          shrd ebx,ecx,16               ;Multiplikation anpassen
          add ebx,[edi+12*3+Col]        ;homogenen Anteil addieren
          mov TP.[esi+Col],ebx
        endm
        cmp ebx,BACK_PLANE              ;wenn Punkt hinter Sichtebene, keine Projektion
        jg NoTransformation
        mov eax,TP.X.[esi]              ;TP.X / TP.Z + Pan.X
        cdq
        shld edx,eax,16
        shl eax,16
        idiv ebx
        add eax,LX.[Pan]
        mov PP.LX.[esi],eax
        mov eax,TP.Y.[esi]              ;TP.Y / TP.Z + Pan.Y
        cdq
        shld edx,eax,16
        shl eax,16
        idiv ebx
        add eax,LY.[Pan]
        mov PP.LY.[esi],eax
NoTransformation:
        add esi,size _POINT3D
        dec ebp
        jnz TransformPoints_
        ret
TransformPoints_ endp

        end

