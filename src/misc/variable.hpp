//
// variable.hpp, static variable routines
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _VARIABLE_HPP_INCLUDED
#define _VARIABLE_HPP_INCLUDED

#include <stdlib.h>
#include <string.hpp>
#include "types.h"
#include "list.hpp"

class CVarSection;
class CVarList;

class CVarKey : public CISLink<CVarKey> {
  const char *Name;
public:
  CVarKey( CVarSection &Section, const char *Name );
  const char *GetName() const { return Name; }
  virtual void operator = ( const char *Value ) = 0;
  virtual ostream &operator >> ( ostream &strm ) const = 0;
};

class CVarSection : public CISLink<CVarSection> {
  CISList<CVarKey> KeyList;
  const char *Name;
public:
  CVarSection( CVarList &VarList, const char *Name );
  const char *GetName() const { return Name; }
  AddKey( CVarKey *Key ) { KeyList.Append( Key ); }
  CVarKey *SearchKey( const String &Name ) const;
  friend class CVarKeyIter;
};

class CVarList {
  CISList<CVarSection> SectionList;
public:
  AddSection( CVarSection *Section ) { SectionList.Append( Section ); }
  CVarSection *SearchSection( const String &Name )  const;
  friend class CVarSectionIter;
};

class CVarKeyIter : public CISIter<CVarKey> {
public:
  CVarKeyIter( const CVarSection *Section ) : CISIter<CVarKey>( Section->KeyList ) {}
};

class CVarSectionIter : public CISIter<CVarSection> {
public:
  CVarSectionIter( const CVarList *VarList ) : CISIter<CVarSection>( VarList->SectionList ) {}
};

class CBVar : public CVarKey {
  BOOL Value, Default;
public:
  CBVar( CVarSection &Section, const char *Name, BOOL _Value ) :
         CVarKey( Section, Name ) { Value = Default = _Value; }
  void operator = ( const char *ValueStrg ) { Value = atoi( ValueStrg ); }
  void operator = ( BOOL _Value ) { Value = _Value; }
  void operator = ( const CBVar &Var ) { Value = Var.Value; }
  operator BOOL() const { return Value; }
  BOOL GetValue() { return Value; }
  BOOL GetDefault() { return Default; }
  ostream &operator >> ( ostream &strm ) const { return strm << ( Value ? '1' : '0' ); }
};

class CFVar : public CVarKey {
  float Value, Default;
public:
  CFVar( CVarSection &Section, const char *Name, float _Value ) :
         CVarKey( Section, Name ) { Value = Default = _Value; }
  void operator = ( const char *ValueStrg ) { Value = atoi( ValueStrg ); }
  void operator = ( float _Value ) { Value = _Value; }
  void operator = ( const CFVar &Var ) { Value = Var.Value; }
  operator float() const { return Value; }
  float GetValue() { return Value; }
  float GetDefault() { return Default; }
  ostream &operator >> ( ostream &strm ) const { return strm << ( Value ? '1' : '0' ); }
};

ostream &operator << ( ostream &strm, const CVarKey &VarKey );
ostream &operator << ( ostream &strm, const CVarSection &VarSection );
ostream &operator << ( ostream &strm, const CVarList &VarList );

#endif // _VARIABLE_HPP_INCLUDED

