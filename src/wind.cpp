//
// wind.cpp
//
// Copyright Marius Greuel 1996. All rights reserved
//

#include "vars.hpp"
#include "vecmath.hpp"
#include "wind.hpp"

CVec16 WindSpeed;

void CalcWind()
{
  if( !WindOn ) {
    WindSpeed = CVec16( 0.0,0.0,0.0 );
    return;
  }
  FIX16 Lenght = WindBaseSpeed;
  FIX16 Angle = WindBaseDir;
  WindSpeed.v[X] = 0.0;//Lenght * cos( Angle / 360.0 * _2PI );
  WindSpeed.v[Y] = 0.0;
  WindSpeed.v[Z] = 0.0;//Lenght * sin( Angle / 360.0 * _2PI );
}
