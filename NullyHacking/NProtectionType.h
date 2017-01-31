#pragma once

#include <Windows.h>

#include "NTypes.h"

namespace Nully
{
  enum class NProtectionType : uint32_t
  {
    Execute = PAGE_EXECUTE, // Enables execute access to the committed region of pages. An attempt to write to the committed region results in an access violation.
    ExecuteRead = PAGE_EXECUTE_READ, // Enables execute or read-only access to the committed region of pages. An attempt to write to the committed region results in an access violation.
    ExecuteReadWrite = PAGE_EXECUTE_READWRITE, // Enables execute, read - only, or read / write access to the committed region of pages.
    ExecuteWriteCopy = PAGE_EXECUTE_WRITECOPY, // Enables execute, read-only, or copy-on-write access to a mapped view of a file mapping object. An attempt to write to a committed copy-on-write page results in a private copy of the page being made for the process. The private page is marked as PAGE_EXECUTE_READWRITE, and the change is written to the new page.

    NoAccess = PAGE_NOACCESS, // Disables all access to the committed region of pages. An attempt to read from, write to, or execute the committed region results in an access violation.
    ReadOnly = PAGE_READONLY, // Enables read-only access to the committed region of pages. An attempt to write to the committed region results in an access violation. If Data Execution Prevention is enabled, an attempt to execute code in the committed region results in an access violation.
    ReadWrite = PAGE_READWRITE, // Enables read-only or read/write access to the committed region of pages. If Data Execution Prevention is enabled, attempting to execute code in the committed region results in an access violation.
    WriteCopy = PAGE_WRITECOPY, // Enables read-only or copy-on-write access to a mapped view of a file mapping object. An attempt to write to a committed copy-on-write page results in a private copy of the page being made for the process. The private page is marked as PAGE_READWRITE, and the change is written to the new page. If Data Execution Prevention is enabled, attempting to execute code in the committed region results in an access violation.

    Guard = PAGE_GUARD, // Pages in the region become guard pages. Any attempt to access a guard page causes the system to raise a STATUS_GUARD_PAGE_VIOLATION exception and turn off the guard page status. Guard pages thus act as a one-time access alarm. For more information, see Creating Guard Pages.
    NoCache = PAGE_NOCACHE, // Sets all pages to be non-cachable. Applications should not use this attribute except when explicitly required for a device. Using the interlocked functions with memory that is mapped with SEC_NOCACHE can result in an EXCEPTION_ILLEGAL_INSTRUCTION exception.
    WriteCombine = PAGE_WRITECOMBINE // Applications should not use this attribute except when explicitly required for a device. Using the interlocked functions with memory that is mapped as write-combined can result in an EXCEPTION_ILLEGAL_INSTRUCTION exception.
  };
}