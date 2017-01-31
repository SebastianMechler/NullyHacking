#include "NRemoteThread.h"

namespace Nully
{
  NRemoteThread::NRemoteThread()
    : m_threadHandle(nullptr)
    , m_exitCode(0)
  {
  }

  NRemoteThread::NRemoteThread(const HANDLE a_process, LPTHREAD_START_ROUTINE a_function, LPVOID a_parameter)
    : m_threadHandle(nullptr)
    , m_exitCode(0)
  {
    Create(a_process, a_function, a_parameter);
  }

  NResult NRemoteThread::Create(const HANDLE a_process, LPTHREAD_START_ROUTINE a_function, LPVOID a_parameter)
  {
    if (a_process == nullptr || a_function == nullptr || a_parameter == nullptr)
    {
      return NResult::Nullptr;
    }

    if (m_threadHandle)
    {
      return NResult::RemoteThreadAlreadyExists;
    }

    m_threadHandle = CreateRemoteThread(a_process, 0, 0, a_function, a_parameter, 0, nullptr);

    if (m_threadHandle == nullptr)
    {
      return NResult::RemoteThreadCreationFailed;
    }

    return NResult::Success;
  }

  void NRemoteThread::WaitUntilFinished()
  {
    if (m_threadHandle)
    {
      WaitForSingleObject(m_threadHandle, INFINITE);
    }
  }

  DWORD NRemoteThread::GetExitCode()
  {
    GetExitCodeThread(m_threadHandle, &m_exitCode);
    return m_exitCode;
  }

  NRemoteThread::~NRemoteThread()
  {
    if (m_threadHandle)
    {
      CloseHandle(m_threadHandle);
    }
  }
}
