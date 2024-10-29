// Shoot Them Up Game. All Rights Reserved.

#include "AI/STUAIController.h"
#include "AI/STUAIBaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Component/STUAIPerceptionComponent.h"
#include "Component/STURespawnComponent.h"

ASTUAIController::ASTUAIController()
{
    STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>(TEXT("STUAIPerceptionComponent"));
    SetPerceptionComponent(*STUAIPerceptionComponent);

    STURespawnComponent = CreateDefaultSubobject<USTURespawnComponent>(TEXT("STURespawnComponent"));

    bWantsPlayerState = true;
}

void ASTUAIController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);

    const auto STUCharacter = Cast<ASTUAIBaseCharacter>(InPawn);
    if (STUCharacter)
    {
        RunBehaviorTree(STUCharacter->BehaviorTreeAsset);
    }
}

void ASTUAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const auto AimActor = GetFocusOnActor();
    SetFocus(AimActor);
}

AActor *ASTUAIController::GetFocusOnActor() const
{
    if (!GetBlackboardComponent())
    {
        return nullptr;
    }

    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
