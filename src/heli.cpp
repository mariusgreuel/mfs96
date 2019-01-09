//
// heli.cpp
//
// Copyright Marius Greuel 1996. All rights reserved
//

#include "vars.hpp"
#include "vecmath.hpp"
#include "heli.hpp"

#define ROLL    0
#define GIER    1
#define NICK    2
#define PITCH   3

extern FIX16 FrameTime;
extern FIX16 Stick[4];
extern CVec16 WindSpeed;

void Disp( const char *format, FIX16 f1 );
void Disp( const char *format, FIX16 f1, FIX16 f2 );
void Disp( const char *format, FIX16 f1, FIX16 f2, FIX16 f3 );
void Disp( const char *format, FIX30 f1, FIX30 f2, FIX30 f3 );

/////////////////////////////////////////////////////////////////////////////

CHelicopter::CHelicopter()
{
  Init();
}

void CHelicopter::Init()
{
  MotorRPM = 0.0;
  GroundSpeed = CVec16( 0.0, 0.0, 0.0 );
  RotateSpeed = CVec16( 0.0, 0.0, 0.0 );
}

void CHelicopter::CalcFlightPath( CMatrix30 &m )
{
  int i,j;
  Disp( "Time:", FrameTime );

  for( i=0; i<3; i++ )
    Disp( "M.v", m.v[i].v[0], m.v[i].v[1], m.v[i].v[2] );
  Disp( "M.h", m.h.v[0], m.h.v[1], m.h.v[2] );

  FIX16 MotorRPM = Stick[ PITCH ] + 1;
  Disp( "MotorRPM %f", MotorRPM );
  AirSpeed = GroundSpeed - WindSpeed;
  FIX16 AirSpeedZ = dot( m.v[Z], AirSpeed );

  FIX16 GroundEffect = 1 + GroundEffectFactor / ( 1 + m.v[H].v[Y] / GroundEffectRange  );
  Disp( "Groundeffect: %6.3f", GroundEffect );

  for( i=0; i<3; i++ )
    Moment.v[i] = Stick[i] * Sensitivity[i];

  Moment.v[Y] += AirSpeedZ * TurnWithWind[Y];

//  Force = m.v[Y] * EnginePower * MotorRPM * GroundEffect;
  Force.v[Y] -= Mass * Gravity;

  for( i=0; i<3; i++ ) {
    RotateSpeed.v[i] += Moment.v[i] / Inertia[i] * FrameTime;
    FIX16 SpinValue = Spin[i] / Inertia[i] * FrameTime;
    if( SpinValue < 1 )
      RotateSpeed.v[i] *= 1 - SpinValue;
    else
      RotateSpeed.v[i] = 0.0;
  }
  for( i=0; i<3; i++ ) {
    GroundSpeed.v[i] += Force.v[i] / MassInertia * FrameTime;
    FIX16 DragValue = 0.0;
    for( j=0; j<3; j++ )
      DragValue += AirSpeed.v[i] * abs( m.v[j].v[i] ) * Drag[j] / MassInertia * FrameTime;
    if( abs( DragValue ) < abs( GroundSpeed.v[i] ) )
      GroundSpeed.v[i] -= DragValue;
    else
      GroundSpeed.v[i] = 0.0;
  }
}

// Bewegungsvektor holen
CVec16 CHelicopter::GetMoveVector()
{
  return GroundSpeed * FrameTime;
}

// Rotierungsvektor holen
CVec16 CHelicopter::GetRotateVector()
{
  return RotateSpeed * FrameTime;
}


