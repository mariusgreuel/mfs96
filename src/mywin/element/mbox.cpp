//
// mbox.cpp
//
// Copyright Marius Greuel 1996. All rights reserved.
// 

#include "mywin.hpp"
#include "text.hpp"
#include "button.hpp"
#include "mbox.hpp"

/////////////////////////////////////////////////////////////////////////////
// class MessageBox

// MessageBox erzeugen
CMessageBox::Create( CWnd *Parent, char *Text, char *Title, DWORD Style )
{
  CRect r( 0,0,GetTextLineExtent( Title )+10,0 );
  CalcTextSize( r, Text );
  CalcButtonSize( r, Style );
  CalcWindowRect( r, WS_OVERLAPPED );
  CWnd::Create( Title, WS_OVERLAPPED|WS_MOVEABLE, r, Parent );
  SetWindowPos( CPoint( 0,0 ), SW_CENTER );
  SetupText( Text );
  SetupButtons( Style );
}

// Anzahl der Buttons ermitteln
int CMessageBox::GetNumButtons( DWORD Style )
{
  int NumButtons = 0;
  for( int i=0; i<PREDEFINED_BUTTONS; i++ ) {
    if( Style & 1 )
      NumButtons++;
    Style >>= 1;
  }
  return NumButtons;
}

// Ausmaﬂe des Dialog mit Text anpassen
void CMessageBox::CalcTextSize( CRect &r, const char *Text )
{
  CSize Size = GetTextExtent( Text );
  Size.width += 10;
  Size.height += 10;
  if( r.width < Size.width )
    r.width = Size.width;
  r.height += Size.height;
}

// Ausmaﬂe des Dialog mit Buttons anpassen
void CMessageBox::CalcButtonSize( CRect &r, DWORD Style )
{
  short Width = GetNumButtons( Style ) * STD_BUTTONDIST;
  if( r.width < Width )
    r.width = Width;
  r.height += STD_BUTTONHEIGHT+5;
}

// Text erzeugen
void CMessageBox::SetupText( const char *Text )
{
  CRect r;
  GetClientRect( r );
  r.bottom -= STD_BUTTONHEIGHT+5;
  CText *TextWnd = new CText;
  TextWnd->Create( Text, r, this );
  TextWnd->SetTextColor( BLACK );
  TextWnd->SetTextAlign( TA_CENTER | TA_MIDDLE );
}

// Buttons erzeugen
void CMessageBox::SetupButtons( DWORD Style )
{
  CRect r;
  GetClientRect( r );
  r.x = r.width - ( GetNumButtons( Style ) * STD_BUTTONDIST
                - (STD_BUTTONDIST-STD_BUTTONWIDTH)) >> 1;
  r.y = r.bottom - STD_BUTTONHEIGHT-5;
  if( Style & MB_OK ) {
    new CStdButton( MB_OK, CPoint( r.x, r.y ), this );
    r.x += STD_BUTTONDIST;
  }
  if( Style & MB_YES ) {
    new CStdButton( MB_YES, CPoint( r.x, r.y ), this );
    r.x += STD_BUTTONDIST;
  }
  if( Style & MB_NO ) {
    new CStdButton( MB_NO, CPoint( r.x, r.y ), this );
    r.x += STD_BUTTONDIST;
  }
  if( Style & MB_CANCEL ) {
    new CStdButton( MB_CANCEL, CPoint( r.x, r.y ), this );
    r.x += STD_BUTTONDIST;
  }
  if( Style & MB_DEFAULT ) {
    new CStdButton( MB_DEFAULT, CPoint( r.x, r.y ), this );
    r.x += STD_BUTTONDIST;
  }
  if( Style & MB_HELP ) {
    new CStdButton( MB_HELP, CPoint( r.x, r.y ), this );
    r.x += STD_BUTTONDIST;
  }
}

// MessageBox ausgeben
WORD MessageBox( char *Text, char *Title, DWORD Style )
{
  CMessageBox *Wnd = new CMessageBox;
  Wnd->Create( NULL, Text, Title, Style );
  DWORD ID = Wnd->GetReturnID( TRUE );
  delete Wnd;
  return ID;
}

