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

	UFUNCTION()
	void OnTargetDeadHandler();

	UFUNCTION()
	void OnSelfDeadHandler();

	UFUNCTION()
	void OnGetHitHandler();

	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	FName IsTargetDeadKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	FName IsSelfDeadKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	FName DistanceKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	FName PlayerPositionKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	FName GetHitKey;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBlackboardData* BlackboardAsset;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class AWolf* Wolf;
};
