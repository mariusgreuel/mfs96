//
// uart.hpp
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _UART_HPP_INCLUDED
#define _UART_HPP_INCLUDED

#define RBR             0       // Datenregister
#define IER             1       // Interrupt Freigaberegister
#define IIR             2       // Interrupt Id Register
#define LCR             3       // Leitungs Kontrollregister
#define MCR             4       // Modem Kontrollregister
#define LSR             5       // Leitungs Statusregister
#define MSR             6       // Modem Statusregister
#define DCTS            01      // Status DCTS
#define DDSR            02      // Status DDSR
#define CTS             0x10    // Status CTS
#define DSR             0x20    // Status DSR
#define DTR             1       // Leitung DTR
#define RTS             2       // Leitung RTS
#define OUT2            8       // Leitung IRQ Freigabe

#endif // _UART_HPP_INCLUDED

