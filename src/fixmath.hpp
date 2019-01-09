//
// fixmath.hpp, 32 Bit fixpoint arithmetic
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _FIXMATH_HPP_INCLUDED
#define _FIXMATH_HPP_INCLUDED

#include <iostream.h>

#define _PI     3.14159265359
#define _PI2    (_PI/2)
#define _2PI    (2*_PI)

extern "C" {
  long SqrtFix16( long );
};

long FixMul16( long, long );
#pragma aux FixMul16 =          \
        "imul edx"              \
        "shrd eax,edx,16"       \
        parm [eax][edx]         \
        value [eax];

long FixDiv16( long, long );
#pragma aux FixDiv16 =          \
        "cdq"                   \
        "ror eax,16"            \
        "mov dx,ax"             \
        "sub ax,ax"             \
        "idiv ecx"              \
        parm [eax][ecx]         \
        value [eax]             \
        modify [edx];

// 32 bit fixpoint value with 1 bit before and 30 bit behind the decimal point
// plus 1 bit for signed flag (prefered for (co)sinus values)
// all calculation is done in assembler so we don't need any more
class FIX30 {
  long f;
public:
  FIX30() {}
  FIX30( long o ) { f = o; }    // assign FIX type
  FIX30( double o ) { f = (long)( o * ( 1 << 30 ) ); }
  long Fix() const { return f; }
  float Float() const { return (float)f / ( 1 << 30 ); }
  double Double() const { return (double)f / ( 1 << 30 ); }
};
inline ostream &operator << ( ostream &strm, const FIX30 &o ) { strm << o.Double(); return strm; }

// convert float value int FIX16 format
inline long ToFix16( float o ) { return (long)( o * ( 1 << 16 ) ); }

// 32 bit fixpoint value with 15 bit before and 16 bit behind the decimal point
// plus 1 bit for signed flag
class FIX16 {
  long f;
public:
  FIX16() {}
  FIX16( long o ) { f = o; }    // is FIX
  FIX16( int o ) { f = (long)o << 16; }
  FIX16( double o ) { f = (long)( o * ( 1 << 16 ) ); }
  FIX16( const FIX30 &o ) { f = o.Fix() >> 14; }
  FIX16 &operator =  ( const FIX16 &o ) { f =  o.f; return *this; }
  FIX16 &operator += ( const FIX16 &o ) { f += o.f; return *this; }
  FIX16 &operator -= ( const FIX16 &o ) { f -= o.f; return *this; }
  FIX16 &operator *= ( const FIX16 &o ) { f = FixMul16( f, o.f ); return *this; }
  FIX16 &operator /= ( const FIX16 &o ) { f = FixDiv16( f, o.f ); return *this; }
  FIX16 &operator =  ( float o ) { f =  ToFix16( o ); return *this; }
  FIX16 &operator += ( float o ) { f += ToFix16( o ); return *this; }
  FIX16 &operator -= ( float o ) { f -= ToFix16( o ); return *this; }
  FIX16 &operator *= ( float o ) { f = FixMul16( f, ToFix16( o ) ); return *this; }
  FIX16 &operator /= ( float o ) { f = FixDiv16( f, ToFix16( o ) ); return *this; }
  FIX16 operator + () const { return *this; }
  FIX16 operator - () const { return FIX16( -f ); }
  long Fix() const { return f; }
  int Int() const { return (int)( f >> 16 ); }
  float Float() const { return (float)f / ( 1 << 16 ); }
  double Double() const { return (double)f / ( 1 << 16 ); }
};

inline FIX16 operator + ( const FIX16 &o1, const FIX16 &o2 ) { return FIX16( o1.Fix() + o2.Fix() ); }
inline FIX16 operator - ( const FIX16 &o1, const FIX16 &o2 ) { return FIX16( o1.Fix() - o2.Fix() ); }
inline FIX16 operator * ( const FIX16 &o1, const FIX16 &o2 ) { return FIX16( FixMul16( o1.Fix(), o2.Fix() ) ); }
inline FIX16 operator / ( const FIX16 &o1, const FIX16 &o2 ) { return FIX16( FixDiv16( o1.Fix(), o2.Fix() ) ); }
inline FIX16 operator + ( const FIX16 &o1, float o2 ) { return FIX16( o1.Fix() + ToFix16( o2 ) ); }
inline FIX16 operator - ( const FIX16 &o1, float o2 ) { return FIX16( o1.Fix() - ToFix16( o2 ) ); }
inline FIX16 operator * ( const FIX16 &o1, float o2 ) { return FIX16( FixMul16( o1.Fix(), ToFix16( o2 ) ) ); }
inline FIX16 operator / ( const FIX16 &o1, float o2 ) { return FIX16( FixDiv16( o1.Fix(), ToFix16( o2 ) ) ); }
inline FIX16 operator + ( float o1, const FIX16 &o2 ) { return FIX16( ToFix16( o1 ) + o2.Fix() ); }
inline FIX16 operator - ( float o1, const FIX16 &o2 ) { return FIX16( ToFix16( o1 ) - o2.Fix() ); }
inline FIX16 operator * ( float o1, const FIX16 &o2 ) { return FIX16( FixMul16( ToFix16( o1 ), o2.Fix() ) ); }
inline FIX16 operator / ( float o1, const FIX16 &o2 ) { return FIX16( FixDiv16( ToFix16( o1 ), o2.Fix() ) ); }
inline int operator <  ( const FIX16 &o1, const FIX16 &o2 ) { return o1.Fix() <  o2.Fix(); }
inline int operator >  ( const FIX16 &o1, const FIX16 &o2 ) { return o1.Fix() >  o2.Fix(); }
inline int operator <= ( const FIX16 &o1, const FIX16 &o2 ) { return o1.Fix() <= o2.Fix(); }
inline int operator >= ( const FIX16 &o1, const FIX16 &o2 ) { return o1.Fix() >= o2.Fix(); }
inline int operator == ( const FIX16 &o1, const FIX16 &o2 ) { return o1.Fix() == o2.Fix(); }
inline int operator != ( const FIX16 &o1, const FIX16 &o2 ) { return o1.Fix() != o2.Fix(); }
inline int operator <  ( const FIX16 &o1, float o2 ) { return o1.Fix() <  ToFix16( o2 ); }
inline int operator >  ( const FIX16 &o1, float o2 ) { return o1.Fix() >  ToFix16( o2 ); }
inline int operator <= ( const FIX16 &o1, float o2 ) { return o1.Fix() <= ToFix16( o2 ); }
inline int operator >= ( const FIX16 &o1, float o2 ) { return o1.Fix() >= ToFix16( o2 ); }
inline int operator == ( const FIX16 &o1, float o2 ) { return o1.Fix() == ToFix16( o2 ); }
inline int operator != ( const FIX16 &o1, float o2 ) { return o1.Fix() != ToFix16( o2 ); }
inline int operator <  ( float o1, const FIX16 &o2 ) { return ToFix16( o1 ) <  o2.Fix(); }
inline int operator >  ( float o1, const FIX16 &o2 ) { return ToFix16( o1 ) >  o2.Fix(); }
inline int operator <= ( float o1, const FIX16 &o2 ) { return ToFix16( o1 ) <= o2.Fix(); }
inline int operator >= ( float o1, const FIX16 &o2 ) { return ToFix16( o1 ) >= o2.Fix(); }
inline int operator == ( float o1, const FIX16 &o2 ) { return ToFix16( o1 ) == o2.Fix(); }
inline int operator != ( float o1, const FIX16 &o2 ) { return ToFix16( o1 ) != o2.Fix(); }
inline FIX16 abs( const FIX16 &o ) { return o.Fix() > 0 ? o : -o; }
inline FIX16 sqrt( const FIX16 &o ) { return FIX16( SqrtFix16( o.Fix() ) ); }
inline ostream &operator << ( ostream &strm, const FIX16 &o ) { strm << o.Double(); return strm; }

class CFixMath {
  static long *SinusTable;
protected:
  static long GetSinusTableEntry( int v );
public:
  CFixMath();
  ~CFixMath();
  friend FIX30 fsin( FIX16 v );
  friend FIX30 fcos( FIX16 v );
};

#endif //_FIXMATH_HPP_INCLUDED

