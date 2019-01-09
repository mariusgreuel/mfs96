//
// fixmath.cpp
//
// Copyright Marius Greuel 1996. All rights reserved
//

#include <math.h>
#include "fixmath.hpp"

#define SINUS_TABLE_SIZE 4096

long *CFixMath::SinusTable;

/////////////////////////////////////////////////////////////////////////////
// CFixMath

// Sinus Tabelle aufbauen
CFixMath::CFixMath()
{
  SinusTable = new long[ SINUS_TABLE_SIZE+1 ];
  double Phi = 0;
  double PhiInc = _PI2 / SINUS_TABLE_SIZE;
  long *TableEntry = SinusTable;
  for( int i=0; i<SINUS_TABLE_SIZE+1; i++ ) {
    *TableEntry = (long)( sin( Phi ) * ( 1 << 30 ) );
    Phi += PhiInc;
    TableEntry++;
  }
}

// Sinus Tabelle löschen
CFixMath::~CFixMath()
{
  delete[] SinusTable;
}

// Wert aus Sinus Tabelle holen
long CFixMath::GetSinusTableEntry( int p )
{
  p &= SINUS_TABLE_SIZE * 4 - 1;        // 360° maskieren
  if( p <= SINUS_TABLE_SIZE * 2 ) {     // <= 180° ?
    if( p <= SINUS_TABLE_SIZE )         // <= 90° ?
      return *( SinusTable + p );
    else
      return *( SinusTable + SINUS_TABLE_SIZE * 2 - p );
  } else {                              // ist > 180°
    if( p <= SINUS_TABLE_SIZE * 3 )      // <= 270° ?
      return -*( SinusTable + p - SINUS_TABLE_SIZE * 2 );
    else
      return -*( SinusTable + SINUS_TABLE_SIZE * 4 - p );
  }
}

// Sinus aus Tabelle holen
FIX30 fsin( FIX16 v )
{
  return FIX30( CFixMath::GetSinusTableEntry( ( v * 4*SINUS_TABLE_SIZE + 0.5 ).Int() ) );
}

// Cosinus aus Tabelle holen
FIX30 fcos( FIX16 v )
{
  return FIX30( CFixMath::GetSinusTableEntry( ( v * 4*SINUS_TABLE_SIZE + 0.5 ).Int() + SINUS_TABLE_SIZE ) );
}


