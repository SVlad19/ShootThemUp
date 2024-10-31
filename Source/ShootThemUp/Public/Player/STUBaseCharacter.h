// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class USTUHealthComponent;
class USTUWeaponComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
    GENERATED_BODY()

  public:
    ASTUBaseCharacter(const FObjectInitializer &ObjInit);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual bool IsRunning() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

    virtual void Tick(float DeltaTime) override;

    void SetPlayerColor(const FLinearColor &Color);

  protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<USTUHealthComponent> HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<USTUWeaponComponent> WeaponComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    TObjectPtr<UAnimMontage> DeathAnimMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
    FVector2D LandedDamageVelocity = FVector2D(900.f, 1200.f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
    FVector2D LandedDamage = FVector2D(10.f, 100.f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Material")
    FName MaterialColorName = "Paint color";

    virtual void BeginPlay() override;

    virtual void OnDeath();

  private:
    virtual void OnHealthChanged(float Health, float HealthDelta);

    UFUNCTION()
    void OnGroundLanded(const FHitResult &Hit);
};
