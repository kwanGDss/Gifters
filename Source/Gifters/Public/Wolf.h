// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Wolf.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMonsterDelegate);

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

	virtual void PostInitializeComponents() override;

	void ChangeDamageColor();

	UFUNCTION(BlueprintCallable, Category = "Wolf")
	void OnTargetDead();

	UFUNCTION(BlueprintCallable, Category = "Wolf")
	void OnSelfDead();

	UFUNCTION(BlueprintCallable, Category = "Wolf")
	void OnGetHit();

	// Event for target dead
	UPROPERTY(BlueprintAssignable, Category = "Wolf")
	FMonsterDelegate OnTargetDeadDelegate;

	// Event for self dead
	UPROPERTY(BlueprintAssignable, Category = "Wolf")
	FMonsterDelegate OnSelfDeadDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Wolf")
	FMonsterDelegate OnGetHitDelegate;

	UFUNCTION()
	float GetDistanceToPlayer();

	UFUNCTION()
	FVector GetPlayerPosition();

	UFUNCTION()
	bool GetGetHit();

	UFUNCTION(BlueprintCallable)
	void OnDeadMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable)
	void Bite();

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material", meta = (AllowPrivateAccess = true))
	UMaterial* DefaultMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material", meta = (AllowPrivateAccess = true))
	UMaterialInstance* HitColorMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	bool bIsDamaged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	float BackHealthPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Stat", meta = (AllowPrivateAccess = true))
	UProgressBar* BackHPProgressBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Character", meta = (AllowPrivateAccess = true))
	class AMyGiftersCharacter* PlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Stat", meta = (AllowPrivateAccess = true))
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "AI", meta = (AllowPrivateAccess = true))
	float DistanceToPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Anim", meta = (AllowPrivateAccess = true))
	UAnimMontage* BiteMontage;
};
