;//
;// image.asm
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

#include "ega16.inc"

        .data
        extrn   GraphicSegment  : DWORD
Len     dd 0
RRR     db 0
Plane   db 0

        .code

        public _ImageSize_
        public _GetImage_
        public _PutImage_

;errechnet die Speichermenge eines Bildausschnittes
;> EAX = Breite, EDX = Höhe
;< EAX = Größe in Bytes ( eax = dy*(dx+7)/8*4 + 6 )
_ImageSize_ proc
        push ebx
        add eax,7
        and al,0f8h
        mov ebx,edx
        cdq
        mul ebx
        shr eax,1
        add eax,6
        pop ebx
        ret
_ImageSize_ endp

;speichert einen Bildausschnitt
;> EAX = X1, EDX = Y1
;  EBX = X2, ECX = Y2
;  *Puffer
;< -
_GetImage_ proc
        push ebp
        mov ebp,esp
        push esi
        push edi
        mov edi,[ebp+8]                 ;EDI = Buffer
        cmp eax,ebx
        jbe NoMinXChange
        xchg eax,ebx
NoMinXChange:
        cmp edx,ecx
        jbe NoMinYChange
        xchg edx,ecx
NoMinYChange:
        imul esi,edx,BYTES_PER_LINE
        add esi,[GraphicSegment]        ;Line offset in ESI = x * BPL
        sub ebx,eax                     ;ebx = dx
        sub ecx,edx                     ;ecx = dy
        mov [edi],ebx
        add edi,2
        mov [edi],ecx
        add edi,2
        mov word ptr [edi],4
        add edi,2
        mov [Len],ecx                   ;Anzahl Zeilen
        mov cl,al
        and cl,07
        shr eax,3
        add esi,eax                     ;Offset in ESI
        add ebx,7
        shr ebx,3                       ;Bytes pro Zeile
;        dec ebx
;        shr ebx,3                       ;Bytes pro Zeile
;        inc ebx
        mov edx,GDC
        mov al,4
        out dx,al
        inc dl
ReadLineLoop:
        mov [Plane],3
ReadBPLoop:
        push esi
        mov ebp,ebx
        mov al,[Plane]
        out dx,al                       ;BitPlane setzen
        mov ah,[esi]
        inc esi
ReadPixel:
        mov al,[esi]
        inc esi
        mov ch,al
        shl eax,cl
        mov [edi],ah
        inc edi
        mov ah,ch
        dec ebp
        jnz ReadPixel
        pop esi
        dec [Plane]
        jns ReadBPLoop
        add esi,BYTES_PER_LINE
        dec [Len]
        jnz ReadLineLoop
        pop edi
        pop esi
        pop ebp
        ret 4
_GetImage_ endp

WriteLine macro
        mov dl,(GDC+1) and 0ffh
        or ebp,ebp
        jz xxx
        mov al,0ffh
        shr al,cl
        out dx,al
        mov al,[esi]
        inc esi
        mov ch,al
        shr al,cl
        xchg [edi],al
        inc edi
        dec ebp
        jz LL2
        mov al,0ffh
        out dx,al
PutPixel:
        mov al,[esi]
        inc esi
        mov ah,ch
        mov ch,al
        shr eax,cl
        xchg [edi],al
        inc edi
        dec ebp
        jnz PutPixel
LL2:    mov al,[esi]
        inc esi
        mov ah,ch
        cmp cl,[RRR]
        jbe LL3
        mov ch,al
        shr eax,cl
        xchg [edi],al
        inc edi
        mov ah,ch
        mov al,-1
LL3:    push cx
        push ax
        mov cl,7
        sub cl,[RRR]
        mov al,-1
        shl al,cl
        out dx,al
        pop ax
        pop cx
        shr eax,cl
        xchg [edi],al
xxx:
        endm     

;schreibt einen Bildausschnitt
;> EAX = X1, EDX = Y1
;  EBX = *Puffer
_PutImage_ proc
        pushad
        push ecx
        push esi
        push edi
        push ebp
        mov esi,ebx                     ;Puffer in ESI
        movzx ebx,word ptr [esi]
        add esi,2
        movzx ebp,word ptr [esi]        ;Anzahl Zeilen
        mov [Len],ebp
        add esi,4
        imul edi,edx,BYTES_PER_LINE
        add edi,[GraphicSegment]         ;Line offset in EDI
        dec ebx
        mov cl,al
        and cl,7
        mov ch,al
        add ch,bl
        and ch,7
        mov [RRR],ch
        shr eax,3
        add edi,eax                     ;Offset in EDI
        shr ebx,3                       ;Bytes pro Zeile in EBX
        mov dx,GDC
        mov ax,0001h
        out dx,ax                       ;Enable S/R, Prozessor Byte verwenden
        mov ax,0003h
        out dx,ax                       ;DataRotate, Inhalt überschreiben
        mov al,8
        out dx,al                       ;Index BitMask setzen
        mov dx,SEQ
        mov al,02h
        out dx,al                       ;Index BitPlane schreiben
WriteLineLoop:
        mov [Plane],8
WriteBPLoop:
        push edi
        mov ebp,ebx
        mov dl,(SEQ+1) and 0ffh
        mov al,[Plane]
        out dx,al                       ;BitPlane setzen
        WriteLine
        pop edi
        shr [Plane],1
        jnz WriteBPLoop
        add edi,BYTES_PER_LINE
        dec [Len]
        jnz WriteLineLoop
        mov dx,GDC
        mov ax,0f01h
        out dx,ax                       ;Enable S/R, S/R Byte verwenden
        mov dx,SEQ
        mov ax,0f02h
        out dx,ax                       ;Alle Planes ein
        pop ebp
        pop edi
        pop esi
        pop ecx
        popad
        ret
_PutImage_ endp

        end

