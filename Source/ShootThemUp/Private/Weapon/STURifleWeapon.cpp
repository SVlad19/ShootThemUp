// Shoot Them Up Game. All Rights Reserved.


#include "Weapon/STURifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Engine/DamageEvents.h"

void ASTURifleWeapon::StartFire()
{
    GetWorld()->GetTimerManager().SetTimer(ShootTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
    MakeShot();
}

void ASTURifleWeapon::StopFire()
{
    GetWorld()->GetTimerManager().ClearTimer(ShootTimerHandle);
}

void ASTURifleWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty())
    {
        StopFire();
        return;
    }

    FVector TraceStart;
    FVector TraceEnd;

    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopFire();
        return;
    }

    FHitResult HitResult;

    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit)
    {
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 24.f, FColor::Red, false, 5.f);
        MakeDamage(HitResult);
    }

    DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraceData(FVector &TraceStart, FVector &TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;

    if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
    {
        return false;
    }

    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    TraceStart = ViewLocation; // SocketTransform.GetLocation();
    TraceEnd = TraceStart + ShootDirection * TraceDistance;
    return true;
}

void ASTURifleWeapon::MakeDamage(const FHitResult &HitResult)
{
    const auto Enemy = HitResult.GetActor();
    if (!Enemy)
    {
        return;
    }

    Enemy->TakeDamage(Damage, FDamageEvent{}, GetPlayerController(), this);
}
