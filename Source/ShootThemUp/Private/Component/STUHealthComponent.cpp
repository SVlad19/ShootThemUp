// Shoot Them Up Game. All Rights Reserved.

#include "Component/STUHealthComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Perception/AISense_Damage.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "STUGameModeBase.h"
#include "TimerManager.h"

USTUHealthComponent::USTUHealthComponent()
{

    PrimaryComponentTick.bCanEverTick = false;
}

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0);
    SetHealth(MaxHealth);

    AActor *ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakePointDamage.AddDynamic(this, &USTUHealthComponent::OnTakePointDamage);
        ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &USTUHealthComponent::OnTakeRadialDamage);
    }
}

void USTUHealthComponent::HealUpdate()
{
    SetHealth(Health + HealModifier);

    if (FMath::IsNearlyEqual(Health, MaxHealth))
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimer);
    }
}

void USTUHealthComponent::OnTakePointDamage(AActor *DamagedActor, float Damage, AController *InstigatedBy,
                                            FVector HitLocation, UPrimitiveComponent *FHitComponent, FName BoneName,
                                            FVector ShotFromDirection, const UDamageType *DamageType,
                                            AActor *DamageCauser)
{
    const auto FinalDamage = Damage * GetPointDamageModifier(DamagedActor, BoneName);

    ApplyDamage(FinalDamage, InstigatedBy);
}

void USTUHealthComponent::OnTakeRadialDamage(AActor *DamagedActor, float Damage, const UDamageType *DamageType,
                                             FVector Origin, const FHitResult &HitInfo, AController *InstigatedBy,
                                             AActor *DamageCauser)
{
    ApplyDamage(Damage, InstigatedBy);
}

void USTUHealthComponent::ApplyDamage(float Damage, AController *InstigatedBy)
{
    if (Damage <= 0.f || IsDead())
    {
        return;
    }

    SetHealth(Health - Damage);
    GetWorld()->GetTimerManager().ClearTimer(HealTimer);

    if (IsDead())
    {
        Killed(InstigatedBy);
        OnDeath.Broadcast();
    }
    else if (AutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimer, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true,
                                               HealDelay);
    }

    PlayCameraShake();
    ReportDamageEvent(Damage, InstigatedBy);

    // if (DamageType)
    //{
    //     if (DamageType->IsA<USTUFireDamageType>())
    //     {
    //         UE_LOG(LogTemp, Display,TEXT("FireDamage"));
    //     }
    //     else if (DamageType->IsA<USTUIceDamageType>())
    //     {
    //         UE_LOG(LogTemp, Display, TEXT("IceDamage"));
    //     }
    // }
}

float USTUHealthComponent::GetPointDamageModifier(AActor *DamagedActor, const FName &BoneName)
{
    const auto Character = Cast<ACharacter>(DamagedActor);
    if (!Character)
    {
        return 1.f;
    }

    const auto PhysicalMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
    if (!DamageModifiers.Contains(PhysicalMaterial))
    {
        return 1.f;
    }

    return DamageModifiers[PhysicalMaterial];
}

void USTUHealthComponent::ReportDamageEvent(float Damage, AController *InstigatedBy)
{
    if (!InstigatedBy)
    {
        return;
    }

    UAISense_Damage::ReportDamageEvent(GetWorld(), GetOwner(), InstigatedBy->GetPawn(), Damage,
                                       InstigatedBy->GetPawn()->GetActorLocation(), GetOwner()->GetActorLocation());
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
    const auto NextHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
    const auto HealthDelta = NextHealth - Health;

    Health = NextHealth;
    OnHealthChanged.Broadcast(Health, HealthDelta);
}

void USTUHealthComponent::Killed(AController *KillerController)
{
    const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GameMode)
    {
        return;
    }

    const auto Player = Cast<APawn>(GetOwner());
    const auto VictimController = Player ? Player->Controller : nullptr;

    GameMode->Killed(KillerController, VictimController);
}

void USTUHealthComponent::PlayCameraShake()
{
    if (IsDead())
    {
        return;
    }

    const auto Player = Cast<APawn>(GetOwner());
    if (!Player)
    {
        return;
    }

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager)
    {
        return;
    }

    Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

bool USTUHealthComponent::TryToAddHP(float HPAmount)
{
    if (Health == MaxHealth || IsDead())
    {
        return false;
    }

    SetHealth(Health + HPAmount);

    return true;
}
