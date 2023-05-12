// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

UChooseRandomLocation::UChooseRandomLocation()
{
    NodeName = TEXT("Choose Random Location");
}

EBTNodeResult::Type UChooseRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
    if (Pawn == nullptr)
        return EBTNodeResult::Failed;

    FVector Origin = Pawn->GetActorLocation();

    FNavLocation NavLocation;

    UWorld* World = Pawn->GetWorld();
    if (World == nullptr)
        return EBTNodeResult::Failed;

    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
    if (NavSystem == nullptr)
        return EBTNodeResult::Failed;

    if (!NavSystem->GetRandomReachablePointInRadius(Origin, 5000.0f, NavLocation))
        return EBTNodeResult::Failed;

    OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocation.SelectedKeyName, NavLocation.Location);

    return EBTNodeResult::Succeeded;
}
