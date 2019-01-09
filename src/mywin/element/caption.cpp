//
// caption.cpp
//
// Copyright Marius Greuel 1996. All rights reserved.
// 

#include "mywin.hpp"
#include "caption.hpp"

#define ColorCaptionActive      DARKBLUE
#define ColorCaptionTextActive  WHITE
#define ColorCaptionPassive     WHITE
#define ColorCaptionTextPassive BLACK

/////////////////////////////////////////////////////////////////////////////
// class Caption

// Titelleiste erzeugen
BOOL CCaption::Create( CWnd *Parent )
{
  CRect r;
  Parent->GetClientRect( r );
  r.height = CAPTION_HEIGHT;
  CWnd::Create( NULL, WS_VISIBLE|WS_CHILD, r, Parent );
  Parent->ResizeClientWindow( -CAPTION_HEIGHT, RR_TOP );
  return TRUE;
}

// Titelleiste zeichen
void CCaption::Draw()
{
  CDC *hDC = GetDC();
  CRect r;
  GetClientRect( r );
  if( GetParent()->GetFlag() & WF_ACTIVE )
    hDC->SetColor( ColorCaptionActive );
  else
    hDC->SetColor( ColorCaptionPassive );
  hDC->FillRect( r );
  r.bottom++;
  if( GetParent()->GetFlag() & WF_ACTIVE )
    hDC->SetColor( ColorCaptionTextActive );
  else
    hDC->SetColor( ColorCaptionTextPassive );
  hDC->SetTextAlign( TA_CENTER | TA_MIDDLE );
  hDC->DrawText( GetParent()->GetWindowText(), r );
  ReleaseDC( hDC );
}

void CCaption::OnLButtonDown( WORD, CPoint p )
{
  if( GetParent()->GetStyle() & WS_MOVEABLE ) {
    SetCapture();
    Captured = TRUE;
    LastPos = p;
  }
}

void CCaption::OnLButtonUp( WORD, CPoint )
{
  if( Captured ) {
    ReleaseCapture();
    Captured = FALSE;
  }
}

void CCaption::OnMouseMove( WORD, CPoint p )
{
  if( Captured ) {
    GetParent()->SetWindowPos( p - LastPos, SW_RELATIVE );
  }
}

/////////////////////////////////////////////////////////////////////////////
// class Wnd Erweiterung

// ein Fenster mit Titelleiste ausstatten
BOOL CWnd::SetCaption()
{
  return (new CCaption)->Create( this );
}

