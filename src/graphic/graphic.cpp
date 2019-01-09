//
// graphic.cpp, basic graphic routines
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include "graphic.hpp"

/////////////////////////////////////////////////////////////////////////////
// Class Graphic

// Grafikmodus setzen
void CGraphic::SetGraphMode()
{
  if( !( Flag & GRAPHMODE ) ) {
    Flag |= GRAPHMODE;
    _SaveScreen();
    _SetGraphicMode();
    _ClearScreen();
    SetPalette();
  }
}

// Textmodus setzen
void CGraphic::SetTextMode()
{
  if( Flag & GRAPHMODE ) {
    Flag &= ~GRAPHMODE;
    _SetTextMode();
    _RestoreScreen();
  }
}

// Palettetabelle initialisieren
void CGraphic::InitPalette()
{
  for( int i=0; i<MAXCOLORS; i++ )
    Palette[i] = INVALID_COLOR;
}

// Palette neu setzen
void CGraphic::SetPaletteEntry( COLORREG Register, COLOR Color )
{
  if( Register < MAXCOLORS )
    Palette[ Register ] = Color & 0xffffff;
}

// Palette neu setzen
void CGraphic::SetPalette()
{
  for( int i=0; i<MAXCOLORS; i++ )
    if( Palette[i] != INVALID_COLOR )
      _SetColorReg( i, Palette[i] );
}

/////////////////////////////////////////////////////////////////////////////
// Verschiedenes

// Text ausgeben
void DrawTextXY( const char *Text, short x, short y )
{
  while( *Text && *Text != '\n' ) {
    _DrawCharC( *Text, x, y );
    Text++;
    x += FontCharWidth;
  }
}

const char *SkipTextLine( const char *Text )
{
  while( *Text && *Text != '\n' )
    Text++;
  if( *Text == '\n' )
    Text++;
  return Text;
}

// Ausmaße einer Textzeile holen
short GetTextLineExtent( const char *Text )
{
  short w = 0;
  while( *Text && *Text++ != '\n' )
    w += FontCharWidth;
  return w;
}

// Ausmaße des Textes holen
CSize GetTextExtent( const char *Text )
{
  CSize Size( 0, 0 );
  while( *Text ) {
    short w = 0;
    while( *Text && *Text++ != '\n' )
      w += FontCharWidth;
    if( Size.cx < w )
      Size.cx = w;
    Size.cy += FontCharHeight;
  }
  return Size;
}


