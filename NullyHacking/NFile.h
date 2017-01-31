#pragma once
#include <iostream>
#include <Windows.h>
#include <string>

#include "NResult.h"
#include "NTypes.h"
#include "NFileMode.h"
#include "NFilePosition.h"

namespace Nully
{
  class NFile
  {
  public:
    ////=======================================================================
    //// Constructor / Destructor
    ////=======================================================================

    /// @brief Default-Constructor.
    NFile();

    /// @brief Constructor which opens the diresed file.
    /// @param a_path: Path with filename.
    /// @param a_mode: File-Mode e. g. for Read, Write, ...
    NFile(const char* a_path, const NFileMode a_mode);

    /// @brief Destructor which closes open resources.
    ~NFile();

    ////=======================================================================
    //// Public Member functions
    ////=======================================================================

    /// @brief Opens the desired file.
    /// @param a_path Path with filename.
    /// @param a_mode File-Mode e. g. for Read, Write, ...
    /// @return Returns NResult::Success on success and NResult::Nullptr or NResult::FileOpenError on failure.
    NResult Open(const char* a_path, const NFileMode a_mode);

    /// @brief Reads a file (file must be opened in the correct mode).
    /// @param a_buffer: Pointer to a buffer which will receive the read-bytes.
    /// @param a_bufferSize: Size of a_buffer in bytes.
    /// @return Returns NResult::Success on success and NResult::Nullptr or NResult::FileReadError on failure.
    NResult Read(void* a_buffer, const size_t a_bufferSize) const;

    /// @brief Reads a line from a file (file must be opened in the correct mode).
    /// @param a_buffer: Pointer to a buffer which will receive the read-bytes.
    /// @param a_bufferSize: Size of a_buffer in bytes.
    /// @return Returns NResult::FileReadLine if there is more to read or NResult::Success when reading is done and NResult::Nullptr on failure.
    NResult ReadLine(char* a_buffer, const size_t a_bufferSize) const;

    /// @brief Writes to a file (file must be opened in the correct mode).
    /// @param a_buffer: Pointer to a buffer which contains data to be written.
    /// @param a_bufferSize: Size of a_buffer in bytes.
    /// @return Returns NResult::Success on success and NResult::Nullptr or NResult::FileWriteError on failure.
    NResult Write(const void* a_buffer, const size_t a_bufferSize) const;

    /// @brief Sets the file pointer at specific position / offset.
    /// @param a_position: Position of file-pointer.
    /// @param a_offset: Offset which will be added after the position has been set.
    void SetFilePosition(const NFilePosition a_position, const int64_t a_offset = 0) const;

    /// @brief Opens a file to obtain the file-size
    /// @param path: path to the file as c-string
    static uint64_t GetFileSize(const char* a_path);

  private:

    ////=======================================================================
    //// Private Member functions
    ////=======================================================================

    /// @brief Closes open resources.
    inline void Close();


    ////=======================================================================
    //// Private Members
    ////=======================================================================
    FILE* m_file;
  };
}