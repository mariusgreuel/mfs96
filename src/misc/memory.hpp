//
// memory.hpp, memory handling
//
// Copyright Marius Greuel 1996. All rights reserved.
//

#ifndef _MEMORY_HPP_INCLUDED
#define _MEMORY_HPP_INCLUDED

#include "error.hpp"

class CMemory : public CError {
public:
  class Corrupted {};
  class Exhausted {};
  CMemory();
};

#endif

