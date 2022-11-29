// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyUserWidget.h"
#include "MyGiftersCharacter.h"

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

void AMyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PlayerCharacter = Cast<AMyGiftersCharacter>(InPawn);
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FloatPlayerHUD();
	PlayerHUD->BindCharacterStat(PlayerCharacter->GetCharacterStat());
}

void AMyPlayerController::FloatPlayerHUD()
{
	if (IsValid(PlayerHUDClass))
	{
		PlayerHUD = Cast<UMyUserWidget>(CreateWidget(GetWorld(), PlayerHUDClass));

		if (IsValid(PlayerHUD))
		{
			PlayerHUD->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("AddToViewport"));
		}
	}
}