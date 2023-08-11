// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

UCLASS()
class GIFTERS_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ThrowGrenade(FVector LaunchVelocity);
	
	void Explode();

	class UProjectileMovementComponent* GetGrenadeProjectileMesh() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = true))
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grenade", meta = (AllowPrivateAccess = true))
	float ExplosionRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grenade", meta = (AllowPrivateAccess = true))
	float TimeToExplode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grenade", meta = (AllowPrivateAccess = true))
	float GrenadeDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Particle", meta = (AllowPrivateAccess = true))
	class UParticleSystem* ExplosionEffect;

	FTimerHandle ExplodeTimerHandle;
};
