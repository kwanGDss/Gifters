// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class GIFTERS_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCharacterStat(class UGiftersStatComponent* NewCharacterStat);

protected:
	virtual void NativeConstruct() override;

	void UpdateHPWidget();
	void UpdateSPWidget();
	void UpdatePose();

	void BrushRenderTarget(UTextureRenderTarget2D* RenderTarget);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
	class UGiftersStatComponent* CurrentCharacterStat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = true))
	class UProgressBar* HPProgressBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = true))
	UProgressBar* SPProgressBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = true))
	class UImage* Crosshair;
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = true))
	//UImage* Minimap;
};
