//
// debug.hpp, development phase debug routines
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _DEBUG_HPP_INCLUDED
#define _DEBUG_HPP_INCLUDED

#include "types.h"

class CDebug {
  static BYTE DebugLevel;
public:
  friend void SetDebugLevel( BYTE Level ) { DebugLevel = Level; }
  friend BYTE GetDebugLevel() { return DebugLevel; }
};

#endif

