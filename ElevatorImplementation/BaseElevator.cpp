// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Environment/BaseElevator.h"

#include "Characters/BaseHero.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ABaseElevator::ABaseElevator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creating root component and setting it as our new root.
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	SetRootComponent(RootSceneComponent);

	// Creating the static mesh and attaching it to the root scene.
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootSceneComponent);

	// Creating the trigger area and attaching it to the static mesh.
	TriggerArea = CreateDefaultSubobject<UBoxComponent>("TriggerArea");
	TriggerArea->SetupAttachment(StaticMesh);

	// Creating the trigger area and attaching it to the static mesh.
	TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &ABaseElevator::OnElevatorTriggered);
}

// Called when the game starts or when spawned
void ABaseElevator::BeginPlay()
{
	Super::BeginPlay();

	/**********
	*	Defines the lowest height as where the current actor location Z is 
	*	and adding the elevator height to it to define the highest height.
	**********/
	LowestHeight = GetActorLocation().Z; 
	HighestHeight = LowestHeight + ElevatorHeight; 

	/**********
	*	If CurveFloat is non null, then proceed to setup the timeline. The timeline needs a Curve ref to be able to function.
	**********/
	if (CurveFloat)
	{
		//	Binding the ElevatorMovementTimelineOnUpdate function to the OnTimelineFloat object.
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("ElevatorMovementTimelineOnUpdate"));

		//	Adding the float interpolation using our curve and the OnTimelineFloat object ref.
		ElevatorMovementTimeline.AddInterpFloat(CurveFloat, TimelineProgress);

		//	Setting the timeline looping to false.
		ElevatorMovementTimeline.SetLooping(false);

		/**********
		*	Creating the OnTimelineEventStatic, binding it to ElevatorMovementTimelineOnFinished,
		*	then set the timeline finished func to the TimelineFinishedEvent.
		**********/
		FOnTimelineEventStatic TimelineFinishedEvent;
		TimelineFinishedEvent.BindUFunction(this, FName("ElevatorMovementTimelineOnFinished"));
		ElevatorMovementTimeline.SetTimelineFinishedFunc(TimelineFinishedEvent);
	}
	
}

// Called every frame
void ABaseElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Ticking the elevator movement timeline.
	ElevatorMovementTimeline.TickTimeline(DeltaTime);
}


void ABaseElevator::OnElevatorTriggered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Trying to cast the other actor to ABaseHero, if cast is successful, move elevator.
	ABaseHero* Hero = Cast<ABaseHero>(OtherActor); 
	if(Hero != nullptr) MoveElevator();
}

float ABaseElevator::GetLowestHeight()
{
	return LowestHeight;
}

float ABaseElevator::GetHighestHeight()
{
	return HighestHeight;
}

void ABaseElevator::MoveElevator()
{
	// If the current state is true, play the timeline forward. Otherwise, play it in reverse.
	CurrentState ? ElevatorMovementTimeline.Play() : ElevatorMovementTimeline.Reverse();
}

void ABaseElevator::ElevatorMovementTimelineOnUpdate(float Progress)
{
	/**********
	*	Get the current location, then sets the Z value of the retrieved vector to a 
	*	lerped float between the lowest height and highest height, using the timeline 
	*	progress as alpha for the lerp.
	**********/
	FVector UpcomingActorLocation = GetActorLocation();
	UpcomingActorLocation.Z = FMath::Lerp(LowestHeight, HighestHeight, Progress);

	// Setting the actor location to the new vector.
	SetActorLocation(UpcomingActorLocation);
}

void ABaseElevator::ElevatorMovementTimelineOnFinished()
{
	//	Switches the CurrentState value to its boolean opposite.
	CurrentState = !CurrentState;
}