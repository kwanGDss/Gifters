// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GiftersStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnSPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnPoseChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GIFTERS_API UGiftersStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGiftersStatComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void IncreaseHP(float HP);
	void DecreaseHP(float HP);

	void IncreaseSP(float SP);
	void DecreaseSP(float SP);

	float GetHP();
	float GetSP();

	void ChangePose(bool bIsCombat);

	bool GetPose();

	void RegisterRenderTarget(UTextureRenderTarget2D* InMiniMapRenderTarget);

	UTextureRenderTarget2D* GetRenderTarget();

	void IncreaseKillMonsterCount(int Count);
	int GetKillMonsterCount() const;

	int GetGrenadeCount();
	void SetGrenadeCount(int Count);

	void IncreaseGrenadeCount();
	void DecreaseGrenadeCount();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FOnHPChangedDelegate OnHPChanged;
	FOnSPChangedDelegate OnSPChanged;
	FOnPoseChangedDelegate OnPoseChanged;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float HealthPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float StaminaPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	bool bIsCombat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	UTextureRenderTarget2D* MiniMapRenderTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	int KillMonsterCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	int GrenadeCount;
};
