// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <filesystem>

#include "Detours/include/detours.h"

#include <format>
#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Appenders/RollingFileAppender.h>
#ifndef _DEBUG
//#define PLOG_DISABLE_LOGGING
#endif

#include "Hook.hpp"
#include "ProcessEventHooks.hpp"
#include "ProcessInternalHooks.hpp"
#include "NativeHooks.hpp"
#include "Helper.hpp"

//#define HOOK_CALLFUNCTION
#define LOG_FILE_NAME "TribesAscendModTemplate.txt"

void OnDLLProcessAttach()
{
    auto baseAddress{ reinterpret_cast<unsigned int>(GetModuleHandle(0)) };

    std::filesystem::remove(LOG_FILE_NAME);

#ifdef _DEBUG
    static plog::RollingFileAppender<plog::TxtFormatter> fileAppender(LOG_FILE_NAME);
    plog::init(plog::verbose, &fileAppender);
#else
    static plog::RollingFileAppender<plog::TxtFormatter> fileAppender(LOG_FILE_NAME);
    plog::init(plog::info, &fileAppender);
#endif
    PLOG_INFO << "Successfully Injected DLL.";
    PLOG_INFO << std::format("Base address: {0}", reinterpret_cast<void*>(baseAddress));

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DetourAttach(&(PVOID&)OriginalProcessEventFunction, ProcessEventHook);
    DetourAttach(&(PVOID&)OriginalProcessInternalFunction, ProcessInternalHook);

    // For some reason multiple hooks on CallFunction (eg. multiple injected dlls)
    // causes buggy behaviour/crashes. We should really only hook CallFunction when
    // tracing UFunctions in the logs
#if defined(_DEBUG) && defined(HOOK_CALLFUNCTION)
    DetourAttach(&(PVOID&)OriginalCallFunctionFunction, CallFunctionHook);
#endif

    // Hook native functions

    // This is the engine tick function. Hooking here to get the start of each tick
    DetourAttach(&(PVOID&)OriginalUGameEngineTick, UGameEngine_Tick_Hook);

    auto error{ DetourTransactionCommit() };

    ProcessEventHooks = UFunctionHooks<ProcessEventPrototype>(OriginalProcessEventFunction);
    ProcessInternalHooks = UFunctionHooks<ProcessInternalPrototype>(OriginalProcessInternalFunction);
    CallFunctionHooks = UFunctionHooks<CallFunctionPrototype>(OriginalCallFunctionFunction);

    // Insert UFunction hooks below
    // For example:
    // ProcessInternalHooks.AddHook("Function TribesGame.TrProjectile.HurtRadius_Internal", TrProjectile_HurtRadius_Internal_Hook,
    // FunctionHookType::kPre, FunctionHookAbsorb::kAbsorb);
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)OnDLLProcessAttach, NULL, NULL, NULL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

