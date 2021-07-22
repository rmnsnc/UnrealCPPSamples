// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UNiagaraSystem;
class USoundBase;

UCLASS()
class NEONNARTHEX_API AProjectileBase : public AActor
{
	GENERATED_BODY()

	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

		UFUNCTION()
		void PlayEffectAndSound(FVector SpawnLocation);
	
	public:	
		// Sets default values for this actor's properties
		AProjectileBase();

		// Called every frame
		virtual void Tick(float DeltaTime) override;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* SphereCollision;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StaticMesh;

		// Projectile movement component.
		UPROPERTY(VisibleAnywhere, Category = "Movement")
		UProjectileMovementComponent* ProjectileMovementComponent;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
		class UNiagaraSystem* ImpactSystem;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		class USoundCue* ImpactSound;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AffectEnemy = false;
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AffectPlayer = false;
};
