// Shoot Them Up Game. All Rights Reserved.


#include "Component/STUAIWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "EnhancedInputComponent.h"

void USTUAIWeaponComponent::StartFire()
{
    if (!CanFire())
    {
        return;
    }

    if (CurrentWeapon->IsAmmoEmpty())
    {
        SwapWeapon(FInputActionValue());
    }
    else
    {
        CurrentWeapon->StartFire();
    }
}

void USTUAIWeaponComponent::SwapWeapon(const FInputActionValue &Value)
{
    if (!CanEquip())
    {
        return;
    }

    int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    while (NextIndex != CurrentWeaponIndex)
    {
        if (!Weapons[NextIndex]->IsAmmoEmpty())
        {
            break;
        }
        NextIndex = (NextIndex + 1) % Weapons.Num();

    }

    if (CurrentWeaponIndex != NextIndex)
    {
        CurrentWeaponIndex = NextIndex;
        EquipWeapon(CurrentWeaponIndex);
    }
}

