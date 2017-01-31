#pragma once

#include <NTypes.h>

namespace Nully
{
  enum class NObjectType : uint32_t
  {
    NONE = 0,
    ITEM = 1,
    CONTAINER = 2,
    UNIT = 3,
    PLAYER = 4,
    GAMEOBJ = 5,
    DYNOBJ = 6,
    CORPSE = 7,
    FORCEDWORD = 0xFFFFFFFF
  };
}
