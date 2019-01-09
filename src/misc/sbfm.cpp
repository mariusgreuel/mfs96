//
// sbfm.cpp, adlib/soundblaster command subset
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include <conio.h>
#include <string.h>
#include "sbfm.hpp"

// offsets for cell control registers
static BYTE RegBaseTable[] = {
  0, 1, 2, 8, 9, 10, 16, 17, 18
};

// default instrument for CFMInst structure
static CFMInst DefaultInst = {
  0x01, 0x4f, 0xf1, 0x53, 0,
  0x11, 0x0,  0xf2, 0x74, 0
};

// note frequencies
WORD fNum[ 12 ] = {
  363, 385, 408, 432, 458, 485, 514, 544, 577, 611, 647, 686
};

/////////////////////////////////////////////////////////////////////////////
// Class CFMSound

void Wait( long howlong )
{
  for( long i=0; i<howlong*100; i++ );
}

// CFMSound detektieren
BOOL CFMSound::_Detect()
{
  FM_WriteReg( FMR_TEST, 0 );                   // initialize test register
  FM_WriteReg( FMR_TIMERCTRL, 0x60 );           // reset both timers
  FM_WriteReg( FMR_TIMERCTRL, 0x80 );           // enable timer interrupts
  int Stat1 = inp( FM_ADLIBBASE + FMR_STATUS ); // read the status port
  FM_WriteReg( FMR_TIMER1, 0xff );              // write ff to the timer 1 count
  FM_WriteReg( FMR_TIMERCTRL, 0x21 );           // start timer 1
  Wait( 90 );                                   // wait 90 microseconds
  int Stat2 = inp( FM_ADLIBBASE + FMR_STATUS ); // read the status port
  FM_WriteReg( FMR_TIMERCTRL, 0x60 );           // reset both timers
  FM_WriteReg( FMR_TIMERCTRL, 0x80 );           // enable timer interrupts
  if(( Stat1 & 0xe0 ) == 0 && ( Stat2 & 0xe0 ) == 0xc0 )
    return TRUE;
  return FALSE;
}

// CFMSound aktivieren
void CFMSound::_Startup()
{
  Reset();
}

// CFMSound deaktivieren
void CFMSound::_Shutdown()
{
  for( BYTE i=0; i<FM_MAXCHANNELS; i++ )
    SetVolume( i, 0 );
}

// resets the FM card
void CFMSound::Reset()
{
  memset( &FMRegs, 0, sizeof( FMREGS ));        // initialize the regs struct
  FMRegs.Test = 0;
  FMRegs.RythmDep = 0xc0;
  FMRegs.SpchSel = 0;
  FM_WriteReg( FMR_TEST, FMRegs.Test );
  FM_WriteReg( FMR_RYTHMDEP, FMRegs.RythmDep );
  FM_WriteReg( FMR_SPCHSEL, FMRegs.SpchSel );
  for( int i=0; i<FM_MAXCHANNELS; i++ ) {
    FM_WriteReg( FMR_FNUMLOW+i, FMRegs.fNumLow[ i ] );
    FM_WriteReg( FMR_FNUMHIKB+i, FMRegs.fNumHiKb[ i ] );
    FM_WriteReg( FMR_FEEDCONN+i, FMRegs.FeedConn[ i ] );
    LoadInstrument( i, DefaultInst );
  }
}

// puts the FM chip into 9-voice FM music mode
void CFMSound::SetMusicMode()
{
  FMRegs.SpchSel &= ~0x80;
  FMRegs.RythmDep &= ~0x20;
  FM_WriteReg( FMR_SPCHSEL, FMRegs.SpchSel );
  FM_WriteReg( FMR_RYTHMDEP, FMRegs.RythmDep );
}

// puts the FM chip into 6-voice / 5-rythm mode
void CFMSound::SetRythmMode()
{
  FMRegs.SpchSel &= ~0x80;
  FMRegs.RythmDep &= ~0x1f;
  FMRegs.RythmDep |= 0x20;
  FM_WriteReg( FMR_SPCHSEL, FMRegs.SpchSel );
  FM_WriteReg( FMR_RYTHMDEP, FMRegs.RythmDep );
}

// turns on the specified rythm instrument
// inst range 0..3
void CFMSound::EnableRythmKey( BYTE Inst, BOOL On )
{
  if( On )
    FMRegs.RythmDep |= 0x10 >> Inst;
  else
    FMRegs.RythmDep &= ~( 0x10 >> Inst );
  FM_WriteReg( FMR_RYTHMDEP, FMRegs.RythmDep );
}

// turns on/off increased vibrato depth for all voices.
void CFMSound::EnableVibratoDepth( BOOL On )
{
  if( On )
    FMRegs.RythmDep |= 0x40;
  else
    FMRegs.RythmDep &= ~40;
  FM_WriteReg( FMR_RYTHMDEP, FMRegs.RythmDep );
}

// turns on/off increased tremolo depth for all voices
void CFMSound::EnableTremoloDepth( BOOL On )
{
  if( On )
    FMRegs.RythmDep |= 0x80;
  else
    FMRegs.RythmDep &= ~0x80;
  FM_WriteReg( FMR_RYTHMDEP, FMRegs.RythmDep );
}

// turns on/off a specified voice channel
void CFMSound::EnableKey( BYTE Channel, BOOL On )
{
  if( On )
    FMRegs.fNumHiKb[ Channel ] |= 0x20;
  else
    FMRegs.fNumHiKb[ Channel ] &= ~0x20;
   int i = FMR_FNUMHIKB + Channel;
  FM_WriteReg( i, FMRegs.fNumHiKb[ Channel ] );
}

// sets the output frequency of a voice channel
// fNum range 0..1024
void CFMSound::SetFreq( BYTE Channel, WORD fNum )
{
  FMRegs.fNumLow[ Channel ] = (BYTE)(fNum & 0xff);
  FMRegs.fNumHiKb[ Channel ] &= ~0x03;
  FMRegs.fNumHiKb[ Channel ] |= (BYTE)(( fNum >> 8 ) & 3 );
  int i = FMR_FNUMLOW + Channel;
  FM_WriteReg( i, FMRegs.fNumLow[ Channel ] );
      i = FMR_FNUMHIKB + Channel;
  FM_WriteReg( i, FMRegs.fNumHiKb[ Channel ] );
}

// sets the output octave for the specified voice channel
// octave range 0..7.
void CFMSound::SetOct( BYTE Channel, BYTE Octave )
{
  FMRegs.fNumHiKb[ Channel ] &= ~0x1c;
  FMRegs.fNumHiKb[ Channel ] |= ( Octave & 7 ) << 2;
  int i = FMR_FNUMHIKB + Channel;
  FM_WriteReg( i, FMRegs.fNumHiKb[ Channel ] );
}

// Specifies how the two operator cells will be connected
// conn range 0..1
void CFMSound::SetConn( BYTE Channel, BYTE Conn )
{
  FMRegs.FeedConn[ Channel ] &= ~0x1;
  FMRegs.FeedConn[ Channel ] |= Conn & 1;
  FM_WriteReg( FMR_FEEDCONN + Channel, FMRegs.FeedConn[ Channel ] );
}

// Sets the type of feedback modification used on the voice
// feedback range 0..7
void CFMSound::SetFeedBack( BYTE Channel, BYTE FeedBack )
{
  FMRegs.FeedConn[ Channel ] &= ~0xe;
  FMRegs.FeedConn[ Channel ] |= ( FeedBack & 7 ) << 1;
  FM_WriteReg( FMR_FEEDCONN + Channel, FMRegs.FeedConn[ Channel ] );
}

// sets the volume for the specified voice channel
// volume range 0..63
void CFMSound::SetVolume( BYTE Channel, BYTE Volume )
{
  FM_WriteReg( 0x43 + Channel, 0x3f - Volume & 0x3f );
}

// load the instrument InstNr with the pseudo InstClass
void CFMSound::LoadInstrument( BYTE Channel, CFMInst &Inst )
{
  BYTE RegBase = RegBaseTable[ Channel ];
  FM_WriteReg( 1, 0x20 );
  FM_WriteReg( RegBase+0x20,   Inst.Mod.Params );
  FM_WriteReg( RegBase+0x20+3, Inst.Car.Params );
  FM_WriteReg( RegBase+0x40,   Inst.Mod.ScaleOut );
  FM_WriteReg( RegBase+0x40+3, Inst.Car.ScaleOut );
  FM_WriteReg( RegBase+0x60,   Inst.Mod.AttkDec );
  FM_WriteReg( RegBase+0x60+3, Inst.Car.AttkDec );
  FM_WriteReg( RegBase+0x80,   Inst.Mod.SustRel );
  FM_WriteReg( RegBase+0x80+3, Inst.Car.SustRel );
  FM_WriteReg( RegBase+0xe0,   Inst.Mod.WaveSel );
  FM_WriteReg( RegBase+0xe0+3, Inst.Car.WaveSel );
}


