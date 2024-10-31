// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUBaseWidget.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUBaseWidget : public UUserWidget
{
    GENERATED_BODY()

  public:
    void Show();

  protected:
    UPROPERTY(meta = (BindWidgetAnim), Transient)
    TObjectPtr<UWidgetAnimation> ShowAnimation;
};
