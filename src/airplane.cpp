//
// airplane.cpp
//
// Copyright Marius Greuel 1996. All rights reserved
//

#include "timer.hpp"
#include "rcbox.hpp"
#include "vars.hpp"
#include "vecmath.hpp"
#include "object.hpp"
#include "heli.hpp"
#include "wind.hpp"
#include "airplane.hpp"

extern CObject *Airplane;
extern CTimer Timer;
extern CRCBox RCBox;

CHelicopter Helicopter;

/////////////////////////////////////////////////////////////////////////////
#include <stdio.h>

extern void DrawTextXY( const char *Text, short x, short y );
extern "C" void _SetColor( short Color );
int Row = 0;

void Disp( const char *format, FIX16 f1 )
{
  if( Row >= 25 )
    return;
  char b[80];
  sprintf( b, "%s %8.4f", format, f1.Float() );
  _SetColor( 15 );
  DrawTextXY( b, 10, 10 * Row + 40 );
  Row++;
}
void Disp( const char *format, FIX16 f1, FIX16 f2 )
{
  if( Row >= 25 )
    return;
  char b[80];
  sprintf( b, "%s %8.4f, %8.4f", format, f1.Float(), f2.Float() );
  _SetColor( 15 );
  DrawTextXY( b, 10, 10 * Row + 40 );
  Row++;
}
void Disp( const char *format, FIX16 f1, FIX16 f2, FIX16 f3 )
{
  if( Row >= 25 )
    return;
  char b[80];
  sprintf( b, "%s %8.4f, %8.4f, %8.4f", format, f1.Float(), f2.Float(), f3.Float() );
  _SetColor( 15 );
  DrawTextXY( b, 10, 10 * Row + 40 );
  Row++;
}
void Disp( const char *format, FIX30 f1, FIX30 f2, FIX30 f3 )
{
  if( Row >= 25 )
    return;
  char b[80];
  sprintf( b, "%s %12.8Lf, %12.8Lf, %12.8Lf", format, f1.Double(), f2.Double(), f3.Double() );
  _SetColor( 15 );
  DrawTextXY( b, 10, 10 * Row + 40 );
  Row++;
}
/////////////////////////////////////////////////////////////////////////////

FIX16 FrameTime, RealFrameTime;
FIX16 Stick[4];

void CalcTime()
{
  if( Timer.IsInstalled() ) {
    RealFrameTime = Timer.Read() * 0.838e-6;
    Timer.Reset();
    if( RealFrameTime > 0.2 )
      RealFrameTime = 0.2;
  } else
    RealFrameTime = 0.14;
  FrameTime = RealFrameTime * TimeAdjust;
}

BOOL CalcStickValues()
{
  if( !RCBox.IsInstalled() )
    return FALSE;
  if( !RCBox.DataValid() )
    return FALSE;
  int s[4] = { GIER, PITCH, ROLL, NICK };
  for( int i=0; i<4; i++ ) {
    Stick[ s[i] ] = RCBox.Get( i ) / 256.0;
    if( !RCBoxNegChannel[i] )
      Stick[ s[i] ] *= -1;
  }
  return TRUE;
}

void InitAirplane()
{
  Timer.Reset();
  Helicopter.Init();
}

void CalcAirplane()
{
  Row = 0;
  CalcTime();
  CalcWind();
  Helicopter.CalcFlightPath( Airplane->Matrix );
  if( CalcStickValues() == FALSE )
    return;
  Airplane->Move( Helicopter.GetMoveVector() );
  Airplane->Rotate( Helicopter.GetRotateVector() );
  if( Airplane->Matrix.h.v[Y] < 0.0 ) {
    Airplane->Matrix.h.v[Y] = 0.0;
    Helicopter.Init();
  }
}


