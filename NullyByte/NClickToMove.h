#pragma once

#include "NClickToMoveType.h"
#include "NResult.h"
#include "NVector3.h"
#include "NHackBase.h"
#include "NFunctions.h"

namespace Nully
{
  class NClickToMove
  {
  public:
    NClickToMoveType GetType();
    void SetType(const NClickToMoveType a_type);
    void MoveTo(const NVector3& a_position, const NClickToMoveType a_type = NClickToMoveType::Ground);
    NVector3 GetMoveTo();

  private:
    uint32_t m_wowBase;
  };
}