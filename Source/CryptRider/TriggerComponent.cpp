// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
// once we find an actor using `GetAcceptableActor`
// we need to simulate the physics to false, so the grabbed actor won't collide with any other actor
// in our scenario, we want to move the statue with the secret gate..
// now, take the fetched mover component, and move the actor in move offset direction
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* actor = GetAcceptableActor();
    if(actor != nullptr)
    {
        UPrimitiveComponent* component = Cast<UPrimitiveComponent>(actor->GetRootComponent());
        if(component != nullptr)
        {
            component->SetSimulatePhysics(false);
        }

        actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
        Mover->SetShouldMove(true);
    }
    else
    {
        Mover->SetShouldMove(false);
    }
}


// `GetOverlappingActors` will return the array of actors, 
// we need to check based on tags to return an selective actor...
AActor* UTriggerComponent::GetAcceptableActor() const
{
    TArray<AActor*> actors;
    GetOverlappingActors(actors);

    for (AActor* actor : actors)
    {
        if(actor->ActorHasTag(AcceptableActorTag) && !actor->ActorHasTag("Grabbed"))
        {
            return actor;
        }
    }

    return nullptr;
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
    Mover = NewMover;
}
