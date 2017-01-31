#pragma once

namespace Nully
{
  enum class NFileMode
  {
    Read, // Open file for input operations. The file must exist.
    Write, // Create an empty file for output operations. If a file with the same name already exists, its contents are discarded and the file is treated as a new empty file.
    Append, // Open file for output at the end of a file. Output operations always write data at the end of the file, expanding it. Repositioning operations (fseek, fsetpos, rewind) are ignored. The file is created if it does not exist.
    ReadPlus, // Open a file for update(both for input and output).The file must exist.
    WritePlus, // Create an empty file and open it for update (both for input and output). If a file with the same name already exists its contents are discarded and the file is treated as a new empty file.
    AppendPlus, // Open a file for update (both for input and output) with all output operations writing data at the end of the file. Repositioning operations (fseek, fsetpos, rewind) affects the next input operations, but output operations move the position back to the end of file. The file is created if it does not exist.
  };
}