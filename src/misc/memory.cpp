//
// memory.cpp, memory handling
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#include <new.h>
#include "memory.hpp"

// neuer NoMemory-Handler
static void MyNewHandler()
{
  throw CMemory::Exhausted();
}

// new_handler setzen
CMemory::CMemory()
{
  set_new_handler( MyNewHandler );
}

