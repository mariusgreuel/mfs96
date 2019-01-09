;//
;// _except.asm, protected mode exception handler
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

STACK_SIZE      = 256           ; 256 DWords muessen reichen

_DATA   segment dword public use32 'DATA'
        public _OldExceptionHandler0
        public _OldExceptionHandler6
        public _OldExceptionHandler0C
        public _OldExceptionHandler0D
        public _OldExceptionHandler0E
_OldExceptionHandler0   dp ?
_OldExceptionHandler6   dp ?
_OldExceptionHandler0C  dp ?
_OldExceptionHandler0D  dp ?
_OldExceptionHandler0E  dp ?
_DATA   ends

_STACK  segment dword public use32 'STACK'
                dd STACK_SIZE dup (?)
MiniStack       dd ?
OldStackPtr     dp ?
_STACK  ends

DGROUP  group _DATA, _STACK

_TEXT   segment byte public use32 'CODE'
        assume cs:_TEXT, ds:DGROUP

        public ExceptionHandler0_
        public ExceptionHandler6_
        public ExceptionHandler0C_
        public ExceptionHandler0D_
        public ExceptionHandler0E_

        extrn DefaultExceptionHandler_ : proc

;den Defaulthandler aufrufen
CallDefaultHandler proc
        push eax
        mov dword ptr [OldStackPtr],esp
        mov word ptr [OldStackPtr+4],ss         ; ss:esp sichern
        mov ax,_STACK
        mov ss,ax
        mov esp,offset MiniStack                ; biﬂchen Platz schaffen
        pushad
        call DefaultExceptionHandler_
        popad
        lss esp,[OldStackPtr]                   ; ss:esp wiederherstellen
        pop eax
        ret
CallDefaultHandler endp

;der neue exception handler 0 (divide by zero)
ExceptionHandler0_ proc
        call CallDefaultHandler
        jmp [_OldExceptionHandler0]
ExceptionHandler0_ endp

;der neue exception handler 6 (invalid opcode)
ExceptionHandler6_ proc
        call CallDefaultHandler
        jmp [_OldExceptionHandler6]
ExceptionHandler6_ endp

;der neue exception handler 0c (stack fault)
ExceptionHandler0C_ proc
        call CallDefaultHandler
        jmp [_OldExceptionHandler0C]
ExceptionHandler0C_ endp

;der neue exception handler 0d (general protection fault)
ExceptionHandler0D_ proc
        call CallDefaultHandler
        jmp [_OldExceptionHandler0D]
ExceptionHandler0D_ endp

;der neue exception handler 0e (page fault)
ExceptionHandler0E_ proc
        call CallDefaultHandler
        jmp [_OldExceptionHandler0E]
ExceptionHandler0E_ endp

_TEXT   ends

        end

