#pragma once

#include <windows.h> 
#include <tlhelp32.h> 
#include <tchar.h> 

#include "NProcess.h"
#include "NVector.h"

namespace Nully
{
  class NModule
  {
  public:
    void GetModuleList(NProcess& a_process, NVector<MODULEENTRY32>& a_result);
    void GetModuleList(DWORD a_processID, NVector<MODULEENTRY32>& a_result);
    MODULEENTRY32 FindModuleByName(const char* a_name, NVector<MODULEENTRY32>& a_modules);
  };
}
