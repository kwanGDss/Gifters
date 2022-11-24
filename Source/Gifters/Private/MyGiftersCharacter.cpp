// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGiftersCharacter.h"
#include "MyUserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

AMyGiftersCharacter::AMyGiftersCharacter()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -97.0f), FRotator(0.0f, -90.0f, 0.0f));

	GetFollowCamera()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 200.0f), FRotator(-25.0f, 0.0f, 0.0f));
	GetCameraBoom()->TargetArmLength = 300.0f;
	GetCameraBoom()->bEnableCameraLag = true;

	GetCharacterMovement()->JumpZVelocity = 300.0f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Drongo(TEXT("/Game/ParagonDrongo/Characters/Heroes/Drongo/Meshes/Drongo_GDC"));
	if (SK_Drongo.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Drongo.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_Drongo(TEXT("/Game/ParagonDrongo/Characters/Heroes/Drongo/Drongo_AnimBlueprint"));
	if (ABP_Drongo.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ABP_Drongo.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Drongo(TEXT("/Game/ParagonDrongo/Characters/Heroes/Drongo/Animations/Primary_Fire_Montage"));
	if (AM_Drongo.Succeeded())
	{
		FireMontage = AM_Drongo.Object;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_PlayerHUD(TEXT("/Game/HUDs/PlayerHUD"));
	if (UI_PlayerHUD.Succeeded())
	{
		PlayerHUDClass = UI_PlayerHUD.Class;
	}

	bIsAttacking = false;
	bSaveAttack = false;
	AttackCount = 0;
}

void AMyGiftersCharacter::Attack()
{
	if (bIsAttacking)
	{
		bSaveAttack = true;
	}
	else
	{
		bIsAttacking = true;

		PlayAttackMontage();
	}
}

void AMyGiftersCharacter::ComboAttackSave()
{
	if (bSaveAttack)
	{
		bSaveAttack = false;

		PlayAttackMontage();
	}
}

void AMyGiftersCharacter::ResetCombo()
{
	AttackCount = 0;
	bSaveAttack = false;
	bIsAttacking = false;
}

void AMyGiftersCharacter::Fire()
{
	FHitResult HitResult;
	FVector StartedFire = UKismetMathLibrary::TransformLocation(GetActorTransform(), FVector(100.0f, 20.0f, 57.0f));
	FVector EndedFire = GetActorForwardVector() * 15000.0f + StartedFire;
	TArray<AActor*> IgnoreActors;

	UKismetSystemLibrary::LineTraceSingleByProfile(GetWorld(), StartedFire, EndedFire, "Fire", false, IgnoreActors, EDrawDebugTrace::ForOneFrame, HitResult, true);
	UKismetSystemLibrary::DrawDebugBox(GetWorld(), StartedFire, FVector::OneVector, FLinearColor::Red, FRotator::ZeroRotator, 10.0f);
	UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Fire"));
}

void AMyGiftersCharacter::OnFireMontageStarted(UAnimMontage* AnimMontage)
{
	if (AnimMontage == FireMontage)
	{
		Fire();
	}
}

void AMyGiftersCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyGiftersCharacter::Attack);
}

void AMyGiftersCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MyAnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	MyAnimInstance->OnMontageStarted.AddDynamic(this, &AMyGiftersCharacter::OnFireMontageStarted);
}

void AMyGiftersCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(PlayerHUDClass))
	{
		PlayerHUD = Cast<UUserWidget>(CreateWidget(GetWorld(), PlayerHUDClass));
		if(IsValid(PlayerHUD))
		{
			PlayerHUD->AddToViewport();
		}
	}
}

void AMyGiftersCharacter::PlayAttackMontage()
{
	switch (AttackCount)
	{
	case 0:
		AttackCount = 1;
		PlayAnimMontage(FireMontage);
		break;
	case 1:
		AttackCount = 0;
		PlayAnimMontage(FireMontage);
		break;
	default:
		break;
	}
}
