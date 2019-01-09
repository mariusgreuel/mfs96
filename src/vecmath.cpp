//
// vecmath.cpp, Vektor und Matritzenrechnung
//
// Copyright Marius Greuel 1996. All rights reserved
//
// alle Matritzen sind homogen der Form m[4][3] == CVector[4]
//

#include "vecmath.hpp"

extern "C" void MulMatrix( const CMatrix30 *, const CMatrix16 *, CMatrix16 * );
#pragma aux MulMatrix parm [esi][edi][ebx];
extern "C" void RotateMatrix30ByAxe( CMatrix30 *, const CVec16 *, long, long );
#pragma aux RotateMatrix30ByAxe parm [eax][ebx][ecx][edx];

/////////////////////////////////////////////////////////////////////////////
// CMatrix30

// Einheitsmatrix erstellen
void CMatrix30::Init()
{
  for( int i=0; i<3; i++ )
    for( int j=0; j<3; j++ )
      v[i].v[j] = i == j ? 1.0 : 0.0;
  for( i=0; i<3; i++ )
    h.v[i] = 0.0;
}

// Matrix um eine Rotationsaxe rotieren
// Betrag des Vectors entspricht dem Winkel
void CMatrix30::RotateByAxe( const CVec16 &v )
{
  FIX16 Phi = abs( v );
  RotateMatrix30ByAxe( this, &v, fsin( Phi ).Fix(), fcos( Phi ).Fix() );
}

/////////////////////////////////////////////////////////////////////////////
// CMatrix16

// Einheitsmatrix erstellen
void CMatrix16::Init()
{
  for( int i=0; i<4; i++ )
    for( int j=0; j<3; j++ )
      v[i].v[j] = i == j ? 1.0 : 0.0;
}

// Matrix um Ursprung rotieren
void CMatrix16::Rotate( int c1, int c2, FIX16 _sin, FIX16 _cos )
{
  for( int i=0; i<4; i++ ) {
    FIX16 temp   = _sin * v[i].v[c1] + _cos * v[i].v[c2];
    v[i].v[c1] = _cos * v[i].v[c1] - _sin * v[i].v[c2];
    v[i].v[c2] = temp;
  }
}

// Matrix mittels Vektor skalieren
CMatrix16 &CMatrix16::operator *= ( CVec16 vec )
{
  for( int i=0; i<4; i++ )
    for( int j=0; j<3; j++ )
      v[i].v[j] *= vec.v[j];
  return *this;
}

// Matrizen multiplizieren
CMatrix16 operator * ( const CMatrix30 &m1, const CMatrix16 &m2 )
{
  CMatrix16 m;
  MulMatrix( &m1, &m2, &m );
/*
  for( int i=0; i<3; i++ ) {
    for( int j=0; j<3; j++ ) {
      FIX16 d = 0.0;
      for( int k=0; k<3; k++ )
        d += m1.v[i].v[k] * m2.v[k].v[j];
      m.v[i].v[j] = d;
    }
  }
  for( int j=0; j<3; j++ ) {
    FIX16 d = 0.0;
    for( int k=0; k<3; k++ )
      d += m1.h.v[k] * m2.v[k].v[j];
    d += m2.v[3].v[j];
    m.v[i].v[j] = d;
  }
*/
  return m;
}

