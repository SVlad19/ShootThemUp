// Shoot Them Up Game. All Rights Reserved.


#include "UI/STUPlayerHUDWidget.h"
#include "Component/STUHealthComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    const auto Player = GetOwningPlayerPawn();

    if (!Player)
    {
        return 0.f;
    }

    const auto HealthComponent = Player->GetComponentByClass<USTUHealthComponent>();

    if (!HealthComponent)
    {

        return 0.f;
    }

    return HealthComponent->GetHealthPercent();
}
