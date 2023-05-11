// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayMontage.h"
#include "AIController.h"
#include "Animation/AnimInstance.h"

UPlayMontage::UPlayMontage()
{
	NodeName = "Play Montage";

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Wolf_Bite(TEXT("/Game/QuadrapedCreatures/Barghest/Animations/AM_BARGHEST_BiteNormal"));
	if (AM_Wolf_Bite.Succeeded())
	{
		MontageToPlay = AM_Wolf_Bite.Object;
	}
}

EBTNodeResult::Type UPlayMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwningComp = &OwnerComp;
	AIController = OwnerComp.GetAIOwner();

	APawn* ControlledPawn = AIController->GetPawn();
	UAnimInstance* AnimInstance = ControlledPawn->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();

	if (AnimInstance && MontageToPlay)
	{
		AnimInstance->Montage_Play(MontageToPlay);
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindUObject(this, &UPlayMontage::OnMontageEnded);
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UPlayMontage::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (OwningComp && AIController)
	{
		FinishLatentTask(*OwningComp, EBTNodeResult::Succeeded);
	}
}