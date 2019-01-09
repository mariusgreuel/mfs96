//
// mouse.cpp, mouse io routines
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include <i86.h>
#include "dpmi.hpp"
#include "debug.hpp"
#include "mouse.hpp"

/////////////////////////////////////////////////////////////////////////////
// Class Mouse

// Mouse detektieren
BOOL CMouse::_Detect()
{
  union REGS r;
  if( D32GetRMVector( 0x33 ) ) {
    r.w.ax = 0;
    int386( 0x33, &r, &r );
    if( r.w.ax != 0 )
      return TRUE;
  }
  return FALSE;
}

// Mausposition und -tasten lesen
void CMouse::Read()
{
  LastKey = Key;
  LastPos = Pos;
  union REGS r;
  r.w.ax = 3;
  int386( 0x33, &r, &r );
  Key = r.w.bx & 0x3;
  Pos.x = r.w.cx;
  Pos.y = r.w.dx;
  Action = 0;
  if( Key & MK_LBUTTON && !( LastKey & MK_LBUTTON ))
    Action |= MA_LBUTTONDOWN;
  if( Key & MK_RBUTTON && !( LastKey & MK_RBUTTON ))
    Action |= MA_RBUTTONDOWN;
  if( LastKey & MK_LBUTTON && !( Key & MK_LBUTTON ))
    Action |= MA_LBUTTONUP;
  if( LastKey & MK_RBUTTON && !( Key & MK_RBUTTON ))
    Action |= MA_RBUTTONUP;
  if( Pos != LastPos )
    Action |= MA_MOUSEMOVE;
}

