#include "NExecutablePath.h"
#include <cstdio>

namespace Nully
{
  void NExecutablePath::Initialize()
  {
    char tmp[MAX_PATH];
    GetModuleFileNameA(GetModuleHandle("NullyByte.dll"), tmp, MAX_PATH);

    char* pTMP = tmp + strlen(tmp) - strlen("NullyByte.dll");
    *pTMP = '\0';

    m_root = tmp;
    m_profile = ConcatFromRoot("Profile\\");
    m_plugin = ConcatFromRoot("Plugin\\");

    //MessageBox(0, m_root.c_str(), "", MB_OK);
    //MessageBox(0, m_profile.c_str(), "", MB_OK);
    //MessageBox(0, m_plugin.c_str(), "", MB_OK);
  }

  NString Nully::NExecutablePath::ConcatFromRoot(char * a_concat)
  {
    return m_root + a_concat;
  }

  NString NExecutablePath::Concat(NString & a_string, char * a_concat)
  {
    return a_string + a_concat;
  }

  NString& NExecutablePath::GetRoot()
  {
    return m_root;
  }

  NString& NExecutablePath::GetPlugin()
  {
    return m_plugin;
  }

  NString& NExecutablePath::GetProfile()
  {
    return m_profile;
  }

}
