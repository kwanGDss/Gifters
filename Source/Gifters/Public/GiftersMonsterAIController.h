// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GiftersMonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class GIFTERS_API AGiftersMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AGiftersMonsterAIController();
	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBlackboardData* BlackboardAsset;

};
