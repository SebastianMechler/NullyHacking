#pragma once
#include <Windows.h>

#include "NTypes.h"

namespace Nully
{
  enum class NAllocationType : uint32_t
  {
    Commit = MEM_COMMIT, // Allocates memory charges (from the overall size of memory and the paging files on disk) for the specified reserved memory pages. The function also guarantees that when the caller later initially accesses the memory, the contents will be zero. Actual physical pages are not allocated unless/until the virtual addresses are actually accessed.
    Reserve = MEM_RESERVE, // Reserves a range of the process's virtual address space without allocating any actual physical storage in memory or in the paging file on disk.
    Reset = MEM_RESET, // Indicates that data in the memory range specified by lpAddress and dwSize is no longer of interest. The pages should not be read from or written to the paging file. However, the memory block will be used again later, so it should not be decommitted. This value cannot be used with any other value.
    ResetUndo = MEM_RESET_UNDO, // MEM_RESET_UNDO should only be called on an address range to which MEM_RESET was successfully applied earlier. It indicates that the data in the specified memory range specified by lpAddress and dwSize is of interest to the caller and attempts to reverse the effects of MEM_RESET. If the function succeeds, that means all data in the specified address range is intact. If the function fails, at least some of the data in the address range has been replaced with zeroes.
    TopDown = MEM_TOP_DOWN, // Allocates memory at the highest possible address. This can be slower than regular allocations, especially when there are many allocations.
    Physical = MEM_PHYSICAL, // Reserves an address range that can be used to map Address Windowing Extensions (AWE) pages.
    LargePages = MEM_LARGE_PAGES, // Allocates memory using large page support.
  };

  // operator overloading to allow logical or
  inline NAllocationType operator|(NAllocationType a_left, NAllocationType a_right)
  {
    return static_cast<NAllocationType>(static_cast<int>(a_left) | static_cast<int>(a_right));
  }
}