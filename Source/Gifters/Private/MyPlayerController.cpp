// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyUserWidget.h"

AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UMyUserWidget> HUD_PlayerHUD(TEXT("/Game/HUDs/PlayerHUD"));
	if (HUD_PlayerHUD.Succeeded())
	{
		PlayerHUDClass = HUD_PlayerHUD.Class;
	}
}

void AMyPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(PlayerHUDClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("AMyPlayerController::BeginPlay()"));
		PlayerHUD = Cast<UMyUserWidget>(CreateWidget(GetWorld(), PlayerHUDClass));

		if (IsValid(PlayerHUD))
		{
			UE_LOG(LogTemp, Warning, TEXT("AMyPlayerController::AddToViewport()"));
			PlayerHUD->AddToViewport();
		}
	}
}
