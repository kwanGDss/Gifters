// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGiftersCharacter.h"
#include "MyUserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GiftersStatComponent.h"

#define NEED_STAMINA_JUMP 20.0f
#define NEED_STAMINA_RUN 10.0f
#define RESTORE_STAMINA 10.0f
#define LEAST_NEED_STAMINA_RUN 10.0f
#define MAX_STAMINA_POINT 100.0f
#define MAX_HEALTH_POINT 100.0f
#define COMBAT_POSE_CAMERA_DISTANCE 100.0f

AMyGiftersCharacter::AMyGiftersCharacter()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -97.0f), FRotator(0.0f, -90.0f, 0.0f));

	GetFollowCamera()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 200.0f), FRotator(-25.0f, 0.0f, 0.0f));
	GetCameraBoom()->TargetArmLength = 300.0f;
	GetCameraBoom()->bEnableCameraLag = true;

	GetCharacterMovement()->JumpZVelocity = 300.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	PistolStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("PistolStartPoint"));
	PistolStartPoint->SetupAttachment(GetMesh(), TEXT("pistol_cylinder"));
	PistolStartPoint->SetRelativeLocationAndRotation(FVector(0.0f, 20.0f, 0.0f), FRotator(0.0f, 90.0f, 0.0f));

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

	CharacterStat = CreateDefaultSubobject<UGiftersStatComponent>(TEXT("CharacterStat"));

	bIsAttacking = false;
	bSaveAttack = false;
	AttackCount = 0;
	bPressedShift = false;
	bIsRunning = false;
	bRestoreStamina = true;
	bIsCombat = false;
}

void AMyGiftersCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Running
	if (bIsRunning)
	{
		CharacterStat->DecreaseSP(NEED_STAMINA_RUN * DeltaTime);
		bRestoreStamina = false;
	}
	else
	{
		bRestoreStamina = true;
	}

	if (GetCharacterStat()->GetSP() <= KINDA_SMALL_NUMBER)
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		bIsRunning = false;
		bRestoreStamina = true;
	}

	if (GetCharacterMovement()->IsFalling())
	{
		bRestoreStamina = false;
	}

	//Restore Stamina
	if (bRestoreStamina)
	{
		CharacterStat->IncreaseSP(RESTORE_STAMINA * DeltaTime);
	}
}

void AMyGiftersCharacter::Attack()
{
	if (bIsCombat)
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
	FVector StartedFire;
	FVector EndedFire;
	TArray<AActor*> IgnoreActors;

	//if(bIsCombat == false)
	//{
	//	StartedFire = PistolStartPoint();
	//	EndedFire = StartedFire + GetCameraBoom()->GetForwardVector() * 15000.0f;
	//}
	//else
	{
		StartedFire = PistolStartPoint->GetComponentLocation();
		EndedFire = StartedFire + GetFollowCamera()->GetForwardVector() * 15000.0f;
	}

	GetWorld()->LineTraceSingleByProfile(HitResult, StartedFire, EndedFire, "Fire");
	DrawDebugLine(GetWorld(), StartedFire, EndedFire, FColor::Red, false, 5.0f, 0, 5.0f);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Fire")); 
}

void AMyGiftersCharacter::OnFireMontageStarted(UAnimMontage* AnimMontage)
{
	//if (AnimMontage == FireMontage)
	//{
	//	Fire();
	//}
}

UGiftersStatComponent* AMyGiftersCharacter::GetCharacterStat()
{
	if (CharacterStat != nullptr)
	{
		return CharacterStat;
	}

	return nullptr;
}

void AMyGiftersCharacter::Run()
{
	bPressedShift = true;

	if (CharacterStat->GetSP() >= KINDA_SMALL_NUMBER)
	{
		GetCharacterMovement()->MaxWalkSpeed = 750.f;
		bIsRunning = true;
	}
}

void AMyGiftersCharacter::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	bPressedShift = false;
	bIsRunning = false;
}

void AMyGiftersCharacter::Jump()
{
	if (CharacterStat->GetSP() > NEED_STAMINA_JUMP)
	{
		Super::Jump();
		CharacterStat->DecreaseSP(NEED_STAMINA_JUMP);
	}
}

void AMyGiftersCharacter::ChangeCombatPose()
{
	if(bIsCombat)
	{
		bIsCombat = false;
		GetFollowCamera()->AddRelativeLocation(FVector(0.0f, -COMBAT_POSE_CAMERA_DISTANCE, 0.0f));
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	else
	{
		bIsCombat = true;
		GetFollowCamera()->AddRelativeLocation(FVector(0.0f, COMBAT_POSE_CAMERA_DISTANCE, 0.0f));
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void AMyGiftersCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AMyGiftersCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AMyGiftersCharacter::MoveRight);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyGiftersCharacter::Attack);
	PlayerInputComponent->BindAction("DamagedBySelf", IE_Pressed, this, &AMyGiftersCharacter::DamagedBySelf);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMyGiftersCharacter::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AMyGiftersCharacter::Walk);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyGiftersCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Combat", IE_Pressed, this, &AMyGiftersCharacter::ChangeCombatPose);
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

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("BeginPlay"));
}

void AMyGiftersCharacter::MoveForward(float Value)
{
	Super::MoveForward(Value);

	MoveForwardValue = Value;
}

void AMyGiftersCharacter::MoveRight(float Value)
{
	Super::MoveRight(Value);

	MoveRightValue = Value;
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

//TEST
void AMyGiftersCharacter::DamagedBySelf()
{
	UE_LOG(LogTemp, Warning, TEXT("DamagedBySelf"));
	CharacterStat->DecreaseHP(10.0f);
}

