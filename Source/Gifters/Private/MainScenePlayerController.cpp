// Fill out your copyright notice in the Description page of Project Settings.


#include "MainScenePlayerController.h"
#include "EngineUtils.h"
#include "MainSceneWidget.h"
#include "Camera/CameraActor.h"

AMainScenePlayerController::AMainScenePlayerController()
{
	static ConstructorHelpers::FClassFinder<UMainSceneWidget> HUD_MainSceneHUD(TEXT("/Game/HUDs/MainSceneHUD"));
	if (HUD_MainSceneHUD.Succeeded())
	{
		MainSceneHUDClass = HUD_MainSceneHUD.Class;
	}
}

void AMainScenePlayerController::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ACameraActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		SetViewTargetWithBlend(*ActorItr);
		break;
	}

	FloatMainSceneHUD();

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	SetInputMode(FInputModeGameAndUI());
}

void AMainScenePlayerController::FloatMainSceneHUD()
{
	if (IsValid(MainSceneHUDClass))
	{
		MainSceneHUD = Cast<UMainSceneWidget>(CreateWidget(GetWorld(), MainSceneHUDClass));

		if (IsValid(MainSceneHUD))
		{
			MainSceneHUD->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("MainSceneHUD.AddToViewport()"));
		}
	}
}
