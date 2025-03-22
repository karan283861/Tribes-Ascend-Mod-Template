#include <format>
#include <unordered_map>
#include <plog/Log.h>
#include "NativeHooks.hpp"
#include "Helper.hpp"

UGameEngineTickPrototype OriginalUGameEngineTick = reinterpret_cast<UGameEngineTickPrototype>(UGAMEENGINE_TICK_ADDRESS);
void __fastcall UGameEngine_Tick_Hook(UGameEngine* GameEngine, void* Unused, float DeltaSeconds)
{
    PLOG_DEBUG << __FUNCTION__;
    Engine = GameEngine;
    OriginalUGameEngineTick(GameEngine, Unused, DeltaSeconds);
}