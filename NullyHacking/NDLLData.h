#pragma once

#include "NTypes.h"

typedef HMODULE(__stdcall *NLoadDLL)(LPCSTR);
typedef BOOL(__stdcall *NFreeDLL)(HMODULE);

namespace Nully
{
  struct NDLLLoadData
  {
    NLoadDLL loadLibraryAddress;
    char dllPath[256];
  };

  struct NDLLFreeData
  {
    NFreeDLL freeLibraryAddress;
    HMODULE module;
  };
}
