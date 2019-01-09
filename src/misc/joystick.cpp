//
// joystick.cpp, joystick routines
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include <i86.h>
#include "joystick.hpp"

// use BIOS functions instead of direct hardware access
#define USE_BIOS

/////////////////////////////////////////////////////////////////////////////
// Class Joystick

#ifdef USE_BIOS

// Joystick detektieren
BOOL CJoystick::_Detect()
{
  union REGS r;
  r.h.ah = 0x84;
  r.w.dx = 0;
  int386( 0x15, &r, &r );
  if( r.x.cflag == 0 )
    return TRUE;
  else
    return FALSE;
}

// Position und Tasten lesen
// TRUE, wenn Fehler
BOOL CJoystick::Read()
{
  union REGS r;
  r.h.ah = 0x84;
  r.w.dx = 0;
  int386( 0x15, &r, &r );
  if( r.x.cflag )
    return TRUE;
  Stick1.Switch = r.h.al >> 4 & 3;
  Stick2.Switch = r.h.al >> 6 & 3;
  r.h.ah = 0x84;
  r.w.dx = 1;
  int386( 0x15, &r, &r );
  if( r.x.cflag )
    return TRUE;
  Stick1.Pos = CPoint( r.w.ax-50, r.w.bx-50 );
  Stick2.Pos = CPoint( r.w.cx-50, r.w.dx-50 );
  return FALSE;
}

#endif
