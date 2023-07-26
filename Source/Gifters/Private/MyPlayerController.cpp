// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyUserWidget.h"
#include "MyGiftersCharacter.h"
#include "GameOverWidget.h"
#include "PauseWidget.h"

AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UMyUserWidget> HUD_PlayerHUD(TEXT("/Game/HUDs/PlayerHUD"));
	if (HUD_PlayerHUD.Succeeded())
	{
		PlayerHUDClass = HUD_PlayerHUD.Class;
	}

	static ConstructorHelpers::FClassFinder<UGameOverWidget> HUD_GameOverHUD(TEXT("/Game/HUDs/GameOverHUD"));
	if (HUD_GameOverHUD.Succeeded())
	{
		GameOverHUDClass = HUD_GameOverHUD.Class;
	}

	static ConstructorHelpers::FClassFinder<UPauseWidget> HUD_PauseHUD(TEXT("/Game/HUDs/PauseHUD"));
	if (HUD_PauseHUD.Succeeded())
	{
		PauseHUDClass = HUD_PauseHUD.Class;
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
	PlayerCharacter->OnSelfDeadDelegate.AddDynamic(this, &AMyPlayerController::FloatGameOverHUD);
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FloatPlayerHUD();
	FloatPauseHUD();

	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;

	SetInputMode(FInputModeGameOnly());
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseGame", IE_Pressed, this, &AMyPlayerController::TogglePause);
}

void AMyPlayerController::FloatPlayerHUD()
{
	if (IsValid(PlayerHUDClass))
	{
		PlayerHUD = Cast<UMyUserWidget>(CreateWidget(GetWorld(), PlayerHUDClass));
		PlayerHUD->BindCharacterStat(PlayerCharacter->GetCharacterStat());

		if (IsValid(PlayerHUD))
		{
			PlayerHUD->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("PlayerHUD.AddToViewport()"));
		}
	}
}

void AMyPlayerController::FloatGameOverHUD()
{
	if (IsValid(GameOverHUDClass))
	{
		GameOverHUD = Cast<UGameOverWidget>(CreateWidget(GetWorld(), GameOverHUDClass));
		GameOverHUD->BindCharacterStat(PlayerCharacter->GetCharacterStat());
		UE_LOG(LogTemp, Warning, TEXT("GameOverHUD.CreateWidget()"));

		if (IsValid(GameOverHUD))
		{
			GameOverHUD->AddToViewport();

			bShowMouseCursor = true;
			bEnableClickEvents = true;
			bEnableMouseOverEvents = true;

			SetInputMode(FInputModeGameAndUI());
			//AWorldSettings* WorldSettings = GetWorld()->GetWorldSettings();
			//WorldSettings->SetTimeDilation(0.f);
			UE_LOG(LogTemp, Warning, TEXT("GameOverHUD.AddToViewport()"));
		}
	}
}

void AMyPlayerController::FloatPauseHUD()
{
	if (IsValid(PauseHUDClass))
	{
		PauseHUD = Cast<UPauseWidget>(CreateWidget(GetWorld(), PauseHUDClass));

		if (IsValid(PauseHUD))
		{
			PauseHUD->AddToViewport();
			PauseHUD->SetVisibility(ESlateVisibility::Collapsed);
			UE_LOG(LogTemp, Warning, TEXT("PauseHUD.AddToViewport()"));
		}
	}
}

void AMyPlayerController::TogglePause()
{
	UE_LOG(LogTemp, Warning, TEXT("TogglePause() : %hs"), IsPaused() ? "True" : "False");
	if (IsPaused())
	{
		// If the game is paused, unpause the game and hide the pause menu
		SetPause(false);
		PauseHUD->SetVisibility(ESlateVisibility::Collapsed);

		bShowMouseCursor = false;
		bEnableClickEvents = false;
		bEnableMouseOverEvents = false;

		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		// If the game is not paused, pause the game and show the pause menu
		SetPause(true);
		PauseHUD->SetVisibility(ESlateVisibility::Visible);

		bShowMouseCursor = true;
		bEnableClickEvents = true;
		bEnableMouseOverEvents = true;

		SetInputMode(FInputModeGameAndUI());
	}
}
