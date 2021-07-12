// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/TimelineComponent.h"

#include "BaseElevator.generated.h"

// class UCurveFloat;

UCLASS()
class NEONNARTHEX_API ABaseElevator : public AActor
{
	GENERATED_BODY()

	protected:
		////////////////////////////////////////////////////////////////// UE CHARACTER CLASS
		
		//	Called when the game starts or when spawned
		virtual void BeginPlay() override;

		//////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////// ELEVATOR

		//================================================== PROPERTIES

		FTimeline ElevatorMovementTimeline;

		UPROPERTY(EditAnywhere, Category="BaseElevator|Timeline")
		UCurveFloat* CurveFloat;

		//==================================================

		//================================================== FUNCTIONS
		
		UFUNCTION(BlueprintCallable, Category = "BaseElevator")
		void MoveElevator();


		//==================================================

		//////////////////////////////////////////////////////////////////

	private:
		////////////////////////////////////////////////////////////////// ELEVATOR

		//================================================== PROPERTIES

		//	Lowest height for the elevator. Starting point when CurrentState is true.
		float LowestHeight = 0.0f;
		//	Highest height for the elevator. Starting point when CurrentState is false.
		float HighestHeight = 100.0f;

		//	The root scene component.
		UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta=(AllowPrivateAccess="true"), Category="BaseElevator")
		class USceneComponent* RootSceneComponent = nullptr;

		//	Static mesh for the elevator actor.
		UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess = "true"), Category = "BaseElevator")
		class UStaticMeshComponent* StaticMesh = nullptr;

		//	The trigger area of the elevator. When overlapped, the elevator will move.
		UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess = "true"), Category = "BaseElevator")
		class UBoxComponent* TriggerArea = nullptr;

		//==================================================


		//================================================== FUNCTIONS

		/** Function that executes when the elevator trigger area is overlapped.
		*
		*   @return (void)
		*/
		UFUNCTION()
		void OnElevatorTriggered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		//==================================================

		////////////////////////////////////////////////////////////////// 
	
	public:	
		////////////////////////////////////////////////////////////////// UE CHARACTER CLASS
		
		//	Sets default values for this actor's properties
		ABaseElevator();

		//	Called every frame
		virtual void Tick(float DeltaTime) override;

		////////////////////////////////////////////////////////////////// 


		////////////////////////////////////////////////////////////////// ELEVATOR

		//================================================== PROPERTIES

		//	Current state of the elevator in bool value to determine Z direction of the elevator.
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Elevator")
		bool CurrentState = true;

		//	The elevator height span between lowest height and highest height.
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Elevator")
		float ElevatorHeight = 100.0f;

		//==================================================


		//================================================== FUNCTIONS


		/** Returns the lowest height of this elevator actor.
		*
		*   @return (float) The lowest height of this elevator actor.
		*/
		UFUNCTION(BlueprintPure, Category = "BaseElevator")
		float GetLowestHeight();

		/** Returns the highest height of this elevator actor.
		*
		*   @return (float) The highest height of this elevator actor.
		*/
		UFUNCTION(BlueprintPure, Category = "BaseElevator")
		float GetHighestHeight();


		/** Function that executes everytime the elevator movement timeline is updated.
		*
		*   @param Progress: (float) The current progress of the elevator movement timeline.
		*   @return (void)
		*/
		UFUNCTION(Category = "BaseElevator|Timeline")
		void ElevatorMovementTimelineOnUpdate(float Progress);

		/** Function that executes when the elevator movement timeline is finished.
		*
		*   @return (void)
		*/
		UFUNCTION(Category = "BaseElevator|Timeline")
		void ElevatorMovementTimelineOnFinished();

		//==================================================

		////////////////////////////////////////////////////////////////// 
};
