//
// variable.cpp, static variable routines
//
// Copyright Marius Greuel 1996. All rights reserved.
//
// CAUTION: all CVarKey, CVarSection, CVarList and it's names have to be static!
//

#include "variable.hpp"

/////////////////////////////////////////////////////////////////////////////
// class VarKey

CVarKey::CVarKey( CVarSection &Section, const char *_Name )
{
  Name = _Name;
  Section.AddKey( this );
}

/////////////////////////////////////////////////////////////////////////////
// class VarSection

CVarSection::CVarSection( CVarList &VarList, const char *_Name )
{
  Name = _Name;
  VarList.AddSection( this );
}

// search key in list
CVarKey *CVarSection::SearchKey( const String &Name ) const
{
  for( CVarKeyIter KeyIter( this ); KeyIter(); KeyIter++ )
    if( KeyIter()->GetName() == Name )
      break;
  return KeyIter();
}

/////////////////////////////////////////////////////////////////////////////
// class VarList

// search section in list
CVarSection *CVarList::SearchSection( const String &Name ) const
{
  for( CVarSectionIter SectionIter( this ); SectionIter(); SectionIter++ )
    if( SectionIter()->GetName() == Name )
      break;
  return SectionIter();
}

/////////////////////////////////////////////////////////////////////////////
// operator << (stream, class)

// show keys
ostream &operator << ( ostream &strm, const CVarKey &VarKey )
{
  strm << VarKey.GetName() << '=';
  VarKey >> strm;
  return strm << '\n';
}

// show section with keys
ostream &operator << ( ostream &strm, const CVarSection &Section )
{
  strm << "\n[" << Section.GetName() << "]\n";
  for( CVarKeyIter KeyIter( &Section ); KeyIter(); KeyIter++ )
    strm << *KeyIter();
  return strm;
}

// show whole list of variables
ostream &operator << ( ostream &strm, const CVarList &VarList )
{
  for( CVarSectionIter SectionIter( &VarList ); SectionIter(); SectionIter++ )
    strm << *SectionIter();
  return strm;
}


