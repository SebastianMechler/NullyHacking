#pragma once

// include plugin
#include <IPlugin.h>
#include <NPlayer.h>

namespace Nully
{
  class Mage : public IPlugin
  {
  public:
    void Initialize() override;
    NPluginState Update(NPlayer& a_player) override;
    void Shutdown() override;
    ~Mage();
  };
}