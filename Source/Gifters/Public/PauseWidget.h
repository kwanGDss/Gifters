// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class GIFTERS_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnResumeButtonClicked();

	UFUNCTION()
	void OnRestartButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

protected:
	virtual void NativeConstruct() override;
};
