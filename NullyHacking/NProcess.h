#pragma once
#include <Windows.h>
#include <Psapi.h> // EnumProcessModules
#include <algorithm> // sorting

#include "NTypes.h"
#include "NResult.h"
#include "NProcessData.h"
#include "NAllocationType.h"
#include "NDeallocationType.h"
#include "NProtectionType.h"
#include "NVector.h"
#include "NMap.h"
#include "NProcessModule.h"
#include "NRemoteThread.h"

namespace Nully
{
  class NProcess
  {
  public:
    ////=======================================================================
    //// Constructor / Destructor
    ////=======================================================================

    /// @brief Default constructor
    NProcess();

    /// @brief Constructor which creates a new instance and opens the passed processID
    /// @param a_processID Unique ID from a given process
    NProcess(const uint32_t a_processID);

    /// @brief Destructor which releases used resources
    ~NProcess();

    ////=======================================================================
    //// Public Member functions
    ////=======================================================================

    /// @brief Opens a process. The process-handle will be stored internally.
    /// @param a_processID Unique ID from a given process
    /// @return onSuccess: NResult::Success
    /// @return onFailure: NResult::ProcessOpenError
    NResult Open(const uint32_t a_processID);

    /// @brief Opens a process. The process-handle will be stored internally.
    /// @param a_processData data which contains the id of the process.
    /// @return onSuccess: NResult::Success 
    /// @return onFailure: NResult::ProcessOpenError
    NResult Open(const NProcessData& a_processData);

    /// @brief Returns the resource/handle for the process.
    const HANDLE GetHandle() const;

    /// @brief Closes all used resources, will also be done by Destructor (RAII).
    void Close();

    /// @brief Reads bytes of type T at the location of a_address.
    /// @param a_address Valid number which will be read from.
    /// @param a_result [Optional] If you care about errors, make sure to pass a reference here.
    /// @return onSuccess: Sets a_result to NResult::Success and returns the read value.
    /// @return onFailure: a_result contains failure information e. g. NResult::Nullptr, NResult::ProcessReadError, NResult::ProcessReadErrorSize and 0 will be returned.
    template <typename T>
    T Read(const void* a_address, NResult& a_result = NResult::Success) const;

    /// @brief Writes a value of type T to the passed address.
    /// @param a_address Valid number which will be read from.
    /// @param a_value Value which will be written to the location of a_address.
    /// @return onSuccess: NResult::Success
    /// @return onFailure: NResult::ProcessWriteError, NResult::ProcessWriteErrorSize, NResult::Nullptr
    template <typename T>
    NResult Write(void* a_address, const T a_value) const;

    /// @brief Reads c-string until a '\0' is found.
    /// @param a_address Valid number which will be read from.
    /// @param a_buffer Buffer which will receive the read bytes.
    /// @param a_bufferSize Size of the buffer, to prevent overwriting memory.
    /// @return onSuccess: NResult::Success 
    /// @return onFailure: NResult::ProcessReadError or NResult::Nullptr
    NResult ReadText(const void* a_address, char* a_buffer, const size_t a_bufferSize) const;

    /// @brief Writes a c-string to address.
    /// @param a_address Valid number which will be written to.
    /// @param a_buffer Buffer which contains the data of bytes.
    /// @param a_bufferLength [Optional] In order to write bytes until '\0' is found the correct length of c-string must be passed.
    /// @return onSuccess: NResult::Success
    /// @return onFailure: NResult::ProcessWriteError, NResult::ProcessWriteErrorSize, NResult::Nullptr
    NResult WriteText(void* a_address, const char* a_buffer, const size_t a_bufferLength = 0) const;

    /// @brief Writes data to address.
    /// @param a_address Valid number which will be written to.
    /// @param a_buffer pointer to buffer which contains the data.
    /// @param a_bufferSize Size of the buffer in bytes, to prevent overwriting memory.
    /// @return onSuccess: NResult::Success
    /// @return onFailure: NResult::ProcessWriteError, NResult::ProcessWriteErrorSize, NResult::Nullptr
    NResult WriteBytes(void* a_address, const void* a_buffer, const size_t a_bufferSize) const;

    /// @brief Allocates memory inside the opened process. Allocated memory is initialized with zero. You don't have to care about freeing allocated memory. This process is done by the class itself in Destructor.
    /// @param a_size The size of the region of memory to allocate, in bytes.
    /// @param a_allocation The type of memory allocation.
    /// @param a_protection The memory protection for the region of pages to be allocated.
    /// @param a_address [Optional] Address, that specifies a desired starting address for the region of pages that you want to allocate.
    /// @return onSuccess: NResult::Success
    /// @return onFailure: NResult::ProcessAllocateError
    NResult VirtualAllocate(const size_t a_size, const NAllocationType a_allocation, const NProtectionType a_protection, void*& a_allocatedBaseAddress, void* a_desiredAddress = nullptr);

    /// @brief Changes the protection-state of memory inside the process.
    /// @param a_address A pointer to the address of the region of pages whose access protection attributes are to be changed.
    /// @param a_size The size of the region whose access protection attributes are changed, in bytes.
    /// @param a_newProtection The memory protection option.
    /// @param a_oldProtection A pointer to a variable that receives the previous access protection of the first page in the specified region of pages. If this parameter is NULL or does not point to a valid variable, the function fails.
    /// @return onSuccess: NResult::Success
    /// @return onFailure: NResult::ProcessProtectError
    NResult VirtualProtect(void* a_address, const size_t a_size, const NProtectionType a_newProtection, NProtectionType*& a_oldProtection);

    /// @brief Calculates the base module address.
    /// @return onSuccess: valid pointer of base-address
    /// @return onFailure: nullptr
    void* GetBaseAddress();

    /// @brief Returns the processID
    /// @return Returns the processID
    uint32_t GetProcessID() const;

    /// @brief Writes all processes with name and id to desired file. If the file already exists, the contents will be deleted.
    /// @param a_processData Data which is obtained from GetProcessListByName.
    /// @param a_filePath Path to the file.
    /// @return onSuccess: NResult::Success
    /// @return onFailure: NResult::Nullptr
    NResult DumpToFile(const NVector<NProcessData>& a_processData, const char* a_filePath);

    // TODOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
    /// @brief Enumerates all modules in the opened process. Limit is set to 250 number of modules.
    /// @return onSuccess: vector
    /// @return onFailure: empty vector
    NVector<NProcessModule> GetModuleList();

    ////=======================================================================
    //// Public Static functions
    ////=======================================================================

    /// @brief Sets debug privileges to the executeable in order to read/write to other processes. Use this if you have trouble with reading/writing.
    /// @return onSuccess: true
    /// @return onFailure: false
    static bool SetDebugPrivilegeState(const bool a_state);

    /// @brief Retrieves a list processes. Process without name will be flagged with "unknown"
    /// @param a_processName Name of the searching process or nullptr to receive all current processes.
    /// @param a_getFullProcessPathName [Optional] set this to true if you want the path to the executeable file on the harddisk.
    static NVector<NProcessData> GetProcessListByName(const char* a_processName, const bool a_getFullProcessPathName = false);

  private:

    ////=======================================================================
    //// Private Member functions
    ////=======================================================================
    /// @brief Deleted copy constructor
    NProcess(const NProcess&) = delete;

    /// @brief Deleted move constructor
    NProcess(const NProcess&&) = delete;

    /// @brief Deleted assignment operator
    NProcess& operator=(const NProcess&) = delete;


    ////=======================================================================
    //// Private Members
    ////=======================================================================
    static const uint32_t m_maxProcesses = 8096;
    void* m_processHandle;
    uint32_t m_processID;
    NMap<void*, size_t> m_virtualAllocations;
  };
}

// template function implementation
#include "NProcess.hpp" 
