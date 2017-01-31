#pragma once

#include <stdint.h>

namespace Nully
{
  enum class NClassType : uint8_t
  {
    None = 0,
    Warrior = 1,
    Paladin = 2,
    Hunter = 3,
    Rogue = 4,
    Priest = 5,
    Deathknight = 6,
    Shaman = 7,
    Mage = 8,
    Warlock = 9,
    Monk = 10,
    Druid = 11,
  };
}