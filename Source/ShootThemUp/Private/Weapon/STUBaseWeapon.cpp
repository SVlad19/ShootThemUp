// Shoot Them Up Game. All Rights Reserved.

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"


ASTUBaseWeapon::ASTUBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
    SetRootComponent(WeaponMeshComponent);
}

void ASTUBaseWeapon::StopFire()
{
}

void ASTUBaseWeapon::StartFire()
{
}

void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMeshComponent);
    checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn`t be less or equal zero"));
    checkf(DefaultAmmo.Clips > 0, TEXT("Clips count couldn`t be less or equal zero"));
    CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::MakeShot()
{ 
}

APlayerController *ASTUBaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player)
    {
        return nullptr;
    }

    return Player->GetController<APlayerController>();
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector &ViewLocation, FRotator &ViewRotation) const
{
    APlayerController *Controller = GetPlayerController();

    if (!Controller)
    {
        return false;
    }

    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

    return true;
}

FVector ASTUBaseWeapon::GetMuzzleLocation() const
{
    return WeaponMeshComponent->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector &TraceStart, FVector &TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;

    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
    {
        return false;
    }

    const FVector ShootDirection =ViewRotation.Vector();
    TraceStart = ViewLocation; // SocketTransform.GetLocation();
    TraceEnd = TraceStart + ShootDirection * TraceDistance;
    return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult &HitResult, const FVector &TraceStart, const FVector &TraceEnd)
{

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    DrawDebugLine(GetWorld(), GetMuzzleLocation(), TraceEnd, FColor::Red, false, 3.f, 0, 3.f);
    GetWorld()->LineTraceSingleByChannel(HitResult, GetMuzzleLocation(), TraceEnd, ECollisionChannel::ECC_Visibility,
                                         CollisionParams);
}

void ASTUBaseWeapon::DecreaseAmmo()
{
    if (CurrentAmmo.Bullets == 0)
    {
        return;
    }

    CurrentAmmo.Bullets--;

    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        StopFire();
        OnClipEmpty.Broadcast();
    }
}

void ASTUBaseWeapon::ChangeClip()
{
    if (!CurrentAmmo.Infinite)
    {
        if (CurrentAmmo.Clips == 0)
        {
            return;
        }

        CurrentAmmo.Clips--;
    }

    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
}

void ASTUBaseWeapon::LogAmmo()
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(LogTemp, Display, TEXT("%s"), *AmmoInfo);
}