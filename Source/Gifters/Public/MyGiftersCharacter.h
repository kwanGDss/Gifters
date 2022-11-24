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

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

private:
	void PlayAttackMontage();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	bool bIsAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	bool bSaveAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	int AttackCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	UAnimMontage* FireMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	UAnimInstance* MyAnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> PlayerHUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = true))
	class UUserWidget* PlayerHUD;
};
