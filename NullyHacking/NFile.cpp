#include "NFile.h"

namespace Nully
{
  NFile::NFile()
    : m_file(nullptr)
  {

  }

  NFile::NFile(const char * a_path, const NFileMode a_mode)
    : m_file(nullptr)
  {
    Open(a_path, a_mode);
  }

  NResult NFile::Open(const char * a_path, const NFileMode a_mode)
  {
    if (m_file)
    {
      Close();
    }

    std::string openMode = "";

    switch (a_mode)
    {
    case NFileMode::Read:
      openMode = "rb";
      break;
    case NFileMode::Write:
      openMode = "wb";
      break;
    case NFileMode::Append:
      openMode = "ab";
      break;
    case NFileMode::ReadPlus:
      openMode = "rb+";
      break;
    case NFileMode::WritePlus:
      openMode = "wb+";
      break;
    case NFileMode::AppendPlus:
      openMode = "ab+";
      break;
    default:
      openMode = "rb";
      break;
    }

    FILE* file = nullptr;
    fopen_s(&file, a_path, openMode.c_str());
    if (file == nullptr)
    {
      return NResult::FileOpen;
    }
    else
    {
      Close();
      this->m_file = file;
      return NResult::Success;
    }
  }

  NResult NFile::Read(void * a_buffer, const size_t a_bufferSize) const
  {
    if (this->m_file == nullptr || a_buffer == nullptr)
    {
      return NResult::Nullptr;
    }

    size_t readBytes = fread(a_buffer, a_bufferSize, 1, this->m_file);
    if (readBytes != a_bufferSize)
    {
      // file-end maybe reached...
      return NResult::FileRead;
    }
    else
    {
      return NResult::Success;
    }
  }

  NResult NFile::ReadLine(char* a_buffer, const size_t a_bufferSize) const
  {
    if (this->m_file == nullptr || a_buffer == nullptr)
    {
      return NResult::Nullptr;
    }

    if (fgets(a_buffer, a_bufferSize, m_file) == nullptr)
    {
      return NResult::Success;
    }

    return NResult::FileReadLine;
  }

  NResult NFile::Write(const void * a_buffer, const size_t a_bufferSize) const
  {
    if (this->m_file == nullptr || a_buffer == nullptr)
    {
      return NResult::Nullptr;
    }

    size_t bytesWritten = fwrite(a_buffer, a_bufferSize, 1, this->m_file);

    if (bytesWritten != a_bufferSize)
    {
      return NResult::FileWrite;
    }

    return NResult::Success;
  }

  void NFile::SetFilePosition(const NFilePosition a_position, const int64_t a_offset) const
  {
    if (this->m_file == nullptr)
    {
      return;
    }

    unsigned char pos = 0;

    switch (a_position)
    {
    case NFilePosition::Begin:
      pos = SEEK_SET;
      break;
    case NFilePosition::Current:
      pos = SEEK_CUR;
      break;
    case NFilePosition::End:
      pos = SEEK_END;
      break;
    default:
      pos = SEEK_SET;
      break;
    }

    _fseeki64(this->m_file, a_offset, pos);
  }

  NFile::~NFile()
  {
    Close();
  }

  uint64_t NFile::GetFileSize(const char * a_path)
  {
    if (a_path == nullptr)
    {
      return 0;
    }

    HANDLE hFile = CreateFileA(a_path, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hFile == nullptr)
    {
      return 0;
    }

    LARGE_INTEGER size;
    size.QuadPart = 0;

    ::GetFileSizeEx(hFile, &size);

    CloseHandle(hFile);

    if (size.QuadPart > 0)
    {
      return size.QuadPart;
    }

    return 0;
  }

  inline void NFile::Close()
  {
    if (m_file)
    {
      fclose(m_file);
    }
  }
}