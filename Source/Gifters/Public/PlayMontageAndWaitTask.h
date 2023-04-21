// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PlayMontageAndWaitTask.generated.h"

/**
 * 
 */
UCLASS()
class GIFTERS_API UPlayMontageAndWaitTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UPlayMontageAndWaitTask();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MontageToPlay;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    FOnMontageBlendingOutStarted MontageBlendingOutDelegate;

    UBehaviorTreeComponent* MyOwnerComp;
};
