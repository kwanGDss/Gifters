// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PlayHitAnimationAndWait.generated.h"

UCLASS()
class GIFTERS_API UPlayHitAnimationAndWait : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
        class UAnimMontage* HitMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blackboard")
        FBlackboardKeySelector IsMonsterHit;

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
    UBehaviorTreeComponent* OwningComp;
    AAIController* AIController;

    void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
