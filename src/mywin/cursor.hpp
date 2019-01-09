//
// cursor.hpp
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _CURSOR_HPP_INCLUDED
#define _CURSOR_HPP_INCLUDED

#include "types.hpp"

typedef char *HCURSOR;

class CCursor {
private:
  int     Displayed : 1;
  HCURSOR Cursor;
  CPoint  Pos, Size;
  char    *ScreenBuffer;
  int     Page;
public:
  CCursor( short Width, short Height );
  ~CCursor();
  void Set( HCURSOR Cursor ) { CCursor::Cursor = Cursor; }
  HCURSOR Get() { return Cursor; }
  void Move( CPoint Pos );
  void Show();
  void Hide();
};

void DisplayMouseCursor( CPoint Pos );

#endif

