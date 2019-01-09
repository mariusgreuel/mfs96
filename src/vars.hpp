//
// vars.hpp
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include "fixmath.hpp"
#include "variable.hpp"

// Variablentypen um Klasse CF16Var erweitern
class CF16Var : public CVarKey {
  FIX16 Value, Default, Min, Max;
public:
  CF16Var( CVarSection &Section, const char *Name, FIX16 _Value, FIX16 _Min = -1.0, FIX16 _Max = 1.0 ) :
           CVarKey( Section, Name ) { Value = Default = _Value; Min = _Min; Max = _Max; }
  void operator = ( const char *ValueStrg ) { Value = atof( ValueStrg ); }
  void operator = ( FIX16 _Value ) { Value = _Value; }
  void operator = ( const CF16Var &Var ) { Value = Var.Value; }
  operator FIX16() const { return Value; }
  FIX16 GetValue() { return Value; }
  FIX16 GetDefault() { return Default; }
  FIX16 GetMin() { return Min; }
  FIX16 GetMax() { return Max; }
  ostream &operator >> ( ostream &strm ) const { return strm << Value; }
};

// Steuerung
extern CBVar
  RCBoxNegChannel[],
  RCBoxNegPulse;
extern CF16Var
  RCBoxMinPulseWidth,
  RCBoxMaxPulseWidth;

extern CBVar
  SoundOn;

// Allgemeines
extern CF16Var
  ScreenAspectRatio,
  ViewHeight,
  TimeAdjust,
  Gravity;

// Flugzeug
extern CF16Var
  WingLift;

// Hubschrauber
extern CF16Var
  Mass,
  MassInertia,
  Sensitivity[],
  Inertia[],
  Spin[],
  Drag[],
  TurnWithWind[],
  GroundEffectRange,
  GroundEffectFactor,
  EnginePower,
  EngineRiseRate,
  RPMLift,
  CollectiveLift;

// Wind
extern CBVar
  WindOn;

extern CF16Var
  WindBaseSpeed,
  WindSpeedChgFreq,
  WindSpeedChgRange,
  WindSpeedRateOfChg,
  WindSpeedChgDurMin,
  WindSpeedChgDurMax,
  WindBaseDir,
  WindDirChgFreq,
  WindDirChgRange,
  WindDirRateOfChg,
  WindDirChgDurMin,
  WindDirChgDurMax,
  GustFreq,
  GustPeakMin,
  GustPeakMax,
  GustDirMin,
  GustDirMax,
  GustDurMin,
  GustDurMax;


