// Copyright Epic Games, Inc. All Rights Reserved.

#include "GiftersGameMode.h"
#include "GiftersCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "MyGiftersCharacter.h"

AGiftersGameMode::AGiftersGameMode()
{
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = AMyGiftersCharacter::StaticClass();

}
