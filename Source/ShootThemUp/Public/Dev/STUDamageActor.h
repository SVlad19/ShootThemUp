// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUDamageActor.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUDamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTUDamageActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USceneComponent *SceneComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Radius = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FColor SphereColor = FColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool DoFullDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UDamageType> DamageType;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
