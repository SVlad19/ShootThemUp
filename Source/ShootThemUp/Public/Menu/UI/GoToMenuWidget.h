// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GoToMenuWidget.generated.h"

class UButton;

UCLASS()
class SHOOTTHEMUP_API UGoToMenuWidget : public UUserWidget
{
	GENERATED_BODY()

  protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> MenuGameButton;

  private:

    UFUNCTION()
    void OnGoToMenu();
};
