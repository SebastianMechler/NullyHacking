#pragma once

// wrapper macro to calc base address to each offset
#define NADD_BASE(p, a) (void*)(a + (uint32_t)p.GetBaseAddress())

#define NCTM_X 0x84D890 // 1.12.1 rebased
#define NCTM_Y 0x84D894 // 1.12.1 rebased
#define NCTM_Z 0x84D898 // 1.12.1 rebased
#define NCTM_ACTION 0x84D888 // 1.12.1 rebased

#define NOBJECT_MANAGER 0x0741414 // 1.12.1 rebased
#define NOBJECT_MANAGER_FIRST_OBJECT 0xAC // 1.12.1 rebased
#define NOBJECT_MANAGER_NEXT_OBJECT 0x3C // 1.12.1 rebased

// STRUCT
#define NPLAYER_OBJECT_TYPE 0x14 // 1.12.1 rebased
#define NPLAYER_GUID 0x30 // 1.12.1 rebased
#define NPLAYER_POSITION 0x9B8 // 1.12.1 rebased
#define NPLAYER_ROTATION 0x9c4 // 1.12.1 rebased // 9c4 // 9f8
#define NPLAYER_MOVEMENT_FLAG 0x9e8 // 1.12.1 rebased
// END STRUCT

#define NPLAYER_CURRENT_LEVEL 0x7C7624 // 1.12.1 rebased
#define NPLAYER_GLOBAL_GUID 0x741E30 // 1.12.1 rebased
#define NPLAYER_IS_ALIVE 0x7C6B60 // 1.12.1 rebased [1 byte]
#define NPLAYER_CURRENT_TARGET 0x74E2D8 // 1.12.1 rebased [8 byte]
#define NPLAYER_LAST_TARGET 0x74E2E0 // 1.12.1 rebased [8 byte]
#define NPLAYER_NAME 0x827D88 // 1.12.1 rebased
#define NPLAYER_CLASS 0x827e81 // 1.12.1 rebased [1 byte]
#define NPLAYER_CASTING_SPELL_ID 0x8eca88 // 1.12.1 rebased [4 byte]

// FUNCTIONS
#define NFRAMESCRIPT_EXECUTE 0x00704CD0 // 1.12.1
#define NPLAYER_SET_FACTING 0x0076DCE0 // 1.12.1

typedef void(__cdecl *NPlayer_SetFacing)(float);

// edx + eax = guid
//#define NPLAYER_X 0x00C7B544
//#define NPLAYER_Y 0x00C7B548
//#define NPLAYER_Z 0x00C7B54C

/*
guid is at [obj+0x30], 8 bytes
B4E2C8 - Current Object Track Guid
B4E2D0 - Interact Target Guid
B4E2D8 - Locked Target Guid
B4E2E8 - Last Enemy Target Guid
*/