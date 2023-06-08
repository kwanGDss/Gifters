// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Wolf.h"
#include "GiftersGameMode.generated.h"

UCLASS(minimalapi)
class AGiftersGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGiftersGameMode();

	void AGiftersGameMode::SpawnMonster();

	//virtual void Tick(float DeltaTime) override; // ���� ƽ�� ���� �������̵�

protected:
	virtual void BeginPlay() override;

private:
	TArray<ASpawnPoint*> SpawnPoints;
};