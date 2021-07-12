// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/TimelineComponent.h"

#include "BaseElevator.generated.h"

// class UCurveFloat;

UCLASS()
class SAMPLES_API ABaseActor : public AActor
{
	GENERATED_BODY()

	protected:
		////////////////////////////////////////////////////////////////// UE CHARACTER CLASS
		
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		//////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////// ELEVATOR

		//================================================== PROPERTIES

		FTimeline Timeline;

		UPROPERTY(EditAnywhere, Category="Timeline")
		UCurveFloat* CurveFloat;

		//==================================================

		//////////////////////////////////////////////////////////////////

	
	public:	
		////////////////////////////////////////////////////////////////// UE CHARACTER CLASS
		
		// Sets default values for this actor's properties
		ABaseActor();

		// Called every frame
		virtual void Tick(float DeltaTime) override;

		////////////////////////////////////////////////////////////////// 


		////////////////////////////////////////////////////////////////// ELEVATOR

		//================================================== PROPERTIES


		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Timeline")
		bool PlayState = true;

		//==================================================


		//================================================== FUNCTIONS

        /** Function that executes everytime the timeline is updated.
		*
		*   @param Progress: (float) The current progress of the timeline.
		*   @return (void)
		*/
		UFUNCTION(Category = "Timeline")
		void Timeline_OnUpdate(float Progress);

        /** Function that executes when the timeline is finished.
		*
		*   @return (void)
		*/
		UFUNCTION(Category = "Timeline")
		void Timeline_OnFinished();

		//==================================================

		////////////////////////////////////////////////////////////////// 
};
