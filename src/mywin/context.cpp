//
// context.cpp, screen output routines
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include "context.hpp"

/////////////////////////////////////////////////////////////////////////////
// Class DC

// Text in CDC ausgeben
void CDC::DrawText( const char *Text, const CRect &r )
{
  short cx, cy;
  cy = top + r.top;
  CSize Size = GetTextExtent( Text );
  _SetColor( Color );
  if( TextAlign & TA_BOTTOM && Size.cy < r.bottom - r.top )
    cy += r.bottom - r.top - Size.cy;
  else if( TextAlign & TA_MIDDLE && Size.cy < r.bottom - r.top )
    cy += ( r.bottom - r.top - Size.cy ) >> 1;
  while( *Text ) {
    cx = left + r.left;
    if( TextAlign & TA_RIGHT ) {
      short Width = GetTextLineExtent( Text );
      if( Width < r.right - r.left )
        cx += r.right - r.left - Width;
    } else if( TextAlign & TA_CENTER ) {
      short Width = GetTextLineExtent( Text );
      if( Width < r.right - r.left )
        cx += ( r.right - r.left - Width ) >> 1;
    }
    DrawTextXY( Text, cx, cy );
    Text = SkipTextLine( Text );
    cy += FontCharHeight;
  }
}

// 3D-Rahmen in CDC ausgeben
void CDC::Draw3DFrame( const CRect &_r, DWORD Style )
{
  CRect r( _r );
  r.left += left; r.top += top; r.right += left; r.bottom += top;
  char i, Thickness = Style & DF_WIDTH;
  if( !Thickness )
    Thickness++;
  COLOR cl1, cl2;
  if( Style & DF_OUT ) {
    cl1 = ColorShadowLight;
    cl2 = ColorShadowDark;
  } else if( Style & DF_IN ) {
    cl1 = ColorShadowDark;
    cl2 = ColorShadowLight;
  } else if( Style & DF_PRESSED ) {
    cl1 = ColorShadowDark;
    cl2 = ColorShadowDark;
  }
  for( i=0; i<Thickness; i++ ) {
    _SetColor( cl1 );
    _DrawLineHC( r.left+i, r.top+i, r.right-i );
    _DrawLineVC( r.left+i, r.top+i+1, r.bottom-i );
    _SetColor( cl2 );
    _DrawLineHC( r.left+i+1, r.bottom-i-1, r.right-i );
    _DrawLineVC( r.right-i-1, r.top+i+1, r.bottom-i-1 );
  }
}


