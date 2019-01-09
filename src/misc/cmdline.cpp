//
// cmdline.cpp, command line processing
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include <ctype.h>
#include <iostream.h>
#include <string.h>
#include <strstrea.h>
#include "cmdline.hpp"

#define SWITCH_CHAR1    '-'
#define SWITCH_CHAR2    '/'

#pragma initialize after library
CISList<CCmdLineSwitch> CCmdLine::SwitchList;

// neuen Schalter
CCmdLineSwitch::CCmdLineSwitch( const char *_Name )
{
  CCmdLine::SwitchList.Append( this );
  Name = _Name;
  Active = FALSE;
}

// Kommandozeile auswerten
void CCmdLine::Evaluate( int argc, char *argv[] )
{
  int Arg = 1;
  while( Arg < argc ) {
    char *ArgText = argv[ Arg++ ];
    if( *ArgText == SWITCH_CHAR1 || *ArgText == SWITCH_CHAR2 ) {
      ArgText++;
      for( CISIter<CCmdLineSwitch> SwitchIter( SwitchList ); SwitchIter(); SwitchIter++ ) {
        if( stricmp( ArgText, SwitchIter()->Name ) == 0 ) {
          SwitchIter()->Active = TRUE;
          break;
        }
      }
      if( SwitchIter() == NULL ) {
        cerr << "Invalid option near '-" << ArgText << "'";
        throw CError( "" );
      }
    } else {
      ArgList.Append( new CCmdLineArg( ArgText ) );
    }
  }
}

// neues Argument speichern
const char *CCmdLine::GetArg( int ArgC )
{
  CISIter<CCmdLineArg> ArgIter( ArgList );
  CCmdLineArg *Arg = ArgIter[ ArgC ];
  if( Arg == NULL )
    return NULL;
  return Arg->ArgText;
}


