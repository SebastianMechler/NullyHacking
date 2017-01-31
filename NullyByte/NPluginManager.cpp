#include "NPluginManager.h"

namespace Nully
{
  bool NPluginManager::Load(const char * a_plugin)
  {
    HMODULE plugin = LoadLibrary(a_plugin);
    if (plugin)
    {
      // plugin sucessfully loaded
      GetPluginFunc getPluginFunction = (GetPluginFunc)GetProcAddress(plugin, NPLUGIN_FUNCTION_NAME);
      if (getPluginFunction)
      {
        IPlugin* loadedPlugin = getPluginFunction();
        m_plugins.push_back(loadedPlugin);
        return true;
      }
    }

    return false;
  }

  void NPluginManager::InitializePlugins()
  {
    for (auto& e : m_plugins)
    {
      e->Initialize();
    }
  }

  NPluginState NPluginManager::Update(NPlayer& a_player)
  {
    NPluginState botState = NPluginState::Continue;

    for (auto& e : m_plugins)
    {
      NPluginState state = e->Update(a_player);

      if (state == NPluginState::Wait)
      {
        botState = state;
      }
    }

    return botState;
  }

  void NPluginManager::Shutdown()
  {
    for (auto& e : m_plugins)
    {
      e->Shutdown();
    }
  }
}
