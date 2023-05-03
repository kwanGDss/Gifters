// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayHitAnimationAndWait.h"
#include "AIController.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"

UPlayHitAnimationAndWait::UPlayHitAnimationAndWait()
{
    bNotifyTick = true;
}

EBTNodeResult::Type UPlayHitAnimationAndWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    ACharacter* Character = AIController ? Cast<ACharacter>(AIController->GetPawn()) : nullptr;
    if (Character)
    {
        UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
        if (AnimInstance && !AnimInstance->Montage_IsPlaying(HitAnimation))
        {
            AnimInstance->Montage_Play(HitAnimation);

            FTimerDelegate TimerDel;
            TimerDel.BindUObject(this, &UPlayHitAnimationAndWait::OnAnimationFinished, &OwnerComp);
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, HitAnimation->GetPlayLength(), false);
        }
        else
        {
            AnimInstance->Montage_Stop(0.1f, HitAnimation);
            AnimInstance->Montage_Play(HitAnimation);

            FTimerDelegate TimerDel;
            TimerDel.BindUObject(this, &UPlayHitAnimationAndWait::OnAnimationFinished, &OwnerComp);
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, HitAnimation->GetPlayLength(), false);
        }
    }

    return EBTNodeResult::InProgress;
}

void UPlayHitAnimationAndWait::OnAnimationFinished(UBehaviorTreeComponent* OwnerComp)
{
    if (OwnerComp)
    {
        FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
    }
}
