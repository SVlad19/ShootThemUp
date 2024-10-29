// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUCoreTypes.h"
#include "STUGameHUD.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUGameHUD : public AHUD
{
    GENERATED_BODY()

  public:
    virtual void DrawHUD() override;

  protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PauseWidgetClass;

  private:
    void DrawCrossHair();
    void OnMatchStateChanged(ESTUMatchState State);

    UPROPERTY()
    TMap<ESTUMatchState, UUserWidget *> GameWidgets;

    UPROPERTY()
    TWeakObjectPtr<UUserWidget> CurrentWidget;
};
