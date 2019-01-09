//
// sbfm.hpp, adlib/soundblaster command subset
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _SBFM_HPP_INCLUDED
#define _SBFM_HPP_INCLUDED

#include "types.h"
#include "driver.hpp"
#include "sbfmreg.hpp"

#define FM_ADLIBBASE    0x388
#define FM_MAXCHANNELS  9

#define FMR_STATUS      0       // status register
#define FMR_TEST        1       // the test register
#define FMR_TIMER1      2       // timer1 register
#define FMR_TIMER2      3       // timer 2 register
#define FMR_TIMERCTRL   4       // timer control register
#define FMR_SPCHSEL     8       // speech select/keyboard split
#define FMR_RYTHMDEP    0xbd    // rythm select/depth/drum keyon
#define FMR_PARAMS      0x20    // params, index 0-8
#define FMR_SCALEOUT    0x40    // scale/output level, index 0-8
#define FMR_ATTKDEC     0x60    // attack/decay, index 0-8
#define FMR_SUSTREL     0x80    // sustain/release, index 0-8
#define FMR_FNUMLOW     0xa0    // freq. select low, index 0-8
#define FMR_FNUMHIKB    0xb0    // freq. hi/octave/keyon, index 0-8
#define FMR_FEEDCONN    0xc0    // feedback/connection, index 0-8
#define FMR_WAVESEL     0xe0    // wave select, index 0-8

enum Scale{ Cs, D, Ds, E, F, Fs, G, Gs, A, As, B, C };
enum Rythm{ Bass, Snare, TomTom, TopCym, HiHat };
extern WORD fNum[ 12 ];

// FM register set
typedef struct {
  BYTE Test;
  BYTE Timer1;
  BYTE Timer2;
  BYTE TimerCtrl;
  BYTE SpchSel;
  BYTE RythmDep;
  BYTE fNumLow[ FM_MAXCHANNELS ];
  BYTE fNumHiKb[ FM_MAXCHANNELS ];
  BYTE FeedConn[ FM_MAXCHANNELS ];      // feedback/connect
}FMREGS;

// struct for modulation and carrier register
struct CFMInstModCar {
  BYTE Params;                  // parameter bits
  BYTE ScaleOut;                // scaling/output level
  BYTE AttkDec;                 // attack/decay rate
  BYTE SustRel;                 // substain/release rate
  BYTE WaveSel;                 // wave select
  void SetMultiple( BYTE Rate ) { Params &= ~0xf; Params |= Rate & 0xf; }
  void EnableScaling( BOOL On ) { Params &= ~0x10; if( On ) Params |= 0x10; };
  void EnableSustain( BOOL On ) { Params &= ~0x20; if( On ) Params |= 0x20; };
  void EnableVibrato( BOOL On ) { Params &= ~0x40; if( On ) Params |= 0x40; };
  void EnableTremolo( BOOL On ) { Params &= ~0x80; if( On ) Params |= 0x80; };
  void SetLevel( BYTE Rate ) { ScaleOut &= ~0x3f; ScaleOut |= Rate & 0x3f; }
  void SetScale( BYTE Rate ) { ScaleOut &= ~0xc0; ScaleOut |= ( Rate & 3 ) << 6; }
  void SetDecay( BYTE Rate ) { AttkDec &= ~0xf; AttkDec |= Rate & 0xf; }
  void SetAttack( BYTE Rate ) { AttkDec &= ~0xf0; AttkDec |= ( Rate & 0xf ) << 4; }
  void SetRelease( BYTE Rate ) { SustRel &= ~0xf; SustRel |= Rate & 0xf; }
  void SetSustain( BYTE Rate ) { SustRel &= ~0xf0; SustRel |= ( Rate & 0xf ) << 4; }
  void SetWave( BYTE Rate ) { WaveSel &= ~0x3; WaveSel |= Rate & 3; }
};

struct CFMInst {
  CFMInstModCar Mod, Car;
  void SetMultiple( BYTE Rate ) { Mod.SetMultiple( Rate ); Car.SetMultiple( Rate ); }
  void EnableScaling( BOOL On ) { Mod.EnableScaling( On ); Car.EnableScaling( On ); }
  void EnableSustain( BOOL On ) { Mod.EnableSustain( On ); Car.EnableSustain( On ); }
  void EnableVibrato( BOOL On ) { Mod.EnableVibrato( On ); Car.EnableVibrato( On ); }
  void EnableTremolo( BOOL On ) { Mod.EnableTremolo( On ); Car.EnableTremolo( On ); }
  void SetLevel( BYTE Rate ) { Mod.SetLevel( Rate ); Car.SetLevel( Rate ); };
  void SetScale( BYTE Rate ) { Mod.SetScale( Rate ); Car.SetScale( Rate ); }
  void SetDecay( BYTE Rate ) { Mod.SetDecay( Rate ); Car.SetDecay( Rate ); }
  void SetAttack( BYTE Rate ) { Mod.SetAttack( Rate ); Car.SetAttack( Rate ); }
  void SetRelease( BYTE Rate ) { Mod.SetRelease( Rate ); Car.SetRelease( Rate ); }
  void SetSustain( BYTE Rate ) { Mod.SetSustain( Rate ); Car.SetSustain( Rate ); }
  void SetWave( BYTE Rate ) { Mod.SetWave( Rate ); Car.SetWave( Rate ); }
};

class CFMSound : public CDriver {
  FMREGS FMRegs;
protected:
  BOOL _Detect();
  void _Startup();
  void _Shutdown();
public:
  CFMSound() : CDriver( "CFMSound" ) {}
  ~CFMSound() { Shutdown(); }
  void Reset();
  void SetMusicMode();
  void SetRythmMode();
  void EnableRythmKey( BYTE Inst, BOOL On );
  void EnableVibratoDepth( BOOL On );
  void EnableTremoloDepth( BOOL On );
  void EnableKey( BYTE Channel, BOOL On );
  void SetFreq( BYTE Channel, WORD FNum );
  void SetOct( BYTE Channel, BYTE Octave );
  void SetConn( BYTE Channel, BYTE Conn );
  void SetFeedBack( BYTE Channel, BYTE FeedBack );
  void SetVolume( BYTE Channel, BYTE Volume );
  void LoadInstrument( BYTE Channel, CFMInst &Inst );
};

#endif // _SBFM_HPP_INCLUDED

