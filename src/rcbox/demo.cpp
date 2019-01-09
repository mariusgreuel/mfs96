//
// Demo.cpp, read rcbox from serial port
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include <iostream.h>
#include <conio.h>
#include "debug.hpp"
#include "timer.hpp"
#include "rcbox.hpp"

int main()
{
  CTimer Timer;
  CRCBox RCBox;
  
  SetDebugLevel( 1 );
  cout << "Test RC-BOX\n";
  Timer.Detect( 1 );
  RCBox.Detect( 1 );
  if( !RCBox.IsInstalled() )
    cout << "RCBox not found!\n";
  Timer.Startup();
  RCBox.Startup();
  while( !kbhit()) {
    if( RCBox.DataValid() )
      cout << "Data good => ";
    else
      cout << "Data bad  => ";
    cout << dec << "RM: " << RM_DataPtr->RM_Count << ", PM: " << RM_DataPtr->PM_Count << ",  ";
    for( int x=0; x<CHANNELS; x++ ) {
      cout.width( 4 );
      cout << RCBox.Get( x );
    }
    cout << '\r';
  }
  cout << "\n";
  getch();
  return 0;
}
