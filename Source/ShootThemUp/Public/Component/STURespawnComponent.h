// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STURespawnComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTURespawnComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    USTURespawnComponent();

    void Respawn(int32 RespawnTime);

    FORCEINLINE int32 GetRespawnCountDown() const
    {
        return RespawnCountDown;
    }

    FORCEINLINE bool IsRespawnInProgress() const
    {
        return GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
    }

  private:
    FTimerHandle RespawnTimerHandle;
    int32 RespawnCountDown = 0;

    void RespawnTimerUpdate();
};
