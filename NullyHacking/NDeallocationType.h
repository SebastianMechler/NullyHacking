#pragma once
#pragma once
#include <Windows.h>

#include "NTypes.h"

namespace Nully
{
  enum class NDeallocationType : uint32_t
  {
    Decommit = MEM_DECOMMIT, // Decommits the specified region of committed pages. After the operation, the pages are in the reserved state. Do not use this value with MEM_RELEASE.
    Release = MEM_RELEASE, // Releases the specified region of pages. After the operation, the pages are in the free state. Do not use this value with MEM_DECOMMIT.
  };

  // operator overloading to allow logical or
  inline NDeallocationType operator|(NDeallocationType a_left, NDeallocationType a_right)
  {
    return static_cast<NDeallocationType>(static_cast<int>(a_left) | static_cast<int>(a_right));
  }
}