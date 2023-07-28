// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GiftersStatComponent.h"

void UGameOverWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UButton* RestartButton = Cast<UButton>(GetWidgetFromName(TEXT("RestartButton")));
    if (RestartButton)
    {
        RestartButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnRestartButtonClicked);
    }

    UButton* ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("ExitButton")));
    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnExitButtonClicked);
    }
}

void UGameOverWidget::OnRestartButtonClicked()
{
    // get the current level name
    FString currentLevelName = UGameplayStatics::GetCurrentLevelName(this, true);

    // open the current level (i.e., restart the level)
    UGameplayStatics::OpenLevel(this, FName(*currentLevelName));
}

void UGameOverWidget::OnExitButtonClicked()
{
    FGenericPlatformMisc::RequestExit(false);
}

void UGameOverWidget::BindCharacterStat(UGiftersStatComponent* NewCharacterStat)
{
    CurrentCharacterStat = NewCharacterStat;
}