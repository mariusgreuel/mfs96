//
// cursor.cpp
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include "types.hpp"
#include "graphic.hpp"
#include "cursor.hpp"

/////////////////////////////////////////////////////////////////////////////
// class Cursor

// Cursor Constructor
CCursor::CCursor( short Width, short Height)
{
  Displayed = FALSE;
  Pos.x = Pos.y = 0;
  Size.x = Width;
  Size.y = Height;
  Cursor = NULL;
  ScreenBuffer = new char[ _ImageSize( Width, Height ) ];
}

// Cursor Destructor
CCursor::~CCursor()
{
  if( ScreenBuffer )
    delete [] ScreenBuffer;
}

// Cursor bewegen
void CCursor::Move( CPoint p )
{
  if( Pos != p ) {
    Hide();
    Pos = p;
  }
  if( !Displayed )
    Show();
}

// Cursor darstellen
void CCursor::Show( void )
{
  if( !Displayed && ScreenBuffer ) {
    Page = _GetActivePage();
    _GetImage( Pos.x, Pos.y, Pos.x+Size.x, Pos.y+Size.y, ScreenBuffer );
    if( Cursor )
      _PutImage( Pos.x, Pos.y, Cursor, 0 );
    DisplayMouseCursor( Pos ); // Provisorium
    Displayed = TRUE;
  }
}

// Cursor verstecken und überschriebenen Bereich restaurieren
void CCursor::Hide( void )
{
  if( Displayed && ScreenBuffer ) {
    int CurrPage = _SetActivePage( Page );
    _PutImage( Pos.x, Pos.y, ScreenBuffer, 0 );
    _SetActivePage( CurrPage );
    Displayed = FALSE;
  }
}

// Mauscursor darstellen
void DisplayMouseCursor( CPoint Pos )
{
  static short int c[] = { 0,0, 8,8, 0,12 }, cc[6];
  cc[0] = c[0] + Pos.x; cc[1] = c[1] + Pos.y;
  cc[2] = c[2] + Pos.x; cc[3] = c[3] + Pos.y;
  cc[4] = c[4] + Pos.x; cc[5] = c[5] + Pos.y;
  _SetColor( WHITE );
  _FillPolygon( 3, cc );
  _SetColor( BLACK );
  _DrawPolygon( 3, cc );
}

