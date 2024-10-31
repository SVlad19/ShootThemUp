// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUAIBaseCharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUAIBaseCharacter : public ASTUBaseCharacter
{
    GENERATED_BODY()

  public:
    ASTUAIBaseCharacter(const FObjectInitializer &ObjInit);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

    virtual void Tick(float DeltaTime) override;

  protected:
    virtual void OnDeath() override;
    virtual void BeginPlay() override;
    virtual void OnHealthChanged(float Health, float HealthDelta) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    float HealthVisibilityDistance = 1000.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UWidgetComponent> HealthWidgetComponent;

    private:
    void UpdateHealthWidgetVisibility();
};
