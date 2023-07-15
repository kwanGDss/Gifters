// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GiftersCharacter.h"
#include "MyGiftersCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDeadDelegate);

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
	void ComboAttackSave();

	UFUNCTION(BlueprintCallable)
	void ResetCombo();

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

	UFUNCTION(BlueprintCallable, Category = "Player")
	void OnSelfDead();

	UPROPERTY(BlueprintAssignable, Category = "Wolf")
	FPlayerDeadDelegate OnSelfDeadDelegate;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	void PlayAttackMontage();

	//TEST
	void DamagedBySelf();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	bool bIsAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	bool bSaveAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	int AttackCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = true))
	UAnimMontage* FireMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = true))
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Stat", meta = (AllowPrivateAccess = true))
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = true))
	UAnimMontage* DeathMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniMap", meta = (AllowPrivateAccess = true))
	UCameraComponent* MiniMapCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniMap", meta = (AllowPrivateAccess = true))
	UTextureRenderTarget2D* MiniMapRenderTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniMap", meta = (AllowPrivateAccess = true))
	USceneCaptureComponent2D* SceneCapture;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniMap", meta = (AllowPrivateAccess = true))
	UMaterial* MiniMapMaterial;
};
