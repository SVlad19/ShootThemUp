// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"


class USkeletalMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

  public:
    ASTUBaseWeapon();

    virtual void StartFire();
    virtual void StopFire();

    void ChangeClip();
    FORCEINLINE bool CanReload() const
    {
        return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
    }

    FORCEINLINE FWeaponUIData GetUIData() const
    {
        return UIData;
    }

    FORCEINLINE FAmmoData GetAmmoData() const
    {
        return CurrentAmmo;
    }

    FOnClipEmptySignature OnClipEmpty;

  protected:
    virtual void BeginPlay() override;
    virtual void MakeShot();
    virtual bool GetTraceData(FVector &TraceStart, FVector &TraceEnd) const;
    APlayerController *GetPlayerController() const;
    bool GetPlayerViewPoint(FVector &ViewLocation, FRotator &ViewRotation) const;
    FVector GetMuzzleLocation() const;
    void MakeHit(FHitResult &HitResult, const FVector &TraceStart, const FVector &TraceEnd);

    void DecreaseAmmo();
    FORCEINLINE bool IsAmmoEmpty() const
    {
        return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && CurrentAmmo.Bullets == 0;
    }
    FORCEINLINE bool IsClipEmpty() const
    {
        return CurrentAmmo.Bullets == 0;
    }

    void LogAmmo();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{15, 10, false};

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<USkeletalMeshComponent> WeaponMeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FWeaponUIData UIData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Socket")
    FName MuzzleSocketName = "MazzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trace")
    float TraceDistance = 1500.f;

  private:
    FAmmoData CurrentAmmo;
};
