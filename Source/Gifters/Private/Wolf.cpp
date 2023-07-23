// Fill out your copyright notice in the Description page of Project Settings.


#include "Wolf.h"

#include "MyGiftersCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GiftersMonsterAIController.h"
#include "GiftersStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AWolf::AWolf()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -85.674369f), FRotator(0.0f, -90.0f, 0.0f));

	Mane = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mane"));
	Mane->SetupAttachment(GetMesh());

	Tail = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Tail"));
	Tail->SetupAttachment(GetMesh());

	HPBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Wolf(TEXT("/Game/QuadrapedCreatures/Barghest/Meshes/SK_BARGHEST"));
	if (SK_Wolf.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Wolf.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Wolf_Mane(TEXT("/Game/QuadrapedCreatures/Barghest/Meshes/SK_BARGHEST_MANE"));
	if (SK_Wolf_Mane.Succeeded())
	{
		Mane->SetSkeletalMesh(SK_Wolf_Mane.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Wolf_Tail(TEXT("/Game/QuadrapedCreatures/Barghest/Meshes/SK_BARGHEST_TAIL"));
	if (SK_Wolf_Tail.Succeeded())
	{
		Tail->SetSkeletalMesh(SK_Wolf_Tail.Object);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HPBar(TEXT("/Game/HUDs/MonsterHPBar"));
	if (UI_HPBar.Succeeded())
	{
		HPBarWidgetComponent->SetWidgetClass(UI_HPBar.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Wolf_GetHit(TEXT("/Game/QuadrapedCreatures/Barghest/Animations/AM_BARGHEST_GetHit"));
	if (AM_Wolf_GetHit.Succeeded())
	{
		GetHitMontage = AM_Wolf_GetHit.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Wolf_Death(TEXT("/Game/QuadrapedCreatures/Barghest/Animations/AM_BARGHEST_Death"));
	if (AM_Wolf_Death.Succeeded())
	{
		DeathMontage = AM_Wolf_Death.Object;
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_Wolf(TEXT("/Game/QuadrapedCreatures/Barghest/Blueprint/ABP_Wolf"));
	if (ABP_Wolf.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ABP_Wolf.Class);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MI_Wolf(TEXT("/Game/QuadrapedCreatures/Barghest/Materials/M_BARGHEST"));
	if (MI_Wolf.Succeeded())
	{
		DefaultMaterial = MI_Wolf.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MI_HitColorWolf(TEXT("/Game/QuadrapedCreatures/Barghest/Materials/M_BARGHEST_Inst"));
	if (MI_HitColorWolf.Succeeded())
	{
		HitColorMaterial = MI_HitColorWolf.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Wolf_Bite(TEXT("/Game/QuadrapedCreatures/Barghest/Animations/AM_BARGHEST_BiteNormal"));
	if (AM_Wolf_Bite.Succeeded())
	{
		BiteMontage = AM_Wolf_Bite.Object;
	}

	AIControllerClass = AGiftersMonsterAIController::StaticClass();

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	HPBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HPBarWidgetComponent->SetupAttachment(RootComponent);
	HPBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));

	GetCharacterMovement()->MaxWalkSpeed = 250.0f;

	HealthPoint = 100.0f;
	BackHealthPoint = 100.0f;
	bIsDamaged = false;
	bIsDead = false;
	DistanceToPlayer = 0.0f;
}

float AWolf::GetDistanceToPlayer()
{
	return DistanceToPlayer;
}

FVector AWolf::GetPlayerPosition()
{
	return PlayerCharacter->GetActorLocation();
}

bool AWolf::GetGetHit()
{
	return bIsDamaged;
}

void AWolf::OnDeadMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == DeathMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("DEADEADEAD"));
		Destroy();
	}
}

void AWolf::Bite()
{
	UE_LOG(LogTemp, Warning, TEXT("BITE"));
	UGameplayStatics::ApplyDamage(PlayerCharacter, 10.0f, GetController(), this, NULL);
}

// Called when the game starts or when spawned
void AWolf::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Monster BeginPlay"));
	HPProgressBar = Cast<UProgressBar>(HPBarWidgetComponent->GetUserWidgetObject()->GetWidgetFromName(TEXT("MonsterHPBar")));
	BackHPProgressBar = Cast<UProgressBar>(HPBarWidgetComponent->GetUserWidgetObject()->GetWidgetFromName(TEXT("BackHPBar")));

	UpdateHPWidget();
	Mane->SetMasterPoseComponent(GetMesh());
	Tail->SetMasterPoseComponent(GetMesh());

	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	Mane->SetCollisionObjectType(ECC_Pawn);
	Tail->SetCollisionObjectType(ECC_Pawn);

	PlayerCharacter = Cast<AMyGiftersCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerCharacter->OnSelfDeadDelegate.AddDynamic(this, &AWolf::OnTargetDead);

	DistanceToPlayer = GetDistanceTo(PlayerCharacter);

	WolfAnimInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	WolfAnimInstance->OnMontageEnded.AddDynamic(this, &AWolf::OnDeadMontageEnded);
	//WolfAnimInstance->OnMontageStarted.AddDynamic(this, &AWolf::Bite);
}

float AWolf::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (HealthPoint <= 0.0f)
	{
		return HealthPoint;
	}

	HealthPoint -= Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	bIsDamaged = true;
	OnGetHit();
	UpdateHPWidget();
	if (HealthPoint <= 0.0f)
	{
		//WolfAnimInstance->Montage_Stop(1.0f);
		//PlayAnimMontage(DeathMontage);
		OnSelfDead();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		HPBarWidgetComponent->SetVisibility(false);
		PlayerCharacter->GetCharacterStat()->IncreaseKillMonsterCount(1);
		//Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetHit..."));
		//WolfAnimInstance->Montage_Stop(1.0f);
		//PlayAnimMontage(BiteMontage);
	}
	ChangeDamageColor();

	return HealthPoint;
}

void AWolf::UpdateHPWidget()
{
	HPProgressBar->SetPercent(HealthPoint / 100.0f);
	BackHPProgressBar->SetPercent(BackHealthPoint / 100.0f);
}

// Called every frame
void AWolf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDamaged == true)
	{
		BackHealthPoint = UKismetMathLibrary::FInterpTo(BackHealthPoint, HealthPoint, DeltaTime, 3.0f);
		if (FMath::IsNearlyEqual(BackHealthPoint, HealthPoint, 0.01f))
		{
			bIsDamaged = false;
			//OnGetHit();
		}

		UpdateHPWidget();
	}

	FHitResult MeshHitResult;
	FHitResult HPBarHitResult;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		GetMesh()->GetComponentLocation(),
		PlayerCharacter->GetFollowCamera()->GetComponentLocation(),
		ObjectTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		MeshHitResult,
		true
	);

	UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		HPBarWidgetComponent->GetComponentLocation(),
		PlayerCharacter->GetFollowCamera()->GetComponentLocation(),
		ObjectTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		HPBarHitResult,
		true
	);

	if (MeshHitResult.bBlockingHit)
	{
		GetMesh()->SetRenderCustomDepth(true);
		Mane->SetRenderCustomDepth(true);
		Tail->SetRenderCustomDepth(true);
	}
	else
	{
		GetMesh()->SetRenderCustomDepth(false);
		Mane->SetRenderCustomDepth(false);
		Tail->SetRenderCustomDepth(false);
	}

	if (HPBarHitResult.bBlockingHit)
	{
		HPProgressBar->SetVisibility(ESlateVisibility::Hidden);
		BackHPProgressBar->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{

		HPProgressBar->SetVisibility(ESlateVisibility::Visible);
		BackHPProgressBar->SetVisibility(ESlateVisibility::Visible);
	}

	DistanceToPlayer = GetDistanceTo(PlayerCharacter);

	if (WolfAnimInstance && WolfAnimInstance->Montage_IsPlaying(DeathMontage))
	{
		if (WolfAnimInstance->Montage_GetPosition(DeathMontage) >= DeathMontage->GetPlayLength())
		{
			Destroy();
		}
	}
}

// Called to bind functionality to input
void AWolf::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWolf::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AWolf::ChangeDamageColor()
{
	GetMesh()->SetMaterial(0, HitColorMaterial);

	FTimerHandle WaitHandle;
	float WaitTime = 0.2f;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			GetMesh()->SetMaterial(0, DefaultMaterial);
		}), WaitTime, false);
}

void AWolf::OnTargetDead()
{
	OnTargetDeadDelegate.Broadcast();
}

void AWolf::OnSelfDead()
{
	bIsDead = true;
	OnSelfDeadDelegate.Broadcast();
}

void AWolf::OnGetHit()
{
	OnGetHitDelegate.Broadcast();
}
