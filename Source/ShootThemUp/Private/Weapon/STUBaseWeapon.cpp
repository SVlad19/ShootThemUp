// Shoot Them Up Game. All Rights Reserved.

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

ASTUBaseWeapon::ASTUBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
    SetRootComponent(WeaponMeshComponent);
}

void ASTUBaseWeapon::StartFire()
{
}

void ASTUBaseWeapon::StopFire()
{
}

void ASTUBaseWeapon::Zoom(bool Enabled)
{
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
    if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0)
    {
        return false;
    }

    if (IsAmmoEmpty())
    {
        CurrentAmmo.Clips = FMath::Clamp(CurrentAmmo.Clips + ClipsAmount, 0, DefaultAmmo.Clips + 1);
        OnClipEmpty.Broadcast(this);
    }
    else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
    {
        const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount;
        if (DefaultAmmo.Clips - NextClipsAmount >= 0)
        {
            CurrentAmmo.Clips = NextClipsAmount;
        }
        else
        {
            CurrentAmmo.Clips = DefaultAmmo.Clips;
            CurrentAmmo.Bullets = DefaultAmmo.Bullets;
        }
    }
    else
    {
        CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    }

    return true;
}

void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMeshComponent);
    checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn`t be less or equal zero"));
    checkf(DefaultAmmo.Clips > 0, TEXT("Clips count couldn`t be less or equal zero"));
    CurrentAmmo = DefaultAmmo;
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector &ViewLocation, FRotator &ViewRotation) const
{
    const auto STUCharacter = Cast<ACharacter>(GetOwner());
    if (!STUCharacter)
    {
        return false;
    }

    if (STUCharacter->IsPlayerControlled())
    {
        APlayerController *Controller = STUCharacter->GetController<APlayerController>();

        if (!Controller)
        {
            return false;
        }

        Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    }
    else
    {
        ViewLocation = GetMuzzleLocation();
        ViewRotation = WeaponMeshComponent->GetSocketRotation(MuzzleSocketName);
    }

    return true;
}

FVector ASTUBaseWeapon::GetMuzzleLocation() const
{
    return WeaponMeshComponent->GetSocketLocation(MuzzleSocketName);
}

void ASTUBaseWeapon::MakeShot()
{
}

bool ASTUBaseWeapon::GetTraceData(FVector &TraceStart, FVector &TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;

    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
    {
        return false;
    }

    const FVector ShootDirection = ViewRotation.Vector();
    TraceStart = ViewLocation; // SocketTransform.GetLocation();
    TraceEnd = TraceStart + ShootDirection * TraceDistance;
    return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult &HitResult, const FVector &TraceStart, const FVector &TraceEnd)
{

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.bReturnPhysicalMaterial = true;

    // DrawDebugLine(GetWorld(), GetMuzzleLocation(), TraceEnd, FColor::Red, false, 3.f, 0, 3.f);
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
        OnClipEmpty.Broadcast(this);
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

UNiagaraComponent *ASTUBaseWeapon::SpawnMuzzleFX()
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX, WeaponMeshComponent, MuzzleSocketName,
                                                        FVector::ZeroVector, FRotator::ZeroRotator,
                                                        EAttachLocation::SnapToTarget, true);
}
