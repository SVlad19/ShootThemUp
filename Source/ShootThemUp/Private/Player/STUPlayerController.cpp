// Shoot Them Up Game. All Rights Reserved.

#include "Player/STUPlayerController.h"
#include "Component/STURespawnComponent.h"
#include "Config/InputDataConfig.h"
#include "EnhancedInputComponent.h"
#include "STUGameModeBase.h"

ASTUPlayerController::ASTUPlayerController()
{
    STURespawnComponent = CreateDefaultSubobject<USTURespawnComponent>(TEXT("STURespawnComponent"));
}

void ASTUPlayerController::OnPauseGame(const FInputActionValue &Value)
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode())
    {
        return;
    }

    GetWorld()->GetAuthGameMode()->SetPause(this);
}

void ASTUPlayerController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);

    OnNewPawn.Broadcast(InPawn);
}

void ASTUPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (GetWorld())
    {
        if (const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()))
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ASTUPlayerController::OnMatchStateChanged);
        }
    }
}

void ASTUPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent *Input = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (!Input)
        {
            return;
        }

        Input->BindAction(InputActions->PauseGameInput, ETriggerEvent::Triggered, this,
                          &ASTUPlayerController::OnPauseGame);
    }
}

void ASTUPlayerController::OnMatchStateChanged(ESTUMatchState State)
{
    if (State == ESTUMatchState::InProgress)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
}
