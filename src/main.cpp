//
// main.cpp, initialisation of static modules
//
// Copyright Marius Greuel 1996. All rights reserved
//

#include <stdlib.h>
#include <conio.h>
#include <iostream.h>
#include "debug.hpp"
#include "except.hpp"
#include "error.hpp"
#include "memory.hpp"
#include "chkwin95.hpp"
#include "timer.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "rcbox.hpp"
#include "graphic.hpp"
#include "fixmath.hpp"
#include "cmdline.hpp"
#include "script.hpp"

extern void InitMfs();

// static data that needs prior init
#pragma initialize library
CException Exception;
CMemory    Memory;
CTimer     Timer;
CKeyboard  Keyboard;
CMouse     Mouse;
CRCBox     RCBox;
CGraphic   Graphic;
CFixMath   FixMath;
CCmdLine   CmdLine;

// command line switches
#pragma initialize program
CCmdLineSwitch SwitchDebug1     ( "d1" );
CCmdLineSwitch SwitchDebug2     ( "d2" );
CCmdLineSwitch SwitchDebug3     ( "d3" );
CCmdLineSwitch SwitchNoTimer    ( "dt" );
CCmdLineSwitch SwitchNoMouse    ( "dm" );
CCmdLineSwitch SwitchNoRCBox    ( "drc" );
CCmdLineSwitch IgnoreWindows    ( "iw" );
CCmdLineSwitch SwitchRetrace    ( "r" );
CCmdLineSwitch SwitchHelp1      ( "h" );
CCmdLineSwitch SwitchHelp2      ( "?" );

const char *MFS =
    "Modellflugsimulator Version 1.0\n"
    "Copyright (c) Marius Greuel 1995. All rights reserved.";
const char *DOS4GW =
    "DOS/4GW Protected Mode Run-time Version 1.97\n"
    "Copyright (c) Rational Systems, Inc. 1990-1994";
static const char *HelpString = 
    "Syntax: MFS [/Options] [ScriptFile[.if]]\n"
    "Options:\n"
    "\t/r\tdon't wait for screen retrace\n"
    "\t/?\tdisplay this help message\n";

extern "C" BOOL DefaultExceptionHandler()
{
  Graphic.SetTextMode();
  Graphic.Shutdown();
  RCBox.Shutdown();
  Mouse.Shutdown();
  Keyboard.Shutdown();
  Timer.Shutdown();
  return TRUE;          // Can't handle exception
}

int Retrace = 1;

int main( int argc, char *argv[] )
{
  try {
    cerr << MFS << '\n';
    CmdLine.Evaluate( argc, argv );
    if( SwitchHelp1.IsOn() || SwitchHelp2.IsOn() ) {
      cout << HelpString;
      return EXIT_SUCCESS;
    }
    if( IgnoreWindows.IsOff() && IsWindowsRunning() ) {
      cerr << "This program was not designed to run properly under Windows!\n" \
              "Press enter to continue, any other key to abort.\n";
      if( getch() != 0xd )
        return EXIT_FAILURE;
    }
    if( SwitchDebug1.IsOn() )
      SetDebugLevel( 1 );
    else if( SwitchDebug2.IsOn() )
      SetDebugLevel( 2 );
    else if( SwitchDebug3.IsOn() )
      SetDebugLevel( 3 );
    if( SwitchRetrace.IsOn() )
      Retrace = 0;
    Timer.Detect( SwitchNoTimer.IsOff() );
    Keyboard.Detect( 1 );
    Mouse.Detect( SwitchNoMouse.IsOff() );
    RCBox.Detect( SwitchNoRCBox.IsOff() );
    Graphic.Detect( 1 );
    if( !Graphic.IsInstalled() )
      throw CError( "VGA card required!" );
    Timer.Startup();
    Keyboard.Startup();
    Mouse.Startup();
    RCBox.Startup();
    Graphic.Startup();
    EvalScriptFile( CmdLine.GetArg( 0 ) );
    Graphic.SetGraphMode();
    InitMfs();
    Graphic.SetTextMode();
  }
  catch( CError e ) {
    Graphic.SetTextMode();
    cerr << e.Message << '\n';
    return EXIT_FAILURE;
  }
  catch( CMemory::Exhausted ) {
    Graphic.SetTextMode();
    cerr << "Sorry, memory exhausted!\n";
    return EXIT_FAILURE;
  }
  catch(...) {
    Graphic.SetTextMode();
    cerr << "Unknown exception caught!\n";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

