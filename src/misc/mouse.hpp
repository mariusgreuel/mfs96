//
// mouse.hpp, mouse io routines
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _MOUSE_HPP_INCLUDED
#define _MOUSE_HPP_INCLUDED

#include "types.hpp"
#include "driver.hpp"

// MouseKeys
#define MK_LBUTTON      1
#define MK_RBUTTON      2

// MouseAction
#define MA_LBUTTONDOWN  1
#define MA_RBUTTONDOWN  2
#define MA_LBUTTONUP    4
#define MA_RBUTTONUP    8
#define MA_MOUSEMOVE    0x10

class CMouse : public CDriver {
public:
  CPoint Pos, LastPos;
  WORD Key, LastKey;
  WORD Action;
protected:
  BOOL _Detect();
  void _Startup() {};
  void _Shutdown() {};
public:
  CMouse() : CDriver( "CMouse" ) { Pos.x = Pos.y = 0; }
  ~CMouse() { Shutdown(); }
  void Read();
};

#endif // _MOUSE_HPP_INCLUDED

