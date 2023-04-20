// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayMontageAndWaitTask.h"
#include "AIController.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"

UPlayMontageAndWaitTask::UPlayMontageAndWaitTask()
{
    bNotifyTaskFinished = true;

    static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Wolf_Death(TEXT("/Game/QuadrapedCreatures/Barghest/Animations/AM_BARGHEST_Death"));
    if (AM_Wolf_Death.Succeeded())
    {
        MontageToPlay = AM_Wolf_Death.Object;
    }
}

EBTNodeResult::Type UPlayMontageAndWaitTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    MyOwnerComp = &OwnerComp;
    ACharacter* Character = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());

    if (Character && MontageToPlay)
    {
        UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            MontageBlendingOutDelegate.BindUFunction(this, "OnMontageEnded");
            AnimInstance->Montage_Play(MontageToPlay);
            AnimInstance->Montage_SetBlendingOutDelegate(MontageBlendingOutDelegate, MontageToPlay);

            return EBTNodeResult::InProgress;
        }
    }

    return EBTNodeResult::Failed;
}

void UPlayMontageAndWaitTask::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    MyOwnerComp = nullptr;
}

void UPlayMontageAndWaitTask::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (MyOwnerComp)
    {
        MyOwnerComp->OnTaskFinished(this, bInterrupted ? EBTNodeResult::Aborted : EBTNodeResult::Succeeded);
    }
}