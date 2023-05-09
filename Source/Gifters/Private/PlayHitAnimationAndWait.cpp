#include "PlayHitAnimationAndWait.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"

EBTNodeResult::Type UPlayHitAnimationAndWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UE_LOG(LogTemp, Warning, TEXT("UPlayHitAnimationAndWait"));
    OwningComp = &OwnerComp;
    AIController = OwnerComp.GetAIOwner();

    APawn* ControlledPawn = AIController->GetPawn();
    UAnimInstance* AnimInstance = ControlledPawn->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();

    if (AnimInstance && HitMontage)
    {
        AnimInstance->Montage_Play(HitMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
        FOnMontageEnded MontageEndedDelegate;
        MontageEndedDelegate.BindUObject(this, &UPlayHitAnimationAndWait::OnMontageEnded);
        AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, HitMontage);
        return EBTNodeResult::InProgress;
    }

    return EBTNodeResult::Failed;
}

void UPlayHitAnimationAndWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    bool IsHit = BlackboardComponent->GetValueAsBool(IsMonsterHit.SelectedKeyName);

    if (IsHit)
    {
        ExecuteTask(OwnerComp, NodeMemory);
    }
}

void UPlayHitAnimationAndWait::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (OwningComp && AIController)
    {
        OwningComp->GetBlackboardComponent()->SetValueAsBool(IsMonsterHit.SelectedKeyName, false);
        FinishLatentTask(*OwningComp, EBTNodeResult::Succeeded);
    }
}