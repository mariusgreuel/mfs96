//
// keyboard.hpp, keyboard io routines
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _KEYBOARD_HPP_INCLUDED
#define _KEYBOARD_HPP_INCLUDED

#include "types.h"
#include "driver.hpp"
#include "keys.h"

typedef unsigned short KEY;

class CKeyboard : public CDriver {
  KEY *Buffer, *FirstKey, *LastKey;
  KEY ConvertKey( KEY Key );
protected:
  BOOL _Detect() { return TRUE; }
  void _Startup();
  void _Shutdown();
public:
  CKeyboard() : CDriver( "CKeyboard" ) {}
  ~CKeyboard() { Shutdown(); }
  void PurgeBuffer();
  BOOL KeyPressed();
  void Read();
  KEY PutVKey( KEY VKey );
  KEY GetVKey();
  KEY GetKeyboardState();
};

#endif // _KEYBOARD_HPP_INCLUDED

