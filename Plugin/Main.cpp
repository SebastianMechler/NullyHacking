#include "Mage.h"

Nully::Mage g_mage;

extern "C"
{
  __declspec(dllexport) Nully::IPlugin* GetPlugin()
  {
    return &g_mage;
  }
}