//
// bsptree.hpp
//
// Copyright Marius Greuel 1996. All rights reserved
//

#include "types.h"
#include "math.hpp"

class CBspPlane {
  union {
    CBspPlane *Next;
    CBspPlane *BeforeList;
  };
  CBspPlane *BehindList;
protected:
  int Points;
  CVector GetPoint( int );
protected:
  CHVector GetNormalVector();
  BYTE LocatePoints( const CHVector &RefPlaneVector );
public:
};

class CBspTree {
  CBspPlane *BspRoot;
protected:
  CBspPlane *SearchSplitLink( CBspPlane *BspList );
public:
  void BuildTree( CBspPlane **BspList );
};


