//
// context.hpp
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _CONTEXT_HPP_INCLUDED
#define _CONTEXT_HPP_INCLUDED

#include "graphic.hpp"

#define ColorShadowLight        WHITE
#define ColorShadowDark         DARKWHITE

// Stile für TextAlign
#define TA_LEFT         1
#define TA_RIGHT        2
#define TA_CENTER       4
#define TA_TOP          0x10
#define TA_BOTTOM       0x20
#define TA_MIDDLE       0x40

// Stile für Draw3DFrame
#define DF_WIDTH        0xff
#define DF_IN           0x100
#define DF_OUT          0x200
#define DF_PRESSED      0x400

class CDC : public CRect {
private:
  COLOR         Color;
  BYTE          TextAlign;
public:
  CDC( const CRect &r ) : CRect( r )
  {
    Color = BLACK;
    TextAlign = 0;
  }
  void SetColor( COLOR c )
  {
    Color = c;
  }
  void SetTextAlign( BYTE Style )
  {
    TextAlign = Style;
  }
  void DrawLineH( short x1, short y, short x2 )
  {
    _SetColor( Color );
    _DrawLineHC( left+x1, top+y, left+x2 );
  }
  void DrawLineV( short x, short y1, short y2 )
  {
    _SetColor( Color );
    _DrawLineVC( left+x, top+y1, top+y2 );
  }
  void DrawRect( const CRect &r )
  {
    _SetColor( Color );
    _DrawRectC( left+r.left, top+r.top, left+r.right, top+r.bottom );
  }
  void FillRect( const CRect &r )
  {
    _SetColor( Color );
    _FillRectC( left+r.left, top+r.top, left+r.right, top+r.bottom );
  }
  void DrawText( const char *Text, const CRect &r );
  void Draw3DFrame( const CRect &r, DWORD Style = DF_OUT+1 );
};

#endif

