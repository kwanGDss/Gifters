// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "GiftersStatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void UMyUserWidget::BindCharacterStat(UGiftersStatComponent* NewCharacterStat)
{
	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &UMyUserWidget::UpdateHPWidget);
	NewCharacterStat->OnSPChanged.AddUObject(this, &UMyUserWidget::UpdateSPWidget);
	NewCharacterStat->OnPoseChanged.AddUObject(this, &UMyUserWidget::UpdatePose);
}

void UMyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));
	SPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("StaminaBar")));
	Crosshair = Cast<UImage>(GetWidgetFromName(TEXT("Crosshairs")));
	//Minimap = Cast<UImage>(GetWidgetFromName(TEXT("Minimap")));

	UpdateHPWidget();
	UpdateSPWidget();
	UpdatePose();
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

void UMyUserWidget::UpdatePose()
{
	if (IsValid(CurrentCharacterStat))
	{

			if (CurrentCharacterStat->GetPose() == true)
			{
				Crosshair->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				Crosshair->SetVisibility(ESlateVisibility::Hidden);
			}
		
	}
}

void UMyUserWidget::BrushRenderTarget(UTextureRenderTarget2D* RenderTarget)
{

}
