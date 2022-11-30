// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "GiftersStatComponent.h"
#include "Components/ProgressBar.h"

void UMyUserWidget::BindCharacterStat(UGiftersStatComponent* NewCharacterStat)
{
	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &UMyUserWidget::UpdateHPWidget);
	NewCharacterStat->OnSPChanged.AddUObject(this, &UMyUserWidget::UpdateSPWidget);
}

void UMyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));
	SPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("StaminaBar")));
	UpdateHPWidget();
	UpdateSPWidget();
}

void UMyUserWidget::UpdateHPWidget()
{
	if (IsValid(CurrentCharacterStat))
	{
		if (IsValid(HPProgressBar))
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHP()/100.0f);
		}
	}
}

void UMyUserWidget::UpdateSPWidget()
{
	if (IsValid(CurrentCharacterStat))
	{
		if (IsValid(SPProgressBar))
		{
			SPProgressBar->SetPercent(CurrentCharacterStat->GetSP() / 100.0f);
		}
	}
}
