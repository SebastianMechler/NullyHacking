#include "NClickToMove.h"
#include "NOffsets.h"


namespace Nully
{
  NClickToMoveType NClickToMove::GetType()
  {
    NClickToMoveType* type = (NClickToMoveType*)(g_base + NCTM_ACTION);
    return *type;
  }

  void NClickToMove::SetType(const NClickToMoveType a_type)
  {
    NClickToMoveType* type = (NClickToMoveType*)(g_base + NCTM_ACTION);
    *type = a_type;
  }

  void NClickToMove::MoveTo(const NVector3 & a_position, const NClickToMoveType a_type)
  {
    NVector3* pos = (NVector3*)(g_base + NCTM_X);
    *pos = a_position;

    this->SetType(a_type);
  }

  //void NClickToMove::MoveToByTeleport(NPlayer & a_player)
  //{
  //  this->SetType(NClickToMoveType::Stop);
  //  a_player.SetPosition(GetMoveTo());
  //  a_player.SetMovementFlag(NMovementFlag::Stand);
  //}

  NVector3 NClickToMove::GetMoveTo()
  {
    NVector3* pos = (NVector3*)(g_base + NCTM_X);
    return *pos;
  }
}

