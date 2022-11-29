// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "GiftersStatComponent.h"
#include "Components/ProgressBar.h"

void UMyUserWidget::BindCharacterStat(UGiftersStatComponent* NewCharacterStat)
{
	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &UMyUserWidget::UpdateHPWidget);
}

void UMyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));
	UpdateHPWidget();
}

void UMyUserWidget::UpdateHPWidget()
{
	if (IsValid(CurrentCharacterStat))
	{
		if (IsValid(HPProgressBar))
		{
			UE_LOG(LogTemp, Warning, TEXT("SetPercent_HP : %f"), CurrentCharacterStat->GetHP() / 100.0f);
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHP()/100.0f);
		}
	}
}