// Shoot Them Up Game. All Rights Reserved.

#include "Component/STUHealthComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraShakeBase.h"

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
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamageHandle);
    }
}

void USTUHealthComponent::HealUpdate()
{
    SetHealth(Health + HealModifier);

    if (FMath::IsNearlyEqual(Health,MaxHealth))
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimer);
    }
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
    const auto NextHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
    const auto HealthDelta = NextHealth - Health;

    Health = NextHealth;
    OnHealthChanged.Broadcast(Health, HealthDelta);
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

void USTUHealthComponent::OnTakeAnyDamageHandle(AActor *DamagedActor, float Damage, const UDamageType *DamageType,
                                                AController *InstigatedBy, AActor *DamageCauser)
{
    if (Damage <= 0.f || IsDead())
    {
        return;
    }

    SetHealth(Health - Damage);
    GetWorld()->GetTimerManager().ClearTimer(HealTimer);
  
    if (IsDead())
    {
        OnDeath.Broadcast();
    }
    else if (AutoHeal)
    {
            GetWorld()->GetTimerManager().SetTimer(HealTimer, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true,
                                               HealDelay);
    }

    PlayCameraShake();

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

bool USTUHealthComponent::TryToAddHP(float HPAmount)
{
    if (Health == MaxHealth || IsDead())
    {
        return false;
    }

    SetHealth(Health + HPAmount);

    return true;
}
