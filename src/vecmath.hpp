//
// vecmath.hpp
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _VECMATH_HPP_INCLUDED
#define _VECMATH_HPP_INCLUDED

#include "fixmath.hpp"

#define X 0
#define Y 1
#define Z 2
#define H 3

#define ROT_X           1, 2            // Rotationsachsen
#define ROT_NX          2, 1
#define ROT_Y           0, 2
#define ROT_NY          2, 0
#define ROT_Z           0, 1
#define ROT_NZ          1, 0

class CVec16;

extern "C" {
  long Vec16Abs( const CVec16 & );
  #pragma aux Vec16Abs parm[ebx] value[eax];
};

// vector with type 'FIX30'
class CVec30 {
public:
  FIX30 v[3];
public:
  CVec30() {}
};

// vector with type 'FIX16'
class CVec16 {
public:
  FIX16 v[3];
public:
  CVec16() {}
  CVec16( FIX16 x, FIX16 y, FIX16 z )  { v[X] = x; v[Y] = y; v[Z] = z; }
  CVec16( const CVec16 &v1 ) { v[X] = v1.v[X]; v[Y] = v1.v[Y]; v[Z] = v1.v[Z]; }
  CVec16( const CVec30 &v1 ) { v[X] = v1.v[X]; v[Y] = v1.v[Y]; v[Z] = v1.v[Z]; }
  const CVec16 &operator  = ( const CVec16 &v1 ) { v[X]  = v1.v[X]; v[Y]  = v1.v[Y]; v[Z]  = v1.v[Z]; return *this; }
  const CVec16 &operator += ( const CVec16 &v1 ) { v[X] += v1.v[X]; v[Y] += v1.v[Y]; v[Z] += v1.v[Z]; return *this; }
  const CVec16 &operator -= ( const CVec16 &v1 ) { v[X] -= v1.v[X]; v[Y] -= v1.v[Y]; v[Z] -= v1.v[Z]; return *this; }
  const CVec16 &operator *= ( FIX16 f ) { v[X] *= f; v[Y] *= f; v[Z] *= f; return *this; }
  const CVec16 &operator /= ( FIX16 f ) { v[X] /= f; v[Y] /= f; v[Z] /= f; return *this; }
};
inline CVec16 operator + ( const CVec16 &v1, const CVec16 &v2 ) { return CVec16( v1.v[X] + v2.v[X], v1.v[Y] + v2.v[Y], v1.v[Z] + v2.v[Z] ); }
inline CVec16 operator - ( const CVec16 &v1, const CVec16 &v2 ) { return CVec16( v1.v[X] - v2.v[X], v1.v[Y] - v2.v[Y], v1.v[Z] - v2.v[Z] ); }
inline CVec16 operator * ( const CVec16 &v1, FIX16 f ) { return CVec16( v1.v[X] * f, v1.v[Y] * f, v1.v[Z] * f ); }
inline CVec16 operator * ( FIX16 f, const CVec16 &v1 ) { return CVec16( v1.v[X] * f, v1.v[Y] * f, v1.v[Z] * f ); }
inline FIX16 abs( const CVec16 &v1 ) { return Vec16Abs( v1 ); }
inline FIX16 dot( const CVec16 &v1, const CVec16 &v2 ) { return v1.v[X] * v2.v[X] + v1.v[Y] * v2.v[Y] + v1.v[Z] * v2.v[Z]; }
inline CVec16 cross( const CVec16 &v1, const CVec16 &v2 ) { return CVec16( v1.v[Y] * v2.v[Z] - v1.v[Z] * v2.v[Y], v1.v[Z] * v2.v[X] - v1.v[X] * v2.v[Z], v1.v[X] * v2.v[Y] - v1.v[Y] * v2.v[X] ); }

// homogen Vec16
class CHVec16 : public CVec16 {
public:
  FIX16 h;
public:
  CHVec16() {}
  CHVec16( FIX16 x, FIX16 y, FIX16 z, FIX16 _h ) : CVec16( x, y, z ) { h = _h; }
  CHVec16( const CVec16 &v, FIX16 _h ) : CVec16( v ) { h = _h; }
};

// matrix with type FIX30, homogen part is FIX16
class CMatrix30 {
public:
  CVec30 v[3];
  CVec16 h;
public:
  void Init();
  void Rotate( int c1, int c2, FIX16 _sin, FIX16 _cos );
  void RotateByAxe( const CVec16 &v );
};

// matix with typ FIX16
class CMatrix16 {
public:
  CVec16 v[4];
public:
  void Init();
  CMatrix16 &operator *= ( CVec16 v );
  void Rotate( int c1, int c2, FIX16 _sin, FIX16 _cos );
};

CMatrix16 operator * ( const CMatrix30 &m1, const CMatrix16 &m2 );

#endif

