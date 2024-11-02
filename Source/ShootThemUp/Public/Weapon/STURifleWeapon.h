// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UAudioComponent;

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

  public:
    ASTURifleWeapon();

    virtual void BeginPlay() override;
    void StartFire() override;
    void StopFire() override;

  protected:
    virtual void MakeShot() override;
    virtual bool GetTraceData(FVector &TraceStart, FVector &TraceEnd) const override;
    void MakeDamage(const FHitResult &HitResult);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire")
    float TimeBetweenShots = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire")
    float BulletSpread = 1.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trace")
    float Damage = 10.f;   

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trace")
    TObjectPtr<UNiagaraSystem>TraceFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trace")
    FString TraceTargetName = "TraceTarget";

    UPROPERTY(VisibleAnywhere, Category = "VFX")
    TObjectPtr<USTUWeaponFXComponent> WeaponFXComponent;

  private:
    FTimerHandle ShootTimerHandle;

    UPROPERTY()
    TWeakObjectPtr<UNiagaraComponent> MuzzleFXComponent;

    UPROPERTY()
    UAudioComponent *FireAudioComponent;

    void InitFX();
    void SetFXActive(bool IsActive);
    void SpawnTraceFX(const FVector &TraceStart, const FVector &TraceEnd);

    AController *GetControlller() const;
};
