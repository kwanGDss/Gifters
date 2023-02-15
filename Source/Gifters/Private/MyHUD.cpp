// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.h"

AMyHUD::AMyHUD()
{
	static ConstructorHelpers::FClassFinder<UMyUserWidget> HUD_PlayerHUD(TEXT("/Game/HUDs/PlayerHUD"));
	if (HUD_PlayerHUD.Succeeded())
	{
		PlayerHUDClass = HUD_PlayerHUD.Class;
	}
}

void AMyHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(PlayerHUDClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("AMyHUD::PostInitializeComponents()"));
		PlayerHUD = Cast<UMyUserWidget>(CreateWidget(GetWorld(), PlayerHUDClass));
		if (IsValid(PlayerHUD))
		{
			UE_LOG(LogTemp, Warning, TEXT("AMyHUD::DrawHUD()"));
			PlayerHUD->AddToViewport();
		}
	}
}
//void AMyHUD::DrawHUD()
//{
//
//}

void AMyHUD::PostRender()
{
	Super::PostRender();

	if (IsValid(PlayerHUD))
	{
		UE_LOG(LogTemp, Warning, TEXT("AMyHUD::DrawHUD()"));
		PlayerHUD->AddToViewport();
	}
}
