// Shoot Them Up Game. All Rights Reserved.

#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Component/STUCharacterMovementComponent.h"
#include "Config/InputDataConfig.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/STUPlayerController.h"

ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer &ObjInit)
    : Super(
          ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
}

void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController *PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(InputMapping, 0);
        }
    }
}

void ASTUBaseCharacter::Move(const FInputActionValue &Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();
    // 1.
    // AddMovementInput(GetActorForwardVector(), MovementVector.X);
    // AddMovementInput(GetActorRightVector(), MovementVector.Y);

    // 2.

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    IsMovingForward = MovementVector.X > 0.f;
    AddMovementInput(ForwardDirection, MovementVector.X);

    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(RightDirection, MovementVector.Y);
}

void ASTUBaseCharacter::Look(const FInputActionValue &Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();

    AddControllerPitchInput(LookAxisVector.Y);
    AddControllerYawInput(LookAxisVector.X);
}

void ASTUBaseCharacter::Jumping(const FInputActionValue &Value)
{
    Jump();
}

void ASTUBaseCharacter::Sprint(const FInputActionValue &Value)
{

    IsSprinting = Value.Get<bool>();
}

bool ASTUBaseCharacter::IsRunning() const
{
    return !GetVelocity().IsZero() && IsSprinting && IsMovingForward;
}

float ASTUBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero())
    {
        return 0.f;
    }

    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);

    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent *Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (!InputActions)
        {
            return;
        }

        Input->BindAction(InputActions->MoveInput, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::Move);
        Input->BindAction(InputActions->LookInput, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::Look);
        Input->BindAction(InputActions->JumpInput, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::Jumping);
        Input->BindAction(InputActions->SprintInput, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::Sprint);
    }
}