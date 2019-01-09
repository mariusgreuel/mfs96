//
// port.hpp, comfortable port io
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _PORT_HPP_INCLUDED
#define _PORT_HPP_INCLUDED

#ifndef __WATCOMC__             // for compatiblity with Borland C++
  #define outp  outportb
  #define inp   inportb
#endif

#include <conio.h>

class CPort {
  int Port;
  int Data;
  int InvertedBits;
  int MaskFlag;
  int Mask;
public:
  CPort();
  void Set( int Port );
  int  Get();
  void Invert( int InvertedBits );
  CPort &operator [] ( int Mask );
  CPort &operator =  ( int Data );
  operator int();
};

// initialize variables
inline CPort::CPort()
{
  MaskFlag = 0;
  Port = -1;
  Data = 0;
  InvertedBits = 0;
}

// set port adress
inline void CPort::Set( int Port )
{
  CPort::Port  = Port;
}

// return port adress
inline int CPort::Get()
{
  return Port;
}

// set inverted bits
inline void CPort::Invert( int InvertedBits )
{
  CPort::InvertedBits = InvertedBits;
}

// set index
inline CPort &CPort::operator [] ( int Mask )
{
  if( !MaskFlag ) {
    MaskFlag = 1;
    CPort::Mask = Mask;
  }
  return *this;
}

// write data to port
inline CPort &CPort::operator = ( int OutByte )
{
  if( MaskFlag ) {
    MaskFlag = 0;
    if( OutByte )
      Data |= Mask;
    else
      Data &= ~Mask;
  } else
    Data = OutByte;
  outp( Port, Data ^ InvertedBits );
  return *this;
}

// read data from port
inline CPort::operator int()
{
  if( MaskFlag ) {
    MaskFlag = 0;
    return ( inp( Port ) ^ InvertedBits ) & Mask ? 1 : 0;
  } else
    return inp( Port ) ^ InvertedBits;
}

#endif // _PORT_HPP_INCLUDED

