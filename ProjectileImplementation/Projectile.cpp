// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ProjectileBase.h"

#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	//	Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//	Sets tags for this projectile.
	this->Tags = { "Bullet", "Player" };

	//	Create the Sphere Collision object and sets the radius to 10.
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->InitSphereRadius(10.f);
	
	//	Create the Sphere Collision object and sets the radius to 10.
	RootComponent = SphereCollision;

	//	Sets the Sphere Collision collision profile name OverlapOnlyPawn so that it interacts as it should 
	//	with both world dynamic and static as well as pawns. 
	SphereCollision->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	//	Create mesh object and attach it to the Sphere Collision object.
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SphereCollision);

	//	Load the Mesh, the material, and the sound assets.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Materials/M_Protos"));
	static ConstructorHelpers::FObjectFinder<USoundBase> ImpactSoundAsset(TEXT("/Game/Audio/Waves/SE/SE_ProtoImpact_1"));

	//	If impact system is null, load the Niagara asset and assign it to Impact System.
	if (!ImpactSystem)
	{
		ImpactSystem = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/FX/Systems/NS_ProtoImpact"), nullptr, LOAD_None, nullptr);
	}

	//	If impact sound is null, load the sound asset and assign it to Impact Sound.
	if (!ImpactSound)
	{
		ImpactSound = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, TEXT("/Game/Audio/Cues/SE/SE_ProtoImpact")));
	}


	//	If the mesh asset was successfully loaded.
	if (MeshAsset.Succeeded())
	{
		//	Assign the asset to the static mesh object.
		StaticMesh->SetStaticMesh(MeshAsset.Object);

		//	Set the relative location of the mesh to 0,0,0.
		StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		//	Set the scale of the mesh to the needed size.
		StaticMesh->SetWorldScale3D(FVector(0.75f, .15f, .1f));
		StaticMesh->SetCollisionProfileName("NoCollision");

		// If the material asset was successfully loaded.
		if (MaterialAsset.Succeeded())
		{
			//	Set the static mesh material to the material asset.
			StaticMesh->SetMaterial(0, MaterialAsset.Object);
		}
	}

	//	If projectile movement component is valid.
	if (!ProjectileMovementComponent)
	{
		//	Creating the projectile movement component
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

		//	Assign the component to move and update. The sphere collision as it is the root component.
		ProjectileMovementComponent->SetUpdatedComponent(SphereCollision);

		//	Set the initial speed and max speed.
		ProjectileMovementComponent->InitialSpeed = 3000.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;

		//	Setting the rotation to follow velocity of the projectile.
		ProjectileMovementComponent->bRotationFollowsVelocity = true;

		//	Enabling bounce, setting bounciness and gravity scale.
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(2.0f);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnOverlap);
	SphereCollision->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileBase::PlayEffectAndSound(FVector SpawnLocation)
{
	if (ImpactSystem && ImpactSound)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactSystem, SpawnLocation);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, SpawnLocation);
	}
}

void AProjectileBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			if (AffectPlayer)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, "Touched a player");
				PlayEffectAndSound(GetActorLocation());
				if (IsValid(this))
				{
					Destroy();
				}
			}
		}
		else if (OtherActor->ActorHasTag("Enemy"))
		{
			if (AffectEnemy)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, "Touched an enemy");
				PlayEffectAndSound(GetActorLocation());
				if (IsValid(this))
				{
					Destroy();
				}
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, "Touched something else");
			PlayEffectAndSound(GetActorLocation());
			if (IsValid(this))
			{
				Destroy();
			}
		}
	}
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComponent != nullptr))
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			if (AffectPlayer)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, "Touched a player");
				PlayEffectAndSound(GetActorLocation());
				if (IsValid(this))
				{
					Destroy();
				}
			}
		}
		else if (OtherActor->ActorHasTag("Enemy"))
		{
			if (AffectEnemy)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, "Touched an enemy");
				PlayEffectAndSound(GetActorLocation());
				if (IsValid(this))
				{
					Destroy();
				}
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, "Touched something else");
			PlayEffectAndSound(GetActorLocation());
			if (IsValid(this))
			{
				Destroy();
			}
		}
	}
}
