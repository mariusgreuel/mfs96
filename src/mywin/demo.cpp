//
// demo.cpp
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include "memory.hpp"
#include "keyboard.hpp"
#include "graphic.hpp"
#include "mywin.hpp"
#include "elements.hpp"

// hier befindet sich die gesamten statischen Daten mit Constructoren
#pragma initialize library before;
CMemory   Memory;
CKeyboard Keyboard;
CMouse    Mouse;
CGraphic  Graphic;

int Retrace = 1;

char *PrgName =   "Kleines Fenster Demo Version 1.0";
char *Copyright = "Copyright (c) Marius Greuel 1995.";

enum {
  CM_PROGRAMEND = 999
};

void SetupMenu( CWnd *Parent )
{
  CMenu *Menu = new CMenu;
    CMenu *File = Menu->AppendMenu( "Datei", 100 );
      File->AppendItem( "Item A", 101 );
      File->AppendItem( "Item B", 102 );
      File->AppendItem( "Item C", 103 );
      CMenu *UM = File->AppendMenu( "Untermenue", 104 );
        UM->AppendItem( "Item A", 401 );
        UM->AppendItem( "Item B", 402 );
        UM->AppendItem( "Item C", 403 );
    CMenu *Edit = Menu->AppendMenu( "Bearbeiten", 200 );
      Edit->AppendItem( "Item A", 201 );
      Edit->AppendItem( "Item B", 202 );
      Edit->AppendItem( "Item C", 203 );
    Menu->AppendItem( "Beenden!", CM_PROGRAMEND );
  Parent->SetMenu( Menu );
}

class DemoWindow : public CWnd {
public:
  void OnPaint( void );
  void OnMouseMove( WORD, CPoint );
  void OnLButtonDown( WORD, CPoint );
  void OnCommand( DWORD );
  DemoWindow( void );
};

void DemoWindow::OnPaint( void )
{
  static int count;
  static char c=0;
  char mess[128];
  sprintf( mess, "Kleines Demo: %u !", count++ );
  
  CDC *hDC = GetDC();
  CRect r( 20, 20, 100, 100 );
  hDC->SetColor( c++ );
  hDC->DrawText( mess, r );
  ReleaseDC( hDC );
}

void DemoWindow::OnCommand( DWORD Cmd )
{
  char mess[128];
  switch( Cmd ) {
    case ID_CANCEL:
    case CM_PROGRAMEND:
      SetEscapeFlag(); break;
    default:
      sprintf( mess, "Button mit ID %u gedrueckt !", Cmd );
      MessageBox( mess, "Demo", MB_OK );
      break;
  }
}

void DemoWindow::OnMouseMove( WORD, CPoint )
{
  CDC *hDC = GetDC();
  CRect r( 20, 20, 100, 100 );
  hDC->SetColor( 12 );
  hDC->DrawText( "Moved", r );
  ReleaseDC( hDC );
}
  
void DemoWindow::OnLButtonDown( WORD, CPoint )
{
  MessageBox( "10 MByte locker machen...", PrgName, MB_OK );
  char *x = new char[10*1024*1024];

}

DemoWindow::DemoWindow( void )
{
  Create( PrgName, WS_VISIBLE|WS_THICKFRAME|WS_CAPTION,
          CRect( 0,0, 640, 350 ), NULL );
  SetupMenu( this );
  CStatusBar *StatusBar = new CStatusBar;
  StatusBar->Create( this );
  CSlider *Slider = new CSlider;
  Slider->Create( "Wert1: ", CRect( 100,100, 200,20 ), this );
  CSwitchButton *Switch = new CSwitchButton;
  Switch->Create( "Schalter", CRect( 100,150, 200,14 ), this );
}

/////////////////////////////////////////////////////////////////////////////

int main( int, char *[] )
{
  cout << PrgName << '\n' << Copyright << '\n';
  Keyboard.Detect( 1 );
  Mouse.Detect( 1 );
  Graphic.Detect( 1 );
  Keyboard.Startup();
  Mouse.Startup();
  Graphic.Startup();
  try {
    Graphic.SetGraphMode();
    CWnd *MainWindow = new CWnd();
    CRect r; Graphic.GetScreenRect( r );
    MainWindow->Create( "MAIN", WS_VISIBLE|WS_BACKGROUND, r, NULL );
    CWnd *Wnd = new DemoWindow;
    while( !GetEscapeFlag() ) {
      ProcessMessages();
    }
    delete Wnd;
    delete MainWindow;
    Graphic.SetTextMode();
  }
  catch( CMemory::Exhausted ) {
    Graphic.SetTextMode();
    cerr << "Sorry, memory exhausted !\n";
    return EXIT_FAILURE;
  }
  catch(...) {
    Graphic.SetTextMode();
    cerr << "Unknown exception caught !\n";
    return EXIT_FAILURE;
  }
  return 0;
}


