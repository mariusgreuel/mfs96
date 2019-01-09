//
// mywin.cpp, tiny windows managment
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "keyboard.hpp"
#include "keys.h"
#include "context.hpp"
#include "cursor.hpp"
#include "mywin.hpp"

#define ColorBorder1            BLACK
#define ColorBorder2            GRAY
#define ColorBackground         GRAY

extern CKeyboard Keyboard;
extern CMouse    Mouse;

CCursor MouseCursor[2] = {
  CCursor( 20,20 ),
  CCursor( 20,20 ),
};

CWnd *CWnd::MainWindow = NULL;
CWnd *CWnd::FocusWindow = NULL;
CWnd *CWnd::CaptureWindow = NULL;
DWORD CWnd::ReturnID = ID_FALSE;

extern int Retrace;

/////////////////////////////////////////////////////////////////////////////
// class Wnd

/////////////////////////////////////////////////////////////////////////////
// Konstruktion, Initialisierung, Destruktion

// CWnd - Hauptfunktion
CWnd::CWnd() : CRect( 0,0,0,0 ) 
{
  WindowFlag = WF_ACTIVE;
  RedrawFlag = 0;
  Client = CRect( 0,0,0,0 );
}

// Fenster erzeugen
BOOL CWnd::Create( const char *WindowName, DWORD Style, const CRect &Rect, CWnd *Parent )
{
  if( Parent == NULL )
    Parent = MainWindow;
  CWnd::Style = Style;
  CWnd::Parent = Parent;
  if( Parent )
    Parent->ChildList.Insert( this );
  else
    MainWindow = this;
  SetWindowText( WindowName );
  SetWindowPos( CPoint( Rect.x, Rect.y ));
  SetWindowSize( CSize( Rect.width, Rect.height ));
  if( Style & WS_BORDER )
    ResizeClientWindow( -BORDER_WIDTH, RR_ALL );
  else if( Style & WS_THICKFRAME )
    ResizeClientWindow( -THICK_FRAME_WIDTH, RR_ALL );
  if( OnCreate() ) {
    delete this;
    return FALSE;
  }
  if( Style & WS_CAPTION )
    SetCaption();
  RedrawWindow();
  return TRUE;
}

// Fenster entfernen
CWnd::~CWnd()
{
  CWnd *Wnd;
  while( ( Wnd = ChildList.Find()) != NULL )
    delete Wnd;
  if( Parent )
    Parent->ChildList.Get( this );
  else
    MainWindow = NULL;
  if( MainWindow )
    MainWindow->RedrawWindow();
}

/////////////////////////////////////////////////////////////////////////////
// Fensterzeichenfunktionen

// Dünnen Rahmen zeichen
void CWnd::DrawBorder()
{
  CDC *hDC = GetWindowDC();
  CRect r;
  GetWindowRect( r );
  hDC->SetColor( ColorBorder1 );
  hDC->DrawRect( r );
  ReleaseDC( hDC );
}

// Dicken Rahmen zeichen
void CWnd::DrawThickFrame()
{
  CDC *hDC = GetWindowDC();
  CRect r;
  GetWindowRect( r );
  for( char i=0; i<THICK_FRAME_WIDTH; i++ ) {
    if( i == 0 ) {
      hDC->SetColor( ColorBorder1 );
      hDC->DrawRect( r );
    } else if( i==1 )
      hDC->Draw3DFrame( r, DF_OUT + 1 );
    else {
      hDC->SetColor( ColorBorder2 );
      hDC->DrawRect( r );
    }
    r.left++; r.top++; r.right--; r.bottom--;
  }
  ReleaseDC( hDC );
}

// Fensterhintergrund zeichen
void CWnd::DrawBackground()
{
  CDC *hDC = GetDC();
  CRect r;
  GetClientRect( r );
  hDC->SetColor( ColorBackground );
  hDC->FillRect( r );
  ReleaseDC( hDC );
}

/////////////////////////////////////////////////////////////////////////////
// Positions- und Größenänderung

// Die Außmaße eines Fenster aufgrund des Clientbereichs berechen
void CWnd::CalcWindowRect( CRect &r, DWORD Style ) const
{
  if( Style & WS_BORDER ) {
    r.width += 2*BORDER_WIDTH;
    r.height += 2*BORDER_WIDTH;
  } else if( Style & WS_THICKFRAME ) {
    r.width += THICK_FRAME_WIDTH;
    r.height += 2*THICK_FRAME_WIDTH;
  }
  if( Style & WS_CAPTION )
    r.height += CAPTION_HEIGHT;
}

// Position des Fensters setzen
void CWnd::SetWindowPos( CPoint p, BYTE Flag )
{
  CPoint dp;
  if( Flag & SW_ABSOLUTE ) {
    dp.x = p.x - x;
    dp.y = p.y - y;
  } else if( Flag & SW_RELATIVE ) {
    dp.x = p.x;
    dp.y = p.y;
  } else if( Flag & SW_CENTER ) {
    CRect r;
    if( Parent && Style & WS_CHILD )
      Parent->GetClientRect( r );
    else
      MainWindow->GetWindowRect( r );
    dp.x = ( r.right - r.left - width ) / 2 - x;
    dp.y = ( r.bottom - r.top - height ) / 2 - y;
  } else if( Parent && Style & WS_CHILD ) {
    dp.x = Parent->Client.x + p.x - x;
    dp.y = Parent->Client.y + p.y - y;
  } else {
    dp.x = p.x - x;
    dp.y = p.y - y;
  }
  if( dp.x || dp.y ) {
    x += dp.x;
    y += dp.y;
    Client.x += dp.x;
    Client.y += dp.y;
    OnMove( p );
    for( CIDIter<CWnd> Wnd( ChildList ); Wnd(); Wnd++ )
      if( Wnd()->Style & WS_CHILD )
        Wnd()->SetWindowPos( dp, SW_RELATIVE );
    MainWindow->RedrawWindow();
  }
}

// Größe des Fensters setzen
void CWnd::SetWindowSize( CSize s, BYTE Flag )
{
  CSize ds;
  if( Flag & SW_RELATIVE ) {
    ds.cx = s.width;
    ds.cy = s.height;
  } else {
    ds.cx = s.width - width;
    ds.cy = s.height - height;
  }
  if( ds.cx || ds.cy ) {
    width += ds.cx;
    height += ds.cy;
    Client.width += ds.cx;
    Client.height += ds.cy;
    OnSize( s );
    MainWindow->RedrawWindow();
  }
}

// Clientfenster beschneiden
void CWnd::ResizeClientWindow( short Pixel, BYTE Edge )
{
  if( Edge & RR_LEFT ) {
    Client.left -= Pixel;
    Client.width += Pixel;
  }
  if( Edge & RR_TOP ) {
    Client.top -= Pixel;
    Client.height += Pixel;
  }
  if( Edge & RR_RIGHT )
    Client.width += Pixel;
  if( Edge & RR_BOTTOM )
    Client.height += Pixel;
}

/////////////////////////////////////////////////////////////////////////////
// Darstellung

// Fenster neuzeichnen
void CWnd::UpdateWindow( DWORD ParentFlag )
{
  if( RedrawFlag & RDW_INVALID1 )
    RedrawFlag &= ~RDW_INVALID1;
  else
    RedrawFlag &= ~RDW_INVALID2;
  if( RedrawFlag & RDW_CLIENT || ParentFlag & RDW_CLIENT )
    OnPaint();
  if( RedrawFlag & RDW_NONCLIENT || ParentFlag & RDW_NONCLIENT )
    OnNCPaint();
  if( RedrawFlag & RDW_CHILDREN )
    ParentFlag |= RedrawFlag;
  for( CIDIter<CWnd> Wnd( ChildList, -1 ); Wnd(); Wnd-- )
    Wnd()->UpdateWindow( ParentFlag );
  if( !( RedrawFlag & RDW_INVALID ) )
    RedrawFlag = 0;
}

// Fenster zum Neuzeichnen vorbereiten
void CWnd::RedrawWindow( DWORD Flags )
{
  RedrawFlag = Flags|RDW_INVALID;
}

void CWnd::OnNCPaint()
{
  if( !(Style & WS_VISIBLE ))
    return;
  if( Style & WS_BORDER )
    DrawBorder();
  else if( Style & WS_THICKFRAME )
    DrawThickFrame();
  if( Style & WS_BACKGROUND )
    DrawBackground();
}

/////////////////////////////////////////////////////////////////////////////
// Prozessfunktionen

// Sendet Tastaturmeldungen
void CWnd::SendKeyboardMessages( DWORD Key )
{
  OnChar( Key );
}

// Sendet Mausmeldungen
void CWnd::SendMouseMessages()
{
  CPoint Pos = Mouse.Pos - CPoint( Client.x, Client.y );
  if( Mouse.Action & MA_MOUSEMOVE )
    OnMouseMove( Mouse.Key, Pos );
  if( Mouse.Action & MA_LBUTTONDOWN )
    OnLButtonDown( Mouse.Key, Pos );
  if( Mouse.Action & MA_RBUTTONDOWN )
    OnRButtonDown( Mouse.Key, Pos );
  if( Mouse.Action & MA_LBUTTONUP )
    OnLButtonUp( Mouse.Key, Pos );
  if( Mouse.Action & MA_RBUTTONUP )
    OnRButtonUp( Mouse.Key, Pos );
}

// verabeitet Tastaturmeldungen
BOOL CWnd::ProcessKeyboardMessages( DWORD Key )
{
  if( !ChildList.IsEmpty() ) {
    for( CIDIter<CWnd> Wnd( ChildList ); Wnd(); Wnd++ )
      if( Wnd()->ProcessKeyboardMessages( Key ) )
        return TRUE;
  }
  SendKeyboardMessages( Key );
  return FALSE;
}

// verabeitet Mausmeldungen
BOOL CWnd::ProcessMouseMessages()
{
  if( !ChildList.IsEmpty() ) {
    for( CIDIter<CWnd> Wnd( ChildList ); Wnd(); Wnd++ )
      if( Wnd()->ProcessMouseMessages() )
        return TRUE;
  }
  if( IsInWindowRect( Mouse.Pos ) ) {
//    if( IsInClientRect( Mouse.Pos - CPoint( Client.x, Client.y ) ) )
      SendMouseMessages();
    return TRUE;
  } else if( this == FocusWindow )
    SendMouseMessages();
  return FALSE;
}

// prüft Tastatur und Maus auf Ereignisse und versendet ggf. Meldungen
void ProcessMessages()
{
  static int p=0;
  _SetVisiblePage( p );
  _SetActivePage( p^= 1 );
  if( Retrace )
    _WaitForRetrace();
  if( Mouse.IsInstalled() )
    MouseCursor[p].Hide();
  CWnd::MainWindow->UpdateWindow();
  if( Mouse.IsInstalled() )
    MouseCursor[p].Move( Mouse.Pos );
  if( Keyboard.IsInstalled() ) {
    while( Keyboard.KeyPressed() ) {
      KEY Key = Keyboard.GetVKey();
      switch( LOWORD( Key )) {
        case VK_ESCAPE : SetEscapeFlag(); break;
      }
      if( CWnd::FocusWindow )
        CWnd::FocusWindow->ProcessKeyboardMessages( Key );
      else
        CWnd::MainWindow->ProcessKeyboardMessages( Key );
    }
  }
  if( Mouse.IsInstalled() ) {
    Mouse.Read();
    if( CWnd::CaptureWindow )
      CWnd::CaptureWindow->SendMouseMessages();
    else if( CWnd::FocusWindow )
      CWnd::FocusWindow->ProcessMouseMessages();
    else
      CWnd::MainWindow->ProcessMouseMessages();
  }
}

// setzt den Returncode
void CWnd::SetReturnID( DWORD ID )
{
  ReturnID = ID;
}

// wartet bis der Returncode gesetzt wird und gibt diesen zurück
DWORD CWnd::GetReturnID( BOOL Modal )
{
  if( Modal )
    SetFocus();
  ReturnID = ID_FALSE;
  while( ReturnID == ID_FALSE )
    ProcessMessages();
  ReturnID = ID_FALSE;
  if( Modal )
    ReleaseFocus();
  return ReturnID;
}

/////////////////////////////////////////////////////////////////////////////
// Verschiedenes und Müll

BOOL Escape = FALSE;
void SetEscapeFlag() { Escape = TRUE; }
BOOL GetEscapeFlag() { return Escape; }


