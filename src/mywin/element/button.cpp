//
// button.cpp
//
// Copyright Marius Greuel 1996. All rights reserved.
// 

#include "mywin.hpp"
#include "button.hpp"

#define ColorElementBack        GRAY
#define ColorElementFace        BLACK
#define ColorElementText        BLACK
#define ColorElement            BLACK

/////////////////////////////////////////////////////////////////////////////
// class Button

void CButton::OnMouseMove( WORD, CPoint p )
{
  if( Flag & BT_CAPTURED  ) {
    if( IsInClientRect( p ) ) {
      if( Flag & BT_PRESSED && !( Flag & BT_CHECKED ) ) {
        Flag |= BT_CHECKED|BT_HIGHLIGHTED;
        RedrawWindow();
      }
    } else {
      if( Flag & BT_PRESSED ) {
        Flag &= ~(BT_CHECKED|BT_HIGHLIGHTED);
        RedrawWindow();
      } else {
        ReleaseCapture();
        Flag &= ~(BT_CAPTURED|BT_HIGHLIGHTED);
        RedrawWindow();
      }
    }
  } else if( IsInClientRect( p ) ) {
    Flag |= BT_CAPTURED|BT_HIGHLIGHTED;
    SetCapture();
    RedrawWindow();
  }
}

void CButton::OnLButtonDown( WORD, CPoint )
{
  if( Flag & BT_CAPTURED ) {
    Flag |= BT_PRESSED|BT_CHECKED;
    RedrawWindow();
  } else {
    Flag |= BT_CAPTURED|BT_HIGHLIGHTED|BT_PRESSED|BT_CHECKED;
    SetCapture();
    RedrawWindow();
  }
}

void CButton::OnLButtonUp( WORD, CPoint )
{
  if( Flag & BT_CAPTURED && Flag & BT_PRESSED ) {
    if( Flag & BT_CHECKED )
      Action();
    Flag &= ~(BT_CAPTURED|BT_PRESSED|BT_HIGHLIGHTED|BT_CHECKED);
    ReleaseCapture();
    RedrawWindow();
  }
}

/////////////////////////////////////////////////////////////////////////////
// class PushButton

// PushButton zeichnen
void CPushButton::Draw()
{
  CDC *hDC = GetDC();
  CRect r;
  GetClientRect( r );
  hDC->SetColor( ColorElementBack );
  hDC->FillRect( r );
  if( GetState() & BT_CHECKED )
    hDC->Draw3DFrame( r, DF_IN );
  else if( GetState() & (BT_HIGHLIGHTED|BT_FOCUSED) )
    hDC->Draw3DFrame( r, DF_OUT );
  if( GetState() & BT_HIGHLIGHTED )
    hDC->SetColor( RED );
  else
    hDC->SetColor( ColorElementText );
  if( GetState() & BT_CHECKED ) {
    r.left++; r.top++; r.right++; r.bottom++;
  }
  hDC->SetTextAlign( TA_CENTER|TA_MIDDLE );
  hDC->DrawText( GetWindowText(), r );
  ReleaseDC( hDC );
}

/////////////////////////////////////////////////////////////////////////////
// class Switch

// Schalter zeichnen
void CSwitchButton::Draw()
{
  CDC *hDC = GetDC();
  CRect r;
  GetClientRect( r );
  hDC->SetColor( ColorElementBack );
  hDC->FillRect( r );
  hDC->SetColor( ColorElementFace );
  hDC->DrawRect( CRect( 0,0, r.height, r.height ) );
  if( Value ) {
    hDC->SetColor( ColorElementFace );
    hDC->FillRect( CRect( 3,3, r.height-3, r.height-3 ) );
  }
  hDC->SetColor( ColorElementText );
  hDC->SetTextAlign( TA_LEFT|TA_MIDDLE );
  hDC->DrawText( GetWindowText(), CRect( r.height+5, 0, r.width-r.height-5, r.height ) );
  ReleaseDC( hDC );
}


