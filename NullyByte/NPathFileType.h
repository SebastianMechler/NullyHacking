#pragma once
#include "NMap.h"
#include "NVector.h"
#include "NVector3.h"

namespace Nully
{
  typedef NVector<NVector3> NPaths;
  typedef NMap<uint32_t, uint32_t> NUnitIDs;

  enum class NPathFileType
  {
    Position,
    UnitID,
    None,
  };
}
