// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Grenade(TEXT("/Game/ParagonDrongo/FX/Meshes/Heroes/Drongo/SM_Drongo_Grenade_FX_Body02"));
	if (SM_Grenade.Succeeded())
	{
		MeshComponent->SetStaticMesh(SM_Grenade.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_Grenade_Explosion(TEXT("/Game/ParagonDrongo/FX/Particles/Abilities/Grenade/FX/P_Drongo_Grenade_EndExplosion"));
	if (P_Grenade_Explosion.Succeeded())
	{
		ExplosionEffect = P_Grenade_Explosion.Object;
	}

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->UpdatedComponent = MeshComponent;
	ProjectileMovementComponent->InitialSpeed = 2000.f;
	ProjectileMovementComponent->MaxSpeed = 2000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;

	ExplosionRadius = 500.0f;
	TimeToExplode = 1.0f;
	GrenadeDamage = 100.0f;
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrenade::ThrowGrenade(FVector LaunchVelocity)
{
	UE_LOG(LogTemp, Warning, TEXT("Throwing grenade!"));
	ProjectileMovementComponent->Velocity = LaunchVelocity;

	GetWorld()->GetTimerManager().SetTimer(ExplodeTimerHandle, this, &AGrenade::Explode, TimeToExplode, false);
}

void AGrenade::Explode()
{
	UE_LOG(LogTemp, Warning, TEXT("Grenade exploded!"));
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	UGameplayStatics::ApplyRadialDamage(this, GrenadeDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController());

	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	}

	Destroy();
}

UProjectileMovementComponent* AGrenade::GetGrenadeProjectileMesh() const
{
	return ProjectileMovementComponent;
}
