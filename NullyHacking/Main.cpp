#include "NProcess.h"
#include "NVector3.h"
#include "NDLLInjector.h"
#include "NModule.h"

using namespace Nully;

void main(int argc, char *argv[])
{
  NProcess process;
  auto wowList = process.GetProcessListByName("WoW.exe");
  if (wowList.size() == 0)
  {
    MessageBox(0, "Could not find any wow-process.", "", MB_OK);
    return;
  }

  NProcess::SetDebugPrivilegeState(true);
  auto processResult = process.Open(wowList[0]);
  if (processResult != NResult::Success)
  {
    MessageBox(0, "Failed to open WoW.exe.", "", MB_OK);
    return;
  }

  char exePath[MAX_PATH] = {};
  char dllPath[MAX_PATH] = {};
  // Will contain exe path
  HMODULE hModule = GetModuleHandle(NULL);
  if (hModule != NULL)
  {
    // When passing NULL to GetModuleHandle, it returns handle of exe itself
    GetModuleFileName(hModule, exePath, (sizeof(exePath)));
  }

  size_t len = strlen(exePath);
  if (len == 0)
  {
    MessageBox(0, "Sorry but exePath is wrong.", "", MB_OK);
    return;
  }

  char* p = exePath + strlen(exePath);
  uint32_t sub = 0;
  while (*p != '\\')
  {
    p--;
    sub++;
  }

  std::strncpy(dllPath, exePath, strlen(exePath) - sub + 1);
  strcat(dllPath, argv[1]);
  std::cout << "Generated DLL-Path: " << dllPath << std::endl;

  NDLLInjector injector;
  bool isDone = injector.Inject(process, dllPath);//dllPath);//"G:\\02-C++\\02-2016\\NullyHacking\\build\\NullyByte\\Release\\NullyByte.dll");

  if (isDone)
  {
    std::cout << "Successfully injected the dll" << std::endl;
  }
  else
  {
    std::cout << "Failed to inject the dll" << std::endl;
  }



  NModule module;
  NVector<MODULEENTRY32> m_result;
  module.GetModuleList(process, m_result);
  auto moduleFound = module.FindModuleByName("NullyByte.dll", m_result);

  system("pause");
  //HMODULE mod = LoadLibraryA("G:\\02-C++\\02-2016\\SampleDll\\Release\\SampleDll.dll");
  //FreeLibrary(mod);
  
  //injector.Eject(process, moduleFound.hModule);
  
}


//NDLLInjector injector;
//bool isDone = injector.Inject(process, "G:\\02-C++\\02-2016\\SampleDll\\Release\\SampleDll.dll");

//NModule module;
//NVector<MODULEENTRY32> m_result;
//module.GetModuleList(process, m_result);
//auto moduleFound = module.FindModuleByName("SampleDll.dll", m_result);
//
////HMODULE mod = LoadLibraryA("G:\\02-C++\\02-2016\\SampleDll\\Release\\SampleDll.dll");
////FreeLibrary(mod);
//
//injector.Eject(process, moduleFound.hModule);

/*
["gmIsland"] = {x = 16227.799805, y = 16403.400391, z = -64.380402},
["stormwind"] = {x = -8846.41, y = 648.23, z = 96.60},
["orgrimmar"] = {x = 1651.24, y = -4410.22, z = 17.00},
["brill"] =  {x = 2413.24, y = -60.22, z = 55.00},
*/