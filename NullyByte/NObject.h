#pragma once

#include <NVector3.h>
#include <NTypes.h>

#include "NObjectType.h"
#include "NMovementFlag.h"

namespace Nully
{
  typedef unsigned long long NGuid;
  // 0x8 = ptr to healthstructure
#pragma pack(push, 1)
  struct NObjectData1
  {
    // 0x58 = currentHealth
    char _unused[88];
    uint32_t health; // 0x58
    uint32_t energy; // 0x5C
    uint32_t _unused2[4];
    uint32_t maxHealth; // 0x70
    uint32_t maxEnergy; // 0x74
    uint32_t _unused3[4];
    uint32_t currentLevel; // 0x88 NOT TESTED
    uint32_t unitID; // 0x8C
  };

  struct NObject
  {
    char _unused[8]; // 0x0
    NObjectData1* data1; // 0x8
    char _unused8[8];
    NObjectType type; // 0x14
    uint32_t _unused4[6]; // 0x18
    NGuid guid; // 0x30
    uint32_t _unused3[1]; // 0x34 - 0x3B
    size_t next; // 0x3C
    uint32_t _unused2[606];
    NVector3 position; // 0x9B8
    float rotation; // 0x9C4
    uint32_t _unused5[8];
    NMovementFlag movementFlag; // 0x9E8

    //uint32_t unused6[1271];
    //uint32_t health; // 0x1DC8
    //uint32_t energy; // 0x1DCC
    //uint32_t unused7[5];
    //uint32_t healthMax; // 1DE0
    //uint32_t energyMax; // 1DE4

    uint32_t GetHealth()
    {
      return data1->health;
    }

    uint32_t GetEnergy()
    {
      return data1->energy;
    }

    uint32_t GetMaxHealth()
    {
      return data1->maxHealth;
    }

    uint32_t GetMaxEnergy()
    {
      return data1->maxEnergy;
    }

    uint32_t GetUnitID()
    {
      return data1->unitID;
    }

    uint32_t GetLevel()
    {
      return data1->currentLevel;
    }
  };


#pragma pack(pop)
}

/*
    char _unused[20]; // 0x0
    NObjectType* type; // 0x14
    uint32_t _unused4[6]; // 0x18
    NGuid* guid; // 0x30
    uint32_t _unused3[1]; // 0x34 - 0x3B
    NObject* next; // 0x3C
    uint32_t _unused2[606];
    NVector3* position; // 0x9B8
    float* rotation;
    uint32_t _unused5[8];
    NMovementFlag* movementFlag; // 0x9E8
*/

//struct NObject
//{
//  char _unused[20]; // 0x0
//  NObjectType type; // 0x14
//  uint32_t _unused4[6]; // 0x18
//  NGuid guid; // 0x30
//  uint32_t _unused3[1]; // 0x34 - 0x3B
//  size_t next; // 0x3C
//  uint32_t _unused2[606];
//  NVector3 position; // 0x9B8
//  float rotation;
//  uint32_t _unused5[8];
//  NMovementFlag movementFlag; // 0x9E8
//  uint32_t unused6[1271];
//  uint32_t health; // 0x1DC8
//  uint32_t energy; // 0x1DCC
//  uint32_t unused7[5];
//  uint32_t healthMax; // 1DE0
//  uint32_t energyMax; // 1DE4
//};