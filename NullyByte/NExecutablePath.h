#pragma once

#include <Windows.h>
#include <NString.h>

namespace Nully
{
  class NExecutablePath
  {
  public:
    void Initialize();

    NString ConcatFromRoot(char* a_concat);
    NString Concat(NString& a_string, char* a_concat);

    NString& GetRoot();
    NString& GetPlugin();
    NString& GetProfile();

  private:
    NString m_root;
    NString m_plugin;
    NString m_profile;
  };
}