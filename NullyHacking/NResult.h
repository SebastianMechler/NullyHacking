#pragma once

namespace Nully
{
  enum class NResult
  {
    Success,
    Nullptr,

    FileOpen,
    FileWrite,
    FileRead,
    FileReadLine,

    ProcessWriteError,
    ProcessWriteErrorSize,
    ProcessOpenError,
    ProcessReadError,
    ProcessReadErrorSize,
    ProcessAllocateError,
    ProcessProtectError,

    RemoteThreadCreationFailed,
    RemoteThreadAlreadyExists,
  };
}