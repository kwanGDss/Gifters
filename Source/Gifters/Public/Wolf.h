// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Wolf.generated.h"

UCLASS()
class GIFTERS_API AWolf : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWolf();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	void UpdateHPWidget();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Mesh", meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Mane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Mesh", meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Tail;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Stat", meta = (AllowPrivateAccess = true))
	class UWidgetComponent* HPBarWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Stat", meta = (AllowPrivateAccess = true))
	class UProgressBar* HPProgressBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Stat", meta = (AllowPrivateAccess = true))
	float HealthPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Anim", meta = (AllowPrivateAccess = true))
	UAnimMontage* GetHitMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Anim", meta = (AllowPrivateAccess = true))
	UAnimMontage* DeathMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = true))
	UAnimInstance* WolfAnimInstance;
};
