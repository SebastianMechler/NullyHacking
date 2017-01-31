#pragma once

#include "NTypes.h"
#include "NString.h"

namespace Nully
{
  struct NProcessModule
  {
    void* address;
    NString name;
  };
}
