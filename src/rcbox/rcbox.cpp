//
// rcbox.cpp, read rcbox from serial port
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include <iostream.h>
#include <conio.h>
#include <string.h>
#include "debug.hpp"
#include "timer.hpp"
#include "dpmi.hpp"
#include "uart.hpp"
#include "pic.hpp"
#include "rcbox.hpp"

/////////////////////////////////////////////////////////////////////////////
// Class RCBox

// COM-Port feststellen (ComPort Adressen ab 40:0 durchsuchen)
BOOL CRCBox::_Detect()
{
  cout << hex;
  Port = 0;
  if( GetDebugLevel() > 0 )
    cout << "search at ";
  WORD *PortPtr = (WORD*)D32LinAdr( 0x40, 0 );
  for( int i=0; i<4; i++ ) {
    if( *PortPtr != 0 ) {
      if( GetDebugLevel() > 0 )
        cout <<*PortPtr << "h, ";
      if( IsInterfacePresent( *PortPtr ) ) {
        Port = *PortPtr;
        break;
      }
    }
    PortPtr++;
  }
  if( Port != 0 ) {
    if( GetDebugLevel() > 0 )
      cout << "\b\b; found at " << Port << "h ";
  } else {
    Port = 0x2f8;
    if( GetDebugLevel() > 0 )
      cout << "\b\b; using default at " << Port << "h ";
  }
  cout << dec;
  return TRUE;
}

// RCBox aktivieren
void CRCBox::_Startup()
{
  RM_CodePtr = D32AllocDosMem( RC_DATA_SIZE );
  RM_DataPtr = (RC_DATA*)( (char*)RM_CodePtr + RC_DATA_OFFSET );
  memcpy( RM_CodePtr, RM_Handler, RC_DATA_SIZE );
  RM_DataPtr->Port = Port;
  SetTiming( RC_STD_MIN_PW, RC_STD_MAX_PW );
  IRQ = GetComIRQ( Port );
  SaveUARTRegs( Port );
  OldIRQMask = GetIRQMask();
  DisableIRQ( IRQ );
  OldPM = D32GetPMVector( IRQToInt( IRQ ));
  OldRM = D32GetRMVector( IRQToInt( IRQ ));
  D32SetPMVector( IRQToInt( IRQ ), PM_Handler );
  D32SetRMVector( IRQToInt( IRQ ), RM_CodePtr );
  SetUARTRegs( Port );
  EnableIRQ( IRQ );
  RM_DataPtr->Trigger = 0;
  Timer.Reset();
}

// RCBox deaktivieren
void CRCBox::_Shutdown()
{
  DisableIRQ( IRQ );
  RestoreUARTRegs( Port );
  D32SetRMVector( IRQToInt( IRQ ), OldRM );
  D32SetPMVector( IRQToInt( IRQ ), OldPM );
  SetIRQMask( OldIRQMask );
  D32FreeDosMem( RM_CodePtr );
}

// Prüfen, ob über RTS und DSR eine Brücke geschaltet ist
BOOL CRCBox::IsInterfacePresent( int Port )
{
  BOOL Present = TRUE;
  SaveUARTRegs( Port );
  inp( Port+MSR );              // DDSR löschen
  outp( Port+MCR, 0 );          // RTS Low
  WaitMs( 1 );                  // DSR muß Low sein
  if(( inp( Port+MSR ) & DSR ) != 0 )
    Present = FALSE;
  outp( Port+MCR, RTS );        // RTS High
  WaitMs( 1 );                  // DSR muß High sein
  if(( inp( Port+MSR ) & DSR ) == 0 )
    Present = FALSE;
  inp( Port+MSR );              // DDSR löschen
  RestoreUARTRegs( Port );
  return Present;
}

// Register eines UARTs sichern
void CRCBox::SaveUARTRegs( int Port )
{
  OldLCR = inp( Port+LCR );
  outp( Port+LCR, 0 );
  OldIER = inp( Port+IER );
  OldMCR = inp( Port+MCR );
  inp( Port );                  // aufräumen
}

// Register eines UARTs restaurieren
void CRCBox::RestoreUARTRegs( int Port )
{
  outp( Port+LCR, 0 );          // DLAB = 0
  outp( Port+IER, OldIER );
  outp( Port+LCR, OldLCR );
  outp( Port+MCR, OldMCR );
  inp( Port+RBR );
  inp( Port+MSR );              // aufräumen
}

// Register eines UARTs programmieren
void CRCBox::SetUARTRegs( int Port )
{
  outp( Port+LCR, 0 );          // DLAB = 0
  outp( Port+IER, 8 );
  outp( Port+MCR, RTS+OUT2 );   // RTS = HIGH, Enable Ints
  inp( Port+RBR );
  inp( Port+MSR );
}

// liefert zur Basisadresse eines COM-Ports die zugehörigen IRQ
int CRCBox::GetComIRQ( int Port )
{
  switch( Port ) {
    case 0x3f8 : return 4;
    case 0x2f8 : return 3;
    case 0x3e8 : return 4;
    case 0x2e8 : return 3;
    default    : return 4;
  }
}

// Impulstiming setzen (Zeiten in us)
void CRCBox::SetTiming( int _MinPulse, int _MaxPulse )
{
  MinPulse = _MinPulse * 1.193;  // in Ticks umrechnen
  MaxPulse = _MaxPulse * 1.193;
  RM_DataPtr->SyncLen = MaxPulse * 1.2;
  for( int i=0; i<CHANNELS; i++ )
    ZeroTable[i] = MinPulse + (( MaxPulse - MinPulse ) >> 1 );
}

// Daten gültig ?
BOOL CRCBox::DataValid()
{
  if( RM_DataPtr->Trigger ) {
    RM_DataPtr->Trigger = 0;
    Timer.Reset();
  } else {
    if( Timer.Read() > 100000 )         // wenn Totmann > 0.1 s
      return FALSE;
  }
  for( int i=0; i<CHANNELS; i++ )
    if( RM_DataPtr->Channel[i] < MinPulse-50 || RM_DataPtr->Channel[i] > MaxPulse+50 )
      return FALSE;
  return TRUE;
}

// Knüppelwert holen
int CRCBox::Get( int Stick )
{
  int v = ( RM_DataPtr->Channel[ Stick ] - ZeroTable[ Stick ] ) * 25569 >> 16;
  if( v<-256 ) return -256;
  if( v>256 ) return 256;
  return v;
}
