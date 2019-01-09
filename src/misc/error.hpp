//
// error.hpp - default error handling
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _ERROR_HPP_INCLUDED
#define _ERROR_HPP_INCLUDED

#include "types.h"

class CError {
  static int Errors;
  static int Fatals;
public:
  const char *Message;
public:
  CError() : Message( NULL ) {}
  CError( const char *m ) : Message( m ) {}
};

#endif // _ERROR_HPP_INCLUDED

