//
// pic.hpp, hardware IRQ routines
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _PIC_HPP_INCLUDED
#define _PIC_HPP_INCLUDED

#define MOCW1           0x21
#define SOCW1           0xa1
#define MBASE           0x08
#define SBASE           0x70

#include <conio.h>

// zu einem IRQ den koresspondieren Int holen
inline int IRQToInt( int Nr )
{
  return Nr < 8 ? MBASE + Nr : SBASE + Nr;
}

// IRQ-Masken holen
// LowByte = Master, HihgByte = Slave
inline int GetIRQMask( void )
{
  return inp( MOCW1 ) + ( inp( SOCW1 ) << 8 );
}

// IRQ-Masken setzen
// LowByte = Master, HihgByte = Slave
inline void SetIRQMask( int Mask )
{
  outp( MOCW1, Mask & 0xff );
  outp( SOCW1, Mask >> 8 & 0xff );
}

// einen IRQ freigeben
inline void EnableIRQ( int IRQ )
{
  if( IRQ < 8 )
    outp( MOCW1, inp( MOCW1 ) & ~( 1 << IRQ ));
  else
    outp( SOCW1, inp( SOCW1 ) & ~( 1 << IRQ-8 ));
}

// einen IRQ sperren
inline void DisableIRQ( int IRQ )
{
  if( IRQ < 8 )
    outp( MOCW1, inp( MOCW1 ) | 1 << IRQ );
  else
    outp( SOCW1, inp( SOCW1 ) | 1 << IRQ-8 );
}

#endif // _PIC_HPP_INCLUDED

