// Fill out your copyright notice in the Description page of Project Settings.


#include "GiftersMonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AGiftersMonsterAIController::AGiftersMonsterAIController()
{
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
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
}
