// Fill out your copyright notice in the Description page of Project Settings.


#include "GiftersMonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Wolf.h"

#define SIGHT_RANGE 1000.0f
#define ATTACK_RANGE 300.0f

AGiftersMonsterAIController::AGiftersMonsterAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	//DistanceKey = "Distance";
	PlayerPositionKey = "PlayerPosition";
	IsPlayerInSightRangeKey = "IsPlayerInSightRange";
	IsPlayerInAttackRangeKey = "IsPlayerInAttackRange";
	IsMonsterHitKey = "IsMonsterHit";
	IsMonsterDeadKey = "IsMonsterDead";
	//IsAlreadyHitKey = "IsAlreadyHit";
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
		//Wolf->OnTargetDeadDelegate.AddDynamic(this, &AGiftersMonsterAIController::OnTargetDeadHandler);
		Wolf->OnSelfDeadDelegate.AddDynamic(this, &AGiftersMonsterAIController::OnMonsterDeadHandler);
		Wolf->OnGetHitDelegate.AddDynamic(this, &AGiftersMonsterAIController::OnMonsterHitHandler);
	}
}

//void AGiftersMonsterAIController::OnTargetDeadHandler()
//{
//	UE_LOG(LogTemp, Warning, TEXT("OnTargetDeadHandler"));
//	BlackboardComponent->SetValueAsBool(IsTargetDeadKey, true);
//}

void AGiftersMonsterAIController::OnMonsterDeadHandler()
{
	UE_LOG(LogTemp, Warning, TEXT("OnMonsterDeadHandler"));
	BlackboardComponent->SetValueAsBool(IsMonsterDeadKey, true);
}

void AGiftersMonsterAIController::OnMonsterHitHandler()
{
	UE_LOG(LogTemp, Warning, TEXT("OnMonsterHitHandler"));
	BlackboardComponent->SetValueAsBool(IsMonsterHitKey, true);
}

void AGiftersMonsterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//UE_LOG(LogTemp, Warning, TEXT("GetHit : %s"), );
	//BlackboardComponent->SetValueAsFloat(DistanceKey, FVector::Dist(Wolf->GetActorLocation(), Wolf->GetPlayerPosition()));
	BlackboardComponent->SetValueAsVector(PlayerPositionKey, Wolf->GetPlayerPosition());

	Distance = FVector::Dist(Wolf->GetActorLocation(), Wolf->GetPlayerPosition());

	if (Distance <= SIGHT_RANGE)
	{
		BlackboardComponent->SetValueAsBool(IsPlayerInSightRangeKey, true);
	}
	else 
	{
		BlackboardComponent->SetValueAsBool(IsPlayerInSightRangeKey, false);
	}

	if (Distance <= ATTACK_RANGE)
	{
		BlackboardComponent->SetValueAsBool(IsPlayerInAttackRangeKey, true);
	}
	else 
	{
		BlackboardComponent->SetValueAsBool(IsPlayerInAttackRangeKey, false);
	}
}
