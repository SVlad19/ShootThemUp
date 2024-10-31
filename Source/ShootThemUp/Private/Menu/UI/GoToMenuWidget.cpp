// Shoot Them Up Game. All Rights Reserved.


#include "Menu/UI/GoToMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"

void UGoToMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (MenuGameButton)
    {
        MenuGameButton->OnClicked.AddDynamic(this, &UGoToMenuWidget::OnGoToMenu);
    }
}

void UGoToMenuWidget::OnGoToMenu()
{
    if (!GetWorld())
    {
        return;
    }

    const auto GameInstance = Cast<USTUGameInstance>(GetWorld()->GetGameInstance());

    if (!GameInstance)
    {
        return;
    }

    if (GameInstance->GetMenuLevelName().IsNone())
    {
        return;
    }

    UGameplayStatics::OpenLevel(this, GameInstance->GetMenuLevelName());
}
