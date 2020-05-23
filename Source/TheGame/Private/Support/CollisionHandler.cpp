// Copyright by Shepherd Dowling under the Apache v2 licence


#include "Support/CollisionHandler.h"
#include "Support/Rock.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "UObject/UObjectGlobals.h" 
#include "Engine/World.h" 

#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/InputComponent.h"

#include "Engine/World.h" 

// Sets default values
UCollisionHandler::UCollisionHandler()
{

}

void UCollisionHandler::BeginPlay()
{
	Super::BeginPlay();
}

void UCollisionHandler::Init(ACharacter* thisCharacter, FString&& triggerCapsuleName)
{
	ThisCharacter = thisCharacter;
	TriggerComponent = ARock::GetActorComponent(ThisCharacter, triggerCapsuleName);
}

UPrimitiveComponent* UCollisionHandler::GetTriggerComponent() const {
	return TriggerComponent;
}

void UCollisionHandler::SetCollidingActor(AActor* actor)
{
	CollidingActor = actor;
}

AActor* UCollisionHandler::GetCollidingActor() const
{
	return CollidingActor;
}

FVector UCollisionHandler::GetCollisionLocation()
{
	return CollidingActor->GetTargetLocation();
}

void UCollisionHandler::ModifyDirectional(const FVector& DirectionalRef, float X, float Y)
{
	// This function is called once at a time from Forward/Right movement

	// Use 4 bone sockets to help your collisions navigate
	// Front, Back, Left, Right
	// You can only go into the directions that allow you to move in that direction.
	
	if(X > 0)
	{
		// unless you are blocked from all directions (like you jumped into the players capsule)
		// then don't stear into the opponent's collision (limit all left/right movement)
		true;
	}
	else if(Y > 0)
	{
		// Test if the Head_M is overlapping 
		//	 if yes, then back up
		// else
		//   move forward

		auto HeadLocation = ThisCharacter->GetMesh()->GetBoneLocation(FName("Head_Socket"));
		auto CollidingObject = Cast<UCapsuleComponent>(ARock::GetActorComponent(CollidingActor, "TriggerCapsule"));
		bool CollisionAhead = CollidingObject->OverlapComponent(HeadLocation, Empty.Quat, Empty.Shape);

		if (CollisionAhead && Y < 0)
			ThisCharacter->AddMovementInput(DirectionalRef, Y); // Make movement
	}
}

