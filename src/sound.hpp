//
// sound.hpp
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include "sbfm.hpp"

class CSound : public CFMSound {
public:
  void Startup();
  void DefineVoices();
  void EngineOn();
  void EngineOff();
  void AdjustEngineSound( int rpm );
};


