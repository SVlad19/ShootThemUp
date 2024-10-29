// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputDataConfig;
class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerCharacter : public ASTUBaseCharacter
{
    GENERATED_BODY()

  public:
    ASTUPlayerCharacter(const FObjectInitializer &ObjInit);

    virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

    virtual bool IsRunning() const override;

    virtual void BeginPlay() override;

  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UCameraComponent> CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<USpringArmComponent> SpringArmComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
    TObjectPtr<UInputMappingContext> InputMapping;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
    TObjectPtr<UInputDataConfig> InputActions;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<USphereComponent> CameraCollisionComponent;

    virtual void OnDeath() override;

    UFUNCTION()
    void Move(const FInputActionValue &Value);

    UFUNCTION()
    void Look(const FInputActionValue &Value);

    UFUNCTION()
    void Jumping(const FInputActionValue &Value);

    UFUNCTION()
    void Sprint(const FInputActionValue &Value);

  private:
    bool IsSprinting = false;
    bool IsMovingForward = false;

    UFUNCTION()
    void OnCameraCollisionBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
                                       UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult & SweepResult);

    UFUNCTION()
    void OnCameraCollisionEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
                                     UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

    void CheckCameraOverlap();
};
