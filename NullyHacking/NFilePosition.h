#pragma once

namespace Nully
{
  enum class NFilePosition
  {
    Begin,
    Current,
    End, // WARNING: !NOT SUPPORTED ON ALL PLATFORMS!
  };
}