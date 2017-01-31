#pragma once

#include "NProcess.h"
#include "NFile.h"
#include "NDLLData.h"

static void __stdcall __NullyLoadDLL(void* a_data);
static void __stdcall __NullyFreeDLL(void* a_data);

#define NULLY_KERNEL_PATH "C:\\windows\\system32\\kernel32.dll"
#define NULLY_KERNEL_LOAD_LIBRARY_A "LoadLibraryA"
#define NULLY_KERNEL_FREE_LIBRARY "FreeLibrary"

namespace Nully
{
  // TODO: enumerate modules first, to check if already attached
  
  class NDLLInjector
  {
  public:
    NDLLInjector();
    bool Inject(NProcess& a_process, const char* a_dllPath);
    bool Eject(NProcess& a_process, void* a_module);

  private:
    HMODULE m_loadedModule;
  };
}