// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SetGetHitToFalse.generated.h"

/**
 * 
 */
UCLASS()
class GIFTERS_API USetGetHitToFalse : public UBTTaskNode
{
	GENERATED_BODY()

public:
	USetGetHitToFalse();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
