//
// script.cpp, evaluate script files
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include <string.hpp>
#include "debug.hpp"
#include "error.hpp"
#include "variable.hpp"
#include "infofile.hpp"
#include "object.hpp"
#include "script.hpp"

extern CGraphic Graphic;
extern CVarList VarList;

static const char *SCRIPT_FILENAME = "mfs.if";
static const char *SECTION_COLOR   = "Colors";
static const char *SECTION_OBJECT  = "Objects";
static const char *SECTION_POINT   = "Points";
static const char *SECTION_PLANE   = "Polygons";

/////////////////////////////////////////////////////////////////////////////
// class description

class CScript : public CInfoFile {
protected:
  CVec16 GetVector( const CKey *Key, int Index = 0 );
  COLORREG GetColorReg( String ColorName );
  CBspObject *NewBspObject( String ObjectName );
public:
  void EvalVariableSection();
  void EvalColorSection();
  void EvalObjectSection();
};

class CScriptError : public CError { public:
  CScriptError() { cerr << "mfs(0): "; }
};
class NoColorSection : public CScriptError { public:
  NoColorSection() { cerr << "No colors defined in section '" << SECTION_COLOR << "'\n"; }
};
class UndefinedColor : public CScriptError { public:
  UndefinedColor( String Color ) { cerr << "Undefined color '" << Color << "'"; }
};
class ObjectNotFound : public CScriptError { public:
  ObjectNotFound( String Object ) { cerr << "Object '" << Object << "' not found"; }
};
class ObjectHasNoPoints : public CScriptError { public:
  ObjectHasNoPoints( String Object ) { cerr << "Object '" << Object << "' has no point section"; }
};
class ObjectHasNoPlanes : public CScriptError { public:
  ObjectHasNoPlanes( String Object ) { cerr << "Object '" << Object << "' has no Plane section"; }
};

/////////////////////////////////////////////////////////////////////////////
// help functions

CVec16 CScript::GetVector( const CKey *Key, int Index )
{
  return CVec16(
        atof( Key->GetArg( Index ) ),
        atof( Key->GetArg( Index + 1 ) ),
        atof( Key->GetArg( Index + 2 ) ) );
}

COLORREG CScript::GetColorReg( String ColorName )
{
  CSection *Section = SearchSection( SECTION_COLOR );
  if( Section == NULL )
    throw NoColorSection();
  String Value = Section->GetArg( ColorName );
  if( Value == "" )
    throw UndefinedColor( ColorName );
  return atoi( Value );
}

/////////////////////////////////////////////////////////////////////////////
// evaluate all section relating to varlist

void CScript::EvalVariableSection()
{
  for( CVarSectionIter SectionIter( &VarList ); SectionIter(); SectionIter++ ) {
    CSection *Section = SearchSection( SectionIter()->GetName() );
    if( Section )
      for( CVarKeyIter KeyIter( SectionIter() ); KeyIter(); KeyIter++ ) {
        CKey *Key = Section->SearchKey( KeyIter()->GetName() );
        if( Key )
          *KeyIter() = Key->GetArg();
      }
  }
}

/////////////////////////////////////////////////////////////////////////////
// evaluate section 'Color'

void CScript::EvalColorSection()
{
  CSection *Section = SearchSection( SECTION_COLOR );
  if( Section == NULL )
    throw NoColorSection();
  for( CKeyIter KeyIter( Section ); KeyIter(); KeyIter++ )
    Graphic.SetPaletteEntry( atoi( KeyIter()->GetArg( 0 ) ), HTC2RGB(
        atoi( KeyIter()->GetArg( 1 ) ),
        atoi( KeyIter()->GetArg( 2 ) ),
        atoi( KeyIter()->GetArg( 3 ) ) ) );
}

/////////////////////////////////////////////////////////////////////////////
// evaluate section 'Object'

CBspObject *CScript::NewBspObject( String ObjectName )
{
  if( GetDebugLevel() >= 2 )
    cout << "Build object '" << ObjectName << "': ";
  CSection *ObjectSection = SearchSection( ObjectName );
  if( ObjectSection == NULL )
    throw ObjectNotFound( ObjectName );
  String PointSectionName = ObjectSection->GetArg( SECTION_POINT );
  CSection *PointSection = SearchSection( PointSectionName );
  if( PointSection == NULL )
    throw ObjectHasNoPoints( ObjectName );
  String PlaneSectionName = ObjectSection->GetArg( SECTION_PLANE );
  CSection *PlaneSection = SearchSection( PlaneSectionName );
  if( PlaneSection == NULL )
    throw ObjectHasNoPlanes( ObjectName );
  int Points = PointSection->CountKeys();
  int Planes = PlaneSection->CountKeys();
  if( GetDebugLevel() >= 2 )
    cout << Points << " points, " << Planes << " polygons.\n";
  CBspObject *BspObject = new CBspObject( ObjectName, Points );
  for( CKeyIter PtKeyIter( PointSection ); PtKeyIter(); PtKeyIter++ )
    BspObject->AddPoint( GetVector( PtKeyIter() ) );
  for( CKeyIter PKeyIter( PlaneSection ); PKeyIter(); PKeyIter++ ) {
    PGCOLOR Color;
    Color.FGD = GetColorReg( PKeyIter()->GetArg( 0 ) );
    Color.FGF = GetColorReg( PKeyIter()->GetArg( 1 ) );
    Color.BGD = GetColorReg( PKeyIter()->GetArg( 2 ) );
    Color.BGF = GetColorReg( PKeyIter()->GetArg( 3 ) );
    int Points = 0, PointList[ 50 ];
    for( CArgIter PtArgIter( PKeyIter(), 4 ); PtArgIter(); PtArgIter++ ) {
      int Index = 0;
      for( CKeyIter PtKeyIter( PointSection ); PtKeyIter(); PtKeyIter++ ) {
        if( PtKeyIter()->GetName() == PtArgIter()->GetArg() )
          break;
        Index++;
      }
      if( PtKeyIter() )
        PointList[ Points++ ] = Index;
      else
        cout << "Point '" << PtArgIter()->GetArg() << "' not found\n";
    }
    BspObject->AddPolygon( Points, PointList, Color );
  }
  BspObject->BuildBspTree();
  return BspObject;
}

void CScript::EvalObjectSection()
{
  CSection *Section = SearchSection( SECTION_OBJECT );
  if( Section == NULL )
    return;
  for( CKeyIter KeyIter( Section ); KeyIter(); KeyIter++ ) {
    String ObjectName = KeyIter()->GetArg();
    CBspObject *BspObject = SearchBspObject( ObjectName );
    if( BspObject == NULL ) {
      BspObject = NewBspObject( ObjectName );
      String ShadowObjectName = GetArg( ObjectName, "Shadow" );
      if( ShadowObjectName )
        BspObject->AddShadow( NewBspObject( ShadowObjectName ) );
    }
    CObject *Object = new CObject( BspObject, GetVector( KeyIter(), 1 ) );
  }
}

/////////////////////////////////////////////////////////////////////////////
// evaluate script file

void EvalScriptFile( const char *ScriptFile )
{
  CScript InfoFile;
  if( ScriptFile != NULL )
    InfoFile.AddFile( ScriptFile );
  else
    InfoFile.AddFile( SCRIPT_FILENAME );
  InfoFile.ScanFiles();
  InfoFile.EvalVariableSection();
  InfoFile.EvalColorSection();
  InfoFile.EvalObjectSection();
}


