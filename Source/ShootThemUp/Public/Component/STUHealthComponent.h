// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "STUHealthComponent.generated.h"

class UCameraShakeBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    USTUHealthComponent();

    float GetHealth() const
    {
        return Health;
    }

    UFUNCTION()
    void OnTakeAnyDamageHandle(AActor *DamagedActor, float Damage, const class UDamageType *DamageType,
                               class AController *InstigatedBy, AActor *DamageCauser);

    UFUNCTION(BlueprintCallable, Category = "Health")
    FORCEINLINE bool IsDead() const
    {
        return FMath::IsNearlyZero(Health);
    }

    UFUNCTION(BlueprintCallable, Category = "Health")
    FORCEINLINE float GetHealthPercent() const
    {
        return (Health / MaxHealth);
    }

    bool TryToAddHP(float HPAmount);

    FOnDeath OnDeath;
    FOnHealthChanged OnHealthChanged;

  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    float MaxHealth = 100.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
    bool AutoHeal = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealUpdateTime = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealDelay = 3.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealModifier = 1.f;   
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<UCameraShakeBase> CameraShake;

    virtual void BeginPlay() override;

  private:
    UFUNCTION()
    void HealUpdate();

    float Health = 0.f;
    FTimerHandle HealTimer;

    void SetHealth(float NewHealth);
    void PlayCameraShake();
};
