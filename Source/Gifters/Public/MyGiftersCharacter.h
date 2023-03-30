// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GiftersCharacter.h"
#include "MyGiftersCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GIFTERS_API AMyGiftersCharacter : public AGiftersCharacter
{
	GENERATED_BODY()

public:
	AMyGiftersCharacter();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintCallable)
	void Fire();

	UFUNCTION(BlueprintCallable)
	void OnFireMontageStarted(UAnimMontage* AnimMontage);

	class UGiftersStatComponent* GetCharacterStat();

	void Run();

	void Walk();

	void Jump();

	void ChangeCombatPose();

	void ChangeNonCombatPose();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;

private:
	void PlayAttackMontage();

	//TEST
	void DamagedBySelf();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	bool bIsAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	UAnimMontage* FireMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	UAnimInstance* MyAnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	UGiftersStatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	bool bIsRunning;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	bool bPressedShift;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	bool bRestoreStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	bool bIsCombat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = true))
	float MoveForwardValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = true))
	float MoveRightValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	USceneComponent* PistolStartPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Attack", meta = (AllowPrivateAccess = true))
	bool bIsChangingPose;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Camera", meta = (AllowPrivateAccess = true))
	USpringArmComponent* AimingCameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Particle", meta = (AllowPrivateAccess = true))
	class UParticleSystem* MuzzleFire;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Particle", meta = (AllowPrivateAccess = true))
	UParticleSystem* PrimaryHitWorld;
};
