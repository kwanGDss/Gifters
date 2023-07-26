// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSceneGameMode.h"
#include "MainScenePlayerController.h"

AMainSceneGameMode::AMainSceneGameMode()
{
	PlayerControllerClass = AMainScenePlayerController::StaticClass();
	DefaultPawnClass = nullptr;
}
