// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UPauseWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UButton* ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("ResumeButton")));
    if (ResumeButton)
    {
        ResumeButton->OnClicked.AddDynamic(this, &UPauseWidget::OnResumeButtonClicked);
    }

    UButton* RestartButton = Cast<UButton>(GetWidgetFromName(TEXT("RestartButton")));
    if (RestartButton)
    {
        RestartButton->OnClicked.AddDynamic(this, &UPauseWidget::OnRestartButtonClicked);
    }

    UButton* ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("ExitButton")));
    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UPauseWidget::OnExitButtonClicked);
    }
}

void UPauseWidget::OnResumeButtonClicked()
{
    APlayerController* MyPlayerController = GetWorld()->GetFirstPlayerController();

    MyPlayerController->SetPause(false);
    SetVisibility(ESlateVisibility::Collapsed);

    MyPlayerController->bShowMouseCursor = false;
    MyPlayerController->bEnableClickEvents = false;
    MyPlayerController->bEnableMouseOverEvents = false;

    MyPlayerController->SetInputMode(FInputModeGameOnly());
}

void UPauseWidget::OnRestartButtonClicked()
{
    // get the current level name
    FString currentLevelName = UGameplayStatics::GetCurrentLevelName(this, true);

    // open the current level (i.e., restart the level)
    UGameplayStatics::OpenLevel(this, FName(*currentLevelName));
}

void UPauseWidget::OnExitButtonClicked()
{
    FGenericPlatformMisc::RequestExit(false);
}