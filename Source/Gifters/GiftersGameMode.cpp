// Copyright Epic Games, Inc. All Rights Reserved.

#include "GiftersGameMode.h"
#include "GiftersCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGiftersGameMode::AGiftersGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
