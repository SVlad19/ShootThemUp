// Shoot Them Up Game. All Rights Reserved.

#include "AI/Service/STUChangeWeaponService.h"
#include "AIController.h"
#include "Component/STUAIWeaponComponent.h"
#include "STUUtils.h"
#include "EnhancedInputComponent.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
    NodeName = "Change Weapon";
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();

    if (Controller)
    {
        const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
        if (WeaponComponent && Probability > 0 && FMath::FRand() <= Probability)
        {
            WeaponComponent->SwapWeapon(FInputActionValue());
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
