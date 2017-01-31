#pragma once

namespace Nully
{
  template <typename T>
  T NProcess::Read(const void* a_address, NResult& a_result) const
  {
    if (this->m_processHandle == nullptr || a_address == nullptr)
    {
      a_result = NResult::Nullptr;
      return T();
    }

    SIZE_T numBytesRead = 0;

    T readValue = T();
    if (ReadProcessMemory(this->m_processHandle, a_address, &readValue, sizeof(readValue), &numBytesRead) == 0)
    {
      a_result = NResult::ProcessReadError;
      return T();
    }

    if (numBytesRead != sizeof(readValue))
    {
      a_result = NResult::ProcessReadErrorSize;
      return T();
    }

    return readValue;
  }

  template <typename T>
  NResult NProcess::Write(void* a_address, const T a_value) const
  {
    if (this->m_processHandle == nullptr || a_address == nullptr)
    {
      return NResult::Nullptr;
    }

    SIZE_T numBytesWritten = 0;

    if (WriteProcessMemory(this->m_processHandle, a_address, &a_value, sizeof(a_value), &numBytesWritten) == 0)
    {
      return NResult::ProcessWriteError;
    }

    if (numBytesWritten != sizeof(a_value))
    {
      return NResult::ProcessWriteErrorSize;
    }

    return NResult::Success;
  }
}
