// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "Component/STUWeaponComponent.h"
#include "CoreMinimal.h"
#include "STUAIWeaponComponent.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUAIWeaponComponent : public USTUWeaponComponent
{
    GENERATED_BODY()

  public:
    virtual void StartFire() override;
    virtual void SwapWeapon(const FInputActionValue &Value)override;
};
