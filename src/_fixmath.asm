;//
;// _fixmath.asm, 32 bit fixpoint arithmetic
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

        .code

        public SqrtFix16_
        public SqrtFixSq_

;Wurzel aus einer FIX16 Zahl ziehen
;> eax = Wert
;< eax = Sqrt( Wert )
SqrtFix16_ proc
        or eax,eax
        jz SFix16_L4
        push ebx
        push ecx
        push edx
        push esi
        mov esi,eax
        ror esi,16              ;für Division vorbereiten
        mov ebx,80h
SFix16_L1:
        rol ebx,1
        shr eax,2
        jnz SFix16_L1           ;Wurzel durch log. annähern
        mov cl,5
SFix16_L2:
        mov eax,esi
        sub edx,edx
        xchg dx,ax
        div ebx
        add ebx,eax
        rcr ebx,1
        dec cl
        jz SFix16_L3
        cmp ebx,eax
        jnz SFix16_L2
SFix16_L3:
        mov eax,ebx
        pop esi
        pop edx
        pop ecx
        pop ebx
SFix16_L4:
        ret
SqrtFix16_ endp

;Wurzel aus einer FIX-Quadrat Zahl ziehen
;> edx:eax = Wert
;< eax = Sqrt( edx:eax )
SqrtFixSq_ proc
        push ebx
        or ebx,eax
        or ebx,edx
        jz SFixSq_L4
        push ecx
        push esi
        push edi
        mov esi,eax
        mov edi,edx
        mov ebx,80000000h
SFixSq_L1:
        rol ebx,1
        shrd eax,edx,2
        shr edx,2
        or eax,eax
        jnz SFixSq_L1
        or edx,edx
        jnz SFixSq_L1
        mov cl,8
SFixSq_L2:
        mov eax,esi
        mov edx,edi
        div ebx
        add ebx,eax
        rcr ebx,1
        dec cl
        jz SFixSq_L3
        cmp ebx,eax
        jnz SFixSq_L2
SFixSq_L3:
        mov eax,ebx
        pop edi
        pop esi
        pop ecx
SFixSq_L4:
        pop ebx
        ret
SqrtFixSq_ endp

        end

