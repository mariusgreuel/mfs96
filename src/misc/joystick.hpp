//
// joystick.hpp, joystick routines
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _JOYSTICK_HPP_INCLUDED
#define _JOYSTICK_HPP_INCLUDED

#include "types.hpp"
#include "driver.hpp"

#define JOYSTICK_FIRE   1
#define JOYSTICK_BOMB   2

class CJoystick : public CDriver {
public:
  struct {
    char Switch;
    CPoint Pos;
  }Stick1, Stick2;
protected:
  BOOL _Detect();
  void _Startup() {};
  void _Shutdown() {};
public:
  CJoystick() : CDriver( "CJoystick" ) {}
  ~CJoystick() { Shutdown(); }
  BOOL Read();
};

#endif // _JOYSTICK_HPP_INCLUDED

