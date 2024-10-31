// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUPlayerStatRowWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class SHOOTTHEMUP_API USTUPlayerStatRowWidget : public UUserWidget
{
    GENERATED_BODY()

    public:
    void SetPlayerName(const FText &Text);
    void SetKills(const FText &Text);
    void SetDeaths(const FText &Text);
    void SetTeam(const FText &Text);
    void SetPlayerIndicatorVisibility(bool Visible);
    void SetTeamColor(const FLinearColor &Color);

  protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> PlayerNameTextBlock;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> KillsTextBlock;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> DeathsTextBlock;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TeamTextBlock;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> PlayerIndicatorImage;

        UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> TeamImage;
};
