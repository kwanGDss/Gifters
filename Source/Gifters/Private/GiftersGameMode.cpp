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

    // ������ ��� ASpawnPoint �ν��Ͻ��� �˻�
    for (TActorIterator<ASpawnPoint> It(GetWorld()); It; ++It)
    {
        SpawnPoints.Add(*It);
    }

    // 5�ʸ��� SpawnMonster �Լ��� ȣ���ϴ� Ÿ�̸Ӹ� ����
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AGiftersGameMode::SpawnMonster, 5.0f, true);
}

void AGiftersGameMode::SpawnMonster()
{
    if (SpawnPoints.Num() > 0)
    {
        // �������� ASpawnPoint ����
        int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
        ASpawnPoint* SpawnPoint = SpawnPoints[RandomIndex];

        // ������ ASpawnPoint���� ���� ����
    	GetWorld()->SpawnActor<AWolf>(AWolf::StaticClass(), SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation());

    }
}
