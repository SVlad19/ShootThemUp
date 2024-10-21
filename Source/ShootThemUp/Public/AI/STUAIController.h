// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "STUAIController.generated.h"

class USTUAIPerceptionComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUAIController : public AAIController
{
    GENERATED_BODY()

    public:
    ASTUAIController();

  protected:
    virtual void OnPossess(APawn *InPawn) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<USTUAIPerceptionComponent> STUAIPerceptionComponent;

     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName FocusOnKeyName = "EnemyActor";
  
    virtual void Tick(float DeltaTime) override;

    private:
    AActor *GetFocusOnActor() const;
};
