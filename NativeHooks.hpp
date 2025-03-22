#pragma once

#include "Tribes-Ascend-SDK/SdkHeaders.h"

#define UGAMEENGINE_TICK_ADDRESS (0x00790530)

using UGameEngineTickPrototype = void(__fastcall*)(UGameEngine* GameEngine, void* Unused, float DeltaSeconds);
extern UGameEngineTickPrototype OriginalUGameEngineTick;
void __fastcall UGameEngine_Tick_Hook(UGameEngine* GameEngine, void* Unused, float DeltaSeconds);
