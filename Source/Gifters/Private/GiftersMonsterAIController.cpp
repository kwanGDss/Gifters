// Fill out your copyright notice in the Description page of Project Settings.


#include "GiftersMonsterAIController.h"

#include <string>

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Wolf.h"

AGiftersMonsterAIController::AGiftersMonsterAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	IsTargetDeadKey = "IsTargetDead";
	IsSelfDeadKey = "IsSelfDead";
	DistanceKey = "Distance";
	PlayerPositionKey = "PlayerPosition";
	GetHitKey = "GetHit";
}

void AGiftersMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UE_LOG(LogTemp, Warning, TEXT("AIController::OnPossess"));

	BehaviorTree = LoadObject<UBehaviorTree>(nullptr, TEXT("/Game/Assets/BT_Monster"));
	BlackboardAsset = LoadObject<UBlackboardData>(nullptr, TEXT("/Game/Assets/BB_Monster"));

	if (BehaviorTree)
	{
		if (BlackboardAsset)
		{
			BlackboardComponent->InitializeBlackboard(*BlackboardAsset);
		}
		RunBehaviorTree(BehaviorTree);
	}

	Wolf = Cast<AWolf>(InPawn);

	if (Wolf)
	{
		// 몬스터 이벤트에 AIController 메서드 바인딩
		Wolf->OnTargetDeadDelegate.AddDynamic(this, &AGiftersMonsterAIController::OnTargetDeadHandler);
		Wolf->OnSelfDeadDelegate.AddDynamic(this, &AGiftersMonsterAIController::OnSelfDeadHandler);
		Wolf->OnGetHitDelegate.AddDynamic(this, &AGiftersMonsterAIController::OnGetHitHandler);
	}
}

void AGiftersMonsterAIController::OnTargetDeadHandler()
{
	UE_LOG(LogTemp, Warning, TEXT("OnTargetDeadHandler"));
	BlackboardComponent->SetValueAsBool(IsTargetDeadKey, true);
}

void AGiftersMonsterAIController::OnSelfDeadHandler()
{
	UE_LOG(LogTemp, Warning, TEXT("OnSelfDeadHandler"));
	BlackboardComponent->SetValueAsBool(IsSelfDeadKey, true);
}

void AGiftersMonsterAIController::OnGetHitHandler()
{
	UE_LOG(LogTemp, Warning, TEXT("OnGetHitHandler"));
	BlackboardComponent->SetValueAsBool(GetHitKey, Wolf->GetGetHit());
}

void AGiftersMonsterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//if (BlackboardComponent->GetValueAsBool(IsSelfDeadKey))
	//{
	//	// Stop the BehaviorTree
	//	UE_LOG(LogTemp, Warning, TEXT("Monster STOP!"));
	//	Destroy();
	//}
	//UE_LOG(LogTemp, Warning, TEXT("%f"), FVector::Dist(Wolf->GetActorLocation(), Wolf->GetPlayerPosition()));
	//UE_LOG(LogTemp, Warning, TEXT("Wolf->GetPlayerPosition() : %s"), *Wolf->GetPlayerPosition().ToString());

	BlackboardComponent->SetValueAsFloat(DistanceKey, FVector::Dist(Wolf->GetActorLocation(), Wolf->GetPlayerPosition()));
	BlackboardComponent->SetValueAsVector(PlayerPositionKey, Wolf->GetPlayerPosition());
}
