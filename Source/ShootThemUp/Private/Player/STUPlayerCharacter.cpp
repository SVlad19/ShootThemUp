// Shoot Them Up Game. All Rights Reserved.

#include "Player/STUPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Config/InputDataConfig.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Component/STUHealthComponent.h"
#include "Component/STUWeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer &ObjInit) : Super(ObjInit)
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
    CameraCollisionComponent->SetupAttachment(CameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

}

void ASTUPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(CameraCollisionComponent);

    CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this,
                                                                 &ASTUPlayerCharacter::OnCameraCollisionBeginOverlap);
    
    CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this,
                                                                 &ASTUPlayerCharacter::OnCameraCollisionEndOverlap);
   
    if (APlayerController *PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(InputMapping, 0);
        }
    }
}

void ASTUPlayerCharacter::OnDeath()
{
    Super::OnDeath();

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent *Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (!InputActions)
        {
            return;
        }

        Input->BindAction(InputActions->MoveInput, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::Move);
        Input->BindAction(InputActions->LookInput, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::Look);
        Input->BindAction(InputActions->JumpInput, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::Jumping);
        Input->BindAction(InputActions->SprintInput, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::Sprint);
        Input->BindAction(InputActions->FireInput, ETriggerEvent::Triggered, WeaponComponent.Get(),
                          &USTUWeaponComponent::Fire);
        Input->BindAction(InputActions->SwapInput, ETriggerEvent::Triggered, WeaponComponent.Get(),
                          &USTUWeaponComponent::SwapWeapon);
        Input->BindAction(InputActions->ReloadInput, ETriggerEvent::Triggered, WeaponComponent.Get(),
                          &USTUWeaponComponent::Reload);   
        Input->BindAction(InputActions->ZoomInput, ETriggerEvent::Triggered, WeaponComponent.Get(),
                          &USTUWeaponComponent::ZoomInput);  
    }
}

bool ASTUPlayerCharacter::IsRunning() const
{
    return IsSprinting && IsMovingForward && !GetVelocity().IsZero();
}

void ASTUPlayerCharacter::Move(const FInputActionValue &Value)
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

void ASTUPlayerCharacter::Look(const FInputActionValue &Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();

    AddControllerPitchInput(LookAxisVector.Y);
    AddControllerYawInput(LookAxisVector.X);
}

void ASTUPlayerCharacter::Jumping(const FInputActionValue &Value)
{
    Jump();
}

void ASTUPlayerCharacter::Sprint(const FInputActionValue &Value)
{
    IsSprinting = Value.Get<bool>();
}

void ASTUPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
                                                        UPrimitiveComponent *OtherComp, int32 OtherBodyIndex,
                                                        bool bFromSweep, const FHitResult &SweepResult)
{
    CheckCameraOverlap();
}

void ASTUPlayerCharacter::OnCameraCollisionEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
                                                      UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    CheckCameraOverlap();
}

void ASTUPlayerCharacter::CheckCameraOverlap()
{
    const auto HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
    GetMesh()->SetOwnerNoSee(HideMesh);

    TArray<USceneComponent *> MeshChildren;
    GetMesh()->GetChildrenComponents(true, MeshChildren);

    for (auto MeshChild : MeshChildren)
    {
        const auto MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild);
        if (MeshChildGeometry)
        {
            MeshChildGeometry->SetOwnerNoSee(HideMesh);
        }
    }
}
