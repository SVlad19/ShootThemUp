// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STUCoreTypes.h"
#include "STUPlayerController.generated.h"

class USTURespawnComponent;
struct FInputActionValue;
class UInputDataConfig;

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerController : public APlayerController
{
    GENERATED_BODY()

  public:
    ASTUPlayerController();

  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<USTURespawnComponent> STURespawnComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
    TObjectPtr<UInputDataConfig> InputActions;

    UFUNCTION()
    void OnPauseGame(const FInputActionValue &Value);

    virtual void OnPossess(APawn *InPawn) override;
    virtual void BeginPlay() override;
    virtual void SetupInputComponent();

    private:
    void OnMatchStateChanged(ESTUMatchState State);
};
