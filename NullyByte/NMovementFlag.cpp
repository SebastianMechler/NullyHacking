#include "NMovementFlag.h"

Nully::NMovementFlag operator|(Nully::NMovementFlag a_flag1, Nully::NMovementFlag a_flag2)
{
  return static_cast<Nully::NMovementFlag>(static_cast<uint32_t>(a_flag1) | static_cast<uint32_t>(a_flag2));
}