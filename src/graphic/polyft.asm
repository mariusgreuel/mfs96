;//
;// polyf.asm, fill polygon with transparent color
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

#include "ega16.inc"

        .data
        extrn   GraphicSegment  : DWORD

        .data
FirstPoint              dd ?
LastPoint               dd ?
LeftPoint               dd ?
RightPoint              dd ?
LeftPos                 dd ?
RightPos                dd ?
LeftStep                dd ?
RightStep               dd ?
LeftLen                 dw ?
RightLen                dw ?
RRR                     db ?
Len                     dd ?
Plane                   db ?

        .code
        public _FillPolygonT_

_DrawRow proc
        push edi
        shr ebx,16
        shr edx,16
        cmp ebx,edx
        jbe NoXChange
        xchg ebx,edx 
NoXChange:
        inc edx
        mov cl,dl
        and cl,7
        mov ax,0ff00h
        ror eax,cl              ;Maske in AL
        mov cl,bl
        and cl,7
        mov ah,0ffh
        shr ah,cl               ;Maske in AH
        shr ebx,3
        shr edx,3
        add edi,ebx             ;Offset in EDI
        mov ecx,edx
        sub ecx,ebx             ;Länge in ECX
        mov dx,GDC
        jnz Next
        and ah,al
        mov al,8
        out dx,ax
        inc byte ptr [edi]
        jmp Finish
Next:mov bl,al
        mov al,8
        out dx,ax
        inc byte ptr [edi]
        inc edi
        dec ecx
        jz NoMore
        mov ax,0ff08h
        out dx,ax
comment $
        mov al,ah
        shr ecx,1
        rep stosw
        jnc NoMore
        stosb
$
qqq:    inc byte ptr [edi]
        inc edi
        dec ecx
        jnz qqq
        
NoMore:
        mov al,8
        mov ah,bl
        out dx,ax
        inc byte ptr [edi]
Finish:
        pop edi
        ret
_DrawRow endp

GetNextLeftPoint proc
        mov ebx,[LeftPoint]
        cmp ebx,[RightPoint]
        jz RDone
GetFirstLeftPoint:
        mov ax,[ebx].X
        mov cx,[ebx].Y
        add ebx,-4
        cmp ebx,[FirstPoint]
        jnb NoLeftWrap
        mov ebx,[LastPoint]
NoLeftWrap:
        mov [LeftPoint],ebx
        mov edx,eax
        sub ax,[ebx].X
        sub cx,[ebx].Y
        jz LeftLenIsZero
        shl edx,16
        mov [LeftPos],edx
        shl eax,16
        movsx ecx,cx
        cdq
        idiv ecx
        mov [LeftStep],eax
        neg cx
        mov [LeftLen],cx
LDone:  ret
LeftLenIsZero:
        mov bx,[ebx].X
        shl ebx,16
        mov [LeftPos],ebx
        mov edx,[RightPos]
        call _DrawRow
        jmp GetNextLeftPoint
GetNextLeftPoint endp

GetNextRightPoint proc
        mov ebx,[RightPoint]
        cmp ebx,[LeftPoint]
        jz RDone
GetFirstRightPoint:
        mov ax,[ebx].X
        mov cx,[ebx].Y
        add bx,4
        cmp ebx,[LastPoint]
        jna NoRightWrap
        mov ebx,[FirstPoint]
NoRightWrap:
        mov [RightPoint],ebx
        mov edx,eax
        sub ax,[ebx].X
        sub cx,[ebx].Y
        jz RightLenIsZero
        shl edx,16
        mov [RightPos],edx
        shl eax,16
        movsx ecx,cx
        cdq
        idiv ecx
        mov [RightStep],eax
        neg cx
        mov [RightLen],cx
RDone:  ret
RightLenIsZero:
        mov dx,[ebx].X
        shl edx,16
        mov [RightPos],edx
        mov ebx,[LeftPos]
        call _DrawRow
        jmp GetNextRightPoint
GetNextRightPoint endp

GetMinMaxPointer macro
        mov [FirstPoint],edx    ;Zeiger auf ersten Punkt
        dec eax
        shl eax,2        
        add eax,edx
        mov [LastPoint],eax     ;Zeiger auf letzten Punkt
        endm

SearchHighestPoint macro
        mov cx,-1               ;kleinsten Y-Wert suchen -> CX
SHP_Loop:
        cmp [edx].Y,cx
        jnb NotHigher
        mov cx,[edx].Y          ;neuen Punkt gefunden
        mov ebx,edx
NotHigher:
        add edx,4
        cmp edx,eax
        jbe SHP_Loop
        mov [LeftPoint],ebx
        mov [RightPoint],ebx
        mov ax,[ebx].X
        shl eax,16
        mov [LeftPos],eax
        mov [RightPos],eax
        endm

;zeichnet ein Polygon
;> EAX = Anzahl Punkte
;  EDX = *Punkte
;< -
_FillPolygonT_ proc
        push ebx
        push ecx
        push esi
        push edi
        push ebp
        GetMinMaxPointer
        SearchHighestPoint
        imul di,cx,BYTES_PER_LINE
        movzx edi,di
        add edi,[GraphicSegment]
        mov ebx,[LeftPoint]
        call GetFirstLeftPoint
        jz Raus
        mov ebx,[RightPoint]
        call GetFirstRightPoint
        jz Raus

        mov ebx,[LeftPos]
        mov edx,[RightPos]
        call _DrawRow
FillLoop:
ContinueRight:
        add di,BYTES_PER_LINE
        mov eax,[LeftStep]
        add [LeftPos],eax
        mov eax,[RightStep]
        add [RightPos],eax
        mov ebx,[LeftPos]
        mov edx,[RightPos]
        call _DrawRow
        dec [LeftLen]
        jz LeftLenZero
ContinueLeft:
        dec [RightLen]
        jnz FillLoop
RightLenZero:
        call GetNextRightPoint
        jnz ContinueRight
        jmp Raus
LeftLenZero:
        call GetNextLeftPoint
        jnz ContinueLeft
Raus:   pop ebp
        pop edi
        pop esi
        pop ecx
        pop ebx
        ret
_FillPolygonT_ endp

        end

