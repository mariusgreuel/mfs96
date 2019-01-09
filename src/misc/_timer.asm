;//
;// _timer.asm, 32-bit counter with 0.838ns resolution (requires 8253/8254 or compatible)
;//
;// Copyright Marius Greuel 1994. All rights reserved.
;//
;// Compile:
;//   - small to large or flat model
;//   - register based callings (Watcom __stdcall__)
;//
;// Remarks:
;//   - several chipsets got stumbling timers
;//   - routines make use of timer0 and DOS ticker counter
;//     (will be confused by Windows)
;//   so be careful!

TIMER_CTRL              = 43h           ;control word for timer 8253/8254
TIMER_0                 = 40h           ;i/o port timer 0
CW_INIT_TIMERMODE2      = 00110100b     ;initialize timer 0 command (mode 2)
CW_INIT_TIMERMODE3      = 00110110b     ;initialize timer 0 command (mode 3)
CW_READ_TIMER           = 00000100b     ;read timer 0 command

DOS_TICKERCOUNT         = 046ch         ;DOS ticker count (lin. adress)
TICKER_CONST            = 3600140231    ; ~= 2^32 / 1.193

        .code

        public _SetTimerMode2_   ;void _SetTimerMode2()
        public _SetTimerMode3_   ;void _SetTimerMode3()
        public _ReadTimer_       ;long _ReadTimer()

;set timer mode 3 (default bios mode)
;> -
;< -
_SetTimerMode3_ proc
        mov al,CW_INIT_TIMERMODE3
        jmp short SetTimerMode
_SetTimerMode3_ endp

;set timer mode 2 (square wave, needed for readout)
;> -
;< -
_SetTimerMode2_ proc
        mov al,CW_INIT_TIMERMODE2
;        jmp short SetTimerMode
_SetTimerMode2_ endp

;set timer mode
;> al = mode
;< -
SetTimerMode proc
        cli
        out TIMER_CTRL,al       ;load timer control word
        nop
        nop
        mov al,0
        out TIMER_0,al          ;set 2^16 cycles
        nop
        nop
        out TIMER_0,al
        sti
        ret
SetTimerMode endp

;read timer0 and dos tick counter
;irq 0 must be enabled!
;> -
;< time ( EAX | DX:AX ) in 0.838ns/ticks
_ReadTimer_ proc
#ifdef __386__
        push edx
        mov edx,ds:[DOS_TICKERCOUNT]    ;get dos ticker count in edx
        mov eax,edx
        shl eax,16
        mov al,CW_READ_TIMER            ;stop timer
        out TIMER_CTRL,al
        nop
        nop
        cmp edx,ds:[DOS_TICKERCOUNT]    ;test if ticker count changed
#else
        push ds
        xor ax,ax
        mov ds,ax
        mov dx,ds:[DOS_TICKERCOUNT]     ;get dos ticker count in dx
        mov al,CW_READ_TIMER            ;stop timer
        out TIMER_CTRL,al
        cmp dx,ds:[DOS_TICKERCOUNT]     ;test if ticker count changed
        pop ds
#endif
        pushf                           ;save the zero flag
        in al,TIMER_0                   ;read timer value
        nop
        nop
        mov ah,al
        in al,TIMER_0
        nop
        nop
        xchg al,ah
        not ax
        popf                            ;ticker count changed ?
        jz NoOverflow
        or ah,ah
        js NoOverflow
        mov ax,-1                       ;yes, adjust value
NoOverflow:
#ifdef __386__
        mov edx,TICKER_CONST            ;calc micro seconds
        mul edx
        mov eax,edx
        pop edx
#endif
        ret                             ;32-bit timer counter in eax | dx:ax
_ReadTimer_ endp

        end

