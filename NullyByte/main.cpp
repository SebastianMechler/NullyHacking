#include <Windows.h>

#include <NullyHacking.h>

#include "NPluginManager.h"
#include "NObjectManager.h"
#include "NExecutablePath.h"

#define NSAFE_RETURN __asm /* Port output */ { __asm pop eax  __asm pop ebx __asm pop ecx __asm pop edx } return

//#define NLUA_ACCEPT_DUEL 0x004d4ce0
#define ND3D_END_SCENE 0x005A17B6
#define ND3D_END_SCENE_NEXT_INSTRUCTION 0x005A17BC
#define NCODE_CAVE_ONE 0x0049B070

#define NASM_NOP 0x90
#define ENERGY_MIN 0.5f
#define HP_MIN 0.5f
using namespace Nully;

NObjectManager g_objectManager;
NPluginManager g_pluginManager;

NClickToMove g_ctm;
NPlayer g_player;
bool g_initialized = false;
uint32_t g_currentPathIndex = 0;
NPathFile g_pathFile;
NPaths g_paths;
NUnitIDs g_unitIds;
NObject* g_target = nullptr;
size_t g_endscene = 0;
size_t g_endsceneBack = 0;
bool g_loadedPath = false;
bool g_isStopped = true;
NExecutablePath g_executablePath;

union NAddress
{
  size_t address;
  char components[4];
};

enum class NWriteType : uint8_t
{
  Call = 0xE8,
  //CallDwordPtr = 0xFF,
  Jump = 0xE9,
};

void UpdateStopState()
{
  if (GetAsyncKeyState(VK_F5) & 0x8000)
  {
    NLua_Print("Starting...");
    g_isStopped = false;
  }

  if (GetAsyncKeyState(VK_F6) & 0x8000)
  {
    NLua_Print("Stopping...");
    g_isStopped = true;
  }
}

bool UpdateRecording()
{
  if (GetAsyncKeyState(VK_F1) & 0x8000)
  {
    g_pathFile.StartRecord();
    NLua_Print("Starting to record paths.");
  }

  if (GetAsyncKeyState(VK_F2) & 0x8000)
  {
    g_pathFile.StopRecord();
    NLua_Print("Stopping to record paths.");
  }

  if (GetAsyncKeyState(VK_F3) & 0x8000)
  {
    g_pathFile.StopRecord();
    g_pathFile.SaveRecording(g_executablePath.Concat(g_executablePath.GetProfile(), "SavedPath.txt").c_str());
    NLua_Print("Saved recorded paths.");
  }

  if (GetAsyncKeyState(VK_F4) & 0x8000)
  {
    if (g_pathFile.Reload(g_executablePath.Concat(g_executablePath.GetProfile(), "path.txt").c_str()))
    {
      NLua_Print("Successfully reloaded path.");
      g_paths = g_pathFile.GetPaths();
      g_unitIds = g_pathFile.GetUnitIDs();
    }
    else
    {
      NLua_Print("Failed to reload path.");
    }
  }

  if (g_pathFile.IsRecording())
  {
    g_pathFile.UpdateRecord(g_ctm, g_player, g_objectManager);
    return true;
  }

  return false;
}

void Write(size_t a_WriteToLocation, size_t a_callAddress, NWriteType a_type = NWriteType::Jump, uint32_t a_fillNops = 0)
{
  uint32_t callSize = 5;
  NAddress addr = {};
  char buffer[1024] = {};

  if (a_type == NWriteType::Jump || a_type == NWriteType::Call)
  {
    addr.address = a_callAddress - (a_WriteToLocation + callSize);

    //if (addr.address < 0)
    //{
    //  // we got a jump from bottom to top
    //  addr.address = (0xFFFFFFFF - abs(a_WriteToLocation - a_callAddress)) - 0x4;
    //}

    char lBuffer[5] = { (char)a_type,  addr.components[0], addr.components[1], addr.components[2], addr.components[3] };
    strcpy(buffer, lBuffer);
  }

  DWORD old = 0;
  if (!VirtualProtect((LPVOID)a_WriteToLocation, callSize + a_fillNops, PAGE_EXECUTE_READWRITE, &old))
  {
    return;
  }

  // write buffer to it
  char* p = (char*)a_WriteToLocation;
  for (auto i = 0; i < callSize; i++)
  {
    *p = buffer[i];
    p++;
  }

  // fill nops
  for (auto j = 0; j < a_fillNops; j++)
  {
    *p = NASM_NOP; // nop
    p++;
  }

  DWORD oldTrash = 0;
  VirtualProtect((LPVOID)a_WriteToLocation, callSize + a_fillNops, old, &oldTrash);
}

void Update();

void GetEndScene()
{
  __asm
  {
    push eax
    mov eax, [ecx + 0xA8]
    mov g_endscene, eax
    pop eax

    push eax
    push ecx
    push ebx
    push edx
  }

  // wait in mainthread until g_endscene is not 0
  // write g_endscene+0xD (13dec) jmp to update
  //Write(g_endscene + 0xD, (size_t)Update, NWriteType::Call, 2);

  __asm
  {
    pop eax
    pop ebx
    pop ecx
    pop edx
  }
}

void UpdateMovementFlag()
{
  static bool frame = false;

  if (!frame)
  {
    g_player.SetMovementFlag(NMovementFlag::Stand | NMovementFlag::WalkForwards);
  }
  else
  {
    g_player.SetMovementFlag(NMovementFlag::Stand);
  }

  frame = !frame;
}

void Test()
{
  __asm
  {
    push eax
    push ebx
    push ecx
    push edx
  }

  if (!g_loadedPath)
  {
    if (!g_pathFile.Load(g_executablePath.Concat(g_executablePath.GetProfile(), "path.txt").c_str()))
    {
      NLua_Print("NPathFile => Error loading path.txt");
    }
    g_paths = g_pathFile.GetPaths();
    g_unitIds = g_pathFile.GetUnitIDs();
    g_loadedPath = true;
  }

  UpdateStopState();

  g_objectManager.Update();
  
  // Recording System
  bool isRecording = UpdateRecording();

  if (g_isStopped || isRecording)
  {
    NSAFE_RETURN;
  }

  if (!g_initialized)
  {
    g_player.Init(g_objectManager.GetPlayerBase(), g_ctm, &g_objectManager);
    g_pluginManager.InitializePlugins();
    g_initialized = true;
  }
  else
  {
    if (g_pluginManager.Update(g_player) == NPluginState::Wait)
    {
      NSAFE_RETURN;
    }
  }

  // target is dead
  // find new one
  // try to find new target
  //NLua_Print("No target... getting nearest object.");
  NObject* target = g_objectManager.GetNearestObjectByType(NObjectType::UNIT, g_player.GetPosition(), g_unitIds, 20.5f);

  // no target found ==> move player
  if (target == nullptr)
  {
    // MOVE
    if (g_paths.size() > 0)
    {
      if ((g_paths[g_currentPathIndex] - g_player.GetPosition()).Length() > 0.75f)
      {
        //if (ctm.GetType(process) != NClickToMoveType::Ground)
        {
          g_ctm.MoveTo(g_paths[g_currentPathIndex]);
        }
      }
      else
      {
        if (g_currentPathIndex + 1 >= g_paths.size())
        {
          g_currentPathIndex = 0;
        }
        else
        {
          g_currentPathIndex++;
        }
        g_ctm.MoveTo(g_paths[g_currentPathIndex]);
      }
    }
  }
  else
  {
    // target unit
    g_player.TargetByGuid(target->guid);

    // stop moving
    g_player.StopMoving();
  }


  NSAFE_RETURN;
}

void Update()
{
  __asm
  {
    mov edi, [ebp + 0x08]
    xor esi, esi
    test edi, edi
    call Test

    jmp[g_endsceneBack]
  }
}

// codecave function which has:
// write a jmp to NullyBytes.dll+GetEndScene
// looks:
// mov g_endscene, [eax+A8]
// push eax, ebc, ...
// call dword ptr [ecx+A8]
// pop
// rewrite jmp to origional data
//void AquireEndscene()
//{
//  __asm
//  {
//    push eax
//    mov eax, [ecx + 0xA8]
//    mov g_endscene, eax
//    pop eax
//    call dword ptr [ecx + 0xA8]
//    push eax
//    push ebx
//    push ecx
//    push edx
//  }
//
//  // restore old state
//  char buffer[] = { 0xFF, 0x91, 0xA8, 0x00, 0x00, 0x00 };
//
//  DWORD old = 0;
//  if (!VirtualProtect((LPVOID)ND3D_END_SCENE, sizeof(buffer), PAGE_EXECUTE_READWRITE, &old))
//  {
//    return;
//  }
//
//  char* p = (char*)ND3D_END_SCENE;
//  for (auto i = 0; i < sizeof(buffer); i++)
//  {
//    *p = buffer[i];
//    p++;
//  }
//
//  DWORD old2 = 0;
//  if (!VirtualProtect((LPVOID)ND3D_END_SCENE, sizeof(buffer), old, &old2))
//  {
//    return;
//  }
//
//  __asm
//  {
//    pop eax
//    pop ebx
//    pop ecx
//    pop edx
//  }
//}

// a_jmpToFunction = custom function in dll
// a_codeCaveLocation = location where to place the jmp patch
// a_jmpPatchLocation = location where the patch will be applied to
void WriteJumpPatch(size_t a_jmpToFunction, size_t a_codeCaveLocation, size_t a_jmpPatchLocation)
{
  // write byteCode to a_codeCaveLocation
  char byteCode[] =
  {
    NASM_NOP, NASM_NOP, NASM_NOP, NASM_NOP, NASM_NOP, // nops for our jmp patch to dll function : offset 6
    0xFF, 0x91, 0xA8, 0x00, 0x00, 0x00, // call dword ptr [ecx+A8]
    NASM_NOP, NASM_NOP, NASM_NOP, NASM_NOP, NASM_NOP, // nops for the jmp back to where we came from : offset 11
  };


  DWORD old = 0;
  if (!VirtualProtect((LPVOID)a_codeCaveLocation, sizeof(byteCode), PAGE_EXECUTE_READWRITE, &old))
  {
    return;
  }

  // write buffer to it
  //MessageBox(0, "Writing byteCode to codecave.", "", MB_OK);
  char* p = (char*)a_codeCaveLocation;
  for (auto i = 0; i < sizeof(byteCode); i++)
  {
    *p = byteCode[i];
    p++;
  }

  // write jmpPatch to custom dll function
  //MessageBox(0, "Writing jmpPatch to custom dll.", "", MB_OK);
  Write(a_codeCaveLocation, a_jmpToFunction, NWriteType::Call); // a_jmpToFunction + 6 because of stack management...

  // write jmpPatch back to a_jmpPatchLocation + 5 => next instruction
  //MessageBox(0, "Writing jmpPatch ND3D_END_SCENE_NEXT_INSTRCUTION.", "", MB_OK);
  Write(a_codeCaveLocation + 11, ND3D_END_SCENE_NEXT_INSTRUCTION);

  // write jmpPatch back in our custom dll function, no return
  // will be done by inline asm in function

  // overwrite a_jmpPatchLocation with custom jump to a_codeCaveLocation
  //MessageBox(0, "Writing jmpPatch ND3D_END_SCENE.", "", MB_OK);
  Write(ND3D_END_SCENE, a_codeCaveLocation, NWriteType::Jump, 1);
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
  {
    size_t* pUpdate = (size_t*)(((size_t)Update));
    char buffer[100] = {};
    sprintf(buffer, "%.08X", pUpdate);
    //MessageBox(0, buffer, "3", MB_OK);

    MODULEENTRY32 l_module;
    NModule module;
    NVector<MODULEENTRY32> result;
    module.GetModuleList(GetProcessId(GetCurrentProcess()), result);

    if (result.size() > 0)
    {
      l_module = module.FindModuleByName("WoW.exe", result);
      //sprintf(buffer, "%.08X", l_module.modBaseAddr);
      //MessageBox(0, buffer, "BaseAddress", MB_OK);
    }
    else
    {
      MessageBox(0, buffer, "Failed to find...", MB_OK);
    }
    g_base = (uint32_t)l_module.modBaseAddr;

    WriteJumpPatch((size_t)GetEndScene, NCODE_CAVE_ONE, ND3D_END_SCENE);
    //Write(ND3D_END_SCENE, (size_t)AquireEndscene, NWriteType::Call, 1);
    //MessageBox(0, "Writing...", "", MB_OK);
    
    // make this conditional xD
    while (g_endscene == 0)
    {
      Sleep(10);
    }

    g_executablePath.Initialize();

    g_endsceneBack = g_endscene + 0xC + 0x5;

    // wait in mainthread until g_endscene is not 0
    // write g_endscene+0xD (13dec) jmp to update
    Write(g_endscene + 0xC, (size_t)Update + 0x5, NWriteType::Jump);
    //Write(g_endscene + 0xC, (size_t)Update, NWriteType::Call);
    NString x = g_executablePath.Concat(g_executablePath.GetPlugin(), "Plugin.dll").c_str();
    if (!g_pluginManager.Load(x.c_str()))
    {
      MessageBox(0, x.c_str(), "PluginLoad ==> Failed", MB_OK);
    }
  }
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
    g_pluginManager.Shutdown();
		break;
	}
	return TRUE;
}

//// http://www.ownedcore.com/forums/general/programming/530005-c-c-dynamically-executed-code.html
//// http://zzuks.blogspot.de/2014/12/reversing-1121-wow-calling-functions-of.html
//// http://www.ownedcore.com/forums/world-of-warcraft/world-of-warcraft-bots-programs/wow-memory-editing/394292-help-endscene-hooking.html


/*
void Test()
{
__asm
{
push eax
push ebx
push ecx
push edx
}

if (!g_loadedPath)
{
if (!g_pathFile.Load(g_executablePath.Concat(g_executablePath.GetProfile(), "path.txt").c_str()))
{
NLua_Print("NPathFile => Error loading path.txt");
}
g_paths = g_pathFile.GetPaths();
g_unitIds = g_pathFile.GetUnitIDs();
g_loadedPath = true;
}

UpdateStopState();

g_objectManager.Update();

// Recording System
bool isRecording = UpdateRecording();

if (g_isStopped || isRecording)
{
NSAFE_RETURN;
}


if (!g_initialized)
{
g_player.Init(g_objectManager.GetPlayerBase());
g_pluginManager.InitializePlugins();
g_initialized = true;
}
else
{
if (g_pluginManager.Update(g_player) == NPluginState::Wait)
{
NSAFE_RETURN;
}
}





NObject* t = g_objectManager.GetObjectByGuid(g_player.GetTargetGuid());

if (t != nullptr)
{
NLua_Print("=========");
NLua_Print("CUR_HP: ", t->GetHealth(), false);
NLua_Print("CUR_ENERGY: ", t->GetEnergy(), false);
NLua_Print("OBJ_ADDRESS: ", t);
NLua_Print("MAX_HP: ", t->GetMaxHealth(), false);
NLua_Print("MAX_ENERGY: ", t->GetMaxEnergy(), false);
NLua_Print("UNIT_ID: ", t->GetUnitID(), false);
NLua_Print("UNIT_LEVEL: ", t->GetLevel(), false);
NLua_Print(t->rotation);
NLua_Print("=========");
}

if (g_player.HasTarget())
{
if (g_target)
{
if (g_target->GetHealth() == 0)
{
NLua_Print("Clearing target...");
g_player.ClearTarget();
g_target = nullptr;
}
else
{
// kill routine
//NLua_Print("Killing target...");
g_ctm.SetType(NClickToMoveType::Stop);
g_player.SetMovementFlag(NMovementFlag::Stand);
g_player.FaceTarget(*g_target);
UpdateMovementFlag();

if (g_player.GetPercentageEnergy() < 0.15f)
{
g_player.CastSpellByName("Attack");
}
else
{
//g_player.CastSpellByName("Lightning Bolt");
}
//if (g_player.GetTargetDistance(g_target) > 5.0f)
{
}
//else
{
//g_player.CastSpellByName("Raptor Strike");
}
}
}
else
{
// probably aggroed unit
g_target = g_objectManager.GetObjectByGuid(g_player.GetTargetGuid());
}
}
else
{
// check for his hp/mana
if (g_player.GetPercentageHealth() < HP_MIN || g_player.GetPercentageEnergy() < ENERGY_MIN)
{
NLua_Print("Waiting for mana/hp");
}
else
{
g_target = nullptr;
// try to find new target
//NLua_Print("No target... getting nearest object.");
NObject* target = g_objectManager.GetNearestObjectByType(NObjectType::UNIT, g_player.GetPosition(), g_unitIds, 20.5f);

// no target found ==> move player
if (target == nullptr)
{
if (g_paths.size() == 0)
{
//MessageBox(0, "No paths loaded.", "", MB_OK);
//NLua_Execute("DEFAULT_CHAT_FRAME:AddMessage(\"Waiting for paths to be loaded...\")");
}
else
{
//NLua_Print("Moving...");
if ((g_paths[g_currentPathIndex] - g_player.GetPosition()).Length() > 0.75f)
{
//if (ctm.GetType(process) != NClickToMoveType::Ground)
{
g_ctm.MoveTo(g_paths[g_currentPathIndex]);
}
}
else
{
if (g_currentPathIndex + 1 >= g_paths.size())
{
g_currentPathIndex = 0;
}
else
{
g_currentPathIndex++;
}
g_ctm.MoveTo(g_paths[g_currentPathIndex]);
}
}
}
else
{
NLua_Print(target);
g_target = target;
g_player.TargetByGuid(target->guid);
}
}
}


NSAFE_RETURN;
}
*/