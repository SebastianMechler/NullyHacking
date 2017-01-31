#pragma once

#include "NObject.h"
#include "NVector3.h"
#include "NAPI.h"

typedef void(__fastcall* WoW_FrameScript_Execute)(char*, char*);

void NULLY_API NLua_Execute(char* a_string);
void NULLY_API NLua_Print(char* a_string);
void NULLY_API NLua_Print(Nully::NGuid a_guid);
void NULLY_API NLua_Print(Nully::NObject* a_number);
void NULLY_API NLua_Print(char* a_string, Nully::NObject* a_number);
void NULLY_API NLua_Print(char* a_string, uint32_t a_number, bool a_hex = true);
void NULLY_API NLua_Print(char* a_text, Nully::NVector3 a_vector);
void NULLY_API NLua_Print(float a_float);

void NULLY_API PrintHex(const char* a_string, uint32_t a_number);
