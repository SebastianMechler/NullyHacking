#include "NFunctions.h"
#include <windows.h>
#include <stdio.h>

#include "NHackBase.h"
#include "NOffsets.h"

void NULLY_API NLua_Execute(char* a_string)
{
  if (a_string == nullptr)
    return;

  WoW_FrameScript_Execute exec = (WoW_FrameScript_Execute)(NFRAMESCRIPT_EXECUTE);
  exec(a_string, "nlua.lua");
}

void NULLY_API NLua_Print(char* a_string)
{
  char szBuffer[1000] = {};
  sprintf(szBuffer, "DEFAULT_CHAT_FRAME:AddMessage(\"%s\")", a_string);
  NLua_Execute(szBuffer);
}

void NULLY_API NLua_Print(Nully::NGuid a_guid)
{
  char szBuffer[1000] = {};
  sprintf(szBuffer, "DEFAULT_CHAT_FRAME:AddMessage(\"%.016X\")", a_guid);
  NLua_Execute(szBuffer);
}

void NULLY_API NLua_Print(uint32_t a_number)
{
  char szBuffer[1000] = {};
  sprintf(szBuffer, "DEFAULT_CHAT_FRAME:AddMessage(\"%.08d\")", a_number);
  NLua_Execute(szBuffer);
}

void NULLY_API NLua_Print(char* a_string, uint32_t a_number, bool a_hex)
{
  char szBuffer[1000] = {};
  if (a_hex)
    sprintf(szBuffer, "DEFAULT_CHAT_FRAME:AddMessage(\"%s%.08X\")", a_string, a_number);
  else
    sprintf(szBuffer, "DEFAULT_CHAT_FRAME:AddMessage(\"%s%.08u\")", a_string, a_number);
  NLua_Execute(szBuffer);
}

void NULLY_API NLua_Print(float a_float)
{
  char szBuffer[1000] = {};
  sprintf(szBuffer, "DEFAULT_CHAT_FRAME:AddMessage(\"%.08f\")", a_float);
  NLua_Execute(szBuffer);
}

void NULLY_API NLua_Print(Nully::NObject* a_number)
{
  char szBuffer[1000] = {};
  sprintf(szBuffer, "DEFAULT_CHAT_FRAME:AddMessage(\"%.08X\")", a_number);
  NLua_Execute(szBuffer);
}

void NULLY_API NLua_Print(char* a_string, Nully::NObject* a_number)
{
  char szBuffer[1000] = {};
  sprintf(szBuffer, "DEFAULT_CHAT_FRAME:AddMessage(\"%s%.08X\")", a_string, a_number);
  NLua_Execute(szBuffer);
}

void NULLY_API NLua_Print(char* a_text, Nully::NVector3 a_vector)
{
  char szBuffer[1000] = {};
  sprintf(szBuffer, "DEFAULT_CHAT_FRAME:AddMessage(\"%s %.08f, %.08f, %.08f\")",a_text, a_vector.x, a_vector.y, a_vector.z);
  NLua_Execute(szBuffer);
}

void NULLY_API PrintHex(const char* a_string, uint32_t a_number)
{
  char buffer[100] = {};
  sprintf(buffer, "%s%.08X", a_string, a_number);
  MessageBox(0, buffer, "", MB_OK);
}

