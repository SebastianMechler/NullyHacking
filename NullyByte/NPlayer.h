#pragma once

#include <NVector3.h>

#include "NMovementFlag.h"
#include "NObject.h"
#include "NClassType.h"
#include "NClickToMove.h"
#include "NObjectManager.h"
#include "NAPI.h"

namespace Nully
{
  typedef unsigned char NLevel;

  NULLY_API class NPlayer
  {
  public:
    void Init(NObject* a_base, NClickToMove& a_ctm, NObjectManager* a_objectManager);

    NULLY_API void SetPosition(const NVector3& a_position);
    NULLY_API NVector3 GetPosition();
    NULLY_API void SetMovementFlag(NMovementFlag a_movementFlag);
    NULLY_API NMovementFlag GetMovementFlag();

    NULLY_API NLevel GetLevel();
    NULLY_API NGuid GetGuid();
    NULLY_API float GetRotation();
    NULLY_API void SetRotation(float a_rotation);

    // target
    NULLY_API void FaceTarget(NObject* a_target);
    NULLY_API NGuid GetTargetGuid();
    
    NULLY_API NObject* GetTarget();

    NULLY_API void TargetByGuid(NGuid a_guid);
    NULLY_API void TargetObject(NObject* a_object);
    NULLY_API bool HasTarget();
    NULLY_API float GetTargetDistance(NObject* a_target);
    NULLY_API void ClearTarget();

    NULLY_API void CastSpellByName(char* a_name);

    NULLY_API void StopMoving();

    // returns the players current health
    NULLY_API uint32_t GetHealth();

    // returns the players max health
    NULLY_API uint32_t GetHealthMax();

    // returns the players current energy
    NULLY_API uint32_t GetEnergy();

    // returns the player max energy
    NULLY_API uint32_t GetEnergyMax();

    // returns the current health in percent (ranges from 0.0 to 1.0)
    NULLY_API float GetPercentageHealth();

    // returns the current energy in percent (ranges from 0.0 to 1.0)
    NULLY_API float GetPercentageEnergy();

    NULLY_API NClassType GetClassType();

    NULLY_API bool IsCasting();

  private:
    NULLY_API void SetLastTarget(const NGuid& a_guid);

    NObject* m_base = nullptr;
    NClickToMove m_ctm;
    NObjectManager* m_objectManager;
  };
}