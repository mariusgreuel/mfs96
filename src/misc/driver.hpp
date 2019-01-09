//
// driver.hpp, driver base class
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _DRIVER_HPP_INCLUDED
#define _DRIVER_HPP_INCLUDED

#include <iostream.h>
#include "types.h"

// Achtung!
// Shutdown() muﬂ im Destructor der primary Class aufgerufen werden.
// z.B. ~CTimer() { Shutdown(); }

class CDriver {
  enum { INSTALLED = 1, ACTIVE = 2 };
  BYTE Flag;
  const char *Name;
protected:
  virtual BOOL _Detect() = 0;
  virtual void _Startup() = 0;
  virtual void _Shutdown() = 0;
public:
  CDriver( const char *Name ) : Flag( 0 ), Name( Name ) {}
  void Detect( BOOL Activate );
  void Startup();
  void Shutdown();
  BOOL IsInstalled() { return Flag & INSTALLED ? TRUE:FALSE; }
  BOOL IsActive() { return Flag & ACTIVE ? TRUE:FALSE; }
};

#endif

