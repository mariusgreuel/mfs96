//
// sound.cpp
//
// Copyright Marius Greuel 1996. All rights reserved
//

#include "sound.hpp"

#define ENG_BASE_FREQ1  325     // base freq. for engine sound 1
#define ENG_BASE_FREQ2  400     // base freq. for engine sound 2
#define ENG_BASE_FREQ3  460     // base freq. for engine sound 3
#define BEEPER_FREQ     400

// set engine sound parameter
void CSound::DefineVoices()
{
  CFMInst Engine1, Engine2, Engine3, Beeper;
// Engine harmonic 1
  Engine1.SetAttack( 0xf );
  Engine1.SetDecay( 0 );
  Engine1.SetSustain( 0 );
  Engine1.SetRelease( 5 );
  Engine1.SetMultiple( 0 );
  Engine1.SetWave( 0 );
  Engine1.SetLevel( 0 );
  Engine1.EnableSustain( TRUE );
// Engine harmonic 2
  Engine2.SetAttack( 0xf );
  Engine2.SetDecay( 0 );
  Engine2.SetSustain( 0 );
  Engine2.SetRelease( 5 );
  Engine2.SetMultiple( 1 );
  Engine2.SetWave( 0 );
  Engine2.SetLevel( 10 );
  Engine2.EnableSustain( TRUE );
// Engine harmonic 3
  Engine3.SetAttack( 0xf );
  Engine3.SetDecay( 0 );
  Engine3.SetSustain( 0 );
  Engine3.SetRelease( 5 );
  Engine3.SetMultiple( 1 );
  Engine3.SetWave( 0 );
  Engine3.SetLevel( 10 );
  Engine3.EnableSustain( TRUE );
// Beeper harmonic 3
  Beeper.SetAttack( 0xf );
  Beeper.SetDecay( 0 );
  Beeper.SetSustain( 0 );
  Beeper.SetRelease( 5 );
  Beeper.SetMultiple( 4 );
  Beeper.SetWave( 0 );
  Beeper.SetLevel( 2 );
  Beeper.EnableSustain( TRUE );
  LoadInstrument( 0, Engine1 );
  LoadInstrument( 1, Engine2 );
  LoadInstrument( 2, Engine3 );
  LoadInstrument( 3, Beeper );
  SetFeedBack( 0, 5 );
  SetConn( 0, 0 );
  SetFeedBack( 1, 5 );
  SetConn( 1, 0 );
  SetFeedBack( 2, 5 );
  SetConn( 2, 0 );
  SetFeedBack( 3, 1 );
  SetConn( 3, 1 );
}

// turn engine on
void CSound::EngineOn()
{
  SetOct( 0, 3 );
  SetOct( 1, 3 );
  SetOct( 2, 2 );
  SetFreq( 0, ENG_BASE_FREQ1 );
  SetFreq( 1, ENG_BASE_FREQ2 );
  SetFreq( 2, ENG_BASE_FREQ1 );
  EnableKey( 0, TRUE );
  EnableKey( 1, TRUE );
  EnableKey( 2, TRUE );
}

// turn engine on
void CSound::EngineOff()
{
  SetFreq( 0, 0 );
  SetFreq( 1, 0 );
  SetFreq( 2, 0 );
  EnableKey( 0, FALSE );
  EnableKey( 1, FALSE );
  EnableKey( 2, FALSE );
}

// adjust engine sound
void CSound::AdjustEngineSound( int rpm )
{
  SetFreq( 0, rpm );
  SetFreq( 1, rpm );
}

void CSound::Startup()
{
  if( !IsInstalled() )
    return;
  CFMSound::Startup();
  DefineVoices();
}

