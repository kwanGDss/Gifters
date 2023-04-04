// Fill out your copyright notice in the Description page of Project Settings.


#include "GiftersStatComponent.h"

// Sets default values for this component's properties
UGiftersStatComponent::UGiftersStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	HealthPoint = 100.0f;
	StaminaPoint = 100.0f;
	bIsCombat = false;
}

// Called when the game starts
void UGiftersStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UGiftersStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGiftersStatComponent::IncreaseHP(float HP)
{
	HealthPoint += HP;
	HealthPoint = FMath::Clamp(HealthPoint, 0.0f, 100.0f);
	OnHPChanged.Broadcast();
}

void UGiftersStatComponent::DecreaseHP(float HP)
{
	HealthPoint -= HP;
	HealthPoint = FMath::Clamp(HealthPoint, 0.0f, 100.0f);
	OnHPChanged.Broadcast();
}

void UGiftersStatComponent::IncreaseSP(float SP)
{
	StaminaPoint += SP;
	StaminaPoint = FMath::Clamp(StaminaPoint, 0.0f, 100.0f);
	OnSPChanged.Broadcast();
}

void UGiftersStatComponent::DecreaseSP(float SP)
{
	StaminaPoint -= SP;
	StaminaPoint = FMath::Clamp(StaminaPoint, 0.0f, 100.0f);
	OnSPChanged.Broadcast();
}

float UGiftersStatComponent::GetHP()
{
	return HealthPoint;
}

float UGiftersStatComponent::GetSP()
{
	return StaminaPoint;
}

void UGiftersStatComponent::ChangePose(bool Pose)
{
	UE_LOG(LogTemp, Warning, TEXT("Change Pose"));
	bIsCombat = Pose;
	OnPoseChanged.Broadcast();
}

bool UGiftersStatComponent::GetPose()
{
	return bIsCombat;
}
