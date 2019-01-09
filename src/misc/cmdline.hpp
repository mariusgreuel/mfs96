//
// cmdline.hpp, command line processing
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _CMDLINE_HPP_INCLUDED
#define _CMDLINE_HPP_INCLUDED

#include <string.hpp>
#include "types.h"
#include "error.hpp"
#include "list.hpp"

class CCmdLineSwitch : public CISLink<CCmdLineSwitch> {
  const char *Name;
  BOOL Active;
public:
  CCmdLineSwitch( const char *_Name );
  BOOL IsOn() { return Active; }
  BOOL IsOff() { return !Active; }
  friend class CCmdLine;
};

class CCmdLineArg : public CISLink<CCmdLineArg> {
  const char *ArgText;
protected:
  CCmdLineArg( const char *_ArgText ) { ArgText = _ArgText; }
  friend class CCmdLine;
};

class CCmdLine {
  static CISList<CCmdLineSwitch> SwitchList;
private:
  CISList<CCmdLineArg> ArgList;
public:
  void Evaluate( int argc, char *argv[] );
  const char *GetArg( int ArgC );
  friend class CCmdLineSwitch;
};

#endif // _CMDLINE_HPP_INCLUDED

