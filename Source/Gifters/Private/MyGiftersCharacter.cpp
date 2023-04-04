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
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/PrimitiveComponent.h"

#define NEED_STAMINA_JUMP 20.0f
#define NEED_STAMINA_RUN 10.0f
#define RESTORE_STAMINA 10.0f
#define LEAST_NEED_STAMINA_RUN 10.0f
#define MAX_STAMINA_POINT 100.0f
#define MAX_HEALTH_POINT 100.0f
#define COMBAT_POSE_CAMERA_DISTANCE 100.0f
#define PISTOL_RANGE 50000.0f;
#define AIM_DOWN_POS FVector(0.0f, 50.0f, 30.0f)
#define CHARACTER_CAMERA_POS FVector(0.0f, 0.0f, 50.0f)

AMyGiftersCharacter::AMyGiftersCharacter()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -97.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionProfileName(TEXT("Character"));

	GetFollowCamera()->SetRelativeLocationAndRotation(CHARACTER_CAMERA_POS, FRotator(0.0f, 0.0f, 0.0f));
	GetCameraBoom()->TargetArmLength = 300.0f;
	GetCameraBoom()->bEnableCameraLag = true;

	GetCharacterMovement()->JumpZVelocity = 300.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	PistolStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("PistolStartPoint"));
	PistolStartPoint->SetupAttachment(GetMesh(), TEXT("pistol_cylinder"));
	PistolStartPoint->SetRelativeLocationAndRotation(FVector(0.0f, 20.0f, 0.0f), FRotator(0.0f, 90.0f, 0.0f));

	//AimingCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("AimingCameraBoom"));
	//AimingCameraBoom->SetupAttachment(RootComponent);
	//AimingCameraBoom->bUsePawnControlRotation = true;
	//AimingCameraBoom->SocketOffset = FVector(0.0f, 200.0f, 0.0f);
	//AimingCameraBoom->TargetArmLength = 150.0f;

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

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_MuzzleFlash(TEXT("/Game/ParagonDrongo/FX/Particles/Abilities/Primary/FX/P_Drongo_Primary_MuzzleFlash"));
	if(P_MuzzleFlash.Succeeded())
	{
		MuzzleFire = P_MuzzleFlash.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_PrimaryHitWorld(TEXT("/Game/ParagonDrongo/FX/Particles/Abilities/Primary/FX/P_Drongo_Primary_Hit_World"));
	if (P_PrimaryHitWorld.Succeeded())
	{
		PrimaryHitWorld= P_PrimaryHitWorld.Object;
	}

	CharacterStat = CreateDefaultSubobject<UGiftersStatComponent>(TEXT("CharacterStat"));

	bIsAttacking = false;
	bPressedShift = false;
	bIsRunning = false;
	bRestoreStamina = true;
	bIsCombat = false;
	bIsChangingPose = false;
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

	//Camera movement due to pose change
	if (bIsChangingPose == true && bIsCombat == true)
	{
		GetFollowCamera()->AddRelativeLocation(FMath::VInterpTo(FVector::ZeroVector, AIM_DOWN_POS, DeltaTime, 10.0f));

		if (FVector::Dist(GetFollowCamera()->GetRelativeLocation(), CHARACTER_CAMERA_POS + AIM_DOWN_POS) <= 2.0f)
		{
			bIsChangingPose = false;
		}
	}
	else if (bIsChangingPose == true && bIsCombat == false)
	{
		GetFollowCamera()->AddRelativeLocation(FMath::VInterpTo(FVector::ZeroVector, -AIM_DOWN_POS, DeltaTime, 10.0f));

		if (FVector::Dist(GetFollowCamera()->GetRelativeLocation(), CHARACTER_CAMERA_POS) <= 2.0f)
		{
			bIsChangingPose = false;
		}
	}
}

void AMyGiftersCharacter::Attack()
{
	if (bIsCombat && bIsAttacking == false)
	{
		bIsAttacking = true;

		PlayAttackMontage();
	}
}

void AMyGiftersCharacter::Fire()
{
	FHitResult HitResult;
	FVector StartedFire;
	FVector EndedFire;
	TArray<AActor*> IgnoreActors;
	FRotator HitParticleRotator;

	StartedFire = GetFollowCamera()->GetComponentLocation();
	EndedFire = StartedFire + GetFollowCamera()->GetForwardVector() * PISTOL_RANGE;
	UGameplayStatics::SpawnEmitterAttached(MuzzleFire, PistolStartPoint);

	GetWorld()->LineTraceSingleByChannel(HitResult, StartedFire, EndedFire, ECollisionChannel::ECC_GameTraceChannel1);
	if(HitResult.GetActor() != nullptr)
	{
		HitParticleRotator = UKismetMathLibrary::FindLookAtRotation(HitResult.Location, StartedFire);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PrimaryHitWorld, HitResult.Location, HitParticleRotator);
		if(HitResult.Component->Mobility == EComponentMobility::Movable)
		{
			UGameplayStatics::ApplyDamage(HitResult.GetActor(), 10.0f, GetController(), this, NULL);
			HitResult.Component->AddImpulseAtLocation(-HitResult.ImpactNormal * 10000.0f, HitResult.Location);
		}

		//DrawDebugLine(GetWorld(), PistolStartPoint->GetComponentLocation(), HitResult.Location, FColor::Red, false, 5.0f, 0, 5.0f);
	}
	else
	{
		FVector test = GetFollowCamera()->GetForwardVector() * PISTOL_RANGE;
		//DrawDebugLine(GetWorld(), PistolStartPoint->GetComponentLocation(), test, FColor::Red, false, 5.0f, 0, 5.0f);
	}

	bIsAttacking = false;
}

void AMyGiftersCharacter::OnFireMontageStarted(UAnimMontage* AnimMontage)
{

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
	bIsCombat = true;
	bIsChangingPose = true;

	GetCameraBoom()->TargetArmLength = 70.0f;
	//GetFollowCamera()->AddRelativeLocation(AIM_DOWN_POS);
	GetCameraBoom()->bEnableCameraLag = false;
	CharacterStat->ChangePose(true);
	//GetFollowCamera()->MoveComponent()
	//UKismetSystemLibrary::MoveComponentTo(GetFollowCamera(), FVector::ZeroVector, FRotator::ZeroRotator, true, true, 0.3f,);
	//bIsChangingPose = true;
	//GetFollowCamera()->SetRelativeLocation(FMath::VInterpTo(GetFollowCamera()->GetRelativeLocation(), GetFollowCamera()->GetRelativeLocation() + FVector(0.0f, COMBAT_POSE_CAMERA_DISTANCE, 0.0f), GetWorld()->GetDeltaSeconds(), 5.0f));
}

void AMyGiftersCharacter::ChangeNonCombatPose()
{
	bIsCombat = false;
	bIsChangingPose = true;

	GetCameraBoom()->TargetArmLength = 300.0f;
	//GetFollowCamera()->AddRelativeLocation(-AIM_DOWN_POS);
	GetCameraBoom()->bEnableCameraLag = true;
	CharacterStat->ChangePose(false);
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
	PlayerInputComponent->BindAction("Combat", IE_Released, this, &AMyGiftersCharacter::ChangeNonCombatPose);
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
	PlayAnimMontage(FireMontage);
}

//TEST
void AMyGiftersCharacter::DamagedBySelf()
{
	UE_LOG(LogTemp, Warning, TEXT("DamagedBySelf"));
	CharacterStat->DecreaseHP(10.0f);
}

