//
// vars.cpp - static variables
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include "vars.hpp"

#pragma initialize 35;
CVarList  VarList;

#pragma initialize 36;
CVarSection Common(   VarList, "Common" );
CVarSection Control(  VarList, "Control" );
CVarSection Airplane( VarList, "Airplane" );
CVarSection Heli(     VarList, "Helicopter" );

#pragma initialize 37;

/////////////////////////////////////////////////////////////////////////////
// Allgemeine Parameter

CF16Var
  ScreenAspectRatio( Common, "ScreenAspectRatio",  1.34, 0.5, 2.0 ),
  ViewHeight( Common, "ViewHeight", 1.7,  0.0, 10.0 ),
  TimeAdjust( Common, "TimeAdjust", 1.0,  0.2,  5.0 ),
  Gravity( Common, "Gravity", 9.81, 8.0, 12.0 );
CBVar
  SoundOn( Common, "Sound", FALSE );

/////////////////////////////////////////////////////////////////////////////
// Control Parameter

CF16Var
  RCBoxMinPulseWidth( Control, "RCBoxMinPulseWidth", 1050, 800, 1300 ),
  RCBoxMaxPulseWidth( Control, "RCBoxMaxPulseWidth", 2100, 1800, 2300 );
CBVar
  RCBoxNegPulse( Control, "RCBoxNegPulse", FALSE );
CBVar RCBoxNegChannel[] = {
  CBVar( Control, "RCBoxNegChannelA", FALSE ),
  CBVar( Control, "RCBoxNegChannelB", FALSE ),
  CBVar( Control, "RCBoxNegChannelC", FALSE ),
  CBVar( Control, "RCBoxNegChannelD", FALSE )
};

/////////////////////////////////////////////////////////////////////////////
// Hubschrauber-Parameter

CF16Var Sensitivity[] = {
  CF16Var( Heli, "RollSensitivity",       25.0, 0.0, 100.0 ),
  CF16Var( Heli, "GierSensitivity",       50.0, 0.0, 100.0 ),
  CF16Var( Heli, "NickSensitivity",       25.0, 0.0, 100.0 ),
};
CF16Var Spin[] = {
  CF16Var( Heli, "RollSpin",              60.0, 1.0, 100.0 ),
  CF16Var( Heli, "GierSpin",              50.0, 1.0, 100.0 ),
  CF16Var( Heli, "NickSpin",              60.0, 1.0, 100.0 ),
};
CF16Var Inertia[] = {
  CF16Var( Heli, "RollInertia",           35.0, 1.0, 50.0 ),
  CF16Var( Heli, "GierInertia",           20.0, 1.0, 50.0 ),
  CF16Var( Heli, "NickInertia",           35.0, 1.0, 50.0 ),
};
CF16Var TurnWithWind[] = {
  CF16Var( Heli, "RollWithWind",          0.0, 0.0, 10.0 ),
  CF16Var( Heli, "WeatherFane",           2.0, 0.0, 10.0 ),
  CF16Var( Heli, "PitchWithWind",         0.0, 0.0, 10.0 ),
};
CF16Var Drag[] = {
  CF16Var( Heli, "FrontDrag",             0.1, 0.0, 10.0 ),
  CF16Var( Heli, "TopDrag",               0.1, 0.0, 10.0 ),
  CF16Var( Heli, "SideDrag",              0.1, 0.0, 10.0 ),
};
CF16Var RPMSlow[] = {
  CF16Var( Heli, "RollRPMSlow",           0.0 ),
  CF16Var( Heli, "GierRPMSlow",           0.0 ),
  CF16Var( Heli, "NickRPMSlow",           0.0 ),
};

CF16Var
  Mass(               Heli, "Mass",                   1.0, 5.0, 10.0 ),
  MassInertia(        Heli, "MassInertia",            2.5, 0.1, 10.0 ),
  EnginePower(        Heli, "EnginePower",            1.5, 0.0, 100.0 ),
  EngineRiseRate(     Heli, "EngineRiseRate",         0.5, 0.0, 100.0 ),
  MainRotorThrust(    Heli, "MainRT",                 2.0, 0.0, 100.0 ),
  TailRotorThrust(    Heli, "TailRT",                 0.5, 0.0, 100.0 ),
  RPMLift(            Heli, "RPMLift",                1.0, 0.0, 100.0 ),
  CollectiveLift(     Heli, "CollectiveLift",         0.0, 0.0, 100.0 ),
  TranslationalLift(  Heli, "TranslationalLift",      0.0, 0.0, 100.0 ),
  GroundEffectRange(  Heli, "GroundEffectRange",      1.0, 0.1, 10.0 ),
  GroundEffectFactor( Heli, "GroundEffectFactor",     0.3, 0.0, 2.0 );

/////////////////////////////////////////////////////////////////////////////
// Wind Parameter

CVarSection Wind( VarList, "Wind" );

CBVar
  WindOn( Wind, "Wind", FALSE );
CF16Var
  WindBaseSpeed(          Wind, "Speed",          1.0, 0.0, 100.0 ),
  WindSpeedChgFreq(       Wind, "SpeedChgFreq",   0.0, 0.0, 100.0 ),
  WindSpeedChgRange(      Wind, "SpeedChgRange",  0.0, 0.0, 100.0 ),
  WindSpeedRateOfChg(     Wind, "SpeedRateOfChg", 0.0, 0.0, 100.0 ),
  WindSpeedChgDurMin(     Wind, "SpeedChgDurMin", 0.0, 0.0, 100.0 ),
  WindSpeedChgDurMax(     Wind, "SpeedChgDurMax", 0.0, 0.0, 100.0 ),
  WindBaseDir(            Wind, "Direction",      0.0, 0.0, 360.0 ),
  WindDirChgFreq(         Wind, "DirChgFreq",     0.0, 0.0, 100.0 ),
  WindDirChgRange(        Wind, "DirChgRange",    0.0, 0.0, 100.0 ),
  WindDirRateOfChg(       Wind, "DirRateOfChg",   0.0, 0.0, 100.0 ),
  WindDirChgDurMin(       Wind, "DirChgDurMin",   0.0, 0.0, 100.0 ),
  WindDirChgDurMax(       Wind, "DirChgDurMax",   0.0, 0.0, 100.0 ),
  GustFreq(               Wind, "GustFreq",       0.0, 0.0, 100.0 ),
  GustPeakMin(            Wind, "GustPeakMin",    0.0, 0.0, 100.0 ),
  GustPeakMax(            Wind, "GustPeakMax",    0.0, 0.0, 100.0 ),
  GustDirMin(             Wind, "GustDirMin",     0.0, 0.0, 100.0 ),
  GustDirMax(             Wind, "GustDirMax",     0.0, 0.0, 100.0 ),
  GustDurMin(             Wind, "GustDurMin",     0.0, 0.0, 100.0 ),
  GustDurMax(             Wind, "GustDurMax",     0.0, 0.0, 100.0 );

