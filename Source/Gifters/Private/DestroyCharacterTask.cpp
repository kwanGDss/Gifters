// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyCharacterTask.h"
#include "AIController.h"
#include "GameFramework/Character.h"

UDestroyCharacterTask::UDestroyCharacterTask()
{
}

EBTNodeResult::Type UDestroyCharacterTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController)
    {
        ACharacter* ControlledCharacter = Cast<ACharacter>(AIController->GetPawn());
        if (ControlledCharacter)
        {
            ControlledCharacter->Destroy();
            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}