// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class USTUWeaponFXComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
    GENERATED_BODY()

  public:
    ASTUProjectile();

    FORCEINLINE void SetShotDirection(const FVector &Direction)
    {
        ShotDirection = Direction;
    }

  protected:
    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    TObjectPtr<USphereComponent> CollisionComponent;

    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    TObjectPtr<UProjectileMovementComponent> MovementComponent;

    UPROPERTY(VisibleAnywhere, Category = "VFX")
    TObjectPtr<USTUWeaponFXComponent> WeaponFXComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageRadius = 200.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 50.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool DoFullDamage = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float LifeTime = 5.f;

    virtual void BeginPlay() override;

  private:
    FVector ShotDirection;

    UFUNCTION()
    void OnProjectileHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
                         FVector NormalImpulse, const FHitResult &Hit);

    AController *GetController() const;
};
