#pragma once

#include <Windows.h>

#include "NResult.h"
#include "NTypes.h"

namespace Nully
{
  class NRemoteThread
  {
  public:
    NRemoteThread();
    NRemoteThread(const HANDLE a_process, LPTHREAD_START_ROUTINE a_function, LPVOID a_parameter);

    NResult Create(const HANDLE a_process, LPTHREAD_START_ROUTINE a_function, LPVOID a_parameter);

    void WaitUntilFinished();

    DWORD GetExitCode();

    ~NRemoteThread();

  private:
    DWORD m_exitCode;
    HANDLE m_threadHandle;
  };
}
