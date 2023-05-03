// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PlayHitAnimationAndWait.generated.h"

/**
 *
 */
UCLASS()
class GIFTERS_API UPlayHitAnimationAndWait : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPlayHitAnimationAndWait();

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* HitAnimation;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void OnAnimationFinished(UBehaviorTreeComponent* OwnerComp);

private:
	FTimerHandle TimerHandle;
};