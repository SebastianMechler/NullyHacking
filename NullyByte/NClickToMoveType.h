#pragma once

#include "NTypes.h"

namespace Nully
{
  enum class NClickToMoveType : uint32_t
  {
    Ground = 0x4,
    Stop = 0xC,

    FaceTarget = 0x2,
    //Stop = 0x3,
    //WalkTo = 0x4,
    InteractNpc = 0x5,
    Loot = 0x6,
    InteractObject = 0x7,
    Unknown1 = 0x8,
    Unknown2 = 0x9,
    AttackPos = 0xA,
    AttackGuid = 0xB,
  };
}
