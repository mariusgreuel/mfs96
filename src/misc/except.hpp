//
// except.hpp - dafault exception handling
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _EXCEPT_HPP_INCLUDED
#define _EXCEPT_HPP_INCLUDED

extern "C" {
  extern void __far *OldExceptionHandler0;
  extern void __far *OldExceptionHandler6;
  extern void __far *OldExceptionHandler0C;
  extern void __far *OldExceptionHandler0D;
  extern void __far *OldExceptionHandler0E;
  void ExceptionHandler0();
  void ExceptionHandler6();
  void ExceptionHandler0C();
  void ExceptionHandler0D();
  void ExceptionHandler0E();
};

class CException {
public:
  CException();
  ~CException();
};

#endif // _EXCEPT_HPP_INCLUDED

