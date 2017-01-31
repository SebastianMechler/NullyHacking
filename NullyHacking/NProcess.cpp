#include "NProcess.h"
#include "NFile.h"

namespace Nully
{
  NProcess::NProcess()
    : m_processHandle(nullptr), m_processID(0)
  {
  }

  NProcess::NProcess(const uint32_t a_processID)
    : m_processHandle(nullptr), m_processID(0)
  {
    Open(a_processID);
  }
  NProcess::~NProcess()
  {
    m_processHandle = nullptr;
    m_processID = 0;
    Close();
  }
  NResult NProcess::Open(const uint32_t a_processID)
  {
    HANDLE process = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION, FALSE, a_processID);
    if (process == nullptr)
    {
      return NResult::ProcessOpenError;
    }

    if (this->m_processHandle != nullptr)
    {
      Close();
    }
    else
    {
      this->m_processHandle = process;
      this->m_processID = a_processID;
    }

    return NResult::Success;
  }
  NResult NProcess::Open(const NProcessData & a_processData)
  {
    return Open(a_processData.id);
  }
  const HANDLE NProcess::GetHandle() const
  {
    return m_processHandle;
  }
  void NProcess::Close()
  {
    for (auto &ele : m_virtualAllocations)
    {
      if (FAILED(VirtualFreeEx(m_processHandle, ele.first, 0, static_cast<DWORD>(NDeallocationType::Release))))
      {
        __debugbreak();
      }
    }

    if (this->m_processHandle != nullptr)
    {
      CloseHandle(this->m_processHandle);
      this->m_processHandle = nullptr;
    }
  }
  NResult NProcess::ReadText(const void * a_address, char* a_buffer, const size_t a_bufferSize) const
  {
    if (this->m_processHandle == nullptr || a_buffer == nullptr)
    {
      return NResult::Nullptr;
    }

    // in order to increment a_address bytewise we have to remove the constness and cast it to a byte* pointer
    void* nonconstAddress = const_cast<void*>(a_address);
    char* incrementableAddress = reinterpret_cast<char*>(nonconstAddress);

    uint32_t index = 0;
    for (uint32_t i = 0; i < a_bufferSize; i++)
    {
      if (ReadProcessMemory(this->m_processHandle, incrementableAddress + i, a_buffer + i, 1, nullptr) == 0)
      {
        return NResult::ProcessReadError;
      }

      if (a_buffer[i] == '\0')
      {
        break;
      }

      index = i;
    }

    a_buffer[index] = '\0';

    return NResult::Success;
  }
  NResult NProcess::WriteText(void * a_address, const char* a_buffer, const size_t a_bufferLength) const
  {
    size_t size = a_bufferLength;

    if (size == 0)
    {
      // get text length
      size = strlen(a_buffer) + 1;
    }

    return WriteBytes(a_address, a_buffer, size);
  }
  NResult NProcess::WriteBytes(void * a_address, const void * a_buffer, const size_t a_bufferSize) const
  {
    if (this->m_processHandle == nullptr || a_buffer == nullptr)
    {
      return NResult::Nullptr;
    }

    SIZE_T numBytesWritten = 0;

    if (WriteProcessMemory(this->m_processHandle, a_address, a_buffer, a_bufferSize, &numBytesWritten) == 0)
    {
      return NResult::ProcessWriteError;
    }

    if (numBytesWritten != a_bufferSize)
    {
      return NResult::ProcessWriteErrorSize;
    }

    return NResult::Success;
  }
  NResult NProcess::VirtualAllocate(const size_t a_size, const NAllocationType a_allocation, const NProtectionType a_protection, void*& a_allocatedBaseAddress, void* a_desiredAddress)
  {
    if (this->m_processHandle == nullptr)
    {
      return NResult::Nullptr;
    }

    void* resultAddress = VirtualAllocEx(m_processHandle, a_desiredAddress, a_size, static_cast<DWORD>(a_allocation), static_cast<DWORD>(a_protection));
    if (resultAddress == nullptr)
    {
      return NResult::ProcessAllocateError;
    }
    else
    {
      m_virtualAllocations[resultAddress] = a_size;
      a_allocatedBaseAddress = resultAddress;
      return NResult::Success;
    }
  }
  NResult NProcess::VirtualProtect(void * a_address, const size_t a_size, const NProtectionType a_newProtection, NProtectionType *& a_oldProtection)
  {
    if (this->m_processHandle == nullptr || a_address == nullptr)
    {
      return NResult::Nullptr;
    }

    if (VirtualProtectEx(this->m_processHandle, a_address, a_size, static_cast<DWORD>(a_newProtection), reinterpret_cast<DWORD*>(a_oldProtection)) != 0)
    {
      return NResult::ProcessProtectError;
    }

    return NResult::Success;
  }
  void* NProcess::GetBaseAddress()
  {
    if (this->m_processHandle == nullptr)
    {
      return nullptr;
    }

    // Get the base address of the module
    HMODULE module = nullptr;
    DWORD bytesNeeded = 0;

    EnumProcessModules(this->m_processHandle, &module, sizeof(module), &bytesNeeded);

    //CloseHandle(process);
    return (module);
  }

  uint32_t NProcess::GetProcessID() const
  {
    return m_processID;
  }

  NResult NProcess::DumpToFile(const NVector<NProcessData>& a_processData, const char* a_filePath)
  {
    if (a_filePath == nullptr)
    {
      return NResult::Nullptr;
    }

    NFile file;

    file.Open(a_filePath, NFileMode::Write);

    for (auto it = a_processData.begin(); it != a_processData.end(); it++)
    {
      auto length = it->name.length();
      if (length == 0)
      {
        continue;
      }
      char buffer[2000];

      sprintf_s(buffer, sizeof(buffer), "PID: [%5i] => %s", it->id, it->name.c_str());

      file.Write(buffer, strlen(buffer));
      file.Write("\r\n", 2);
    }

    return NResult::Success;
  }
  NVector<NProcessModule> NProcess::GetModuleList()
  {
    NVector<NProcessModule> processModules;

    if (m_processHandle == nullptr)
    {
      return processModules;
    }

    HMODULE modules[250] = { 0 };
    DWORD read = 0;
    EnumProcessModules(m_processHandle, modules, sizeof(modules), &read);

    if (read == 0)
    {
      return processModules;
    }

    TCHAR buffer[MAX_PATH] = { 0 };

    uint32_t totalModules = read / sizeof(HMODULE);
    for (uint32_t i = 0; i < totalModules; i++)
    {
      // get name of module
      GetModuleFileName(modules[i], buffer, sizeof(buffer) / sizeof(TCHAR));

      // insert new element to result
      processModules.push_back({ modules[i], buffer });
    }

    return processModules;
  }
  bool NProcess::SetDebugPrivilegeState(const bool a_state)
  {
    HANDLE token = NULL; // handle to token
    LUID luid; // local unique identifier

               // retrieves the token from the current process which can be adjusted
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token))
    {
      return false;
    }

    // retrieves the locally unique identifier which is represented by the privilege name
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
    {
      return false;
    }

    // token adjustment
    TOKEN_PRIVILEGES tokenPriv;
    tokenPriv.PrivilegeCount = 1;
    tokenPriv.Privileges[0].Luid = luid;
    tokenPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // store new token in process
    if (!AdjustTokenPrivileges(token, FALSE, &tokenPriv, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
    {
      return false;
    }

    // sucess
    return true;
  }

  NVector<NProcessData> NProcess::GetProcessListByName(const char* a_processName, const bool a_getFullProcessPathName)
  {
    // get all process ids and store them in processes
    DWORD processes[NProcess::m_maxProcesses], sizeNeeded;
    EnumProcesses(&processes[0], sizeof(processes), &sizeNeeded);

    // calculate number of processes
    uint32_t numberOfProcesses = sizeNeeded / sizeof(DWORD);
    std::vector<NProcessData> processList;

    for (uint32_t i = 0; i < numberOfProcesses; i++)
    {
      HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processes[i]);

      char processName[MAX_PATH] = { 0 };
      GetProcessImageFileNameA(process, &processName[0], MAX_PATH);

      NString extractedProcessName = processName;
      int32_t index = extractedProcessName.find_last_of('\\');
      extractedProcessName = std::string(processName + index + 1);

      // Release the handle to the process.
      CloseHandle(process);

      if (a_processName == nullptr || strcmp(extractedProcessName.c_str(), a_processName) == 0)
      {
        // nullptr passed as processName so take all processes
        NProcessData data;
        data.id = processes[i];
        if (a_getFullProcessPathName)
        {
          data.name = processName;
        }
        else
        {
          data.name = extractedProcessName;
        }

        processList.push_back(data);
      }
    }
    return processList;
  }
}



/*
// get all names
wchar_c processName[MAX_PATH] = L"unknown";
char processName1[MAX_PATH] = "";
// Get a handle to the process.
HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);

// Get the process name.
if (process != NULL)
{
HMODULE module;
DWORD bytesNeeded;

if (EnumProcessModules(process, &module, sizeof(module), &bytesNeeded))
{
GetModuleBaseNameA(process, module, processName1, sizeof(processName1) / sizeof(char));
}
}
if (a_processName == nullptr || wcscmp(processName, a_processName) == 0)
{
// nullptr passed as processName so take all processes
SProcessData data;
data.id = processes[i];
data.name = processName1;
processList.push_back(data);
}

// Release the handle to the process.
CloseHandle(process);
*/