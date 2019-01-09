;//
;// clip.asm
;//
;// Copyright Marius Greuel 1996. All rights reserved
;//
;// Clipping Algorithmus nach Cohen-Sutherland
;// rekursiv implementiert
;//

include object.inc

CLIP_LEFT       equ 10h
CLIP_RIGHT      equ 20h
CLIP_TOP        equ 40h
CLIP_BOTTOM     equ 80h

        .data?
        extrn PointBuffer : WORD
        extrn Window      : DWORD

PointsInBuffer  dd ?
FirstLeft       _LPOINT<>
FirstRight      _LPOINT<>
FirstBottom     _LPOINT<>
FirstTop        _LPOINT<>
PosLeft         _LPOINT<>
PosRight        _LPOINT<>
PosBottom       _LPOINT<>
PosTop          _LPOINT<>
ClipFlag        db 0

        .code
        public StorePoints

;> ebx = &Polygon
;< ecx = Anzahl Punkte im Puffer
StorePoints proc
        mov esi,PG_Point.[ebx]
        mov eax,PP.LX.[esi]
        mov [PosLeft.LX],eax
        mov [PosRight.LX],eax
        mov [PosBottom.LX],eax
        mov [PosTop.LX],eax
        mov eax,PP.LY.[esi]
        mov [PosLeft.LY],eax
        mov [PosRight.LY],eax
        mov [PosBottom.LY],eax
        mov [PosTop.LY],eax
        mov [ClipFlag],0
        mov [PointsInBuffer],0
        mov esi,PG_Points.[ebx]
        mov edi,offset PointBuffer
S1:     mov eax,PG_Point.[ebx+esi*4-4]
        mov ecx,PP.LX.[eax]
        mov edx,PP.LY.[eax]
        call ClipLeft
        dec esi
        jnz S1
        mov ecx,[PointsInBuffer]
        ret
StorePoints endp

;Punkt in Puffer eintragen
;> dx = X-Koordinate
;  cx = Y-Koordinate
StorePoint proc
        test [ClipFlag],07h
        jnz IsMove
        shr ecx,6
        shr edx,6
        mov SX.[edi],dx         ;Punkt zu Line speichern
        mov SY.[edi],cx
        add edi,4
        inc [PointsInBuffer]
IsMove: ret
StorePoint endp

;links clippen
;> ecx = X, edx = Y
;< ecx = X, edx = Y
ClipLeft proc
        mov eax,[Window.LLeft]
        cmp LX.[PosLeft],eax
        jl CLL1
        cmp ecx,eax
        jl CLL3
        mov LX.[PosLeft],ecx     ;beide innen
        mov LY.[PosLeft],edx
        jmp ClipRight
CLL1:   cmp ecx,eax
        jl CLL5
        sub eax,ecx               ;von außen nach innen
        xchg edx,LY.[PosLeft]
        sub edx,LY.[PosLeft]     ;dy
        xchg ecx,LX.[PosLeft]
        sub ecx,LX.[PosLeft]     ;edx
        imul edx
        idiv ecx
        add eax,LY.[PosLeft]
        mov ecx,[Window.LLeft]
        mov edx,eax
        test [ClipFlag],CLIP_LEFT
        jnz CLL2
        or [ClipFlag],CLIP_LEFT
        mov LX.[FirstLeft],ecx   ;ersten Schnittpunkt speichern
        mov LY.[FirstLeft],edx
        inc [ClipFlag]
        call ClipRight
        dec [ClipFlag]
        mov ecx,LX.[PosLeft]
        mov edx,LY.[PosLeft]
        jmp ClipRight
CLL2:   call ClipRight          ;Polygon schließen
        mov ecx,LX.[PosLeft]
        mov edx,LY.[PosLeft]
        jmp ClipRight
CLL3:   sub eax,ecx             ;von innen nach außen
        xchg edx,LY.[PosLeft]
        sub edx,LY.[PosLeft]    ;dy
        xchg ecx,LX.[PosLeft]
        sub ecx,LX.[PosLeft]    ;edx
        imul edx
        idiv ecx
        add eax,LY.[PosLeft]
        mov ecx,[Window.LLeft]
        mov edx,eax
        test [ClipFlag],CLIP_LEFT
        jnz CLL4
        or [ClipFlag],CLIP_LEFT ;erster Schnittpunkt
        jmp ClipRight
CLL4:   call ClipRight
        mov ecx,LX.[FirstLeft]  ;Polygon schließen
        mov edx,LY.[FirstLeft]
        jmp ClipRight
CLL5:   mov LX.[PosLeft],ecx    ;beide außen
        mov LY.[PosLeft],edx
        ret
ClipLeft endp

;rechts clippen
;> ecx = X, edx = Y
;< ecx = X, edx = Y
ClipRight proc
        mov eax,[Window.LRight]
        cmp LX.[PosRight],eax
        jg CLR1
        cmp ecx,eax
        jg CLR3
        mov LX.[PosRight],ecx   ;beide innen
        mov LY.[PosRight],edx
        xchg ecx,edx
        jmp ClipTop
CLR1:   cmp ecx,eax
        jg CLR5
        sub eax,ecx             ;von außen nach innen
        xchg edx,LY.[PosRight]
        sub edx,LY.[PosRight]   ;dy
        xchg ecx,LX.[PosRight]
        sub ecx,LX.[PosRight]   ;dx
        imul edx
        idiv ecx
        add eax,LY.[PosRight]
        mov edx,[Window.LRight]
        mov ecx,eax
        test [ClipFlag],CLIP_RIGHT
        jnz CLR2
        or [ClipFlag],CLIP_RIGHT
        mov LX.[FirstRight],edx ;ersten Schnittpunkt speichern
        mov LY.[FirstRight],ecx
        inc [ClipFlag]
        call ClipTop
        dec [ClipFlag]
        mov edx,LX.[PosRight]
        mov ecx,LY.[PosRight]
        jmp ClipTop
CLR2:   call ClipTop            ;Polygon schließen
        mov edx,LX.[PosRight]
        mov ecx,LY.[PosRight]
        jmp ClipTop
CLR3:   sub eax,ecx             ;von innen nach außen
        xchg edx,LY.[PosRight]
        sub edx,LY.[PosRight]   ;dy
        xchg ecx,LX.[PosRight]
        sub ecx,LX.[PosRight]   ;edx
        imul edx
        idiv ecx
        add eax,LY.[PosRight]
        mov edx,[Window.LRight]
        mov ecx,eax
        test [ClipFlag],CLIP_RIGHT
        jnz CLR4
        or [ClipFlag],CLIP_RIGHT ;erster Schnittpunkt
        jmp ClipTop
CLR4:   call ClipTop
        mov edx,LX.[FirstRight]  ;Polygon schließen
        mov ecx,LY.[FirstRight]
        jmp ClipTop
CLR5:   mov LX.[PosRight],ecx    ;beide außen
        mov LY.[PosRight],edx
        ret
ClipRight endp

;oben clippen
;> edx = X, ecx = Y
;< edx = X, ecx = Y
ClipTop proc
        mov eax,[Window.LTop]
        cmp LY.[PosTop],eax
        jl CLT1
        cmp ecx,eax
        jl CLT4
        mov LX.[PosTop],edx      ;beide innen
        mov LY.[PosTop],ecx
        jmp ClipBottom
CLT1:   cmp ecx,eax
        jl CLT6
        sub eax,ecx              ;von außen nach innen
        xchg edx,LX.[PosTop]
        sub edx,LX.[PosTop]      ;edx
        xchg ecx,LY.[PosTop]
        sub ecx,LY.[PosTop]      ;dy
        imul edx
        idiv ecx
        add eax,LX.[PosTop]
        mov ecx,[Window.LTop]
        mov edx,eax
        test [ClipFlag],CLIP_TOP
        jnz CLT3
        test [ClipFlag],0fh
        jnz CLT2
        or [ClipFlag],CLIP_TOP
        mov LX.[FirstTop],edx    ;ersten Schnittpunkt speichern
        mov LY.[FirstTop],ecx
CLT2:   inc [ClipFlag]
        call ClipBottom
        dec [ClipFlag]
        mov edx,LX.[PosTop]
        mov ecx,LY.[PosTop]
        jmp ClipBottom
CLT3:   call ClipBottom         ;Polygon schließen
        mov edx,LX.[PosTop]
        mov ecx,LY.[PosTop]
        jmp ClipBottom
CLT4:   sub eax,ecx             ;von innen nach außen
        xchg edx,LX.[PosTop]
        sub edx,LX.[PosTop]     ;edx
        xchg ecx,LY.[PosTop]
        sub ecx,LY.[PosTop]     ;dy
        imul edx
        idiv ecx
        add eax,LX.[PosTop]
        mov ecx,[Window.LTop]
        mov edx,eax
        test [ClipFlag],CLIP_TOP
        jnz CLT5
        test [ClipFlag],0fh
        jnz CLT5
        or [ClipFlag],CLIP_TOP  ;erster Schnittpunkt
        jmp ClipBottom
CLT5:   call ClipBottom
        mov edx,LX.[FirstTop]   ;Polygon schließen
        mov ecx,LY.[FirstTop]
        jmp ClipBottom
CLT6:   mov LX.[PosTop],edx     ;beide außen
        mov LY.[PosTop],ecx
        ret
ClipTop endp

;unten clippen
;> edx = X, ecx = Y
;< edx = X, ecx = Y
ClipBottom proc
        mov eax,[Window.LBottom]
        cmp LY.[PosBottom],eax
        jg CLB1
        cmp ecx,eax
        jg CLB4
        mov LX.[PosBottom],edx   ;beide innen
        mov LY.[PosBottom],ecx
        jmp StorePoint
CLB1:   cmp ecx,eax
        jg CLB6
        sub eax,ecx              ;von außen nach innen
        xchg edx,LX.[PosBottom]
        sub edx,LX.[PosBottom]   ;edx
        xchg ecx,LY.[PosBottom]
        sub ecx,LY.[PosBottom]   ;dy
        imul edx
        idiv ecx
        add eax,LX.[PosBottom]
        mov ecx,[Window.LBottom]
        mov edx,eax
        test [ClipFlag],CLIP_BOTTOM
        jnz CLB3
        test [ClipFlag],0fh
        jnz CLB2
        or [ClipFlag],CLIP_BOTTOM
        mov LX.[FirstBottom],edx ;ersten Schnittpunkt speichern
        mov LY.[FirstBottom],ecx
CLB2:   inc [ClipFlag]
        call StorePoint
        dec [ClipFlag]
        mov edx,LX.[PosBottom]
        mov ecx,LY.[PosBottom]
        jmp StorePoint
CLB3:   call StorePoint         ;Polygon schließen
        mov edx,LX.[PosBottom]
        mov ecx,LY.[PosBottom]
        jmp StorePoint
CLB4:   sub eax,ecx             ;von innen nach außen
        xchg edx,LX.[PosBottom]
        sub edx,LX.[PosBottom]  ;edx
        xchg ecx,LY.[PosBottom]
        sub ecx,LY.[PosBottom]  ;dy
        imul edx
        idiv ecx
        add eax,LX.[PosBottom]
        mov ecx,[Window.LBottom]
        mov edx,eax
        test [ClipFlag],CLIP_BOTTOM
        jnz CLB5
        test [ClipFlag],0fh
        jnz CLB5
        or [ClipFlag],CLIP_BOTTOM ;erster Schnittpunkt
        jmp StorePoint
CLB5:   call StorePoint
        mov edx,LX.[FirstBottom] ;Polygon schließen
        mov ecx,LY.[FirstBottom]
        jmp StorePoint
CLB6:   mov LX.[PosBottom],edx   ;beide außen
        mov LY.[PosBottom],ecx
        ret
ClipBottom endp

        end

