//
// dpmi.hpp, DPMI support routines
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef __386__
#error dpmi.hpp is for use with 386'er option only
#endif

#ifndef _DPMI_HPP_INCLUDED
#define _DPMI_HPP_INCLUDED

#include <i86.h>
#include "types.h"

#define DPMI_INT        0x31
#define D32RealSeg(P)   (((DWORD)(P) >> 4 ) & 0xFFFF )
#define D32RealOff(P)   ((DWORD)(P) & 0xF )
#define D32LinAdr(S,O)  ((void *)((((S)&0xffff)<<4) + ((O)&0xffff)))

/////////////////////////////////////////////////////////////////////////////
// DOS Memory Managment Services

inline void *D32AllocDosMem( DWORD Size )
{
  union REGS r;
  r.x.eax = 0x0100;                     // DPMI allocate DOS memory
  r.x.ebx = ( Size + 15 ) >> 4;
  int386( DPMI_INT, &r, &r );
  if( r.x.cflag )
    return NULL;
  return D32LinAdr( r.x.eax, 0 );
}

inline void *D32FreeDosMem( void *Ptr )
{
  union REGS r;
  r.x.eax = 0x0101;                     // DPMI free DOS memory
  r.x.edx = D32RealSeg( Ptr );
  int386( DPMI_INT, &r, &r );
  if( r.x.cflag )
    return NULL;
  return Ptr;
}

inline void *D32ResizeDosMem( void *Ptr, DWORD Size )
{
  union REGS r;
  r.x.eax = 0x0102;                     // DPMI resize DOS memory
  r.x.ebx = ( Size + 15 ) >> 4;
  r.x.edx = D32RealSeg( Ptr );
  int386( DPMI_INT, &r, &r );
  if( r.x.cflag )
    return NULL;
  return Ptr;
}

/////////////////////////////////////////////////////////////////////////////
// Interrupt Services

inline void *D32GetRMVector( BYTE IntNr )
{
  union REGS r;
  r.x.eax = 0x0200;                     // DPMI get real mode vector
  r.h.bl = IntNr;
  int386( 0x31, &r, &r );
  return D32LinAdr( r.w.cx, r.w.dx );
}

inline BOOL D32SetRMVector( BYTE IntNr, void *Ptr )
{
  union REGS r;
  r.x.eax = 0x0201;                     // DPMI set real mode vector
  r.h.bl = IntNr;
  r.x.ecx = D32RealSeg( Ptr );
  r.x.edx = D32RealOff( Ptr );
  int386( DPMI_INT, &r, &r );
  if( r.x.cflag )
    return TRUE;
  else
    return FALSE;                       // False if call succeeds
}

inline void __far *D32GetExceptionHandler( BYTE FaultNr )
{
  union REGS r;
  r.x.eax = 0x0202;                     // DPMI get exception handler
  r.h.bl = FaultNr;
  int386( DPMI_INT, &r, &r );
  if( r.x.cflag )
    return NULL;
  return MK_FP( r.w.cx, r.x.edx );
}

inline BOOL D32SetExceptionHandler( BYTE FaultNr, void __far *Ptr )
{
  union REGS r;
  r.x.eax = 0x0203;                     // DPMI get exception handler
  r.h.bl = FaultNr;
  r.x.ecx = FP_SEG( Ptr );
  r.x.edx = FP_OFF( Ptr );
  int386( DPMI_INT, &r, &r );
  if( r.x.cflag )
    return TRUE;
  else
    return FALSE;                       // False if call succeeds
}

inline void __far *D32GetPMVector( BYTE IntNr )
{
  union REGS r;
  r.x.eax = 0x0204;                     // DPMI get protected mode vector
  r.h.bl = IntNr;
  int386( DPMI_INT, &r, &r );
  return MK_FP( r.x.ecx, r.x.edx );
}

inline BOOL D32SetPMVector( BYTE IntNr, void __far *Ptr )
{
  union REGS r;
  r.x.eax = 0x0205;                     // DPMI set protected mode vector
  r.h.bl = IntNr;
  r.x.ecx = FP_SEG( Ptr );
  r.x.edx = FP_OFF( Ptr );
  int386( DPMI_INT, &r, &r );
  if( r.x.cflag )
    return TRUE;
  else
    return FALSE;                       // False if call succeeds
}

/////////////////////////////////////////////////////////////////////////////
// Page Locking Services

inline BOOL D32LockMemory( void *Ptr, DWORD Size )
{
  union REGS r;
  r.x.eax = 0x0600;                     // DPMI lock memory
  r.x.ebx = ( (DWORD)Ptr >> 16) & 0xffff;
  r.x.ecx = (DWORD)Ptr & 0xffff;
  r.x.esi = ( Size >> 16) & 0xffff;
  r.x.edi = Size & 0xffff;
  int386( DPMI_INT, &r, &r );
  if( r.x.cflag )
    return TRUE;
  else
    return FALSE;                       // False if call succeeds
}

inline BOOL D32UnlockMemory( void *Ptr, DWORD Size )
{
  union REGS r;
  r.x.eax = 0x0601;                     // DPMI unlock memory
  r.x.ebx = ( (DWORD)Ptr >> 16) & 0xffff;
  r.x.ecx = (DWORD)Ptr & 0xffff;
  r.x.esi = ( Size >> 16) & 0xffff;
  r.x.edi = Size & 0xffff;
  int386( DPMI_INT, &r, &r );
  if( r.x.cflag )
    return TRUE;
  else
    return FALSE;                       // False if call succeeds
}

#endif // _DPMI_HPP_INCLUDED

