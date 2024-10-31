// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STUCoreTypes.h"
#include "STUGameInstance.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance
{
    GENERATED_BODY()

  public:
    FORCEINLINE FLevelData GetStartupLevel()
    {
        return StartupLevel;
    }

    FORCEINLINE void SetStartupLevel(const FLevelData &LevelData)
    {
        StartupLevel = LevelData;
    }

    FORCEINLINE FName GetMenuLevelName()
    {
        return MenuLevelName;
    }

    FORCEINLINE const TArray<FLevelData> &GetLevelsData() const
    {
        return LevelsData;
    }

  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameInstance", meta = (ToolTip = "Level names must be unique"))
    TArray<FLevelData> LevelsData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameInstance")
    FName MenuLevelName = NAME_None;

  private:
    FLevelData StartupLevel;
};
