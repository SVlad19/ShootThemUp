// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBasePickup : public AActor
{
    GENERATED_BODY()

  public:
    ASTUBasePickup();

    bool CouldBeTaken()const;

  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pickup")
    TObjectPtr<USphereComponent> CollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    float RespawnTime = 5.f;

    virtual void BeginPlay() override;
    virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;

  public:
    virtual void Tick(float DeltaTime) override;

  private:
    float RotationYaw = 0.f;
    FTimerHandle RespawnTimerHandle;

    virtual bool GivePickupTo(APawn *PlayerPawn);
    void PickapWasTaken();
    void Respawn();
    void GenerateRotationYaw();
};
