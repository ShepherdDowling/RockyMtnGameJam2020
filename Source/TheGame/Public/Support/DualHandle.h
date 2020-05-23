// Copyright by Shepherd Dowling under the Apache v2 licence

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DualHandle.generated.h"

class ACharacter;
class UPhysicsHandleComponent;
class UPrimitiveComponent;

UCLASS()
class THEGAME_API UDualHandle : public UActorComponent
{
	GENERATED_BODY()
	
private:	
	ACharacter* ThisCharacter = nullptr;
	UPrimitiveComponent* TargetComponent = nullptr;
	FName BoneName;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UPhysicsHandleComponent* One = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UPhysicsHandleComponent* Two = nullptr;


	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float Friction = 9999999;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float GrabOffset = 50;

public:
	UDualHandle();
	// Place this in "AActor::BeginPlay()"
	void Init(ACharacter* iThisCharacter, UPrimitiveComponent* iTargetComponent, FName&& iBoneName);
	// Place this in "AActor::Tick()"
	void UpdateGrip();
};
