#include "Mage.h"

#include <Windows.h>

namespace Nully
{
  void Mage::Initialize()
  {
    NLua_Print("Plugin: [Mage.dll] initialized");
  }

  NPluginState Mage::Update(NPlayer& a_player)
  {
    if (a_player.GetClassType() != NClassType::Warlock)
    {
      return NPluginState::Continue;
    }

    if (!a_player.HasTarget())
    {
      NLua_Print("Searching for target...");

      // wait for mana
      if (a_player.GetPercentageEnergy() < 0.75f)
      {
        return NPluginState::Wait;
      }

      return NPluginState::Continue;
    }
    
    a_player.FaceTarget(a_player.GetTarget());
    if (a_player.GetPercentageEnergy() > 0.25f)
    {
      a_player.CastSpellByName("Shadow Bolt");
    }
    

    // tell the bot to wait
    return NPluginState::Wait;
  }

  void Mage::Shutdown()
  {
  }

  Mage::~Mage()
  {
  }
}
