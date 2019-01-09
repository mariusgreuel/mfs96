;//
;// _rcbox.asm, read rcbox from serial port
;//
;// Copyright Marius Greuel 1996. All rights reserved.
;//

include "timer.inc"
include "pic.inc"
include "uart.inc"

RM_DataOffset           = 80h   ;Start des Datenbereiches (muß unten beim ORG angepaßt werden, BUG!)
CHANNELS                = 7     ;Anzahl Kanäle
ONLY_PULSE              = 1     ;nur Pulslänge auswerten
NEG_PULSE               = 2     ;negative Signale
SHOW_PULSE              = 4     ;zeigt Pulslänge in us

;dieser Datenbereich muß mit der Struktur RCDATA in rcbox.h übereinstimmen
_RC_DATA        struc
Channel         dw CHANNELS dup (0)
Port            dw 0            ;Port des Interfaces
SyncLen         dw 0            ;min. Länge des Startimpulses in Ticks
TimerTicks      dw 0            ;Meßvariable für die Impulslänge
CurrentChannel  db 0            ;Kanal, der gerade gelesen wird
Mode            db 0            ;Modus
Trigger         db 0            ;Ereignis Sync Impuls gefunden
RM_Count        dw 0            ;Anzahl IRQs im Real-Mode
PM_Count        dw 0            ;Anzahl IRQs im Protected-Mode
_RC_DATA        ends

        public RM_Handler_
        public PM_Handler_
        public _RM_CodePtr
        public _RM_DataPtr

;////////////////////////////////////////////////////////////////////////////
;// 16-bit real mode handler

_TEXT16 segment byte public use16 'CODE'
        assume cs:_TEXT16

;Dies ist der neue real mode handler
RM_Handler_ proc
        push ax
        push bx
        push dx
        push si
        push ds
        mov ax,cs
        mov ds,ax
        mov si,RM_DataOffset
        inc [si].RM_Count
        mov dx,[si].Port
        add dl,IIR
        in al,dx                        ;testen, ob es unser INT war
        or al,al
        jnz RM_Finished
        add dl,MSR-IIR
        in al,dx                        ;testen, ob das Signal gewechselt hat
        test al,DCTS
        jz RM_Finished
        test [si].Mode,NEG_PULSE        ;negative Pulse auswerten ?
        jz RM_NoNegative
        not al                          ;ja, Signal invertieren
RM_NoNegative:
        test al,CTS                     ;Signal testen
        pushf
        mov al,CW_READ_TIMER
        out TIMER_CTRL,al
        in al,TIMER_0
        mov ah,al
        in al,TIMER_0
        xchg al,ah
        popf
        jz RM_CTS_Low
        xchg ax,[si].TimerTicks         ;Startwert festlegen
        sub ax,[si].TimerTicks          ;Differenz ermitteln
        cmp ax,[si].SyncLen             ;testen,ob's ein SYNC-Impuls ist
        jae RM_IsSync
        mov bl,[si].CurrentChannel      ;nein, aktuellen Kanal holen
        cmp bl,CHANNELS                 ;nur soviel Kanäle einlesen
        jae RM_Finished
        shl bl,1
        mov bh,0
        mov [si+bx].Channel,ax          ;Wert eintragen
        inc [si].CurrentChannel
        jmp RM_Finished
RM_IsSync:
        mov [si].CurrentChannel,0
        inc [si].Trigger
        jmp RM_Finished
RM_CTS_Low:
        test [si].Mode,ONLY_PULSE       ;nur Pulslänge auswerten ?
        jz RM_Finished
        mov [si].TimerTicks,ax          ;ja, neuen Startwert festlegen
RM_Finished:
        mov al,CW_NON_SPEC_EOI
        out MASTER_PIC,al               ;Interrupt bestätigen
        pop ds
        pop si
        pop dx
        pop bx
        pop ax
        iret
RM_Handler_ endp
        
;       org RM_DataOffset
        org 80h

RM_Data_ _RC_DATA<>                     ;Daten im Codesegment

_TEXT16   ends

;////////////////////////////////////////////////////////////////////////////
;// 32-bit protected mode handler

_DATA   segment dword public use32 'DATA'
_RM_CodePtr  dd 0
_RM_DataPtr  dd 0
_DATA   ends

DGROUP  group _DATA

_TEXT   segment byte public use32 'CODE'
        assume cs:_TEXT, ds:DGROUP

;Dies ist der neue protected mode handler
PM_Handler_ proc
        push eax
        push ebx
        push edx
        push esi
        push ds
        mov eax,DGROUP
        mov ds,ax
        mov esi,[_RM_DataPtr]           ;Daten im DOS-Speicher
        inc [esi].PM_Count
        mov dx,[esi].Port
        add dl,IIR
        in al,dx                        ;testen, ob es unser INT war
        or al,al
        jnz PM_Finished
        add dl,MSR-IIR
        in al,dx                        ;testen, ob das Signal gewechselt hat
        test al,DCTS
        jz PM_Finished
        test [esi].Mode,NEG_PULSE       ;negative Pulse auswerten ?
        jz PM_NoNegative
        not al                          ;ja, Signal invertieren
PM_NoNegative:
        test al,CTS                     ;Signal testen
        pushf
        mov al,CW_READ_TIMER
        out TIMER_CTRL,al
        in al,TIMER_0
        mov ah,al
        in al,TIMER_0
        xchg al,ah
        popf
        jz PM_CTS_Low
        xchg ax,[esi].TimerTicks        ;Startwert festlegen
        sub ax,[esi].TimerTicks         ;Differenz ermitteln
        cmp ax,[esi].SyncLen            ;testen,ob's ein SYNC-Impuls ist
        jae PM_IsSync
        mov bl,[esi].CurrentChannel     ;nein, aktuellen Kanal holen
        cmp bl,CHANNELS                 ;nur soviel Kanäle einlesen
        jae PM_Finished
        shl bl,1
        movzx ebx,bl
        mov [esi+ebx].Channel,ax        ;Wert eintragen
        inc [esi].CurrentChannel
        jmp PM_Finished
PM_IsSync:
        mov [esi].CurrentChannel,0
        inc [esi].Trigger
        jmp PM_Finished
PM_CTS_Low:
        test [esi].Mode,ONLY_PULSE      ;nur Pulslänge auswerten ?
        jz PM_Finished
        mov [esi].TimerTicks,ax         ;ja, neuen Startwert festlegen
PM_Finished:
        mov al,CW_NON_SPEC_EOI
        out MASTER_PIC,al               ;Interrupt bestätigen
        pop ds
        pop esi
        pop edx
        pop ebx
        pop eax
        iretd
PM_Handler_ endp

_TEXT   ends

        end

