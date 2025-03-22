#pragma once
#include "Tribes-Ascend-SDK/SdkHeaders.h"

#define GWORLD_ADDRESS (0x013cb664)

extern UWorld* GWorld;
extern UEngine* Engine;

// Check if a player is valid... this is, check if it's alive
bool IsPawnValid(ATrPlayerPawn* pawn);

// Get all instances of a specific UObject type in the GObjects buffer
template< class T >
std::vector<T*> GetInstancesUObjects(void)
{
    std::vector<T*> foundUObjects;

    for (int i = 0; i < UObject::GObjObjects()->Count; ++i)
    {
        UObject* Object = UObject::GObjObjects()->Data[i];
        if (!Object || !Object->IsA(T::StaticClass()))
            continue;

        foundUObjects.push_back(reinterpret_cast<T*>(Object));
    }
    return foundUObjects;
}