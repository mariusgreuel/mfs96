;//
;// _vecmath.asm, 32 bit fixpoint vector arithmetic
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

X       = 0
Y       = 4
Z       = 8

        .data?
SinPhi  dd ?
CosPhi  dd ?
Help1   dd ?
Help2   dd ?
Sinus   dd ?
Cosinus dd ?

        .code
        public Vec16Abs_
        public RotateMatrix30ByAxe_
        public MulMatrix_

        extrn SqrtFixSq_ : proc

;Betrag eines Vektors errechnen
;> ebx = &Vector
;< eax = Betrag
Vec16Abs_ proc
        push edx
        push esi
        push edi
        mov eax,X.[ebx]
        imul eax
        mov esi,eax
        mov edi,edx
        mov eax,Y.[ebx]
        imul eax
        add esi,eax
        adc edi,edx
        mov eax,Z.[ebx]
        imul eax
        add eax,esi
        adc edx,edi
        call SqrtFixSq_
        pop edi
        pop esi
        pop edx
        ret
Vec16Abs_ endp

;Matrix rotieren
;> esi = Zeiger auf MatrixZeile 1
;  edi = Zeiger auf MatrixSpalte 2
;< -
RotateMatrix30 proc
        push ebp
        mov ebp,3
RM30_1: mov eax,[edi]
        imul [Sinus]
        mov ebx,eax
        mov ecx,edx
        mov eax,[esi]
        imul [Cosinus]
        sub eax,ebx
        sbb edx,ecx
        shld edx,eax,2
        xchg [esi],edx
        mov eax,edx
        imul [Sinus]
        mov ebx,eax
        mov ecx,edx
        mov eax,[edi]
        imul [Cosinus]
        add eax,ebx
        adc edx,ecx
        shld edx,eax,2
        mov [edi],edx
        add esi,4       ;nächste Spalte
        add edi,4
        dec ebp
        jnz RM30_1
        pop ebp
        ret
RotateMatrix30 endp

;Vektor16 in einen Vektor30 mit Einheitslänge umrechnen
;> ebx = &Vector
;  eax = Divisor
;< -
MakeEVec30 proc
        mov ecx,eax
        xor eax,eax
        mov edx,X.[ebx]
        shrd eax,edx,2
        sar edx,2
        idiv ecx
        mov X.[ebx],eax
        xor eax,eax
        mov edx,Y.[ebx]
        shrd eax,edx,2
        sar edx,2
        idiv ecx
        mov Y.[ebx],eax
        xor eax,eax
        mov edx,Z.[ebx]
        shrd eax,edx,2
        sar edx,2
        idiv ecx
        mov Z.[ebx],eax
        ret
MakeEVec30 endp

;Matrix um Achse Vector rotieren
;> eax = &Matrix30
;  ebx = &Vector30
;  ecx = sin( Phi )
;  edx = cos( Phi )
;< -
RotateMatrix30ByAxe_ proc
        pushad
        mov ebp,eax
        mov [SinPhi],ecx
        mov [CosPhi],edx
        call Vec16Abs_
        or eax,eax
        jz RM3
        call MakeEVec30
        mov eax,Z.[ebx]
        imul eax
        mov esi,eax
        mov edi,edx
        mov eax,Y.[ebx]
        imul eax
        add eax,esi
        adc edx,edi
        mov esi,eax
        mov edi,edx
        call SqrtFixSq_
        mov [Help1],eax             ; h1 = Sqrt( Y^2 + Z^2 )
        mov eax,X.[ebx]
        imul eax
        add eax,esi
        adc edx,edi
        call SqrtFixSq_
        mov [Help2],eax             ; h2 = Sqrt( X^2 + Y^2 + Z^2 )
        cmp eax,10h
        jl RM3
        cmp [Help1],10h
        jl RM1
        xor eax,eax
        mov edx,Y.[ebx]
        shrd eax,edx,2
        sar edx,2
        idiv [Help1]
        mov [Sinus],eax         ;sin = Y / h1
        xor eax,eax
        mov edx,Z.[ebx]
        shrd eax,edx,2
        sar edx,2
        idiv [Help1]
        mov [Cosinus],eax       ;cos = Z / h1
        jmp RM2
RM1:    mov [Sinus],0
        mov [Cosinus],1 shl 30
RM2:    push [Sinus]
        push [Cosinus]
        lea esi,[ebp+1*12]
        lea edi,[ebp+2*12]
        push X.[ebx]
        call RotateMatrix30     ;um X-Achse rotieren
        pop edx
        xor eax,eax
        shrd eax,edx,2
        sar edx,2
        idiv [Help2]
        mov [Sinus],eax         ;sin = x / h2
        xor eax,eax
        mov edx,[Help1]
        shrd eax,edx,2
        sar edx,2
        idiv [Help2]
        mov [Cosinus],eax       ;cos = h1 / h2
        push [Sinus]
        push [Cosinus]
        lea esi,[ebp+0*12]
        lea edi,[ebp+2*12]
        call RotateMatrix30     ;um Y-Achse rotieren
        mov eax,[SinPhi]
        mov edx,[CosPhi]
        mov [Sinus],eax
        mov [Cosinus],edx
        lea esi,[ebp+0*12]
        lea edi,[ebp+1*12]
        call RotateMatrix30     ;um Z-Achse rotieren
        pop [Cosinus]
        pop [Sinus]
        lea esi,[ebp+2*12]
        lea edi,[ebp+0*12]
        call RotateMatrix30     ;um Y-Achse rotieren
        pop [Cosinus]
        pop [Sinus]
        lea esi,[ebp+2*12]
        lea edi,[ebp+1*12]
        call RotateMatrix30     ;um X-Achse rotieren
RM3:    popad
        ret
RotateMatrix30ByAxe_ endp

;eine 30'er Matrix mit einer 16'er multiplizieren
;> esi = &Matrix30
;  edi = &Matrix16
;  ebx = &Matrix16      ;Ziel
MulMatrix_ proc
        pushad
        irp I,<0,4,8,12>
          irp J,<0,4,8>
            irp K,<0,4,8>
              mov eax,[esi+I*3+K]
              imul dword ptr [edi+K*3+J]
              if K eq 0
                mov ebp,eax
                mov ecx,edx
              else
                add ebp,eax
                adc ecx,edx
              endif
            endm
            if I ne 12
              shld ecx,ebp,2
            else
              shld ecx,ebp,16
              add ecx,[edi+12*3+J]
            endif
            mov [ebx+I*3+J],ecx
          endm
        endm
        popad
        ret
MulMatrix_ endp

        end

