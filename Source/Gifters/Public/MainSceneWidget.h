// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainSceneWidget.generated.h"

/**
 * 
 */
UCLASS()
class GIFTERS_API UMainSceneWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnStartButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

protected:
	virtual void NativeConstruct() override;
};