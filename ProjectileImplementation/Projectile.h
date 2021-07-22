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
		//	Called when the game starts or when spawned.
		virtual void BeginPlay() override;

		/** Function defining events to happen on component overlapping.
		*
		*   @param OverlappedComponent: (UPrimitiveComponent*) Component being overlapped.
		*	@param OtherActor: (AActor*) The other actor being overlapped.
		*	@param OtherComp: (UPrimitiveComponent*) The other component being overlapped.
		*	@param OtherBodyIndex: (int32) The other component's body index.
		*	@param bFromSweep: (bool) Did overlapped from sweep?
		*	@param SweepResult: (const FHitResult&) Sweep result if overlapped from sweep.
		*   @return (void)
		*/
		UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		//	Function defining events to happen on component hit.

		/** Function defining events to happen on component overlapping.
		*
		*   @param HitComponent: (UPrimitiveComponent*) Component being hit.
		*	@param OtherActor: (AActor*) The other actor being hit.
		*	@param OtherComponent: (UPrimitiveComponent*) The other component being hit.
		*	@param NormalImpulse: (FVector) Sweep result if overlapped from sweep.
		* 	@param Hit: (const FHitResult&) Hit result.
		*   @return (void)
		*/
		UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

		/** Spawns the Niagara ImpactSystem and the ImpactSound at the given location.
		*
		*	@param SpawnLocation: (FVector) Location to spawn system and sound.
		*   @return (void)
		*/
		UFUNCTION()
		void PlayEffectAndSound(FVector SpawnLocation);
	
	public:	
		//	Sets default values for this actor's properties
		AProjectileBase();

		//	Called every frame
		virtual void Tick(float DeltaTime) override;

		//	Sphere collision arround the projectile
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* SphereCollision;

		//	The static mesh for this projectile.
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StaticMesh;

		//	Projectile movement component.
		UPROPERTY(VisibleAnywhere, Category = "Movement")
		UProjectileMovementComponent* ProjectileMovementComponent;

		//	The Niagara system to be spawned when hitting or overlapping actors.
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
		class UNiagaraSystem* ImpactSystem;

		//	The sound cue to be played when hitting or overlapping actors.
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		class USoundCue* ImpactSound;

		//	Does this projectile affects actors tagged with "Enemy".
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AffectEnemy = false;
	
		//	Does this projectile affects actors tagged with "Player".
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AffectPlayer = false;
};
