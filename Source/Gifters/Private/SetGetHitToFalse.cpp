// Fill out your copyright notice in the Description page of Project Settings.


#include "SetGetHitToFalse.h"
#include "BehaviorTree/BlackboardComponent.h"

USetGetHitToFalse::USetGetHitToFalse()
{
    NodeName = "Set IsDamaged to False";
}

EBTNodeResult::Type USetGetHitToFalse::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        FName GetHitKeyName = "GetHit";
        BlackboardComp->SetValueAsBool(GetHitKeyName, false);
    }

    return EBTNodeResult::Succeeded;
}