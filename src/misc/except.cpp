//
// except.cpp - default exception handling
//
// Copyright Marius Greuel 1996. All rights reserved
//

#include "dpmi.hpp"
#include "except.hpp"

// wenn CException eingebunden wird, muﬂ die Funktion
// extern "C" BOOL DefaultExceptionHandler()
// definiert werden

// Exceptions abfangen
CException::CException()
{
  // divide by zero
  OldExceptionHandler0 = D32GetExceptionHandler( 0 );
  D32SetExceptionHandler( 0, ExceptionHandler0 );
  // invalid opcode
  OldExceptionHandler6 = D32GetExceptionHandler( 6 );
  D32SetExceptionHandler( 6, ExceptionHandler6 );
  // stack fault
  OldExceptionHandler0C = D32GetExceptionHandler( 0x0c );
  D32SetExceptionHandler( 0x0c, ExceptionHandler0C );
  // general protection fault
  OldExceptionHandler0D = D32GetExceptionHandler( 0x0d );
  D32SetExceptionHandler( 0x0d, ExceptionHandler0D );
  // page fault
  OldExceptionHandler0E = D32GetExceptionHandler( 0x0e );
  D32SetExceptionHandler( 0x0e, ExceptionHandler0E );
}

// alte Exception handler restaurieren
CException::~CException()
{
  D32SetExceptionHandler( 0,    OldExceptionHandler0 );
  D32SetExceptionHandler( 0x0c, OldExceptionHandler0C );
  D32SetExceptionHandler( 0x0d, OldExceptionHandler0D );
  D32SetExceptionHandler( 0x0e, OldExceptionHandler0E );
}


