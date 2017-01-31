#pragma once

#include "NTypes.h"

namespace Nully
{
  // NOT TESTED
  enum class NMovementFlag : uint32_t
  {
    WalkForwards = 0x10000001,
    WalkBackwards = 0x10000002,
    StrafeLeft = 0x10000004,
    StrafeRight = 0x10000008,
    TurnLeft = 0x10000010,
    TurnRight = 0x10000020,
    Jump = 0x10001000,
    Stand = 0x0,
    Swimming = 0x200000,
  };
}

Nully::NMovementFlag operator|(Nully::NMovementFlag a_flag1, Nully::NMovementFlag a_flag2);
