// Shoot Them Up Game. All Rights Reserved.


#include "Pickup/STUHealthPickup.h"
#include "Component/STUHealthComponent.h"
#include "STUUtils.h"

bool ASTUHealthPickup::GivePickupTo(APawn *PlayerPawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);
    if (!HealthComponent || HealthComponent->IsDead())
    {
        return false;
    }

    return HealthComponent->TryToAddHP(HPAmount);
}
