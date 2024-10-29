// Shoot Them Up Game. All Rights Reserved.


#include "AI/Decorator/STUNeedAmmoDecorator.h"
#include "Component/STUWeaponComponent.h"
#include "STUUtils.h"
#include "AIController.h"

USTUNeedAmmoDecorator::USTUNeedAmmoDecorator()
{
    NodeName = "Need Ammo";
}

bool USTUNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller)
    {
        return false;
    }

    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
    if (!WeaponComponent)
    {
        return false;
    }

    return WeaponComponent->NeedAmmo(WeaponClass);
}
