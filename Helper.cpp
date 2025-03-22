#include <format>
#include <plog/Log.h>
#include "Helper.hpp"

UWorld* GWorld{ *reinterpret_cast<UWorld**>(GWORLD_ADDRESS) };
UEngine* Engine{ nullptr };

bool IsPawnValid(ATrPlayerPawn* pawn)
{
    if (pawn && pawn->PlayerReplicationInfo && pawn->Health)
    {
        return true;
    }
    PLOG_WARNING << "Pawn is no longer valid";
    return false;
}