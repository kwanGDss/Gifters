// Copyright Epic Games, Inc. All Rights Reserved.

#include "GiftersGameMode.h"
#include "MyGiftersCharacter.h"
#include "MyPlayerController.h"
#include "SpawnPoint.h"
#include "Wolf.h"


AGiftersGameMode::AGiftersGameMode()
{
    DefaultPawnClass = AMyGiftersCharacter::StaticClass();
    PlayerControllerClass = AMyPlayerController::StaticClass();
}

void AGiftersGameMode::BeginPlay()
{
    Super::BeginPlay();

    // 레벨의 모든 ASpawnPoint 인스턴스를 검색
    for (TActorIterator<ASpawnPoint> It(GetWorld()); It; ++It)
    {
        SpawnPoints.Add(*It);
    }

    // 5초마다 SpawnMonster 함수를 호출하는 타이머를 설정
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AGiftersGameMode::SpawnMonster, 5.0f, true);
}

void AGiftersGameMode::SpawnMonster()
{
    if (SpawnPoints.Num() > 0)
    {
        // 랜덤으로 ASpawnPoint 선택
        int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
        ASpawnPoint* SpawnPoint = SpawnPoints[RandomIndex];

        // 선택한 ASpawnPoint에서 몬스터 스폰
    	GetWorld()->SpawnActor<AWolf>(AWolf::StaticClass(), SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation());

    }
}
