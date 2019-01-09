//
// chkwin95, auf Pr�senz von Windows testen
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include <i86.h>
#include "chkwin95.hpp"

// l�uft Windows zur Zeit ?
// Test mit Multiplexinterrupt 2fh (normale Methode)
BOOL IsWindowsRunning()
{
  union REGS r;
  r.x.eax = 0x1600;             // get version code
  int386( 0x2f, &r, &r );
  if( ( r.h.al & 0x7f ) != 0 )
    return TRUE;                // Windows running
  else
    return FALSE;
}

