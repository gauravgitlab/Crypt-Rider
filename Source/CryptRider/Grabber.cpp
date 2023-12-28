// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	// FRotator rotation = GetComponentRotation();
	// FString rotationInString = rotation.ToCompactString();
	// UE_LOG(LogTemp, Display, TEXT("Grabber rotation : %s"), *rotationInString);

	// UPhysicsHandleComponent used to grab the statue and releae the statue...
	UPhysicsHandleComponent* physicsHandleComponent = GetPhysicsHandle();
	if(physicsHandleComponent == nullptr)
		return;

	// if Grabbed Component is not null, set the grabbed item location and rotation using physics handler
	// when user grab any object, it will place at certain forward position of player...
	if(physicsHandleComponent->GetGrabbedComponent() != nullptr)
	{
		FVector targetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		physicsHandleComponent->SetTargetLocationAndRotation(targetLocation, GetComponentRotation());
	}
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* physicsHandleComponent = GetPhysicsHandle();
	if(physicsHandleComponent == nullptr)
		return;

	// check when we find if any actor to grab, 
	// once we found an actor,
	// enable its simulate physics and wake all rigitbodies..
	// add another tag `Grabbed`
	// and Detach From actor
	FHitResult hitResult;
	if(GetGrabbleInReach(hitResult))
	{
		UPrimitiveComponent* hitComponent = hitResult.GetComponent();

		hitComponent->SetSimulatePhysics(true);
		hitComponent->WakeAllRigidBodies();

		AActor* hitActor = hitResult.GetActor();
		hitActor->Tags.Add("Grabbed");
		hitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		physicsHandleComponent->GrabComponentAtLocationWithRotation(
			hitComponent, NAME_None, hitResult.ImpactPoint, GetComponentRotation());

		// DrawDebugSphere(world, hitResult.Location, 10, 10, FColor::Green, false, 5);
		// DrawDebugSphere(world, hitResult.ImpactPoint, 10, 10, FColor::Red, false, 5);
		
		// AActor *hitActor = hitResult.GetActor();
		// UE_LOG(LogTemp, Display, TEXT("Hit object name : %s"), *hitActor->GetActorNameOrLabel());
	}
}

// this method helps to cast a sphere range ( Ray cast ) to find the collission with another actors
// SweepSingleByChannel will return an actor based on containing in casting sphere in given `end` direction
// `ECC_GameTraceChannel2` is trace channel for `Grabber`, this will help only to select `Grabber` actors
bool UGrabber::GetGrabbleInReach(FHitResult& OutHitResult) const
{
	UWorld* world = GetWorld();
	FVector start = GetComponentLocation();
	FVector end = start + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(world, start, end, FColor::Red);

	FCollisionShape sphere = FCollisionShape::MakeSphere(GrabRadius);
	return world->SweepSingleByChannel(OutHitResult, start, end, FQuat::Identity, ECC_GameTraceChannel2, sphere);
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* physicsHandleComponent = GetPhysicsHandle();
	if(physicsHandleComponent == nullptr)
		return;

	if(physicsHandleComponent->GetGrabbedComponent() != nullptr)
	{
		// on release, remove the tag `Grabbed`
		AActor* grabbedActor = physicsHandleComponent->GetGrabbedComponent()->GetOwner();
		grabbedActor->Tags.Remove("Grabbed");

		// release component to drop an grad actor
		physicsHandleComponent->GetGrabbedComponent()->WakeAllRigidBodies();
		physicsHandleComponent->ReleaseComponent();
	}	
}

UPhysicsHandleComponent* UGrabber :: GetPhysicsHandle() const
{
	return GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
}
