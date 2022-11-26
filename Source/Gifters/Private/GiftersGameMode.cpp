// Copyright Epic Games, Inc. All Rights Reserved.

#include "GiftersGameMode.h"
#include "MyGiftersCharacter.h"
#include "MyPlayerController.h"

AGiftersGameMode::AGiftersGameMode()
{
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = AMyGiftersCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	//static ConstructorHelpers::FClassFinder<APawn> BP_Drongo(TEXT("/Game/ParagonDrongo/Characters/Heroes/Drongo/DrongoPlayerCharacter"));
	//if(BP_Drongo.Succeeded())
	//{
	//	DefaultPawnClass = BP_Drongo.Class;
	//}
}
