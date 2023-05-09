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
	virtual void Tick(float DeltaSeconds) override;

	//UFUNCTION()
	//void OnTargetDeadHandler();

	UFUNCTION()
	void OnMonsterDeadHandler();

	UFUNCTION()
	void OnMonsterHitHandler();

public:

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	//FName DistanceKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	FName PlayerPositionKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	FName IsPlayerInSightRangeKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	FName IsPlayerInAttackRangeKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	FName IsMonsterHitKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	FName IsMonsterDeadKey;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBlackboardData* BlackboardAsset;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class AWolf* Wolf;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Distance", meta = (AllowPrivateAccess = true))
	float Distance;
};
