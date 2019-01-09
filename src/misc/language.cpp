//
// language.cpp
//
// Copyright Marius Greuel 1996. All rights reserved.
//

int StrCount = 0;

class CStr {
  static int StrCount;
  int StrID;
  const char *Message;
public:
  CStr();
  const char *operator ()
};

CStr::CStr()
{
  StrID = ++StrCount;
}

CStr Hello
