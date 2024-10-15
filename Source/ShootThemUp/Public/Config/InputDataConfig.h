// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputDataConfig.generated.h"

class UInputAction;

UCLASS()
class SHOOTTHEMUP_API UInputDataConfig : public UDataAsset
{
    GENERATED_BODY()

  public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction *MoveInput;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction *LookInput;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction *JumpInput;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction *SprintInput;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction *FireInput;   
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction *SwapInput;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction *ReloadInput;
};
