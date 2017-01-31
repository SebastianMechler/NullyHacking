#pragma once

#include "NPlayer.h"

#define NPLUGIN_FUNCTION_NAME "GetPlugin"

namespace Nully
{
  enum class NPluginState
  {
    Wait, // indicates the bot not to execute movement-system / target-system
    Continue // indicates the bot to continue movement-system / target-system
  };

  class IPlugin
  {
  public:
    virtual void Initialize() = 0;
    virtual NPluginState Update(NPlayer& a_player) = 0;
    virtual void Shutdown() = 0;

    virtual ~IPlugin() {};
  };
}

typedef Nully::IPlugin* (*GetPluginFunc)(); // function prototype to obtain a plugin
