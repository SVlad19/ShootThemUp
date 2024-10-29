// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "STUPlayerState.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerState : public APlayerState
{
    GENERATED_BODY()

  public:
    FORCEINLINE void SetTeamID(int32 ID)
    {
        TeamID = ID;
    }

    FORCEINLINE int32 GetTeamID() const
    {
        return TeamID;
    }

    FORCEINLINE void SetTeamColor(const FLinearColor &Color)
    {
        TeamColor = Color;
    }

    FORCEINLINE FLinearColor GetTeamColor() const
    {
        return TeamColor;
    }

    FORCEINLINE void AddKill()
    {
        ++KillsNum;
    }

    FORCEINLINE int32 GetKillsNum()
    {
        return KillsNum;
    }

    FORCEINLINE int32 GetDeathsNum()
    {
        return DeathsNum;
    }

    FORCEINLINE void AddDeath()
    {
        ++DeathsNum;
    }

    void LogInfo();

  private:
    int32 TeamID;
    FLinearColor TeamColor;

    int32 KillsNum = 0;
    int32 DeathsNum = 0;
};
