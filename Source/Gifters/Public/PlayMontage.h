// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PlayMontage.generated.h"

/**
 * 
 */
UCLASS()
class GIFTERS_API UPlayMontage : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UPlayMontage();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

protected:
    UBehaviorTreeComponent* OwningComp;
    AAIController* AIController;

private:
    UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* MontageToPlay;
};