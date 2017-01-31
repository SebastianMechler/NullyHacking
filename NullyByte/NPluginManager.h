#pragma once

#include <windows.h>

#include <NVector.h>

#include "IPlugin.h"

// TODO: FreeLibrary

namespace Nully
{
  typedef NVector<IPlugin*> NPluginMap;

  class NPluginManager
  {
  public:
    bool Load(const char* a_plugin);
    void InitializePlugins();
    NPluginState Update(NPlayer& a_player);
    void Shutdown();

    NPluginMap m_plugins;
  };
}
