// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputDataConfig;
class USpringArmComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
    GENERATED_BODY()

  public:
    ASTUBaseCharacter(const FObjectInitializer& ObjInit);
    
    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const;
    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection()const;

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

  public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent *CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent *SpringArmComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
    UInputMappingContext *InputMapping;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
    UInputDataConfig *InputActions;

  private:
    bool IsSprinting = false;
    bool IsMovingForward = false;
};
