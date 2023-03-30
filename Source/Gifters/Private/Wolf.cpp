// Fill out your copyright notice in the Description page of Project Settings.


#include "Wolf.h"

#include "Components/CapsuleComponent.h"

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
}

// Called when the game starts or when spawned
void AWolf::BeginPlay()
{
	Super::BeginPlay();

	Mane->SetMasterPoseComponent(GetMesh());
	Tail->SetMasterPoseComponent(GetMesh());

	GetMesh()->SetCollisionProfileName(TEXT("Monster"));
	//GetMesh()->SetSimulatePhysics(true);

	UE_LOG(LogTemp, Warning, TEXT("Monster"));
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

