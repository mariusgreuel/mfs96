//
// infofile.hpp, information file routines
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _INFOFILE_HPP_INCLUDED
#define _INFOFILE_HPP_INCLUDED

#include <fstream.h>
#include <string.hpp>
#include "types.h"
#include "list.hpp"
#include "error.hpp"

class CInfoFileError : public CError {};

class CArg : public CISLink<CArg> {
  const String Value;
public:
  CArg( const String &_Value ) : Value( _Value ) {}
  const String &GetArg() const { return Value; }
};

class CKey : public CISLink<CKey> {
  CISList<CArg> ArgList;
  const String Name;
protected:
  void DestroyArgs();
public:
  CKey( const String &_Name ) : Name( _Name ) {}
  ~CKey() { DestroyArgs(); }
  const String &GetName() const { return Name; }
  void AddArg( const String &Value );
  const String &GetArg( int ArgC = 0 ) const;
  int CountArgs() const { return ArgList.Count(); }
  friend class CArgIter;
  friend class CSection;
};

class CSection : public CISLink<CSection> {
  CISList<CKey> KeyList;
  const String Name;
protected:
  void DestroyKeys();
public:
  CSection( const String &_Name ) : Name( _Name ) {}
  ~CSection() { DestroyKeys(); }
  const String &GetName() const { return Name; }
  CKey *SearchKey( const String &Name ) const;
  CKey *AddKey( const String &Name );
  const String &GetArg( const String &KeyName, int ArgC = 0 ) const;
  int CountKeys() const { return KeyList.Count(); }
  friend class CKeyIter;
};

class CInfoList {
  CISList<CSection> SectionList;
protected:
  void DestroySections();
public:
  ~CInfoList() { DestroySections(); }
  CSection *SearchSection( const String &Name ) const;
  CSection *AddSection( const String &Name );
  const String &GetArg( const String &SectionName, const String &KeyName, int ArgC = 0 ) const;
  int CountSections() const { return SectionList.Count(); }
  friend class CSectionIter;
};

class CInfoFile : public CInfoList {
  ifstream InFile;
protected:
  BOOL SkipWhiteSpace();
  BOOL SkipChar( char SkipChar );
  String GetString();
  void ScanInfoFile( const char *FileName );
protected:
  class CantOpenFile : public CInfoFileError { public:
    CantOpenFile( const char *FileName ) { cerr << "Cant open '" << FileName << "'"; }
  };
  class IncludeRecursion : public CInfoFileError { public:
    IncludeRecursion( const char *FileName ) { cerr << "Include recursion '" << FileName << "'"; }
  };
public:
  AddFile( const char *Filename );
  void ScanFiles();
};

class CArgIter : public CISIter<CArg> {
public:
  CArgIter( const CKey *Key, int Start = 0 ) : CISIter<CArg>( Key->ArgList, Start ) {}
};

class CKeyIter : public CISIter<CKey> {
public:
  CKeyIter( const CSection *Section ) : CISIter<CKey>( Section->KeyList ) {}
};

class CSectionIter : public CISIter<CSection> {
public:
  CSectionIter( const CInfoList *InfoList ) : CISIter<CSection>( InfoList->SectionList ) {}
};

ostream &operator << ( ostream &strm, const CKey &Key );
ostream &operator << ( ostream &strm, const CSection &Section );
ostream &operator << ( ostream &strm, const CInfoList &InfoList );

#endif // _INFOFILE_HPP_INCLUDED


