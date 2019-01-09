//
// slide.cpp
//
// Copyright Marius Greuel 1996. All rights reserved.
// 

#include <stdio.h>
#include "mywin.hpp"
#include "slider.hpp"

#define ColorElementBack        GRAY
#define ColorElementFace        BLACK
#define ColorElementText        BLACK
#define ColorElement            BLACK

#define SLIDE_BAR_WIDTH         100
#define SLIDE_TEXT_WIDTH        40
#define SLIDER_WIDTH            8

/////////////////////////////////////////////////////////////////////////////
// class Slider

// Schieber zeichnen
void CSlider::Draw()
{
  CDC *hDC = GetDC();
  CRect r;
  GetClientRect( r );
  hDC->SetColor( ColorElementBack );
  hDC->FillRect( r );
  hDC->SetColor( ColorElementText );
  hDC->SetTextAlign( TA_LEFT|TA_MIDDLE );
  hDC->DrawText( GetWindowText(), r );
  char ValBuf[10];
  if( Max - Min < 2.1 )
    sprintf( ValBuf, "%2.2f", Value );
  else if( Max - Min < 21.0 )
    sprintf( ValBuf, "%3.1f", Value );
  else
    sprintf( ValBuf, "%4.0f", Value );
  hDC->SetTextAlign( TA_RIGHT|TA_MIDDLE );
  hDC->DrawText( ValBuf, r );
  r.left = r.right-SLIDE_TEXT_WIDTH-SLIDE_BAR_WIDTH-SLIDER_WIDTH/2;
  hDC->SetColor( ColorShadowDark );
  hDC->DrawLineH( r.left, r.bottom - r.top >> 1, r.left+SLIDE_BAR_WIDTH+SLIDER_WIDTH );
  hDC->SetColor( ColorShadowLight );
  hDC->DrawLineH( r.left, r.bottom - r.top >> 1, r.left+SLIDE_BAR_WIDTH+SLIDER_WIDTH );
#ifdef MYFIX
  r.left += (int)(( Value - Min ) / ( Max - Min ) * SLIDE_BAR_WIDTH + 0.5 ).Int();
#else
  r.left += (int)(( Value - Min ) / ( Max - Min ) * SLIDE_BAR_WIDTH + 0.5 );
#endif
  r.right = r.left + SLIDER_WIDTH;
  hDC->SetColor( ColorElementBack );
  hDC->FillRect( r );
  hDC->SetColor( ColorElementFace );
  hDC->Draw3DFrame( r );
  ReleaseDC( hDC );
}

void CSlider::OnMouseMove( WORD, CPoint p )
{
  if( Flag & CAPTURED ) {
    CRect r;
    GetClientRect( r );
    p.x -= r.right-SLIDE_TEXT_WIDTH-SLIDE_BAR_WIDTH;
    if( p.x <= 0 )
      SetValue( Min );
    else if( p.x >= SLIDE_BAR_WIDTH )
      SetValue( Max );
    else
      SetValue( Min + ( Max - Min ) * p.x / SLIDE_BAR_WIDTH );
  }
}

void CSlider::OnLButtonDown( WORD Key, CPoint p )
{
  CRect r;
  GetClientRect( r );
  if( p.x >= r.right-SLIDE_TEXT_WIDTH-SLIDE_BAR_WIDTH && p.x <= r.right-SLIDE_TEXT_WIDTH ) {
    Flag |= CAPTURED;
    SetCapture();
    OnMouseMove( Key, p );
  }
}

void CSlider::OnLButtonUp( WORD, CPoint )
{
  if( Flag & CAPTURED ) {
    Flag &= ~CAPTURED;
    ReleaseCapture();
  }
}

