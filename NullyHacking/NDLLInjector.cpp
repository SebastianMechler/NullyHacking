#include "NDLLInjector.h"

namespace Nully
{
  NDLLInjector::NDLLInjector()
    : m_loadedModule(nullptr)
  {
  }
  bool NDLLInjector::Inject(NProcess& a_process, const char * a_dllPath)
  {
    if (a_dllPath == nullptr || a_process.GetHandle() == nullptr)
    {
      return false;
    }

    NProcess::SetDebugPrivilegeState(true);

    // get size of DLL
    uint64_t dllSize = NFile::GetFileSize(a_dllPath);
    if (dllSize == 0)
    {
      // file does not exist, or is empty
      return false;
    }

    // prepare data which will be passed to the remote-function
    // this data contains the addresses of LoadLibraryA and FreeLibrary which is always mapped in the same user-space inside the executable
    NDLLLoadData data = {};
    strcpy_s(data.dllPath, sizeof(data.dllPath), a_dllPath);
    data.loadLibraryAddress = (NLoadDLL)GetProcAddress(GetModuleHandleA(NULLY_KERNEL_PATH), NULLY_KERNEL_LOAD_LIBRARY_A);
    
    if (data.loadLibraryAddress == nullptr)
    {
      return false;
    }

    // allocate space for data
    void* allocatedDataAddress = nullptr;
    if (a_process.VirtualAllocate(sizeof(NDLLLoadData), NAllocationType::Commit, NProtectionType::ExecuteReadWrite, allocatedDataAddress) != NResult::Success)
    {
      return false;
    }

    // write data to process
    if (a_process.WriteBytes(allocatedDataAddress, &data, sizeof(NDLLLoadData)) != NResult::Success)
    {
      return false;
    }

    // allocate space for remote-function
    void* allocatedFunctionAddress = nullptr;
    size_t functionSize = 400; // you can't determine the real function byte-length, it's enough to just write more bytes than needed
    if (a_process.VirtualAllocate(functionSize, NAllocationType::Commit, NProtectionType::ExecuteReadWrite, allocatedFunctionAddress) != NResult::Success)
    {
      return false;
    }

    // write remote-function to process
    if (a_process.WriteBytes(allocatedFunctionAddress, (void*)__NullyLoadDLL, functionSize) != NResult::Success)
    {
      return false;
    }

    // create thread and pass data to it
    NRemoteThread thread;
    if (thread.Create(a_process.GetHandle(), LPTHREAD_START_ROUTINE(allocatedFunctionAddress), allocatedDataAddress) != NResult::Success)
    {
      return false;
    }

    // wait until thread finishes
    thread.WaitUntilFinished();
    return true;
  }

  bool NDLLInjector::Eject(NProcess & a_process, void * a_module)
  {
    if (a_process.GetHandle() == nullptr || a_module == nullptr)
    {
      return false;
    }

    NProcess::SetDebugPrivilegeState(true);

    // prepare data which will be passed to the remote-function
    // this data contains the addresses of LoadLibraryA and FreeLibrary which is always mapped in the same user-space inside the executable
    NDLLFreeData data = {};
    data.freeLibraryAddress = (NFreeDLL)GetProcAddress(GetModuleHandleA(NULLY_KERNEL_PATH), NULLY_KERNEL_FREE_LIBRARY);
    data.module = (HMODULE)a_module;

    if (data.freeLibraryAddress == nullptr)
    {
      return false;
    }

    // allocate space for data
    void* allocatedDataAddress = nullptr;
    if (a_process.VirtualAllocate(sizeof(NDLLFreeData), NAllocationType::Commit, NProtectionType::ExecuteReadWrite, allocatedDataAddress) != NResult::Success)
    {
      return false;
    }

    // write data to process
    if (a_process.WriteBytes(allocatedDataAddress, &data, sizeof(NDLLFreeData)) != NResult::Success)
    {
      return false;
    }

    // allocate space for remote-function
    void* allocatedFunctionAddress = nullptr;
    size_t functionSize = 400; // you can't determine the real function byte-length, it's enough to just write more bytes than needed
    if (a_process.VirtualAllocate(functionSize, NAllocationType::Commit, NProtectionType::ExecuteReadWrite, allocatedFunctionAddress) != NResult::Success)
    {
      return false;
    }

    // write remote-function to process
    if (a_process.WriteBytes(allocatedFunctionAddress, (void*)__NullyFreeDLL, functionSize) != NResult::Success)
    {
      return false;
    }

    // create thread and pass data to it
    NRemoteThread thread;
    if (thread.Create(a_process.GetHandle(), LPTHREAD_START_ROUTINE(allocatedFunctionAddress), allocatedDataAddress) != NResult::Success)
    {
      return false;
    }

    // wait until thread finishes
    thread.WaitUntilFinished();
    return true;
  }
}

#pragma runtime_checks("", off )
static void __stdcall __NullyLoadDLL(void* a_data)
{
  if (a_data == nullptr)
  {
    return;
  }

  Nully::NDLLLoadData* data = (Nully::NDLLLoadData*)a_data;

  NLoadDLL loadDLL = data->loadLibraryAddress;
  loadDLL(data->dllPath);
}
#pragma runtime_checks( "", restore)



#pragma runtime_checks("", off )
static void __stdcall __NullyFreeDLL(void* a_data)
{
  if (a_data == nullptr)
  {
    return;
  }

  Nully::NDLLFreeData* data = (Nully::NDLLFreeData*)a_data;

  NFreeDLL freeDLL = data->freeLibraryAddress;
  freeDLL(data->module);
}
#pragma runtime_checks( "", restore)