#pragma once

#include "NClickToMove.h"
#include "NObjectManager.h"
#include "NPlayer.h"
#include "NPathFileType.h"

#define NPATH_POSITION "pos"
#define NPATH_UNIT_ID "uid"
#define NPATH_PREFIX_LENGTH 3
#define NPATH_PREFIX_LENGTH_WITH_SPACE NPATH_PREFIX_LENGTH + 1

namespace Nully
{
  class NPathFile
  {
  public:
    NPathFile();
    bool Load(const char* a_file);
    bool Reload(const char* a_file);
    NPaths& GetPaths();
    NUnitIDs& GetUnitIDs();

    void StartRecord();
    void UpdateRecord(NClickToMove& a_clickToMove, NPlayer& a_player, NObjectManager& a_objectManager);
    void StopRecord();
    bool IsRecording() const;

    bool SaveRecording(const char* a_file);

  private:
    void ToPosition(char* a_data);
    void ToUnitID(char* a_data);
    NPathFileType GetType(char* data);

    NPaths m_paths; // LOADING
    NUnitIDs m_unitIDs; // loading;

    NPaths m_record; // RECORDING
    NUnitIDs m_recordUnitID;

    bool m_isRecording;
    NVector3 m_lastRecordedWaypoint;
  };
}