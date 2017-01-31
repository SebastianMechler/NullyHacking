#include "NPlayer.h"

#include <string>

#include "NOffsets.h"
#include "NHackBase.h"
#include "NFunctions.h"


namespace Nully
{
  void NPlayer::Init(NObject* a_base, NClickToMove& a_ctm, NObjectManager* a_objectManager)
  {
    m_base = a_base;
    m_ctm = a_ctm;
    m_objectManager = a_objectManager;
  }

  void  NPlayer::SetPosition(const NVector3 & a_position)
  {
    m_base->position = a_position;
  }
  NVector3  NPlayer::GetPosition()
  {
    return m_base->position;
  }
  void  NPlayer::SetMovementFlag(NMovementFlag a_movementFlag)
  {
    m_base->movementFlag = a_movementFlag;
  }
  void  NPlayer::SetLastTarget(const NGuid & a_guid)
  {
    NGuid* guid = (NGuid*)((size_t)g_base + NPLAYER_LAST_TARGET);
    *guid = a_guid;
  }

  NMovementFlag  NPlayer::GetMovementFlag()
  {
    return m_base->movementFlag;
  }
  NLevel  NPlayer::GetLevel()
  {
    NLevel* level = (NLevel*)((size_t)g_base + NPLAYER_CURRENT_LEVEL);
    return *level;
  }
  NGuid  NPlayer::GetGuid()
  {
    return m_base->guid;
  }
  float  NPlayer::GetRotation()
  {
    return m_base->rotation;
  }
  void  NPlayer::SetRotation(float a_rotation)
  {
    m_base->rotation = a_rotation;
  }

  void  NPlayer::FaceTarget(NObject* a_target)
  {
    float rot = GetRotation();
    NVector3 rotVec;
    rotVec.x = cos(rot);
    rotVec.y = sinf(rot);
    
    

    NObject* target = m_objectManager->GetObjectByGuid(GetTargetGuid());
    if (target)
    {
      NVector3 lookAt = (target->position - this->GetPosition()).Normalized();
      
      NVector3 tmpLook = lookAt;
      tmpLook.z = 0.0f;
      if (NVector3::Dot(tmpLook, rotVec) <= 0.75f)
      {
        m_ctm.MoveTo(GetPosition() + lookAt);
      }

      //float angle = lookAt.GetAngleInRad();
      //SetRotation(angle);
    }   
  }

   NGuid NPlayer::GetTargetGuid()
  {
    NGuid* guid = (NGuid*)((size_t)g_base + NPLAYER_CURRENT_TARGET);
    return *guid;
  }
   NObject* NPlayer::GetTarget()
  {
    return m_objectManager->GetObjectByGuid(GetTargetGuid());
  }

  void  NPlayer::CastSpellByName(char * a_name)
  {
    char buffer[300] = {};
    sprintf(buffer, "CastSpellByName(\"%s\")", a_name);
    NLua_Execute(&buffer[0]);
  }
  void  NPlayer::TargetByGuid(NGuid a_guid)
  {
    this->SetLastTarget(a_guid);
    NLua_Execute("TargetLastTarget();");
  }
  void  NPlayer::TargetObject(NObject * a_object)
  {
    if (a_object)
    {
      TargetByGuid(a_object->guid);
    }
  }
  bool  NPlayer::HasTarget()
  {
    return GetTargetGuid() > 0;
  }
  float  NPlayer::GetTargetDistance(NObject * a_target)
  {
    return NVector3::Distance(a_target->position, this->GetPosition());
  }
  void  NPlayer::ClearTarget()
  {
    NLua_Execute("ClearTarget()");
  }
  void  NPlayer::StopMoving()
  {
    //m_ctm.SetType(NClickToMoveType::Stop);
    //SetMovementFlag(NMovementFlag::Stand);
    m_ctm.MoveTo(m_base->position);
  }
  uint32_t  NPlayer::GetHealth()
  {
    return m_base->GetHealth();
  }
  uint32_t  NPlayer::GetHealthMax()
  {
    return m_base->GetMaxHealth();
  }
  uint32_t  NPlayer::GetEnergy()
  {
    return m_base->GetEnergy();
  }
  uint32_t  NPlayer::GetEnergyMax()
  {
    return m_base->GetMaxEnergy();
  }
  float  NPlayer::GetPercentageHealth()
  {
    return static_cast<float>(GetHealth()) / GetHealthMax();
  }
  float  NPlayer::GetPercentageEnergy()
  {
    return static_cast<float>(GetEnergy()) / static_cast<float>(GetEnergyMax());
  }
  NClassType  NPlayer::GetClassType()
  {
    return *(NClassType*)((size_t)g_base + NPLAYER_CLASS);
  }
  NULLY_API bool NPlayer::IsCasting()
  {
    if (*(uint32_t*)((size_t)g_base + NPLAYER_CASTING_SPELL_ID) > 0)
    {
      return true;
    }
    return false;
  }
}
