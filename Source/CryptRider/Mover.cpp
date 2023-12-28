// Fill out your copyright notice in the Description page of Project Settings.

#include "Mover.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	// ...
	// cache original Localtion of actor
	OriginalLocation = GetOwner()->GetActorLocation();
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	// AActor* owner = GetOwner();
	// FString ownerName = owner->GetActorNameOrLabel();
	// FVector ownerLocation = owner->GetActorLocation();
	// FString ownerLocationInStr = ownerLocation.ToCompactString();
	// UE_LOG(LogTemp, Display, TEXT("Owner name is %s on location %s"), *ownerName, *ownerLocationInStr);

	// set original location to every tick
	// once Should move is set to true, add offset in target location,
	// else move back to original location...
	FVector targetLocation = OriginalLocation;
	if(ShouldMove)
	{
		targetLocation = OriginalLocation + MoveOffset;
	}

	FVector currentLocation = GetOwner()->GetActorLocation();
	float speed = MoveOffset.Length() / MoveTime;

	FVector newLocation = FMath::VInterpConstantTo(currentLocation, targetLocation, DeltaTime, speed); // interpolation from one location to another location
	GetOwner()->SetActorLocation(newLocation);
}

void UMover::SetShouldMove(bool NewShouldMove)
{
    ShouldMove = NewShouldMove;
}

