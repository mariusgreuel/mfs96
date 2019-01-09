//
// mfs.cpp, Modellflugsimulator V1.0a
//
// Copyright Marius Greuel 1996. All rights reserved
//

#include "mywin.hpp"
#include "airplane.hpp"
#include "mfs.hpp"

#include <conio.h>

void InitMfs()
{
  CWnd *MainWindow = new CWnd();
  MainWindow->Create( "MAIN", WS_VISIBLE|WS_BACKGROUND, CRect( 0,0,640,350 ), NULL );
  CWnd *Wnd = new MfsWindow;
  InitAirplane();
  while( !GetEscapeFlag() ) {
    if( Wnd->GetFocus() == NULL ) {
      CalcAirplane();
      Wnd->RedrawWindow( RDW_CLIENT );
    }
    ProcessMessages();
//    getch();
  }
  delete Wnd;
  delete MainWindow;
}

