//
// driver.cpp, driver base class
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include <iostream.h>
#include "debug.hpp"
#include "driver.hpp"

static const char *DetectStr = "_DETECT";
static const char *StartupStr = "_STARTUP";
static const char *ShutdownStr = "_SHUTDOWN";

// detektieren
void CDriver::Detect( BOOL Activate )
{
  if( Activate ) {
    if( GetDebugLevel() >= 1 )
      cout << Name << DetectStr << ": ";
    if( _Detect() )
      Flag |= INSTALLED;
    if( Flag & INSTALLED )
      if( GetDebugLevel() >= 1 )
        cout << "ok\n";
    else
      if( GetDebugLevel() >= 1 )
        cout << "failed\n";
  } else if( GetDebugLevel() >= 1 )
    cout << "skipping "<< Name << DetectStr << "\n";
}

// Treiber aktivieren
void CDriver::Startup()
{
  if( !( Flag & INSTALLED ))
    return;
  if( Flag & ACTIVE )
    return;
  Flag |= ACTIVE;
  if( GetDebugLevel() >= 1 )
    cout << Name << StartupStr << '\n';
  _Startup();
}

// Treiber deaktivieren
void CDriver::Shutdown()
{
  if( !( Flag & ACTIVE ) )
    return;
  Flag &= ~ACTIVE;
  if( GetDebugLevel() >= 1 )
    cout << Name << ShutdownStr << '\n';
  _Shutdown();
}


