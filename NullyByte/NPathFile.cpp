#include "NPathFile.h"

#include <string>

#include <NFile.h>
#include "NFunctions.h"

namespace Nully
{
  NPathFile::NPathFile()
    : m_isRecording(false)
  {
  }
  bool NPathFile::Load(const char * a_file)
  {
    if (a_file == nullptr)
    {
      return false;
    }

    NFile file;
    if (file.Open(a_file, NFileMode::Read) != NResult::Success)
    {
      return false;
    }

    char buffer[1024] = {};
    NPathFileType type = NPathFileType::None;

    uint32_t numPositions = 0;
    uint32_t numUnitIDs = 0;

    while (file.ReadLine(buffer, sizeof(buffer)) == NResult::FileReadLine)
    {
      type = GetType(buffer);

      switch (type)
      {
      case Nully::NPathFileType::Position:
        ToPosition(buffer + NPATH_PREFIX_LENGTH_WITH_SPACE);
        numPositions++;
        break;
      case Nully::NPathFileType::UnitID:
        ToUnitID(buffer + NPATH_PREFIX_LENGTH_WITH_SPACE);
        numUnitIDs++;
        break;
      case Nully::NPathFileType::None:
        break;
      default:
        break;
      }
    }

    NLua_Print("Number of Postions loaded: ", numPositions, false);
    NLua_Print("Number of UnitIDs loaded: ", numUnitIDs, false);

    return true;
  }

  bool NPathFile::Reload(const char * a_file)
  {
    m_paths.clear();
    m_unitIDs.clear();

    return Load(a_file);
  }

  NPaths& NPathFile::GetPaths()
  {
    return m_paths;
  }

  NUnitIDs & NPathFile::GetUnitIDs()
  {
    return m_unitIDs;
  }

  void NPathFile::StartRecord()
  {
    m_isRecording = true;
    m_record.clear();
    m_recordUnitID.clear();
  }

  void NPathFile::UpdateRecord(NClickToMove& a_clickToMove, NPlayer& a_player, NObjectManager& a_objectManager)
  {
    auto type = a_clickToMove.GetType();
    if (type != NClickToMoveType::Stop)
    {
      NVector3 currentPosition = a_clickToMove.GetMoveTo();
      if (m_lastRecordedWaypoint != currentPosition)
      {
        m_record.push_back(currentPosition);
        m_lastRecordedWaypoint = currentPosition;
        NLua_Print("Added waypoint: ", currentPosition);

        if (a_player.HasTarget())
        {
          NObject* target = a_objectManager.GetObjectByGuid(a_player.GetTargetGuid());
          uint32_t unitID = target->GetUnitID();

          if (target)
          {
            m_recordUnitID[unitID] = unitID;
            NLua_Print("Added UnitID: ", unitID, false);
          }
        }
      }
      else
      {
        //std::cout << "WAYPOINT ALREADY EXIST LOL" << std::endl;
      }
    }
  }

  void NPathFile::StopRecord()
  {
    m_isRecording = false;
    m_lastRecordedWaypoint = NVector3::zero;
  }
  bool NPathFile::IsRecording() const
  {
    return m_isRecording;
  }
  bool NPathFile::SaveRecording(const char * a_file)
  {
    if (a_file == nullptr)
    {
      return false;
    }

    if (m_record.size() == 0)
    {
      return false;
    }

    NFile file;
    if (file.Open(a_file, NFileMode::Write) != NResult::Success)
    {
      return false;
    }

    char buffer[1024] = {};

    char prefix[4] = {};

    // save all positions
    strcpy(prefix, NPATH_POSITION);
    for (auto& ele : m_record)
    {
      sprintf(buffer, "%s x=%.08f y=%.08f z=%.08f\r\n", prefix,  ele.x, ele.y, ele.z);
      file.Write(buffer, strlen(buffer));
    }

    // save all unit-ids
    strcpy(prefix, NPATH_UNIT_ID);
    for (auto& ele : m_recordUnitID)
    {
      sprintf(buffer, "%s %u\r\n", prefix, ele.second);
      file.Write(buffer, strlen(buffer));
    }

    return true;
  }
  void NPathFile::ToPosition(char * a_data)
  {
    NVector3 data;
    sscanf_s(a_data, "x=%f y=%f z=%f", &data.x, &data.y, &data.z);
    m_paths.push_back(data);
  }

  void NPathFile::ToUnitID(char * a_data)
  {
    uint32_t unitID = 0;
    sscanf_s(a_data, "%u", &unitID);
    m_unitIDs[unitID] = unitID;
  }

  NPathFileType NPathFile::GetType(char * data)
  {
    if (data == nullptr)
    {
      return NPathFileType::None;
    }

    if (std::strncmp(data, NPATH_POSITION, NPATH_PREFIX_LENGTH) == 0)
    {
      return NPathFileType::Position;
    }
    else if (std::strncmp(data, NPATH_UNIT_ID, NPATH_PREFIX_LENGTH) == 0)
    {
      return NPathFileType::UnitID;
    }
    

    return NPathFileType::None;
  }
}
