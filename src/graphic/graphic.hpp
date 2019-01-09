//
// graphic.hpp, basic graphic routines
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _GRAPHIC_HPP_INCLUDED
#define _GRAPHIC_HPP_INCLUDED

#include "types.hpp"
#include "driver.hpp"
#include "ega16.h"

#define XRES                    640
#define YRES                    350
#define FontCharWidth           8
#define FontCharHeight          8
#define MAXCOLORS               16
#define INVALID_COLOR           -1

typedef unsigned long COLOR;
typedef unsigned char COLORREG;

enum MYCOLORS {
  BLACK,
  DARKBLUE,
  DARKGREEN,
  DARKMGREEN,
  DARKRED,
  DARKBROWN,
  DARKYELLOW,
  DARKWHITE,
  GRAY,
  BLUE,
  GREEN,
  MGREEN,
  RED,
  BROWN,
  YELLOW,
  WHITE,
  COLOR_SHADOW = 254,
  COLOR_NULL = 255
};

class CGraphic : public CDriver {
  enum { GRAPHMODE = 1 };
  BYTE Flag;
  COLOR Palette[ MAXCOLORS ];
private:
  void InitPalette();
  void SetPalette();
protected:
  BOOL _Detect() { return _DetectGraph(); }
  void _Startup() { InitPalette(); }
  void _Shutdown() { SetTextMode(); }
public:
  CGraphic() : CDriver( "CGraphic" ) {}
  ~CGraphic() { Shutdown(); }
  void SetTextMode();
  void SetGraphMode();
  void SetPaletteEntry( BYTE Register, COLOR Color );
  void GetScreenRect( CRect &r ) { r.x = r.y = 0; r.width = XRES; r.height = YRES; }
};

short GetTextLineExtent( const char *Text );
CSize GetTextExtent( const char *Text );

void DrawTextXY( const char *Text, short x, short y );
const char *SkipTextLine( const char *Text );

inline COLOR HTC2RGB( BYTE Color, BYTE Contrast, BYTE Brightness )
{
  return ( Color << 16 ) + ( Contrast << 8 ) + Brightness;
}

#endif // _GRAPHIC_HPP_INCLUDED

