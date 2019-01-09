//
// timer.cpp, hardware timer handling for 8253/8254 and compatible
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include "timer.hpp"

// global variables for time measuring
static int  CTimer::NumTimers = 0;
static TIME CTimer::GlobalTimeStart = 0;
static TIME CTimer::GlobalTime = 0;

/////////////////////////////////////////////////////////////////////////////
// Class Timer

// Timer detektieren
BOOL CTimer::_Detect()
{
  return TRUE;
}

// Timer aktivieren
void CTimer::_Startup()
{
  GlobalTimeStart = _ReadTimer();
  _SetTimerMode2();
  Reset();
}

// Timer deaktivieren
void CTimer::_Shutdown()
{
  _SetTimerMode3();
}

