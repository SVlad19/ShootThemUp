// Shoot Them Up Game. All Rights Reserved.

#include "AI/STUAIBaseCharacter.h"
#include "AI/STUAIController.h"
#include "BrainComponent.h"
#include "Component/STUAIWeaponComponent.h"
#include "Component/STUHealthComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/STUHealthBarWidget.h"

ASTUAIBaseCharacter::ASTUAIBaseCharacter(const FObjectInitializer &ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))

{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = ASTUAIController::StaticClass();

    bUseControllerRotationYaw = false;

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.f, 200.f, 0.f);
    }

    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
    HealthWidgetComponent->SetupAttachment(GetRootComponent());
    HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

void ASTUAIBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateHealthWidgetVisibility();
}

void ASTUAIBaseCharacter::OnDeath()
{
    Super::OnDeath();

    const auto STUController = Cast<AAIController>(Controller);
    if (STUController && STUController->BrainComponent)
    {
        STUController->BrainComponent->Cleanup();
    }
}

void ASTUAIBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthWidgetComponent);
}

void ASTUAIBaseCharacter::OnHealthChanged(float Health, float HealthDelta)
{
    const auto HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
    if (!HealthBarWidget)
    {
        return;
    }
    HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
}

void ASTUAIBaseCharacter::UpdateHealthWidgetVisibility()
{
    if (!GetWorld()->GetFirstPlayerController() || !GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator())
    {
        return;
    }

    const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();
    const auto AICharacterLocation = GetActorLocation();

    const auto Distance = (PlayerLocation - AICharacterLocation).Size();

    HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance);
}
