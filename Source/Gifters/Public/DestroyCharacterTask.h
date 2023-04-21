// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DestroyCharacterTask.generated.h"

/**
 * 
 */
UCLASS()
class GIFTERS_API UDestroyCharacterTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UDestroyCharacterTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
