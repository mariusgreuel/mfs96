;//
;// polyf.asm
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
        public _FillPolygon_

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
Next:   mov bl,al
        mov al,8
        out dx,ax
        inc byte ptr [edi]
        inc edi
        dec ecx
        jz NoMore
        mov ax,0ff08h
        out dx,ax
        mov bh,cl
        mov eax,-1
        shr ecx,2
        rep stosd
        mov cl,bh
        and cl,3
        rep stosb
NoMore: mov al,8
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
_FillPolygon_ proc
        push ebx
        push ecx
        push esi
        push edi
        push ebp
        mov [FirstPoint],edx    ;Zeiger auf ersten Punkt
        dec eax
        shl eax,2        
        add eax,edx
        mov [LastPoint],eax     ;Zeiger auf letzten Punkt
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
_FillPolygon_ endp

comment $

;zeichnet horizontale Linie
;> BX = Line-Offset
;  SI = X1
;  DI = X2
DRAW_ROW macro
        cmp di,si
        jbe @@L11
        xchg si,di
@@L11:  inc si

        mov cx,si
        and cl,7
        mov ax,0ff00h
        ror ax,cl

        mov cx,di
        and cl,7
        mov ah,-1
        shr ah,cl

        shr si,3
        shr di,3

        mov dx,GDC

        sub si,di
        add di,bp

        or si,si
        jnz @@L12

        and ah,al
        mov al,8
        out dx,ax
        or es:[di],ah
        jmp @@L14
@@L12:  push ax
        mov al,8
        out dx,ax
        or es:[di],ah
        inc di
        dec si
        jz @@L13
        mov ax,0ff08h
        out dx,ax
        mov al,ah
        mov cx,si
        shr cx,1
        rep stosw
        jnc @@L13
        stosb
@@L13:  pop ax
        xchg al,ah
        mov al,8
        out dx,ax
        or es:[di],ah
@@L14:  endm

GET_NEXT_POINTER1 proc
        mov bx,cs:POINTER1
        mov cx,[bx].X
        mov si,cx
        mov dx,[bx].Y
        add bx,-size POINT
        cmp bx,cs:MIN_POINTER
        jnb @@L1
        mov bx,cs:MAX_POINTER
@@L1:   sub dx,[bx].Y
        mov cs:DY1,dx
        neg dx
        mov cs:ADD1,-1
        sub cx,[bx].X
        jnb @@L5
        neg cs:ADD1
        neg cx
@@L5:   mov cs:DX1,cx
        cmp cx,dx
        jbe @@L9
        mov dx,cx
        jmp @@L8
@@L9:   mov cx,dx
        neg cx
@@L8:   sar cx,1
        mov cs:DIFF1,cx
        inc dx
        mov cs:LEN1,dx
        mov cs:POINTER1,bx
        ret
        endp

GET_NEXT_POINTER2 proc
        mov bx,cs:POINTER2
        mov cx,[bx].X
        mov di,cx
        mov dx,[bx].Y
        add bx,type POINT
        cmp bx,cs:MAX_POINTER
        jna @@L1
        mov bx,cs:MIN_POINTER
@@L1:   sub dx,[bx].Y
        mov cs:DY2,dx
        neg dx
        sub cx,[bx].X
        mov cs:ADD2,-1
        jnb @@L5
        neg cs:ADD2
        neg cx
@@L5:   mov cs:DX2,cx
        cmp cx,dx
        jbe @@L8
        mov dx,cx
        jmp @@L9
@@L8:   mov cx,dx
        neg cx
@@L9:   sar cx,1
        mov cs:DIFF2,cx
        inc dx
        mov cs:LEN2,dx
        mov cs:POINTER2,bx
        ret
        endp

CALC_LINE_1 macro
@@L21:  mov ax,cs:DIFF1
        mov cx,cs:LEN1
        or ax,ax
        js @@L23
@@L22:  dec cx
        jz @@L29
        add si,cs:ADD1
        add ax,cs:DY1
        jns @@L22
        add ax,cs:DX1
        mov cs:DIFF1,ax
        mov cs:LEN1,cx
        or al,1
        jmp @@L210
@@L29:  mov ax,cs:POINTER1
        cmp cs:POINTER2,ax
        jz @@L210
        call GET_NEXT_POINTER1
        jmp @@L21
@@L23:  dec cx
        jz @@L29
        add ax,cs:DX1
        js @@L24        
        add si,cs:ADD1
        add ax,cs:DY1
@@L24:  mov cs:DIFF1,ax
        mov cs:LEN1,cx
        or al,1
@@L210: endm

CALC_LINE_2 macro
@@L31:  mov ax,cs:DIFF2
        mov cx,cs:LEN2
        or ax,ax
        js @@L33
@@L32:  dec cx
        jz @@L39
        add di,cs:ADD2
        add ax,cs:DY2
        jns @@L32
        add ax,cs:DX2
        mov cs:DIFF2,ax
        mov cs:LEN2,cx
        or al,1
        jmp @@L310
@@L39:  mov ax,cs:POINTER1
        cmp cs:POINTER2,ax
        jz @@L310
        call GET_NEXT_POINTER2
        jmp @@L31
@@L33:  dec cx
        jz @@L39
        add ax,cs:DX2
        js @@L34        
        add di,cs:ADD2
        add ax,cs:DY2
@@L34:  mov cs:DIFF2,ax
        mov cs:LEN2,cx
        or al,1
@@L310: endm

MIN_POINTER             dw 0
MAX_POINTER             dw 0
POINTER1                dw 0
POINTER2                dw 0
DX1                     dw 0
DY1                     dw 0
DX2                     dw 0
DY2                     dw 0
DIFF1                   dw 0
DIFF2                   dw 0
LEN1                    dw 0
LEN2                    dw 0
ADD1                    dw 0
ADD2                    dw 0
LAST_POINTER1           equ 01
LAST_POINTER2           equ 01

GetMinMaxPointer macro
        mov cs:[MIN_POINTER],si         ;Zeiger auf ersten Punkt
        dec cx
        shl cx,2        
        add cx,si
        mov cs:[MAX_POINTER],cx         ;Zeiger auf letzten Punkt
        endm

SearchHighestPoint macro
        mov ax,-1                       ;kleinsten Y-Wert suchen -> AX
        mov bx,si
@@L1:   cmp [si].Y,ax
        jnb @@L2
        mov ax,[si].Y
        mov bx,si
@@L2:   add si,type POINT
        cmp si,cx
        jna @@L1
        mov cs:POINTER1,bx
        mov cs:POINTER2,bx
        endm

_FillPolygon proc near
        GetMinMaxPointer
        SearchHighestPoint
        imul bp,ax,BYTES_PER_LINE


        call GET_NEXT_POINTER1
        call GET_NEXT_POINTER2
@@L3:   push si di
        DRAW_ROW
        pop di si
        add bp,BYTES_PER_LINE
        CALC_LINE_1
        CALC_LINE_2
        jnz @@L3
        ret
        endp

$

        end

