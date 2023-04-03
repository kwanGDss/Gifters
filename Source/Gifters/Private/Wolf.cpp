// Fill out your copyright notice in the Description page of Project Settings.


#include "Wolf.h"

#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"

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

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	HPBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HPBarWidgetComponent->SetupAttachment(RootComponent);
	HPBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));

	HealthPoint = 100.0f;
}

// Called when the game starts or when spawned
void AWolf::BeginPlay()
{
	Super::BeginPlay();

	HPProgressBar = Cast<UProgressBar>(HPBarWidgetComponent->GetUserWidgetObject()->GetWidgetFromName(TEXT("MonsterHPBar")));

	UpdateHPWidget();
	Mane->SetMasterPoseComponent(GetMesh());
	Tail->SetMasterPoseComponent(GetMesh());

	GetMesh()->SetCollisionProfileName(TEXT("Monster"));
	//GetMesh()->SetSimulatePhysics(true);

	UE_LOG(LogTemp, Warning, TEXT("Monster"));
}

float AWolf::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (HealthPoint <= 0.0f)
	{
		return HealthPoint;
	}

	HealthPoint -= Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); 
	UpdateHPWidget();
	if(HealthPoint <= 0.0f)
	{
		PlayAnimMontage(DeathMontage);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UE_LOG(LogTemp, Warning, TEXT("Death() : %f"), PlayAnimMontage(DeathMontage));
	}
	else
	{
		PlayAnimMontage(GetHitMontage);
		UE_LOG(LogTemp, Warning, TEXT("GetHit() : %f"), PlayAnimMontage(GetHitMontage));
	}

	return HealthPoint;
}

void AWolf::UpdateHPWidget()
{
	HPProgressBar->SetPercent(HealthPoint / 100.0f);
}

// Called every frame
void AWolf::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWolf::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

