// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "STULevelItemWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;

UCLASS()
class SHOOTTHEMUP_API USTULevelItemWidget : public UUserWidget
{
    GENERATED_BODY()

  public:
    void SetLevelData(const FLevelData &Data);

    FORCEINLINE FLevelData GetLevelData() const
    {
        return LevelData;
    }

    void SetSelected(bool IsSelected);

    FOnLevelSelectedSignature OnLevelSelected;

  protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> LevelSelectButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> LevelNameTextBlock;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> LevelImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> FrameImage;

  private:
    UFUNCTION()
    void OnLevelButtonClicked();

    UFUNCTION()
    void OnLevelButtonHovered();  
    
    UFUNCTION()
    void OnLevelButtonUnHovered();

    FLevelData LevelData;
};
