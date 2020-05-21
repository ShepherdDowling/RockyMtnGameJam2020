// Copyright by Shepherd Dowling under the Apache v2 licence

#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollisionHandler.generated.h"

class AActor;
class ACharacter;
class UPrimitiveComponent;

// This Class is used for handling collisions between multple characters
UCLASS()
class THEGAME_API UCollisionHandler : public UActorComponent
{
	GENERATED_BODY()
private:

protected:

public:
	struct Move
	{
		FVector Direction;
		float Value;
	};

	enum EDirectionals
	{
		ForwardBackward,
		LeftRight
	};
	ACharacter* CollidingActor = nullptr;
	ACharacter* ThisActor = nullptr;
	UPrimitiveComponent* TriggerCapsule = nullptr;


	UCollisionHandler();
	void SetThisActor(ACharacter* thisActor);
	void SetTriggerCapsule(UPrimitiveComponent* triggerCapsule);

	void SetCollidingActor(ACharacter* actor);
	ACharacter* GetCollidingActor() const;
	FVector GetCollisionLocation();

	void ModifyDirectional(const FVector& DirectionalRef, float X, float Y);
};
