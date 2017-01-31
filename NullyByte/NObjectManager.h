#pragma once

#include <map>
#include <NVector.h>
#include <NVector3.h>

#include "NObject.h"
#include "NPathFileType.h"

namespace Nully
{
  typedef NVector<NObject*> NObjectList;
  typedef std::multimap<NObjectType, NObject*> NObjectList2;
  class NObjectManager
  {
  public:
    NObjectManager();

    void Update();
    NObject* GetPlayerBase() const;

    uint32_t GetListByType(NObjectType a_type, NObjectList& a_list);

    NObject* GetNearestObjectByType(NObjectType a_type, NVector3& a_position, NUnitIDs& a_unitIDs, float a_maxDistance = 100.0f);
    NObject* GetObjectByGuid(NGuid a_guid);

    NObjectList2 m_objects2;
    NObject* m_objects[2000];
  private:
    NObjectManager(const NObjectManager&);
    NObject* m_player = nullptr;
  };
}