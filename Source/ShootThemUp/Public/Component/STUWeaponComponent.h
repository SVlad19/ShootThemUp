// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "Weapon/STUBaseWeapon.h"
#include "STUWeaponComponent.generated.h"


struct FInputActionValue;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    USTUWeaponComponent();

    void Fire(const FInputActionValue &Value);
    void StartFire();
    void StopFire();
    void SwapWeapon(const FInputActionValue &Value);
    void Reload(const FInputActionValue &Value);

    bool GetWeaponUIData(FWeaponUIData &UIData) const;
    bool GetWeaponAmmoData(FAmmoData &AmmoData) const;
 

  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket";

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    TObjectPtr<UAnimMontage> EquipAnimMontage;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  private:
    void SpawnWeapons();
    void AttachWeaponToSocket(TObjectPtr<ASTUBaseWeapon> Weapon, USceneComponent *SceneComponent,
                              const FName &SocketName);

    void EquipWeapon(int32 WeaponIndex);
    void PlayAnimMontage(TObjectPtr<UAnimMontage> Animation);

    UPROPERTY()
    TObjectPtr<ASTUBaseWeapon> CurrentWeapon;

    UPROPERTY()
    UAnimMontage *CurrentReloadAnimMontage = nullptr;

    UPROPERTY()
    TArray<TObjectPtr<ASTUBaseWeapon>> Weapons;
    int32 CurrentWeaponIndex = 0;
    bool EquipAnimInProgress = false;
    bool ReloadAnimInProgress = false;

    void InitAnimations();
    void OnEquipFinished(USkeletalMeshComponent *MeshComp);
    void OnReloadFinished(USkeletalMeshComponent *MeshComp);

    FORCEINLINE bool CanFire() const
    {
        return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
    }
    FORCEINLINE bool CanEquip() const
    {
        return !EquipAnimInProgress && !ReloadAnimInProgress;
    }
    FORCEINLINE bool CanReload() const
    {
        return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload();
    }

    void OnEmptyClip();
    void ChangeClip();
};
