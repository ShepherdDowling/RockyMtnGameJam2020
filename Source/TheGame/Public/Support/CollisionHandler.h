// Copyright by Shepherd Dowling under the Apache v2 licence

#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollisionHandler.generated.h"

class AActor;
class ACharacter;
class UPrimitiveComponent;

// This Class is used for handling collisions between multple characters
// and Static (Non-Movable) Meshes
UCLASS()
class THEGAME_API UCollisionHandler : public UActorComponent
{
	GENERATED_BODY()
private:
	struct FLocation
	{
		struct Directional
		{
			FVector Socket;
			FName Name;
		};
		Directional Front;
		Directional Back;
		Directional Left;
		Directional Right;
	};

	FLocation Location;

	ACharacter* ThisCharacter = nullptr;
	AActor* CollidingActor = nullptr;
	UPrimitiveComponent* TriggerComponent;

	struct FEmptyObject
	{
		const FQuat Quat;
		const FCollisionShape Shape;
		FEmptyObject() : Quat(FQuat()), Shape(FCollisionShape::MakeSphere(1)) {}
	};

	FEmptyObject Empty;
protected:

public:
	struct FMove
	{
		FVector Direction;
		float Value;
	};

	enum EDirectionals
	{
		ForwardBackward,
		LeftRight
	};
	UCollisionHandler();
	virtual void BeginPlay() override;
	void Init(ACharacter* thisCharacter, FString&& TriggerCapsuleName);

	UPrimitiveComponent* GetTriggerComponent() const;
	void SetCollidingActor(AActor* actor);
	AActor* GetCollidingActor() const;
	FVector GetCollisionLocation();

	void ModifyDirectional(const FVector& DirectionalRef, float X, float Y);
};
