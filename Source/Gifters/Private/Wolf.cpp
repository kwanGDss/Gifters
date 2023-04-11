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

	AIControllerClass = AGiftersMonsterAIController::StaticClass();

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	HPBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HPBarWidgetComponent->SetupAttachment(RootComponent);
	HPBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));

	HealthPoint = 100.0f;
	BackHealthPoint = 100.0f;
	bIsDamaged = false;
}

// Called when the game starts or when spawned
void AWolf::BeginPlay()
{
	Super::BeginPlay();

	HPProgressBar = Cast<UProgressBar>(HPBarWidgetComponent->GetUserWidgetObject()->GetWidgetFromName(TEXT("MonsterHPBar")));
	BackHPProgressBar = Cast<UProgressBar>(HPBarWidgetComponent->GetUserWidgetObject()->GetWidgetFromName(TEXT("BackHPBar")));

	UpdateHPWidget();
	Mane->SetMasterPoseComponent(GetMesh());
	Tail->SetMasterPoseComponent(GetMesh());

	GetMesh()->SetCollisionProfileName(TEXT("Monster"));

	PlayerCharacter = Cast<AMyGiftersCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//PlayerCharacter = Cast<APlayerCameraManager>UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
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
	UpdateHPWidget();
	if(HealthPoint <= 0.0f)
	{
		PlayAnimMontage(DeathMontage);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		HPBarWidgetComponent->DestroyComponent();
		UE_LOG(LogTemp, Warning, TEXT("Death() : %f"), PlayAnimMontage(DeathMontage));
	}
	else
	{
		PlayAnimMontage(GetHitMontage);
		UE_LOG(LogTemp, Warning, TEXT("GetHit() : %f"), PlayAnimMontage(GetHitMontage));
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

	if(bIsDamaged==true)
	{
		BackHealthPoint = UKismetMathLibrary::FInterpTo(BackHealthPoint, HealthPoint, DeltaTime, 3.0f);
		if(FMath::IsNearlyEqual(BackHealthPoint, HealthPoint, 0.01f))
		{
			bIsDamaged = false;
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

	if(MeshHitResult.bBlockingHit)
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

	//ActorLineTraceSingle(HitResult, GetActorLocation(), GetWorld()->GetCurrentLevel()->)

}

// Called to bind functionality to input
void AWolf::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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

