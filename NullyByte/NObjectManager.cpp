#include "NObjectManager.h"

#include <NFile.h>

#include "NOffsets.h"
#include "NHackBase.h"
#include "NFunctions.h"



namespace Nully
{
  NObjectManager::NObjectManager()
  {
  }
  void NObjectManager::Update()
  {
    m_objects2.clear();

    for (auto& e : m_objects)
    {
      e = nullptr;
    }
    uint32_t* objectManagerBase2 = (uint32_t*)(g_base + NOBJECT_MANAGER);

    uint32_t* obj = (uint32_t*)(*objectManagerBase2 + NOBJECT_MANAGER_FIRST_OBJECT);
    NObject* object = (NObject*)(*obj);
    m_objects2.insert({object->type, object});

    NGuid* localPlayerGuid = (NGuid*)(g_base + NPLAYER_GLOBAL_GUID);

    uint32_t numObjects = 1;
    uint32_t currentIndex = 0;
    while (object != nullptr && (((size_t)object) & 0x00000001) != 0x1)
    {   
      //PrintHex("NextObject ==> ", (uint32_t)object->next);
      //NFile file;
      //file.Open("objects.txt", NFileMode::Append);

      if (m_player == nullptr && object->type == NObjectType::PLAYER)
      {
        if (object->guid == *localPlayerGuid)
        {
          // we found our player
          m_player = object;
        }
      }

      //m_objects[currentIndex] = object;



      // Dump
      //char szBuffer[50] = {};
      //sprintf(szBuffer, "ID=%.04d --- %.08X\n", currentIndex, object);
      //file.Write(szBuffer, strlen(szBuffer));
      //file.~NFile();

      if ((uint32_t)object > 0x3E227008 || object->next > 0x3E227008 || ((size_t)object) & 0x00000001 == 0x1)
      {
        MessageBox(0, "FOUND CRASH OBJECT", "", MB_OK);
        return;
      }

      m_objects2.insert(std::pair<NObjectType, NObject*>(object->type, object));
      object = (NObject*)object->next;
      numObjects++;
      currentIndex++;
    }
  }
  NObject* NObjectManager::GetPlayerBase() const
  {
    return m_player;
  }
  uint32_t NObjectManager::GetListByType(NObjectType a_type, NObjectList& a_list)
  {
    auto ret = m_objects2.equal_range(a_type);

    uint32_t numObjects = 0;
    for (auto it = ret.first; it != ret.second; it++)
    {
      a_list.push_back(it->second);
      numObjects++;
    }

    return numObjects;
  }
  NObject* NObjectManager::GetNearestObjectByType(NObjectType a_type, NVector3& a_position, NUnitIDs& a_unitIDs, float maxDistance)
  {
    NObjectList list;
    uint32_t numObjects = this->GetListByType(a_type, list);

    if (numObjects == 0)
    {
      return nullptr;
    }

    float nearestObjectDistance = 5000.0f;
    uint32_t nearestObjectIndex = 0;
    bool isSet = false;
    for (uint32_t i = 0; i < numObjects; i++)
    {
      // calculate distance from a_position to list[i].position;
      float distance = (a_position - list[i]->position).Length();

      bool isUnitIdAllowed = a_unitIDs.find(list[i]->GetUnitID()) != a_unitIDs.end();

      if (isUnitIdAllowed && list[i]->GetHealth() != 0 && distance <= maxDistance && distance <= nearestObjectDistance) // maybe add float.epsilon for this (more precisely, but for now skip it
      {
        nearestObjectIndex = i;
        nearestObjectDistance = distance;
        isSet = true;
      }
    }

    if (isSet)
    {
      return list[nearestObjectIndex];
    }
    else
    {
      return nullptr;
    }
  }
  NObject* NObjectManager::GetObjectByGuid(NGuid a_guid)
  {
    for (auto& e : m_objects2)
    {
      if (e.second->guid == a_guid)
      {
        return e.second;
      }
    }

    return nullptr;
  }
}
