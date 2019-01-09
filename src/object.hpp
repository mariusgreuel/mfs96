//
// object.hpp
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _OBJECT_HPP_INCLUDED
#define _OBJECT_HPP_INCLUDED

#include <string.hpp>
#include "types.hpp"
#include "list.hpp"
#include "vecmath.hpp"
#include "graphic.hpp"

#define MAX_POINTS      32

struct PGCOLOR {
  COLORREG FGD, FGF, BGD, BGF;
};

struct C3D_Point {
  CVec16  op;           // Objektpunkt
  CVec16  tp;           // transformierter op
  CLPoint pp;           // projezierter tp
};

class CBspPlane {
public:
  class CBspPlane *Before, *Behind;
  PGCOLOR Color;
  int Points;
  C3D_Point *Point[ MAX_POINTS ];
protected:
  CHVec16 GetNormalVector();
  int LocatePoints( const CHVec16 &RefPlaneVector );
  void SetToTop( CBspPlane **List );
public:
  CBspPlane( int _Points, int Index[], C3D_Point *_Point, PGCOLOR _Color );
  int StorePoints( int Points );
  void Display();
  friend class CBspObject;
};

// Polygonorientiertes Objekt
class CBspObject : public CIDLink<CBspObject> {
  static CIDList<CBspObject> BspObjectList;
public:
  int Points;
  int Polygons;
  C3D_Point *Point;
  CBspPlane *BspRoot;
  String Name;
  CBspObject *Shadow;
protected:
  CBspPlane *SearchSplitPlane( CBspPlane *List );
  void BuildBspSubTree( CBspPlane **Root );
public:
  CBspObject( const char *Name, int Points );
  ~CBspObject();
  friend CBspObject *SearchBspObject( const char *ObjectName );
  void AddPoint( const CVec16 &v );
  void AddPolygon( int Points, int PointList[], PGCOLOR Color );
  void AddShadow( CBspObject *Shadow );
  void BuildBspTree();
  CBspObject *GetShadow() { return Shadow; }
  void Display( const CMatrix16 &Matrix );
};

// Objekt
class CObject : public CIDLink<CObject> {
  static CIDList<CObject> ObjectList;
  CBspObject *BspObject;
public:
  CMatrix30 Matrix;
public:
  CObject( CBspObject *BspObject, const CVec16 &Position );
  ~CObject();
  void Move( const CVec16 &Vector );
  void Rotate( const CVec16 &Vector );
  friend void DisplayScene( const CRect &r );
};

#endif


