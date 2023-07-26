// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSceneWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainSceneWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UButton* StartButton = Cast<UButton>(GetWidgetFromName(TEXT("StartButton")));
    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &UMainSceneWidget::OnStartButtonClicked);
    }

    UButton* ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("ExitButton")));
    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UMainSceneWidget::OnExitButtonClicked);
    }
}

void UMainSceneWidget::OnStartButtonClicked()
{
    UGameplayStatics::OpenLevel(this, "Showcase", true, "GameMode=Class'/Script/Gifters.GiftersGameMode'");
}

void UMainSceneWidget::OnExitButtonClicked()
{
    FGenericPlatformMisc::RequestExit(false);
}
