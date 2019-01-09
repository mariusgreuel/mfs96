//
// text.cpp
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include "mywin.hpp"
#include "text.hpp"

/////////////////////////////////////////////////////////////////////////////
// class Text

void CText::Draw()
{
  CDC *hDC = GetDC();
  CRect r;
  GetClientRect( r );
  hDC->SetColor( Color );
  hDC->SetTextAlign( Align );
  hDC->DrawText( GetWindowText(), r );
  ReleaseDC( hDC );
}


