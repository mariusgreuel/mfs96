//
// infofile.cpp, information file routines
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include "debug.hpp"
#include "infofile.hpp"

#define COMMENT_CHAR            ';'
#define QUOTE_CHAR              '"'
#define SECTION_START_CHAR      '['
#define SECTION_END_CHAR        ']'
#define ASSIGN_CHAR             '='
#define SEQUENCE_CHAR           ','

static const char *DefaultSection = "Default";
static const char *IncludeSection = "Include";

static String NullString = "";

/////////////////////////////////////////////////////////////////////////////
// class Key

// destroy all arguments
void CKey::DestroyArgs()
{
  CArg *Arg;
  while( ( Arg = ArgList.Get() ) != NULL )
    delete Arg;
}

// add argument to key
void CKey::AddArg( const String &Value )
{
  CArg *Arg = new CArg( Value );
  ArgList.Append( Arg );
}

// return value from key
const String &CKey::GetArg( int ArgC ) const
{
  CArgIter ArgIter( this );
  CArg *Arg = ArgIter[ ArgC ];
  if( Arg == NULL )
    return NullString;
  return Arg->GetArg();
}

/////////////////////////////////////////////////////////////////////////////
// class Section

// destroy all keys
void CSection::DestroyKeys()
{
  CKey *Key;
  while( ( Key = KeyList.Get() ) != NULL )
    delete Key;
}

// search key in list
CKey *CSection::SearchKey( const String &Name ) const
{
  for( CKeyIter KeyIter( this ); KeyIter(); KeyIter++ )
    if( KeyIter()->GetName() == Name )
      break;
  return KeyIter();
}

// add key to section
CKey *CSection::AddKey( const String &Name )
{
  CKey *Key;
  if( Name != "" && ( Key = SearchKey( Name ) ) != NULL ) {
    Key->DestroyArgs();
  } else {
    Key = new CKey( Name );
    KeyList.Append( Key );
  }
  return Key;
}

// return value from key
const String &CSection::GetArg( const String &KeyName, int ArgC ) const
{
  CKey *Key = SearchKey( KeyName );
  if( Key == NULL )
    return NullString;
  return Key->GetArg( ArgC );
}

/////////////////////////////////////////////////////////////////////////////
// class InfoList

// destroy all sections
void CInfoList::DestroySections()
{
  CSection *Section;
  while( ( Section = SectionList.Get() ) != NULL )
    delete Section;
}

// search section in list
CSection *CInfoList::SearchSection( const String &Name ) const
{
  for( CSectionIter SectionIter( this ); SectionIter(); SectionIter++ )
    if( SectionIter()->GetName() == Name )
      break;
  return SectionIter();
}

// add section to infolist
CSection *CInfoList::AddSection( const String &Name )
{
  CSection *Section = SearchSection( Name );
  if( Section == NULL ) {
    Section = new CSection( Name );
    SectionList.Append( Section );
  }
  return Section;
}

// return value from key in section
const String &CInfoList::GetArg( const String &SectionName, const String &KeyName, int ArgC ) const
{
  CSection *Section = SearchSection( SectionName );
  if( Section == NULL )
    return NullString;
  return Section->GetArg( KeyName, ArgC );
}

/////////////////////////////////////////////////////////////////////////////
// class InfoFile

// skip whitespace and comments (preceded by a semicolon)
// return TRUE if char follows
BOOL CInfoFile::SkipWhiteSpace()
{
  char ch;
  while( InFile.get( ch ) && isspace( ch ) && ch != '\n' );
  InFile.putback( ch );
  if( InFile.eof() || ch == '\n' || ch == COMMENT_CHAR )
    return FALSE;
  return TRUE;
}

// skip a given char
// return TRUE if char found
BOOL CInfoFile::SkipChar( char SkipChar )
{
  char ch;
  if( SkipWhiteSpace() == FALSE )
    return FALSE;
  InFile.get( ch );
  if( ch == SkipChar )
    return TRUE;
  InFile.putback( ch );
  return FALSE;
}

// return a string (with or without quotes)
String CInfoFile::GetString()
{
  char ch;
  String Word;
  if( SkipChar( QUOTE_CHAR ) ) {
    while( InFile.get( ch ) && ch != '\n' && ch != QUOTE_CHAR )
      Word += ch;
    InFile.putback( ch );
    SkipChar( QUOTE_CHAR );
  } else {
    while( InFile.get( ch ) && ( isalnum( ch  ) ||
        ch == '_' || ch == '.' || ch == '+' || ch == '-' ) )
      Word += ch;
    InFile.putback( ch );
  }
  return Word;
}

// scan file and build info list
void CInfoFile::ScanInfoFile( const char *FileName )
{
  if( GetDebugLevel() >= 2 )
    cout << "Process file '" << FileName << "'\n";
  InFile.open( FileName, ios::in );
  if( InFile.fail() )
    throw CantOpenFile( FileName );
  CSection *Section = SearchSection( DefaultSection );
  while( !InFile.eof() ) {
    if( SkipWhiteSpace() ) {
      if( SkipChar( SECTION_START_CHAR ) ) {
        String SectionName = GetString();
        SkipChar( SECTION_END_CHAR );
        Section = AddSection( SectionName );
      } else {
        CKey *Key;
        String KeyName = GetString();
        if( SkipChar( ASSIGN_CHAR ) ) {
          Key = Section->AddKey( KeyName );
          Key->AddArg( GetString() );
        } else {
          Key = Section->AddKey( "" );
          Key->AddArg( KeyName );
        }
        while( SkipChar( SEQUENCE_CHAR ) )
          Key->AddArg( GetString() );
      }
    }
    char ch;
    while( InFile.get( ch ) && ch != '\n' );
  }
  InFile.close();
}

// add input file to section 'Include'
CInfoFile::AddFile( const char *FileName )
{
  AddSection( IncludeSection )->AddKey( "" )->AddArg( FileName );
}

// scan files listed insection 'Include'
void CInfoFile::ScanFiles()
{
  CKey LastFiles( "" );
  AddSection( DefaultSection );
  CSection *Section = SearchSection( IncludeSection );
  for( CKeyIter KeyIter( Section ); KeyIter(); KeyIter++ )
    for( CArgIter ArgIter( KeyIter() ); ArgIter(); ArgIter++ ) {
      String FileName = ArgIter()->GetArg();
      for( CArgIter LastFilesIter( &LastFiles ); LastFilesIter(); LastFilesIter++ )
        if( LastFilesIter()->GetArg() == FileName )
          throw IncludeRecursion( FileName );
      LastFiles.AddArg( FileName );
      ScanInfoFile( FileName );
    }
}

/////////////////////////////////////////////////////////////////////////////
// operator << (stream, class)

// show key with arguments
ostream &operator << ( ostream &strm, const CKey &Key )
{
  if( Key.GetName() != "" )
    strm << Key.GetName() << '=';
  CArgIter ArgIter( &Key );
  do {
    strm << ArgIter()->GetArg();
    ArgIter++;
    if( ArgIter() )
      strm << ',';
  } while( ArgIter() );
  strm << '\n';
  return strm;
}

// show section with keys
ostream &operator << ( ostream &strm, const CSection &Section )
{
  strm << "\n[" << Section.GetName() << "]\n";
  for( CKeyIter KeyIter( &Section ); KeyIter(); KeyIter++ )
    strm << *KeyIter();
  return strm;
}

// show whole infofile
ostream &operator << ( ostream &strm, const CInfoList &InfoList )
{
  for( CSectionIter SectionIter( &InfoList ); SectionIter(); SectionIter++ )
    strm << *SectionIter();
  return strm;
}


