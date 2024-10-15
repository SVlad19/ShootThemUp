// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputDataConfig;
class USpringArmComponent;
class USTUHealthComponent;
class UTextRenderComponent;
class USTUWeaponComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
    GENERATED_BODY()

  public:
    ASTUBaseCharacter(const FObjectInitializer &ObjInit);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const;
    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

  public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

  protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void Move(const FInputActionValue &Value);

    UFUNCTION()
    void Look(const FInputActionValue &Value);

    UFUNCTION()
    void Jumping(const FInputActionValue &Value);

    UFUNCTION()
    void Sprint(const FInputActionValue &Value);

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UCameraComponent> CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<USTUHealthComponent> HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<USTUWeaponComponent> WeaponComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UTextRenderComponent> HealthTextComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<USpringArmComponent> SpringArmComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
    TObjectPtr<UInputMappingContext> InputMapping;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
    TObjectPtr<UInputDataConfig> InputActions;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    TObjectPtr<UAnimMontage> DeathAnimMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
    FVector2D LandedDamageVelocity = FVector2D(900.f, 1200.f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
    FVector2D LandedDamage = FVector2D(10.f, 100.f);

  private:
    void OnDeath();
    void OnHealthChanged(float Health);
    UFUNCTION()
    void OnGroundLanded(const FHitResult &Hit);

    bool IsSprinting = false;
    bool IsMovingForward = false;
};
