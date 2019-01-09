//
// vars.h
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include "variable.h"

// Steuerung
extern CBVar
  RCBoxNegChannel[],
  RCBoxNegPulse;
extern CFVar
  RCBoxMinPulseWidth,
  RCBoxMaxPulseWidth;

extern CBVar
  SoundOn;

// Allgemeines
extern CFVar
  ScreenAspectRatio,
  ViewHeight,
  TimeAdjust,
  Gravity;

// Flugzeug
extern CFVar
  WingLift;

// Hubschrauber
extern CFVar
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

extern CFVar
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


