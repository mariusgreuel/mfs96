//
// timer.hpp, hardware timer handling for 8253/8254 and compatible
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _TIMER_HPP_INCLUDED
#define _TIMER_HPP_INCLUDED

#include "types.h"
#include "driver.hpp"

typedef DWORD TIME;

extern "C" {
  void _SetTimerMode2();
  void _SetTimerMode3();
  TIME _ReadTimer();
};

class CTimer : public CDriver {
  static NumTimers;
  static TIME GlobalTimeStart, GlobalTime;
  TIME LocalTimeStart, LocalTime;
protected:
  BOOL _Detect();
  void _Startup();
  void _Shutdown();
public:
  CTimer() : CDriver( "CTimer" ) { NumTimers++; }
  ~CTimer() { NumTimers--; if( NumTimers == 0 ) Shutdown(); }
  void Reset() { LocalTimeStart = LocalTime = _ReadTimer(); }
  TIME Read() { return LocalTime = _ReadTimer() - LocalTimeStart; }
  TIME ReadGlobal() { return GlobalTime = _ReadTimer() - GlobalTimeStart; }
  TIME Get() { return LocalTime; }
  TIME GetGlobal() { return GlobalTime; }
  friend void Wait( TIME WaitTime );
};

// wait some time (in ticks)
inline void Wait( TIME WaitTime )
{
  TIME StartTime = _ReadTimer();
  while( _ReadTimer() - StartTime < WaitTime );
}

// wait some time (in ms)
inline void WaitMs( DWORD WaitTime )
{
  while( WaitTime ) {
    TIME StartTime = _ReadTimer();
    while( _ReadTimer() - StartTime < 1193 );   // wait 1ms
    WaitTime--;
  }
}

#endif

