//
// statusb.cpp
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include "mywin.hpp"
#include "statusb.hpp"

#define StatusBarHeight         12
#define ColorStatusBar          RED
#define ColorStatusBarLine      BLACK
#define ColorStatusBarText      WHITE

/////////////////////////////////////////////////////////////////////////////
// class StatusBar

// Statusleiste erzeugen
BOOL CStatusBar::Create( CWnd *Parent )
{
  CRect r;
  Parent->GetClientRect( r );
  r.top = r.bottom - StatusBarHeight;
  r.height = StatusBarHeight;
  CWnd::Create( NULL, WS_VISIBLE|WS_CHILD, r, Parent );
  Parent->ResizeClientWindow( -StatusBarHeight, RR_BOTTOM );
  return TRUE;
}

// Statusleiste zeichen
void CStatusBar::DrawStatusBar( void )
{
  CDC *hDC = GetDC();
  CRect r;
  GetClientRect( r );
  hDC->SetColor( ColorStatusBarLine );
  hDC->DrawLineH( r.left, r.top, r.right );
  r.top++;
  hDC->SetColor( ColorStatusBar );
  hDC->FillRect( r );
  const char *Text = GetWindowText();
  if( Text != NULL ) {
    hDC->SetColor( ColorStatusBarText );
    hDC->SetTextAlign( TA_LEFT|TA_MIDDLE );
    hDC->DrawText( Text, r );
  }
  ReleaseDC( hDC );
}

// OnPaint
void CStatusBar::OnPaint( void )
{
  DrawStatusBar();
}

/////////////////////////////////////////////////////////////////////////////
// class Wnd Erweiterung

// ein Fenster mit Statusleiste ausstatten
BOOL CWnd::SetStatusBar( void )
{
  return (new CStatusBar)->Create( this );
}

