;uart.inc, declartions for uart 8250, serial communication

RBR                     = 0     ;Datenregister
IER                     = 1     ;Interrupt Freigaberegister
IIR                     = 2     ;Interrupt Id Register
LCR                     = 3     ;Leitungs Kontrollregister
MCR                     = 4     ;Modem Kontrollregister
MSR                     = 6     ;Modem Statusregister
DCTS                    = 01    ;Status DCTS
DDSR                    = 02    ;Status DDSR
CTS                     = 10h   ;Status CTS
DSR                     = 20h   ;Status DSR
DTR                     = 1     ;Leitung DTR
RTS                     = 2     ;Leitung RTS
OUT2                    = 8     ;Leitung IRQ Freigabe

