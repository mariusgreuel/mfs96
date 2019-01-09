//
// rcbox.hpp, read rcbox from serial port
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _RCBOX_HPP_INCLUDED
#define _RCBOX_HPP_INCLUDED

#include "types.h"
#include "driver.hpp"

// diese Zeilen müssen identisch mit denen in _rcbox.asm sein
#define RC_DATA_OFFSET          0x80
#define RC_DATA_SIZE            0x100
#define CHANNELS                7
#define RC_STD_MODE             0
#define RC_ONLY_PULSE           1
#define RC_NEG_PULSE            2
#define RC_SHOW_PULSE           4

#define RC_STD_MIN_PW           1050
#define RC_STD_MAX_PW           2150
#define RC_MIN_PW               800
#define RC_MAX_PW               2300
#define RC_MIN_PULSE_DIFF       800

typedef struct tag_RC_DATA {
  WORD Channel[ CHANNELS ];
  WORD Port;
  WORD SyncLen;
  WORD TimerTicks;
  BYTE CurrentChannel;
  BYTE Mode;
  BYTE Trigger;
  WORD RM_Count, PM_Count;
}RC_DATA;

extern "C" {
  void RM_Handler();
  void PM_Handler();
  extern void *RM_CodePtr;
  extern RC_DATA *RM_DataPtr;
};

class CRCBox : public CDriver {
  CTimer Timer;
  void *OldRM, __far *OldPM;
  BYTE OldLCR, OldIER, OldMCR, OldIRQMask;
  int  Port, IRQ;
  int  MinPulse, MaxPulse;
  WORD ZeroTable[ CHANNELS ];
private:
  BOOL IsInterfacePresent( int Port );
  void SaveUARTRegs( int Port );
  void RestoreUARTRegs( int Port );
  void SetUARTRegs( int Port );
  int  GetComIRQ( int Port );
protected:
  BOOL _Detect();
  void _Startup();
  void _Shutdown();
public:
  CRCBox() : CDriver( "CRCBox" ) {}
  ~CRCBox() { Shutdown(); }
  BOOL DataValid();
  int Get( int Nr );
  void SetTiming( int Min, int Max );
};

#endif // _RCBOX_HPP_INCLUDED

