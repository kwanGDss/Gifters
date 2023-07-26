// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainScenePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GIFTERS_API AMainScenePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainScenePlayerController();

protected:
	virtual void BeginPlay() override;
	void FloatMainSceneHUD();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> MainSceneHUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = true))
	class UMainSceneWidget* MainSceneHUD;
};
