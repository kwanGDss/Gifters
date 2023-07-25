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
}

void UGameOverWidget::OnRestartButtonClicked()
{
    // get the current level name
    FString currentLevelName = UGameplayStatics::GetCurrentLevelName(this, true);

    // open the current level (i.e., restart the level)
    UGameplayStatics::OpenLevel(this, FName(*currentLevelName));
}

void UGameOverWidget::BindCharacterStat(UGiftersStatComponent* NewCharacterStat)
{
    CurrentCharacterStat = NewCharacterStat;
}