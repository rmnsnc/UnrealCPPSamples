#include "Actors/Environment/BaseElevator.h"

#include "TimelineImplementation/TimelineSample.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ABaseActor::ABaseActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseActor::BeginPlay()
{
	Super::BeginPlay();

    /**********
    **  If CurveFloat is non null, then proceed to setup the timeline. The timeline needs a Curve ref to be able to function.
    **********/
	if (CurveFloat)
	{
        // Binding the Timeline_OnUpdate function to the OnTimelineFloat object.
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("Timeline_OnUpdate"));

        // Adding the float interpolation using our curve and the OnTimelineFloat object ref.
		Timeline.AddInterpFloat(CurveFloat, TimelineProgress);
        // Setting the timeline looping to false.
		Timeline.SetLooping(false);

        // Creating the OnTimelineEventStatic binding.
		FOnTimelineEventStatic TimelineFinishedEvent;
		TimelineFinishedEvent.BindUFunction(this, FName("Timeline_OnFinished"));

        // Setting my OnTimelineEventStatic function to my timeline finished func.
		Timeline.SetTimelineFinishedFunc(TimelineFinishedEvent);

        // Play forward or reverse depending on the PlayState bool.
        PlayState ? Timeline.Play() : Timeline.Reverse();
	}
}

// Called every frame
void ABaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // Ticking my timeline using DeltaTime from the owning Tick.
	Timeline.TickTimeline(DeltaTime);
}

void ABaseActor::Timeline_OnUpdate(float Progress)
{
    /**********
    **  Could literally be calling anything, here I am just moving the current actor.
    **********/
	FVector UpcomingActorLocation = GetActorLocation();
	UpcomingActorLocation.Z = FMath::Lerp(UpcomingActorLocation.Z, UpcomingActorLocation.Z+100, Progress);
	SetActorLocation(UpcomingActorLocation);
}

void ABaseActor::Timeline_OnFinished()
{
    /**********
    **  Could do anything, but here I am just setting play state to its opposite bool value.
    **  The PlayState bool is used in BeginPlay to decide if you play the timeline forward or reverse.
    **  One could always add some function that toggles this PlayState in a later time.
    **********/
	PlayState = !PlayState;
}