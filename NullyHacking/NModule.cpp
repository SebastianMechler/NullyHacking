#include "NModule.h"

namespace Nully
{
  void NModule::GetModuleList(NProcess& a_process, NVector<MODULEENTRY32>& a_result)
  {
    GetModuleList(a_process.GetProcessID(), a_result);
  }
  void NModule::GetModuleList(DWORD a_processID, NVector<MODULEENTRY32>& a_result)
  {
    HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
    MODULEENTRY32 moduleEntry;

    //  Take a snapshot of all modules in the specified process. 
    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, a_processID);
    if (hModuleSnap == INVALID_HANDLE_VALUE)
    {
      return;
    }

    //  Set the size of the structure before using it. 
    moduleEntry.dwSize = sizeof(MODULEENTRY32);

    //  Retrieve information about the first module, 
    //  and exit if unsuccessful 
    if (!Module32First(hModuleSnap, &moduleEntry))
    {
      CloseHandle(hModuleSnap);     // Must clean up the snapshot object! 
      return;
    }

    //  Now walk the module list of the process, 
    //  and display information about each module 
    do
    {
      a_result.push_back(moduleEntry);
    } while (Module32Next(hModuleSnap, &moduleEntry));


    CloseHandle(hModuleSnap);
    return;
  }

  MODULEENTRY32 NModule::FindModuleByName(const char* a_name, NVector<MODULEENTRY32>& a_modules)
  {
    for (auto& ele : a_modules)
    {
      if (strcmp(ele.szModule, a_name) == 0)
      {
        return ele;
      }
    }

    MODULEENTRY32 entry = {};
    return entry;
  }
}