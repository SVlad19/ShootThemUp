// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "UI/STUBaseWidget.h"
#include "STUPlayerHUDWidget.generated.h"

class UProgressBar;

UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public USTUBaseWidget
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetWeaponUIData(FWeaponUIData &UIData) const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetWeaponAmmoData(FAmmoData &AmmoData) const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerAlive() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerSpectating() const;

    UFUNCTION(BlueprintImplementableEvent, Category = "UI")
    void OnTakeDamage();

    UFUNCTION(BlueprintCallable, Category = "UI")
    int32 GetKillsNum() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    FString FormatBullets(int32 BulletsNum) const;

    virtual void NativeOnInitialized() override;

  protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> HealthProgressBar;

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    TObjectPtr<UWidgetAnimation> DamageAnimation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float PercentColorThreshold = 0.3f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor GoodColor = FLinearColor::White;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor BadColor = FLinearColor::Red;

  private:
    void OnHealthChanged(float Health, float HealthDelta);
    void OnNewPawn(APawn *Pawn);
    void UpdateHealthBar();
};
