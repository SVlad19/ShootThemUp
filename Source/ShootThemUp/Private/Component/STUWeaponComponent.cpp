// Shoot Them Up Game. All Rights Reserved.

#include "Component/STUWeaponComponent.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"

USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::Fire(const FInputActionValue &Value)
{
    const auto bIsFire = Value.Get<bool>();

    if (!CurrentWeapon)
    {
        return;
    }

    if (bIsFire)
    {
        StartFire();
    }
    else
    {
        StopFire();
    }
}

void USTUWeaponComponent::StartFire()
{
    if (!CanFire())
    {
        return;
    }

    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
    CurrentWeapon->StopFire();
}

void USTUWeaponComponent::SwapWeapon(const FInputActionValue &Value)
{
    if (!CanEquip())
    {
        return;
    }
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::Reload(const FInputActionValue &Value)
{
    ChangeClip();
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
    InitAnimations();
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    CurrentWeapon = nullptr;
    for (auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();
}

void USTUWeaponComponent::SpawnWeapons()
{
    const auto Owner = Cast<ACharacter>(GetOwner());

    if (!GetWorld() || !Owner)
    {
        return;
    }

    for (auto OneWeaponData : WeaponData)
    {
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
        if (!Weapon)
        {
            continue;
        }

        Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
        Weapon->SetOwner(GetOwner());
        Weapons.Add(Weapon);

        AttachWeaponToSocket(Weapon, Owner->GetMesh(), WeaponArmorySocketName);
    }
}

void USTUWeaponComponent::AttachWeaponToSocket(TObjectPtr<ASTUBaseWeapon> Weapon, USceneComponent *SceneComponent,
                                               const FName &SocketName)
{
    if (!Weapon || !SceneComponent)
    {
        return;
    }
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
    {
        return;
    }

    const auto Owner = Cast<ACharacter>(GetOwner());
    if (!Owner)
    {
        return;
    }

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        AttachWeaponToSocket(CurrentWeapon, Owner->GetMesh(), WeaponArmorySocketName);
    }

    CurrentWeapon = Weapons[WeaponIndex];

    const auto CurrentWeaponData = WeaponData.FindByPredicate(
        [&](const FWeaponData &Data) { return Data.WeaponClass == CurrentWeapon->GetClass(); });

    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

    AttachWeaponToSocket(CurrentWeapon, Owner->GetMesh(), WeaponEquipSocketName);
    EquipAnimInProgress = true;
    PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::PlayAnimMontage(TObjectPtr<UAnimMontage> Animation)
{
    const auto Owner = Cast<ACharacter>(GetOwner());

    if (!Owner)
    {
        return;
    }

    Owner->PlayAnimMontage(Animation);
}

void USTUWeaponComponent::InitAnimations()
{
    auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);

    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    }
    else
    {
        checkNoEntry();
    }

    for (auto OneWeaponData : WeaponData)
    {
        auto ReloadFinishedNotify =
            AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);

        if (!ReloadFinishedNotify)
        {
            checkNoEntry();
        }

         ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);     
    }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent *MeshComp)
{
    const auto Owner = Cast<ACharacter>(GetOwner());

    if (!Owner || Owner->GetMesh() != MeshComp)
    {
        return;
    }

    EquipAnimInProgress = false;
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent *MeshComp)
{
    const auto Owner = Cast<ACharacter>(GetOwner());

    if (!Owner || Owner->GetMesh() != MeshComp)
    {
        return;
    }

    ReloadAnimInProgress = false;
}

void USTUWeaponComponent::OnEmptyClip()
{
    ChangeClip();
}

void USTUWeaponComponent::ChangeClip()
{
    if (!CanReload())
    {
        return;
    }

    StopFire();
    CurrentWeapon->ChangeClip();

    ReloadAnimInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);
}
